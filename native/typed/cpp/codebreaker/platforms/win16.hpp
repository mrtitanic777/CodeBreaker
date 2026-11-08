// codebreaker/platforms/win16.hpp -- typed C++ of codebreaker.platforms.win16
//
// 16-bit Windows: NE modules (Windows 3.x and the 16-bit half of Windows 95). Their functions
// come back as C or, where they were written in assembly, as assembly (analyze/origin.py).
//
// The six slots: the NE reader (formats/ne.py); the x86 in 16-bit protected mode; the Pascal and C
// conventions of 16-bit Windows (describe16 writes them: __stdcall16far ... ); the 16-bit emulator
// (emulate/windows16.py, the judge's runner); OpenWatcom's wcc; the module linked by
// build/module._build16 (wlink, with what is kept as assembly assembled by wasm), the judge's
// build of the functions by build/functions.build16. Its steps on the engine: engine/win16.py.
//
// Typing notes:
//   * The platforms package has no typed port of its own (the msx/gb ports hold their PLATFORM as plain
//     records too). PLATFORM() here is a record of the values the Python's six slots hold: a Ref is
//     (module, name, leading arguments), a Cpu its language id plus fields, and so on.  Each field the
//     Python leaves out (None) is std::nullopt.  Built on first use, so no other file's start-up order matters.
//   * segments() / entries() return the typed NE reader's Segment / Entry records (the Python's dicts).
//   * call(): the Python asks PLATFORM.emulator.run (emulate/windows16.recorded) to run the export; that
//     module has no typed port, so the runner is a parameter: run(path, ordinal, args) -> RunRecord (what the
//     Python reads of the record: `ended` and `results`).  `export` is passed as its ordinal.
//     An ImportError from the runner goes on; any other exception is a result, "error: <type>".
//   * results.get("AX") may be any value in the Python; the emulator gives ints or none, so it is
//     std::optional<int64_t> here.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::platforms::win16 {

// platforms.Ref: a function of another module, named here and imported when first used (with leading arguments)
struct Ref {
    py::str module;
    py::str name;
    py::list<py::str> args;
};

// platforms.Cpu: the SLEIGH language id plus bits, endian, decoder, registers, stack
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

// The segment table: [{idx, kind, file_off, length, flags, minalloc, ...}].
py::list<formats::ne::Segment> segments(const py::str& path);

py::list<formats::ne::Entry> entries(const py::str& path);

// One export of a module called with zero arguments: (how it ended, its result).
std::tuple<py::str, std::optional<int64_t>> call(const py::str& path, int64_t ordinal, const Runner& run);

// The medium model's C library (clibm.lib), linked with the rebuilt C.
py::str library();

py::dict<py::str, py::str> tools();

// lane_of(path) == "ne"
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::win16
