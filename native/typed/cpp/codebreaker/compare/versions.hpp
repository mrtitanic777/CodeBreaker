// codebreaker/compare/versions.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/compare/versions*.{hpp,cpp};
// this header keeps the old py:: surface (Side and Function as the old callers read them, the Comparison with its py::list / py::dict
// members, Product, main) for the ports and the dynamic exe that still use it. Conversion and forwarding only; what the comparison asks of the
// function store and the corpus is answered by forwarding to the old ports below (COMPAT(py): remove with them). Deleted with its last
// py:: consumer.
//
// Not carried over: the `Decoder` / `Ins` / `Operand` stand-ins of platforms.decoder (the new module decodes through cb::platforms), and
// `set_capstone_dll` (a no-op: the decoder loads capstone.dll itself).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <tuple>

#include <nlohmann/json.hpp>

#include "cb/compare/versions.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/inspect.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::compare::versions {

namespace nv = ::cb::compare::versions;
using formats::inspect::Value;
using py::str;

inline const py::list<str> LEVELS = {str("bytes"), str("text"), str("shape")};
inline const str ENGINE = str("carried");
inline void set_capstone_dll(const str&) {}
inline std::function<double()>& clock() {
    static std::function<double()> c = [] { return py::time::time(); };
    return c;
}

// ("f", key) another function of the module, ("i", name) an imported one
struct Call {
    str kind, key;
    bool operator==(const Call& o) const { return kind == o.kind && key == o.key; }
};
struct Ids {
    str bytes, text, shape;
    const str& at(const str& level) const { return level == str("bytes") ? bytes : level == str("text") ? text : shape; }
};
struct Function {
    str key, name;
    bool named = false;
    int64_t at = 0, size = 0;
    std::vector<std::tuple<int64_t, str>> rows;
    std::vector<str> text, shape;
    Ids ids;
    std::vector<Call> calls;
    std::set<std::u32string> marks;
    int64_t jumps = 0;
    int64_t count() const { return static_cast<int64_t>(shape.size()); }
};
class Side {
  public:
    Side(const str& path_, const str& kind_, int64_t code_ = 0) : path(path_), kind(kind_), code(code_) { name = py::os::path::basename(path_).lower(); }
    str path, kind, name;
    int64_t code = 0;
    py::dict<str, std::shared_ptr<Function>> functions;
    py::list<str> order;
    std::map<std::u32string, std::set<std::u32string>> callers;
    void add(const std::shared_ptr<Function>& f) {
        functions.set(f->key, f);
        order.append(f->key);
    }
    int64_t size() const {
        int64_t n = 0;
        for (const auto& kv : functions.items()) n += std::get<1>(kv)->size;
        return n;
    }
};

namespace shim_detail {

inline Function from_new(const nv::Function& n) {
    Function f;
    f.key = ::cb::compat::to_py(n.key);
    f.name = ::cb::compat::to_py(n.name);
    f.named = n.named;
    f.at = n.at;
    f.size = n.size;
    for (const nv::Row& r : n.rows) f.rows.emplace_back(static_cast<int64_t>(r.address), ::cb::compat::to_py(r.text));
    for (const std::string& t : n.text) f.text.push_back(::cb::compat::to_py(t));
    for (const std::string& t : n.shape) f.shape.push_back(::cb::compat::to_py(t));
    f.ids = {::cb::compat::to_py(n.ids.bytes), ::cb::compat::to_py(n.ids.text), ::cb::compat::to_py(n.ids.shape)};
    for (const nv::Call& c : n.calls) f.calls.push_back({str(c.to == nv::Call::To::Function ? "f" : "i"), ::cb::compat::to_py(c.key)});
    for (const std::string& m : n.marks) f.marks.insert(str(m).u32());
    f.jumps = n.jumps;
    return f;
}
inline std::shared_ptr<Side> from_new(const nv::Side& n) {
    auto s = std::make_shared<Side>(str(n.path().string()), str(std::string(nv::kind_name(n.kind()))), n.code());
    for (const nv::Function& f : n.functions()) s->add(std::make_shared<Function>(from_new(f)));
    for (const auto& kv : s->functions.items())
        for (const Call& c : std::get<1>(kv)->calls)
            if (c.kind == str("f") && s->functions.contains(c.key)) s->callers[c.key.u32()].insert(std::get<0>(kv).u32());
    return s;
}

// The function store and the corpus as the new comparison asks of them, through the old ports.
class OldLedger : public nv::Ledger {
  public:
    using Store = std::shared_ptr<engine::native_engine::FunctionStore>;
    explicit OldLedger(const Store& fs) : given_(fs) {}

    std::optional<std::string> sha(const std::filesystem::path& m) override {
        const auto s = base::corpus::sha(str(m.string()));
        return s ? std::optional<std::string>(s->utf8()) : std::nullopt;
    }
    std::optional<std::string> owner(const std::filesystem::path& m) override {
        const auto s = base::corpus::owner(str(m.string()));
        return s ? std::optional<std::string>(s->utf8()) : std::nullopt;
    }
    std::optional<Identity> identity(const std::filesystem::path& m) override {
        try {
            const auto id = verify::reconstruct::identity(str(m.string()));
            return Identity{std::get<0>(id).utf8(), std::get<1>(id).utf8()};
        } catch (const py::ValueError&) {
            return std::nullopt;
        } catch (const py::OSError&) {
            return std::nullopt;
        }
    }
    std::map<std::string, Known> best(const Identity& id) override {
        std::map<std::string, Known> out;
        for (const auto& kv : fs()->best(str(id.sha), str(id.profile)).items()) {
            const auto& row = std::get<1>(kv);
            Known k;
            k.klass = engine::native_engine::cell_str(row.klass).utf8();
            if (const auto e = engine::native_engine::cell_opt_str(row.engine)) k.engine = e->utf8();
            if (const auto n = engine::native_engine::cell_opt_str(row.name)) k.name = n->utf8();
            if (std::holds_alternative<str>(row.source)) k.source = std::get<str>(row.source).utf8();
            out[std::get<0>(kv).utf8()] = std::move(k);
        }
        return out;
    }
    std::map<std::string, std::string> bodies(const std::string& sha) override {
        std::map<std::string, std::string> out;
        for (const auto& kv : fs()->bodies(str(sha)).items()) out[std::get<0>(kv).utf8()] = std::get<0>(std::get<1>(kv)).utf8();
        return out;
    }
    void name_body(const std::string& digest, const std::string& name, const std::string& source) override {
        fs()->name_body(str(digest), str(name), str(source));
    }
    void file_carried(const Identity& id, const std::string& module_name, const std::filesystem::path& path, nv::Kind kind,
                      std::span<const std::string> addresses, const std::string& rev, std::span<const Carried> rows) override {
        fs()->add_module(str(id.sha), str(module_name), str(path.string()), str(std::string(nv::kind_name(kind))));
        py::list<str> addrs;
        for (const std::string& a : addresses) addrs.append(str(a));
        fs()->add_functions(str(id.sha), addrs);
        py::list<engine::native_engine::FunctionStore::RecordRow> out;
        for (const Carried& c : rows) {
            engine::native_engine::FunctionStore::RecordRow row;
            row.addr = str(c.address);
            row.name = c.name ? engine::native_engine::Cell(str(*c.name)) : engine::native_engine::Cell();
            row.source = engine::native_engine::Cell(str(c.source));
            row.sig = engine::native_engine::Cell(py::hashlib::sha1(str(c.source).encode("utf-8", "replace")).hexdigest());
            row.verdict = verify::verdicts::INCONCLUSIVE;
            row.reason = engine::native_engine::Cell(str(c.reason));
            out.append(row);
        }
        fs()->record_many(str(id.sha), ENGINE, engine::native_engine::Cell(str(rev)), str(id.profile), out);
    }

  private:
    // the store is opened when something asks for it (a comparison that only reads two files never makes one)
    Store& fs() {
        if (!fs_) fs_ = given_ ? given_ : store::functions::open();
        return fs_;
    }
    Store given_, fs_;
};

}  // namespace shim_detail

struct Pair {
    str from, to, from_name, to_name, status, level, how;
    double score = 0.0;
    int64_t size_from = 0, size_to = 0;
};
struct Counts {
    int64_t identical = 0, identical_bytes = 0, identical_text = 0, changed = 0, added = 0, removed = 0;
};
struct CodeBytes {
    int64_t to_bytes = 0, identical = 0, changed = 0, added = 0;
};
struct NameRow {
    str to, name, from;
    bool identical = false;
};
struct Reconstruction {
    str from, to, klass;
    std::optional<str> engine, name;
    str source;
};
using Store = std::shared_ptr<engine::native_engine::FunctionStore>;

// What compare() found.
class Comparison {
  public:
    std::shared_ptr<Side> a, b;
    double seconds = 0.0;
    std::vector<Pair> pairs;
    py::list<str> removed, added;

    explicit Comparison(std::shared_ptr<const nv::Comparison> impl) : impl_(std::move(impl)), seconds(impl_->seconds()) {
        a = shim_detail::from_new(impl_->from());
        b = shim_detail::from_new(impl_->to());
        for (const nv::Pair& p : impl_->pairs())
            pairs.push_back({str(p.from), str(p.to), str(p.from_name), str(p.to_name), str(p.identical ? "identical" : "changed"),
                             str(p.level ? std::string(nv::level_name(*p.level)) : std::string()), str(std::string(nv::how_name(p.how))), p.score,
                             p.size_from, p.size_to});
        for (std::size_t x : impl_->removed()) removed.append(str(impl_->from().function(x).key));
        for (std::size_t y : impl_->added()) added.append(str(impl_->to().function(y).key));
    }
    Counts counts() const {
        const nv::Counts c = impl_->counts();
        return {c.identical, c.identical_bytes, c.identical_text, c.changed, c.added, c.removed};
    }
    CodeBytes code() const {
        const nv::CodeBytes c = impl_->code();
        return {c.to_bytes, c.identical, c.changed, c.added};
    }
    std::vector<NameRow> names() const {
        std::vector<NameRow> out;
        for (const nv::NameRow& n : impl_->names()) out.push_back({str(n.to), str(n.name), str(n.from), n.identical});
        return out;
    }
    std::vector<Reconstruction> reconstructions(const Store& fs = nullptr) const {
        return ::cb::compat::guard([&] {
            shim_detail::OldLedger ledger(fs);
            std::vector<Reconstruction> out;
            for (const nv::Reconstruction& r : impl_->reconstructions(ledger))
                out.push_back({str(r.from), str(r.to), str(r.klass), r.engine ? std::optional<str>(str(*r.engine)) : std::nullopt,
                               r.name ? std::optional<str>(str(*r.name)) : std::nullopt, str(r.source)});
            return out;
        });
    }
    std::optional<int64_t> carry_names(const Store& fs = nullptr) const {
        return ::cb::compat::guard([&]() -> std::optional<int64_t> {
            shim_detail::OldLedger ledger(fs);
            const auto n = impl_->carry_names(ledger);
            return n ? std::optional<int64_t>(static_cast<int64_t>(*n)) : std::nullopt;
        });
    }
    int64_t carry(const Store& fs = nullptr) const {
        return ::cb::compat::guard([&] {
            shim_detail::OldLedger ledger(fs);
            return static_cast<int64_t>(impl_->carry(ledger));
        });
    }
    str report() const { return str(impl_->report()); }
    // the --json document as the old tree of values
    Value as_json(bool listings = true, const Store& fs = nullptr) const {
        return ::cb::compat::guard([&] {
            shim_detail::OldLedger ledger(fs);
            return to_value_(::nlohmann::ordered_json::parse(impl_->json(listings, &ledger)));
        });
    }

  private:
    static Value to_value_(const ::nlohmann::ordered_json& j) {
        if (j.is_null()) return Value();
        if (j.is_boolean()) return Value(j.get<bool>());
        if (j.is_number_integer()) return Value(j.get<int64_t>());
        if (j.is_number()) return Value(j.get<double>());
        if (j.is_string()) return Value(str(j.get<std::string>()));
        if (j.is_array()) {
            Value v = Value::list();
            for (const auto& x : j) v.push(to_value_(x));
            return v;
        }
        Value v = Value::dict();
        for (auto it = j.begin(); it != j.end(); ++it) v.set(str(it.key()), to_value_(it.value()));
        return v;
    }
    std::shared_ptr<const nv::Comparison> impl_;
};

// Read a module and fingerprint its functions. ValueError for a file whose kind is not compared.
inline std::shared_ptr<Side> load(const str& path) {
    return ::cb::compat::guard([&] { return shim_detail::from_new(nv::load(std::filesystem::path(path.utf8()))); });
}
// Match the functions of two builds of a module.
inline Comparison compare(const str& path_a, const str& path_b, double least = 0.6) {
    return ::cb::compat::guard([&] {
        return Comparison(std::make_shared<const nv::Comparison>(nv::compare(std::filesystem::path(path_a.utf8()), std::filesystem::path(path_b.utf8()), least)));
    });
}
inline double likeness(const Function& fa, const Function& fb) {
    if (fa.ids.shape == fb.ids.shape) return 1.0;
    nv::Function a, b;
    for (const str& s : fa.shape) a.shape.push_back(s.utf8());
    for (const str& s : fb.shape) b.shape.push_back(s.utf8());
    a.ids.shape = fa.ids.shape.utf8();
    b.ids.shape = fb.ids.shape.utf8();
    return nv::likeness(a, b);
}

// The functions several programs have in common: -> [(instructions, {program name: [keys]})]
using Where = std::vector<std::pair<str, py::list<str>>>;
inline std::vector<std::pair<int64_t, Where>> shared(const py::list<str>& paths, const str& level = str("text"), int64_t least = 8) {
    return ::cb::compat::guard([&] {
        std::vector<std::filesystem::path> ps;
        for (const str& p : paths) ps.emplace_back(p.utf8());
        std::vector<std::pair<int64_t, Where>> out;
        for (const nv::Shared& s : nv::shared(ps, nv::level_from_name(level.utf8()).value_or(nv::Level::Text), static_cast<std::size_t>(least))) {
            Where w;
            for (const auto& [name, keys] : s.where) {
                py::list<str> ks;
                for (const std::string& k : keys) ks.append(str(k));
                w.emplace_back(str(name), ks);
            }
            out.emplace_back(static_cast<int64_t>(s.instructions), w);
        }
        return out;
    });
}

// ---- a whole product
struct ModuleRow {
    str name, status;                        // compared | same file | new module | failed
    std::optional<int64_t> functions, bytes, identical, identical_code, changed, changed_code, added, removed;
    std::optional<double> seconds;
    std::optional<str> error;
};
inline Value to_value(const ModuleRow& r) {
    Value v = Value::dict();
    v.set("name", Value(r.name));
    v.set("status", Value(r.status));
    if (r.error) v.set("error", Value(*r.error));
    auto put = [&](const char* k, const std::optional<int64_t>& x) {
        if (x) v.set(k, Value(*x));
    };
    put("functions", r.functions);
    put("bytes", r.bytes);
    put("identical", r.identical);
    put("identical_code", r.identical_code);
    put("changed", r.changed);
    put("changed_code", r.changed_code);
    put("added", r.added);
    put("removed", r.removed);
    if (r.seconds) v.set("seconds", Value(*r.seconds));
    return v;
}
struct Product {
    str from, to;
    py::list<ModuleRow> modules;
    std::vector<std::pair<str, int64_t>> totals;
};
inline Value to_value(const Product& p) {
    Value v = Value::dict();
    v.set("from", Value(p.from));
    v.set("to", Value(p.to));
    Value mods = Value::list();
    for (const ModuleRow& r : p.modules) mods.push(to_value(r));
    v.set("modules", std::move(mods));
    Value t = Value::dict();
    for (const auto& kv : p.totals) t.set(kv.first, Value(kv.second));
    v.set("totals", std::move(t));
    return v;
}
// How much of product `new_` is already known from product `old`.
inline Product product(const str& old, const str& new_, int64_t sample = 0, std::optional<int64_t> jobs = std::nullopt, double least = 0.6,
                       const std::function<void(const str&)>& log = nullptr) {
    return ::cb::compat::guard([&] {
        const nv::Product n = nv::product(base::corpus::detail::instance(), old.utf8(), new_.utf8(), static_cast<std::size_t>(sample),
                                          jobs ? std::optional<int>(static_cast<int>(*jobs)) : std::nullopt, least,
                                          [&](const std::string& line) {
                                              if (log) log(str(line));
                                              else {
                                                  py::print(str(line));
                                                  std::fflush(stdout);
                                              }
                                          });
        Product out;
        out.from = str(n.from);
        out.to = str(n.to);
        for (const nv::ModuleRow& r : n.modules) {
            ModuleRow m;
            m.name = str(r.name);
            m.status = str(std::string(nv::status_name(r.status)));
            m.functions = r.functions;
            m.bytes = r.bytes;
            m.identical = r.identical;
            m.identical_code = r.identical_code;
            m.changed = r.changed;
            m.changed_code = r.changed_code;
            m.added = r.added;
            m.removed = r.removed;
            m.seconds = r.seconds;
            if (r.error) m.error = str(*r.error);
            out.modules.append(m);
        }
        for (const auto& [k, v] : n.totals) out.totals.emplace_back(str(k), v);
        return out;
    });
}

inline const char* DOC = ::cb::compare::versions::kUsage;

// python -m codebreaker versions ...
inline int64_t main(const py::list<str>& argv_in) {
    std::vector<std::string> args{"versions"};
    for (const str& a : argv_in) args.push_back(a.utf8());
    return ::cb::compat::guard([&]() -> int64_t {
        shim_detail::OldLedger ledger(nullptr);
        const ::cb::cli::Cli cli("codebreaker", {nv::command(base::corpus::detail::instance(), ledger)});
        return cli.run(args);
    });
}

}  // namespace codebreaker::compare::versions
