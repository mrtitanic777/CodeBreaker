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
JCC={"jl":"<","jle":"<=","jg":">","jge":">=","je":"==","jne":"!="}      # materialized value
INV={"jge":"<","jg":"<=","jle":">","jl":">=","jne":"==","je":"!=","jz":"!=","jnz":"=="}  # skip/exit-if-not
CJMP=set(JCC)|{"jz","jnz"}

class R:
    def __init__(s, ins, fs, base=0, cxmap=None):
        s.ins=ins; s.fs=fs; s.ax=None; s.si=None; s.cx=None; s.div=None; s.maxidx=-1; s.args=[]
        s.longfn=False; s.lmax=-1; s.floatfn=False; s.bx=None; s.dx=None; s._cmp=None
        s.base=base; s.cxmap=cxmap or {}; s.locals=set()
        s.hoisted=set(); s.out=None; s.i=0             # register value-tracking state
        s.retlive=False                                 # ax currently holds a live return value?
        s.axsrc=None; s.bxsrc=None                      # instr idx where ax/bx const value was LOADED
        s.ranks=[]; s.regconst=[]                        # per top-level stmt: source rank + reg-routed-const flag
        s.tgt={}                                        # branch idx -> target idx
        a2i={ii.address:i for i,ii in enumerate(ins)}
        for i,ii in enumerate(ins):
            if ii.mnemonic in CJMP or ii.mnemonic=="jmp":
                m=re.match(r"0x([0-9a-f]+)$", ii.op_str)
                if m: s.tgt[i]=a2i.get(int(m.group(1),16))
    def call_op(s, ci, default):
        """Resolve a CALL instruction's target to a C operator via the CX-helper map."""
        b=s.ins[ci].bytes
        if len(b)<3 or b[0]!=0xe8: return default
        rel=b[1]|(b[2]<<8); rel-= 0x10000 if rel>=0x8000 else 0
        tgt=(s.base + s.ins[ci].address + 3 + rel) & 0xffff
        return s.cxmap.get(tgt, default)
    def num(s,x): return int(x,0)
    def pname(s,off):
        idx=(off-s.fs-4)//2; s.maxidx=max(s.maxidx,idx)
        return chr(ord('a')+idx) if 0<=idx<26 else "p%d"%idx
    def memname(s,op):
        op=op.replace("word ptr ","").replace("byte ptr ","")
        m=re.search(r"\[bp \+ (0x[0-9a-f]+|\d+)\]",op)
        if m or "[bp]" in op:                                 # [bp] == offset 0
            o=s.num(m.group(1)) if m else 0
            if o < s.fs: s.locals.add(o); return "v%d"%o     # local (below saved BP)
            return s.pname(o)                                 # parameter
        m=re.search(r"\[si \+ (0x[0-9a-f]+|\d+)\]",op)
        if m: return "%s->f%d"%(s.si,s.num(m.group(1)))
        if "[si]" in op: return "*%s"%s.si
        m=re.search(r"\[(0x[0-9a-f]+|\d+)\]",op)
        if m: return "g_%x"%s.num(m.group(1))       # distinct global per address
        return None
    def rhs(s,op): return str(s.num(op)) if re.match(r"(0x[0-9a-f]+|\d+)$",op) else s.memname(op)

    def _writes(s,mn,dst,reg):
        """Does instruction (mn,dst) overwrite register reg (so its value is lost)?"""
        if dst==reg: return True
        if reg in ("ax","dx") and mn in ("imul","idiv","mul","div","cwd"): return True
        if reg=="ax" and mn=="call": return True
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
        return cur

    def _val(s,tok):
        """Resolve a source operand token: register->tracked value (rsrc), else imm/mem."""
        if tok=="ax": return s.ax                             # accumulator self-operand (add ax,ax)
        return s.rsrc(tok) if tok in ("bx","cx","dx") else s.rhs(tok)

    def emit(s, out, text, rank=None, rc=False):
        """Append a statement, its source rank (for un-scheduling), and whether it is a
        register-routed constant store (a signal for dead-store recovery)."""
        out.append(text); s.ranks.append(s.i if rank is None else rank); s.regconst.append(rc)

    def expr_step(s,i):
        """Update ax/si/cx from a value-producing instruction (no statement)."""
        s.i=i; old=s.ax; ii=s.ins[i]; mn=ii.mnemonic; ops=ii.op_str
        if mn=="mov" and ops.startswith("ax, "):
            src=ops[4:]
            if src=="dx" and s.div: s.ax="(%s %% %s)"%s.div; s.axsrc=None
            elif src in ("bx","cx","dx"): s.ax=s.rsrc(src); s.axsrc=None   # reg->reg copy
            elif re.match(r"(0x[0-9a-f]+|\d+)$",src): s.ax=s.rhs(src); s.axsrc=i  # LOADED const
            else: s.ax=s.rhs(src); s.axsrc=None
        elif mn=="mov" and ops.startswith("si, "): s.si=s.memname(ops[4:])
        elif mn=="mov" and ops.startswith("bx, "):
            s.bx=s.rhs(ops[4:]); s.bxsrc=i if re.match(r"(0x[0-9a-f]+|\d+)$",ops[4:]) else None
        elif mn=="mov" and ops.startswith("cx, "): s.cx=s.rhs(ops[4:])
        elif mn=="mov" and ops.startswith("dx, "): s.dx=s.rhs(ops[4:])
        elif mn=="xor" and ops in ("ax, ax","bx, bx"): s.ax="0"     # self-xor = clear (return 0)
        elif mn in ALU and ops.startswith("ax, "): s.ax="(%s %s %s)"%(s.ax,ALU[mn],s._val(ops[4:]))
        elif mn=="imul": s.ax="(%s * %s)"%(s.ax,s._val(ops))
        elif mn=="idiv": rn=s._val(ops); s.div=(s.ax,rn); s.ax="(%s / %s)"%(s.ax,rn)
        elif mn=="shl" and ops=="ax, cl": s.ax="(%s << %s)"%(s.ax,s.cx)
        elif mn=="sar" and ops=="ax, cl": s.ax="(%s >> %s)"%(s.ax,s.cx)
        if s.ax is not old: s.retlive=True                    # ax got a live computed value

    def cond_at(s,i):
        """Recover a branch condition starting at i (loads + cmp/test + jcc).
        Return (cond_str, jcc_idx)."""
        j=i
        def resolve(t):
            if t=="ax": return s.ax
            if t=="bx": return s.bx
            v=s.memname(t)
            return v if v is not None else (str(s.num(t)) if re.match(r"(0x[0-9a-f]+|\d+)$",t) else t)
        while s.ins[j].mnemonic not in CJMP:
            mn=s.ins[j].mnemonic; ops=s.ins[j].op_str
            if mn=="cmp":
                p=ops.split(", "); s._cmp=(resolve(p[0]), resolve(p[1]))
            elif mn=="test": s._cmp=None
            else: s.expr_step(j)
            j+=1
        jcc=s.ins[j].mnemonic
        cond="(%s %s %s)"%(s._cmp[0], INV[jcc], s._cmp[1]) if s._cmp else s.ax
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
        return 0

    def step(s,i,out):
        """Straight-line: process ins[i], maybe append a statement. Return next i."""
        s.i=i; s.out=out
        if i in s.hoisted: return i+1                          # store already emitted (hoisted)
        n=s.long_pat(i)
        if n: s.retlive=True; return i+n
        ii=s.ins[i]; mn=ii.mnemonic; ops=ii.op_str
        if mn=="cmp" and ops.startswith("ax, ") and i+2<len(s.ins) and s.ins[i+1].mnemonic in JCC and s.ins[i+2].mnemonic=="xor":
            s.ax="(%s %s %s)"%(s.ax, JCC[s.ins[i+1].mnemonic], s.memname(ops[4:])); s.retlive=True; s.axsrc=None
            while i<len(s.ins) and s.ins[i].op_str!="ax, bx": i+=1
            return i+1
        if mn=="push":
            s.args.append(s.ax if ops=="ax" else (s.memname(ops) or ops)); return i+1
        if mn=="call":
            m=re.match(r"(0x[0-9a-f]+|\d+)$", ops); tgt=s.num(m.group(1)) if m else 0
            expr="sub_%x(%s)"%(tgt, ", ".join(reversed(s.args))); s.args=[]
            if s.fs==0: s.emit(out,expr+";"); s.ax=None; s.retlive=False   # statement (result discarded)
            else: s.ax=expr; s.retlive=True                    # result used
            return i+1
        if mn=="mov" and ops=="sp, bp": return i+1             # post-call arg cleanup
        if mn=="mov" and "[" in ops.split(", ")[0]:            # store: mem = reg | immediate
            dest,_,src=ops.rpartition(", ")
            if src=="ax":
                # a reused CONST loaded earlier: rank the store at its LOAD (un-schedule the defer)
                rank = s.axsrc if (s.axsrc is not None and s.axsrc < i) else i
                rc = (s.axsrc is not None) and bool(re.match(r"-?\d+$|-?0x[0-9a-f]+$", str(s.ax)))
                s.emit(out,"%s = %s;"%(s.memname(dest), s.ax), rank, rc)
                s.ax=s.memname(dest); s.axsrc=None; s.retlive=False; return i+1   # aliases var
            if src in ("bx","cx"):
                rank = s.bxsrc if (src=="bx" and s.bxsrc is not None and s.bxsrc < i) else i
                s.emit(out,"%s = %s;"%(s.memname(dest), getattr(s,src)), rank)  # raw tracked value
                setattr(s,src,s.memname(dest))
                if src=="bx": s.bxsrc=None
                return i+1                                      # backward alias
            if re.match(r"(0x[0-9a-f]+|\d+)$",src):
                s.emit(out,"%s = %s;"%(s.memname(dest), s.num(src))); return i+1
        if mn in ALU and "[" in ops.split(", ")[0] and ops.endswith(", ax"):   # mem <op>= ax
            d=s.memname(ops.split(",")[0]); s.emit(out,"%s = %s %s %s;"%(d,d,ALU[mn],s.ax)); return i+1
        if mn in ("inc","dec") and ops in ("ax","bx"):
            s.ax="(%s %s 1)"%(s.ax,"+" if mn=="inc" else "-"); s.retlive=True; return i+1
        if mn in ("inc","dec"):
            nm=s.memname(ops); o="+" if mn=="inc" else "-"
            # fs>0 means MC1 allocated a temp -> source used the temp form x = x OP 1 (not x++/x--)
            s.emit(out,"%s = %s %s 1;"%(nm,nm,o) if s.fs>0 else "%s%s;"%(nm,"++" if mn=="inc" else "--"))
            return i+1
        if mn=="ret" and s.ax is not None and s.retlive: s.emit(out,"return %s;"%s.ax); return i+1
        s.expr_step(i); return i+1

    def walk(s,lo,hi):
        out=[]; i=lo
        while i<hi:
            s.out=out                                          # current block (for hoisted stores)
            # do-while: i is the target of a backward CONDITIONAL jump (test at the bottom)
            backc=[j for j,t in s.tgt.items() if t==i and s.ins[j].mnemonic in CJMP and i<j<hi]
            if backc:
                J=min(backc); ci=J-1
                while ci>i and s.ins[ci].mnemonic not in ("cmp","test"): ci-=1
                cm=s.ins[ci]; jcc=s.ins[J].mnemonic
                if cm.mnemonic=="cmp":
                    p=cm.op_str.split(", ")
                    def rv(t): return s.ax if t=="ax" else s.bx if t=="bx" else (s.memname(t) or (str(s.num(t)) if re.match(r"(0x[0-9a-f]+|\d+)$",t) else t))
                    l,r=rv(p[0]),rv(p[1])
                    cond=(l if str(r)=="0" else "(%s %s %s)"%(l, JCC.get(jcc, "!=" if jcc=="jne" else "=="), r))
                else: cond=s.ax
                body=s.walk(i, ci)
                out.append("do {"); out+=["    "+b for b in body]; out.append("} while (%s);"%cond)
                i=J+1; continue
            # while-loop: some later jmp targets i (back-edge to a loop top)
            back=[j for j,t in s.tgt.items() if t==i and s.ins[j].mnemonic=="jmp" and i<j<hi]
            if back:
                J=min(back); cond,jcc=s.cond_at(i); exit_idx=s.tgt[jcc]
                body=s.walk(jcc+1, J)
                out.append("while (%s) {"%cond); out+=["    "+b for b in body]; out.append("}")
                i=J+1; continue
            # if: cmp/test + forward conditional jump that skips a block (not materialized)
            if s.ins[i].mnemonic in ("cmp","test") and i+1<hi and s.ins[i+1].mnemonic in CJMP \
               and s.tgt.get(i+1,-1)>i+1 and not (i+2<len(s.ins) and s.ins[i+2].mnemonic=="xor"):
                cond,jcc=s.cond_at(i); tgt=s.tgt[jcc]
                body=s.walk(jcc+1, tgt)
                out.append("if (%s) {"%cond); out+=["    "+b for b in body]; out.append("}")
                i=tgt; continue
            i=s.step(i,out)
        return out

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
    fs=0; k=1
    if ins[k].mnemonic=="sub" and ins[k].op_str.startswith("sp"):
        fs=int(ins[k].op_str.split(",")[1],16); k+=1
        while "bp, sp" not in ins[k].op_str: k+=1
    k+=1
    r=R(ins,fs,base,cxmap); stmts=r.walk(k,len(ins))
    # UN-SCHEDULE: MC2 defers a reused const's store while its load stays at the source
    # position. For straight-line blocks, reorder statements by source rank (load index for
    # reused consts, store index otherwise) so the recompile reproduces the same emission.
    if stmts and len(r.ranks)==len(stmts) and not any("{" in x or "}" in x for x in stmts):
        perm=sorted(range(len(stmts)), key=lambda j: r.ranks[j])
        stmts=[stmts[j] for j in perm]; rc=[r.regconst[j] for j in perm]
        stmts=_dead_store_recover(stmts, rc)               # recover elided dead stores + copies
    loc=sorted(r.locals)
    if r.floatfn or r.longfn:
        pt="float" if r.floatfn else "long"
        return fs, [chr(ord('a')+j) for j in range(r.lmax+1)], stmts, pt, loc
    return fs, [chr(ord('a')+j) for j in range(r.maxidx+1)], stmts, "int", loc

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
