// codebreaker/decompile/vmx128.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/vmx128.{hpp,cpp}; this header keeps the old py:: surface for what still calls it: recompile/disassemble,
// the Python tests through the exe (`main`). Conversion and forwarding only; deleted with its last py:: consumer.
//
//     vmx128::main({"0x102320C3", ...})        decode instruction words
//     vmx128::main({"--coverage", file})       vector decode coverage over .text
//     vmx128::main({"--sleigh", out})          write the SLEIGH (vmx128.sinc)
#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/vmx128.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"   // COMPAT(py): remove when formats/powerpc_image and formats/xex are refactored (--coverage)
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::vmx128 {

namespace nv = ::cb::decompile::vmx128;

namespace detail {
inline py::str form_name(nv::Form f) {
    static const char* const names[] = {"load", "store", "sldoi", "abc", "madd", "perm", "simm", "bimm", "permwi", "d3d", "rlimi", "b", "cmp"};
    return py::str(names[static_cast<int>(f)]);
}
}  // namespace detail

// (mnemonic, pattern, mask, form, semantics): the 77 rows
using Row = std::tuple<py::str, int64_t, int64_t, py::str, py::str>;
// (mnemonic, extended opcode, form, the user operation the SLEIGH calls)
using X31Row = std::tuple<py::str, int64_t, py::str, py::str>;

inline py::list<Row> rows_list() {
    py::list<Row> out;
    for (const nv::Row& r : nv::rows()) {
        out.append(Row(::cb::compat::to_py(r.mnemonic), r.pattern, r.mask, detail::form_name(r.form), ::cb::compat::to_py(r.semantics)));
    }
    return out;
}
inline py::list<X31Row> x31_list() {
    py::list<X31Row> out;
    for (const nv::X31Row& r : nv::x31_rows()) {
        out.append(X31Row(::cb::compat::to_py(r.mnemonic), r.extended_opcode, detail::form_name(r.form), ::cb::compat::to_py(r.operation)));
    }
    return out;
}
inline const py::list<Row> ROWS = rows_list();
inline const py::list<X31Row> X31 = x31_list();

// (mnemonic, operand text) for a VMX128 instruction word (or one of the X31 rows), else None.
inline std::optional<std::tuple<py::str, py::str>> decode(int64_t word) {
    const auto d = nv::decode(static_cast<std::uint32_t>(word & 0xFFFFFFFF));
    if (!d) return std::nullopt;
    return std::make_tuple(::cb::compat::to_py(d->mnemonic), ::cb::compat::to_py(d->operands));
}

inline std::optional<py::str> text(int64_t word) {
    const auto t = nv::text(static_cast<std::uint32_t>(word & 0xFFFFFFFF));
    if (!t) return std::nullopt;
    return ::cb::compat::to_py(*t);
}

// {'vector words', 'vmx128', 'other'} over a PowerPC image's code section
inline py::dict<py::str, int64_t> coverage(const formats::powerpc_image::Image& img) {
    std::vector<std::uint32_t> words;
    for (int64_t va = std::get<0>(img.code); va < std::get<1>(img.code); va += 4) {
        if (const auto w = img.word(va)) words.push_back(static_cast<std::uint32_t>(*w));
    }
    const nv::Coverage c = nv::coverage(words);
    py::dict<py::str, int64_t> out;
    out.set(py::str("vector words"), static_cast<int64_t>(c.vector_words));
    out.set(py::str("vmx128"), static_cast<int64_t>(c.vmx128));
    out.set(py::str("other"), static_cast<int64_t>(c.other));
    return out;
}

// The text of languages/src/ppc/vmx128.sinc; `declared`: the user operations the other PowerPC .sinc files already define.
inline py::str sleigh(const py::set<py::str>& declared = py::set<py::str>()) {
    std::set<std::string> d;
    for (const py::str& s : declared) d.insert(s.utf8());
    return ::cb::compat::to_py(nv::sleigh(d));
}

inline int64_t main(const py::list<py::str>& argv) {
    return ::cb::compat::guard([&]() -> int64_t {
        if (argv.empty() || argv[0] == "-h" || argv[0] == "--help") {
            py::print(py::str("VMX128 -- the Xbox 360 (Xenon) 128-register extension of AltiVec."));
            return 0;
        }
        if (argv[0] == "--sleigh") {
            if (py::len(argv) < 2) {
                py::print(py::str("usage: --sleigh <out.sinc>"));
                return 2;
            }
            const std::size_t rows = nv::write_sleigh_file(std::filesystem::path(argv[1].utf8()));
            py::print(py::percent("wrote %s (%d rows)", argv[1], static_cast<int64_t>(rows)));
            return 0;
        }
        if (argv[0] == "--coverage") {
            py::str path = argv[1].lower().endswith(py::str(".xex")) ? formats::xex::basefile(argv[1]) : argv[1];
            py::ref<formats::powerpc_image::Image> img = formats::powerpc_image::read(path);
            const py::dict<py::str, int64_t> c = coverage(*img);
            for (const char* k : {"vector words", "vmx128", "other"}) py::print(py::percent("  %-13s %d", py::str(k), c[py::str(k)]));
            return 0;
        }
        for (const py::str& tok : argv) {
            const int64_t w = py::int_parse(tok, 16);
            const auto t = text(w);
            py::print(py::percent("0x%08X  %s", w, t ? *t : py::str("(not VMX128)")));
        }
        return 0;
    });
}

}  // namespace codebreaker::decompile::vmx128
