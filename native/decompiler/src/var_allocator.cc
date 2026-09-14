// XEXMagic - Variable Allocator
// Classifies SSA values into: arg, result, ExternReference, Index, Temp.

#include "var_allocator.h"

#include <cstdio>
#include <map>
#include <set>

// Check if a value is used as a loop index:
// - Incremented/decremented inside a block that has a back-edge
// - Used in a compare before a backward branch
static bool IsLoopIndex(const ILFunction& func, int value_id) {
  // Find all blocks with back-edges (successor address < block start)
  std::set<uint32_t> loop_blocks;
  for (auto& block : func.blocks) {
    for (uint32_t succ : block.successors) {
      if (succ < block.start_addr || succ == block.start_addr) {
        loop_blocks.insert(block.start_addr);
      }
    }
  }
  if (loop_blocks.empty()) return false;

  // Check if value_id is the destination of an Add/Sub in a loop block,
  // where one source is the same register as the destination
  for (auto& block : func.blocks) {
    if (loop_blocks.find(block.start_addr) == loop_blocks.end()) continue;
    for (auto& il : block.instrs) {
      if ((il.op == ILOp::Add || il.op == ILOp::Sub) &&
          il.dst.id == value_id) {
        return true;
      }
      // Also: BranchDecrCTRZero/NonZero patterns count
      if ((il.op == ILOp::BranchDecrCTRZero ||
           il.op == ILOp::BranchDecrCTRNonZero)) {
        return true;  // CTR is used as loop counter
      }
    }
  }

  // Check if the value is compared and then used in a backward branch
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      if ((il.op == ILOp::Compare || il.op == ILOp::CompareU ||
           il.op == ILOp::CompareImm || il.op == ILOp::CompareImmU) &&
          (il.src1.id == value_id || il.src2.id == value_id)) {
        // Check if this compare's result feeds a backward branch
        for (auto& il2 : block.instrs) {
          if (il2.op == ILOp::BranchCond && il2.cmp_ref == il.dst.id &&
              il2.target <= block.start_addr) {
            return true;
          }
        }
      }
    }
  }

  return false;
}

// Check if a value comes from an external reference:
// - Address computed via lis + ori (two-instruction absolute address)
// - Non-stack, non-parameter memory load from a global address
// - Relocation target
static bool IsExternReference(const ILFunction& func, int value_id) {
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      if (il.dst.id != value_id) continue;

      // Call results are "result", not extern references
      if (il.op == ILOp::Call) return false;

      // Load from non-stack address (RA != r1)
      if (il.op == ILOp::Load || il.op == ILOp::LoadIndexed) {
        if (il.src1.ppc_reg != 1 && il.src1.ppc_reg != kILRegNone) {
          // Not stack-relative load — could be global
          // Check if base comes from a lis (assign of shifted immediate)
          for (auto& v : func.values) {
            if (v.id == il.src1.id) {
              // Check if this value was assigned a high immediate (lis pattern)
              for (auto& b2 : func.blocks) {
                for (auto& il2 : b2.instrs) {
                  if (il2.dst.id == v.id && il2.op == ILOp::Assign) {
                    // Check if the name looks like a hex constant (lis pattern)
                    if (!il2.src1.name.empty() && il2.src1.name[0] == '0')
                      return true;
                  }
                }
              }
            }
          }
        }
      }

      // Assign of a high constant (lis pattern)
      if (il.op == ILOp::Assign && !il.src1.name.empty()) {
        const auto& n = il.src1.name;
        if (n.size() > 4 && n.find("0x") == 0) {
          // Large constant, likely an address
          return true;
        }
      }
    }
  }
  return false;
}

void AllocateVariables(ILFunction& func) {
  int temp_counter = 0;
  int index_counter = 0;
  int extern_counter = 0;
  int result_counter = 0;

  // Track which value IDs are call results (r3 after Call)
  std::set<int> call_result_ids;
  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      if (il.op == ILOp::Call && il.dst.id != kILNoValue) {
        call_result_ids.insert(il.dst.id);
      }
    }
  }

  // Track which value IDs are parameters
  std::set<int> param_ids;
  for (auto& p : func.params) {
    param_ids.insert(p.id);
  }

  // Track which value IDs are immediates (already named)
  std::set<int> immediate_ids;
  for (auto& v : func.values) {
    if (!v.name.empty() && v.ppc_reg == kILRegNone) {
      // Immediate constant — keep its name
      immediate_ids.insert(v.id);
    }
  }

  // Assign names
  for (auto& v : func.values) {
    if (v.id == kILNoValue) continue;
    if (!v.name.empty()) continue;  // Already named (params, immediates)

    char buf[32];

    // Priority 1: Parameters (already named during function building)
    if (param_ids.count(v.id)) continue;

    // Priority 2: Call results
    if (call_result_ids.count(v.id)) {
      snprintf(buf, sizeof(buf), "result%02d", result_counter++);
      v.name = buf;
      continue;
    }

    // Priority 3: Extern references
    if (IsExternReference(func, v.id)) {
      snprintf(buf, sizeof(buf), "ExternReference%02d", extern_counter++);
      v.name = buf;
      continue;
    }

    // Priority 4: Loop indices
    if (IsLoopIndex(func, v.id)) {
      snprintf(buf, sizeof(buf), "Index%02d", index_counter++);
      v.name = buf;
      continue;
    }

    // Priority 5: Temporaries
    if (immediate_ids.count(v.id)) continue;
    snprintf(buf, sizeof(buf), "Temp%02d", temp_counter++);
    v.name = buf;
  }

  // Propagate names back to instructions
  std::map<int, std::string> name_map;
  for (auto& v : func.values) {
    if (v.id != kILNoValue && !v.name.empty())
      name_map[v.id] = v.name;
  }

  for (auto& block : func.blocks) {
    for (auto& il : block.instrs) {
      auto fix = [&](ILValue& val) {
        if (val.id != kILNoValue && val.name.empty()) {
          auto it = name_map.find(val.id);
          if (it != name_map.end()) val.name = it->second;
        }
      };
      fix(il.dst);
      fix(il.src1);
      fix(il.src2);
      fix(il.src3);
    }
  }
}
