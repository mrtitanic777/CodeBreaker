// Capstone -> X86Insn adapter. See capstone_decode.h.
#define CAPSTONE_STATIC
#include "win95/capstone_decode.h"
#include "capstone.h"

#include <cstdlib>
#include <cstring>
#include <string>

namespace win95 {
namespace {

// One reusable handle per mode (detail on). Opened lazily; never closed (process
// lifetime). Capstone handles are not thread-shared, but the engine decodes single-
// threaded per module.
csh g_h16 = 0, g_h32 = 0;

csh handle(bool op16) {
  csh& h = op16 ? g_h16 : g_h32;
  if (!h) {
    if (cs_open(CS_ARCH_X86, op16 ? CS_MODE_16 : CS_MODE_32, &h) != CS_ERR_OK) return 0;
    cs_option(h, CS_OPT_DETAIL, CS_OPT_ON);
  }
  return h;
}

// Capstone x86 register -> our reg id (class*16 + idx), or kNoReg.
int mapReg(unsigned r) {
  switch (r) {
    case X86_REG_AL: case X86_REG_AH: case X86_REG_AX: case X86_REG_EAX: return RegId(RC_GPR, 0);
    case X86_REG_CL: case X86_REG_CH: case X86_REG_CX: case X86_REG_ECX: return RegId(RC_GPR, 1);
    case X86_REG_DL: case X86_REG_DH: case X86_REG_DX: case X86_REG_EDX: return RegId(RC_GPR, 2);
    case X86_REG_BL: case X86_REG_BH: case X86_REG_BX: case X86_REG_EBX: return RegId(RC_GPR, 3);
    case X86_REG_SP: case X86_REG_ESP: return RegId(RC_GPR, 4);
    case X86_REG_BP: case X86_REG_EBP: return RegId(RC_GPR, 5);
    case X86_REG_SI: case X86_REG_ESI: return RegId(RC_GPR, 6);
    case X86_REG_DI: case X86_REG_EDI: return RegId(RC_GPR, 7);
    case X86_REG_ES: return RegId(RC_SEG, 0);
    case X86_REG_CS: return RegId(RC_SEG, 1);
    case X86_REG_SS: return RegId(RC_SEG, 2);
    case X86_REG_DS: return RegId(RC_SEG, 3);
    case X86_REG_FS: return RegId(RC_SEG, 4);
    case X86_REG_GS: return RegId(RC_SEG, 5);
    case X86_REG_ST0: return RegId(RC_ST, 0);
    case X86_REG_ST1: return RegId(RC_ST, 1);
    case X86_REG_ST2: return RegId(RC_ST, 2);
    case X86_REG_ST3: return RegId(RC_ST, 3);
    case X86_REG_ST4: return RegId(RC_ST, 4);
    case X86_REG_ST5: return RegId(RC_ST, 5);
    case X86_REG_ST6: return RegId(RC_ST, 6);
    case X86_REG_ST7: return RegId(RC_ST, 7);
    default: return kNoReg;
  }
}

bool isBranch(const std::string& m) {
  return m == "jmp" || m == "call" || m == "loop" || m == "loope" || m == "loopne" ||
         (m.size() && m[0] == 'j');
}

}  // namespace

X86Insn DecodeX86Cap(const uint8_t* data, size_t size, size_t pos, uint32_t addr,
                     bool default_op16) {
  X86Insn out;
  out.addr = addr;
  out.len = 1;
  csh h = handle(default_op16);
  if (!h || pos >= size) return out;

  const uint8_t* p = data + pos;
  size_t avail = size - pos;
  uint64_t a = addr;
  // Reuse one cs_insn per (mode) for the whole run -- cs_disasm_iter is built for
  // this. Allocating/freeing a cs_insn per instruction was the decode hot-path
  // cost. Never freed (process lifetime), like the handles.
  static thread_local cs_insn* insn16 = nullptr;
  static thread_local cs_insn* insn32 = nullptr;
  cs_insn*& slot = default_op16 ? insn16 : insn32;
  if (!slot) slot = cs_malloc(h);
  cs_insn* insn = slot;
  if (!insn) return out;
  if (!cs_disasm_iter(h, &p, &avail, &a, insn)) return out;

  // Win95's task scheduler predates the XMM register file and CR4.OSFXSR, so it never saves
  // or restores SSE state -- SSE (and everything built on XMM: SSE2/3/4x, AVX, FMA, AES,
  // PCLMUL) cannot be real code in this corpus and, run under Win95, would #UD or corrupt
  // state. When Capstone decodes such an instruction here it has mis-decoded DATA as code, so
  // reject it (out.valid stays false -> the caller advances a byte and treats the span as
  // data). NOTE the era line is deliberately at SSE, not earlier: MMX aliases the x87 register
  // stack (no OS support needed) and the P6 integer ops (CMOV/FCOMI/RDPMC) are decodable and
  // runnable inside 16-bit segments, so those are legitimate and are NOT rejected here --
  // MMX/P6 mis-decodes belong to code/data separation, not the era gate.
  static const uint8_t kNoXmm[] = {
      X86_GRP_SSE1, X86_GRP_SSE2, X86_GRP_SSE3, X86_GRP_SSE41, X86_GRP_SSE42,
      X86_GRP_SSE4A, X86_GRP_AVX, X86_GRP_AVX2, X86_GRP_AVX512, X86_GRP_FMA,
      X86_GRP_FMA4, X86_GRP_AES, X86_GRP_PCLMUL};
  if (insn->detail) {
    for (uint8_t gi = 0; gi < insn->detail->groups_count; ++gi)
      for (uint8_t k : kNoXmm)
        if (insn->detail->groups[gi] == k) return out;   // XMM-based: data, not Win95 code
  }
  {
    // A few more that Capstone groups under MMX (they run on the MM registers) but that actually
    // require SSE support Win95 never enables (the SSE-era integer extensions -- pavgw/pshufw/...),
    // plus the ring-0 fast-syscall pair -- none can be real Win16/Win95 user code, so a decode of
    // them here is data. (Base MMX, which truly aliases x87 and needs no OS support, is NOT listed.)
    static const char* kNo[] = {
        "pavgb", "pavgw", "pshufw", "pmulhuw", "pminub", "pmaxub", "pminsw", "pmaxsw", "psadbw",
        "pmovmskb", "pextrw", "pinsrw", "maskmovq", "movntq", "pshufb", "sysenter", "sysexit"};
    const char* mm = insn->mnemonic;
    for (const char* s : kNo) if (!std::strcmp(mm, s)) return out;
  }

  std::string mn = insn->mnemonic;
  // strip rep/repne prefixes folded into the mnemonic, recording them as flags
  if (mn.rfind("rep ", 0) == 0) { out.rep = true; mn = mn.substr(4); }
  else if (mn.rfind("repe ", 0) == 0 || mn.rfind("repz ", 0) == 0) { out.rep = true; mn = mn.substr(5); }
  else if (mn.rfind("repne ", 0) == 0 || mn.rfind("repnz ", 0) == 0) { out.repne = true; mn = mn.substr(6); }

  out.mnem = mn;
  out.len = insn->size;
  out.valid = true;
  cs_x86& x = insn->detail->x86;
  bool branch = isBranch(mn);

  int n = 0;
  for (int i = 0; i < x.op_count && n < 3; ++i) {
    cs_x86_op& op = x.operands[i];
    X86Op& d = out.ops[n];
    d.size = op.size;
    if (op.type == X86_OP_REG) {
      d.kind = OpKind::Reg;
      d.reg = mapReg(op.reg);
      d.hi8 = op.reg == X86_REG_AH || op.reg == X86_REG_CH || op.reg == X86_REG_DH ||
              op.reg == X86_REG_BH;
    } else if (op.type == X86_OP_IMM) {
      if (branch) {
        d.kind = OpKind::Rel; d.imm = (int64_t)op.imm;
        // In 16-bit mode Capstone wraps a near branch target to the 16-bit segment offset, dropping
        // the segment base -- so a call lands on offset 0x10 while the callee is defined at the linear
        // address seg_base|0x10. A near branch stays in-segment, so re-attach the segment base (the
        // high 16 bits of this instruction's linear address). Keeps call targets consistent with the
        // function definitions/discovery -- otherwise the whole intra-segment call graph is unlinkable.
        if (default_op16 && (addr & 0xFFFF0000u) && (uint64_t)d.imm < 0x10000u)
          d.imm = (int64_t)((addr & 0xFFFF0000u) | ((uint32_t)d.imm & 0xFFFFu));
      } else      { d.kind = OpKind::Imm; d.imm = (int64_t)op.imm; }
    } else if (op.type == X86_OP_MEM) {
      d.kind = OpKind::Mem;
      d.base = mapReg(op.mem.base);
      d.index = mapReg(op.mem.index);
      d.scale = op.mem.scale;
      d.disp = (int32_t)op.mem.disp;
      d.seg = mapReg(op.mem.segment);
    } else {
      d.kind = OpKind::None;
    }
    ++n;
  }
  out.nops = n;

  // far call/jmp/ret: Capstone spells the far forms lcall/ljmp/retf/iret
  if (mn == "retf" || mn == "iret" || mn == "iretd" ||
      mn.rfind("lcall", 0) == 0 || mn.rfind("ljmp", 0) == 0)
    out.is_far = true;
  if (mn == "lcall") out.mnem = "call";     // normalize to the lifter's names
  if (mn == "ljmp")  out.mnem = "jmp";

  return out;                                 // insn is reused, not freed per call
}

X86Insn DecodeX86Auto(const uint8_t* data, size_t size, size_t pos, uint32_t addr,
                      bool default_op16) {
  // Capstone is the default decoder; WT_CAPSTONE=0 falls back to the native decoder.
  static int mode = -1;
  if (mode < 0) {
    const char* e = std::getenv("WT_CAPSTONE");
    mode = (e && e[0] == '0') ? 0 : 1;
  }
  return mode ? DecodeX86Cap(data, size, pos, addr, default_op16)
              : DecodeX86(data, size, pos, addr, default_op16);
}

}  // namespace win95
