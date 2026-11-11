// codebreaker/coverage/ledger.cpp -- typed C++ of codebreaker.coverage.ledger (see ledger.hpp).
//
// ledger.py -- the coverage ledger: how much of every module of the corpus is accounted for.  Two sources and nothing
// else: the corpus (base/corpus) says which modules exist and how much code each holds; the function store
// (store/functions) says what has been made of each function.  The store is opened read-only: measuring coverage never
// changes a result and can run beside a sweep.

#include "codebreaker/coverage/ledger.hpp"

#include <algorithm>
#include <ctime>
#include <map>
#include <set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/inspect.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "json.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::coverage::ledger {

using engine::native_engine::Cell;
using formats::inspect::Value;
using py::str;

namespace F = store::functions;
namespace V = verify::verdicts;

// ---- the vocabulary
const py::list<std::tuple<str, str>>& KINDS() {
    static const py::list<std::tuple<str, str>> k = {{str("pe"), str("32-bit")}, {str("ne"), str("16-bit")},
                                                    {str("le"), str("driver")}, {str("dos"), str("DOS")},
                                                    {str("java"), str("Java")}};
    return k;
}

const py::dict<str, str>& TITLE() {
    static const py::dict<str, str> t = [] {
        py::dict<str, str> d;
        for (const auto& kv : KINDS()) d.set(std::get<0>(kv), std::get<1>(kv));
        return d;
    }();
    return t;
}

const py::list<str>& COUNTS() {
    static const py::list<str> c = [] {
        py::list<str> out = {str("functions"), str("decompiled"), str("compiled")};
        for (const str& k : F::ORDER()) out.append(k);
        for (const char* k : {"code_bytes", "bytes_known", "bytes_verified", "bytes_weak"}) out.append(str(k));
        return out;
    }();
    return c;
}

namespace {

// _SWEPT: what `sweep` goes through; _EXTS: the extensions per kind (java is not in the binary corpus)
const py::list<str>& exts_of(const str& kind) {
    static const py::list<str> swept = {str(".dll"), str(".exe"), str(".drv"), str(".ocx"), str(".cpl")};
    static const py::list<str> le = {str(".vxd"), str(".386"), str(".pdr"), str(".sys"), str(".drv"), str(".exe")};
    static const py::list<str> dos = {str(".exe"), str(".com"), str(".sys"), str(".vxd"), str(".386")};
    static const py::list<str> none;
    if (kind == str("pe") || kind == str("ne")) return swept;
    if (kind == str("le")) return le;
    if (kind == str("dos")) return dos;
    return none;
}
bool has_exts(const str& kind) { return kind == str("pe") || kind == str("ne") || kind == str("le") || kind == str("dos"); }

using Stamp = std::tuple<int64_t, int64_t, Cell>;
// _rows: (sha, digest) -> (stamp, counts)
std::map<std::string, std::pair<std::optional<Stamp>, Counts>>& rows_cache() {
    static std::map<std::string, std::pair<std::optional<Stamp>, Counts>> m;
    return m;
}

bool cell_truthy(const Cell& c) {
    if (std::holds_alternative<str>(c)) return !std::get<str>(c).empty();
    if (std::holds_alternative<py::bytes>(c)) return !std::get<py::bytes>(c).empty();
    if (std::holds_alternative<int64_t>(c)) return std::get<int64_t>(c) != 0;
    if (std::holds_alternative<double>(c)) return std::get<double>(c) != 0.0;
    return false;
}

// settings.state(*parts): a path inside the state folder, created if it is not there
str state_path(const str& name) {
    str path = py::os::path::join(py::settings::get_str(str("state")), name);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

bool less_str(const str& a, const str& b) { return a.u32() < b.u32(); }

}  // namespace

Counts::Counts() {
    for (const str& k : F::ORDER()) classes.set(k, 0);
}

Counts::Counts(const Counts& o)
    : functions(o.functions), decompiled(o.decompiled), compiled(o.compiled), classes(o.classes.copy()), code_bytes(o.code_bytes),
      bytes_known(o.bytes_known), bytes_verified(o.bytes_verified), bytes_weak(o.bytes_weak), judged(o.judged) {}

Counts& Counts::operator=(const Counts& o) {
    functions = o.functions;
    decompiled = o.decompiled;
    compiled = o.compiled;
    classes = o.classes.copy();
    code_bytes = o.code_bytes;
    bytes_known = o.bytes_known;
    bytes_verified = o.bytes_verified;
    bytes_weak = o.bytes_weak;
    judged = o.judged;
    return *this;
}

int64_t Counts::at(const str& key) const {
    if (key == str("functions")) return functions;
    if (key == str("decompiled")) return decompiled;
    if (key == str("compiled")) return compiled;
    if (key == str("code_bytes")) return code_bytes;
    if (key == str("bytes_known")) return bytes_known;
    if (key == str("bytes_verified")) return bytes_verified;
    if (key == str("bytes_weak")) return bytes_weak;
    if (key == str("judged")) return judged;
    return classes.at(key);
}

// ---- the store and the profiles
Store open_store(const std::optional<str>& path) {
    return F::open(path && !path->empty() ? *path : F::default_path(), true);
}

py::dict<str, std::optional<str>> profiles(const Store& fs) {
    py::dict<str, std::optional<str>> out;
    for (const auto& kv : verify::reconstruct::digests().items()) {
        const str& kind = std::get<0>(kv);
        std::optional<str> digest = std::get<1>(kv);
        out.set(kind, digest);
        if (!digest) out.set(kind, fs ? fs->latest_profile(kind) : std::nullopt);
    }
    return out;
}

// ---- the corpus
py::list<CorpusModule> corpus_modules(bool refresh) {
    str memo_path = state_path(str("coverage_corpus.json"));
    // the memo: path -> [size, mtime, code_bytes, kind]
    struct Entry {
        int64_t size = 0, mtime = 0, code_bytes = 0;
        str kind;
        bool ok = false;               // the remembered entry is a well-formed one
        std::optional<Value> odd;      // a malformed entry as it was read (written back as it is)
    };
    // a parsed JSON value as a Value (only a malformed memo entry is converted)
    auto convert = [](const py::V& v, auto&& self) -> Value {
        if (v.is_none()) return Value();
        if (v.is_bool()) return Value(v.b);
        if (v.is_int()) return Value(v.as_int());
        if (v.is_str()) return Value(str(v.str()));
        if (v.is_dict()) {
            Value d = Value::dict();
            for (const auto& kv : v.items()) d.set(str(kv.first.str()), self(kv.second, self));
            return d;
        }
        Value l = Value::list();
        if (v.is_seq()) for (const py::V& x : v.seq()) l.push(self(x, self));
        return l;
    };
    py::dict<str, Entry> memo;
    if (!refresh) {
        try {
            str text = py::open_rb(memo_path).read().decode("utf-8");
            py::V data = py::json_parse(text.utf8());
            if (data.is_dict()) {
                for (const auto& kv : data.items()) {
                    Entry e;
                    const py::V& v = kv.second;
                    if (v.is_list() && v.size() >= 4 && v[0].is_int() && v[1].is_int() && v[2].is_int() && v[3].is_str()) {
                        e.size = v[0].as_int();
                        e.mtime = v[1].as_int();
                        e.code_bytes = v[2].as_int();
                        e.kind = str(v[3].str());
                        e.ok = true;
                    } else {
                        e.odd = convert(v, convert);
                    }
                    memo.set(str(kv.first.str()), e);
                }
            }
        } catch (const py::OSError&) {
        } catch (const py::ValueError&) {
        } catch (const py::PyError&) {
        }
    }
    py::list<CorpusModule> out;
    bool changed = false;
    for (const auto& kt : KINDS()) {
        const str& kind = std::get<0>(kt);
        if (!has_exts(kind)) continue;                       // not in the binary corpus (java: given by folder)
        for (const str& path : base::corpus::modules(kind, exts_of(kind))) {
            int64_t size = py::os::path::getsize(path);
            int64_t mtime = static_cast<int64_t>(py::os::path::getmtime(path));
            Entry got;
            bool have = memo.contains(path);
            if (have) got = memo.at(path);
            if (!have || !got.ok || got.size != size || got.mtime != mtime) {
                formats::inspect::Description info = formats::inspect::describe(path);
                got = Entry();
                got.size = size;
                got.mtime = mtime;
                got.code_bytes = info.summary.get_int(str("code_bytes"), 0);
                got.kind = info.format && *info.format == str("W3") ? str("le") : kind;
                got.ok = true;
                memo.set(path, got);
                changed = true;
            }
            CorpusModule m;
            m.name = py::os::path::basename(path).lower();
            m.path = path;
            m.kind = got.kind;
            m.size = size;
            m.code_bytes = got.code_bytes;
            out.append(m);
        }
    }
    if (changed) {
        Value dump = Value::dict();
        for (const auto& kv : memo.items()) {                  // (the keys are unique: appended, not searched for)
            const Entry& e = std::get<1>(kv);
            dump.keys.push_back(std::get<0>(kv));
            if (e.odd) {
                dump.items.push_back(*e.odd);
            } else {
                Value l = Value::list();
                l.push(Value(e.size));
                l.push(Value(e.mtime));
                l.push(Value(e.code_bytes));
                l.push(Value(e.kind));
                dump.items.push_back(std::move(l));
            }
        }
        py::file f = py::open(memo_path + str(".tmp"), str("w"), str("utf-8"));
        f.write(formats::inspect::dumps(dump));
        f.close();
        py::os::replace(memo_path + str(".tmp"), memo_path);
    }
    std::vector<CorpusModule> sorted;
    for (const CorpusModule& m : out) sorted.push_back(m);
    std::stable_sort(sorted.begin(), sorted.end(), [](const CorpusModule& a, const CorpusModule& b) { return less_str(a.name, b.name); });
    py::list<CorpusModule> result;
    for (const auto& m : sorted) result.append(m);
    return result;
}

py::dict<str, int64_t> extents(const str& path, const str& kind, const py::list<str>& addrs) {
    struct Span {
        int64_t key, lo, hi;
    };
    std::vector<Span> spans;
    std::vector<std::pair<str, std::optional<std::pair<int64_t, int64_t>>>> where;   // address -> (span key, offset)
    try {
        if (kind == str("pe")) {
            formats::pe::PEFile f = formats::pe::open_pe(path);
            int64_t n = 0;
            for (const auto& s : f.code_sections()) {
                int64_t va = static_cast<int64_t>(std::get<0>(s));
                spans.push_back({n++, va, va + std::get<1>(s).size()});
            }
            for (const str& a : addrs) {
                int64_t va = py::int_parse(a, 16);
                std::optional<std::pair<int64_t, int64_t>> at;
                for (const Span& s : spans)
                    if (s.lo <= va && va < s.hi) { at = std::make_pair(s.key, va); break; }
                where.emplace_back(a, at);
            }
        } else if (kind == str("ne")) {
            formats::ne::NEFile nef(path);
            for (const formats::ne::Segment& s : nef.segments())
                if (s.kind == str("CODE")) spans.push_back({s.idx, 0, s.length});
            for (const str& a : addrs) {
                py::list<str> parts = py::split(a, str(":"));
                if (parts.size() != 2) throw py::ValueError("not enough values to unpack (expected 2)");
                where.emplace_back(a, std::make_pair(py::int_parse(parts[0]), py::int_parse(parts[1], 16)));
            }
        }
    } catch (const py::OSError&) {
        return py::dict<str, int64_t>();
    } catch (const py::ValueError&) {
        return py::dict<str, int64_t>();
    }
    std::map<int64_t, int64_t> ends;
    for (const Span& s : spans) ends[s.key] = s.hi;
    std::vector<std::pair<int64_t, std::vector<std::pair<int64_t, str>>>> starts;   // in the order the keys were first met
    for (const auto& w : where) {
        if (!w.second) continue;
        int64_t k = w.second->first, off = w.second->second;
        auto it = ends.find(k);
        if (it == ends.end() || !(off < it->second)) continue;
        auto s = std::find_if(starts.begin(), starts.end(), [&](const auto& p) { return p.first == k; });
        if (s == starts.end()) {
            starts.emplace_back(k, std::vector<std::pair<int64_t, str>>());
            s = starts.end() - 1;
        }
        s->second.emplace_back(off, w.first);
    }
    py::dict<str, int64_t> out;
    for (auto& ks : starts) {
        auto& found = ks.second;
        std::stable_sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.first != b.first ? a.first < b.first : less_str(a.second, b.second);
        });
        for (size_t i = 0; i < found.size(); i++) {
            int64_t nxt = i + 1 < found.size() ? found[i + 1].first : ends[ks.first];
            out.set(found[i].second, nxt - found[i].first);
        }
    }
    return out;
}

// ---- counting
std::tuple<py::dict<str, str>, int64_t> _classes(const Store& fs, const str& sha, const std::optional<str>& digest) {
    py::dict<str, engine::native_engine::Candidate> best;
    if (digest && !digest->empty()) best = fs->best(sha, *digest);
    py::list<str> addrs = fs->addresses(sha);
    py::dict<str, str> cls;
    int64_t judged = 0;
    for (const str& a : addrs) {
        if (best.contains(a)) {
            cls.set(a, engine::native_engine::cell_str(best.at(a).klass));
            judged++;
        } else {
            cls.set(a, str("no-source"));
        }
    }
    return {cls, judged};
}

Counts _counts(const Store& fs, const str& sha, const std::optional<str>& digest, const str& path, const str& kind,
               const std::optional<Stamp>& stamp) {
    std::string key = sha.utf8() + std::string(1, '\0') + (digest ? "1" + digest->utf8() : std::string("0"));
    if (stamp) {
        auto it = rows_cache().find(key);
        if (it != rows_cache().end() && it->second.first && *it->second.first == *stamp) return it->second.second;
    }
    auto classified = _classes(fs, sha, digest);
    const py::dict<str, str>& cls = std::get<0>(classified);
    Counts c;
    c.functions = cls.size();
    for (const auto& kv : cls.items()) c.classes.set(std::get<1>(kv), c.classes.at(std::get<1>(kv)) + 1);
    for (const str& k : V::BUILT()) c.compiled += c.classes.at(k);
    py::set<str> decompiled;
    for (const auto& r : fs->candidates(sha, digest ? *digest : str("")))
        if (cell_truthy(r.source)) decompiled.add(engine::native_engine::cell_str(r.addr));
    c.decompiled = decompiled.size();
    py::dict<str, int64_t> size;
    if (!path.empty() && py::os::path::isfile(path)) {
        py::list<str> addrs;
        for (const auto& kv : cls.items()) addrs.append(std::get<0>(kv));
        size = extents(path, kind, addrs);
    }
    for (const auto& kv : size.items()) {
        c.bytes_known += std::get<1>(kv);
        if (cls.at(std::get<0>(kv)) == str("verified")) c.bytes_verified += std::get<1>(kv);
        if (cls.at(std::get<0>(kv)) == str("weak")) c.bytes_weak += std::get<1>(kv);
    }
    c.judged = std::get<1>(classified);
    rows_cache()[key] = {stamp, c};
    return c;
}

Summary _total(const py::list<Row>& rows) {
    Summary out;
    for (const Row& r : rows) {
        out.functions += r.functions;
        out.decompiled += r.decompiled;
        out.compiled += r.compiled;
        for (const str& k : F::ORDER()) out.classes.set(k, out.classes.at(k) + r.classes.at(k));
        out.code_bytes += r.code_bytes;
        out.bytes_known += r.bytes_known;
        out.bytes_verified += r.bytes_verified;
        out.bytes_weak += r.bytes_weak;
        out.judged += r.judged;
        out.in_store += r.in_store ? 1 : 0;
        out.measured += r.measured ? 1 : 0;
    }
    out.modules = rows.size();
    return out;
}

Ledger measure(const Store& fs_in, const std::optional<str>& module, bool refresh) {
    bool own = !fs_in;
    Store fs = fs_in ? fs_in : open_store();
    py::list<Row> rows;
    py::dict<str, std::optional<str>> digests;
    try {
        digests = profiles(fs);
        std::vector<std::pair<str, std::vector<std::tuple<str, str, str>>>> stored;   // name -> [(sha, path, kind)]
        py::dict<str, Stamp> stamps;
        if (fs) stamps = fs->stamps();
        if (fs) {
            for (const auto& row : fs->module_rows()) {
                str sha = engine::native_engine::cell_str(row[0]);
                str name = engine::native_engine::cell_str(row[1]);
                str path = engine::native_engine::cell_str(row[2]);
                str kind = engine::native_engine::cell_str(row[3]);
                auto it = std::find_if(stored.begin(), stored.end(), [&](const auto& p) { return p.first == name; });
                if (it == stored.end()) {
                    stored.emplace_back(name, std::vector<std::tuple<str, str, str>>());
                    it = stored.end() - 1;
                }
                it->second.emplace_back(sha, path, kind);
            }
        }
        py::list<CorpusModule> mods = corpus_modules(refresh);
        py::set<str> have;
        for (const CorpusModule& m : mods) have.add(m.name);
        std::optional<str> want = module && !module->empty() ? std::optional<str>(py::os::path::basename(*module).lower()) : std::nullopt;
        for (const auto& nv : stored) {                       // in the store but not a corpus module
            const str& name = nv.first;
            if (!have.contains(name) && (!want || *want == name || *want == std::get<0>(py::os::path::splitext(name)))) {
                const auto& first = nv.second[0];
                const str& path = std::get<1>(first);
                const str& kind = std::get<2>(first);
                int64_t size = py::os::path::isfile(path) ? py::os::path::getsize(path) : 0;
                CorpusModule m;
                m.name = name;
                m.path = path;
                m.kind = TITLE().contains(kind) ? kind : str("pe");
                m.size = size;
                m.code_bytes = size ? formats::inspect::describe(path).summary.get_int(str("code_bytes"), 0) : 0;
                mods.append(m);
            }
        }
        std::vector<CorpusModule> chosen;
        for (const CorpusModule& m : mods) {
            if (!want || m.name == *want || std::get<0>(py::os::path::splitext(m.name)) == *want) chosen.push_back(m);
        }
        std::stable_sort(chosen.begin(), chosen.end(), [](const CorpusModule& a, const CorpusModule& b) { return less_str(a.name, b.name); });
        for (const CorpusModule& m : chosen) {
            Row row;
            row.name = m.name;
            row.path = m.path;
            row.kind = m.kind;
            row.size = m.size;
            row.code_bytes = m.code_bytes;
            std::vector<std::tuple<str, str, str>> versions;
            for (const auto& nv : stored)
                if (nv.first == m.name) versions = nv.second;
            if (versions.size() > 1) {                        // several files have had this name
                std::optional<str> mine = base::corpus::sha(m.path);
                std::vector<std::tuple<str, str, str>> same;
                for (const auto& v : versions)
                    if (mine && std::get<0>(v) == *mine) same.push_back(v);
                if (same.empty()) same.push_back(versions[0]);
                versions = same;
            }
            if (!versions.empty()) {
                const str& sha = std::get<0>(versions[0]);
                std::optional<str> digest;
                if (digests.contains(m.kind)) digest = digests.at(m.kind);
                std::optional<Stamp> stamp;
                if (stamps.contains(sha)) stamp = stamps.at(sha);
                Counts c = _counts(fs, sha, digest, m.path, m.kind, stamp);
                static_cast<Counts&>(row) = c;
                row.code_bytes = m.code_bytes;
                row.in_store = true;
                row.sha = sha;
                row.measured = row.functions != 0 && row.judged == row.functions;
            }
            rows.append(row);
        }
    } catch (...) {
        if (own && fs) fs->close();
        throw;
    }
    if (own && fs) fs->close();
    Ledger led;
    led.profiles = digests;
    for (const auto& kt : KINDS()) {
        py::list<Row> of;
        for (const Row& r : rows)
            if (r.kind == std::get<0>(kt)) of.append(r);
        Summary s = _total(of);
        s.kind = std::get<0>(kt);
        s.name = std::get<1>(kt);
        led.kinds.append(s);
    }
    led.total = _total(rows);
    led.total.kind = str("");
    led.total.name = str("all");
    led.modules = rows;
    std::time_t now = std::time(nullptr);
    std::tm tmv{};
    localtime_s(&tmv, &now);
    char buf[40];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", &tmv);
    led.generated = str(buf);
    led.corpus = base::corpus::root();
    return led;
}

py::dict<str, py::list<std::tuple<str, int64_t>>> reasons(const Store& fs, const str& sha, const str& digest, int64_t most) {
    // why[class] = Counter, in the order met
    std::vector<std::pair<str, std::vector<std::pair<str, int64_t>>>> why;
    auto counter = [&](const str& cls) -> std::vector<std::pair<str, int64_t>>& {
        for (auto& w : why)
            if (w.first == cls) return w.second;
        why.emplace_back(cls, std::vector<std::pair<str, int64_t>>());
        return why.back().second;
    };
    auto bump = [](std::vector<std::pair<str, int64_t>>& c, const str& k) {
        for (auto& e : c)
            if (e.first == k) { e.second++; return; }
        c.emplace_back(k, 1);
    };
    for (const auto& kv : fs->best(sha, digest).items()) {
        const engine::native_engine::Candidate& r = std::get<1>(kv);
        str cls = engine::native_engine::cell_str(r.klass);
        if (cls == str("weak")) {
            for (const str& code : r.weak) bump(counter(str("weak")), F::WEAK().at(code));
        } else if (cls != str("verified")) {
            str reason = engine::native_engine::cell_str(r.reason);
            if (reason.empty()) reason = str("(no reason recorded)");
            bump(counter(cls), reason.slice(0, 70));
        }
    }
    py::dict<str, py::list<std::tuple<str, int64_t>>> out;
    for (auto& w : why) {
        std::stable_sort(w.second.begin(), w.second.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        py::list<std::tuple<str, int64_t>> top;
        int64_t n = 0;
        for (const auto& e : w.second) {
            if (n++ >= most) break;
            top.append(std::make_tuple(e.first, e.second));
        }
        out.set(w.first, top);
    }
    return out;
}

// ---- as text
str _pct(int64_t a, int64_t b) {
    if (!b) return str("     -");
    return py::percent("%5.1f%%", 100.0 * static_cast<double>(a) / static_cast<double>(b));
}

py::list<str> _table(const py::list<str>& head, const py::list<py::list<str>>& rows) {
    py::list<py::list<str>> all;
    all.append(head);
    for (const auto& r : rows) all.append(r);
    std::vector<int64_t> widths;
    for (int64_t i = 0; i < head.size(); i++) {
        int64_t w = 0;
        for (const auto& r : all) w = std::max(w, r[i].size());
        widths.push_back(w);
    }
    py::list<str> out;
    for (const auto& r : all) {
        py::list<str> cells;
        for (int64_t i = 0; i < r.size(); i++) cells.append(i == 0 ? r[i].ljust(widths[static_cast<size_t>(i)]) : r[i].rjust(widths[static_cast<size_t>(i)]));
        out.append(py::join(str("  "), cells).rstrip());
    }
    return out;
}

namespace {

py::list<str> count_cells(const Counts& r) {
    py::list<str> out = {py::to_str(r.functions), py::to_str(r.decompiled), py::to_str(r.compiled)};
    for (const str& k : F::ORDER()) out.append(py::to_str(r.classes.at(k)));
    out.append(_pct(r.classes.at(str("verified")), r.functions));
    return out;
}

py::list<str> cat(const py::list<str>& a, const py::list<str>& b) {
    py::list<str> out = a.copy();
    for (const str& x : b) out.append(x);
    return out;
}

}  // namespace

str _state(const Row& r) {
    return r.measured ? str("measured") : r.judged ? str("part measured") : r.in_store ? str("not measured now") : str("untouched");
}

py::list<str> render(const Ledger& led, bool every) {
    py::list<str> out;
    out.append(py::percent("Coverage of %s", led.corpus));
    py::list<str> shown_profiles;
    for (const char* k : {"pe", "ne"}) {
        std::optional<str> d;
        if (led.profiles.contains(str(k))) d = led.profiles.at(str(k));
        shown_profiles.append(py::percent("%s %s", TITLE().at(str(k)), d && !d->empty() ? *d : str("(none)")));
    }
    out.append(str("verifier settings in force: ") + py::join(str(", "), shown_profiles));
    out.append(str(""));
    out.append(str("Functions, by kind of module"));
    py::list<str> names = {str("functions"), str("decompiled"), str("compiled")};
    for (const str& k : F::ORDER()) names.append(F::TITLE().at(k));
    names.append(str("verified %"));
    py::list<Summary> kinds_and_total = led.kinds.copy();
    kinds_and_total.append(led.total);
    {
        py::list<py::list<str>> rows;
        for (const Summary& r : kinds_and_total)
            rows.append(cat(py::list<str>{r.name, py::to_str(r.modules), py::to_str(r.in_store), py::to_str(r.measured)}, count_cells(r)));
        out.extend(_table(cat(py::list<str>{str("kind"), str("modules"), str("in store"), str("measured")}, names), rows));
    }
    out.append(str(""));
    out.append(str("Bytes of code, by kind of module"));
    {
        py::list<py::list<str>> rows;
        for (const Summary& r : kinds_and_total)
            rows.append(py::list<str>{r.name, py::to_str(r.code_bytes), py::to_str(r.bytes_known), _pct(r.bytes_known, r.code_bytes),
                                      py::to_str(r.bytes_verified), _pct(r.bytes_verified, r.code_bytes), py::to_str(r.bytes_weak),
                                      _pct(r.bytes_weak, r.code_bytes)});
        out.extend(_table(py::list<str>{str("kind"), str("code bytes"), str("in known functions"), str(""), str("in VERIFIED"),
                                        str(""), str("in VERIFIED (weak)"), str("")},
                          rows));
    }
    py::list<Row> shown;
    for (const Row& r : led.modules)
        if (every || r.in_store) shown.append(r);
    out.append(str(""));
    out.append(py::percent("Modules%s", every ? str("") : str(" the store holds (--all lists the rest)")));
    if (!shown.empty()) {
        py::list<py::list<str>> rows;
        for (const Row& r : shown) {
            py::list<str> row = {r.name, TITLE().at(r.kind), _state(r)};
            row = cat(row, count_cells(r));
            row.append(py::to_str(r.code_bytes));
            row.append(_pct(r.bytes_known, r.code_bytes));
            row.append(_pct(r.bytes_verified, r.code_bytes));
            rows.append(row);
        }
        out.extend(_table(cat(cat(py::list<str>{str("module"), str("kind"), str("state")}, names),
                              py::list<str>{str("code bytes"), str("known"), str("verified")}),
                          rows));
    } else {
        out.append(str("(none)"));
    }
    return out;
}

int64_t write_csv(const Ledger& led, const str& path) {
    std::vector<str> cols = {str("name"), str("kind"), str("state"), str("path"), str("size")};
    for (const str& c : COUNTS()) cols.push_back(c);
    cols.push_back(str("judged"));
    auto field = [](const str& s) {
        bool quote = false;
        for (char32_t c : s.u32())
            if (c == ',' || c == '"' || c == '\r' || c == '\n') quote = true;
        if (!quote) return s;
        return str("\"") + s.replace(str("\""), str("\"\"")) + str("\"");
    };
    std::string text;
    auto write_row = [&](const std::vector<str>& cells) {
        for (size_t i = 0; i < cells.size(); i++) text += (i ? "," : "") + field(cells[i]).utf8();
        text += "\r\n";
    };
    write_row(cols);
    for (const Row& r : led.modules) {
        std::vector<str> cells;
        for (const str& c : cols) {
            if (c == str("kind")) cells.push_back(TITLE().at(r.kind));
            else if (c == str("state")) cells.push_back(_state(r));
            else if (c == str("name")) cells.push_back(r.name);
            else if (c == str("path")) cells.push_back(r.path);
            else if (c == str("size")) cells.push_back(py::to_str(r.size));
            else cells.push_back(py::to_str(r.at(c)));
        }
        write_row(cells);
    }
    py::file f = py::open(path, str("wb"));
    f.write(py::bytes(text));
    f.close();
    return led.modules.size();
}

// ---- the ledger as the Python's dict
namespace {

void counts_into(Value& v, const Counts& c, bool with_judged_last) {
    v.set(str("functions"), Value(c.functions));
    v.set(str("decompiled"), Value(c.decompiled));
    v.set(str("compiled"), Value(c.compiled));
    for (const str& k : F::ORDER()) v.set(k, Value(c.classes.at(k)));
    v.set(str("code_bytes"), Value(c.code_bytes));
    v.set(str("bytes_known"), Value(c.bytes_known));
    v.set(str("bytes_verified"), Value(c.bytes_verified));
    v.set(str("bytes_weak"), Value(c.bytes_weak));
    if (with_judged_last) v.set(str("judged"), Value(c.judged));
}

Value summary_value(const Summary& s) {
    Value v = Value::dict();
    counts_into(v, s, true);
    v.set(str("modules"), Value(s.modules));
    v.set(str("in_store"), Value(s.in_store));
    v.set(str("measured"), Value(s.measured));
    v.set(str("kind"), Value(s.kind));
    v.set(str("name"), Value(s.name));
    return v;
}

Value row_value(const Row& r) {
    Value v = Value::dict();
    v.set(str("name"), Value(r.name));
    v.set(str("path"), Value(r.path));
    v.set(str("kind"), Value(r.kind));
    v.set(str("size"), Value(r.size));
    v.set(str("code_bytes"), Value(r.code_bytes));
    v.set(str("in_store"), Value(r.in_store));
    v.set(str("measured"), Value(r.measured));
    v.set(str("judged"), Value(r.judged));
    v.set(str("functions"), Value(r.functions));
    v.set(str("decompiled"), Value(r.decompiled));
    v.set(str("compiled"), Value(r.compiled));
    for (const str& k : F::ORDER()) v.set(k, Value(r.classes.at(k)));
    v.set(str("bytes_known"), Value(r.bytes_known));
    v.set(str("bytes_verified"), Value(r.bytes_verified));
    v.set(str("bytes_weak"), Value(r.bytes_weak));
    if (r.sha) v.set(str("sha"), Value(*r.sha));
    return v;
}

}  // namespace

Value to_value(const Ledger& led) {
    Value v = Value::dict();
    Value p = Value::dict();
    for (const auto& kv : led.profiles.items()) p.set(std::get<0>(kv), std::get<1>(kv) ? Value(*std::get<1>(kv)) : Value());
    v.set(str("profiles"), std::move(p));
    Value kinds = Value::list();
    for (const Summary& s : led.kinds) kinds.push(summary_value(s));
    v.set(str("kinds"), std::move(kinds));
    v.set(str("total"), summary_value(led.total));
    Value mods = Value::list();
    for (const Row& r : led.modules) mods.push(row_value(r));
    v.set(str("modules"), std::move(mods));
    v.set(str("generated"), Value(led.generated));
    v.set(str("corpus"), Value(led.corpus));
    return v;
}

// ---- the command
const char* DOC =
    "ledger.py -- the coverage ledger: how much of every module of the corpus is accounted for.\n"
    "\n"
    "    python -m codebreaker coverage                  by kind of module, then every module the store holds\n"
    "    python -m codebreaker coverage --all            list the modules nothing has been done to as well\n"
    "    python -m codebreaker coverage --module NAME    one module: its counts, its bytes, why functions fail\n"
    "    python -m codebreaker coverage --csv FILE       one row per corpus module, for a spreadsheet\n"
    "    python -m codebreaker coverage --json           the whole ledger as JSON\n"
    "\n"
    "    from codebreaker.coverage import ledger\n"
    "    led = ledger.measure()            # {'profiles', 'kinds': [row], 'total': row, 'modules': [row]}\n"
    "    fs = ledger.open_store()          # the function store, opened read-only\n"
    "\n"
    "Two sources and nothing else: the corpus (base/corpus.py) says which modules exist and how much\n"
    "code each holds; the function store (store/functions.py) says what has been made of each\n"
    "function. A row -- for a module, a kind of module, or everything -- counts:\n"
    "\n"
    "    functions     functions the store knows the module has\n"
    "    decompiled    of those, the ones some reconstruction has source for\n"
    "    compiled      of those, the ones whose source was built and run beside the original\n"
    "    verified, weak, unverified, not-judged, no-compile, no-source\n"
    "                  the store's own classes (functions.classify / ORDER / TITLE) under the verifier\n"
    "                  settings in force now; the six add up to `functions`\n"
    "    code_bytes    bytes of code in the file (the inspector's figure)\n"
    "    bytes_known   bytes that lie in a known function: from its start to the next known start, or\n"
    "                  to the end of its section or segment\n"
    "    bytes_verified, bytes_weak     the same, for functions of that class\n"
    "\n"
    "Kinds are 32-bit (PE), 16-bit (NE), driver (LE and the W3 bundle) and DOS. Only the first two\n"
    "can be reconstructed and judged today, so the other two show what is waiting. The store is\n"
    "opened read-only: measuring coverage never changes a result and can run beside a sweep.\n";

namespace {

std::optional<str> option(const py::list<str>& argv, const char* name) {
    for (int64_t i = 0; i < argv.size(); i++)
        if (argv[i] == str(name)) return i + 1 < argv.size() ? std::optional<str>(argv[i + 1]) : std::nullopt;
    return std::nullopt;
}

}  // namespace

int64_t main(const py::list<str>& argv) {
    if (argv.contains(str("-h")) || argv.contains(str("--help"))) {
        py::print(str(DOC));
        return 0;
    }
    Store fs = open_store();
    if (!fs) {
        py::print(py::percent("there is no function store yet (%s): nothing has been reconstructed; the corpus is listed alone",
                              F::default_path()));
    }
    std::optional<str> module = option(argv, "--module");
    Ledger led = measure(fs, module, argv.contains(str("--refresh")));
    if (module && led.modules.empty()) {
        py::print(py::percent("no module %s in the corpus or the store", py::repr(*module)));
        return 2;
    }
    std::optional<str> csv_path = option(argv, "--csv");
    if (csv_path) py::print(py::percent("wrote %d modules to %s", write_csv(led, *csv_path), *csv_path));
    if (argv.contains(str("--json"))) {
        py::print(formats::inspect::dumps(to_value(led), 1));
    } else if (module) {
        for (const Row& r : led.modules) {
            py::print(py::percent("%s  (%s, %s)  %s", r.name, TITLE().at(r.kind), _state(r), r.path));
            py::list<py::list<str>> rows;
            py::list<str> keys = {str("functions"), str("decompiled"), str("compiled")};
            for (const str& k : F::ORDER()) keys.append(k);
            for (const str& k : keys) {
                bool plain = k == str("functions") || k == str("decompiled") || k == str("compiled");
                rows.append(py::list<str>{plain ? k : F::TITLE().at(k), py::to_str(r.at(k)), _pct(r.at(k), r.functions)});
            }
            for (const str& l : _table(py::list<str>{str(""), str("functions"), str("share")}, rows)) py::print(l);
            py::print(py::percent("code bytes %d; in known functions %d (%s); in VERIFIED %d (%s); in VERIFIED (weak) %d (%s)", r.code_bytes,
                                  r.bytes_known, _pct(r.bytes_known, r.code_bytes).strip(), r.bytes_verified,
                                  _pct(r.bytes_verified, r.code_bytes).strip(), r.bytes_weak, _pct(r.bytes_weak, r.code_bytes).strip()));
            std::optional<str> digest;
            if (led.profiles.contains(r.kind)) digest = led.profiles.at(r.kind);
            if (r.in_store && digest && !digest->empty()) {
                auto why = reasons(fs, *r.sha, *digest);
                std::vector<std::pair<str, py::list<std::tuple<str, int64_t>>>> items;
                for (const auto& kv : why.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
                std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
                    return F::ORDER().index(a.first) < F::ORDER().index(b.first);
                });
                for (const auto& cls_found : items) {
                    py::print(py::percent("%s, by cause:", F::TITLE().at(cls_found.first)));
                    for (const auto& wn : cls_found.second) py::print(py::percent("  %5d  %s", std::get<1>(wn), std::get<0>(wn)));
                }
            }
        }
    } else if (!csv_path) {
        for (const str& l : render(led, argv.contains(str("--all")))) py::print(l);
    }
    if (fs) fs->close();
    return 0;
}

}  // namespace codebreaker::coverage::ledger
