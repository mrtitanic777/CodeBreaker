// codebreaker/recompile/powerpc.hpp -- COMPAT SHIM over cb/recompile/powerpc.hpp (docs/CPP_DESIGN.md 6.3): the old functions in
// py:: types, the new recompiler called. Wired to the old powerpc_image reader and vmx128 decoder (the new module takes the image
// as data and the decoder as a callback), and to settings.json for the compiler; deleted with its last py:: consumer
// (recompile/project).
//
//     auto [cpp, called, stats] = powerpc::recompile_function(body, base, address, size);
//     auto [body, mapping, agg] = powerpc::recompile_image(path);
//     py::str header = powerpc::runtime_header();
//     powerpc::main(argv);                                   python -m codebreaker recompile <image or .xex> --all -o game.cpp
//
// What the module is: Xbox 360 PowerPC recompiled to C++ that runs on a PC (one statement per guest instruction); see
// cb/recompile/powerpc.hpp. What is different on purpose: an instruction whose operands cannot be read is a PPC_UNIMPLEMENTED
// trap (the Python raised, or wrote "None" into a statement that recompile_function then turned into the same trap).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/powerpc.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "py.hpp"
#include "py_settings.hpp"

namespace codebreaker::recompile::powerpc {

inline constexpr int64_t WORD = 4;

namespace shim_ {

namespace cbp = cb::recompile::powerpc;

inline py::str default_root() {
    py::str here = py::os::path::abspath(py::str(__FILE__));
    for (int i = 0; i < 6; ++i) here = py::os::path::dirname(here);
    return here;
}

}  // namespace shim_

// the checkout (default: six folders above this header)
inline py::str ROOT = shim_::default_root();
inline void set_root(const py::str& root) { ROOT = root; }

// {bctr address: (index register, [targets])}
using JumpTables = py::dict<int64_t, std::tuple<int64_t, py::list<int64_t>>>;

namespace shim_ {

inline cbp::JumpTables jump_tables_of(const std::optional<JumpTables>& tables) {
    cbp::JumpTables out;
    if (!tables) return out;
    for (const auto& kv : tables->items()) {
        cb::recompile::jumptable::JumpTable t;
        t.index_reg = static_cast<std::uint32_t>(std::get<0>(std::get<1>(kv)));
        for (const int64_t a : std::get<1>(std::get<1>(kv))) t.targets.push_back(static_cast<std::uint32_t>(a));
        out[static_cast<std::uint32_t>(std::get<0>(kv))] = std::move(t);
    }
    return out;
}

inline cbp::CompilerEnv compiler_env() {
    cbp::CompilerEnv env;
    const std::optional<py::str> gpp = py::settings::get_opt_str(py::str("gpp"));
    if (gpp) env.gpp = cb::fs::from_text(cb::compat::from_py(*gpp));
    env.checkout_root = cb::fs::from_text(cb::compat::from_py(ROOT));
    return env;
}

}  // namespace shim_

// Return a list of C++ statement strings for one instruction. `raw` is the 32-bit big-endian instruction word; `jump_tables` maps a
// bctr's address to (index_reg, [targets]) so it emits a real switch instead of an indirect call.
inline py::list<py::str> emit_insn(int64_t va, const py::str& mn, const py::str& op_str, const py::set<int64_t>& /*labels*/, int64_t func_lo,
                                   int64_t func_hi, int64_t raw = 0, const std::optional<JumpTables>& jump_tables = std::nullopt) {
    return cb::compat::guard([&] {
        cb::recompile::disassemble::Row row;
        row.address = static_cast<std::uint32_t>(va);
        row.word = static_cast<std::uint32_t>(raw);
        row.mnemonic = cb::compat::from_py(mn);
        row.operands = cb::compat::from_py(op_str);
        const std::vector<std::string> out = cb::recompile::powerpc::emit_insn(
            row, {static_cast<std::uint32_t>(func_lo), static_cast<std::uint32_t>(func_hi)}, shim_::jump_tables_of(jump_tables));
        py::list<py::str> result;
        for (const std::string& s : out) result.append(cb::compat::to_py(s));
        return result;
    });
}

// recompile_function's stats dict
struct Stats {
    int64_t total = 0;
    int64_t unimpl = 0;
    int64_t translated = 0;
    int64_t labels = 0;
};

// Emit the C++ for one guest function. Returns (cpp_text, called, stats). When `img` (the whole loaded image) is supplied, `bctr`
// switch tables are recovered from it so indirect jumps become real C++ switches; `is_exec(addr)` validates candidate table targets.
inline std::tuple<py::str, py::set<int64_t>, Stats> recompile_function(const py::bytes& body, int64_t /*base_va*/, int64_t func_va, int64_t size,
                                                                       const py::bytes* img = nullptr, int64_t img_base = 0,
                                                                       const std::function<bool(int64_t)>& is_exec = nullptr) {
    return cb::compat::guard([&] {
        const cb::Bytes bytes = cb::compat::from_py(body);
        const cb::Bytes image_bytes = img ? cb::compat::from_py(*img) : cb::Bytes();
        std::optional<cb::recompile::powerpc::GuestImage> guest;
        if (img) {
            guest = cb::recompile::powerpc::GuestImage{image_bytes, static_cast<std::uint32_t>(img_base), is_exec ? std::function<bool(std::uint32_t)>(
                                                                                                           [&](std::uint32_t a) { return is_exec(a); })
                                                                                                     : std::function<bool(std::uint32_t)>()};
        }
        const auto r = cb::recompile::powerpc::recompile_function(disassemble::shim_::mine(), bytes, static_cast<std::uint32_t>(func_va),
                                                                  static_cast<std::uint32_t>(size), guest ? &*guest : nullptr);
        py::set<int64_t> called;
        for (const std::uint32_t a : r.called) called.add(a);
        return std::make_tuple(cb::compat::to_py(r.cpp), called,
                               Stats{static_cast<int64_t>(r.stats.total), static_cast<int64_t>(r.stats.unimplemented), static_cast<int64_t>(r.stats.translated),
                                     static_cast<int64_t>(r.stats.labels)});
    });
}

// The runtime header: enough of rex/ppc.h for the output to compile on its own.
inline py::str runtime_header() { return cb::compat::to_py(cb::recompile::powerpc::runtime_header()); }

// Forward-declare called guest functions so a single .cpp compiles alone.
inline py::str _emit_forward_decls(const py::set<int64_t>& called) {
    std::set<std::uint32_t> a;
    for (const int64_t v : called) a.insert(static_cast<std::uint32_t>(v));
    return cb::compat::to_py(cb::recompile::powerpc::forward_declarations(a));
}

// recompile_image's agg dict
struct Agg {
    int64_t funcs = 0;
    int64_t total = 0;
    int64_t translated = 0;
    int64_t unimpl = 0;
    int64_t discovered = 0;
    int64_t tail_entries = 0;   // "tail entries"
    int64_t lo = 0;
    int64_t hi = 0;
    py::list<py::str> parts;    // each function's C++, in address order
    py::list<int64_t> decls;
    py::list<int64_t> chosen;
    py::list<int64_t> skipped;
    py::ref<formats::powerpc_image::Image> image;
};

// Find every function (discover) and recompile each. -> (C++ text, mapping table text, agg)
inline std::tuple<py::str, py::str, Agg> recompile_image(const py::str& path, std::optional<int64_t> limit = std::nullopt,
                                                         std::optional<int64_t> jobs = std::nullopt,
                                                         const std::optional<py::set<int64_t>>& skip = std::nullopt,
                                                         const std::optional<py::list<int64_t>>& extra = std::nullopt) {
    return cb::compat::guard([&] {
        namespace cbp = cb::recompile::powerpc;
        py::ref<formats::powerpc_image::Image> img = discover::_image(path);
        cbp::Program program{discover::shim_::image_of(*img), cb::recompile::disassemble::flatten(disassemble::shim_::facts_of(path))};
        cbp::ImageOptions options;
        if (limit) options.limit = static_cast<std::size_t>(std::max<int64_t>(*limit, 0));
        if (jobs) options.jobs = static_cast<unsigned>(std::max<int64_t>(*jobs, 1));
        if (skip)
            for (const int64_t a : *skip) options.skip.insert(static_cast<std::uint32_t>(a));
        if (extra)
            for (const int64_t a : *extra) options.extra.push_back(static_cast<std::uint32_t>(a));
        const cbp::ImageResult made = cbp::recompile_image(program, disassemble::shim_::vector_decoder(), options);
        Agg agg;
        const cbp::Aggregate& g = made.agg;
        agg.funcs = static_cast<int64_t>(g.funcs);
        agg.total = static_cast<int64_t>(g.total);
        agg.translated = static_cast<int64_t>(g.translated);
        agg.unimpl = static_cast<int64_t>(g.unimplemented);
        agg.discovered = static_cast<int64_t>(g.discovered);
        agg.tail_entries = static_cast<int64_t>(g.tail_entries);
        agg.lo = g.lo;
        agg.hi = g.hi;
        for (const std::string& p : g.parts) agg.parts.append(cb::compat::to_py(p));
        for (const std::uint32_t a : g.decls) agg.decls.append(a);
        for (const std::uint32_t a : g.chosen) agg.chosen.append(a);
        for (const std::uint32_t a : g.skipped) agg.skipped.append(a);
        agg.image = img;
        return std::make_tuple(cb::compat::to_py(made.body), cb::compat::to_py(made.mapping), agg);
    });
}

// A C++ compiler for checking the output: settings.json `gpp`, else g++ on PATH (or None).
inline std::optional<py::str> compiler() {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        const auto found = cb::recompile::powerpc::find_compiler(shim_::compiler_env());
        if (!found) return std::nullopt;
        return cb::compat::to_py(cb::fs::to_text(*found));
    });
}

// The include folders of the rexglue kernel core (native/rexglue/kernel), in its CMake order; [] when it is not in the tree.
inline py::list<py::str> kernel_includes() {
    return cb::compat::guard([&] {
        py::list<py::str> out;
        for (const auto& p : cb::recompile::powerpc::kernel_includes(cb::fs::from_text(cb::compat::from_py(ROOT)))) out.append(cb::compat::to_py(cb::fs::to_text(p)));
        return out;
    });
}

// (compiles, compiler's message) for one translation unit with the runtime header -- against the kernel core's PPCContext (`bridge`, as
// a generated project builds), else standalone. `compiles` is None when there is no compiler.
inline std::tuple<std::optional<bool>, py::str> check_compiles(const py::str& full, bool bridge = true) {
    return cb::compat::guard([&] {
        const auto r = cb::recompile::powerpc::check_compiles(cb::compat::from_py(full), shim_::compiler_env(), bridge);
        return std::make_tuple(r.compiles, cb::compat::to_py(r.message));
    });
}

// python -m codebreaker recompile <image or .xex> [--all] [--at ADDR --size N] [-o FILE] [--limit N] [--check] [--runtime HEADER]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t {
        namespace cbp = cb::recompile::powerpc;
        cbp::Hooks hooks;
        hooks.load_image = [](const std::filesystem::path& p) {
            const py::str path = cb::compat::to_py(cb::fs::to_text(p));
            return cbp::LoadedImage{discover::shim_::image_of(*discover::_image(path)), disassemble::shim_::facts_of(path)};
        };
        hooks.vector = disassemble::shim_::vector_decoder();
        hooks.compiler = shim_::compiler_env();
        const auto project = std::make_shared<std::optional<base::converted::Project>>();
        hooks.project_output = [project](const std::filesystem::path& binary) {
            *project = base::converted::project(cb::compat::to_py(cb::fs::to_text(binary)));
            return cb::fs::from_text((*project)->src.utf8()) / cb::fs::from_text((*project)->name().lower().utf8() + ".cpp");
        };
        hooks.record = [project](const std::filesystem::path&, const std::vector<std::filesystem::path>& written, std::string_view notes) {
            py::list<py::str> outputs;
            for (const auto& w : written) outputs.append(cb::compat::to_py(cb::fs::to_text(w)));
            if (*project) (*project)->record(py::str("recompile"), outputs, cb::compat::to_py(notes));
        };
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return cb::recompile::powerpc::main(hooks, args);
    });
}

}  // namespace codebreaker::recompile::powerpc
