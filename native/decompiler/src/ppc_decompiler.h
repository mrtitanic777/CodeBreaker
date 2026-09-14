// XEXMagic - Basic PPC Decompiler
// Translates PPC disassembly into C-like pseudocode

#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "ppc_disasm.h"

// ---------------------------------------------------------------------------
// IR
// ---------------------------------------------------------------------------

enum class IROp {
  Assign,
  Add,
  Sub,
  Mul,
  Div,
  And,
  Or,
  Xor,
  Not,
  Shl,
  Shr,
  Sar,
  Load8,
  Load16,
  Load32,
  Store8,
  Store16,
  Store32,
  Compare,
  Branch,
  BranchCond,
  Call,
  Return,
  Nop,
  Unknown,
};

struct IRInstr {
  IROp op = IROp::Unknown;
  std::string dst;        // destination variable/register
  std::string src1;       // first source
  std::string src2;       // second source (or immediate as string)
  uint32_t address = 0;   // source PPC address
  uint32_t target = 0;    // branch/call target
  std::string comment;    // raw disasm or note
};

// ---------------------------------------------------------------------------
// Basic Block
// ---------------------------------------------------------------------------

struct BasicBlock {
  uint32_t start_addr = 0;
  uint32_t end_addr = 0;
  std::vector<IRInstr> ir;
  std::vector<uint32_t> successors;
  bool is_return = false;
};

// ---------------------------------------------------------------------------
// Function
// ---------------------------------------------------------------------------

struct DecompiledFunction {
  std::string name;
  uint32_t entry = 0;
  uint32_t end = 0;
  uint32_t stack_frame_size = 0;
  int arg_count = 0;           // detected argument count (r3-r10)
  bool returns_value = false;
  std::vector<BasicBlock> blocks;
  std::set<std::string> local_vars;
  std::string pseudocode;
};

// ---------------------------------------------------------------------------
// Decompiler
// ---------------------------------------------------------------------------

class PPCDecompiler {
 public:
  // Set the export map (ordinal/name -> address) for resolving call targets.
  void SetExportMap(const std::map<uint32_t, std::string>& addr_to_name);

  // Add a known function name at an address.
  void AddFunctionName(uint32_t addr, const std::string& name);

  // Set relocation map: instruction_offset -> symbol_name.
  // Used for COFF libs where call targets are all 0 (relocatable).
  void SetRelocationMap(const std::map<uint32_t, std::string>& relocs) {
    reloc_map_ = relocs;
  }

  // Detect functions in a code buffer.
  // data: big-endian PPC instructions, len: byte count, base_addr: VA of first byte.
  // export_addrs: known entry points from export table.
  std::vector<DecompiledFunction> DetectFunctions(
      const uint8_t* data, size_t len, uint32_t base_addr,
      const std::vector<uint32_t>& export_addrs = {});

  // Decompile a single function given its instruction range.
  DecompiledFunction DecompileFunction(const uint8_t* data, size_t len,
                                       uint32_t base_addr, uint32_t func_start,
                                       uint32_t func_end);

  // Get the name for an address, or generate sub_XXXXXXXX.
  std::string GetFunctionName(uint32_t addr) const;

 private:
  // Build basic blocks from disassembled instructions.
  std::vector<BasicBlock> BuildBasicBlocks(
      const std::vector<DisasmLine>& lines, uint32_t func_start,
      uint32_t func_end);

  // Translate disassembly lines within a basic block to IR.
  void TranslateToIR(BasicBlock& block, const std::vector<DisasmLine>& lines,
                     uint32_t func_start, uint32_t func_end);

  // Generate pseudocode from basic blocks.
  std::string GeneratePseudocode(const DecompiledFunction& func);

  // Detect stack frame size from prologue.
  uint32_t DetectStackFrame(const std::vector<DisasmLine>& lines);

  // Detect argument count by scanning for r3-r10 reads before writes.
  int DetectArgCount(const std::vector<DisasmLine>& lines);

  // Detect if function returns a value (writes to r3 before blr).
  bool DetectReturnsValue(const std::vector<DisasmLine>& lines);

  std::map<uint32_t, std::string> func_names_;
  std::map<uint32_t, std::string> reloc_map_;  // offset -> symbol for COFF
};
