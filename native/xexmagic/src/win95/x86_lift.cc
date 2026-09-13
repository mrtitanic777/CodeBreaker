// Win95 port: x86 -> IL lifter (first functional core).
// v1 scope: the common integer/memory/branch/call subset, enough to drive the
// shared DecompileToC on real Win16 functions. SSA is built with a per-function
// register map advanced in address order (exact across straight-line and forward
// code; cross-join phi handling is a later refinement).
#include "win95/x86_lift.h"

#include <algorithm>
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

  // --- pass 1: decode all instructions, collect block leaders ---
  std::vector<X86Insn> insns;
  std::set<uint32_t> leaders{entry};
  for (uint32_t a = entry; a < end;) {
    X86Insn in = DecodeX86(image, image_size, a, a, true);
    insns.push_back(in);
    uint32_t next = a + (in.valid ? in.len : 1);
    const std::string& m = in.mnem;
    bool isJcc = m.size() && m[0] == 'j' && m != "jmp";
    if (m == "jmp" || isJcc) {
      if (in.ops[0].kind == OpKind::Rel) {
        leaders.insert((uint32_t)in.ops[0].imm);
        if (isJcc) leaders.insert(next);   // fallthrough
      }
    } else if (m == "ret" || m == "retf" || m == "iret") {
      leaders.insert(next);
    }
    a = next;
  }

  // index instructions by address
  std::map<uint32_t, size_t> at;
  for (size_t i = 0; i < insns.size(); ++i) at[insns[i].addr] = i;

  // --- function-global register state (approximate SSA) ---
  std::map<int, ILValue> reg;   // GPR slot -> current value
  std::map<int, ILValue> params;      // param index -> value (from [bp+N]), r3..->a1..
  std::vector<ILValue> pend_args;     // pushed values awaiting the next call
  std::set<int> incoming;             // value ids that are undefined entry-reg values
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
      uint32_t next = a + (in.valid ? in.len : 1);
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
          // [bp + disp], disp >= 6 == an incoming parameter (far frame: saved bp +
          // far return addr = 6). Map to r3.. so the back-end names them a1, a2...
          if (o.base != kNoReg && (o.base % 16) == 5 && o.index == kNoReg && o.disp >= 6) {
            int pidx = (o.disp - 6) / 2;
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
        ILInstr x; x.op = ILOp::SarImm; x.address = a; x.src1 = getReg(kGpBase, 2); x.imm = 15;
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
          x.call_name = nit != names_.end() ? nit->second : ("sub_" + std::to_string(x.target));
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
      } else {
        ILInstr x; x.op = ILOp::InlineAsm; x.address = a; x.asm_text = FormatX86(in);
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
