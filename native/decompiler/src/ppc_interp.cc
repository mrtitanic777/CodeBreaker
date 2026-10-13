// XEXMagic - PowerPC interpreter. See ppc_interp.h.

#include "ppc_interp.h"

#include "kernel_exports.h"

#include <cstring>

namespace {

constexpr uint32_t kPageSize = 0x1000;
constexpr uint32_t kRetSentinel = 0xDEADF00C;  // LR value that ends the run

// Sparse memory: pages are faulted in from the image on first touch and are
// zero when the image has no bytes there (BSS, stack, scratch).
class Mem {
 public:
  Mem(InterpImageReader r, void* c) : reader_(r), ctx_(c) {}

  uint8_t R8(uint32_t va) {
    auto& p = Page(va & ~(kPageSize - 1));
    return p[va & (kPageSize - 1)];
  }
  void W8(uint32_t va, uint8_t v) {
    auto& p = Page(va & ~(kPageSize - 1));
    // Every store in the interpreter funnels through here, so one check
    // catches stbx/sthx/stwx/memset-style writes alike -- which is the point:
    // the interesting writer is usually the one you did NOT predict.
    if (watch_hits && va >= watch_lo && va <= watch_hi &&
        watch_hits->size() < 2048) {
      InterpMemHit h;
      h.pc = *cur_pc;
      h.addr = va;
      h.old_value = p[va & (kPageSize - 1)];
      h.new_value = v;
      h.step = *cur_step;
      watch_hits->push_back(h);
    }
    p[va & (kPageSize - 1)] = v;
  }

  // Wiring for the write watch. Set together or not at all.
  uint32_t watch_lo = 0, watch_hi = 0;
  std::vector<InterpMemHit>* watch_hits = nullptr;
  const uint32_t* cur_pc = nullptr;
  const uint64_t* cur_step = nullptr;
  uint32_t R32(uint32_t va) {
    return ((uint32_t)R8(va) << 24) | ((uint32_t)R8(va + 1) << 16) |
           ((uint32_t)R8(va + 2) << 8) | (uint32_t)R8(va + 3);
  }
  uint16_t R16(uint32_t va) {
    return (uint16_t)(((uint32_t)R8(va) << 8) | R8(va + 1));
  }
  void W32(uint32_t va, uint32_t v) {
    W8(va, (uint8_t)(v >> 24));
    W8(va + 1, (uint8_t)(v >> 16));
    W8(va + 2, (uint8_t)(v >> 8));
    W8(va + 3, (uint8_t)v);
  }
  void W16(uint32_t va, uint16_t v) {
    W8(va, (uint8_t)(v >> 8));
    W8(va + 1, (uint8_t)v);
  }

 private:
  std::vector<uint8_t>& Page(uint32_t base) {
    auto it = pages_.find(base);
    if (it != pages_.end()) return it->second;
    std::vector<uint8_t> p(kPageSize, 0);
    for (uint32_t i = 0; i < kPageSize; i++) {
      int b = reader_(ctx_, base + i);
      if (b >= 0) p[i] = (uint8_t)b;
    }
    return pages_.emplace(base, std::move(p)).first->second;
  }
  std::map<uint32_t, std::vector<uint8_t>> pages_;
  InterpImageReader reader_;
  void* ctx_;
};

struct State {
  uint32_t gpr[32] = {};
  // High words of 64-bit values, kept only for registers a `ld` loaded, so
  // ld/std round-trips a full doubleword on a 32-bit GPR model. See case 58.
  uint32_t gpr_hi[32] = {};
  bool hi_valid[32] = {};
  uint32_t lr = 0, ctr = 0, xer_ca = 0;
  uint32_t msr = 0;
  double fpr[32] = {};
  uint8_t cr[8] = {};        // per-field: bit0 LT, bit1 GT, bit2 EQ
  uint32_t pc = 0;
};

// Field bits [first..last] in PowerPC numbering, where bit 0 is the MSB and
// `first <= last`. The width is last-first+1; getting that backwards yields a
// negative shift count and silently returns garbage.
inline uint32_t Bits(uint32_t w, int first, int last) {
  return (w >> (31 - last)) & ((1u << (last - first + 1)) - 1);
}
inline uint32_t OP(uint32_t w) { return w >> 26; }
inline uint32_t RD(uint32_t w) { return (w >> 21) & 31; }
inline uint32_t RA(uint32_t w) { return (w >> 16) & 31; }
inline uint32_t RB(uint32_t w) { return (w >> 11) & 31; }
inline int32_t SIMM(uint32_t w) { return (int16_t)(uint16_t)(w & 0xFFFF); }
inline uint32_t UIMM(uint32_t w) { return w & 0xFFFF; }
inline uint32_t XO(uint32_t w) { return (w >> 1) & 0x3FF; }

uint32_t Rotl(uint32_t v, uint32_t n) {
  n &= 31;
  return n ? ((v << n) | (v >> (32 - n))) : v;
}

uint32_t MaskMbMe(uint32_t mb, uint32_t me) {
  uint32_t m;
  if (mb <= me) {
    m = 0;
    for (uint32_t i = mb; i <= me; i++) m |= 0x80000000u >> i;
  } else {
    m = 0xFFFFFFFFu;
    for (uint32_t i = me + 1; i < mb; i++) m &= ~(0x80000000u >> i);
  }
  return m;
}

void SetCR(State& s, int field, int32_t a, int32_t b) {
  s.cr[field] = (uint8_t)(a < b ? 1 : (a > b ? 2 : 4));
}
void SetCRU(State& s, int field, uint32_t a, uint32_t b) {
  s.cr[field] = (uint8_t)(a < b ? 1 : (a > b ? 2 : 4));
}
void SetCR0(State& s, uint32_t v) { SetCR(s, 0, (int32_t)v, 0); }

// Snapshot the parameter registers at the moment of a call. Captured here
// rather than reconstructed later because the callee overwrites r3 almost
// immediately, so after the fact the arguments are already gone.
InterpCall MakeCall(const State& s, uint32_t target, uint32_t lr) {
  InterpCall c;
  c.target = target;
  c.lr = lr;
  for (int i = 0; i < 8; i++) c.arg[i] = s.gpr[3 + i];
  return c;
}

}  // namespace

InterpResult PPCInterpRun(const InterpRequest& req, InterpImageReader reader,
                          void* reader_ctx) {
  InterpResult res;
  Mem mem(reader, reader_ctx);
  State s;

  for (const auto& w : req.writes)
    for (size_t i = 0; i < w.data.size(); i++)
      mem.W8(w.addr + (uint32_t)i, w.data[i]);

  for (size_t i = 0; i < req.args.size() && i < 8; i++)
    s.gpr[3 + i] = req.args[i];
  s.gpr[1] = req.stack_top;            // stack pointer
  s.gpr[13] = 0x10000000;              // TLS-ish base; reads land on zero pages
  s.lr = kRetSentinel;
  s.pc = req.entry;

  uint64_t steps = 0;

  // Armed after the initial `writes` are applied, so seeding the input buffer
  // does not show up as a hit.
  if (req.watch_mem_lo || req.watch_mem_hi) {
    mem.watch_lo = req.watch_mem_lo;
    mem.watch_hi = req.watch_mem_hi ? req.watch_mem_hi : req.watch_mem_lo;
    mem.watch_hits = &res.mem_watch;
    mem.cur_pc = &s.pc;
    mem.cur_step = &steps;
  }

  while (steps++ < req.max_steps) {
    if (s.pc == kRetSentinel) {
      res.ok = true;
      break;
    }
    auto stub = req.stubs.find(s.pc);
    if (stub != req.stubs.end()) {       // stubbed call: return a value
      s.gpr[3] = stub->second;
      s.pc = s.lr;
      continue;
    }
    // XEX import thunk left unpatched in a raw PE. Recognised in place rather
    // than from an import table, so this works on an image with no XEX header.
    // See kernel_exports.h.
    {
      uint32_t w0 = mem.R32(s.pc);
      if ((w0 >> 24) == 0x01 &&
          xexmagic::IsImportThunk(w0, mem.R32(s.pc + 4), mem.R32(s.pc + 8),
                                  mem.R32(s.pc + 12))) {
        InterpKernelCall kc;
        kc.thunk = s.pc;
        kc.ordinal = w0 & 0xFFFF;
        kc.name = xexmagic::KernelExportName(kc.ordinal);
        for (int i = 0; i < 4; i++) kc.arg[i] = s.gpr[3 + i];
        xexmagic::KernelMem km;
        km.read8 = [&mem](uint32_t a) { return mem.R8(a); };
        km.write8 = [&mem](uint32_t a, uint8_t v) { mem.W8(a, v); };
        auto kr = xexmagic::KernelInvoke(kc.ordinal, s.gpr, km);
        kc.implemented = kr.implemented;
        kc.r3 = kr.r3;
        if (res.kernel_calls.size() < 4096) res.kernel_calls.push_back(kc);
        s.gpr[3] = kr.r3;
        s.pc = s.lr;
        continue;
      }
    }
    auto intr = req.intrinsics.find(s.pc);
    if (intr != req.intrinsics.end()) {  // XDK memory routine: do it natively
      uint32_t d = s.gpr[3], b = s.gpr[4], n = s.gpr[5];
      if (intr->second == InterpRequest::kMemset) {
        for (uint32_t i = 0; i < n; i++) mem.W8(d + i, (uint8_t)b);
      } else if (n && d > b && d < b + n) {          // overlapping: copy back
        for (uint32_t i = n; i-- > 0;) mem.W8(d + i, mem.R8(b + i));
      } else {
        for (uint32_t i = 0; i < n; i++) mem.W8(d + i, mem.R8(b + i));
      }
      s.gpr[3] = d;                      // all three return the destination
      s.pc = s.lr;
      continue;
    }
    uint32_t w = mem.R32(s.pc);
    uint32_t next = s.pc + 4;
    res.last_pc = s.pc;
    if (res.trace.size() < req.trace_steps && steps >= req.trace_from &&
        (!req.trace_pc_lo ||
         (s.pc >= req.trace_pc_lo &&
          (!req.trace_pc_hi || s.pc <= req.trace_pc_hi)))) {
      InterpTrace t;
      t.pc = s.pc;
      t.word = w;
      memcpy(t.gpr, s.gpr, sizeof(t.gpr));
      res.trace.push_back(t);
    }

    // Sampled before the instruction executes so the comparison below sees the
    // value this instruction started from.
    const uint32_t watch_before =
        (req.watch_gpr >= 0 && req.watch_gpr < 32) ? s.gpr[req.watch_gpr] : 0;

    switch (OP(w)) {
      case 14: s.gpr[RD(w)] = (RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w); break; // addi
      case 15: s.gpr[RD(w)] = (RA(w) ? s.gpr[RA(w)] : 0) + (SIMM(w) << 16); break; // addis
      case 24: s.gpr[RA(w)] = s.gpr[RD(w)] | UIMM(w); break;               // ori
      case 25: s.gpr[RA(w)] = s.gpr[RD(w)] | (UIMM(w) << 16); break;       // oris
      case 26: s.gpr[RA(w)] = s.gpr[RD(w)] ^ UIMM(w); break;               // xori
      case 27: s.gpr[RA(w)] = s.gpr[RD(w)] ^ (UIMM(w) << 16); break;       // xoris
      case 28: s.gpr[RA(w)] = s.gpr[RD(w)] & UIMM(w); SetCR0(s, s.gpr[RA(w)]); break;
      case 29: s.gpr[RA(w)] = s.gpr[RD(w)] & (UIMM(w) << 16); SetCR0(s, s.gpr[RA(w)]); break;
      case 12: case 13: {                                                  // addic(.)
        uint32_t a = s.gpr[RA(w)];
        uint64_t r = (uint64_t)a + (uint64_t)(int64_t)SIMM(w);
        s.gpr[RD(w)] = (uint32_t)r;
        s.xer_ca = (r >> 32) & 1;
        if (OP(w) == 13) SetCR0(s, s.gpr[RD(w)]);
        break;
      }
      // twi: conditional trap. The compiler emits these as bounds/NULL asserts
      // all through normal code, so treating one as unimplemented halts a run
      // that the hardware would have sailed straight through. Evaluate the
      // condition and only stop when the trap would actually have fired --
      // that way a genuine assert still surfaces instead of being hidden.
      case 3: {
        int32_t a = (int32_t)s.gpr[RA(w)];
        int32_t b = (int32_t)SIMM(w);
        uint32_t to = RD(w);
        bool fire = ((to & 0x10) && a < b) ||
                    ((to & 0x08) && a > b) ||
                    ((to & 0x04) && a == b) ||
                    ((to & 0x02) && (uint32_t)a < (uint32_t)b) ||
                    ((to & 0x01) && (uint32_t)a > (uint32_t)b);
        if (fire) {
          res.error = "twi trap fired";
          res.last_pc = s.pc;
          res.steps = steps;
          goto finish;
        }
        break;
      }
      case 7: s.gpr[RD(w)] = s.gpr[RA(w)] * (uint32_t)SIMM(w); break;               // mulli
      case 8: {                                                                     // subfic
        uint64_t r = (uint64_t)(uint32_t)SIMM(w) + (uint64_t)(uint32_t)(~s.gpr[RA(w)]) + 1;
        s.gpr[RD(w)] = (uint32_t)r;
        s.xer_ca = (r >> 32) & 1;
        break;
      }
      case 11: SetCR(s, (int)(RD(w) >> 2), (int32_t)s.gpr[RA(w)], SIMM(w)); break;  // cmpi
      case 10: SetCRU(s, (int)(RD(w) >> 2), s.gpr[RA(w)], UIMM(w)); break;          // cmpli
      case 32: s.gpr[RD(w)] = mem.R32((RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w)); break; // lwz
      case 33: s.gpr[RA(w)] = s.gpr[RA(w)] + SIMM(w); s.gpr[RD(w)] = mem.R32(s.gpr[RA(w)]); break;
      case 34: s.gpr[RD(w)] = mem.R8((RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w)); break;  // lbz
      case 35: s.gpr[RA(w)] = s.gpr[RA(w)] + SIMM(w); s.gpr[RD(w)] = mem.R8(s.gpr[RA(w)]); break;
      case 36: mem.W32((RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w), s.gpr[RD(w)]); break;  // stw
      case 37: s.gpr[RA(w)] = s.gpr[RA(w)] + SIMM(w); mem.W32(s.gpr[RA(w)], s.gpr[RD(w)]); break;
      case 38: mem.W8((RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w), (uint8_t)s.gpr[RD(w)]); break;
      case 39: s.gpr[RA(w)] = s.gpr[RA(w)] + SIMM(w); mem.W8(s.gpr[RA(w)], (uint8_t)s.gpr[RD(w)]); break;
      case 40: s.gpr[RD(w)] = mem.R16((RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w)); break; // lhz
      case 42: {                                                                     // lha
        uint16_t v = mem.R16((RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w));
        s.gpr[RD(w)] = (uint32_t)(int32_t)(int16_t)v;
        break;
      }
      case 44: mem.W16((RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w), (uint16_t)s.gpr[RD(w)]); break;
      // 64-bit forms. GPRs are modelled 32-bit, so `ld` keeps the high word in
      // a shadow (gpr_hi) that `std` writes back.
      //
      // This used to drop the high word on load and store a hard ZERO, with a
      // comment claiming that was fine "for the pointers and counters this is
      // used for". It is not: the guest memcpy at 0x8256D358 copies with
      // ld/std, so every 8-byte block came out with its first four bytes
      // blanked. Nothing errored — the interpreter just produced wrong memory,
      // and the code under test then took a failure path that looked like a
      // genuine result. A fastfile header parsed through it reported "corrupt
      // or unreadable" for a file that was in fact byte-correct.
      //
      // The shadow is set by `ld` and consumed by `std`. It is only trusted for
      // a register that `ld` actually loaded (hi_valid), so a register built by
      // 32-bit arithmetic still stores a zero high word, which is right for a
      // 32-bit model. Load-then-store — memcpy, register save/restore — is the
      // case that matters and it now round-trips all eight bytes.
      case 58: {                                                                     // ld/ldu/lwa
        int32_t d = (int32_t)(int16_t)(uint16_t)(w & 0xFFFC);
        uint32_t ea = (RA(w) ? s.gpr[RA(w)] : 0) + d;
        if ((w & 3) == 2) {                                                          // lwa
          s.gpr[RD(w)] = mem.R32(ea);
          s.hi_valid[RD(w)] = false;
        } else {
          s.gpr_hi[RD(w)] = mem.R32(ea);                                             // high word
          s.hi_valid[RD(w)] = true;
          s.gpr[RD(w)] = mem.R32(ea + 4);                                            // low word
          if ((w & 3) == 1) s.gpr[RA(w)] = ea;
        }
        break;
      }
      case 62: {                                                                     // std/stdu
        int32_t d = (int32_t)(int16_t)(uint16_t)(w & 0xFFFC);
        uint32_t ea = (RA(w) ? s.gpr[RA(w)] : 0) + d;
        mem.W32(ea, s.hi_valid[RD(w)] ? s.gpr_hi[RD(w)] : 0);
        mem.W32(ea + 4, s.gpr[RD(w)]);
        if ((w & 3) == 1) s.gpr[RA(w)] = ea;
        break;
      }
      case 20: {                                                                     // rlwimi
        uint32_t m = MaskMbMe(Bits(w, 21, 25), Bits(w, 26, 30));
        s.gpr[RA(w)] = (Rotl(s.gpr[RD(w)], Bits(w, 16, 20)) & m) | (s.gpr[RA(w)] & ~m);
        if (w & 1) SetCR0(s, s.gpr[RA(w)]);
        break;
      }
      // 64-bit rotate-and-mask (MD/MDS form): rldicl, rldicr, rldic, rldimi,
      // rldcl, rldcr. Evaluated as a real 64-bit operation against the gpr_hi
      // shadow, then split back. Compilers emit `clrldi rX,rY,32` (rldicl with
      // sh=0, mb=32) constantly just to zero-extend, so leaving this
      // unimplemented stops any run that reaches ordinary 64-bit pointer code.
      case 30: {
        uint64_t v = ((uint64_t)(s.hi_valid[RD(w)] ? s.gpr_hi[RD(w)] : 0) << 32) |
                     s.gpr[RD(w)];
        uint32_t xo6 = (w >> 1) & 0xF;
        uint32_t sh = ((w >> 11) & 31) | (((w >> 1) & 1) << 5);
        uint32_t fld = ((w >> 6) & 31) | (((w >> 5) & 1) << 5);
        bool mds = (xo6 == 8 || xo6 == 9);                     // rldcl / rldcr
        if (mds) sh = s.gpr[RB(w)] & 63;
        uint64_t r = sh ? ((v << sh) | (v >> (64 - sh))) : v;
        auto ones = [](uint32_t hi_bit, uint32_t lo_bit) -> uint64_t {
          // PowerPC bit 0 is the MSB; inclusive range.
          uint64_t m = 0;
          for (uint32_t i = hi_bit; i <= lo_bit && i < 64; i++)
            m |= (uint64_t)1 << (63 - i);
          return m;
        };
        uint32_t op4 = (w >> 2) & 7;
        uint64_t mask, out;
        if (xo6 == 8) {                                        // rldcl: mb..63
          mask = ones(fld, 63); out = r & mask;
        } else if (xo6 == 9) {                                 // rldcr: 0..me
          mask = ones(0, fld); out = r & mask;
        } else if (op4 == 0) {                                 // rldicl: mb..63
          mask = ones(fld, 63); out = r & mask;
        } else if (op4 == 1) {                                 // rldicr: 0..me
          mask = ones(0, fld); out = r & mask;
        } else if (op4 == 2) {                                 // rldic: mb..63-sh
          mask = ones(fld, 63 - sh); out = r & mask;
        } else {                                               // rldimi
          mask = ones(fld, 63 - sh);
          uint64_t a = ((uint64_t)(s.hi_valid[RA(w)] ? s.gpr_hi[RA(w)] : 0) << 32) |
                       s.gpr[RA(w)];
          out = (r & mask) | (a & ~mask);
        }
        s.gpr[RA(w)] = (uint32_t)out;
        s.gpr_hi[RA(w)] = (uint32_t)(out >> 32);
        s.hi_valid[RA(w)] = true;
        if (w & 1) SetCR0(s, s.gpr[RA(w)]);
        break;
      }
      // Floating point. Enough to get through prologues (which save f31 with
      // stfd) and the size/percentage arithmetic the DB uses for its progress
      // and error messages. Guest FP is IEEE double, which maps straight onto
      // the host.
      case 48: case 49: {                                                            // lfs/lfsu
        uint32_t ea = (RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w);
        uint32_t b = mem.R32(ea);
        float f; std::memcpy(&f, &b, 4);
        s.fpr[RD(w)] = (double)f;
        if (OP(w) == 49) s.gpr[RA(w)] = ea;
        break;
      }
      case 50: case 51: {                                                            // lfd/lfdu
        uint32_t ea = (RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w);
        uint64_t b = ((uint64_t)mem.R32(ea) << 32) | mem.R32(ea + 4);
        std::memcpy(&s.fpr[RD(w)], &b, 8);
        if (OP(w) == 51) s.gpr[RA(w)] = ea;
        break;
      }
      case 52: case 53: {                                                            // stfs/stfsu
        uint32_t ea = (RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w);
        float f = (float)s.fpr[RD(w)];
        uint32_t b; std::memcpy(&b, &f, 4);
        mem.W32(ea, b);
        if (OP(w) == 53) s.gpr[RA(w)] = ea;
        break;
      }
      case 54: case 55: {                                                            // stfd/stfdu
        uint32_t ea = (RA(w) ? s.gpr[RA(w)] : 0) + SIMM(w);
        uint64_t b; std::memcpy(&b, &s.fpr[RD(w)], 8);
        mem.W32(ea, (uint32_t)(b >> 32));
        mem.W32(ea + 4, (uint32_t)b);
        if (OP(w) == 55) s.gpr[RA(w)] = ea;
        break;
      }
      case 59: case 63: {                                                            // FP arithmetic
        uint32_t xo5 = (w >> 1) & 0x1F;
        double a = s.fpr[RA(w)], b = s.fpr[RB(w)], c = s.fpr[(w >> 6) & 31];
        switch (xo5) {
          case 18: s.fpr[RD(w)] = b != 0.0 ? a / b : 0.0; break;   // fdiv
          case 20: s.fpr[RD(w)] = a - b; break;                    // fsub
          case 21: s.fpr[RD(w)] = a + b; break;                    // fadd
          case 25: s.fpr[RD(w)] = a * c; break;                    // fmul
          case 28: s.fpr[RD(w)] = c * a - b; break;                // fmsub
          case 29: s.fpr[RD(w)] = c * a + b; break;                // fmadd
          default: {
            uint32_t xo10 = (w >> 1) & 0x3FF;
            if (xo10 == 72) s.fpr[RD(w)] = b;                      // fmr
            else if (xo10 == 264) s.fpr[RD(w)] = b < 0 ? -b : b;   // fabs
            else if (xo10 == 40) s.fpr[RD(w)] = -b;                // fneg
            else if (xo10 == 0 || xo10 == 32) {                    // fcmpu/fcmpo
              int f = (int)(RD(w) >> 2);
              s.cr[f] = (uint8_t)(a < b ? 1 : (a > b ? 2 : 4));
            } else if (xo10 == 15 || xo10 == 14) {                 // fctiw(z)
              uint64_t iv = (uint64_t)(int64_t)b;
              std::memcpy(&s.fpr[RD(w)], &iv, 8);
            }
            break;
          }
        }
        break;
      }
      // Traps. Compilers emit tdi/twi right after a divide to fault on a zero
      // divisor. Nothing here models exceptions, and the divide already returns
      // 0 for that case, so the check is a no-op rather than a failure.
      case 2: break;                                                                 // tdi
      case 21: {                                                                     // rlwinm
        uint32_t m = MaskMbMe(Bits(w, 21, 25), Bits(w, 26, 30));
        s.gpr[RA(w)] = Rotl(s.gpr[RD(w)], Bits(w, 16, 20)) & m;
        if (w & 1) SetCR0(s, s.gpr[RA(w)]);
        break;
      }
      case 23: {                                                                     // rlwnm
        uint32_t m = MaskMbMe(Bits(w, 21, 25), Bits(w, 26, 30));
        s.gpr[RA(w)] = Rotl(s.gpr[RD(w)], s.gpr[RB(w)] & 31) & m;
        if (w & 1) SetCR0(s, s.gpr[RA(w)]);
        break;
      }
      case 18: {                                                                     // b/bl
        int32_t li = (int32_t)((w & 0x03FFFFFCu) << 6) >> 6;
        uint32_t tgt = (w & 2) ? (uint32_t)li : s.pc + (uint32_t)li;
        if (w & 1) {
          s.lr = next;
          if (req.trace_calls) res.calls.push_back(MakeCall(s, tgt, next));
        }
        next = tgt;
        break;
      }
      case 16: {                                                                     // bc
        uint32_t bo = RD(w), bi = RA(w);
        bool ctr_ok = true, cond_ok = true;
        if (!(bo & 4)) { s.ctr--; ctr_ok = (bo & 2) ? (s.ctr == 0) : (s.ctr != 0); }
        if (!(bo & 16)) {
          bool bit = (s.cr[bi >> 2] >> (3 - (bi & 3))) & 1;
          // cr field bits: LT=0x1 GT=0x2 EQ=0x4 in our packing
          uint8_t f = s.cr[bi >> 2];
          switch (bi & 3) {
            case 0: bit = (f & 1) != 0; break;   // LT
            case 1: bit = (f & 2) != 0; break;   // GT
            case 2: bit = (f & 4) != 0; break;   // EQ
            default: bit = false; break;         // SO
          }
          cond_ok = ((bo & 8) != 0) == bit;
        }
        if (ctr_ok && cond_ok) {
          int32_t bd = (int32_t)(int16_t)(uint16_t)(w & 0xFFFC);
          next = (w & 2) ? (uint32_t)bd : s.pc + (uint32_t)bd;
          if (w & 1) s.lr = s.pc + 4;
        }
        break;
      }
      case 19: {                                                                     // bclr/bcctr
        uint32_t xo = XO(w), bo = RD(w), bi = RA(w);
        bool cond_ok = true;
        if (!(bo & 16)) {
          uint8_t f = s.cr[bi >> 2];
          bool bit = false;
          switch (bi & 3) {
            case 0: bit = (f & 1) != 0; break;
            case 1: bit = (f & 2) != 0; break;
            case 2: bit = (f & 4) != 0; break;
            default: break;
          }
          cond_ok = ((bo & 8) != 0) == bit;
        }
        if (xo == 16) {                       // bclr
          if (cond_ok) { uint32_t t = s.lr; if (w & 1) s.lr = next; next = t; }
        } else if (xo == 528) {               // bcctr
          if (cond_ok) {
            if (w & 1) {
              s.lr = next;
              if (req.trace_calls) res.calls.push_back(MakeCall(s, s.ctr, next));
            }
            next = s.ctr;
          }
        } else if (xo == 150 || xo == 274 || xo == 0 || xo == 33 || xo == 129 ||
                   xo == 193 || xo == 225 || xo == 257 || xo == 289 || xo == 417 ||
                   xo == 449) {
          // isync / CR logical ops: no effect on the state we model
        } else {
          res.error = "unimplemented op19 xo=" + std::to_string(xo);
          res.last_pc = s.pc;
          res.steps = steps;
          goto finish;
        }
        break;
      }
      case 31: {
        uint32_t xo = XO(w);
        switch (xo) {
          case 266: case 266 + 512: s.gpr[RD(w)] = s.gpr[RA(w)] + s.gpr[RB(w)];
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;                       // add
          case 40: case 40 + 512: s.gpr[RD(w)] = s.gpr[RB(w)] - s.gpr[RA(w)];
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;                       // subf
          case 8: {                                                          // subfc
            uint64_t r = (uint64_t)s.gpr[RB(w)] + (uint64_t)(uint32_t)(~s.gpr[RA(w)]) + 1;
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 10: {                                                         // addc
            uint64_t r = (uint64_t)s.gpr[RA(w)] + (uint64_t)s.gpr[RB(w)];
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 138: {                                                        // adde
            uint64_t r = (uint64_t)s.gpr[RA(w)] + (uint64_t)s.gpr[RB(w)] + s.xer_ca;
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 136: {                                                        // subfe
            uint64_t r = (uint64_t)(uint32_t)(~s.gpr[RA(w)]) + (uint64_t)s.gpr[RB(w)] + s.xer_ca;
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 104: s.gpr[RD(w)] = 0u - s.gpr[RA(w)];
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;                       // neg
          case 202: {                                                        // addze
            uint64_t r = (uint64_t)s.gpr[RA(w)] + s.xer_ca;
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 234: {                                                        // addme
            uint64_t r = (uint64_t)s.gpr[RA(w)] + 0xFFFFFFFFull + s.xer_ca;
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 200: {                                                        // subfze
            uint64_t r = (uint64_t)(uint32_t)(~s.gpr[RA(w)]) + s.xer_ca;
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 232: {                                                        // subfme
            uint64_t r = (uint64_t)(uint32_t)(~s.gpr[RA(w)]) + 0xFFFFFFFFull + s.xer_ca;
            s.gpr[RD(w)] = (uint32_t)r; s.xer_ca = (r >> 32) & 1;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 235: case 235 + 512: s.gpr[RD(w)] = s.gpr[RA(w)] * s.gpr[RB(w)];
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;                       // mullw
          // High word of a 32x32 multiply. The compiler emits these for the
          // strength-reduced division in string/hash code, so a parser run
          // walks straight into one.
          case 75: {                                                         // mulhw
            int64_t p = (int64_t)(int32_t)s.gpr[RA(w)] *
                        (int64_t)(int32_t)s.gpr[RB(w)];
            s.gpr[RD(w)] = (uint32_t)((uint64_t)p >> 32);
            if (w & 1) SetCR0(s, s.gpr[RD(w)]);
            break;
          }
          case 11: {                                                         // mulhwu
            uint64_t p = (uint64_t)s.gpr[RA(w)] * (uint64_t)s.gpr[RB(w)];
            s.gpr[RD(w)] = (uint32_t)(p >> 32);
            if (w & 1) SetCR0(s, s.gpr[RD(w)]);
            break;
          }
          case 491: {                                                        // divw
            int32_t a = (int32_t)s.gpr[RA(w)], b = (int32_t)s.gpr[RB(w)];
            s.gpr[RD(w)] = b ? (uint32_t)(a / b) : 0;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 459: {                                                        // divwu
            uint32_t a = s.gpr[RA(w)], b = s.gpr[RB(w)];
            s.gpr[RD(w)] = b ? a / b : 0;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]); break;
          }
          case 28: s.gpr[RA(w)] = s.gpr[RD(w)] & s.gpr[RB(w)];
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // and
          case 444: s.gpr[RA(w)] = s.gpr[RD(w)] | s.gpr[RB(w)];
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // or
          case 316: s.gpr[RA(w)] = s.gpr[RD(w)] ^ s.gpr[RB(w)];
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // xor
          case 60: s.gpr[RA(w)] = s.gpr[RD(w)] & ~s.gpr[RB(w)];
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // andc
          case 412: s.gpr[RA(w)] = s.gpr[RD(w)] | ~s.gpr[RB(w)];
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // orc
          case 476: s.gpr[RA(w)] = ~(s.gpr[RD(w)] & s.gpr[RB(w)]);
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // nand
          case 124: s.gpr[RA(w)] = ~(s.gpr[RD(w)] | s.gpr[RB(w)]);
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // nor
          case 284: s.gpr[RA(w)] = ~(s.gpr[RD(w)] ^ s.gpr[RB(w)]);
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // eqv
          case 24: s.gpr[RA(w)] = (s.gpr[RB(w)] & 0x20) ? 0
                                 : (s.gpr[RD(w)] << (s.gpr[RB(w)] & 31));
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // slw
          case 536: s.gpr[RA(w)] = (s.gpr[RB(w)] & 0x20) ? 0
                                  : (s.gpr[RD(w)] >> (s.gpr[RB(w)] & 31));
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // srw
          case 792: {                                                        // sraw
            uint32_t sh = s.gpr[RB(w)] & 63;
            int32_t v = (int32_t)s.gpr[RD(w)];
            if (sh > 31) sh = 31;
            s.gpr[RA(w)] = (uint32_t)(v >> sh);
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;
          }
          case 824: {                                                        // srawi
            uint32_t sh = Bits(w, 16, 20);
            int32_t v = (int32_t)s.gpr[RD(w)];
            s.xer_ca = (v < 0 && (v & ((1 << sh) - 1))) ? 1 : 0;
            s.gpr[RA(w)] = (uint32_t)(v >> sh);
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;
          }
          case 26: {                                                         // cntlzw
            uint32_t v = s.gpr[RD(w)], n = 0;
            while (n < 32 && !(v & (0x80000000u >> n))) n++;
            s.gpr[RA(w)] = n;
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;
          }
          case 954: s.gpr[RA(w)] = (uint32_t)(int32_t)(int8_t)s.gpr[RD(w)];
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // extsb
          case 922: s.gpr[RA(w)] = (uint32_t)(int32_t)(int16_t)s.gpr[RD(w)];
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;                       // extsh
          // 64-bit multiply/divide, evaluated through the gpr_hi shadow.
          case 233: case 457: case 489: {              // mulld / divdu / divd
            auto get = [&](uint32_t i) -> uint64_t {
              return ((uint64_t)(s.hi_valid[i] ? s.gpr_hi[i] : 0) << 32) | s.gpr[i];
            };
            uint64_t a = get(RA(w)), b = get(RB(w)), out = 0;
            uint32_t xo9 = (w >> 1) & 0x1FF;
            if (xo9 == 233) out = a * b;                                     // mulld
            else if (xo9 == 457) out = b ? a / b : 0;                        // divdu
            else out = b ? (uint64_t)((int64_t)a / (int64_t)b) : 0;          // divd
            s.gpr[RD(w)] = (uint32_t)out;
            s.gpr_hi[RD(w)] = (uint32_t)(out >> 32);
            s.hi_valid[RD(w)] = true;
            if (w & 1) SetCR0(s, s.gpr[RD(w)]);
            break;
          }
          case 986:                                                          // extsw
            s.gpr[RA(w)] = s.gpr[RD(w)];
            s.gpr_hi[RA(w)] = (s.gpr[RD(w)] & 0x80000000u) ? 0xFFFFFFFFu : 0;
            s.hi_valid[RA(w)] = true;
            if (w & 1) SetCR0(s, s.gpr[RA(w)]); break;
          case 0: SetCR(s, (int)(RD(w) >> 2), (int32_t)s.gpr[RA(w)],
                        (int32_t)s.gpr[RB(w)]); break;                       // cmp
          case 32: SetCRU(s, (int)(RD(w) >> 2), s.gpr[RA(w)], s.gpr[RB(w)]); break; // cmpl
          case 23: s.gpr[RD(w)] = mem.R32((RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)]); break;
          case 87: s.gpr[RD(w)] = mem.R8((RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)]); break;
          case 279: s.gpr[RD(w)] = mem.R16((RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)]); break;
          case 151: mem.W32((RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)], s.gpr[RD(w)]); break;
          case 215: mem.W8((RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)], (uint8_t)s.gpr[RD(w)]); break;
          case 407: mem.W16((RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)], (uint16_t)s.gpr[RD(w)]); break;
          case 21: {                                                         // ldx
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)];
            s.gpr_hi[RD(w)] = mem.R32(a);                                    // keep the high word
            s.hi_valid[RD(w)] = true;
            s.gpr[RD(w)] = mem.R32(a + 4);
            break;
          }
          case 149: {                                                        // stdx
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)];
            mem.W32(a, s.hi_valid[RD(w)] ? s.gpr_hi[RD(w)] : 0);
            mem.W32(a + 4, s.gpr[RD(w)]);
            break;
          }
          case 341: s.gpr[RD(w)] = mem.R32((RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)]); break; // lwax
          case 20: case 84: {                                                // lwarx / ldarx
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)];
            s.gpr[RD(w)] = mem.R32(xo == 84 ? a + 4 : a);
            break;
          }
          case 150: case 214: {                                              // stwcx. / stdcx.
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)];
            if (xo == 214) { mem.W32(a, 0); mem.W32(a + 4, s.gpr[RD(w)]); }
            else mem.W32(a, s.gpr[RD(w)]);
            s.cr[0] = 4;                       // always "succeeded": EQ set
            break;
          }
          case 343: {                                                        // lhax
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)];
            s.gpr[RD(w)] = (uint32_t)(int32_t)(int16_t)mem.R16(a);
            break;
          }
          case 375: {                                                        // lhaux
            uint32_t a = s.gpr[RA(w)] + s.gpr[RB(w)];
            s.gpr[RD(w)] = (uint32_t)(int32_t)(int16_t)mem.R16(a);
            s.gpr[RA(w)] = a;
            break;
          }
          case 55: { uint32_t a = s.gpr[RA(w)] + s.gpr[RB(w)];               // lwzux
            s.gpr[RD(w)] = mem.R32(a); s.gpr[RA(w)] = a; break; }
          case 119: { uint32_t a = s.gpr[RA(w)] + s.gpr[RB(w)];              // lbzux
            s.gpr[RD(w)] = mem.R8(a); s.gpr[RA(w)] = a; break; }
          case 311: { uint32_t a = s.gpr[RA(w)] + s.gpr[RB(w)];              // lhzux
            s.gpr[RD(w)] = mem.R16(a); s.gpr[RA(w)] = a; break; }
          case 183: { uint32_t a = s.gpr[RA(w)] + s.gpr[RB(w)];              // stwux
            mem.W32(a, s.gpr[RD(w)]); s.gpr[RA(w)] = a; break; }
          case 247: { uint32_t a = s.gpr[RA(w)] + s.gpr[RB(w)];              // stbux
            mem.W8(a, (uint8_t)s.gpr[RD(w)]); s.gpr[RA(w)] = a; break; }
          case 439: { uint32_t a = s.gpr[RA(w)] + s.gpr[RB(w)];              // sthux
            mem.W16(a, (uint16_t)s.gpr[RD(w)]); s.gpr[RA(w)] = a; break; }
          case 662: {                                                        // stwbrx
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)], v = s.gpr[RD(w)];
            mem.W8(a, (uint8_t)v); mem.W8(a + 1, (uint8_t)(v >> 8));
            mem.W8(a + 2, (uint8_t)(v >> 16)); mem.W8(a + 3, (uint8_t)(v >> 24));
            break;
          }
          case 918: {                                                        // sthbrx
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)], v = s.gpr[RD(w)];
            mem.W8(a, (uint8_t)v); mem.W8(a + 1, (uint8_t)(v >> 8));
            break;
          }
          case 534: {                                                        // lwbrx
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)];
            s.gpr[RD(w)] = (uint32_t)mem.R8(a) | ((uint32_t)mem.R8(a + 1) << 8) |
                           ((uint32_t)mem.R8(a + 2) << 16) | ((uint32_t)mem.R8(a + 3) << 24);
            break;
          }
          case 790: {                                                        // lhbrx
            uint32_t a = (RA(w) ? s.gpr[RA(w)] : 0) + s.gpr[RB(w)];
            s.gpr[RD(w)] = (uint32_t)mem.R8(a) | ((uint32_t)mem.R8(a + 1) << 8);
            break;
          }
          case 339: {                                                        // mfspr
            uint32_t spr = ((w >> 16) & 0x1F) | (((w >> 11) & 0x1F) << 5);
            s.gpr[RD(w)] = (spr == 8) ? s.lr : (spr == 9 ? s.ctr : 0);
            break;
          }
          case 467: {                                                        // mtspr
            uint32_t spr = ((w >> 16) & 0x1F) | (((w >> 11) & 0x1F) << 5);
            if (spr == 8) s.lr = s.gpr[RD(w)];
            else if (spr == 9) s.ctr = s.gpr[RD(w)];
            break;
          }
          case 19: {                                                          // mfcr
            uint32_t v = 0;
            for (int i = 0; i < 8; i++) v |= ((uint32_t)s.cr[i] & 0xF) << (28 - i * 4);
            s.gpr[RD(w)] = v;
            break;
          }
          case 144: break;                                                    // mtcrf
          // MSR. Not modelled beyond storing what was written: guest code uses
          // mfmsr/mtmsr only to disable interrupts around an lwarx/stwcx pair,
          // and with one thread the critical section is already exclusive.
          case 83: s.gpr[RD(w)] = s.msr; break;                               // mfmsr
          case 146: s.msr = s.gpr[RD(w)]; break;                              // mtmsr
          case 178: s.msr = s.gpr[RD(w)]; break;                              // mtmsrd
          case 598: case 854: case 982: case 470: case 54: case 86: case 246:
          case 278: case 310: case 566: case 1014:
            break;   // sync/cache hints: no effect on the modelled state
          default:
            res.error = "unimplemented op31 xo=" + std::to_string(xo);
            res.last_pc = s.pc;
            res.steps = steps;
            goto finish;
        }
        break;
      }
      case 4: case 5: case 6:
        // VMX / VMX128: not modelled. Skipped and counted (see vmx_skipped).
        res.vmx_skipped++;
        break;
      default:
        res.error = "unimplemented opcode " + std::to_string(OP(w));
        res.last_pc = s.pc;
        res.steps = steps;
        goto finish;
    }

    if (req.watch_gpr >= 0 && req.watch_gpr < 32 &&
        s.gpr[req.watch_gpr] != watch_before) {
      // Capped: a watched register in a hot loop would otherwise grow this
      // without bound.
      if (res.watch.size() < 4096) {
        InterpWatchHit h;
        h.pc = s.pc;
        h.old_value = watch_before;
        h.new_value = s.gpr[req.watch_gpr];
        h.step = steps;
        res.watch.push_back(h);
      }
    }

    s.pc = next;
  }

finish:
  // Dumps are collected even when the run ended on an error. They were
  // previously skipped by the early returns, which meant memory could only be
  // inspected for runs that already succeeded - exactly backwards, since a
  // failed run is when you need to see what the guest actually wrote.
  if (!res.ok && res.error.empty()) res.error = "step budget exhausted";
  res.steps = steps;
  res.r3 = s.gpr[3];
  for (const auto& d : req.dumps) {
    InterpDumpResult r;
    r.addr = d.addr;
    for (uint32_t i = 0; i < d.len; i++) r.data.push_back(mem.R8(d.addr + i));
    res.dumps.push_back(std::move(r));
  }
  return res;
}
