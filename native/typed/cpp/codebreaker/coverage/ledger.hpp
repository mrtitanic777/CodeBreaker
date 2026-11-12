// codebreaker/coverage/ledger.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/coverage/ledger*.{hpp,cpp};
// this header keeps the old py:: surface (Counts, Row, Summary, Ledger with their py::dict / py::list members, open_store, measure,
// reasons, render, write_csv, to_value, main) for the ports and the dynamic exe that still use it. Conversion and forwarding only: the
// old store (native_engine::FunctionStore) is opened again, read-only, as a cb::engine::FunctionStore over the same files; the corpus is the
// old corpus port's process-wide cb::base::Corpus; which verifier settings each kind is judged under is asked of the old
// verify/reconstruct. COMPAT(py): remove with store/functions, verify/reconstruct and the callers of this header.
//
// Not carried over: `_classes` and `_counts` (the new Ledger counts a module by its sha, and keeps the counts itself).
//
// DIVERGES, for the old callers: the `--module` output and `render` are the new module's text (the same columns); a row's keys in the
// `--json` output are in the order the new report writes them; an address the function store holds that does not parse is left out of a
// module's byte counts instead of dropping the whole module's.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/coverage/ledger.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/inspect.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"

namespace codebreaker::coverage::ledger {

namespace nl = ::cb::coverage::ledger;
namespace shim_ {
namespace F = ::codebreaker::store::functions;
}

using Store = std::shared_ptr<engine::native_engine::FunctionStore>;

// KINDS: (kind, title)
inline const py::list<std::tuple<py::str, py::str>>& KINDS() {
    static const py::list<std::tuple<py::str, py::str>> k = [] {
        py::list<std::tuple<py::str, py::str>> out;
        for (const nl::Kind& kind : nl::kKinds)
            out.append({::cb::compat::to_py(std::string(kind.id)), ::cb::compat::to_py(std::string(kind.title))});
        return out;
    }();
    return k;
}
inline const py::dict<py::str, py::str>& TITLE() {
    static const py::dict<py::str, py::str> t = [] {
        py::dict<py::str, py::str> d;
        for (const auto& kv : KINDS()) d.set(std::get<0>(kv), std::get<1>(kv));
        return d;
    }();
    return t;
}
// COUNTS: ("functions", "decompiled", "compiled") + ORDER + ("code_bytes", "bytes_known", "bytes_verified", "bytes_weak")
inline const py::list<py::str>& COUNTS() {
    static const py::list<py::str> c = [] {
        py::list<py::str> out = {py::str("functions"), py::str("decompiled"), py::str("compiled")};
        for (const py::str& k : shim_::F::ORDER()) out.append(k);
        for (const char* k : {"code_bytes", "bytes_known", "bytes_verified", "bytes_weak"}) out.append(py::str(k));
        return out;
    }();
    return c;
}

// The numbers of a row (COUNTS and "judged").
struct Counts {
    int64_t functions = 0, decompiled = 0, compiled = 0;
    py::dict<py::str, int64_t> classes;           // {class: functions} for every class of verdicts::ORDER
    int64_t code_bytes = 0, bytes_known = 0, bytes_verified = 0, bytes_weak = 0;
    int64_t judged = 0;
    Counts() {
        for (const py::str& k : shim_::F::ORDER()) classes.set(k, 0);
    }
    Counts(const Counts& o) { *this = o; }
    Counts& operator=(const Counts& o) {
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
    // r[key] for a key of COUNTS or "judged"
    int64_t at(const py::str& key) const {
        if (key == py::str("functions")) return functions;
        if (key == py::str("decompiled")) return decompiled;
        if (key == py::str("compiled")) return compiled;
        if (key == py::str("code_bytes")) return code_bytes;
        if (key == py::str("bytes_known")) return bytes_known;
        if (key == py::str("bytes_verified")) return bytes_verified;
        if (key == py::str("bytes_weak")) return bytes_weak;
        if (key == py::str("judged")) return judged;
        return classes.at(key);
    }
};

struct CorpusModule {
    py::str name, path, kind;
    int64_t size = 0, code_bytes = 0;
};

// A module row of the ledger.
struct Row : Counts {
    py::str name, path, kind;
    int64_t size = 0;
    bool in_store = false, measured = false;
    std::optional<py::str> sha;                   // set when the store holds the module
};

// A kind row (or the total): the rows' sums, and how many modules / how many in the store / how many measured.
struct Summary : Counts {
    py::str kind, name;
    int64_t modules = 0, in_store = 0, measured = 0;
};

struct Ledger {
    py::dict<py::str, std::optional<py::str>> profiles;   // {kind: digest}
    py::list<Summary> kinds;
    Summary total;
    py::list<Row> modules;
    py::str generated, corpus;
};

namespace shim_ {

namespace conv = ::cb::compat;

inline nl::Services services() {
    nl::Services s;
    s.corpus = &::codebreaker::base::corpus::detail::instance();
    s.state_dir = conv::legacy::path_of(py::settings::get_str(py::str("state")));
    s.verifier_profiles = [] {
        std::map<std::string, std::optional<std::string>> out;
        for (const auto& kv : verify::reconstruct::digests().items()) {
            const auto& digest = std::get<1>(kv);
            out[std::get<0>(kv).utf8()] = digest ? std::optional<std::string>(digest->utf8()) : std::nullopt;
        }
        return out;
    };
    return s;
}

// The old store's files, opened for reading by the new function store (null: no store).
inline std::unique_ptr<::cb::engine::FunctionStore> reopen(const Store& fs) {
    if (!fs) return nullptr;
    return std::make_unique<::cb::engine::FunctionStore>(conv::legacy::path_of(fs->path), conv::legacy::path_of(fs->folder), true);
}

inline void counts_from(Counts& out, const nl::Counts& c) {
    out.functions = c.functions;
    out.decompiled = c.decompiled;
    out.compiled = c.compiled;
    for (const auto& [cls, n] : c.classes) out.classes.set(py::str(std::string(::cb::verify::word(cls))), n);
    out.code_bytes = c.code_bytes;
    out.bytes_known = c.bytes_known;
    out.bytes_verified = c.bytes_verified;
    out.bytes_weak = c.bytes_weak;
    out.judged = c.judged;
}

inline Summary summary_from(const nl::Summary& s) {
    Summary out;
    counts_from(out, s);
    out.kind = conv::to_py(s.kind);
    out.name = conv::to_py(s.name);
    out.modules = s.modules;
    out.in_store = s.in_store;
    out.measured = s.measured;
    return out;
}

inline Ledger ledger_from(const nl::Report& r) {
    Ledger out;
    for (const auto& [kind, digest] : r.profiles)
        out.profiles.set(conv::to_py(kind), digest ? std::optional<py::str>(conv::to_py(*digest)) : std::nullopt);
    for (const nl::Summary& s : r.kinds) out.kinds.append(summary_from(s));
    out.total = summary_from(r.total);
    for (const nl::Row& row : r.modules) {
        Row m;
        counts_from(m, row);
        m.name = conv::to_py(row.name);
        m.path = conv::to_py(row.path);
        m.kind = conv::to_py(row.kind);
        m.size = row.size;
        m.in_store = row.in_store;
        m.measured = row.measured;
        if (row.sha) m.sha = conv::to_py(*row.sha);
        out.modules.append(m);
    }
    out.generated = conv::to_py(r.generated);
    out.corpus = conv::to_py(r.corpus);
    return out;
}

inline void counts_into(nl::Counts& out, const Counts& c) {
    out.functions = c.functions;
    out.decompiled = c.decompiled;
    out.compiled = c.compiled;
    for (::cb::verify::Class cls : ::cb::verify::kClassOrder) out.classes[cls] = c.classes.at(py::str(std::string(::cb::verify::word(cls))));
    out.code_bytes = c.code_bytes;
    out.bytes_known = c.bytes_known;
    out.bytes_verified = c.bytes_verified;
    out.bytes_weak = c.bytes_weak;
    out.judged = c.judged;
}

inline nl::Summary summary_into(const Summary& s) {
    nl::Summary out;
    counts_into(out, s);
    out.kind = conv::from_py(s.kind);
    out.name = conv::from_py(s.name);
    out.modules = s.modules;
    out.in_store = s.in_store;
    out.measured = s.measured;
    return out;
}

inline nl::Report report_of(const Ledger& l) {
    nl::Report out;
    for (const auto& kv : l.profiles.items()) {
        const auto& digest = std::get<1>(kv);
        out.profiles[conv::from_py(std::get<0>(kv))] = digest ? std::optional<std::string>(conv::from_py(*digest)) : std::nullopt;
    }
    for (const Summary& s : l.kinds) out.kinds.push_back(summary_into(s));
    out.total = summary_into(l.total);
    for (const Row& row : l.modules) {
        nl::Row m;
        counts_into(m, row);
        m.name = conv::from_py(row.name);
        m.path = conv::from_py(row.path);
        m.kind = conv::from_py(row.kind);
        m.size = row.size;
        m.in_store = row.in_store;
        m.measured = row.measured;
        if (row.sha) m.sha = conv::from_py(*row.sha);
        out.modules.push_back(std::move(m));
    }
    out.generated = conv::from_py(l.generated);
    out.corpus = conv::from_py(l.corpus);
    return out;
}

inline py::list<py::str> lines_of(const std::vector<std::string>& v) {
    py::list<py::str> out;
    for (const std::string& s : v) out.append(conv::to_py(s));
    return out;
}

}  // namespace shim_

// The function store, read-only; None when there is none.
inline Store open_store(const std::optional<py::str>& path = std::nullopt) {
    return shim_::F::open(path && !path->empty() ? *path : shim_::F::default_path(), true);
}

// {kind: digest} -- the verifier settings in force now, per kind of module.
inline py::dict<py::str, std::optional<py::str>> profiles(const Store& fs = nullptr) {
    const auto store = shim_::reopen(fs);
    nl::Ledger ledger(shim_::services(), store.get());
    py::dict<py::str, std::optional<py::str>> out;
    for (const auto& [kind, digest] : ledger.profiles())
        out.set(::cb::compat::to_py(kind), digest ? std::optional<py::str>(::cb::compat::to_py(*digest)) : std::nullopt);
    return out;
}

inline py::list<CorpusModule> corpus_modules(bool refresh = false) {
    nl::Ledger ledger(shim_::services(), nullptr);
    py::list<CorpusModule> out;
    for (const nl::CorpusModule& m : ledger.corpus_modules(refresh))
        out.append({::cb::compat::to_py(m.name), ::cb::compat::to_py(m.path), ::cb::compat::to_py(m.kind), m.size, m.code_bytes});
    return out;
}

// {address: bytes} -- how far each known function reaches
inline py::dict<py::str, int64_t> extents(const py::str& path, const py::str& kind, const py::list<py::str>& addrs) {
    std::vector<std::string> a;
    for (const py::str& x : addrs) a.push_back(x.utf8());
    py::dict<py::str, int64_t> out;
    for (const auto& [addr, bytes] : nl::extents(::cb::compat::legacy::path_of(path), kind.utf8(), a)) out.set(py::str(addr), bytes);
    return out;
}

// The ledger. `fs` null: the store is opened here (and closed). `module` limits it to one name.
inline Ledger measure(const Store& fs = nullptr, const std::optional<py::str>& module = std::nullopt, bool refresh = false) {
    const Store opened = fs ? fs : open_store();
    const auto store = shim_::reopen(opened);
    nl::Ledger ledger(shim_::services(), store.get());
    const nl::Report report = ledger.measure(module ? std::optional<std::string_view>(module->utf8()) : std::nullopt, refresh);
    if (!fs && opened) opened->close();
    return shim_::ledger_from(report);
}

// {class: [(reason, functions)]} -- why the functions of a module that are not verified are not, commonest first.
inline py::dict<py::str, py::list<std::tuple<py::str, int64_t>>> reasons(const Store& fs, const py::str& sha, const py::str& digest,
                                                                         int64_t most = 8) {
    const auto store = shim_::reopen(fs);
    nl::Ledger ledger(shim_::services(), store.get());
    py::dict<py::str, py::list<std::tuple<py::str, int64_t>>> out;
    for (const auto& [cls, found] : ledger.reasons(sha.utf8(), digest.utf8(), static_cast<std::size_t>(most))) {
        py::list<std::tuple<py::str, int64_t>> top;
        for (const auto& [why, n] : found) top.append(std::make_tuple(py::str(why), n));
        out.set(py::str(std::string(::cb::verify::word(cls))), top);
    }
    return out;
}

// ---- as text
inline py::str _pct(int64_t a, int64_t b) { return ::cb::compat::to_py(nl::percent(a, b)); }
inline py::list<py::str> render(const Ledger& led, bool every = false) { return shim_::lines_of(nl::render(shim_::report_of(led), every)); }
inline py::str _state(const Row& r) {
    nl::Row row;
    row.in_store = r.in_store;
    row.measured = r.measured;
    row.judged = r.judged;
    return py::str(std::string(nl::state_of(row)));
}
inline int64_t write_csv(const Ledger& led, const py::str& path) {
    return static_cast<int64_t>(nl::write_csv(shim_::report_of(led), ::cb::compat::legacy::path_of(path)));
}

// The whole ledger as the Python's dict (the --json output).
inline formats::inspect::Value to_value(const Ledger& led) {
    using Value = formats::inspect::Value;
    const auto counts_value = [](Value& v, const Counts& c) {
        v.set(py::str("functions"), Value(c.functions));
        v.set(py::str("decompiled"), Value(c.decompiled));
        v.set(py::str("compiled"), Value(c.compiled));
        for (const py::str& k : shim_::F::ORDER()) v.set(k, Value(c.classes.at(k)));
        v.set(py::str("code_bytes"), Value(c.code_bytes));
        v.set(py::str("bytes_known"), Value(c.bytes_known));
        v.set(py::str("bytes_verified"), Value(c.bytes_verified));
        v.set(py::str("bytes_weak"), Value(c.bytes_weak));
        v.set(py::str("judged"), Value(c.judged));
    };
    const auto summary_value = [&](const Summary& s) {
        Value v = Value::dict();
        counts_value(v, s);
        v.set(py::str("modules"), Value(s.modules));
        v.set(py::str("in_store"), Value(s.in_store));
        v.set(py::str("measured"), Value(s.measured));
        v.set(py::str("kind"), Value(s.kind));
        v.set(py::str("name"), Value(s.name));
        return v;
    };
    Value v = Value::dict();
    Value p = Value::dict();
    for (const auto& kv : led.profiles.items()) p.set(std::get<0>(kv), std::get<1>(kv) ? Value(*std::get<1>(kv)) : Value());
    v.set(py::str("profiles"), std::move(p));
    Value kinds = Value::list();
    for (const Summary& s : led.kinds) kinds.push(summary_value(s));
    v.set(py::str("kinds"), std::move(kinds));
    v.set(py::str("total"), summary_value(led.total));
    Value mods = Value::list();
    for (const Row& r : led.modules) {
        Value m = Value::dict();
        m.set(py::str("name"), Value(r.name));
        m.set(py::str("path"), Value(r.path));
        m.set(py::str("kind"), Value(r.kind));
        m.set(py::str("size"), Value(r.size));
        m.set(py::str("in_store"), Value(r.in_store));
        m.set(py::str("measured"), Value(r.measured));
        counts_value(m, r);
        if (r.sha) m.set(py::str("sha"), Value(*r.sha));
        mods.push(std::move(m));
    }
    v.set(py::str("modules"), std::move(mods));
    v.set(py::str("generated"), Value(led.generated));
    v.set(py::str("corpus"), Value(led.corpus));
    return v;
}

// the module's text, printed by `coverage --help`
inline const char* DOC = "coverage: how much of the corpus is accounted for, from the function store and the corpus (see cb/coverage/ledger.hpp)\n";

// python -m codebreaker coverage [--module NAME] [--all] [--csv FILE] [--json] [--refresh]
inline int64_t main(const py::list<py::str>& argv) {
    nl::Services s = shim_::services();
    const std::filesystem::path index = ::cb::compat::legacy::path_of(shim_::F::default_path());
    s.store_index = index;
    s.store_folder = ::cb::compat::legacy::path_of(shim_::F::databases_folder(shim_::F::default_path(), false));
    std::vector<std::string> args = {"coverage"};
    for (const py::str& a : argv) args.push_back(a.utf8());
    return ::cb::cli::Cli("codebreaker", {nl::command(s)}).run(args);
}

}  // namespace codebreaker::coverage::ledger
