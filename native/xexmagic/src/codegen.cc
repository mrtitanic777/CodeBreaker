// XEXMagic - C/C++ Code Generator

#include "codegen.h"

#include <algorithm>
#include <cstdio>
#include <map>
#include <set>
#include <sstream>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::string ValName(const ILValue& v) {
  if (!v.name.empty()) return v.name;
  if (v.id == kILNoValue) return "0";
  char buf[16];
  snprintf(buf, sizeof(buf), "v%d", v.id);
  return buf;
}

static std::string TypeStr(ILType t, bool cpp) {
  (void)cpp;  // Same types for C and C++ with stdint
  return ILTypeName(t);
}

static std::string MemCast(ILType access_type) {
  switch (access_type) {
    case ILType::U8:  return "*(uint8_t*)";
    case ILType::S8:  return "*(int8_t*)";
    case ILType::U16: return "*(uint16_t*)";
    case ILType::S16: return "*(int16_t*)";
    case ILType::U32: return "*(uint32_t*)";
    case ILType::S32: return "*(int32_t*)";
    case ILType::U64: return "*(uint64_t*)";
    case ILType::S64: return "*(int64_t*)";
    case ILType::F32: return "*(float*)";
    case ILType::F64: return "*(double*)";
    default:          return "*(uint32_t*)";
  }
}

static std::string AddrExpr(const ILValue& base, int32_t offset) {
  std::string base_name = ValName(base);
  if (base.id == kILNoValue) {
    // Absolute address
    char buf[32];
    snprintf(buf, sizeof(buf), "(uintptr_t)0x%X", (uint32_t)offset);
    return buf;
  }
  if (offset == 0) {
    return "(uintptr_t)" + base_name;
  }
  if (offset > 0) {
    char buf[64];
    snprintf(buf, sizeof(buf), "(uintptr_t)%s + 0x%X",
             base_name.c_str(), (uint32_t)offset);
    return buf;
  }
  char buf[64];
  snprintf(buf, sizeof(buf), "(uintptr_t)%s - 0x%X",
           base_name.c_str(), (uint32_t)(-offset));
  return buf;
}

static std::string IndexedAddrExpr(const ILValue& base, const ILValue& idx) {
  std::string b = (base.id == kILNoValue) ? "0" : ValName(base);
  return "(uintptr_t)" + b + " + " + ValName(idx);
}

static std::string LabelName(uint32_t addr, uint32_t func_entry) {
  char buf[32];
  snprintf(buf, sizeof(buf), "label_%X", addr - func_entry);
  return buf;
}

// ---------------------------------------------------------------------------
// Generate source for one function
// ---------------------------------------------------------------------------

std::string GenerateFunctionSource(const ILFunction& func,
                                   const CodeGenConfig& config) {
  std::ostringstream out;

  // Validation comment
  if (config.emit_validation && func.validated) {
    float cov = (func.total_instructions > 0)
        ? (float)(func.total_instructions - func.inline_asm_count)
          / (float)func.total_instructions * 100.0f
        : 100.0f;
    out << "/* Validation: " << (func.validation_errors.empty() ? "PASS" : "FAIL")
        << " (" << (func.total_instructions - func.inline_asm_count) << "/"
        << func.total_instructions << " instructions, "
        << (int)cov << "." << (int)((cov - (int)cov) * 10) << "% coverage";
    if (!func.validation_warnings.empty())
      out << ", " << func.validation_warnings.size() << " warnings";
    out << ") */\n";
  }

  // Cycle comment
  if (config.emit_cycle_comments && func.total_cycles > 0) {
    float ns = (float)func.total_cycles / 3.2f;
    out << "/* Cycle estimate (single-issue Xenon PPE, L1-hit, predicted branches):\n";
    out << " *   Total straight-line: " << func.total_cycles << " cycles ("
        << (int)ns << "." << (int)((ns - (int)ns) * 10) << " ns @ 3.2 GHz)\n";
    if (func.hot_path_cycles > 0) {
      float hns = (float)func.hot_path_cycles / 3.2f;
      out << " *   Hot path (longest block): " << func.hot_path_cycles
          << " cycles (" << (int)hns << "." << (int)((hns - (int)hns) * 10)
          << " ns)\n";
    }
    out << " *   Breakdown:";
    if (func.count_alu)    out << " " << func.count_alu << " ALU (" << func.cycles_alu << "cy)";
    if (func.count_load)   out << " " << func.count_load << " Load (" << func.cycles_load << "cy)";
    if (func.count_store)  out << " " << func.count_store << " Store (" << func.cycles_store << "cy)";
    if (func.count_fp)     out << " " << func.count_fp << " FP (" << func.cycles_fp << "cy)";
    if (func.count_branch) out << " " << func.count_branch << " Branch (" << func.cycles_branch << "cy)";
    if (func.count_mul)    out << " " << func.count_mul << " Mul (" << func.cycles_mul << "cy)";
    if (func.count_div)    out << " " << func.count_div << " Div (" << func.cycles_div << "cy)";
    if (func.count_system) out << " " << func.count_system << " System (" << func.cycles_system << "cy)";
    out << "\n */\n";
  }

  // Function header comment
  out << "/* " << func.name << " (0x" << std::hex << func.entry << ", 0x"
      << (func.end - func.entry) << std::dec << " bytes) */\n";

  // Signature
  out << TypeStr(func.return_type, config.cpp_mode) << " " << func.name << "(";
  for (size_t i = 0; i < func.params.size(); i++) {
    if (i > 0) out << ", ";
    out << TypeStr(func.params[i].type, config.cpp_mode) << " " << ValName(func.params[i]);
  }
  out << ") {\n";

  // Collect all variables that need declaration
  std::set<int> declared;
  std::map<int, ILType> var_types;
  for (auto& p : func.params) declared.insert(p.id);
  // Also skip immediates
  for (auto& v : func.values) {
    if (v.ppc_reg == kILRegNone && !v.name.empty() &&
        (v.name[0] == '0' || v.name[0] == '-' ||
         (v.name[0] >= '1' && v.name[0] <= '9'))) {
      declared.insert(v.id);  // numeric literal, don't declare
    }
  }

  // Collect all dst values that need declarations
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      if (il.dst.id != kILNoValue && !declared.count(il.dst.id)) {
        if (il.op != ILOp::Nop && il.op != ILOp::StackAlloc &&
            il.op != ILOp::Store && il.op != ILOp::StoreIndexed &&
            il.op != ILOp::StoreUpdate && il.op != ILOp::StoreMultiple &&
            il.op != ILOp::StoreConditional && il.op != ILOp::StoreByteReverse &&
            il.op != ILOp::FStore && il.op != ILOp::FStoreIndexed &&
            il.op != ILOp::FStoreUpdate &&
            il.op != ILOp::Branch && il.op != ILOp::BranchCond &&
            il.op != ILOp::BranchCTR && il.op != ILOp::BranchLR &&
            il.op != ILOp::BranchDecrCTRZero &&
            il.op != ILOp::BranchDecrCTRNonZero &&
            il.op != ILOp::Return && il.op != ILOp::Syscall &&
            il.op != ILOp::InlineAsm && il.op != ILOp::MoveSPR &&
            il.op != ILOp::MoveCR && il.op != ILOp::MoveMSR) {
          std::string name = ValName(il.dst);
          if (!name.empty() && name[0] != 'v') {  // Skip unnamed
            var_types[il.dst.id] = il.dst.type;
            declared.insert(il.dst.id);
          }
        }
      }
    }
  }

  // Emit declarations grouped by type
  std::map<ILType, std::vector<std::string>> decl_groups;
  for (auto& [id, type] : var_types) {
    for (auto& v : func.values) {
      if (v.id == id) {
        std::string name = ValName(v);
        // Don't redeclare parameters or immediate literals
        bool is_param = false;
        for (auto& p : func.params) {
          if (p.id == id) { is_param = true; break; }
        }
        if (!is_param && !name.empty() && name[0] != 'v' &&
            !(name[0] >= '0' && name[0] <= '9') && name[0] != '-') {
          decl_groups[type].push_back(name);
        }
        break;
      }
    }
  }

  // Deduplicate declaration names
  for (auto& [type, names] : decl_groups) {
    std::set<std::string> seen;
    std::vector<std::string> unique;
    for (auto& n : names) {
      if (seen.insert(n).second) unique.push_back(n);
    }
    for (auto& n : unique) {
      out << "    " << TypeStr(type, config.cpp_mode) << " " << n << ";\n";
    }
  }

  // Build address -> block index for label emission
  std::set<uint32_t> label_targets;
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      if (il.op == ILOp::Branch || il.op == ILOp::BranchCond ||
          il.op == ILOp::BranchDecrCTRZero ||
          il.op == ILOp::BranchDecrCTRNonZero) {
        if (il.target >= func.entry && il.target < func.end)
          label_targets.insert(il.target);
      }
    }
  }

  // Helper to find the compare instruction for a cmp_ref
  auto findCmp = [&](int cmp_ref) -> const ILInstr* {
    for (auto& b : func.blocks) {
      for (auto& i : b.instrs) {
        if (i.dst.id == cmp_ref &&
            (i.op == ILOp::Compare || i.op == ILOp::CompareU ||
             i.op == ILOp::CompareImm || i.op == ILOp::CompareImmU ||
             i.op == ILOp::FCompareOrdered || i.op == ILOp::FCompareUnordered))
          return &i;
      }
    }
    return nullptr;
  };

  // Emit blocks
  for (size_t bi = 0; bi < func.blocks.size(); bi++) {
    auto& block = func.blocks[bi];

    // Label if needed
    if (label_targets.count(block.start_addr)) {
      out << "\n" << LabelName(block.start_addr, func.entry) << ":\n";
    } else if (func.blocks.size() > 1 && bi > 0) {
      out << "\n";
    }

    // Block cycle comment
    if (config.emit_cycle_comments && block.total_cycles > 0) {
      out << "    /* block " << bi << ": " << block.instrs.size()
          << " instrs, " << block.total_cycles << " cycles */\n";
    }

    for (auto& il : block.instrs) {
      std::string dst = ValName(il.dst);
      std::string s1 = ValName(il.src1);
      std::string s2 = ValName(il.src2);
      std::string s3 = ValName(il.src3);

      switch (il.op) {
        case ILOp::Nop:
        case ILOp::StackAlloc:
          break;  // Suppress

        case ILOp::Assign:
          out << "    " << dst << " = " << s1 << ";\n";
          break;

        case ILOp::Add:
          out << "    " << dst << " = " << s1 << " + " << s2 << ";\n";
          break;
        case ILOp::AddCarry:
          out << "    " << dst << " = " << s1 << " + " << s2 << " /* +carry */;\n";
          break;
        case ILOp::Sub:
          out << "    " << dst << " = " << s1 << " - " << s2 << ";\n";
          break;
        case ILOp::SubCarry:
          out << "    " << dst << " = " << s1 << " - " << s2 << " /* -borrow */;\n";
          break;
        case ILOp::Mul:
          out << "    " << dst << " = " << s1 << " * " << s2 << ";\n";
          break;
        case ILOp::MulHigh:
          out << "    " << dst << " = (uint32_t)((int64_t)" << s1 << " * (int64_t)" << s2 << " >> 32);\n";
          break;
        case ILOp::MulHighU:
          out << "    " << dst << " = (uint32_t)((uint64_t)" << s1 << " * (uint64_t)" << s2 << " >> 32);\n";
          break;
        case ILOp::Div:
          out << "    " << dst << " = (int32_t)" << s1 << " / (int32_t)" << s2 << ";\n";
          break;
        case ILOp::DivU:
          out << "    " << dst << " = " << s1 << " / " << s2 << ";\n";
          break;
        case ILOp::Neg:
          out << "    " << dst << " = -" << s1 << ";\n";
          break;

        case ILOp::And:
          out << "    " << dst << " = " << s1 << " & " << s2 << ";\n";
          break;
        case ILOp::Or:
          out << "    " << dst << " = " << s1 << " | " << s2 << ";\n";
          break;
        case ILOp::Xor:
          out << "    " << dst << " = " << s1 << " ^ " << s2 << ";\n";
          break;
        case ILOp::Nand:
          out << "    " << dst << " = ~(" << s1 << " & " << s2 << ");\n";
          break;
        case ILOp::Nor:
          out << "    " << dst << " = ~(" << s1 << " | " << s2 << ");\n";
          break;
        case ILOp::Orc:
          out << "    " << dst << " = " << s1 << " | ~" << s2 << ";\n";
          break;
        case ILOp::Andc:
          out << "    " << dst << " = " << s1 << " & ~" << s2 << ";\n";
          break;
        case ILOp::Eqv:
          out << "    " << dst << " = ~(" << s1 << " ^ " << s2 << ");\n";
          break;
        case ILOp::Not:
          out << "    " << dst << " = ~" << s1 << ";\n";
          break;

        case ILOp::Shl:
          if (il.src2.id != kILNoValue)
            out << "    " << dst << " = " << s1 << " << " << s2 << ";\n";
          else
            out << "    " << dst << " = " << s1 << " << " << il.imm << ";\n";
          break;
        case ILOp::Shr:
          if (il.src2.id != kILNoValue)
            out << "    " << dst << " = " << s1 << " >> " << s2 << ";\n";
          else
            out << "    " << dst << " = " << s1 << " >> " << il.imm << ";\n";
          break;
        case ILOp::Sar:
          out << "    " << dst << " = (int32_t)" << s1 << " >> " << s2 << ";\n";
          break;
        case ILOp::SarImm:
          out << "    " << dst << " = (int32_t)" << s1 << " >> " << il.imm << ";\n";
          break;

        case ILOp::RotateAndMask: {
          char buf[64];
          snprintf(buf, sizeof(buf), "0x%08X", il.mask);
          out << "    " << dst << " = __rotl(" << s1 << ", " << il.imm
              << ") & " << buf << ";\n";
          break;
        }
        case ILOp::RotateInsert: {
          char buf[64];
          snprintf(buf, sizeof(buf), "0x%08X", il.mask);
          out << "    " << dst << " = (" << s2 << " & ~" << buf
              << ") | (__rotl(" << s1 << ", " << il.imm << ") & " << buf << ");\n";
          break;
        }
        case ILOp::RotateAndMaskReg: {
          char buf[64];
          snprintf(buf, sizeof(buf), "0x%08X", il.mask);
          out << "    " << dst << " = __rotl(" << s1 << ", " << s2
              << ") & " << buf << ";\n";
          break;
        }
        case ILOp::CountLeadingZeros:
          out << "    " << dst << " = __clz(" << s1 << ");\n";
          break;

        case ILOp::ExtendByte:
          out << "    " << dst << " = (uint32_t)(int32_t)(int8_t)" << s1 << ";\n";
          break;
        case ILOp::ExtendHalf:
          out << "    " << dst << " = (uint32_t)(int32_t)(int16_t)" << s1 << ";\n";
          break;
        case ILOp::ExtendWord:
          out << "    " << dst << " = (uint32_t)(int32_t)" << s1 << ";\n";
          break;

        // ---- Memory ----
        case ILOp::Load:
          out << "    " << dst << " = " << MemCast(il.access_type)
              << "(" << AddrExpr(il.src1, il.imm) << ");\n";
          break;
        case ILOp::LoadUpdate:
          out << "    " << dst << " = " << MemCast(il.access_type)
              << "(" << AddrExpr(il.src1, il.imm) << ");\n";
          break;
        case ILOp::LoadIndexed:
          out << "    " << dst << " = " << MemCast(il.access_type)
              << "(" << IndexedAddrExpr(il.src1, il.src2) << ");\n";
          break;
        case ILOp::Store:
          out << "    " << MemCast(il.access_type) << "(" << AddrExpr(il.src1, il.imm)
              << ") = " << s2 << ";\n";
          break;
        case ILOp::StoreUpdate:
          out << "    " << MemCast(il.access_type) << "(" << AddrExpr(il.src1, il.imm)
              << ") = " << s2 << ";\n";
          break;
        case ILOp::StoreIndexed:
          out << "    " << MemCast(il.access_type) << "("
              << IndexedAddrExpr(il.src1, il.src2) << ") = " << s3 << ";\n";
          break;
        case ILOp::LoadMultiple:
          out << "    /* lmw: load r" << il.dst.ppc_reg << "-r31 from ("
              << s1 << " + " << il.imm << ") */\n";
          break;
        case ILOp::StoreMultiple:
          out << "    /* stmw: store r" << il.dst.ppc_reg << "-r31 to ("
              << s1 << " + " << il.imm << ") */\n";
          break;
        case ILOp::LoadReserved:
          out << "    " << dst << " = __lwarx(" << IndexedAddrExpr(il.src1, il.src2) << ");\n";
          break;
        case ILOp::StoreConditional:
          out << "    __stwcx(" << IndexedAddrExpr(il.src1, il.src2) << ", " << s3 << ");\n";
          break;
        case ILOp::LoadByteReverse:
          out << "    " << dst << " = __byteswap(" << MemCast(il.access_type)
              << "(" << IndexedAddrExpr(il.src1, il.src2) << "));\n";
          break;
        case ILOp::StoreByteReverse:
          out << "    " << MemCast(il.access_type) << "("
              << IndexedAddrExpr(il.src1, il.src2) << ") = __byteswap(" << s3 << ");\n";
          break;

        // ---- Float memory ----
        case ILOp::FLoad:
          out << "    " << dst << " = " << MemCast(il.access_type)
              << "(" << AddrExpr(il.src1, il.imm) << ");\n";
          break;
        case ILOp::FStore:
          out << "    " << MemCast(il.access_type) << "(" << AddrExpr(il.src1, il.imm)
              << ") = " << s2 << ";\n";
          break;
        case ILOp::FLoadIndexed:
          out << "    " << dst << " = " << MemCast(il.access_type)
              << "(" << IndexedAddrExpr(il.src1, il.src2) << ");\n";
          break;
        case ILOp::FStoreIndexed:
          out << "    " << MemCast(il.access_type) << "("
              << IndexedAddrExpr(il.src1, il.src2) << ") = " << s3 << ";\n";
          break;
        case ILOp::FLoadUpdate:
          out << "    " << dst << " = " << MemCast(il.access_type)
              << "(" << AddrExpr(il.src1, il.imm) << ");\n";
          break;
        case ILOp::FStoreUpdate:
          out << "    " << MemCast(il.access_type) << "(" << AddrExpr(il.src1, il.imm)
              << ") = " << s2 << ";\n";
          break;

        // ---- Float arithmetic ----
        case ILOp::FAdd:
          out << "    " << dst << " = " << s1 << " + " << s2 << ";\n"; break;
        case ILOp::FSub:
          out << "    " << dst << " = " << s1 << " - " << s2 << ";\n"; break;
        case ILOp::FMul:
          out << "    " << dst << " = " << s1 << " * " << s2 << ";\n"; break;
        case ILOp::FDiv:
          out << "    " << dst << " = " << s1 << " / " << s2 << ";\n"; break;
        case ILOp::FMulAdd:
          out << "    " << dst << " = " << s1 << " * " << s2 << " + " << s3 << ";\n"; break;
        case ILOp::FMulSub:
          out << "    " << dst << " = " << s1 << " * " << s2 << " - " << s3 << ";\n"; break;
        case ILOp::FNMulAdd:
          out << "    " << dst << " = -(" << s1 << " * " << s2 << " + " << s3 << ");\n"; break;
        case ILOp::FNMulSub:
          out << "    " << dst << " = -(" << s1 << " * " << s2 << " - " << s3 << ");\n"; break;
        case ILOp::FNeg:
          out << "    " << dst << " = -" << s1 << ";\n"; break;
        case ILOp::FAbs:
          out << "    " << dst << " = __fabs(" << s1 << ");\n"; break;
        case ILOp::FNAbs:
          out << "    " << dst << " = -__fabs(" << s1 << ");\n"; break;
        case ILOp::FMove:
          out << "    " << dst << " = " << s1 << ";\n"; break;
        case ILOp::FRecipEst:
          out << "    " << dst << " = __fres(" << s1 << ");\n"; break;
        case ILOp::FRSqrtEst:
          out << "    " << dst << " = __frsqrte(" << s1 << ");\n"; break;
        case ILOp::FSqrt:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = __builtin_sqrtf(" << s1 << ");\n";
          else
            out << "    " << dst << " = __builtin_sqrt(" << s1 << ");\n";
          break;
        case ILOp::FSelect:
          out << "    " << dst << " = (" << s1 << " >= 0.0) ? " << s2 << " : " << s3 << ";\n"; break;
        case ILOp::FRoundSingle:
          out << "    " << dst << " = (float)" << s1 << ";\n"; break;
        case ILOp::FConvToIntWord:
        case ILOp::FConvToIntWordTrunc:
          out << "    " << dst << " = (double)(int32_t)" << s1 << ";\n"; break;
        case ILOp::FConvToIntDbl:
        case ILOp::FConvToIntDblTrunc:
          out << "    " << dst << " = (double)(int64_t)" << s1 << ";\n"; break;
        case ILOp::FConvFromInt:
          out << "    " << dst << " = (double)" << s1 << ";\n"; break;

        // ---- Float compare ----
        case ILOp::FCompareOrdered:
        case ILOp::FCompareUnordered:
          out << "    /* fcmp " << s1 << ", " << s2 << " → cr" << il.cr_field << " */\n";
          break;

        // ---- Integer compare ----
        case ILOp::Compare:
          out << "    /* cmp " << s1 << ", " << s2 << " → cr" << il.cr_field << " */\n";
          break;
        case ILOp::CompareU:
          out << "    /* cmpl " << s1 << ", " << s2 << " → cr" << il.cr_field << " */\n";
          break;
        case ILOp::CompareImm:
          out << "    /* cmpi " << s1 << ", " << il.imm << " → cr" << il.cr_field << " */\n";
          break;
        case ILOp::CompareImmU:
          out << "    /* cmpli " << s1 << ", " << il.imm << " → cr" << il.cr_field << " */\n";
          break;

        // ---- Branches ----
        case ILOp::Branch:
          if (il.target >= func.entry && il.target < func.end) {
            out << "    goto " << LabelName(il.target, func.entry) << ";\n";
          } else {
            out << "    goto /* external 0x" << std::hex << il.target << std::dec << " */;\n";
          }
          break;

        case ILOp::BranchCond: {
          // Try to resolve the condition
          auto* cmp = findCmp(il.cmp_ref);
          std::string cond_str;
          if (cmp) {
            std::string lhs = ValName(cmp->src1);
            std::string rhs;
            if (cmp->op == ILOp::CompareImm || cmp->op == ILOp::CompareImmU) {
              rhs = std::to_string(cmp->imm);
            } else {
              rhs = ValName(cmp->src2);
            }
            // Apply unsigned cast if needed
            bool is_unsigned = (cmp->op == ILOp::CompareU || cmp->op == ILOp::CompareImmU);
            ILCmpKind kind = il.cmp_kind;
            if (is_unsigned && kind >= ILCmpKind::LT && kind <= ILCmpKind::GE) {
              // Promote to unsigned comparison
              kind = (ILCmpKind)((int)kind + (int)ILCmpKind::LTU - (int)ILCmpKind::LT);
            }
            cond_str = lhs + " " + ILCmpKindStr(kind) + " " + rhs;
          } else {
            cond_str = "/* cond cr" + std::to_string(il.cr_field) + " " +
                       ILCmpKindStr(il.cmp_kind) + " */";
          }

          if (il.target >= func.entry && il.target < func.end) {
            out << "    if (" << cond_str << ") goto "
                << LabelName(il.target, func.entry) << ";\n";
          } else {
            out << "    if (" << cond_str << ") goto /* external 0x"
                << std::hex << il.target << std::dec << " */;\n";
          }
          break;
        }

        case ILOp::BranchDecrCTRZero:
          if (il.target >= func.entry && il.target < func.end) {
            out << "    if (--ctr == 0) goto " << LabelName(il.target, func.entry) << ";\n";
          } else {
            out << "    if (--ctr == 0) goto /* 0x" << std::hex << il.target << std::dec << " */;\n";
          }
          break;
        case ILOp::BranchDecrCTRNonZero:
          if (il.target >= func.entry && il.target < func.end) {
            out << "    if (--ctr != 0) goto " << LabelName(il.target, func.entry) << ";\n";
          } else {
            out << "    if (--ctr != 0) goto /* 0x" << std::hex << il.target << std::dec << " */;\n";
          }
          break;

        case ILOp::BranchCTR:
          out << "    goto /* CTR */;\n"; break;
        case ILOp::BranchLR:
          out << "    goto /* LR */;\n"; break;

        case ILOp::Call:
          if (il.dst.id != kILNoValue && !dst.empty() && dst[0] != 'v') {
            out << "    " << dst << " = " << il.call_name << "(";
          } else {
            out << "    " << il.call_name << "(";
          }
          out << ");\n";
          break;

        case ILOp::Return:
          if (func.return_type != ILType::Void) {
            // Find the most recent r3 value
            std::string ret_val = "0";
            for (auto& v : func.values) {
              if (v.ppc_reg == 3 && !v.name.empty()) ret_val = v.name;
            }
            out << "    return " << ret_val << ";\n";
          } else {
            out << "    return;\n";
          }
          break;

        // ---- SPR ----
        case ILOp::LoadTimeBase:
          if (il.imm == 269) {
            // TBU: upper 32 bits of time base
            out << "#ifdef _XBOX360\n";
            out << "    " << dst << " = (uint32_t)(__mftb() >> 32);\n";
            out << "#else\n";
            out << "    " << dst << " = __rdtsc() >> 32;  /* TBU */\n";
            out << "#endif\n";
          } else {
            // TB: full/lower time base (SPR 268 or default)
            out << "#ifdef _XBOX360\n";
            out << "    " << dst << " = (uint32_t)__mftb();\n";
            out << "#else\n";
            out << "    " << dst << " = __rdtsc();  /* TB */\n";
            out << "#endif\n";
          }
          break;
        case ILOp::MoveSPR:
          if (il.spr_read) {
            const char* spr_name = "spr";
            if (il.spr == ILSPR::LR) spr_name = "lr";
            else if (il.spr == ILSPR::CTR) spr_name = "ctr";
            out << "    " << dst << " = " << spr_name << ";\n";
          } else {
            const char* spr_name = "spr";
            if (il.spr == ILSPR::LR) spr_name = "lr";
            else if (il.spr == ILSPR::CTR) spr_name = "ctr";
            out << "    " << spr_name << " = " << s1 << ";\n";
          }
          break;
        case ILOp::MoveCR:
          if (il.spr_read) out << "    " << dst << " = cr;\n";
          else out << "    cr = " << s1 << ";\n";
          break;
        case ILOp::MoveMSR:
          if (il.spr_read) out << "    " << dst << " = msr;\n";
          else out << "    msr = " << s1 << ";\n";
          break;

        // ---- CR ops ----
        case ILOp::CRAnd: case ILOp::CROr: case ILOp::CRXor:
        case ILOp::CRNand: case ILOp::CRNor: case ILOp::CRAndc:
        case ILOp::CROrc: case ILOp::CREqv: case ILOp::MoveCRField:
          out << "    /* " << ILOpName(il.op) << " */\n";
          break;

        // ---- System ----
        case ILOp::Syscall:
          out << "    __syscall();\n"; break;
        case ILOp::Sync:
          out << "    __sync();\n"; break;
        case ILOp::LWSync:
          out << "    __lwsync();\n"; break;
        case ILOp::PTESync:
          out << "    __ptesync();\n"; break;
        case ILOp::ISync:
          out << "    __isync();\n"; break;
        case ILOp::EIEIO:
          out << "    __eieio();\n"; break;

        // ---- Cache ----
        case ILOp::DCacheFlush:
          out << "    __dcbf(" << s1 << ", " << s2 << ");\n"; break;
        case ILOp::DCacheStore:
          out << "    __dcbst(" << s1 << ", " << s2 << ");\n"; break;
        case ILOp::DCacheTouch:
          out << "    __dcbt(" << s1 << ", " << s2 << ");\n"; break;
        case ILOp::DCacheTouchStore:
          out << "    __dcbtst(" << s1 << ", " << s2 << ");\n"; break;
        case ILOp::DCacheZero:
          out << "    __dcbz(" << s1 << ", " << s2 << ");\n"; break;
        case ILOp::ICacheInvalidate:
          out << "    __icbi(" << s1 << ", " << s2 << ");\n"; break;

        // ---- Trap ----
        case ILOp::Trap:
          out << "    __trap(" << s1 << ", " << s2 << ");\n"; break;
        case ILOp::TrapImm:
          out << "    __trap(" << s1 << ", " << il.imm << ");\n"; break;

        case ILOp::FStoreIntWord:
          out << "    " << MemCast(ILType::U32) << "("
              << IndexedAddrExpr(il.src1, il.src2) << ") = (uint32_t)" << s3 << ";\n";
          break;

        case ILOp::LoadFPSCR:
          out << "    " << dst << " = __mffs();\n"; break;
        case ILOp::StoreFPSCR:
          out << "    __mtfsf(" << il.mask << ", " << s1 << ");\n"; break;

        case ILOp::VMXOp:
          out << "    /* vmx: " << il.asm_text << " */\n"; break;

        case ILOp::InlineAsm:
          out << "    __asm__(\"" << il.asm_text << "\");\n"; break;

        default:
          out << "    /* unhandled IL: " << ILOpName(il.op) << " */\n";
          break;
      }
    }
  }

  out << "}\n";
  return out.str();
}

// ---------------------------------------------------------------------------
// Generate full source file
// ---------------------------------------------------------------------------

std::string GenerateSource(const std::vector<ILFunction>& funcs,
                           const CodeGenConfig& config,
                           const char* source_file) {
  std::ostringstream out;

  // Header
  out << "/* XEXMagic Decompiler Output */\n";
  if (source_file) out << "/* Source: " << source_file << " */\n";
  out << "\n";

  // Includes
  if (config.cpp_mode) {
    out << "#include <cstdint>\n";
  } else {
    out << "#include <stdint.h>\n";
  }
  out << "\n";

  // Helper macros
  out << "/* PPC intrinsic helpers */\n";
  out << "#define __rotl(v, n) (((v) << (n)) | ((v) >> (32 - (n))))\n";
  out << "#define __clz(v) __builtin_clz(v)\n";
  out << "#ifndef __fabs\n";
  out << "#define __fabs(v) ((v) < 0.0 ? -(v) : (v))\n";
  out << "#endif\n";
  out << "#define __fres(v) (1.0f / (v))\n";
  out << "#define __frsqrte(v) (1.0f / __builtin_sqrtf(v))\n";
  out << "#define __byteswap(v) __builtin_bswap32(v)\n";
  out << "\n";

  // Namespace open
  if (!config.namespace_name.empty()) {
    out << "namespace " << config.namespace_name << " {\n\n";
  }

  // Forward declarations
  std::set<std::string> all_names;
  for (auto& f : funcs) all_names.insert(f.name);

  std::set<std::string> extern_funcs;
  for (auto& f : funcs) {
    for (auto& name : f.called_functions) {
      if (all_names.find(name) == all_names.end() && !name.empty()) {
        extern_funcs.insert(name);
      }
    }
  }

  if (!extern_funcs.empty()) {
    out << "/* Forward declarations (external) */\n";
    for (auto& name : extern_funcs) {
      if (name.find("/*") != std::string::npos) continue;  // skip comments
      out << "extern uint32_t " << name << "();\n";
    }
    out << "\n";
  }

  // Emit functions
  for (auto& f : funcs) {
    out << GenerateFunctionSource(f, config) << "\n";
  }

  // Namespace close
  if (!config.namespace_name.empty()) {
    out << "} // namespace " << config.namespace_name << "\n";
  }

  return out.str();
}
