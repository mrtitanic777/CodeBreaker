// Win95 port: x86 -> IL lifter (first functional core).
// v1 scope: the common integer/memory/branch/call subset, enough to drive the
// shared DecompileToC on real Win16 functions. SSA is built with a per-function
// register map advanced in address order (exact across straight-line and forward
// code; cross-join phi handling is a later refinement).
#include "win95/x86_lift.h"
#include "win95/capstone_decode.h"

#include <algorithm>
#include <cctype>
#include <set>

namespace win95 {

namespace {
// x86 GPR index (0..7) -> an ILValue "register slot" id, based at 20 so it never
// collides with the decompiler's arg/param registers r3..r10 (which we populate
// from x86 stack pushes / [bp+N] params instead). ax=20,cx,dx,bx,sp,bp,si,di=27.
constexpr int kGpBase = 20;
int RegSlot(int regid) { return kGpBase + (regid % 16); }  // GPR index 0..7
bool IsGpr(const X86Op& o) { return o.kind == OpKind::Reg && o.reg / 16 == RC_GPR; }

ILCmpKind JccKind(const std::string& m) {
  if (m == "je")  return ILCmpKind::EQ;
  if (m == "jne") return ILCmpKind::NE;
  if (m == "jl")  return ILCmpKind::LT;
  if (m == "jge") return ILCmpKind::GE;
  if (m == "jle") return ILCmpKind::LE;
  if (m == "jg")  return ILCmpKind::GT;
  if (m == "jb")  return ILCmpKind::LTU;
  if (m == "jae") return ILCmpKind::GEU;
  if (m == "jbe") return ILCmpKind::LEU;
  if (m == "ja")  return ILCmpKind::GTU;
  if (m == "js")  return ILCmpKind::LT;   // sign set <=> result < 0 (signed)
  if (m == "jns") return ILCmpKind::GE;
  return ILCmpKind::NE;  // jo/jno/jp/jnp approximated
}
inline bool JccUnsigned(ILCmpKind k) {
  return k == ILCmpKind::LTU || k == ILCmpKind::GTU ||
         k == ILCmpKind::LEU || k == ILCmpKind::GEU;
}
ILType WidthType(int sz, bool sign=false) {
  if (sz == 1) return sign ? ILType::S8  : ILType::U8;
  if (sz == 4) return sign ? ILType::S32 : ILType::U32;
  return sign ? ILType::S16 : ILType::U16;
}
}  // namespace

ILFunction X86Lifter::LiftFunction(const uint8_t* image, size_t image_size,
                                   uint32_t entry, uint32_t end,
                                   const std::string& name) {
  ILFunction fn;
  fn.name = name;
  fn.entry = entry;
  fn.end = end;

  // ABI/decode width. 16-bit far frame: [bp + 6] first param (saved bp + far
  // return = 6), 2-byte slots. 32-bit near frame: [ebp + 8] first param (saved
  // ebp + near return = 8), 4-byte slots. def_op16 drives the decoder's default
  // operand size.
  const bool def_op16 = !bits32_;
  const int  param_base = bits32_ ? 8 : 6;
  const int  slot_sz   = bits32_ ? 4 : 2;
  const int  signfill_bits = bits32_ ? 31 : 15;

  // Bytes an instruction consumes beyond its own length: a VxD `int 20h` swallows
  // the 4-byte service-id dword that follows it (so decode resumes after it).
  auto extraBytes = [&](const X86Insn& in) -> uint32_t {
    if (vxd_mode_ && in.valid && in.mnem == "int" &&
        in.ops[0].kind == OpKind::Imm && in.ops[0].imm == 0x20)
      return 4;
    return 0;
  };
  // Bytes to advance past an instruction -- always >= 1 so decode makes forward
  // progress even if the decoder ever reports a zero length (guards against an
  // infinite loop in the pass-2 walk, which has no visited-set of its own).
  auto advBytes = [&](const X86Insn& in) -> uint32_t {
    uint32_t adv = (in.valid && in.len > 0) ? (uint32_t)in.len : 1;
    return adv + extraBytes(in);
  };

  // --- pass 1: recursive-descent decode from entry. Follow the control flow
  // (fallthrough + branch targets), decoding ONLY reachable code -- so data and
  // inter-function padding is never decoded as instructions (the main source of
  // bogus __asm and misdecodes at scale). Indirect jumps end a run (no target
  // to follow -- jump-table recovery is a later lever). ---
  // A C string literal embedded in a code segment (e.g. `push offset msg; call FatalExit` where
  // FatalExit never returns, then the message bytes follow) must not be decoded as instructions.
  // English text decodes into exactly the garbage we otherwise emit ("Unable" -> push/outs/popa/
  // bound...). Detect a NUL-terminated printable run of >=8 bytes (or a very long printable run) and
  // treat that address as data: the reachability walk stops there instead of decoding the string.
  auto dataRunAt = [&](uint32_t a) -> bool {
    if (a >= image_size) return false;
    uint32_t i = a;
    while (i < image_size && i < a + 256) {
      uint8_t b = image[i];
      if (b == 0x00) break;                 // terminator
      if (b < 0x20 || b > 0x7E) return false;   // non-printable -> not a C string
      ++i;
    }
    uint32_t runlen = i - a;
    return runlen >= 8;                      // >=8 printable then NUL (or >=256): a string, not code
  };

  // A pointer / jump table carries an NE relocation fixup every 2-4 bytes; ordinary code never
  // packs relocations that densely. If >=2 fixup sites fall within the next 8 bytes, we are at a
  // relocated data table, not code -- stop the walk (data-region typing from the NE reloc records).
  auto relocDenseAt = [&](uint32_t a) -> bool {
    if (reloc_sites_.empty()) return false;
    auto it = reloc_sites_.lower_bound(a);
    int n = 0;
    for (; it != reloc_sites_.end() && *it < a + 8; ++it) ++n;
    return n >= 2;
  };

  // A run of zero bytes is zero-initialized data, inter-function padding, or a thunk's connection-data
  // blob -- never code. The byte 0x00 decodes as the degenerate `add [mem],al`, so a walk that runs off
  // the end of a real function into such a run emits thousands of identical `add` lines: this is exactly
  // what turned FT_TAPIFTHKTHKCONNECTIONDATA into a 32,468-line "function" (its few real thunk bytes,
  // then its zero-filled connection-data blob). >=8 consecutive zeros at the decode point is
  // unambiguously data -- no compiler emits `add [bx+si],al` four times over -- so stop the walk here.
  auto zeroRunAt = [&](uint32_t a) -> bool {
    if (a + 8 > image_size) return false;
    for (uint32_t i = a; i < a + 8; ++i) if (image[i] != 0x00) return false;
    return true;
  };

  std::vector<X86Insn> insns;
  std::set<uint32_t> leaders{entry};
  std::map<uint32_t, size_t> at;
  std::vector<uint32_t> work{entry};
  while (!work.empty()) {
    uint32_t a = work.back(); work.pop_back();
    while (a >= entry && a < end && !at.count(a)) {
      if (dataRunAt(a)) break;              // walked into an embedded string literal -> stop (data)
      if (relocDenseAt(a)) break;           // walked into a relocated pointer/jump table -> stop (data)
      if (zeroRunAt(a)) break;              // walked into a zero-fill data/padding region -> stop (data)
      X86Insn in = DecodeX86Auto(image, image_size, a, a, def_op16);
      // An undecodable byte on a reachable path means the walk has run off the end of the code into
      // data (a table, padding, a literal): real reachable code never contains an invalid opcode.
      // Stop the run here instead of advancing one byte and grinding through the data (which is what
      // produced the cascades of `(bad)` / sysexit / MMX-on-data). The byte is left undecoded (data).
      if (!in.valid) break;
      at[a] = insns.size();
      insns.push_back(in);
      uint32_t next = a + advBytes(in);
      const std::string& m = in.mnem;
      // loop/loope/loopne decrement CX and branch conditionally -- pass 2 emits a
      // BranchCond for them, so their target must be a leader (else the goto has
      // no label: E1018). jcxz/jecxz already match the j* test below.
      bool isLoop = m == "loop" || m == "loope" || m == "loopne" ||
                    m == "loopz" || m == "loopnz";
      bool isJcc = (m.size() && m[0] == 'j' && m != "jmp") || isLoop;
      if (m == "jmp") {
        if (in.ops[0].kind == OpKind::Rel) {
          leaders.insert((uint32_t)in.ops[0].imm); work.push_back((uint32_t)in.ops[0].imm);
        }
        break;                              // unconditional: no fallthrough
      }
      if (isJcc) {
        if (in.ops[0].kind == OpKind::Rel) {
          leaders.insert((uint32_t)in.ops[0].imm); work.push_back((uint32_t)in.ops[0].imm);
        }
        leaders.insert(next);               // fallthrough is a block leader
        a = next; continue;                 // and follow it
      }
      if (m == "ret" || m == "retf" || m == "iret") break;   // no successors
      // A call to a function that never returns (FatalExit, abort, exit, ...) has no
      // fallthrough: what follows is data (a message string, a jump/dispatch table). Falling
      // through decodes that data as code, the main remaining source of mis-decodes. Resolve
      // the callee name (far calls via the relocation-built call_sites_, near via names_) and
      // stop the run at a no-return target, so the trailing data is never decoded.
      if (m == "call") {
        std::string tn;
        auto cs = call_sites_.find(a);
        if (cs != call_sites_.end()) tn = cs->second.second;
        else if (in.ops[0].kind == OpKind::Rel || in.ops[0].kind == OpKind::Imm) {
          auto nit = names_.find((uint32_t)in.ops[0].imm);
          if (nit != names_.end()) tn = nit->second;
        }
        std::string u;
        size_t dot = tn.find_last_of(".:");        // strip a KERNEL. / module: prefix
        for (size_t ci = (dot == std::string::npos ? 0 : dot + 1); ci < tn.size(); ++ci)
          u += (char)std::toupper((unsigned char)tn[ci]);
        static const char* kNoRet[] = {
            "FATALEXIT", "FATALAPPEXIT", "EXITKERNEL", "ABORT", "_ABORT", "EXIT", "_EXIT",
            "_CEXIT", "_AMSG_EXIT", "_AMSGEXIT", "EXITPROCESS", "TERMINATEAPP"};
        bool noret = false;
        for (const char* s : kNoRet) if (u == s) { noret = true; break; }
        if (noret) break;                          // no-return: stop; trailing bytes are data
      }
      a = next;                             // call + ordinary ops: fall through
    }
  }

  // --- function-global register state (approximate SSA) ---
  std::map<int, ILValue> reg;   // GPR slot -> current value
  std::map<int, ILValue> params;      // param index -> value (from [bp+N]), r3..->a1..
  std::vector<ILValue> pend_args;     // pushed values awaiting the next call
  std::set<int> incoming;             // value ids that are undefined entry-reg values
  // x87 FP register stack, modeled at lift time. back() == ST(0) (top). Compiler
  // FP code balances the stack within an expression, so a straight per-function
  // simulation recovers ordinary float/double arithmetic (see the "f*" handlers).
  std::vector<ILValue> fpst;
  auto getReg = [&](int slot, int sz) -> ILValue {
    auto it = reg.find(slot);
    if (it != reg.end()) return it->second;
    ILValue v = fn.AllocValue(WidthType(sz), slot);  // incoming/undefined
    incoming.insert(v.id);
    reg[slot] = v;
    return v;
  };
  auto setReg = [&](int slot, ILValue v) { v.ppc_reg = slot; reg[slot] = v; };

  // last flag-setting op, so a following jcc builds the right Compare.
  enum FlagKind { FK_CMP, FK_TEST, FK_ARITH } flag_kind = FK_ARITH;
  ILValue flagA = ILNoVal(), flagB = ILNoVal();
  bool flag_imm = false; int64_t flag_immv = 0; int flag_sz = 2;

  // operand -> value (for reg/imm; mem handled by caller via Load)
  auto immVal = [&](int64_t v, int sz) {
    ILValue r = fn.AllocValue(WidthType(sz));
    return r;  // placeholder; imm carried on the instr
  };

  // --- pass 2: lift block by block, in address order ---
  std::vector<uint32_t> leadv(leaders.begin(), leaders.end());
  std::sort(leadv.begin(), leadv.end());

  for (size_t li = 0; li < leadv.size(); ++li) {
    uint32_t bstart = leadv[li];
    if (bstart >= end) break;
    uint32_t bend = (li + 1 < leadv.size()) ? leadv[li + 1] : end;
    ILBasicBlock bb;
    bb.start_addr = bstart;
    bb.block_id = (int)li;
    pend_args.clear();   // args are pushed within the block that calls

    for (uint32_t a = bstart; a < bend;) {
      auto iit = at.find(a);
      if (iit == at.end()) break;
      const X86Insn& in = insns[iit->second];
      uint32_t next = a + advBytes(in);
      const std::string& m = in.mnem;

      auto emitMemAddr = [&](const X86Op& o, ILInstr& ins) {
        // base value for a memory operand; index folded as Add if present
        ILValue base = ILNoVal();
        if (o.base != kNoReg) base = getReg(RegSlot(o.base), 2);
        ins.imm = o.disp;
        ins.access_type = WidthType(o.size);
        return base;
      };

      // A [bp - N] operand (base=BP, no index, negative disp) is a stack local.
      // Give each distinct offset its own virtual slot (base 1000, off the GPR
      // 20-27, param r3.., and frame ranges) so loads/stores become tracked SSA
      // locals -- rendered as named vNN, not raw *(r25 - N) memory refs.
      auto frameSlotOf = [](const X86Op& o) -> int {
        if (o.kind == OpKind::Mem && o.base != kNoReg && (o.base % 16) == 5 &&
            o.index == kNoReg && o.disp < 0)
          return 1000 + (-o.disp);
        return -1;
      };

      auto srcValue = [&](const X86Op& o) -> ILValue {
        if (IsGpr(o)) return getReg(RegSlot(o.reg), o.size);
        if (o.kind == OpKind::Mem) {
          int fs = frameSlotOf(o);
          if (fs >= 0) return getReg(fs, o.size);
          // [bp + disp], disp >= param_base == an incoming parameter (16-bit far
          // frame: saved bp + far return = 6, 2-byte slots; 32-bit near frame:
          // saved ebp + near return = 8, 4-byte slots). Map to r3.. so the
          // back-end names them a1, a2...
          if (o.base != kNoReg && (o.base % 16) == 5 && o.index == kNoReg && o.disp >= param_base) {
            int pidx = (o.disp - param_base) / slot_sz;
            auto pit = params.find(pidx);
            if (pit != params.end()) return pit->second;
            ILValue pv = fn.AllocValue(WidthType(o.size), 3 + pidx);
            params[pidx] = pv;
            return pv;
          }
          ILInstr ld; ld.op = ILOp::Load; ld.address = a;
          ld.src1 = emitMemAddr(o, ld);
          ld.dst = fn.AllocValue(WidthType(o.size));
          bb.instrs.push_back(ld);
          return ld.dst;
        }
        // imm: a fresh value carrying the constant via Assign/imm
        ILInstr li2; li2.op = ILOp::Assign; li2.address = a;
        li2.dst = fn.AllocValue(WidthType(o.size));
        li2.imm = (int32_t)o.imm;
        bb.instrs.push_back(li2);
        return li2.dst;
      };

      auto storeTo = [&](const X86Op& dst, ILValue v) {
        if (IsGpr(dst)) { setReg(RegSlot(dst.reg), v); return; }
        if (dst.kind == OpKind::Mem) {
          int fs = frameSlotOf(dst);
          if (fs >= 0) { setReg(fs, v); return; }   // [bp - N] local <- v
          ILInstr st; st.op = ILOp::Store; st.address = a;
          st.src1 = emitMemAddr(dst, st); st.src2 = v;
          bb.instrs.push_back(st);
        }
      };

      auto binop = [&](ILOp op) {
        ILValue s1 = srcValue(in.ops[0]);
        ILValue s2 = srcValue(in.ops[1]);
        ILInstr x; x.op = op; x.address = a; x.src1 = s1; x.src2 = s2;
        x.dst = fn.AllocValue(WidthType(in.ops[0].size));
        bb.instrs.push_back(x);
        storeTo(in.ops[0], x.dst);
        // arithmetic sets flags: condition is result vs 0
        flag_kind = FK_ARITH; flagA = x.dst; flag_imm = true; flag_immv = 0;
        flag_sz = in.ops[0].size;
      };

      // --- x87 FP stack helpers (back() == ST0) -----------------------------
      auto fnewUndef = [&]() -> ILValue {          // an undefined incoming FP value
        ILValue v = fn.AllocValue(ILType::F64); incoming.insert(v.id); return v;
      };
      auto fpush = [&](ILValue v) { fpst.push_back(v); };
      auto ftop  = [&]() -> ILValue {
        if (fpst.empty()) fpst.push_back(fnewUndef());
        return fpst.back();
      };
      auto fnth  = [&](int i) -> ILValue {
        int idx = (int)fpst.size() - 1 - i;
        if (idx < 0) { ILValue v = fnewUndef(); fpst.insert(fpst.begin(), v); return v; }
        return fpst[(size_t)idx];
      };
      auto fsetnth = [&](int i, ILValue v) {
        int idx = (int)fpst.size() - 1 - i;
        if (idx >= 0 && idx < (int)fpst.size()) fpst[(size_t)idx] = v;
        else if (fpst.empty()) fpst.push_back(v);
      };
      auto fsetTop = [&](ILValue v) { if (fpst.empty()) fpst.push_back(v); else fpst.back() = v; };
      auto fpop = [&]() { if (!fpst.empty()) fpst.pop_back(); };
      auto stIndex = [&](const X86Op& o) -> int {  // st(i) reg -> i, else -1
        if (o.kind == OpKind::Reg && o.reg >= RegId(RC_ST, 0) && o.reg < RegId(RC_ST, 8))
          return o.reg - RegId(RC_ST, 0);
        return -1;
      };
      auto fpMemType = [&](const X86Op& o) { return o.size == 4 ? ILType::F32 : ILType::F64; };
      // Load a real (float/double) memory operand as an FP value.
      auto fLoadReal = [&](const X86Op& o) -> ILValue {
        ILInstr ld; ld.op = ILOp::FLoad; ld.address = a;
        ld.src1 = emitMemAddr(o, ld);            // sets imm/access_type (overridden below)
        ld.access_type = fpMemType(o);
        ld.dst = fn.AllocValue(fpMemType(o));
        bb.instrs.push_back(ld);
        return ld.dst;
      };
      // Load an integer memory operand and convert to FP (fild).
      auto fLoadInt = [&](const X86Op& o) -> ILValue {
        ILValue iv = srcValue(o);
        ILInstr cv; cv.op = ILOp::FConvFromInt; cv.address = a; cv.src1 = iv;
        cv.dst = fn.AllocValue(ILType::F64); bb.instrs.push_back(cv);
        return cv.dst;
      };
      // Store ST0 to a real memory operand.
      auto fStoreReal = [&](const X86Op& o, ILValue v) {
        ILInstr st; st.op = ILOp::FStore; st.address = a;
        st.src1 = emitMemAddr(o, st); st.src2 = v; st.access_type = fpMemType(o);
        bb.instrs.push_back(st);
      };
      // Convert ST0 to int and store (fist/fistp).
      auto fStoreInt = [&](const X86Op& o, ILValue v) {
        ILInstr cv; cv.op = ILOp::FConvToIntWordTrunc; cv.address = a; cv.src1 = v;
        cv.dst = fn.AllocValue(WidthType(o.size)); bb.instrs.push_back(cv);
        ILInstr st; st.op = ILOp::Store; st.address = a;
        st.src1 = emitMemAddr(o, st); st.src2 = cv.dst; st.access_type = WidthType(o.size);
        bb.instrs.push_back(st);
      };
      auto fpConst = [&](int val) -> ILValue {     // fld1/fldz -> 1.0 / 0.0
        ILInstr c; c.op = ILOp::Assign; c.address = a; c.imm = val;
        c.dst = fn.AllocValue(ILType::F64); bb.instrs.push_back(c); return c.dst;
      };
      auto fpBin = [&](ILOp op, ILValue x, ILValue y) -> ILValue {
        ILInstr i; i.op = op; i.address = a; i.src1 = x; i.src2 = y;
        i.dst = fn.AllocValue(ILType::F64); bb.instrs.push_back(i); return i.dst;
      };
      auto fpUn = [&](ILOp op) {                    // fchs/fabs/fsqrt on ST0
        ILInstr i; i.op = op; i.address = a; i.src1 = ftop();
        i.dst = fn.AllocValue(ILType::F64); bb.instrs.push_back(i); fsetTop(i.dst);
      };
      auto fpArithOp = [&](const std::string& mn) -> ILOp {
        if (mn.find("add") != std::string::npos) return ILOp::FAdd;
        if (mn.find("mul") != std::string::npos) return ILOp::FMul;
        if (mn.find("div") != std::string::npos) return ILOp::FDiv;
        return ILOp::FSub;
      };

      if (m == "mov" || m == "movzx" || m == "movsx") {
        ILValue v = srcValue(in.ops[1]);
        storeTo(in.ops[0], v);
      } else if (m == "les" || m == "lds") {
        // far-ptr load: reg = *(far*)mem. Keep the whole 32-bit far pointer in the
        // register slot so a later es:[reg]/ds:[reg] deref reads through it.
        ILInstr x; x.op = ILOp::Load; x.address = a;
        x.src1 = emitMemAddr(in.ops[1], x);
        x.access_type = ILType::Ptr;
        x.dst = fn.AllocValue(ILType::Ptr);
        bb.instrs.push_back(x);
        storeTo(in.ops[0], x.dst);
      } else if (m == "lea") {
        ILInstr x; x.op = ILOp::Add; x.address = a;
        x.src1 = (in.ops[1].base != kNoReg) ? getReg(RegSlot(in.ops[1].base), 2) : fn.AllocValue(ILType::U16);
        x.imm = in.ops[1].disp;
        x.dst = fn.AllocValue(ILType::U16);
        bb.instrs.push_back(x);
        storeTo(in.ops[0], x.dst);
      } else if (m == "add") binop(ILOp::Add);
      else if (m == "sub") binop(ILOp::Sub);
      else if (m == "adc") binop(ILOp::AddCarry);
      else if (m == "sbb") binop(ILOp::SubCarry);
      else if (m == "and") binop(ILOp::And);
      else if (m == "or")  binop(ILOp::Or);
      else if (m == "xor") binop(ILOp::Xor);
      else if (m == "shl" || m == "sal") binop(ILOp::Shl);
      else if (m == "shr") binop(ILOp::Shr);
      else if (m == "sar") binop(ILOp::Sar);
      else if ((m == "rol" || m == "ror") && in.ops[1].kind == OpKind::Imm) {
        // rotate by immediate -> the recognizable ((x<<n)|(x>>(w-n))) C idiom
        int w = in.ops[0].size * 8;
        int n = (int)(in.ops[1].imm % w);
        ILValue x = srcValue(in.ops[0]);
        ILType t = WidthType(in.ops[0].size);
        int ln = (m == "rol") ? n : (w - n);   // left-shift amount
        int rn = w - ln;                        // right-shift amount
        ILInstr sl; sl.op = ILOp::Shl; sl.address = a; sl.src1 = x; sl.imm = ln; sl.dst = fn.AllocValue(t); bb.instrs.push_back(sl);
        ILInstr sr; sr.op = ILOp::Shr; sr.address = a; sr.src1 = x; sr.imm = rn; sr.dst = fn.AllocValue(t); bb.instrs.push_back(sr);
        ILInstr o; o.op = ILOp::Or; o.address = a; o.src1 = sl.dst; o.src2 = sr.dst; o.dst = fn.AllocValue(t); bb.instrs.push_back(o);
        storeTo(in.ops[0], o.dst);
      }
      else if (m == "neg" || m == "not") {
        ILInstr x; x.op = (m == "neg") ? ILOp::Neg : ILOp::Not; x.address = a;
        x.src1 = srcValue(in.ops[0]); x.dst = fn.AllocValue(WidthType(in.ops[0].size));
        bb.instrs.push_back(x); storeTo(in.ops[0], x.dst);
        flag_kind = FK_ARITH; flagA = x.dst; flag_imm = true; flag_immv = 0; flag_sz = in.ops[0].size;
      }
      else if (m == "cbw" || m == "cwde") {     // AX = sign-extend AL (EAX = sext AX)
        ILInstr x; x.op = ILOp::ExtendByte; x.address = a; x.src1 = getReg(kGpBase, 1);
        x.dst = fn.AllocValue(ILType::U16); bb.instrs.push_back(x); setReg(kGpBase, x.dst);
      }
      else if (m == "cwd" || m == "cdq") {       // DX = (AX < 0) ? -1 : 0  (sign fill)
        ILInstr x; x.op = ILOp::SarImm; x.address = a; x.src1 = getReg(kGpBase, slot_sz); x.imm = signfill_bits;
        x.dst = fn.AllocValue(ILType::U16); bb.instrs.push_back(x); setReg(kGpBase + 2, x.dst);
      }
      else if (m == "inc") { ILInstr x; x.op=ILOp::Add; x.address=a; x.src1=srcValue(in.ops[0]); x.imm=1; x.dst=fn.AllocValue(WidthType(in.ops[0].size)); bb.instrs.push_back(x); storeTo(in.ops[0],x.dst);}
      else if (m == "dec") { ILInstr x; x.op=ILOp::Sub; x.address=a; x.src1=srcValue(in.ops[0]); x.imm=1; x.dst=fn.AllocValue(WidthType(in.ops[0].size)); bb.instrs.push_back(x); storeTo(in.ops[0],x.dst);}
      else if (m == "cmp") { flag_kind=FK_CMP; flagA = srcValue(in.ops[0]); if (in.ops[1].kind==OpKind::Imm){flag_imm=true;flag_immv=in.ops[1].imm;}else{flagB=srcValue(in.ops[1]);flag_imm=false;} flag_sz=in.ops[0].size; }
      else if (m == "test") { flag_kind=FK_TEST; flagA = srcValue(in.ops[0]); flagB = srcValue(in.ops[1]); flag_imm=false; flag_sz=in.ops[0].size; }
      else if (m == "call") {
        ILInstr x; x.op = ILOp::Call; x.address = a;
        auto cs = call_sites_.find(a);                 // far call resolved via relocation
        if (cs != call_sites_.end()) { x.target = cs->second.first; x.call_name = cs->second.second; }
        else if (in.ops[0].kind == OpKind::Rel) {
          x.target = (uint32_t)in.ops[0].imm;
          auto nit = names_.find(x.target);
          // Unnamed target: spell it sub_%06X (hex linear addr) to EXACTLY match the definition
          // name the module driver emits (test_decomp.cc `sub_%06X`). Decimal here (std::to_string)
          // made every unnamed call name disagree with its definition -> an undefined reference the
          // linker had to stub. Hex on both sides makes the call resolve to the real body.
          char sb[24]; std::snprintf(sb, sizeof sb, "sub_%06X", x.target);
          x.call_name = nit != names_.end() ? nit->second : sb;
        } else {
          x.call_name = "indirect";                    // call through reg/mem (v1)
        }
        // stage pushed args into the arg registers r3.. (PASCAL push order); the
        // back-end reads r3..r10 as the call's arguments.
        for (size_t i = 0; i < pend_args.size() && i < 8; ++i) {
          ILInstr ar; ar.op = ILOp::Assign; ar.address = a; ar.src1 = pend_args[i];
          ar.dst = fn.AllocValue(pend_args[i].type, 3 + (int)i);
          bb.instrs.push_back(ar);
        }
        pend_args.clear();
        x.dst = fn.AllocValue(ILType::U32, kGpBase);   // DX:AX (long returns); AX truncates on 16-bit use
        bb.instrs.push_back(x);
        setReg(kGpBase, x.dst);
        // DX = high word of the return (for long-returning calls like _llseek);
        // dropped by DCE when the caller only uses AX.
        ILInstr hi; hi.op = ILOp::Shr; hi.address = a; hi.src1 = x.dst; hi.imm = 16;
        hi.dst = fn.AllocValue(ILType::U16, kGpBase + 2); bb.instrs.push_back(hi);
        setReg(kGpBase + 2, hi.dst);
      } else if (m == "ret" || m == "retf") {
        ILInstr x; x.op = ILOp::Return; x.address = a;
        auto it = reg.find(kGpBase); if (it != reg.end()) x.src1 = it->second;
        bb.instrs.push_back(x);
        bb.is_return = true;
      } else if (m == "jmp") {
        ILInstr x; x.op = ILOp::Branch; x.address = a;
        if (in.ops[0].kind == OpKind::Rel) { x.target = (uint32_t)in.ops[0].imm; bb.successors.push_back(x.target); }
        bb.instrs.push_back(x);
      } else if (m.size() && m[0] == 'j') {   // jcc
        ILCmpKind kind = JccKind(m);
        bool uns = JccUnsigned(kind);
        ILValue cmpA = flagA;
        bool useImm = flag_imm;
        int64_t immv = flag_immv;
        if (flag_kind == FK_TEST) {
          // test a,b -> condition on (a & b); test r,r collapses to (r vs 0)
          if (flagA.id == flagB.id) { useImm = true; immv = 0; }
          else {
            ILInstr an; an.op = ILOp::And; an.address = a; an.src1 = flagA; an.src2 = flagB;
            an.dst = fn.AllocValue(WidthType(flag_sz)); bb.instrs.push_back(an);
            cmpA = an.dst; useImm = true; immv = 0;
          }
        }
        ILInstr cmp; cmp.address = a; cmp.src1 = cmpA;
        if (useImm) { cmp.op = uns ? ILOp::CompareImmU : ILOp::CompareImm; cmp.imm = (int32_t)immv; }
        else { cmp.op = uns ? ILOp::CompareU : ILOp::Compare; cmp.src2 = flagB; }
        cmp.dst = fn.AllocValue(ILType::Bool);
        bb.instrs.push_back(cmp);
        ILInstr br; br.op = ILOp::BranchCond; br.address = a;
        br.cmp_ref = cmp.dst.id; br.cmp_kind = kind;
        if (in.ops[0].kind == OpKind::Rel) br.target = (uint32_t)in.ops[0].imm;
        bb.instrs.push_back(br);
        bb.successors.push_back(br.target);
        bb.successors.push_back(next);   // fallthrough
      } else if (m == "push") {
        // An outgoing argument, unless it is a prologue save: push of an untouched
        // incoming callee-saved reg (bp/si/di) or a segment reg.
        const X86Op& o = in.ops[0];
        bool save = false;
        if (IsGpr(o)) {
          int idx = o.reg % 16;
          ILValue cur = getReg(RegSlot(o.reg), 2);
          if ((idx == 5 || idx == 6 || idx == 7) && incoming.count(cur.id)) save = true;
        } else if (o.kind == OpKind::Reg && o.reg / 16 == RC_SEG) {
          save = true;
        }
        if (!save) pend_args.push_back(srcValue(o));
      } else if (m == "pop" || m == "nop" || m == "leave" || m == "enter" ||
                 m == "cld" || m == "std" || m == "cli" || m == "sti" ||
                 m == "push" /*unreached*/) {
        // frame teardown/setup + flag-only ops carry no meaning for a reader
      } else if (m == "mul" || m == "imul") {
        // group3 one-operand: DX:AX = AX * rm; two/three-operand imul: reg = reg*rm
        if (in.nops >= 2) {
          ILInstr x; x.op = ILOp::Mul; x.address = a;
          x.src1 = srcValue(in.ops[in.nops - 2]); x.src2 = srcValue(in.ops[in.nops - 1]);
          x.dst = fn.AllocValue(WidthType(in.ops[0].size));
          bb.instrs.push_back(x); storeTo(in.ops[0], x.dst);
        } else {
          ILInstr x; x.op = ILOp::Mul; x.address = a;
          x.src1 = getReg(kGpBase, 2); x.src2 = srcValue(in.ops[0]);
          x.dst = fn.AllocValue(ILType::U16); bb.instrs.push_back(x); setReg(kGpBase, x.dst);
        }
      } else if (m == "div" || m == "idiv") {
        ILInstr x; x.op = (m == "idiv") ? ILOp::Div : ILOp::DivU; x.address = a;
        x.src1 = getReg(kGpBase, 2); x.src2 = srcValue(in.ops[0]);
        x.dst = fn.AllocValue(ILType::U16); bb.instrs.push_back(x); setReg(kGpBase, x.dst);
      } else if (m == "xchg") {
        ILValue v0 = srcValue(in.ops[0]), v1 = srcValue(in.ops[1]);
        storeTo(in.ops[0], v1); storeTo(in.ops[1], v0);
      } else if (m == "loop" || m == "loope" || m == "loopne") {
        int cx = kGpBase + 1;                          // CX
        ILInstr d; d.op = ILOp::Sub; d.address = a; d.src1 = getReg(cx, 2); d.imm = 1;
        d.dst = fn.AllocValue(ILType::U16); bb.instrs.push_back(d); setReg(cx, d.dst);
        ILInstr cmp; cmp.op = ILOp::CompareImm; cmp.address = a; cmp.src1 = d.dst; cmp.imm = 0;
        cmp.dst = fn.AllocValue(ILType::Bool); bb.instrs.push_back(cmp);
        ILInstr br; br.op = ILOp::BranchCond; br.address = a; br.cmp_ref = cmp.dst.id;
        br.cmp_kind = ILCmpKind::NE;
        if (in.ops[0].kind == OpKind::Rel) br.target = (uint32_t)in.ops[0].imm;
        bb.instrs.push_back(br);
        bb.successors.push_back(br.target); bb.successors.push_back(next);
      } else if (m.rfind("movs", 0) == 0 || m.rfind("stos", 0) == 0 || m.rfind("lods", 0) == 0) {
        int esz = m.back() == 'b' ? 1 : m.back() == 'd' ? 4 : 2;
        int DI = kGpBase + 7, SI = kGpBase + 6, CX = kGpBase + 1, AX = kGpBase;
        auto stageCall = [&](const char* nm, std::initializer_list<ILValue> args) {
          int i = 0;
          for (ILValue v : args) { ILInstr ar; ar.op = ILOp::Assign; ar.address = a;
            ar.src1 = v; ar.dst = fn.AllocValue(v.type, 3 + i++); bb.instrs.push_back(ar); }
          ILInstr c; c.op = ILOp::Call; c.address = a; c.call_name = nm;
          c.dst = fn.AllocValue(ILType::U16, kGpBase); bb.instrs.push_back(c);
        };
        if (m[0] == 'm') {                    // movs: copy [si]->[di]
          if (in.rep) stageCall("memcpy", {getReg(DI, 2), getReg(SI, 2), getReg(CX, 2)});
          else { ILInstr ld; ld.op = ILOp::Load; ld.address = a; ld.src1 = getReg(SI, 2);
                 ld.access_type = WidthType(esz); ld.dst = fn.AllocValue(WidthType(esz));
                 bb.instrs.push_back(ld);
                 ILInstr st; st.op = ILOp::Store; st.address = a; st.src1 = getReg(DI, 2);
                 st.src2 = ld.dst; st.access_type = WidthType(esz); bb.instrs.push_back(st); }
        } else if (m[0] == 's') {             // stos: fill [di] with al/ax
          if (in.rep) stageCall("memset", {getReg(DI, 2), getReg(AX, esz), getReg(CX, 2)});
          else { ILInstr st; st.op = ILOp::Store; st.address = a; st.src1 = getReg(DI, 2);
                 st.src2 = getReg(AX, esz); st.access_type = WidthType(esz); bb.instrs.push_back(st); }
        } else {                              // lods: al/ax = [si]
          ILInstr ld; ld.op = ILOp::Load; ld.address = a; ld.src1 = getReg(SI, 2);
          ld.access_type = WidthType(esz); ld.dst = fn.AllocValue(WidthType(esz));
          bb.instrs.push_back(ld); setReg(AX, ld.dst);
        }
      } else if (m.rfind("scas", 0) == 0 || m.rfind("cmps", 0) == 0) {
        int esz = m.back() == 'b' ? 1 : m.back() == 'd' ? 4 : 2;
        int DI = kGpBase + 7, SI = kGpBase + 6, CX = kGpBase + 1, AX = kGpBase;
        if (in.rep || in.repne) {
          // rep scas -> memchr(edi, al, cx); rep cmps -> memcmp(esi, edi, cx)
          std::initializer_list<ILValue> args = (m[0] == 's')
              ? std::initializer_list<ILValue>{getReg(DI, 2), getReg(AX, esz), getReg(CX, 2)}
              : std::initializer_list<ILValue>{getReg(SI, 2), getReg(DI, 2), getReg(CX, 2)};
          int i = 0;
          for (ILValue v : args) { ILInstr ar; ar.op = ILOp::Assign; ar.address = a;
            ar.src1 = v; ar.dst = fn.AllocValue(v.type, 3 + i++); bb.instrs.push_back(ar); }
          ILInstr c; c.op = ILOp::Call; c.address = a;
          c.call_name = (m[0] == 's') ? "memchr" : "memcmp";
          c.dst = fn.AllocValue(ILType::U16, kGpBase); bb.instrs.push_back(c);
          setReg(kGpBase, c.dst);
        } else {
          // single: compare and set flags (scas: al/ax vs [di]; cmps: [si] vs [di])
          ILInstr ld1; ld1.op = ILOp::Load; ld1.address = a;
          ld1.src1 = getReg(m[0] == 's' ? DI : SI, 2); ld1.access_type = WidthType(esz);
          ld1.dst = fn.AllocValue(WidthType(esz)); bb.instrs.push_back(ld1);
          if (m[0] == 's') { flag_kind = FK_CMP; flagA = getReg(AX, esz); flagB = ld1.dst; flag_imm = false; }
          else { ILInstr ld2; ld2.op = ILOp::Load; ld2.address = a; ld2.src1 = getReg(DI, 2);
                 ld2.access_type = WidthType(esz); ld2.dst = fn.AllocValue(WidthType(esz)); bb.instrs.push_back(ld2);
                 flag_kind = FK_CMP; flagA = ld1.dst; flagB = ld2.dst; flag_imm = false; }
          flag_sz = esz;
        }
      } else if (m == "fld") {
        const X86Op& o = in.ops[0]; int si = stIndex(o);
        fpush(si >= 0 ? fnth(si) : fLoadReal(o));
      } else if (m == "fild") {
        fpush(fLoadInt(in.ops[0]));
      } else if (m == "fld1") { fpush(fpConst(1));
      } else if (m == "fldz") { fpush(fpConst(0));
      } else if (m == "fldpi" || m == "fldl2e" || m == "fldl2t" ||
                 m == "fldlg2" || m == "fldln2" || m == "fbld") {
        fpush(fnewUndef());          // transcendental/BCD constant: defined, unmodeled
      } else if (m == "fst" || m == "fstp") {
        const X86Op& o = in.ops[0]; int si = stIndex(o);
        ILValue t = ftop();
        if (si >= 0) fsetnth(si, t); else fStoreReal(o, t);
        if (m == "fstp") fpop();
      } else if (m == "fist" || m == "fistp" || m == "fisttp" || m == "fbstp") {
        fStoreInt(in.ops[0], ftop());
        if (m != "fist") fpop();
      } else if (m == "fchs") { fpUn(ILOp::FNeg);
      } else if (m == "fabs") { fpUn(ILOp::FAbs);
      } else if (m == "fsqrt") { fpUn(ILOp::FSqrt);
      } else if (m == "fxch") {
        int i = 1; if (in.nops >= 1) { int x = stIndex(in.ops[0]); if (x >= 0) i = x; }
        ILValue a0 = ftop(), ai = fnth(i);
        fsetTop(ai); fsetnth(i, a0);
      } else if (m == "fadd" || m == "fsub" || m == "fmul" || m == "fdiv" ||
                 m == "fsubr" || m == "fdivr" || m == "faddp" || m == "fsubp" ||
                 m == "fmulp" || m == "fdivp" || m == "fsubrp" || m == "fdivrp" ||
                 m == "fiadd" || m == "fisub" || m == "fimul" || m == "fidiv" ||
                 m == "fisubr" || m == "fidivr") {
        ILOp op = fpArithOp(m);
        bool rev = m.find("subr") != std::string::npos || m.find("divr") != std::string::npos;
        bool pop = m.back() == 'p';
        bool integer = m.size() > 1 && m[1] == 'i';
        int dstIdx = 0; ILValue rhs;
        if (in.nops == 0) {
          if (pop) { dstIdx = 1; rhs = ftop(); }        // fADDp -> ST1 op= ST0, pop
          else     { dstIdx = 0; rhs = fnth(1); }        // fADD  -> ST0 op= ST1
        } else if (in.nops == 1) {
          const X86Op& o = in.ops[0]; int si = stIndex(o);
          if (si >= 0) { if (pop) { dstIdx = si; rhs = ftop(); }
                         else     { dstIdx = 0;  rhs = fnth(si); } }
          else { dstIdx = 0; rhs = integer ? fLoadInt(o) : fLoadReal(o); }
        } else {
          int d = stIndex(in.ops[0]), s = stIndex(in.ops[1]);
          dstIdx = (d >= 0) ? d : 0;
          rhs = (s >= 0) ? fnth(s) : (integer ? fLoadInt(in.ops[1]) : fLoadReal(in.ops[1]));
        }
        ILValue lhs = fnth(dstIdx);
        ILValue r = rev ? fpBin(op, rhs, lhs) : fpBin(op, lhs, rhs);
        fsetnth(dstIdx, r);
        if (pop) fpop();
      } else if (m == "fcom" || m == "fcomp" || m == "fcompp" || m == "fucom" ||
                 m == "fucomp" || m == "fucompp" || m == "fcomi" || m == "fcomip" ||
                 m == "fucomi" || m == "fucomip" || m == "ficom" || m == "ficomp" ||
                 m == "ftst") {
        ILValue A = ftop(), B;
        if (m == "ftst") B = fpConst(0);
        else if (in.nops >= 1) { int si = stIndex(in.ops[0]);
          B = (si >= 0) ? fnth(si)
                        : ((m[1] == 'i') ? fLoadInt(in.ops[0]) : fLoadReal(in.ops[0])); }
        else B = fnth(1);
        // Record the FP compare. A following (fnstsw ax; sahf) jcc, or a fcomi
        // jcc, reads it -- the unsigned jcc mapping matches fcom+sahf exactly.
        flag_kind = FK_ARITH; flagA = A; flagB = B; flag_imm = false; flag_sz = 4;
        if (m == "fcompp" || m == "fucompp") { fpop(); fpop(); }
        else if (m.back() == 'p') fpop();
      } else if (m == "fnstsw" || m == "fstsw" || m == "fnstcw" || m == "fstcw" ||
                 m == "fldcw" || m == "fnstenv" || m == "fstenv" || m == "fldenv" ||
                 m == "fnclex" || m == "fclex" || m == "finit" || m == "fninit" ||
                 m == "fnsave" || m == "fsave" || m == "frstor" || m == "fwait" ||
                 m == "fnop" || m == "fdecstp" || m == "fincstp" || m == "ffree" ||
                 m == "ffreep") {
        // FP control/status/no-data-effect ops. Crucially fnstsw is a no-op here
        // so the recorded FP compare survives to the following jcc.
        if (m == "ffreep") fpop();
      } else if (m.rfind("set", 0) == 0 && m.size() > 3 && m != "setssbsy") {
        // setcc: dst = (cond), built from the pending flag state (like jcc).
        ILCmpKind kind = JccKind("j" + m.substr(3));
        ILInstr sc; sc.op = ILOp::SetCond; sc.address = a; sc.cmp_kind = kind;
        auto mkImm = [&](int64_t v) -> ILValue {
          ILInstr z; z.op = ILOp::Assign; z.address = a; z.imm = (int32_t)v;
          z.dst = fn.AllocValue(WidthType(flag_sz)); bb.instrs.push_back(z); return z.dst;
        };
        if (flag_kind == FK_TEST && flagA.id != flagB.id) {
          ILInstr an; an.op = ILOp::And; an.address = a; an.src1 = flagA; an.src2 = flagB;
          an.dst = fn.AllocValue(WidthType(flag_sz)); bb.instrs.push_back(an);
          sc.src1 = an.dst; sc.src2 = mkImm(0);
        } else if (flag_kind == FK_TEST) { sc.src1 = flagA; sc.src2 = mkImm(0);
        } else if (flag_imm) { sc.src1 = flagA; sc.src2 = mkImm(flag_immv);
        } else { sc.src1 = flagA; sc.src2 = flagB; }
        sc.dst = fn.AllocValue(WidthType(in.ops[0].size));
        bb.instrs.push_back(sc);
        storeTo(in.ops[0], sc.dst);
      } else if (m == "shld" || m == "shrd") {
        // double-precision shift: shld d,s,c -> d = (d<<c)|(s>>(bits-c)); shrd is
        // the mirror. Count may be imm or CL -- modelled as a value either way.
        int sz = in.ops[0].size, bits = sz * 8;
        ILValue d = srcValue(in.ops[0]), s = srcValue(in.ops[1]), c = srcValue(in.ops[2]);
        ILInstr bc; bc.op = ILOp::Assign; bc.address = a; bc.imm = bits;
        bc.dst = fn.AllocValue(WidthType(sz)); bb.instrs.push_back(bc);
        ILInstr sub; sub.op = ILOp::Sub; sub.address = a; sub.src1 = bc.dst; sub.src2 = c;
        sub.dst = fn.AllocValue(WidthType(sz)); bb.instrs.push_back(sub);
        ILInstr s1; s1.op = (m == "shld") ? ILOp::Shl : ILOp::Shr; s1.address = a;
        s1.src1 = d; s1.src2 = c; s1.dst = fn.AllocValue(WidthType(sz)); bb.instrs.push_back(s1);
        ILInstr s2; s2.op = (m == "shld") ? ILOp::Shr : ILOp::Shl; s2.address = a;
        s2.src1 = s; s2.src2 = sub.dst; s2.dst = fn.AllocValue(WidthType(sz)); bb.instrs.push_back(s2);
        ILInstr orr; orr.op = ILOp::Or; orr.address = a; orr.src1 = s1.dst; orr.src2 = s2.dst;
        orr.dst = fn.AllocValue(WidthType(sz)); bb.instrs.push_back(orr);
        storeTo(in.ops[0], orr.dst);
        flag_kind = FK_ARITH; flagA = orr.dst; flag_imm = true; flag_immv = 0; flag_sz = sz;
      } else if (m == "pushad" || m == "popad" || m == "pusha" || m == "popa" ||
                 m == "pushal" || m == "popal" || m == "pushaw" || m == "popaw" ||
                 m == "pushfd" || m == "popfd" || m == "pushf" || m == "popf" ||
                 m == "pushfw" || m == "popfw" || m == "sahf" || m == "lahf" ||
                 m == "clc" || m == "stc" || m == "cmc" || m == "wait" ||
                 m == "hlt" || m == "int3") {
        // register/flag block save-restore + flag manipulation: no data meaning
        // for a reader (sahf after fnstsw is the FP-compare idiom, handled above).
      } else if (vxd_mode_ && m == "int" && in.ops[0].kind == OpKind::Imm &&
                 in.ops[0].imm == 0x20) {
        // VxD service call: int 20h; dd <(VxD id << 16) | service ordinal>. Render
        // as a named call so the reader sees the invoked service, and DX:AX holds
        // the (documented) return.
        uint32_t svc = 0;
        if (a + in.len + 4 <= image_size)
          svc = image[a + in.len] | (image[a + in.len + 1] << 8) |
                (image[a + in.len + 2] << 16) | ((uint32_t)image[a + in.len + 3] << 24);
        char nm[24]; std::snprintf(nm, sizeof nm, "VxDCall_%08X", svc);
        ILInstr x; x.op = ILOp::Call; x.address = a; x.call_name = nm;
        x.dst = fn.AllocValue(ILType::U32, kGpBase);
        bb.instrs.push_back(x);
        setReg(kGpBase, x.dst);
      } else {
        // The fallback text lands in an __asm{} block, so emit MASM/Watcom syntax that our
        // assemblers can actually build (word ptr [..], 21h, bare string mnemonics). Also carry the
        // raw bytes: an assembler that cannot spell the mnemonic (undocumented/rare opcodes) can
        // still emit the exact instruction via `db`, so no real behaviour is ever dropped.
        ILInstr x; x.op = ILOp::InlineAsm; x.address = a;
        x.asm_text = FormatX86(in, AsmSyntax::Masm);
        {
          char hb[8]; std::string hx;
          uint32_t n = (in.valid && in.len > 0) ? (uint32_t)in.len : 1;
          for (uint32_t bi = 0; bi < n && a + bi < image_size; ++bi) {
            std::snprintf(hb, sizeof hb, "%s0%02Xh", bi ? "," : "", image[a + bi]);
            hx += hb;
          }
          x.raw_hex = hx;
        }
        bb.instrs.push_back(x);
        if (getenv("LIFT_DIAG")) std::fprintf(stderr, "UNHANDLED %s\n", m.c_str());
      }
      a = next;
    }
    bb.end_addr = bend;
    if (!bb.is_return && bb.successors.empty() && bend < end)
      bb.successors.push_back(bend);   // fallthrough to next block
    fn.blocks.push_back(bb);
  }

  // Prune blocks unreachable from entry: module-mode function bounds run to the
  // next leader, so trailing alignment/data past the final `ret` gets decoded as
  // a block with no predecessor. Drop those (kills misdecoded padding tails).
  if (!fn.blocks.empty()) {
    std::map<uint32_t, size_t> bidx;
    for (size_t i = 0; i < fn.blocks.size(); ++i) bidx[fn.blocks[i].start_addr] = i;
    std::set<size_t> reach;
    std::vector<size_t> work{0};
    while (!work.empty()) {
      size_t b = work.back(); work.pop_back();
      if (!reach.insert(b).second) continue;
      for (uint32_t s : fn.blocks[b].successors) {
        auto it = bidx.find(s);
        if (it != bidx.end() && !reach.count(it->second)) work.push_back(it->second);
      }
    }
    if (reach.size() < fn.blocks.size()) {
      std::vector<ILBasicBlock> kept;
      for (size_t i = 0; i < fn.blocks.size(); ++i)
        if (reach.count(i)) kept.push_back(std::move(fn.blocks[i]));
      fn.blocks = std::move(kept);
    }
  }

  fn.total_instructions = (int)insns.size();
  return fn;
}

}  // namespace win95
