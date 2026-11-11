// cb/emulate/capstone_x86.hpp -- Capstone's x86 engines for the emulators and the recorder.
//
// The one place an x86 decoder is made. An engine is made on first use for a width (16, 32 or 64-bit code) with or
// without instruction detail (the operands and registers each instruction reads and writes), and kept for the life
// of the calling thread: a Capstone handle is not safe to share between threads, so each thread has its own, closed
// when the thread ends. The DLL is the one beside the executable (cb/core/capstone_api.hpp).
//
//     csh h = capstone_x86::engine(32, /*detail=*/false);    // csh is Capstone's handle type (size_t)
//     cs_disasm_iter(h, ...);
//
// The handle is a plain integer so this header does not pull in Capstone's; the .cpp that decodes includes
// <capstone/capstone.h> (through cb/core/capstone_api.hpp) and gets `csh` from it.
#pragma once

#include <cstddef>
#include <filesystem>

namespace cb::emulate::capstone_x86 {

// The calling thread's engine for `bits`-bit x86 code (16, 32 or 64; anything else is a UsageError), with `detail`
// on or off. Valid until the thread ends; never close it.
std::size_t engine(int bits, bool detail);

// The path of the capstone.dll in use.
const std::filesystem::path& dll_path();

}  // namespace cb::emulate::capstone_x86
