#!/usr/bin/env python3
"""mc2inv.py - INVERSE of mc2model: MC2/LC2 output bytes -> C, by recognizing the
exact codegen templates (not heuristics). Now recovers if/while control flow via a
recursive block walker over branch structure. Prototype for the Compiler-Inverse
Engine's bytes->quads->C. Validated by round-trip (recovered C recompiles identical).

Usage: python mc2inv.py <image.bin> <hexoff> <len>
"""
import sys, re, os
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from src.engines.inverse import framemodel
_MD = Cs(CS_ARCH_X86, CS_MODE_16)

ALU={"add":"+","sub":"-","and":"&","or":"|","xor":"^"}
JCC={"jl":"<","jle":"<=","jg":">","jge":">=","je":"==","jne":"!=",      # jump's OWN taken condition
      "ja":">","jae":">=","jb":"<","jbe":"<=","jc":"<","jnc":">="}       # unsigned (signedness is an
                                                                         # approximation; size/count cmps
                                                                         # compare small positive values)
INV={"jge":"<","jg":"<=","jle":">","jl":">=","jne":"==","je":"!=","jz":"!=","jnz":"==",  # skip/exit-if-not
     "ja":"<=","jae":"<","jb":">=","jbe":">","jc":">=","jnc":"<"}        # unsigned, negated (fall-through)
CJMP=set(JCC)|{"jz","jnz","js","jns"}
BYTEREG=("al","bl","cl","dl","ah","bh","ch","dh")        # 8-bit sub-registers (exposed as vars)
BYTEPARENT={"al":"ax","ah":"ax","bl":"bx","bh":"bx","cl":"cx","ch":"cx","dl":"dx","dh":"dx"}
WORDHALVES={"ax":("al","ah"),"bx":("bl","bh"),"cx":("cl","ch"),"dx":("dl","dh")}
TRACKED_BYTE={"mov","add","sub","or","and","xor","inc","dec","shl","sal","shr","sar","neg","not"}   # byte-reg ops tracked as breg values

# Optional driver hook: CALLINFO(call_addr_0based, "near"|"far", target) -> number of bytes the
# CALLEE pops on return (`ret N`: N>0 = Pascal convention, args pushed left-to-right and N/2
# words consumed), 0 for a plain `ret` (C convention), or None when unknown.
CALLINFO=None
# Verification aid: when True, byte-wide memory operands are emitted as BYTE(x) so a checker can
# tell 8-bit from 16-bit accesses. Off for normal (readable) output.
MARK=False
DEADREGS=os.environ.get("MC2INV_DEADREGS","0")=="1"     # drop register assignments nothing reads
UNSIGNED_CAST=os.environ.get("MC2INV_UNSIGNED","0")=="1"   # readable listings: mark unsigned compares
# Optional driver fact: does the function being lifted return a value? (True/False from a
# whole-program "is the result ever read by a caller" analysis; None -> local heuristic.)
RETVAL=None
# Optional driver hook: SWITCH(addr_0based) -> case-target addresses (0-based, table order) of the
# jump-table dispatch `jmp cs:[bx+table]` at that address, or None.
SWITCH=None

def _decode(code):
    """Instruction list for a function. Linear sweep, unless it dispatches through a jump table:
    the table (and anything after it) would be swept as garbage, so follow control flow instead
    (fall-through, direct jumps, table targets) and decode only reachable bytes."""
    lin=list(_MD.disasm(code,0))
    if SWITCH is None or not any(i.mnemonic=="jmp" and "[" in i.op_str for i in lin): return lin
    seen={}; work=[0]
    while work:
        a=work.pop()
        while 0<=a<len(code) and a not in seen:
            ii=next(_MD.disasm(code[a:a+16],a),None)
            if ii is None: break
            seen[a]=ii; mn=ii.mnemonic
            m=re.match(r"(0x[0-9a-f]+|\d+)$", ii.op_str or "")
            if mn in ("ret","retf"): break
            if mn=="jmp":
                if m: work.append(int(m.group(1),0))
                else:
                    try: work.extend(SWITCH(a) or [])
                    except Exception: pass
                break
            if mn in CJMP and m: work.append(int(m.group(1),0))
            a+=ii.size
    return [seen[a] for a in sorted(seen)]

_WREG=("ax","bx","cx","dx","si","di")
def _memparts(op):
    """('word'|'byte', base_text, signed_offset) of a memory operand, else None."""
    m=re.match(r"(word|byte) ptr (?:\w\w:)?\[(.*)\]$", op.strip())
    if not m: return None
    inner=m.group(2)
    m2=re.match(r"(.*?)(?: ([+-]) )?(0x[0-9a-f]+|\d+)$", inner)
    if m2 and (m2.group(1)=="" or m2.group(2)):
        off=int(m2.group(3),0); return m.group(1), m2.group(1), -off if m2.group(2)=="-" else off
    return m.group(1), inner, 0

def carry_kind(ins, k):
    """Classify the adc/sbb at ins[k] as part of a multi-word idiom the lifter models:
      'pair'  add|sub LO,x ; adc|sbb HI,y      (HI:LO a register pair or two adjacent words)
      'neg'   neg LO ; adc HI,0 ; neg HI        (32-bit negate)
      'setcc' cmp a,b ; sbb r,r                 (r = -(a < b), unsigned)
    else None. Used by BOTH the lifter and the driver's gate, so acceptance implies emission."""
    if k<1 or k>=len(ins): return None
    c=ins[k]; p=ins[k-1]
    if c.mnemonic in ("rcl","rcr"):                       # 32-bit shift by one through the carry
        cd,_,cs=c.op_str.partition(", "); pd,_,ps=(p.op_str or "").partition(", ")
        if cs!="1" or ps!="1" or cd not in _WREG or pd not in _WREG or cd==pd: return None
        if c.mnemonic=="rcl" and p.mnemonic in ("shl","sal"): return "shl1"      # shl LO,1 ; rcl HI,1
        if c.mnemonic=="rcr" and p.mnemonic in ("shr","sar"): return "shr1" if p.mnemonic=="shr" else "sar1"
        return None
    if c.mnemonic not in ("adc","sbb") or ", " not in c.op_str or ", " not in (p.op_str or ""):
        if not (c.mnemonic in ("adc","sbb") and p.mnemonic=="neg"): return None
    cd,_,cs=c.op_str.partition(", "); pd,_,ps=(p.op_str or "").partition(", ")
    if c.mnemonic=="sbb" and cd==cs and cd in _WREG and p.mnemonic=="cmp": return "setcc"
    if c.mnemonic=="adc" and cs=="0" and p.mnemonic=="neg" and p.op_str in _WREG and cd in _WREG \
       and cd!=p.op_str and k+1<len(ins) and ins[k+1].mnemonic=="neg" and ins[k+1].op_str==cd:
        return "neg"
    if (p.mnemonic,c.mnemonic) not in (("add","adc"),("sub","sbb")): return None
    if pd in _WREG and cd in _WREG and pd!=cd: return "pair"
    a,b=_memparts(pd),_memparts(cd)
    if a and b and a[0]==b[0]=="word" and a[1]==b[1] and b[2]-a[2]==2: return "pair"
    return None

class R:
    def __init__(s, ins, fs, base=0, cxmap=None, msc3=False):
        s.msc3=msc3                                     # MS-C 3.0 frame convention (bp at frame top)
        s.ins=ins; s.fs=fs; s.ax=None; s.si=None; s.di=None; s.cx=None; s.div=None; s.maxidx=-1; s.args=[]
        s.longfn=False; s.lmax=-1; s.floatfn=False; s.bx=None; s.dx=None; s._cmp=None
        s.base=base; s.cxmap=cxmap or {}; s.locals=set()
        s.breg={}                                       # 8-bit sub-register -> tracked value (byte loads)
        s.hoisted=set(); s.out=None; s.i=0             # register value-tracking state
        s.retlive=False                                 # ax currently holds a live return value?
        s.axsrc=None; s.bxsrc=None                      # instr idx where ax/bx const value was LOADED
        s.ranks=[]; s.regconst=[]                        # per top-level stmt: source rank + reg-routed-const flag
        s.callstmt=False                                # force calls to emit as statements (empty-body re-lift)
        s.gototargets=set(); s.emitgoto=False           # goto-fallback: target addrs needing a label; pass-2 flag
        s.tgt={}                                        # branch idx -> target idx
        a2i={ii.address:i for i,ii in enumerate(ins)}
        for i,ii in enumerate(ins):
            if ii.mnemonic in CJMP or ii.mnemonic=="jmp":
                m=re.match(r"(0x[0-9a-f]+|\d+)$", ii.op_str)   # targets < 10 print without 0x
                if m: s.tgt[i]=a2i.get(int(m.group(1),0))
        # Address of the common epilogue: the trailing run of mov sp,bp / leave / pop / ret.
        # A jmp into it is `return <value in ax>` (the "set result, goto exit" idiom), not a
        # no-op -- missing this dropped the returned value and left empty if-bodies.
        s.epi=None
        if ins and ins[-1].mnemonic in ("ret","retf"):
            e=ins[-1].address; j=len(ins)-2
            while j>=0 and (ins[j].mnemonic in ("leave","pop") or
                            (ins[j].mnemonic=="mov" and ins[j].op_str=="sp, bp")):
                e=ins[j].address; j-=1
            s.epi=e
        # far function (retf): the return address is 4 bytes, so params start at [bp+6]
        s.pbase=6 if any(x.mnemonic=="retf" for x in ins) else 4
        # `ret N` = this function is Pascal-convention with N/2 word params: the FIRST declared
        # param is the one pushed first = the HIGHEST bp offset.
        s.retn=0
        for x in ins:
            if x.mnemonic in ("ret","retf") and re.match(r"(0x[0-9a-f]+|\d+)$", x.op_str or ""):
                s.retn=int(x.op_str,0); break
        if s.retn: s.maxidx=s.retn//2-1
    def call_op(s, ci, default):
        """Resolve a CALL instruction's target to a C operator via the CX-helper map."""
        b=s.ins[ci].bytes
        if len(b)<3 or b[0]!=0xe8: return default
        rel=b[1]|(b[2]<<8); rel-= 0x10000 if rel>=0x8000 else 0
        tgt=(s.base + s.ins[ci].address + 3 + rel) & 0xffff
        return s.cxmap.get(tgt, default)
    def num(s,x): return int(x,0)
    def pname(s,off):
        # param index from the bp+ offset. 2.03 (bp at frame bottom): off-fs-4; 3.0 (bp at
        # frame top, standard): params at [bp+4],[bp+6],... -> off-4.
        pb=getattr(s,"pbase",4)
        idx=((off-pb)//2) if s.msc3 else ((off-s.fs-pb)//2)
        if getattr(s,"retn",0) and 0<=idx<s.retn//2: idx=s.retn//2-1-idx      # Pascal: reversed
        s.maxidx=max(s.maxidx,idx)
        return chr(ord('a')+idx) if 0<=idx<26 else "p%d"%idx
    def _ptrbase(s,reg):
        """Readable base for a pointer-register deref: the register's tracked pointer value
        if it has one (not a plain number), else the register name itself (an implicit
        struct pointer). Keeps [bx+N] etc. readable instead of leaking raw asm."""
        v=getattr(s,reg,None)
        if v is None or re.match(r"-?\d+$|-?0x[0-9a-f]+$",str(v)): return reg
        v=str(v)                                          # a non-trivial base must be parenthesized:
        if re.match(r"^[A-Za-z_]\w*(->f\d+)*$",v) or (v.startswith("(") and v.endswith(")")): return v
        return "(%s)"%v                                   # `*a` + `->f3` is (*a)->f3, not *(a->f3)
    def memname(s,op):
        r=s._memname0(op)
        if MARK and r is not None and "byte ptr" in op and "[" in op: return "BYTE(%s)"%r
        return r
    def _memname0(s,op):
        op=op.replace("word ptr ","").replace("byte ptr ","")
        op=re.sub(r"\b(es|ds|ss|cs|fs|gs):","",op)            # drop segment override (far deref)
        if s.msc3:                                            # 3.0: locals [bp-N], params [bp+N>=4]
            m=re.search(r"\[bp - (0x[0-9a-f]+|\d+)\]",op)
            if m:
                o=s.num(m.group(1)); s.locals.add(o); return "v%d"%o
            m=re.search(r"\[bp \+ (0x[0-9a-f]+|\d+)\]",op)
            if m:
                o=s.num(m.group(1)); nm=s.pname(o)
                return "HIBYTE(%s)"%nm if (o-getattr(s,"pbase",4))%2 else nm
            # fall through to si/global handling below
        else:
            m=re.search(r"\[bp \+ (0x[0-9a-f]+|\d+)\]",op)
            if m or "[bp]" in op:                             # [bp] == offset 0
                o=s.num(m.group(1)) if m else 0
                if o < s.fs: s.locals.add(o); return "v%d"%o  # local (below saved BP)
                nm=s.pname(o)                                 # parameter
                return "HIBYTE(%s)"%nm if (o-s.fs-getattr(s,"pbase",4))%2 else nm
        m=re.search(r"\[bp - (0x[0-9a-f]+|\d+)\]",op)         # negative bp offset is always a
        if m:                                                  # local (even when the chkstk prologue
            o=s.num(m.group(1)); s.locals.add(o); return "v%d"%o  # wasn't detected to set msc3)
        for reg in ("si","bx","di"):                          # pointer-register derefs
            cv=getattr(s,reg,None)
            if cv is not None and re.match(r"\d+$",str(cv)):  # reg = constant address -> global
                m=re.search(r"\[%s(?: ([+-]) (0x[0-9a-f]+|\d+))?\]"%reg,op)
                if m:
                    d=s.num(m.group(2)) if m.group(2) else 0
                    return "g_%x"%((int(cv)+(d if m.group(1)!="-" else -d))&0xffff)
            m=re.search(r"\[%s \+ (0x[0-9a-f]+|\d+)\]"%reg,op)
            if m: return "%s->f%d"%(s._ptrbase(reg),s.num(m.group(1)))
            m=re.search(r"\[%s - (0x[0-9a-f]+|\d+)\]"%reg,op)  # negative offset -> index back
            if m: return "%s[-%d]"%(s._ptrbase(reg),s.num(m.group(1)))
            if "[%s]"%reg in op: return "*%s"%s._ptrbase(reg)
        m=re.search(r"\[(bx|bp|si|di) \+ (si|di) ([+-]) (0x[0-9a-f]+|\d+)\]",op)   # base+index+/-disp
        if m: return "%s[%s %s %d]"%(s._ptrbase(m.group(1)), s._ptrbase(m.group(2)), m.group(3), s.num(m.group(4)))
        m=re.search(r"\[bx \+ si\]|\[bx \+ di\]|\[si \+ di\]",op)  # indexed; name by base reg
        if m: return "%s[%s]"%(s._ptrbase(op[1:3]), s._ptrbase(op.split(" + ")[1].rstrip("]")))
        m=re.search(r"\[(0x[0-9a-f]+|\d+)\]",op)
        if m: return "g_%x"%s.num(m.group(1))       # distinct global per address
        if op.strip() in ("ax","bx","cx","dx","si","di","bp","ds","es","ss","cs"):
            return op.strip()                        # bare register -> expose as a variable
        return None
    def rhs(s,op): return str(s.num(op)) if re.match(r"(0x[0-9a-f]+|\d+)$",op) else s.memname(op)

    def movptr(s,d,src):
        """mov si/di, X.  A pointer register kept as a bare source-register NAME is wrong once
        that source is clobbered (e.g. `mov si,ax` then ax reused -> si->f became ax->f). So
        snapshot a volatile source (a live register, or a side-effecting expression) into the
        destination as its own variable `d = <val>;`, and only alias a stable name/field/global
        directly (re-referenceable, no clobber risk)."""
        REGS=("ax","bx","cx","dx","si","di","bp","sp")
        if src in REGS:
            val=getattr(s,src,None); val=src if val is None else str(val)
            if val in REGS or "(" in val:                 # volatile register / expression
                s.emit(s.out,"%s = %s;"%(d,val)); setattr(s,d,d)
                if "(" in val and src in ("ax","bx","cx","dx","si","di"):
                    setattr(s,src,d)                       # src == the copy now: don't re-evaluate
                    if src=="ax": s.axsrc=None
            else:                                          # stable name/field/global -> alias
                setattr(s,d,val)
        elif re.match(r"(0x[0-9a-f]+|\d+)$",src):            # constant (often a data address)
            setattr(s,d,str(s.num(src)))
        else:
            setattr(s,d,s.memname(src))

    def _writes(s,mn,dst,reg):
        """Does instruction (mn,dst) overwrite register reg (so its value is lost)?"""
        if dst==reg: return True
        if reg in ("ax","dx") and mn in ("imul","idiv","mul","div","cwd"): return True
        if reg=="ax" and mn in ("call","lcall"): return True
        return False

    def future_store(s,reg,i):
        """Index of the next `mov [mem], reg` before reg is overwritten, else None.
        Identifies that reg's current value BELONGS to a variable stored later (the
        value-numbering optimization: value kept live in a register across other work)."""
        for j in range(i+1, len(s.ins)):
            mn=s.ins[j].mnemonic; ops=s.ins[j].op_str; dst=ops.split(", ")[0]
            if mn=="mov" and dst.replace("word ptr ","").replace("byte ptr ","").startswith("[") \
               and ops.endswith(", "+reg):
                return j
            if s._writes(mn,dst,reg): return None
        return None

    def rsrc(s,reg):
        """Resolve a register used as an expression operand to its symbolic value.
        If the value is an immediate that will later be stored to a variable, HOIST that
        store (emit `var = imm` now) so the variable's definition precedes this use, and
        return the variable — reversing the compiler's store-reordering."""
        cur=getattr(s,reg,None)
        if cur is not None and re.match(r"-?\d+$|-?0x[0-9a-f]+$",str(cur)):
            j=s.future_store(reg,s.i)
            if j is not None:
                var=s.memname(s.ins[j].op_str.split(", ")[0])
                if j not in s.hoisted and s.out is not None:
                    s.emit(s.out,"%s = %s;"%(var,cur)); s.hoisted.add(j)
                setattr(s,reg,var); return var
        return cur if cur is not None else reg                # untracked -> expose the register

    def _val(s,tok):
        """Resolve a source operand token: register->tracked value (rsrc), else imm/mem.
        Never yields None: an untracked register falls back to its own name (a live variable)."""
        if tok=="ax": return s.ax if s.ax is not None else "ax"
        if tok in ("si","di"):
            v=getattr(s,tok,None); return v if v is not None else tok
        return s.rsrc(tok) if tok in ("bx","cx","dx") else s.rhs(tok)

    _HALF={"ax":"ax|al|ah","bx":"bx|bl|bh","cx":"cx|cl|ch","dx":"dx|dl|dh","si":"si","di":"di"}
    def _live(s, reg, i):
        """Is `reg` read after ins[i] before being redefined, on any path?"""
        rx=re.compile(r"\b(%s)\b"%s._HALF[reg]); seen=set(); work=[i+1]
        while work:
            j=work.pop()
            while j is not None and 0<=j<len(s.ins) and j not in seen:
                seen.add(j); ii=s.ins[j]; mn=ii.mnemonic; ops=ii.op_str or ""
                d0,_,src=ops.partition(", ")
                if mn in ("ret","retf"):
                    if reg=="ax" and s._retval(): return True
                    break
                if mn in ("mov","lea","les","lds","pop") and d0==reg:
                    if rx.search(src): return True
                    break                                  # redefined
                if mn in ("xor","sub") and d0==reg and src==reg: break
                if rx.search(ops): return True
                if mn in ("call","lcall") and reg in ("ax","bx","cx","dx"): break   # caller-saved
                if mn=="jmp": j=s.tgt.get(j); continue
                if mn in CJMP: work.append(s.tgt.get(j))
                j+=1
        return False

    _WR=("ax","bx","cx","dx","si","di")
    _IMPLW={"mul":("ax","dx"),"imul":("ax","dx"),"div":("ax","dx"),"idiv":("ax","dx"),
            "cwd":("dx",),"cdq":("dx",),"cbw":("ax",),"cwde":("ax",),"xlatb":("ax",),
            "lodsb":("ax","si"),"lodsw":("ax","si"),"stosb":("di",),"stosw":("di",),
            "movsb":("si","di"),"movsw":("si","di"),"scasb":("di",),"scasw":("di",),"xlatb":("ax",),
            "cmpsb":("si","di"),"cmpsw":("si","di"),"loop":("cx",),
            "call":("ax","bx","cx","dx"),"lcall":("ax","bx","cx","dx")}
    def _wregs(s, mn, ops):
        """Word registers an instruction (re)defines."""
        base=mn.split()[-1]
        w=set(s._IMPLW.get(base,()))
        if mn.startswith("rep"): w.add("cx")
        if base in ("cmp","test","push","jmp","ret","retf","nop","mul","imul","div","idiv") \
           or base in CJMP or base in s._IMPLW:
            return w
        p=[x.strip() for x in ops.split(",")] if ops else []
        if p:
            d=BYTEPARENT.get(p[0],p[0])
            if d in s._WR: w.add(d)
            if base=="xchg" and len(p)>1:
                d=BYTEPARENT.get(p[1],p[1])
                if d in s._WR: w.add(d)
        return w

    def _kill_aliases(s, i):
        """ins[i] is about to redefine some register(s) W. Any OTHER lazily tracked value whose
        text reads W (e.g. di = `bx->f6` and now `les bx,..`) would silently change meaning: pin
        it into its register first if still needed. Pending call arguments are pinned to temps."""
        ii=s.ins[i]; w=s._wregs(ii.mnemonic, ii.op_str or "")
        if not w or s.out is None: return
        for W in w:
            rx=re.compile(r"(?<![\w>.])(%s)(?![\w(])"%s._HALF[W])
            for O in s._WR:
                if O==W: continue
                v=getattr(s,O,None)
                if v is None or str(v)==O: continue
                if rx.search(str(v)):
                    if s._live(O,i-1) or s._hascall(v): s._materialize(s.out,O)
                    else: setattr(s,O,None)
            for b8,v in list(s.breg.items()):
                if BYTEPARENT[b8]!=W and rx.search(str(v)):
                    if s._live(BYTEPARENT[b8],i-1):
                        s.out.append("%s = %s;"%(b8,v)); s.ranks.append(i); s.regconst.append(False)
                    s.breg.pop(b8,None)
            for k,a in enumerate(s.args):
                if ii.mnemonic in ("call","lcall"): break  # this call consumes the pending args now
                if rx.search(str(a)):
                    s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn
                    s.out.append("%s = %s;"%(t,a)); s.ranks.append(i); s.regconst.append(False)
                    s.args[k]=t

    def _pin(s, out, regs):
        """Pin the lazily tracked values of `regs` into the register variables (if still read
        later), leaving them bare -- used wherever control flow re-converges on a loop."""
        # Parallel assignment: if one pinned value reads a register that this same batch assigns
        # (dx:ax = f(dx:ax)), evaluate every value into a temporary first.
        batch=[r for r in s._WR if r in regs and getattr(s,r,None) is not None and str(getattr(s,r))!=r
               and (s._live(r,s.i) or s._hascall(getattr(s,r)))]
        if len(batch)>1 and any(re.search(r"(?<![\w>.])(%s)(?![\w(])"%s._HALF[a], str(getattr(s,b2)))
                                for a in batch for b2 in batch if a!=b2):
            for r in batch:
                s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn
                out.append("%s = %s;"%(t,getattr(s,r))); s.ranks.append(s.i); s.regconst.append(False)
                setattr(s,r,t)
        for r in s._WR:
            if r not in regs: continue
            for b8 in [b for b in s.breg if BYTEPARENT[b]==r]:
                v=s.breg.pop(b8)
                if str(v)!=b8 and s._live(r,s.i):
                    out.append("%s = %s;"%(b8,v)); s.ranks.append(s.i); s.regconst.append(False)
            v=getattr(s,r,None)
            if v is None or str(v)==r: continue
            if s._live(r,s.i) or s._hascall(v): s._materialize(out,r)
            else: setattr(s,r,None)

    def _mem_lazy(s):
        """Registers whose lazily tracked value READS MEMORY (anything but a plain constant). A
        loop may change that memory, so such a value must be captured before the loop is entered
        (`di = p; while (..) p++;` -- not `di = p` re-evaluated inside the loop)."""
        out=set()
        for r in s._WR:
            v=getattr(s,r,None)
            if v is not None and str(v)!=r and not re.match(r"-?\d+$",str(v)): out.add(r)
        for b8 in s.breg:
            if not re.match(r"-?\d+$",str(s.breg[b8])): out.add(BYTEPARENT[b8])
        return out

    def _tree_bids(s, nodes, acc):
        for n in nodes:
            k=n[0]
            if k=="block": acc.add(n[1])
            elif k=="if": acc.add(n[1]); s._tree_bids(n[2],acc)
            elif k=="ifelse": acc.add(n[1]); s._tree_bids(n[2],acc); s._tree_bids(n[3],acc)
            elif k in ("while","loop"): acc.add(n[1]); s._tree_bids(n[2],acc)
            elif k=="switch":
                acc.add(n[1])
                for _,arm in n[2]: s._tree_bids(arm,acc)
            elif k=="dowhile": acc.add(n[1]); acc.add(n[3]); s._tree_bids(n[2],acc)
        return acc

    def _has_break(s, nodes):
        for n in nodes:
            k=n[0]
            if k=="break": return True
            if k=="if" and s._has_break(n[2]): return True
            if k=="ifelse" and (s._has_break(n[2]) or s._has_break(n[3])): return True
            if k=="switch" and any(s._has_break(arm) for _,arm in n[2]): return True
        return False

    def _loop_written(s, blocks, bids):
        w=set()
        for b in bids:
            for ii in blocks[b].insns: w |= s._wregs(ii.mnemonic, ii.op_str or "")
        return w

    @staticmethod
    def _hascall(v):
        """Does this value text contain a real CALL (not one of the width/long pseudo-operators)?"""
        return any(n not in ("BYTE","HIBYTE","SX","S8","S16","LONG","LO","HI","LSHR","LSAR")
                   for n in re.findall(r"([A-Za-z_]\w*)\(", str(v)))

    def _wlive(s, reg, i):
        """Is the full WORD register read after ins[i] before either it or one of its halves'
        parents is redefined?"""
        rx=re.compile(r"\b%s\b"%reg); seen=set(); work=[i+1]
        while work:
            j=work.pop()
            while j is not None and 0<=j<len(s.ins) and j not in seen:
                seen.add(j); ii=s.ins[j]; mn=ii.mnemonic; ops=ii.op_str or ""
                d0,_,src=ops.partition(", ")
                if mn in ("ret","retf"):
                    if reg=="ax" and s._retval(): return True
                    break
                if mn in ("mov","lea","les","lds","pop") and d0==reg:
                    if rx.search(src): return True
                    break
                if mn in ("xor","sub") and d0==reg and src==reg: break
                if mn in ("cbw","cwde") and reg=="ax": break            # ax rebuilt from al
                if mn in ("call","lcall"): break
                if rx.search(ops): return True
                if mn=="jmp": j=s.tgt.get(j); continue
                if mn in CJMP: work.append(s.tgt.get(j))
                j+=1
        return False

    def _byte_settle(s, d8, i):
        """A byte half just got a lazily tracked value while its parent word is an untracked
        register variable. If the WORD is read later (`mov al,x` on one path, `push ax` after the
        merge), the byte write must really happen in the variable: emit it now."""
        par=BYTEPARENT[d8]
        if getattr(s,par,None) is not None or s.out is None or d8 not in s.breg: return
        if s._wlive(par, i):
            v=s.breg.pop(d8)
            if str(v)!=d8:
                s.out.append("%s = %s;"%(d8,v)); s.ranks.append(i); s.regconst.append(False)

    def _materialize(s, out, reg, rank=None):
        """Pin a lazily-tracked register value into the register variable (`reg = <value>;`)."""
        v=getattr(s,reg,None)
        if v is None or str(v)==reg: return
        # The assignment changes the VARIABLE `reg`. Any other pending text that mentions it must
        # be settled first: text holding exactly this value now simply IS the variable; text that
        # read the old variable (a pushed argument `bx->f4`, another register's `(bx + 2)`) is
        # frozen into a temporary / its own register before the variable changes.
        rx=re.compile(r"(?<![\w>.])(%s)(?![\w(])"%s._HALF[reg]); sv=str(v)
        for k2,a2 in enumerate(s.args):
            a2=str(a2)
            if sv in a2 and len(sv)>len(reg): s.args[k2]=a2.replace(sv,reg)
            elif rx.search(a2):
                s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn
                out.append("%s = %s;"%(t,a2)); s.ranks.append(s.i); s.regconst.append(False); s.args[k2]=t
        batch=[]
        for o in s._WR:
            if o==reg: continue
            ov=getattr(s,o,None)
            if ov is None or str(ov)==o: continue
            if sv in str(ov) and len(sv)>len(reg): setattr(s,o,str(ov).replace(sv,reg))
            elif rx.search(str(ov)): batch.append((o,str(ov)))
        if batch:
            # several register variables change together (dx:ax pairs): each new value is computed
            # from the OLD variables, so evaluate them all before assigning any.
            batch.append((reg,sv)); tmp=[]
            for o,ov in batch:
                s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn; tmp.append((o,t))
                out.append("%s = %s;"%(t,ov)); s.ranks.append(s.i); s.regconst.append(False)
            for o,t in tmp:
                out.append("%s = %s;"%(o,t)); s.ranks.append(s.i); s.regconst.append(False)
                if o!=reg: setattr(s,o,"ax" if o=="ax" else None)
        else:
            out.append("%s = %s;"%(reg,v)); s.ranks.append(s.i if rank is None else rank); s.regconst.append(False)
        setattr(s,reg,"ax" if reg=="ax" else None)

    def emit(s, out, text, rank=None, rc=False):
        """Append a statement, its source rank (for un-scheduling), and whether it is a
        register-routed constant store (a signal for dead-store recovery).
        A statement that WRITES a variable first pins every still-live register whose lazily
        tracked value reads that variable (`mov bx,[p]; inc [p]; mov [bx],al` is `bx = p; p++;
        *bx = ..`, not `p++; *p = ..`)."""
        for r in s._WR:                                    # evaluation order: a call still deferred
            v=getattr(s,r,None)                            # in a register runs BEFORE this statement
            if v is not None and str(v)!=r and s._hascall(v) and str(v) not in text:
                s._materialize(out, r, rank)
        for k,a in enumerate(s.args):
            if s._hascall(a) and str(a) not in text:
                s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn
                out.append("%s = %s;"%(t,a)); s.ranks.append(s.i); s.regconst.append(False); s.args[k]=t
        for r in s._WR:
            v=getattr(s,r,None)
            if v is None or str(v)==r or not s._hascall(v) or str(v) not in text: continue
            if text.endswith(" = %s;"%v): continue             # plain store: the caller aliases it
            if s._live(r, s.i):
                s._materialize(out, r, rank); text=text.replace(str(v), r)
            else: setattr(s, r, None)                          # evaluated here, never read again
        m=re.match(r"(.+?)(?: = (.*);|\+\+;|--;)$", text)
        if m:
            core=re.sub(r"^BYTE\((.*)\)$", r"\1", m.group(1))
            pat8=re.compile(r"(?<![\w>.])%s(?![\w(])"%re.escape(core))
            for b8,v in list(s.breg.items()):
                if str(v)!=b8 and pat8.search(str(v)) and str(v)==(m.group(2) or ""):
                    s.breg[b8]=m.group(1)                  # `x = <al's value>`: al now simply equals x
                    continue                               # (its old text would re-read the changed x)
                if str(v)!=b8 and pat8.search(str(v)) and str(v)!=(m.group(2) or ""):
                    s.breg.pop(b8)
                    if s._live(BYTEPARENT[b8], s.i):
                        out.append("%s = %s;"%(b8,v)); s.ranks.append(s.i); s.regconst.append(False)
        if MARK and m and re.search(r"->|\*|\[|g_", m.group(1)):
            for r in s._WR:
                v=getattr(s,r,None)
                if v is None or str(v)==r or (m.group(2) is not None and str(v)==m.group(2)): continue
                if re.search(r"->|\*|\[|g_", str(v)) and s._live(r, s.i): s._materialize(out, r, rank)
        if m and m.group(1) not in ("ax","bx","cx","dx","si","di") and m.group(1) not in BYTEREG:
            lhs=re.sub(r"^BYTE\((.*)\)$", r"\1", m.group(1)); rhs=m.group(2)
            pat=re.compile(r"(?<![\w>.])%s(?![\w(])"%re.escape(lhs))
            for r in ("ax","bx","cx","dx","si","di"):
                v=getattr(s,r,None)
                if v is None or str(v)==r or (rhs is not None and str(v)==rhs): continue
                if pat.search(str(v)) and s._live(r, s.i): s._materialize(out, r, rank)
        out.append(text); s.ranks.append(s.i if rank is None else rank); s.regconst.append(rc)

    def _binval(s, mn, d0):
        """Invalidate byte-register tracking that an instruction clobbers: a call returns in
        ax (al/ah); a write to a word register redefines both its 8-bit halves."""
        if mn in ("call","lcall"): s.breg.pop("al",None); s.breg.pop("ah",None)
        elif d0 in WORDHALVES and mn not in ("cmp","test","push"):
            for h in WORDHALVES[d0]: s.breg.pop(h,None)
        elif d0 in BYTEREG and mn not in ("cmp","test","push"):
            setattr(s, BYTEPARENT[d0], None)             # byte write makes the parent word unknown
            if mn not in TRACKED_BYTE: s.breg.pop(d0, None)   # statement-emitting/unmodeled -> clear

    def _byte_on_word(s, mn, ops):
        """`and|or|xor r8, imm` where the PARENT word register holds a tracked value and the byte
        half is not separately tracked: a bit-field edit of the word (`mov ax,[x]; and al,0xf8;
        or al,5; mov [x],ax`). Exact as a word op: and al,K == W & (0xFF00|K); or/xor al,K ==
        W |/^ K; the ah forms shift K by 8. Without this the byte write just invalidated the word
        and the whole computation was dropped (`x = ax`). Returns True if handled."""
        if mn not in ("and","or","xor") or ", " not in ops: return False
        d8, src = ops.split(", ",1)
        if d8 not in BYTEREG or d8 in s.breg or not re.match(r"(0x[0-9a-f]+|\d+)$", src): return False
        par=BYTEPARENT[d8]; cur=getattr(s,par,None)
        if par not in ("ax","bx","cx","dx") or cur is None: return False
        k=s.num(src); hi=d8.endswith("h")
        if mn=="and": k=((k<<8)|0xFF) if hi else (0xFF00|k)
        elif hi: k<<=8
        setattr(s,par,"(%s %s 0x%x)"%(cur,ALU[mn],k))
        if par=="ax": s.axsrc=None; s.retlive=True
        if par=="bx": s.bxsrc=None
        return True

    def expr_step(s,i):
        """Update ax/si/cx from a value-producing instruction (no statement)."""
        s.i=i; old=s.ax; ii=s.ins[i]; mn=ii.mnemonic; ops=ii.op_str
        if mn in ("or","and","test") and ops.count(",")==1 and "[" not in ops \
           and ops.split(",")[0].strip()==ops.split(",")[1].strip(): return   # flags-only self test
        s._kill_aliases(i)
        if s._byte_on_word(mn, ops): return
        _d=ops.split(",")[0].strip()
        _sv=None                                           # `mov r8, r8'`: read the source half first
        if mn=="mov" and ops.count(", ")==1 and ops.split(", ")[0] in BYTEREG and ops.split(", ")[1] in BYTEREG:
            _q=getattr(s,"_s8",None)
            _sv=_q[1] if (_q and _q[0]==i) else s._bval(ops.split(", ")[1])
        _q=getattr(s,"_sm",None)
        if not (_q and _q[0]==i):
            _o2=ops.split(", ",1)
            s._sm=(i, s.memname(_o2[1])) if (mn=="mov" and _d in BYTEREG and len(_o2)==2 and "[" in _o2[1]) else None
        _p=getattr(s,"_p8",None)
        pre8=_p[1] if (_p and _p[0]==i) else (s._bval(_d) if _d in BYTEREG else None)   # byte value BEFORE the write
        s._binval(mn, _d)
        if mn=="mov" and ops.split(", ")[0] in BYTEREG:        # byte load/copy: r8 = mem | imm | r8'
            d8=ops.split(", ")[0]; src=ops.split(", ",1)[1]
            if re.match(r"(0x[0-9a-f]+|\d+)$",src): s.breg[d8]=str(s.num(src))
            elif src in BYTEREG: s.breg[d8]=_sv if _sv is not None else s.breg.get(src,src)
            else:
                _q=getattr(s,"_sm",None)
                s.breg[d8]=(_q[1] if (_q and _q[0]==i and _q[1]) else s.memname(src)) or d8
            if BYTEPARENT[d8] in ("ax","bx","cx","dx","si","di"): setattr(s,BYTEPARENT[d8],None)
            s._byte_settle(d8, i)
            return
        if mn in ("add","sub","or","and","xor") and ops.split(", ")[0] in BYTEREG:   # byte-reg ALU
            d8=ops.split(", ")[0]; src=ops.split(", ",1)[1]
            if mn in ("sub","xor") and src==d8:                           # sub/xor r8,r8 -> clear to 0
                s.breg[d8]="0"
                lo={"ah":"al","bh":"bl","ch":"cl","dh":"dl"}.get(d8)
                if lo and lo in s.breg:                                   # zero-extend: word = low byte
                    setattr(s, BYTEPARENT[d8], s._b8(s.breg[lo]))
                    if d8=="ah": s.axsrc=None; s.retlive=True
                else:
                    _w=getattr(s,"_pw",None); _w=_w[1] if (_w and _w[0]==i) else None
                    if lo and _w is not None and str(_w)!=BYTEPARENT[d8]:   # clear the HIGH half of a
                        setattr(s, BYTEPARENT[d8], "(%s & 0xff)"%_w)         # tracked word
                        s.breg.pop(d8,None)
                        if d8=="ah": s.axsrc=None; s.retlive=True
                    else: s._byte_settle(d8, i)
                return
            cur=s.breg.get(d8,pre8 if pre8 is not None else d8); opc={"add":"+","sub":"-","or":"|","and":"&","xor":"^"}[mn]
            if re.match(r"(0x[0-9a-f]+|\d+)$",src): sv=str(s.num(src))
            elif src in BYTEREG: sv=s.breg.get(src,src)
            else: sv=s.memname(src) or src
            s.breg[d8]="(%s %s %s)"%(cur,opc,sv); s._byte_settle(d8, i); return
        if mn in ("inc","dec") and ops in BYTEREG:             # byte-reg inc/dec
            s.breg[ops]="(%s %s 1)"%(s.breg.get(ops,pre8 if pre8 is not None else ops), "+" if mn=="inc" else "-"); return
        if mn in ("cbw","cwde") and "al" in s.breg:            # sign-extend al -> ax (int)al
            s.ax=("SX(%s)"%s.breg["al"]) if MARK else s.breg["al"]; s.axsrc=None; return
        if mn in ("cbw","cwde") and MARK:                      # ax = (int)(char)ax
            s.ax="SX(%s)"%(s.ax if s.ax is not None else "ax"); s.axsrc=None; s.retlive=True; return
        if mn=="mov" and ops.startswith("ax, "):
            src=ops[4:]
            if src=="dx" and s.div: s.ax="(%s %% %s)"%s.div; s.axsrc=None
            elif src in ("bx","cx","dx"): s.ax=s.rsrc(src); s.axsrc=None   # reg->reg copy
            elif src in ("si","di"):
                v=getattr(s,src,None); s.ax=v if v is not None else src; s.axsrc=None
            elif re.match(r"(0x[0-9a-f]+|\d+)$",src): s.ax=s.rhs(src); s.axsrc=i  # LOADED const
            else: s.ax=s.rhs(src); s.axsrc=None
        elif mn=="mov" and ops.startswith(("si, ","di, ")):
            d=ops[:2]; src=ops[4:]; s.movptr(d,src)
        elif mn in ("les","lds"):                              # load far pointer -> reg (near model)
            dst, _, src = ops.partition(", ")
            if dst in ("ax","bx","cx","dx","si","di"): setattr(s, dst, s.memname(src))
        elif mn=="lea":                                        # effective address -> reg = &addr
            dst, _, src = ops.partition(", ")
            nm = s.memname(src)
            if dst in ("ax","bx","cx","dx","si","di") and nm: setattr(s, dst, "&"+nm)
        elif mn=="xchg":                                       # register swap -> exchange tracked values
            o = ops.split(", ")
            R = ("ax","bx","cx","dx","si","di")
            if len(o)==2 and o[0] in R and o[1] in R:
                va = getattr(s,o[0]); vb = getattr(s,o[1])
                setattr(s, o[0], vb if vb is not None else o[1])
                setattr(s, o[1], va if va is not None else o[0])
        elif mn=="mov" and ops.startswith("bx, "):
            src=ops[4:]
            if src in ("ax","cx","dx","si","di","bp"): s.movptr("bx",src); s.bxsrc=None
            else: s.bx=s.rhs(src); s.bxsrc=i if re.match(r"(0x[0-9a-f]+|\d+)$",src) else None
        elif mn=="mov" and ops.startswith("cx, "):
            src=ops[4:]; s.movptr("cx",src) if src in ("ax","bx","dx","si","di","bp") else setattr(s,"cx",s.rhs(src))
        elif mn=="mov" and ops.startswith("dx, "):
            src=ops[4:]; s.movptr("dx",src) if src in ("ax","bx","cx","si","di","bp") else setattr(s,"dx",s.rhs(src))
        elif mn in ("xor","sub") and ops=="ax, ax": s.ax="0"; s.axsrc=None   # self-xor/sub = clear
        elif mn in ("xor","sub") and ops=="bx, bx": s.bx="0"; s.bxsrc=None   # (clears bx, NOT ax)
        elif mn in ("cwd","cdq"):
            _v=str(s.ax if s.ax is not None else "ax")
            s.dx=("65535" if int(_v)&0x8000 else "0") if re.match(r"\d+$",_v) else "(0 - (%s >> 15))"%_v
        elif mn in ("or","and") and ops=="ax, ax": pass             # `or/and ax,ax` is flag-only (ax
                                                                    # unchanged) -- never `(ax|ax)`, which
                                                                    # would DUPLICATE a call in s.ax
        else:
            axv = s.ax if s.ax is not None else "ax"             # untracked ax -> the register
            cxv = s.cx if s.cx is not None else "cx"
            R=("ax","bx","cx","dx","si","di"); d0=ops.split(",")[0].strip()
            def rv(r): v=getattr(s,r,None); return v if v is not None else r
            def shcnt():
                if ops.endswith("cl"): return s.breg["cl"] if "cl" in s.breg else cxv
                return s._val(ops.split(", ")[1])
            if mn in ALU and ops.startswith("ax, "): s.ax="(%s %s %s)"%(axv,ALU[mn],s._val(ops[4:]))
            elif mn in ("imul","mul"): s.ax="(%s * %s)"%(axv,s._val(ops))
            elif mn in ("idiv","div"): rn=s._val(ops); s.div=(axv,rn); s.ax="(%s / %s)"%(axv,rn); s.dx="(%s %% %s)"%(axv,rn)
            elif mn in ("shl","sal") and d0 in R: setattr(s,d0,"(%s << %s)"%(rv(d0),shcnt()))
            elif mn in ("shr","sar") and d0 in R: setattr(s,d0,"(%s >> %s)"%(rv(d0),shcnt()))
            elif mn=="neg" and d0 in R: setattr(s,d0,"(-%s)"%rv(d0))
            elif mn=="not" and d0 in R: setattr(s,d0,"(~%s)"%rv(d0))
        if s.ax is not old: s.retlive=True                    # ax got a live computed value

    def cond_at(s,i):
        """Recover a branch condition starting at i (loads + cmp/test + jcc).
        Return (cond_str, jcc_idx)."""
        j=i
        def resolve(t):
            if t=="ax": return s.ax if s.ax is not None else "ax"   # untracked -> the register
            if t=="bx": return s.bx if s.bx is not None else "bx"
            if t in ("cx","dx","si","di"):                # compare a register by its tracked value
                v=getattr(s,t,None); return v if v is not None else t
            if t in BYTEREG: return s._bval(t, True)
            v=s.memname(t)
            return v if v is not None else (str(s.num(t)) if re.match(r"(0x[0-9a-f]+|\d+)$",t) else t)
        while j<len(s.ins) and s.ins[j].mnemonic not in CJMP:
            mn=s.ins[j].mnemonic; ops=s.ins[j].op_str
            if mn=="cmp":
                p=ops.split(", "); s._cmp=(resolve(p[0]), resolve(p[1]))
            elif mn=="test":                                   # test X,Y; jcc == cmp (X & Y), 0
                p=ops.split(", "); a0=resolve(p[0])
                s._cmp=((a0 if len(p)>1 and p[0]==p[1] else "(%s & %s)"%(a0, resolve(p[1]))), "0")
            else: s.expr_step(j)
            j+=1
        if j>=len(s.ins):                                 # no conditional jump before the end
            return (s.ax if s.ax is not None else "0"), j
        jcc=s.ins[j].mnemonic
        if jcc in ("js","jns"):
            k=j-1
            while k>=0 and s.ins[k].mnemonic in ("mov","lea","push","pop","les","cbw","cwde"): k-=1
            o=s.ins[k].op_str if k>=0 else ""
            w="S8" if ("byte ptr" in o or o.split(",")[0].strip() in BYTEREG) else "S16"
            val=("(%s - %s)"%s._cmp) if s._cmp else s._flagval(j)
            if MARK: val="%s(%s)"%(w,val)
            return "(%s %s 0)"%(val, ">=" if jcc=="js" else "<"), j     # fall-through sense
        sg=None
        if MARK and jcc in ("jl","jle","jg","jge"):
            k=j-1
            while k>=0 and s.ins[k].mnemonic in ("mov","lea","push","pop","les","cbw","cwde"): k-=1
            o=s.ins[k].op_str if k>=0 else ""
            sg="S8" if ("byte ptr" in o or o.split(",")[0].strip() in BYTEREG) else "S16"
        if not s._cmp:                                    # a cmp/test earlier in this block whose flags
            k=j-1; lo=getattr(s,"_cond_lo",0)             # survive the (non-flag) statements after it
            while k>=lo and k>=0:
                mk=s.ins[k].mnemonic
                if mk=="cmp":
                    p=s.ins[k].op_str.split(", "); s._cmp=(resolve(p[0]), resolve(p[1])); break
                if mk=="test":
                    p=s.ins[k].op_str.split(", "); a0=resolve(p[0])
                    s._cmp=((a0 if p[0]==p[1] else "(%s & %s)"%(a0, resolve(p[1]))), "0"); break
                if mk in s._FLAGSET or mk in ("call","lcall","jmp","ret","retf") or mk in CJMP: break
                k-=1
        if not s._cmp and getattr(s,"_lastcmp",None):
            # A jcc with no flag-setting instruction of its own in this block tests the flags of
            # the PREVIOUS compare (`cmp a,b; ja X; jb Y` -- the long-compare ladder): reuse it.
            k=j-1; lo=getattr(s,"_cond_lo",0); own=False
            while k>=lo:
                if s.ins[k].mnemonic not in ("mov","lea","les","lds","push","pop","nop","xchg","cbw","cwde","cwd","cdq"):
                    own=True; break
                k-=1
            if not own:
                s._cmp=s._lastcmp[0]
                if MARK and jcc in ("jl","jle","jg","jge"): sg=s._lastcmp[1]
        if s._cmp:
            a_,b_=s._cmp
            if sg is None and MARK:
                k=j-1
                while k>=0 and s.ins[k].mnemonic in ("mov","lea","push","pop","les","cbw","cwde"): k-=1
                o=s.ins[k].op_str if k>=0 else ""
                w="S8" if ("byte ptr" in o or o.split(",")[0].strip() in BYTEREG) else "S16"
            else: w=sg
            s._lastcmp=((a_,b_), w or "S16")
            if sg: a_,b_="%s(%s)"%(sg,a_),"%s(%s)"%(sg,b_)
            elif UNSIGNED_CAST and not MARK and jcc in ("jb","jae","ja","jbe","jnb","jnae","jna","jnbe","jc","jnc"):
                # the variables are declared int, so an unsigned compare has to say so
                if re.match(r"-\d+$", str(b_)): b_="0x%x"%(int(b_)&0xFFFF)
                a_="(unsigned)%s"%a_
            cond="(%s %s %s)"%(a_, INV.get(jcc,"?"), b_)
        else:                                             # flags from the last ALU result: test it
            val=s._flagval(j)                             # against 0 in the jcc's own sense (was a
            op=INV.get(jcc)                               # bare `ax` regardless of je/jne or of
            if sg: val="%s(%s)"%(sg,val)                  # which register set the flags)
            cond="(%s %s 0)"%(val,op) if op else str(val)
        for r in s._WR:                                   # a call evaluated in this test is spent
            v=getattr(s,r,None)
            if v is not None and s._hascall(v) and str(v) in cond: setattr(s,r,None)
        return cond, j

    _FLAGSET=("add","sub","and","or","xor","inc","dec","neg","shl","sal","shr","sar","adc","sbb")
    def _flagval(s, j):
        """Value whose flags the conditional jump at ins[j] tests: the destination of the nearest
        preceding flag-setting instruction (current tracked value). Falls back to ax."""
        k=j-1
        while k>=0:
            mn=s.ins[k].mnemonic
            if mn in ("call","lcall","jmp","ret","retf") or mn in CJMP: break
            if mn in s._FLAGSET:
                d0=s.ins[k].op_str.split(",")[0].strip()
                if d0=="ax": break
                if d0 in ("bx","cx","dx","si","di"):
                    v=getattr(s,d0,None); return v if v is not None else d0
                if d0 in BYTEREG: return s._bval(d0, True)
                if "[" in d0:
                    return s.memname(d0) or "ax"
                break
            k-=1
        return s.ax if s.ax is not None else "ax"

    @staticmethod
    def _b8(v):
        """A byte-register value used as a number (compared, zero-extended): byte arithmetic wraps
        at 8 bits, so anything but a plain byte read / small constant / masked value is masked."""
        v=str(v)
        if re.match(r"(BYTE\([^()]*(\([^()]*\))*[^()]*\)|\d{1,2}|1\d\d|2[0-4]\d|25[0-5]|[a-d][lh])$", v)            or v.endswith("& 0xff)") or not re.search(r"[-+*<>]", v):
            return v
        return "(%s & 0xff)"%v

    def _bval(s, r8, trunc=False):
        """Current value of an 8-bit register: its own tracked byte value, else (low halves) the
        tracked value of the parent word -- e.g. `call f; mov [p],al` stores f()'s low byte."""
        if r8 in s.breg: return s._b8(s.breg[r8]) if trunc else s.breg[r8]
        pv=getattr(s,BYTEPARENT[r8],None)
        if pv is None: return r8
        if not r8.endswith("l"): return "((%s >> 8) & 0xff)"%pv      # high half of a tracked word
        if trunc and not re.match(r"(BYTE\(.*\)|\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$", str(pv)):
            return "(%s & 0xff)"%pv                       # a TEST of the low byte only (`or al,al`)
        return pv

    def lparam(s,op):
        m=re.search(r"\[bp \+ (0x[0-9a-f]+|\d+)\]",op); off=s.num(m.group(1))
        idx=(off-s.fs-4)//4; s.lmax=max(s.lmax,idx); return chr(ord('a')+idx)

    def long_pat(s,i):
        """Recognize a LONG/FLOAT operation starting at i; set s.ax; return #instrs consumed, else 0."""
        ins=s.ins
        # FLOAT binop: mov ax,[..]; mov bx,[..]; mov cx,[..]; mov dx,[..]; lea si,[..]; push si; call; call
        if (i+7<len(ins) and ins[i].op_str.startswith("ax, word ptr [bp")
                and ins[i+1].op_str.startswith("bx, word ptr [bp")
                and ins[i+3].op_str.startswith("dx, word ptr [bp")
                and ins[i+4].mnemonic=="lea" and ins[i+5].mnemonic=="push"
                and ins[i+6].mnemonic=="call" and ins[i+7].mnemonic=="call"):
            s.floatfn=True; a=s.lparam(ins[i+3].op_str); b=s.lparam(ins[i+1].op_str)
            op=s.call_op(i+6,"+")                  # first CX call = the double operation (CXA/CXM/..)
            s.ax="(%s %s %s)"%(a,op,b); return 8
        # add/sub: mov ax,[X]; add/sub ax,[Y]; mov bx,[..]; adc/sbb bx,[..]
        if (i+3<len(ins) and ins[i].mnemonic=="mov" and ins[i].op_str.startswith("ax, word ptr [bp")
                and ins[i+1].mnemonic in ("add","sub") and ins[i+1].op_str.startswith("ax, word ptr [bp")
                and ins[i+2].mnemonic=="mov" and ins[i+2].op_str.startswith("bx, word ptr [bp")
                and ins[i+3].mnemonic in ("adc","sbb")):
            s.longfn=True; a=s.lparam(ins[i].op_str); b=s.lparam(ins[i+1].op_str)
            s.ax="(%s %s %s)"%(a, "+" if ins[i+1].mnemonic=="add" else "-", b); return 4
        # mul/div: mov ax,[A+2]; mov bx,[A]; mov cx,[B+2]; mov dx,[B]; call; mov ax,bx
        if (i+5<len(ins) and ins[i].op_str.startswith("ax, word ptr [bp")
                and ins[i+1].op_str.startswith("bx, word ptr [bp")
                and ins[i+3].op_str.startswith("dx, word ptr [bp") and ins[i+4].mnemonic=="call"
                and ins[i+5].op_str=="ax, bx"):
            s.longfn=True; a=s.lparam(ins[i+1].op_str); b=s.lparam(ins[i+3].op_str)
            op=s.call_op(i+4,"*")                  # CXM33=* / CXD33=/ (resolved via cxmap, else default *)
            s.ax="(%s %s %s)"%(a,op,b); return 6
        # long += imm on a bp-local: mov ax,[bp-A]; mov dx,[bp-B]; add ax,IMM; adc dx,0  (dx:ax=L)
        if (i+3<len(ins) and ins[i].mnemonic=="mov" and re.match(r"ax, word ptr \[bp - ",ins[i].op_str)
                and ins[i+1].mnemonic=="mov" and re.match(r"dx, word ptr \[bp - ",ins[i+1].op_str)
                and ins[i+2].mnemonic in ("add","sub") and re.match(r"ax, (0x[0-9a-f]+|\d+)$",ins[i+2].op_str)
                and ins[i+3].mnemonic in ("adc","sbb") and ins[i+3].op_str=="dx, 0"
                and (ins[i+2].mnemonic=="add")==(ins[i+3].mnemonic=="adc")):
            s.longfn=True; lo=s.memname(ins[i].op_str.split(", ",1)[1])
            op="+" if ins[i+2].mnemonic=="add" else "-"
            s.ax="(%s %s %s)"%(lo,op,s.num(ins[i+2].op_str.split(", ")[1])); return 4
        return 0

    def long_stmt(s,i,out):
        """A 32-bit (long) statement carried across a lo/hi word pair; return #instrs or 0.
        add/sub word ptr [LO],IMM ; adc/sbb word ptr [LO+2],0  ==  *(long*)&LO += IMM
        (the compiler splits a `long` += const into a low-word op plus a carry into the high
        word; LO+2 is the contiguous high word in the little-endian data segment)."""
        ins=s.ins
        if i+1>=len(ins): return 0
        a,b=ins[i],ins[i+1]
        if a.mnemonic in ("add","sub") and b.mnemonic in ("adc","sbb") and \
                (a.mnemonic=="add")==(b.mnemonic=="adc"):
            ma=re.match(r"word ptr \[0x([0-9a-f]+)\], (0x[0-9a-f]+|\d+)$",a.op_str)
            mb=re.match(r"word ptr \[0x([0-9a-f]+)\], 0$",b.op_str)
            if ma and mb:
                lo=int(ma.group(1),16); hi=int(mb.group(1),16)
                if hi==lo+2:
                    op="+" if a.mnemonic=="add" else "-"
                    s.emit(out,"*(long *)&g_%x %s= %s;"%(lo,op,s.num(ma.group(2)))); return 2
        return 0

    def _opval(s, tok):
        """Current value of an instruction operand (immediate / register / memory)."""
        tok=tok.strip()
        if re.match(r"(0x[0-9a-f]+|\d+)$",tok): return str(s.num(tok))
        if tok=="ax": return s.ax if s.ax is not None else "ax"
        if tok in s._WR:
            v=getattr(s,tok,None); return v if v is not None else tok
        if tok in BYTEREG: return s._bval(tok, True)
        return s.memname(tok) or tok

    def _carry(s, i, out):
        """Multi-word arithmetic through the carry flag (see carry_kind). The 32-bit value is
        written LONG(hi, lo); LO()/HI() take its halves. Faithful rather than pretty: a later
        readability pass can fold `x = LO(e); x2 = HI(e)` into a long assignment."""
        ins=s.ins
        if ins[i].mnemonic=="sbb" and carry_kind(ins,i)=="setcc":
            s._kill_aliases(i)
            a,_,b=ins[i-1].op_str.partition(", ")
            r=ins[i].op_str.split(",")[0].strip()
            s._binval("mov", r)
            setattr(s,r,"(0 - (%s < %s))"%(s._opval(a),s._opval(b)))
            if r=="ax": s.axsrc=None; s.retlive=True
            if r=="bx": s.bxsrc=None
            return 1
        kind=carry_kind(ins,i+1) if i+1<len(ins) else None
        if kind in ("shl1","shr1","sar1"):
            for r in s._WR:
                if s._hascall(getattr(s,r,None) or ""): s._materialize(out,r)
            a=ins[i].op_str.split(",")[0].strip(); b=ins[i+1].op_str.split(",")[0].strip()
            lo,hi=(a,b) if kind=="shl1" else (b,a)         # shr/sar HI,1 ; rcr LO,1
            s._kill_aliases(i); s._kill_aliases(i+1)       # settle dependants FIRST, then read operands
            v="LONG(%s, %s)"%(s._opval(hi),s._opval(lo))
            E="%s + %s"%(v,v) if kind=="shl1" else "%s(%s)"%("LSHR" if kind=="shr1" else "LSAR",v)
            s._binval("mov",lo); s._binval("mov",hi)
            setattr(s,lo,"LO(%s)"%E); setattr(s,hi,"HI(%s)"%E)
            if "ax" in (lo,hi): s.axsrc=None; s.retlive=True
            if "bx" in (lo,hi): s.bxsrc=None
            return 2
        if kind not in ("pair","neg"): return 0
        for r in s._WR:                                    # never duplicate a deferred call
            if s._hascall(getattr(s,r,None) or ""): s._materialize(out,r)
        p,c=ins[i],ins[i+1]
        if kind=="neg":
            lo=p.op_str.strip(); hi=c.op_str.split(",")[0].strip()
            s._kill_aliases(i); s._kill_aliases(i+1)
            E="0 - LONG(%s, %s)"%(s._opval(hi),s._opval(lo)); n=3
        else:
            lo,_,x=p.op_str.partition(", "); hi,_,y=c.op_str.partition(", ")
            op="+" if p.mnemonic=="add" else "-"
            if lo in s._WR:                                # settle dependants FIRST, then read operands
                s._kill_aliases(i); s._kill_aliases(i+1)
            X,Y=s._opval(x),s._opval(y)
            if lo not in s._WR:                            # two adjacent memory words
                ml,mh=s.memname(lo),s.memname(hi)
                s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn
                s.emit(out,"%s = LONG(%s, %s) %s LONG(%s, %s);"%(t,mh,ml,op,Y,X))
                s.emit(out,"%s = LO(%s);"%(ml,t)); s.emit(out,"%s = HI(%s);"%(mh,t))
                return 2
            lo0,hi0=s._opval(lo),s._opval(hi)
            E="LONG(%s, %s) %s LONG(%s, %s)"%(hi0,lo0,op,Y,X); n=2
        s._binval("mov",lo); s._binval("mov",hi)
        setattr(s,lo,"LO(%s)"%E); setattr(s,hi,"HI(%s)"%E)
        if "ax" in (lo,hi): s.axsrc=None; s.retlive=True
        if "bx" in (lo,hi): s.bxsrc=None
        return n

    def _take_args(s, i, kind, target):
        """Arguments of the call at ins[i], in C source order. Pascal callees (CALLINFO says they
        pop N bytes) consume exactly N/2 pushed words, in push order; C callees consume what the
        caller's `add sp,N` cleans up (reversed). Unknown -> every pending push (legacy)."""
        n=None; pascal=False
        if CALLINFO is not None:
            try: rn=CALLINFO(s.ins[i].address, kind, target)
            except Exception: rn=None
            if rn: pascal=True; n=rn//2
        if n is None and i+1<len(s.ins):
            nx=s.ins[i+1]
            if nx.mnemonic=="add" and re.match(r"sp, (0x[0-9a-f]+|\d+)$", nx.op_str):
                n=s.num(nx.op_str[4:])//2
        if n is None or n>len(s.args): take=s.args; s.args=[]
        else: take=s.args[len(s.args)-n:]; s.args=s.args[:len(s.args)-n]
        return [str(a) for a in (take if pascal else reversed(take))]

    _AXR=re.compile(r"\b(ax|al|ah)\b")
    def _axrw(s, mn, ops):
        """How an instruction treats the value in ax: 'def' (overwrites it), 'use' (its value is
        emitted/consumed), 'xform' (updates ax in place -- stays tracked), 'ret', or None."""
        if mn in ("call","lcall","lodsb","lodsw"): return "def"
        if mn in ("ret","retf"): return "ret"
        p=[x.strip() for x in ops.split(",")] if ops else []
        d0=p[0] if p else ""; src=p[1] if len(p)>1 else ""
        if mn in ("mov","lea","les","lds","pop"):
            if d0 in ("ax","al"): return "use" if s._AXR.search(src) else "def"
            if d0=="ah": return "xform"
            return "use" if s._AXR.search(ops) else None
        if mn in ("xor","sub") and ops.replace(" ","")=="ax,ax": return "def"
        if mn in ("or","and","test") and ops.replace(" ","") in ("ax,ax","al,al"): return "use"
        if mn in ("cbw","cwde","cwd","cdq"): return "xform"
        if mn in ("mul","imul","div","idiv"): return "xform"
        if mn in ("cmp","test","push","stosb","stosw","scasb","scasw","out","xchg"):
            return "use" if (s._AXR.search(ops) or mn in ("stosb","stosw","scasb","scasw")) else None
        if d0 in ("ax","al","ah"): return "xform"
        return "use" if s._AXR.search(ops) else None

    def _retval(s):
        """Does this function return a value? A value-returning C function leaves a result in ax
        on EVERY path; if some path from the entry reaches a ret without ax ever being written,
        the function is void and its final ret does not consume ax."""
        if getattr(s, "_rv", None) is None and RETVAL is not None: s._rv=bool(RETVAL)
        if getattr(s, "_rv", None) is None:
            seen=set(); work=[(getattr(s,"emit_start",0), False)]; rv=True
            while work and rv:
                j,d=work.pop()
                while j is not None and 0<=j<len(s.ins) and (j,d) not in seen:
                    seen.add((j,d)); ii=s.ins[j]; mn=ii.mnemonic
                    if mn in ("ret","retf"):
                        if not d: rv=False
                        break
                    if s._axrw(mn, ii.op_str or "")=="def": d=True
                    if mn=="jmp": j=s.tgt.get(j); continue
                    if mn in CJMP: work.append((s.tgt.get(j), d))
                    j+=1
            s._rv=rv
        return s._rv

    def _ax_fate(s, i):
        """Follow the value ax holds after ins[i] along every path until it is redefined.
        Returns (uses, reaches_ret, store_first): the instruction indices that consume it, whether
        it is still live at a ret, and whether the very next consumer (straight-line) is a plain
        store `mov [mem], ax` (after which the value is aliased to that variable)."""
        seen=set(); work=[i+1]; uses=[]; ret=False; first=None; linear=True; s._retlin=False
        while work:
            j=work.pop()
            while j is not None and 0<=j<len(s.ins) and j not in seen:
                seen.add(j); ii=s.ins[j]; mn=ii.mnemonic; k=s._axrw(mn, ii.op_str)
                if k=="def": break
                if k=="ret":
                    if s._retval():
                        ret=True
                        if linear: s._retlin=True
                    break
                if k=="use":
                    uses.append(j)
                    if first is None and linear: first=j
                if mn=="jmp": j=s.tgt.get(j); linear=False; continue
                if mn in CJMP:
                    work.append(s.tgt.get(j)); linear=False
                j+=1
        sf=False; s._firstlin=first
        if first is not None:
            f=s.ins[first]; d=f.op_str.split(", ")[0]
            sf=(f.mnemonic=="mov" and "[" in d and f.op_str.endswith(", ax"))
        return uses, ret, sf

    def _call_result(s, i, out, expr):
        """A call's value lives in ax. Dead (never read before being overwritten) -> a statement;
        read once (or first stored to a variable) -> embedded at its use; read several times ->
        materialized `ax = f();` so the call is not textually duplicated."""
        for r in ("si","di"):                              # callee-saved copies of a VARIABLE: the
            v=getattr(s,r,None)                            # callee may change it (a global, or a local
            if v is not None and str(v)!=r and not re.match(r"^&|^-?\d+$",str(v)) and s._live(r,i):   # whose
                s._materialize(out, r)                     # address escaped), so capture the value now
        for r in s._WR:                                    # an earlier deferred call that is not
            v=getattr(s,r,None)                            # an argument of this one runs first
            if v is not None and str(v)!=r and s._hascall(v) and str(v) not in expr:
                s._materialize(out, r)
        s.bx=s.cx=s.dx=None; s.bxsrc=None
        for b8 in ("bl","bh","cl","ch","dl","dh","al","ah"): s.breg.pop(b8,None)
        uses, ret, sf = s._ax_fate(i)
        if s.callstmt or (not uses and not ret):
            s.emit(out,expr+";"); s.ax=None; s.retlive=False
        elif s._live("dx", i):                             # a long result: dx is read too, so the
            s.emit(out,"ax = %s;"%expr); s.ax="ax"; s.retlive=True   # call must run before that read
        elif uses and s._firstlin is None:                 # first read is past a branch: pin it
            s.emit(out,"ax = %s;"%expr); s.ax="ax"; s.retlive=True
        elif not uses and not s._retlin:                   # only reaches the ret, across a branch/
            s.emit(out,"ax = %s;"%expr); s.ax="ax"; s.retlive=True   # merge: pin it (never defer a
        elif not sf and len(uses)==1 and s.ins[uses[0]].mnemonic in ("cmp","test","or","and") \
             and (lambda f: f[0] or f[1])(s._ax_fate(uses[0])):
            s.emit(out,"ax = %s;"%expr); s.ax="ax"; s.retlive=True   # tested AND read again: pin
        elif sf or len(uses)<=1:                           # call over control flow)
            s.ax=expr; s.retlive=True
        else:
            s.emit(out,"ax = %s;"%expr); s.ax="ax"; s.retlive=True
        s.axsrc=None

    def step(s,i,out):
        """Straight-line: process ins[i], maybe append a statement. Return next i."""
        s.i=i; s.out=out
        if i in s.hoisted: return i+1                          # store already emitted (hoisted)
        if MARK:                                           # checker mode: the explicit LONG form
            n=s._carry(i,out)                              # (the `*(long *)&x += 1` sugar below is
            if n: return i+n                               # for readers, not for the interpreter)
        n=s.long_pat(i)
        if n: s.retlive=True; return i+n
        n=s.long_stmt(i,out)
        if n: return i+n
        n=s._carry(i,out)
        if n: return i+n
        ii=s.ins[i]; mn=ii.mnemonic; ops=ii.op_str; d0=ops.split(",")[0].strip()
        if mn in ("or","and","test") and ops.count(",")==1 and d0==ops.split(",")[1].strip() \
           and "[" not in ops: return i+1                 # `or r,r` = flags only: value/tracking unchanged
        s._kill_aliases(i)
        if s._byte_on_word(mn, ops): return i+1
        for _t in re.findall(r"\b[abcd][lh]\b", ops):       # a byte op on a register still holding
            if _t not in s.breg and s._hascall(getattr(s,BYTEPARENT[_t],None) or ""):   # a deferred
                s._materialize(out, BYTEPARENT[_t])        # call: evaluate the call first, once
        s._p8=(i, s._bval(d0)) if d0 in BYTEREG else None
        _o2=ops.split(", ",1)                              # `mov bl,[bx]`: read the SOURCE through the
        s._sm=(i, s.memname(_o2[1])) if (mn=="mov" and d0 in BYTEREG and len(_o2)==2 and "[" in _o2[1]) else None   # old bx
        s._pw=(i, getattr(s,BYTEPARENT[d0],None)) if d0 in BYTEREG else None
        _o=ops.split(", ")
        s._s8=(i, s._bval(_o[1])) if (mn=="mov" and len(_o)==2 and _o[0] in BYTEREG and _o[1] in BYTEREG) else None
        s._binval(mn, d0)
        if mn=="mov" and "[" in d0 and ops.rpartition(", ")[2] in BYTEREG:   # store: mem = byte-reg
            src=ops.rpartition(", ")[2]
            if src not in s.breg and s._hascall(getattr(s,BYTEPARENT[src],None) or ""):
                s._materialize(out, BYTEPARENT[src])
            s.emit(out,"%s = %s;"%(s.memname(d0 if "byte ptr" in d0 else "byte ptr "+d0), s._bval(src)))
            return i+1
        if mn=="cmp" and ops.startswith("ax, ") and i+2<len(s.ins) and s.ins[i+1].mnemonic in JCC and s.ins[i+2].mnemonic=="xor":
            s.ax="(%s %s %s)"%(s.ax, JCC[s.ins[i+1].mnemonic], s.memname(ops[4:])); s.retlive=True; s.axsrc=None
            while i<len(s.ins) and s.ins[i].op_str!="ax, bx": i+=1
            return i+1
        if mn=="push":
            if ops=="cs" and i+1<len(s.ins) and s.ins[i+1].mnemonic=="call": return i+1
            if ops=="ax": s.args.append(s.ax if s.ax is not None else "ax")
            elif ops in ("bx","cx","dx","si","di"):
                v=getattr(s,ops,None); s.args.append(v if v is not None else ops)
            else: s.args.append(s.memname(ops) or ops)
            return i+1
        if mn=="call":
            m=re.match(r"(0x[0-9a-f]+|\d+)$", ops)
            args=", ".join(s._take_args(i, "near", s.num(m.group(1)) if m else None))
            if m:
                expr="sub_%x(%s)"%(s.num(m.group(1)), args)
            else:                                              # indirect call: through a ptr/reg
                fp=s.memname(ops) or ops
                expr="(*%s)(%s)"%(fp, args)
            s._call_result(i, out, expr)
            return i+1
        if mn=="lcall":                                        # far call `lcall seg, off`: the target's
            mm=re.match(r"(0x[0-9a-f]+|\d+),\s*(0x[0-9a-f]+|\d+)$", ops)   # load-module offset is seg*16+off
            args=", ".join(s._take_args(i, "far", (s.num(mm.group(1)), s.num(mm.group(2))) if mm else None))
            if mm:
                expr="sub_%x(%s)"%(s.num(mm.group(1))*16 + s.num(mm.group(2)), args)
            else:                                              # indirect far call through a ptr/reg
                fp=s.memname(ops) or ops; expr="(*%s)(%s)"%(fp, args)
            s._call_result(i, out, expr)
            return i+1
        if mn=="mov" and ops=="sp, bp": return i+1             # post-call arg cleanup
        if mn=="mov" and "[" in ops.split(", ")[0]:            # store: mem = reg | immediate
            dest,_,src=ops.rpartition(", ")
            if src=="ax":
                # a reused CONST loaded earlier: rank the store at its LOAD (un-schedule the defer)
                rank = s.axsrc if (s.axsrc is not None and s.axsrc < i) else i
                rc = (s.axsrc is not None) and bool(re.match(r"-?\d+$|-?0x[0-9a-f]+$", str(s.ax)))
                s.emit(out,"%s = %s;"%(s.memname(dest), s.ax if s.ax is not None else "ax"), rank, rc)
                if s.ax is not None and str(s.ax)!="ax": s.ax=s.memname(dest)     # expr -> aliases var
                s.axsrc=None; s.retlive=False; return i+1
            if src in ("bx","cx","dx","si","di"):
                rank = s.bxsrc if (src=="bx" and s.bxsrc is not None and s.bxsrc < i) else i
                rv = getattr(s,src) if getattr(s,src) is not None else src          # untracked -> reg name
                s.emit(out,"%s = %s;"%(s.memname(dest), rv), rank)
                if str(rv)!=src: setattr(s,src,s.memname(dest))
                if src=="bx": s.bxsrc=None
                return i+1                                      # backward alias
            if re.match(r"(0x[0-9a-f]+|\d+)$",src):
                s.emit(out,"%s = %s;"%(s.memname(dest), s.num(src))); return i+1
        if mn in ALU and "[" in ops.split(", ")[0] and ops.endswith(", ax"):   # mem <op>= ax
            d=s.memname(ops.split(",")[0]); av=s.ax if s.ax is not None else "ax"   # untracked -> bare ax
            s.emit(out,"%s = %s %s %s;"%(d,d,ALU[mn],av)); return i+1
        if mn in ALU and "[" in ops.split(", ")[0]:            # mem <op>= imm | reg
            dst,_,src=ops.rpartition(", ")
            if re.match(r"(0x[0-9a-f]+|\d+)$",src) or src in ("bx","cx","dx","si","di"):
                d=s.memname(dst); rv=s.num(src) if re.match(r"(0x[0-9a-f]+|\d+)$",src) else s._val(src)
                s.emit(out,"%s = %s %s %s;"%(d,d,ALU[mn],rv)); return i+1
            if src in BYTEREG:
                d=s.memname(dst if "byte ptr" in dst else "byte ptr "+dst)
                s.emit(out,"%s = %s %s %s;"%(d,d,ALU[mn],s._bval(src))); return i+1
        if mn in ("inc","dec") and ops=="ax":
            base = s.ax if s.ax is not None else "ax"       # untracked -> reg name
            s.ax="(%s %s 1)"%(base,"+" if mn=="inc" else "-"); s.retlive=True; s.axsrc=None; return i+1
        if mn in ("inc","dec") and ops in ("bx","cx","dx","si","di"):
            cur=getattr(s,ops,None)
            s.emit(out,"%s = %s %s 1;"%(ops, ops if cur is None else cur, "+" if mn=="inc" else "-"))
            setattr(s,ops,None)
            if ops=="bx": s.bxsrc=None
            return i+1
        if mn in ("inc","dec") and ops not in BYTEREG:
            nm=s.memname(ops) or ops; o="+" if mn=="inc" else "-"
            # fs>0 means MC1 allocated a temp -> source used the temp form x = x OP 1 (not x++/x--)
            pn=nm if re.match(r"^[A-Za-z_]\w*(->f\d+)*$",nm) or nm.startswith("BYTE(") else "(%s)"%nm   # (*p)++, not *p++
            s.emit(out,"%s = %s %s 1;"%(nm,nm,o) if s.fs>0 else "%s%s;"%(pn,"++" if mn=="inc" else "--"))
            return i+1
        if mn=="int":                                          # software interrupt -> named syscall
            s.emit(out,"int_%s();"%ops.replace("0x","").replace(" ","")); return i+1
        if mn in ("shl","sal","shr","sar") and "[" in ops.split(",")[0]:   # memory-dest shift
            d=s.memname(ops.split(",")[0])
            cnt=(s.breg["cl"] if "cl" in s.breg else (s.cx if s.cx is not None else "cx")) \
                if ops.endswith("cl") else s._val(ops.split(", ")[1])
            op="<<" if mn in ("shl","sal") else ">>"; s.emit(out,"%s = %s %s %s;"%(d,d,op,cnt)); return i+1
        if mn in ("neg","not") and "[" in ops:                 # memory-dest unary
            d=s.memname(ops); s.emit(out,"%s = %s%s;"%(d,"-" if mn=="neg" else "~",d)); return i+1
        # byte-register shift/unary: al/bl/.. are low sub-registers the word model doesn't track,
        # so emit them as statements on the exposed byte-register variable (faithful transcription).
        if d0 in BYTEREG and mn in ("shl","sal","shr","sar"):
            cur=s._p8[1] if s._p8 else d0
            cnt=(s.breg["cl"] if "cl" in s.breg else (s.cx if s.cx is not None else "cl")) \
                if ops.endswith("cl") else s._val(ops.split(", ")[1])
            op="<<" if mn in ("shl","sal") else ">>"
            s.breg[d0]="((%s %s %s) & 0xff)"%(cur,op,cnt); return i+1
        if d0 in BYTEREG and mn in ("neg","not"):
            cur=s._p8[1] if s._p8 else d0
            s.breg[d0]="((%s%s) & 0xff)"%("-" if mn=="neg" else "~",cur); return i+1
        # ALU on a non-ax word register (bx/cx/dx/si/di): the symbolic model tracks only ax, so
        # these were silently dropped. Emit as statements on the register variable (clearing any
        # stale pointer track so later derefs stay consistent with the bare name). sub/xor r,r is
        # a clear; or r,r is a flag-only test (consumed by cond_at) -> no statement.
        if mn in ALU and d0 in ("bx","cx","dx","si","di") and len(ops.split(", "))==2:
            src=ops.split(", ")[1].strip()
            if src==d0 and mn in ("sub","xor") and d0=="dx" \
               and i+1<len(s.ins) and s.ins[i+1].mnemonic in ("div","idiv"):
                setattr(s,"dx",None); return i+1          # `sub/xor dx,dx` before div = setup, no stmt
            if src==d0 and mn in ("sub","xor"):
                s.emit(out,"%s = 0;"%d0)
            elif src==d0 and mn=="or":
                pass
            else:
                rhs=s._val(src) if src in ("ax","bx","cx","dx","si","di") else s.rhs(src)
                cur=getattr(s,d0,None)             # reg = <its current value> op rhs (the value lives
                s.emit(out,"%s = %s %s %s;"%(d0, d0 if cur is None else cur, ALU[mn], rhs))   # in the reg)
            setattr(s,d0,None)
            return i+1
        # 16-bit string ops. Block (rep-prefixed) forms -> the C library idiom; single-byte/word
        # forms -> pointer deref with post-increment. di/si/cx/al come from the tracked registers
        # (fall back to the register name, never None). Comparison forms (scas/cmps) are left
        # UNMODELED on purpose (flag/compare semantics) so the correctness gate declines them.
        # rep/repne movs|stos: on string moves/stores the repne (F2) prefix is identical to rep
        # (F3) -- neither movs nor stos tests ZF -- so treat both as the block idiom. The word
        # (movsw/stosw) forms count CX *words*, so the byte length passed to memcpy/memset is CX*2.
        if mn in ("rep movsb","rep movsw","repne movsb","repne movsw"):
            n=s.cx or "cx"
            if mn.endswith("movsw"): n=str(int(n)*2) if str(n).isdigit() else "(%s * 2)"%n
            s.emit(out,"%s(%s, %s, %s);"%(("memcpy_w" if mn.endswith("w") else "memcpy_b") if MARK else "memcpy",
                                          s.di or "di", s.si or "si", n))
            s.si=s.di=None; s.cx="0"; return i+1
        if mn in ("rep stosb","rep stosw","repne stosb","repne stosw"):
            n=s.cx or "cx"
            if mn.endswith("stosw"): n=str(int(n)*2) if str(n).isdigit() else "(%s * 2)"%n
            s.emit(out,"%s(%s, %s, %s);"%(("memset_w" if mn.endswith("w") else "memset_b") if MARK else "memset",
                                          s.di or "di", s.ax if s.ax is not None else "ax", n))
            s.di=None; s.cx="0"; return i+1
        # Single string instructions (hand-written assembly loops), direction flag clear: each is
        # a load/store through si/di followed by the pointer step, written as plain statements on
        # the register variables. Every register involved is settled into its variable first.
        if mn in ("movsb","movsw","stosb","stosw","lodsb","lodsw","xlatb"):
            w=2 if mn.endswith("w") else 1
            def pinreg(r):
                v=getattr(s,r,None)
                if v is not None and str(v)!=r: s._materialize(out,r)
                setattr(s,r,None)
            def settle_ax():
                pinreg("ax")
                for b8 in ("al","ah"):
                    if b8 in s.breg:
                        v=s.breg.pop(b8)
                        if str(v)!=b8: s.emit(out,"%s = %s;"%(b8,v))
            B=(lambda x:"BYTE(%s)"%x) if MARK else (lambda x:"*(char *)%s"%x[1:] if x.startswith("*") else x)
            if mn[:4]=="movs":
                pinreg("si"); pinreg("di")
                s.emit(out,("%s = %s;"%(B("*di"),B("*si"))) if w==1 else "*di = *si;")
                s.emit(out,"si = si + %d;"%w); s.emit(out,"di = di + %d;"%w)
            elif mn[:4]=="stos":
                pinreg("di"); settle_ax()
                s.emit(out,("%s = al;"%B("*di")) if w==1 else "*di = ax;")
                s.emit(out,"di = di + %d;"%w)
            elif mn[:4]=="lods":
                pinreg("si"); settle_ax()
                s.emit(out,("al = %s;"%B("*si")) if w==1 else "ax = *si;")
                s.emit(out,"si = si + %d;"%w); s.axsrc=None; s.retlive=True
            else:                                              # xlatb: al = table[al], table at bx
                pinreg("bx"); settle_ax()
                s.emit(out,"al = %s;"%B("*(bx + al)")); s.axsrc=None; s.retlive=True
            return i+1
        if mn=="jmp":                                          # tail-call: jmp to an external target
            m=re.match(r"(0x[0-9a-f]+|\d+)$", ops)
            if m:
                t=s.num(m.group(1))
                if t>s.ins[-1].address or t<s.ins[0].address:  # outside this function -> tail call
                    s.emit(out,"return sub_%x(%s);"%(t & 0xFFFF, ", ".join(str(a) for a in reversed(s.args)))); s.args=[]
                elif s.epi is not None and t>=s.epi and s.ax is not None and s.retlive:
                    s.emit(out,"return %s;"%s.ax)              # jmp to common epilogue w/ live result
                elif not (i+1<len(s.ins) and t==s.ins[i+1].address):   # not a no-op jmp-to-next
                    s.gototargets.add(t); s.emit(out,"goto L_%x;"%t)    # unstructured jump -> goto+label
            return i+1
        if mn=="ret" and s.ax is not None and s.retlive: s.emit(out,"return %s;"%s.ax); return i+1
        s.expr_step(i); return i+1

    def _reset(s):
        """Reset value-tracking + emission state for goto pass 2, keeping gototargets/tgt/epi."""
        s.ax=s.si=s.di=s.cx=s.bx=s.dx=s.div=None; s.breg={}
        s.maxidx=-1; s.lmax=-1; s.args=[]; s.longfn=False; s.floatfn=False
        s.hoisted=set(); s.out=None; s.i=0; s.retlive=False; s.axsrc=None; s.bxsrc=None
        s.ranks=[]; s.regconst=[]; s.locals=set(); s.callstmt=False

    _REGS=("ax","si","di","cx","bx","dx")
    def _snap(s):
        d={r:getattr(s,r) for r in s._REGS}
        d.update(div=s.div, retlive=s.retlive, axsrc=s.axsrc, bxsrc=s.bxsrc, args=list(s.args), breg=dict(s.breg), i=s.i)
        return d
    def _rest(s,d):
        for r in s._REGS: setattr(s,r,d[r])
        s.div=d["div"]; s.retlive=d["retlive"]; s.axsrc=d["axsrc"]; s.bxsrc=d["bxsrc"]; s.args=list(d["args"]); s.breg=dict(d["breg"])
        s.i=d.get("i",s.i)                                 # "where we are" drives the liveness queries
    @staticmethod
    def _terminal(body):
        """A branch body that already ends in break/continue/return/goto does NOT fall through to
        the merge, so phi materialization appended to it would be unreachable (dead) code."""
        for ln in reversed(body):
            if ln.strip():
                return re.match(r"(break|continue|return\b|goto )", ln.strip()) is not None
        return False

    def _phi_args(s, a_args, b_args, a_body, b_body, a_term, b_term):
        """Merge the pending-argument stacks of two converging paths: a slot the paths filled
        differently becomes a temp assigned on each path (`if (c) t = x; else t = y; f(t)`)."""
        rr0=re.compile(r"(?<![\w>.])(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh)(?![\w(])")
        def one(args, body):                               # only one path reaches the merge: its
            out=[]                                         # register-reading args are still frozen,
            for x in args:                                 # because the merge may assign the register
                if rr0.search(str(x)):
                    s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn
                    body.append("%s = %s;"%(t,x)); out.append(t)
                else: out.append(x)
            s.args=out
        if a_term and not b_term: one(b_args, b_body); return
        if b_term and not a_term: one(a_args, a_body); return
        if len(a_args)!=len(b_args): return
        # (an argument whose text reads a REGISTER is frozen too, even when both paths agree on
        #  the text: the merge may assign that register variable right after)
        rr=re.compile(r"(?<![\w>.])(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh)(?![\w(])")
        if list(a_args)==list(b_args) and not any(rr.search(str(x)) for x in a_args): return
        out=[]
        for x,y in zip(a_args,b_args):
            if str(x)==str(y) and not rr.search(str(x)): out.append(x); continue
            s._tn=getattr(s,"_tn",0)+1; t="t%d"%s._tn
            a_body.append("%s = %s;"%(t,x)); b_body.append("%s = %s;"%(t,y)); out.append(t)
        s.args=out

    def _phi(s, then_snap, else_snap, then_b, else_b):
        """Merge two branch register-states. For a register the branches leave DIFFERENT, emit
        `reg = <value>;` at the end of each branch (materializing the conditional assignment that
        value-tracking would otherwise defer and lose at the merge) and expose the bare register
        afterwards; where the branches agree, keep the shared value. A branch that ends in a
        terminal (break/return/...) does not reach the merge -> skip its materialization (dead)."""
        tterm, eterm = s._terminal(then_b), s._terminal(else_b)
        s._phi_args(then_snap["args"], else_snap["args"], then_b, else_b, tterm, eterm)
        for r in s._REGS:
            tv, ev = then_snap[r], else_snap[r]
            if tv==ev:
                setattr(s, r, tv); continue
            if r in s._WR and not s._live(r, s.i) and not s._hascall(tv) and not s._hascall(ev):
                setattr(s, r, None); continue              # dead after the merge: nothing to keep
            if tv is not None and str(tv)!=r and not tterm: then_b.append("%s = %s;"%(r,tv))
            if ev is not None and str(ev)!=r and not eterm: else_b.append("%s = %s;"%(r,ev))
            setattr(s, r, None)                      # merged -> bare register after the branch
        tbr, ebr = then_snap["breg"], else_snap["breg"]       # 8-bit sub-registers set per branch
        for r in BYTEREG:
            tv, ev = tbr.get(r), ebr.get(r)
            if tv==ev:
                if tv is not None: s.breg[r]=tv
                continue
            if tv is not None and str(tv)!=r and not tterm: then_b.append("%s = %s;"%(r,tv))
            if ev is not None and str(ev)!=r and not eterm: else_b.append("%s = %s;"%(r,ev))
            s.breg.pop(r, None)

    def _body(s,lo,hi):
        """walk a structured body; if it comes back empty, its work was side-effect calls whose
        results were tracked as values and never emitted -- re-lift the SAME range with calls
        forced to statements so the branch isn't silently dropped. Additive by construction: it
        only changes output when the normal walk produced nothing, so it can't regress a body
        that already had statements. Value-state is snapshotted and restored around the re-lift."""
        body=s.walk(lo,hi)
        if any(b.strip() for b in body) or s.callstmt:
            return body
        snap=(s.ax,s.si,s.di,s.bx,s.cx,s.dx,list(s.args),s.div,s.retlive,s.axsrc,s.bxsrc,s.out,dict(s.breg))
        s.callstmt=True
        body2=s.walk(lo,hi)
        s.callstmt=False
        s.ax,s.si,s.di,s.bx,s.cx,s.dx,s.args,s.div,s.retlive,s.axsrc,s.bxsrc,s.out,s.breg=snap
        return body2 if any(b.strip() for b in body2) else body

    def walk(s,lo,hi):
        out=[]; i=lo
        while i<hi:
            s.out=out                                          # current block (for hoisted stores)
            if s.emitgoto and s.ins[i].address in s.gototargets:  # label a goto target (pass 2),
                out.append("L_%x:"%s.ins[i].address)              # before structuring/stepping it
            # do-while: i is the target of a backward CONDITIONAL jump (test at the bottom)
            backc=[j for j,t in s.tgt.items() if t==i and s.ins[j].mnemonic in CJMP and i<j<hi]
            if backc:
                J=min(backc); ci=J-1
                while ci>i and s.ins[ci].mnemonic not in ("cmp","test"): ci-=1
                cm=s.ins[ci]; jcc=s.ins[J].mnemonic
                if cm.mnemonic=="cmp":
                    p=cm.op_str.split(", ")
                    def rv(t):
                        if t=="ax": return s.ax if s.ax is not None else "ax"
                        if t=="bx": return s.bx if s.bx is not None else "bx"
                        return s.memname(t) or (str(s.num(t)) if re.match(r"(0x[0-9a-f]+|\d+)$",t) else t)
                    l,r=rv(p[0]),rv(p[1])
                    cond=(l if str(r)=="0" else "(%s %s %s)"%(l, JCC.get(jcc, "!=" if jcc=="jne" else "=="), r))
                else: cond=s.ax if s.ax is not None else "ax"   # bare value test -> the register
                body=s._body(i, ci)
                out.append("do {"); out+=["    "+b for b in body]; out.append("} while (%s);"%cond)
                i=J+1; continue
            # while-loop: some later jmp targets i (back-edge to a loop top)
            back=[j for j,t in s.tgt.items() if t==i and s.ins[j].mnemonic=="jmp" and i<j<hi]
            if back:
                J=min(back); cond,jcc=s.cond_at(i); exit_idx=s.tgt[jcc]
                body=s._body(jcc+1, J)
                out.append("while (%s) {"%cond); out+=["    "+b for b in body]; out.append("}")
                i=J+1; continue
            # if: cmp/test + forward conditional jump that skips a block (not materialized).
            # A branch whose target address isn't an instruction boundary in this range has
            # tgt == None -> decline the structured form and fall through to linear emission
            # (never crash, never guess the structure).
            tt=s.tgt.get(i+1)
            if s.ins[i].mnemonic in ("cmp","test") and i+1<hi and s.ins[i+1].mnemonic in CJMP \
               and tt is not None and tt>i+1 and not (i+2<len(s.ins) and s.ins[i+2].mnemonic=="xor"):
                cond,jcc=s.cond_at(i); tgt=s.tgt.get(jcc)
                if tgt is not None and not (jcc+1<=tgt<=hi):
                    # conditional jump OUT of this block (e.g. a short-circuit guard to a shared
                    # else/exit): structuring would drop it, so represent it as a guarded goto.
                    ta=s.ins[tgt].address if 0<=tgt<len(s.ins) else (tgt & 0xFFFF)
                    s.gototargets.add(ta); out.append("if (!(%s)) goto L_%x;"%(cond, ta))
                    i=jcc+1; continue
                if tgt is not None and jcc+1<=tgt<=hi:
                    # if-else: the then-block ends with `jmp END` that skips an else-block
                    # [tgt, END).  END must be a forward join strictly past the else; a jmp to the
                    # epilogue is a `return` (plain if), so require END before the epilogue.
                    je=tgt-1
                    end=s.tgt.get(je) if (je>=jcc+1 and s.ins[je].mnemonic=="jmp") else None
                    end_ok=(end is not None and tgt<end<=hi
                            and (s.epi is None or s.ins[end].address<s.epi))
                    if end_ok:
                        pre=s._snap()
                        then_b=s._body(jcc+1, je); then_s=s._snap()
                        s._rest(pre); else_b=s._body(tgt, end); else_s=s._snap()
                        s._phi(then_s, else_s, then_b, else_b)   # materialize conditional assignments
                        out.append("if (%s) {"%cond); out+=["    "+b for b in then_b]
                        out.append("} else {"); out+=["    "+b for b in else_b]; out.append("}")
                        i=end; continue
                    # plain if: phi against the implicit else (registers keep their pre-if value).
                    # A register the then-branch changes is materialized `reg = <then>;` inside and
                    # `reg = <pre>;` BEFORE the if (so the not-taken path is defined), then exposed
                    # as the bare register -- otherwise `if(c){reg=V}; use reg` kept V unconditionally.
                    pre=s._snap()
                    body=s._body(jcc+1, tgt); then_s=s._snap()
                    for r in s._REGS:
                        if then_s[r]==pre[r]:
                            setattr(s,r,then_s[r]); continue
                        pv,tv=pre[r],then_s[r]
                        if pv is not None and str(pv)!=r: out.append("%s = %s;"%(r,pv))
                        if tv is not None and str(tv)!=r: body.append("%s = %s;"%(r,tv))
                        setattr(s,r,None)
                    out.append("if (%s) {"%cond); out+=["    "+b for b in body]; out.append("}")
                    i=tgt; continue
            i=s.step(i,out)
        return out

    # ------------------------------------------------------------------ CFG tree emitter (Phase B)
    _NEG = {"==":"!=","!=":"==","<":">=",">=":"<",">":"<=","<=":">"}
    def _negate(s, cond):
        m = re.match(r"^\((.*) (==|!=|<=|>=|<|>) (.*)\)$", cond)
        if m: return "(%s %s %s)" % (m.group(1), s._NEG[m.group(2)], m.group(3))
        return "!(%s)" % cond

    def _cmp_start(s, jcc_idx, lo):
        """Index where the condition feeding the jcc begins: the nearest cmp/test at or before the
        jcc (within the block), else the jcc itself (a bare-value test)."""
        # Only PURE REGISTER LOADS may sit between the compare and the jump (they are replayed
        # value-only by cond_at). Anything else -- an ALU op that resets the flags, a store, a
        # call -- means the instructions up to the jump are ordinary statements: emit them all,
        # and let cond_at find the flag source (it used to grab an earlier, unrelated cmp and
        # silently skip `sbb ax,ax; neg ax; mov [x],al` in between).
        j = jcc_idx - 1
        while j >= lo:
            mn = s.ins[j].mnemonic; d0 = (s.ins[j].op_str or "").split(",")[0].strip()
            if mn in ("cmp", "test"): return j
            if mn in ("cbw", "cwde", "nop") or (mn in ("mov", "lea", "les") and "[" not in d0
                                                 and d0 in s._WR + BYTEREG):
                j -= 1; continue
            break
        return jcc_idx

    def _emit_range(s, lo, hi, out):
        """Emit straight-line statements for instruction indices [lo, hi) via step().
        Skips indices before s.emit_start (the prologue)."""
        i = max(lo, getattr(s, "emit_start", 0))
        while i < hi:
            ni = s.step(i, out)
            i = ni if ni > i else i + 1

    def _emit_block(s, blk, out):
        """A leaf basic block: emit its statements, dropping the terminator (the tree owns control
        flow). A ret with a live ax becomes `return <ax>`; an out-of-range jmp is a tail call."""
        end = blk.hi - 1 if blk.term in ("jmp", "cond", "ret", "tailcall") else blk.hi
        s._emit_range(blk.lo, end, out)
        if blk.term == "ret" and s.ax is not None and s.retlive:
            out.append("return %s;" % s.ax)
        elif blk.term == "ret" and s._retval():            # value-returning fn, result merged into
            v = s.ax if s.ax is not None else (s.breg.get("al") or "ax")   # the register variable
            out.append("return %s;" % v)
        elif blk.term == "tailcall":
            last = s.ins[blk.hi - 1]
            m = re.match(r"(0x[0-9a-f]+|\d+)$", last.op_str)
            if m:
                out.append("return sub_%x(%s);" % (int(m.group(1), 0) & 0xFFFF,
                            ", ".join(str(a) for a in reversed(s.args)))); s.args = []

    def _emit_cond(s, blk, out, flush=None):
        """Emit a cond block's leading statements and return its branch condition (fall-through
        sense, matching cond_at/INV)."""
        jcc = blk.hi - 1
        cs = s._cmp_start(jcc, blk.lo)
        s._emit_range(blk.lo, cs, out)
        s.out = out                                # statements cond_at pins/hoists belong HERE
        s._cond_lo = blk.lo
        if cs > blk.lo or any(x.mnemonic in ("call","lcall") for x in s.ins[blk.lo:jcc]): pass
        if flush is not None: flush(out)
        s._cmp = None                              # avoid a stale compare leaking into a bare-value test
        cond, _ = s.cond_at(cs)
        return cond

    def _flush_calls(s, snap, pre, body):
        """A structured branch ended with a CALL still deferred in a tracked register (not yet
        stored/passed/tested). Emit it in the branch (`reg = f();`) -- otherwise the branch can
        look empty and be collapsed, silently dropping the call."""
        if s._terminal(body): return
        for r in s._REGS:
            v=snap.get(r)
            if v is not None and v!=pre.get(r) and s._hascall(v):
                body.append("%s = %s;"%(r,v)); snap[r]="ax" if r=="ax" else None

    @staticmethod
    def _selfref(r, v):
        """Does the lazily tracked text `v` read the register variable `r` (or a half of it)?
        Once `r = v;` is emitted, `v` no longer denotes the same value."""
        names = {r} | {h for h, par in BYTEPARENT.items() if par == r or h == r} | ({BYTEPARENT[r]} if r in BYTEPARENT else set())
        return any(re.search(r"(?<![\w>.])%s(?![\w(])" % n, str(v)) for n in names)

    def _plain_if(s, cond, body, pre, then_s, out):
        """Emit a plain if(cond){body}: materialize the phi against the implicit (not-taken) else --
        a register the body changes is set `reg=<then>;` inside and `reg=<pre>;` before, then exposed
        bare. Skips emitting when the body is empty (a no-op guard)."""
        bterm = s._terminal(body)                              # body falls through to the merge?
        s._phi_args(then_s["args"], pre["args"], body, out, bterm, False)
        for r in s._REGS:
            if then_s[r] == pre[r]:
                setattr(s, r, then_s[r]); continue
            if r in s._WR and not s._live(r, s.i) and not s._hascall(pre[r]) and not s._hascall(then_s[r]):
                setattr(s, r, None); continue                  # dead after the merge
            if pre[r] is not None and str(pre[r]) != r:
                out.append("%s = %s;" % (r, pre[r]))
                if (s._hascall(pre[r]) or s._selfref(r, pre[r])) and str(pre[r]) in cond:
                    cond = cond.replace(str(pre[r]), r)        # don't re-evaluate the call in the test
                                                               # (nor a value that reads r itself)
            if then_s[r] is not None and str(then_s[r]) != r and not bterm: body.append("%s = %s;" % (r, then_s[r]))
            setattr(s, r, None)
        pbr, tbr = pre["breg"], then_s["breg"]                 # 8-bit sub-registers vs the implicit else
        for r in BYTEREG:
            pv, tv = pbr.get(r), tbr.get(r)
            if pv == tv:
                if tv is not None: s.breg[r] = tv
                continue
            if pv is not None and str(pv) != r:
                out.append("%s = %s;" % (r, pv))
                if s._selfref(r, pv) and str(pv) in cond:      # `al = al << n; if (al & m)` -- not the
                    cond = cond.replace(str(pv), r)            # shift applied a second time in the test
            if tv is not None and str(tv) != r and not bterm: body.append("%s = %s;" % (r, tv))
            s.breg.pop(r, None)
        if any(x.strip() for x in body):
            out.append("if (%s) {" % cond); out += ["    " + x for x in body]; out.append("}")

    def _emit_seq(s, blocks, nodes, out):
        for node in nodes:
            s._emit_node(blocks, node, out)

    def _emit_node(s, blocks, node, out):
        kind = node[0]
        if kind in ("block", "if", "ifelse", "while", "loop", "dowhile", "switch") and getattr(s, "labels", None):
            a = blocks[node[1]].addr
            if a in s.labels and a not in s._labeled:
                # A goto lands here with every register in its variable, so the fall-in path must
                # agree: pin what it still tracks lazily (nothing to pin after a terminal).
                # Arguments already pushed when control arrives (pushed on each path before the
                # shared call) travel in per-slot temporaries so every path agrees on them.
                na = getattr(s, "gotoargs", {}).get(a, 0)
                if s._terminal(out):
                    for r in s._WR: setattr(s, r, None)
                    s.breg = {}
                else:
                    s.i = blocks[node[1]].lo - 1           # liveness as seen from the label
                    s._pin(out, set(s._WR))
                    for k2, v2 in enumerate(s.args[-na:] if na else []):
                        out.append("targ%d = %s;" % (k2, v2))
                if na: s.args = (s.args[:-na] if len(s.args) >= na else []) + ["targ%d" % k2 for k2 in range(na)]
                out.append("L_%x:" % a); s._labeled.add(a)
        if kind == "block":
            s._emit_block(blocks[node[1]], out)
        elif kind == "if":
            b, then_t, join, neg = node[1], node[2], node[3], node[4]
            cond = s._emit_cond(blocks[b], out)
            if neg: cond = s._negate(cond)
            pre = s._snap()
            body = []; s._emit_seq(blocks, then_t, body); then_s = s._snap()
            s._flush_calls(then_s, pre, body)
            s._rest(pre)
            s._plain_if(cond, body, pre, then_s, out)
        elif kind == "ifelse":
            b, then_t, else_t, join = node[1], node[2], node[3], node[4]
            cond = s._emit_cond(blocks[b], out)
            pre = s._snap()
            tb = []; s._emit_seq(blocks, then_t, tb); ts = s._snap()
            s._flush_calls(ts, pre, tb)
            s._rest(pre)
            eb = []; s._emit_seq(blocks, else_t, eb); es = s._snap()
            s._flush_calls(es, pre, eb)
            # Merge FIRST (each branch gets the register values it leaves that are read later),
            # THEN drop a branch that is still empty. Collapsing before the merge lost a branch
            # whose only effect was a register result (`else ax = 0`).
            s._phi(ts, es, tb, eb)
            te, ee = any(x.strip() for x in tb), any(x.strip() for x in eb)
            if te and ee:
                out.append("if (%s) {" % cond); out += ["    " + x for x in tb]
                out.append("} else {"); out += ["    " + x for x in eb]; out.append("}")
            elif te:
                out.append("if (%s) {" % cond); out += ["    " + x for x in tb]; out.append("}")
            elif ee:
                out.append("if (%s) {" % s._negate(cond)); out += ["    " + x for x in eb]; out.append("}")
        elif kind in ("while", "loop"):
            b, body_t = node[1], node[2]
            # Loop-carried registers: value tracking is lazy (a register holds an expression until
            # it is consumed), which is only valid along straight-line/forked flow. Anything a
            # loop writes must live in the register variable at the loop head, at every back-edge
            # (end of body / continue) and at every break -- pinned here.
            W = s._loop_written(blocks, s._tree_bids(body_t, {b}))
            s._pin(out, W | s._mem_lazy())
            s._loops = getattr(s, "_loops", []); s._loops.append(W)
            if kind == "while":
                lead = []
                cond = s._emit_cond(blocks[b], lead)
                if len(node) > 4 and node[4]: cond = s._negate(cond)
                if s._has_break(body_t): s._pin(lead, W)       # exits must agree on the state
                hs = s._snap()
                body = []; s._emit_seq(blocks, body_t, body)
                if not s._terminal(body): s._pin(body, W)
                while body and body[-1].strip() == "continue;": body.pop()    # falls to the test anyway
                s._loops.pop(); s._rest(hs)
                if any(x.strip() for x in lead):
                    # the header block has statements before its test: they run EVERY iteration,
                    # so they belong inside the loop (hoisting them ran them once -> wrong).
                    out.append("for (;;) {"); out += ["    " + x for x in lead]
                    out.append("    if (%s) break;" % s._negate(cond))
                    out += ["    " + x for x in body]; out.append("}")
                else:
                    out.append("while (%s) {" % cond); out += ["    " + x for x in body]; out.append("}")
            else:                                  # endless/complex loop
                body = []; s._emit_seq(blocks, body_t, body)
                if not s._terminal(body): s._pin(body, W)
                while body and body[-1].strip() == "continue;": body.pop()
                s._loops.pop()
                for r in W: setattr(s, r, None)
                for b8 in [x for x in s.breg if BYTEPARENT[x] in W]: s.breg.pop(b8)
                out.append("for (;;) {"); out += ["    " + x for x in body]; out.append("}")
        elif kind == "dowhile":
            b, body_t, latch = node[1], node[2], node[3]
            W = s._loop_written(blocks, s._tree_bids(body_t, {b, latch}))
            s._pin(out, W | s._mem_lazy())
            s._loops = getattr(s, "_loops", []); s._loops.append(W)
            body = []; s._emit_seq(blocks, body_t[:-1], body)      # body minus the latch block node
            cond = s._emit_cond(blocks[latch], body, flush=lambda o: s._pin(o, W))
            if len(node) > 4 and node[4]: cond = s._negate(cond)
            s._loops.pop()
            for r in W: setattr(s, r, None)
            for b8 in [x for x in s.breg if BYTEPARENT[x] in W]: s.breg.pop(b8)
            out.append("do {"); out += ["    " + x for x in body]; out.append("} while (%s);" % cond)
        elif kind == "switch":
            b, cases = node[1], node[2]
            blk = blocks[b]; hi = blk.hi
            t3 = [(x.mnemonic, x.op_str.replace(" ", "")) for x in s.ins[max(blk.lo, hi-3):hi]]
            if len(t3) != 3 or t3[1] != ("xchg", "bx,ax") or t3[0] not in (("add", "ax,ax"), ("shl", "ax,1")):
                raise ValueError("unrecognized jump-table dispatch")
            s._emit_range(blk.lo, hi-3, out)
            allr = set(s._WR)
            if s._hascall(s.ax or ""): s._materialize(out, "ax")
            sel = s.ax if s.ax is not None else "ax"
            s._pin(out, allr - {"ax", "bx"})               # arms start from plain register variables
            s.ax = None; s.bx = None; s.breg = {}          # (the dispatch itself clobbers ax/bx)
            pre = s._snap(); arms = []
            for vals, arm in cases:
                s._rest(pre); body = []; s._emit_seq(blocks, arm, body)
                if not s._terminal(body): s._pin(body, allr)
                arms.append((vals, body))
            for r in allr: setattr(s, r, None)
            s.breg = {}
            if any(s._has_break(arm) for _, arm in cases):
                # an arm leaves an enclosing LOOP with `break`: inside a C switch that would only
                # leave the switch, so render the dispatch as an if-chain on the selector.
                s._tn = getattr(s, "_tn", 0) + 1; t = "t%d" % s._tn
                out.append("%s = %s;" % (t, sel))
                def chain(k, ind):
                    if k == len(arms): return []
                    vals, body = arms[k]
                    c = " || ".join("(%s == %d)" % (t, v) for v in vals)
                    L = [ind + "if (%s) {" % c] + [ind + "    " + x for x in body]
                    rest = chain(k + 1, ind + "    ")
                    return L + ([ind + "} else {"] + rest + [ind + "}"] if rest else [ind + "}"])
                out += chain(0, "")
            else:
                out.append("switch (%s) {" % sel)
                for vals, body in arms:
                    out += ["case %d:" % v for v in vals]
                    out += ["    " + x for x in body]
                    if not s._terminal(body): out.append("    break;")
                out.append("}")
        elif kind == "break":
            if getattr(s, "_loops", None): s._pin(out, s._loops[-1])
            out.append("break;")
        elif kind == "continue":
            if getattr(s, "_loops", None): s._pin(out, s._loops[-1])
            out.append("continue;")
        elif kind == "goto":
            b, t = node[1], node[2]
            if t is not None:
                ta = blocks[t].addr                        # t is a BLOCK id
                s.i = blocks[t].lo - 1                     # liveness as seen from the TARGET
                s._pin(out, set(s._WR))                    # registers live in their variables at a goto
                s.gotoargs = getattr(s, "gotoargs", {})
                if s.args:
                    s.gotoargs[ta] = max(s.gotoargs.get(ta, 0), len(s.args))
                    for k2, v2 in enumerate(s.args): out.append("targ%d = %s;" % (k2, v2))
                s.gototargets.add(ta); out.append("goto L_%x;" % ta)

    def emit_tree(s, blocks, tree):
        top = []; s.out = top                      # s.out is re-pointed at branch lists while
        s._emit_seq(blocks, tree, top)             # emitting -- return the TOP-LEVEL list
        return top

CX_OP = {"CXA":"+","CXS":"-","CXM":"*","CXD":"/"}   # helper family -> binary operator

def build_cxmap(image, libpaths):
    """{image_offset -> operator} for CX* arithmetic helpers located in a linked image."""
    import struct
    from src.engines.inverse.libmatch import iter_modules, module_functions, masked_eq
    cxmap={}
    for lib in libpaths:
        d=open(lib,"rb").read()
        for ms in iter_modules(d):
            for nm,code,mask in module_functions(d,ms):
                op=CX_OP.get(nm[:3].upper())
                if not op or len(code)<6: continue
                L=len(code); c0=bytes([code[0]]); pos=image.find(c0)
                while pos!=-1 and pos+L<=len(image):
                    if masked_eq(code,mask,image[pos:pos+L]): cxmap[pos]=op
                    pos=image.find(c0,pos+1)
    return cxmap

def _dead_store_recover(stmts, rc):
    """Recover MC2's dead-store elimination. `a=X; b=a; a=Y`: the compiler elides a's first
    store (a is overwritten) but keeps X live in a register to serve `b=a`, routing b's store
    through the register (6B) instead of an immediate (5B). mc2inv sees `b=X` as a
    register-routed const store (rc=True) with b NOT used as a copy source -> the register
    routing is unjustified, signalling an elided dead store. Reconstruct `a=X; b=a`, where a
    is the UNIQUE other local (re)assigned elsewhere. Gated on exactly one host so it never
    fires ambiguously; leaves the statement unchanged otherwise."""
    def is_used(V):                                    # V read as an rvalue anywhere (copy OR expression)?
        return any(re.search(r"\b%s\b"%re.escape(V), t.split(" = ",1)[1]) for t in stmts if " = " in t)
    res=[]
    for idx,s in enumerate(stmts):
        m=re.match(r"(v\d+) = (-?\d+);$", s)
        if idx<len(rc) and rc[idx] and m and not is_used(m.group(1)):
            V,C=m.group(1),m.group(2)
            hosts=[]
            for t in stmts:
                mt=re.match(r"(v\d+) = ", t)
                if mt and mt.group(1)!=V and mt.group(1) not in hosts: hosts.append(mt.group(1))
            if len(hosts)==1:
                W=hosts[0]
                res.append("%s = %s;"%(W,C))       # recovered dead store  (a = X)
                res.append("%s = %s;"%(V,W))       # the copy that kept it live  (b = a)
                continue
        res.append(s)
    return res

def recover(code, base=0, cxmap=None):
    ins=list(_MD.disasm(code,0))
    if len(ins) < 2:
        return 0, [], [], "int", []                       # too small to be a real function
    fs=0; k=1
    if ins[k].mnemonic=="sub" and ins[k].op_str.startswith("sp"):
        fs=int(ins[k].op_str.split(",")[1],16); k+=1
        while k<len(ins) and "bp, sp" not in ins[k].op_str: k+=1
    k+=1
    # MS-C 3.00 stack-check prologue (absent in 2.03): after `mov bp,sp`, the frame is
    # allocated by `mov ax,<framesize>; call __chkstk` (or `xor ax,ax; call __chkstk` for a
    # zero-size frame) rather than an explicit `sub sp,N`. Detect it, take the framesize from
    # the ax load, skip both, and switch to the 3.0 frame convention (bp at the frame top:
    # locals [bp-N], params [bp+N]). Gated on fs==0 so a genuine 2.03 `sub sp` frame is kept.
    msc3=False
    if fs==0 and k+1 < len(ins) and ins[k+1].mnemonic=="call":
        if ins[k].mnemonic=="mov" and ins[k].op_str.startswith("ax,"):
            try:
                fs=int(ins[k].op_str.split(",")[1].strip(),16); msc3=True; k+=2
            except ValueError:
                pass
        elif ins[k].mnemonic=="xor" and ins[k].op_str.replace(" ","")=="ax,ax":
            fs=0; msc3=True; k+=2
    r=R(ins,fs,base,cxmap,msc3=msc3); stmts=r.walk(k,len(ins))
    # Goto fallback: unstructured jumps/branches emitted `goto L_x` + registered target x. If any
    # fired, re-walk with emitgoto so the (possibly earlier) target labels get placed (2-pass).
    if r.gototargets:
        r._reset(); r.emitgoto=True; stmts=r.walk(k,len(ins))
        stmts=_fold_shortcircuit(stmts)                    # recover && short-circuits from guarded gotos
    # UN-SCHEDULE: MC2 defers a reused const's store while its load stays at the source
    # position. For straight-line blocks, reorder statements by source rank (load index for
    # reused consts, store index otherwise) so the recompile reproduces the same emission.
    if stmts and len(r.ranks)==len(stmts) and not any("{" in x or "}" in x or "goto" in x or x.startswith("L_") for x in stmts):
        perm=sorted(range(len(stmts)), key=lambda j: r.ranks[j])
        stmts=[stmts[j] for j in perm]; rc=[r.regconst[j] for j in perm]
        stmts=_dead_store_recover(stmts, rc)               # recover elided dead stores + copies
    loc=sorted(r.locals)
    if r.floatfn or r.longfn:
        pt="float" if r.floatfn else "long"
        return fs, [chr(ord('a')+j) for j in range(r.lmax+1)], stmts, pt, loc
    return fs, [chr(ord('a')+j) for j in range(r.maxidx+1)], stmts, "int", loc


def _prologue(ins):
    """Shared prologue detection: returns (fs, k, msc3) for the start of the real body."""
    if not (ins[0].mnemonic=="push" and ins[0].op_str=="bp"):
        k=0                                               # frameless: the body starts at once,
        while k < len(ins) and ins[k].mnemonic=="push" and ins[k].op_str in ("si","di")               and any(x.mnemonic=="pop" and x.op_str==ins[k].op_str for x in ins[k+1:]):
            k+=1                                          # after any callee-saved register pushes
        return 0, k, False
    fs=0; k=1
    if ins[k].mnemonic=="sub" and ins[k].op_str.startswith("sp"):
        fs=int(ins[k].op_str.split(",")[1],16); k+=1
        while k<len(ins) and "bp, sp" not in ins[k].op_str: k+=1
    k+=1
    msc3=False
    if fs==0 and k+1 < len(ins) and ins[k+1].mnemonic in ("call","lcall"):
        if ins[k].mnemonic=="mov" and ins[k].op_str.startswith("ax,"):
            try: fs=int(ins[k].op_str.split(",")[1].strip(),16); msc3=True; k+=2
            except ValueError: pass
        elif ins[k].mnemonic=="xor" and ins[k].op_str.replace(" ","")=="ax,ax":
            fs=0; msc3=True; k+=2
    if fs==0 and not msc3 and k < len(ins) and ins[1].op_str=="bp, sp"        and ins[k].mnemonic=="sub" and ins[k].op_str.startswith("sp,"):
        try: fs=int(ins[k].op_str.split(",")[1].strip(),16); msc3=True; k+=1   # MS-C 5: sub sp AFTER mov bp,sp
        except ValueError: pass
    # callee-saved register variables: `push di` / `push si` right after the frame setup, restored
    # by a matching pop in the epilogue -- register saves, NOT call arguments.
    if ins[1].op_str=="bp, sp":
        while k < len(ins) and ins[k].mnemonic=="push" and ins[k].op_str in ("si","di")               and any(x.mnemonic=="pop" and x.op_str==ins[k].op_str for x in ins[k+1:]):
            k+=1
    return fs, k, msc3


def _tidy(lines):
    """Purely syntactic, equivalence-preserving clean-up of the emitted statements:
         } else {                              } else if (c) {
             if (c) { X } [else ...]    ==>        X
         }                                     } [else ...]
       and
         if (a) {                              if (a && b) {
             if (b) { X }               ==>        X
         }                                     }
    (the second only when neither `if` has an else). Each fires only when the inner `if` is the
    sole statement of the enclosing block, so no statement changes its guard."""
    def ind(s): return len(s) - len(s.lstrip(" "))
    changed = True
    while changed:
        changed = False
        for i in range(len(lines) - 2):
            a = lines[i]; n = ind(a); pad = " " * n; inner = pad + "    "
            b = lines[i + 1]
            if not (b.startswith(inner + "if (") and b.endswith(") {") and ind(b) == n + 4): continue
            is_else = a == pad + "} else {"
            is_if = a.startswith(pad + "if (") and a.endswith(") {") and not a.startswith(pad + "if (!(") is None
            if not (is_else or (a.startswith(pad + "if (") and a.endswith(") {"))): continue
            # end of the inner if-chain: first plain close at the inner indent
            j = i + 2; has_else = False
            while j < len(lines):
                if lines[j] == inner + "}": break
                if ind(lines[j]) == n + 4 and lines[j].startswith(inner + "} else"): has_else = True
                if ind(lines[j]) <= n and lines[j].strip(): j = None; break
                j += 1
            if j is None or j >= len(lines) - 1 or lines[j + 1] != pad + "}": continue
            if is_else:
                lines[i:j + 2] = [pad + "} else " + b.strip()] + [x[4:] for x in lines[i + 2:j]] + [pad + "}"]
                changed = True; break
            if not has_else:                                    # `if (a) { if (b) { X } }`
                ca = a[n + 3:-2]; cb = b[n + 7:-2]
                lines[i:j + 2] = [pad + "if (%s && %s) {" % (ca, cb)] + [x[4:] for x in lines[i + 2:j]] + [pad + "}"]
                changed = True; break
    return lines

def _adjacent(lo, hi):
    """Are `lo` and `hi` the low and high word of one 32-bit object (hi sits 2 bytes above lo)?"""
    m, n = re.fullmatch(r"g_([0-9a-f]+)", lo), re.fullmatch(r"g_([0-9a-f]+)", hi)
    if m and n: return int(n.group(1), 16) == int(m.group(1), 16) + 2
    m, n = re.fullmatch(r"v(\d+)", lo), re.fullmatch(r"v(\d+)", hi)
    if m and n: return int(n.group(1)) == int(m.group(1)) - 2          # locals are named by bp-N
    m, n = re.fullmatch(r"(.+)->f(\d+)", lo), re.fullmatch(r"(.+)->f(\d+)", hi)
    if m and n: return m.group(1) == n.group(1) and int(n.group(2)) == int(m.group(2)) + 2
    return False

def _fold_long_expr(e):
    """Rewrite LONG(hi, lo) terms as C long values where that is exact: an adjacent word pair is
    the long object itself, a zero high word is a widened int. Other terms are left as they are."""
    out = []; i = 0
    while i < len(e):
        if e.startswith("LONG(", i) and (i == 0 or not (e[i-1].isalnum() or e[i-1] == "_")):
            d = 0; j = i + 5; cut = None
            while j < len(e):
                if e[j] == "(": d += 1
                elif e[j] == ")":
                    if d == 0: break
                    d -= 1
                elif e[j] == "," and d == 0 and cut is None: cut = j
                j += 1
            if cut is not None and j < len(e):
                hi = e[i+5:cut].strip(); lo = e[cut+1:j].strip()
                if _adjacent(lo, hi): out.append("*(long *)&%s" % lo); i = j + 1; continue
                if hi == "0":
                    out.append("(long)%s" % (lo if re.fullmatch(r"[\w>.\-]+", lo) else "(%s)" % lo)); i = j + 1; continue
        out.append(e[i]); i += 1
    return "".join(out)

def _fold_longs(lines):
    """Presentation only (not used in checker mode): a 32-bit result stored as two adjacent words
         lo = LO(E);  hi = HI(E);          (or through a temporary  t = E; lo = LO(t); hi = HI(t);)
    is one long assignment `*(long *)&lo = E;`, and `+=` / `-=` when E starts from that object."""
    out = []; i = 0
    while i < len(lines):
        a = lines[i]; b = lines[i+1] if i + 1 < len(lines) else ""; c = lines[i+2] if i + 2 < len(lines) else ""
        m1 = re.fullmatch(r"(\s*)(.+?) = LO\((.*)\);", a); m2 = re.fullmatch(r"(\s*)(.+?) = HI\((.*)\);", b)
        E = None; n = 0
        if m1 and m2 and m1.group(1) == m2.group(1) and m1.group(3) == m2.group(3) and _adjacent(m1.group(2), m2.group(2)):
            E = m1.group(3); n = 2; ind = m1.group(1); lo = m1.group(2)
        else:
            m0 = re.fullmatch(r"(\s*)(t\d+) = (.*LONG\(.*);", a)
            m1 = re.fullmatch(r"(\s*)(.+?) = LO\((t\d+)\);", b); m2 = re.fullmatch(r"(\s*)(.+?) = HI\((t\d+)\);", c)
            if m0 and m1 and m2 and m1.group(3) == m2.group(3) == m0.group(2) and _adjacent(m1.group(2), m2.group(2)) \
               and not any(re.search(r"\b%s\b" % m0.group(2), x) for x in lines[i+3:]):
                E = m0.group(3); n = 3; ind = m0.group(1); lo = m1.group(2)
        if E is None: out.append(a); i += 1; continue
        e2 = _fold_long_expr(E); obj = "*(long *)&%s" % lo
        if e2.startswith(obj + " + "): out.append("%s%s += %s;" % (ind, obj, e2[len(obj)+3:]))
        elif e2.startswith(obj + " - "): out.append("%s%s -= %s;" % (ind, obj, e2[len(obj)+3:]))
        else: out.append("%s%s = %s;" % (ind, obj, e2))
        i += n
    return out

_DR_ATOMS = {"ax": ("al", "ah"), "bx": ("bl", "bh"), "cx": ("cl", "ch"), "dx": ("dl", "dh"), "si": ("si",), "di": ("di",),
             "al": ("al",), "ah": ("ah",), "bl": ("bl",), "bh": ("bh",), "cl": ("cl",), "ch": ("ch",), "dl": ("dl",), "dh": ("dh",)}
_DR_TOK = re.compile(r"(?<![\w>.])(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh)(?![\w(])")
_DR_PURE = {"BYTE", "HIBYTE", "SX", "S8", "S16", "LONG", "LO", "HI", "LSHR", "LSAR", "U32", "S32", "unsigned", "int", "char", "long"}

def _dead_regs(lines):
    """Remove assignments to register variables that no path reads (`ax = x;` left behind by
    conservative pinning at merges and gotos). Liveness over the structured statements, gotos
    and switch arms included; only side-effect-free `reg = expr;` lines are removed, and a
    register is assumed read after the function's end. Then drop the empty `else {}` / `if {}`
    shells this leaves."""
    def uses(t):
        out = set()
        for m in _DR_TOK.finditer(t): out.update(_DR_ATOMS[m.group(1)])
        return out
    def pure(e):
        return "++" not in e and "--" not in e and all(m.group(1) in _DR_PURE for m in re.finditer(r"([A-Za-z_]\w*)\s*\(", e))
    for _round in range(16):
        ops = []; labels = {}; fix = []
        def op(use=(), dfn=(), line=None, succ=None, rem=False):
            ops.append(dict(use=set(use), dfn=set(dfn), line=line, succ=succ, rem=rem, src=pos[0] - 1)); return ops[-1]
        pos = [0]; bad = [False]
        def block(brk, cont):
            while pos[0] < len(lines):
                i = pos[0]; ln = lines[i].strip(); pos[0] += 1
                if not ln or ln.startswith("/*"): continue
                if ln == "}": return "end", None
                if ln == "} else {": return "else", None
                m = re.match(r"\} else if \((.*)\) \{$", ln)
                if m: return "elif", m.group(1)
                m = re.match(r"\} while \((.*)\);$", ln)
                if m: return "dowhile", m.group(1)
                m = re.match(r"if \((.*)\) (break|continue);$", ln)
                if m:
                    o = op(uses(m.group(1))); tgt = brk if m.group(2) == "break" else cont
                    if tgt is None: bad[0] = True; return "end", None
                    o["succ"] = ["next", tgt]; continue
                m = re.match(r"if \((.*)\) \{$", ln)
                if m:
                    def chain(c):
                        jz = op(uses(c)); jz["succ"] = ["next", None]
                        end, arg = block(brk, cont)
                        if end in ("else", "elif"):
                            jm = op(); jm["succ"] = [None]; jz["succ"][1] = len(ops)
                            if end == "else": block(brk, cont)
                            else: chain(arg)
                            jm["succ"][0] = len(ops)
                        else: jz["succ"][1] = len(ops)
                    chain(m.group(1)); continue
                m = re.match(r"while \((.*)\) \{$", ln)
                if m or ln == "for (;;) {":
                    top = len(ops); b = ["tgt", None]
                    jz = op(uses(m.group(1))) if m else None
                    block(b, ["tgt", top]); op(succ=[top])
                    b[1] = len(ops)
                    if jz: jz["succ"] = ["next", len(ops)]
                    continue
                if ln == "do {":
                    top = len(ops); b = ["tgt", None]; c = ["tgt", None]
                    end, arg = block(b, c)
                    if end != "dowhile": bad[0] = True; return "end", None
                    c[1] = len(ops); op(uses(arg), succ=["next", top]); b[1] = len(ops); continue
                m = re.match(r"switch \((.*)\) \{$", ln)
                if m:
                    sw = op(uses(m.group(1))); sw["succ"] = []; b = ["tgt", None]
                    fix.append((sw, b)); sw["_cases"] = True
                    stack_sw.append(sw); block(b, cont); stack_sw.pop()
                    b[1] = len(ops); sw["succ"].append(len(ops)); continue
                if re.match(r"(case -?\w+|default):$", ln):
                    if not stack_sw: bad[0] = True; return "end", None
                    stack_sw[-1]["succ"].append(len(ops)); continue
                if ln in ("break;", "continue;"):
                    tgt = brk if ln == "break;" else cont
                    if tgt is None: bad[0] = True; return "end", None
                    op(succ=[tgt]); continue
                m = re.match(r"return\b(.*);$", ln)
                if m: op(uses(m.group(1)) | ({"dl", "dh"} if m.group(1).strip() else set()), succ=[]); continue   # (dx: a long result)
                m = re.match(r"goto (\w+);$", ln)
                if m: op(succ=[("label", m.group(1))]); continue
                m = re.match(r"(\w+):$", ln)
                if m: labels[m.group(1)] = len(ops); continue
                if not ln.endswith(";"): bad[0] = True; return "end", None
                m = re.match(r"(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh) = (.*);$", ln)
                if m and " = " not in m.group(2):
                    op(uses(m.group(2)), _DR_ATOMS[m.group(1)], line=i, rem=pure(m.group(2)))
                else:
                    op(uses(ln))
            return "eof", None
        stack_sw = []
        end, _ = block(None, None)
        if bad[0] or end != "eof": return lines
        n = len(ops)
        def res(x, k):
            if x == "next": return k + 1
            if isinstance(x, list): return x[1]
            if isinstance(x, tuple): return labels.get(x[1], -1)
            return x
        succ = []
        for k, o in enumerate(ops):
            s_ = [k + 1] if o["succ"] is None else [res(x, k) for x in o["succ"]]
            if any(x is None or x == -1 for x in s_): return lines
            succ.append(s_)
        allr = {"al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "si", "di"}
        livein = [set() for _ in range(n + 1)]                 # nothing is read after the end: a function that
                                                             # returns a value says so with `return ax;`
        changed = True
        while changed:
            changed = False
            for k in range(n - 1, -1, -1):
                o = ops[k]; out = set()
                for x in succ[k]: out |= livein[min(x, n)]
                o["out"] = out
                new = (out - o["dfn"]) | o["use"]
                if new != livein[k]: livein[k] = new; changed = True
        kill = {o["line"] for o in ops if o["rem"] and o["line"] is not None and not (o["dfn"] & o["out"])}
        # A register (or temporary) set on one line and read exactly once on the next, dead after
        # it: put the value where it is read (`ax = f(); v4 = ax;` -> `v4 = f();`). Not into a
        # loop test (re-evaluated), not past another call or a short-circuit that could reorder
        # or skip the evaluation.
        calls = lambda t: sum(1 for m in re.finditer(r"([A-Za-z_]\w*)\s*\(", t) if m.group(1) not in _DR_PURE
                              and m.group(1) not in ("if", "switch", "return", "while"))
        sub = {}; busy = set(kill)
        for k in range(n - 1):
            o, q = ops[k], ops[k + 1]
            i = o["src"]
            if succ[k] != [k + 1] or q["src"] != i + 1 or i in busy or i + 1 in busy: continue
            m = re.match(r"(\s*)(ax|bx|cx|dx|si|di|t\d+) = (.*);$", lines[i])
            if not m or " = " in m.group(3): continue
            reg, e = m.group(2), m.group(3); nxt = lines[i + 1]; t = nxt.strip()
            if not (t.endswith(";") or re.match(r"(if|switch) \(.*\) \{$", t)) or re.match(r"if \(.*\) (break|continue);$", t): continue
            if t.startswith(("goto ", "break", "continue")): continue
            pat = re.compile(r"(?<![\w>.&])%s(?![\w(])" % reg)
            if len(pat.findall(nxt)) != 1: continue
            if re.search(r"(BYTE|HIBYTE|LONG|LO|HI)\(%s\)" % reg, nxt) or re.match(r"\s*%s\b" % reg, nxt): continue
            if reg[0] == "t":
                if "LONG(" in e or "(long" in e or sum(len(re.findall(r"\b%s\b" % reg, x)) for x in lines) != 2: continue
            else:
                at = set(_DR_ATOMS[reg])
                if (at & q["out"]) - q["dfn"]: continue                      # still read later
                if any(h in _DR_ATOMS and h != reg and set(_DR_ATOMS[h]) & at for h in _DR_TOK.findall(nxt)): continue   # al/ah of it
            ce, cn = calls(e), calls(nxt)
            if (ce and cn) or cn > 1 or "++" in e or "--" in e: continue
            if ("&&" in nxt or "||" in nxt) and (ce or cn): continue
            if not pure(e) and re.match(r"\s*[^=]*[\[*>][^=]* = ", nxt) and False: continue
            atom = re.match(r"(\w+|\d+|[A-Za-z_]\w*\([^()]*\))$", e) or (e.startswith("(") and e.endswith(")"))
            sub[i] = pat.sub(lambda _m: e if atom else "(%s)" % e, nxt); busy.update((i, i + 1))
        if not kill and not sub: break
        for i, v in sub.items(): lines[i + 1] = v
        kill |= set(sub)
        lines = [l for i, l in enumerate(lines) if i not in kill]
        ch = True                                            # empty shells left behind
        while ch:
            ch = False
            for i in range(len(lines) - 1):
                a, b = lines[i].strip(), lines[i+1].strip()
                if a == "} else {" and b == "}":
                    del lines[i]; ch = True; break
                m = re.match(r"if \((.*)\) \{$", a)
                if m and b == "}" and pure(m.group(1)):
                    del lines[i:i+2]; ch = True; break
                m = re.match(r"(\s*)if \(\((.*) [!=]= 0\)\) \{$", lines[i])
                if m and b == "}":                           # only the test's call matters: keep it
                    lines[i:i+2] = ["%s%s;" % (m.group(1), m.group(2))]; ch = True; break
                m = re.match(r"(\s*)((?:\} else )?if) \((.*)\) \{$", lines[i])
                m2 = re.match(r"\} else if \((.*)\) \{$", b)
                if m and m2:                                 # empty then-arm before an else-if: nest
                    d = 0; j = i + 2                         # the rest of the chain under the opposite test
                    while j < len(lines):
                        t = lines[j].strip()
                        if t.startswith("}") and d == 0 and t == "}": break
                        if t.startswith("}"): d -= 1
                        if t.endswith("{"): d += 1
                        j += 1
                    if j < len(lines):
                        ind = m.group(1)
                        lines[i+2:j+1] = ["    " + x for x in lines[i+2:j+1]]
                        lines.insert(j + 1, ind + "}")
                        lines[i:i+2] = ["%s%s (!%s) {" % (ind, m.group(2), m.group(3) if m.group(3).startswith("(") else "(%s)" % m.group(3)),
                                        "%s    if (%s) {" % (ind, m2.group(1))]
                        ch = True; break
                if m and b == "} else {":                    # empty then-arm: test the opposite
                    lines[i:i+2] = ["%s%s (!%s) {" % (m.group(1), m.group(2), m.group(3) if m.group(3).startswith("(") else "(%s)" % m.group(3))]
                    ch = True; break
    return lines

def _fold_ladders(lines):
    """The compiler tests a 32-bit value as a ladder - high words first, low words only when
    those are equal - which structuring leaves as three nested ifs with the body duplicated:
        if (Ah <= Bh) { if (Ah >= Bh) { if (Al < Bl) { X } } else { X } }
    Fold it back into one comparison of LONG(hi, lo) values when the three arms really are the
    truth table of a single relation (every body compared textually)."""
    def ind(s): return len(s) - len(s.lstrip(" "))
    def parse_if(i):
        """lines[i] is `if (c) {` -> (cond, then_lines, else_lines|None, index after the closing brace)"""
        m = re.match(r"( *)if \((.*)\) \{$", lines[i])
        if not m: return None
        d = len(m.group(1)); j = i + 1
        while j < len(lines) and not (ind(lines[j]) == d and lines[j].strip().startswith("}")): j += 1
        if j >= len(lines): return None
        t = lines[j].strip()
        if t == "}": return m.group(2), lines[i+1:j], None, j + 1
        if t != "} else {": return None
        k = j + 1
        while k < len(lines) and not (ind(lines[k]) == d and lines[k].strip().startswith("}")): k += 1
        if k >= len(lines) or lines[k].strip() != "}": return None
        return m.group(2), lines[i+1:j], lines[j+1:k], k + 1
    def rel(c):
        """`(A op B)` -> (A, op, B, signedness 'u'|'s') with the top-level operator"""
        if not (c.startswith("(") and c.endswith(")")): return None
        c = c[1:-1]; d = 0
        for i, ch in enumerate(c):
            if ch == "(": d += 1
            elif ch == ")": d -= 1
            elif d == 0 and ch == " ":
                for op in ("<=", ">=", "<", ">"):
                    if c.startswith(op + " ", i + 1):
                        a, b = c[:i], c[i + len(op) + 2:]
                        if "&&" in a or "||" in a or "&&" in b or "||" in b or "S8(" in a + b: return None
                        ms, mb = re.fullmatch(r"S16\((.*)\)", a), re.fullmatch(r"S16\((.*)\)", b)
                        if ms and mb: return ms.group(1), op, mb.group(1), "s"
                        if a.startswith("(unsigned)"): return a[10:], op, b, "u"
                        return a, op, b, ("u" if MARK else "s")
        return None
    def same(x, y): return [l.strip() for l in (x or [])] == [l.strip() for l in (y or [])]
    i = 0; out = list(lines); changed = False
    while i < len(lines):
        O = parse_if(i)
        r1 = rel(O[0]) if O else None
        if not r1 or r1[1] not in ("<=", ">=") or not O[1] or O is None: i += 1; continue
        I = parse_if(i + 1)
        if not I or I[3] != i + 1 + len(O[1]): i += 1; continue
        r2 = rel(I[0])
        if not r2 or (r2[0], r2[2], r2[3]) != (r1[0], r1[2], r1[3]) or {r1[1], r2[1]} != {"<=", ">="}: i += 1; continue
        Lw = parse_if(i + 2)
        if not Lw or Lw[3] != i + 2 + len(I[1]): i += 1; continue
        r3 = rel(Lw[0])
        if not r3 or r3[3] != "u" or not Lw[1]: i += 1; continue
        X, Y1 = Lw[1], Lw[2]
        lt_body, gt_body = (I[2], O[2]) if r1[1] == "<=" else (O[2], I[2])
        c_lt, c_gt = r3[1] in ("<", "<="), r3[1] in (">", ">=")
        if not same(lt_body, X if c_lt else Y1) or not same(gt_body, X if c_gt else Y1): i += 1; continue
        d = " " * ind(lines[i])
        w = ("U32", "S32")[r1[3] == "s"]
        if MARK: c = "(%s(LONG(%s, %s)) %s %s(LONG(%s, %s)))" % (w, r1[0], r3[0], r3[1], w, r1[2], r3[2])
        else:
            c = _fold_long_expr("(%sLONG(%s, %s) %s LONG(%s, %s))" % ("(unsigned long)" if r1[3] == "u" else "", r1[0], r3[0], r3[1], r1[2], r3[2]))
        new = [d + "if (%s) {" % c] + [l[8:] if l.startswith(" " * 8) else l for l in X]
        if Y1: new += [d + "} else {"] + [l[8:] if l.startswith(" " * 8) else l for l in Y1]
        new.append(d + "}")
        lines[i:O[3]] = new; changed = True
    return lines

def _neg_cond(c):
    """Negation of a condition text: flip a single relation, otherwise `!(c)`."""
    if c.startswith("(") and c.endswith(")"):
        x = c[1:-1]; d = 0; ok = True; at = None
        for i, ch in enumerate(x):
            if ch == "(": d += 1
            elif ch == ")":
                d -= 1
                if d < 0: ok = False; break
            elif d == 0 and ch == " ":
                for op in ("==", "!=", "<=", ">=", "<", ">", "&&", "||"):
                    if x.startswith(op + " ", i + 1):
                        if op in ("&&", "||") or at is not None: ok = False
                        else: at = (i, op)
        if ok and d == 0 and at:
            i, op = at
            flip = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}[op]
            return "(%s %s %s)" % (x[:i], flip, x[i + len(op) + 2:])
        if ok and d == 0 and x.startswith("!") and x[1:].startswith("(") and x.endswith(")"):
            dd = 0
            for j, ch in enumerate(x[1:]):
                dd += ch == "("; dd -= ch == ")"
                if dd == 0: break
            if j == len(x) - 2: return x[1:]
    return "(!(%s))" % c

def _fold_goto_or(lines):
    """Short-circuit conditions that structuring left as a jump into a guarded block. With L the
    first statement of an arm of the inner `if` (which is the whole then-arm of the outer one):

        if (A) { if (B) { L: X } [else { Y }] } else { goto L; }   ==>  if (!A || B) { X } [else { Y }]
        if (A) { if (B) { X } else { L: Y } }   else { goto L; }   ==>  if (A && B) { X } else { Y }
        if (P) { L: X } else if (Q) { goto L; } ...                ==>  if (P || Q) { X } ...

    The tests still run in the same order and under the same conditions. A label goes when this
    was its last goto."""
    def ind(s): return len(s) - len(s.lstrip(" "))
    def close(i, d):
        j = i + 1
        while j < len(lines) and not (ind(lines[j]) == d and lines[j].strip().startswith("}")): j += 1
        return j if j < len(lines) else None
    def top_or(c):
        d = 0
        for i, ch in enumerate(c):
            d += ch == "("; d -= ch == ")"
            if d == 0 and c.startswith(" || ", i): return True
        return False
    def par(c): return "(%s)" % c if top_or(c) else c
    def others(L, skip): return sum(1 for q, x in enumerate(lines) if q != skip and re.search(r"\bgoto %s;" % L, x))
    def dedent(xs): return [x[4:] if x.startswith("    ") else x for x in xs]
    changed = True
    while changed:
        changed = False
        for i in range(len(lines) - 3):
            m = re.match(r"( *)((?:\} else )?if) \((.*)\) \{$", lines[i])
            if not m: continue
            d = len(m.group(1)); pad = m.group(1); j = close(i, d)
            if j is None: continue
            # --- if (P) { L: X } else if (Q) { goto L; } ...
            mq = re.match(r"\} else if \((.*)\) \{$", lines[j].strip())
            lab = re.match(r"(\w+):$", lines[i + 1].strip())
            if mq and lab and j + 2 < len(lines) and lines[j + 1].strip() == "goto %s;" % lab.group(1) \
               and ind(lines[j + 2]) == d and lines[j + 2].strip().startswith("}"):
                L = lab.group(1); keep = others(L, j + 1)
                lines[i] = "%s%s (%s || %s) {" % (pad, m.group(2), m.group(3), mq.group(1))
                del lines[j:j + 2]
                if not keep: del lines[i + 1]
                changed = True; break
            if lines[j].strip() != "} else {" or j + 2 >= len(lines): continue
            g = re.match(r"goto (\w+);$", lines[j + 1].strip())
            if not g or lines[j + 2].strip() != "}" or ind(lines[j + 2]) != d: continue
            mi = re.match(r"( *)if \((.*)\) \{$", lines[i + 1])
            if not mi or len(mi.group(1)) != d + 4: continue
            k = close(i + 1, d + 4)                           # end of the inner then-arm
            if k is None: continue
            t = lines[k].strip()
            if t == "}": e_end = k; ybody = None
            elif t == "} else {":
                e_end = close(k, d + 4)
                if e_end is None or lines[e_end].strip() != "}": continue
                ybody = lines[k + 1:e_end]
            else: continue
            if e_end + 1 != j: continue                       # the inner if is the whole then-arm
            L = g.group(1); keep = others(L, j + 1); xbody = lines[i + 2:k]
            if xbody and xbody[0].strip() == L + ":":
                if not keep: xbody = xbody[1:]
                cond = "%s || %s" % (_neg_cond(m.group(3)), mi.group(2))
            elif ybody and ybody[0].strip() == L + ":":
                if not keep: ybody = ybody[1:]
                cond = "%s && %s" % (par(m.group(3)), par(mi.group(2)))
            else: continue
            new = ["%s%s (%s) {" % (pad, m.group(2), cond)] + dedent(xbody)
            if ybody is not None: new += [pad + "} else {"] + dedent(ybody)
            new.append(pad + "}")
            lines[i:j + 3] = new; changed = True; break
    return lines

def recover_cfg(code, base=0, cxmap=None):
    """EXPERIMENTAL Phase-B path: structure the CFG (cfg.structure) and emit via R.emit_tree,
    reusing the SAME R value model as recover(). Not the default; used behind a flag / for A/B
    comparison against recover() until it is proven >= the linear walk on every function."""
    from src.engines.inverse import cfg as _cfg
    _cfg.SWITCH=SWITCH
    ins=_decode(code)
    if len(ins) < 2:
        return 0, [], [], "int", []
    fs, k, msc3 = _prologue(ins)
    r=R(ins,fs,base,cxmap,msc3=msc3); r.emit_start=k
    end_addr = ins[-1].address + ins[-1].size
    blocks,_,entry = _cfg.build_cfg(ins, 0, end_addr)
    tree = _cfg.structure(blocks, entry)
    r.labels=set(); r._labeled=set()
    stmts = r.emit_tree(blocks, tree)
    if r.gototargets:                                      # pass 2: place each target's label once
        r2=R(ins,fs,base,cxmap,msc3=msc3); r2.emit_start=k
        r2.labels=set(r.gototargets); r2._labeled=set(); r2.gotoargs=dict(getattr(r,"gotoargs",{}))
        stmts = r2.emit_tree(blocks, tree); r=r2
    stmts = _tidy(list(stmts))
    if DEADREGS:
        stmts = _dead_regs(list(stmts))
        if MARK or UNSIGNED_CAST: stmts = _fold_ladders(list(stmts))   # (needs to know which compares are unsigned)
        stmts = _tidy(_fold_goto_or(list(stmts)))            # `||` left as a jump into the guarded block
    if not MARK:
        stmts = _fold_longs(stmts)
        used = [x for x in ("ax","bx","cx","dx","si","di")
                if any(re.search(r"(?<![\w>.])%s(?![\w(])" % x, ln) for ln in stmts)]
        if used and any(ln.strip() for ln in stmts):               # which names are machine registers
            stmts = ["/* register variables: %s */" % ", ".join(used)] + stmts
    loc=sorted(r.locals)
    if r.floatfn or r.longfn:
        pt="float" if r.floatfn else "long"
        return fs, [chr(ord('a')+j) for j in range(r.lmax+1)], stmts, pt, loc
    return fs, [chr(ord('a')+j) for j in range(r.maxidx+1)], stmts, "int", loc

def _fold_shortcircuit(lines):
    """Recover an `&&` short-circuit the goto-fallback left as a jump into the else block:
        if (A) {                         if ((A) && (B)) {
            if (!(B)) goto L_X;   ==>         <then-rest>
            <then-rest>                   } else {
        } else {                              <else-rest>
            L_X:                          }
            <else-rest>
        }
    Equivalent: originally A&&!B jumps to L_X (= else entry), A&&B falls into then, !A falls into
    else. Only fires when the guard is the then's FIRST line, `L_X:` is the else's FIRST line, and
    L_X is referenced exactly once (so removing it orphans nothing). Idempotent; loops to chain
    A&&B&&C. Pure structural rewrite -- no value state crosses, so it cannot drop logic."""
    guard=re.compile(r"^(\s*)if \(!\((.*)\)\) goto (L_[0-9a-f]+);$")
    changed=True
    while changed:
        changed=False
        for i,ln in enumerate(lines):
            m=re.match(r"^(\s*)if \((.*)\) \{$", ln)
            if not m: continue
            ind, A = m.group(1), m.group(2)
            g=guard.match(lines[i+1]) if i+1<len(lines) else None
            if not g or g.group(1)!=ind+"    ": continue
            B, L = g.group(2), g.group(3)
            # find this if's own `} else {` by EXACT indent: a nested else inside the then-block is
            # indented deeper, so the first line equal to `<ind>} else {` is ours. A plain `<ind>}`
            # first means the if had no else -> not our pattern.
            je=None
            for j in range(i+2, len(lines)):
                s=lines[j].rstrip()
                if s==ind+"} else {": je=j; break
                if s==ind+"}": break
            if je is None or je+1>=len(lines): continue
            if lines[je+1].strip()!=L+":": continue            # label must be the else's first line
            if sum(1 for x in lines if x.strip()==L+":")!=1: continue
            if sum(1 for x in lines if re.search(r"goto %s;"%L, x))!=1: continue
            lines=lines[:i]+["%sif ((%s) && (%s)) {"%(ind,A,B)]+lines[i+2:je+1]+lines[je+2:]
            changed=True; break
    return lines

def detect_copies(stmts):
    """Find live local-to-local copies `vX = vY;` in straight-line stmts, as
    (dest_off, src_off) pairs, where src is initialized before and NOT overwritten after
    (the condition under which MC2 swaps src/dest frame offsets)."""
    if any("{" in s or "}" in s for s in stmts): return []      # straight-line only
    parsed=[]                                                    # (lval_off | None, src_off | None)
    for s in stmts:
        m=re.match(r"v(\d+) = v(\d+);$", s.strip())
        if m: parsed.append((int(m.group(1)), int(m.group(2)))); continue
        m2=re.match(r"v(\d+) =", s.strip())
        parsed.append((int(m2.group(1)) if m2 else None, None))
    copies=[]
    for i,(dest,src) in enumerate(parsed):
        if src is None: continue
        init_before = any(parsed[j][0]==src for j in range(i))
        over_after  = any(parsed[j][0]==src for j in range(i+1,len(parsed)))
        if init_before and not over_after: copies.append((dest,src))
    return copies

def local_decls(fs, locals, longset=None, copies=None):
    """framemodel-driven local declarations (gap-filled, layout-exact) as a string block."""
    ordered,_ = framemodel.emit_local_decls(locals or [], fs, longset, copies)
    return "".join("    %s\n"%framemodel.fmt_decl(*d) for d in ordered)

def to_c(name,fs,params,stmts,ptype="int",locals=None,longset=None):
    sig="%s(%s)"%(name,", ".join(params)) if params else "%s()"%name   # int return (K&R default)
    decl=("%s %s;\n"%(ptype,", ".join(params))) if params else ""
    ldecl=local_decls(fs, locals, longset, detect_copies(stmts))
    return "%s\n%s{\n%s%s\n}\n"%(sig,decl,ldecl,"\n".join("    "+s for s in stmts))

def main():
    img=open(sys.argv[1],"rb").read(); off=int(sys.argv[2],16); ln=int(sys.argv[3])
    fs,params,stmts,pt,loc=recover(img[off:off+ln]); print(to_c("f%05x"%off,fs,params,stmts,pt,loc))

if __name__=="__main__": main()
