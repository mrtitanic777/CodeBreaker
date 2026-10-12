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

// Bytes of arguments the function at `target` removes on return: N of its first `ret N` (0 for a
// plain `ret`), following a direct tail jump; -1 when that cannot be read off the code. This is
// what a caller needs to know how many of the values it pushed belong to THIS call.
int X86Lifter::CalleePop(uint32_t target) {
  auto it = pop_cache_.find(target);
  if (it != pop_cache_.end()) return it->second;
  int result = -1;
  uint32_t a = target;
  int hops = 0;
  for (int n = 0; n < 4000 && img_ && a < img_size_; ++n) {
    X86Insn in = DecodeX86Auto(img_, img_size_, a, a, !bits32_);
    if (!in.valid) break;
    if (in.mnem == "ret") {
      result = (in.nops > 0 && in.ops[0].kind == OpKind::Imm) ? (int)in.ops[0].imm : 0;
      break;
    }
    if (in.mnem == "retf" || in.mnem == "iret" || in.mnem == "int3") break;
    if (in.mnem == "jmp") {
      if (in.ops[0].kind != OpKind::Rel || ++hops > 4) break;
      a = (uint32_t)in.ops[0].imm;
      continue;
    }
    a += in.len;
  }
  pop_cache_[target] = result;
  return result;
}

ILFunction X86Lifter::LiftFunction(const uint8_t* image, size_t image_size,
                                   uint32_t entry, uint32_t end,
                                   const std::string& name) {
  img_ = image; img_size_ = image_size;
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
  std::set<int> paramIds;             // the value ids in `params`
  std::vector<ILValue> pend_args;     // pushed values awaiting the next call
  std::map<int, ILValue> esp_args;    // 32-bit: [esp+N] arg writes (offset -> value) awaiting a call
  std::set<int> incoming;             // value ids that are undefined entry-reg values
  // x87 FP register stack, modeled at lift time. back() == ST(0) (top). Compiler
  // FP code balances the stack within an expression, so a straight per-function
  // simulation recovers ordinary float/double arithmetic (see the "f*" handlers).
  std::vector<ILValue> fpst;
  // Per-block register flow (see "register flow" below). `reg` is the state of the block being
  // lifted; a slot no path has written holds the function-entry value, which is ONE value per slot
  // for the whole function (entryIn), not a fresh one per block.
  const bool regflow = !getenv("CB_NO_REGFLOW");
  std::map<int, ILValue> entryIn;     // slot -> its function-entry (incoming) value
  std::set<int> mergeIds;             // value ids that are merge values (assigned in several preds)
  ILBasicBlock* cur_bb = nullptr;     // block being lifted (for copies emitted by setReg)
  uint32_t cur_addr = 0;              // address of the instruction being lifted
  auto slotIncoming = [&](int slot, ILType t) -> ILValue {
    auto e = entryIn.find(slot);
    if (e != entryIn.end()) return e->second;
    ILValue v = fn.AllocValue(t, slot);              // incoming/undefined
    incoming.insert(v.id);
    entryIn[slot] = v;
    return v;
  };
  auto getReg = [&](int slot, int sz) -> ILValue {
    auto it = reg.find(slot);
    if (it != reg.end()) return it->second;
    ILValue v;
    if (regflow) {
      v = slotIncoming(slot, WidthType(sz));
    } else {
      v = fn.AllocValue(WidthType(sz), slot);        // incoming/undefined
      incoming.insert(v.id);
    }
    reg[slot] = v;
    return v;
  };
  // Constant/value tracking for indirect-call resolution: regConst[slot] = the immediate address last
  // loaded into that GPR, if any. `mov reg, imm` / `lea reg, [imm]` set it; ANY other write clears it
  // (so a stale constant never resolves a real computed call). At `call reg` / `jmp reg` a live entry
  // that points at a function turns the indirect call into a direct one -- the data-flow lever that
  // recovers `mov reg, offset FUNC; call reg` (callback registration, dispatch trampolines).
  std::map<int, int64_t> regConst;
  // Values loaded from an import-table slot (by IL value id) -> the import's name. Keyed by value,
  // not register, so it holds wherever the pointer is kept and across blocks.
  std::map<int, std::string> importVal;
  // Return-address-in-register idiom (shared stdcall-cleanup return trampolines): a `pop reg` taken
  // while the stack is still at entry depth loads the caller's return address, and a later `jmp reg`
  // on that register is a RETURN (value in AX), not an indirect call. Track the register that holds
  // it and whether the stack has been disturbed yet (any push/call/enter or write to ESP).
  int retaddr_reg = -1; bool stack_dirty = false;
  auto setReg = [&](int slot, ILValue v) {
    // A merge value is a VARIABLE (it is assigned at the end of each predecessor), unlike every
    // other value, which is defined once. Binding it to a second slot (`mov ebx, esi`) must
    // therefore take a copy: an alias would silently change when the merge value is next assigned
    // (the loop latch), giving the other register the new value instead of the one it captured.
    if (cur_bb && v.ppc_reg != slot && mergeIds.count(v.id)) {
      ILInstr cp; cp.op = ILOp::Assign; cp.address = cur_addr; cp.src1 = v;
      cp.dst = fn.AllocValue(v.type);
      cur_bb->instrs.push_back(cp);
      v = cp.dst;
    }
    // The register tag names a value that has no defining instruction: a parameter keeps its own
    // (a1, a2...) and a function-entry register its own, wherever the value is copied. Retagging
    // them made `mov ebx, [arg]` print the argument as a register parameter called bx.
    if (!incoming.count(v.id) && !paramIds.count(v.id)) v.ppc_reg = slot;
    reg[slot] = v; regConst.erase(slot);
    if (slot == retaddr_reg) retaddr_reg = -1;
    if (slot == kGpBase + 4) stack_dirty = true; };

  // last flag-setting op, so a following jcc builds the right Compare.
  enum FlagKind { FK_CMP, FK_TEST, FK_ARITH } flag_kind = FK_ARITH;
  ILValue flagA = ILNoVal(), flagB = ILNoVal();
  bool flag_imm = false; int64_t flag_immv = 0; int flag_sz = 2;

  // --- stack-pointer tracking (32-bit) -----------------------------------------------------
  // sp_off is ESP now minus ESP at function entry (0 at entry, more negative as the stack grows).
  // Knowing it turns `[esp+N]` into what it is: at an entry-relative offset of 4 or more, an
  // incoming argument; below 0, a local or a pushed value. Without it every function compiled
  // without a frame pointer read its arguments as `*(sp + N)` on an undefined `sp`. It also lets
  // push and pop be modelled as a store to and a load from the slot they touch.
  // If the tracking does not come back to zero at a `ret`, it was wrong somewhere (an unusual
  // stack adjustment): the function is lifted again without it rather than trusted.
  static thread_local bool tl_no_sptrack = false;
  const bool sptrack = bits32_ && regflow && !tl_no_sptrack && !getenv("CB_NO_SPTRACK");
  int sp_off = 0; bool sp_known = sptrack;
  int bp_off = 0; bool bp_known = false;       // EBP = entry ESP + bp_off, once `mov ebp, esp` is seen
  int assumed_pop = 0;                         // argument bytes assumed popped by callees (see call)
  bool sp_bad = false;
  auto stackSlotOf = [&](int e) -> int {       // the tracked-local slot for entry-relative offset e < 0
    if (e >= 0) return -1;
    if (bp_known) { int d = e - bp_off; return d < 0 ? 1000 + (-d) : -1; }   // same slot as [ebp+d]
    return 3000 + (-e);
  };

  // operand -> value (for reg/imm; mem handled by caller via Load)
  auto immVal = [&](int64_t v, int sz) {
    ILValue r = fn.AllocValue(WidthType(sz));
    return r;  // placeholder; imm carried on the instr
  };

  // --- pass 2: lift block by block, in address order ---
  std::vector<uint32_t> leadv(leaders.begin(), leaders.end());
  std::sort(leadv.begin(), leadv.end());

  // Per-block AX snapshot for return-phi recovery (below): the AX value live at
  // each block's END, and whether the block WROTE AX itself. Register slot tags
  // live in `reg`, not on instruction dsts, so this is the only place to capture
  // which value each path would return.
  std::map<uint32_t, ILValue> blkAxOut;
  std::map<uint32_t, bool> blkWroteAx;

  // --- register flow -------------------------------------------------------------------
  // A block's registers and stack locals start from what its PREDECESSORS left, not from
  // whatever the block before it in address order happened to leave. Lifting used to carry one
  // register map straight down the address space, so a block reached by a jump inherited the
  // values of an unrelated block, and a join of two paths kept only the address-last value.
  // Blocks are now lifted in reverse postorder; a block's entry state is its predecessors' exit
  // states where they agree, and a MERGE VALUE where they differ (or where a not-yet-lifted
  // predecessor -- a loop back edge -- may differ). A merge value is assigned at the end of each
  // predecessor by a merge move, the form the IL back-end already coalesces into one variable.
  // CB_NO_REGFLOW=1 restores the old straight-line behaviour.
  const size_t nblk = leadv.size();
  std::map<uint32_t, size_t> blkIdx;
  for (size_t i = 0; i < nblk; ++i) blkIdx[leadv[i]] = i;
  std::vector<std::vector<size_t>> bsucc(nblk), bpred(nblk);
  for (size_t i = 0; i < nblk; ++i) {
    uint32_t bs = leadv[i];
    if (bs >= end) continue;
    uint32_t be = (i + 1 < nblk) ? leadv[i + 1] : end;
    const X86Insn* last = nullptr;
    uint32_t nx = bs;
    for (uint32_t a = bs; a < be;) {
      auto it = at.find(a);
      if (it == at.end()) break;
      last = &insns[it->second];
      a += advBytes(*last);
      nx = a;
    }
    if (!last) continue;
    auto edge = [&](uint32_t t) {
      auto it = blkIdx.find(t);
      if (it == blkIdx.end() || t >= end) return;
      for (size_t s : bsucc[i]) if (s == it->second) return;
      bsucc[i].push_back(it->second);
      bpred[it->second].push_back(i);
    };
    const std::string& lm = last->mnem;
    bool lLoop = lm == "loop" || lm == "loope" || lm == "loopne" || lm == "loopz" || lm == "loopnz";
    bool lJcc = (lm.size() && lm[0] == 'j' && lm != "jmp") || lLoop;
    if (lm == "ret" || lm == "retf" || lm == "iret") {
    } else if (lm == "jmp") {
      if (last->ops[0].kind == OpKind::Rel) edge((uint32_t)last->ops[0].imm);
    } else if (lJcc) {
      if (last->ops[0].kind == OpKind::Rel) edge((uint32_t)last->ops[0].imm);
      edge(nx);
    } else if (nx >= be && at.count(be)) {
      edge(be);                                    // ran off the end of the block: falls through
    }
  }
  // lifting order: reverse postorder from the entry block, then anything unreached, by address
  std::vector<size_t> order;
  std::vector<char> reachable(nblk, 1);
  if (regflow && blkIdx.count(entry)) {
    std::vector<char> seen(nblk, 0);
    std::vector<size_t> post;
    std::vector<std::pair<size_t, size_t>> stk{{blkIdx[entry], 0}};
    seen[blkIdx[entry]] = 1;
    while (!stk.empty()) {
      auto& f = stk.back();
      if (f.second < bsucc[f.first].size()) {
        size_t s = bsucc[f.first][f.second++];
        if (!seen[s]) { seen[s] = 1; stk.push_back({s, 0}); }
      } else {
        post.push_back(f.first);
        stk.pop_back();
      }
    }
    order.assign(post.rbegin(), post.rend());
    for (size_t i = 0; i < nblk; ++i) if (!seen[i]) order.push_back(i);
    reachable = seen;
  } else {
    for (size_t i = 0; i < nblk; ++i) order.push_back(i);
  }
  struct BState {
    std::map<int, ILValue> reg;
    int fk = 0; ILValue fa, fb; bool fimm = false; int64_t fimmv = 0; int fsz = 2;
    std::vector<ILValue> fp;
    int sp = 0; bool spk = false; int bp = 0; bool bpk = false;
  };
  std::vector<BState> bexit(nblk);
  std::vector<char> lifted(nblk, 0), haveBlk(nblk, 0);
  std::vector<ILBasicBlock> blks(nblk);
  struct PMove { size_t pred; ILValue dst; ILValue src; };
  std::vector<PMove> pmoves;                       // merge moves to place at the end of `pred`
  struct PFix { size_t pred; ILValue dst; int slot; };
  std::vector<PFix> pfix;                          // back edges: source known once `pred` is lifted
  auto enterBlock = [&](size_t bi) {
    reg.clear();
    std::vector<size_t> T, U;
    for (size_t p : bpred[bi]) (lifted[p] ? T : U).push_back(p);
    assumed_pop = 0;
    if (T.empty()) {                               // function entry, or reached only by back edges
      flag_kind = FK_ARITH; flagA = ILNoVal(); flagB = ILNoVal();
      flag_imm = false; flag_immv = 0; flag_sz = 2;
      fpst.clear();
      bool isEntry = blkIdx.count(entry) && blkIdx[entry] == bi;
      sp_off = 0; sp_known = sptrack && isEntry; bp_off = 0; bp_known = false;
      return;
    }
    // The stack depth must be the same on every way into a block; if the predecessors
    // disagree the tracking is not usable here.
    sp_off = bexit[T[0]].sp; sp_known = sptrack && bexit[T[0]].spk;
    bp_off = bexit[T[0]].bp; bp_known = bexit[T[0]].bpk;
    for (size_t p : T) {
      if (!bexit[p].spk || bexit[p].sp != sp_off) sp_known = false;
      if (!bexit[p].bpk || bexit[p].bp != bp_off) bp_known = false;
    }
    // Condition flags and the x87 stack are not merged: take them from the fall-through
    // predecessor when there is one (a jcc reads the flags its own block or that one set).
    size_t fpred = T[0];
    for (size_t p : T) if (p + 1 == bi) fpred = p;
    const BState& fs = bexit[fpred];
    flag_kind = (FlagKind)fs.fk; flagA = fs.fa; flagB = fs.fb;
    flag_imm = fs.fimm; flag_immv = fs.fimmv; flag_sz = fs.fsz;
    fpst = fs.fp;
    std::set<int> slots;
    for (size_t p : T) for (const auto& kv : bexit[p].reg) slots.insert(kv.first);
    for (int s : slots) {
      // what each lifted predecessor leaves in this slot: its last write; else, for a register,
      // the function-entry value; else (a stack local it never wrote) nothing
      std::vector<std::pair<size_t, ILValue>> vals;
      ILType ty = ILType::U32;
      for (size_t p : T) {
        auto it = bexit[p].reg.find(s);
        if (it != bexit[p].reg.end()) { ty = it->second.type; break; }
      }
      std::set<int> distinct;
      for (size_t p : T) {
        auto it = bexit[p].reg.find(s);
        if (it != bexit[p].reg.end()) { vals.push_back({p, it->second}); distinct.insert(it->second.id); }
        else if (s < 1000) { ILValue v = slotIncoming(s, ty); vals.push_back({p, v}); distinct.insert(v.id); }
      }
      if (vals.empty()) continue;
      if (U.empty() && distinct.size() == 1) { reg[s] = vals[0].second; continue; }
      ILValue m = fn.AllocValue(ty, s);
      mergeIds.insert(m.id);
      for (const auto& pv : vals) pmoves.push_back({pv.first, m, pv.second});
      for (size_t p : U) pfix.push_back({p, m, s});
      reg[s] = m;
    }
  };

  for (size_t oi = 0; oi < order.size(); ++oi) {
    size_t li = order[oi];
    uint32_t bstart = leadv[li];
    if (bstart >= end) continue;
    uint32_t bend = (li + 1 < leadv.size()) ? leadv[li + 1] : end;
    ILBasicBlock bb;
    bb.start_addr = bstart;
    bb.block_id = (int)li;
    pend_args.clear();   // args are pushed within the block that calls
    esp_args.clear();    // ...and stack-written args likewise don't cross block boundaries
    regConst.clear();    // tracked constants don't survive a control-flow merge; re-derive per block
    if (regflow) enterBlock(li);
    cur_bb = &bb;
    int axInId = reg.count(kGpBase) ? reg[kGpBase].id : kILNoValue;

    for (uint32_t a = bstart; a < bend;) {
      auto iit = at.find(a);
      if (iit == at.end()) break;
      cur_addr = a;
      const X86Insn& in = insns[iit->second];
      uint32_t next = a + advBytes(in);
      const std::string& m = in.mnem;
      if (m == "push" || m == "pushad" || m == "pusha" || m == "pushfd" || m == "pushf" ||
          m == "call" || m == "enter") stack_dirty = true;   // stack no longer at entry depth

      // Stack-pointer bookkeeping for the instructions that move ESP other than push, pop and
      // call (those are handled where they are lifted).
      if (sptrack) {
        const bool dstEsp = in.nops >= 1 && IsGpr(in.ops[0]) && (in.ops[0].reg % 16) == 4 &&
                            in.ops[0].size >= 2;
        if ((m == "sub" || m == "add") && dstEsp && in.nops >= 2 && in.ops[1].kind == OpKind::Imm) {
          int k = (int)in.ops[1].imm;
          if (m == "sub") {
            sp_off -= k;
          } else {
            // `add esp, N` after calls whose arguments were assumed popped by the callee is the
            // CALLER cleaning up (cdecl): that part has been counted already.
            int undo = std::min(k, assumed_pop);
            sp_off += k - undo;
            assumed_pop -= undo;
          }
        } else if (m == "mov" && dstEsp && in.nops >= 2 && IsGpr(in.ops[1]) &&
                   (in.ops[1].reg % 16) == 5) {                        // mov esp, ebp
          if (bp_known) sp_off = bp_off; else sp_known = false;
        } else if (m == "mov" && in.nops >= 2 && IsGpr(in.ops[0]) && (in.ops[0].reg % 16) == 5 &&
                   in.ops[0].size == 4 && IsGpr(in.ops[1]) && (in.ops[1].reg % 16) == 4) {
          bp_off = sp_off; bp_known = sp_known;                        // mov ebp, esp
        } else if (m == "leave") {                                     // mov esp, ebp; pop ebp
          if (bp_known && sp_known) {
            sp_off = bp_off + 4;
            bp_known = false;
            setReg(kGpBase + 5, slotIncoming(kGpBase + 5, ILType::U32));   // caller's EBP again
          } else {
            sp_known = false;
          }
        } else if (m == "pushfd" || m == "pushf") { sp_off -= 4;
        } else if (m == "popfd" || m == "popf") { sp_off += 4;
        } else if (m == "pushad" || m == "pusha") { sp_off -= 32;
        } else if (m == "popad" || m == "popa") { sp_off += 32;
        } else if (m == "enter" || (dstEsp && m != "pop" && m != "push" && m != "cmp" && m != "test")) {
          sp_known = false;                                            // an adjustment we do not model
        }
      }

      auto emitMemAddr = [&](const X86Op& o, ILInstr& ins) {
        // The address of a memory operand, less its displacement (which rides on the instruction
        // as its immediate): base + index*scale. The index used to be dropped, so every indexed
        // access (`[eax+ecx*4]`, a 16-bit `[bx+si]`) read or wrote the base address alone.
        ILValue base = ILNoVal();
        if (o.base != kNoReg) base = getReg(RegSlot(o.base), 2);
        if (o.index != kNoReg) {
          ILType at_ty = bits32_ ? ILType::U32 : ILType::U16;
          ILValue idx = getReg(RegSlot(o.index), 2);
          if (o.scale > 1) {
            ILInstr sh; sh.op = ILOp::Shl; sh.address = a; sh.src1 = idx;
            sh.imm = (o.scale == 2) ? 1 : (o.scale == 4) ? 2 : 3;
            sh.dst = fn.AllocValue(at_ty);
            bb.instrs.push_back(sh);
            idx = sh.dst;
          }
          if (base.id == kILNoValue) {
            base = idx;
          } else {
            ILInstr ad; ad.op = ILOp::Add; ad.address = a; ad.src1 = base; ad.src2 = idx;
            ad.dst = fn.AllocValue(at_ty);
            bb.instrs.push_back(ad);
            base = ad.dst;
          }
        }
        ins.imm = o.disp;
        ins.access_type = WidthType(o.size);
        return base;
      };

      // A [bp - N] operand (base=BP, no index, negative disp) is a stack local.
      // Give each distinct offset its own virtual slot (base 1000, off the GPR
      // 20-27, param r3.., and frame ranges) so loads/stores become tracked SSA
      // locals -- rendered as named vNN, not raw *(r25 - N) memory refs.
      // [esp + N] with the stack depth known: its entry-relative offset.
      auto espRel = [&](const X86Op& o, int* e) -> bool {
        if (!sp_known || o.kind != OpKind::Mem || o.base == kNoReg || (o.base % 16) != 4 ||
            o.index != kNoReg)
          return false;
        *e = sp_off + o.disp;
        return true;
      };
      auto frameSlotOf = [&](const X86Op& o) -> int {
        if (o.kind == OpKind::Mem && o.base != kNoReg && (o.base % 16) == 5 &&
            o.index == kNoReg && o.disp < 0)
          return 1000 + (-o.disp);
        int e;
        if (espRel(o, &e) && e < 0) return stackSlotOf(e);     // a local or pushed value
        return -1;
      };

      // AH/CH/DH/BH are encoded as 8-bit registers 4..7, the same numbers as SP/BP/SI/DI. They are
      // the second byte of AX/CX/DX/BX: treating them as slots 4..7 made `mov ah, x` overwrite SP
      // and `mov bh, x` overwrite DI.
      auto isHi8 = [](const X86Op& o) {
        return o.kind == OpKind::Reg && o.reg / 16 == RC_GPR && o.size == 1 &&
               (o.hi8 || (o.reg % 16) >= 4);
      };
      // slot of the full register a high-byte register belongs to (the Capstone decoder reports
      // AH as register 0 with hi8 set; the native one as register 4)
      auto hi8Slot = [](const X86Op& o) { return kGpBase + (o.hi8 ? (o.reg % 16) : (o.reg % 16) - 4); };
      auto constVal = [&](int32_t k, ILType t) -> ILValue {
        ILInstr c; c.op = ILOp::Assign; c.address = a; c.imm = k;
        c.dst = fn.AllocValue(t);
        bb.instrs.push_back(c);
        return c.dst;
      };
      auto srcValue = [&](const X86Op& o) -> ILValue {
        if (isHi8(o)) {                              // (full >> 8) & 0xFF
          ILValue full = getReg(hi8Slot(o), 2);
          ILInstr sh; sh.op = ILOp::Shr; sh.address = a; sh.src1 = full; sh.imm = 8;
          sh.dst = fn.AllocValue(ILType::U32); bb.instrs.push_back(sh);
          ILInstr an; an.op = ILOp::And; an.address = a; an.src1 = sh.dst;
          an.src2 = constVal(0xFF, ILType::U32);
          an.dst = fn.AllocValue(ILType::U8); bb.instrs.push_back(an);
          return an.dst;
        }
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
            paramIds.insert(pv.id);
            return pv;
          }
          // The same parameter reached through ESP (no frame pointer): entry-relative offset 4
          // is the first argument, just above the return address.
          int pe;
          if (espRel(o, &pe) && pe >= 4 && (pe % 4) == 0 && pe < 4 + 4 * 32) {
            int pidx = (pe - 4) / 4;
            auto pit = params.find(pidx);
            if (pit != params.end()) return pit->second;
            ILValue pv = fn.AllocValue(WidthType(o.size), 3 + pidx);
            params[pidx] = pv;
            paramIds.insert(pv.id);
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
        if (isHi8(dst)) {                            // full = (full & ~0xFF00) | ((v & 0xFF) << 8)
          int slot = hi8Slot(dst);
          ILType ft = bits32_ ? ILType::U32 : ILType::U16;
          ILValue full = getReg(slot, 2);
          ILInstr keep; keep.op = ILOp::And; keep.address = a; keep.src1 = full;
          keep.src2 = constVal(bits32_ ? (int32_t)0xFFFF00FF : 0x00FF, ft);
          keep.dst = fn.AllocValue(ft); bb.instrs.push_back(keep);
          ILInstr lo; lo.op = ILOp::And; lo.address = a; lo.src1 = v;
          lo.src2 = constVal(0xFF, ft);
          lo.dst = fn.AllocValue(ft); bb.instrs.push_back(lo);
          ILInstr sh; sh.op = ILOp::Shl; sh.address = a; sh.src1 = lo.dst; sh.imm = 8;
          sh.dst = fn.AllocValue(ft); bb.instrs.push_back(sh);
          ILInstr o; o.op = ILOp::Or; o.address = a; o.src1 = keep.dst; o.src2 = sh.dst;
          o.dst = fn.AllocValue(ft); bb.instrs.push_back(o);
          setReg(slot, o.dst);
          return;
        }
        if (IsGpr(dst)) { setReg(RegSlot(dst.reg), v); return; }
        if (dst.kind == OpKind::Mem) {
          int fs = frameSlotOf(dst);
          // A write just above the stack top may be an outgoing argument set up with `mov`
          // instead of `push`; keep it as a candidate argument as well as a tracked slot.
          if (fs >= 0 && bits32_ && dst.base != kNoReg && (dst.base % 16) == 4 &&
              dst.index == kNoReg && dst.disp >= 0 && dst.disp < 256)
            esp_args[dst.disp] = v;
          if (fs >= 0) { setReg(fs, v); return; }   // [bp - N] / [esp + N] local <- v
          // 32-bit outgoing argument written directly to the stack: `mov [esp+N], v` (optimized MSVC
          // sets up call args this way instead of `push`). Record it by offset as a pending arg
          // instead of a store; consumed at the next call in offset order ([esp+0]=arg0). This is the
          // call-site half of arity recovery -- without it, mov-style calls emit `f()` (no args).
          if (bits32_ && dst.base != kNoReg && (dst.base % 16) == 4 &&
              dst.index == kNoReg && dst.disp >= 0 && dst.disp < 256) {
            esp_args[dst.disp] = v;
            return;
          }
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
        if (m == "mov" && in.ops[1].kind == OpKind::Mem && in.ops[1].base == kNoReg &&
            in.ops[1].index == kNoReg) {
          auto is = import_slots_.find((uint32_t)in.ops[1].disp);
          if (is != import_slots_.end()) importVal[v.id] = is->second;
        }
        storeTo(in.ops[0], v);                          // storeTo->setReg cleared any old constant
        if (IsGpr(in.ops[0]) && in.ops[1].kind == OpKind::Imm)
          regConst[RegSlot(in.ops[0].reg)] = in.ops[1].imm;   // mov reg, imm  -> track the constant
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
        // reg = base + index*scale + disp. With no base or index it is a plain constant (an Add
        // with no left operand renders as its immediate).
        ILInstr x; x.op = ILOp::Add; x.address = a;
        x.src1 = emitMemAddr(in.ops[1], x);              // also sets x.imm = disp
        x.access_type = ILType::U32;
        x.dst = fn.AllocValue(bits32_ ? ILType::U32 : ILType::U16);
        bb.instrs.push_back(x);
        storeTo(in.ops[0], x.dst);
      } else if ((m == "xor" || m == "sub") && IsGpr(in.ops[0]) && IsGpr(in.ops[1]) &&
                 in.ops[0].reg == in.ops[1].reg && in.ops[0].size == in.ops[1].size) {
        // `xor r, r` / `sub r, r` is the zeroing idiom: it does not READ the register. Lifting it
        // as r ^ r made the incoming register look used, which invented a register parameter on
        // about a third of all functions.
        ILInstr z; z.op = ILOp::Assign; z.address = a; z.imm = 0;
        z.dst = fn.AllocValue(WidthType(in.ops[0].size));
        bb.instrs.push_back(z);
        storeTo(in.ops[0], z.dst);
        flag_kind = FK_ARITH; flagA = z.dst; flag_imm = true; flag_immv = 0;
        flag_sz = in.ops[0].size;
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
      else if (m == "inc" || m == "dec") {
        // inc/dec set the zero and sign flags from their result (only carry is left alone), so a
        // following jz/jnz/js tests THIS result. Without recording that, `dec ecx; jnz loop`
        // branched on whatever arithmetic came before it.
        ILInstr x; x.op = (m == "inc") ? ILOp::Add : ILOp::Sub; x.address = a;
        x.src1 = srcValue(in.ops[0]); x.imm = 1;
        x.dst = fn.AllocValue(WidthType(in.ops[0].size));
        bb.instrs.push_back(x); storeTo(in.ops[0], x.dst);
        flag_kind = FK_ARITH; flagA = x.dst; flag_imm = true; flag_immv = 0; flag_sz = in.ops[0].size;
      }
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
        } else if (in.ops[0].kind == OpKind::Reg &&
                   regConst.count(RegSlot(in.ops[0].reg))) {
          // DATA-FLOW: the called register holds a constant address (`mov reg, offset FUNC; call reg`)
          // -- resolve the indirect call to that direct target instead of the opaque `indirect`.
          uint32_t t = (uint32_t)regConst[RegSlot(in.ops[0].reg)];
          auto nit = names_.find(t);
          if (nit != names_.end()) { x.target = t; x.call_name = nit->second; }      // known function
          else if (t >= 0x1000 && t < 0x10000000u) {                                 // plausible code addr
            x.target = t; char sb[24]; std::snprintf(sb, sizeof sb, "sub_%06X", t); x.call_name = sb;
          } else x.call_name = "indirect";
        } else {
          // A call through a register or a memory cell. Keep the pointer it calls through, so the
          // back-end can write the call (`((int (*)())p)(...)`) instead of a placeholder that
          // loses where control goes. `mask` records who removes the arguments: an `add esp, N`
          // right after the call means the caller does (cdecl); otherwise the callee (stdcall).
          x.call_name = "indirect";
          if (bits32_ && (in.ops[0].kind == OpKind::Reg || in.ops[0].kind == OpKind::Mem)) {
            x.src1 = srcValue(in.ops[0]);
            auto iv = importVal.find(x.src1.id);
            bool callerPops = false;
            if (iit->second + 1 < insns.size()) {
              const X86Insn& nx = insns[iit->second + 1];
              callerPops = nx.mnem == "add" && nx.nops >= 2 && nx.ops[0].kind == OpKind::Reg &&
                           IsGpr(nx.ops[0]) && (nx.ops[0].reg % 16) == 4 && nx.ops[1].kind == OpKind::Imm;
            }
            x.mask = callerPops ? 2u : 1u;
            if (iv != importVal.end()) {               // the pointer is an import: call it by name
              x.call_name = iv->second; x.src1 = ILValue(); x.mask = 0;
            }
          }
        }
        // If this call's args were written to the stack (`mov [esp+N],v`) rather than pushed,
        // pend_args is empty -- pull them from esp_args in ascending-offset order ([esp+0]=arg0).
        size_t npushed = pend_args.size();
        // How many of the pushed values are THIS call's arguments. A stdcall callee says so
        // itself (`ret N` -> N/4); for a caller-cleans call the `add esp, N` after it does.
        // Values pushed earlier than that belong to a later call and stay pending. Without this
        // a call took every value on the list, so callers and callees disagreed on argument
        // counts all over a module.
        std::vector<ILValue> later_args;
        int known_pop = -1;
        if (bits32_ && in.ops[0].kind == OpKind::Rel && cs == call_sites_.end()) {
          known_pop = CalleePop((uint32_t)in.ops[0].imm);
          int want = -1;
          if (known_pop > 0) want = known_pop / 4;
          else if (known_pop == 0 && iit->second + 1 < insns.size()) {
            const X86Insn& nx = insns[iit->second + 1];
            if (nx.mnem == "add" && nx.nops >= 2 && IsGpr(nx.ops[0]) && (nx.ops[0].reg % 16) == 4 &&
                nx.ops[1].kind == OpKind::Imm)
              want = (int)nx.ops[1].imm / 4;
            else
              want = 0;      // plain `ret` and nothing cleaned up after the call: no stack arguments
          }
          if (want >= 0 && npushed > (size_t)want) {
            later_args.assign(pend_args.begin(), pend_args.end() - want);
            pend_args.erase(pend_args.begin(), pend_args.end() - want);
            npushed = pend_args.size();
          }
        }
        if (pend_args.empty() && !esp_args.empty()) {
          for (auto& kv : esp_args) pend_args.push_back(kv.second);   // std::map iterates by key asc
        }
        esp_args.clear();
        // 32-bit conventions (stdcall, cdecl) push the LAST argument first, so the first argument
        // is the last value pushed. Win16 Pascal pushes left to right, which is the order below
        // without reversal. Staging 32-bit arguments in push order passed every call's arguments
        // backwards.
        if (bits32_ && npushed > 1) std::reverse(pend_args.begin(), pend_args.end());
        if (sp_known && npushed) {
          // Assume the callee removes its arguments (stdcall, the Win32 API convention). If the
          // caller does it instead, the `add esp, N` that follows is recognised and not counted
          // twice. The C runtime and wsprintf are cdecl: nothing is assumed for them.
          const std::string& cn = x.call_name;
          bool cdeclCallee = cn.find("CRTDLL") != std::string::npos ||
                             cn.find("MSVCRT") != std::string::npos ||
                             cn.find("wsprintf") != std::string::npos;
          if (known_pop > 0) sp_off += known_pop;            // exact: the callee's own `ret N`
          else if (known_pop == 0) { /* the caller cleans up; its `add esp` is counted when seen */ }
          else if (!cdeclCallee) { sp_off += 4 * (int)npushed; assumed_pop += 4 * (int)npushed; }
        }
        // stage pushed args into the arg registers r3.. (PASCAL push order); the
        // back-end reads r3..r10 as the call's arguments.
        for (size_t i = 0; i < pend_args.size() && i < 8; ++i) {
          ILInstr ar; ar.op = ILOp::Assign; ar.address = a; ar.src1 = pend_args[i];
          ar.dst = fn.AllocValue(pend_args[i].type, 3 + (int)i);
          bb.instrs.push_back(ar);
        }
        // Tell the back-end exactly how many arguments this call has. It otherwise takes every
        // argument register that holds a value -- and this function's OWN parameters live in
        // those registers, so a call with no arguments was printed passing the caller's a1.
        if (bits32_) x.imm = 0x100 + (int)std::min<size_t>(pend_args.size(), 8);
        pend_args = later_args;
        x.dst = fn.AllocValue(ILType::U32, kGpBase);   // DX:AX (long returns); AX truncates on 16-bit use
        bb.instrs.push_back(x);
        setReg(kGpBase, x.dst);
        // DX = high word of the return (for long-returning calls like _llseek);
        // dropped by DCE when the caller only uses AX.
        ILInstr hi; hi.op = ILOp::Shr; hi.address = a; hi.src1 = x.dst; hi.imm = 16;
        hi.dst = fn.AllocValue(ILType::U16, kGpBase + 2); bb.instrs.push_back(hi);
        setReg(kGpBase + 2, hi.dst);
      } else if (m == "ret" || m == "retf") {
        if (sptrack && sp_known && sp_off != 0) sp_bad = true;   // the stack model did not balance
        {
          int popn = (in.nops > 0 && in.ops[0].kind == OpKind::Imm) ? (int)in.ops[0].imm : 0;
          if (fn.callee_pop == -1) fn.callee_pop = popn;
          else if (fn.callee_pop != popn) fn.callee_pop = -2;
        }
        ILInstr x; x.op = ILOp::Return; x.address = a;
        // If the body WROTE AX, that is the return value (x86 returns in AX). Promote it into r3 so
        // the back-end renders `return <value>` -- the back-end's return register is r3, which x86's
        // AX slot 20 does not otherwise feed, so without this a value-computing function renders a
        // void `return;` and its whole AX-producing chain is dead-code-eliminated (sbb/neg boolean
        // idioms, xchg old-value returns, etc). Guarded on AX having been written in the body, so a
        // function that never touches AX stays void.
        auto it = reg.find(kGpBase);
        if (it != reg.end()) {
          ILInstr cp; cp.op = ILOp::Assign; cp.address = a; cp.src1 = it->second;
          cp.dst = fn.AllocValue(ILType::U32, 3);
          bb.instrs.push_back(cp);
          x.src1 = cp.dst;
        }
        bb.instrs.push_back(x);
        bb.is_return = true;
      } else if (m == "jmp" && in.ops[0].kind == OpKind::Reg && IsGpr(in.ops[0]) &&
                 RegSlot(in.ops[0].reg) == retaddr_reg) {
        // `jmp reg` on the entry-popped return address == RETURN (value in AX). The shared
        // stdcall-cleanup return trampoline (pop edx; lea esp,[esp+eax*4]; mov eax,V; jmp edx) --
        // without this it mis-lifts to an indirect branch to sub_000000. AX is unambiguously the
        // return value here, so promote it into r3 (the back-end's return register, which x86's AX
        // slot 20 does not otherwise feed) so the body renders `return V` instead of a void `return;`.
        ILInstr x; x.op = ILOp::Return; x.address = a;
        auto it = reg.find(kGpBase);
        if (it != reg.end()) {
          ILInstr cp; cp.op = ILOp::Assign; cp.address = a; cp.src1 = it->second;
          cp.dst = fn.AllocValue(ILType::U32, 3);
          bb.instrs.push_back(cp);
          x.src1 = cp.dst;
        }
        bb.instrs.push_back(x);
        bb.is_return = true;
      } else if (m == "jmp") {
        ILInstr x; x.op = ILOp::Branch; x.address = a;
        if (in.ops[0].kind == OpKind::Rel) { x.target = (uint32_t)in.ops[0].imm; bb.successors.push_back(x.target); }
        // A tail jmp into another function is emitted as `T(); return;`. Resolve T's name the same way
        // a call does -- far jmp via the relocation call_sites_, near via names_ -- so the tail-call
        // name matches the DEFINITION's name (both come from the names map). Without this the tail call
        // fell back to sub_<addr> while the body was defined under its real name (e.g. PEEKMESSAGE32's
        // shared body) -> an undefined reference. Harmless for an intra-function goto: the emitter uses
        // call_name only when the branch actually leaves the function.
        {
          auto cs = call_sites_.find(a);
          if (cs != call_sites_.end()) { x.target = cs->second.first; x.call_name = cs->second.second; }
          else { auto nit = names_.find(x.target); if (nit != names_.end()) x.call_name = nit->second; }
        }
        // Register-convention tail call: a dispatch thunk that sets an argument register then jumps to
        // a shared worker (`mov eax,<idx> ; jmp worker`) passes that register AS the worker's argument.
        // Stage the x86 GPR arg registers that hold a CONSTANT, in Watcom __watcall order (AX,DX,BX,CX),
        // into r3.. so the tail call emits `worker(<idx>)` instead of dropping the value -- matching the
        // worker's register param (the decompiler_c register live-in promotion). Gated on a constant
        // (mov reg,imm), taken contiguously from AX, and only when the jmp LEAVES the function (a tail
        // call, not an intra-function goto -- the worker is often unnamed so call_name is empty here).
        // Must run before the Branch is pushed so the args precede it.
        if (in.ops[0].kind == OpKind::Rel && (x.target < entry || x.target >= end)) {
          static const int kArgReg[4] = {kGpBase + 0, kGpBase + 2, kGpBase + 3, kGpBase + 1};  // AX,DX,BX,CX
          for (int p = 0; p < 4; p++) {
            if (!regConst.count(kArgReg[p])) break;      // contiguous constants from AX
            auto rv = reg.find(kArgReg[p]);
            if (rv == reg.end()) break;
            ILInstr ar; ar.op = ILOp::Assign; ar.address = a;
            ar.src1 = rv->second; ar.dst = fn.AllocValue(rv->second.type, 3 + p);
            bb.instrs.push_back(ar);
          }
        }
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
        { auto nit = names_.find(br.target);   // conditional tail branch: name it like the definition
          if (nit != names_.end()) br.call_name = nit->second; }
        bb.instrs.push_back(br);
        bb.successors.push_back(br.target);
        bb.successors.push_back(next);   // fallthrough
      } else if (m == "push") {
        // An outgoing argument, unless it is a prologue save: push of an untouched
        // incoming callee-saved reg (bp/si/di) or a segment reg.
        const X86Op& o = in.ops[0];
        bool save = false;
        ILValue pv;                                  // the value pushed (none for a segment register)
        if (IsGpr(o) && !isHi8(o)) {
          int idx = o.reg % 16;
          pv = getReg(RegSlot(o.reg), 2);
          // EBX is callee-saved in 32-bit code too (in Win16, BX is scratch)
          bool calleeSaved = idx == 5 || idx == 6 || idx == 7 || (bits32_ && idx == 3);
          if (calleeSaved && incoming.count(pv.id)) save = true;
        } else if (o.kind == OpKind::Reg && o.reg / 16 == RC_SEG) {
          save = true;
        } else {
          pv = srcValue(o);
        }
        if (!save) pend_args.push_back(pv);
        if (sp_known) {                              // the push is also a store to the new stack top
          sp_off -= 4;
          int s = stackSlotOf(sp_off);
          if (s >= 0 && pv.id != kILNoValue) setReg(s, pv);
        }
      } else if (m == "pop") {
        // An entry `pop reg` (stack still at entry depth) loads the caller's return address; mark it
        // so a later `jmp reg` on it is recognised as a return. A later pop of that reg clears it.
        if (in.nops >= 1 && IsGpr(in.ops[0]) && !isHi8(in.ops[0])) {
          int sl = RegSlot(in.ops[0].reg);
          if (!stack_dirty) retaddr_reg = sl;
          else if (sl == retaddr_reg) retaddr_reg = -1;
          if (sp_known) {
            // The register receives what is on the stack top. `pop` used to leave the register
            // unchanged, so a value saved with push and restored with pop was lost, and a
            // callee-saved register kept its in-function value after being restored.
            int s = stackSlotOf(sp_off);
            auto it = s >= 0 ? reg.find(s) : reg.end();
            if ((in.ops[0].reg % 16) == 5 && bp_known && sp_off == bp_off) {
              bp_known = false;                      // pop ebp: the frame's saved EBP
              setReg(sl, slotIncoming(sl, ILType::U32));
            } else if (it != reg.end()) {
              ILValue v = it->second;
              // a value popped back is no longer waiting to be a call argument
              if (!pend_args.empty() && pend_args.back().id == v.id) pend_args.pop_back();
              setReg(sl, v);
            }
          }
        }
        if (sp_known) sp_off += 4;
      } else if (m == "nop" || m == "leave" || m == "enter" ||
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
    if (reg.count(kGpBase)) {
      blkAxOut[bb.start_addr] = reg[kGpBase];
      blkWroteAx[bb.start_addr] = (reg[kGpBase].id != axInId);   // AX changed within THIS block
    }
    cur_bb = nullptr;
    BState& ex = bexit[li];
    ex.reg = reg;
    ex.fk = (int)flag_kind; ex.fa = flagA; ex.fb = flagB;
    ex.fimm = flag_imm; ex.fimmv = flag_immv; ex.fsz = flag_sz;
    ex.fp = fpst;
    ex.sp = sp_off; ex.spk = sp_known; ex.bp = bp_off; ex.bpk = bp_known;
    if (sptrack && reachable[li] && !sp_known) sp_bad = true;   // lost track of the stack here
    lifted[li] = 1;
    blks[li] = std::move(bb);
    haveBlk[li] = 1;
  }

  if (sptrack && sp_bad) {
    // The stack model failed for this function. Mixing tracked slots with raw memory accesses
    // would be worse than either, so lift it again the plain way.
    tl_no_sptrack = true;
    ILFunction plain = LiftFunction(image, image_size, entry, end, name);
    tl_no_sptrack = false;
    return plain;
  }

  if (regflow) {
    // Back edges: the predecessor has been lifted now, so its value for the slot is known.
    for (const PFix& f : pfix) {
      if (!lifted[f.pred]) continue;
      auto it = bexit[f.pred].reg.find(f.slot);
      if (it != bexit[f.pred].reg.end()) pmoves.push_back({f.pred, f.dst, it->second});
      else if (f.slot < 1000) pmoves.push_back({f.pred, f.dst, slotIncoming(f.slot, f.dst.type)});
    }
    // A merge value that every predecessor assigns the SAME value (or itself, round a loop that
    // never changes it) is not a merge at all: replace it by that value. Loop headers create a
    // merge for every live slot up front, so this is what leaves only the loop-carried ones.
    std::map<int, ILValue> subst;
    auto resolve = [&](ILValue v) {
      for (int guard = 0; guard < 256; ++guard) {
        auto it = subst.find(v.id);
        if (it == subst.end()) break;
        v = it->second;
      }
      return v;
    };
    for (bool changed = true; changed;) {
      changed = false;
      std::map<int, std::set<int>> srcIds;
      std::map<int, ILValue> anySrc;
      for (const PMove& mv : pmoves) {
        if (subst.count(mv.dst.id)) continue;
        ILValue s = resolve(mv.src);
        if (s.id == mv.dst.id) continue;
        srcIds[mv.dst.id].insert(s.id);
        anySrc[mv.dst.id] = s;
      }
      for (const auto& kv : srcIds) {
        if (kv.second.size() == 1) { subst[kv.first] = anySrc[kv.first]; changed = true; }
      }
    }
    if (!subst.empty()) {
      auto fix = [&](ILValue& v) {
        if (v.id != kILNoValue && subst.count(v.id)) { ILValue r = resolve(v); v.id = r.id; v.type = r.type; }
      };
      for (size_t i = 0; i < nblk; ++i) {
        if (!haveBlk[i]) continue;
        for (ILInstr& x : blks[i].instrs) { fix(x.src1); fix(x.src2); fix(x.src3); }
      }
      for (auto& kv : blkAxOut) fix(kv.second);
    }
    // A merge value nothing reads is dead, and so are the moves that feed it. Dropping them
    // matters beyond tidiness: a move from a register no path has written reads its function-entry
    // value, and a read of an entry register is what makes it a parameter.
    std::set<int> used;
    for (size_t i = 0; i < nblk; ++i) {
      if (!haveBlk[i]) continue;
      for (const ILInstr& x : blks[i].instrs)
        for (const ILValue* v : {&x.src1, &x.src2, &x.src3})
          if (v->id != kILNoValue) used.insert(v->id);
    }
    std::set<int> liveMerge;
    for (bool grow = true; grow;) {
      grow = false;
      for (const PMove& mv : pmoves) {
        if (subst.count(mv.dst.id)) continue;
        if (!used.count(mv.dst.id) && !liveMerge.count(mv.dst.id)) continue;
        ILValue s = resolve(mv.src);
        if (mergeIds.count(s.id) && !used.count(s.id) && liveMerge.insert(s.id).second) grow = true;
      }
    }
    // Place the surviving merge moves at the end of each predecessor, ahead of its terminator.
    auto isTerm = [](ILOp op) {
      return op == ILOp::Branch || op == ILOp::BranchCond || op == ILOp::Return ||
             op == ILOp::BranchLR || op == ILOp::BranchCTR;
    };
    std::map<size_t, std::vector<std::pair<ILValue, ILValue>>> perPred;   // pred -> [(dst, src)]
    for (const PMove& mv : pmoves) {
      if (subst.count(mv.dst.id) || !haveBlk[mv.pred]) continue;
      if (!used.count(mv.dst.id) && !liveMerge.count(mv.dst.id)) continue;      // dead merge
      ILValue s = resolve(mv.src);
      if (s.id == mv.dst.id) continue;
      auto& lst = perPred[mv.pred];
      bool dup = false;
      for (const auto& e : lst) if (e.first.id == mv.dst.id) { dup = true; break; }
      if (!dup) lst.push_back({mv.dst, s});
    }
    for (auto& pp : perPred) {
      auto& ins = blks[pp.first].instrs;
      size_t pos = ins.size();
      while (pos > 0 && isTerm(ins[pos - 1].op)) pos--;
      // the r3 promotion copy that precedes a Return belongs with the terminator
      uint32_t at_addr = pos > 0 ? ins[pos - 1].address
                                 : (ins.empty() ? blks[pp.first].start_addr : ins[0].address);
      std::set<int> dsts;
      for (const auto& e : pp.second) dsts.insert(e.first.id);
      std::vector<ILInstr> add;
      // The moves of one edge happen "at once". If one reads a merge value another writes
      // (two variables exchanging values round a loop), read it into a temporary first.
      for (auto& e : pp.second) {
        if (dsts.count(e.second.id)) {
          ILInstr t; t.op = ILOp::Assign; t.address = at_addr; t.src1 = e.second;
          t.dst = fn.AllocValue(e.second.type);
          add.push_back(t);
          e.second = t.dst;
        }
      }
      for (const auto& e : pp.second) {
        ILInstr mv; mv.op = ILOp::Assign; mv.merge_move = true; mv.address = at_addr;
        mv.dst = e.first; mv.src1 = e.second;
        add.push_back(mv);
      }
      ins.insert(ins.begin() + pos, add.begin(), add.end());
    }
  }
  for (size_t i = 0; i < nblk; ++i)
    if (haveBlk[i]) fn.blocks.push_back(std::move(blks[i]));

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

  // --- return-phi recovery -------------------------------------------------
  // The register map above is function-global and lifted in ADDRESS order, so a
  // single `ret` reached from several paths only sees the address-previous
  // path's AX. When paths carry DIFFERENT return values into one shared `ret`
  // (a phi of returns), that collapses to one value and the other paths' AX
  // computations are dead-code-eliminated. Recover it: a block that reaches a
  // *bare* ret block (one that returns the incoming AX unchanged -- it writes
  // no AX of its own) by an explicit BRANCH, while holding its own AX, gets its
  // own `return <that AX>`. Fall-through into the shared ret still returns via
  // the linear promotion, so only the branch-reached paths need splitting.
  // (kernel32 GetLastError: the `jmp`-reached per-thread-error path had its
  // `mov eax,[eax+0x70]` dropped and returned the global error like the
  // fall-through path; now each path returns its own value.)
  {
    constexpr int kAX = kGpBase;                       // x86 EAX slot
    // bare-ret block = is_return and does NOT write AX of its own (returns the
    // incoming AX). A block's AX writes are tracked in blkWroteAx; its promotion
    // cp to r3 (ppc_reg 3) is not an AX write, so a `ret`-only block is bare.
    auto isBareRet = [&](const ILBasicBlock& b) {
      auto w = blkWroteAx.find(b.start_addr);
      return b.is_return && !(w != blkWroteAx.end() && w->second);
    };
    std::set<uint32_t> bareRet;
    for (const auto& b : fn.blocks) if (isBareRet(b)) bareRet.insert(b.start_addr);
    if (!bareRet.empty()) {
      for (auto& b : fn.blocks) {
        if (b.instrs.empty() || isBareRet(b)) continue;
        auto wit = blkWroteAx.find(b.start_addr);
        if (wit == blkWroteAx.end() || !wit->second) continue;   // no own AX: returns incoming, no divergence
        ILValue axval = blkAxOut[b.start_addr];        // this block's live AX-out (the value it returns)
        ILInstr& term = b.instrs.back();
        if (axval.id == kILNoValue) continue;
        if (term.op == ILOp::Branch && bareRet.count(term.target)) {
          // unconditional jmp -> this block returns its own AX
          uint32_t at = term.address;
          b.instrs.pop_back();
          ILInstr cp; cp.op = ILOp::Assign; cp.address = at; cp.src1 = axval;
          cp.dst = fn.AllocValue(ILType::U32, 3);      // promote into the return register
          b.instrs.push_back(cp);
          ILInstr ret; ret.op = ILOp::Return; ret.address = at; ret.src1 = cp.dst;
          b.instrs.push_back(ret);
          b.is_return = true; b.successors.clear();
        } else if (term.op == ILOp::BranchCond && bareRet.count(term.target)) {
          // conditional jcc -> `if (cond) return <AX>;` then fall through. Carry
          // the value in src3 (src1/src2/cmp_ref hold the condition) and let the
          // back-end render it; no r3 promotion, so the fall-through path's
          // return is never polluted. src3 is use-counted, so the AX def survives.
          term.is_cond_return = true;
          term.src3 = axval;
          std::vector<uint32_t> keep;                  // drop the taken (ret) edge, keep fall-through
          for (uint32_t s : b.successors) if (s != term.target) keep.push_back(s);
          b.successors = keep;
        }
      }
    }
  }

  fn.total_instructions = (int)insns.size();
  return fn;
}

}  // namespace win95
