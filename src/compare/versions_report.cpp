// compare/versions_report.cpp -- what a comparison says: its counts, report and document; the work carried across builds; a
// whole product; the command (see versions.hpp).
// cb-unity: alone

#include <charconv>
#include <chrono>
#include <cmath>
#include <format>

#include <nlohmann/json.hpp>

#include "base/corpus.hpp"
#include "compare/versions.hpp"
#include "compare/versions_internal.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "jobs/pool.hpp"

namespace cb::compare::versions {

using detail::Interner;
using detail::SeqMatcher;

namespace {

double round_to(double x, int digits) {
    const std::string s = std::format("{:.{}f}", x, digits);
    double v = 0;
    std::from_chars(s.data(), s.data() + s.size(), v);
    return v;
}

}  // namespace

// ---- Comparison ---------------------------------------------------------------------------------------------------------------
Comparison::Comparison(std::shared_ptr<const Side> a, std::shared_ptr<const Side> b, const Pairing& pairing, double seconds)
    : a_(std::move(a)), b_(std::move(b)), seconds_(seconds) {
    std::vector<const Pairing::Link*> by_from(a_->count(), nullptr);
    std::vector<bool> matched(b_->count(), false);
    for (const Pairing::Link& l : pairing.links) {
        by_from[l.from] = &l;
        matched[l.to] = true;
    }
    for (std::size_t x = 0; x < a_->count(); ++x) {
        const Pairing::Link* l = by_from[x];
        if (!l) {
            removed_.push_back(x);
            continue;
        }
        const Function& fa = a_->function(x);
        const Function& fb = b_->function(l->to);
        Pair p;
        p.from = fa.key;
        p.to = fb.key;
        p.from_name = fa.name;
        p.to_name = fb.name;
        for (Level lv : kLevels)
            if (fa.ids.at(lv) == fb.ids.at(lv)) {
                p.level = lv;
                break;
            }
        p.identical = p.level == Level::Bytes || p.level == Level::Text;
        p.how = l->how;
        p.score = p.identical ? 1.0 : round_to(likeness(fa, fb), 3);
        p.size_from = fa.size;
        p.size_to = fb.size;
        pairs_.push_back(std::move(p));
        pair_from_.push_back(x);
        pair_to_.push_back(l->to);
    }
    for (std::size_t y = 0; y < b_->count(); ++y)
        if (!matched[y]) added_.push_back(y);
}

Counts Comparison::counts() const {
    Counts c;
    for (const Pair& p : pairs_) {
        if (p.identical) {
            ++c.identical;
            if (p.level == Level::Bytes) ++c.identical_bytes;
            if (p.level == Level::Text) ++c.identical_text;
        } else {
            ++c.changed;
        }
    }
    c.added = static_cast<std::int64_t>(added_.size());
    c.removed = static_cast<std::int64_t>(removed_.size());
    return c;
}

CodeBytes Comparison::code() const {
    CodeBytes out;
    out.to_bytes = b_->size();
    for (std::size_t y : added_) out.added += b_->function(y).size;
    for (const Pair& p : pairs_) (p.identical ? out.identical : out.changed) += p.size_to;
    return out;
}

std::vector<NameRow> Comparison::names() const {
    std::vector<NameRow> out;
    for (std::size_t i = 0; i < pairs_.size(); ++i)
        if (a_->function(pair_from_[i]).named && !b_->function(pair_to_[i]).named) out.push_back({pairs_[i].to, pairs_[i].from_name, pairs_[i].from, pairs_[i].identical});
    return out;
}

std::vector<Reconstruction> Comparison::reconstructions(Ledger& ledger) const {
    const std::optional<Ledger::Identity> id = ledger.identity(a_->path());
    if (!id) return {};
    const std::map<std::string, Ledger::Known> best = ledger.best(*id);
    std::vector<Reconstruction> out;
    for (const Pair& p : pairs_) {
        const auto row = best.find(p.from);
        if (row == best.end() || !p.identical) continue;
        if ((row->second.klass == "verified" || row->second.klass == "weak") && !row->second.source.empty())
            out.push_back({p.from, p.to, row->second.klass, row->second.engine, row->second.name, row->second.source});
    }
    return out;
}

std::optional<std::size_t> Comparison::carry_names(Ledger& ledger) const {
    const std::map<std::string, std::string> bodies = ledger.bodies(ledger.sha(b_->path()).value_or(""));
    if (bodies.empty()) return std::nullopt;
    std::size_t n = 0;
    for (const NameRow& row : names()) {
        const auto body = bodies.find(row.to);
        if (row.identical && body != bodies.end()) {
            const std::optional<std::string> owner = ledger.owner(a_->path());
            ledger.name_body(body->second, row.name,
                             std::format("the same function in {} of {}", a_->name(), owner && !owner->empty() ? *owner : "another build"));
            ++n;
        }
    }
    return n;
}

std::size_t Comparison::carry(Ledger& ledger) const {
    const std::vector<Reconstruction> found = reconstructions(ledger);
    if (found.empty()) return 0;
    const std::optional<Ledger::Identity> id = ledger.identity(b_->path());
    if (!id) return 0;
    std::vector<std::string> addresses;
    for (const Function& f : b_->functions()) addresses.push_back(f.key);
    const std::string old = ledger.sha(a_->path()).value_or("");
    std::vector<Ledger::Carried> rows;
    for (const Reconstruction& r : found) {
        const Function& fb = *b_->find(r.to);
        rows.push_back({r.to, fb.named ? std::optional<std::string>(fb.name) : r.name, r.source,
                        std::format("carried: identical to {} {} ({} there); not judged here", a_->name(), r.from, r.klass)});
    }
    ledger.file_carried(*id, b_->name(), b_->path(), b_->kind(), addresses, old.substr(0, 12), rows);
    return found.size();
}

std::string Comparison::report() const {
    const Counts c = counts();
    const CodeBytes code_ = code();
    auto pc = [&](std::int64_t n) { return code_.to_bytes ? 100.0 * static_cast<double>(n) / static_cast<double>(code_.to_bytes) : 0.0; };
    return std::format(
        "{}: {} functions -> {}: {}\n  identical {} (same bytes {}, same instructions {})   changed {}   added {}   removed {}\n  of the new build's {} bytes "
        "of code: {:.1f}% identical, {:.1f}% changed, {:.1f}% added   ({:.1f}s)",
        a_->name(), a_->count(), b_->name(), b_->count(), c.identical, c.identical_bytes, c.identical_text, c.changed, c.added, c.removed, code_.to_bytes,
        pc(code_.identical), pc(code_.changed), pc(code_.added), seconds_);
}

std::string Comparison::json(bool listings, Ledger* ledger) const {
    using J = nlohmann::ordered_json;
    auto about = [&](const Side& side) {
        J v = J::object();
        v["path"] = side.path().string();
        v["name"] = side.name();
        const std::optional<std::string> owner = ledger ? ledger->owner(side.path()) : std::nullopt;
        const std::optional<std::string> sha = ledger ? ledger->sha(side.path()) : std::nullopt;
        v["corpus"] = owner ? J(*owner) : J(nullptr);
        v["sha"] = sha ? J(*sha) : J(nullptr);
        v["kind"] = std::string(kind_name(side.kind()));
        v["functions"] = side.count();
        v["bytes"] = side.size();
        v["code"] = side.code();
        return v;
    };
    auto lines = [&](const Side& side, const std::vector<std::size_t>& keys) {
        J d = J::object();
        for (std::size_t k : keys) {
            const Function& f = side.function(k);
            J l = J::array();
            for (std::size_t i = 0; i < f.rows.size() && i < f.text.size(); ++i) l.push_back(J::array({std::format("{:x}", f.rows[i].address), f.rows[i].text, f.text[i]}));
            d[f.key] = std::move(l);
        }
        return d;
    };
    J out = J::object();
    out["from"] = about(*a_);
    out["to"] = about(*b_);
    const Counts c = counts();
    out["counts"] = J{{"identical", c.identical}, {"identical_bytes", c.identical_bytes}, {"identical_text", c.identical_text},
                      {"changed", c.changed}, {"added", c.added}, {"removed", c.removed}};
    const CodeBytes cb = code();
    out["code"] = J{{"to_bytes", cb.to_bytes}, {"identical", cb.identical}, {"changed", cb.changed}, {"added", cb.added}};
    out["seconds"] = round_to(seconds_, 2);
    J pv = J::array();
    for (std::size_t i = 0; i < pairs_.size(); ++i) {
        const Pair& p = pairs_[i];
        J d = J::object();
        d["from"] = p.from;
        d["to"] = p.to;
        d["from_name"] = p.from_name;
        d["to_name"] = p.to_name;
        d["status"] = p.identical ? "identical" : "changed";
        d["level"] = p.level ? std::string(level_name(*p.level)) : std::string();
        d["how"] = std::string(how_name(p.how));
        d["score"] = p.score;
        d["sizes"] = J::array({p.size_from, p.size_to});
        if (!p.identical && listings) {
            Interner in;
            const std::vector<int> ta = in.all(a_->function(pair_from_[i]).text), tb = in.all(b_->function(pair_to_[i]).text);
            J ops = J::array();
            for (const auto& op : SeqMatcher(ta, tb).opcodes()) {
                const char* tag = op.tag == SeqMatcher::Tag::Equal ? "equal" : op.tag == SeqMatcher::Tag::Replace ? "replace" : op.tag == SeqMatcher::Tag::Delete ? "delete" : "insert";
                ops.push_back(J::array({tag, op.i1, op.i2, op.j1, op.j2}));
            }
            d["ops"] = std::move(ops);
        }
        pv.push_back(std::move(d));
    }
    out["pairs"] = std::move(pv);
    auto listed = [&](const Side& side, const std::vector<std::size_t>& keys) {
        J l = J::array();
        for (std::size_t k : keys) l.push_back(J{{"key", side.function(k).key}, {"name", side.function(k).name}, {"size", side.function(k).size}});
        return l;
    };
    out["added"] = listed(*b_, added_);
    out["removed"] = listed(*a_, removed_);
    J nv = J::array();
    for (const NameRow& n : names()) nv.push_back(J{{"to", n.to}, {"name", n.name}, {"from", n.from}, {"identical", n.identical}});
    out["names"] = std::move(nv);
    J rv = J::array();
    if (ledger)
        for (const Reconstruction& r : reconstructions(*ledger))
            rv.push_back(J{{"from", r.from}, {"to", r.to}, {"class", r.klass}, {"engine", r.engine ? J(*r.engine) : J(nullptr)},
                           {"name", r.name ? J(*r.name) : J(nullptr)}, {"source", r.source}});
    out["reconstructions"] = std::move(rv);
    if (listings) {
        std::vector<std::size_t> from_keys, to_keys;
        for (std::size_t i = 0; i < pairs_.size(); ++i)
            if (!pairs_[i].identical) {
                from_keys.push_back(pair_from_[i]);
                to_keys.push_back(pair_to_[i]);
            }
        from_keys.insert(from_keys.end(), removed_.begin(), removed_.end());
        to_keys.insert(to_keys.end(), added_.begin(), added_.end());
        out["listings"] = J{{"from", lines(*a_, from_keys)}, {"to", lines(*b_, to_keys)}};
    }
    return out.dump(2);
}

Comparison compare(const std::filesystem::path& a, const std::filesystem::path& b, double least) {
    const auto t0 = std::chrono::steady_clock::now();
    auto sa = std::make_shared<const Side>(load(a));
    auto sb = std::make_shared<const Side>(load(b));
    const Pairing pairing = match(*sa, *sb, least);
    return Comparison(std::move(sa), std::move(sb), pairing, std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count());
}

// ---- a whole product ----------------------------------------------------------------------------------------------------------
std::string_view status_name(ModuleStatus status) {
    switch (status) {
        case ModuleStatus::Compared: return "compared";
        case ModuleStatus::SameFile: return "same file";
        case ModuleStatus::NewModule: return "new module";
        case ModuleStatus::Failed: return "failed";
    }
    return "";
}

std::int64_t Product::total(std::string_view key) const {
    for (const auto& [k, v] : totals)
        if (k == key) return v;
    return 0;
}

namespace {

struct Item {
    std::string name;
    std::filesystem::path from, to;
};

std::string failure_text(const std::exception& e) {
    const auto* known = dynamic_cast<const Error*>(&e);
    return std::format("{}: {}", known ? known->kind() : std::string_view("Error"), std::string(e.what()).substr(0, 120));
}

ModuleRow one_module(base::Corpus& corpus, double least, const Item& item) {
    ModuleRow row;
    row.name = item.name;
    try {
        if (corpus.sha(item.from) == corpus.sha(item.to)) {
            const Side side = load(item.to);
            row.status = ModuleStatus::SameFile;
            row.functions = static_cast<std::int64_t>(side.count());
            row.bytes = side.size();
            row.identical = static_cast<std::int64_t>(side.count());
            row.identical_code = side.size();
            row.changed = 0;
            row.changed_code = 0;
            row.added = 0;
            row.removed = 0;
            row.seconds = 0.0;
            return row;
        }
        const Comparison c = compare(item.from, item.to, least);
        const Counts n = c.counts();
        const CodeBytes code = c.code();
        row.functions = static_cast<std::int64_t>(c.to().count());
        row.bytes = code.to_bytes;
        row.identical = n.identical;
        row.identical_code = code.identical;
        row.changed = n.changed;
        row.changed_code = code.changed;
        row.added = n.added;
        row.removed = n.removed;
        row.seconds = round_to(c.seconds(), 1);
    } catch (const std::exception& e) {   // isolation boundary: one module must not end the run
        row.status = ModuleStatus::Failed;
        row.error = failure_text(e);
    }
    return row;
}

}  // namespace

Product product(base::Corpus& corpus, const std::string& from, const std::string& to, std::size_t sample, std::optional<int> jobs, double least,
                const std::function<void(const std::string&)>& log) {
    auto say = [&](const std::string& s) {
        if (log) log(s);
        else cb::out("{}\n", s);
    };
    std::vector<std::filesystem::path> paths;
    {
        const base::Corpus::Selected scope(corpus, to);
        for (base::Lane lane : {base::Lane::Pe, base::Lane::Ne}) {
            const std::vector<std::filesystem::path> some = corpus.modules(lane);
            paths.insert(paths.end(), some.begin(), some.end());
        }
        std::stable_sort(paths.begin(), paths.end(), [](const auto& a, const auto& b) { return text::lower(a.filename().string()) < text::lower(b.filename().string()); });
        if (sample && sample < paths.size()) {
            std::vector<std::filesystem::path> pick;
            for (std::size_t i = 0; i < sample; ++i) pick.push_back(paths[i * paths.size() / sample]);
            paths = std::move(pick);
        }
    }
    std::vector<Item> items;
    std::vector<ModuleRow> rows;
    std::vector<std::pair<ModuleRow, std::filesystem::path>> fresh;   // modules the old product has no counterpart of
    {
        const base::Corpus::Selected scope(corpus, from);
        for (const std::filesystem::path& p : paths) {
            const std::string name = text::lower(p.filename().string());
            if (const auto before = corpus.retail(name)) {
                const auto lane = corpus.lane(*before);
                if (lane && (*lane == base::Lane::Pe || *lane == base::Lane::Ne)) {
                    items.push_back({name, *before, p});
                    continue;
                }
            }
            ModuleRow r;
            r.name = name;
            r.status = ModuleStatus::NewModule;
            fresh.emplace_back(std::move(r), p);
        }
    }
    jobs::RunOptions<Item, ModuleRow> opts;
    opts.jobs = std::min(jobs && *jobs ? *jobs : jobs::default_jobs(), 6);
    opts.small = 4;
    opts.crashed = [](const Item& item) {
        ModuleRow r;
        r.name = item.name;
        r.status = ModuleStatus::Failed;
        r.error = "the worker died";
        return r;
    };
    const auto done = jobs::run(
        items, [&](const Item& item) { return one_module(corpus, least, item); }, [](const Item& item) { return item.name; }, opts);
    for (auto& [row, path] : fresh) {   // a new module is all added code
        try {
            const Side side = load(path);
            row.functions = static_cast<std::int64_t>(side.count());
            row.bytes = side.size();
            row.added = static_cast<std::int64_t>(side.count());
        } catch (const std::exception& e) {   // isolation boundary: one module
            row.status = ModuleStatus::Failed;
            row.error = failure_text(e);
        }
        rows.push_back(std::move(row));
    }
    for (const auto& [name, row] : done) rows.push_back(row);
    std::stable_sort(rows.begin(), rows.end(), [](const ModuleRow& a, const ModuleRow& b) { return a.name < b.name; });
    Product out;
    out.from = from;
    out.to = to;
    auto bump = [&](const std::string& k, std::int64_t n) {
        for (auto& e : out.totals)
            if (e.first == k) {
                e.second += n;
                return;
            }
        out.totals.emplace_back(k, n);
    };
    for (const ModuleRow& r : rows) {
        bump("modules", 1);
        bump(std::string(status_name(r.status)), 1);
        bump("functions", r.functions.value_or(0));
        bump("bytes", r.bytes.value_or(0));
        bump("identical", r.identical.value_or(0));
        bump("identical_code", r.identical_code.value_or(0));
        bump("changed", r.changed.value_or(0));
        bump("changed_code", r.changed_code.value_or(0));
        bump("added", r.added.value_or(0));
        bump("removed", r.removed.value_or(0));
    }
    for (const ModuleRow& r : rows) {
        if (r.status == ModuleStatus::Failed)
            say(std::format("  {:<14} FAILED {}", r.name, r.error.value_or("")));
        else
            say(std::format("  {:<14} {:<10} {:>5} functions: identical {:>5}  changed {:>5}  added {:>5}  removed {:>5}", r.name, status_name(r.status),
                            r.functions.value_or(0), r.identical.value_or(0), r.changed.value_or(0), r.added.value_or(0), r.removed.value_or(0)));
        out.modules.push_back(r);
    }
    return out;
}

std::string product_json(const Product& p, std::optional<double> seconds) {
    using J = nlohmann::ordered_json;
    J v = J::object();
    v["from"] = p.from;
    v["to"] = p.to;
    J mods = J::array();
    for (const ModuleRow& r : p.modules) {
        J m = J::object();
        m["name"] = r.name;
        m["status"] = std::string(status_name(r.status));
        if (r.error) m["error"] = *r.error;
        auto put = [&](const char* k, const std::optional<std::int64_t>& x) {
            if (x) m[k] = *x;
        };
        put("functions", r.functions);
        put("bytes", r.bytes);
        put("identical", r.identical);
        put("identical_code", r.identical_code);
        put("changed", r.changed);
        put("changed_code", r.changed_code);
        put("added", r.added);
        put("removed", r.removed);
        if (r.seconds) m["seconds"] = *r.seconds;
        mods.push_back(std::move(m));
    }
    v["modules"] = std::move(mods);
    J t = J::object();
    for (const auto& [k, n] : p.totals) t[k] = n;
    if (seconds) t["seconds"] = *seconds;
    v["totals"] = std::move(t);
    return v.dump(2);
}

// ---- the command ---------------------------------------------------------------------------------------------------------------
const char* const kUsage =
    "codebreaker versions <module> --from CORPUS --to CORPUS [--json FILE] [--carry]\n"
    "codebreaker versions <fileA> <fileB> [--json FILE] [--carry]\n"
    "codebreaker versions --all --from CORPUS --to CORPUS [--sample N] [--jobs N] [--json FILE]\n"
    "codebreaker versions --shared <program> <program> ... [--level text|shape|bytes] [--json FILE]\n"
    "\n"
    "Match the functions of two builds of a module and say which are identical, changed, added\n"
    "and removed; with --all, do it for every module of a product and total it; with --shared,\n"
    "list the functions several different programs have in common.";

namespace {

void save(const std::optional<std::string>& to, const std::string& text) {
    if (!to) return;
    fs::write_text(*to, text);
    cb::out("wrote {}\n", *to);
}

int run_shared(const cli::Args& args, base::Corpus& corpus, const std::vector<std::string>& names) {
    if (names.size() < 2) throw UsageError("--shared needs two programs or more");
    const std::string level_text = args.value_or("level", "text");
    const std::optional<Level> level = level_from_name(level_text);
    if (!level) throw UsageError("--level is one of bytes, text, shape");
    std::vector<std::filesystem::path> paths;
    for (const std::string& n : names) {
        const auto r = corpus.retail(n);
        paths.push_back(r ? *r : std::filesystem::path(n));
    }
    const std::vector<Shared> found = shared(paths, *level);
    std::vector<std::string> stems;
    for (const auto& p : paths) stems.push_back(text::lower(p.filename().string()));
    cb::out("{}: {} functions in two programs or more ({})\n", text::join(stems, ", "), found.size(), level_text);
    for (std::size_t i = 0; i < stems.size(); ++i) {
        for (std::size_t j = i + 1; j < stems.size(); ++j) {
            std::size_t both = 0;
            for (const Shared& s : found) {
                const auto has = [&](const std::string& n) { return std::any_of(s.where.begin(), s.where.end(), [&](const auto& w) { return w.first == n; }); };
                if (has(stems[i]) && has(stems[j])) ++both;
            }
            cb::out("  {} and {}: {}\n", stems[i], stems[j], both);
        }
    }
    if (!found.empty()) cb::out("largest (instructions: program@keys):\n");
    for (std::size_t k = 0; k < found.size() && k < 12; ++k) {
        auto where = found[k].where;
        std::stable_sort(where.begin(), where.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        std::vector<std::string> parts;
        for (const auto& w : where) parts.push_back(w.first + "@" + text::join(w.second, ","));
        cb::out("  {:>4}  {}\n", found[k].instructions, text::join(parts, "  "));
    }
    if (const auto out = args.value("json")) {
        using J = nlohmann::ordered_json;
        J d = J::object();
        d["level"] = level_text;
        d["programs"] = stems;
        J sh = J::array();
        for (const Shared& s : found) {
            J where = J::object();
            for (const auto& w : s.where) where[w.first] = w.second;
            sh.push_back(J{{"instructions", s.instructions}, {"where", std::move(where)}});
        }
        d["shared"] = std::move(sh);
        save(out, d.dump(2));
    }
    return 0;
}

}  // namespace

int run(const cli::Args& args, base::Corpus& corpus, Ledger& ledger) {
    const std::optional<std::string> from = args.value("from"), to = args.value("to"), out = args.value("json");
    double least = 0.6;
    if (const auto v = args.value("least")) {
        const auto [end, ec] = std::from_chars(v->data(), v->data() + v->size(), least);
        if (ec != std::errc() || end != v->data() + v->size()) throw UsageError("--least expects a number, got '" + *v + "'");
    }
    const std::vector<std::string> names = args.values("files");
    if (args.flag("shared")) return run_shared(args, corpus, names);
    if (args.flag("all") && from && to) {
        const auto t0 = std::chrono::steady_clock::now();
        const std::optional<int> jobs = args.integer<int>("jobs");
        const Product result = product(corpus, *from, *to, args.integer<std::size_t>("sample").value_or(0), jobs && *jobs ? jobs : std::nullopt, least);
        const double seconds = round_to(std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count(), 1);
        const std::int64_t bytes = result.total("bytes") ? result.total("bytes") : 1;
        cb::out("{} -> {}: {} modules ({} compared, {} the same file, {} new, {} failed)\n  {} functions: identical {}, changed {}, added {}; removed from the old "
                "ones {}\n  of {} bytes of code: {:.1f}% identical to {}, {:.1f}% changed   ({:.0f}s)\n",
                *from, *to, result.total("modules"), result.total("compared"), result.total("same file"), result.total("new module"), result.total("failed"),
                result.total("functions"), result.total("identical"), result.total("changed"), result.total("added"), result.total("removed"), result.total("bytes"),
                100.0 * static_cast<double>(result.total("identical_code")) / static_cast<double>(bytes), *from,
                100.0 * static_cast<double>(result.total("changed_code")) / static_cast<double>(bytes), seconds);
        save(out, product_json(result, seconds));
        return result.total("failed") ? 1 : 0;
    }
    std::filesystem::path path_a, path_b;
    std::error_code ec;
    if (names.size() == 2 && std::filesystem::is_regular_file(names[0], ec) && std::filesystem::is_regular_file(names[1], ec)) {
        path_a = names[0];
        path_b = names[1];
    } else if (names.size() == 1 && from && to) {
        std::optional<std::filesystem::path> pa, pb;
        {
            const base::Corpus::Selected scope(corpus, *from);
            pa = corpus.retail(names[0]);
        }
        {
            const base::Corpus::Selected scope(corpus, *to);
            pb = corpus.retail(names[0]);
        }
        if (!pa || !pb) throw UsageError(std::format("no {} in {}", names[0], !pa ? *from : *to));
        path_a = *pa;
        path_b = *pb;
    } else {
        cb::out("{}\n", kUsage);
        return 2;
    }
    const Comparison c = compare(path_a, path_b, least);
    cb::out("{}\n", c.report());
    const std::vector<NameRow> carried = c.names();
    const std::vector<Reconstruction> known = c.reconstructions(ledger);
    const std::size_t onto = static_cast<std::size_t>(std::count_if(carried.begin(), carried.end(), [](const NameRow& n) { return n.identical; }));
    cb::out("  names carried to the new build: {} ({} onto identical bodies)   verified reconstructions that carry: {}\n", carried.size(), onto, known.size());
    if (args.flag("carry")) {
        cb::out("  filed {} reconstructions for {} in the store, to be judged\n", c.carry(ledger), c.to().name());
        const auto named = c.carry_names(ledger);
        cb::out("  names given to its bodies in the store: {}\n", named ? std::to_string(*named) : "none -- its bodies are not recorded there yet");
    }
    if (out) save(out, c.json(true, &ledger));
    return 0;
}

cli::Command declaration() {
    cli::Command c;
    c.name = "versions";
    c.summary = "match the functions of two builds: identical, changed, added, removed";
    c.area = "Compare";
    c.description = kUsage;
    c.params = {cli::Positional{"files", "a module name (with --from and --to), two files, or the programs of --shared", false, true},
                cli::Option{"--from", "", "the product of the old build", cli::Arg::Value, "CORPUS"},
                cli::Option{"--to", "", "the product of the new build", cli::Arg::Value, "CORPUS"},
                cli::Option{"--json", "", "write the document here", cli::Arg::Value, "FILE"},
                cli::Option{"--least", "", "how alike two functions must be to be paired", cli::Arg::Value, "N", "0.6"},
                cli::Option{"--sample", "", "with --all: this many modules, evenly spread", cli::Arg::Value, "N"},
                cli::Option{"--jobs", "", "with --all: modules compared at once", cli::Arg::Value, "N"},
                cli::Option{"--level", "", "with --shared: bytes, text or shape", cli::Arg::Value, "LEVEL", "text"},
                cli::Option{"--all", "", "every module of the product", cli::Arg::Flag},
                cli::Option{"--shared", "", "the functions the programs share", cli::Arg::Flag},
                cli::Option{"--carry", "", "file the carried reconstructions and names in the store", cli::Arg::Flag}};
    return c;
}

cli::Command command(base::Corpus& corpus, Ledger& ledger) {
    cli::Command c = declaration();
    c.handler = [&corpus, &ledger](const cli::Args& args) { return run(args, corpus, ledger); };
    return c;
}

}  // namespace cb::compare::versions
