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
CJMP=set(JCC)|{"jz","jnz"}
BYTEREG=("al","bl","cl","dl","ah","bh","ch","dh")        # 8-bit sub-registers (exposed as vars)
BYTEPARENT={"al":"ax","ah":"ax","bl":"bx","bh":"bx","cl":"cx","ch":"cx","dl":"dx","dh":"dx"}
WORDHALVES={"ax":("al","ah"),"bx":("bl","bh"),"cx":("cl","ch"),"dx":("dl","dh")}
TRACKED_BYTE={"mov","add","sub","or","and","xor","inc","dec"}   # byte-reg ops tracked as breg values

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
                m=re.match(r"0x([0-9a-f]+)$", ii.op_str)
                if m: s.tgt[i]=a2i.get(int(m.group(1),16))
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
        idx=((off-4)//2) if s.msc3 else ((off-s.fs-4)//2); s.maxidx=max(s.maxidx,idx)
        return chr(ord('a')+idx) if 0<=idx<26 else "p%d"%idx
    def _ptrbase(s,reg):
        """Readable base for a pointer-register deref: the register's tracked pointer value
        if it has one (not a plain number), else the register name itself (an implicit
        struct pointer). Keeps [bx+N] etc. readable instead of leaking raw asm."""
        v=getattr(s,reg,None)
        if v is None or re.match(r"-?\d+$|-?0x[0-9a-f]+$",str(v)): return reg
        return str(v)
    def memname(s,op):
        op=op.replace("word ptr ","").replace("byte ptr ","")
        op=re.sub(r"\b(es|ds|ss|cs|fs|gs):","",op)            # drop segment override (far deref)
        if s.msc3:                                            # 3.0: locals [bp-N], params [bp+N>=4]
            m=re.search(r"\[bp - (0x[0-9a-f]+|\d+)\]",op)
            if m:
                o=s.num(m.group(1)); s.locals.add(o); return "v%d"%o
            m=re.search(r"\[bp \+ (0x[0-9a-f]+|\d+)\]",op)
            if m:
                return s.pname(s.num(m.group(1)))
            # fall through to si/global handling below
        else:
            m=re.search(r"\[bp \+ (0x[0-9a-f]+|\d+)\]",op)
            if m or "[bp]" in op:                             # [bp] == offset 0
                o=s.num(m.group(1)) if m else 0
                if o < s.fs: s.locals.add(o); return "v%d"%o  # local (below saved BP)
                return s.pname(o)                             # parameter
        m=re.search(r"\[bp - (0x[0-9a-f]+|\d+)\]",op)         # negative bp offset is always a
        if m:                                                  # local (even when the chkstk prologue
            o=s.num(m.group(1)); s.locals.add(o); return "v%d"%o  # wasn't detected to set msc3)
        for reg in ("si","bx","di"):                          # pointer-register derefs
            m=re.search(r"\[%s \+ (0x[0-9a-f]+|\d+)\]"%reg,op)
            if m: return "%s->f%d"%(s._ptrbase(reg),s.num(m.group(1)))
            m=re.search(r"\[%s - (0x[0-9a-f]+|\d+)\]"%reg,op)  # negative offset -> index back
            if m: return "%s[-%d]"%(s._ptrbase(reg),s.num(m.group(1)))
            if "[%s]"%reg in op: return "*%s"%s._ptrbase(reg)
        m=re.search(r"\[bx \+ si\]|\[bx \+ di\]|\[si \+ di\]",op)  # indexed; name by base reg
        if m: return "%s[%s]"%(s._ptrbase(op[1:3]), op.split(" + ")[1].rstrip("]"))
        m=re.search(r"\[(0x[0-9a-f]+|\d+)\]",op)
        if m: return "g_%x"%s.num(m.group(1))       # distinct global per address
        if op.strip() in ("ax","bx","cx","dx","si","di","bp"):
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
            else:                                          # stable name/field/global -> alias
                setattr(s,d,val)
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
        return s.rsrc(tok) if tok in ("bx","cx","dx") else s.rhs(tok)

    def emit(s, out, text, rank=None, rc=False):
        """Append a statement, its source rank (for un-scheduling), and whether it is a
        register-routed constant store (a signal for dead-store recovery)."""
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

    def expr_step(s,i):
        """Update ax/si/cx from a value-producing instruction (no statement)."""
        s.i=i; old=s.ax; ii=s.ins[i]; mn=ii.mnemonic; ops=ii.op_str
        s._binval(mn, ops.split(",")[0].strip())
        if mn=="mov" and ops.split(", ")[0] in BYTEREG:        # byte load/copy: r8 = mem | imm | r8'
            d8=ops.split(", ")[0]; src=ops.split(", ",1)[1]
            if re.match(r"(0x[0-9a-f]+|\d+)$",src): s.breg[d8]=str(s.num(src))
            elif src in BYTEREG: s.breg[d8]=s.breg.get(src,src)
            else: s.breg[d8]=s.memname(src) or d8
            if BYTEPARENT[d8] in ("ax","bx","cx","dx","si","di"): setattr(s,BYTEPARENT[d8],None)
            return
        if mn in ("add","sub","or","and","xor") and ops.split(", ")[0] in BYTEREG:   # byte-reg ALU
            d8=ops.split(", ")[0]; src=ops.split(", ",1)[1]
            if mn in ("sub","xor") and src==d8: s.breg[d8]="0"; return   # sub/xor r8,r8 -> clear to 0
            cur=s.breg.get(d8,d8); opc={"add":"+","sub":"-","or":"|","and":"&","xor":"^"}[mn]
            if re.match(r"(0x[0-9a-f]+|\d+)$",src): sv=str(s.num(src))
            elif src in BYTEREG: sv=s.breg.get(src,src)
            else: sv=s.memname(src) or src
            s.breg[d8]="(%s %s %s)"%(cur,opc,sv); return
        if mn in ("inc","dec") and ops in BYTEREG:             # byte-reg inc/dec
            s.breg[ops]="(%s %s 1)"%(s.breg.get(ops,ops), "+" if mn=="inc" else "-"); return
        if mn in ("cbw","cwde") and "al" in s.breg:            # sign-extend al -> ax (int)al
            s.ax=s.breg["al"]; s.axsrc=None; return
        if mn=="mov" and ops.startswith("ax, "):
            src=ops[4:]
            if src=="dx" and s.div: s.ax="(%s %% %s)"%s.div; s.axsrc=None
            elif src in ("bx","cx","dx"): s.ax=s.rsrc(src); s.axsrc=None   # reg->reg copy
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
        elif mn=="xor" and ops in ("ax, ax","bx, bx"): s.ax="0"     # self-xor = clear (return 0)
        else:
            axv = s.ax if s.ax is not None else "ax"             # untracked ax -> the register
            cxv = s.cx if s.cx is not None else "cx"
            R=("ax","bx","cx","dx","si","di"); d0=ops.split(",")[0].strip()
            def rv(r): v=getattr(s,r,None); return v if v is not None else r
            def shcnt(): return cxv if ops.endswith("cl") else s._val(ops.split(", ")[1])
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
        cond="(%s %s %s)"%(s._cmp[0], INV.get(jcc,"?"), s._cmp[1]) if s._cmp \
             else (s.ax if s.ax is not None else "ax")   # bare value test -> the register
        return cond, j

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

    def step(s,i,out):
        """Straight-line: process ins[i], maybe append a statement. Return next i."""
        s.i=i; s.out=out
        if i in s.hoisted: return i+1                          # store already emitted (hoisted)
        n=s.long_pat(i)
        if n: s.retlive=True; return i+n
        n=s.long_stmt(i,out)
        if n: return i+n
        ii=s.ins[i]; mn=ii.mnemonic; ops=ii.op_str; d0=ops.split(",")[0].strip()
        s._binval(mn, d0)
        if mn=="mov" and "[" in d0 and ops.rpartition(", ")[2] in BYTEREG:   # store: mem = byte-reg
            src=ops.rpartition(", ")[2]
            s.emit(out,"%s = %s;"%(s.memname(d0.replace("byte ptr ","")), s.breg.get(src, src)))
            return i+1
        if mn=="cmp" and ops.startswith("ax, ") and i+2<len(s.ins) and s.ins[i+1].mnemonic in JCC and s.ins[i+2].mnemonic=="xor":
            s.ax="(%s %s %s)"%(s.ax, JCC[s.ins[i+1].mnemonic], s.memname(ops[4:])); s.retlive=True; s.axsrc=None
            while i<len(s.ins) and s.ins[i].op_str!="ax, bx": i+=1
            return i+1
        if mn=="push":
            if ops=="ax": s.args.append(s.ax if s.ax is not None else "ax")
            else: s.args.append(s.memname(ops) or ops)
            return i+1
        if mn=="call":
            m=re.match(r"(0x[0-9a-f]+|\d+)$", ops)
            args=", ".join(str(a) for a in reversed(s.args)); s.args=[]
            if m:
                expr="sub_%x(%s)"%(s.num(m.group(1)), args)
            else:                                              # indirect call: through a ptr/reg
                fp=s.memname(ops) or ops
                expr="(*%s)(%s)"%(fp, args)
            if s.fs==0 or s.callstmt: s.emit(out,expr+";"); s.ax=None; s.retlive=False  # discarded
            else: s.ax=expr; s.retlive=True                    # result used
            return i+1
        if mn=="lcall":                                        # far call `lcall seg, off`: the target's
            args=", ".join(str(a) for a in reversed(s.args)); s.args=[]   # load-module offset is seg*16+off
            mm=re.match(r"(0x[0-9a-f]+|\d+),\s*(0x[0-9a-f]+|\d+)$", ops)   # (verified: lands on fn starts)
            if mm:
                expr="sub_%x(%s)"%(s.num(mm.group(1))*16 + s.num(mm.group(2)), args)
            else:                                              # indirect far call through a ptr/reg
                fp=s.memname(ops) or ops; expr="(*%s)(%s)"%(fp, args)
            if s.fs==0 or s.callstmt: s.emit(out,expr+";"); s.ax=None; s.retlive=False
            else: s.ax=expr; s.retlive=True
            return i+1
        if mn=="mov" and ops=="sp, bp": return i+1             # post-call arg cleanup
        if mn=="mov" and "[" in ops.split(", ")[0]:            # store: mem = reg | immediate
            dest,_,src=ops.rpartition(", ")
            if src=="ax":
                # a reused CONST loaded earlier: rank the store at its LOAD (un-schedule the defer)
                rank = s.axsrc if (s.axsrc is not None and s.axsrc < i) else i
                rc = (s.axsrc is not None) and bool(re.match(r"-?\d+$|-?0x[0-9a-f]+$", str(s.ax)))
                s.emit(out,"%s = %s;"%(s.memname(dest), s.ax if s.ax is not None else "ax"), rank, rc)
                s.ax=s.memname(dest); s.axsrc=None; s.retlive=False; return i+1   # aliases var
            if src in ("bx","cx","dx","si","di"):
                rank = s.bxsrc if (src=="bx" and s.bxsrc is not None and s.bxsrc < i) else i
                rv = getattr(s,src) if getattr(s,src) is not None else src          # untracked -> reg name
                s.emit(out,"%s = %s;"%(s.memname(dest), rv), rank)
                setattr(s,src,s.memname(dest))
                if src=="bx": s.bxsrc=None
                return i+1                                      # backward alias
            if re.match(r"(0x[0-9a-f]+|\d+)$",src):
                s.emit(out,"%s = %s;"%(s.memname(dest), s.num(src))); return i+1
        if mn in ALU and "[" in ops.split(", ")[0] and ops.endswith(", ax"):   # mem <op>= ax
            d=s.memname(ops.split(",")[0]); s.emit(out,"%s = %s %s %s;"%(d,d,ALU[mn],s.ax)); return i+1
        if mn in ALU and "[" in ops.split(", ")[0]:            # mem <op>= imm | reg
            dst,_,src=ops.rpartition(", ")
            if re.match(r"(0x[0-9a-f]+|\d+)$",src) or src in ("bx","cx","dx","si","di"):
                d=s.memname(dst); rv=s.num(src) if re.match(r"(0x[0-9a-f]+|\d+)$",src) else s._val(src)
                s.emit(out,"%s = %s %s %s;"%(d,d,ALU[mn],rv)); return i+1
        if mn in ("inc","dec") and ops in ("ax","bx"):
            base = getattr(s, ops) if getattr(s, ops) is not None else ops   # untracked -> reg name
            s.ax="(%s %s 1)"%(base,"+" if mn=="inc" else "-"); s.retlive=True; return i+1
        if mn in ("inc","dec"):
            nm=s.memname(ops) or ops; o="+" if mn=="inc" else "-"
            # fs>0 means MC1 allocated a temp -> source used the temp form x = x OP 1 (not x++/x--)
            s.emit(out,"%s = %s %s 1;"%(nm,nm,o) if s.fs>0 else "%s%s;"%(nm,"++" if mn=="inc" else "--"))
            return i+1
        if mn=="int":                                          # software interrupt -> named syscall
            s.emit(out,"int_%s();"%ops.replace("0x","").replace(" ","")); return i+1
        if mn in ("shl","sal","shr","sar") and "[" in ops.split(",")[0]:   # memory-dest shift
            d=s.memname(ops.split(",")[0])
            cnt=(s.cx if s.cx is not None else "cx") if ops.endswith("cl") else s._val(ops.split(", ")[1])
            op="<<" if mn in ("shl","sal") else ">>"; s.emit(out,"%s = %s %s %s;"%(d,d,op,cnt)); return i+1
        if mn in ("neg","not") and "[" in ops:                 # memory-dest unary
            d=s.memname(ops); s.emit(out,"%s = %s%s;"%(d,"-" if mn=="neg" else "~",d)); return i+1
        # byte-register shift/unary: al/bl/.. are low sub-registers the word model doesn't track,
        # so emit them as statements on the exposed byte-register variable (faithful transcription).
        if d0 in BYTEREG and mn in ("shl","sal","shr","sar"):
            cnt="cl" if ops.endswith("cl") else s._val(ops.split(", ")[1])
            op="<<" if mn in ("shl","sal") else ">>"; s.emit(out,"%s = %s %s %s;"%(d0,d0,op,cnt)); return i+1
        if d0 in BYTEREG and mn in ("neg","not"):
            s.emit(out,"%s = %s%s;"%(d0,"-" if mn=="neg" else "~",d0)); return i+1
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
                base=getattr(s,d0,None)
                base=base if (base is not None and re.match(r"^[A-Za-z_]\w*$",str(base))) else d0
                s.emit(out,"%s = %s %s %s;"%(base,base,ALU[mn],rhs))
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
            s.emit(out,"memcpy(%s, %s, %s);"%(s.di or "di", s.si or "si", n)); return i+1
        if mn in ("rep stosb","rep stosw","repne stosb","repne stosw"):
            n=s.cx or "cx"
            if mn.endswith("stosw"): n=str(int(n)*2) if str(n).isdigit() else "(%s * 2)"%n
            s.emit(out,"memset(%s, %s, %s);"%(s.di or "di", s.ax if s.ax is not None else "ax", n)); return i+1
        if mn in ("movsb","movsw"):
            s.emit(out,"*%s++ = *%s++;"%(s.di or "di", s.si or "si")); return i+1
        if mn in ("stosb","stosw"):
            s.emit(out,"*%s++ = %s;"%(s.di or "di", s.ax if s.ax is not None else "ax")); return i+1
        if mn in ("lodsb","lodsw"):
            s.ax="*%s++"%(s.si or "si"); s.retlive=True; return i+1
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
        d.update(div=s.div, retlive=s.retlive, axsrc=s.axsrc, bxsrc=s.bxsrc, args=list(s.args), breg=dict(s.breg))
        return d
    def _rest(s,d):
        for r in s._REGS: setattr(s,r,d[r])
        s.div=d["div"]; s.retlive=d["retlive"]; s.axsrc=d["axsrc"]; s.bxsrc=d["bxsrc"]; s.args=list(d["args"]); s.breg=dict(d["breg"])
    def _phi(s, then_snap, else_snap, then_b, else_b):
        """Merge two branch register-states. For a register the branches leave DIFFERENT, emit
        `reg = <value>;` at the end of each branch (materializing the conditional assignment that
        value-tracking would otherwise defer and lose at the merge) and expose the bare register
        afterwards; where the branches agree, keep the shared value."""
        for r in s._REGS:
            tv, ev = then_snap[r], else_snap[r]
            if tv==ev:
                setattr(s, r, tv); continue
            if tv is not None and str(tv)!=r: then_b.append("%s = %s;"%(r,tv))
            if ev is not None and str(ev)!=r: else_b.append("%s = %s;"%(r,ev))
            setattr(s, r, None)                      # merged -> bare register after the branch
        tbr, ebr = then_snap["breg"], else_snap["breg"]       # 8-bit sub-registers set per branch
        for r in BYTEREG:
            tv, ev = tbr.get(r), ebr.get(r)
            if tv==ev:
                if tv is not None: s.breg[r]=tv
                continue
            if tv is not None and str(tv)!=r: then_b.append("%s = %s;"%(r,tv))
            if ev is not None and str(ev)!=r: else_b.append("%s = %s;"%(r,ev))
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
        j = jcc_idx - 1
        while j >= lo and s.ins[j].mnemonic not in ("cmp", "test"):
            j -= 1
        return j if j >= lo and s.ins[j].mnemonic in ("cmp", "test") else jcc_idx

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
        elif blk.term == "tailcall":
            last = s.ins[blk.hi - 1]
            m = re.match(r"0x([0-9a-f]+)$", last.op_str)
            if m:
                out.append("return sub_%x(%s);" % (int(m.group(1), 16) & 0xFFFF,
                            ", ".join(str(a) for a in reversed(s.args)))); s.args = []

    def _emit_cond(s, blk, out):
        """Emit a cond block's leading statements and return its branch condition (fall-through
        sense, matching cond_at/INV)."""
        jcc = blk.hi - 1
        cs = s._cmp_start(jcc, blk.lo)
        s._emit_range(blk.lo, cs, out)
        s._cmp = None                              # avoid a stale compare leaking into a bare-value test
        cond, _ = s.cond_at(cs)
        return cond

    def _plain_if(s, cond, body, pre, then_s, out):
        """Emit a plain if(cond){body}: materialize the phi against the implicit (not-taken) else --
        a register the body changes is set `reg=<then>;` inside and `reg=<pre>;` before, then exposed
        bare. Skips emitting when the body is empty (a no-op guard)."""
        for r in s._REGS:
            if then_s[r] == pre[r]:
                setattr(s, r, then_s[r]); continue
            if pre[r] is not None and str(pre[r]) != r: out.append("%s = %s;" % (r, pre[r]))
            if then_s[r] is not None and str(then_s[r]) != r: body.append("%s = %s;" % (r, then_s[r]))
            setattr(s, r, None)
        pbr, tbr = pre["breg"], then_s["breg"]                 # 8-bit sub-registers vs the implicit else
        for r in BYTEREG:
            pv, tv = pbr.get(r), tbr.get(r)
            if pv == tv:
                if tv is not None: s.breg[r] = tv
                continue
            if pv is not None and str(pv) != r: out.append("%s = %s;" % (r, pv))
            if tv is not None and str(tv) != r: body.append("%s = %s;" % (r, tv))
            s.breg.pop(r, None)
        if any(x.strip() for x in body):
            out.append("if (%s) {" % cond); out += ["    " + x for x in body]; out.append("}")

    def _emit_seq(s, blocks, nodes, out):
        for node in nodes:
            s._emit_node(blocks, node, out)

    def _emit_node(s, blocks, node, out):
        kind = node[0]
        if kind == "block":
            s._emit_block(blocks[node[1]], out)
        elif kind == "if":
            b, then_t, join, neg = node[1], node[2], node[3], node[4]
            cond = s._emit_cond(blocks[b], out)
            if neg: cond = s._negate(cond)
            pre = s._snap()
            body = []; s._emit_seq(blocks, then_t, body); then_s = s._snap()
            s._rest(pre)
            s._plain_if(cond, body, pre, then_s, out)
        elif kind == "ifelse":
            b, then_t, else_t, join = node[1], node[2], node[3], node[4]
            cond = s._emit_cond(blocks[b], out)
            pre = s._snap()
            tb = []; s._emit_seq(blocks, then_t, tb); ts = s._snap()
            s._rest(pre)
            eb = []; s._emit_seq(blocks, else_t, eb); es = s._snap()
            # A genuinely-empty tree branch -> collapse to a plain if (the CFG proves the region is
            # empty, so this is faithful, not a dropped-statement mask as it would be in walk()).
            if not any(x.strip() for x in tb) and any(x.strip() for x in eb):
                s._plain_if(s._negate(cond), eb, pre, es, out)
            elif not any(x.strip() for x in eb) and any(x.strip() for x in tb):
                s._plain_if(cond, tb, pre, ts, out)
            else:
                s._phi(ts, es, tb, eb)
                out.append("if (%s) {" % cond); out += ["    " + x for x in tb]
                out.append("} else {"); out += ["    " + x for x in eb]; out.append("}")
        elif kind in ("while", "loop"):
            b, body_t = node[1], node[2]
            if kind == "while":
                cond = s._emit_cond(blocks[b], out)
                body = []; s._emit_seq(blocks, body_t, body)
                out.append("while (%s) {" % cond); out += ["    " + x for x in body]; out.append("}")
            else:                                  # endless/complex loop
                body = []; s._emit_seq(blocks, body_t, body)
                out.append("for (;;) {"); out += ["    " + x for x in body]; out.append("}")
        elif kind == "dowhile":
            b, body_t, latch = node[1], node[2], node[3]
            body = []; s._emit_seq(blocks, body_t[:-1], body)      # body minus the latch block node
            cond = s._emit_cond(blocks[latch], body)
            out.append("do {"); out += ["    " + x for x in body]; out.append("} while (%s);" % cond)
        elif kind == "break":
            out.append("break;")
        elif kind == "continue":
            out.append("continue;")
        elif kind == "goto":
            b, t = node[1], node[2]
            if t is not None:
                ta = s.ins[t].address if isinstance(t, int) and 0 <= t < len(s.ins) else t
                s.gototargets.add(ta); out.append("goto L_%x;" % ta)

    def emit_tree(s, blocks, tree):
        s.out = []
        s._emit_seq(blocks, tree, s.out)
        return s.out

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
    fs=0; k=1
    if ins[k].mnemonic=="sub" and ins[k].op_str.startswith("sp"):
        fs=int(ins[k].op_str.split(",")[1],16); k+=1
        while k<len(ins) and "bp, sp" not in ins[k].op_str: k+=1
    k+=1
    msc3=False
    if fs==0 and k+1 < len(ins) and ins[k+1].mnemonic=="call":
        if ins[k].mnemonic=="mov" and ins[k].op_str.startswith("ax,"):
            try: fs=int(ins[k].op_str.split(",")[1].strip(),16); msc3=True; k+=2
            except ValueError: pass
        elif ins[k].mnemonic=="xor" and ins[k].op_str.replace(" ","")=="ax,ax":
            fs=0; msc3=True; k+=2
    return fs, k, msc3


def recover_cfg(code, base=0, cxmap=None):
    """EXPERIMENTAL Phase-B path: structure the CFG (cfg.structure) and emit via R.emit_tree,
    reusing the SAME R value model as recover(). Not the default; used behind a flag / for A/B
    comparison against recover() until it is proven >= the linear walk on every function."""
    from src.engines.inverse import cfg as _cfg
    ins=list(_MD.disasm(code,0))
    if len(ins) < 2:
        return 0, [], [], "int", []
    fs, k, msc3 = _prologue(ins)
    r=R(ins,fs,base,cxmap,msc3=msc3); r.emit_start=k
    end_addr = ins[-1].address + ins[-1].size
    blocks,_,entry = _cfg.build_cfg(ins, 0, end_addr)
    tree = _cfg.structure(blocks, entry)
    stmts = r.emit_tree(blocks, tree)
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
