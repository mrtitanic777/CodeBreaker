// Win95 port: 16/32-bit x86 decoder implementation.
#include "win95/x86_decode.h"

#include <cstdio>
#include <cstring>

namespace win95 {

namespace {

struct Cursor {
  const uint8_t* d;
  size_t size;
  size_t p;       // current read position
  bool bad = false;
  uint8_t u8() { if (p >= size) { bad = true; return 0; } return d[p++]; }
  uint16_t u16() { uint16_t a = u8(); uint16_t b = u8(); return a | (b << 8); }
  uint32_t u32() { uint32_t a = u16(); uint32_t b = u16(); return a | (b << 16); }
  int8_t s8() { return (int8_t)u8(); }
  int16_t s16() { return (int16_t)u16(); }
  int32_t s32() { return (int32_t)u32(); }
};

const char* kGpr8[8]  = {"al","cl","dl","bl","ah","ch","dh","bh"};
const char* kGpr16[8] = {"ax","cx","dx","bx","sp","bp","si","di"};
const char* kGpr32[8] = {"eax","ecx","edx","ebx","esp","ebp","esi","edi"};
const char* kSeg[6]   = {"es","cs","ss","ds","fs","gs"};

X86Op RegOp(int idx, int size) {
  X86Op o; o.kind = OpKind::Reg; o.reg = RegId(RC_GPR, idx); o.size = size; return o;
}
X86Op SegOp(int idx) {
  X86Op o; o.kind = OpKind::Reg; o.reg = RegId(RC_SEG, idx); o.size = 2; return o;
}
X86Op ImmOp(int64_t v, int size) {
  X86Op o; o.kind = OpKind::Imm; o.imm = v; o.size = size; return o;
}

// Decode a ModRM byte. Fills rm_op (reg or mem) and returns the reg field (0..7).
int ModRM(Cursor& c, bool addr32, int opsize, int seg_override, X86Op& rm_op) {
  uint8_t modrm = c.u8();
  int mod = modrm >> 6, reg = (modrm >> 3) & 7, rm = modrm & 7;
  if (mod == 3) {                       // register direct
    rm_op = RegOp(rm, opsize);
    return reg;
  }
  rm_op.kind = OpKind::Mem;
  rm_op.size = opsize;
  rm_op.seg = seg_override;
  rm_op.base = kNoReg; rm_op.index = kNoReg; rm_op.scale = 1; rm_op.disp = 0;
  if (!addr32) {                        // 16-bit addressing
    static const int base16[8] = {3, 3, 5, 5, 6, 7, 5, 3};   // bx,bx,bp,bp,si,di,bp,bx
    static const int idx16[8]  = {6, 7, 6, 7, -1, -1, -1, -1}; // si,di,si,di,-,-,-,-
    if (mod == 0 && rm == 6) {          // disp16 direct
      rm_op.disp = c.u16();
    } else {
      rm_op.base = RegId(RC_GPR, base16[rm]);
      if (idx16[rm] >= 0) rm_op.index = RegId(RC_GPR, idx16[rm]);
      if (mod == 1) rm_op.disp = c.s8();
      else if (mod == 2) rm_op.disp = c.s16();
    }
  } else {                              // 32-bit addressing
    if (rm == 4) {                      // SIB
      uint8_t sib = c.u8();
      int ss = sib >> 6, idx = (sib >> 3) & 7, bas = sib & 7;
      if (idx != 4) { rm_op.index = RegId(RC_GPR, idx); rm_op.scale = 1 << ss; }
      if (bas == 5 && mod == 0) rm_op.disp = c.s32();
      else rm_op.base = RegId(RC_GPR, bas);
    } else if (mod == 0 && rm == 5) {
      rm_op.disp = c.s32();
    } else {
      rm_op.base = RegId(RC_GPR, rm);
    }
    if (mod == 1) rm_op.disp = c.s8();
    else if (mod == 2) rm_op.disp = c.s32();
  }
  return reg;
}

const char* kArith[8] = {"add","or","adc","sbb","and","sub","xor","cmp"};
const char* kShift[8] = {"rol","ror","rcl","rcr","shl","shr","sal","sar"};
const char* kJcc[16]  = {"jo","jno","jb","jae","je","jne","jbe","ja",
                         "js","jns","jp","jnp","jl","jge","jle","jg"};

}  // namespace

X86Insn DecodeX86(const uint8_t* data, size_t size, size_t pos, uint32_t addr,
                  bool default_op16) {
  X86Insn in; in.addr = addr;
  Cursor c{data, size, pos};

  // --- prefixes ---
  bool op16 = default_op16;
  bool addr16 = default_op16;
  for (;;) {
    uint8_t b = (c.p < size) ? data[c.p] : 0;
    if (b == 0x66) { in.opsize = true; op16 = !op16; c.p++; }
    else if (b == 0x67) { in.addrsize = true; addr16 = !addr16; c.p++; }
    else if (b == 0xF0) { in.lock = true; c.p++; }
    else if (b == 0xF2) { in.repne = true; c.p++; }
    else if (b == 0xF3) { in.rep = true; c.p++; }
    else if (b == 0x2E) { in.seg_override = RegId(RC_SEG, 1); c.p++; }
    else if (b == 0x36) { in.seg_override = RegId(RC_SEG, 2); c.p++; }
    else if (b == 0x3E) { in.seg_override = RegId(RC_SEG, 3); c.p++; }
    else if (b == 0x26) { in.seg_override = RegId(RC_SEG, 0); c.p++; }
    else if (b == 0x64) { in.seg_override = RegId(RC_SEG, 4); c.p++; }
    else if (b == 0x65) { in.seg_override = RegId(RC_SEG, 5); c.p++; }
    else if (b == 0x9B) { c.p++; }      // fwait prefix-ish; ignore
    else break;
  }
  int osz = op16 ? 2 : 4;               // current operand size (non-byte)
  bool a32 = !addr16;

  uint8_t op = c.u8();
  auto one = [&](const char* m, X86Op a) { in.mnem = m; in.ops[0] = a; in.nops = 1; };
  auto two = [&](const char* m, X86Op a, X86Op b) { in.mnem = m; in.ops[0] = a; in.ops[1] = b; in.nops = 2; };
  auto rel8 = [&]() { int8_t r = c.s8(); X86Op o; o.kind = OpKind::Rel; o.imm = (int64_t)(c.p - pos + addr + r) - ( (int64_t)c.p - (int64_t)pos ) + 0; o.imm = addr + (c.p - pos) + r; o.size = 2; return o; };
  auto rel16 = [&]() { int16_t r = op16 ? c.s16() : (int16_t)c.s32(); X86Op o; o.kind = OpKind::Rel; o.imm = addr + (c.p - pos) + r; o.size = 2; return o; };

  // segment push/pop + BCD ops interleaved in 00-3F
  if (op == 0x06) one("push", SegOp(0));
  else if (op == 0x07) one("pop", SegOp(0));
  else if (op == 0x0E) one("push", SegOp(1));
  else if (op == 0x16) one("push", SegOp(2));
  else if (op == 0x17) one("pop", SegOp(2));
  else if (op == 0x1E) one("push", SegOp(3));
  else if (op == 0x1F) one("pop", SegOp(3));
  else if (op == 0x27) in.mnem = "daa";
  else if (op == 0x2F) in.mnem = "das";
  else if (op == 0x37) in.mnem = "aaa";
  else if (op == 0x3F) in.mnem = "aas";
  // arithmetic group 00-3D (add/or/adc/sbb/and/sub/xor/cmp), pattern of 6
  else if (op < 0x40 && (op & 7) < 6 && (op & 0xC0) == 0 &&
      ((op >> 3) & 7) < 8 && (op & 0x07) <= 5 && kArith[(op >> 3) & 7][0]) {
    int kind = (op >> 3) & 7;
    int lo = op & 7;
    if (lo == 0) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two(kArith[kind], rm, RegOp(r, 1)); }
    else if (lo == 1) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two(kArith[kind], rm, RegOp(r, osz)); }
    else if (lo == 2) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two(kArith[kind], RegOp(r, 1), rm); }
    else if (lo == 3) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two(kArith[kind], RegOp(r, osz), rm); }
    else if (lo == 4) { two(kArith[kind], RegOp(0, 1), ImmOp(c.u8(), 1)); }
    else { two(kArith[kind], RegOp(0, osz), ImmOp(op16 ? c.u16() : c.u32(), osz)); }
  }
  else if (op >= 0x40 && op <= 0x47) one("inc", RegOp(op - 0x40, osz));
  else if (op >= 0x48 && op <= 0x4F) one("dec", RegOp(op - 0x48, osz));
  else if (op >= 0x50 && op <= 0x57) one("push", RegOp(op - 0x50, osz));
  else if (op >= 0x58 && op <= 0x5F) one("pop", RegOp(op - 0x58, osz));
  else if (op == 0x68) one("push", ImmOp(op16 ? c.u16() : c.u32(), osz));
  else if (op == 0x6A) one("push", ImmOp(c.s8(), osz));
  else if (op == 0x69) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); in.mnem="imul"; in.ops[0]=RegOp(r,osz); in.ops[1]=rm; in.ops[2]=ImmOp(op16?c.u16():c.u32(),osz); in.nops=3; }
  else if (op == 0x6B) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); in.mnem="imul"; in.ops[0]=RegOp(r,osz); in.ops[1]=rm; in.ops[2]=ImmOp(c.s8(),osz); in.nops=3; }
  else if (op >= 0x70 && op <= 0x7F) one(kJcc[op - 0x70], rel8());
  else if (op == 0x80) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two(kArith[r], rm, ImmOp(c.u8(), 1)); }
  else if (op == 0x81) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two(kArith[r], rm, ImmOp(op16 ? c.u16() : c.u32(), osz)); }
  else if (op == 0x83) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two(kArith[r], rm, ImmOp(c.s8(), osz)); }
  else if (op == 0x84) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two("test", rm, RegOp(r, 1)); }
  else if (op == 0x85) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("test", rm, RegOp(r, osz)); }
  else if (op == 0x86) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two("xchg", rm, RegOp(r, 1)); }
  else if (op == 0x87) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("xchg", rm, RegOp(r, osz)); }
  else if (op == 0x88) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two("mov", rm, RegOp(r, 1)); }
  else if (op == 0x89) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("mov", rm, RegOp(r, osz)); }
  else if (op == 0x8A) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two("mov", RegOp(r, 1), rm); }
  else if (op == 0x8B) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("mov", RegOp(r, osz), rm); }
  else if (op == 0x8C) { X86Op rm; int r = ModRM(c, a32, 2, in.seg_override, rm); two("mov", rm, SegOp(r)); }
  else if (op == 0x8D) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("lea", RegOp(r, osz), rm); }
  else if (op == 0x8E) { X86Op rm; int r = ModRM(c, a32, 2, in.seg_override, rm); two("mov", SegOp(r), rm); }
  else if (op == 0x8F) { X86Op rm; ModRM(c, a32, osz, in.seg_override, rm); one("pop", rm); }
  else if (op == 0x90) in.mnem = "nop";
  else if (op >= 0x91 && op <= 0x97) two("xchg", RegOp(0, osz), RegOp(op - 0x90, osz));
  else if (op == 0x98) in.mnem = op16 ? "cbw" : "cwde";
  else if (op == 0x99) in.mnem = op16 ? "cwd" : "cdq";
  else if (op == 0x9A) { X86Op o; o.kind = OpKind::FarPtr; o.far_off = op16 ? c.u16() : c.u32(); o.far_seg = c.u16(); in.is_far = true; one("call", o); }
  else if (op == 0x9C) in.mnem = "pushf";
  else if (op == 0x9D) in.mnem = "popf";
  else if (op == 0x9E) in.mnem = "sahf";
  else if (op == 0x9F) in.mnem = "lahf";
  else if (op == 0xA0) { X86Op m; m.kind=OpKind::Mem; m.size=1; m.seg=in.seg_override; m.disp=a32?c.u32():c.u16(); two("mov", RegOp(0,1), m); }
  else if (op == 0xA1) { X86Op m; m.kind=OpKind::Mem; m.size=osz; m.seg=in.seg_override; m.disp=a32?c.u32():c.u16(); two("mov", RegOp(0,osz), m); }
  else if (op == 0xA2) { X86Op m; m.kind=OpKind::Mem; m.size=1; m.seg=in.seg_override; m.disp=a32?c.u32():c.u16(); two("mov", m, RegOp(0,1)); }
  else if (op == 0xA3) { X86Op m; m.kind=OpKind::Mem; m.size=osz; m.seg=in.seg_override; m.disp=a32?c.u32():c.u16(); two("mov", m, RegOp(0,osz)); }
  else if (op == 0xA4) in.mnem = "movsb";
  else if (op == 0xA5) in.mnem = op16 ? "movsw" : "movsd";
  else if (op == 0xA6) in.mnem = "cmpsb";
  else if (op == 0xA7) in.mnem = op16 ? "cmpsw" : "cmpsd";
  else if (op == 0xA8) two("test", RegOp(0, 1), ImmOp(c.u8(), 1));
  else if (op == 0xA9) two("test", RegOp(0, osz), ImmOp(op16 ? c.u16() : c.u32(), osz));
  else if (op == 0xAA) in.mnem = "stosb";
  else if (op == 0xAB) in.mnem = op16 ? "stosw" : "stosd";
  else if (op == 0xAC) in.mnem = "lodsb";
  else if (op == 0xAD) in.mnem = op16 ? "lodsw" : "lodsd";
  else if (op == 0xAE) in.mnem = "scasb";
  else if (op == 0xAF) in.mnem = op16 ? "scasw" : "scasd";
  else if (op >= 0xB0 && op <= 0xB7) two("mov", RegOp(op - 0xB0, 1), ImmOp(c.u8(), 1));
  else if (op >= 0xB8 && op <= 0xBF) two("mov", RegOp(op - 0xB8, osz), ImmOp(op16 ? c.u16() : c.u32(), osz));
  else if (op == 0xC0) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two(kShift[r], rm, ImmOp(c.u8(), 1)); }
  else if (op == 0xC1) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two(kShift[r], rm, ImmOp(c.u8(), 1)); }
  else if (op == 0xC2) { in.is_far = false; one("ret", ImmOp(c.u16(), 2)); }
  else if (op == 0xC3) in.mnem = "ret";
  else if (op == 0xC4) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("les", RegOp(r, osz), rm); }
  else if (op == 0xC5) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("lds", RegOp(r, osz), rm); }
  else if (op == 0xC6) { X86Op rm; ModRM(c, a32, 1, in.seg_override, rm); two("mov", rm, ImmOp(c.u8(), 1)); }
  else if (op == 0xC7) { X86Op rm; ModRM(c, a32, osz, in.seg_override, rm); two("mov", rm, ImmOp(op16 ? c.u16() : c.u32(), osz)); }
  else if (op == 0xC8) { in.mnem="enter"; in.ops[0]=ImmOp(c.u16(),2); in.ops[1]=ImmOp(c.u8(),1); in.nops=2; }
  else if (op == 0xC9) in.mnem = "leave";
  else if (op == 0xCA) { in.is_far = true; one("retf", ImmOp(c.u16(), 2)); }
  else if (op == 0xCB) { in.is_far = true; in.mnem = "retf"; }
  else if (op == 0xCC) one("int", ImmOp(3, 1));
  else if (op == 0xCD) one("int", ImmOp(c.u8(), 1));
  else if (op == 0xCF) in.mnem = "iret";
  else if (op >= 0xD0 && op <= 0xD3) {
    X86Op rm; int sz = (op & 1) ? osz : 1; int r = ModRM(c, a32, sz, in.seg_override, rm);
    X86Op amt = (op < 0xD2) ? ImmOp(1, 1) : RegOp(1, 1);   // 1 or CL
    two(kShift[r], rm, amt);
  }
  else if (op == 0xD7) in.mnem = "xlat";
  else if (op >= 0xD8 && op <= 0xDF) { X86Op rm; ModRM(c, a32, osz, in.seg_override, rm); in.mnem = "fpu"; in.ops[0]=rm; in.nops=1; } // x87 stub
  else if (op == 0xE0) one("loopne", rel8());
  else if (op == 0xE1) one("loope", rel8());
  else if (op == 0xE2) one("loop", rel8());
  else if (op == 0xE3) one(op16 ? "jcxz" : "jecxz", rel8());
  else if (op == 0xE4) two("in", RegOp(0,1), ImmOp(c.u8(),1));
  else if (op == 0xE6) two("out", ImmOp(c.u8(),1), RegOp(0,1));
  else if (op == 0xE8) one("call", rel16());
  else if (op == 0xE9) one("jmp", rel16());
  else if (op == 0xEA) { X86Op o; o.kind = OpKind::FarPtr; o.far_off = op16 ? c.u16() : c.u32(); o.far_seg = c.u16(); in.is_far = true; one("jmp", o); }
  else if (op == 0xEB) one("jmp", rel8());
  else if (op == 0xF4) in.mnem = "hlt";
  else if (op == 0xF5) in.mnem = "cmc";
  else if (op == 0xF6 || op == 0xF7) {                 // group 3
    int sz = (op & 1) ? osz : 1; X86Op rm; int r = ModRM(c, a32, sz, in.seg_override, rm);
    const char* g3[8] = {"test","test","not","neg","mul","imul","div","idiv"};
    if (r <= 1) two(g3[r], rm, (sz==1)?ImmOp(c.u8(),1):ImmOp(op16?c.u16():c.u32(),sz));
    else one(g3[r], rm);
  }
  else if (op == 0xF8) in.mnem = "clc";
  else if (op == 0xF9) in.mnem = "stc";
  else if (op == 0xFA) in.mnem = "cli";
  else if (op == 0xFB) in.mnem = "sti";
  else if (op == 0xFC) in.mnem = "cld";
  else if (op == 0xFD) in.mnem = "std";
  else if (op == 0xFE) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); one(r == 0 ? "inc" : "dec", rm); }
  else if (op == 0xFF) {                               // group 5
    X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm);
    const char* g5[8] = {"inc","dec","call","call","jmp","jmp","push","?"};
    if (r == 3 || r == 5) in.is_far = true;
    one(g5[r], rm);
  }
  else if (op == 0x0F) {                               // two-byte map (subset)
    uint8_t o2 = c.u8();
    if (o2 >= 0x80 && o2 <= 0x8F) one(kJcc[o2 - 0x80], rel16());
    else if (o2 >= 0x90 && o2 <= 0x9F) { X86Op rm; ModRM(c, a32, 1, in.seg_override, rm); std::string m = "set"; m += kJcc[o2-0x90]+1; one(m.c_str(), rm); }
    else if (o2 == 0xB6) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two("movzx", RegOp(r, osz), rm); }
    else if (o2 == 0xB7) { X86Op rm; int r = ModRM(c, a32, 2, in.seg_override, rm); two("movzx", RegOp(r, osz), rm); }
    else if (o2 == 0xBE) { X86Op rm; int r = ModRM(c, a32, 1, in.seg_override, rm); two("movsx", RegOp(r, osz), rm); }
    else if (o2 == 0xBF) { X86Op rm; int r = ModRM(c, a32, 2, in.seg_override, rm); two("movsx", RegOp(r, osz), rm); }
    else if (o2 == 0xAF) { X86Op rm; int r = ModRM(c, a32, osz, in.seg_override, rm); two("imul", RegOp(r, osz), rm); }
    else { in.mnem = "(0f)"; }
  }
  else { in.valid = false; in.len = 1; return in; }

  if (c.bad) { in.valid = false; in.len = 1; return in; }
  in.len = (int)(c.p - pos);
  in.valid = true;
  return in;
}

std::string RegName(int reg, int size) {
  int cls = reg / 16, idx = reg % 16;
  if (cls == RC_SEG) return idx < 6 ? kSeg[idx] : "?seg";
  if (cls == RC_ST) return "st" + std::to_string(idx);
  if (idx > 7) return "?r";
  return size == 1 ? kGpr8[idx] : size == 4 ? kGpr32[idx] : kGpr16[idx];
}

// One unsigned hex literal in the chosen syntax: NASM `0x1f`, MASM/Watcom `1fh` (leading 0
// when it would start with a letter, so `ffh` -> `0ffh`, which wasm/ML require).
static std::string HexU(unsigned long long u, AsmSyntax syn) {
  char b[32];
  if (syn == AsmSyntax::Nasm) { std::snprintf(b, sizeof b, "0x%llx", u); return b; }
  std::snprintf(b, sizeof b, "%llx", u);
  std::string s = b;
  if (!s.empty() && s[0] >= 'a' && s[0] <= 'f') s = "0" + s;
  return s + "h";
}

static std::string HexI(int64_t v, AsmSyntax syn) {
  return v < 0 ? "-" + HexU((unsigned long long)(-v), syn) : HexU((unsigned long long)v, syn);
}

static std::string OpStr(const X86Op& o, AsmSyntax syn) {
  switch (o.kind) {
    case OpKind::Reg: return RegName(o.reg, o.size);
    case OpKind::Imm: return HexI(o.imm, syn);
    case OpKind::Rel: return HexU((unsigned)o.imm, syn);
    case OpKind::FarPtr: return HexU(o.far_seg, syn) + ":" + HexU(o.far_off, syn);
    case OpKind::Mem: {
      // size specifier: MASM needs `ptr` (`word ptr [..]`), NASM does not (`word [..]`).
      const char* sz = o.size == 1 ? "byte" : o.size == 4 ? "dword" : "word";
      std::string s = sz;
      s += (syn == AsmSyntax::Masm) ? " ptr " : " ";
      // segment override: MASM places it before the brackets (`es:[si]`), NASM inside (`[es:si]`).
      std::string seg = (o.seg != kNoReg) ? RegName(o.seg, 2) + ":" : "";
      if (syn == AsmSyntax::Masm) s += seg;
      s += "[";
      if (syn == AsmSyntax::Nasm) s += seg;
      bool any = false;
      if (o.base != kNoReg) { s += RegName(o.base, 2); any = true; }
      if (o.index != kNoReg) { if (any) s += "+"; s += RegName(o.index, 2); if (o.scale > 1) s += "*" + std::to_string(o.scale); any = true; }
      if (o.disp || !any) { if (any && o.disp >= 0) s += "+"; s += HexI(o.disp, syn); }
      s += "]";
      return s;
    }
    default: return "";
  }
}

// String / port instructions (movs, stos, lods, scas, cmps, ins, outs) carry their
// operands implicitly in x86: the addressing is fixed to [(e)si]/[(e)di]/DX and a size
// suffix, so an assembler wants only the bare mnemonic (`movsb`, `rep stosw`). Our decoder
// attaches explicit operands for its own IL, but printing them (`outsw dx, word ptr [si]`)
// is both wrong disassembly and unassemblable -- suppress them for these mnemonics.
static bool ImplicitOperandString(const char* m) {
  static const char* kPfx[] = {"movs", "stos", "lods", "scas", "cmps", "ins", "outs"};
  for (const char* p : kPfx) {
    size_t n = std::strlen(p);
    if (std::strncmp(m, p, n) != 0) continue;
    char t = m[n];
    if (t == '\0') return true;                                 // bare "movs"
    if ((t == 'b' || t == 'w' || t == 'd') && m[n + 1] == '\0') return true;  // "movsb"/"stosw"/...
  }
  return false;
}

std::string FormatX86(const X86Insn& in, AsmSyntax syn) {
  if (!in.valid) return "(bad)";
  std::string s;
  if (in.lock) s += "lock ";
  if (in.rep) s += "rep ";
  if (in.repne) s += "repne ";
  s += in.mnem;
  if (ImplicitOperandString(in.mnem.c_str())) return s; // bare mnemonic, no explicit operands
  for (int i = 0; i < in.nops; ++i) { s += i ? ", " : " "; s += OpStr(in.ops[i], syn); }
  return s;
}

}  // namespace win95
