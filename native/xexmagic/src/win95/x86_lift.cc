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
// x86 GPR index (0..7) -> an ILValue "register slot" id reused as ppc_reg.
int RegSlot(int regid) { return regid % 16; }  // GPR index 0..7
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
  auto getReg = [&](int slot, int sz) -> ILValue {
    auto it = reg.find(slot);
    if (it != reg.end()) return it->second;
    ILValue v = fn.AllocValue(WidthType(sz), slot);  // incoming/undefined
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

      auto srcValue = [&](const X86Op& o) -> ILValue {
        if (IsGpr(o)) return getReg(RegSlot(o.reg), o.size);
        if (o.kind == OpKind::Mem) {
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
      } else if (m == "lea") {
        ILInstr x; x.op = ILOp::Add; x.address = a;
        x.src1 = (in.ops[1].base != kNoReg) ? getReg(RegSlot(in.ops[1].base), 2) : fn.AllocValue(ILType::U16);
        x.imm = in.ops[1].disp;
        x.dst = fn.AllocValue(ILType::U16);
        bb.instrs.push_back(x);
        storeTo(in.ops[0], x.dst);
      } else if (m == "add") binop(ILOp::Add);
      else if (m == "sub") binop(ILOp::Sub);
      else if (m == "and") binop(ILOp::And);
      else if (m == "or")  binop(ILOp::Or);
      else if (m == "xor") binop(ILOp::Xor);
      else if (m == "shl" || m == "sal") binop(ILOp::Shl);
      else if (m == "shr") binop(ILOp::Shr);
      else if (m == "sar") binop(ILOp::Sar);
      else if (m == "inc") { ILInstr x; x.op=ILOp::Add; x.address=a; x.src1=srcValue(in.ops[0]); x.imm=1; x.dst=fn.AllocValue(WidthType(in.ops[0].size)); bb.instrs.push_back(x); storeTo(in.ops[0],x.dst);}
      else if (m == "dec") { ILInstr x; x.op=ILOp::Sub; x.address=a; x.src1=srcValue(in.ops[0]); x.imm=1; x.dst=fn.AllocValue(WidthType(in.ops[0].size)); bb.instrs.push_back(x); storeTo(in.ops[0],x.dst);}
      else if (m == "cmp") { flag_kind=FK_CMP; flagA = srcValue(in.ops[0]); if (in.ops[1].kind==OpKind::Imm){flag_imm=true;flag_immv=in.ops[1].imm;}else{flagB=srcValue(in.ops[1]);flag_imm=false;} flag_sz=in.ops[0].size; }
      else if (m == "test") { flag_kind=FK_TEST; flagA = srcValue(in.ops[0]); flagB = srcValue(in.ops[1]); flag_imm=false; flag_sz=in.ops[0].size; }
      else if (m == "call") {
        ILInstr x; x.op = ILOp::Call; x.address = a;
        if (in.ops[0].kind == OpKind::Rel) x.target = (uint32_t)in.ops[0].imm;
        auto nit = names_.find(x.target);
        x.call_name = nit != names_.end() ? nit->second : ("sub_" + std::to_string(x.target));
        x.dst = fn.AllocValue(ILType::U16, 0);   // result in AX
        bb.instrs.push_back(x);
        setReg(0, x.dst);
      } else if (m == "ret" || m == "retf") {
        ILInstr x; x.op = ILOp::Return; x.address = a;
        auto it = reg.find(0); if (it != reg.end()) x.src1 = it->second;
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
      } else {
        // push/pop/nop/string/misc: model as InlineAsm annotation (DCE-friendly)
        if (m != "nop" && m != "push" && m != "pop") {
          ILInstr x; x.op = ILOp::InlineAsm; x.address = a; x.asm_text = FormatX86(in);
          bb.instrs.push_back(x);
        }
      }
      a = next;
    }
    bb.end_addr = bend;
    if (!bb.is_return && bb.successors.empty() && bend < end)
      bb.successors.push_back(bend);   // fallthrough to next block
    fn.blocks.push_back(bb);
  }

  fn.total_instructions = (int)insns.size();
  return fn;
}

}  // namespace win95
