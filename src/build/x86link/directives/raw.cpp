// build/x86link/directives/raw.cpp -- `format raw bin|hex`: the image as it is, from `option offset` (a port of wlink's
// cmdraw.c).
#include "build/x86link/directives/formats.hpp"

#ifdef _RAW

namespace cb::x86link::directives {

namespace {

bool ProcRawBIN() {
    FmtData.def_ext = E_BIN;
    FmtData.raw_hex_output = false;
    return true;
}

bool ProcRawHEX() {
    FmtData.def_ext = E_HEX;
    FmtData.raw_hex_output = true;
    return true;
}

constexpr Entry raw_formats[] = {
    {"BIN", ProcRawBIN, MK_RAW},
    {"HEX", ProcRawHEX, MK_RAW},
};

}  // namespace

bool ProcRawFormat() {
    LinkState |= LS_MAKE_RELOCS | LS_FMT_DECIDED;   // (relocations made; the format is decided)
    FmtData.base = 0;                               // (the default offset)
    process_one_or_fail(raw_formats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

#endif
