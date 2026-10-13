// XEXMagic - PPC Disassembler
// Standalone PowerPC disassembler for Xbox 360 executables

#include "ppc_disasm.h"

#include <cstdarg>
#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <string>

#include "ppc/ppc_opcode.h"
#include "ppc/ppc_opcode_info.h"
#include "xenia/base/string_buffer.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static inline uint32_t BITS(uint32_t w, int hi, int lo) {
  // PPC bit numbering: bit 0 is MSB (bit 31 in C).
  // BITS(w, 0, 5) extracts primary opcode.
  return (w >> (31 - lo)) & ((1u << (lo - hi + 1)) - 1);
}

static inline int32_t SignExt(uint32_t val, int bits) {
  uint32_t sign = 1u << (bits - 1);
  return (int32_t)((val ^ sign) - sign);
}

static int snfmt(char* buf, size_t sz, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int r = vsnprintf(buf, sz, fmt, ap);
  va_end(ap);
  return r;
}

// Field extractors (PPC bit numbering)
static inline uint32_t PrimaryOp(uint32_t w) { return BITS(w, 0, 5); }
static inline uint32_t RD(uint32_t w)        { return BITS(w, 6, 10); }
static inline uint32_t RS(uint32_t w)        { return BITS(w, 6, 10); }
static inline uint32_t FRD(uint32_t w)       { return BITS(w, 6, 10); }
static inline uint32_t FRS(uint32_t w)       { return BITS(w, 6, 10); }
static inline uint32_t RA(uint32_t w)        { return BITS(w, 11, 15); }
static inline uint32_t RB(uint32_t w)        { return BITS(w, 16, 20); }
static inline uint32_t FRA(uint32_t w)       { return BITS(w, 11, 15); }
static inline uint32_t FRB(uint32_t w)       { return BITS(w, 16, 20); }
static inline uint32_t FRC(uint32_t w)       { return BITS(w, 21, 25); }
static inline uint32_t RC(uint32_t w)        { return w & 1; }
static inline uint32_t OE(uint32_t w)        { return BITS(w, 21, 21); }
static inline uint32_t LK(uint32_t w)        { return w & 1; }
static inline uint32_t AA(uint32_t w)        { return (w >> 1) & 1; }
static inline uint32_t BO(uint32_t w)        { return BITS(w, 6, 10); }
static inline uint32_t BI(uint32_t w)        { return BITS(w, 11, 15); }
static inline uint32_t BD(uint32_t w)        { return BITS(w, 16, 29); }
static inline uint32_t LI(uint32_t w)        { return BITS(w, 6, 29); }
static inline int16_t  SIMM(uint32_t w)      { return (int16_t)(w & 0xFFFF); }
static inline uint16_t UIMM(uint32_t w)      { return (uint16_t)(w & 0xFFFF); }
static inline uint32_t SH(uint32_t w)        { return BITS(w, 16, 20); }
static inline uint32_t MB(uint32_t w)        { return BITS(w, 21, 25); }
static inline uint32_t ME(uint32_t w)        { return BITS(w, 26, 30); }
static inline uint32_t XO_10(uint32_t w)     { return BITS(w, 21, 30); } // X-form extended
static inline uint32_t XO_9(uint32_t w)      { return BITS(w, 22, 30); } // XO-form extended
static inline uint32_t SPR(uint32_t w) {
  uint32_t spr = BITS(w, 11, 20);
  return ((spr & 0x1F) << 5) | ((spr >> 5) & 0x1F);
}
static inline uint32_t CRM(uint32_t w) { return BITS(w, 12, 19); }
static inline uint32_t CRFD(uint32_t w) { return BITS(w, 6, 8); }
static inline uint32_t CRFS(uint32_t w) { return BITS(w, 11, 13); }
static inline uint32_t TO(uint32_t w) { return BITS(w, 6, 10); }

// A-form sub-opcode (bits 26-30)
static inline uint32_t AFormXO(uint32_t w) { return BITS(w, 26, 30); }

// ---------------------------------------------------------------------------
// Branch condition decode
// ---------------------------------------------------------------------------

static const char* BranchCondMnemonic(uint32_t bo, uint32_t bi,
                                       char* suffix_buf, size_t suffix_sz) {
  uint32_t cr_field = bi >> 2;
  uint32_t cr_bit = bi & 3;  // 0=lt 1=gt 2=eq 3=so

  bool decr = !(bo & 0x04);       // decrement CTR
  bool ctr_zero = (bo & 0x02);    // branch if CTR==0 (after decr)
  bool no_cond = (bo & 0x10);     // ignore condition
  bool cond_true = (bo & 0x08);   // branch if condition true

  suffix_buf[0] = '\0';
  if (cr_field != 0) {
    snfmt(suffix_buf, suffix_sz, "cr%u", cr_field);
  }

  if (decr && no_cond) {
    return ctr_zero ? "bdz" : "bdnz";
  }
  if (!decr && no_cond) {
    return "b"; // unconditional (via bc)
  }
  if (!decr && !no_cond) {
    static const char* cond_names_true[4]  = {"lt", "gt", "eq", "so"};
    static const char* cond_names_false[4] = {"ge", "le", "ne", "ns"};
    if (cond_true)
      return cond_names_true[cr_bit];
    else
      return cond_names_false[cr_bit];
  }
  // decr + condition: less common, fall through to raw
  return nullptr;
}

// ---------------------------------------------------------------------------
// Format a single instruction
// ---------------------------------------------------------------------------

static const int kBufSize = 128;

// SPR names
static const char* SprName(uint32_t spr) {
  switch (spr) {
    case 1:   return "xer";
    case 8:   return "lr";
    case 9:   return "ctr";
    case 18:  return "dsisr";
    case 19:  return "dar";
    case 22:  return "dec";
    case 26:  return "srr0";
    case 27:  return "srr1";
    case 272: return "sprg0";
    case 273: return "sprg1";
    case 274: return "sprg2";
    case 275: return "sprg3";
    case 284: return "tbl";
    case 285: return "tbu";
    case 287: return "pvr";
    default:  return nullptr;
  }
}

// Rc suffix
static inline char RcDot(uint32_t w) { return RC(w) ? '.' : '\0'; }

static std::string FormatRcSuffix(const char* mnemonic, uint32_t w) {
  char buf[32];
  if (RC(w))
    snfmt(buf, sizeof(buf), "%s.", mnemonic);
  else
    snfmt(buf, sizeof(buf), "%s", mnemonic);
  return buf;
}

// The hand-written decoder below covers the integer, branch and FP core. The
// rest of the ISA - the 64-bit fixed-point forms, AltiVec, and the Xenon
// VMX128 encodings - comes from the generated tables, which know the whole
// instruction set. Anything the switch would have printed as a raw word is
// handed here first.
static bool TableDisasm(uint32_t address, uint32_t code, std::string* out) {
  using namespace xe::cpu::ppc;
  if (LookupOpcode(code) == PPCOpcode::kInvalid) return false;
  xe::StringBuffer sb;
  if (!DisasmPPC(address, code, &sb)) return false;
  *out = sb.to_string();
  // Trailing padding from the table formatter, and a name with no operands is
  // not worth printing over the raw word.
  while (!out->empty() && out->back() == ' ') out->pop_back();
  return !out->empty();
}

std::string DisasmInstructionCore(uint32_t address, uint32_t code);

std::string DisasmInstruction(uint32_t address, uint32_t code) {
  std::string text = DisasmInstructionCore(address, code);
  if (text.compare(0, 5, ".long") == 0) {
    std::string table_text;
    if (TableDisasm(address, code, &table_text)) return table_text;
  }
  return text;
}

std::string DisasmInstructionCore(uint32_t address, uint32_t code) {
  char buf[kBufSize];
  uint32_t op = PrimaryOp(code);

  switch (op) {
    // -----------------------------------------------------------------------
    // Opcode 2: tdi
    // -----------------------------------------------------------------------
    case 2: {
      snfmt(buf, kBufSize, "tdi     %u, r%u, %d", TO(code), RA(code), SIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 3: twi
    // -----------------------------------------------------------------------
    case 3: {
      snfmt(buf, kBufSize, "twi     %u, r%u, %d", TO(code), RA(code), SIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 7: mulli
    // -----------------------------------------------------------------------
    case 7: {
      snfmt(buf, kBufSize, "mulli   r%u, r%u, %d", RD(code), RA(code), SIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 8: subfic
    // -----------------------------------------------------------------------
    case 8: {
      snfmt(buf, kBufSize, "subfic  r%u, r%u, %d", RD(code), RA(code), SIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 10: cmpli (cmplwi alias)
    // -----------------------------------------------------------------------
    case 10: {
      uint32_t crfd = CRFD(code);
      uint32_t L = BITS(code, 10, 10);
      if (crfd == 0 && L == 0)
        snfmt(buf, kBufSize, "cmplwi  r%u, 0x%X", RA(code), UIMM(code));
      else if (L == 0)
        snfmt(buf, kBufSize, "cmplwi  cr%u, r%u, 0x%X", crfd, RA(code), UIMM(code));
      else if (crfd == 0)
        snfmt(buf, kBufSize, "cmpldi  r%u, 0x%X", RA(code), UIMM(code));
      else
        snfmt(buf, kBufSize, "cmpldi  cr%u, r%u, 0x%X", crfd, RA(code), UIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 11: cmpi (cmpwi alias)
    // -----------------------------------------------------------------------
    case 11: {
      uint32_t crfd = CRFD(code);
      uint32_t L = BITS(code, 10, 10);
      if (crfd == 0 && L == 0)
        snfmt(buf, kBufSize, "cmpwi   r%u, %d", RA(code), SIMM(code));
      else if (L == 0)
        snfmt(buf, kBufSize, "cmpwi   cr%u, r%u, %d", crfd, RA(code), SIMM(code));
      else if (crfd == 0)
        snfmt(buf, kBufSize, "cmpdi   r%u, %d", RA(code), SIMM(code));
      else
        snfmt(buf, kBufSize, "cmpdi   cr%u, r%u, %d", crfd, RA(code), SIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 12: addic
    // -----------------------------------------------------------------------
    case 12: {
      snfmt(buf, kBufSize, "addic   r%u, r%u, %d", RD(code), RA(code), SIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 13: addic.
    // -----------------------------------------------------------------------
    case 13: {
      snfmt(buf, kBufSize, "addic.  r%u, r%u, %d", RD(code), RA(code), SIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 14: addi / li / subi
    // -----------------------------------------------------------------------
    case 14: {
      if (RA(code) == 0) {
        snfmt(buf, kBufSize, "li      r%u, %d", RD(code), SIMM(code));
      } else if (SIMM(code) < 0) {
        snfmt(buf, kBufSize, "subi    r%u, r%u, %d", RD(code), RA(code), -(int)SIMM(code));
      } else {
        snfmt(buf, kBufSize, "addi    r%u, r%u, %d", RD(code), RA(code), SIMM(code));
      }
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 15: addis / lis
    // -----------------------------------------------------------------------
    case 15: {
      if (RA(code) == 0)
        snfmt(buf, kBufSize, "lis     r%u, 0x%X", RD(code), UIMM(code));
      else
        snfmt(buf, kBufSize, "addis   r%u, r%u, 0x%X", RD(code), RA(code), UIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 16: bc / bca / bcl / bcla (and aliases: beq, bne, blt, bgt, etc.)
    // -----------------------------------------------------------------------
    case 16: {
      uint32_t bo = BO(code);
      uint32_t bi = BI(code);
      int32_t bd = SignExt(BD(code) << 2, 16);
      uint32_t target;
      if (AA(code))
        target = (uint32_t)bd;
      else
        target = address + bd;

      char cr_suffix[16];
      const char* mnem = BranchCondMnemonic(bo, bi, cr_suffix, sizeof(cr_suffix));
      if (mnem) {
        char full_mnem[32];
        snfmt(full_mnem, sizeof(full_mnem), "b%s%s%s", mnem,
              LK(code) ? "l" : "", AA(code) ? "a" : "");
        if (cr_suffix[0])
          snfmt(buf, kBufSize, "%-8s %s, 0x%08X", full_mnem, cr_suffix, target);
        else
          snfmt(buf, kBufSize, "%-8s0x%08X", full_mnem, target);
      } else {
        snfmt(buf, kBufSize, "bc%s%s   %u, %u, 0x%08X",
              LK(code) ? "l" : "", AA(code) ? "a" : "", bo, bi, target);
      }
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 17: sc
    // -----------------------------------------------------------------------
    case 17: {
      snfmt(buf, kBufSize, "sc");
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 18: b / ba / bl / bla
    // -----------------------------------------------------------------------
    case 18: {
      int32_t li = SignExt(LI(code) << 2, 26);
      uint32_t target;
      if (AA(code))
        target = (uint32_t)li;
      else
        target = address + li;
      snfmt(buf, kBufSize, "b%s%s    0x%08X",
            LK(code) ? "l" : "", AA(code) ? "a" : "", target);
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 19: XL-form (bclr, bcctr, crand, cror, crxor, mcrf, isync, etc.)
    // -----------------------------------------------------------------------
    case 19: {
      uint32_t xo = XO_10(code);
      switch (xo) {
        case 0: { // mcrf
          snfmt(buf, kBufSize, "mcrf    cr%u, cr%u", CRFD(code), CRFS(code));
          return buf;
        }
        case 16: { // bclr / bclrl
          uint32_t bo = BO(code);
          uint32_t bi = BI(code);
          if (bo == 20 && bi == 0) {
            snfmt(buf, kBufSize, "blr%s", LK(code) ? "l" : "");
          } else {
            char cr_suffix[16];
            const char* mnem = BranchCondMnemonic(bo, bi, cr_suffix, sizeof(cr_suffix));
            if (mnem) {
              char full_mnem[32];
              snfmt(full_mnem, sizeof(full_mnem), "b%slr%s", mnem, LK(code) ? "l" : "");
              if (cr_suffix[0])
                snfmt(buf, kBufSize, "%-8s%s", full_mnem, cr_suffix);
              else
                snfmt(buf, kBufSize, "%s", full_mnem);
            } else {
              snfmt(buf, kBufSize, "bclr%s  %u, %u", LK(code) ? "l" : "", bo, bi);
            }
          }
          return buf;
        }
        case 150: // isync
          snfmt(buf, kBufSize, "isync");
          return buf;
        case 257: // crand
          snfmt(buf, kBufSize, "crand   %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 129: // crandc
          snfmt(buf, kBufSize, "crandc  %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 289: // creqv
          snfmt(buf, kBufSize, "creqv   %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 225: // crnand
          snfmt(buf, kBufSize, "crnand  %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 33: // crnor
          snfmt(buf, kBufSize, "crnor   %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 449: // cror
          snfmt(buf, kBufSize, "cror    %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 417: // crorc
          snfmt(buf, kBufSize, "crorc   %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 193: // crxor
          snfmt(buf, kBufSize, "crxor   %u, %u, %u", RD(code), RA(code), RB(code));
          return buf;
        case 528: { // bcctr / bcctrl
          uint32_t bo = BO(code);
          uint32_t bi = BI(code);
          if (bo == 20 && bi == 0) {
            snfmt(buf, kBufSize, "bctr%s", LK(code) ? "l" : "");
          } else {
            char cr_suffix[16];
            const char* mnem = BranchCondMnemonic(bo, bi, cr_suffix, sizeof(cr_suffix));
            if (mnem) {
              char full_mnem[32];
              snfmt(full_mnem, sizeof(full_mnem), "b%sctr%s", mnem, LK(code) ? "l" : "");
              if (cr_suffix[0])
                snfmt(buf, kBufSize, "%-8s%s", full_mnem, cr_suffix);
              else
                snfmt(buf, kBufSize, "%s", full_mnem);
            } else {
              snfmt(buf, kBufSize, "bcctr%s %u, %u", LK(code) ? "l" : "", bo, bi);
            }
          }
          return buf;
        }
        default:
          break;
      }
      snfmt(buf, kBufSize, ".long   0x%08X  ; op19 xo=%u", code, xo);
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 20: rlwimi
    // -----------------------------------------------------------------------
    case 20: {
      auto mn = FormatRcSuffix("rlwimi", code);
      snfmt(buf, kBufSize, "%-8sr%u, r%u, %u, %u, %u",
            mn.c_str(), RA(code), RS(code), SH(code), MB(code), ME(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 21: rlwinm (and common aliases: clrlwi, clrrwi, slwi, srwi, etc.)
    // -----------------------------------------------------------------------
    case 21: {
      auto mn = FormatRcSuffix("rlwinm", code);
      snfmt(buf, kBufSize, "%-8sr%u, r%u, %u, %u, %u",
            mn.c_str(), RA(code), RS(code), SH(code), MB(code), ME(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 23: rlwnm
    // -----------------------------------------------------------------------
    case 23: {
      auto mn = FormatRcSuffix("rlwnm", code);
      snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u, %u, %u",
            mn.c_str(), RA(code), RS(code), RB(code), MB(code), ME(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 24: ori / nop
    // -----------------------------------------------------------------------
    case 24: {
      if (RS(code) == 0 && RA(code) == 0 && UIMM(code) == 0) {
        snfmt(buf, kBufSize, "nop");
      } else if (UIMM(code) == 0 && RS(code) == RA(code)) {
        // mr alias isn't quite right for ori, but ori rA,rS,0 is effectively nop/mr
        snfmt(buf, kBufSize, "ori     r%u, r%u, 0", RA(code), RS(code));
      } else {
        snfmt(buf, kBufSize, "ori     r%u, r%u, 0x%X", RA(code), RS(code), UIMM(code));
      }
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 25: oris
    // -----------------------------------------------------------------------
    case 25: {
      snfmt(buf, kBufSize, "oris    r%u, r%u, 0x%X", RA(code), RS(code), UIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 26: xori
    // -----------------------------------------------------------------------
    case 26: {
      snfmt(buf, kBufSize, "xori    r%u, r%u, 0x%X", RA(code), RS(code), UIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 27: xoris
    // -----------------------------------------------------------------------
    case 27: {
      snfmt(buf, kBufSize, "xoris   r%u, r%u, 0x%X", RA(code), RS(code), UIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 28: andi.
    // -----------------------------------------------------------------------
    case 28: {
      snfmt(buf, kBufSize, "andi.   r%u, r%u, 0x%X", RA(code), RS(code), UIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 29: andis.
    // -----------------------------------------------------------------------
    case 29: {
      snfmt(buf, kBufSize, "andis.  r%u, r%u, 0x%X", RA(code), RS(code), UIMM(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 31: X/XO/XFX-form (ALU, compare, SPR, cache, etc.)
    // -----------------------------------------------------------------------
    case 31: {
      uint32_t xo10 = XO_10(code);
      uint32_t xo9 = XO_9(code);

      // Try XO-form first (bits 22-30)
      switch (xo9) {
        case 266: { // add / addo
          auto mn = FormatRcSuffix(OE(code) ? "addo" : "add", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 10: { // addc
          auto mn = FormatRcSuffix(OE(code) ? "addco" : "addc", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 138: { // adde
          auto mn = FormatRcSuffix(OE(code) ? "addeo" : "adde", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 234: { // addme
          auto mn = FormatRcSuffix(OE(code) ? "addmeo" : "addme", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RD(code), RA(code));
          return buf;
        }
        case 202: { // addze
          auto mn = FormatRcSuffix(OE(code) ? "addzeo" : "addze", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RD(code), RA(code));
          return buf;
        }
        case 491: { // divw
          auto mn = FormatRcSuffix(OE(code) ? "divwo" : "divw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 459: { // divwu
          auto mn = FormatRcSuffix(OE(code) ? "divwuo" : "divwu", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 235: { // mullw
          auto mn = FormatRcSuffix(OE(code) ? "mullwo" : "mullw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 104: { // neg
          auto mn = FormatRcSuffix(OE(code) ? "nego" : "neg", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RD(code), RA(code));
          return buf;
        }
        case 40: { // subf
          auto mn = FormatRcSuffix(OE(code) ? "subfo" : "subf", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 8: { // subfc
          auto mn = FormatRcSuffix(OE(code) ? "subfco" : "subfc", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 136: { // subfe
          auto mn = FormatRcSuffix(OE(code) ? "subfeo" : "subfe", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 232: { // subfme
          auto mn = FormatRcSuffix(OE(code) ? "subfmeo" : "subfme", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RD(code), RA(code));
          return buf;
        }
        case 200: { // subfze
          auto mn = FormatRcSuffix(OE(code) ? "subfzeo" : "subfze", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RD(code), RA(code));
          return buf;
        }
        case 75: { // mulhw
          auto mn = FormatRcSuffix("mulhw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        case 11: { // mulhwu
          auto mn = FormatRcSuffix("mulhwu", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RD(code), RA(code), RB(code));
          return buf;
        }
        default:
          break;
      }

      // X-form (bits 21-30)
      switch (xo10) {
        // Compare
        case 0: { // cmp / cmpw
          uint32_t crfd = CRFD(code);
          uint32_t L = BITS(code, 10, 10);
          if (crfd == 0 && L == 0)
            snfmt(buf, kBufSize, "cmpw    r%u, r%u", RA(code), RB(code));
          else if (L == 0)
            snfmt(buf, kBufSize, "cmpw    cr%u, r%u, r%u", crfd, RA(code), RB(code));
          else if (crfd == 0)
            snfmt(buf, kBufSize, "cmpd    r%u, r%u", RA(code), RB(code));
          else
            snfmt(buf, kBufSize, "cmpd    cr%u, r%u, r%u", crfd, RA(code), RB(code));
          return buf;
        }
        case 32: { // cmpl / cmplw
          uint32_t crfd = CRFD(code);
          uint32_t L = BITS(code, 10, 10);
          if (crfd == 0 && L == 0)
            snfmt(buf, kBufSize, "cmplw   r%u, r%u", RA(code), RB(code));
          else if (L == 0)
            snfmt(buf, kBufSize, "cmplw   cr%u, r%u, r%u", crfd, RA(code), RB(code));
          else if (crfd == 0)
            snfmt(buf, kBufSize, "cmpld   r%u, r%u", RA(code), RB(code));
          else
            snfmt(buf, kBufSize, "cmpld   cr%u, r%u, r%u", crfd, RA(code), RB(code));
          return buf;
        }

        // Logic
        case 28: { // and
          auto mn = FormatRcSuffix("and", code);
          if (RS(code) == RB(code)) {
            // mr alias: or rA, rS, rS ... wait, and rA,rS,rS is also mr
            // Actually mr is 'or'. and rA,rS,rS is just and.
          }
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 60: { // andc
          auto mn = FormatRcSuffix("andc", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 444: { // or / mr
          auto mn = FormatRcSuffix("or", code);
          if (RS(code) == RB(code) && !RC(code)) {
            snfmt(buf, kBufSize, "mr      r%u, r%u", RA(code), RS(code));
          } else if (RS(code) == RB(code)) {
            snfmt(buf, kBufSize, "mr.     r%u, r%u", RA(code), RS(code));
          } else {
            snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          }
          return buf;
        }
        case 412: { // orc
          auto mn = FormatRcSuffix("orc", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 316: { // xor
          auto mn = FormatRcSuffix("xor", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 476: { // nand
          auto mn = FormatRcSuffix("nand", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 124: { // nor / not
          auto mn = FormatRcSuffix("nor", code);
          if (RS(code) == RB(code) && !RC(code)) {
            snfmt(buf, kBufSize, "not     r%u, r%u", RA(code), RS(code));
          } else if (RS(code) == RB(code)) {
            snfmt(buf, kBufSize, "not.    r%u, r%u", RA(code), RS(code));
          } else {
            snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          }
          return buf;
        }
        case 284: { // eqv
          auto mn = FormatRcSuffix("eqv", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }

        // Count / Extend
        case 26: { // cntlzw
          auto mn = FormatRcSuffix("cntlzw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RA(code), RS(code));
          return buf;
        }
        case 954: { // extsb
          auto mn = FormatRcSuffix("extsb", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RA(code), RS(code));
          return buf;
        }
        case 922: { // extsh
          auto mn = FormatRcSuffix("extsh", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RA(code), RS(code));
          return buf;
        }
        case 986: { // extsw (64-bit)
          auto mn = FormatRcSuffix("extsw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u", mn.c_str(), RA(code), RS(code));
          return buf;
        }

        // Shift
        case 24: { // slw
          auto mn = FormatRcSuffix("slw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 536: { // srw
          auto mn = FormatRcSuffix("srw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 792: { // sraw
          auto mn = FormatRcSuffix("sraw", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, r%u", mn.c_str(), RA(code), RS(code), RB(code));
          return buf;
        }
        case 824: { // srawi
          auto mn = FormatRcSuffix("srawi", code);
          snfmt(buf, kBufSize, "%-8sr%u, r%u, %u", mn.c_str(), RA(code), RS(code), SH(code));
          return buf;
        }

        // Load indexed
        case 87: { // lbzx
          snfmt(buf, kBufSize, "lbzx    r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 279: { // lhzx
          snfmt(buf, kBufSize, "lhzx    r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 343: { // lhax
          snfmt(buf, kBufSize, "lhax    r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 23: { // lwzx
          snfmt(buf, kBufSize, "lwzx    r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 341: { // lwax (64-bit)
          snfmt(buf, kBufSize, "lwax    r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 21: { // ldx (64-bit)
          snfmt(buf, kBufSize, "ldx     r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 535: { // lfsx
          snfmt(buf, kBufSize, "lfsx    fr%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 599: { // lfdx
          snfmt(buf, kBufSize, "lfdx    fr%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 534: { // lwbrx
          snfmt(buf, kBufSize, "lwbrx   r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 790: { // lhbrx
          snfmt(buf, kBufSize, "lhbrx   r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }
        case 20: { // lwarx
          snfmt(buf, kBufSize, "lwarx   r%u, r%u, r%u", RD(code), RA(code), RB(code));
          return buf;
        }

        // Store indexed
        case 215: { // stbx
          snfmt(buf, kBufSize, "stbx    r%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 407: { // sthx
          snfmt(buf, kBufSize, "sthx    r%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 151: { // stwx
          snfmt(buf, kBufSize, "stwx    r%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 149: { // stdx
          snfmt(buf, kBufSize, "stdx    r%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 663: { // stfsx
          snfmt(buf, kBufSize, "stfsx   fr%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 727: { // stfdx
          snfmt(buf, kBufSize, "stfdx   fr%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 662: { // stwbrx
          snfmt(buf, kBufSize, "stwbrx  r%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 918: { // sthbrx
          snfmt(buf, kBufSize, "sthbrx  r%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }
        case 150: { // stwcx.
          snfmt(buf, kBufSize, "stwcx.  r%u, r%u, r%u", RS(code), RA(code), RB(code));
          return buf;
        }

        // SPR
        case 339: { // mfspr
          uint32_t spr = SPR(code);
          const char* name = SprName(spr);
          if (spr == 8) {
            snfmt(buf, kBufSize, "mflr    r%u", RD(code));
          } else if (spr == 9) {
            snfmt(buf, kBufSize, "mfctr   r%u", RD(code));
          } else if (name) {
            snfmt(buf, kBufSize, "mfspr   r%u, %s", RD(code), name);
          } else {
            snfmt(buf, kBufSize, "mfspr   r%u, %u", RD(code), spr);
          }
          return buf;
        }
        case 467: { // mtspr
          uint32_t spr = SPR(code);
          const char* name = SprName(spr);
          if (spr == 8) {
            snfmt(buf, kBufSize, "mtlr    r%u", RS(code));
          } else if (spr == 9) {
            snfmt(buf, kBufSize, "mtctr   r%u", RS(code));
          } else if (name) {
            snfmt(buf, kBufSize, "mtspr   %s, r%u", name, RS(code));
          } else {
            snfmt(buf, kBufSize, "mtspr   %u, r%u", spr, RS(code));
          }
          return buf;
        }
        case 19: { // mfcr
          snfmt(buf, kBufSize, "mfcr    r%u", RD(code));
          return buf;
        }
        case 144: { // mtcrf
          uint32_t crm = CRM(code);
          if (crm == 0xFF)
            snfmt(buf, kBufSize, "mtcr    r%u", RS(code));
          else
            snfmt(buf, kBufSize, "mtcrf   0x%02X, r%u", crm, RS(code));
          return buf;
        }
        case 83: { // mfmsr
          snfmt(buf, kBufSize, "mfmsr   r%u", RD(code));
          return buf;
        }
        case 146: { // mtmsr
          snfmt(buf, kBufSize, "mtmsr   r%u", RS(code));
          return buf;
        }
        case 178: { // mtmsrd
          snfmt(buf, kBufSize, "mtmsrd  r%u", RS(code));
          return buf;
        }

        // Trap
        case 4: { // tw
          snfmt(buf, kBufSize, "tw      %u, r%u, r%u", TO(code), RA(code), RB(code));
          return buf;
        }
        case 68: { // td
          snfmt(buf, kBufSize, "td      %u, r%u, r%u", TO(code), RA(code), RB(code));
          return buf;
        }

        // Sync / barriers
        case 598: { // sync
          uint32_t L = BITS(code, 9, 10);
          if (L == 1)
            snfmt(buf, kBufSize, "lwsync");
          else if (L == 2)
            snfmt(buf, kBufSize, "ptesync");
          else
            snfmt(buf, kBufSize, "sync");
          return buf;
        }
        case 854: // eieio
          snfmt(buf, kBufSize, "eieio");
          return buf;

        // Cache
        case 86:  snfmt(buf, kBufSize, "dcbf    r%u, r%u", RA(code), RB(code)); return buf;
        case 54:  snfmt(buf, kBufSize, "dcbst   r%u, r%u", RA(code), RB(code)); return buf;
        case 278: snfmt(buf, kBufSize, "dcbt    r%u, r%u", RA(code), RB(code)); return buf;
        case 246: snfmt(buf, kBufSize, "dcbtst  r%u, r%u", RA(code), RB(code)); return buf;
        case 1014: snfmt(buf, kBufSize, "dcbz    r%u, r%u", RA(code), RB(code)); return buf;
        case 982: snfmt(buf, kBufSize, "icbi    r%u, r%u", RA(code), RB(code)); return buf;

        default:
          break;
      }
      snfmt(buf, kBufSize, ".long   0x%08X  ; op31 xo10=%u xo9=%u", code, xo10, xo9);
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 32: lwz
    // -----------------------------------------------------------------------
    case 32: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lwz     r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 33: lwzu
    // -----------------------------------------------------------------------
    case 33: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lwzu    r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 34: lbz
    // -----------------------------------------------------------------------
    case 34: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lbz     r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 35: lbzu
    // -----------------------------------------------------------------------
    case 35: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lbzu    r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 36: stw
    // -----------------------------------------------------------------------
    case 36: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stw     r%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 37: stwu
    // -----------------------------------------------------------------------
    case 37: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stwu    r%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 38: stb
    // -----------------------------------------------------------------------
    case 38: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stb     r%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 39: stbu
    // -----------------------------------------------------------------------
    case 39: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stbu    r%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 40: lhz
    // -----------------------------------------------------------------------
    case 40: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lhz     r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 41: lhzu
    // -----------------------------------------------------------------------
    case 41: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lhzu    r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 42: lha
    // -----------------------------------------------------------------------
    case 42: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lha     r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 44: sth
    // -----------------------------------------------------------------------
    case 44: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "sth     r%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 45: sthu
    // -----------------------------------------------------------------------
    case 45: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "sthu    r%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 46: lmw
    // -----------------------------------------------------------------------
    case 46: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lmw     r%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 47: stmw
    // -----------------------------------------------------------------------
    case 47: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stmw    r%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 48: lfs
    // -----------------------------------------------------------------------
    case 48: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lfs     fr%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 49: lfsu
    // -----------------------------------------------------------------------
    case 49: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lfsu    fr%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 50: lfd
    // -----------------------------------------------------------------------
    case 50: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lfd     fr%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 51: lfdu
    // -----------------------------------------------------------------------
    case 51: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "lfdu    fr%u, 0x%X(r%u)", RD(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 52: stfs
    // -----------------------------------------------------------------------
    case 52: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stfs    fr%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 53: stfsu
    // -----------------------------------------------------------------------
    case 53: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stfsu   fr%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 54: stfd
    // -----------------------------------------------------------------------
    case 54: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stfd    fr%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 55: stfdu
    // -----------------------------------------------------------------------
    case 55: {
      int16_t d = SIMM(code);
      snfmt(buf, kBufSize, "stfdu   fr%u, 0x%X(r%u)", RS(code), (uint16_t)d, RA(code));
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 58: ld / ldu / lwa (DS-form)
    // -----------------------------------------------------------------------
    case 58: {
      uint32_t ds = (code & 0xFFFC);
      int16_t offset = (int16_t)(int32_t)ds; // sign-extend the 16-bit
      // Actually DS-form: offset = bits 16-29 << 2, sub-opcode = bits 30-31
      uint32_t sub = code & 3;
      int32_t disp = SignExt((code >> 2) & 0x3FFF, 14) << 2;
      switch (sub) {
        case 0: snfmt(buf, kBufSize, "ld      r%u, %d(r%u)", RD(code), disp, RA(code)); break;
        case 1: snfmt(buf, kBufSize, "ldu     r%u, %d(r%u)", RD(code), disp, RA(code)); break;
        case 2: snfmt(buf, kBufSize, "lwa     r%u, %d(r%u)", RD(code), disp, RA(code)); break;
        default: snfmt(buf, kBufSize, ".long   0x%08X  ; op58 sub=%u", code, sub); break;
      }
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 59: single-precision FP (A-form)
    // -----------------------------------------------------------------------
    case 59: {
      uint32_t axo = AFormXO(code);
      switch (axo) {
        case 21: { // fadds
          auto mn = FormatRcSuffix("fadds", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRB(code));
          return buf;
        }
        case 20: { // fsubs
          auto mn = FormatRcSuffix("fsubs", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRB(code));
          return buf;
        }
        case 25: { // fmuls
          auto mn = FormatRcSuffix("fmuls", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code));
          return buf;
        }
        case 18: { // fdivs
          auto mn = FormatRcSuffix("fdivs", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRB(code));
          return buf;
        }
        case 29: { // fmadds
          auto mn = FormatRcSuffix("fmadds", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 28: { // fmsubs
          auto mn = FormatRcSuffix("fmsubs", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 31: { // fnmadds
          auto mn = FormatRcSuffix("fnmadds", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 30: { // fnmsubs
          auto mn = FormatRcSuffix("fnmsubs", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 24: { // fres
          auto mn = FormatRcSuffix("fres", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        default:
          break;
      }
      // X-form FP under opcode 59
      uint32_t fxo = XO_10(code);
      switch (fxo) {
        case 12: { // frsp -- actually under opcode 63
          break;
        }
        default:
          break;
      }
      snfmt(buf, kBufSize, ".long   0x%08X  ; op59 axo=%u", code, axo);
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 62: std / stdu (DS-form)
    // -----------------------------------------------------------------------
    case 62: {
      uint32_t sub = code & 3;
      int32_t disp = SignExt((code >> 2) & 0x3FFF, 14) << 2;
      switch (sub) {
        case 0: snfmt(buf, kBufSize, "std     r%u, %d(r%u)", RS(code), disp, RA(code)); break;
        case 1: snfmt(buf, kBufSize, "stdu    r%u, %d(r%u)", RS(code), disp, RA(code)); break;
        default: snfmt(buf, kBufSize, ".long   0x%08X  ; op62 sub=%u", code, sub); break;
      }
      return buf;
    }

    // -----------------------------------------------------------------------
    // Opcode 63: double-precision FP (A-form and X-form)
    // -----------------------------------------------------------------------
    case 63: {
      // Try A-form first (bits 26-30)
      uint32_t axo = AFormXO(code);
      switch (axo) {
        case 21: { // fadd
          auto mn = FormatRcSuffix("fadd", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRB(code));
          return buf;
        }
        case 20: { // fsub
          auto mn = FormatRcSuffix("fsub", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRB(code));
          return buf;
        }
        case 25: { // fmul
          auto mn = FormatRcSuffix("fmul", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code));
          return buf;
        }
        case 18: { // fdiv
          auto mn = FormatRcSuffix("fdiv", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRB(code));
          return buf;
        }
        case 29: { // fmadd
          auto mn = FormatRcSuffix("fmadd", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 28: { // fmsub
          auto mn = FormatRcSuffix("fmsub", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 31: { // fnmadd
          auto mn = FormatRcSuffix("fnmadd", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 30: { // fnmsub
          auto mn = FormatRcSuffix("fnmsub", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 23: { // fsel
          auto mn = FormatRcSuffix("fsel", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u, fr%u, fr%u", mn.c_str(), FRD(code), FRA(code), FRC(code), FRB(code));
          return buf;
        }
        case 26: { // frsqrte
          auto mn = FormatRcSuffix("frsqrte", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        default:
          break;
      }

      // X-form (bits 21-30)
      uint32_t fxo = XO_10(code);
      switch (fxo) {
        case 0: { // fcmpu
          snfmt(buf, kBufSize, "fcmpu   cr%u, fr%u, fr%u", CRFD(code), FRA(code), FRB(code));
          return buf;
        }
        case 32: { // fcmpo
          snfmt(buf, kBufSize, "fcmpo   cr%u, fr%u, fr%u", CRFD(code), FRA(code), FRB(code));
          return buf;
        }
        case 72: { // fmr
          auto mn = FormatRcSuffix("fmr", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 264: { // fabs
          auto mn = FormatRcSuffix("fabs", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 40: { // fneg
          auto mn = FormatRcSuffix("fneg", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 136: { // fnabs
          auto mn = FormatRcSuffix("fnabs", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 12: { // frsp
          auto mn = FormatRcSuffix("frsp", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 14: { // fctiwz
          auto mn = FormatRcSuffix("fctiwz", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 15: { // fctiwz (alternate encoding)
          auto mn = FormatRcSuffix("fctiwz", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 846: { // fcfid
          auto mn = FormatRcSuffix("fcfid", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 814: { // fctid
          auto mn = FormatRcSuffix("fctid", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        case 815: { // fctidz
          auto mn = FormatRcSuffix("fctidz", code);
          snfmt(buf, kBufSize, "%-8sfr%u, fr%u", mn.c_str(), FRD(code), FRB(code));
          return buf;
        }
        default:
          break;
      }
      snfmt(buf, kBufSize, ".long   0x%08X  ; op63 axo=%u fxo=%u", code, axo, fxo);
      return buf;
    }

    default:
      break;
  }

  snfmt(buf, kBufSize, ".long   0x%08X", code);
  return buf;
}

std::vector<DisasmLine> DisasmBuffer(const uint8_t* data, size_t len,
                                     uint32_t base_addr) {
  std::vector<DisasmLine> lines;
  lines.reserve(len / 4);

  for (size_t i = 0; i + 3 < len; i += 4) {
    uint32_t code = ((uint32_t)data[i] << 24) | ((uint32_t)data[i + 1] << 16) |
                    ((uint32_t)data[i + 2] << 8) | (uint32_t)data[i + 3];
    uint32_t addr = base_addr + (uint32_t)i;
    DisasmLine line;
    line.address = addr;
    line.code = code;
    line.text = DisasmInstruction(addr, code);
    lines.push_back(std::move(line));
  }
  return lines;
}
