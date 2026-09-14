// XEXMagic - Xenon PPE Cycle Counter
// Based on IBM Cell PPE (same core as Xbox 360 Xenon).
// Source: Cell BE Programming Handbook v1.1, CellPerformance articles.

#include "xenon_cycles.h"

int GetXenonCycles(const ILInstr& instr) {
  switch (instr.op) {
    // ---- FXU simple pipelined: 2 cycles ----
    case ILOp::Assign:
    case ILOp::Add:
    case ILOp::AddCarry:
    case ILOp::Sub:
    case ILOp::SubCarry:
    case ILOp::Neg:
    case ILOp::And:
    case ILOp::Or:
    case ILOp::Xor:
    case ILOp::Nand:
    case ILOp::Nor:
    case ILOp::Orc:
    case ILOp::Andc:
    case ILOp::Eqv:
    case ILOp::Not:
    case ILOp::ExtendByte:
    case ILOp::ExtendHalf:
    case ILOp::ExtendWord:
    case ILOp::CountLeadingZeros:
    case ILOp::Compare:
    case ILOp::CompareU:
    case ILOp::CompareImm:
    case ILOp::CompareImmU:
    case ILOp::CRAnd:
    case ILOp::CROr:
    case ILOp::CRXor:
    case ILOp::CRNand:
    case ILOp::CRNor:
    case ILOp::CRAndc:
    case ILOp::CROrc:
    case ILOp::CREqv:
    case ILOp::MoveCRField:
    case ILOp::MoveSPR:
    case ILOp::LoadTimeBase:
    case ILOp::MoveCR:
    case ILOp::MoveMSR:
    case ILOp::RotateAndMask:   // rlwinm: M-form, not microcoded
    case ILOp::RotateInsert:    // rlwimi: M-form, not microcoded
      return 2;

    // ---- FXU microcoded shift: 11 cycles ----
    case ILOp::Shl:
    case ILOp::Shr:
    case ILOp::Sar:
    case ILOp::SarImm:
    case ILOp::RotateAndMaskReg: // rlwnm: register rotate, microcoded
      return 11;

    // ---- FXU multiply: variable ----
    case ILOp::Mul:
      // mulli = 6cy, mullw = 9cy. Use 9 as conservative estimate.
      // Check if it's mulli (opcode 7) by looking at ppc_word
      if ((instr.ppc_word >> 26) == 7) return 6;
      return 9;
    case ILOp::MulHigh:
    case ILOp::MulHighU:
      return 9;

    // ---- FXU divide: 36 cycles average (10-38 range) ----
    case ILOp::Div:
    case ILOp::DivU:
      return 36;

    // ---- LSU: 4 cycles (L1 hit) ----
    case ILOp::Load:
    case ILOp::Store:
    case ILOp::LoadIndexed:
    case ILOp::StoreIndexed:
    case ILOp::LoadUpdate:
    case ILOp::StoreUpdate:
    case ILOp::LoadReserved:
    case ILOp::StoreConditional:
    case ILOp::LoadByteReverse:
    case ILOp::StoreByteReverse:
    case ILOp::FLoad:
    case ILOp::FStore:
    case ILOp::FLoadIndexed:
    case ILOp::FStoreIndexed:
    case ILOp::FLoadUpdate:
    case ILOp::FStoreUpdate:
      return 4;

    // ---- LSU multiple: 4*N cycles ----
    case ILOp::LoadMultiple:
    case ILOp::StoreMultiple: {
      // N = 32 - starting register
      int start_reg = instr.dst.ppc_reg;
      if (start_reg < 0 || start_reg > 31) start_reg = 0;
      int n = 32 - start_reg;
      if (n < 1) n = 1;
      return 4 * n;
    }

    // ---- BRU: 1 cycle (predicted) ----
    case ILOp::Branch:
    case ILOp::BranchCond:
    case ILOp::BranchCTR:
    case ILOp::BranchLR:
    case ILOp::BranchDecrCTRZero:
    case ILOp::BranchDecrCTRNonZero:
    case ILOp::Call:
    case ILOp::Return:
      return 1;

    // ---- FPU: 6 cycles pipelined ----
    case ILOp::FAdd:
    case ILOp::FSub:
    case ILOp::FMul:
    case ILOp::FMulAdd:
    case ILOp::FMulSub:
    case ILOp::FNMulAdd:
    case ILOp::FNMulSub:
    case ILOp::FNeg:
    case ILOp::FAbs:
    case ILOp::FNAbs:
    case ILOp::FMove:
    case ILOp::FRecipEst:
    case ILOp::FRSqrtEst:
    case ILOp::FSelect:
    case ILOp::FRoundSingle:
    case ILOp::FConvToIntWord:
    case ILOp::FConvToIntWordTrunc:
    case ILOp::FConvToIntDbl:
    case ILOp::FConvToIntDblTrunc:
    case ILOp::FConvFromInt:
    case ILOp::FCompareOrdered:
    case ILOp::FCompareUnordered:
    case ILOp::LoadFPSCR:
    case ILOp::StoreFPSCR:
      return 6;

    case ILOp::FSqrt:
      if (instr.access_type == ILType::F32) return 18;
      return 33;

    case ILOp::FStoreIntWord:
      return 4;

    case ILOp::VMXOp:
      return 4;  // VMX ops are typically 4 cycles on PPE

    // ---- FPU divide: non-pipelined ----
    case ILOp::FDiv:
      // fdivs = 18cy, fdiv = 33cy
      if (instr.access_type == ILType::F32) return 18;
      return 33;

    // ---- System / Sync ----
    case ILOp::Syscall:
      return 100;  // highly variable
    case ILOp::Sync:
      return 200;
    case ILOp::LWSync:
    case ILOp::PTESync:
      return 50;
    case ILOp::ISync:
    case ILOp::EIEIO:
      return 10;

    // ---- Cache ops ----
    case ILOp::DCacheFlush:
    case ILOp::DCacheStore:
    case ILOp::DCacheTouch:
    case ILOp::DCacheTouchStore:
    case ILOp::DCacheZero:
    case ILOp::ICacheInvalidate:
      return 4;

    // ---- Trap ----
    case ILOp::Trap:
    case ILOp::TrapImm:
      return 100;

    // ---- Stack / Nop ----
    case ILOp::StackAlloc:
      return 2;  // stwu r1, -N(r1)
    case ILOp::Nop:
      return 1;

    case ILOp::InlineAsm:
      return 2;  // best guess
  }
  return 2;
}

// Classify instruction for the summary breakdown
enum CycleCategory { CAT_ALU, CAT_LOAD, CAT_STORE, CAT_FP, CAT_BRANCH,
                     CAT_MUL, CAT_DIV, CAT_SYSTEM };

static CycleCategory Categorize(ILOp op) {
  switch (op) {
    case ILOp::Mul: case ILOp::MulHigh: case ILOp::MulHighU:
      return CAT_MUL;
    case ILOp::Div: case ILOp::DivU:
      return CAT_DIV;
    case ILOp::Load: case ILOp::LoadIndexed: case ILOp::LoadUpdate:
    case ILOp::LoadMultiple: case ILOp::LoadReserved:
    case ILOp::LoadByteReverse:
    case ILOp::FLoad: case ILOp::FLoadIndexed: case ILOp::FLoadUpdate:
      return CAT_LOAD;
    case ILOp::Store: case ILOp::StoreIndexed: case ILOp::StoreUpdate:
    case ILOp::StoreMultiple: case ILOp::StoreConditional:
    case ILOp::StoreByteReverse:
    case ILOp::FStore: case ILOp::FStoreIndexed: case ILOp::FStoreUpdate:
      return CAT_STORE;
    case ILOp::FStoreIntWord:
      return CAT_STORE;
    case ILOp::VMXOp:
      return CAT_FP;  // close enough
    case ILOp::FAdd: case ILOp::FSub: case ILOp::FMul: case ILOp::FDiv:
    case ILOp::FMulAdd: case ILOp::FMulSub:
    case ILOp::FNMulAdd: case ILOp::FNMulSub:
    case ILOp::FNeg: case ILOp::FAbs: case ILOp::FNAbs: case ILOp::FMove:
    case ILOp::FRecipEst: case ILOp::FRSqrtEst: case ILOp::FSqrt:
    case ILOp::FSelect:
    case ILOp::FRoundSingle:
    case ILOp::FConvToIntWord: case ILOp::FConvToIntWordTrunc:
    case ILOp::FConvToIntDbl: case ILOp::FConvToIntDblTrunc:
    case ILOp::FConvFromInt:
    case ILOp::FCompareOrdered: case ILOp::FCompareUnordered:
    case ILOp::LoadFPSCR: case ILOp::StoreFPSCR:
      return CAT_FP;
    case ILOp::Branch: case ILOp::BranchCond: case ILOp::BranchCTR:
    case ILOp::BranchLR: case ILOp::BranchDecrCTRZero:
    case ILOp::BranchDecrCTRNonZero: case ILOp::Call: case ILOp::Return:
      return CAT_BRANCH;
    case ILOp::LoadTimeBase:
    case ILOp::Syscall: case ILOp::Sync: case ILOp::LWSync:
    case ILOp::PTESync: case ILOp::ISync: case ILOp::EIEIO:
    case ILOp::DCacheFlush: case ILOp::DCacheStore: case ILOp::DCacheTouch:
    case ILOp::DCacheTouchStore: case ILOp::DCacheZero:
    case ILOp::ICacheInvalidate: case ILOp::Trap: case ILOp::TrapImm:
      return CAT_SYSTEM;
    default:
      return CAT_ALU;
  }
}

void ComputeFunctionCycles(ILFunction& func) {
  func.total_cycles = 0;
  func.count_alu = func.cycles_alu = 0;
  func.count_load = func.cycles_load = 0;
  func.count_store = func.cycles_store = 0;
  func.count_fp = func.cycles_fp = 0;
  func.count_branch = func.cycles_branch = 0;
  func.count_mul = func.cycles_mul = 0;
  func.count_div = func.cycles_div = 0;
  func.count_system = func.cycles_system = 0;

  for (auto& block : func.blocks) {
    block.total_cycles = 0;
    for (auto& il : block.instrs) {
      il.cycles = GetXenonCycles(il);
      block.total_cycles += il.cycles;

      auto cat = Categorize(il.op);
      switch (cat) {
        case CAT_ALU:    func.count_alu++; func.cycles_alu += il.cycles; break;
        case CAT_LOAD:   func.count_load++; func.cycles_load += il.cycles; break;
        case CAT_STORE:  func.count_store++; func.cycles_store += il.cycles; break;
        case CAT_FP:     func.count_fp++; func.cycles_fp += il.cycles; break;
        case CAT_BRANCH: func.count_branch++; func.cycles_branch += il.cycles; break;
        case CAT_MUL:    func.count_mul++; func.cycles_mul += il.cycles; break;
        case CAT_DIV:    func.count_div++; func.cycles_div += il.cycles; break;
        case CAT_SYSTEM: func.count_system++; func.cycles_system += il.cycles; break;
      }
    }
    func.total_cycles += block.total_cycles;
  }

  // Hot path: find the longest path through the CFG (simple: max block cycles)
  func.hot_path_cycles = 0;
  for (auto& block : func.blocks) {
    if (block.total_cycles > func.hot_path_cycles)
      func.hot_path_cycles = block.total_cycles;
  }
}
