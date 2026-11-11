// codebreaker/verify/lanes/z80_flat.hpp -- typed C++ port of codebreaker.verify.lanes.z80_flat.
//
// z80_flat.py -- the lane of a flat Z80 program (one block at one address): the ZX Spectrum's and
// the MSX's (verify/lanes/zx.py, verify/lanes/msx.py), each giving its machine's ROM; its reader and
// writer are its platform's loader and packager (codebreaker/platforms/zx.py, msx.py).
//
// The program's source is written (source/z80_writer.py through the machine's writer) and built
// (build/z80_asm.py build_flat); a block that comes back identical IS the original. When it is not
// -- the source was edited -- each function is run in CodeBreaker's Z80 (emulate/z80.py) in the
// original's memory and in the rebuilt one's, from the same registers, and what it did is compared:
// how the run ended, the registers and flags, the memory it left, and every port access, in order.
// The machine's ROM is not run: each of its bytes reads as RET, so a call into the ROM returns at
// once, the same way in both runs.
//
// Typing notes:
//   * lane(kind, rev, rom, ...) returns a Lane made of the module's closures. It is not an
//     image_lane::Lane, because that one runs a file's path and this lane runs a loaded program: the
//     class Lane here has the same profile / prepare / build / judge (prepare() is image_lane's).
//   * The platform (no port) gives the lane two things, passed to lane(): the packager's writer (a
//     FlatWritten: image_lane's Written and the built block, the dict's "built image") and the loader's
//     open (Program: the reader's record, of which the lane reads `memory` when it has one, `origin`, `image`).
//     The emulator is the typed z80 port.
//   * The Python's judge subjects are (program, image) pairs: Subject.
//   * effects() returns what the Python's tuple holds: the run's end, A F BC DE HL IX IY, the bytes the run
//     changed in memory as a list of "address value" text, and the port accesses as text lines
//     ("kind address value"). The run's exit, None or text, is text ("None" for None).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/verify/lanes/image_lane.hpp"

namespace codebreaker::verify::lanes::z80_flat {

using image_lane::Effects;
using image_lane::State;

extern const py::str SAME;   // "the program built from this source is the original, byte for byte"

const py::list<State>& STATES();   // STATES
const py::list<py::str>& WHAT();   // WHAT

// a program the platform's loader read (formats/zx.py, msx.py)
struct Program {
    std::optional<py::bytearray> memory;   // absent: the program has none (a bytearray of 64 KB zeros is used)
    int64_t origin = 0;
    py::bytes image;
};

// what the packager's writer returns: image_lane's dict and the built block
struct FlatWritten : image_lane::Written {
    std::optional<py::bytes> built_image;   // written["built image"]
};
using FlatWriter = std::function<FlatWritten(const py::str& module_path, const py::str& work)>;

// (program, image): what effects() runs
struct Subject {
    Program program;
    py::bytes image;
};

struct Prepared {
    image_lane::Prepared base;
    FlatWritten written;
};

// what judge() is given for the whole program: (same, original, rebuilt)
struct Context {
    bool same;
    Subject original;
    Subject rebuilt;
};

struct Built {
    std::optional<Context> context;   // None: it did not build
    py::dict<py::str, image_lane::Item> items;
    py::dict<py::str, py::str> missing;
};

// lane(kind, rev, rom)'s effects: `rom` is the (lo, hi) addresses the machine's ROM occupies
Effects effects(const py::str& kind, const std::tuple<int64_t, int64_t>& rom, const Subject& subject, int64_t entry, const State& state);   // _effects

class Lane {
  public:
    // kind: the name results are filed under (its platform's loader reads a Program); rev; rom: the
    // addresses the machine's ROM occupies; writer: the platform's packager's; open: its loader's
    Lane(const py::str& kind, int64_t rev, const std::tuple<int64_t, int64_t>& rom, const FlatWriter& writer,
         const std::function<Program(const py::str& path)>& open);

    py::str KIND;
    int64_t REV;
    py::str SAME;

    std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt) const;
    Prepared prepare(const py::str& module_path, const py::str& work) const;
    Built build(const Prepared& m, const py::str& module_path, const py::str& work) const;
    image_lane::Verdict judge(const std::optional<Context>& context, const image_lane::Item& item) const;

  private:
    std::tuple<int64_t, int64_t> rom_;
    FlatWriter writer_;
    std::function<Program(const py::str&)> open_;
    image_lane::Lane inner_;   // profile()
};

// z80_flat.lane(kind, rev, rom)
Lane lane(const py::str& kind, int64_t rev, const std::tuple<int64_t, int64_t>& rom, const FlatWriter& writer,
          const std::function<Program(const py::str& path)>& open);

}  // namespace codebreaker::verify::lanes::z80_flat
