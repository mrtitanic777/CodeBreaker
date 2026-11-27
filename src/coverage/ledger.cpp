#include "coverage/ledger.hpp"
#include "core/sys.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <format>
#include <set>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"
#include "formats/inspect.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"

namespace cb::coverage::ledger {

namespace stdfs = std::filesystem;

namespace {

// What `sweep` goes through, and the extensions of each kind (java is not in the binary corpus: it is given by folder).
const std::vector<std::string> kLedgerSwept = {".dll", ".exe", ".drv", ".ocx", ".cpl"};

struct LedgerLane {
    base::Lane lane;
    std::vector<std::string> extensions;
};

std::optional<LedgerLane> ledger_lane_of(std::string_view kind) {
    if (kind == "pe") return LedgerLane{base::Lane::Pe, kLedgerSwept};
    if (kind == "ne") return LedgerLane{base::Lane::Ne, kLedgerSwept};
    if (kind == "le") return LedgerLane{base::Lane::Le, base::le_module_extensions()};
    if (kind == "dos") return LedgerLane{base::Lane::Dos, {".exe", ".com", ".sys", ".vxd", ".386"}};
    return std::nullopt;
}

// (size, whole seconds since 1970 of the last write) of a file, as the memo remembers them.
std::pair<std::int64_t, std::int64_t> ledger_stat(const stdfs::path& path) {
    const auto size = static_cast<std::int64_t>(stdfs::file_size(path));
    const auto time = cb::fs::last_write_time(path);
    const auto since = std::chrono::clock_cast<std::chrono::system_clock>(time).time_since_epoch();
    return {size, std::chrono::duration_cast<std::chrono::seconds>(since).count()};
}

std::int64_t ledger_code_bytes_of(const formats::inspect::Description& d) {
    return d.summary ? static_cast<std::int64_t>(d.summary->code_bytes) : 0;
}

std::string ledger_basename_lower(const std::filesystem::path& p) { return text::lower(fs::to_text(p.filename())); }

std::string ledger_stem(const std::string& name) {
    const std::size_t dot = name.rfind('.');
    return dot == std::string::npos || dot == 0 ? name : name.substr(0, dot);
}

std::string ledger_clock_text() {
    const std::time_t now = std::time(nullptr);
    const std::tm local = sys::local_tm(now);
    char buf[40];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", &local);
    return buf;
}

bool ledger_has_text(const std::optional<std::string>& s) { return s && !s->empty(); }

}  // namespace

std::string_view kind_title(std::string_view kind) {
    for (const Kind& k : kKinds)
        if (k.id == kind) return k.title;
    return "";
}

Counts::Counts() {
    for (store::Class c : store::kClassOrder) classes[c] = 0;
}

// ---- the extents ---------------------------------------------------------------------------------------------------

std::map<std::string, std::int64_t> extents(const stdfs::path& path, std::string_view kind, const std::vector<std::string>& addrs) {
    struct Span {
        std::int64_t key, lo, hi;
    };
    std::vector<Span> spans;
    // each address: which span it is in, and where
    std::vector<std::pair<std::string, std::optional<std::pair<std::int64_t, std::int64_t>>>> where;
    try {
        if (kind == "pe") {
            const formats::pe::Image image = formats::pe::Image::open(path);
            std::int64_t n = 0;
            for (const auto& cs : image.code_sections()) {
                const auto va = static_cast<std::int64_t>(cs.va);
                spans.push_back({n++, va, va + static_cast<std::int64_t>(image.section_data(*cs.section).size())});
            }
            for (const std::string& a : addrs) {
                const auto va = text::parse_int<std::int64_t>(a, 16);
                if (!va) continue;
                std::optional<std::pair<std::int64_t, std::int64_t>> at;
                for (const Span& s : spans)
                    if (s.lo <= *va && *va < s.hi) {
                        at = std::make_pair(s.key, *va);
                        break;
                    }
                where.emplace_back(a, at);
            }
        } else if (kind == "ne") {
            const formats::ne::Module module = formats::ne::Module::open_file(path);
            for (const formats::ne::Segment& s : module.segments())
                if (!s.is_data()) spans.push_back({s.index, 0, static_cast<std::int64_t>(s.length)});
            for (const std::string& a : addrs) {
                const std::size_t colon = a.find(':');
                if (colon == std::string::npos) continue;
                const auto seg = text::parse_int<std::int64_t>(a.substr(0, colon), 10);
                const auto off = text::parse_int<std::int64_t>(a.substr(colon + 1), 16);
                if (seg && off) where.emplace_back(a, std::make_pair(*seg, *off));
            }
        }
    } catch (const Error&) {   // a file that cannot be read has no extents (the counts say so)
        return {};
    }
    std::map<std::int64_t, std::int64_t> ends;
    for (const Span& s : spans) ends[s.key] = s.hi;
    std::map<std::int64_t, std::vector<std::pair<std::int64_t, std::string>>> starts;   // span -> (offset, address)
    for (const auto& [addr, at] : where) {
        if (!at) continue;
        const auto end = ends.find(at->first);
        if (end == ends.end() || at->second >= end->second) continue;
        starts[at->first].emplace_back(at->second, addr);
    }
    std::map<std::string, std::int64_t> out;
    for (auto& [key, found] : starts) {
        std::sort(found.begin(), found.end());
        for (std::size_t i = 0; i < found.size(); ++i) {
            const std::int64_t next = i + 1 < found.size() ? found[i + 1].first : ends[key];
            out[found[i].second] = next - found[i].first;
        }
    }
    return out;
}

Summary total_of(const std::vector<Row>& rows) {
    Summary out;
    for (const Row& r : rows) {
        out.functions += r.functions;
        out.decompiled += r.decompiled;
        out.compiled += r.compiled;
        for (store::Class c : store::kClassOrder) out.classes[c] += r.classes.at(c);
        out.code_bytes += r.code_bytes;
        out.bytes_known += r.bytes_known;
        out.bytes_verified += r.bytes_verified;
        out.bytes_weak += r.bytes_weak;
        out.judged += r.judged;
        out.in_store += r.in_store ? 1 : 0;
        out.measured += r.measured ? 1 : 0;
    }
    out.modules = static_cast<std::int64_t>(rows.size());
    return out;
}

// ---- the ledger ----------------------------------------------------------------------------------------------------

Ledger::Ledger(Services services, store::FunctionStore* store) : services_(std::move(services)), store_(store) {
    CB_ASSERT(services_.corpus != nullptr, "the ledger needs the corpus");
}

std::map<std::string, std::optional<std::string>> Ledger::profiles() const {
    std::map<std::string, std::optional<std::string>> out;
    if (!services_.verifier_profiles) return out;
    for (auto [kind, digest] : services_.verifier_profiles()) {
        if (!digest && store_) digest = store_->latest_profile(kind);   // (the settings last used stand in)
        out[kind] = digest;
    }
    return out;
}

std::vector<CorpusModule> Ledger::corpus_modules(bool refresh) {
    // The memo: path -> [size, mtime, code_bytes, kind].
    const stdfs::path memo_path = services_.state_dir / "coverage_corpus.json";
    Json memo = Json::object();
    if (!refresh) {
        try {
            Json loaded = json::load(memo_path);
            if (loaded.is_object()) memo = std::move(loaded);
        } catch (const Error&) {   // no memo, or one that is damaged: every file is read again
        }
    }
    std::vector<CorpusModule> out;
    bool changed = false;
    for (const Kind& k : kKinds) {
        const auto lane = ledger_lane_of(k.id);
        if (!lane) continue;
        for (const stdfs::path& path : services_.corpus->modules(lane->lane, lane->extensions)) {
            const auto [size, mtime] = ledger_stat(path);
            const std::string key = fs::to_text(path);
            std::int64_t code_bytes = 0;
            std::string kind(k.id);
            const Json* got = memo.contains(key) ? &memo[key] : nullptr;
            const bool fresh = got && got->is_array() && got->size() >= 4 && (*got)[0] == size && (*got)[1] == mtime &&
                               (*got)[2].is_number_integer() && (*got)[3].is_string();
            if (fresh) {
                code_bytes = (*got)[2].get<std::int64_t>();
                kind = (*got)[3].get<std::string>();
            } else {
                const formats::inspect::Description info = formats::inspect::describe(path);
                code_bytes = ledger_code_bytes_of(info);
                if (info.format && *info.format == "W3") kind = "le";
                memo[key] = Json::array({size, mtime, code_bytes, kind});
                changed = true;
            }
            out.push_back({ledger_basename_lower(path), key, kind, size, code_bytes});
        }
    }
    if (changed) {
        try {
            fs::make_dirs(services_.state_dir);
            json::save(memo_path, memo, json::Style::Compact);
        } catch (const IoError&) {   // the memo is a convenience: a state folder that cannot be written is not a failure
        }
    }
    std::stable_sort(out.begin(), out.end(), [](const CorpusModule& a, const CorpusModule& b) { return a.name < b.name; });
    return out;
}

Counts Ledger::counts(const std::string& sha, const std::optional<std::string>& digest, const stdfs::path& path, const std::string& kind) {
    const auto key = std::make_pair(sha, digest.value_or(""));
    Stamp stamp;
    bool stamped = false;
    if (const auto it = stamps_.find(sha); it != stamps_.end()) {
        stamp = it->second;
        stamped = true;
        if (const auto kept = kept_.find(key); kept != kept_.end() && kept->second.first == stamp) return kept->second.second;
    }
    // every known function and its class: the best candidate's, and no-source for one nothing has been judged for
    std::map<std::string, store::Best> best;
    if (digest && !digest->empty()) best = store_->best(sha, *digest);
    std::vector<std::string> addrs = store_->addresses(sha);
    // (a driver's functions are object:offset; a bundle judged as a DOS program before it had the driver lane left plain addresses behind)
    if (kind == "le") std::erase_if(addrs, [](const std::string& a) { return a.find(':') == std::string::npos; });
    std::map<std::string, store::Class> classes;
    Counts c;
    for (const std::string& a : addrs) {
        const auto it = best.find(a);
        const store::Class cls = it == best.end() ? store::Class::NoSource : it->second.cls;
        classes[a] = cls;
        if (it != best.end()) ++c.judged;
        ++c.classes[cls];
    }
    c.functions = static_cast<std::int64_t>(addrs.size());
    for (store::Class built : store::kClassBuilt) c.compiled += c.classes.at(built);
    std::set<std::string> decompiled;
    for (const store::Candidate& cand : store_->candidates(sha, digest.value_or("")))
        if (ledger_has_text(cand.source) && classes.contains(cand.addr)) decompiled.insert(cand.addr);
    c.decompiled = static_cast<std::int64_t>(decompiled.size());
    std::error_code ec;
    if (!path.empty() && stdfs::is_regular_file(path, ec)) {
        for (const auto& [addr, bytes] : extents(path, kind, addrs)) {
            c.bytes_known += bytes;
            if (classes.at(addr) == store::Class::Verified) c.bytes_verified += bytes;
            if (classes.at(addr) == store::Class::Weak) c.bytes_weak += bytes;
        }
    }
    if (stamped) kept_[key] = {stamp, c};
    return c;
}

Report Ledger::measure(std::optional<std::string_view> module, bool refresh) {
    Report report;
    report.profiles = profiles();
    stamps_.clear();
    // name -> every module of the store by that name
    std::map<std::string, std::vector<store::ModuleRow>> stored;
    if (store_) {
        for (const auto& [sha, s] : store_->summaries()) stamps_[sha] = Stamp{s.functions, s.judged, s.last};
        for (store::ModuleRow& row : store_->module_rows()) stored[row.name].push_back(std::move(row));
    }
    std::vector<CorpusModule> mods = corpus_modules(refresh);
    std::set<std::string> have;
    for (const CorpusModule& m : mods) have.insert(m.name);
    std::optional<std::string> want;
    if (module && !module->empty()) want = text::lower(fs::to_text(fs::from_text(*module).filename()));
    for (const auto& [name, versions] : stored) {   // in the store but not a corpus module
        if (have.contains(name) || (want && *want != name && *want != ledger_stem(name))) continue;
        const store::ModuleRow& first = versions.front();
        std::error_code ec;
        const stdfs::path file = fs::from_text(first.path);
        const bool there = !first.path.empty() && stdfs::is_regular_file(file, ec);
        const std::int64_t size = there ? static_cast<std::int64_t>(stdfs::file_size(file)) : 0;
        CorpusModule m;
        m.name = name;
        m.path = first.path;
        m.kind = kind_title(first.kind).empty() ? "pe" : first.kind;
        m.size = size;
        m.code_bytes = size ? ledger_code_bytes_of(formats::inspect::describe(file)) : 0;
        mods.push_back(std::move(m));
    }
    if (want) {
        std::erase_if(mods, [&](const CorpusModule& m) { return m.name != *want && ledger_stem(m.name) != *want; });
    }
    std::stable_sort(mods.begin(), mods.end(), [](const CorpusModule& a, const CorpusModule& b) { return a.name < b.name; });

    for (const CorpusModule& m : mods) {
        Row row;
        row.name = m.name;
        row.path = m.path;
        row.kind = m.kind;
        row.size = m.size;
        row.code_bytes = m.code_bytes;
        std::vector<store::ModuleRow> versions;
        if (const auto it = stored.find(m.name); it != stored.end()) versions = it->second;
        if (versions.size() > 1) {   // several files have had this name: the corpus file's own
            const auto mine = services_.corpus->sha(fs::from_text(m.path));
            std::vector<store::ModuleRow> same;
            for (const store::ModuleRow& v : versions)
                if (mine && v.sha == *mine) same.push_back(v);
            if (same.empty()) same.push_back(versions.front());
            versions = std::move(same);
        }
        if (!versions.empty()) {
            const std::string& sha = versions.front().sha;
            std::optional<std::string> digest;
            if (const auto p = report.profiles.find(m.kind); p != report.profiles.end()) digest = p->second;
            static_cast<Counts&>(row) = counts(sha, digest, fs::from_text(m.path), m.kind);
            row.code_bytes = m.code_bytes;
            row.in_store = true;
            row.sha = sha;
            row.measured = row.functions != 0 && row.judged == row.functions;
        }
        report.modules.push_back(std::move(row));
    }
    for (const Kind& k : kKinds) {
        std::vector<Row> of;
        for (const Row& r : report.modules)
            if (r.kind == k.id) of.push_back(r);
        Summary s = total_of(of);
        s.kind = k.id;
        s.name = k.title;
        report.kinds.push_back(std::move(s));
    }
    report.total = total_of(report.modules);
    report.total.kind = "";
    report.total.name = "all";
    report.generated = services_.now_text ? services_.now_text() : ledger_clock_text();
    report.corpus = fs::to_text(services_.corpus->root());
    return report;
}

Ledger::Reasons Ledger::reasons(const std::string& sha, const std::string& digest, std::size_t most) const {
    // per class, the reasons in the order met with their counts
    std::map<store::Class, std::vector<std::pair<std::string, std::int64_t>>> why;
    const auto bump = [&](store::Class cls, const std::string& reason) {
        auto& counter = why[cls];
        for (auto& [r, n] : counter)
            if (r == reason) {
                ++n;
                return;
            }
        counter.emplace_back(reason, 1);
    };
    for (const auto& [addr, best] : store_->best(sha, digest)) {
        if (best.cls == store::Class::Weak) {
            for (store::WeakLimit limit : best.weak) bump(store::Class::Weak, std::string(store::explanation(limit)));
        } else if (best.cls != store::Class::Verified) {
            std::string reason = best.candidate.reason.empty() ? "(no reason recorded)" : best.candidate.reason;
            bump(best.cls, reason.substr(0, 70));
        }
    }
    Reasons out;
    for (store::Class cls : store::kClassOrder) {
        const auto it = why.find(cls);
        if (it == why.end()) continue;
        auto counter = it->second;
        std::stable_sort(counter.begin(), counter.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        if (counter.size() > most) counter.resize(most);
        out.emplace_back(cls, std::move(counter));
    }
    return out;
}

}  // namespace cb::coverage::ledger
