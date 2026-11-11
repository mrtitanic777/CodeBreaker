// codebreaker/verify/lanes/dos.hpp -- typed C++ port of codebreaker.verify.lanes.dos.
//
// dos.py -- what is particular to reconstructing a real-mode program.
//
// One of the kinds reconstruct.py works with (see there for what a kind provides). The program's source is
// assembly written from its image (source/dos_writer.py); it is assembled and linked, and the result is
// compared with the original as a whole: every byte of the load module, the relocation table, the entry
// point and the stack.
//
// Nothing is run. A program that comes back identical IS the original, so each of its functions does what
// the original's does -- a stronger statement than any number of runs would give, and the verdict says that
// this is how it was reached. A program that does not come back identical leaves all of its functions
// unverified, with where it first differs.
//
// What writes and rebuilds the program, and its tools, are the platform's packager
// (codebreaker/platforms/dos.py); its emulator slot is empty.
//
// Typing notes:
//   * `P = platforms.of(KIND)` is the platform's packager, which has no port (platforms/__init__ and dos.py are
//     Python): set_packager() installs it as two functions, tools() ({name: exe path}) and writer(module_path,
//     work) -> Written. Asking before one is set is a RuntimeError.
//   * the writer's dict is Written: source / functions / linked / notes / same / built are its keys, an absent
//     key an empty optional (the Python reads `source`, `functions`, `linked` and `notes` with .get and `same`
//     with [], whose KeyError build() raises).
//   * the kind's four functions: profile -> (digest, settings text), prepare -> Prepared, build -> Built
//     ((context, {name: item}, {name: why})), judge -> Verdict ((verdict, detail, list, text)).
//   * not ported: nothing (the module is functions and constants only).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::verify::lanes::dos {

extern const py::str KIND;   // "dos"
inline constexpr int64_t REV = 93;
extern const py::str SAME;

// a function the writer wrote: its name (the writer's dict holds more, the lane asks only this)
struct Function {
    py::str name;
};

// what the packager's writer returns (a dict)
struct Written {
    std::optional<py::str> source;                          // the assembly source file written
    std::optional<py::dict<int64_t, Function>> functions;   // address -> function
    std::optional<bool> linked;                             // the program was assembled and linked
    std::optional<py::list<py::str>> notes;                 // what the build said, the last line last
    std::optional<bool> same;                               // the built program is the original, byte for byte
    std::optional<py::str> built;                           // the built file
};

struct Packager {
    std::function<py::dict<py::str, py::str>()> tools;                                // {name: exe path}
    std::function<Written(const py::str& module_path, const py::str& work)> writer;   // writes and rebuilds the program
};

void set_packager(const Packager& packager);

// (a digest of the settings a verdict depends on, the text it is made from)
std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt);

struct Prepared {
    Written written;
    py::str header;                              // the source text (read as latin-1)
    py::list<py::str> order;                     // function names, by address
    py::dict<py::str, py::str> key;              // name -> "%05x" of its address
    py::list<py::str> all_keys;
    py::dict<py::str, py::str> texts;
    py::str sig;
};

Prepared prepare(const py::str& module_path, const py::str& work);

// (did the program come back identical, why not)
using Context = std::tuple<bool, py::str>;
// a function's item for judge: (its key,)
using Item = std::tuple<py::str>;

struct Built {
    Context context;
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> missing;   // {name: why it is not in the build}
};

Built build(const Prepared& m, const py::str& module_path, const py::str& work);

// (verdict, what differs, [], text)
using Verdict = std::tuple<py::str, py::str, py::list<py::str>, py::str>;

Verdict judge(const Context& context, const Item& item);

}  // namespace codebreaker::verify::lanes::dos
