// codebreaker/platforms/vxd.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/platforms/vxd.hpp for the ports that still use the
// old API. No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the exe's build is wired for
// C++20.
//
// Old API: Ref, Cpu, Convention, Loader, Emulator, Compiler, Packager, Platform, _recognise, recognises, entries, tools,
// PLATFORM. The values of PLATFORM() come from cb::platforms::vxd() (one source of truth); the Refs (which module's function
// a slot names) were Python-module-name data and are the constants below, copied from the old port until the lanes that
// read them are refactored onto the layers' own tables.
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/platforms/vxd.hpp"
#include "codebreaker/build/driver.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/le.hpp"
#include "py.hpp"

namespace codebreaker::platforms::vxd {

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
    py::str arguments;
    int64_t word = 0;
    py::str order;
    py::list<py::str> results;
    py::str pops;
    py::list<py::str> preserved;
};

struct Loader {
    py::str container;
    Ref open, entries, parts;
};

struct Emulator {
    py::str engine;
    Ref run;
};

struct Compiler {
    py::str language, toolchain;
    Ref tool, tools;
};

struct Packager {
    Ref writer, functions, tools;
};

struct Platform {
    py::str name, title, family, source, lane;
    Cpu cpu;
    Loader loader;
    Convention convention;
    Emulator emulator;
    Compiler compiler;
    Packager packager;
};

// An LE image, or a W3 container of them (vmm32.vxd: VMM and the drivers loaded with it).
inline bool _recognise(const py::str& path) {
    return formats::detect::holds_drivers(path, py::list<py::bytes>{py::bytes("W3")});
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

// {label: (object, offset)} of the first driver in the file: its control procedure, its services, its API procedures.
inline py::dict<py::str, formats::le::Place> entries(const py::str& path) {
    py::dict<py::str, formats::le::Place> out;
    const auto found = cb::compat::guard([&] { return cb::platforms::vxd_entry_points(std::filesystem::path(path.utf8())); });
    for (const auto& [label, place] : found) out.set(py::str(label), formats::le::Place(place.object, place.offset));
    return out;
}

// {"wasm": exe, "wlink": exe}
inline py::dict<py::str, py::str> tools() {
    py::dict<py::str, py::str> out;
    for (const char* t : {"wasm", "wlink"}) out.set(py::str(t), build::driver::tool(py::str(t)));
    return out;
}

namespace detail {

inline Ref ref(const char* module, const char* name, std::initializer_list<const char*> args = {}) {
    Ref r{py::str(module), py::str(name), {}};
    for (const char* a : args) r.args.append(py::str(a));
    return r;
}

inline Platform make_platform() {
    const char* here = "codebreaker.platforms.vxd";
    const cb::platforms::Platform n = cb::platforms::vxd();
    Platform p;
    p.name = cb::compat::to_py(n.name);
    p.title = cb::compat::to_py(n.title);
    p.family = cb::compat::to_py(n.family);
    p.source = cb::compat::to_py(cb::platforms::source_text(*n.source));
    p.lane = py::str("codebreaker.verify.lanes.") + cb::compat::to_py(*n.lane);
    p.loader.container = cb::compat::to_py(n.loader.container);
    p.loader.open = ref("codebreaker.formats.le", "members");
    p.loader.entries = ref(here, "entries");
    p.loader.parts = ref("codebreaker.formats.le", "members");
    p.cpu.language = cb::compat::to_py(n.cpu.language);
    p.cpu.bits = *n.cpu.bits;
    p.cpu.endian = py::str("little");
    p.cpu.decoder = std::make_tuple(py::str("capstone"), py::str("x86"), int64_t(32));
    p.cpu.registers = cb::compat::to_py(n.cpu.registers);
    p.cpu.stack = cb::compat::to_py(n.cpu.stack);
    p.convention.names = cb::compat::to_py(n.convention.names);
    p.convention.arguments = py::str("registers");
    p.convention.word = *n.convention.word;
    p.convention.order = py::str("registers");
    p.convention.results = cb::compat::to_py(n.convention.results);
    p.convention.pops = py::str("callee");
    p.convention.preserved = cb::compat::to_py(n.convention.preserved);
    p.emulator.engine = py::str("unicorn");
    p.emulator.run = ref("codebreaker.emulate.driver", "recorded");
    p.compiler.language = py::str("asm");
    p.compiler.toolchain = cb::compat::to_py(n.compiler.toolchain);
    p.compiler.tool = ref("codebreaker.build.driver", "tool", {"wasm"});
    p.compiler.tools = ref(here, "tools");
    p.packager.writer = ref("codebreaker.source.driver_writer", "reconstruct");
    p.packager.functions = ref("codebreaker.build.driver", "build");
    p.packager.tools = ref(here, "tools");
    return p;
}

}  // namespace detail

// function-local static: built on first use, so no other file's start-up order matters
inline const Platform& PLATFORM() {
    static const Platform p = detail::make_platform();
    return p;
}

}  // namespace codebreaker::platforms::vxd
