// codebreaker/emulate/capstone_x86.hpp -- Capstone's x86 decoders, as platforms.decoder(bits, detail) keeps them.
//
// The emulators of this group decode instructions themselves in a few places (the function carve of a PE, the branch log
// and the fault note of a 16-bit run): they ask for the capstone engine of a width, with or without detail, one of each
// kind per thread, from the capstone.dll the process has loaded (the recorder hands it to cbtrace.dll, or the host
// names it with load_capstone()).
#pragma once

#include <capstone/capstone.h>

#include <cstdint>

#include "py.hpp"

namespace codebreaker::emulate::capstone_x86 {

// capstone.dll's entry points
struct Api {
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free_ = nullptr;
    decltype(&cs_reg_name) reg_name = nullptr;
    decltype(&cs_regs_access) regs_access = nullptr;
};

// Use the capstone.dll at `path` (otherwise the one the process has loaded, else "capstone.dll" on the search path).
void load_capstone(const py::str& path);

const Api& api();

// The engine for 16, 32 or 64-bit x86 code, with (detail) each instruction's operands and registers: one of each kind
// per thread.
csh handle(int64_t bits, bool detail);

}  // namespace codebreaker::emulate::capstone_x86
