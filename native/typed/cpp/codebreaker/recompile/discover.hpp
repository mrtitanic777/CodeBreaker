// codebreaker/recompile/discover.hpp -- SHIM over cb/recompile/discover.hpp (docs/CPP_DESIGN.md 6.3): the old functions in py:: types,
// the new discovery called. Wired to the old powerpc_image reader (the new module takes the image as data) and the old disassemble
// shim's thread disassembler; deleted with its last py:: consumer (recompile/powerpc, recompile/config, recompile/x360project,
// analyze/ppc_calls).
//
//     auto [funcs, sources, img] = discover::discover(path, true, extra);
//     funcs       {address: size, 0 when unknown} (now in address order)
//     sources     how many came from each place: pdata, entry, calls, pointers, constants, seed
//     img         the image (formats/powerpc_image.Image)
//
//     python -m codebreaker.recompile.discover <image or .xex> [--list]
//
// What the module is: every function of an Xbox 360 image, for the recompiler; see cb/recompile/discover.hpp.
#pragma once

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/discover.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "py.hpp"

namespace codebreaker::recompile::discover {

inline constexpr int64_t WORD = 4;

namespace shim_ {

namespace cbd = cb::recompile::discover;

// What discovery reads of the old image reader's Image.
inline cbd::Image image_of(const formats::powerpc_image::Image& img) {
    cbd::Image out;
    out.code_begin = static_cast<std::uint32_t>(std::get<0>(img.code));
    out.code_end = static_cast<std::uint32_t>(std::get<1>(img.code));
    out.entry = static_cast<std::uint32_t>(img.entry);
    out.code = cb::compat::from_py(img.bytes_at(std::get<0>(img.code), std::get<1>(img.code) - std::get<0>(img.code)));
    for (const auto& s : img.sections) {
        out.sections.push_back({cb::compat::from_py(std::get<0>(s)), static_cast<std::uint32_t>(std::get<1>(s)), static_cast<std::uint32_t>(std::get<3>(s)),
                                cb::compat::from_py(img.bytes_at(std::get<1>(s), std::get<2>(s)))});
    }
    for (const auto& kv : img.extents.items()) out.extents[static_cast<std::uint32_t>(std::get<0>(kv))] = static_cast<std::uint32_t>(std::get<0>(std::get<1>(kv)));
    for (const auto& kv : img.imports.items()) out.imports.insert(static_cast<std::uint32_t>(std::get<0>(kv)));
    return out;
}

inline cbd::Functions functions_of(const py::dict<int64_t, int64_t>& known) {
    cbd::Functions out;
    for (const auto& kv : known.items()) out[static_cast<std::uint32_t>(std::get<0>(kv))] = static_cast<std::uint32_t>(std::get<1>(kv));
    return out;
}

inline py::set<int64_t> set_of(const std::vector<std::uint32_t>& addresses) {
    py::set<int64_t> out;
    for (const std::uint32_t a : addresses) out.add(a);
    return out;
}

}  // namespace shim_

// the instructions a function's first plausibly is (_PROLOGUE)
inline const py::set<py::str>& _PROLOGUE() {
    static const py::set<py::str> s = {py::str("mflr"), py::str("stwu"), py::str("stw"), py::str("std"), py::str("mr"), py::str("li"), py::str("lis"),
                                       py::str("or"), py::str("addi"), py::str("stmw"), py::str("b"), py::str("clrlwi"), py::str("rlwinm"),
                                       py::str("cmpwi"), py::str("cmplwi"), py::str("lwz"), py::str("add")};
    return s;
}

// bclr, bcctr, rfid
inline const py::set<int64_t>& _XL_ENDERS() {
    static const py::set<int64_t> s = {16, 528, 18};
    return s;
}

// the image of a path: a .xex's basefile, or the PowerPC PE itself
inline py::ref<formats::powerpc_image::Image> _image(const py::str& path) {
    return formats::powerpc_image::read(formats::xex::is_xex(path) ? formats::xex::basefile(path) : path);
}

// A function's first instruction plausibly stands at `a`: it is a prologue-like instruction, or any instruction (not a data word) right
// after the end of a function (blr, bctr, a plain branch, padding).
inline bool _plausible_start(const py::bytes& code, int64_t lo, int64_t a) {
    return cb::compat::guard([&] {
        cb::recompile::discover::Image image;
        image.code = cb::compat::from_py(code);
        image.code_begin = static_cast<std::uint32_t>(lo);
        image.code_end = static_cast<std::uint32_t>(lo + static_cast<int64_t>(image.code.size()));
        return cb::recompile::discover::plausible_start(disassemble::shim_::mine(), image, static_cast<std::uint32_t>(a));
    });
}

// Function starts found as pointers in the data sections (see the module's notes).
inline py::set<int64_t> from_pointers(const formats::powerpc_image::Image& img, const py::dict<int64_t, int64_t>& known) {
    return cb::compat::guard([&] {
        return shim_::set_of(cb::recompile::discover::from_pointers(shim_::image_of(img), disassemble::shim_::mine(), shim_::functions_of(known)));
    });
}

// Function starts found as addresses built in the code: `lis rD, hi` and then `addi rX, rD, lo` / `ori rX, rD, lo`.
inline py::set<int64_t> from_constants(const formats::powerpc_image::Image& img, const py::dict<int64_t, int64_t>& known) {
    return cb::compat::guard([&] {
        return shim_::set_of(cb::recompile::discover::from_constants(shim_::image_of(img), disassemble::shim_::mine(), shim_::functions_of(known)));
    });
}

using Result = std::tuple<py::dict<int64_t, int64_t>, py::dict<py::str, int64_t>, py::ref<formats::powerpc_image::Image>>;

inline Result discover(const py::str& path, bool pointer_scan = true, const std::optional<py::list<int64_t>>& extra = std::nullopt) {
    return cb::compat::guard([&]() -> Result {
        py::ref<formats::powerpc_image::Image> img = _image(path);
        cb::recompile::discover::Options options;
        options.pointer_scan = pointer_scan;
        if (extra) {
            for (const int64_t a : *extra) options.seeds.push_back(static_cast<std::uint32_t>(a));
        }
        const cb::recompile::discover::Discovery found =
            cb::recompile::discover::discover(shim_::image_of(*img), disassemble::shim_::mine(), options);
        py::dict<int64_t, int64_t> funcs;
        for (const auto& [address, size] : found.functions) funcs.set(address, size);
        py::dict<py::str, int64_t> sources;
        const cb::recompile::discover::Sources& s = found.sources;
        sources.set(py::str("pdata"), static_cast<int64_t>(s.pdata));
        sources.set(py::str("entry"), static_cast<int64_t>(s.entry));
        sources.set(py::str("calls"), static_cast<int64_t>(s.calls));
        sources.set(py::str("pointers"), static_cast<int64_t>(s.pointers));
        sources.set(py::str("constants"), static_cast<int64_t>(s.constants));
        sources.set(py::str("seed"), static_cast<int64_t>(s.seed));
        return Result(funcs, sources, img);
    });
}

// main(argv) is the Python's main: `argv` the arguments after the program's name. The command prints through cb::out (to stdout), so `out`
// is not used.
inline int64_t main(const py::list<py::str>& argv, std::ostream& /*out*/ = std::cout) {
    return cb::compat::guard([&]() -> int64_t {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        const cb::recompile::discover::ImageReader reader = [](const std::filesystem::path& path) {
            return shim_::image_of(*_image(cb::compat::to_py(cb::fs::to_text(path))));
        };
        return cb::recompile::discover::main(reader, disassemble::shim_::vector_decoder(), args);
    });
}

}  // namespace codebreaker::recompile::discover
