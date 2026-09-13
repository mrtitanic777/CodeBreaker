// XEXMagic - IL Builder: PPC → IL translation
// Complete coverage of all PPC mnemonics handled by the disassembler.

#include "il_builder.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <set>
#include <string>
#include <unordered_map>

#include "progress.h"
#include "ppc/ppc_decode_data.h"
#include "ppc/ppc_opcode.h"
#include "ppc/ppc_opcode_info.h"

// ---------------------------------------------------------------------------
// PPC field extraction
// ---------------------------------------------------------------------------

uint32_t ILBuilder::Bits(uint32_t w, int hi, int lo) {
  return (w >> (31 - lo)) & ((1u << (lo - hi + 1)) - 1);
}
uint32_t ILBuilder::PrimOp(uint32_t w) { return Bits(w, 0, 5); }
uint32_t ILBuilder::RD(uint32_t w)  { return Bits(w, 6, 10); }
uint32_t ILBuilder::RS(uint32_t w)  { return Bits(w, 6, 10); }
uint32_t ILBuilder::RA(uint32_t w)  { return Bits(w, 11, 15); }
uint32_t ILBuilder::RB(uint32_t w)  { return Bits(w, 16, 20); }
uint32_t ILBuilder::RC(uint32_t w)  { return Bits(w, 21, 25); }
int16_t  ILBuilder::SIMM(uint32_t w) { return (int16_t)(w & 0xFFFF); }
uint16_t ILBuilder::UIMM(uint32_t w) { return (uint16_t)(w & 0xFFFF); }
uint32_t ILBuilder::LK(uint32_t w)  { return w & 1; }
uint32_t ILBuilder::AA(uint32_t w)  { return (w >> 1) & 1; }
uint32_t ILBuilder::LI(uint32_t w)  { return Bits(w, 6, 29); }
uint32_t ILBuilder::BO(uint32_t w)  { return Bits(w, 6, 10); }
uint32_t ILBuilder::BI(uint32_t w)  { return Bits(w, 11, 15); }
uint32_t ILBuilder::BD(uint32_t w)  { return Bits(w, 16, 29); }
uint32_t ILBuilder::XO10(uint32_t w) { return Bits(w, 21, 30); }
uint32_t ILBuilder::XO9(uint32_t w)  { return Bits(w, 22, 30); }
uint32_t ILBuilder::SH(uint32_t w)  { return Bits(w, 16, 20); }
uint32_t ILBuilder::MB(uint32_t w)  { return Bits(w, 21, 25); }
uint32_t ILBuilder::ME(uint32_t w)  { return Bits(w, 26, 30); }
uint32_t ILBuilder::CRM(uint32_t w) { return Bits(w, 12, 19); }
uint32_t ILBuilder::FRA(uint32_t w) { return Bits(w, 11, 15); }
uint32_t ILBuilder::FRB(uint32_t w) { return Bits(w, 16, 20); }
uint32_t ILBuilder::FRC(uint32_t w) { return Bits(w, 21, 25); }
uint32_t ILBuilder::FRD(uint32_t w) { return Bits(w, 6, 10); }

uint32_t ILBuilder::SPR(uint32_t w) {
  uint32_t spr = Bits(w, 11, 20);
  return ((spr & 0x1F) << 5) | ((spr >> 5) & 0x1F);
}

int32_t ILBuilder::SignExt(uint32_t val, int bits) {
  uint32_t sign = 1u << (bits - 1);
  return (int32_t)((val ^ sign) - sign);
}

uint32_t ILBuilder::ReadBE32(const uint8_t* p) {
  return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
         ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

uint32_t ILBuilder::ComputeRlwinmMask(uint32_t mb, uint32_t me) {
  uint32_t mask = 0;
  if (mb <= me) {
    for (uint32_t i = mb; i <= me; i++)
      mask |= (1u << (31 - i));
  } else {
    // Wrapping mask
    for (uint32_t i = mb; i <= 31; i++)
      mask |= (1u << (31 - i));
    for (uint32_t i = 0; i <= me; i++)
      mask |= (1u << (31 - i));
  }
  return mask;
}

// ---------------------------------------------------------------------------
// SSA register helpers
// ---------------------------------------------------------------------------

ILValue ILBuilder::GetGPR(ILFunction& func, RegState& state, uint32_t r) {
  if (r >= 32) return ILNoVal();
  if (state.gpr[r] != kILNoValue) {
    // Find existing value
    for (auto& v : func.values) {
      if (v.id == state.gpr[r]) return v;
    }
  }
  // Create initial value for this register
  ILValue v = func.AllocValue(ILType::U32, (int)r);
  state.gpr[r] = v.id;
  return v;
}

ILValue ILBuilder::GetFPR(ILFunction& func, RegState& state, uint32_t r) {
  if (r >= 32) return ILNoVal();
  if (state.fpr[r] != kILNoValue) {
    for (auto& v : func.values) {
      if (v.id == state.fpr[r]) return v;
    }
  }
  ILValue v = func.AllocValue(ILType::F64, 32 + (int)r);
  state.fpr[r] = v.id;
  return v;
}

ILValue ILBuilder::SetGPR(ILFunction& func, RegState& state, uint32_t r) {
  if (r >= 32) return ILNoVal();
  ILValue v = func.AllocValue(ILType::U32, (int)r);
  // Deferred: applied by FlushPendingWrites once the instruction is done, so
  // sources read within this instruction still see the previous value.
  pending_writes_.emplace_back((int)r, v.id);
  return v;
}

ILValue ILBuilder::SetFPR(ILFunction& func, RegState& state, uint32_t r) {
  if (r >= 32) return ILNoVal();
  ILValue v = func.AllocValue(ILType::F64, 32 + (int)r);
  pending_writes_.emplace_back(32 + (int)r, v.id);
  return v;
}

// Vector registers live at reg numbers 64..191, past the GPRs, FPRs and the
// condition register, so the existing pending-write and merge machinery
// carries them without knowing what they are.
static constexpr int kVRRegBase = 64;

ILValue ILBuilder::GetVR(ILFunction& func, RegState& state, uint32_t r) {
  if (r >= 128) return ILNoVal();
  if (state.vr[r] != kILNoValue) {
    for (auto& v : func.values) {
      if (v.id == state.vr[r]) return v;
    }
  }
  ILValue v = func.AllocValue(ILType::V128U, kVRRegBase + (int)r);
  state.vr[r] = v.id;
  return v;
}

ILValue ILBuilder::SetVR(ILFunction& func, RegState& state, uint32_t r,
                         ILType type) {
  if (r >= 128) return ILNoVal();
  ILValue v = func.AllocValue(type, kVRRegBase + (int)r);
  pending_writes_.emplace_back(kVRRegBase + (int)r, v.id);
  return v;
}

void ILBuilder::FlushPendingWrites(RegState& state) {
  for (const auto& [reg, id] : pending_writes_) {
    if (reg < 32) state.gpr[reg] = id;
    else if (reg < 64) state.fpr[reg - 32] = id;
    else state.vr[reg - kVRRegBase] = id;
  }
  pending_writes_.clear();
}

// ---------------------------------------------------------------------------
// Function name resolution
// ---------------------------------------------------------------------------

std::string ILBuilder::GetFunctionName(uint32_t addr) const {
  auto it = func_names_.find(addr);
  if (it != func_names_.end()) return it->second;
  char buf[32];
  snprintf(buf, sizeof(buf), "sub_%08X", addr);
  return buf;
}

// ---------------------------------------------------------------------------
// Translate one PPC instruction to IL
// ---------------------------------------------------------------------------

void ILBuilder::TranslateInstruction(
    ILFunction& func, ILBasicBlock& block, RegState& state,
    uint32_t address, uint32_t w, const std::string& disasm_text,
    uint32_t func_start, uint32_t func_end) {

  uint32_t op = PrimOp(w);
  ILInstr il;
  il.address = address;
  il.ppc_word = w;

  // Apply this instruction's destination writes on the way out, whichever of
  // the many `return`s or `break`s we leave through.
  struct PendingGuard {
    ILBuilder* self;
    RegState* st;
    ~PendingGuard() { self->FlushPendingWrites(*st); }
  } pending_guard{this, &state};

  auto emit = [&](ILInstr& i) {
    block.instrs.push_back(std::move(i));
  };

  // The vector unit is decoded from the generated tables, which know the whole
  // VMX128 encoding. This has to run before the primary-opcode switch: opcodes
  // 4, 5 and 6 have cases of their own there that predate vector support and
  // would emit the instruction as inline asm.
  if (TranslateVector(func, block, state, address, w)) return;

  auto makeImm = [&](int32_t val) -> ILValue {
    ILValue v = func.AllocValue(ILType::S32);
    // Immediate values get special names during codegen
    v.name = std::to_string(val);
    return v;
  };

  auto makeUImm = [&](uint32_t val) -> ILValue {
    ILValue v = func.AllocValue(ILType::U32);
    char buf[16];
    snprintf(buf, sizeof(buf), "0x%X", val);
    v.name = buf;
    return v;
  };

  switch (op) {
    // =====================================================================
    // addi / li / subi (opcode 14)
    // =====================================================================
    case 14: {
      ILValue dst = SetGPR(func, state, RD(w));
      if (RA(w) == 0) {
        il.op = ILOp::Assign;
        il.dst = dst;
        il.src1 = makeImm(SIMM(w));
      } else {
        il.op = ILOp::Add;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = makeImm(SIMM(w));
      }
      emit(il);
      break;
    }

    // =====================================================================
    // addis / lis (opcode 15)
    // =====================================================================
    case 15: {
      ILValue dst = SetGPR(func, state, RD(w));
      uint32_t shifted = (uint32_t)UIMM(w) << 16;
      if (RA(w) == 0) {
        il.op = ILOp::Assign;
        il.dst = dst;
        il.src1 = makeUImm(shifted);
      } else {
        il.op = ILOp::Add;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = makeUImm(shifted);
      }
      emit(il);
      break;
    }

    // =====================================================================
    // addic / addic. (opcode 12, 13)
    // =====================================================================
    case 12: case 13: { // addic (12), addic. (13) — both set CA
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::Add;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RA(w));
      il.src2 = makeImm(SIMM(w));
      il.set_ca = true;
      il.rc = (op == 13);  // addic. updates CR0
      emit(il);
      break;
    }

    // =====================================================================
    // mulli (opcode 7)
    // =====================================================================
    case 7: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::Mul;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RA(w));
      il.src2 = makeImm(SIMM(w));
      emit(il);
      break;
    }

    // =====================================================================
    // subfic (opcode 8)
    // =====================================================================
    case 8: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::Sub;
      il.dst = dst;
      il.src1 = makeImm(SIMM(w));
      il.src2 = GetGPR(func, state, RA(w));
      emit(il);
      break;
    }

    // =====================================================================
    // cmpwi / cmpdi (opcode 11)
    // =====================================================================
    case 11: {
      uint32_t crfd = Bits(w, 6, 8);
      il.op = ILOp::CompareImm;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      il.cr_field = (int)crfd;
      // Create compare result value
      ILValue cmp_val = func.AllocValue(ILType::Bool);
      il.dst = cmp_val;
      state.cr[crfd] = cmp_val.id;
      emit(il);
      break;
    }

    // =====================================================================
    // cmplwi / cmpldi (opcode 10)
    // =====================================================================
    case 10: {
      uint32_t crfd = Bits(w, 6, 8);
      il.op = ILOp::CompareImmU;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = (int32_t)UIMM(w);
      il.cr_field = (int)crfd;
      ILValue cmp_val = func.AllocValue(ILType::Bool);
      il.dst = cmp_val;
      state.cr[crfd] = cmp_val.id;
      emit(il);
      break;
    }

    // =====================================================================
    // bc - conditional branch (opcode 16)
    // =====================================================================
    case 16: {
      int32_t bd = SignExt(BD(w) << 2, 16);
      uint32_t target_addr = AA(w) ? (uint32_t)bd : address + bd;
      uint32_t bo = BO(w);
      uint32_t bi = BI(w);
      int crfld = (int)(bi / 4);
      int crbit = (int)(bi % 4);  // 0=LT, 1=GT, 2=EQ, 3=SO

      // Decode BO field for common patterns
      bool decr_ctr = !(bo & 4);
      bool ctr_zero = (bo & 2);
      bool no_cond = (bo & 16);
      bool cond_true = (bo & 8);

      if (decr_ctr && !no_cond) {
        // bdz/bdnz with condition — complex, use BranchCond
        il.op = ctr_zero ? ILOp::BranchDecrCTRZero : ILOp::BranchDecrCTRNonZero;
      } else if (decr_ctr) {
        il.op = ctr_zero ? ILOp::BranchDecrCTRZero : ILOp::BranchDecrCTRNonZero;
      } else if (no_cond) {
        il.op = ILOp::Branch;  // unconditional (bo=20)
      } else {
        il.op = ILOp::BranchCond;
      }

      il.target = target_addr;
      il.cr_field = crfld;

      // Map CR bit to comparison kind
      if (!no_cond) {
        if (cond_true) {
          switch (crbit) {
            case 0: il.cmp_kind = ILCmpKind::LT; break;
            case 1: il.cmp_kind = ILCmpKind::GT; break;
            case 2: il.cmp_kind = ILCmpKind::EQ; break;
            case 3: il.cmp_kind = ILCmpKind::EQ; break; // SO (overflow)
          }
        } else {
          switch (crbit) {
            case 0: il.cmp_kind = ILCmpKind::GE; break;
            case 1: il.cmp_kind = ILCmpKind::LE; break;
            case 2: il.cmp_kind = ILCmpKind::NE; break;
            case 3: il.cmp_kind = ILCmpKind::NE; break;
          }
        }
        il.cmp_ref = state.cr[crfld];
      }

      if (LK(w)) {
        il.op = ILOp::Call;
        il.call_name = GetFunctionName(target_addr);
      }
      emit(il);
      break;
    }

    // =====================================================================
    // b / bl / ba / bla (opcode 18)
    // =====================================================================
    case 18: {
      int32_t li = SignExt(LI(w) << 2, 26);
      uint32_t target_addr = AA(w) ? (uint32_t)li : address + li;

      // In unlinked code the displacement of an external branch is whatever
      // the assembler left behind - typically -address, so it decodes as a
      // branch to offset 0, which is the containing function's own entry. The
      // relocation, not the displacement, is what says where it goes.
      auto rel_it = reloc_map_.find(address);
      bool external = rel_it != reloc_map_.end() && !AA(w);
      uint32_t reloc_target = 0;
      if (external) {
        auto tgt_it = reloc_targets_.find(address);
        if (tgt_it != reloc_targets_.end()) reloc_target = tgt_it->second;
      }

      if (LK(w)) {
        // bl — function call
        il.op = ILOp::Call;
        il.target = target_addr;

        // Check relocation map first
        if (rel_it != reloc_map_.end()) {
          il.call_name = rel_it->second;
          // 0 rather than the instruction's own address: the callee is
          // somewhere else entirely, and a bogus target makes the symbol
          // lookup name the call after whatever function contains it.
          il.target = external ? reloc_target : target_addr;
        } else {
          il.call_name = GetFunctionName(target_addr);
        }

        // Return value: new SSA for r3
        ILValue ret_val = SetGPR(func, state, 3);
        il.dst = ret_val;

        // Invalidate caller-saved registers
        for (uint32_t r : {0u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u}) {
          state.gpr[r] = kILNoValue;
        }

        // Track called function
        if (!il.call_name.empty()) {
          func.called_functions.push_back(il.call_name);
        }
      } else if (external) {
        // A tail call to another object, not a branch inside this function.
        il.op = ILOp::Branch;
        il.target = reloc_target;
        il.call_name = rel_it->second;
      } else {
        // b — unconditional branch
        il.op = ILOp::Branch;
        il.target = target_addr;
      }
      emit(il);
      break;
    }

    // =====================================================================
    // opcode 19: bclr, bcctr, CR ops, isync, rfid
    // =====================================================================
    case 19: {
      uint32_t xo = XO10(w);
      switch (xo) {
        case 16: {  // bclr
          uint32_t bo = BO(w);
          if (bo == 20 && BI(w) == 0) {
            il.op = ILOp::Return;
          } else if (bo & 16) {
            // Unconditional branch to LR
            il.op = ILOp::BranchLR;
          } else {
            il.op = ILOp::BranchCond;
            il.is_cond_return = true;  // conditional bclr = conditional return
            int crfld = (int)(BI(w) / 4);
            int crbit = (int)(BI(w) % 4);
            il.cr_field = crfld;
            il.cmp_ref = state.cr[crfld];
            bool cond_true = (bo & 8);
            if (cond_true) {
              switch (crbit) {
                case 0: il.cmp_kind = ILCmpKind::LT; break;
                case 1: il.cmp_kind = ILCmpKind::GT; break;
                case 2: il.cmp_kind = ILCmpKind::EQ; break;
                default: il.cmp_kind = ILCmpKind::EQ; break;
              }
            } else {
              switch (crbit) {
                case 0: il.cmp_kind = ILCmpKind::GE; break;
                case 1: il.cmp_kind = ILCmpKind::LE; break;
                case 2: il.cmp_kind = ILCmpKind::NE; break;
                default: il.cmp_kind = ILCmpKind::NE; break;
              }
            }
          }
          emit(il);
          break;
        }
        case 528: {  // bcctr / bctrl (including conditional: bnectr, beqctrl, etc.)
          uint32_t bo = BO(w);
          if (bo == 20 || (bo & 16)) {
            // Unconditional branch to CTR
            if (LK(w)) {
              il.op = ILOp::CallCTR;
            } else {
              il.op = ILOp::BranchCTR;
            }
          } else {
            // Conditional branch to CTR — parse BO/BI like bclr
            il.op = ILOp::BranchCond;
            il.is_cond_ctr = true;
            il.is_cond_ctr_link = LK(w) != 0;
            int crfld = (int)(BI(w) / 4);
            int crbit = (int)(BI(w) % 4);
            il.cr_field = crfld;
            il.cmp_ref = state.cr[crfld];
            bool cond_true = (bo & 8);
            if (cond_true) {
              switch (crbit) {
                case 0: il.cmp_kind = ILCmpKind::LT; break;
                case 1: il.cmp_kind = ILCmpKind::GT; break;
                case 2: il.cmp_kind = ILCmpKind::EQ; break;
                default: il.cmp_kind = ILCmpKind::EQ; break;
              }
            } else {
              switch (crbit) {
                case 0: il.cmp_kind = ILCmpKind::GE; break;
                case 1: il.cmp_kind = ILCmpKind::LE; break;
                case 2: il.cmp_kind = ILCmpKind::NE; break;
                default: il.cmp_kind = ILCmpKind::NE; break;
              }
            }
          }
          emit(il);
          break;
        }
        case 150: {  // isync
          il.op = ILOp::ISync;
          emit(il);
          break;
        }
        case 50: {  // rfi
          il.op = ILOp::Return;
          emit(il);
          break;
        }
        // CR logical operations. These carry no SSA operands (they work on
        // individual CR bits), so keep the instruction text — without it the
        // decompiler has nothing to emit and the op vanishes from the output.
        case 257: { il.op = ILOp::CRAnd;  il.asm_text = disasm_text; emit(il); break; }
        case 449: { il.op = ILOp::CROr;   il.asm_text = disasm_text; emit(il); break; }
        case 193: { il.op = ILOp::CRXor;  il.asm_text = disasm_text; emit(il); break; }
        case 225: { il.op = ILOp::CRNand; il.asm_text = disasm_text; emit(il); break; }
        case 33:  { il.op = ILOp::CRNor;  il.asm_text = disasm_text; emit(il); break; }
        case 129: { il.op = ILOp::CRAndc; il.asm_text = disasm_text; emit(il); break; }
        case 417: { il.op = ILOp::CROrc;  il.asm_text = disasm_text; emit(il); break; }
        case 289: { il.op = ILOp::CREqv;  il.asm_text = disasm_text; emit(il); break; }
        case 0:   { il.op = ILOp::MoveCRField; il.asm_text = disasm_text; emit(il); break; } // mcrf
        default: {
          il.op = ILOp::InlineAsm;
          il.asm_text = disasm_text;
          emit(il);
          break;
        }
      }
      break;
    }

    // =====================================================================
    // rlwimi (opcode 20)
    // =====================================================================
    case 20: {
      uint32_t sh = SH(w), mb = MB(w), me = ME(w);
      uint32_t mask = ComputeRlwinmMask(mb, me);
      ILValue dst = SetGPR(func, state, RA(w));
      il.op = ILOp::RotateInsert;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RS(w));
      il.src2 = GetGPR(func, state, RA(w)); // existing value
      il.imm = (int32_t)sh;
      il.mask = mask;
      emit(il);
      break;
    }

    // =====================================================================
    // rlwinm / rlwinm. (opcode 21)
    // =====================================================================
    case 21: {
      uint32_t sh = SH(w), mb = MB(w), me = ME(w);
      uint32_t mask = ComputeRlwinmMask(mb, me);
      ILValue src = GetGPR(func, state, RS(w));
      ILValue dst = SetGPR(func, state, RA(w));

      // Pattern match: pure shift left
      if (sh != 0 && mb == 0 && me == 31 - sh) {
        il.op = ILOp::Shl;
        il.dst = dst;
        il.src1 = src;
        il.imm = (int32_t)sh;
        emit(il);
      }
      // Pattern match: pure shift right (logical)
      else if (sh != 0 && mb == sh && me == 31 && (32 - sh) == sh) {
        // rotl(x, 32-n) & mask(n, 31) = x >> n
        // Actually: sh = 32-n, mb = 32-sh = n
        // So if sh = 32 - mb and me == 31
        il.op = ILOp::Shr;
        il.dst = dst;
        il.src1 = src;
        il.imm = (int32_t)(32 - sh);
        emit(il);
      }
      // Pattern match: pure right shift (general)
      else if (sh != 0 && mb == (32 - sh) && me == 31) {
        il.op = ILOp::Shr;
        il.dst = dst;
        il.src1 = src;
        il.imm = (int32_t)(32 - sh);
        emit(il);
      }
      // Pattern match: pure mask (no rotate)
      else if (sh == 0) {
        il.op = ILOp::And;
        il.dst = dst;
        il.src1 = src;
        il.src2 = makeUImm(mask);
        emit(il);
      }
      // General case: rotate and mask
      else {
        il.op = ILOp::RotateAndMask;
        il.dst = dst;
        il.src1 = src;
        il.imm = (int32_t)sh;
        il.mask = mask;
        emit(il);
      }
      // Record compare update for rlwinm.
      if (w & 1) { // Rc bit
        ILValue cmp_val = func.AllocValue(ILType::Bool);
        ILInstr cmp_il;
        cmp_il.op = ILOp::CompareImm;
        cmp_il.address = address;
        cmp_il.ppc_word = w;
        cmp_il.dst = cmp_val;
        cmp_il.src1 = dst;
        cmp_il.imm = 0;
        cmp_il.cr_field = 0;
        state.cr[0] = cmp_val.id;
        emit(cmp_il);
      }
      break;
    }

    // =====================================================================
    // rlwnm (opcode 23)
    // =====================================================================
    case 23: {
      uint32_t mb = MB(w), me = ME(w);
      uint32_t mask = ComputeRlwinmMask(mb, me);
      ILValue dst = SetGPR(func, state, RA(w));
      il.op = ILOp::RotateAndMaskReg;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RS(w));
      il.src2 = GetGPR(func, state, RB(w));
      il.mask = mask;
      emit(il);
      break;
    }

    // =====================================================================
    // 64-bit rotate/shift (opcode 30): rldicl, rldicr, rldic, rldimi, rldcl, rldcr
    // MD-form: SH = bit30<<5 | bits16-20, MB = bit26<<5 | bits21-25
    // =====================================================================
    case 30: {
      uint32_t xop = Bits(w, 27, 29);  // sub-opcode in PPC bits 27-29
      uint32_t sh = ((w >> 1) & 0x20) | ((w >> 11) & 0x1F);  // SH5<<5 | SH
      uint32_t mb = ((w >> 5) & 0x20) | ((w >> 6) & 0x1F);   // MB5<<5 | MB

      ILValue src = GetGPR(func, state, RS(w));
      ILValue dst = SetGPR(func, state, RA(w));

      switch (xop) {
        case 0: { // rldicl: rotate left, mask from mb to 63
          // Common case: srdi (sh == 64-mb) → shift right by mb
          il.op = ILOp::RotateAndMask;
          il.dst = dst; il.src1 = src;
          il.imm = (int32_t)sh;
          // Compute 64-bit mask MASK(mb, 63), store low 32 bits
          // For codegen: emit as 64-bit rotate + AND with mask
          il.mask = mb | 0x20000000;  // Signal "rldicl" to codegen
          il.asm_text = "rldicl";
          break;
        }
        case 1: { // rldicr: rotate left, mask from 0 to mb(me)
          il.op = ILOp::RotateAndMask;
          il.dst = dst; il.src1 = src;
          il.imm = (int32_t)sh;
          il.mask = mb | 0x80000000;  // High bit signals "rldicr" to codegen
          il.asm_text = "rldicr";
          break;
        }
        case 2: { // rldic: rotate left, mask from mb to 63-sh
          il.op = ILOp::RotateAndMask;
          il.dst = dst; il.src1 = src;
          il.imm = (int32_t)sh;
          il.mask = mb | 0x40000000;  // Signal "rldic"
          il.asm_text = "rldic";
          break;
        }
        case 3: { // rldimi: rotate left, insert into RA with mask
          il.op = ILOp::RotateInsert;
          il.dst = dst; il.src1 = src;
          il.src2 = GetGPR(func, state, RA(w));  // existing RA value
          il.imm = (int32_t)sh;
          il.mask = mb | 0x40000000;  // Signal "rldimi" (64-bit)
          il.asm_text = "rldimi";
          break;
        }
        case 4: { // rldcl: rotate left by register, mask from mb to 63
          il.op = ILOp::RotateAndMaskReg;
          il.dst = dst; il.src1 = src;
          il.src2 = GetGPR(func, state, RB(w));  // shift amount in register
          il.mask = mb | 0x20000000;  // Signal "rldcl" (64-bit, mask mb..63)
          il.asm_text = "rldcl";
          break;
        }
        case 5: { // rldcr: rotate left by register, mask from 0 to mb
          il.op = ILOp::RotateAndMaskReg;
          il.dst = dst; il.src1 = src;
          il.src2 = GetGPR(func, state, RB(w));  // shift amount in register
          il.mask = mb | 0x80000000;  // Signal "rldcr" (64-bit, mask 0..mb)
          il.asm_text = "rldcr";
          break;
        }
        default: {
          il.op = ILOp::InlineAsm;
          il.asm_text = disasm_text;
          break;
        }
      }
      il.rc = (w & 1) != 0;
      emit(il);
      break;
    }

    // =====================================================================
    // ori / nop (opcode 24)
    // =====================================================================
    case 24: {
      if (RS(w) == 0 && RA(w) == 0 && UIMM(w) == 0) {
        il.op = ILOp::Nop;
        emit(il);
      } else {
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Or;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = makeUImm(UIMM(w));
        emit(il);
      }
      break;
    }

    // =====================================================================
    // oris (opcode 25)
    // =====================================================================
    case 25: {
      ILValue dst = SetGPR(func, state, RA(w));
      il.op = ILOp::Or;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RS(w));
      il.src2 = makeUImm((uint32_t)UIMM(w) << 16);
      emit(il);
      break;
    }

    // =====================================================================
    // xori (opcode 26), xoris (opcode 27)
    // =====================================================================
    case 26: {
      ILValue dst = SetGPR(func, state, RA(w));
      il.op = ILOp::Xor;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RS(w));
      il.src2 = makeUImm(UIMM(w));
      emit(il);
      break;
    }
    case 27: {
      ILValue dst = SetGPR(func, state, RA(w));
      il.op = ILOp::Xor;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RS(w));
      il.src2 = makeUImm((uint32_t)UIMM(w) << 16);
      emit(il);
      break;
    }

    // =====================================================================
    // andi. (opcode 28), andis. (opcode 29)
    // =====================================================================
    case 28: {
      ILValue dst = SetGPR(func, state, RA(w));
      il.op = ILOp::And;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RS(w));
      il.src2 = makeUImm(UIMM(w));
      emit(il);
      // andi. always sets cr0
      ILValue cmp_val = func.AllocValue(ILType::Bool);
      ILInstr cmp_il;
      cmp_il.op = ILOp::CompareImm;
      cmp_il.address = address;
      cmp_il.ppc_word = w;
      cmp_il.dst = cmp_val;
      cmp_il.src1 = dst;
      cmp_il.imm = 0;
      cmp_il.cr_field = 0;
      state.cr[0] = cmp_val.id;
      emit(cmp_il);
      break;
    }
    case 29: {
      ILValue dst = SetGPR(func, state, RA(w));
      il.op = ILOp::And;
      il.dst = dst;
      il.src1 = GetGPR(func, state, RS(w));
      il.src2 = makeUImm((uint32_t)UIMM(w) << 16);
      emit(il);
      ILValue cmp_val = func.AllocValue(ILType::Bool);
      ILInstr cmp_il;
      cmp_il.op = ILOp::CompareImm;
      cmp_il.address = address;
      cmp_il.ppc_word = w;
      cmp_il.dst = cmp_val;
      cmp_il.src1 = dst;
      cmp_il.imm = 0;
      cmp_il.cr_field = 0;
      state.cr[0] = cmp_val.id;
      emit(cmp_il);
      break;
    }

    // =====================================================================
    // lwz (32), lwzu (33), lbz (34), lbzu (35)
    // =====================================================================
    case 32: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::Load;
      il.dst = dst;
      il.access_type = ILType::U32;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 33: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::LoadUpdate;
      il.dst = dst;
      il.access_type = ILType::U32;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      // Update RA
      SetGPR(func, state, RA(w));
      break;
    }
    case 34: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::Load;
      il.dst = dst;
      il.access_type = ILType::U8;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      dst.type = ILType::U8;
      emit(il);
      break;
    }
    case 35: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::LoadUpdate;
      il.dst = dst;
      il.access_type = ILType::U8;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }

    // =====================================================================
    // stw (36), stwu (37), stb (38), stbu (39)
    // =====================================================================
    case 36: {
      il.op = ILOp::Store;
      il.access_type = ILType::U32;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.src2 = GetGPR(func, state, RS(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 37: {
      // stwu — check for stack frame setup
      if (RS(w) == 1 && RA(w) == 1) {
        il.op = ILOp::StackAlloc;
        il.imm = -SIMM(w);  // positive size
        emit(il);
      } else {
        il.op = ILOp::StoreUpdate;
        il.access_type = ILType::U32;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RS(w));
        il.imm = SIMM(w);
        emit(il);
        SetGPR(func, state, RA(w));
      }
      break;
    }
    case 38: {
      il.op = ILOp::Store;
      il.access_type = ILType::U8;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.src2 = GetGPR(func, state, RS(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 39: {
      il.op = ILOp::StoreUpdate;
      il.access_type = ILType::U8;
      il.src1 = GetGPR(func, state, RA(w));
      il.src2 = GetGPR(func, state, RS(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }

    // =====================================================================
    // lhz (40), lhzu (41), lha (42)
    // =====================================================================
    case 40: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::Load;
      il.dst = dst;
      il.access_type = ILType::U16;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 41: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::LoadUpdate;
      il.dst = dst;
      il.access_type = ILType::U16;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }
    case 42: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::Load;
      il.dst = dst;
      il.access_type = ILType::S16;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }

    // =====================================================================
    // lhau (opcode 43)
    // =====================================================================
    case 43: {
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = ILOp::LoadUpdate;
      il.dst = dst;
      il.access_type = ILType::S16;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }

    // =====================================================================
    // sth (44), sthu (45)
    // =====================================================================
    case 44: {
      il.op = ILOp::Store;
      il.access_type = ILType::U16;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.src2 = GetGPR(func, state, RS(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 45: {
      il.op = ILOp::StoreUpdate;
      il.access_type = ILType::U16;
      il.src1 = GetGPR(func, state, RA(w));
      il.src2 = GetGPR(func, state, RS(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }

    // =====================================================================
    // lmw (46), stmw (47)
    // =====================================================================
    case 46: {
      il.op = ILOp::LoadMultiple;
      il.dst.ppc_reg = (int)RD(w);
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      // Mark all loaded registers as new SSA values
      for (uint32_t r = RD(w); r < 32; r++) {
        SetGPR(func, state, r);
      }
      emit(il);
      break;
    }
    case 47: {
      il.op = ILOp::StoreMultiple;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      il.dst.ppc_reg = (int)RS(w);
      emit(il);
      break;
    }

    // =====================================================================
    // lfs (48), lfsu (49), lfd (50), lfdu (51)
    // =====================================================================
    case 48: {
      ILValue dst = SetFPR(func, state, FRD(w));
      il.op = ILOp::FLoad;
      il.dst = dst;
      il.access_type = ILType::F32;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 49: {
      ILValue dst = SetFPR(func, state, FRD(w));
      il.op = ILOp::FLoadUpdate;
      il.dst = dst;
      il.access_type = ILType::F32;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }
    case 50: {
      ILValue dst = SetFPR(func, state, FRD(w));
      il.op = ILOp::FLoad;
      il.dst = dst;
      il.access_type = ILType::F64;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 51: {
      ILValue dst = SetFPR(func, state, FRD(w));
      il.op = ILOp::FLoadUpdate;
      il.dst = dst;
      il.access_type = ILType::F64;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }

    // =====================================================================
    // stfs (52), stfsu (53), stfd (54), stfdu (55)
    // =====================================================================
    case 52: {
      il.op = ILOp::FStore;
      il.access_type = ILType::F32;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.src2 = GetFPR(func, state, FRD(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 53: {
      il.op = ILOp::FStoreUpdate;
      il.access_type = ILType::F32;
      il.src1 = GetGPR(func, state, RA(w));
      il.src2 = GetFPR(func, state, FRD(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }
    case 54: {
      il.op = ILOp::FStore;
      il.access_type = ILType::F64;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.src2 = GetFPR(func, state, FRD(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }
    case 55: {
      il.op = ILOp::FStoreUpdate;
      il.access_type = ILType::F64;
      il.src1 = GetGPR(func, state, RA(w));
      il.src2 = GetFPR(func, state, FRD(w));
      il.imm = SIMM(w);
      emit(il);
      SetGPR(func, state, RA(w));
      break;
    }

    // =====================================================================
    // sc - system call (opcode 17)
    // =====================================================================
    case 17: {
      il.op = ILOp::Syscall;
      emit(il);
      break;
    }

    // =====================================================================
    // VMX / AltiVec (opcode 4) and VMX128 extensions (opcode 5, 6)
    // Xbox 360 also uses opcodes 22 for VMX128 load/store
    // =====================================================================
    case 4: {
      // Xbox 360 VMX128: VX128_1 form uses xop mask 0x7F3
      uint32_t xop = w & 0x7F3;
      uint32_t vd128l = (w >> 21) & 0x1F;
      uint32_t vd128h = (w >> 2) & 0x3;
      uint32_t vd128 = vd128l | (vd128h << 5);
      uint32_t ra_field = (w >> 16) & 0x1F;
      uint32_t rb_field = (w >> 11) & 0x1F;

      if (xop == 0x0C3) { // lvx128
        il.op = ILOp::VMXLoad;
        il.imm = (int32_t)vd128;
        il.src1 = (ra_field == 0) ? ILNoVal() : GetGPR(func, state, ra_field);
        il.src2 = GetGPR(func, state, rb_field);
        emit(il); break;
      }
      if (xop == 0x1C3) { // stvx128
        il.op = ILOp::VMXStore;
        il.imm = (int32_t)vd128;
        il.src1 = (ra_field == 0) ? ILNoVal() : GetGPR(func, state, ra_field);
        il.src2 = GetGPR(func, state, rb_field);
        emit(il); break;
      }
      // VMX128 partial loads/stores — handle as VMXOp with ppc_word
      // lvlx128 (0x403), lvrx128 (0x443), lvlxl128 (0x603), lvrxl128 (0x643)
      // stvlx128 (0x503), stvrx128 (0x543), stvlxl128 (0x703), stvrxl128 (0x743)
      // stvewx128 (0x183), lvewx128 (0x083), lvxl128 (0x2C3), stvxl128 (0x3C3)
      // lvsl128 (0x003), lvsr128 (0x043)
      // All other opcode 4 VMX — keep as generic VMXOp
      il.op = ILOp::VMXOp;
      il.asm_text = disasm_text;
      emit(il);
      break;
    }
    case 5:
    case 6: {
      // Xbox 360 VMX128 extensions — keep as generic VMXOp for now
      il.op = ILOp::VMXOp;
      il.asm_text = disasm_text;
      emit(il);
      break;
    }

    // =====================================================================
    // opcode 0: not a real instruction — import thunks / embedded data
    // =====================================================================
    case 0: {
      il.op = ILOp::Nop;
      emit(il);
      break;
    }

    // =====================================================================
    // opcode 1: not a real instruction — import thunks / embedded data
    // =====================================================================
    case 1: {
      il.op = ILOp::Nop;
      emit(il);
      break;
    }

    // =====================================================================
    // tdi - trap doubleword immediate (opcode 2)
    // =====================================================================
    case 2: {
      il.op = ILOp::TrapImm;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }

    // =====================================================================
    // twi - trap word immediate (opcode 3)
    // =====================================================================
    case 3: {
      il.op = ILOp::TrapImm;
      il.src1 = GetGPR(func, state, RA(w));
      il.imm = SIMM(w);
      emit(il);
      break;
    }

    // =====================================================================
    // opcode 31: X-form / XO-form (ALU, SPR, cache, etc.)
    // =====================================================================
    case 31: {
      uint32_t xo10 = XO10(w);
      uint32_t xo9 = XO9(w);

      // Set Rc for ALL opcode-31 instructions (bit 0).
      // Individual handlers may override. Codegen emits CR0 update if il.rc is set.
      il.rc = (w & 1) != 0;

      // ---- SPR moves ----
      if (xo10 == 339) {  // mfspr
        uint32_t spr_val = SPR(w);
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::MoveSPR;
        il.dst = dst;
        il.spr_read = true;
        if (spr_val == 8) { il.spr = ILSPR::LR; }
        else if (spr_val == 9) { il.spr = ILSPR::CTR; }
        else if (spr_val == 1) { il.spr = ILSPR::XER; }
        else if (spr_val == 256) { il.spr = ILSPR::VRSAVE; }
        else if (spr_val == 287) { il.spr = ILSPR::PVR; }
        else { il.spr = ILSPR::Other; il.imm = (int32_t)spr_val; }
        emit(il);
        break;
      }
      if (xo10 == 371) {  // mftb
        uint32_t tbr = SPR(w);  // TBR uses same encoding as SPR
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadTimeBase;
        il.dst = dst;
        il.imm = (int32_t)tbr;  // 268 = TB (lower/full), 269 = TBU (upper)
        emit(il);
        break;
      }
      if (xo10 == 467) {  // mtspr
        uint32_t spr_val = SPR(w);
        il.op = ILOp::MoveSPR;
        il.src1 = GetGPR(func, state, RS(w));
        il.spr_read = false;
        if (spr_val == 8) {
          il.spr = ILSPR::LR;
          state.lr = il.src1.id;
        } else if (spr_val == 9) {
          il.spr = ILSPR::CTR;
          state.ctr = il.src1.id;
        } else if (spr_val == 1) {
          il.spr = ILSPR::XER;
        } else if (spr_val == 256) {
          il.spr = ILSPR::VRSAVE;
        } else {
          il.spr = ILSPR::Other;
          il.imm = (int32_t)spr_val;
        }
        emit(il);
        break;
      }

      // ---- mfcr (xo10=19) ----
      if (xo10 == 19) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::MoveCR;
        il.dst = dst;
        il.spr_read = true;
        emit(il);
        break;
      }
      // ---- mtcrf (xo10=144) ----
      if (xo10 == 144) {
        il.op = ILOp::MoveCR;
        il.src1 = GetGPR(func, state, RS(w));
        il.spr_read = false;
        il.mask = CRM(w);
        emit(il);
        break;
      }

      // ---- mfmsr (xo10=83), mtmsr (xo10=146), mtmsrd (xo10=178) ----
      if (xo10 == 83) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::MoveMSR;
        il.dst = dst;
        il.spr_read = true;
        emit(il);
        break;
      }
      if (xo10 == 146 || xo10 == 178) {
        il.op = ILOp::MoveMSR;
        il.src1 = GetGPR(func, state, RS(w));
        il.spr_read = false;
        emit(il);
        break;
      }

      // ---- Compare (xo10=0: cmp, xo10=32: cmpl) ----
      if (xo10 == 0) {
        uint32_t crfd = Bits(w, 6, 8);
        ILValue cmp_val = func.AllocValue(ILType::Bool);
        il.op = ILOp::Compare;
        il.dst = cmp_val;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.cr_field = (int)crfd;
        state.cr[crfd] = cmp_val.id;
        emit(il);
        break;
      }
      if (xo10 == 32) {
        uint32_t crfd = Bits(w, 6, 8);
        ILValue cmp_val = func.AllocValue(ILType::Bool);
        il.op = ILOp::CompareU;
        il.dst = cmp_val;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.cr_field = (int)crfd;
        state.cr[crfd] = cmp_val.id;
        emit(il);
        break;
      }

      // ---- or/mr (xo10=444) ----
      if (xo10 == 444) {
        ILValue dst = SetGPR(func, state, RA(w));
        if (RS(w) == RB(w)) {
          il.op = ILOp::Assign;
          il.dst = dst;
          il.src1 = GetGPR(func, state, RS(w));
        } else {
          il.op = ILOp::Or;
          il.dst = dst;
          il.src1 = GetGPR(func, state, RS(w));
          il.src2 = GetGPR(func, state, RB(w));
        }
        emit(il);
        break;
      }

      // ---- Logic X-form ----
      if (xo10 == 28) { // and
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::And;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 60) { // andc
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Andc;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 316) { // xor
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Xor;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 476) { // nand
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Nand;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 124) { // nor / not
        ILValue dst = SetGPR(func, state, RA(w));
        if (RS(w) == RB(w)) {
          il.op = ILOp::Not;
          il.dst = dst;
          il.src1 = GetGPR(func, state, RS(w));
        } else {
          il.op = ILOp::Nor;
          il.dst = dst;
          il.src1 = GetGPR(func, state, RS(w));
          il.src2 = GetGPR(func, state, RB(w));
        }
        emit(il); break;
      }
      if (xo10 == 412) { // orc
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Orc;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 284) { // eqv
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Eqv;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }

      // ---- Shift X-form ----
      if (xo10 == 24) { // slw
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Shl;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 536) { // srw
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Shr;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 792) { // sraw — sets CA
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Sar;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.set_ca = true;
        il.rc = (w & 1) != 0;
        emit(il); break;
      }
      if (xo10 == 824) { // srawi — sets CA
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::SarImm;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.imm = (int32_t)SH(w);
        il.set_ca = true;
        il.rc = (w & 1) != 0;
        emit(il); break;
      }

      // ---- cntlzw (xo10=26) ----
      if (xo10 == 26) {
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::CountLeadingZeros;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        emit(il); break;
      }

      // ---- extsb (xo10=954), extsh (xo10=922), extsw (xo10=986) ----
      if (xo10 == 954) {
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::ExtendByte;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        emit(il); break;
      }
      if (xo10 == 922) {
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::ExtendHalf;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        emit(il); break;
      }
      if (xo10 == 986) {
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::ExtendWord;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        emit(il); break;
      }

      // ---- Indexed load/store ----
      if (xo10 == 23) { // lwzx
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 87) { // lbzx
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U8;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 279) { // lhzx
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U16;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 343) { // lhax
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::S16;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 341) { // lwax
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::S32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 151) { // stwx
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }
      if (xo10 == 215) { // stbx
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U8;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }
      if (xo10 == 407) { // sthx
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U16;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }

      // ---- Atomic: lwarx (xo10=20), stwcx. (xo10=150) ----
      if (xo10 == 20) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadReserved;
        il.dst = dst;
        il.access_type = ILType::U32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 150) {
        il.op = ILOp::StoreConditional;
        il.access_type = ILType::U32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }

      // ---- Byte-reverse: lwbrx (xo10=534), stwbrx (xo10=662) ----
      if (xo10 == 534) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadByteReverse;
        il.dst = dst;
        il.access_type = ILType::U32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 790) { // lhbrx
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadByteReverse;
        il.dst = dst;
        il.access_type = ILType::U16;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 662) {
        il.op = ILOp::StoreByteReverse;
        il.access_type = ILType::U32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }
      if (xo10 == 918) { // sthbrx
        il.op = ILOp::StoreByteReverse;
        il.access_type = ILType::U16;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }

      // ---- FP indexed load/store ----
      if (xo10 == 535) { // lfsx
        ILValue dst = SetFPR(func, state, FRD(w));
        il.op = ILOp::FLoadIndexed;
        il.dst = dst;
        il.access_type = ILType::F32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 599) { // lfdx
        ILValue dst = SetFPR(func, state, FRD(w));
        il.op = ILOp::FLoadIndexed;
        il.dst = dst;
        il.access_type = ILType::F64;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 663) { // stfsx
        il.op = ILOp::FStoreIndexed;
        il.access_type = ILType::F32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetFPR(func, state, FRD(w));
        emit(il); break;
      }
      if (xo10 == 727) { // stfdx
        il.op = ILOp::FStoreIndexed;
        il.access_type = ILType::F64;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetFPR(func, state, FRD(w));
        emit(il); break;
      }

      // ---- stfiwx (xo10=983) ----
      if (xo10 == 983) {
        il.op = ILOp::FStoreIntWord;
        il.access_type = ILType::U32;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetFPR(func, state, FRD(w));
        emit(il); break;
      }

      // ---- 64-bit indexed load/store: ldx (xo10=21), stdx (xo10=149) ----
      if (xo10 == 21) { // ldx
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U64;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 149) { // stdx
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U64;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }
      // ---- stwux (xo10=183) ----
      if (xo10 == 183) {
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U32;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il);
        SetGPR(func, state, RA(w)); // update
        break;
      }

      // ---- 64-bit divide: divdu (xo9=457), divd (xo9=489) ----
      if (xo10 == 457 || (xo10 >= 456 && xo10 <= 458)) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::DivU;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }

      // ---- 64-bit multiply: mulld (xo9=233), mulhdu (xo9=9) ----
      if (xo9 == 233 || xo9 == 233+512) { // mulld, mulldo
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::Mul;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo9 == 9) { // mulhdu
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::MulHighU;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      // ---- 64-bit divide: divd (xo9=489), divdo (xo9=489+512) ----
      if (xo9 == 489 || xo9 == 489+512) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::Div;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }

      // ---- 64-bit shifts ----
      if (xo10 == 27) { // sld
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Shl;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 794) { // srad
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Sar;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 539) { // srd
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::Shr;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }

      // ---- cntlzd (xo10=58) ----
      if (xo10 == 58) {
        ILValue dst = SetGPR(func, state, RA(w));
        il.op = ILOp::CountLeadingZeros;
        il.dst = dst;
        il.src1 = GetGPR(func, state, RS(w));
        emit(il); break;
      }

      // ---- ldarx (xo10=84), stdcx. (xo10=214) ----
      if (xo10 == 84) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadReserved;
        il.dst = dst;
        il.access_type = ILType::U64;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 214) {
        il.op = ILOp::StoreConditional;
        il.access_type = ILType::U64;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il); break;
      }

      // ---- Update-indexed load/store variants ----
      if (xo10 == 119) { // lbzux
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U8;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il);
        SetGPR(func, state, RA(w));
        break;
      }
      if (xo10 == 311) { // lhzux
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U16;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il);
        SetGPR(func, state, RA(w));
        break;
      }
      if (xo10 == 53) { // ldux
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U64;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il);
        SetGPR(func, state, RA(w));
        break;
      }
      if (xo10 == 247) { // stbux
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U8;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il);
        SetGPR(func, state, RA(w));
        break;
      }
      if (xo10 == 181) { // stdux
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U64;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il);
        SetGPR(func, state, RA(w));
        break;
      }

      // ---- FP update-indexed: lfsux (xo10=567), stfsux (xo10=695) ----
      if (xo10 == 567) {
        ILValue dst = SetFPR(func, state, FRD(w));
        il.op = ILOp::FLoadIndexed;
        il.dst = dst;
        il.access_type = ILType::F32;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il);
        SetGPR(func, state, RA(w));
        break;
      }
      if (xo10 == 695) {
        il.op = ILOp::FStoreIndexed;
        il.access_type = ILType::F32;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetFPR(func, state, FRD(w));
        emit(il);
        SetGPR(func, state, RA(w));
        break;
      }

      // ---- lhaux (xo10=375): Load Halfword Algebraic with Update Indexed ----
      if (xo10 == 375) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::S16;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il);
        SetGPR(func, state, RA(w));
        ILInstr upd = {};
        upd.op = ILOp::Add; upd.dst = SetGPR(func, state, RA(w));
        upd.src1 = GetGPR(func, state, RA(w)); upd.src2 = GetGPR(func, state, RB(w));
        upd.address = address; emit(upd);
        break;
      }
      // ---- sthux (xo10=439): Store Halfword with Update Indexed ----
      if (xo10 == 439) {
        il.op = ILOp::StoreIndexed;
        il.access_type = ILType::U16;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetGPR(func, state, RS(w));
        emit(il);
        SetGPR(func, state, RA(w));
        ILInstr upd = {};
        upd.op = ILOp::Add; upd.dst = SetGPR(func, state, RA(w));
        upd.src1 = GetGPR(func, state, RA(w)); upd.src2 = GetGPR(func, state, RB(w));
        upd.address = address; emit(upd);
        break;
      }
      // ---- lfdux (xo10=631): Load Float Double with Update Indexed ----
      if (xo10 == 631) {
        ILValue dst = SetFPR(func, state, FRD(w));
        il.op = ILOp::FLoadIndexed;
        il.dst = dst;
        il.access_type = ILType::F64;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il);
        SetGPR(func, state, RA(w));
        ILInstr upd = {};
        upd.op = ILOp::Add; upd.dst = SetGPR(func, state, RA(w));
        upd.src1 = GetGPR(func, state, RA(w)); upd.src2 = GetGPR(func, state, RB(w));
        upd.address = address; emit(upd);
        break;
      }
      // ---- stfdux (xo10=759): Store Float Double with Update Indexed ----
      if (xo10 == 759) {
        il.op = ILOp::FStoreIndexed;
        il.access_type = ILType::F64;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        il.src3 = GetFPR(func, state, FRD(w));
        emit(il);
        SetGPR(func, state, RA(w));
        ILInstr upd = {};
        upd.op = ILOp::Add; upd.dst = SetGPR(func, state, RA(w));
        upd.src1 = GetGPR(func, state, RA(w)); upd.src2 = GetGPR(func, state, RB(w));
        upd.address = address; emit(upd);
        break;
      }

      // ---- lwzux (xo10=55): Load Word and Zero with Update Indexed ----
      if (xo10 == 55) {
        ILValue dst = SetGPR(func, state, RD(w));
        il.op = ILOp::LoadIndexed;
        il.dst = dst;
        il.access_type = ILType::U32;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il);
        SetGPR(func, state, RA(w));
        // Emit base register update: rA = rA + rB
        ILInstr upd = {};
        upd.op = ILOp::Add;
        upd.dst = SetGPR(func, state, RA(w));
        upd.src1 = GetGPR(func, state, RA(w));
        upd.src2 = GetGPR(func, state, RB(w));
        upd.address = address;
        emit(upd);
        break;
      }

      // ---- VMX vector load/store (opcode 31) ----
      if (xo10 == 103) { // lvx vD, rA, rB
        il.op = ILOp::VMXLoad;
        il.imm = RD(w);  // VD (vector register 0-31)
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 231) { // stvx vS, rA, rB
        il.op = ILOp::VMXStore;
        il.imm = RD(w);  // VS (vector register 0-31)
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 359) { // lvxl (same as lvx for emulation)
        il.op = ILOp::VMXLoad;
        il.imm = RD(w);
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 487) { // stvxl (same as stvx for emulation)
        il.op = ILOp::VMXStore;
        il.imm = RD(w);
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      // lvsl, lvsr — leave as VMXOp for now
      if (xo10 == 6 || xo10 == 38) {
        il.op = ILOp::VMXOp;
        il.asm_text = disasm_text;
        emit(il); break;
      }
      // ---- VMX128 / Xbox 360 extensions ----
      // stvx128 (xo10=231+256=487? or separate)
      // Xbox 360 VMX128 load/store use xo10 values:
      // 827 = stvx128, 519 = lvx128, 826 = stvlx128, etc.
      if (xo10 == 519) { // lvx128
        uint32_t vd128 = ((w >> 21) & 0x1F) | (((w >> 0) & 0x3) << 5); // VD128
        il.op = ILOp::VMXLoad;
        il.imm = (int32_t)vd128;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 827) { // stvx128
        uint32_t vs128 = ((w >> 21) & 0x1F) | (((w >> 0) & 0x3) << 5); // VS128
        il.op = ILOp::VMXStore;
        il.imm = (int32_t)vs128;
        il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }
      if (xo10 == 826) { // stvlx128
        il.op = ILOp::VMXOp;
        il.asm_text = disasm_text;
        emit(il); break;
      }
      // Other VMX128 extensions — keep as VMXOp for now
      if (xo10 == 647 || xo10 == 679 || xo10 == 711 || xo10 == 199 ||
          xo10 == 539 || xo10 == 603) {
        il.op = ILOp::VMXOp;
        il.asm_text = disasm_text;
        emit(il); break;
      }

      // ---- Cache / sync instructions ----
      if (xo10 == 86)  { il.op = ILOp::DCacheFlush; emit(il); break; }
      if (xo10 == 54)  { il.op = ILOp::DCacheStore; emit(il); break; }
      if (xo10 == 278) { il.op = ILOp::DCacheTouch; emit(il); break; }
      if (xo10 == 246) { il.op = ILOp::DCacheTouchStore; emit(il); break; }
      if (xo10 == 1014){ il.op = ILOp::DCacheZero; emit(il); break; }
      if (xo10 == 982) { il.op = ILOp::ICacheInvalidate; emit(il); break; }
      if (xo10 == 598) { // sync / lwsync / ptesync
        uint32_t l = Bits(w, 9, 10);
        if (l == 1) il.op = ILOp::LWSync;
        else if (l == 2) il.op = ILOp::PTESync;
        else il.op = ILOp::Sync;
        emit(il); break;
      }
      if (xo10 == 854) { il.op = ILOp::EIEIO; emit(il); break; }

      // ---- trap: tw (xo10=4) ----
      if (xo10 == 4) {
        il.op = ILOp::Trap;
        il.src1 = GetGPR(func, state, RA(w));
        il.src2 = GetGPR(func, state, RB(w));
        emit(il); break;
      }

      // ---- XO-form ALU (use xo9 field) ----
      switch (xo9) {
        // Helper: Rc bit (bit 0) sets CR0, OE bit (+512) sets XER[OV/SO]
        #define ALU_FLAGS() \
          il.rc = (w & 1) != 0; \
          il.set_ov = (xo9 >= 512)

        case 266: case 266+512: { // add, addo
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::Add; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          ALU_FLAGS(); emit(il); break;
        }
        case 10: case 10+512: { // addc, addco — rD=rA+rB, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::Add; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 138: case 138+512: { // adde, addeo — rD=rA+rB+CA, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::AddCarry; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 234: case 234+512: { // addme, addmeo — rD=rA+CA-1, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::AddCarry; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = makeImm(-1);
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 202: case 202+512: { // addze, addzeo — rD=rA+CA, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::AddCarry; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = makeImm(0);
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 40: case 40+512: { // subf, subfo — rD=rB-rA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::Sub; il.dst = dst;
          il.src1 = GetGPR(func, state, RB(w));
          il.src2 = GetGPR(func, state, RA(w));
          ALU_FLAGS(); emit(il); break;
        }
        case 8: case 8+512: { // subfc, subfco — rD=~rA+rB+1, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::Sub; il.dst = dst;
          il.src1 = GetGPR(func, state, RB(w));
          il.src2 = GetGPR(func, state, RA(w));
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 136: case 136+512: { // subfe, subfeo — rD=~rA+rB+CA, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::SubCarry; il.dst = dst;
          il.src1 = GetGPR(func, state, RB(w));
          il.src2 = GetGPR(func, state, RA(w));
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 232: case 232+512: { // subfme, subfmeo — rD=~rA+CA-1, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::SubCarry; il.dst = dst;
          il.src1 = makeImm(-1);
          il.src2 = GetGPR(func, state, RA(w));
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 200: case 200+512: { // subfze, subfzeo — rD=~rA+CA, set CA
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::SubCarry; il.dst = dst;
          il.src1 = makeImm(0);
          il.src2 = GetGPR(func, state, RA(w));
          il.set_ca = true; ALU_FLAGS(); emit(il); break;
        }
        case 104: case 104+512: { // neg, nego
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::Neg; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          ALU_FLAGS(); emit(il); break;
        }
        case 75: case 75+512: { // mulhw
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::MulHigh; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          il.rc = (w & 1) != 0; emit(il); break;
        }
        case 11: { // mulhwu
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::MulHighU; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          il.rc = (w & 1) != 0; emit(il); break;
        }
        case 235: case 235+512: { // mullw, mullwo
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::Mul; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          ALU_FLAGS(); emit(il); break;
        }
        case 491: case 491+512: { // divw, divwo
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::Div; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          ALU_FLAGS(); emit(il); break;
        }
        case 459: case 459+512: { // divwu, divwuo
          ILValue dst = SetGPR(func, state, RD(w));
          il.op = ILOp::DivU; il.dst = dst;
          il.src1 = GetGPR(func, state, RA(w));
          il.src2 = GetGPR(func, state, RB(w));
          ALU_FLAGS(); emit(il); break;
        }
        #undef ALU_FLAGS
        default: {
          // Fallback for any opcode-31 instruction not explicitly handled
          il.op = ILOp::InlineAsm;
          il.asm_text = disasm_text;
          emit(il);
          break;
        }
      }
      break;
    }

    // =====================================================================
    // Floating point: opcodes 59 (single) and 63 (double)
    // =====================================================================
    case 59: { // A-form single-precision
      uint32_t xo5 = Bits(w, 26, 30);
      ILValue dst = SetFPR(func, state, FRD(w));
      switch (xo5) {
        case 18: { // fdivs
          il.op = ILOp::FDiv;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 20: { // fsubs
          il.op = ILOp::FSub;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 21: { // fadds
          il.op = ILOp::FAdd;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 24: { // fres
          il.op = ILOp::FRecipEst;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 25: { // fmuls
          il.op = ILOp::FMul;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 22: { // fsqrts
          il.op = ILOp::FSqrt;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 26: { // frsqrte
          il.op = ILOp::FRSqrtEst;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 28: { // fmsubs
          il.op = ILOp::FMulSub;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 29: { // fmadds
          il.op = ILOp::FMulAdd;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 30: { // fnmsubs
          il.op = ILOp::FNMulSub;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        case 31: { // fnmadds
          il.op = ILOp::FNMulAdd;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F32;
          emit(il); break;
        }
        default: {
          il.op = ILOp::InlineAsm;
          il.asm_text = disasm_text;
          emit(il); break;
        }
      }
      break;
    }

    case 63: { // X-form / A-form double-precision
      uint32_t xo10 = XO10(w);
      uint32_t xo5 = Bits(w, 26, 30);

      // X-form FP instructions (xo10 field)
      switch (xo10) {
        case 0: { // fcmpu
          uint32_t crfd = Bits(w, 6, 8);
          ILValue cmp_val = func.AllocValue(ILType::Bool);
          il.op = ILOp::FCompareUnordered;
          il.dst = cmp_val;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.cr_field = (int)crfd;
          state.cr[crfd] = cmp_val.id;
          emit(il);
          return;
        }
        case 32: { // fcmpo
          uint32_t crfd = Bits(w, 6, 8);
          ILValue cmp_val = func.AllocValue(ILType::Bool);
          il.op = ILOp::FCompareOrdered;
          il.dst = cmp_val;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.cr_field = (int)crfd;
          state.cr[crfd] = cmp_val.id;
          emit(il);
          return;
        }
        case 40: { // fneg
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FNeg;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 72: { // fmr
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FMove;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 264: { // fabs
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FAbs;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 136: { // fnabs
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FNAbs;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 12: { // frsp
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FRoundSingle;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 14: { // fctiw
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FConvToIntWord;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 15: { // fctiwz
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FConvToIntWordTrunc;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 814: { // fctid
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FConvToIntDbl;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 815: { // fctidz
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FConvToIntDblTrunc;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 846: { // fcfid
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::FConvFromInt;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); return;
        }
        case 583: { // mffs - move from FPSCR
          ILValue dst = SetFPR(func, state, FRD(w));
          il.op = ILOp::LoadFPSCR;
          il.dst = dst;
          emit(il); return;
        }
        case 711: { // mtfsf - move to FPSCR fields
          il.op = ILOp::StoreFPSCR;
          il.src1 = GetFPR(func, state, FRB(w));
          il.mask = Bits(w, 7, 14);  // FM field
          emit(il); return;
        }
        case 134: { // mtfsfi - move to FPSCR field immediate
          // Sets one 4-bit FPSCR field to an immediate value
          uint32_t crfd = Bits(w, 6, 8);
          uint32_t imm = Bits(w, 16, 19);
          il.op = ILOp::Nop;  // FPSCR field writes rarely affect execution
          il.asm_text = "mtfsfi";
          emit(il); return;
        }
        case 70: { // mtfsb0 - clear FPSCR bit
          il.op = ILOp::Nop;  // FPSCR bit clear rarely affects execution
          il.asm_text = "mtfsb0";
          emit(il); return;
        }
        case 38: { // mtfsb1 - set FPSCR bit
          il.op = ILOp::Nop;  // FPSCR bit set rarely affects execution
          il.asm_text = "mtfsb1";
          emit(il); return;
        }
        case 64: { // mcrfs - move to CR from FPSCR
          uint32_t crfd = Bits(w, 6, 8);
          uint32_t crfs = Bits(w, 11, 13);
          il.op = ILOp::Nop;  // CR from FPSCR — rarely used in game logic
          il.asm_text = "mcrfs";
          emit(il); return;
        }
      }

      // A-form double-precision (xo5 field)
      ILValue dst = SetFPR(func, state, FRD(w));
      switch (xo5) {
        case 18: { // fdiv
          il.op = ILOp::FDiv;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 20: { // fsub
          il.op = ILOp::FSub;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 21: { // fadd
          il.op = ILOp::FAdd;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 23: { // fsel
          il.op = ILOp::FSelect;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          emit(il); break;
        }
        case 22: { // fsqrt
          il.op = ILOp::FSqrt;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 25: { // fmul
          il.op = ILOp::FMul;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 26: { // frsqrte
          il.op = ILOp::FRSqrtEst;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRB(w));
          emit(il); break;
        }
        case 28: { // fmsub
          il.op = ILOp::FMulSub;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 29: { // fmadd
          il.op = ILOp::FMulAdd;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 30: { // fnmsub
          il.op = ILOp::FNMulSub;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        case 31: { // fnmadd
          il.op = ILOp::FNMulAdd;
          il.dst = dst;
          il.src1 = GetFPR(func, state, FRA(w));
          il.src2 = GetFPR(func, state, FRC(w));
          il.src3 = GetFPR(func, state, FRB(w));
          il.access_type = ILType::F64;
          emit(il); break;
        }
        default: {
          il.op = ILOp::InlineAsm;
          il.asm_text = disasm_text;
          emit(il); break;
        }
      }
      break;
    }

    // =====================================================================
    // 64-bit load/store: ld (58), std (62) - for completeness
    // =====================================================================
    case 58: {
      uint32_t ds = w & 0xFFFC;
      uint32_t xo2 = w & 3;
      ILValue dst = SetGPR(func, state, RD(w));
      il.op = (xo2 == 2) ? ILOp::Load : ILOp::Load; // lwa vs ld
      il.dst = dst;
      il.access_type = (xo2 == 2) ? ILType::S32 : ILType::U64;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.imm = (int16_t)(ds);
      emit(il);
      break;
    }
    case 62: {
      uint32_t ds = w & 0xFFFC;
      il.op = ILOp::Store;
      il.access_type = ILType::U64;
      il.src1 = (RA(w) == 0) ? ILNoVal() : GetGPR(func, state, RA(w));
      il.src2 = GetGPR(func, state, RS(w));
      il.imm = (int16_t)(ds);
      emit(il);
      break;
    }

    // =====================================================================
    // Default: anything the disassembler can decode but we haven't mapped
    // =====================================================================
    default: {
      // Null words (0x00000000) are padding, not instructions
      if (w == 0x00000000) {
        il.op = ILOp::Nop;
        emit(il);
      } else {
        il.op = ILOp::InlineAsm;
        il.asm_text = disasm_text;
        emit(il);
      }
      break;
    }
  }
}

// ---------------------------------------------------------------------------
// Vector unit (VMX / VMX128)
// ---------------------------------------------------------------------------

// How an instruction interprets the 128 bits it produces. Taken from the
// mnemonic because that is exactly what it encodes: the "fp" family works on
// four floats, the signed-saturating and sign-extending forms on signed lanes,
// everything else on unsigned lanes.
static ILType VectorResultType(const char* mnem) {
  std::string n(mnem);
  if (n.find("fp") != std::string::npos) return ILType::V128F;
  // Conversions name their source, not their result: vcfux/vcfsx produce
  // floats, vctuxs/vctsxs consume them.
  if (n.compare(0, 4, "vcfu") == 0 || n.compare(0, 4, "vcfs") == 0)
    return ILType::V128F;
  if (n.compare(0, 4, "vctu") == 0) return ILType::V128U;
  if (n.compare(0, 4, "vcts") == 0) return ILType::V128S;
  static const char* kSigned[] = {"vadds",  "vsubs",  "vmaxs", "vmins",
                                  "vcmpgts", "vsras", "vupkhs", "vupkls",
                                  "vmulos", "vmules", "vsum",   "vpks",
                                  "vmhadds", "vmhraddshs", "vmsums"};
  for (const char* p : kSigned)
    if (n.compare(0, strlen(p), p) == 0) return ILType::V128S;
  if (n.compare(0, 4, "vsra") == 0) return ILType::V128S;
  return ILType::V128U;
}

bool ILBuilder::TranslateVector(ILFunction& func, ILBasicBlock& block,
                                RegState& state, uint32_t address,
                                uint32_t w) {
  using namespace xe::cpu::ppc;
  PPCOpcode opcode = LookupOpcode(w);
  if (opcode == PPCOpcode::kInvalid) return false;
  const PPCOpcodeDisasmInfo& dis = GetOpcodeDisasmInfo(opcode);
  // Group alone is not the test: vector loads and stores are classified as
  // memory instructions, and lvx/stvx use the plain X form. What identifies a
  // vector instruction is that it names a vector register.
  bool vector_group = dis.group == PPCOpcodeGroup::kV;
  // Loads are lv*, stores are stv* - the 'v' is one character further along in
  // a store, which is what made every vector store miss this test and fall
  // through to the old handler that never recorded the value being stored.
  bool vector_mem = dis.group == PPCOpcodeGroup::kM &&
                    (strncmp(dis.name, "lv", 2) == 0 ||
                     strncmp(dis.name, "stv", 3) == 0);
  if (!vector_group && !vector_mem) return false;

  PPCDecodeData d;
  d.address = address;
  d.code = w;

  ILInstr il;
  il.address = address;
  il.ppc_word = w;
  il.call_name = dis.name;

  // Loads and stores address memory through a GPR pair, and everything else
  // is register to register. Splitting on the format keeps the register
  // numbering right: VMX128 spreads its register fields across the word.
  uint32_t vd = 0, va = 0, vb = 0, vc = 0, ra = 0, rb = 0;
  bool has_va = false, has_vb = false, has_vc = false, mem = false;
  switch (dis.format) {
    case PPCOpcodeFormat::kVX:
      vd = d.VX.VD(); va = d.VX.VA(); vb = d.VX.VB();
      has_va = has_vb = true;
      break;
    case PPCOpcodeFormat::kVC:
      vd = d.VC.VD(); va = d.VC.VA(); vb = d.VC.VB();
      has_va = has_vb = true;
      break;
    case PPCOpcodeFormat::kVA:
      vd = d.VA.VD(); va = d.VA.VA(); vb = d.VA.VB(); vc = d.VA.VC();
      has_va = has_vb = has_vc = true;
      break;
    case PPCOpcodeFormat::kVX128:
      vd = d.VX128.VD(); va = d.VX128.VA(); vb = d.VX128.VB();
      has_va = has_vb = true;
      break;
    case PPCOpcodeFormat::kVX128_1:
      vd = d.VX128_1.VD(); ra = d.VX128_1.RA(); rb = d.VX128_1.RB();
      mem = true;
      break;
    // lvx / stvx and the element-wise forms: an X-form instruction whose
    // "RD" field selects a vector register.
    case PPCOpcodeFormat::kX:
      vd = d.X.RT(); ra = d.X.RA(); rb = d.X.RB();
      mem = true;
      break;
    case PPCOpcodeFormat::kVX128_2:
      vd = d.VX128_2.VD(); va = d.VX128_2.VA(); vb = d.VX128_2.VB();
      vc = d.VX128_2.VC();
      has_va = has_vb = has_vc = true;
      break;
    case PPCOpcodeFormat::kVX128_3:
      vd = d.VX128_3.VD(); vb = d.VX128_3.VB();
      has_vb = true;
      break;
    case PPCOpcodeFormat::kVX128_4:
      vd = d.VX128_4.VD(); vb = d.VX128_4.VB();
      has_vb = true;
      break;
    case PPCOpcodeFormat::kVX128_5:
      vd = d.VX128_5.VD(); va = d.VX128_5.VA(); vb = d.VX128_5.VB();
      has_va = has_vb = true;
      break;
    case PPCOpcodeFormat::kVX128_R:
      vd = d.VX128_R.VD(); va = d.VX128_R.VA(); vb = d.VX128_R.VB();
      has_va = has_vb = true;
      break;
    case PPCOpcodeFormat::kVX128_P:
      vd = d.VX128_P.VD(); vb = d.VX128_P.VB();
      has_vb = true;
      break;
    default:
      return false;
  }

  const bool is_store = dis.name[0] == 's';
  if (mem) {
    // lvx / stvx and their VMX128 forms: RA0 means literal zero, not r0.
    ILValue base = ra ? GetGPR(func, state, ra) : ILNoVal();
    ILValue index = GetGPR(func, state, rb);
    il.op = is_store ? ILOp::VMXStore : ILOp::VMXLoad;
    il.access_type = ILType::V128U;
    if (is_store) {
      il.src1 = base;
      il.src2 = index;
      il.src3 = GetVR(func, state, vd);
    } else {
      il.dst = SetVR(func, state, vd, ILType::V128U);
      il.src1 = base;
      il.src2 = index;
    }
    block.instrs.push_back(std::move(il));
    FlushPendingWrites(state);
    return true;
  }

  il.op = ILOp::VectorOp;
  if (has_va) il.src1 = GetVR(func, state, va);
  if (has_vb) il.src2 = GetVR(func, state, vb);
  if (has_vc) il.src3 = GetVR(func, state, vc);
  // A compare writes CR6 as well, but the value that matters to the reader is
  // the mask in the destination register.
  il.dst = SetVR(func, state, vd, VectorResultType(dis.name));
  block.instrs.push_back(std::move(il));
  FlushPendingWrites(state);
  return true;
}

// ---------------------------------------------------------------------------
// Build basic blocks
// ---------------------------------------------------------------------------

std::vector<ILBasicBlock> ILBuilder::BuildBlocks(
    const std::vector<DisasmLine>& lines,
    uint32_t func_start, uint32_t func_end) {
  if (lines.empty()) return {};

  std::set<uint32_t> leaders;
  leaders.insert(func_start);

  for (auto& line : lines) {
    uint32_t w = line.code;
    uint32_t op = PrimOp(w);

    if (op == 18 && !LK(w)) { // b (unconditional, not call)
      int32_t li = SignExt(LI(w) << 2, 26);
      uint32_t target = AA(w) ? (uint32_t)li : line.address + li;
      if (target >= func_start && target < func_end)
        leaders.insert(target);
      if (line.address + 4 < func_end)
        leaders.insert(line.address + 4);
    } else if (op == 16) { // bc
      int32_t bd = SignExt(BD(w) << 2, 16);
      uint32_t target = AA(w) ? (uint32_t)bd : line.address + bd;
      if (target >= func_start && target < func_end)
        leaders.insert(target);
      if (line.address + 4 < func_end)
        leaders.insert(line.address + 4);
    } else if (op == 19) { // bclr/bcctr
      if (line.address + 4 < func_end)
        leaders.insert(line.address + 4);
    }
  }

  std::vector<uint32_t> sorted(leaders.begin(), leaders.end());
  std::sort(sorted.begin(), sorted.end());

  std::vector<ILBasicBlock> blocks;
  for (size_t i = 0; i < sorted.size(); i++) {
    ILBasicBlock bb;
    bb.start_addr = sorted[i];
    bb.end_addr = (i + 1 < sorted.size()) ? sorted[i + 1] : func_end;
    bb.block_id = (int)i;

    // Determine successors from last instruction
    uint32_t last_addr = bb.end_addr - 4;
    for (auto& line : lines) {
      if (line.address == last_addr) {
        uint32_t w = line.code;
        uint32_t op = PrimOp(w);
        if (w == 0x4E800020) { // blr
          bb.is_return = true;
        } else if (op == 18 && !LK(w)) {
          int32_t li = SignExt(LI(w) << 2, 26);
          uint32_t target = AA(w) ? (uint32_t)li : line.address + li;
          bb.successors.push_back(target);
        } else if (op == 16) {
          int32_t bd = SignExt(BD(w) << 2, 16);
          uint32_t target = AA(w) ? (uint32_t)bd : line.address + bd;
          bb.successors.push_back(target);
          if (line.address + 4 < func_end)
            bb.successors.push_back(line.address + 4);
        } else {
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
// Function detection
// ---------------------------------------------------------------------------

std::vector<std::pair<uint32_t, uint32_t>> ILBuilder::DetectFunctions(
    const uint8_t* data, size_t len, uint32_t base_addr,
    const std::vector<uint32_t>& export_addrs) {
  std::set<uint32_t> entries;
  // Addresses something CALLS, plus exports. These are function starts by
  // definition, so no function may extend across one. The mflr-prologue guesses
  // below are only hints -- a leaf saves no LR, and mflr also appears mid-body
  // -- so they must not be used as boundaries.
  std::set<uint32_t> hard_entries;

  for (uint32_t addr : export_addrs) {
    if (addr >= base_addr && addr < base_addr + len) {
      entries.insert(addr);
      hard_entries.insert(addr);
    }
  }

  for (size_t i = 0; i + 3 < len; i += 4) {
    uint32_t w = ReadBE32(data + i);
    uint32_t addr = base_addr + (uint32_t)i;

    // mflr r0 = prologue
    if (w == 0x7C0802A6) {
      if (i >= 4) {
        uint32_t prev = ReadBE32(data + i - 4);
        if (PrimOp(prev) == 37 && RS(prev) == 1 && RA(prev) == 1) {
          entries.insert(addr - 4);
          continue;
        }
      }
      entries.insert(addr);
    }

    // bl target
    if (PrimOp(w) == 18 && LK(w) && !AA(w)) {
      int32_t li = SignExt(LI(w) << 2, 26);
      uint32_t target = addr + li;
      if (target >= base_addr && target < base_addr + len) {
        entries.insert(target);
        hard_entries.insert(target);
      }
    }
  }

  std::vector<uint32_t> sorted(entries.begin(), entries.end());
  std::sort(sorted.begin(), sorted.end());

  std::vector<std::pair<uint32_t, uint32_t>> funcs;
  for (size_t idx = 0; idx < sorted.size(); idx++) {
    uint32_t start = sorted[idx];
    uint32_t limit = (idx + 1 < sorted.size()) ? sorted[idx + 1]
                                                 : base_addr + (uint32_t)len;
    // Scan for the terminator. A `blr` only ends the function when nothing
    // branches PAST it: a routine whose early-exit sits before a loop or a
    // contention/slow path (spinlocks, retry loops) has its `blr` in the
    // middle, and stopping there truncates the tail into a phantom function.
    // So track the furthest branch target seen and require the terminator to
    // be at or past it — same rule as ScanFunctionEnd.
    const uint32_t kMaxScan = 0x8000;
    uint32_t max_target = start;
    // For fall-through helpers (save/restore) the blr may be past the next
    // entry, hence the +256; a branch target beyond that extends the window.
    uint32_t scan_limit = std::min(limit + 256, base_addr + (uint32_t)len);
    uint32_t end = limit;

    // The next address something calls. Nothing -- not a far branch target, not
    // a missing terminator -- may carry the scan across it: that address is
    // another function's first instruction. Without this, one forward branch
    // lets a scan swallow every routine after it (the CRT string/math block in
    // MW3 came out as a single 5.5KB "function" holding strstr, strncpy,
    // strncat, atan, asin, ...).
    auto hard_it = hard_entries.upper_bound(start);
    const uint32_t hard_limit = (hard_it != hard_entries.end())
                                    ? *hard_it
                                    : base_addr + (uint32_t)len;
    for (uint32_t a = start;; a += 4) {
      uint32_t cap = std::max(scan_limit, max_target + 4);
      cap = std::min(cap, std::min(start + kMaxScan, base_addr + (uint32_t)len));
      cap = std::min(cap, hard_limit);  // never cross a call target
      if (a >= cap) {
        if (end > hard_limit) end = hard_limit;
        break;
      }
      size_t off = a - base_addr;
      if (off + 3 >= len) break;
      uint32_t w = ReadBE32(data + off);
      uint32_t prim = PrimOp(w);
      if (prim == 18 && !AA(w)) {  // b / bl
        int32_t li = SignExt(LI(w) << 2, 26);
        uint32_t tgt = a + (uint32_t)li;
        if (!LK(w)) {  // unconditional b
          if (tgt > max_target && tgt < start + kMaxScan) max_target = tgt;
          // No fall-through, and nothing branches past: function ends here
          // (tail call / tail restore that carries no blr of its own).
          if (a >= max_target) { end = a + 4; break; }
        }
      } else if (prim == 16 && !AA(w)) {  // bc — conditional
        int32_t bd = (int32_t)(int16_t)(uint16_t)(w & 0xFFFCu);
        uint32_t tgt = a + (uint32_t)bd;
        if (tgt > max_target && tgt < start + kMaxScan) max_target = tgt;
      }
      if ((w == 0x4E800020u || w == 0x4E800420u) && a >= max_target) {
        end = a + 4;  // blr / bctr
        break;
      }
      // A `mflr r0` is the standard prologue, so it marks the NEXT function
      // starting — the current one cannot extend past it. Acts as a hard bound
      // when a missing/undetected terminator would otherwise let the scan run
      // on and swallow following functions. (Only a bound: leaf functions save
      // no LR, so its absence proves nothing.)
      if (w == 0x7C0802A6u && a > start) {
        // `stwu r1, -N(r1)` immediately before belongs to that prologue too.
        uint32_t nf = a;
        if (a >= start + 4 && off >= 4) {
          uint32_t prev = ReadBE32(data + off - 4);
          if (PrimOp(prev) == 37 && RS(prev) == 1 && RA(prev) == 1) nf = a - 4;
        }
        if (nf > start) { end = nf; break; }
      }
    }
    if (end > hard_limit) end = hard_limit;
    if (end > start && end - start >= 4 && !IsImportThunk(start))
      funcs.push_back({start, end});
  }
  return funcs;
}

// ---------------------------------------------------------------------------
// Stack/arg/return detection
// ---------------------------------------------------------------------------

uint32_t ILBuilder::DetectStackFrame(const uint8_t* data, size_t len,
                                     uint32_t off) {
  // A prologue that saves registers through the compiler's helper thunks runs
  // several instructions before it allocates the frame - XGCopySurface's stwu
  // is the sixth instruction, behind mflr, two bl's, a stfd and a subi. A
  // five-instruction window missed it, and a frame size of zero switches off
  // stack-argument detection, so everything past the eighth parameter
  // disappeared from the signature and from type binding.
  for (int i = 0; i < 24 && (off + i * 4 + 3) < len; i++) {
    uint32_t w = ReadBE32(data + off + i * 4);
    if (PrimOp(w) == 37 && RS(w) == 1 && RA(w) == 1) {
      int16_t d = SIMM(w);
      if (d < 0) return (uint32_t)(-d);
    }
  }
  return 0;
}

int ILBuilder::DetectArgCount(const uint8_t* data, size_t len,
                              uint32_t off, size_t flen) {
  bool read[8] = {}, written[8] = {};
  for (size_t i = 0; i < flen && (off + i + 3) < len; i += 4) {
    uint32_t w = ReadBE32(data + off + i);
    uint32_t op = PrimOp(w);
    auto markRead = [&](uint32_t r) {
      if (r >= 3 && r <= 10 && !written[r-3]) read[r-3] = true;
    };
    auto markWrite = [&](uint32_t r) {
      if (r >= 3 && r <= 10) written[r-3] = true;
    };
    switch (op) {
      case 14: case 15: case 12: case 13:
        markRead(RA(w)); markWrite(RD(w)); break;
      case 31: markRead(RA(w)); markRead(RB(w)); markWrite(RD(w)); break;
      case 32: case 33: case 34: case 35: case 40: case 41: case 42:
        markRead(RA(w)); markWrite(RD(w)); break;
      case 36: case 37: case 38: case 39: case 44: case 45:
        markRead(RS(w)); markRead(RA(w)); break;
      default: break;
    }
  }
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (read[i]) count = i + 1; else break;
  }
  return count;
}

bool ILBuilder::DetectReturnsValue(const uint8_t* data, size_t len,
                                   uint32_t off, size_t flen) {
  for (size_t i = 0; i < flen && (off + i + 3) < len; i += 4) {
    uint32_t w = ReadBE32(data + off + i);
    uint32_t op = PrimOp(w);
    if ((op >= 14 && op <= 15) || op == 7 || op == 8 || op == 31 ||
        (op >= 32 && op <= 35) || (op >= 40 && op <= 42)) {
      if (RD(w) == 3) return true;
    }
  }
  return false;
}

// ---------------------------------------------------------------------------
// Build IL for a single function
// ---------------------------------------------------------------------------

ILFunction ILBuilder::BuildFunction(
    const uint8_t* data, size_t len, uint32_t base_addr,
    uint32_t func_start, uint32_t func_end, const std::string& name) {

  ILFunction func;
  func.name = name.empty() ? GetFunctionName(func_start) : name;
  func.entry = func_start;
  func.end = func_end;

  size_t off = func_start - base_addr;
  size_t flen = func_end - func_start;
  if (off + flen > len) flen = len - off;

  func.stack_frame_size = DetectStackFrame(data, len, (uint32_t)off);
  int arg_count = DetectArgCount(data, len, (uint32_t)off, flen);
  func.return_type = DetectReturnsValue(data, len, (uint32_t)off, flen)
                         ? ILType::U32 : ILType::Void;

  // Create parameter values
  for (int i = 0; i < arg_count; i++) {
    ILValue p = func.AllocValue(ILType::U32, 3 + i);
    char buf[16];
    snprintf(buf, sizeof(buf), "arg%d", i);
    p.name = buf;
    func.params.push_back(p);
    // Update the value in the pool with the name
    func.values.back().name = p.name;
  }

  // Disassemble
  auto lines = DisasmBuffer(data + off, flen, func_start);

  // Build blocks
  func.blocks = BuildBlocks(lines, func_start, func_end);

  // Init register state
  RegState state;
  state.Init();
  for (int i = 0; i < arg_count; i++) {
    state.gpr[3 + i] = func.params[i].id;
  }

  // Translate each block.
  //
  // Register state has to follow the CONTROL FLOW graph, not address order.
  // Threading one RegState through the blocks in address order makes a block
  // read whatever the textually-preceding block happened to leave in a
  // register, even when that block is not one of its predecessors -- so a
  // value defined on one arm of an if/else silently leaks into the other.
  // (Found in xam.xex's g726adpcm: the `anmag != 0` arm's read of r10 picked
  // up the other arm's `li r10,32`, and the whole `anmant` computation
  // decompiled to a constant expression.)
  //
  // Each block's entry state is the merge of its predecessors' exit states.
  // Where predecessors disagree, a merge value is allocated and the SSA is
  // destructed on the spot: an Assign goes in ahead of each predecessor's
  // terminator, so no phi nodes are needed downstream. Merge assignments that
  // turn out to be unused are removed by the C backend's DCE.
  const size_t nblk = func.blocks.size();
  std::unordered_map<uint32_t, int> blk_of;
  for (size_t i = 0; i < nblk; i++) blk_of[func.blocks[i].start_addr] = (int)i;

  std::vector<std::vector<int>> preds(nblk);
  for (size_t i = 0; i < nblk; i++) {
    for (uint32_t s : func.blocks[i].successors) {
      auto it = blk_of.find(s);
      if (it != blk_of.end()) preds[it->second].push_back((int)i);
    }
  }

  // Reverse postorder, so a block is translated after its forward preds.
  std::vector<int> order;
  std::vector<char> seen(nblk, 0);
  if (nblk) {
    struct Frame { int b; size_t next; };
    std::vector<Frame> fr;
    std::vector<int> post;
    fr.push_back({0, 0});
    seen[0] = 1;
    while (!fr.empty()) {
      Frame& f = fr.back();
      if (f.next < func.blocks[f.b].successors.size()) {
        auto it = blk_of.find(func.blocks[f.b].successors[f.next++]);
        if (it != blk_of.end() && !seen[it->second]) {
          seen[it->second] = 1;
          fr.push_back({it->second, 0});
        }
      } else {
        post.push_back(f.b);
        fr.pop_back();
      }
    }
    order.assign(post.rbegin(), post.rend());
    // Blocks unreachable from the entry still have to be translated.
    for (size_t i = 0; i < nblk; i++)
      if (!seen[i]) order.push_back((int)i);
  }

  // slot: 0-31 GPR, 32-63 FPR, 64-71 CR, 72-199 VR
  constexpr int kSlots = 200;
  auto slot = [](RegState& st, int s) -> int& {
    if (s < 32) return st.gpr[s];
    if (s < 64) return st.fpr[s - 32];
    if (s < 72) return st.cr[s - 64];
    return st.vr[s - 72];
  };
  auto slot_type = [](int s) {
    if (s >= 32 && s < 64) return ILType::F64;
    if (s >= 72) return ILType::V128U;
    return ILType::U32;
  };
  auto slot_reg = [](int s) -> int {
    if (s < 64) return s;
    // Vector registers carry the same numbering the rest of the builder uses
    // (kVRRegBase + vN), so a merge move between two of them is recognised as
    // one register and coalesced like any other.
    if (s >= 72) return kVRRegBase + (s - 72);
    return kILRegNone;
  };
  auto is_terminator = [](ILOp op) {
    switch (op) {
      case ILOp::Branch: case ILOp::BranchCond: case ILOp::BranchCTR:
      case ILOp::BranchLR: case ILOp::BranchDecrCTRZero:
      case ILOp::BranchDecrCTRNonZero: case ILOp::Return:
        return true;
      default:
        return false;
    }
  };
  auto add_merge_move = [&](int pred_blk, const ILValue& dst, int src_id,
                            int s) {
    if (src_id == kILNoValue || src_id == dst.id) return;
    ILInstr mv;
    mv.op = ILOp::Assign;
    mv.merge_move = true;
    mv.address = func.blocks[pred_blk].end_addr - 4;
    mv.dst = dst;
    mv.src1 = ILValue{src_id, slot_type(s), slot_reg(s), {}};
    auto& ins = func.blocks[pred_blk].instrs;
    size_t pos = ins.size();
    while (pos > 0 && is_terminator(ins[pos - 1].op)) pos--;
    ins.insert(ins.begin() + pos, std::move(mv));
  };

  std::vector<RegState> exit_state(nblk);
  std::vector<char> translated(nblk, 0);
  struct PhiFix { int pred; ILValue dst; int s; };
  std::vector<PhiFix> fixups;

  for (int bi : order) {
    auto& block = func.blocks[bi];
    RegState st;
    if (preds[bi].empty()) {
      st = state;  // entry block (or unreachable): function-entry state
    } else {
      bool have = false;
      for (int p : preds[bi]) {
        if (!translated[p]) continue;
        if (!have) { st = exit_state[p]; have = true; continue; }
        for (int s = 0; s < kSlots; s++) {
          if (slot(st, s) != slot(exit_state[p], s)) slot(st, s) = kILNoValue;
        }
      }
      if (!have) st = state;
      // Registers that disagree, or that a not-yet-translated predecessor
      // (a back edge) may redefine, need a merge value.
      bool back_edge = false;
      for (int p : preds[bi])
        if (!translated[p]) back_edge = true;
      for (int s = 0; s < kSlots; s++) {
        bool conflict = (slot(st, s) == kILNoValue);
        if (!conflict && back_edge) {
          for (int p : preds[bi]) {
            if (translated[p] && slot(exit_state[p], s) != slot(st, s))
              conflict = true;
          }
          if (!conflict) continue;  // agrees so far; back edge patched below
        }
        if (!conflict) continue;
        bool any = false;
        for (int p : preds[bi])
          if (translated[p] && slot(exit_state[p], s) != kILNoValue) any = true;
        if (!any && !back_edge) continue;
        ILValue m = func.AllocValue(slot_type(s), slot_reg(s));
        for (int p : preds[bi]) {
          if (translated[p]) add_merge_move(p, m, slot(exit_state[p], s), s);
          else fixups.push_back({p, m, s});
        }
        slot(st, s) = m.id;
      }
    }

    for (auto& line : lines) {
      if (line.address < block.start_addr || line.address >= block.end_addr)
        continue;
      TranslateInstruction(func, block, st, line.address, line.code,
                           line.text, func_start, func_end);
    }
    exit_state[bi] = st;
    translated[bi] = 1;
  }

  // Back edges: their predecessors are known now.
  for (auto& f : fixups)
    if (translated[f.pred])
      add_merge_move(f.pred, f.dst, slot(exit_state[f.pred], f.s), f.s);

  return func;
}

// ---------------------------------------------------------------------------
// Detect and build all functions
// ---------------------------------------------------------------------------

std::vector<ILFunction> ILBuilder::DetectAndBuildFunctions(
    const uint8_t* data, size_t len, uint32_t base_addr,
    const std::vector<uint32_t>& export_addrs) {

  auto ranges = DetectFunctions(data, len, base_addr, export_addrs);

  std::vector<ILFunction> funcs;
  xexmagic::progress::BeginPhase("building functions", ranges.size());
  for (auto& [start, end] : ranges) {
    std::string name = GetFunctionName(start);
    xexmagic::progress::SetCurrent(name);
    funcs.push_back(BuildFunction(data, len, base_addr, start, end, name));
    xexmagic::progress::Complete();
  }
  return funcs;
}
