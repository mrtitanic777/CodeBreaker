// XEXMagic - IL Builder: PPC → IL translation
// Translates raw PPC instructions into typed SSA-form IL.
// Complete coverage of all 176 PPC mnemonics the disassembler handles.

#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "il.h"
#include "ppc_disasm.h"

class ILBuilder {
 public:
  // Set function/relocation maps for resolving call targets.
  void SetFunctionNames(const std::map<uint32_t, std::string>& names) {
    func_names_ = names;
  }
  void SetRelocationMap(const std::map<uint32_t, std::string>& relocs) {
    reloc_map_ = relocs;
  }
  // Call site -> real target, for code that has not been linked yet and whose
  // branches all encode 0. Without it every call in an unlinked object points
  // at itself and the callgraph is empty.
  void SetRelocationTargets(const std::map<uint32_t, uint32_t>& targets) {
    reloc_targets_ = targets;
  }

  // Build IL for a single function.
  ILFunction BuildFunction(const uint8_t* data, size_t len,
                           uint32_t base_addr, uint32_t func_start,
                           uint32_t func_end, const std::string& name);

  // Detect functions in a code buffer and build IL for each.
  std::vector<ILFunction> DetectAndBuildFunctions(
      const uint8_t* data, size_t len, uint32_t base_addr,
      const std::vector<uint32_t>& export_addrs = {});

  std::string GetFunctionName(uint32_t addr) const;

  // Import thunk addresses to exclude from decompilation.
  void SetImportThunkAddresses(const std::set<uint32_t>& addrs) {
    import_thunk_addrs_ = addrs;
  }
  bool IsImportThunk(uint32_t addr) const {
    return import_thunk_addrs_.count(addr) > 0;
  }

 private:
  // PPC instruction field extraction (duplicated from decompiler for independence)
  static uint32_t Bits(uint32_t w, int hi, int lo);
  static uint32_t PrimOp(uint32_t w);
  static uint32_t RD(uint32_t w);
  static uint32_t RS(uint32_t w);
  static uint32_t RA(uint32_t w);
  static uint32_t RB(uint32_t w);
  static uint32_t RC(uint32_t w);  // for float FRC field
  static int16_t SIMM(uint32_t w);
  static uint16_t UIMM(uint32_t w);
  static uint32_t LK(uint32_t w);
  static uint32_t AA(uint32_t w);
  static uint32_t LI(uint32_t w);
  static uint32_t BO(uint32_t w);
  static uint32_t BI(uint32_t w);
  static uint32_t BD(uint32_t w);
  static uint32_t XO10(uint32_t w);
  static uint32_t XO9(uint32_t w);
  static uint32_t SPR(uint32_t w);
  static uint32_t SH(uint32_t w);
  static uint32_t MB(uint32_t w);
  static uint32_t ME(uint32_t w);
  static uint32_t CRM(uint32_t w);
  static uint32_t FRA(uint32_t w);
  static uint32_t FRB(uint32_t w);
  static uint32_t FRC(uint32_t w);
  static uint32_t FRD(uint32_t w);
  static int32_t SignExt(uint32_t val, int bits);
  static uint32_t ReadBE32(const uint8_t* p);
  static uint32_t ComputeRlwinmMask(uint32_t mb, uint32_t me);

  // SSA register state: current value ID for each register
  struct RegState {
    int gpr[32] = {};   // GPR r0-r31 → SSA value ID
    int fpr[32] = {};   // FPR fr0-fr31 → SSA value ID
    int cr[8] = {};     // CR fields cr0-cr7 → SSA value ID (of compare)
    // VMX128 has 128 vector registers, not AltiVec's 32. The upper ones are
    // where the Xenon compilers keep everything hot, so a 32-entry file would
    // lose most of the vector dataflow.
    int vr[128] = {};   // vr0-vr127 → SSA value ID
    int lr = kILNoValue;
    int ctr = kILNoValue;

    void Init() {
      for (int i = 0; i < 32; i++) gpr[i] = kILNoValue;
      for (int i = 0; i < 32; i++) fpr[i] = kILNoValue;
      for (int i = 0; i < 8; i++) cr[i] = kILNoValue;
      for (int i = 0; i < 128; i++) vr[i] = kILNoValue;
      lr = kILNoValue;
      ctr = kILNoValue;
    }
  };

  // Get current SSA value for a GPR, or create an initial one.
  ILValue GetGPR(ILFunction& func, RegState& state, uint32_t r);
  ILValue GetFPR(ILFunction& func, RegState& state, uint32_t r);

  // Destination writes are deferred until the whole instruction has been
  // translated. PPC reads every source before writing the destination, so an
  // instruction like `addi r11, r11, 6960` must see the OLD r11; writing the
  // new SSA value into RegState immediately made the source read its own
  // freshly-allocated value (it decompiled as `v7 = v7 + 0x1B30`). Applies to
  // any instruction whose destination register is also a source.
  std::vector<std::pair<int, int>> pending_writes_;  // reg (0-31 GPR, 32-63 FPR) -> value id
  void FlushPendingWrites(RegState& state);

  // Set a new SSA value for a GPR/FPR.
  ILValue SetGPR(ILFunction& func, RegState& state, uint32_t r);
  ILValue SetFPR(ILFunction& func, RegState& state, uint32_t r);

  // Vector registers. `type` is the element interpretation the instruction
  // gives its result, which is what decides how the variable is spelled.
  ILValue GetVR(ILFunction& func, RegState& state, uint32_t r);
  ILValue SetVR(ILFunction& func, RegState& state, uint32_t r, ILType type);
  // Translates one VMX / VMX128 instruction using the generated opcode
  // tables. Returns false when the word is not a vector instruction.
  bool TranslateVector(ILFunction& func, ILBasicBlock& block, RegState& state,
                       uint32_t address, uint32_t w);

  // Translate one PPC instruction into IL instructions.
  void TranslateInstruction(ILFunction& func, ILBasicBlock& block,
                            RegState& state, uint32_t address, uint32_t w,
                            const std::string& disasm_text,
                            uint32_t func_start, uint32_t func_end);

  // Build basic blocks from disassembly.
  std::vector<ILBasicBlock> BuildBlocks(const std::vector<DisasmLine>& lines,
                                        uint32_t func_start, uint32_t func_end);

  // Detect function boundaries.
  std::vector<std::pair<uint32_t, uint32_t>> DetectFunctions(
      const uint8_t* data, size_t len, uint32_t base_addr,
      const std::vector<uint32_t>& export_addrs);

  // Detect stack frame, arg count, return value.
  uint32_t DetectStackFrame(const uint8_t* data, size_t len, uint32_t off);
  int DetectArgCount(const uint8_t* data, size_t len, uint32_t off, size_t flen);
  bool DetectReturnsValue(const uint8_t* data, size_t len, uint32_t off,
                          size_t flen);

  std::map<uint32_t, std::string> func_names_;
  std::map<uint32_t, std::string> reloc_map_;
  std::map<uint32_t, uint32_t> reloc_targets_;
  std::set<uint32_t> import_thunk_addrs_;
};
