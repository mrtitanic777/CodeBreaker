// codebreaker/decompile/xenos.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/xenos.{hpp,cpp}; this header keeps the old py:: surface. Conversion and forwarding only.
// Nothing else in the old tree includes it; deleted with the old harness.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/xenos.hpp"
#include "py.hpp"

namespace codebreaker::decompile::xenos {

namespace detail {
namespace nx = ::cb::decompile::xenos;

inline std::uint32_t word(int64_t v) { return static_cast<std::uint32_t>(static_cast<std::uint64_t>(v) & 0xffffffffull); }
inline py::list<py::str> lines(const std::vector<std::string>& v) { return ::cb::compat::to_py(v); }
}  // namespace detail

// the usage line `python -m codebreaker.decompile.xenos` prints for -h
inline const py::str DOC = "xenos.py -- the Xbox 360 GPU's (Xenos) shader microcode, as instructions.";

inline const py::dict<int64_t, py::str> VECTOR = [] {
    py::dict<int64_t, py::str> d;
    for (std::uint32_t op = 0; op < 32; ++op) {
        if (!detail::nx::vector_name(op).empty()) d.set(op, ::cb::compat::to_py(detail::nx::vector_name(op)));
    }
    return d;
}();
// how many sources each vector opcode reads
inline const py::list<int64_t> VECTOR_SOURCES = [] {
    py::list<int64_t> l;
    for (std::uint32_t op = 0; op < 32; ++op) l.append(detail::nx::vector_sources(op));
    return l;
}();
inline const py::dict<int64_t, py::str> SCALAR = [] {
    py::dict<int64_t, py::str> d;
    for (std::uint32_t op = 0; op < 64; ++op) {
        if (!detail::nx::scalar_name(op).empty()) d.set(op, ::cb::compat::to_py(detail::nx::scalar_name(op)));
    }
    return d;
}();
inline const py::dict<int64_t, py::str> CONTROL = [] {
    py::dict<int64_t, py::str> d;
    for (std::uint32_t op = 0; op < 16; ++op) {
        if (!detail::nx::control_name(op).empty()) d.set(op, ::cb::compat::to_py(detail::nx::control_name(op)));
    }
    return d;
}();
inline const py::str CHANNELS_ = "xyzw";

// a pixel shader as fxc built it (/XuFormatRaw /Fh), and fxc's own listing of it
inline const py::list<int64_t> SAMPLE = [] {
    py::list<int64_t> l;
    for (std::uint32_t w : detail::nx::kSample) l.append(w);
    return l;
}();
inline const py::list<py::str> SAMPLE_LISTING = [] {
    py::list<py::str> l;
    for (std::string_view s : detail::nx::kSampleListing) l.append(::cb::compat::to_py(s));
    return l;
}();

inline py::str _mask(int64_t m) { return ::cb::compat::to_py(detail::nx::mask_text(detail::word(m))); }
inline py::str _swizzle(int64_t sw) { return ::cb::compat::to_py(detail::nx::swizzle_text(detail::word(sw))); }
inline py::str _register(int64_t selected, int64_t number) {
    return ::cb::compat::to_py(detail::nx::register_text(selected != 0, detail::word(number)));
}

// The words are 32-bit values (as the hardware's are).
inline py::list<py::str> alu(int64_t d0, int64_t d1, int64_t d2) {
    return detail::lines(detail::nx::alu(detail::word(d0), detail::word(d1), detail::word(d2)));
}
inline py::list<py::str> control(int64_t d0, int64_t d1, int64_t d2) {
    return detail::lines(detail::nx::control(detail::word(d0), detail::word(d1), detail::word(d2)));
}
inline py::list<py::str> disassemble(const py::list<int64_t>& dwords) {
    std::vector<std::uint32_t> words;
    for (int64_t w : dwords) words.push_back(detail::word(w));
    return detail::lines(detail::nx::disassemble(words));
}

inline int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0] == "-h" || argv[0] == "--help") {
        py::print(DOC);
        return 0;
    }
    py::list<int64_t> words;
    if (argv[0] == "--sample") {
        words = SAMPLE;
    } else {
        for (const py::str& x : argv) words.append(py::int_from_str(x, 16));
    }
    for (const py::str& line : disassemble(words)) py::print(line);
    if (argv[0] == "--sample") py::print(py::str("; the compiler's listing: ") + py::join(py::str(" | "), SAMPLE_LISTING));
    return 0;
}

}  // namespace codebreaker::decompile::xenos
