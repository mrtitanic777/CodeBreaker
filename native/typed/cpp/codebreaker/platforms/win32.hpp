// codebreaker/platforms/win32.hpp -- typed C++ from codebreaker.platforms.win32.
//
// 32-bit Windows: PE images for the 386 and later (machine type 0x14C).
//
// The six slots: the PE reader (formats/pe.py); the 386 in flat 32-bit mode; the Win32 conventions
// (__stdcall, __cdecl, __fastcall, __thiscall: EAX the result, EBX ESI EDI EBP kept); the 32-bit
// emulator (emulate/emulator.py trace_function, on Unicorn, as a run record); OpenWatcom's wcc386 with the options
// c_text gives; the module linked by build/module._build32, the judge's build by
// build/functions.build32.
//
// Typing notes:
//   * The platforms package has no typed port in this tree (dos / java / msx / vxd / win16 hold their PLATFORM as plain records
//     too): the record types of a platform (Ref, Cpu, Loader, Convention, Emulator, Compiler, Packager, Platform) are declared
//     here with the Python slots' fields (each None unless given: std::optional), and PLATFORM() is the record of the values
//     the Python's six slots hold: a Ref is (module, name, leading arguments; no keyword arguments are used).  Built on first
//     use, so no other file's start-up order matters.
//   * The Python's `recognise=lambda path: ...` is _recognise() (named: a C++ lambda cannot be reached by name); recognises() is
//     Platform.recognises (OSError and ValueError mean "no").
//   * call(): the Python asks PLATFORM.emulator.run (emulate/emulator.recorded) to run the export with the image's data
//     sections mapped (build/module._sections: repeated in win32.cpp, as the typed build/module.cpp needs the z3 headers to
//     compile), externals stubbed and win32._argc resolving signatures; neither has a typed port,
//     so the runner is a parameter: run(path, address, args, mem_init) -> RunRecord (what the Python reads of the record:
//     `ended` and `results`).  `export` is passed as its address.  An ImportError from the runner goes on; any other
//     exception is a result, "error: <type>".
//   * results.get("EAX") may be any value in the Python; the emulator gives ints or none: std::optional<int64_t>.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::platforms::win32 {

// platforms.Ref: a function or value of another module, named here and imported when first used (with leading arguments)
struct Ref {
    py::str module;
    py::str name;
    py::list<py::str> args;
};

// platforms.Cpu: the SLEIGH language id plus bits, endian, decoder, registers, stack (each None unless given)
struct Cpu {
    py::str language;
    std::optional<int64_t> bits;
    std::optional<py::str> endian;
    // ("capstone", arch, bits) or the module of CodeBreaker's own decoder
    std::optional<std::variant<std::tuple<py::str, py::str, int64_t>, py::str>> decoder;
    std::optional<py::list<py::str>> registers;
    std::optional<py::str> stack;
};

struct Loader {
    std::optional<py::str> container;
    std::optional<Ref> open, readable, parts, entries, exports, imports;
};

struct Convention {
    std::optional<py::list<py::str>> names;
    std::optional<std::variant<int64_t, py::str>> arguments;   // how many, or "registers"
    std::optional<int64_t> word;
    std::optional<py::str> order;
    std::optional<py::list<py::str>> results;
    std::optional<py::str> pops;
    std::optional<py::list<py::str>> preserved;
};

struct Emulator {
    std::optional<py::str> engine;
    std::optional<Ref> run, call;
};

struct Compiler {
    std::optional<py::str> language, toolchain;
    std::optional<Ref> tool, tools;
    std::optional<std::variant<Ref, py::list<py::str>>> flags;   // a value, or a Ref to one
    std::optional<Ref> env, compile, library;
};

struct Packager {
    std::optional<Ref> link, functions, writer, tools, linker, assembler, resources;
};

struct Platform {
    py::str name, title, family;
    std::optional<py::str> source, lane, steps;
    std::optional<int64_t> machine;
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
using Runner = std::function<RunRecord(const py::str& path, int64_t address, const py::list<int64_t>& args,
                                       const py::list<std::tuple<int64_t, py::bytes>>& mem_init)>;

// [(address, bytes)] of its code sections.
py::list<std::tuple<int64_t, py::bytes>> sections(const py::str& path);

// One export of a module called with zero arguments: (how it ended, its result).
std::tuple<py::str, std::optional<int64_t>> call(const py::str& path, int64_t address, const Runner& run);

py::dict<py::str, py::str> tools();

// lane_of(path) == "pe" and pe_machine(path) != 0x01F2
bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::win32
