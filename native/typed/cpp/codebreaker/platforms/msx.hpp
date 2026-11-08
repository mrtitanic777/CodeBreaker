// codebreaker/platforms/msx.hpp -- C++17 port of codebreaker.platforms.msx.
//
// The MSX: Z80 code on a cartridge (.rom). Its programs were written in assembly, which is what
// comes back (source/msx_writer.py, sjasmplus / pasmo syntax); the cartridge built from that source
// is compared with the original, and its functions are run in CodeBreaker's own Z80
// (emulate/z80.py).
//
// The Python builds PLATFORM with platforms.machine(...), a Platform of six slot objects. The
// platforms package has no typed port yet, so the description is held here as plain records that
// carry the same values the Python's slots do (Ref -> (module, name), Cpu -> language + fields).
#pragma once

#include "py.hpp"

namespace codebreaker::platforms::msx {

// platforms.Ref: a function of another module, named here and imported when first used
struct Ref {
    py::str module;
    py::str name;
};

// platforms.Cpu: the SLEIGH language id plus bits, endian, decoder, registers, stack
struct Cpu {
    py::str language;
    int64_t bits = 0;
    py::str endian;
    py::str decoder;  // a module of CodeBreaker's own (never a capstone tuple for the MSX)
    py::list<py::str> registers;
    py::str stack;
};

// platforms.Platform as platforms.machine() fills it for an assembly-source machine
struct Platform {
    py::str name;
    py::str title;
    py::str family;
    py::str source;
    py::str lane;
    Cpu cpu;
    py::str container;  // loader.container
    Ref reader;         // loader.open
    py::str engine;     // emulator.engine
    Ref runner;         // emulator.run
    py::str language;   // compiler.language
    py::str toolchain;  // compiler.toolchain
    Ref assembler;      // compiler.tool
    Ref writer;         // packager.writer
    py::list<py::str> convention_names;
    py::str convention_arguments;
    py::str convention_order;
    py::list<py::str> results;  // convention.results
    py::str pops;
};

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

extern const Platform PLATFORM;

}  // namespace codebreaker::platforms::msx
