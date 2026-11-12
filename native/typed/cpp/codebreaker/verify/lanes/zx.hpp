// codebreaker/verify/lanes/zx.hpp -- COMPAT SHIM over cb/verify/lanes/zx.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/zx.hpp: the kind's name, the lane's version and where the ZX Spectrum's ROM is. The lane itself
// is z80_flat's, still the old port: `lane()` hands it those three things and the platform's writer and open, as zx.py did.
#pragma once

#include <cstdint>
#include <functional>
#include <tuple>

#include "cb/verify/lanes/zx.hpp"
#include "codebreaker/verify/lanes/z80_flat.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::zx {

namespace nz = cb::verify::lanes::zx;

inline const py::str KIND{std::string(nz::kKind)};   // "zx"
inline constexpr int64_t REV = nz::kRev;
// z80_flat.SAME (a function: z80_flat's constant lives in another translation unit)
inline const py::str& SAME() { return z80_flat::SAME; }

// the addresses the machine's ROM occupies
inline constexpr int64_t ROM_LO = nz::kRomLow, ROM_HI = nz::kRomHigh;

// LANE: z80_flat.lane("zx", 2, (0x0000, 0x4000))
inline z80_flat::Lane lane(const z80_flat::FlatWriter& writer, const std::function<z80_flat::Program(const py::str& path)>& open) {
    return z80_flat::lane(KIND, REV, std::make_tuple(ROM_LO, ROM_HI), writer, open);
}

}  // namespace codebreaker::verify::lanes::zx
