// Capstone-backed x86 decoder: same signature/shape as DecodeX86 (x86_decode.h) but
// driven by Capstone 4.0.2, so the lifter gets accurate instruction boundaries and
// typed/sized operands. Drop-in: fills the same X86Insn the native decoder does.
#pragma once
#include <cstddef>
#include <cstdint>
#include "win95/x86_decode.h"

namespace win95 {

// Decode one instruction at data[pos] (addr = its linear address). default_op16
// selects 16-bit (NE/MZ) vs 32-bit (PE/LE) mode. On failure valid==false, len==1.
X86Insn DecodeX86Cap(const uint8_t* data, size_t size, size_t pos, uint32_t addr,
                     bool default_op16 = true);

// Dispatch: Capstone when the env var WT_CAPSTONE is set (once, cached), else the
// native DecodeX86. Lets the whole engine A/B the decode backend at runtime.
X86Insn DecodeX86Auto(const uint8_t* data, size_t size, size_t pos, uint32_t addr,
                      bool default_op16 = true);

}  // namespace win95
