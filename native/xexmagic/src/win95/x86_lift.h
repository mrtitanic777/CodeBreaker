// Win95 port: x86 -> IL lifter.
// Consumes decoded X86Insn (x86_decode.h) over a function's linear range, builds
// basic blocks, and emits the shared SSA IL (il.h) so the existing DecompileToC
// back-end renders readable C -- unchanged from the PPC path. Replaces
// ppc_decompiler / il_builder for the x86 port.
//
// Register model: x86 GPRs map to ILValue.ppc_reg slots (the field is just "which
// machine register this value came from" -- reused, not PPC-specific). Flags are
// modelled by remembering the operands of the last flag-setting instruction and
// materialising an IL Compare when a Jcc consumes them (the dominant cmp/test+jcc
// shape). Far pointers/segments are carried as annotations for now.
#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>

#include "il.h"
#include "win95/x86_decode.h"

namespace win95 {

class X86Lifter {
 public:
  // Resolve a call target address to a name (exports/imports/known functions).
  void SetNames(const std::map<uint32_t, std::string>& names) { names_ = names; }

  // Lift one function spanning [entry, end) in the flat image into IL.
  ILFunction LiftFunction(const uint8_t* image, size_t image_size,
                          uint32_t entry, uint32_t end, const std::string& name);

 private:
  std::map<uint32_t, std::string> names_;
};

}  // namespace win95
