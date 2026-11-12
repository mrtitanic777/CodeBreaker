// codebreaker/symbols/debug_build.hpp -- SHIM over cb/symbols/debug_build.hpp (docs/CPP_DESIGN.md 6.3): real internal function names for a shipped NE
// module, matched function by function against a debug build of the same version whose linker .MAP names every function. The old types in (the old
// Seg, file names, py::dicts), the new matcher called, the old types out. Deleted with its last py:: consumer (debug_build_estimate).
//
//     auto index = build_debug_index("KRNL386.EXE", "KRNL386.MAP", ordinals);
//     auto [names, counts] = match_module(retail_exe, index, ordinals);
//
// Differences from the old port that the shim cannot hide:
//   * The signature of an instruction is made by the new masking code, which reads the branch mnemonics as whole words as the Python's alternation
//     did (`jne` is not `je` plus junk); a name or text the Python's pattern read across a line end is not read so.
//   * A names.json that is not one is a ParseError, which reaches old callers as the ValueError they caught, not as a JSONDecodeError.
//   * `_BR`, the pattern, is not offered.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/debug_build.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"
#include "py.hpp"

namespace py {
template <> struct hasher<py::list<py::str>> {
    size_t operator()(const py::list<py::str>& l) const {
        size_t h = 0x345678;
        for (const py::str& s : l) h = h * 1000003 ^ hasher<py::str>()(s);
        return h;
    }
};
}  // namespace py

namespace codebreaker::symbols::debug_build {

namespace cbd = cb::symbols::debug_build;

inline constexpr int64_t MIN_LEN = 5;   // functions under 5 instructions share skeletons -> collision-prone

using Sig = py::list<py::str>;

// json.JSONDecodeError (a ValueError that names itself so)
struct JSONDecodeError : py::ValueError {
    explicit JSONDecodeError(const std::string& m) : py::ValueError(m) {}
    const char* type() const override { return "JSONDecodeError"; }
};

namespace shim_ {

inline Sig sig_of(const cbd::Signature& s) { return cb::compat::to_py(s); }
inline cbd::Signature new_sig(const Sig& s) { return cb::compat::from_py(s); }

inline cb::source::ne_asm::segment::SharedOrdinals ordinals_of(const py::dict<py::str, py::str>& d) {
    auto out = std::make_shared<cb::source::ne_asm::segment::Ordinals>();
    for (const auto& kv : d.items()) (*out)[cb::compat::from_py(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
    return out;
}

inline py::dict<py::str, py::str> old_names(const cbd::Names& names) {
    py::dict<py::str, py::str> out;
    for (const auto& [key, name] : names) out.set(cb::compat::to_py(key.text()), cb::compat::to_py(name));
    return out;
}

}  // namespace shim_

// Offset-invariant signature of the function at `start`: one entry per instruction, OFFSET-dependent operands masked, immediates kept.
inline Sig norm_sig(const source::ne_asm::segment::Seg& sg, int64_t start) {
    return cb::compat::guard([&] { return shim_::sig_of(cbd::signature(sg.inner(), cb::compat::int_from_py<uint32_t>(start))); });
}

// {signature -> NAME} for every uniquely-signatured named function in a debug build (its .MAP names them; ambiguous signatures are dropped).
inline py::dict<Sig, py::str> build_debug_index(const py::str& debug_exe, const py::str& debug_map, const py::dict<py::str, py::str>& ordinals) {
    return cb::compat::guard([&] {
        const auto module = cb::source::ne_asm::fixups::open_module(cb::fs::from_text(cb::compat::from_py(debug_exe)));
        const std::string map = cb::fs::read_text(cb::fs::from_text(cb::compat::from_py(debug_map)));
        py::dict<Sig, py::str> out;
        for (const auto& [sig, name] : cbd::build_index(module, map, shim_::ordinals_of(ordinals))) out.set(shim_::sig_of(sig), cb::compat::to_py(name));
        return out;
    });
}

// ({"seg:off" -> NAME}: the exactly-matching real names for the anonymous internal functions of `retail_exe`, with all three false-positive guards
// applied, (agree, disagree) over the known exports).
inline std::tuple<py::dict<py::str, py::str>, std::tuple<int64_t, int64_t>> match_module(const py::str& retail_exe, const py::dict<Sig, py::str>& sig2name,
                                                                                          const py::dict<py::str, py::str>& ordinals, int64_t min_len = MIN_LEN) {
    return cb::compat::guard([&] {
        const auto module = cb::source::ne_asm::fixups::open_module(cb::fs::from_text(cb::compat::from_py(retail_exe)));
        cbd::SignatureIndex index;
        for (const auto& kv : sig2name.items()) index[shim_::new_sig(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
        const cbd::Match found = cbd::match_module(module, index, shim_::ordinals_of(ordinals), cb::compat::int_from_py<std::size_t>(min_len));
        return std::make_tuple(shim_::old_names(found.names), std::make_tuple(static_cast<int64_t>(found.agree), static_cast<int64_t>(found.disagree)));
    });
}

// names.json ("seg:off" -> NAME)  ->  {seg:int -> {off:int -> NAME}} for trace.Seg(name_override=...).
inline py::dict<int64_t, py::dict<int64_t, py::str>> load_override(const py::str& names_json) {
    return cb::compat::guard([&] {
        py::dict<int64_t, py::dict<int64_t, py::str>> out;
        for (const auto& [segment, offsets] : cbd::parse_overrides(cb::fs::read_text(cb::fs::from_text(cb::compat::from_py(names_json))))) {
            py::dict<int64_t, py::str> inner;
            for (const auto& [offset, name] : offsets) inner.set(int64_t{offset}, cb::compat::to_py(name));
            out.set(static_cast<int64_t>(segment), inner);
        }
        return out;
    });
}

// End-to-end: build the debug index, match, optionally write names.json.
inline std::tuple<py::dict<py::str, py::str>, std::tuple<int64_t, int64_t>> recover(const py::str& retail_exe, const py::str& debug_exe, const py::str& debug_map,
                                                                                    const py::dict<py::str, py::str>& ordinals,
                                                                                    const std::optional<py::str>& out_json = std::nullopt) {
    return cb::compat::guard([&] {
        const auto retail = cb::source::ne_asm::fixups::open_module(cb::fs::from_text(cb::compat::from_py(retail_exe)));
        const auto debug = cb::source::ne_asm::fixups::open_module(cb::fs::from_text(cb::compat::from_py(debug_exe)));
        const cbd::Match found = cbd::recover(retail, debug, cb::fs::read_text(cb::fs::from_text(cb::compat::from_py(debug_map))), shim_::ordinals_of(ordinals));
        if (out_json) {
            const auto path = cb::fs::from_text(cb::compat::from_py(*out_json));
            cb::fs::make_dirs(path.parent_path());
            cb::fs::write_text(path, cbd::to_json(found.names));
        }
        return std::make_tuple(shim_::old_names(found.names), std::make_tuple(static_cast<int64_t>(found.agree), static_cast<int64_t>(found.disagree)));
    });
}

}  // namespace codebreaker::symbols::debug_build
