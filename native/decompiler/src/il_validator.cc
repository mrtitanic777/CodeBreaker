// XEXMagic - IL Validator

#include "il_validator.h"

#include <cstdio>
#include <map>
#include <set>

ILValidationResult ValidateILFunction(ILFunction& func) {
  ILValidationResult result;
  result.valid = true;

  std::set<int> defined_ids;
  std::map<uint32_t, int> block_addr_map;

  // Register parameter SSA IDs as defined
  for (auto& p : func.params) {
    defined_ids.insert(p.id);
  }

  // Also register all values from the pool (some are created as initial reads)
  for (auto& v : func.values) {
    defined_ids.insert(v.id);
  }

  // Build block address map
  for (size_t i = 0; i < func.blocks.size(); i++) {
    block_addr_map[func.blocks[i].start_addr] = (int)i;
  }

  // Check 1: Every block has at least one instruction
  for (auto& block : func.blocks) {
    if (block.instrs.empty()) {
      char buf[128];
      snprintf(buf, sizeof(buf),
               "Block at 0x%08X has no instructions", block.start_addr);
      result.warnings.push_back(buf);
    }
  }

  // Check 2: Successor addresses resolve to known blocks
  for (auto& block : func.blocks) {
    for (uint32_t succ : block.successors) {
      if (block_addr_map.find(succ) == block_addr_map.end()) {
        // Could be an external jump - warning, not error
        char buf[128];
        snprintf(buf, sizeof(buf),
                 "Block 0x%08X successor 0x%08X not in function",
                 block.start_addr, succ);
        result.warnings.push_back(buf);
      }
    }
  }

  // Check 3: Count instructions, check for InlineAsm
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      result.total_instructions++;
      if (il.op == ILOp::InlineAsm) {
        result.inline_asm_count++;
        char buf[256];
        snprintf(buf, sizeof(buf),
                 "InlineAsm at 0x%08X: %s", il.address, il.asm_text.c_str());
        result.warnings.push_back(buf);
      }
    }
  }

  // Check 4: Type consistency for float ops
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      switch (il.op) {
        case ILOp::FAdd: case ILOp::FSub: case ILOp::FMul: case ILOp::FDiv:
        case ILOp::FMulAdd: case ILOp::FMulSub:
        case ILOp::FNMulAdd: case ILOp::FNMulSub:
        case ILOp::FNeg: case ILOp::FAbs: case ILOp::FNAbs: case ILOp::FMove:
        case ILOp::FRecipEst: case ILOp::FRSqrtEst: case ILOp::FSelect:
        case ILOp::FRoundSingle:
        case ILOp::FConvToIntWord: case ILOp::FConvToIntWordTrunc:
        case ILOp::FConvToIntDbl: case ILOp::FConvToIntDblTrunc:
        case ILOp::FConvFromInt:
          if (il.dst.id != kILNoValue && !ILTypeIsFloat(il.dst.type)) {
            char buf[128];
            snprintf(buf, sizeof(buf),
                     "FP op at 0x%08X has non-float dst type", il.address);
            result.warnings.push_back(buf);
          }
          break;
        default:
          break;
      }
    }
  }

  // Check 5: Call targets have names
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      if (il.op == ILOp::Call && il.call_name.empty()) {
        char buf[128];
        snprintf(buf, sizeof(buf),
                 "Call at 0x%08X has no resolved target name", il.address);
        result.warnings.push_back(buf);
      }
    }
  }

  // Coverage
  if (result.total_instructions > 0) {
    result.coverage_pct = (float)(result.total_instructions - result.inline_asm_count)
                          / (float)result.total_instructions * 100.0f;
  }

  if (!result.errors.empty()) result.valid = false;

  // Store on function
  func.validated = true;
  func.validation_errors = result.errors;
  func.validation_warnings = result.warnings;
  func.total_instructions = result.total_instructions;
  func.inline_asm_count = result.inline_asm_count;

  return result;
}
