// codebreaker/verify/lanes/msx.hpp -- typed C++ port of codebreaker.verify.lanes.msx.
//
// msx.py -- what is particular to reconstructing an MSX cartridge: the BIOS ROM at $0000. Its reader
// (formats/msx.py) and writer (source/msx_writer.py) are its platform's (platforms/msx.py). The lane
// itself is verify/lanes/z80_flat.py's.
//
// Typing notes:
//   * The module is `z80_flat.lane("msx", 2, (0x0000, 0x4000)).export(globals())`. lane() makes that
//     Lane (verify/lanes/z80_flat.hpp); the platform's packager's writer and loader's open (the platforms
//     package has no registry in C++) are the two functions given to it. export() is not ported (it fills a
//     Python module's namespace); KIND, REV and SAME are the constants it fills.
#pragma once

#include <cstdint>
#include <functional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/verify/lanes/z80_flat.hpp"

namespace codebreaker::verify::lanes::msx {

extern const py::str KIND;   // "msx"
inline constexpr int64_t REV = 2;
// z80_flat.SAME (a function: z80_flat's constant lives in another translation unit, so a copy made at
// static-initialization time could be taken before it exists)
const py::str& SAME();

// the addresses the machine's ROM occupies
inline constexpr int64_t ROM_LO = 0x0000, ROM_HI = 0x4000;

// LANE: z80_flat.lane("msx", 2, (0x0000, 0x4000))
z80_flat::Lane lane(const z80_flat::FlatWriter& writer, const std::function<z80_flat::Program(const py::str& path)>& open);

}  // namespace codebreaker::verify::lanes::msx
