// codebreaker/verify/lanes/nes.hpp -- typed C++ port of codebreaker.verify.lanes.nes.
//
// nes.py -- what is particular to reconstructing a NES program.
//
// One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
// source is assembly written from its cartridge (source/nes_writer.py); it is assembled and linked
// (build/m6502_asm.py, or the cc65 suite), and the cartridge built is compared with the original.
// One that comes back identical IS the original, so each of its functions does what the
// original's does. When it is not identical -- the source was edited -- each function is run in
// CodeBreaker's 6502 (emulate/m6502.py) in both cartridges, from the same registers, and what it
// did is compared: how the run ended, A X Y and the flags, the RAM it left, and every access to
// the picture and sound hardware, in order.
//
// Typing notes:
//   * The module is `Lane("nes", 2, _STATES, _effects, WHAT, start).export(globals())`. lane() makes that
//     Lane (verify/lanes/image_lane.hpp); the platform's packager (no port) is the `platform` function
//     given to it, as in image_lane. export() is not ported (it fills a Python module's namespace).
//   * effects() is the Python's _effects: the tuple it returns is image_lane::Effects, in the order of WHAT.
//     The hardware accesses are one list of text, one line per access ("kind address value", in order).
//     The run's exit, None or text, is text ("None" for None).
#pragma once

#include <cstdint>
#include <functional>

#include "py.hpp"
#include "codebreaker/verify/lanes/image_lane.hpp"

namespace codebreaker::verify::lanes::nes {

using image_lane::Effects;
using image_lane::State;

extern const py::str KIND;   // "nes"
inline constexpr int64_t REV = 2;

inline constexpr int64_t HARDWARE_LO = 0x2000, HARDWARE_HI = 0x401F;   // _HARDWARE

// the registers a function starts from, and how its 2 KB of RAM is filled (seed 0: all zero)
const py::list<State>& STATES();   // _STATES
const py::list<py::str>& WHAT();

Effects effects(const py::str& path, int64_t entry, const State& state);   // _effects

// "from A=%(a)02X X=%(x)02X Y=%(y)02X" % state
py::str start(const State& state);

image_lane::Lane lane(const std::function<image_lane::Writer()>& platform);

}  // namespace codebreaker::verify::lanes::nes
