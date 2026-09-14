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

  // Select the ABI/decode width: false = 16-bit NE (default; params at [bp+6],
  // 2-byte slots, 16-bit operand default), true = 32-bit PE/LE (params at
  // [ebp+8], 4-byte slots, 32-bit operand default). Everything else in the lift
  // is width-agnostic.
  void SetBits32(bool on) { bits32_ = on; }

  // VxD (LE) mode: model the `int 20h; dd <service id>` idiom as a single VxD
  // service call -- consuming the 4-byte service dword so it is not misdecoded as
  // instructions, and rendering a readable VxDCall_<id>() instead of `__asm int`.
  void SetVxDMode(bool on) { vxd_mode_ = on; }

  // Resolve a far call/jmp by the instruction's linear address to (target, name),
  // from the NE relocation table. Far operands in an unlinked/relocated NE encode
  // 0, so the fixup table is the only source of the real target.
  void SetCallSites(const std::map<uint32_t, std::pair<uint32_t, std::string>>& cs) {
    call_sites_ = cs;
  }

  // Lift one function spanning [entry, end) in the flat image into IL.
  ILFunction LiftFunction(const uint8_t* image, size_t image_size,
                          uint32_t entry, uint32_t end, const std::string& name);

 private:
  std::map<uint32_t, std::string> names_;
  std::map<uint32_t, std::pair<uint32_t, std::string>> call_sites_;
  bool bits32_ = false;
  bool vxd_mode_ = false;
};

}  // namespace win95
