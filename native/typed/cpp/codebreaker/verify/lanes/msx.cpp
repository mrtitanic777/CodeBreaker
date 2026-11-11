// codebreaker/verify/lanes/msx.cpp -- typed C++ port of codebreaker.verify.lanes.msx (see msx.hpp).

#include "codebreaker/verify/lanes/msx.hpp"

namespace codebreaker::verify::lanes::msx {

const py::str KIND("msx");
const py::str& SAME() { return z80_flat::SAME; }

z80_flat::Lane lane(const z80_flat::FlatWriter& writer, const std::function<z80_flat::Program(const py::str& path)>& open) {
    return z80_flat::lane(KIND, REV, std::make_tuple(ROM_LO, ROM_HI), writer, open);
}

}  // namespace codebreaker::verify::lanes::msx
