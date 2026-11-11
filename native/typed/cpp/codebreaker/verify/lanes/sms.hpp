// codebreaker/verify/lanes/sms.hpp -- typed C++ port of codebreaker.verify.lanes.sms.
//
// sms.py -- what is particular to reconstructing a Master System / Game Gear program.
//
// One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
// source is assembly written from its cartridge (source/sms_writer.py); it is assembled
// (build/z80_asm.py, or WLA-DX), and the cartridge built is compared with the original. One that
// comes back identical IS the original, so each of its functions does what the original's does.
// When it is not identical -- the source was edited -- each function is run in CodeBreaker's Z80
// (emulate/z80.py) in both cartridges, from the same registers, and what it did is compared: how
// the run ended, the registers and flags, the 8 KB of RAM it left, and every port access and
// mapper write, in order.
//
// Typing notes:
//   * The module is `Lane("sms", 1, _STATES, _effects, WHAT, start).export(globals())`. lane() makes that
//     Lane (verify/lanes/image_lane.hpp); the platform's packager (no port) is the `platform` function
//     given to it. export() is not ported (it fills a Python module's namespace).
//   * effects() is the Python's _effects: the tuple it returns is image_lane::Effects, in the order of WHAT.
//     The port and mapper accesses are one list of text, one line per access ("kind address value").
//     The run's exit, None or text, is text ("None" for None).
#pragma once

#include <cstdint>
#include <functional>

#include "py.hpp"
#include "codebreaker/verify/lanes/image_lane.hpp"

namespace codebreaker::verify::lanes::sms {

using image_lane::Effects;
using image_lane::State;

extern const py::str KIND;   // "sms"
inline constexpr int64_t REV = 1;

inline constexpr int64_t MAPPER_LO = 0xFFFC, MAPPER_HI = 0xFFFF;   // _MAPPER

// the registers a function starts from, and how its RAM is filled (seed 0: all zero)
const py::list<State>& STATES();   // _STATES
const py::list<py::str>& WHAT();

Effects effects(const py::str& path, int64_t entry, const State& state);   // _effects

// "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state
py::str start(const State& state);

image_lane::Lane lane(const std::function<image_lane::Writer()>& platform);

}  // namespace codebreaker::verify::lanes::sms
