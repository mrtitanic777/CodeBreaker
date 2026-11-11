// codebreaker/platforms/dos.hpp -- typed C++ of codebreaker.platforms.dos
//
// Real-mode DOS programs: MZ executables, and .COM files and device drivers, which have no
// header (formats/dos.py tells them apart). Their source is assembly, rebuilt and compared.
//
// The six slots: the DOS reader (formats/dos.py: the load module, its relocations, a packed one
// unpacked); the 8086 in real mode; the near and far C conventions the decompiler writes; no
// emulator -- nothing is run: a program that comes back byte for byte IS the original; OpenWatcom's
// wasm; the program written back as assembly and linked by wlink (source/dos_writer.py).
//
// Typing notes:
//   * The platforms package has no typed port of its own (the win16/msx ports hold their PLATFORM as
//     plain records too). PLATFORM() is a record of the values the Python's six slots hold: a Ref is
//     (module, name, leading arguments), a Cpu its language id plus fields. Built on first use.
//   * The Python's empty Emulator() slot has no fields: the record has none either.
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"

namespace codebreaker::platforms::dos {

// platforms.Ref: a function of another module, named here and imported when first used
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
    int64_t word = 0;
    py::str order;
    py::list<py::str> results;
    py::str pops;
};

struct Loader {
    py::str container;
    Ref open, parts;
};

struct Compiler {
    py::str language, toolchain;
    Ref tool, tools;
};

struct Packager {
    Ref writer, tools;
};

struct Platform {
    py::str name, title, family, source, lane;
    Cpu cpu;
    Loader loader;
    Convention convention;
    Compiler compiler;
    Packager packager;
};

// detect.kind(path) == "dos"   (.COM files and plain driver images too, which have no header)
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

// {"wasm": exe, "wlink": exe}
py::dict<py::str, py::str> tools();

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::dos
