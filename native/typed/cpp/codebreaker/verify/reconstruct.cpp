// codebreaker/verify/reconstruct.cpp -- C++17 port of codebreaker.verify.reconstruct (see reconstruct.hpp).
//
// reconstruct.py -- the path from a module to judged reconstructions of its functions.  Decompile the module, build the
// functions that have no verdict yet for the text they have now, run each beside the original on the same inputs, and
// record what was found.  Functions already judged for their current text are not built or run again.

#include "codebreaker/verify/reconstruct.hpp"

#include <algorithm>
#include <map>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/engine/lanes.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::reconstruct {

namespace V = verdicts;
using engine::native_engine::Cell;

const py::str SOURCE = py::str("decompiler");   // who a candidate in the store came from

const verdicts::NotRun& NOT_RUN() { return V::NOT_RUN(); }
const py::list<py::str>& GOOD() { return V::GOOD(); }

namespace {

// KINDS: the name results are filed under -> the lane that knows that kind (in registration order)
std::vector<std::pair<py::str, std::shared_ptr<Kind>>>& registry() {
    static std::vector<std::pair<py::str, std::shared_ptr<Kind>>> r;
    return r;
}

py::dict<py::str, std::optional<py::str>>& digest_cache() {
    static py::dict<py::str, std::optional<py::str>> d;
    return d;
}

// os.path.basename(os.path.normpath(path)).lower()
py::str module_name(const py::str& module_path) {
    return py::os::path::basename(py::os::path::normpath(module_path)).lower();
}

// `x or ""` of a store cell, as text
py::str cell_text(const Cell& c) {
    if (std::holds_alternative<py::str>(c)) return std::get<py::str>(c);
    if (std::holds_alternative<int64_t>(c)) return std::get<int64_t>(c) ? py::to_str(std::get<int64_t>(c)) : py::str("");
    if (std::holds_alternative<double>(c)) return std::get<double>(c) != 0.0 ? py::to_str(std::get<double>(c)) : py::str("");
    return py::str("");
}

bool cell_is(const Cell& c, const py::str& s) { return std::holds_alternative<py::str>(c) && std::get<py::str>(c) == s; }
Cell cell_of(const py::str& s) { return Cell(s); }
Cell cell_of(const std::optional<py::str>& s) { return s ? Cell(*s) : Cell(); }
int64_t cell_int(const Cell& c) { return std::holds_alternative<int64_t>(c) ? std::get<int64_t>(c) : 0; }

// one row reconstruct.run records (the Python's dict with the keys it has)
struct Row {
    py::str addr;
    py::str sig;
    Cell name, source;
    py::str verdict;
    Cell reason, limits, detail, problem;
};

engine::native_engine::FunctionStore::RecordRow record_row(const Row& r) {
    engine::native_engine::FunctionStore::RecordRow out;
    out.addr = r.addr;
    out.name = r.name;
    out.source = r.source;
    out.sig = Cell(r.sig);
    out.verdict = r.verdict;
    out.reason = r.reason;
    out.limits = r.limits;
    out.detail = r.detail;
    out.problem = r.problem;
    return out;
}

bool engine_enabled() {
    std::optional<py::str> v = py::os::environ_::get(py::str("CB_ENGINE"));
    return !v || *v != py::str("0");
}

}  // namespace

void register_kind(const py::str& name, const std::shared_ptr<Kind>& kind) {
    for (auto& e : registry()) {
        if (e.first == name) {
            e.second = kind;
            return;
        }
    }
    registry().emplace_back(name, kind);
    digest_cache() = py::dict<py::str, std::optional<py::str>>();
}

py::dict<py::str, py::str> kinds() {
    py::dict<py::str, py::str> out;
    for (const auto& e : registry()) out.set(e.first, e.second->KIND());
    return out;
}

std::shared_ptr<Kind> kind_named(const py::str& name) {
    for (const auto& e : registry()) {
        if (e.first == name) return e.second;
    }
    throw py::KeyError(name.utf8());
}

std::optional<py::str> kind_name(const py::str& module_path) {
    // platforms.identify(path): the first platform that recognises the file; its name when it has a lane
    for (const engine::lanes::LanePlatform& p : engine::lanes::platforms()) {
        if (p.identifies && p.identifies(module_path)) return p.name;
    }
    return std::nullopt;
}

std::shared_ptr<Kind> kind_of(const py::str& module_path) {
    std::optional<py::str> name = kind_name(module_path);
    if (!name) {
        std::optional<py::str> lane = base::corpus::lane(module_path);
        throw py::ValueError(py::percent("%s: no way to reconstruct a %s file yet", py::os::path::basename(module_path),
                                         lane && !lane->empty() ? *lane : py::str("non-executable")).utf8());
    }
    return kind_named(*name);
}

py::dict<py::str, std::optional<py::str>> digests() {
    py::dict<py::str, std::optional<py::str>>& cache = digest_cache();
    if (cache.empty()) {
        for (const auto& e : registry()) {
            try {
                cache.set(e.first, std::optional<py::str>(std::get<0>(e.second->profile(py::str("")))));
            } catch (const py::ImportError&) {
                throw;
            } catch (const std::exception&) {
                cache.set(e.first, std::optional<py::str>());
            }
        }
    }
    return cache;
}

std::tuple<py::str, py::str> identity(const py::str& module_path) {
    std::shared_ptr<Kind> kind = kind_of(module_path);
    return {module_sha(*kind, module_path), std::get<0>(kind->profile(module_path))};
}

py::str module_sha(const Kind& kind, const py::str& module_path) {
    std::optional<py::str> own = kind.sha(module_path);
    std::optional<py::str> got = own ? own : base::corpus::sha(module_path);
    if (got && !got->empty()) return *got;
    return py::str("");
}

py::set<py::str> _reaches(const py::dict<py::str, py::str>& texts, const py::list<py::str>& built,
                          const py::dict<py::str, py::str>& dropped) {
    if (dropped.empty()) return py::set<py::str>();
    py::list<py::str> escaped;
    for (const py::str& d : dropped) escaped.append(py::re::escape(d));
    py::re::Pattern pat = py::re::compile(py::str("\\b(") + py::join(py::str("|"), escaped) + py::str(")\\s*\\("));
    py::re::Pattern call = py::re::compile(py::str("\\b([A-Za-z_]\\w*)\\s*\\("));
    py::set<py::str> in_built = py::set<py::str>::from(built);
    py::dict<py::str, py::set<py::str>> calls;
    for (const py::str& n : built) {
        py::set<py::str> found;
        for (const py::str& c : call.findall(texts.at(n)).vec())
            if (in_built.contains(c)) found.add(c);
        calls.set(n, found);
    }
    py::set<py::str> tainted;
    for (const py::str& n : built) {
        const py::str& text = texts.at(n);
        // text.split("{", 1)[-1]: what follows the first "{" (the whole text when there is none)
        int64_t at = text.find(py::str("{"));
        py::str body = at < 0 ? text : text.slice_from(at + 1);
        if (pat.search(body)) tainted.add(n);
    }
    bool grew = true;
    while (grew) {
        grew = false;
        for (const py::str& n : built) {
            if (tainted.contains(n)) continue;
            bool reaches = false;
            for (const py::str& c : calls.at(n))
                if (tainted.contains(c)) {
                    reaches = true;
                    break;
                }
            if (reaches) {
                tainted.add(n);
                grew = true;
            }
        }
    }
    return tainted;
}

py::dict<py::str, py::str> signatures(const Prepared& m, const std::optional<py::str>& whole,
                                      const std::optional<py::dict<py::str, py::str>>& own) {
    const py::dict<py::str, py::str>& texts = m.texts;
    if (texts.empty() || m.header.empty()) {
        py::dict<py::str, py::str> out;
        for (const py::str& n : m.order) out.set(n, whole ? *whole : py::str(""));
        return out;
    }
    py::set<py::str> names;
    for (const py::str& t : texts) names.add(t);
    py::re::Pattern word = py::re::compile(py::str("[A-Za-z_]\\w*"));
    // calls[n]: the names of the module's functions n mentions, but n, sorted
    py::dict<py::str, py::list<py::str>> calls;
    for (const auto& kv : texts.items()) {
        const py::str& n = std::get<0>(kv);
        py::set<py::str> found;
        for (const py::str& w : word.findall(std::get<1>(kv)).vec())
            if (names.contains(w) && !(w == n)) found.add(w);
        std::vector<py::str> v(found.begin(), found.end());
        std::sort(v.begin(), v.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
        calls.set(n, py::list<py::str>(v));
    }
    // strongly connected groups (Tarjan, without recursion: call chains are long), callees first
    py::dict<py::str, int64_t> index, low, group;
    py::set<py::str> on;
    py::list<py::str> stack;
    std::vector<std::vector<py::str>> order;
    int64_t count = 0;
    for (const py::str& root : texts) {
        if (index.contains(root)) continue;
        std::vector<std::pair<py::str, int64_t>> work;
        work.emplace_back(root, 0);
        while (!work.empty()) {
            py::str n = work.back().first;
            int64_t i = work.back().second;
            work.pop_back();
            if (i == 0) {
                index.set(n, count);
                low.set(n, count);
                count += 1;
                stack.append(n);
                on.add(n);
            }
            const py::list<py::str>& cs = calls.at(n);
            bool broke = false;
            for (int64_t j = i; j < cs.size(); ++j) {
                const py::str& c = cs[j];
                if (!index.contains(c)) {
                    work.emplace_back(n, j + 1);
                    work.emplace_back(c, 0);
                    broke = true;
                    break;
                }
                if (on.contains(c)) low.set(n, std::min(low.at(n), index.at(c)));
            }
            if (!broke) {
                if (low.at(n) == index.at(n)) {
                    std::vector<py::str> members;
                    while (true) {
                        py::str x = stack.pop();
                        on.discard(x);
                        members.push_back(x);
                        if (x == n) break;
                    }
                    order.push_back(members);
                    for (const py::str& x : members) group.set(x, static_cast<int64_t>(order.size()) - 1);
                }
                if (!work.empty()) {
                    const py::str& parent = work.back().first;
                    low.set(parent, std::min(low.at(parent), low.at(n)));
                }
            }
        }
    }
    py::str base = own ? py::str("built")
                       : py::hashlib::sha1(m.header.encode("utf-8", "replace")).hexdigest();
    std::vector<py::str> sig_of;
    py::dict<py::str, py::str> out;
    for (size_t g = 0; g < order.size(); ++g) {   // callees come before their callers
        const std::vector<py::str>& popped = order[g];   // (the order the walk gave them: the answer's keys come in it)
        std::vector<py::str> members = popped;
        py::hashlib::Hash h = py::hashlib::sha1(base.encode("utf-8"));
        std::sort(members.begin(), members.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
        for (const py::str& x : members) {
            py::str body = own ? own->get(x, py::str("absent")) : texts.at(x);
            h.update(x.encode("utf-8") + py::bytes(std::string("\0", 1)) + body.encode("utf-8", "replace") +
                     py::bytes(std::string("\0", 1)));
        }
        std::set<int64_t> others;
        for (const py::str& x : members)
            for (const py::str& c : calls.at(x)) others.insert(group.at(c));
        others.erase(static_cast<int64_t>(g));
        for (int64_t other : others) h.update(sig_of[static_cast<size_t>(other)].encode("utf-8"));
        sig_of.push_back(h.hexdigest());
        for (const py::str& x : popped) out.set(x, sig_of.back());
    }
    return out;
}

namespace {

// run(), its decompile, build and judge made by the engine (engine/lanes): only what changed is made again, and the
// store is given the rows that differ from what it holds.
std::tuple<py::str, py::str> run_on_engine(const py::str& module_path, const std::shared_ptr<Kind>& kind,
                                           const std::shared_ptr<engine::native_engine::FunctionStore>& fs,
                                           const std::function<void(const py::str&)>& log) {
    py::str sha = module_sha(*kind, module_path);
    py::str name = module_name(module_path);
    double t0 = py::time::time();
    engine::lanes::Judged j = engine::lanes::judged(module_path);
    fs->add_module(sha, name, module_path, kind->KIND());
    fs->add_profile(j.profile, j.settings);
    fs->add_functions(sha, j.all_keys);
    fs->put_text(sha, SOURCE, py::str("header"), j.header);
    if (kind->LIBRARY()) {
        py::dict<py::str, std::tuple<py::str, py::str>> found;
        for (const auto& kv : symbols::runtime_library::identify(module_path).items())
            found.set(symbols::names::key(module_path, std::get<0>(kv)), std::get<1>(kv));
        fs->set_identified(sha, found);
    }
    py::dict<py::str, engine::native_engine::Candidate> known = fs->judged(sha, j.profile, SOURCE);
    py::list<engine::native_engine::FunctionStore::RecordRow> rows;
    int64_t fresh_rows = 0;
    for (const engine::lanes::JudgedRow& r : j.rows) {
        bool differs = !known.contains(r.addr);
        if (!differs) {
            const engine::native_engine::Candidate& k = known.at(r.addr);
            differs = !(cell_text(k.sig) == r.sig && cell_text(k.name) == r.name &&
                        cell_text(k.source) == (r.source ? *r.source : py::str("")) && cell_text(k.verdict) == r.verdict &&
                        cell_text(k.reason) == r.reason && cell_text(k.limits) == r.limits &&
                        cell_text(k.detail) == r.detail);
        }
        if (!differs) continue;
        Row row;
        row.addr = r.addr;
        row.sig = r.sig;
        row.name = cell_of(r.name);
        row.source = cell_of(r.source);
        row.verdict = r.verdict;
        row.reason = cell_of(r.reason);
        row.limits = cell_of(r.limits);
        row.detail = cell_of(r.detail);
        row.problem = cell_of(r.problem);
        rows.append(record_row(row));
        fresh_rows += 1;
    }
    Cell rev = Cell(kind->SOURCE_REV() ? *kind->SOURCE_REV() : py::to_str(decompile::decompiler::REV));
    fs->record_many(sha, SOURCE, rev, j.profile, rows);
    if (!j.built.empty()) fs->set_built(sha, j.profile, j.built);
    py::list<int64_t> kept_made = j.made.get(py::percent("lane.%s.verdict", kind->KIND()), py::list<int64_t>{0, 0});
    py::dict<py::str, Cell> c = fs->scores(j.profile, sha);
    py::list<py::str> parts;
    for (const py::str& k : store::functions::SCORED())
        if (cell_int(c.at(k))) parts.append(py::percent("%s %d", store::functions::TITLE().at(k), cell_int(c.at(k))));
    log(py::percent("%s: %d functions on the engine, %d judged now, %d kept, %d rows new to the store: %s  (%.0fs)", name,
                    j.all_keys.size(), kept_made[1], kept_made[0], fresh_rows, py::join(py::str(", "), parts),
                    py::time::time() - t0));
    engine::facts::hold();
    return {sha, j.profile};
}

}  // namespace

std::tuple<py::str, py::str> run(const py::str& module_path, const std::shared_ptr<engine::native_engine::FunctionStore>& fs_in,
                                 const std::function<void(const py::str&)>& log_in) {
    std::function<void(const py::str&)> log = log_in ? log_in : [](const py::str& s) { py::print(s); };
    std::shared_ptr<Kind> kind = kind_of(module_path);
    std::shared_ptr<engine::native_engine::FunctionStore> fs = fs_in;
    if (!fs) fs = std::make_shared<engine::native_engine::FunctionStore>(store::functions::default_path());
    if (engine_enabled() && engine::lanes::on(module_path)) return run_on_engine(module_path, kind, fs, log);
    py::str sha = module_sha(*kind, module_path);
    auto [digest, settings] = kind->profile(module_path);
    py::str name = module_name(module_path);
    double t0 = py::time::time();
    py::str work = build::watcom::_scratch(py::list<py::str>{py::str("reconstruct"), kind->KIND(), name});
    Prepared m = kind->prepare(module_path, work);
    double t1 = py::time::time();
    py::dict<py::str, py::str> sig;
    if (kind->PER_FUNCTION()) {
        sig = signatures(m, m.sig);
    } else {
        for (const py::str& n : m.order) sig.set(n, m.sig);
    }
    fs->add_module(sha, name, module_path, kind->KIND());
    fs->add_profile(digest, settings);
    fs->add_functions(sha, m.all_keys);
    fs->put_text(sha, SOURCE, py::str("header"), m.header);
    if (kind->LIBRARY()) {
        py::dict<py::str, std::tuple<py::str, py::str>> found;
        for (const auto& kv : symbols::runtime_library::identify(module_path).items())
            found.set(symbols::names::key(module_path, std::get<0>(kv)), std::get<1>(kv));
        fs->set_identified(sha, found);
    }
    py::dict<py::str, engine::native_engine::Candidate> known = fs->judged(sha, digest, SOURCE);
    std::vector<py::str> fresh;
    for (const py::str& n : m.order) {
        if (!m.key.contains(n)) continue;
        auto k = known.get(m.key.at(n));
        if (!(k && cell_is(k->sig, sig.at(n)))) fresh.push_back(n);
    }
    py::set<py::str> have;
    for (const py::str& v : m.key.values()) have.add(v);
    std::vector<Row> rows;
    for (const py::str& k : m.all_keys) {
        if (have.contains(k)) continue;
        Row r;
        r.addr = k;
        r.sig = py::str("-");
        r.name = cell_of(py::str(""));
        r.verdict = V::NO_SOURCE;
        r.reason = cell_of(py::str("the decompiler produced nothing for this function"));
        r.problem = cell_of(py::str("none"));
        rows.push_back(r);
    }
    double t2 = py::time::time(), t3 = t2;
    int64_t carried = 0;
    if (!fresh.empty()) {
        Built built = kind->build(m, module_path, work);
        t2 = py::time::time();
        // What was built, per function: its own object code and that of everything it reaches.  A function whose text
        // changed but whose compiled code did not -- nor that of anything it calls -- is the same function to a judge
        // that runs code: its verdict is kept.
        py::dict<py::str, py::str> objects;
        if (kind->PER_FUNCTION() && build::functions::OBJECTS.contains(work)) objects = build::functions::OBJECTS.at(work);
        py::dict<py::str, py::str> built_now;
        if (!objects.empty()) built_now = signatures(m, std::nullopt, objects);
        if (!built_now.empty()) {
            py::dict<py::str, Cell> before = fs->built(sha, digest);
            py::dict<py::str, engine::native_engine::Candidate> had;
            for (const engine::native_engine::Candidate& r : fs->candidates(sha, digest, SOURCE)) {
                if (!std::holds_alternative<std::monostate>(r.verdict)) had.set(cell_text(r.addr), r);
            }
            py::set<py::str> keep;
            for (const py::str& n : fresh) {
                py::str k = m.key.at(n);
                if (built.items.contains(n) && before.contains(k) && !std::holds_alternative<std::monostate>(before.at(k)) &&
                    built_now.contains(n) && cell_is(before.at(k), built_now.at(n)) && had.contains(k)) {
                    const engine::native_engine::Candidate& r = had.at(k);
                    py::str verdict = cell_text(r.verdict);
                    if (verdict == V::NO_COMPILE || verdict == V::NO_SOURCE) continue;
                    Row row;
                    row.addr = k;
                    row.sig = sig.at(n);
                    row.name = cell_of(n);
                    row.source = cell_of(m.texts.at(n));
                    row.verdict = verdict;
                    row.reason = r.reason;
                    row.limits = r.limits;
                    row.detail = r.detail;
                    rows.push_back(row);
                    keep.add(n);
                }
            }
            carried = keep.size();
            std::vector<py::str> still;
            for (const py::str& n : fresh)
                if (!keep.contains(n)) still.push_back(n);
            fresh = still;
            py::dict<py::str, py::str> sigs;
            for (const auto& kv : built_now.items())
                if (m.key.contains(std::get<0>(kv))) sigs.set(m.key.at(std::get<0>(kv)), std::get<1>(kv));
            fs->set_built(sha, digest, sigs);
        }
        py::list<py::str> built_names;
        for (const py::str& n : built.items) built_names.append(n);
        py::set<py::str> tainted = _reaches(m.texts, built_names, built.dropped);
        for (const py::str& n : fresh) {
            if (built.items.contains(n)) continue;
            Row r;
            r.addr = m.key.at(n);
            r.sig = sig.at(n);
            r.name = cell_of(n);
            r.source = cell_of(m.texts.at(n));
            r.verdict = V::NO_COMPILE;
            py::str why = built.dropped.get(n, py::str(""));
            r.reason = cell_of(why.empty() ? py::str("not in the build") : why);
            r.problem = cell_of(py::str("compile"));
            rows.push_back(r);
        }
        py::dict<py::str, py::str> name_of;
        py::list<std::tuple<py::str, Item>> todo;
        for (const py::str& n : fresh) {
            if (!built.items.contains(n)) continue;
            name_of.set(m.key.at(n), n);
            todo.append(std::make_tuple(m.key.at(n), built.items.at(n)));
        }
        jobs::pool::Options<std::tuple<py::str, Item>, Verdict> opts;
        opts.crashed = [](const std::tuple<py::str, Item>&) { return NOT_RUN(); };
        const Kind& kref = *kind;
        py::dict<py::str, Verdict> judged = jobs::pool::run_first(
            [&kref](const Context& context, const std::tuple<py::str, Item>& item) { return kref.judge(context, std::get<1>(item)); },
            todo, built.context, opts);
        for (const auto& kv : judged.items()) {
            const py::str& k = std::get<0>(kv);
            auto [verdict, reason, limits, detail] = std::get<1>(kv);
            const py::str& n = name_of.at(k);
            if (tainted.contains(n) && V::GOOD().contains(verdict)) {
                py::set<py::str> s = py::set<py::str>::from(limits);
                s.add(py::str("stub-callee"));     // a function it calls was a stand-in
                std::vector<py::str> v(s.begin(), s.end());
                std::sort(v.begin(), v.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
                limits = py::list<py::str>(v);
            }
            Row r;
            r.addr = k;
            r.sig = sig.at(n);
            r.name = cell_of(n);
            r.source = cell_of(m.texts.at(n));
            r.verdict = verdict;
            r.reason = cell_of(reason);
            r.limits = cell_of(py::join(py::str(","), limits));
            r.detail = cell_of(detail);
            rows.push_back(r);
        }
        t3 = py::time::time();
    }
    py::list<engine::native_engine::FunctionStore::RecordRow> out;
    for (const Row& r : rows) out.append(record_row(r));
    Cell rev = Cell(kind->SOURCE_REV() ? *kind->SOURCE_REV() : py::to_str(decompile::decompiler::REV));
    fs->record_many(sha, SOURCE, rev, digest, out);
    py::dict<py::str, Cell> c = fs->scores(digest, sha);
    py::list<py::str> parts;
    for (const py::str& k : store::functions::SCORED())
        if (cell_int(c.at(k))) parts.append(py::percent("%s %d", store::functions::TITLE().at(k), cell_int(c.at(k))));
    log(py::percent("%s: %d functions, %d judged now%s: %s  (%.0fs: decompile %.0f, build %.0f, judge %.0f)", name,
                    m.all_keys.size(), static_cast<int64_t>(fresh.size()),
                    carried ? py::percent(", %d kept (same code)", carried) : py::str(""), py::join(py::str(", "), parts),
                    py::time::time() - t0, t1 - t0, t2 - t1, t3 - t2));
    return {sha, digest};
}

}  // namespace codebreaker::verify::reconstruct
