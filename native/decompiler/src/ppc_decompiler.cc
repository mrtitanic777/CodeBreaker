// XEXMagic - Basic PPC Decompiler
// Translates PPC disassembly into C-like pseudocode

#include "ppc_decompiler.h"

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <sstream>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static inline uint32_t ReadBE32(const uint8_t* p) {
  return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
         ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static std::string HexAddr(uint32_t addr) {
  char buf[16];
  snprintf(buf, sizeof(buf), "0x%08X", addr);
  return buf;
}

static std::string RegName(uint32_t r) {
  char buf[8];
  snprintf(buf, sizeof(buf), "r%u", r);
  return buf;
}

static std::string FRegName(uint32_t r) {
  char buf[8];
  snprintf(buf, sizeof(buf), "fr%u", r);
  return buf;
}

static std::string VarForReg(uint32_t r) {
  if (r >= 3 && r <= 10) {
    char buf[16];
    snprintf(buf, sizeof(buf), "arg%u", r - 3);
    return buf;
  }
  return RegName(r);
}

static std::string VarForStackOffset(uint32_t offset) {
  char buf[32];
  snprintf(buf, sizeof(buf), "local_%X", offset);
  return buf;
}

// Extract fields from a big-endian instruction word
static inline uint32_t BITS(uint32_t w, int hi, int lo) {
  return (w >> (31 - lo)) & ((1u << (lo - hi + 1)) - 1);
}
static inline uint32_t PrimaryOp(uint32_t w) { return BITS(w, 0, 5); }
static inline uint32_t RD(uint32_t w)  { return BITS(w, 6, 10); }
static inline uint32_t RS(uint32_t w)  { return BITS(w, 6, 10); }
static inline uint32_t RA(uint32_t w)  { return BITS(w, 11, 15); }
static inline uint32_t RB(uint32_t w)  { return BITS(w, 16, 20); }
static inline int16_t  SIMM(uint32_t w) { return (int16_t)(w & 0xFFFF); }
static inline uint16_t UIMM(uint32_t w) { return (uint16_t)(w & 0xFFFF); }
static inline uint32_t LK(uint32_t w)  { return w & 1; }
static inline uint32_t AA(uint32_t w)  { return (w >> 1) & 1; }
static inline uint32_t LI(uint32_t w)  { return BITS(w, 6, 29); }
static inline uint32_t BO(uint32_t w)  { return BITS(w, 6, 10); }
static inline uint32_t BI(uint32_t w)  { return BITS(w, 11, 15); }
static inline uint32_t BD(uint32_t w)  { return BITS(w, 16, 29); }
static inline uint32_t XO_10(uint32_t w) { return BITS(w, 21, 30); }
static inline uint32_t XO_9(uint32_t w) { return BITS(w, 22, 30); }
static inline uint32_t SPR(uint32_t w) {
  uint32_t spr = BITS(w, 11, 20);
  return ((spr & 0x1F) << 5) | ((spr >> 5) & 0x1F);
}

static int32_t SignExt(uint32_t val, int bits) {
  uint32_t sign = 1u << (bits - 1);
  return (int32_t)((val ^ sign) - sign);
}

// ---------------------------------------------------------------------------
// PPCDecompiler
// ---------------------------------------------------------------------------

void PPCDecompiler::SetExportMap(
    const std::map<uint32_t, std::string>& addr_to_name) {
  for (auto& [addr, name] : addr_to_name) {
    func_names_[addr] = name;
  }
}

void PPCDecompiler::AddFunctionName(uint32_t addr, const std::string& name) {
  func_names_[addr] = name;
}

std::string PPCDecompiler::GetFunctionName(uint32_t addr) const {
  auto it = func_names_.find(addr);
  if (it != func_names_.end()) return it->second;
  char buf[32];
  snprintf(buf, sizeof(buf), "sub_%08X", addr);
  return buf;
}

// ---------------------------------------------------------------------------
// Function detection
// ---------------------------------------------------------------------------

std::vector<DecompiledFunction> PPCDecompiler::DetectFunctions(
    const uint8_t* data, size_t len, uint32_t base_addr,
    const std::vector<uint32_t>& export_addrs) {
  std::set<uint32_t> entries;

  // Add export addresses as confirmed entries
  for (uint32_t addr : export_addrs) {
    if (addr >= base_addr && addr < base_addr + len) {
      entries.insert(addr);
    }
  }

  // Scan for mflr r0 (0x7C0802A6) as prologue indicator
  // and bl targets as function entries
  for (size_t i = 0; i + 3 < len; i += 4) {
    uint32_t w = ReadBE32(data + i);
    uint32_t addr = base_addr + (uint32_t)i;

    // mflr r0 = 0x7C0802A6
    if (w == 0x7C0802A6) {
      // The function start is likely this instruction or 1-2 instructions before
      // Check if previous instruction is stwu r1, -N(r1) (prologue reorder)
      if (i >= 4) {
        uint32_t prev = ReadBE32(data + i - 4);
        if (PrimaryOp(prev) == 37 && RS(prev) == 1 && RA(prev) == 1) {
          // stwu r1, d(r1) before mflr - function starts at stwu
          entries.insert(addr - 4);
          continue;
        }
      }
      entries.insert(addr);
    }

    // bl target = opcode 18, LK=1, AA=0
    if (PrimaryOp(w) == 18 && LK(w) && !AA(w)) {
      int32_t li = SignExt(LI(w) << 2, 26);
      uint32_t target = addr + li;
      if (target >= base_addr && target < base_addr + len) {
        entries.insert(target);
      }
    }
  }

  // Sort entries and pair them up: each function runs from its entry to
  // the next entry (or to the first blr after).
  std::vector<uint32_t> sorted_entries(entries.begin(), entries.end());
  std::sort(sorted_entries.begin(), sorted_entries.end());

  std::vector<DecompiledFunction> funcs;
  for (size_t idx = 0; idx < sorted_entries.size(); idx++) {
    uint32_t start = sorted_entries[idx];
    uint32_t limit = (idx + 1 < sorted_entries.size())
                         ? sorted_entries[idx + 1]
                         : base_addr + (uint32_t)len;

    // Scan for the function terminator. A function ends at a `blr`, or at an
    // unconditional `b`/`bctr` that nothing branches past (a tail restore
    // `b __restgprlr_NN` or a tail call `b <other_func>`, neither of which has
    // a `blr` of its own). Track the furthest forward branch target so an
    // early terminator inside an if/else body doesn't end the scan.
    // For fall-through helpers (save/restore) the shared blr can be just past
    // the next entry, so scan up to 256 bytes past 'limit'.
    uint32_t scan_limit = std::min(limit + 256, base_addr + (uint32_t)len);
    uint32_t end = limit;
    uint32_t max_target = start;
    for (uint32_t a = start; a < scan_limit; a += 4) {
      size_t off = a - base_addr;
      if (off + 3 >= len) break;
      uint32_t w = ReadBE32(data + off);
      uint32_t prim = (w >> 26) & 0x3F;
      if (prim == 18 && !(w & 1)) {  // unconditional b (LK=0)
        int32_t li = (int32_t)((w & 0x03FFFFFCu) << 6) >> 6;
        uint32_t tgt = (w & 2) ? (uint32_t)li : a + (uint32_t)li;
        if (tgt > max_target && tgt < scan_limit) max_target = tgt;
        if (a >= max_target) { end = a + 4; break; }  // tail restore / tail call
      } else if (prim == 16 && !(w & 1)) {  // bc (conditional) target tracking
        int32_t bd = (int32_t)(int16_t)(uint16_t)(w & 0xFFFCu);
        uint32_t tgt = (w & 2) ? (uint32_t)bd : a + (uint32_t)bd;
        if (tgt > max_target && tgt < scan_limit) max_target = tgt;
      }
      if ((w == 0x4E800020u || w == 0x4E800420u) && a >= max_target) {
        end = a + 4;  // blr / bctr
        break;
      }
    }

    if (end <= start || end - start < 4) continue;

    funcs.push_back(
        DecompileFunction(data, len, base_addr, start, end));
  }

  return funcs;
}

// ---------------------------------------------------------------------------
// Single function decompilation
// ---------------------------------------------------------------------------

DecompiledFunction PPCDecompiler::DecompileFunction(
    const uint8_t* data, size_t len, uint32_t base_addr, uint32_t func_start,
    uint32_t func_end) {
  DecompiledFunction func;
  func.entry = func_start;
  func.end = func_end;
  func.name = GetFunctionName(func_start);

  // Disassemble the function range
  size_t off = func_start - base_addr;
  size_t flen = func_end - func_start;
  if (off + flen > len) flen = len - off;

  auto lines = DisasmBuffer(data + off, flen, func_start);

  func.stack_frame_size = DetectStackFrame(lines);
  func.arg_count = DetectArgCount(lines);
  func.returns_value = DetectReturnsValue(lines);

  // Build basic blocks
  func.blocks = BuildBasicBlocks(lines, func_start, func_end);

  // Translate each block to IR
  for (auto& block : func.blocks) {
    TranslateToIR(block, lines, func_start, func_end);
  }

  // Generate pseudocode
  func.pseudocode = GeneratePseudocode(func);

  return func;
}

// ---------------------------------------------------------------------------
// Stack frame detection
// ---------------------------------------------------------------------------

uint32_t PPCDecompiler::DetectStackFrame(
    const std::vector<DisasmLine>& lines) {
  // Look for stwu r1, -N(r1) in the first few instructions
  for (size_t i = 0; i < lines.size() && i < 5; i++) {
    uint32_t w = lines[i].code;
    // stwu = opcode 37, RS=1, RA=1
    if (PrimaryOp(w) == 37 && RS(w) == 1 && RA(w) == 1) {
      int16_t d = SIMM(w);
      return (uint32_t)(-d);
    }
  }
  return 0;
}

// ---------------------------------------------------------------------------
// Argument count detection
// ---------------------------------------------------------------------------

int PPCDecompiler::DetectArgCount(const std::vector<DisasmLine>& lines) {
  // Track which of r3-r10 are read before being written
  bool read[8] = {};
  bool written[8] = {};

  for (auto& line : lines) {
    uint32_t w = line.code;
    uint32_t op = PrimaryOp(w);

    // Rough check: for D-form loads/stores, RA is a source
    // For arithmetic, RA and RB are sources, RD is dest
    // This is a simplification; just detect common patterns.

    auto markRead = [&](uint32_t r) {
      if (r >= 3 && r <= 10 && !written[r - 3]) read[r - 3] = true;
    };
    auto markWrite = [&](uint32_t r) {
      if (r >= 3 && r <= 10) written[r - 3] = true;
    };

    switch (op) {
      case 14: case 15:  // addi, addis
      case 12: case 13:  // addic
        markRead(RA(w));
        markWrite(RD(w));
        break;
      case 31: {  // XO/X-form
        uint32_t xo9 = XO_9(w);
        // Most ALU: RD = RA op RB
        markRead(RA(w));
        markRead(RB(w));
        markWrite(RD(w));
        // mtspr: RS is a read
        if (XO_10(w) == 467) {
          markRead(RS(w));
        }
        break;
      }
      case 32: case 33: case 34: case 35:  // lwz, lwzu, lbz, lbzu
      case 40: case 41: case 42:            // lhz, lhzu, lha
        markRead(RA(w));
        markWrite(RD(w));
        break;
      case 36: case 37: case 38: case 39:  // stw, stwu, stb, stbu
      case 44: case 45:                     // sth, sthu
        markRead(RS(w));
        markRead(RA(w));
        break;
      default:
        break;
    }
  }

  // Count contiguous args from r3
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (read[i])
      count = i + 1;
    else
      break;
  }
  return count;
}

// ---------------------------------------------------------------------------
// Return value detection
// ---------------------------------------------------------------------------

bool PPCDecompiler::DetectReturnsValue(
    const std::vector<DisasmLine>& lines) {
  // Check if r3 is written anywhere (simple heuristic)
  for (auto& line : lines) {
    uint32_t w = line.code;
    uint32_t op = PrimaryOp(w);
    // Any instruction with RD == 3 (for D-form or XO-form dest)
    if ((op >= 14 && op <= 15) || (op >= 32 && op <= 35) ||
        (op >= 40 && op <= 42) || op == 7 || op == 8) {
      if (RD(w) == 3) return true;
    }
    if (op == 31) {
      if (RD(w) == 3) return true;
    }
  }
  return false;
}

// ---------------------------------------------------------------------------
// Basic block construction
// ---------------------------------------------------------------------------

std::vector<BasicBlock> PPCDecompiler::BuildBasicBlocks(
    const std::vector<DisasmLine>& lines, uint32_t func_start,
    uint32_t func_end) {
  if (lines.empty()) return {};

  // Collect block leaders: first instruction, targets of branches,
  // instructions after branches
  std::set<uint32_t> leaders;
  leaders.insert(func_start);

  for (auto& line : lines) {
    uint32_t w = line.code;
    uint32_t op = PrimaryOp(w);

    if (op == 18) {  // b/bl/ba/bla
      int32_t li = SignExt(LI(w) << 2, 26);
      uint32_t target = AA(w) ? (uint32_t)li : line.address + li;
      if (!LK(w)) {
        // Unconditional branch (not call)
        if (target >= func_start && target < func_end)
          leaders.insert(target);
        if (line.address + 4 < func_end)
          leaders.insert(line.address + 4);
      }
    } else if (op == 16) {  // bc
      int32_t bd = SignExt(BD(w) << 2, 16);
      uint32_t target = AA(w) ? (uint32_t)bd : line.address + bd;
      if (target >= func_start && target < func_end)
        leaders.insert(target);
      if (line.address + 4 < func_end)
        leaders.insert(line.address + 4);
    } else if (op == 19) {  // bclr/bcctr
      if (line.address + 4 < func_end)
        leaders.insert(line.address + 4);
    }
  }

  // Sort leaders
  std::vector<uint32_t> sorted_leaders(leaders.begin(), leaders.end());
  std::sort(sorted_leaders.begin(), sorted_leaders.end());

  // Build blocks
  std::vector<BasicBlock> blocks;
  for (size_t i = 0; i < sorted_leaders.size(); i++) {
    BasicBlock bb;
    bb.start_addr = sorted_leaders[i];
    bb.end_addr = (i + 1 < sorted_leaders.size()) ? sorted_leaders[i + 1]
                                                    : func_end;

    // Find last instruction in this block
    uint32_t last_addr = bb.end_addr - 4;
    for (auto& line : lines) {
      if (line.address == last_addr) {
        uint32_t w = line.code;
        uint32_t op = PrimaryOp(w);
        if (w == 0x4E800020) {  // blr
          bb.is_return = true;
        } else if (op == 18 && !LK(w)) {
          int32_t li = SignExt(LI(w) << 2, 26);
          uint32_t target = AA(w) ? (uint32_t)li : line.address + li;
          bb.successors.push_back(target);
        } else if (op == 16) {
          int32_t bd = SignExt(BD(w) << 2, 16);
          uint32_t target = AA(w) ? (uint32_t)bd : line.address + bd;
          bb.successors.push_back(target);
          // Fall-through
          if (line.address + 4 < func_end)
            bb.successors.push_back(line.address + 4);
        } else {
          // Fall through
          if (bb.end_addr < func_end)
            bb.successors.push_back(bb.end_addr);
        }
        break;
      }
    }

    blocks.push_back(std::move(bb));
  }

  return blocks;
}

// ---------------------------------------------------------------------------
// IR translation
// ---------------------------------------------------------------------------

void PPCDecompiler::TranslateToIR(BasicBlock& block,
                                   const std::vector<DisasmLine>& lines,
                                   uint32_t func_start,
                                   uint32_t func_end) {
  for (auto& line : lines) {
    if (line.address < block.start_addr || line.address >= block.end_addr)
      continue;

    uint32_t w = line.code;
    uint32_t op = PrimaryOp(w);
    IRInstr ir;
    ir.address = line.address;
    ir.comment = line.text;

    switch (op) {
      // addi / li / subi
      case 14: {
        ir.dst = RegName(RD(w));
        if (RA(w) == 0) {
          ir.op = IROp::Assign;
          ir.src1 = std::to_string(SIMM(w));
        } else {
          ir.op = IROp::Add;
          ir.src1 = RegName(RA(w));
          ir.src2 = std::to_string(SIMM(w));
        }
        break;
      }
      // addis / lis
      case 15: {
        ir.dst = RegName(RD(w));
        if (RA(w) == 0) {
          ir.op = IROp::Assign;
          char buf[16];
          snprintf(buf, sizeof(buf), "0x%X0000", UIMM(w));
          ir.src1 = buf;
        } else {
          ir.op = IROp::Add;
          ir.src1 = RegName(RA(w));
          char buf[16];
          snprintf(buf, sizeof(buf), "0x%X0000", UIMM(w));
          ir.src2 = buf;
        }
        break;
      }
      // addic
      case 12: case 13: {
        ir.op = IROp::Add;
        ir.dst = RegName(RD(w));
        ir.src1 = RegName(RA(w));
        ir.src2 = std::to_string(SIMM(w));
        break;
      }
      // mulli
      case 7: {
        ir.op = IROp::Mul;
        ir.dst = RegName(RD(w));
        ir.src1 = RegName(RA(w));
        ir.src2 = std::to_string(SIMM(w));
        break;
      }
      // subfic
      case 8: {
        ir.op = IROp::Sub;
        ir.dst = RegName(RD(w));
        ir.src1 = std::to_string(SIMM(w));
        ir.src2 = RegName(RA(w));
        break;
      }
      // ori / nop
      case 24: {
        if (RS(w) == 0 && RA(w) == 0 && UIMM(w) == 0) {
          ir.op = IROp::Nop;
        } else {
          ir.op = IROp::Or;
          ir.dst = RegName(RA(w));
          ir.src1 = RegName(RS(w));
          char buf[16];
          snprintf(buf, sizeof(buf), "0x%X", UIMM(w));
          ir.src2 = buf;
        }
        break;
      }
      // oris
      case 25: {
        ir.op = IROp::Or;
        ir.dst = RegName(RA(w));
        ir.src1 = RegName(RS(w));
        char buf[16];
        snprintf(buf, sizeof(buf), "0x%X0000", UIMM(w));
        ir.src2 = buf;
        break;
      }
      // xori
      case 26: {
        ir.op = IROp::Xor;
        ir.dst = RegName(RA(w));
        ir.src1 = RegName(RS(w));
        char buf[16];
        snprintf(buf, sizeof(buf), "0x%X", UIMM(w));
        ir.src2 = buf;
        break;
      }
      // andi.
      case 28: {
        ir.op = IROp::And;
        ir.dst = RegName(RA(w));
        ir.src1 = RegName(RS(w));
        char buf[16];
        snprintf(buf, sizeof(buf), "0x%X", UIMM(w));
        ir.src2 = buf;
        break;
      }
      // cmpwi / cmplwi
      case 10: case 11: {
        ir.op = IROp::Compare;
        ir.src1 = RegName(RA(w));
        ir.src2 = std::to_string(SIMM(w));
        break;
      }

      // lwz
      case 32: {
        ir.op = IROp::Load32;
        ir.dst = RegName(RD(w));
        int16_t d = SIMM(w);
        if (RA(w) == 1 && d >= 0) {
          ir.src1 = VarForStackOffset((uint32_t)d);
        } else {
          char buf[32];
          snprintf(buf, sizeof(buf), "*(uint32_t*)(r%u + 0x%X)", RA(w), (uint16_t)d);
          ir.src1 = buf;
        }
        break;
      }
      // lwzu
      case 33: {
        ir.op = IROp::Load32;
        ir.dst = RegName(RD(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint32_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.src1 = buf;
        break;
      }
      // lbz
      case 34: {
        ir.op = IROp::Load8;
        ir.dst = RegName(RD(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint8_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.src1 = buf;
        break;
      }
      // lbzu
      case 35: {
        ir.op = IROp::Load8;
        ir.dst = RegName(RD(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint8_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.src1 = buf;
        break;
      }
      // lhz
      case 40: {
        ir.op = IROp::Load16;
        ir.dst = RegName(RD(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint16_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.src1 = buf;
        break;
      }
      // lhzu
      case 41: {
        ir.op = IROp::Load16;
        ir.dst = RegName(RD(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint16_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.src1 = buf;
        break;
      }
      // lha
      case 42: {
        ir.op = IROp::Load16;
        ir.dst = RegName(RD(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(int16_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.src1 = buf;
        break;
      }

      // stw
      case 36: {
        ir.op = IROp::Store32;
        ir.src1 = RegName(RS(w));
        int16_t d = SIMM(w);
        if (RA(w) == 1 && d >= 0) {
          ir.dst = VarForStackOffset((uint32_t)d);
        } else {
          char buf[32];
          snprintf(buf, sizeof(buf), "*(uint32_t*)(r%u + 0x%X)", RA(w), (uint16_t)d);
          ir.dst = buf;
        }
        break;
      }
      // stwu (prologue/epilogue)
      case 37: {
        ir.op = IROp::Store32;
        ir.src1 = RegName(RS(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint32_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.dst = buf;
        // If this is stwu r1, -N(r1), it's stack frame setup
        if (RS(w) == 1 && RA(w) == 1) {
          ir.op = IROp::Nop;
          ir.comment = "// stack frame setup: " + line.text;
        }
        break;
      }
      // stb
      case 38: {
        ir.op = IROp::Store8;
        ir.src1 = RegName(RS(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint8_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.dst = buf;
        break;
      }
      // stbu
      case 39: {
        ir.op = IROp::Store8;
        ir.src1 = RegName(RS(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint8_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.dst = buf;
        break;
      }
      // sth
      case 44: {
        ir.op = IROp::Store16;
        ir.src1 = RegName(RS(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint16_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.dst = buf;
        break;
      }
      // sthu
      case 45: {
        ir.op = IROp::Store16;
        ir.src1 = RegName(RS(w));
        char buf[32];
        snprintf(buf, sizeof(buf), "*(uint16_t*)(r%u + 0x%X)", RA(w), (uint16_t)SIMM(w));
        ir.dst = buf;
        break;
      }

      // b/bl/ba/bla
      case 18: {
        int32_t li = SignExt(LI(w) << 2, 26);
        uint32_t target = AA(w) ? (uint32_t)li : line.address + li;
        if (LK(w)) {
          ir.op = IROp::Call;
          ir.target = target;
          // Check relocation map first (for COFF libs where target=0).
          uint32_t instr_offset = line.address;
          auto rel_it = reloc_map_.find(instr_offset);
          if (rel_it != reloc_map_.end()) {
            ir.src1 = rel_it->second;
          } else {
            ir.src1 = GetFunctionName(target);
          }
        } else {
          ir.op = IROp::Branch;
          ir.target = target;
        }
        break;
      }

      // bc (conditional branch)
      case 16: {
        int32_t bd = SignExt(BD(w) << 2, 16);
        uint32_t target = AA(w) ? (uint32_t)bd : line.address + bd;
        ir.op = IROp::BranchCond;
        ir.target = target;
        break;
      }

      // opcode 19: bclr/bcctr
      case 19: {
        uint32_t xo = XO_10(w);
        if (xo == 16) {  // bclr
          if (BO(w) == 20 && BI(w) == 0) {
            ir.op = IROp::Return;
          } else {
            ir.op = IROp::BranchCond;
            ir.comment = line.text;
          }
        } else if (xo == 528) {  // bcctr
          ir.op = IROp::Branch;
          ir.comment = line.text;
        } else {
          ir.op = IROp::Unknown;
        }
        break;
      }

      // opcode 31: ALU / SPR / etc.
      case 31: {
        uint32_t xo10 = XO_10(w);
        uint32_t xo9 = XO_9(w);

        // mfspr (xo10 = 339)
        if (xo10 == 339) {
          uint32_t spr = SPR(w);
          ir.op = IROp::Assign;
          ir.dst = RegName(RD(w));
          if (spr == 8) ir.src1 = "lr";
          else if (spr == 9) ir.src1 = "ctr";
          else {
            char buf[16];
            snprintf(buf, sizeof(buf), "spr%u", spr);
            ir.src1 = buf;
          }
          break;
        }
        // mtspr (xo10 = 467)
        if (xo10 == 467) {
          uint32_t spr = SPR(w);
          ir.op = IROp::Assign;
          if (spr == 8) ir.dst = "lr";
          else if (spr == 9) ir.dst = "ctr";
          else {
            char buf[16];
            snprintf(buf, sizeof(buf), "spr%u", spr);
            ir.dst = buf;
          }
          ir.src1 = RegName(RS(w));
          break;
        }

        // Compare (xo10 = 0, 32)
        if (xo10 == 0 || xo10 == 32) {
          ir.op = IROp::Compare;
          ir.src1 = RegName(RA(w));
          ir.src2 = RegName(RB(w));
          break;
        }

        // or (444) = mr alias
        if (xo10 == 444) {
          if (RS(w) == RB(w)) {
            ir.op = IROp::Assign;
            ir.dst = RegName(RA(w));
            ir.src1 = RegName(RS(w));
          } else {
            ir.op = IROp::Or;
            ir.dst = RegName(RA(w));
            ir.src1 = RegName(RS(w));
            ir.src2 = RegName(RB(w));
          }
          break;
        }

        // XO-form ALU
        switch (xo9) {
          case 266: // add
            ir.op = IROp::Add;
            ir.dst = RegName(RD(w));
            ir.src1 = RegName(RA(w));
            ir.src2 = RegName(RB(w));
            break;
          case 10: // addc
            ir.op = IROp::Add;
            ir.dst = RegName(RD(w));
            ir.src1 = RegName(RA(w));
            ir.src2 = RegName(RB(w));
            break;
          case 40: // subf (rD = rB - rA)
            ir.op = IROp::Sub;
            ir.dst = RegName(RD(w));
            ir.src1 = RegName(RB(w));
            ir.src2 = RegName(RA(w));
            break;
          case 235: // mullw
            ir.op = IROp::Mul;
            ir.dst = RegName(RD(w));
            ir.src1 = RegName(RA(w));
            ir.src2 = RegName(RB(w));
            break;
          case 491: // divw
            ir.op = IROp::Div;
            ir.dst = RegName(RD(w));
            ir.src1 = RegName(RA(w));
            ir.src2 = RegName(RB(w));
            break;
          case 459: // divwu
            ir.op = IROp::Div;
            ir.dst = RegName(RD(w));
            ir.src1 = RegName(RA(w));
            ir.src2 = RegName(RB(w));
            break;
          case 104: // neg
            ir.op = IROp::Sub;
            ir.dst = RegName(RD(w));
            ir.src1 = "0";
            ir.src2 = RegName(RA(w));
            break;
          default:
            // X-form logic
            switch (xo10) {
              case 28:  // and
                ir.op = IROp::And;
                ir.dst = RegName(RA(w));
                ir.src1 = RegName(RS(w));
                ir.src2 = RegName(RB(w));
                break;
              case 316: // xor
                ir.op = IROp::Xor;
                ir.dst = RegName(RA(w));
                ir.src1 = RegName(RS(w));
                ir.src2 = RegName(RB(w));
                break;
              case 124: // nor / not
                ir.op = IROp::Not;
                ir.dst = RegName(RA(w));
                ir.src1 = RegName(RS(w));
                break;
              case 24:  // slw
                ir.op = IROp::Shl;
                ir.dst = RegName(RA(w));
                ir.src1 = RegName(RS(w));
                ir.src2 = RegName(RB(w));
                break;
              case 536: // srw
                ir.op = IROp::Shr;
                ir.dst = RegName(RA(w));
                ir.src1 = RegName(RS(w));
                ir.src2 = RegName(RB(w));
                break;
              case 792: // sraw
                ir.op = IROp::Sar;
                ir.dst = RegName(RA(w));
                ir.src1 = RegName(RS(w));
                ir.src2 = RegName(RB(w));
                break;
              case 824: { // srawi
                ir.op = IROp::Sar;
                ir.dst = RegName(RA(w));
                ir.src1 = RegName(RS(w));
                uint32_t sh = BITS(w, 16, 20);
                ir.src2 = std::to_string(sh);
                break;
              }
              default:
                ir.op = IROp::Unknown;
                break;
            }
            break;
        }
        break;
      }

      // rlwinm
      case 21: {
        ir.op = IROp::Unknown;  // Complex rotate-mask, show as asm
        break;
      }
      // rlwimi
      case 20: {
        ir.op = IROp::Unknown;
        break;
      }

      // sc
      case 17: {
        ir.op = IROp::Call;
        ir.src1 = "syscall";
        break;
      }

      default:
        ir.op = IROp::Unknown;
        break;
    }

    block.ir.push_back(std::move(ir));
  }
}

// ---------------------------------------------------------------------------
// Pseudocode generation
// ---------------------------------------------------------------------------

std::string PPCDecompiler::GeneratePseudocode(const DecompiledFunction& func) {
  std::ostringstream out;

  // Function header
  out << "// " << func.name << " (" << HexAddr(func.entry) << ", 0x"
      << std::hex << (func.end - func.entry) << std::dec << " bytes)\n";

  // Signature
  if (func.returns_value)
    out << "uint32_t " << func.name << "(";
  else
    out << "void " << func.name << "(";

  for (int i = 0; i < func.arg_count; i++) {
    if (i > 0) out << ", ";
    out << "uint32_t arg" << i;
  }
  out << ") {\n";

  // Local variables from stack frame
  if (func.stack_frame_size > 0) {
    // Collect unique stack offsets used
    std::set<std::string> locals;
    for (auto& block : func.blocks) {
      for (auto& ir : block.ir) {
        if (ir.dst.find("local_") == 0) locals.insert(ir.dst);
        if (ir.src1.find("local_") == 0) locals.insert(ir.src1);
      }
    }
    for (auto& loc : locals) {
      out << "    uint32_t " << loc << ";\n";
    }
  }

  // Build address -> block index map
  std::map<uint32_t, size_t> block_index;
  for (size_t i = 0; i < func.blocks.size(); i++) {
    block_index[func.blocks[i].start_addr] = i;
  }

  // Emit blocks
  for (size_t bi = 0; bi < func.blocks.size(); bi++) {
    auto& block = func.blocks[bi];
    if (func.blocks.size() > 1) {
      out << "\n";
    }

    for (auto& ir : block.ir) {
      switch (ir.op) {
        case IROp::Nop:
          if (!ir.comment.empty() && ir.comment[0] == '/')
            out << "    " << ir.comment << "\n";
          break;
        case IROp::Assign:
          out << "    " << ir.dst << " = " << ir.src1 << ";\n";
          break;
        case IROp::Add:
          out << "    " << ir.dst << " = " << ir.src1 << " + " << ir.src2 << ";\n";
          break;
        case IROp::Sub:
          out << "    " << ir.dst << " = " << ir.src1 << " - " << ir.src2 << ";\n";
          break;
        case IROp::Mul:
          out << "    " << ir.dst << " = " << ir.src1 << " * " << ir.src2 << ";\n";
          break;
        case IROp::Div:
          out << "    " << ir.dst << " = " << ir.src1 << " / " << ir.src2 << ";\n";
          break;
        case IROp::And:
          out << "    " << ir.dst << " = " << ir.src1 << " & " << ir.src2 << ";\n";
          break;
        case IROp::Or:
          out << "    " << ir.dst << " = " << ir.src1 << " | " << ir.src2 << ";\n";
          break;
        case IROp::Xor:
          out << "    " << ir.dst << " = " << ir.src1 << " ^ " << ir.src2 << ";\n";
          break;
        case IROp::Not:
          out << "    " << ir.dst << " = ~" << ir.src1 << ";\n";
          break;
        case IROp::Shl:
          out << "    " << ir.dst << " = " << ir.src1 << " << " << ir.src2 << ";\n";
          break;
        case IROp::Shr:
          out << "    " << ir.dst << " = " << ir.src1 << " >> " << ir.src2 << ";\n";
          break;
        case IROp::Sar:
          out << "    " << ir.dst << " = (int32_t)" << ir.src1 << " >> " << ir.src2 << ";\n";
          break;
        case IROp::Load8:
        case IROp::Load16:
        case IROp::Load32:
          out << "    " << ir.dst << " = " << ir.src1 << ";\n";
          break;
        case IROp::Store8:
        case IROp::Store16:
        case IROp::Store32:
          out << "    " << ir.dst << " = " << ir.src1 << ";\n";
          break;
        case IROp::Compare:
          out << "    // compare " << ir.src1 << ", " << ir.src2 << "\n";
          break;
        case IROp::Branch: {
          // Check if backward branch (loop)
          if (ir.target < ir.address) {
            out << "    goto loc_" << std::hex << ir.target << std::dec << ";  // loop\n";
          } else {
            out << "    goto loc_" << std::hex << ir.target << std::dec << ";\n";
          }
          break;
        }
        case IROp::BranchCond: {
          if (ir.target < ir.address) {
            out << "    if (cond) goto loc_" << std::hex << ir.target << std::dec
                << ";  // loop back\n";
          } else {
            out << "    if (cond) goto loc_" << std::hex << ir.target << std::dec << ";\n";
          }
          break;
        }
        case IROp::Call:
          if (func.returns_value || !ir.dst.empty()) {
            out << "    r3 = " << ir.src1 << "(";
          } else {
            out << "    " << ir.src1 << "(";
          }
          out << ");\n";
          break;
        case IROp::Return:
          if (func.returns_value)
            out << "    return r3;\n";
          else
            out << "    return;\n";
          break;
        case IROp::Unknown:
          out << "    // asm: " << ir.comment << "\n";
          break;
        default:
          break;
      }
    }
  }

  out << "}\n";
  return out.str();
}
