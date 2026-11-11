// codebreaker/verify/lanes/genesis.hpp -- typed C++ port of codebreaker.verify.lanes.genesis.
//
// genesis.py -- what is particular to reconstructing a Genesis / Mega Drive program.
//
// One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
// source is assembly written from its cartridge (source/genesis_writer.py); it is assembled
// (build/m68k_asm.py, or asm68k / vasm), and the cartridge built is compared with the original. One
// that comes back identical IS the original, so each of its functions does what the original's
// does. When it is not identical -- the source was edited -- each function is run in CodeBreaker's
// 68000 (emulate/m68k.py) in both cartridges, from the same registers, and what it did is
// compared: how the run ended, D0-D7 A0-A6 and the condition codes, the 64 KB of work RAM, and
// every access to the VDP, the I/O ports and the Z80's bus, in order.
//
// Typing notes:
//   * The module is `Lane("genesis", 1, _STATES, _effects, WHAT, start, key="%06x").export(globals())`.
//     image_lane::Lane holds its starting states as dicts of integers, and a Genesis state has two lists of
//     registers in it, so the starting state here is the record State {d, a, fill} and Lane is this module's
//     class: it takes prepare() and build() from an image_lane::Lane (they do not look at the states) and
//     writes profile() and judge() itself, as image_lane.py does. The platform's packager (no port) is the
//     `platform` function given to Lane. export() is not ported (it fills a Python module's namespace).
//   * effects() is the Python's _effects: the tuple it returns is image_lane::Effects, in the order of WHAT.
//     The hardware accesses are one list of text, one line per access ("kind address value").
//     The run's exit, None or text, is text ("None" for None).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/verify/lanes/image_lane.hpp"

namespace codebreaker::verify::lanes::genesis {

using image_lane::Effects;

extern const py::str KIND;   // "genesis"
inline constexpr int64_t REV = 1;

// a starting state: {'d': [8 registers], 'a': [7 registers], 'fill': seed}
struct State {
    py::list<int64_t> d, a;
    int64_t fill = 0;
};

// the VDP, I/O and Z80-bus ranges (_HARDWARE)
inline constexpr int64_t HARDWARE_1_LO = 0xA00000, HARDWARE_1_HI = 0xA1FFFF;
inline constexpr int64_t HARDWARE_2_LO = 0xC00000, HARDWARE_2_HI = 0xC0001F;

const py::list<State>& STATES();   // _STATES
const py::list<py::str>& WHAT();

Effects effects(const py::str& path, int64_t entry, const State& state);   // _effects

// "from D0=%08X A0=%06X" % (state["d"][0], state["a"][0])
py::str start(const State& state);

class Lane {
  public:
    explicit Lane(const std::function<image_lane::Writer()>& platform);

    py::str KIND;
    int64_t REV;
    py::str SAME;

    // (a digest of the settings a verdict depends on, the text it is made from)
    std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt) const;
    image_lane::Prepared prepare(const py::str& module_path, const py::str& work) const;
    image_lane::Built build(const image_lane::Prepared& m, const py::str& module_path, const py::str& work) const;
    image_lane::Verdict judge(const std::optional<image_lane::Context>& context, const image_lane::Item& item) const;

  private:
    image_lane::Lane inner_;   // prepare() and build()
};

}  // namespace codebreaker::verify::lanes::genesis
