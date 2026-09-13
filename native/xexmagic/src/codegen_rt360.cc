// XEXMagic - RT360 Code Generator
// Emits C/C++ using PPCContext* ABI for Nexia360 static recompilation.

#include "codegen_rt360.h"

#include <algorithm>
#include <cstdio>
#include <map>
#include <set>
#include <sstream>

#include "ppc_disasm.h"
#include "xex_loader.h"

// ---------------------------------------------------------------------------
// Save/restore stub inlining
// ---------------------------------------------------------------------------

// Check if target_addr is a __savegprlr_N or __restgprlr_N stub.
// Returns the register number (14-31) or -1 if not a stub.
static int GetSaveGPRLR(uint32_t target, const CodeGenConfig& config) {
  if (config.gplr_save_start == 0) return -1;
  if (target >= config.gplr_save_start &&
      target < config.gplr_save_start + 18 * 4) {
    return 14 + (target - config.gplr_save_start) / 4;
  }
  return -1;
}

static int GetRestGPRLR(uint32_t target, const CodeGenConfig& config) {
  if (config.gplr_rest_start == 0) return -1;
  if (target >= config.gplr_rest_start &&
      target < config.gplr_rest_start + 18 * 4) {
    return 14 + (target - config.gplr_rest_start) / 4;
  }
  return -1;
}

// Emit inline __savegprlr_N: save rN..r31 and LR to PPC stack.
// PPC layout: std rI, offset(r1) where offset(i) = (i-33)*8; stw r12,-8(r1)
// Guest memory is big-endian, so use RT360_BSWAP macros.
static void EmitInlineSaveGPRLR(std::ostream& out, int start_reg,
                                 const std::string& addr_comment) {
  out << "    /* inline __savegprlr_" << start_reg << " */" << addr_comment << "\n";
  out << "    { uint8_t* _mem = ctx->virtual_membase; uint32_t _sp = (uint32_t)ctx->r[1];\n";
  for (int i = start_reg; i <= 31; i++) {
    int offset = (i - 33) * 8;
    out << "      *(uint64_t*)(_mem + _sp + (" << offset
        << ")) = RT360_BSWAP64(ctx->r[" << i << "]);\n";
  }
  // Save LR at sp-8 as 32-bit big-endian
  out << "      *(uint32_t*)(_mem + _sp + (-8)) = RT360_BSWAP32((uint32_t)ctx->lr);\n";
  out << "    }\n";
}

// Emit inline __restgprlr_N: restore rN..r31 and LR from PPC stack, then return.
// volatile_saves: map of callee_reg -> volatile_reg for JIT-compatible restore.
static void EmitInlineRestGPRLR(std::ostream& out, int start_reg,
                                 const std::string& addr_comment,
                                 const std::map<int, int>* volatile_saves = nullptr) {
  // NOTE: volatile reg restore removed — it causes cascading corruption
  // when nested calls also have volatile restores.
  (void)volatile_saves;
  out << "    /* inline __restgprlr_" << start_reg << " */" << addr_comment << "\n";
  out << "    { uint8_t* _mem = ctx->virtual_membase; uint32_t _sp = (uint32_t)ctx->r[1];\n";
  for (int i = start_reg; i <= 31; i++) {
    int offset = (i - 33) * 8;
    out << "      ctx->r[" << i << "] = RT360_BSWAP64(*(uint64_t*)(_mem + _sp + ("
        << offset << ")));\n";
  }
  // Restore LR from sp-8
  out << "      ctx->lr = RT360_BSWAP32(*(uint32_t*)(_mem + _sp + (-8)));\n";
  out << "    }\n";
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static bool IsVolatileGPR(int reg) {
  return reg == 0 || (reg >= 3 && reg <= 12);
}

// Flush volatile locals to context (before calls/returns)
static void EmitFlushVolatiles(std::ostream& out) {
  out << "    ctx->r[0]=_r0;ctx->r[3]=_r3;ctx->r[4]=_r4;ctx->r[5]=_r5;"
      << "ctx->r[6]=_r6;ctx->r[7]=_r7;ctx->r[8]=_r8;ctx->r[9]=_r9;"
      << "ctx->r[10]=_r10;ctx->r[11]=_r11;ctx->r[12]=_r12;\n";
}

// Reload r3 from context after a call (return value)
static void EmitReloadAfterCall(std::ostream& out) {
  out << "    _r3=ctx->r[3];\n";
}

static std::string ValName(const ILValue& v) {
  if (v.id == kILNoValue) return "0";

  if (v.ppc_reg >= 0 && v.ppc_reg < 32) {
    char buf[32];
    if (IsVolatileGPR(v.ppc_reg))
      snprintf(buf, sizeof(buf), "_r%d", v.ppc_reg);
    else
      snprintf(buf, sizeof(buf), "ctx->r[%d]", v.ppc_reg);
    return buf;
  }
  if (v.ppc_reg >= 32 && v.ppc_reg < 64) {
    char buf[32];
    snprintf(buf, sizeof(buf), "ctx->f[%d]", v.ppc_reg - 32);
    return buf;
  }

  // Immediate / constant values encoded in the name
  if (!v.name.empty()) {
    // Check if it's a numeric literal
    if (v.name[0] == '0' || v.name[0] == '-' ||
        (v.name[0] >= '1' && v.name[0] <= '9'))
      return v.name;
  }

  // Fallback: unnamed value with no register (shouldn't happen in RT360 mode)
  if (!v.name.empty()) return v.name;
  char buf[16];
  snprintf(buf, sizeof(buf), "v%d", v.id);
  return buf;
}

// Destination — L-value.
static std::string DstName(const ILValue& v) {
  if (v.ppc_reg >= 0 && v.ppc_reg < 32) {
    char buf[32];
    if (IsVolatileGPR(v.ppc_reg))
      snprintf(buf, sizeof(buf), "_r%d", v.ppc_reg);
    else
      snprintf(buf, sizeof(buf), "ctx->r[%d]", v.ppc_reg);
    return buf;
  }
  if (v.ppc_reg >= 32 && v.ppc_reg < 64) {
    char buf[32];
    snprintf(buf, sizeof(buf), "ctx->f[%d]", v.ppc_reg - 32);
    return buf;
  }
  if (!v.name.empty()) return v.name;
  char buf[16];
  snprintf(buf, sizeof(buf), "v%d", v.id);
  return buf;
}

static std::string TypeStr(ILType t) {
  return ILTypeName(t);
}

// RT360 memory load helper name from access type
static const char* RT360LoadHelper(ILType t) {
  switch (t) {
    case ILType::U8:  return "rt360_load8";
    case ILType::S8:  return "rt360_load_s8";
    case ILType::U16: return "rt360_load16";
    case ILType::S16: return "rt360_load_s16";
    case ILType::U32: case ILType::S32: return "rt360_load32";
    case ILType::U64: case ILType::S64: return "rt360_load64";
    case ILType::F32: return "rt360_loadf32";
    case ILType::F64: return "rt360_loadf64";
    default:          return "rt360_load32";
  }
}

static const char* RT360StoreHelper(ILType t) {
  switch (t) {
    case ILType::U8:  case ILType::S8:  return "rt360_store8";
    case ILType::U16: case ILType::S16: return "rt360_store16";
    case ILType::U32: case ILType::S32: return "rt360_store32";
    case ILType::U64: case ILType::S64: return "rt360_store64";
    case ILType::F32: return "rt360_storef32";
    case ILType::F64: return "rt360_storef64";
    default:          return "rt360_store32";
  }
}

// Generate address expression: (uint32_t)(base + offset) for rt360 helpers
static std::string RT360Addr(const ILValue& base, int32_t offset) {
  std::string b = ValName(base);
  if (base.id == kILNoValue) {
    char buf[32];
    snprintf(buf, sizeof(buf), "0x%X", (uint32_t)offset);
    return buf;
  }
  if (offset == 0) {
    return "(uint32_t)" + b;
  }
  if (offset > 0) {
    char buf[64];
    snprintf(buf, sizeof(buf), "(uint32_t)%s + 0x%X", b.c_str(), (uint32_t)offset);
    return buf;
  }
  char buf[64];
  snprintf(buf, sizeof(buf), "(uint32_t)%s - 0x%X", b.c_str(), (uint32_t)(-offset));
  return buf;
}

static std::string RT360IndexedAddr(const ILValue& base, const ILValue& idx) {
  std::string b = (base.id == kILNoValue) ? "0" : ValName(base);
  return "(uint32_t)(" + b + " + " + ValName(idx) + ")";
}

static std::string LabelName(uint32_t addr, uint32_t func_entry) {
  char buf[32];
  snprintf(buf, sizeof(buf), "label_%X", addr - func_entry);
  return buf;
}

// Block function name (returns void*, used by trampoline).
static std::string BlockFuncName(uint32_t addr, const ILFunction& func) {
  if (addr == func.entry) return "_blk_" + func.name;
  char buf[40];
  snprintf(buf, sizeof(buf), "_blk_%08X", addr);
  return buf;
}

// Entry wrapper name (returns void, starts trampoline). This is what
// the function table and external callers see.
static std::string WrapperFuncName(uint32_t addr, const ILFunction& func) {
  if (addr == func.entry) return func.name;
  char buf[32];
  snprintf(buf, sizeof(buf), "sub_%08X", addr);
  return buf;
}

// ---------------------------------------------------------------------------
// Generate source for one RT360 function
// ---------------------------------------------------------------------------

std::string GenerateRT360FunctionSource(const ILFunction& func,
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
    out << "/* Cycle estimate: " << func.total_cycles << " cycles ("
        << (int)ns << "." << (int)((ns - (int)ns) * 10) << " ns @ 3.2 GHz) */\n";
  }

  // #line directive for source mapping
  out << "#line 1 \"ppc:0x" << std::hex << func.entry << std::dec << "\"\n";

  // Function header comment
  out << "/* " << func.name << " (0x" << std::hex << func.entry << ", 0x"
      << (func.end - func.entry) << std::dec << " bytes, "
      << func.blocks.size() << " blocks) */\n";

  // Detect volatile-to-callee-saved register saves at function entry.
  // Pattern: mr rN, rM where N is callee-saved (14-31) and M is volatile (0,3-12).
  // The JIT's SSA never clobbers the volatile reg, but our codegen does.
  // After restgprlr, restore volatile regs from their callee-saved copies.
  std::map<int, int> volatile_saves;  // callee_reg -> volatile_reg
  {
    bool stop = false;
    for (auto& blk : func.blocks) {
      if (stop) break;
      for (auto& il : blk.instrs) {
      // Skip prolog instructions: mflr, savegprlr call, stwu/StackAlloc, li
      if (il.op == ILOp::Call) {
        int save_reg = GetSaveGPRLR(il.target, config);
        if (save_reg < 0) { stop = true; break; }
        continue;  // savegprlr call, skip
      }
      if (il.op == ILOp::StackAlloc || il.op == ILOp::Nop) continue;
      if (il.op == ILOp::Branch || il.op == ILOp::BranchCond ||
          il.op == ILOp::Return) {
        stop = true; break;
      }
      // Detect mr rN, rM (callee-saved = volatile register save)
      bool is_mr = (il.op == ILOp::Assign && il.src1.ppc_reg >= 0) ||
                   (il.op == ILOp::Or && il.src1.ppc_reg == il.src2.ppc_reg);
      if (is_mr && il.dst.ppc_reg >= 14 && il.dst.ppc_reg <= 31 &&
          il.src1.ppc_reg >= 4 && il.src1.ppc_reg <= 12) {
        // Only restore r4-r12 (volatile non-return regs).
        // r3 is the return value — restoring it would overwrite the result.
        // r0-r2 are special (r0=scratch, r1=SP, r2=TOC).
        volatile_saves[il.dst.ppc_reg] = il.src1.ppc_reg;
      } else if (il.op != ILOp::Assign && il.op != ILOp::Add &&
                 il.op != ILOp::Sub && il.op != ILOp::Store &&
                 il.op != ILOp::StoreUpdate && il.op != ILOp::MoveSPR &&
                 il.op != ILOp::Or) {
        // First non-prolog instruction — stop scanning
        stop = true; break;
      }
      }
    }
  }


  // Find compare for a cmp_ref
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

  // Dispatch function: takes absolute guest address, jumps to the right instruction.
  // Every 4-byte-aligned address within the function has a label — full coverage.
  out << "void " << func.name << "(RT360Context* ctx, uint32_t _addr) {\n";

  // Volatile register locals — shadows ctx->r[N] for r0, r3-r12.
  // The JIT keeps these in host x64 registers; callees get their own copies.
  // We must use C locals to prevent cross-call clobbering.
  out << "    uint64_t _r0, _r3, _r4, _r5, _r6, _r7, _r8, _r9, _r10, _r11, _r12;\n";
  out << "    void* _dispatch_target = 0;\n";

  // Address dispatch: switch sets a goto target, falls through to init, then jumps.
  out << "    switch(_addr) {\n";
  out << "    default: return;\n";
  { std::set<uint32_t> seen_addrs;
    for (auto& block : func.blocks) {
      for (auto& il : block.instrs) {
        if (il.address >= func.entry && il.address < func.end &&
            !seen_addrs.count(il.address)) {
          seen_addrs.insert(il.address);
          out << "    case 0x" << std::hex << il.address << std::dec
              << ": _dispatch_target = &&blk_" << std::hex << il.address << std::dec << "; break;\n";
        }
      }
    }
  }
  out << "    }\n";

  // Init volatile locals from context, then jump to target
  out << "    _r0=ctx->r[0];_r3=ctx->r[3];_r4=ctx->r[4];_r5=ctx->r[5];\n";
  out << "    _r6=ctx->r[6];_r7=ctx->r[7];_r8=ctx->r[8];_r9=ctx->r[9];\n";
  out << "    _r10=ctx->r[10];_r11=ctx->r[11];_r12=ctx->r[12];\n";
  out << "    goto *_dispatch_target;\n\n";

  // Emit all blocks with per-instruction labels
  std::set<uint32_t> emitted_labels;
  for (size_t bi = 0; bi < func.blocks.size(); bi++) {
    auto& block = func.blocks[bi];

    if (config.emit_cycle_comments && block.total_cycles > 0) {
      out << "    /* block " << bi << ": " << block.instrs.size()
          << " instrs, " << block.total_cycles << " cycles */\n";
    }

    for (auto& il : block.instrs) {
      std::string dst = DstName(il.dst);   // L-value (no cast)
      std::string s1 = ValName(il.src1);   // R-value (with cast for GPR)
      std::string s2 = ValName(il.src2);
      std::string s3 = ValName(il.src3);

      // Per-instruction label — every 4-byte address is addressable (deduped)
      if (il.address >= func.entry && il.address < func.end &&
          !emitted_labels.count(il.address)) {
        emitted_labels.insert(il.address);
        out << "blk_" << std::hex << il.address << std::dec << ":\n";
      }

      // Optional PPC address comment
      std::string addr_comment;
      if (config.emit_address_comments) {
        char buf[64];
        snprintf(buf, sizeof(buf), "  /* 0x%08X */", il.address);
        addr_comment = buf;
      }

      // CA flag: save operands as string refs (they're expressions, not variables)
      // The actual save happens inline in the CA computation code below.
      std::string ca_s1 = s1, ca_s2 = s2; // copies of the operand expressions

      switch (il.op) {
        case ILOp::Nop:
          break;
        case ILOp::StackAlloc:
          // stwu r1, -N(r1): store old SP at new SP, then update r1
          // il.imm = positive size (already negated by IL builder)
          out << "    rt360_store32(ctx, (uint32_t)ctx->r[1] - " << il.imm
              << ", (uint32_t)ctx->r[1]);" << addr_comment << "\n";
          out << "    ctx->r[1] = (uint32_t)ctx->r[1] - " << il.imm << ";\n";
          break;

        // ---- Arithmetic ----
        // Xenon runs in 64-bit mode. GPRs are 64-bit.
        // 32-bit ALU ops (add, sub, or, and, shifts) zero-extend result to 64.
        // addi/addis sign-extend the 16-bit immediate to 64 bits.
        // mr (or rA,rS,rS) copies full 64 bits.
        case ILOp::Assign:
          if (il.src1.ppc_reg >= 0 && il.src1.ppc_reg < 32) {
            // mr: copy full 64-bit register
            out << "    " << dst << " = " << s1 << ";" << addr_comment << "\n";
          } else {
            // li/lis: immediate — sign-extend to 64-bit (PPC sign-extends imm16)
            out << "    " << dst << " = (uint64_t)(int64_t)(int32_t)(" << s1 << ");" << addr_comment << "\n";
          }
          break;
        case ILOp::Add:
          if (il.src2.ppc_reg >= 0) {
            // add rD,rA,rB: 64-bit register + register
            out << "    " << dst << " = (uint64_t)(" << s1 << " + " << s2 << ");" << addr_comment << "\n";
          } else {
            // addi rD,rA,imm: 64-bit register + sign-extended immediate
            out << "    " << dst << " = (uint64_t)(" << s1 << " + (int64_t)(int32_t)(" << s2 << "));" << addr_comment << "\n";
          }
          break;
        case ILOp::AddCarry:
          // adde: rD = rA + rB + CA. (addc maps here too but IL builder
          // should NOT add CA for addc — verify IL builder.)
          // JIT stores 64-bit result.
          out << "    " << dst << " = (uint64_t)((uint64_t)" << s1 << " + (uint64_t)" << s2 << " + (uint64_t)ctx->xer_ca);" << addr_comment << "\n"; break;
        case ILOp::Sub:
          // subf: 64-bit subtract
          out << "    " << dst << " = (uint64_t)(" << s1 << " - " << s2 << ");" << addr_comment << "\n"; break;
        case ILOp::SubCarry:
          // subfe: rD = ~rA + rB + CA. JIT stores 64-bit result.
          out << "    " << dst << " = (uint64_t)(~(uint64_t)" << s2 << " + (uint64_t)" << s1 << " + (uint64_t)ctx->xer_ca);" << addr_comment << "\n"; break;
        case ILOp::Mul:
          // mullw: sign-extend both to 64, multiply, store full 64-bit result (JIT behavior)
          out << "    " << dst << " = (uint64_t)((int64_t)(int32_t)(uint32_t)" << s1 << " * (int64_t)(int32_t)(uint32_t)" << s2 << ");" << addr_comment << "\n"; break;
        case ILOp::MulHigh:
          out << "    " << dst << " = (uint32_t)((int64_t)(int32_t)(uint32_t)" << s1 << " * (int64_t)(int32_t)(uint32_t)" << s2 << " >> 32);" << addr_comment << "\n"; break;
        case ILOp::MulHighU:
          out << "    " << dst << " = (uint32_t)((uint64_t)(uint32_t)" << s1 << " * (uint64_t)(uint32_t)" << s2 << " >> 32);" << addr_comment << "\n"; break;
        case ILOp::Div:
          // divw: 32-bit signed divide, zero-extend
          out << "    " << dst << " = (uint32_t)(((int32_t)(uint32_t)" << s2 << " != 0) ? "
              << "((int32_t)(uint32_t)" << s1 << " / (int32_t)(uint32_t)" << s2 << ") : 0);" << addr_comment << "\n"; break;
        case ILOp::DivU:
          out << "    " << dst << " = (uint32_t)(((uint32_t)" << s2 << " != 0) ? "
              << "((uint32_t)" << s1 << " / (uint32_t)" << s2 << ") : 0);" << addr_comment << "\n"; break;
        case ILOp::Neg:
          // neg: 64-bit negate
          out << "    " << dst << " = (uint64_t)(-(int64_t)" << s1 << ");" << addr_comment << "\n"; break;

        // ---- Logic ----
        // PPC 64-bit mode: logical ops operate on full 64-bit registers.
        case ILOp::And:  out << "    " << dst << " = " << s1 << " & " << s2 << ";" << addr_comment << "\n"; break;
        case ILOp::Or:   out << "    " << dst << " = " << s1 << " | " << s2 << ";" << addr_comment << "\n"; break;
        case ILOp::Xor:  out << "    " << dst << " = " << s1 << " ^ " << s2 << ";" << addr_comment << "\n"; break;
        case ILOp::Nand: out << "    " << dst << " = ~(" << s1 << " & " << s2 << ");" << addr_comment << "\n"; break;
        case ILOp::Nor:  out << "    " << dst << " = ~(" << s1 << " | " << s2 << ");" << addr_comment << "\n"; break;
        case ILOp::Orc:  out << "    " << dst << " = " << s1 << " | ~" << s2 << ";" << addr_comment << "\n"; break;
        case ILOp::Andc: out << "    " << dst << " = " << s1 << " & ~" << s2 << ";" << addr_comment << "\n"; break;
        case ILOp::Eqv:  out << "    " << dst << " = ~(" << s1 << " ^ " << s2 << ");" << addr_comment << "\n"; break;
        case ILOp::Not:  out << "    " << dst << " = ~" << s1 << ";" << addr_comment << "\n"; break;

        // ---- Shift/Rotate ----
        // slw/srw: Nexia returns 0 when shift >= 32. We clamp with & 0x3F mask
        // and explicit zero-for->=32 check.
        case ILOp::Shl:
          if (il.src2.id != kILNoValue)
            out << "    { uint32_t _sh = (uint32_t)" << s2 << " & 0x3F; " << dst << " = (_sh < 32) ? (uint32_t)((uint32_t)" << s1 << " << _sh) : 0; }" << addr_comment << "\n";
          else
            out << "    " << dst << " = (uint32_t)((uint32_t)" << s1 << " << " << il.imm << ");" << addr_comment << "\n";
          break;
        case ILOp::Shr:
          if (il.src2.id != kILNoValue)
            out << "    { uint32_t _sh = (uint32_t)" << s2 << " & 0x3F; " << dst << " = (_sh < 32) ? (uint32_t)((uint32_t)" << s1 << " >> _sh) : 0; }" << addr_comment << "\n";
          else
            out << "    " << dst << " = (uint32_t)((uint32_t)" << s1 << " >> " << il.imm << ");" << addr_comment << "\n";
          break;
        case ILOp::Sar:
          // sraw: arithmetic shift right word, sign-extend result to 64-bit
          out << "    { uint32_t _sh = (uint32_t)" << s2 << " & 0x3F; " << dst
              << " = (uint64_t)(int64_t)(int32_t)((_sh < 32) ? ((int32_t)(uint32_t)" << s1 << " >> _sh) : ((int32_t)(uint32_t)" << s1 << " >> 31)); }"
              << addr_comment << "\n"; break;
        case ILOp::SarImm:
          // srawi: arithmetic shift right by immediate, sign-extend result to 64-bit
          out << "    " << dst << " = (uint64_t)(int64_t)((int32_t)(uint32_t)" << s1 << " >> " << il.imm << ");" << addr_comment << "\n"; break;

        case ILOp::RotateAndMask: {
          bool is_64bit = (il.mask & 0xE0000000) != 0;
          if (is_64bit) {
            // 64-bit rotate ops (opcode 30)
            uint32_t sh = (uint32_t)il.imm;
            uint32_t mb = il.mask & 0x3F;
            if (il.mask & 0x80000000) {
              // rldicr: MASK(0, mb)
              uint64_t m = (mb == 63) ? 0xFFFFFFFFFFFFFFFFULL
                           : ~((1ULL << (63 - mb)) - 1);
              if (sh == 0) {
                out << "    " << dst << " = (uint64_t)" << s1 << " & 0x"
                    << std::hex << m << std::dec << "ULL;" << addr_comment << "\n";
              } else if (mb == 63 - sh) {
                // sldi
                out << "    " << dst << " = (uint64_t)" << s1 << " << " << sh << ";" << addr_comment << "\n";
              } else {
                out << "    " << dst << " = rt360_rotl64((uint64_t)" << s1 << ", " << sh
                    << ") & 0x" << std::hex << m << std::dec << "ULL;" << addr_comment << "\n";
              }
            } else if (il.mask & 0x40000000) {
              // rldic: MASK(mb, 63-sh)
              uint32_t me = 63 - sh;
              uint64_t m = (mb <= me) ? ((~0ULL >> mb) & (~0ULL << (63 - me)))
                           : ((~0ULL >> mb) | (~0ULL << (63 - me)));
              out << "    " << dst << " = rt360_rotl64((uint64_t)" << s1 << ", " << sh
                  << ") & 0x" << std::hex << m << std::dec << "ULL;" << addr_comment << "\n";
            } else {
              // rldicl: MASK(mb, 63)
              uint64_t m = ~0ULL >> mb;
              if (sh == 64 - mb && mb > 0) {
                // srdi
                out << "    " << dst << " = (uint64_t)" << s1 << " >> " << mb << ";" << addr_comment << "\n";
              } else if (sh == 0) {
                out << "    " << dst << " = (uint64_t)" << s1 << " & 0x"
                    << std::hex << m << std::dec << "ULL;" << addr_comment << "\n";
              } else {
                out << "    " << dst << " = rt360_rotl64((uint64_t)" << s1 << ", " << sh
                    << ") & 0x" << std::hex << m << std::dec << "ULL;" << addr_comment << "\n";
              }
            }
          } else {
            // 32-bit rotate (opcode 21): use precomputed mask, zero-extend result
            uint32_t sh = il.imm & 0x1F;
            char mbuf[16];
            snprintf(mbuf, sizeof(mbuf), "0x%08X", il.mask);
            out << "    " << dst << " = (uint32_t)(rt360_rotl32(" << s1 << ", " << sh
                << ") & " << mbuf << ");" << addr_comment << "\n";
          }
          break;
        }
        case ILOp::RotateInsert: {
          bool is_64bit = (il.mask & 0x40000000) != 0;
          if (is_64bit) {
            // rldimi: 64-bit rotate and insert
            uint32_t sh = (uint32_t)il.imm;
            uint32_t mb = il.mask & 0x3F;
            uint32_t me = 63 - sh;
            uint64_t m = (mb <= me) ? ((~0ULL >> mb) & (~0ULL << (63 - me)))
                         : ((~0ULL >> mb) | (~0ULL << (63 - me)));
            out << "    { uint64_t _r = rt360_rotl64((uint64_t)" << s1 << ", " << sh << ");\n";
            out << "      " << dst << " = (_r & 0x" << std::hex << m << std::dec
                << "ULL) | ((uint64_t)" << s2 << " & 0x" << std::hex << ~m << std::dec << "ULL); }"
                << addr_comment << "\n";
          } else {
            // 32-bit rlwimi, zero-extend result
            char mbuf[16];
            snprintf(mbuf, sizeof(mbuf), "0x%08X", il.mask);
            uint32_t sh = il.imm & 0x1F;
            out << "    " << dst << " = (uint32_t)((" << s2 << " & ~" << mbuf
                << ") | (rt360_rotl32(" << s1 << ", " << sh << ") & " << mbuf << "));" << addr_comment << "\n";
          }
          break;
        }
        case ILOp::RotateAndMaskReg: {
          if ((il.mask & 0xE0000000) != 0) {
            // 64-bit register-based rotate: rldcl or rldcr
            uint32_t mb = il.mask & 0x3F;
            uint64_t mask64;
            if (il.mask & 0x20000000) {
              // rldcl: MASK(mb, 63)
              mask64 = (~0ULL) >> mb;
            } else {
              // rldcr: MASK(0, mb)
              mask64 = (~0ULL) << (63 - mb);
            }
            char mbuf[24];
            snprintf(mbuf, sizeof(mbuf), "0x%016llXULL", (unsigned long long)mask64);
            out << "    " << dst << " = (uint64_t)(rt360_rotl64((uint64_t)" << s1
                << ", (uint32_t)" << s2 << " & 0x3F) & " << mbuf << ");" << addr_comment << "\n";
          } else {
            // 32-bit rlwnm: rotate by register amount, mask, zero-extend
            char mbuf[16];
            snprintf(mbuf, sizeof(mbuf), "0x%08X", il.mask);
            out << "    " << dst << " = (uint32_t)(rt360_rotl32(" << s1 << ", " << s2
                << ") & " << mbuf << ");" << addr_comment << "\n";
          }
          break;
        }
        case ILOp::CountLeadingZeros:
          // cntlzw: count leading zeros of 32-bit value, result is 0-32
          out << "    " << dst << " = (uint32_t)rt360_cntlzw(" << s1 << ");" << addr_comment << "\n"; break;

        case ILOp::ExtendByte:
          // extsb: sign-extend byte to 64-bit (JIT sign-extends to INT64)
          out << "    " << dst << " = (uint64_t)(int64_t)(int8_t)(uint8_t)" << s1 << ";" << addr_comment << "\n"; break;
        case ILOp::ExtendHalf:
          // extsh: sign-extend halfword to 64-bit (JIT sign-extends to INT64)
          out << "    " << dst << " = (uint64_t)(int64_t)(int16_t)(uint16_t)" << s1 << ";" << addr_comment << "\n"; break;
        case ILOp::ExtendWord:
          // extsw: sign-extend word to 64-bit (this IS a 64-bit operation)
          out << "    " << dst << " = (uint64_t)(int64_t)(int32_t)" << s1 << ";" << addr_comment << "\n"; break;

        // ---- Memory (RT360: use helpers with byte-swap) ----
        case ILOp::Load:
          out << "    " << dst << " = " << RT360LoadHelper(il.access_type)
              << "(ctx, " << RT360Addr(il.src1, il.imm) << ");" << addr_comment << "\n";
          break;
        case ILOp::LoadUpdate:
          // Load + update base register: lXu rD, d(rA) → rD=mem[rA+d], rA=rA+d
          out << "    " << dst << " = " << RT360LoadHelper(il.access_type)
              << "(ctx, " << RT360Addr(il.src1, il.imm) << ");" << addr_comment << "\n";
          // Update base register (src1 = rA): rA = rA + d
          out << "    " << DstName(il.src1) << " = " << ValName(il.src1) << " + " << il.imm << ";\n";
          break;
        case ILOp::LoadIndexed:
          out << "    " << dst << " = " << RT360LoadHelper(il.access_type)
              << "(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n";
          break;
        case ILOp::Store:
          out << "    " << RT360StoreHelper(il.access_type) << "(ctx, "
              << RT360Addr(il.src1, il.imm) << ", " << s2 << ");" << addr_comment << "\n";
          break;
        case ILOp::StoreUpdate:
          // Store + update base register: stXu rS, d(rA) → mem[rA+d]=rS, rA=rA+d
          out << "    " << RT360StoreHelper(il.access_type) << "(ctx, "
              << RT360Addr(il.src1, il.imm) << ", " << s2 << ");" << addr_comment << "\n";
          // Update base register (src1 = rA): rA = rA + d
          out << "    " << DstName(il.src1) << " = " << s1 << " + " << il.imm << ";\n";
          break;
        case ILOp::StoreIndexed:
          out << "    " << RT360StoreHelper(il.access_type) << "(ctx, "
              << RT360IndexedAddr(il.src1, il.src2) << ", " << s3 << ");" << addr_comment << "\n";
          break;

        case ILOp::LoadMultiple: {
          // Unroll: load r[start_reg]..r[31], skip base register to avoid clobber
          int start_reg = il.dst.ppc_reg;
          int base_reg = il.src1.ppc_reg;
          if (start_reg >= 0 && start_reg <= 31) {
            for (int reg = start_reg; reg <= 31; reg++) {
              if (reg == base_reg) continue;  // skip rA — don't clobber base
              out << "    ctx->r[" << reg << "] = (uint64_t)rt360_load32(ctx, "
                  << RT360Addr(il.src1, il.imm + (reg - start_reg) * 4) << ");" << addr_comment << "\n";
            }
          } else {
            out << "    /* lmw: unknown start reg */\n";
          }
          break;
        }
        case ILOp::StoreMultiple: {
          int start_reg = il.dst.ppc_reg;
          if (start_reg >= 0 && start_reg <= 31) {
            for (int reg = start_reg; reg <= 31; reg++) {
              out << "    rt360_store32(ctx, "
                  << RT360Addr(il.src1, il.imm + (reg - start_reg) * 4)
                  << ", (uint32_t)ctx->r[" << reg << "]);" << addr_comment << "\n";
            }
          } else {
            out << "    /* stmw: unknown start reg */\n";
          }
          break;
        }

        case ILOp::LoadReserved:
          out << "    " << dst << " = rt360_lwarx(ctx, "
              << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n";
          break;
        case ILOp::StoreConditional:
          out << "    rt360_stwcx(ctx, " << RT360IndexedAddr(il.src1, il.src2)
              << ", " << s3 << ");" << addr_comment << "\n";
          break;
        case ILOp::LoadByteReverse:
          out << "    " << dst << " = rt360_load32_reverse(ctx, "
              << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n";
          break;
        case ILOp::StoreByteReverse:
          out << "    rt360_store32_reverse(ctx, " << RT360IndexedAddr(il.src1, il.src2)
              << ", " << s3 << ");" << addr_comment << "\n";
          break;

        // ---- Float memory (RT360: use float helpers) ----
        case ILOp::FLoad:
          out << "    " << dst << " = " << RT360LoadHelper(il.access_type)
              << "(ctx, " << RT360Addr(il.src1, il.imm) << ");" << addr_comment << "\n";
          break;
        case ILOp::FStore:
          out << "    " << RT360StoreHelper(il.access_type) << "(ctx, "
              << RT360Addr(il.src1, il.imm) << ", " << s2 << ");" << addr_comment << "\n";
          break;
        case ILOp::FLoadIndexed:
          out << "    " << dst << " = " << RT360LoadHelper(il.access_type)
              << "(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n";
          break;
        case ILOp::FStoreIndexed:
          out << "    " << RT360StoreHelper(il.access_type) << "(ctx, "
              << RT360IndexedAddr(il.src1, il.src2) << ", " << s3 << ");" << addr_comment << "\n";
          break;
        case ILOp::FLoadUpdate:
          out << "    " << dst << " = " << RT360LoadHelper(il.access_type)
              << "(ctx, " << RT360Addr(il.src1, il.imm) << ");" << addr_comment << "\n";
          // Update base register
          out << "    " << DstName(il.src1) << " = " << ValName(il.src1) << " + " << il.imm << ";\n";
          break;
        case ILOp::FStoreUpdate:
          out << "    " << RT360StoreHelper(il.access_type) << "(ctx, "
              << RT360Addr(il.src1, il.imm) << ", " << s2 << ");" << addr_comment << "\n";
          // Update base register
          out << "    " << DstName(il.src1) << " = " << ValName(il.src1) << " + " << il.imm << ";\n";
          break;

        // ---- Float arithmetic (same as standard) ----
        case ILOp::FAdd:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(" << s1 << " + " << s2 << ");" << addr_comment << "\n";
          else
            out << "    " << dst << " = " << s1 << " + " << s2 << ";" << addr_comment << "\n";
          break;
        case ILOp::FSub:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(" << s1 << " - " << s2 << ");" << addr_comment << "\n";
          else
            out << "    " << dst << " = " << s1 << " - " << s2 << ";" << addr_comment << "\n";
          break;
        case ILOp::FMul:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(" << s1 << " * " << s2 << ");" << addr_comment << "\n";
          else
            out << "    " << dst << " = " << s1 << " * " << s2 << ";" << addr_comment << "\n";
          break;
        case ILOp::FDiv:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(" << s1 << " / " << s2 << ");" << addr_comment << "\n";
          else
            out << "    " << dst << " = " << s1 << " / " << s2 << ";" << addr_comment << "\n";
          break;
        case ILOp::FMulAdd:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(" << s1 << " * " << s2 << " + " << s3 << ");" << addr_comment << "\n";
          else
            out << "    " << dst << " = " << s1 << " * " << s2 << " + " << s3 << ";" << addr_comment << "\n";
          break;
        case ILOp::FMulSub:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(" << s1 << " * " << s2 << " - " << s3 << ");" << addr_comment << "\n";
          else
            out << "    " << dst << " = " << s1 << " * " << s2 << " - " << s3 << ";" << addr_comment << "\n";
          break;
        case ILOp::FNMulAdd:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(-(" << s1 << " * " << s2 << " + " << s3 << "));" << addr_comment << "\n";
          else
            out << "    " << dst << " = -(" << s1 << " * " << s2 << " + " << s3 << ");" << addr_comment << "\n";
          break;
        case ILOp::FNMulSub:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)(-(" << s1 << " * " << s2 << " - " << s3 << "));" << addr_comment << "\n";
          else
            out << "    " << dst << " = -(" << s1 << " * " << s2 << " - " << s3 << ");" << addr_comment << "\n";
          break;
        case ILOp::FNeg:  out << "    " << dst << " = -" << s1 << ";" << addr_comment << "\n"; break;
        case ILOp::FAbs:  out << "    " << dst << " = fabs(" << s1 << ");" << addr_comment << "\n"; break;
        case ILOp::FNAbs: out << "    " << dst << " = -fabs(" << s1 << ");" << addr_comment << "\n"; break;
        case ILOp::FMove: out << "    " << dst << " = " << s1 << ";" << addr_comment << "\n"; break;
        case ILOp::FRecipEst:
          // fres: reciprocal estimate — JIT converts to single, reciprocals, converts back
          out << "    " << dst << " = (double)(1.0f / (float)" << s1 << ");" << addr_comment << "\n"; break;
        case ILOp::FRSqrtEst:
          out << "    " << dst << " = 1.0 / sqrt(" << s1 << ");" << addr_comment << "\n"; break;
        case ILOp::FSqrt:
          if (il.access_type == ILType::F32)
            out << "    " << dst << " = (double)(float)sqrt(" << s1 << ");" << addr_comment << "\n";
          else
            out << "    " << dst << " = sqrt(" << s1 << ");" << addr_comment << "\n";
          break;
        case ILOp::FSelect:
          out << "    " << dst << " = (" << s1 << " >= 0.0) ? " << s2 << " : " << s3 << ";" << addr_comment << "\n"; break;
        case ILOp::FRoundSingle:
          out << "    " << dst << " = (float)" << s1 << ";" << addr_comment << "\n"; break;
        case ILOp::FConvToIntWord:
        case ILOp::FConvToIntWordTrunc: {
          // PPC fctiw/fctiwz: convert FP to int32, sign-extend to int64,
          // store bit pattern as double. NaN → 0x80000000.
          out << "    { int64_t _iv;\n";
          out << "      if (" << s1 << " != " << s1 << ") _iv = (int64_t)(int32_t)0x80000000;\n";  // NaN check
          out << "      else _iv = (int64_t)(int32_t)" << s1 << ";\n";
          out << "      memcpy(&" << dst << ", &_iv, 8); }" << addr_comment << "\n";
          break;
        }
        case ILOp::FConvToIntDbl:
        case ILOp::FConvToIntDblTrunc: {
          // PPC fctid/fctidz: convert FP to int64, store bit pattern in FPR.
          // NaN → 0x8000000000000000.
          out << "    { int64_t _iv;\n";
          out << "      if (" << s1 << " != " << s1 << ") _iv = (int64_t)0x8000000000000000ULL;\n";
          out << "      else _iv = (int64_t)" << s1 << ";\n";
          out << "      memcpy(&" << dst << ", &_iv, 8); }" << addr_comment << "\n";
          break;
        }
        case ILOp::FConvFromInt:
          // PPC fcfid: interpret FPR bits as int64, convert to double
          out << "    { int64_t _iv; memcpy(&_iv, &" << s1 << ", 8);"
              << " " << dst << " = (double)_iv; }" << addr_comment << "\n"; break;

        // ---- Float compare (RT360: set CR fields) ----
        case ILOp::FCompareOrdered:
        case ILOp::FCompareUnordered: {
          int crf = il.cr_field;
          // NaN check: x != x is true only for NaN
          out << "    { int _nan = (" << s1 << " != " << s1 << " || " << s2 << " != " << s2 << ");\n";
          out << "      rt360_cr_field(ctx, " << crf << ")->lt = (!_nan && " << s1 << " < " << s2 << ") ? 1 : 0;\n";
          out << "      rt360_cr_field(ctx, " << crf << ")->gt = (!_nan && " << s1 << " > " << s2 << ") ? 1 : 0;\n";
          out << "      rt360_cr_field(ctx, " << crf << ")->eq = (!_nan && " << s1 << " == " << s2 << ") ? 1 : 0;\n";
          out << "      rt360_cr_field(ctx, " << crf << ")->so = _nan ? 1 : 0; }" << addr_comment << "\n";
          break;
        }

        // ---- Integer compare (RT360: set CR fields) ----
        case ILOp::Compare:
          out << "    rt360_compare_signed(ctx, " << il.cr_field << ", (int32_t)"
              << s1 << ", (int32_t)" << s2 << ");" << addr_comment << "\n";
          break;
        case ILOp::CompareU:
          out << "    rt360_compare_unsigned(ctx, " << il.cr_field << ", (uint32_t)"
              << s1 << ", (uint32_t)" << s2 << ");" << addr_comment << "\n";
          break;
        case ILOp::CompareImm:
          out << "    rt360_compare_signed(ctx, " << il.cr_field << ", (int32_t)"
              << s1 << ", (int32_t)" << (int32_t)il.imm << ");" << addr_comment << "\n";
          break;
        case ILOp::CompareImmU:
          out << "    rt360_compare_unsigned(ctx, " << il.cr_field << ", (uint32_t)"
              << s1 << ", " << (uint32_t)il.imm << "u);" << addr_comment << "\n";
          break;

        // ---- Branches ----
        case ILOp::Branch:
          if (il.target >= func.entry && il.target < func.end) {
            out << "    goto blk_" << std::hex << il.target << std::dec << ";" << addr_comment << "\n";
          } else {
            // Inline restore stubs — the tail branch IS the epilog
            int rest_reg = GetRestGPRLR(il.target, config);
            if (rest_reg >= 0) {
              EmitFlushVolatiles(out);
              EmitInlineRestGPRLR(out, rest_reg, addr_comment, &volatile_saves);
              out << "    return;" << addr_comment << "\n";
              break;
            }
            // External branch = tail call
            EmitFlushVolatiles(out);
            out << "    rt360_call(ctx, 0x" << std::hex << il.target << std::dec
                << "); return;" << addr_comment << "\n";
          }
          break;

        case ILOp::BranchCond: {
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
            bool is_unsigned = (cmp->op == ILOp::CompareU || cmp->op == ILOp::CompareImmU);
            ILCmpKind kind = il.cmp_kind;
            if (is_unsigned && kind >= ILCmpKind::LT && kind <= ILCmpKind::GE) {
              kind = (ILCmpKind)((int)kind + (int)ILCmpKind::LTU - (int)ILCmpKind::LT);
            }
            if (is_unsigned) {
              cond_str = "(uint32_t)" + lhs + " " + ILCmpKindStr(kind) + " (uint32_t)" + rhs;
            } else {
              cond_str = "(int32_t)" + lhs + " " + ILCmpKindStr(kind) + " (int32_t)" + rhs;
            }
          } else {
            // Fallback: read CR field directly
            const char* bit = "eq";
            switch (il.cmp_kind) {
              case ILCmpKind::EQ: bit = "eq"; break;
              case ILCmpKind::NE: bit = "eq"; break;  // inverted below
              case ILCmpKind::LT: case ILCmpKind::LTU: bit = "lt"; break;
              case ILCmpKind::GT: case ILCmpKind::GTU: bit = "gt"; break;
              case ILCmpKind::LE: case ILCmpKind::LEU: bit = "lt"; break;  // lt || eq
              case ILCmpKind::GE: case ILCmpKind::GEU: bit = "gt"; break;  // gt || eq
              default: break;
            }
            char buf[64];
            snprintf(buf, sizeof(buf), "rt360_cr_field(ctx, %d)->%s", il.cr_field, bit);
            if (il.cmp_kind == ILCmpKind::NE) {
              cond_str = std::string("!") + buf;
            } else if (il.cmp_kind == ILCmpKind::LE || il.cmp_kind == ILCmpKind::LEU) {
              snprintf(buf, sizeof(buf), "(rt360_cr_field(ctx, %d)->lt || rt360_cr_field(ctx, %d)->eq)",
                       il.cr_field, il.cr_field);
              cond_str = buf;
            } else if (il.cmp_kind == ILCmpKind::GE || il.cmp_kind == ILCmpKind::GEU) {
              snprintf(buf, sizeof(buf), "(rt360_cr_field(ctx, %d)->gt || rt360_cr_field(ctx, %d)->eq)",
                       il.cr_field, il.cr_field);
              cond_str = buf;
            } else {
              cond_str = buf;
            }
          }

          if (il.is_cond_ctr) {
            if (il.is_cond_ctr_link) {
              out << "    if (" << cond_str << ") {\n";
              EmitFlushVolatiles(out);
              out << "      ctx->lr = (uint64_t)0x" << std::hex << (il.address + 4) << std::dec << "u;\n";
              out << "      rt360_call(ctx, (uint32_t)ctx->ctr);\n";
              EmitReloadAfterCall(out);
              out << "    }" << addr_comment << "\n";
            } else {
              out << "    if (" << cond_str << ") { "; EmitFlushVolatiles(out);
              out << "    rt360_call(ctx, (uint32_t)ctx->ctr); return; }" << addr_comment << "\n";
            }
          } else if (il.is_cond_return) {
            out << "    if (" << cond_str << ") { "; EmitFlushVolatiles(out);
            out << "    return; }" << addr_comment << "\n";
          } else if (il.target >= func.entry && il.target < func.end) {
            out << "    if (" << cond_str << ") goto "
                << "blk_" << std::hex << il.target << std::dec << ";" << addr_comment << "\n";
          } else {
            out << "    if (" << cond_str << ") { "; EmitFlushVolatiles(out);
            out << "    rt360_call(ctx, 0x"
                << std::hex << il.target << std::dec << "); return; }" << addr_comment << "\n";
          }
          break;
        }

        case ILOp::BranchDecrCTRZero:
          out << "    ctx->ctr--;\n";
          if (il.target >= func.entry && il.target < func.end) {
            out << "    if (ctx->ctr == 0) goto " << "blk_" << std::hex << il.target << std::dec << ";" << addr_comment << "\n";
          } else {
            out << "    if (ctx->ctr == 0) { "; EmitFlushVolatiles(out);
            out << "    rt360_call(ctx, 0x" << std::hex << il.target << std::dec << "); return; }" << addr_comment << "\n";
          }
          break;
        case ILOp::BranchDecrCTRNonZero:
          out << "    ctx->ctr--;\n";
          if (il.target >= func.entry && il.target < func.end) {
            out << "    if (ctx->ctr != 0) goto " << "blk_" << std::hex << il.target << std::dec << ";" << addr_comment << "\n";
          } else {
            out << "    if (ctx->ctr != 0) { "; EmitFlushVolatiles(out);
            out << "    rt360_call(ctx, 0x" << std::hex << il.target << std::dec << "); return; }" << addr_comment << "\n";
          }
          break;

        case ILOp::BranchCTR:
          EmitFlushVolatiles(out);
          out << "    rt360_call(ctx, (uint32_t)ctx->ctr); return;" << addr_comment << "\n"; break;
        case ILOp::CallCTR:
          EmitFlushVolatiles(out);
          out << "    ctx->lr = (uint64_t)0x" << std::hex << (il.address + 4) << std::dec << "u;\n";
          out << "    rt360_call(ctx, (uint32_t)ctx->ctr);" << addr_comment << "\n";
          EmitReloadAfterCall(out);
          break;
        case ILOp::BranchLR:
          EmitFlushVolatiles(out);
          out << "    rt360_call(ctx, (uint32_t)ctx->lr); return;" << addr_comment << "\n"; break;

        // ---- Call (RT360: dispatch through context) ----
        case ILOp::Call: {
          // Sanitize call_name for use in C comments
          std::string safe_name = il.call_name;
          for (size_t pos = 0; (pos = safe_name.find("*/", pos)) != std::string::npos; )
            safe_name.replace(pos, 2, "* /");
          for (size_t pos = 0; (pos = safe_name.find("/*", pos)) != std::string::npos; )
            safe_name.replace(pos, 2, "/ *");

          // Inline save/restore stubs instead of dispatching
          int save_reg = GetSaveGPRLR(il.target, config);
          if (save_reg >= 0) {
            EmitInlineSaveGPRLR(out, save_reg, addr_comment);
            break;
          }

          EmitFlushVolatiles(out);
          out << "    ctx->lr = (uint64_t)0x" << std::hex << (il.address + 4) << std::dec << "u;\n";

          if (il.target != 0 && config.import_thunk_addrs.count(il.target)) {
            out << "    _thunk_" << std::hex << il.target << std::dec
                << "(ctx, 0x" << std::hex << il.target << std::dec
                << ");  /* " << safe_name << " (inline HLE) */" << addr_comment << "\n";
          } else if (il.target != 0) {
            out << "    rt360_call(ctx, 0x" << std::hex << il.target << std::dec
                << ");  /* " << safe_name << " */" << addr_comment << "\n";
          } else {
            out << "    rt360_call(ctx, 0);  /* " << safe_name << " (unresolved) */" << addr_comment << "\n";
          }
          EmitReloadAfterCall(out);
          break;
        }

        // ---- Return ----
        case ILOp::Return: {
          EmitFlushVolatiles(out);
          out << "    return;" << addr_comment << "\n";
          break;
        }

        // ---- SPR (RT360: use ctx-> directly) ----
        case ILOp::LoadTimeBase:
          out << "    " << dst << " = (uint32_t)rt360_mftb()";
          if (il.imm == 269) out << " >> 32";
          out << ";" << addr_comment << "\n";
          break;
        case ILOp::MoveSPR:
          if (il.spr_read) {
            // mfspr: read full 64-bit SPR value
            if (il.spr == ILSPR::LR)       out << "    " << dst << " = ctx->lr;" << addr_comment << "\n";
            else if (il.spr == ILSPR::CTR)  out << "    " << dst << " = ctx->ctr;" << addr_comment << "\n";
            else if (il.spr == ILSPR::XER)  {
              // Pack XER from split fields: SO | OV | CA | ... (simplified)
              out << "    " << dst << " = ((uint64_t)ctx->xer_so << 31) | ((uint64_t)ctx->xer_ov << 30) | ((uint64_t)ctx->xer_ca << 29);" << addr_comment << "\n";
            }
            else if (il.spr == ILSPR::VRSAVE) out << "    " << dst << " = (uint64_t)ctx->vrsave;" << addr_comment << "\n";
            else if (il.spr == ILSPR::PVR)  out << "    " << dst << " = (uint64_t)0x710700;" << addr_comment << "\n";  // Xenon PVR
            else out << "    " << dst << " = 0;  /* mfspr unknown */" << addr_comment << "\n";
          } else {
            // mtspr: write full 64-bit SPR value
            if (il.spr == ILSPR::LR)       out << "    ctx->lr = " << s1 << ";" << addr_comment << "\n";
            else if (il.spr == ILSPR::CTR)  out << "    ctx->ctr = " << s1 << ";" << addr_comment << "\n";
            else if (il.spr == ILSPR::XER)  {
              out << "    { uint32_t _xv = (uint32_t)" << s1 << ";\n";
              out << "      ctx->xer_so = (_xv >> 31) & 1; ctx->xer_ov = (_xv >> 30) & 1; ctx->xer_ca = (_xv >> 29) & 1; }" << addr_comment << "\n";
            }
            else if (il.spr == ILSPR::VRSAVE) out << "    ctx->vrsave = (uint32_t)" << s1 << ";" << addr_comment << "\n";
            else out << "    /* mtspr unknown = " << s1 << " */" << addr_comment << "\n";
          }
          break;
        case ILOp::MoveCR:
          if (il.spr_read) {
            // mfcr: pack all 8 CR fields into a 32-bit value
            out << "    { uint32_t _crv = 0;" << addr_comment << "\n";
            out << "      for(int _i=0;_i<8;_i++) { RT360CRField* _f=rt360_cr_field(ctx,_i);\n";
            out << "        _crv |= ((_f->lt<<3)|(_f->gt<<2)|(_f->eq<<1)|_f->so) << (28-_i*4); }\n";
            out << "      " << dst << " = _crv; }\n";
          } else {
            // mtcrf: write GPR bits into CR fields selected by mask
            out << "    { uint32_t _val = " << s1 << ";" << addr_comment << "\n";
            for (int f = 0; f < 8; f++) {
              if (il.mask & (1 << (7 - f))) {
                int shift = (7 - f) * 4;
                out << "      rt360_cr_field(ctx, " << f << ")->lt = (_val >> " << (shift + 3) << ") & 1;\n";
                out << "      rt360_cr_field(ctx, " << f << ")->gt = (_val >> " << (shift + 2) << ") & 1;\n";
                out << "      rt360_cr_field(ctx, " << f << ")->eq = (_val >> " << (shift + 1) << ") & 1;\n";
                out << "      rt360_cr_field(ctx, " << f << ")->so = (_val >> " << (shift + 0) << ") & 1;\n";
              }
            }
            out << "    }\n";
          }
          break;
        case ILOp::MoveMSR:
          if (il.spr_read) out << "    " << dst << " = (uint32_t)ctx->msr;" << addr_comment << "\n";
          else out << "    ctx->msr = (uint64_t)" << s1 << ";" << addr_comment << "\n";
          break;

        // ---- CR operations ----
        case ILOp::CRAnd: case ILOp::CROr: case ILOp::CRXor:
        case ILOp::CRNand: case ILOp::CRNor: case ILOp::CRAndc:
        case ILOp::CROrc: case ILOp::CREqv: {
          // CR logical ops: XL-form — BT=bits[6:10], BA=bits[11:15], BB=bits[16:20]
          uint32_t pw = il.ppc_word;
          int bt = (pw >> 21) & 0x1F, ba = (pw >> 16) & 0x1F, bb = (pw >> 11) & 0x1F;
          const char* cr_bits[] = {"lt", "gt", "eq", "so"};
          char dst_str[64], a_str[64], b_str[64];
          snprintf(dst_str, sizeof(dst_str), "rt360_cr_field(ctx, %d)->%s", bt/4, cr_bits[bt%4]);
          snprintf(a_str, sizeof(a_str), "rt360_cr_field(ctx, %d)->%s", ba/4, cr_bits[ba%4]);
          snprintf(b_str, sizeof(b_str), "rt360_cr_field(ctx, %d)->%s", bb/4, cr_bits[bb%4]);
          const char* op_str = "&&";
          switch (il.op) {
            case ILOp::CRAnd:  op_str = "&"; break;
            case ILOp::CROr:   op_str = "|"; break;
            case ILOp::CRXor:  op_str = "^"; break;
            case ILOp::CRNand: op_str = "& ~"; break;  // will negate result
            case ILOp::CRNor:  op_str = "| ~"; break;  // will negate result
            case ILOp::CRAndc: op_str = "& !"; break;
            case ILOp::CROrc:  op_str = "| !"; break;
            case ILOp::CREqv:  op_str = "== (uint8_t)"; break;
            default: break;
          }
          if (il.op == ILOp::CRNand)
            out << "    " << dst_str << " = !(" << a_str << " & " << b_str << ");" << addr_comment << "\n";
          else if (il.op == ILOp::CRNor)
            out << "    " << dst_str << " = !(" << a_str << " | " << b_str << ");" << addr_comment << "\n";
          else if (il.op == ILOp::CRAndc)
            out << "    " << dst_str << " = " << a_str << " & !" << b_str << ";" << addr_comment << "\n";
          else if (il.op == ILOp::CROrc)
            out << "    " << dst_str << " = " << a_str << " | !" << b_str << ";" << addr_comment << "\n";
          else if (il.op == ILOp::CREqv)
            out << "    " << dst_str << " = (" << a_str << " == " << b_str << ") ? 1 : 0;" << addr_comment << "\n";
          else
            out << "    " << dst_str << " = " << a_str << " " << op_str << " " << b_str << ";" << addr_comment << "\n";
          break;
        }
        case ILOp::MoveCRField: {
          // mcrf: copy entire CR field — BF=bits[6:8], BFA=bits[11:13]
          uint32_t pw = il.ppc_word;
          int bf = (pw >> 23) & 0x7, bfa = (pw >> 18) & 0x7;
          out << "    *rt360_cr_field(ctx, " << bf << ") = *rt360_cr_field(ctx, " << bfa << ");" << addr_comment << "\n";
          break;
        }

        // ---- System (RT360: use runtime helpers) ----
        case ILOp::Syscall:
          out << "    /* syscall */;" << addr_comment << "\n"; break;
        case ILOp::Sync:
          out << "    rt360_sync();" << addr_comment << "\n"; break;
        case ILOp::LWSync:
          out << "    rt360_lwsync();" << addr_comment << "\n"; break;
        case ILOp::PTESync:
          out << "    rt360_ptesync();" << addr_comment << "\n"; break;
        case ILOp::ISync:
          out << "    rt360_isync();" << addr_comment << "\n"; break;
        case ILOp::EIEIO:
          out << "    rt360_eieio();" << addr_comment << "\n"; break;

        // ---- Cache (RT360: use runtime helpers) ----
        case ILOp::DCacheFlush:
          out << "    rt360_dcbf(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n"; break;
        case ILOp::DCacheStore:
          out << "    rt360_dcbst(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n"; break;
        case ILOp::DCacheTouch:
          out << "    rt360_dcbt(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n"; break;
        case ILOp::DCacheTouchStore:
          out << "    rt360_dcbt(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n"; break;
        case ILOp::DCacheZero:
          out << "    rt360_dcbz(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n"; break;
        case ILOp::ICacheInvalidate:
          out << "    rt360_icbi(ctx, " << RT360IndexedAddr(il.src1, il.src2) << ");" << addr_comment << "\n"; break;

        // ---- Trap ----
        case ILOp::Trap:
          out << "    rt360_trap(ctx, " << il.imm << ", (int32_t)" << s1 << ", (int32_t)" << s2 << ");" << addr_comment << "\n"; break;
        case ILOp::TrapImm:
          out << "    rt360_trap(ctx, " << il.mask << ", (int32_t)" << s1 << ", " << (int32_t)il.imm << ");" << addr_comment << "\n"; break;

        case ILOp::FStoreIntWord:
          // stfiwx: store low 32 bits of FPR bit pattern (NOT convert to int)
          out << "    { uint64_t _fbits; memcpy(&_fbits, &" << s3 << ", 8);\n";
          out << "      rt360_store32(ctx, " << RT360IndexedAddr(il.src1, il.src2)
              << ", (uint32_t)_fbits); }" << addr_comment << "\n";
          break;

        case ILOp::LoadFPSCR: {
          // mffs: FPSCR value → zero-extend to 64 → reinterpret as double
          out << "    { uint64_t _fv = (uint64_t)ctx->fpscr.value;\n";
          out << "      memcpy(&" << dst << ", &_fv, 8); }" << addr_comment << "\n";
          break;
        }
        case ILOp::StoreFPSCR: {
          // mtfsf: reinterpret FPR bits as int64 → truncate to 32 → apply mask
          out << "    { uint64_t _fb; memcpy(&_fb, &" << s1 << ", 8);\n";
          out << "      uint32_t _newval = (uint32_t)_fb;\n";
          if (il.mask == 0xFF) {
            // Full write
            out << "      ctx->fpscr.value = _newval; }" << addr_comment << "\n";
          } else {
            // Masked write
            uint32_t mask = 0;
            for (int j = 0; j < 8; j++) {
              if (il.mask & (1 << (j ^ 7))) mask |= 0xF << (4 * j);
            }
            char mbuf[16]; snprintf(mbuf, sizeof(mbuf), "0x%08X", mask);
            out << "      ctx->fpscr.value = (_newval & " << mbuf << ") | (ctx->fpscr.value & ~" << mbuf << "); }" << addr_comment << "\n";
          }
          break;
        }

        case ILOp::VMXOp: {
          // Decode VMX instruction from PPC word and emit real C code.
          uint32_t pw = il.ppc_word;
          uint32_t op = pw >> 26;
          uint32_t vd = (pw >> 21) & 0x1F;
          uint32_t va = (pw >> 16) & 0x1F;
          uint32_t vb = (pw >> 11) & 0x1F;
          uint32_t vc = (pw >> 6) & 0x1F;
          uint32_t xo = pw & 0x7FF;  // full extended opcode
          uint32_t xo6 = pw & 0x3F;  // low 6 bits for VX4-form
          uint32_t xo11 = (pw >> 0) & 0x7FF; // VX-form

          bool emitted = false;

          if (op == 4) {
            // VX128_1 format: decode register fields
            uint32_t vd128_1 = ((pw >> 21) & 0x1F) | (((pw >> 2) & 0x3) << 5);
            uint32_t ra_f = (pw >> 16) & 0x1F;
            uint32_t rb_f = (pw >> 11) & 0x1F;
            uint32_t xop_7f3 = pw & 0x7F3;

            // VMX128 load/store (VX128_1 format)
            switch (xop_7f3) {
              case 0x403: { // lvlx128 — load vector left
                std::string ea = (ra_f == 0) ? std::to_string(0) + " + (uint32_t)ctx->r[" + std::to_string(rb_f) + "]"
                    : "(uint32_t)ctx->r[" + std::to_string(ra_f) + "] + (uint32_t)ctx->r[" + std::to_string(rb_f) + "]";
                out << "    { uint32_t _ea = " << ea << ";\n"
                    << "      int _nb = 16 - (_ea & 0xF); uint8_t* _p = (uint8_t*)(ctx->virtual_membase + _ea);\n"
                    << "      uint8_t* _d = (uint8_t*)&ctx->v[" << vd128_1 << "];\n"
                    << "      for(int _i=0;_i<16;_i++) _d[_i] = (_i < _nb) ? _p[_i] : 0; }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 0x443: { // lvrx128 — load vector right
                std::string ea = (ra_f == 0) ? "(uint32_t)ctx->r[" + std::to_string(rb_f) + "]"
                    : "(uint32_t)ctx->r[" + std::to_string(ra_f) + "] + (uint32_t)ctx->r[" + std::to_string(rb_f) + "]";
                out << "    { uint32_t _ea = " << ea << ";\n"
                    << "      int _off = _ea & 0xF; int _nb = _off; uint8_t* _p = (uint8_t*)(ctx->virtual_membase + (_ea & ~0xF));\n"
                    << "      uint8_t* _d = (uint8_t*)&ctx->v[" << vd128_1 << "];\n"
                    << "      for(int _i=0;_i<16;_i++) _d[_i] = (_i >= 16-_nb) ? _p[_i-(16-_nb)] : 0; }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 0x503: { // stvlx128 — store vector left
                std::string ea = (ra_f == 0) ? "(uint32_t)ctx->r[" + std::to_string(rb_f) + "]"
                    : "(uint32_t)ctx->r[" + std::to_string(ra_f) + "] + (uint32_t)ctx->r[" + std::to_string(rb_f) + "]";
                out << "    { uint32_t _ea = " << ea << ";\n"
                    << "      int _nb = 16 - (_ea & 0xF); uint8_t* _p = (uint8_t*)(ctx->virtual_membase + _ea);\n"
                    << "      uint8_t* _s = (uint8_t*)&ctx->v[" << vd128_1 << "];\n"
                    << "      for(int _i=0;_i<_nb;_i++) _p[_i] = _s[_i]; }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 0x543: { // stvrx128 — store vector right
                std::string ea = (ra_f == 0) ? "(uint32_t)ctx->r[" + std::to_string(rb_f) + "]"
                    : "(uint32_t)ctx->r[" + std::to_string(ra_f) + "] + (uint32_t)ctx->r[" + std::to_string(rb_f) + "]";
                out << "    { uint32_t _ea = " << ea << ";\n"
                    << "      int _off = _ea & 0xF; uint8_t* _p = (uint8_t*)(ctx->virtual_membase + (_ea & ~0xF));\n"
                    << "      uint8_t* _s = (uint8_t*)&ctx->v[" << vd128_1 << "];\n"
                    << "      for(int _i=0;_i<_off;_i++) _p[16-_off+_i] = _s[16-_off+_i]; }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 0x183: { // stvewx128 — store vector element word
                std::string ea = (ra_f == 0) ? "(uint32_t)ctx->r[" + std::to_string(rb_f) + "]"
                    : "(uint32_t)ctx->r[" + std::to_string(ra_f) + "] + (uint32_t)ctx->r[" + std::to_string(rb_f) + "]";
                out << "    { uint32_t _ea = " << ea << " & ~3;\n"
                    << "      int _idx = (_ea >> 2) & 3;\n"
                    << "      *(uint32_t*)(ctx->virtual_membase + _ea) = RT360_BSWAP32(ctx->v[" << vd128_1 << "].u32[_idx]); }" << addr_comment << "\n";
                emitted = true; break;
              }
              default: break;
            }

            // VX128_5 form: vsldoi128 — shift left double by octet immediate
            // Discriminator: bit 4 = 1, opcode = 4
            if (!emitted && (pw & 0x10)) {
              // VX128_5 register decode (from LSB):
              // bits 0-1: VB128h, bits 2-3: VD128h, bit 4: disc=1,
              // bit 5: VA128h, bits 6-9: SH, bit 10: VA128H,
              // bits 11-15: VB128l, bits 16-20: VA128l, bits 21-25: VD128l
              uint32_t vd5 = ((pw >> 21) & 0x1F) | (((pw >> 2) & 0x3) << 5);
              uint32_t va5 = ((pw >> 16) & 0x1F) | (((pw >> 5) & 0x1) << 5) | (((pw >> 10) & 0x1) << 6);
              uint32_t vb5 = ((pw >> 11) & 0x1F) | ((pw & 0x3) << 5);
              uint32_t sh5 = (pw >> 6) & 0xF;
              out << "    { RT360Vec128 _r; uint8_t* _a=(uint8_t*)&ctx->v[" << va5
                  << "]; uint8_t* _b=(uint8_t*)&ctx->v[" << vb5 << "];\n"
                  << "      for(int _i=0;_i<16;_i++) { int _si=_i+" << sh5
                  << "; _r.u8[_i]=(_si<16)?_a[_si]:_b[_si-16]; }\n"
                  << "      ctx->v[" << vd5 << "]=_r; }" << addr_comment << "\n";
              emitted = true;
            }
            // Standard AltiVec (opcode 4)
            switch (xo6) {
              case 46: { // vmaddfp vD,vA,vC,vB — vD = vA*vC + vB (xo6=0x2E=46)
                out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = ctx->v["
                    << va << "].f32[_i] * ctx->v[" << vc << "].f32[_i] + ctx->v["
                    << vb << "].f32[_i]; }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 33: // vmhaddshs — multiply-high-add signed halfword saturate
                break; // TODO
              case 34: // vmsumshm — multiply-sum signed halfword modulo
                break; // TODO
              case 36: // vmsumubm
                break; // TODO
              case 37: // vmsummbm
                break; // TODO
              case 38: // vmsumuhm
                break; // TODO
              case 40: // vmrghb
                break; // TODO
              // vpkuhum is xo11=14, not here
              case 43: // vperm vD,vA,vB,vC — vector permute (xo6=0x2B=43)
                out << "    { uint8_t* _a=(uint8_t*)&ctx->v[" << va << "]; uint8_t* _b=(uint8_t*)&ctx->v["
                    << vb << "]; uint8_t* _c=(uint8_t*)&ctx->v[" << vc << "]; RT360Vec128 _r;\n"
                    << "      for(int _i=0;_i<16;_i++) { uint8_t _sel=_c[_i]&0x1F; _r.u8[_i]=(_sel<16)?_a[_sel]:_b[_sel-16]; }\n"
                    << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                emitted = true; break;
              case 42: { // vsel vD,vA,vB,vC — vD = (vA & ~vC) | (vB & vC)
                out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (ctx->v["
                    << va << "].u32[_i] & ~ctx->v[" << vc << "].u32[_i]) | (ctx->v["
                    << vb << "].u32[_i] & ctx->v[" << vc << "].u32[_i]); }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 44: { // vsldoi vD,vA,vB,SH — shift left double by octet immediate
                int sh4 = (pw >> 6) & 0xF; // SH field bits 6-9
                out << "    { RT360Vec128 _r; uint8_t* _a=(uint8_t*)&ctx->v[" << va << "]; uint8_t* _b=(uint8_t*)&ctx->v[" << vb << "];\n"
                    << "      for(int _i=0;_i<16;_i++) { int _si=_i+" << sh4 << "; _r.u8[_i]=(_si<16)?_a[_si]:_b[_si-16]; }\n"
                    << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 47: // vnmsubfp vD,vA,vC,vB — vD = -(vA*vC - vB)
                out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = -(ctx->v["
                    << va << "].f32[_i] * ctx->v[" << vc << "].f32[_i] - ctx->v["
                    << vb << "].f32[_i]); }" << addr_comment << "\n";
                emitted = true; break;
              default: break;
            }

            if (!emitted) {
              // For vector compares, Rc bit (bit 10) sets CR6. Strip it for matching.
              bool vcmp_rc = (xo11 >> 10) & 1;
              uint32_t xo11_base = xo11 & 0x3FF;
              switch (xo11_base) {
                // Vector float arithmetic
                case 10: // vaddfp
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = ctx->v["
                      << va << "].f32[_i] + ctx->v[" << vb << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 74: // vsubfp
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = ctx->v["
                      << va << "].f32[_i] - ctx->v[" << vb << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 266: // vrefp (reciprocal estimate)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = 1.0f / ctx->v["
                      << vb << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 330: // vrsqrtefp (reciprocal square root estimate)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = 1.0f / sqrtf(ctx->v["
                      << vb << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                case 1034: // vmaxfp
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = (ctx->v["
                      << va << "].f32[_i] >= ctx->v[" << vb << "].f32[_i]) ? ctx->v["
                      << va << "].f32[_i] : ctx->v[" << vb << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1098: // vminfp
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = (ctx->v["
                      << va << "].f32[_i] <= ctx->v[" << vb << "].f32[_i]) ? ctx->v["
                      << va << "].f32[_i] : ctx->v[" << vb << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector integer arithmetic
                case 0: // vaddubm
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = ctx->v["
                      << va << "].u8[_i] + ctx->v[" << vb << "].u8[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 64: // vadduhm
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = ctx->v["
                      << va << "].u16[_i] + ctx->v[" << vb << "].u16[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 128: // vadduwm
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] + ctx->v[" << vb << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1024: // vsububm
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = ctx->v["
                      << va << "].u8[_i] - ctx->v[" << vb << "].u8[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1088: // vsubuhm
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = ctx->v["
                      << va << "].u16[_i] - ctx->v[" << vb << "].u16[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1152: // vsubuwm
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] - ctx->v[" << vb << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector logical
                case 1028: // vand
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] & ctx->v[" << vb << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1092: // vandc
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] & ~ctx->v[" << vb << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1156: // vor
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] | ctx->v[" << vb << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1220: // vxor
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] ^ ctx->v[" << vb << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1284: // vnor
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ~(ctx->v["
                      << va << "].u32[_i] | ctx->v[" << vb << "].u32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector splat
                case 652: { // vspltw (xo=0x28C)
                  int uimm = va; // UIMM field in VA position
                  out << "    { uint32_t _v = ctx->v[" << vb << "].u32[" << uimm << "];\n"
                      << "      ctx->v[" << vd << "].u32[0]=_v; ctx->v[" << vd << "].u32[1]=_v;\n"
                      << "      ctx->v[" << vd << "].u32[2]=_v; ctx->v[" << vd << "].u32[3]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 588: { // vsplth (xo=0x24C)
                  int uimm = va;
                  out << "    { uint16_t _v = ctx->v[" << vb << "].u16[" << uimm << "];\n"
                      << "      for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 524: { // vspltb (xo=0x20C)
                  int uimm = va;
                  out << "    { uint8_t _v = ctx->v[" << vb << "].u8[" << uimm << "];\n"
                      << "      for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 908: { // vspltisw
                  int simm = (int32_t)(va << 27) >> 27; // sign-extend 5-bit
                  out << "    { int32_t _v = " << simm << ";\n"
                      << "      ctx->v[" << vd << "].i32[0]=_v; ctx->v[" << vd << "].i32[1]=_v;\n"
                      << "      ctx->v[" << vd << "].i32[2]=_v; ctx->v[" << vd << "].i32[3]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }

                // Vector select
                case 42 + 1024: { // vsel (xo=1100 = 0x44C) — actually xo11=1100
                  break; // TODO: need correct xo
                }

                // Vector merge
                case 140: // vmrghw
                  out << "    { ctx->v[" << vd << "].u32[0] = ctx->v[" << va << "].u32[0];\n"
                      << "      ctx->v[" << vd << "].u32[1] = ctx->v[" << vb << "].u32[0];\n"
                      << "      ctx->v[" << vd << "].u32[2] = ctx->v[" << va << "].u32[1];\n"
                      << "      ctx->v[" << vd << "].u32[3] = ctx->v[" << vb << "].u32[1]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 396: // vmrglw (xo=0x18C=396)
                  out << "    { ctx->v[" << vd << "].u32[0] = ctx->v[" << va << "].u32[2];\n"
                      << "      ctx->v[" << vd << "].u32[1] = ctx->v[" << vb << "].u32[2];\n"
                      << "      ctx->v[" << vd << "].u32[2] = ctx->v[" << va << "].u32[3];\n"
                      << "      ctx->v[" << vd << "].u32[3] = ctx->v[" << vb << "].u32[3]; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector compare float
                case 198: { // vcmpeqfp
                  bool rc = (pw >> 10) & 1;
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (ctx->v["
                      << va << "].f32[_i] == ctx->v[" << vb << "].f32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 454: { // vcmpgefp
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (ctx->v["
                      << va << "].f32[_i] >= ctx->v[" << vb << "].f32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 710: { // vcmpgtfp
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (ctx->v["
                      << va << "].f32[_i] > ctx->v[" << vb << "].f32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                }

                // Vector shift (standard AltiVec)
                case 388: // vslw (xo=0x184)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] << (ctx->v[" << vb << "].u32[_i] & 0x1F); }" << addr_comment << "\n";
                  emitted = true; break;
                case 644: // vsrw (xo=0x284)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ctx->v["
                      << va << "].u32[_i] >> (ctx->v[" << vb << "].u32[_i] & 0x1F); }" << addr_comment << "\n";
                  emitted = true; break;
                case 900: // vsraw (xo=0x384)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].i32[_i] = ctx->v["
                      << va << "].i32[_i] >> (ctx->v[" << vb << "].u32[_i] & 0x1F); }" << addr_comment << "\n";
                  emitted = true; break;
                case 260: // vslb (xo=0x104)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = ctx->v["
                      << va << "].u8[_i] << (ctx->v[" << vb << "].u8[_i] & 0x7); }" << addr_comment << "\n";
                  emitted = true; break;
                case 324: // vslh (xo=0x144)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = ctx->v["
                      << va << "].u16[_i] << (ctx->v[" << vb << "].u16[_i] & 0xF); }" << addr_comment << "\n";
                  emitted = true; break;
                case 516: // vsrb (xo=0x204)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = ctx->v["
                      << va << "].u8[_i] >> (ctx->v[" << vb << "].u8[_i] & 0x7); }" << addr_comment << "\n";
                  emitted = true; break;
                case 580: // vsrh (xo=0x244)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = ctx->v["
                      << va << "].u16[_i] >> (ctx->v[" << vb << "].u16[_i] & 0xF); }" << addr_comment << "\n";
                  emitted = true; break;
                case 4: // vrlw (xo=0x004) — standard rotate left word
                  out << "    { for(int _i=0;_i<4;_i++) { uint32_t _n=ctx->v[" << vb << "].u32[_i]&0x1F; uint32_t _v=ctx->v["
                      << va << "].u32[_i]; ctx->v[" << vd << "].u32[_i]=(_v<<_n)|(_v>>(32-_n)); } }" << addr_comment << "\n";
                  emitted = true; break;
                case 68: // vrlb (xo=0x044)
                  out << "    { for(int _i=0;_i<16;_i++) { uint8_t _n=ctx->v[" << vb << "].u8[_i]&0x7; uint8_t _v=ctx->v["
                      << va << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(_v<<_n)|(_v>>(8-_n)); } }" << addr_comment << "\n";
                  emitted = true; break;
                case 132: // vrlh (xo=0x084)
                  out << "    { for(int _i=0;_i<8;_i++) { uint16_t _n=ctx->v[" << vb << "].u16[_i]&0xF; uint16_t _v=ctx->v["
                      << va << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(_v<<_n)|(_v>>(16-_n)); } }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector splat immediate
                case 780: { // vspltisb (xo=0x30C)
                  int simm = (int32_t)(va << 27) >> 27;
                  out << "    { uint8_t _v = (uint8_t)" << simm << ";\n"
                      << "      for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 844: { // vspltish (xo=0x34C)
                  int simm = (int32_t)(va << 27) >> 27;
                  out << "    { uint16_t _v = (uint16_t)(int16_t)" << simm << ";\n"
                      << "      for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }

                // Vector merge
                case 12: // vmrghb (xo=0x00C)
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { _r.u8[_i*2]=ctx->v["
                      << va << "].u8[_i]; _r.u8[_i*2+1]=ctx->v[" << vb << "].u8[_i]; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 76: // vmrghh (xo=0x04C)
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { _r.u16[_i*2]=ctx->v["
                      << va << "].u16[_i]; _r.u16[_i*2+1]=ctx->v[" << vb << "].u16[_i]; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 268: // vmrglb (xo=0x10C)
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { _r.u8[_i*2]=ctx->v["
                      << va << "].u8[_i+8]; _r.u8[_i*2+1]=ctx->v[" << vb << "].u8[_i+8]; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 332: // vmrglh (xo=0x14C)
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { _r.u16[_i*2]=ctx->v["
                      << va << "].u16[_i+4]; _r.u16[_i*2+1]=ctx->v[" << vb << "].u16[_i+4]; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector shift whole vector
                case 452: // vsl (xo=0x1C4) — shift left by bit count in VB
                  out << "    { int _sh=ctx->v[" << vb << "].u8[15]&0x7; uint8_t* _d=(uint8_t*)&ctx->v[" << vd << "];\n"
                      << "      uint8_t* _s=(uint8_t*)&ctx->v[" << va << "]; RT360Vec128 _r={{0}};\n"
                      << "      for(int _i=0;_i<16;_i++) { _r.u8[_i]=_s[_i]<<_sh; if(_i<15) _r.u8[_i]|=_s[_i+1]>>(8-_sh); }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 708: // vsr (xo=0x2C4) — shift right by bit count in VB
                  out << "    { int _sh=ctx->v[" << vb << "].u8[15]&0x7; uint8_t* _s=(uint8_t*)&ctx->v[" << va << "]; RT360Vec128 _r={{0}};\n"
                      << "      for(int _i=0;_i<16;_i++) { _r.u8[_i]=_s[_i]>>_sh; if(_i>0) _r.u8[_i]|=_s[_i-1]<<(8-_sh); }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;

                // vsldoi handled in xo6 switch above (case 44)

                // Saturating arithmetic
                case 640: // vadduws (xo=0x280) — add unsigned word saturate
                  out << "    { for(int _i=0;_i<4;_i++) { uint64_t _s=(uint64_t)ctx->v[" << va << "].u32[_i]+(uint64_t)ctx->v["
                      << vb << "].u32[_i]; ctx->v[" << vd << "].u32[_i]=(_s>0xFFFFFFFF)?0xFFFFFFFF:(uint32_t)_s; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1664: // vsubuws (xo=0x680) — subtract unsigned word saturate
                  out << "    { for(int _i=0;_i<4;_i++) { uint32_t _a=ctx->v[" << va << "].u32[_i]; uint32_t _b=ctx->v["
                      << vb << "].u32[_i]; ctx->v[" << vd << "].u32[_i]=(_a>_b)?(_a-_b):0; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 512: // vaddubs (xo=0x200) — add unsigned byte saturate
                  out << "    { for(int _i=0;_i<16;_i++) { uint16_t _s=(uint16_t)ctx->v[" << va << "].u8[_i]+(uint16_t)ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(_s>0xFF)?0xFF:(uint8_t)_s; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 576: // vadduhs (xo=0x240) — add unsigned halfword saturate
                  out << "    { for(int _i=0;_i<8;_i++) { uint32_t _s=(uint32_t)ctx->v[" << va << "].u16[_i]+(uint32_t)ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(_s>0xFFFF)?0xFFFF:(uint16_t)_s; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1536: // vsububs (xo=0x600)
                  out << "    { for(int _i=0;_i<16;_i++) { uint8_t _a=ctx->v[" << va << "].u8[_i]; uint8_t _b=ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(_a>_b)?(_a-_b):0; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1600: // vsubuhs (xo=0x640)
                  out << "    { for(int _i=0;_i<8;_i++) { uint16_t _a=ctx->v[" << va << "].u16[_i]; uint16_t _b=ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(_a>_b)?(_a-_b):0; } }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector compare integer
                case 518: // vcmpgtub (xo=0x206)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = (ctx->v["
                      << va << "].u8[_i] > ctx->v[" << vb << "].u8[_i]) ? 0xFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 6: // vcmpequb (xo=0x006)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = (ctx->v["
                      << va << "].u8[_i] == ctx->v[" << vb << "].u8[_i]) ? 0xFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 70: // vcmpequh (xo=0x046)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (ctx->v["
                      << va << "].u16[_i] == ctx->v[" << vb << "].u16[_i]) ? 0xFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 134: // vcmpequw (xo=0x086)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (ctx->v["
                      << va << "].u32[_i] == ctx->v[" << vb << "].u32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector compare integer (more)
                case 582: // vcmpgtuh (xo=0x246)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (ctx->v["
                      << va << "].u16[_i] > ctx->v[" << vb << "].u16[_i]) ? 0xFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 646: // vcmpgtuw (xo=0x286)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (ctx->v["
                      << va << "].u32[_i] > ctx->v[" << vb << "].u32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 774: // vcmpgtsb (xo=0x306)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = ((int8_t)ctx->v["
                      << va << "].u8[_i] > (int8_t)ctx->v[" << vb << "].u8[_i]) ? 0xFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 838: // vcmpgtsh (xo=0x346)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = ((int16_t)ctx->v["
                      << va << "].u16[_i] > (int16_t)ctx->v[" << vb << "].u16[_i]) ? 0xFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 902: // vcmpgtsw (xo=0x386)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ((int32_t)ctx->v["
                      << va << "].i32[_i] > (int32_t)ctx->v[" << vb << "].i32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 966: // vcmpbfp (xo=0x3C6) — bounds check
                  out << "    { for(int _i=0;_i<4;_i++) { float _a=ctx->v[" << va << "].f32[_i]; float _b=ctx->v[" << vb << "].f32[_i];\n"
                      << "      ctx->v[" << vd << "].u32[_i] = ((_a > _b) ? 0x80000000u : 0) | ((_a < -_b) ? 0x40000000u : 0); } }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector multiply
                case 8: // vmuloub (xo=0x008)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (uint16_t)ctx->v["
                      << va << "].u8[_i*2+1] * (uint16_t)ctx->v[" << vb << "].u8[_i*2+1]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 72: // vmulouh (xo=0x048)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (uint32_t)ctx->v["
                      << va << "].u16[_i*2+1] * (uint32_t)ctx->v[" << vb << "].u16[_i*2+1]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 264: // vmulosb (xo=0x108)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (int16_t)(int8_t)ctx->v["
                      << va << "].u8[_i*2+1] * (int16_t)(int8_t)ctx->v[" << vb << "].u8[_i*2+1]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 328: // vmulosh (xo=0x148)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].i32[_i] = (int32_t)(int16_t)ctx->v["
                      << va << "].u16[_i*2+1] * (int32_t)(int16_t)ctx->v[" << vb << "].u16[_i*2+1]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 520: // vmuleub (xo=0x208)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (uint16_t)ctx->v["
                      << va << "].u8[_i*2] * (uint16_t)ctx->v[" << vb << "].u8[_i*2]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 584: // vmuleuh (xo=0x248)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (uint32_t)ctx->v["
                      << va << "].u16[_i*2] * (uint32_t)ctx->v[" << vb << "].u16[_i*2]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 776: // vmulesb (xo=0x308)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (int16_t)(int8_t)ctx->v["
                      << va << "].u8[_i*2] * (int16_t)(int8_t)ctx->v[" << vb << "].u8[_i*2]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 840: // vmulesh (xo=0x348)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].i32[_i] = (int32_t)(int16_t)ctx->v["
                      << va << "].u16[_i*2] * (int32_t)(int16_t)ctx->v[" << vb << "].u16[_i*2]; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector signed saturating add/sub
                case 768: // vaddsbs (xo=0x300)
                  out << "    { for(int _i=0;_i<16;_i++) { int16_t _s=(int16_t)(int8_t)ctx->v[" << va << "].u8[_i]+(int16_t)(int8_t)ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(uint8_t)(int8_t)((_s>127)?127:(_s<-128)?-128:_s); } }" << addr_comment << "\n";
                  emitted = true; break;
                case 832: // vaddshs (xo=0x340)
                  out << "    { for(int _i=0;_i<8;_i++) { int32_t _s=(int32_t)(int16_t)ctx->v[" << va << "].u16[_i]+(int32_t)(int16_t)ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(uint16_t)(int16_t)((_s>32767)?32767:(_s<-32768)?-32768:_s); } }" << addr_comment << "\n";
                  emitted = true; break;
                case 896: // vaddsws (xo=0x380)
                  out << "    { for(int _i=0;_i<4;_i++) { int64_t _s=(int64_t)ctx->v[" << va << "].i32[_i]+(int64_t)ctx->v["
                      << vb << "].i32[_i]; ctx->v[" << vd << "].i32[_i]=(_s>2147483647)?2147483647:(_s<-2147483648LL)?-2147483648:(int32_t)_s; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1792: // vsubsbs (xo=0x700)
                  out << "    { for(int _i=0;_i<16;_i++) { int16_t _s=(int16_t)(int8_t)ctx->v[" << va << "].u8[_i]-(int16_t)(int8_t)ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(uint8_t)(int8_t)((_s>127)?127:(_s<-128)?-128:_s); } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1856: // vsubshs (xo=0x740)
                  out << "    { for(int _i=0;_i<8;_i++) { int32_t _s=(int32_t)(int16_t)ctx->v[" << va << "].u16[_i]-(int32_t)(int16_t)ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(uint16_t)(int16_t)((_s>32767)?32767:(_s<-32768)?-32768:_s); } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1920: // vsubsws (xo=0x780)
                  out << "    { for(int _i=0;_i<4;_i++) { int64_t _s=(int64_t)ctx->v[" << va << "].i32[_i]-(int64_t)ctx->v["
                      << vb << "].i32[_i]; ctx->v[" << vd << "].i32[_i]=(_s>2147483647)?2147483647:(_s<-2147483648LL)?-2147483648:(int32_t)_s; } }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector add/sub carry
                case 384: // vaddcuw (xo=0x180)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = ((uint64_t)ctx->v["
                      << va << "].u32[_i]+(uint64_t)ctx->v[" << vb << "].u32[_i]) > 0xFFFFFFFF ? 1 : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1408: // vsubcuw (xo=0x580)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (ctx->v["
                      << va << "].u32[_i] >= ctx->v[" << vb << "].u32[_i]) ? 1 : 0; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector average
                case 1026: // vavgub (xo=0x402)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = (uint8_t)(((uint16_t)ctx->v["
                      << va << "].u8[_i]+(uint16_t)ctx->v[" << vb << "].u8[_i]+1)>>1); }" << addr_comment << "\n";
                  emitted = true; break;
                case 1090: // vavguh (xo=0x442)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (uint16_t)(((uint32_t)ctx->v["
                      << va << "].u16[_i]+(uint32_t)ctx->v[" << vb << "].u16[_i]+1)>>1); }" << addr_comment << "\n";
                  emitted = true; break;
                case 1154: // vavguw (xo=0x482)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (uint32_t)(((uint64_t)ctx->v["
                      << va << "].u32[_i]+(uint64_t)ctx->v[" << vb << "].u32[_i]+1)>>1); }" << addr_comment << "\n";
                  emitted = true; break;
                case 770: // vavgsb (xo=0x302)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = (uint8_t)(int8_t)(((int16_t)(int8_t)ctx->v["
                      << va << "].u8[_i]+(int16_t)(int8_t)ctx->v[" << vb << "].u8[_i]+1)>>1); }" << addr_comment << "\n";
                  emitted = true; break;
                case 834: // vavgsh (xo=0x342)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (uint16_t)(int16_t)(((int32_t)(int16_t)ctx->v["
                      << va << "].u16[_i]+(int32_t)(int16_t)ctx->v[" << vb << "].u16[_i]+1)>>1); }" << addr_comment << "\n";
                  emitted = true; break;
                case 898: // vavgsw (xo=0x382)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].u32[_i] = (uint32_t)(int32_t)(((int64_t)ctx->v["
                      << va << "].i32[_i]+(int64_t)ctx->v[" << vb << "].i32[_i]+1)>>1); }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector min/max unsigned
                case 514: // vmaxub (xo=0x202)
                  out << "    { for(int _i=0;_i<16;_i++) { uint8_t _a=ctx->v[" << va << "].u8[_i]; uint8_t _b=ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(_a>_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 578: // vmaxuh (xo=0x242)
                  out << "    { for(int _i=0;_i<8;_i++) { uint16_t _a=ctx->v[" << va << "].u16[_i]; uint16_t _b=ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(_a>_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 642: // vmaxuw (xo=0x282)
                  out << "    { for(int _i=0;_i<4;_i++) { uint32_t _a=ctx->v[" << va << "].u32[_i]; uint32_t _b=ctx->v["
                      << vb << "].u32[_i]; ctx->v[" << vd << "].u32[_i]=(_a>_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1538: // vminub (xo=0x602)
                  out << "    { for(int _i=0;_i<16;_i++) { uint8_t _a=ctx->v[" << va << "].u8[_i]; uint8_t _b=ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(_a<_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1602: // vminuh (xo=0x642)
                  out << "    { for(int _i=0;_i<8;_i++) { uint16_t _a=ctx->v[" << va << "].u16[_i]; uint16_t _b=ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(_a<_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1666: // vminuw (xo=0x682)
                  out << "    { for(int _i=0;_i<4;_i++) { uint32_t _a=ctx->v[" << va << "].u32[_i]; uint32_t _b=ctx->v["
                      << vb << "].u32[_i]; ctx->v[" << vd << "].u32[_i]=(_a<_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 322: // vmaxsh (xo=0x142)
                  out << "    { for(int _i=0;_i<8;_i++) { int16_t _a=ctx->v[" << va << "].u16[_i]; int16_t _b=ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(_a>_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1282: // vminsb (xo=0x502)
                  out << "    { for(int _i=0;_i<16;_i++) { int8_t _a=(int8_t)ctx->v[" << va << "].u8[_i]; int8_t _b=(int8_t)ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(uint8_t)((_a<_b)?_a:_b); } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1346: // vminsh (xo=0x542)
                  out << "    { for(int _i=0;_i<8;_i++) { int16_t _a=ctx->v[" << va << "].u16[_i]; int16_t _b=ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "].u16[_i]=(_a<_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 1410: // vminsw (xo=0x582)
                  out << "    { for(int _i=0;_i<4;_i++) { int32_t _a=ctx->v[" << va << "].i32[_i]; int32_t _b=ctx->v["
                      << vb << "].i32[_i]; ctx->v[" << vd << "].i32[_i]=(_a<_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector convert
                case 778: // vcfux (xo=0x30A) — convert unsigned int to float
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = (float)ctx->v["
                      << vb << "].u32[_i] / (float)(1u << " << va << "); }" << addr_comment << "\n";
                  emitted = true; break;
                case 842: // vcfsx (xo=0x34A) — convert signed int to float
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = (float)ctx->v["
                      << vb << "].i32[_i] / (float)(1u << " << va << "); }" << addr_comment << "\n";
                  emitted = true; break;
                case 906: // vctuxs (xo=0x38A) — convert float to unsigned int saturate
                  out << "    { for(int _i=0;_i<4;_i++) { float _v=ctx->v[" << vb << "].f32[_i]*(float)(1u<<"
                      << va << "); ctx->v[" << vd << "].u32[_i]=(_v<0)?0:(_v>4294967295.0f)?0xFFFFFFFF:(uint32_t)_v; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 970: // vctsxs (xo=0x3CA) — convert float to signed int saturate
                  out << "    { for(int _i=0;_i<4;_i++) { float _v=ctx->v[" << vb << "].f32[_i]*(float)(1u<<"
                      << va << "); ctx->v[" << vd << "].i32[_i]=(_v<-2147483648.0f)?-2147483647-1:(_v>2147483647.0f)?2147483647:(int32_t)_v; } }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector round
                case 522: // vrfin (xo=0x20A) — round to nearest
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = roundf(ctx->v["
                      << vb << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                case 586: // vrfiz (xo=0x24A) — round to zero (truncate)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = truncf(ctx->v["
                      << vb << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                case 650: // vrfip (xo=0x28A) — round to +infinity
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = ceilf(ctx->v["
                      << vb << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                case 714: // vrfim (xo=0x2CA) — round to -infinity
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = floorf(ctx->v["
                      << vb << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector shift octet
                case 1036: // vslo (xo=0x40C) — shift left by octet
                  out << "    { int _sh=(ctx->v[" << vb << "].u8[15]>>3)&0xF; RT360Vec128 _r={{0}};\n"
                      << "      uint8_t* _s=(uint8_t*)&ctx->v[" << va << "]; for(int _i=0;_i<16-_sh;_i++) _r.u8[_i]=_s[_i+_sh];\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 1100: // vsro (xo=0x44C) — shift right by octet
                  out << "    { int _sh=(ctx->v[" << vb << "].u8[15]>>3)&0xF; RT360Vec128 _r={{0}};\n"
                      << "      uint8_t* _s=(uint8_t*)&ctx->v[" << va << "]; for(int _i=0;_i<16-_sh;_i++) _r.u8[_i+_sh]=_s[_i];\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector shift arithmetic
                case 772: // vsrab (xo=0x304)
                  out << "    { for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = (uint8_t)((int8_t)ctx->v["
                      << va << "].u8[_i] >> (ctx->v[" << vb << "].u8[_i] & 0x7)); }" << addr_comment << "\n";
                  emitted = true; break;
                case 836: // vsrah (xo=0x344)
                  out << "    { for(int _i=0;_i<8;_i++) ctx->v[" << vd << "].u16[_i] = (uint16_t)((int16_t)ctx->v["
                      << va << "].u16[_i] >> (ctx->v[" << vb << "].u16[_i] & 0xF)); }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector exp/log
                case 394: // vexptefp (xo=0x18A) — 2^x estimate
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = powf(2.0f, ctx->v["
                      << vb << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                case 458: // vlogefp (xo=0x1CA) — log2 estimate
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd << "].f32[_i] = log2f(ctx->v["
                      << vb << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector pack
                case 14: // vpkuhum (xo=0x00E) — pack unsigned halfword unsigned modulo
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) _r.u8[_i]=(uint8_t)ctx->v["
                      << va << "].u16[_i];\n"
                      << "      for(int _i=0;_i<8;_i++) _r.u8[_i+8]=(uint8_t)ctx->v[" << vb << "].u16[_i];\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 78: // vpkuwum (xo=0x04E) — pack unsigned word unsigned modulo
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) _r.u16[_i]=(uint16_t)ctx->v["
                      << va << "].u32[_i];\n"
                      << "      for(int _i=0;_i<4;_i++) _r.u16[_i+4]=(uint16_t)ctx->v[" << vb << "].u32[_i];\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 142: // vpkuhus (xo=0x08E) — pack unsigned halfword unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { uint16_t _v=ctx->v["
                      << va << "].u16[_i]; _r.u8[_i]=(_v>255)?255:(uint8_t)_v; }\n"
                      << "      for(int _i=0;_i<8;_i++) { uint16_t _v=ctx->v[" << vb << "].u16[_i]; _r.u8[_i+8]=(_v>255)?255:(uint8_t)_v; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 206: // vpkuwus (xo=0x0CE) — pack unsigned word unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { uint32_t _v=ctx->v["
                      << va << "].u32[_i]; _r.u16[_i]=(_v>65535)?65535:(uint16_t)_v; }\n"
                      << "      for(int _i=0;_i<4;_i++) { uint32_t _v=ctx->v[" << vb << "].u32[_i]; _r.u16[_i+4]=(_v>65535)?65535:(uint16_t)_v; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 270: // vpkshus (xo=0x10E) — pack signed halfword unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v["
                      << va << "].u16[_i]; _r.u8[_i]=(_v>255)?255:(_v<0)?0:(uint8_t)_v; }\n"
                      << "      for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v[" << vb << "].u16[_i]; _r.u8[_i+8]=(_v>255)?255:(_v<0)?0:(uint8_t)_v; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 334: // vpkswus (xo=0x14E) — pack signed word unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v["
                      << va << "].i32[_i]; _r.u16[_i]=(_v>65535)?65535:(_v<0)?0:(uint16_t)_v; }\n"
                      << "      for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v[" << vb << "].i32[_i]; _r.u16[_i+4]=(_v>65535)?65535:(_v<0)?0:(uint16_t)_v; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 398: // vpkshss (xo=0x18E) — pack signed halfword signed saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v["
                      << va << "].u16[_i]; _r.u8[_i]=(uint8_t)(int8_t)((_v>127)?127:(_v<-128)?-128:_v); }\n"
                      << "      for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v[" << vb << "].u16[_i]; _r.u8[_i+8]=(uint8_t)(int8_t)((_v>127)?127:(_v<-128)?-128:_v); }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 462: // vpkswss (xo=0x1CE) — pack signed word signed saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v["
                      << va << "].i32[_i]; _r.u16[_i]=(uint16_t)(_v>32767)?32767:(_v<-32768)?-32768:(int16_t)_v; }\n"
                      << "      for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v[" << vb << "].i32[_i]; _r.u16[_i+4]=(uint16_t)(_v>32767)?32767:(_v<-32768)?-32768:(int16_t)_v; }\n"
                      << "      ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector unpack
                case 526: // vupkhsb (xo=0x20E) — unpack high signed byte
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) _r.u16[_i]=(uint16_t)(int16_t)(int8_t)ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 590: // vupkhsh (xo=0x24E) — unpack high signed halfword
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) _r.i32[_i]=(int32_t)(int16_t)ctx->v["
                      << vb << "].u16[_i]; ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 654: // vupklsb (xo=0x28E) — unpack low signed byte
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) _r.u16[_i]=(uint16_t)(int16_t)(int8_t)ctx->v["
                      << vb << "].u8[_i+8]; ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                case 718: // vupklsh (xo=0x2CE) — unpack low signed halfword
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) _r.i32[_i]=(int32_t)(int16_t)ctx->v["
                      << vb << "].u16[_i+4]; ctx->v[" << vd << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector sum across
                case 1672: // vsumsws (xo=0x688) — sum across signed word
                  out << "    { int32_t _s=ctx->v[" << vb << "].i32[3]; for(int _i=0;_i<4;_i++) _s+=ctx->v["
                      << va << "].i32[_i];\n"
                      << "      ctx->v[" << vd << "].u32[0]=0; ctx->v[" << vd << "].u32[1]=0; ctx->v[" << vd << "].u32[2]=0; ctx->v[" << vd << "].i32[3]=_s; }" << addr_comment << "\n";
                  emitted = true; break;

                // Vector max/min signed
                case 258: // vmaxsb (xo=0x102)
                  out << "    { for(int _i=0;_i<16;_i++) { int8_t _a=ctx->v[" << va << "].u8[_i]; int8_t _b=ctx->v["
                      << vb << "].u8[_i]; ctx->v[" << vd << "].u8[_i]=(_a>_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;
                case 386: // vmaxsw (xo=0x182)
                  out << "    { for(int _i=0;_i<4;_i++) { int32_t _a=ctx->v[" << va << "].i32[_i]; int32_t _b=ctx->v["
                      << vb << "].i32[_i]; ctx->v[" << vd << "].i32[_i]=(_a>_b)?_a:_b; } }" << addr_comment << "\n";
                  emitted = true; break;

                default: break;
              }
              // CR6 update for Rc (dot-form) vector compares
              if (emitted && vcmp_rc) {
                bool is_vcmp = (xo11_base == 198 || xo11_base == 454 || xo11_base == 710 ||
                               xo11_base == 966 || xo11_base == 6 || xo11_base == 70 ||
                               xo11_base == 134 || xo11_base == 518 || xo11_base == 774 ||
                               xo11_base == 838 || xo11_base == 902 || xo11_base == 582 ||
                               xo11_base == 646);
                if (is_vcmp) {
                  out << "    { uint32_t _all1=0xFFFFFFFF,_all0=0xFFFFFFFF;\n"
                      << "      for(int _i=0;_i<4;_i++) { _all1 &= ctx->v[" << vd << "].u32[_i]; _all0 &= ~ctx->v[" << vd << "].u32[_i]; }\n"
                      << "      ctx->cr6.lt = (_all1==0xFFFFFFFF)?1:0; ctx->cr6.gt = 0; ctx->cr6.eq = (_all0==0xFFFFFFFF)?1:0; ctx->cr6.so = 0; }\n";
                }
              }
            }
          }

          // VMX128 extensions (opcodes 5 and 6) — Xbox 360 specific
          if (!emitted && (op == 5 || op == 6)) {
            // Decode VX128 register fields
            uint32_t vd128 = ((pw >> 21) & 0x1F) | (((pw >> 2) & 0x3) << 5);
            uint32_t va128 = ((pw >> 16) & 0x1F) | (((pw >> 5) & 0x1) << 5) | (((pw >> 10) & 0x1) << 6);
            uint32_t vb128 = ((pw >> 11) & 0x1F) | (((pw >> 0) & 0x3) << 5);

            // VX128 xop is bits 4-9 (6 bits) for most forms
            uint32_t vx128_xop = (pw >> 4) & 0x3F;
            // Some forms use different xop extraction
            uint32_t vx128_full = pw & 0x3D0; // VX128 form

            if (op == 5) {
              // vperm128 uses VX128_2 form with mask 0x210, not 0x3D0
              if ((pw & 0x210) == 0x000) {
                // VX128_2 register decode
                uint32_t vc2 = (pw >> 6) & 0x7; // 3-bit VC field
                // vperm128: vD = permute(vA, vB, vD_as_permute_control)
                out << "    { uint8_t* _a=(uint8_t*)&ctx->v[" << va128 << "]; uint8_t* _b=(uint8_t*)&ctx->v["
                    << vb128 << "]; RT360Vec128 _r;\n"
                    << "      uint8_t* _vc=(uint8_t*)&ctx->v[" << vd128 << "];\n"
                    << "      for(int _i=0;_i<16;_i++) { uint8_t _sel=_vc[_i]&0x1F; _r.u8[_i]=(_sel<16)?_a[_sel]:_b[_sel-16]; }\n"
                    << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                emitted = true;
              }
              if (!emitted) switch (vx128_full) {
                case 0x010: // vaddfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = ctx->v["
                      << va128 << "].f32[_i] + ctx->v[" << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x050: // vsubfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = ctx->v["
                      << va128 << "].f32[_i] - ctx->v[" << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x090: // vmulfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = ctx->v["
                      << va128 << "].f32[_i] * ctx->v[" << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x0D0: // vmaddfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = ctx->v["
                      << va128 << "].f32[_i] * ctx->v[" << vb128 << "].f32[_i] + ctx->v["
                      << vd128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x110: // vmaddcfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = ctx->v["
                      << va128 << "].f32[_i] * ctx->v[" << vb128 << "].f32[_i] + ctx->v["
                      << vd128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x150: // vnmsubfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = -(ctx->v["
                      << va128 << "].f32[_i] * ctx->v[" << vb128 << "].f32[_i] - ctx->v["
                      << vd128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x190: // vmsum3fp128
                  out << "    { float _s=0; for(int _i=0;_i<3;_i++) _s+=ctx->v[" << va128 << "].f32[_i]*ctx->v["
                      << vb128 << "].f32[_i]; for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i]=_s; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x1D0: // vmsum4fp128
                  out << "    { float _s=0; for(int _i=0;_i<4;_i++) _s+=ctx->v[" << va128 << "].f32[_i]*ctx->v["
                      << vb128 << "].f32[_i]; for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i]=_s; }" << addr_comment << "\n";
                  emitted = true; break;
                // vrefp128 (0x1C0) and vrsqrtefp128 (0x340) are opcode 6, handled in op6 switch
                // vmaxfp128 removed — it's opcode 6, not opcode 5 (handled in op6 switch)
                case 0x210: // vand128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = ctx->v["
                      << va128 << "].u32[_i] & ctx->v[" << vb128 << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x250: // vandc128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = ctx->v["
                      << va128 << "].u32[_i] & ~ctx->v[" << vb128 << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x290: // vnor128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = ~(ctx->v["
                      << va128 << "].u32[_i] | ctx->v[" << vb128 << "].u32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x2D0: // vor128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = ctx->v["
                      << va128 << "].u32[_i] | ctx->v[" << vb128 << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                case 0x310: // vxor128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = ctx->v["
                      << va128 << "].u32[_i] ^ ctx->v[" << vb128 << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                // vminfp128 removed — it's opcode 6, not opcode 5 (handled in op6 switch)
                // vperm128 (0x000) now handled above via VX128_2 mask (0x210)
                case 0x350: { // vsel128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = (ctx->v["
                      << va128 << "].u32[_i] & ~ctx->v[" << vd128 << "].u32[_i]) | (ctx->v["
                      << vb128 << "].u32[_i] & ctx->v[" << vd128 << "].u32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x390: { // vslo128
                  out << "    { int _sh = (ctx->v[" << vb128 << "].u8[15] >> 3) & 0xF;\n"
                      << "      RT360Vec128 _r = {{0}}; uint8_t* _s=(uint8_t*)&ctx->v[" << va128 << "];\n"
                      << "      for(int _i=0;_i<16-_sh;_i++) _r.u8[_i]=_s[_i+_sh];\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x3D0: { // vsro128
                  out << "    { int _sh = (ctx->v[" << vb128 << "].u8[15] >> 3) & 0xF;\n"
                      << "      RT360Vec128 _r = {{0}}; uint8_t* _s=(uint8_t*)&ctx->v[" << va128 << "];\n"
                      << "      for(int _i=0;_i<16-_sh;_i++) _r.u8[_i+_sh]=_s[_i];\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                // Pack operations
                case 0x200: { // vpkshss128 — pack signed halfword signed saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v["
                      << va128 << "].u16[_i]; _r.u8[_i]=(_v>127)?127:(_v<-128)?(uint8_t)(int8_t)-128:(uint8_t)(int8_t)_v; }\n"
                      << "      for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v[" << vb128 << "].u16[_i]; _r.u8[_i+8]=(_v>127)?127:(_v<-128)?(uint8_t)(int8_t)-128:(uint8_t)(int8_t)_v; }\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x240: { // vpkshus128 — pack signed halfword unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v["
                      << va128 << "].u16[_i]; _r.u8[_i]=(_v>255)?255:(_v<0)?0:(uint8_t)_v; }\n"
                      << "      for(int _i=0;_i<8;_i++) { int16_t _v=ctx->v[" << vb128 << "].u16[_i]; _r.u8[_i+8]=(_v>255)?255:(_v<0)?0:(uint8_t)_v; }\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x280: { // vpkswss128 — pack signed word signed saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v["
                      << va128 << "].i32[_i]; _r.u16[_i]=(uint16_t)(_v>32767)?32767:(_v<-32768)?-32768:(int16_t)_v; }\n"
                      << "      for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v[" << vb128 << "].i32[_i]; _r.u16[_i+4]=(uint16_t)(_v>32767)?32767:(_v<-32768)?-32768:(int16_t)_v; }\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x2C0: { // vpkswus128 — pack signed word unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v["
                      << va128 << "].i32[_i]; _r.u16[_i]=(_v>65535)?65535:(_v<0)?0:(uint16_t)_v; }\n"
                      << "      for(int _i=0;_i<4;_i++) { int32_t _v=ctx->v[" << vb128 << "].i32[_i]; _r.u16[_i+4]=(_v>65535)?65535:(_v<0)?0:(uint16_t)_v; }\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x300: { // vpkuhum128 — pack unsigned halfword unsigned modulo
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) _r.u8[_i]=(uint8_t)ctx->v["
                      << va128 << "].u16[_i];\n"
                      << "      for(int _i=0;_i<8;_i++) _r.u8[_i+8]=(uint8_t)ctx->v[" << vb128 << "].u16[_i];\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x340: { // vpkuhus128 — pack unsigned halfword unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<8;_i++) { uint16_t _v=ctx->v["
                      << va128 << "].u16[_i]; _r.u8[_i]=(_v>255)?255:(uint8_t)_v; }\n"
                      << "      for(int _i=0;_i<8;_i++) { uint16_t _v=ctx->v[" << vb128 << "].u16[_i]; _r.u8[_i+8]=(_v>255)?255:(uint8_t)_v; }\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x380: { // vpkuwum128 — pack unsigned word unsigned modulo
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) _r.u16[_i]=(uint16_t)ctx->v["
                      << va128 << "].u32[_i];\n"
                      << "      for(int _i=0;_i<4;_i++) _r.u16[_i+4]=(uint16_t)ctx->v[" << vb128 << "].u32[_i];\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x3C0: { // vpkuwus128 — pack unsigned word unsigned saturate
                  out << "    { RT360Vec128 _r; for(int _i=0;_i<4;_i++) { uint32_t _v=ctx->v["
                      << va128 << "].u32[_i]; _r.u16[_i]=(_v>65535)?65535:(uint16_t)_v; }\n"
                      << "      for(int _i=0;_i<4;_i++) { uint32_t _v=ctx->v[" << vb128 << "].u32[_i]; _r.u16[_i+4]=(_v>65535)?65535:(uint16_t)_v; }\n"
                      << "      ctx->v[" << vd128 << "]=_r; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                // vxor128 at 0x310 handled above
                default: break;
              }
            }

            if (!emitted && op == 6) {
              // VX128_3 form uses pw & 0x7F0 for xop.
              // Bit 5 (0x020) is the Rc bit — try both raw and Rc-toggled forms.
              uint32_t vx128_3_raw = pw & 0x7F0;
              uint32_t vx128_4 = pw & 0x730;
              // Try raw first, then toggle Rc bit
              // Try multiple bit toggles: Rc (0x020) and VD128h (0x400)
              for (int _rc_try = 0; _rc_try < 4 && !emitted; _rc_try++) {
              uint32_t vx128_3 = vx128_3_raw;
              if (_rc_try & 1) vx128_3 ^= 0x020; // toggle Rc bit
              if (_rc_try & 2) vx128_3 ^= 0x400; // toggle VD128h bit
              // VX128_4 form (vx128_4 declared above)

              // Try VX128_3 first (more specific)
              switch (vx128_3) {
                case 0x330: { // vrfim128 — round to minus infinity
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = floorf(ctx->v["
                      << vb128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x370: { // vrfin128 — round to nearest
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = roundf(ctx->v["
                      << vb128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x3B0: { // vrfip128 — round to plus infinity
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = ceilf(ctx->v["
                      << vb128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x3F0: { // vrfiz128 — round to zero (truncate)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = truncf(ctx->v["
                      << vb128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x730: { // vspltw128 — splat word
                  uint32_t uimm = (pw >> 16) & 0x3; // UIMM is 2 bits for word index
                  out << "    { uint32_t _v = ctx->v[" << vb128 << "].u32[" << uimm << "];\n"
                      << "      ctx->v[" << vd128 << "].u32[0]=_v; ctx->v[" << vd128 << "].u32[1]=_v;\n"
                      << "      ctx->v[" << vd128 << "].u32[2]=_v; ctx->v[" << vd128 << "].u32[3]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x770: { // vspltisw128 — splat immediate signed word
                  int simm = (int32_t)((pw >> 16) & 0x1F);
                  if (simm & 0x10) simm |= ~0x1F; // sign-extend 5-bit
                  out << "    { int32_t _v = " << simm << ";\n"
                      << "      ctx->v[" << vd128 << "].i32[0]=_v; ctx->v[" << vd128 << "].i32[1]=_v;\n"
                      << "      ctx->v[" << vd128 << "].i32[2]=_v; ctx->v[" << vd128 << "].i32[3]=_v; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x300: { // vmrghw128 — merge high word (0x7F0=0x300)
                  out << "    { ctx->v[" << vd128 << "].u32[0] = ctx->v[" << va128 << "].u32[0];\n"
                      << "      ctx->v[" << vd128 << "].u32[1] = ctx->v[" << vb128 << "].u32[0];\n"
                      << "      ctx->v[" << vd128 << "].u32[2] = ctx->v[" << va128 << "].u32[1];\n"
                      << "      ctx->v[" << vd128 << "].u32[3] = ctx->v[" << vb128 << "].u32[1]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x340: { // vmrglw128 — merge low word (0x7F0=0x340)
                  out << "    { ctx->v[" << vd128 << "].u32[0] = ctx->v[" << va128 << "].u32[2];\n"
                      << "      ctx->v[" << vd128 << "].u32[1] = ctx->v[" << vb128 << "].u32[2];\n"
                      << "      ctx->v[" << vd128 << "].u32[2] = ctx->v[" << va128 << "].u32[3];\n"
                      << "      ctx->v[" << vd128 << "].u32[3] = ctx->v[" << vb128 << "].u32[3]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x2B0: { // vcsxwfp128 — convert signed int to float
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = (float)(int32_t)ctx->v["
                      << vb128 << "].i32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x2F0: { // vcuxwfp128 — convert unsigned int to float
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = (float)ctx->v["
                      << vb128 << "].u32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x230: { // vcfpsxws128 — convert float to signed int
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].i32[_i] = (int32_t)ctx->v["
                      << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x270: { // vcfpuxws128 — convert float to unsigned int
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = (uint32_t)ctx->v["
                      << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x0D0: { // vslw128 — shift left word (0x7F0=0x0D0)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = ctx->v["
                      << va128 << "].u32[_i] << (ctx->v[" << vb128 << "].u32[_i] & 0x1F); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x1D0: { // vsrw128 — shift right word (0x7F0=0x1D0)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = ctx->v["
                      << va128 << "].u32[_i] >> (ctx->v[" << vb128 << "].u32[_i] & 0x1F); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x150: { // vsraw128 — shift right arithmetic word (0x7F0=0x150)
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].i32[_i] = ctx->v["
                      << va128 << "].i32[_i] >> (ctx->v[" << vb128 << "].u32[_i] & 0x1F); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x280: { // vmaxfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = (ctx->v["
                      << va128 << "].f32[_i] >= ctx->v[" << vb128 << "].f32[_i]) ? ctx->v["
                      << va128 << "].f32[_i] : ctx->v[" << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x2C0: { // vminfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = (ctx->v["
                      << va128 << "].f32[_i] <= ctx->v[" << vb128 << "].f32[_i]) ? ctx->v["
                      << va128 << "].f32[_i] : ctx->v[" << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x000: { // vcmpeqfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = (ctx->v["
                      << va128 << "].f32[_i] == ctx->v[" << vb128 << "].f32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x080: { // vcmpgefp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = (ctx->v["
                      << va128 << "].f32[_i] >= ctx->v[" << vb128 << "].f32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x100: { // vcmpgtfp128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = (ctx->v["
                      << va128 << "].f32[_i] > ctx->v[" << vb128 << "].f32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x180: { // vcmpbfp128
                  out << "    { for(int _i=0;_i<4;_i++) { uint32_t _gt=(ctx->v[" << va128 << "].f32[_i]>ctx->v["
                      << vb128 << "].f32[_i])?0x80000000:0; uint32_t _lt=(ctx->v[" << va128 << "].f32[_i]<-ctx->v["
                      << vb128 << "].f32[_i])?0x40000000:0; ctx->v[" << vd128 << "].u32[_i]=_gt|_lt; } }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x200: { // vcmpequw128
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = (ctx->v["
                      << va128 << "].u32[_i] == ctx->v[" << vb128 << "].u32[_i]) ? 0xFFFFFFFF : 0; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x380: { // vupkhsb128 — unpack high signed byte
                  out << "    { RT360Vec128 _t = ctx->v[" << vb128 << "];\n"
                      << "      for(int _i=0;_i<8;_i++) ctx->v[" << vd128 << "].u16[_i] = (int16_t)(int8_t)_t.u8[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x3C0: { // vupklsb128 — unpack low signed byte
                  out << "    { RT360Vec128 _t = ctx->v[" << vb128 << "];\n"
                      << "      for(int _i=0;_i<8;_i++) ctx->v[" << vd128 << "].u16[_i] = (int16_t)(int8_t)_t.u8[_i+8]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x630: { // vrefp128 — reciprocal estimate
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = 1.0f / ctx->v["
                      << vb128 << "].f32[_i]; }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x670: { // vrsqrtefp128 — reciprocal sqrt estimate
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = 1.0f / sqrtf(ctx->v["
                      << vb128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x6B0: { // vexptefp128 — 2^x estimate
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = powf(2.0f, ctx->v["
                      << vb128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x6F0: { // vlogefp128 — log2 estimate
                  out << "    { for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].f32[_i] = log2f(ctx->v["
                      << vb128 << "].f32[_i]); }" << addr_comment << "\n";
                  emitted = true; break;
                }
                case 0x7F0: { // vupkd3d128 — unpack D3D
                  // TODO: complex D3D format unpacking
                  break;
                }
                case 0x050: { // vrlw128 — rotate left word
                  out << "    { for(int _i=0;_i<4;_i++) { uint32_t _n=ctx->v[" << vb128 << "].u32[_i]&0x1F; uint32_t _v=ctx->v["
                      << va128 << "].u32[_i]; ctx->v[" << vd128 << "].u32[_i]=(_v<<_n)|(_v>>(32-_n)); } }" << addr_comment << "\n";
                  emitted = true; break;
                }
                default: break;
              }
              } // end Rc retry loop

              // Try VX128_4 form (vrlimi128)
              if (!emitted) {
                switch (vx128_4) {
                  case 0x710: { // vrlimi128 — rotate left immediate and mask insert
                    uint32_t rot = (pw >> 6) & 0x3; // 2-bit rotation
                    uint32_t mask_imm = (pw >> 2) & 0xF; // 4-bit mask
                    // Rotate words by rot positions, mask insert
                    out << "    { RT360Vec128 _t = ctx->v[" << vb128 << "];\n";
                    if (rot > 0) {
                      out << "      RT360Vec128 _r; for(int _i=0;_i<4;_i++) _r.u32[_i]=_t.u32[(_i+" << rot << ")&3]; _t=_r;\n";
                    }
                    out << "      uint32_t _m[4] = {" << ((mask_imm&8)?"0xFFFFFFFF":"0") << ","
                        << ((mask_imm&4)?"0xFFFFFFFF":"0") << "," << ((mask_imm&2)?"0xFFFFFFFF":"0") << ","
                        << ((mask_imm&1)?"0xFFFFFFFF":"0") << "};\n";
                    out << "      for(int _i=0;_i<4;_i++) ctx->v[" << vd128 << "].u32[_i] = (_t.u32[_i]&_m[_i]) | (ctx->v["
                        << vd128 << "].u32[_i]&~_m[_i]); }" << addr_comment << "\n";
                    emitted = true; break;
                  }
                  default: break;
                }
              }

              // vx128_3 and vx128_4 switches handle all op6 instructions.
              // op6 logic ops (vand128 etc.) are actually op5 — handled above.
            }
          }

          // Opcode 31: standard VMX load/store (lvsl, lvsr, lvewx, etc.)
          if (!emitted && op == 31) {
            uint32_t xo10 = (pw >> 1) & 0x3FF;
            uint32_t ra31 = (pw >> 16) & 0x1F;
            uint32_t rb31 = (pw >> 11) & 0x1F;
            std::string ea31 = (ra31 == 0) ? "(uint32_t)ctx->r[" + std::to_string(rb31) + "]"
                : "((uint32_t)ctx->r[" + std::to_string(ra31) + "] + (uint32_t)ctx->r[" + std::to_string(rb31) + "])";
            switch (xo10) {
              case 6: { // lvsl — load vector for shift left
                out << "    { uint32_t _sh = " << ea31 << " & 0xF;\n"
                    << "      for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = (uint8_t)(_sh + _i); }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 38: { // lvsr — load vector for shift right
                out << "    { uint32_t _sh = " << ea31 << " & 0xF;\n"
                    << "      for(int _i=0;_i<16;_i++) ctx->v[" << vd << "].u8[_i] = (uint8_t)(16 - _sh + _i); }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 7: { // lvebx — load vector element byte indexed
                out << "    { uint32_t _ea = " << ea31 << ";\n"
                    << "      ctx->v[" << vd << "].u8[_ea & 0xF] = *(uint8_t*)(ctx->virtual_membase + _ea); }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 39: { // lvehx — load vector element halfword indexed
                out << "    { uint32_t _ea = " << ea31 << " & ~1u;\n"
                    << "      uint32_t _idx = (_ea >> 1) & 0x7;\n"
                    << "      uint16_t _v; memcpy(&_v, (uint8_t*)(ctx->virtual_membase + _ea), 2);\n"
                    << "      ctx->v[" << vd << "].u16[_idx] = __builtin_bswap16(_v); }" << addr_comment << "\n";
                emitted = true; break;
              }
              case 71: { // lvewx — load vector element word indexed
                out << "    { uint32_t _ea = " << ea31 << " & ~3u;\n"
                    << "      uint32_t _idx = (_ea >> 2) & 0x3;\n"
                    << "      uint32_t _v; memcpy(&_v, (uint8_t*)(ctx->virtual_membase + _ea), 4);\n"
                    << "      ctx->v[" << vd << "].u32[_idx] = __builtin_bswap32(_v); }" << addr_comment << "\n";
                emitted = true; break;
              }
              default: break;
            }
          }

          if (!emitted) {
            out << "    /* vmx: " << il.asm_text << " */" << addr_comment << "\n";
          }
          break;
        }

        case ILOp::VMXLoad: {
          // lvx vD, rA, rB — load 16 bytes from aligned address, byte-swap
          int vr = il.imm;
          std::string addr;
          if (il.src1.id == kILNoValue)
            addr = s2;  // rA=0
          else
            addr = "(uint32_t)(" + s1 + " + " + s2 + ")";
          out << "    { uint32_t _ea = " << addr << " & ~0xFu;\n";
          out << "      uint8_t* _p = (uint8_t*)(ctx->virtual_membase + _ea);\n";
          out << "      uint32_t* _d = (uint32_t*)&ctx->v[" << vr << "];\n";
          out << "      _d[0] = RT360_BSWAP32(*(uint32_t*)(_p + 0));\n";
          out << "      _d[1] = RT360_BSWAP32(*(uint32_t*)(_p + 4));\n";
          out << "      _d[2] = RT360_BSWAP32(*(uint32_t*)(_p + 8));\n";
          out << "      _d[3] = RT360_BSWAP32(*(uint32_t*)(_p + 12)); }" << addr_comment << "\n";
          break;
        }

        case ILOp::VMXStore: {
          // stvx vS, rA, rB — store 16 bytes to aligned address, byte-swap
          int vr = il.imm;
          std::string addr;
          if (il.src1.id == kILNoValue)
            addr = s2;
          else
            addr = "(uint32_t)(" + s1 + " + " + s2 + ")";
          out << "    { uint32_t _ea = " << addr << " & ~0xFu;\n";
          out << "      uint8_t* _p = (uint8_t*)(ctx->virtual_membase + _ea);\n";
          out << "      uint32_t* _s = (uint32_t*)&ctx->v[" << vr << "];\n";
          out << "      *(uint32_t*)(_p + 0) = RT360_BSWAP32(_s[0]);\n";
          out << "      *(uint32_t*)(_p + 4) = RT360_BSWAP32(_s[1]);\n";
          out << "      *(uint32_t*)(_p + 8) = RT360_BSWAP32(_s[2]);\n";
          out << "      *(uint32_t*)(_p + 12) = RT360_BSWAP32(_s[3]); }" << addr_comment << "\n";
          break;
        }

        case ILOp::InlineAsm:
          out << "    /* asm: " << il.asm_text << " */" << addr_comment << "\n"; break;

        default:
          out << "    /* unhandled IL: " << ILOpName(il.op) << " */" << addr_comment << "\n";
          break;
      }

      // Post-instruction flag emission: CA, OV, Rc (CR0)
      // Note: s1/s2 may have been modified by the instruction if dst aliases src.
      // The JIT uses SSA so this never happens there. We use _ca_s1/_ca_s2 temps
      // which were saved BEFORE the instruction executed.
      if (il.set_ca && !dst.empty() && il.dst.ppc_reg >= 0 && il.dst.ppc_reg < 32) {
        // Use ca_s1/ca_s2 (saved before the operation) to avoid aliasing.
        // Emit inline save + CA computation.
        std::string cs1 = "(uint32_t)" + ca_s1;
        std::string cs2 = "(uint32_t)" + ca_s2;
        if (il.op == ILOp::SarImm) {
          out << "    ctx->xer_ca = ((int32_t)" << cs1 << " < 0 && " << il.imm << " > 0 && (" << cs1
              << " & ((1u << " << il.imm << ") - 1)) != 0) ? 1 : 0;\n";
        } else if (il.op == ILOp::Sar) {
          out << "    { uint32_t _sh = " << cs2 << " & 63;\n";
          out << "      ctx->xer_ca = ((int32_t)" << cs1 << " < 0 && _sh > 0 && (" << cs1
              << " & ((1u << (_sh > 31 ? 31 : _sh)) - 1)) != 0) ? 1 : 0; }\n";
        } else if (il.op == ILOp::Sub) {
          out << "    ctx->xer_ca = (" << cs1 << " >= " << cs2 << ") ? 1 : 0;\n";
        } else if (il.op == ILOp::SubCarry) {
          out << "    ctx->xer_ca = ((uint64_t)~" << cs2 << " + (uint64_t)" << cs1
              << " + (uint64_t)ctx->xer_ca) > 0xFFFFFFFFULL ? 1 : 0;\n";
        } else if (il.op == ILOp::AddCarry) {
          out << "    ctx->xer_ca = ((uint64_t)" << cs1 << " + (uint64_t)" << cs2
              << " + (uint64_t)ctx->xer_ca) > 0xFFFFFFFFULL ? 1 : 0;\n";
        } else {
          out << "    ctx->xer_ca = ((uint64_t)" << cs1 << " + (uint64_t)" << cs2
              << ") > 0xFFFFFFFFULL ? 1 : 0;\n";
        }
      }
      if (il.rc && !dst.empty() && il.dst.ppc_reg >= 0 && il.dst.ppc_reg < 32) {
        // Rc bit: update CR0 based on result.
        // JIT compares full 64-bit value against 0 (signed).
        out << "    { int64_t _rcv = (int64_t)" << dst << ";\n";
        out << "      RT360CRField* _cr = rt360_cr_field(ctx, 0);\n";
        out << "      _cr->lt = (_rcv < 0) ? 1 : 0;\n";
        out << "      _cr->gt = (_rcv > 0) ? 1 : 0;\n";
        out << "      _cr->eq = (_rcv == 0) ? 1 : 0;\n";
        out << "      _cr->so = ctx->xer_so; }\n";
      }

      // --trace: dump all registers after every instruction
      if (config.trace_mode && il.op != ILOp::Nop) {
        // Get PPC mnemonic from disassembler
        std::string disasm = il.ppc_word
            ? DisasmInstruction(il.address, il.ppc_word)
            : ILOpName(il.op);
        // Extract just the mnemonic (first word)
        auto sp = disasm.find(' ');
        if (sp != std::string::npos) disasm = disasm.substr(0, sp);
        // Escape for C string
        out << "    rt360_trace_instr(ctx, 0x" << std::hex << il.address
            << std::dec << "u, \"" << disasm << "\");\n";
      }
    }

    // Fall-through between blocks is natural — no explicit jump needed
  }

  out << "}\n";
  return out.str();
}

// ---------------------------------------------------------------------------
// Emit a byte array as a C array initializer
// ---------------------------------------------------------------------------

static void EmitByteArray(std::ostringstream& out, const char* name,
                          const std::vector<uint8_t>& data) {
  out << "static const uint8_t " << name << "[" << data.size() << "] = {\n";
  for (size_t i = 0; i < data.size(); i++) {
    if (i % 16 == 0) out << "    ";
    char buf[8];
    snprintf(buf, sizeof(buf), "0x%02X", data[i]);
    out << buf;
    if (i + 1 < data.size()) out << ",";
    if (i % 16 == 15 || i + 1 == data.size()) out << "\n";
  }
  out << "};\n\n";
}

// ---------------------------------------------------------------------------
// Generate full RT360 source file with guest init data
// ---------------------------------------------------------------------------

std::string GenerateRT360Source(const std::vector<ILFunction>& funcs,
                                const CodeGenConfig& config,
                                const RT360GuestInitData* init_data,
                                const char* source_file) {
  std::ostringstream out;

  std::string module_name = config.rt360_module_name;
  if (module_name.empty()) module_name = "default";

  // Header
  out << "/* RT360 Static Recompilation Output - XEXMagic */\n";
  if (source_file) out << "/* Source: " << source_file << " */\n";
  out << "/* " << funcs.size() << " functions */\n";
  if (init_data) {
    out << "/* Guest init: " << init_data->data_sections.size() << " data sections, "
        << init_data->import_thunks.size() << " import thunks, "
        << init_data->header_fields.size() << " header fields */\n";
  }
  out << "\n";

  // Includes
  out << "#include \"rt360_runtime.h\"\n";
  out << "#include <math.h>\n";
  out << "#include <string.h>\n";
  out << "#include <stdio.h>\n";
  out << "#include <stddef.h>\n\n";
  // =========================================================================
  // Dispatch infrastructure
  // =========================================================================
  uint32_t table_base = init_data ? init_data->base_address : 0x82000000;
  uint32_t table_size = init_data ? init_data->image_size : 0x2330000;
  uint32_t table_entries = table_size >> 2;

  // --- Indirection table: O(1) dispatch for every 4-byte address in XEX range ---
  out << "#define RT360_TABLE_BASE 0x" << std::hex << table_base << "\n";
  out << "#define RT360_TABLE_SIZE 0x" << table_size << "\n";
  out << "#define RT360_TABLE_ENTRIES " << std::dec << table_entries << "\n\n";

  out << "typedef void (*RT360DispatchFn)(RT360Context*, uint32_t);\n";
  out << "typedef void (*RT360NexiaDispatchFn)(RT360Context*, uint32_t);\n";
  out << "typedef struct { RT360DispatchFn func; uint32_t addr; } RT360TableEntry;\n";
  out << "static RT360TableEntry _rt360_table[RT360_TABLE_ENTRIES];\n\n";

  // --- Kernel dispatch tables: direct handler calls by ordinal ---
  out << "#define XBOXKRNL_MAX_ORDINAL 1024\n";
  out << "#define XAM_MAX_ORDINAL 4096\n";
  out << "static RT360KernelHandler kernel_table_xboxkrnl[XBOXKRNL_MAX_ORDINAL];\n";
  out << "static RT360KernelHandler kernel_table_xam[XAM_MAX_ORDINAL];\n\n";

  // --- Nexia fallback dispatch ---
  out << "static RT360NexiaDispatchFn g_nexia_dispatch = 0;\n";
  out << "static FILE* rt360_log = 0;\n";
  out << "static FILE* rt360_get_log() { if (!rt360_log) rt360_log = fopen(\"rt360_trace.log\", \"w\"); return rt360_log; }\n\n";

  // --- Instruction trace: dump all registers after every instruction ---
  if (config.trace_mode) {
    out << "static FILE* rt360_trace_file = 0;\n";
    out << "static FILE* rt360_trace_open() {\n";
    out << "    if (!rt360_trace_file) rt360_trace_file = fopen(\"rt360_instr_trace.log\", \"w\");\n";
    out << "    return rt360_trace_file;\n";
    out << "}\n";
    out << "static void rt360_trace_instr(RT360Context* ctx, uint32_t pc, const char* mnem) {\n";
    out << "    FILE* f = rt360_trace_open(); if (!f) return;\n";
    out << "    fprintf(f, \"PC: 0x%08X  %-10s  ctx_addr: %p\\n\", pc, mnem, (void*)ctx);\n";
    out << "    fprintf(f, \"  r0 =%08X  r1 =%08X  r2 =%08X  r3 =%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[0],(uint32_t)ctx->r[1],(uint32_t)ctx->r[2],(uint32_t)ctx->r[3]);\n";
    out << "    fprintf(f, \"  r4 =%08X  r5 =%08X  r6 =%08X  r7 =%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[4],(uint32_t)ctx->r[5],(uint32_t)ctx->r[6],(uint32_t)ctx->r[7]);\n";
    out << "    fprintf(f, \"  r8 =%08X  r9 =%08X  r10=%08X  r11=%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[8],(uint32_t)ctx->r[9],(uint32_t)ctx->r[10],(uint32_t)ctx->r[11]);\n";
    out << "    fprintf(f, \"  r12=%08X  r13=%08X  r14=%08X  r15=%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[12],(uint32_t)ctx->r[13],(uint32_t)ctx->r[14],(uint32_t)ctx->r[15]);\n";
    out << "    fprintf(f, \"  r16=%08X  r17=%08X  r18=%08X  r19=%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[16],(uint32_t)ctx->r[17],(uint32_t)ctx->r[18],(uint32_t)ctx->r[19]);\n";
    out << "    fprintf(f, \"  r20=%08X  r21=%08X  r22=%08X  r23=%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[20],(uint32_t)ctx->r[21],(uint32_t)ctx->r[22],(uint32_t)ctx->r[23]);\n";
    out << "    fprintf(f, \"  r24=%08X  r25=%08X  r26=%08X  r27=%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[24],(uint32_t)ctx->r[25],(uint32_t)ctx->r[26],(uint32_t)ctx->r[27]);\n";
    out << "    fprintf(f, \"  r28=%08X  r29=%08X  r30=%08X  r31=%08X\\n\",\n";
    out << "        (uint32_t)ctx->r[28],(uint32_t)ctx->r[29],(uint32_t)ctx->r[30],(uint32_t)ctx->r[31]);\n";
    out << "    { uint32_t _crv = 0;\n";
    out << "      for(int _i=0;_i<8;_i++) { RT360CRField* _f=rt360_cr_field(ctx,_i);\n";
    out << "        _crv |= ((_f->lt<<3)|(_f->gt<<2)|(_f->eq<<1)|_f->so) << (28-_i*4); }\n";
    out << "    fprintf(f, \"  lr =%08X  ctr=%08X  cr =%08X  xer_ca=%d\\n\",\n";
    out << "        (uint32_t)ctx->lr,(uint32_t)ctx->ctr, _crv, ctx->xer_ca); }\n";
    out << "}\n\n";
  }

  // --- Call tracer: ring buffer of last 128 context snapshots ---
  out << "typedef struct { uint32_t addr; uint32_t r1_before; uint32_t r1_after; uint32_t r3; uint32_t r13; uint32_t lr; } RT360Trace;\n";
  out << "static RT360Trace _trace_buf[128];\n";
  out << "static int _trace_idx = 0;\n";
  out << "extern \"C\" RT360_EXPORT RT360Trace* rt360_get_trace(int* count) { *count = 128; return _trace_buf; }\n";
  out << "extern \"C\" RT360_EXPORT int rt360_get_trace_idx() { return _trace_idx; }\n\n";

  // --- rt360_call: O(1) table lookup for all addresses ---
  out << "static uint32_t _watch_prev = 0;\n";
  out << "void rt360_call(RT360Context* ctx, uint32_t addr) {\n";
  out << "    /* Watchpoint: detect when 0x8426CE54 becomes non-zero */\n";
  out << "    { uint32_t _wv = RT360_BSWAP32(*(uint32_t*)((uint8_t*)ctx->virtual_membase + 0x8426CE54));\n";
  out << "      if (_wv != _watch_prev) {\n";
  out << "        fprintf(rt360_get_log(), \"WATCH>> 0x8426CE54 changed: 0x%08X -> 0x%08X (caller=0x%08X lr=0x%08X)\\n\",\n";
  out << "               _watch_prev, _wv, addr, (uint32_t)ctx->lr);\n";
  out << "        fflush(rt360_get_log()); _watch_prev = _wv;\n";
  out << "      }\n";
  out << "    }\n";
  out << "    fprintf(rt360_get_log(),\">> 0x%08X r1=%08X r3=%08X r4=%08X r13=%08X lr=%08X\\n\", addr,\n";
  out << "           (uint32_t)ctx->r[1], (uint32_t)ctx->r[3], (uint32_t)ctx->r[4], (uint32_t)ctx->r[13], (uint32_t)ctx->lr);\n";
  out << "    fflush(rt360_get_log());\n";
  out << "    if (addr >= RT360_TABLE_BASE && addr < RT360_TABLE_BASE + RT360_TABLE_SIZE) {\n";
  out << "        uint32_t idx = (addr - RT360_TABLE_BASE) >> 2;\n";
  out << "        RT360TableEntry* e = &_rt360_table[idx];\n";
  out << "        if (e->func) {\n";
  out << "            /* Context restore fixup: if the target context block has SP=0,\n";
  out << "               it was never initialized. Write CURRENT register state so the\n";
  out << "               context switch preserves the caller's frame correctly. */\n";
  out << "            if (addr == 0x824C1590) {\n";
  out << "                uint32_t ctx_block = (uint32_t)ctx->r[3];\n";
  out << "                uint8_t* vmem = (uint8_t*)ctx->virtual_membase;\n";
  out << "                if (ctx_block != 0) {\n";
  out << "                    uint64_t saved_sp = RT360_BSWAP64(*(uint64_t*)(vmem + ctx_block + 0x90));\n";
  out << "                    if (saved_sp == 0) {\n";
  out << "                        /* Block uninitialized — populate with CURRENT regs */\n";
  out << "                        *(uint64_t*)(vmem + ctx_block + 0x90) = RT360_BSWAP64(ctx->r[1]);   /* r1/SP */\n";
  out << "                        *(uint64_t*)(vmem + ctx_block + 0x98) = RT360_BSWAP64(ctx->r[13]);  /* r13/KPCR */\n";
  out << "                        *(uint32_t*)(vmem + ctx_block + 0x130) = RT360_BSWAP32(0x9030);     /* MSR */\n";
  out << "                        *(uint32_t*)(vmem + ctx_block + 0x134) = RT360_BSWAP32((uint32_t)ctx->lr); /* PC = return addr */\n";
  out << "                        *(uint32_t*)(vmem + ctx_block + 0x138) = RT360_BSWAP32(1);          /* CR != 0 so sub_824C187C path is taken */\n";
  out << "                        fprintf(rt360_get_log(), \"CTXFIX>> blk=%08X SP=%08X r13=%08X LR=%08X\\n\",\n";
  out << "                               ctx_block, (uint32_t)ctx->r[1], (uint32_t)ctx->r[13], (uint32_t)ctx->lr);\n";
  out << "                        fflush(rt360_get_log());\n";
  out << "                    }\n";
  out << "                }\n";
  out << "            }\n";
  out << "            if (addr == 0x82462C20) {\n";
  out << "                uint8_t* _vm = (uint8_t*)ctx->virtual_membase;\n";
  out << "                fprintf(rt360_get_log(),\"CRT_INIT>> pre-init=0x%08X\\n\",\n";
  out << "                       RT360_BSWAP32(*(uint32_t*)(_vm + 0x82056590)));\n";
  out << "                fprintf(rt360_get_log(),\"CRT_INIT>> loop1[0..2]:\");\n";
  out << "                for(int _i=0;_i<3;_i++) fprintf(rt360_get_log(),\" %08X\",\n";
  out << "                    RT360_BSWAP32(*(uint32_t*)(_vm+0x825002C8+_i*4)));\n";
  out << "                fprintf(rt360_get_log(),\"\\nCRT_INIT>> loop2[0..19]:\");\n";
  out << "                for(int _i=0;_i<20;_i++) fprintf(rt360_get_log(),\" %08X\",\n";
  out << "                    RT360_BSWAP32(*(uint32_t*)(_vm+0x82500010+_i*4)));\n";
  out << "                fprintf(rt360_get_log(),\"\\n\"); fflush(rt360_get_log());\n";
  out << "            }\n";
  out << "            if (addr == 0x822A3570) {\n";
  out << "                uint8_t* _vm2 = (uint8_t*)ctx->virtual_membase;\n";
  out << "                uint32_t _kpcr2 = (uint32_t)ctx->r[13];\n";
  out << "                uint32_t _tls2 = (_kpcr2 != 0) ? RT360_BSWAP32(*(uint32_t*)(_vm2 + _kpcr2)) : 0;\n";
  out << "                uint32_t _tbase2 = (_tls2 != 0) ? RT360_BSWAP32(*(uint32_t*)(_vm2 + _tls2 + 12)) : 0;\n";
  out << "                uint32_t _idx2 = (uint32_t)ctx->r[3];\n";
  out << "                uint32_t _entry2 = (_tbase2 != 0 && _idx2 < 8) ? RT360_BSWAP32(*(uint32_t*)(_vm2 + _tbase2 + _idx2*4)) : 0;\n";
  out << "                fprintf(rt360_get_log(),\"LOOKUP>> r3=%d kpcr=%08X tls=%08X tbase=%08X entry=%08X\\n\",\n";
  out << "                       _idx2, _kpcr2, _tls2, _tbase2, _entry2);\n";
  out << "                fflush(rt360_get_log());\n";
  out << "            }\n";
  out << "            if (addr == 0x82464F98) {\n";
  out << "                fprintf(rt360_get_log(),\"INIT>> r3=%08X r4=%08X r5=%08X r6=%08X r7=%08X r8=%08X\\n\",\n";
  out << "                       (uint32_t)ctx->r[3], (uint32_t)ctx->r[4], (uint32_t)ctx->r[5],\n";
  out << "                       (uint32_t)ctx->r[6], (uint32_t)ctx->r[7], (uint32_t)ctx->r[8]);\n";
  out << "                fflush(rt360_get_log());\n";
  out << "            }\n";
  out << "            e->func(ctx, e->addr);\n";
  out << "            if (addr == 0x82464F98) {\n";
  out << "                fprintf(rt360_get_log(),\"INIT<< r3=%08X r5=%08X r29=%08X r30=%08X\\n\",\n";
  out << "                       (uint32_t)ctx->r[3], (uint32_t)ctx->r[5],\n";
  out << "                       (uint32_t)ctx->r[29], (uint32_t)ctx->r[30]);\n";
  out << "                fflush(rt360_get_log());\n";
  out << "            }\n";
  out << "            fprintf(rt360_get_log(), \"<< 0x%08X r1=%08X r3=%08X\\n\", addr,\n";
  out << "                   (uint32_t)ctx->r[1], (uint32_t)ctx->r[3]);\n";
  out << "            fflush(rt360_get_log());\n";
  out << "            return;\n";
  out << "        }\n";
  out << "    }\n";
  out << "    fprintf(rt360_get_log(),\">> 0x%08X NEXIA\\n\", addr); fflush(rt360_get_log());\n";
  out << "    if (g_nexia_dispatch) g_nexia_dispatch(ctx, addr);\n";
  out << "    fprintf(rt360_get_log(),\"<< 0x%08X NEXIA r3=%08X\\n\", addr, (uint32_t)ctx->r[3]); fflush(rt360_get_log());\n";
  out << "}\n\n";

  // Forward declarations
  out << "RT360_EXPORT extern RT360FuncEntry rt360_functions[];\n\n";

  // =========================================================================
  // Guest init data (baked from XEX at decompile time)
  // =========================================================================

  if (init_data) {
    out << "/* ================================================================== */\n";
    out << "/* Guest Memory Init Data (baked from XEX - no XEX needed at runtime) */\n";
    out << "/* ================================================================== */\n\n";

    // Data section byte arrays
    for (size_t i = 0; i < init_data->data_sections.size(); i++) {
      auto& sec = init_data->data_sections[i];
      if (sec.data.empty()) continue;
      char name[64];
      snprintf(name, sizeof(name), "rt360_section_%zu_data", i);
      out << "/* Section: " << sec.name << " VA=0x" << std::hex
          << sec.virtual_address << " Size=0x" << sec.virtual_size
          << std::dec << " */\n";
      EmitByteArray(out, name, sec.data);
    }

    // Embedded raw XEX file with file-like I/O
    if (!init_data->xex_image.empty()) {
      out << "/* Embedded raw XEX file (" << init_data->xex_image.size()
          << " bytes) */\n";
      EmitByteArray(out, "rt360_xex_data", init_data->xex_image);
      out << "static const uint32_t rt360_xex_data_size = "
          << init_data->xex_image.size() << ";\n";
      out << "static uint32_t rt360_xex_offset = 0;\n\n";

      out << "extern \"C\" RT360_EXPORT void rt360_xopen(void) {\n";
      out << "    rt360_xex_offset = 0;\n";
      out << "}\n\n";

      out << "extern \"C\" RT360_EXPORT void rt360_xread(uint8_t* buffer, uint32_t len) {\n";
      out << "    for (uint32_t i = 0; i < len; i++)\n";
      out << "        buffer[i] = rt360_xex_data[rt360_xex_offset + i];\n";
      out << "    rt360_xex_offset += len;\n";
      out << "}\n\n";

      out << "extern \"C\" RT360_EXPORT void rt360_xseek(uint32_t offset) {\n";
      out << "    rt360_xex_offset = offset;\n";
      out << "}\n\n";

      out << "extern \"C\" RT360_EXPORT uint32_t rt360_xsize(void) {\n";
      out << "    return rt360_xex_data_size;\n";
      out << "}\n\n";
    }

    // TLS template data
    if (!init_data->tls_info.template_data.empty()) {
      out << "/* TLS template data (slot_count=" << init_data->tls_info.slot_count
          << ", data_size=" << init_data->tls_info.data_size << ") */\n";
      EmitByteArray(out, "rt360_tls_template", init_data->tls_info.template_data);
    }

    // Import thunk table
    if (!init_data->import_thunks.empty()) {
      out << "/* Import thunk table */\n";
      out << "typedef struct { uint32_t thunk_addr; uint32_t value_addr; const char* module; uint32_t ordinal; } RT360ImportThunk;\n";
      out << "static const RT360ImportThunk rt360_import_thunks[] = {\n";
      for (auto& imp : init_data->import_thunks) {
        out << "    { 0x" << std::hex << imp.thunk_address
            << ", 0x" << imp.value_address << std::dec
            << ", \"" << imp.module_name << "\", " << imp.ordinal << " },\n";
      }
      out << "    { 0, 0, 0, 0 }\n";
      out << "};\n\n";
    }

    // XEX header fields table (for RtlImageXexHeaderField)
    if (!init_data->header_fields.empty()) {
      out << "/* XEX header fields (for RtlImageXexHeaderField queries) */\n";
      out << "typedef struct { uint32_t key; uint32_t value; } RT360HeaderField;\n";
      out << "static const RT360HeaderField rt360_header_fields[] = {\n";
      for (auto& hf : init_data->header_fields) {
        out << "    { 0x" << std::hex << hf.key << ", 0x" << hf.value
            << std::dec << " },\n";
      }
      out << "    { 0, 0 }\n";
      out << "};\n\n";
    }

    // Export table (for XexGetProcedureAddress)
    if (!init_data->exports.empty()) {
      out << "/* Export table (ordinal -> guest address) */\n";
      out << "typedef struct { uint32_t ordinal; uint32_t address; } RT360Export;\n";
      out << "static const RT360Export rt360_exports[] = {\n";
      for (auto& [ord, addr] : init_data->exports) {
        out << "    { " << ord << ", 0x" << std::hex << addr << std::dec << " },\n";
      }
      out << "    { 0, 0 }\n";
      out << "};\n\n";
    }

    // Section descriptor table
    out << "/* PE section descriptors */\n";
    out << "typedef struct { const char* name; uint32_t va; uint32_t vsize; uint32_t chars; } RT360SectionDesc;\n";
    out << "static const RT360SectionDesc rt360_sections[] = {\n";
    for (auto& sec : init_data->data_sections) {
      out << "    { \"" << sec.name << "\", 0x" << std::hex << sec.virtual_address
          << ", 0x" << sec.virtual_size << ", 0x" << sec.characteristics
          << std::dec << " },\n";
    }
    out << "    { 0, 0, 0, 0 }\n";
    out << "};\n\n";

    // TLS info struct
    out << "/* TLS info */\n";
    out << "typedef struct { uint32_t slot_count; uint32_t raw_data_addr; uint32_t data_size; uint32_t raw_data_size; } RT360TLSInfo;\n";
    out << "static const RT360TLSInfo rt360_tls_info = { "
        << init_data->tls_info.slot_count << ", 0x" << std::hex
        << init_data->tls_info.raw_data_address << std::dec << ", "
        << init_data->tls_info.data_size << ", "
        << init_data->tls_info.raw_data_size << " };\n\n";

    // ----- rt360_init_guest_memory() -----
    out << "/* Initialize guest memory with baked data from the XEX.\n";
    out << " * Call once before rt360_entry(). Writes data sections,\n";
    out << " * TLS template, and sets up structures the CRT expects. */\n";
    out << "extern \"C\" RT360_EXPORT void rt360_init_guest_memory(RT360Context* ctx) {\n";
    out << "    FILE* _glog = fopen(\"rt360_guest_memory_init.log\", \"w\");\n";
    out << "    if (_glog) fprintf(_glog, \"rt360_init_guest_memory: vmem=%p\\n\", (void*)ctx->virtual_membase);\n";

    // Write data sections
    for (size_t i = 0; i < init_data->data_sections.size(); i++) {
      auto& sec = init_data->data_sections[i];
      if (sec.data.empty()) continue;
      uint32_t dest_addr = init_data->base_address + sec.virtual_address;
      out << "    /* " << sec.name << " -> 0x" << std::hex << dest_addr << std::dec
          << " (" << sec.data.size() << " bytes) */\n";
      out << "    if (_glog) fprintf(_glog, \"  section " << sec.name
          << ": dest=0x" << std::hex << dest_addr << std::dec
          << " size=" << sec.data.size()
          << " ptr=%p\\n\", rt360_memptr(ctx, 0x" << std::hex << dest_addr << std::dec << "));\n";
      out << "    if (_glog) fflush(_glog);\n";
      out << "    memcpy(rt360_memptr(ctx, 0x" << std::hex << dest_addr << std::dec
          << "), rt360_section_" << i << "_data, sizeof(rt360_section_" << i << "_data));\n";
      out << "    if (_glog) { fprintf(_glog, \"    done\\n\"); fflush(_glog); }\n";
    }

    // Write TLS template to its original VA
    if (!init_data->tls_info.template_data.empty() && init_data->tls_info.raw_data_address) {
      out << "\n    /* TLS template data */\n";
      out << "    if (_glog) fprintf(_glog, \"  TLS: dest=0x" << std::hex
          << init_data->tls_info.raw_data_address << std::dec
          << " size=" << init_data->tls_info.template_data.size() << "\\n\");\n";
      out << "    if (_glog) fflush(_glog);\n";
      out << "    memcpy(rt360_memptr(ctx, 0x" << std::hex
          << init_data->tls_info.raw_data_address << std::dec
          << "), rt360_tls_template, sizeof(rt360_tls_template));\n";
      out << "    if (_glog) { fprintf(_glog, \"    done\\n\"); fflush(_glog); }\n";
    }

    out << "    if (_glog) { fprintf(_glog, \"rt360_init_guest_memory complete\\n\"); fclose(_glog); }\n";
    out << "}\n\n";

    // ----- rt360_lookup_import() -----
    out << "/* Look up an import thunk address. Returns ordinal or 0 if not found. */\n";
    out << "extern \"C\" RT360_EXPORT uint32_t rt360_lookup_import(uint32_t thunk_addr, const char** out_module) {\n";
    out << "    for (int i = 0; rt360_import_thunks[i].thunk_addr; i++) {\n";
    out << "        if (rt360_import_thunks[i].thunk_addr == thunk_addr) {\n";
    out << "            if (out_module) *out_module = rt360_import_thunks[i].module;\n";
    out << "            return rt360_import_thunks[i].ordinal;\n";
    out << "        }\n";
    out << "    }\n";
    out << "    return 0;\n";
    out << "}\n\n";

    // ----- rt360_lookup_header_field() -----
    out << "/* Look up a baked XEX header field value. */\n";
    out << "extern \"C\" RT360_EXPORT int rt360_lookup_header_field(uint32_t key, uint32_t* out_value) {\n";
    out << "    for (int i = 0; rt360_header_fields[i].key; i++) {\n";
    out << "        if (rt360_header_fields[i].key == key) {\n";
    out << "            *out_value = rt360_header_fields[i].value;\n";
    out << "            return 1;\n";
    out << "        }\n";
    out << "    }\n";
    out << "    return 0;\n";
    out << "}\n\n";
  }

  // =========================================================================
  // Recompiled functions
  // =========================================================================

  // Namespace
  if (!config.namespace_name.empty()) {
    out << "namespace " << config.namespace_name << " {\n\n";
  }

  // Function dispatch type: void func(ctx, offset)
  out << "typedef void (*RT360DispatchFn)(RT360Context*, uint32_t);\n\n";

  // Forward declarations for import thunk inline functions
  if (init_data) {
    out << "/* Import thunk forward declarations */\n";
    for (auto& imp : init_data->import_thunks) {
      out << "static void _thunk_" << std::hex << imp.thunk_address << std::dec
          << "(RT360Context* ctx, uint32_t _addr);\n";
    }
    out << "\n";
  }

  // Forward declarations
  out << "/* Forward declarations */\n";
  for (auto& f : funcs) {
    out << "void " << f.name << "(RT360Context* ctx, uint32_t _addr);\n";
  }
  out << "\n";

  // Emit functions — each has a switch/goto jump table + all blocks inline
  for (auto& f : funcs) {
    out << GenerateRT360FunctionSource(f, config) << "\n";
  }

  // Namespace close
  if (!config.namespace_name.empty()) {
    out << "} // namespace " << config.namespace_name << "\n\n";
  }

  // =========================================================================
  // Export tables (always C linkage)
  // =========================================================================

  out << "/* RT360 function table */\n";
  if (!config.namespace_name.empty()) {
    out << "extern \"C\" {\n";
  }

  // Import thunk stubs — call kernel handlers directly via dispatch table.
  // handler(ctx, ctx->kernel_state) — verified two-arg ExternHandler signature.
  if (init_data && !init_data->import_thunks.empty()) {
    out << "/* Import thunk stubs — kernel dispatch table calls */\n";
    for (auto& imp : init_data->import_thunks) {
      if (imp.thunk_address == 0) continue;
      out << "static void _thunk_" << std::hex << imp.thunk_address << std::dec
          << "(RT360Context* ctx, uint32_t _addr) {\n";

      // Select the right kernel table by module name, with bounds check
      if (imp.module_name == "xboxkrnl.exe" && imp.ordinal < 1024) {
        out << "    RT360KernelHandler h = kernel_table_xboxkrnl[0x"
            << std::hex << imp.ordinal << std::dec << "];\n";
      } else if (imp.module_name == "xam.xex" && imp.ordinal < 4096) {
        out << "    RT360KernelHandler h = kernel_table_xam[0x"
            << std::hex << imp.ordinal << std::dec << "];\n";
      } else {
        out << "    RT360KernelHandler h = 0;  /* " << imp.module_name
            << " ord 0x" << std::hex << imp.ordinal << std::dec << " */\n";
      }
      out << "    if (h) h(ctx, ctx->kernel_state);\n";
      out << "    else if (g_nexia_dispatch) g_nexia_dispatch(ctx, _addr);\n";
      out << "}\n";
    }
    out << "\n";
  }

  // Function table — dispatch functions + import thunk stubs
  // The dispatch function takes (ctx, offset) — cast to RT360FuncPtr for table.
  size_t thunk_count = (init_data ? init_data->import_thunks.size() : 0);
  size_t total_entries = funcs.size() + thunk_count;
  out << "RT360_EXPORT RT360FuncEntry rt360_functions[] = {\n";
  // Regular functions with entry and end addresses
  for (auto& f : funcs) {
    std::string ns_prefix = config.namespace_name.empty() ? "" : config.namespace_name + "::";
    out << "    { 0x" << std::hex << f.entry << ", 0x" << f.end << std::dec << ", "
        << "(RT360FuncPtr)" << ns_prefix << f.name << ", \"" << f.name << "\" },\n";
  }
  out << "    { 0, 0, 0, 0 }\n";
  out << "};\n\n";

  // Module info
  uint32_t base_addr = init_data ? init_data->base_address
      : (funcs.empty() ? 0 : funcs.front().entry);
  uint32_t mod_size = init_data ? init_data->image_size
      : (funcs.empty() ? 0 : funcs.back().end - funcs.front().entry);
  uint32_t entry = init_data ? init_data->entry_point
      : (funcs.empty() ? 0 : funcs.front().entry);

  out << "RT360_EXPORT RT360ModuleInfo rt360_module_info = {\n";
  out << "    \"" << module_name << "\",\n";
  out << "    0x" << std::hex << base_addr << ",  /* base_addr */\n";
  out << "    0x" << mod_size << ",  /* size */\n";
  out << "    0x" << entry << std::dec << ",  /* entry_point */\n";
  out << "    " << total_entries << ",  /* func_count */\n";
  out << "    rt360_functions\n";
  out << "};\n";

  // RT360Dispatch_entry — table lookup dispatch. O(1).
  out << "\nextern \"C\" RT360_EXPORT int RT360Dispatch_entry(RT360Context* ctx, uint32_t address) {\n";
  out << "    if (address >= RT360_TABLE_BASE && address < RT360_TABLE_BASE + RT360_TABLE_SIZE) {\n";
  out << "        uint32_t idx = (address - RT360_TABLE_BASE) >> 2;\n";
  out << "        RT360TableEntry* e = &_rt360_table[idx];\n";
  out << "        if (e->func) { e->func(ctx, e->addr); return 0; }\n";
  out << "    }\n";
  out << "    /* Not in table — route to Nexia */\n";
  out << "    if (g_nexia_dispatch) { g_nexia_dispatch(ctx, address); return 0; }\n";
  out << "    return 1;\n";
  out << "}\n";

  // rt360_init — single init call, receives everything from Nexia
  out << "\nextern \"C\" RT360_EXPORT void rt360_init(\n";
  out << "    RT360NexiaDispatchFn nexia_dispatch,\n";
  out << "    RT360KernelExport* kernel_exports,\n";
  out << "    uint32_t kernel_export_count,\n";
  out << "    void* virtual_membase,\n";
  out << "    void* physical_membase\n";
  out << ") {\n";
  out << "    g_nexia_dispatch = nexia_dispatch;\n";
  out << "    { FILE* _ofs = fopen(\"rt360_offsets.log\", \"w\");\n";
  out << "      if (_ofs) {\n";
  out << "        fprintf(_ofs, \"r=%zu ctr=%zu lr=%zu f=%zu v=%zu xer_ca=%zu vmem=%zu sizeof=%zu\\n\",\n";
  out << "            offsetof(RT360Context, r), offsetof(RT360Context, ctr), offsetof(RT360Context, lr),\n";
  out << "            offsetof(RT360Context, f), offsetof(RT360Context, v), offsetof(RT360Context, xer_ca),\n";
  out << "            offsetof(RT360Context, virtual_membase), sizeof(RT360Context));\n";
  out << "        fclose(_ofs);\n";
  out << "      }\n";
  out << "    }\n\n";

  // 1. Populate kernel dispatch tables from exports
  out << "    /* Populate kernel dispatch tables */\n";
  out << "    for (uint32_t i = 0; i < kernel_export_count; i++) {\n";
  out << "        RT360KernelExport* e = &kernel_exports[i];\n";
  out << "        if ((strcmp(e->module_name, \"xboxkrnl.exe\") == 0 ||\n";
  out << "             strcmp(e->module_name, \"xboxkrnl\") == 0) && e->ordinal < XBOXKRNL_MAX_ORDINAL)\n";
  out << "            kernel_table_xboxkrnl[e->ordinal] = e->handler;\n";
  out << "        else if ((strcmp(e->module_name, \"xam.xex\") == 0 ||\n";
  out << "                  strcmp(e->module_name, \"xam\") == 0) && e->ordinal < XAM_MAX_ORDINAL)\n";
  out << "            kernel_table_xam[e->ordinal] = e->handler;\n";
  out << "    }\n\n";

  // 2. Populate indirection table from function entries
  out << "    /* Populate indirection table from function entries */\n";
  out << "    for (int i = 0; rt360_functions[i].func; i++) {\n";
  out << "        uint32_t addr = rt360_functions[i].guest_addr;\n";
  out << "        if (addr >= RT360_TABLE_BASE && addr < RT360_TABLE_BASE + RT360_TABLE_SIZE) {\n";
  out << "            uint32_t idx = (addr - RT360_TABLE_BASE) >> 2;\n";
  out << "            _rt360_table[idx].func = (RT360DispatchFn)rt360_functions[i].func;\n";
  out << "            _rt360_table[idx].addr = addr;\n";
  out << "        }\n";
  out << "    }\n";
  out << "    /* Fill ALL gaps between functions — every address dispatches to something */\n";
  out << "    for (int i = 0; rt360_functions[i].func; i++) {\n";
  out << "        uint32_t fbase = rt360_functions[i].guest_addr;\n";
  out << "        uint32_t fnext = rt360_functions[i+1].func ? rt360_functions[i+1].guest_addr\n";
  out << "                         : (RT360_TABLE_BASE + RT360_TABLE_SIZE);\n";
  out << "        RT360DispatchFn fn = (RT360DispatchFn)rt360_functions[i].func;\n";
  out << "        for (uint32_t a = fbase; a < fnext && a < RT360_TABLE_BASE + RT360_TABLE_SIZE; a += 4) {\n";
  out << "            uint32_t idx = (a - RT360_TABLE_BASE) >> 2;\n";
  out << "            if (!_rt360_table[idx].func) {\n";
  out << "                _rt360_table[idx].func = fn;\n";
  out << "                _rt360_table[idx].addr = a;\n";
  out << "            }\n";
  out << "        }\n";
  out << "    }\n\n";

  out << "    /* Clear import thunk slots by scanning guest memory for sc2 (0x44000002) */\n";
  out << "    /* FinishLoadingUserModule rewrites thunks to sc2;blr before rt360_init */\n";
  out << "    {\n";
  out << "        uint8_t* vmem = (uint8_t*)virtual_membase;\n";
  out << "        int cleared = 0;\n";
  out << "        for (uint32_t a = RT360_TABLE_BASE; a < RT360_TABLE_BASE + RT360_TABLE_SIZE; a += 4) {\n";
  out << "            uint32_t idx = (a - RT360_TABLE_BASE) >> 2;\n";
  out << "            if (!_rt360_table[idx].func) continue;\n";
  out << "            uint32_t instr = RT360_BSWAP32(*(uint32_t*)(vmem + a));\n";
  out << "            if (instr == 0x44000042) { /* sc 2 (LEV=2) */\n";
  out << "                /* Clear this and surrounding slots (sc2; blr; nop; nop) */\n";
  out << "                for (int j = -4; j <= 12; j += 4) {\n";
  out << "                    uint32_t ca = a + j;\n";
  out << "                    if (ca >= RT360_TABLE_BASE && ca < RT360_TABLE_BASE + RT360_TABLE_SIZE) {\n";
  out << "                        uint32_t ci = (ca - RT360_TABLE_BASE) >> 2;\n";
  out << "                        _rt360_table[ci].func = 0;\n";
  out << "                        _rt360_table[ci].addr = 0;\n";
  out << "                    }\n";
  out << "                }\n";
  out << "                cleared++;\n";
  out << "            }\n";
  out << "        }\n";
  out << "        fprintf(rt360_get_log(), \"RT360: Cleared %d import thunk slots (sc2 scan)\\n\", cleared);\n";
  out << "        fflush(rt360_get_log());\n";
  out << "    }\n\n";

  // 3. Init guest memory (write baked data sections)
  out << "    /* Init guest memory with baked XEX data sections */\n";
  out << "    /* Guest memory init called separately by Nexia BEFORE\n";
  out << "       FinishLoadingUserModule so thunk rewrites happen on top. */\n";
  out << "    (void)virtual_membase;\n";
  out << "    (void)physical_membase;\n";
  out << "}\n";

  // Entry point wrapper
  if (init_data && init_data->entry_point) {
    std::string entry_name = "sub_" ;
    char ebuf[16];
    snprintf(ebuf, sizeof(ebuf), "%08X", init_data->entry_point);
    entry_name += ebuf;
    // Find the actual function name for the entry point
    for (auto& f : funcs) {
      if (f.entry == init_data->entry_point) {
        entry_name = f.name;
        break;
      }
    }
    out << "\nRT360_EXPORT void rt360_entry(RT360Context* ctx) {\n";
    if (!config.namespace_name.empty())
      out << "    " << config.namespace_name << "::" << entry_name << "(ctx, 0x"
          << std::hex << init_data->entry_point << std::dec << ");\n";
    else
      out << "    " << entry_name << "(ctx, 0x"
          << std::hex << init_data->entry_point << std::dec << ");\n";
    out << "}\n";
  }

  // rt360_call is defined at the top of the file (table lookup).
  // (rt360_dispatch_ptr removed — kernel calls go through kernel_table directly)

  if (!config.namespace_name.empty()) {
    out << "} // extern \"C\"\n";
  }

  return out.str();
}

// ---------------------------------------------------------------------------
// Build RT360GuestInitData from a loaded XEX
// ---------------------------------------------------------------------------

RT360GuestInitData BuildRT360GuestInitData(
    const XexLoader& loader,
    const std::vector<PESectionInfo>& sections,
    const uint8_t* image, size_t image_size) {

  RT360GuestInitData init;
  init.base_address = loader.GetBaseAddress();
  init.entry_point = loader.GetEntryPoint();
  init.title_id = loader.GetTitleId();

  init.image_size = (uint32_t)image_size;

  // xex_image is set by caller with the raw XEX file bytes.
  // Nexia's own XEX loader decompresses and maps sections.

  // Default stack size
  void* stack_val = nullptr;
  if (const_cast<XexLoader&>(loader).GetOptHeader(XEX_HEADER_DEFAULT_STACK_SIZE, &stack_val)) {
    init.default_stack_size = (uint32_t)(uintptr_t)stack_val;
  }

  // Module name
  init.module_name = "default";

  // Extract data sections (non-code sections with data)
  for (auto& sec : sections) {
    bool is_code = (sec.characteristics & 0x00000020) ||
                   (sec.characteristics & 0x20000000);
    // Include data sections AND code sections (for completeness)
    // Code sections carry import thunks and other structures

    RT360DataSection ds;
    ds.name = sec.name;
    ds.virtual_address = sec.virtual_address;
    ds.virtual_size = sec.virtual_size;
    ds.characteristics = sec.characteristics;

    // Read section data from raw decompressed PE using file offsets
    uint32_t copy_size = sec.raw_size;
    if (copy_size > sec.virtual_size) copy_size = sec.virtual_size;
    if (sec.raw_offset + copy_size > image_size) {
      copy_size = (sec.raw_offset < image_size)
                      ? (uint32_t)(image_size - sec.raw_offset)
                      : 0;
    }

    if (copy_size > 0) {
      ds.data.assign(image + sec.raw_offset,
                     image + sec.raw_offset + copy_size);
    }

    init.data_sections.push_back(std::move(ds));
  }

  // Import thunks
  for (auto& lib : loader.import_libraries()) {
    for (auto& imp : lib.imports) {
      RT360ImportThunk thunk;
      thunk.thunk_address = imp.thunk_address;
      thunk.value_address = imp.value_address;
      thunk.module_name = lib.name;
      thunk.ordinal = imp.ordinal;
      init.import_thunks.push_back(std::move(thunk));
    }
  }

  // TLS info
  xex2_opt_tls_info* tls = nullptr;
  if (const_cast<XexLoader&>(loader).GetOptHeader(XEX_HEADER_TLS_INFO, &tls) && tls) {
    init.tls_info.slot_count = tls->slot_count;
    init.tls_info.raw_data_address = tls->raw_data_address;
    init.tls_info.data_size = tls->data_size;
    init.tls_info.raw_data_size = tls->raw_data_size;

    // Extract TLS template data from image
    uint32_t tls_offset = (uint32_t)tls->raw_data_address - init.base_address;
    uint32_t tls_size = tls->raw_data_size;
    if (tls_offset + tls_size <= image_size && tls_size > 0) {
      init.tls_info.template_data.assign(image + tls_offset,
                                          image + tls_offset + tls_size);
    }
  }

  // XEX header fields — bake the common ones
  auto addField = [&](uint32_t key) {
    void* val = nullptr;
    if (const_cast<XexLoader&>(loader).GetOptHeader((xex2_header_keys)key, &val)) {
      RT360XexHeaderField hf;
      hf.key = key;
      hf.value = (uint32_t)(uintptr_t)val;
      init.header_fields.push_back(hf);
    }
  };
  addField(XEX_HEADER_ENTRY_POINT);
  addField(XEX_HEADER_IMAGE_BASE_ADDRESS);
  addField(XEX_HEADER_DEFAULT_STACK_SIZE);
  addField(XEX_HEADER_DEFAULT_HEAP_SIZE);
  addField(XEX_HEADER_SYSTEM_FLAGS);

  // Exports
  auto* sec_info = loader.xex_security_info();
  if (sec_info && sec_info->export_table) {
    uint32_t export_va = sec_info->export_table;
    uint32_t export_offset = export_va - init.base_address;
    if (export_offset + sizeof(xex2_export_table) <= image_size) {
      auto* table = reinterpret_cast<const xex2_export_table*>(image + export_offset);
      uint32_t count = table->count;
      uint32_t base_ordinal = table->base;
      uint32_t imagebaseaddr = table->imagebaseaddr;
      for (uint32_t i = 0; i < count; i++) {
        uint32_t raw;
        memcpy(&raw, &table->ordOffset[i], sizeof(uint32_t));
        uint32_t val = ((raw >> 24) & 0xFF) | ((raw >> 8) & 0xFF00) |
                       ((raw << 8) & 0xFF0000) | ((raw << 24) & 0xFF000000);
        init.exports.push_back({base_ordinal + i, val + imagebaseaddr});
      }
    }
  }

  return init;
}

// ---------------------------------------------------------------------------
// Generate .rt360map JSON source map
// ---------------------------------------------------------------------------

std::string GenerateRT360SourceMap(const std::vector<ILFunction>& funcs,
                                   const CodeGenConfig& config,
                                   const char* source_file) {
  std::ostringstream out;

  std::string module_name = config.rt360_module_name;
  if (module_name.empty()) module_name = "default";

  out << "{\n";
  out << "  \"module\": \"" << module_name << "\",\n";
  if (source_file) out << "  \"source\": \"" << source_file << "\",\n";
  out << "  \"functions\": [\n";

  for (size_t i = 0; i < funcs.size(); i++) {
    auto& f = funcs[i];
    out << "    {\"name\": \"" << f.name << "\", "
        << "\"ppc_start\": \"0x" << std::hex << f.entry << "\", "
        << "\"ppc_end\": \"0x" << f.end << std::dec << "\", "
        << "\"cycles\": " << f.total_cycles << ", "
        << "\"instructions\": " << f.total_instructions << "}";
    if (i + 1 < funcs.size()) out << ",";
    out << "\n";
  }

  out << "  ]\n";
  out << "}\n";

  return out.str();
}
