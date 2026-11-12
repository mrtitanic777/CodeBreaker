// codebreaker/recompile/config.hpp -- COMPAT SHIM over cb/recompile/config.hpp (docs/CPP_DESIGN.md 6.3): the old functions in py::
// types, the new configuration called. Wired to the old powerpc_image reader and the old disassemble shim's thread disassembler (the
// new module takes the image as data); deleted with its last py:: consumer (recompile/x360project).
//
//     py::str text = config::make(xex_path, "ppc", "switch_tables.toml", std::nullopt);
//
// What the module is: the XenonRecomp configuration of a title, from what CodeBreaker finds in it; see cb/recompile/config.hpp.
// What is different on purpose: the switch tables of a file are read in the order the file gives them (as before) but an address given
// twice keeps the first place and the last labels; a TOML address past 32 bits is a TOMLDecodeError here.
#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/config.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "py.hpp"

namespace codebreaker::recompile::config {

// tomllib.TOMLDecodeError (a ValueError)
struct TOMLDecodeError : py::ValueError {
    TOMLDecodeError() : py::ValueError() {}
    explicit TOMLDecodeError(const std::string& m) : py::ValueError(m) {}
    const char* type() const override { return "TOMLDecodeError"; }
};

// The registers kept as locals of each function (the runtime is built with the same settings: x360.cmake reads them back from the
// generated ppc_config.h).
inline const py::list<std::tuple<py::str, bool>> REGISTER_SETTINGS = [] {
    py::list<std::tuple<py::str, bool>> out;
    for (const auto& s : cb::recompile::config::kRegisterSettings) out.append(std::make_tuple(cb::compat::to_py(s.key), s.value));
    return out;
}();

// {table base (the bctr): [case labels]}
using SwitchTables = py::dict<int64_t, py::list<int64_t>>;

namespace shim_ {

namespace cbc = cb::recompile::config;

inline cbc::Title title_of(const formats::powerpc_image::Image& img) {
    cbc::Title title;
    title.image = discover::shim_::image_of(img);
    if (img.run_names)
        for (const auto& kv : img.run_names->items()) title.run_names[static_cast<std::uint32_t>(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
    return title;
}

inline cbc::SwitchTables tables_of(const std::optional<SwitchTables>& switches) {
    cbc::SwitchTables out;
    if (!switches) return out;
    for (const auto& kv : switches->items()) {
        std::vector<std::uint32_t> labels;
        for (const int64_t l : std::get<1>(kv)) labels.push_back(static_cast<std::uint32_t>(l));
        out.insert_or_assign(static_cast<std::uint32_t>(std::get<0>(kv)), std::move(labels));
    }
    return out;
}

inline py::dict<py::str, int64_t> addresses_of(const cbc::Addresses& a) {
    py::dict<py::str, int64_t> out;
    for (const auto& [key, address] : a) out.set(cb::compat::to_py(key), address);
    return out;
}

// The new reader's ParseError is tomllib's TOMLDecodeError here.
template <class F>
auto toml_guard(F&& f) -> decltype(f()) {
    return cb::compat::guard([&]() -> decltype(f()) {
        try {
            return f();
        } catch (const cb::ParseError& e) {
            throw TOMLDecodeError(e.what());
        }
    });
}

}  // namespace shim_

// {config key: address} of the save/restore runs the image has.
inline py::dict<py::str, int64_t> helpers(const formats::powerpc_image::Image& img) {
    return cb::compat::guard([&] { return shim_::addresses_of(cb::recompile::config::helpers(shim_::title_of(img), disassemble::shim_::mine())); });
}

// The first word after the zero padding (or blr) that comes before `a`: a function's start.
inline int64_t _start_before(const py::bytes& code, int64_t lo, int64_t a) {
    return cb::compat::guard([&] {
        return static_cast<int64_t>(cb::recompile::config::start_before(cb::compat::from_py(code), static_cast<std::uint32_t>(lo), static_cast<std::uint32_t>(a)));
    });
}

// The CRT's setjmp and longjmp (the Xbox 360 jmp_buf: r1 at +0x90, CR at +0x130, LR at +0x134).
inline py::dict<py::str, int64_t> jump_buffers(const formats::powerpc_image::Image& img, const py::bytes& /*code*/) {
    return cb::compat::guard([&] { return shim_::addresses_of(cb::recompile::config::jump_buffers(shim_::title_of(img))); });
}

// {table base (the bctr): [case labels]} from a switch-table file (XenonAnalyse's format).
inline SwitchTables read_switch_tables(const py::str& path) {
    return shim_::toml_guard([&] {
        SwitchTables out;
        for (const auto& [base, labels] : cb::recompile::config::read_switch_tables(cb::fs::from_text(cb::compat::from_py(path)))) {
            py::list<int64_t> ls;
            for (const std::uint32_t l : labels) ls.append(l);
            out.set(base, ls);
        }
        return out;
    });
}

// The title's switch-table file: the finder's tables (it walks back from every bctr), with XenonAnalyse's (`analysed`, its file's
// text) added for any bctr the finder did not resolve. -> (the finder's count, XenonAnalyse's added).
inline std::tuple<int64_t, int64_t> write_switch_tables(const py::str& path, const py::str& xex_path, const std::optional<py::str>& analysed = std::nullopt) {
    return shim_::toml_guard([&] {
        const py::ref<formats::powerpc_image::Image> image = discover::_image(xex_path);
        cb::recompile::switches::CodeImage code;
        code.base = static_cast<std::uint32_t>(image->base);
        code.code_begin = static_cast<std::uint32_t>(std::get<0>(image->code));
        code.code_end = static_cast<std::uint32_t>(std::get<1>(image->code));
        code.data = cb::compat::from_py(image->bytes_at(image->base, std::get<1>(image->code) - image->base));
        for (const auto& [start, info] : image->extents.items())
            code.functions.push_back({static_cast<std::uint32_t>(start), static_cast<std::uint32_t>(start + std::get<0>(info))});
        std::sort(code.functions.begin(), code.functions.end(), [](const auto& a, const auto& b) { return a.begin < b.begin; });
        const auto written = cb::recompile::config::write_switch_tables(
            cb::fs::from_text(cb::compat::from_py(path)), code, analysed ? std::optional<std::string>(cb::compat::from_py(*analysed)) : std::nullopt);
        return std::make_tuple(static_cast<int64_t>(written.found), static_cast<int64_t>(written.added));
    });
}

// The address after the first blr, bctr or plain branch at or after `a`.
inline int64_t _end_of_run(const py::bytes& code, int64_t lo, int64_t a) {
    return cb::compat::guard([&] {
        return static_cast<int64_t>(cb::recompile::config::end_of_run(cb::compat::from_py(code), static_cast<std::uint32_t>(lo), static_cast<std::uint32_t>(a)));
    });
}

// [(address, size)] of the functions .pdata leaves out, sized to the next known start.
inline py::list<std::tuple<int64_t, int64_t>> extra_functions(const py::str& xex_path, const formats::powerpc_image::Image& img,
                                                              const std::optional<SwitchTables>& switches = std::nullopt) {
    return cb::compat::guard([&] {
        const cb::recompile::config::Title title = shim_::title_of(img);
        const auto found = cb::recompile::discover::discover(title.image, disassemble::shim_::mine());
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& f : cb::recompile::config::extra_functions(title, found.functions, disassemble::shim_::mine(), shim_::tables_of(switches)))
            out.append(std::make_tuple(static_cast<int64_t>(f.address), static_cast<int64_t>(f.size)));
        (void)xex_path;   // (the image is given: the Python read it again by path)
        return out;
    });
}

// The configuration's text. `file_path` is the .xex as the configuration names it (default: xex_path); `hooks` is TOML text appended
// as it is (midasm_hook tables).
inline py::str make(const py::str& xex_path, const py::str& out_dir = py::str("ppc"), const py::str& switch_tables = py::str("switch_tables.toml"),
                    const std::optional<py::str>& hooks = std::nullopt, const std::optional<py::str>& file_path = std::nullopt) {
    return shim_::toml_guard([&] {
        const py::ref<formats::powerpc_image::Image> image = discover::_image(xex_path);
        cb::recompile::config::MakeOptions options;
        options.out_dir = cb::compat::from_py(out_dir);
        options.switch_tables = cb::compat::from_py(switch_tables);
        if (hooks && !hooks->empty()) options.hooks = cb::compat::from_py(*hooks);
        if (file_path && !file_path->empty()) options.file_path = cb::compat::from_py(*file_path);
        return cb::compat::to_py(
            cb::recompile::config::make(shim_::title_of(*image), disassemble::shim_::mine(), cb::fs::from_text(cb::compat::from_py(xex_path)), options));
    });
}

}  // namespace codebreaker::recompile::config
