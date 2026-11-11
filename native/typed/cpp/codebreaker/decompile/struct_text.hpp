// codebreaker/decompile/struct_text.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/struct_text.{hpp,cpp}; this header keeps the old py:: surface for decompile/c_text, which
// hands the module its three collaborators as py:: callbacks. Conversion and forwarding only; deleted with c_text's
// refactor.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/struct_text.hpp"
#include "py.hpp"

namespace codebreaker::decompile::struct_text {

namespace ns = ::cb::decompile::struct_text;

// One root of a recovered structure: ("arg", va, index) or ("this", va) (no index).
struct Root {
    py::str kind;
    int64_t va = 0;
    std::optional<int64_t> index;
};

// A structure analyze/structures.py recovered: its name and the places that are known to point at it.
struct Structure {
    py::str name;
    py::list<Root> roots;
};

// One access `*(base stars)(param + offset)` through a parameter (cback.struct_accesses).
struct Access {
    py::str base;
    py::str stars;
    int64_t offset = 0;
};

// (type as written, size)
using Field = std::tuple<py::str, int64_t>;
// {offset: (type, size)}
using Fields = py::dict<int64_t, Field>;
// (structure name, its fields)
using Planned = std::tuple<py::str, Fields>;
// {function: {parameter: (structure, {offset: (type, size)})}}
using Plan = py::dict<py::str, py::dict<py::str, Planned>>;

struct Analysis {
    // image.load(module_path) then structures.recover(...); may throw (the plan is then empty).
    std::function<py::list<Structure>(const py::str& module_path)> recover;
    // cback.parameter_names(docs[fn])
    std::function<py::list<py::str>(const py::str& fn)> parameter_names;
    // cback.struct_accesses(docs[fn], param)
    std::function<py::list<Access>(const py::str& fn, const py::str& param)> struct_accesses;
};

// (the field's type as written, its size) for an access `*(base stars)...`; (None, 0) when not a field.
inline std::tuple<std::optional<py::str>, int64_t> kind(const py::str& base, const py::str& stars) {
    auto k = ns::kind(::cb::compat::from_py(base), static_cast<std::size_t>(py::len(stars)));
    if (!k) return {std::nullopt, 0};
    return {::cb::compat::to_py(k->type), k->size};
}

inline Plan plan(const py::str& module_path, const py::set<py::str>& docs, const py::dict<int64_t, py::str>& by_va,
                 const py::dict<py::str, py::str>& declared, const Analysis& analysis) {
    std::set<std::string> new_docs = ::cb::compat::from_py(docs);
    std::map<std::uint64_t, std::string> new_by_va;
    for (const auto& kv : by_va.items()) new_by_va[static_cast<std::uint64_t>(std::get<0>(kv))] = ::cb::compat::from_py(std::get<1>(kv));
    std::map<std::string, std::string> new_declared;
    for (const auto& kv : declared.items()) new_declared[::cb::compat::from_py(std::get<0>(kv))] = ::cb::compat::from_py(std::get<1>(kv));

    ns::Analysis a;
    a.recover = [&](std::string_view path) -> std::optional<std::vector<ns::Structure>> {
        py::list<Structure> found;
        try {   // the old callback may throw anything: the plan is then empty, as it was
            found = analysis.recover(::cb::compat::to_py(path));
        } catch (const std::exception&) {
            return std::nullopt;
        }
        std::vector<ns::Structure> out;
        for (const Structure& s : found) {
            ns::Structure n;
            n.name = ::cb::compat::from_py(s.name);
            for (const Root& r : s.roots) {
                ns::Root nr;
                nr.kind = r.kind == "arg" ? ns::RootKind::Arg : r.kind == "this" ? ns::RootKind::This : ns::RootKind::Other;
                nr.va = static_cast<std::uint64_t>(r.va);
                nr.index = r.index;
                n.roots.push_back(nr);
            }
            out.push_back(std::move(n));
        }
        return out;
    };
    a.parameter_names = [&](const std::string& fn) { return ::cb::compat::from_py(analysis.parameter_names(::cb::compat::to_py(fn))); };
    a.struct_accesses = [&](const std::string& fn, const std::string& param) {
        std::vector<ns::Access> out;
        for (const Access& x : analysis.struct_accesses(::cb::compat::to_py(fn), ::cb::compat::to_py(param))) {
            out.push_back({::cb::compat::from_py(x.base), static_cast<std::size_t>(py::len(x.stars)), x.offset});
        }
        return out;
    };

    ns::Plan p = ns::plan(::cb::compat::from_py(module_path), new_docs, new_by_va, new_declared, a);
    Plan out;
    for (const auto& [fn, by_param] : p) {
        py::dict<py::str, Planned> params;
        for (const auto& [param, planned] : by_param) {
            Fields fields;
            for (const auto& [off, f] : planned.fields) fields.set(off, Field(::cb::compat::to_py(f.type), f.size));
            params.set(::cb::compat::to_py(param), Planned(::cb::compat::to_py(planned.structure), fields));
        }
        out.set(::cb::compat::to_py(fn), params);
    }
    return out;
}

// The header's declarations of the structures `used`.
inline py::str declarations(const Plan& planned, const py::set<py::str>& used) {
    ns::Plan p;
    for (const auto& fi : planned.items()) {
        for (const auto& pi : std::get<1>(fi).items()) {
            const Planned& pl = std::get<1>(pi);
            ns::PlannedParam np;
            np.structure = ::cb::compat::from_py(std::get<0>(pl));
            for (const auto& fl : std::get<1>(pl).items()) {
                np.fields[std::get<0>(fl)] = {::cb::compat::from_py(std::get<0>(std::get<1>(fl))),
                                              static_cast<std::uint32_t>(std::get<1>(std::get<1>(fl)))};
            }
            p[::cb::compat::from_py(std::get<0>(fi))][::cb::compat::from_py(std::get<0>(pi))] = std::move(np);
        }
    }
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(ns::declarations(p, ::cb::compat::from_py(used))); });
}

}  // namespace codebreaker::decompile::struct_text
