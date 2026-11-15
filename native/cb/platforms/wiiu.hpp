// cb/platforms/wiiu.hpp -- the Nintendo Wii U: Espresso (three PowerPC 750 cores of the Broadway's instruction set) code in an RPX executable or an
// RPL library: an ELF whose big sections are stored compressed, which is read decompressed. Programs run under CafeOS and reach the system
// through its libraries, not the hardware; the functions are run in CodeBreaker's own PowerPC interpreter over a flat address space, with the Latte
// window logged.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform wiiu();

}  // namespace cb::platforms
