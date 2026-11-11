// codebreaker/platforms/win16.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/platforms/win16.hpp for the ports that still use
// the old API. No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the exe's build is wired
// for C++20.
//
// Old API: Ref, Cpu, Convention, Loader, Emulator, Compiler, Packager, Platform, RunRecord, Runner, segments, entries, call,
// library, tools, _recognise, recognises, PLATFORM. The values of PLATFORM() come from cb::platforms::win16() (one source
// of truth); the Refs (which module's function a slot names) were Python-module-name data and are the constants below,
// copied from the old port until the lanes that read them are refactored onto the layers' own tables.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/platforms/win16.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::platforms::win16 {

struct Ref {
    py::str module;
    py::str name;
    py::list<py::str> args;
};

struct Cpu {
    py::str language;
    int64_t bits = 0;
    py::str endian;
    std::tuple<py::str, py::str, int64_t> decoder;   // ("capstone", arch, bits)
    py::list<py::str> registers;
    py::str stack;
};

struct Convention {
    py::list<py::str> names;
    int64_t arguments = 0;
    int64_t word = 0;
    py::str order;
    py::list<py::str> results;
    py::str pops;
    py::list<py::str> preserved;
};

struct Loader {
    py::str container;
    Ref open, parts, entries, exports, imports;
};

struct Emulator {
    py::str engine;
    Ref run, call;
};

struct Compiler {
    py::str language, toolchain;
    Ref tool, tools, flags, env, compile, library;
};

struct Packager {
    Ref link, functions, tools, linker, assembler, resources;
};

struct Platform {
    py::str name, title, family, source, lane, steps;
    Cpu cpu;
    Loader loader;
    Convention convention;
    Emulator emulator;
    Compiler compiler;
    Packager packager;
};

// What call() reads of the emulator's record.
struct RunRecord {
    py::str ended;
    py::dict<py::str, int64_t> results;
};
using Runner = std::function<RunRecord(const py::str& path, int64_t ordinal, const py::list<int64_t>& args)>;

inline py::list<formats::ne::Segment> segments(const py::str& path) { return formats::ne::NEFile(path).segments(); }

inline py::list<formats::ne::Entry> entries(const py::str& path) { return formats::ne::NEFile(path).entries(); }

// One export of a module called with zero arguments: (how it ended, its result).
inline std::tuple<py::str, std::optional<int64_t>> call(const py::str& path, int64_t ordinal, const Runner& run) {
    const cb::platforms::Win16Runner inner = [&](const std::filesystem::path& p, std::uint32_t ord, std::span<const std::int64_t> args) {
        py::list<int64_t> words;
        for (const std::int64_t a : args) words.append(a);
        try {
            const RunRecord r = run(cb::compat::to_py(cb::fs::to_text(p)), ord, words);
            cb::platforms::Win16Run out{r.ended.utf8(), {}};
            for (const auto& kv : r.results.items()) out.results[std::get<0>(kv).utf8()] = std::get<1>(kv);
            return out;
        } catch (const py::ImportError&) {
            throw;   // a broken setup passes through call_win16_export untouched (it only catches cb::Error)
        } catch (const py::Exception& e) {   // the emulator giving up is a result too (the old text: the Python class name)
            return cb::platforms::Win16Run{"error: " + py::str(e.type()).utf8(), {}};
        }
    };
    const cb::platforms::Win16Call got = cb::platforms::call_win16_export(std::filesystem::path(path.utf8()), static_cast<std::uint32_t>(ordinal), inner);
    return {py::str(got.ended), got.ax ? std::optional<int64_t>(*got.ax) : std::nullopt};
}

// The medium model's C library (clibm.lib), linked with the rebuilt C.
inline py::str library() { return py::os::path::join(build::watcom::ow_lib(py::str("ne")), py::str("clibm.lib")); }

inline py::dict<py::str, py::str> tools() {
    py::dict<py::str, py::str> out;
    for (const char* t : {"wcc", "wlink", "wasm"}) out.set(py::str(t), build::watcom::ow_tool(py::str(t)));   // (an unfound tool is "" already)
    return out;
}

// lane_of(path) == "ne"
inline bool _recognise(const py::str& path) {
    auto lane = base::corpus::lane(path);
    return lane && *lane == py::str("ne");
}

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
inline bool recognises(const py::str& path) {
    try {
        return _recognise(path);
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
}

namespace detail {

inline Ref ref(const char* module, const char* name, std::initializer_list<const char*> args = {}) {
    Ref r{py::str(module), py::str(name), {}};
    for (const char* a : args) r.args.append(py::str(a));
    return r;
}

inline Platform make_platform() {
    const char* here = "codebreaker.platforms.win16";
    const cb::platforms::Platform n = cb::platforms::win16();
    Platform p;
    p.name = cb::compat::to_py(n.name);
    p.title = cb::compat::to_py(n.title);
    p.family = cb::compat::to_py(n.family);
    p.source = cb::compat::to_py(cb::platforms::source_text(*n.source));
    p.lane = py::str("codebreaker.verify.lanes.") + cb::compat::to_py(*n.lane);
    p.steps = py::str("codebreaker.engine.win16");
    p.loader.container = cb::compat::to_py(n.loader.container);
    p.loader.open = ref("codebreaker.formats.ne", "NEFile");
    p.loader.parts = ref(here, "segments");
    p.loader.entries = ref(here, "entries");
    p.loader.exports = ref("codebreaker.build.definitions", "exports");
    p.loader.imports = ref("codebreaker.build.definitions", "imports");
    p.cpu.language = cb::compat::to_py(n.cpu.language);
    p.cpu.bits = *n.cpu.bits;
    p.cpu.endian = py::str("little");
    p.cpu.decoder = std::make_tuple(py::str("capstone"), py::str("x86"), int64_t(16));
    p.cpu.registers = cb::compat::to_py(n.cpu.registers);
    p.cpu.stack = cb::compat::to_py(n.cpu.stack);
    p.convention.names = cb::compat::to_py(n.convention.names);
    p.convention.arguments = std::get<unsigned>(n.convention.arguments);
    p.convention.word = *n.convention.word;
    p.convention.order = py::str("pascal");
    p.convention.results = cb::compat::to_py(n.convention.results);
    p.convention.pops = py::str("either");
    p.convention.preserved = cb::compat::to_py(n.convention.preserved);
    p.emulator.engine = py::str("unicorn");
    p.emulator.run = ref("codebreaker.emulate.windows16", "recorded");
    p.emulator.call = ref(here, "call");
    p.compiler.language = py::str("c");
    p.compiler.toolchain = cb::compat::to_py(n.compiler.toolchain);
    p.compiler.tool = ref("codebreaker.build.watcom", "ow_tool", {"wcc"});
    p.compiler.tools = ref(here, "tools");
    p.compiler.flags = ref("codebreaker.build.functions", "CFLAGS16");
    p.compiler.env = ref("codebreaker.build.watcom", "ow_env", {"ne"});
    p.compiler.compile = ref("codebreaker.build.functions", "_compile");
    p.compiler.library = ref(here, "library");
    p.packager.link = ref("codebreaker.build.module", "_build16");
    p.packager.functions = ref("codebreaker.build.functions", "build16");
    p.packager.tools = ref(here, "tools");
    p.packager.linker = ref("codebreaker.build.watcom", "ow_tool", {"wlink"});
    p.packager.assembler = ref("codebreaker.build.watcom", "ow_tool", {"wasm"});
    p.packager.resources = ref("codebreaker.build.watcom", "ow_tool", {"wrc"});
    return p;
}

}  // namespace detail

// function-local static: built on first use, so no other file's start-up order matters
inline const Platform& PLATFORM() {
    static const Platform p = detail::make_platform();
    return p;
}

}  // namespace codebreaker::platforms::win16
