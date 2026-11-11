// codebreaker/platforms/vxd.hpp -- typed C++ of codebreaker.platforms.vxd
//
// Windows 95 device drivers: LE images that run in ring 0 (VxDs), written in assembly.
//
// The six slots: the LE reader (formats/le.py; a W3 container's members, vmm32.vxd's); the 386 in
// flat ring 0; a driver's register convention (arguments and results in registers, the carry and
// zero flags an answer too); the driver emulator (emulate/driver.py, on Unicorn); the kit's wasm
// (build/driver.tool: the one that knows the DDK's macros); the driver written back as assembly and
// linked as an LE by wlink (source/driver_writer.py, build/driver.py).
//
// Typing notes:
//   * The platforms package has no typed port of its own (the win16/dos ports hold their PLATFORM as
//     plain records too): PLATFORM() is a record of the values the Python's six slots hold, built on
//     first use. A slot field the Python leaves out (None) is not in the record.
//   * entries() returns the typed LE reader's Place records ((object, offset)).
#pragma once

#include <cstdint>
#include <tuple>

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
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

// {label: (object, offset)} of the first driver in the file: its control procedure, its
// services, its API procedures (formats/le.entry_points).
py::dict<py::str, formats::le::Place> entries(const py::str& path);

// {"wasm": exe, "wlink": exe}
py::dict<py::str, py::str> tools();

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::vxd
