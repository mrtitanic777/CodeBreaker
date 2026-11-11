// codebreaker/verify/lanes/gb.hpp -- typed C++ port of codebreaker.verify.lanes.gb.
//
// gb.py -- what is particular to reconstructing a Game Boy program.
//
// One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
// source is assembly written from its cartridge (source/gb_writer.py); it is assembled
// (build/sm83_asm.py, or RGBDS), and the cartridge built is compared with the original. One that
// comes back identical IS the original, so each of its functions does what the original's does.
// When it is not identical -- the source was edited -- each function is run in CodeBreaker's
// SM83 (emulate/sm83.py) in both cartridges, from the same registers, and what it did is compared:
// how the run ended, the registers and flags, work RAM and high RAM as it left them, and every
// access to the I/O registers and the bank controller, in order.
//
// Typing notes:
//   * The module is `Lane("gb", 1, _STATES, _effects, WHAT, start).export(globals())`. lane() makes that
//     Lane (verify/lanes/image_lane.hpp); the platform's packager (no port) is the `platform` function
//     given to it. export() is not ported (it fills a Python module's namespace).
//   * effects() is the Python's _effects: the tuple it returns is image_lane::Effects, in the order of WHAT.
//     The I/O and bank accesses are one list of text, one line per access ("kind address value").
//     The run's exit, None or text, is text ("None" for None).
#pragma once

#include <cstdint>
#include <functional>

#include "py.hpp"
#include "codebreaker/verify/lanes/image_lane.hpp"

namespace codebreaker::verify::lanes::gb {

using image_lane::Effects;
using image_lane::State;

extern const py::str KIND;   // "gb"
inline constexpr int64_t REV = 1;

inline constexpr int64_t IO_LO = 0xFF00, IO_HI = 0xFF7F;   // _IO

// the registers a function starts from, and how work RAM is filled (seed 0: all zero)
const py::list<State>& STATES();   // _STATES
const py::list<py::str>& WHAT();

Effects effects(const py::str& path, int64_t entry, const State& state);   // _effects

// "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state
py::str start(const State& state);

image_lane::Lane lane(const std::function<image_lane::Writer()>& platform);

}  // namespace codebreaker::verify::lanes::gb
