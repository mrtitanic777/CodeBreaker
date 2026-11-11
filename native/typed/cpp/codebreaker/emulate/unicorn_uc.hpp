// codebreaker/emulate/unicorn_uc.hpp -- the Unicorn-backed implementation of core16::Uc.
//
// The emulators of this group (emulator, windows16), the shadows and the lanes take `core16::Uc&`, the abstract
// engine core16_types.hpp declares.  This is the concrete one: an x86 engine on Unicorn's C API, in the
// unicorn.dll the Python `unicorn` package carries (load_unicorn hands over its path once, as
// exact/emu/dpmi.hpp's load_unicorn does for the DOS executive).
//
//     unicorn_uc::load_unicorn("C:/.../unicorn/lib/unicorn.dll");
//     auto uc = unicorn_uc::open_x86(32);          // unicorn.Uc(UC_ARCH_X86, UC_MODE_32)
//     uc->mem_map(0x1000, 0x1000);
//
// Errors from Unicorn are native_trace::UcError (unicorn.UcError); strerror() is Unicorn's own message for one.
// A callback that throws stops the run (uc_emu_stop) and the exception comes out of emu_start().
#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "codebreaker/core16_types.hpp"
#include "py.hpp"

namespace codebreaker::emulate::unicorn_uc {

// Load unicorn.dll from `dll_path` (once).  RuntimeError when it cannot be loaded.
void load_unicorn(const py::str& dll_path);
bool loaded();

// str(unicorn.UcError(err)): uc_strerror
py::str strerror(int64_t err);

// unicorn.x86_const.UC_X86_REG_<NAME>: the id of an x86 register by its (upper-case) name, None when there is none.
std::optional<int64_t> x86_reg(const py::str& name);

// A new x86 engine: bits 16, 32 or 64 (UC_MODE_16 / 32 / 64).
std::shared_ptr<core16::Uc> open_x86(int64_t bits);

}  // namespace codebreaker::emulate::unicorn_uc
