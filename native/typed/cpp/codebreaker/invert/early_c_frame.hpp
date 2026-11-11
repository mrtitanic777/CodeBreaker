// codebreaker/invert/early_c_frame.hpp -- SHIM over cb/invert/early_c_frame.hpp (docs/CPP_DESIGN.md 6.3); deleted with its
// last py:: consumer. Frame offsets are int64_t here as before; a negative one is outside the new model's domain (the
// compiler allocates upward from bp) and is an OverflowError. The str-keyed forms of the copy functions, the CASES table
// and the self-test of the Python module are gone: the cases are tests of the new module (early_c_frame_test.cpp).
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/invert/early_c_frame.hpp"

namespace codebreaker::invert::early_c_frame {

namespace nf = cb::invert::early_c_frame;

inline int64_t tsize(const py::str& t, int64_t count = 1) {
    return cb::compat::to_py(nf::type_size(cb::compat::from_py(t), cb::compat::int_from_py<std::uint32_t>(count)));
}

// temps  : list of type-strings, in creation order (e.g. ['int','long']).
// locals_: list of (name, type, count) in declaration order (count>=1; arrays >1).
// params : list of (name, type) in order, or None.
// returns (offsets: dict name->off, framesize, param_offsets: dict, temp_bytes).
inline std::tuple<py::dict<py::str, int64_t>, int64_t, py::dict<py::str, int64_t>, int64_t>
layout(const py::list<py::str>& temps, const py::list<std::tuple<py::str, py::str, int64_t>>& locals_,
       const std::optional<py::list<std::tuple<py::str, py::str>>>& params = std::nullopt) {
    return cb::compat::guard([&] {
        std::vector<nf::Local> locals;
        for (const auto& [name, type, count] : locals_) {
            locals.push_back({cb::compat::from_py(name), cb::compat::from_py(type), cb::compat::int_from_py<std::uint32_t>(count)});
        }
        std::vector<nf::Param> ps;
        if (params) {
            for (const auto& [name, type] : *params) ps.push_back({cb::compat::from_py(name), cb::compat::from_py(type)});
        }
        const nf::Layout lay = nf::layout(cb::compat::from_py(temps), locals, ps);
        return std::make_tuple(cb::compat::to_py(lay.local_offsets), cb::compat::to_py(lay.frame_size), cb::compat::to_py(lay.param_offsets),
                               cb::compat::to_py(lay.temp_bytes));
    });
}

namespace detail {
inline std::vector<nf::Copy> copies_of(const py::list<std::tuple<int64_t, int64_t>>& copies) {
    std::vector<nf::Copy> out;
    for (const auto& [dest, src] : copies) {
        out.push_back({cb::compat::int_from_py<nf::Offset>(dest), cb::compat::int_from_py<nf::Offset>(src)});
    }
    return out;
}
}  // namespace detail

// Resolve live copies into per-CHAIN endpoint swaps. Returns a list of (offA, offB) endpoint swaps.
inline py::list<std::tuple<int64_t, int64_t>> _chain_swaps(const py::list<std::tuple<int64_t, int64_t>>& copies) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& [a, b] : nf::chain_swaps(detail::copies_of(copies))) out.append(std::make_tuple<int64_t, int64_t>(a, b));
        return out;
    });
}

// FORWARD model of MC2's copy-reversal. Returns {declared_off: final_off}.
inline py::dict<int64_t, int64_t> apply_copy_swaps(const py::dict<int64_t, int64_t>& offsets,
                                                   const py::list<std::tuple<int64_t, int64_t>>& copies) {
    return cb::compat::guard([&] {
        std::vector<nf::Offset> keys;
        for (const auto& kv : offsets.items()) keys.push_back(cb::compat::int_from_py<nf::Offset>(std::get<0>(kv)));
        return cb::compat::to_py(nf::apply_copy_swaps(keys, detail::copies_of(copies)));
    });
}

// INVERSE: declaration order (list of offsets) to emit so the compiler's copy-chain endpoint swaps reproduce each var at
// its own offset.
inline py::list<int64_t> infer_decl_order(const py::list<int64_t>& offsets, const py::list<std::tuple<int64_t, int64_t>>& copies) {
    return cb::compat::guard([&] {
        std::vector<nf::Offset> offs;
        for (const int64_t o : offsets) offs.push_back(cb::compat::int_from_py<nf::Offset>(o));
        return cb::compat::to_py(nf::infer_decl_order(offs, detail::copies_of(copies)));
    });
}

// The C declarations of the referenced local offsets, in declaration order, with char fillers for the gaps.
// Returns (ordered_decls, names): ordered_decls = [(ctype, name, count), ...].
inline std::tuple<py::list<std::tuple<py::str, py::str, int64_t>>, py::list<py::str>>
emit_local_decls(const py::list<int64_t>& offsets, int64_t framesize, const std::optional<py::set<int64_t>>& longset = std::nullopt,
                 const std::optional<py::list<std::tuple<int64_t, int64_t>>>& copies = std::nullopt) {
    return cb::compat::guard([&] {
        std::vector<nf::Offset> offs;
        for (const int64_t o : offsets) offs.push_back(cb::compat::int_from_py<nf::Offset>(o));
        std::set<nf::Offset> longs;
        if (longset) {
            for (const int64_t o : *longset) longs.insert(cb::compat::int_from_py<nf::Offset>(o));
        }
        const std::vector<nf::Copy> cps = copies ? detail::copies_of(*copies) : std::vector<nf::Copy>{};
        py::list<std::tuple<py::str, py::str, int64_t>> decls;
        py::list<py::str> names;
        for (const nf::Decl& d : nf::emit_local_decls(offs, cb::compat::int_from_py<std::uint32_t>(framesize), longs, cps)) {
            decls.append(std::make_tuple(cb::compat::to_py(d.type), cb::compat::to_py(d.name), static_cast<int64_t>(d.count)));
            if (d.type != "char" || d.name.rfind("_p", 0) != 0) names.append(cb::compat::to_py(d.name));   // (fillers are not variables)
        }
        return std::make_tuple(decls, names);
    });
}

inline py::str fmt_decl(const py::str& t, const py::str& name, int64_t count) {
    return cb::compat::to_py(nf::format_decl({cb::compat::from_py(t), cb::compat::from_py(name), cb::compat::int_from_py<std::uint32_t>(count)}));
}

}  // namespace codebreaker::invert::early_c_frame
