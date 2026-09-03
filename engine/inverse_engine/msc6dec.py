#!/usr/bin/env python3
r"""msc6dec.py -- the INTEGRATION driver for the MSC 6.0a inverse: one recursive pass
that runs register-value tracking (msc6reg), control-flow recovery (if-branches), and the
statement templates TOGETHER over an arbitrary function -- so a function with register
vars AND branches AND calls AND returns inverts as a whole, not one class at a time.

This is the unification the separate recognizers (msc6inv/msc6walk/msc6reg) were building
toward -- the shape of mc2inv's whole-function `recover`, retargeted to MSC 6.0a. Verified
byte-exact via the oracle (relocs masked); declines at the first unmodeled instruction.

    wt inverse msc6dec     # combined regvar+branch+calls self-test
"""
import os, re, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
_MD = Cs(CS_ARCH_X86, CS_MODE_16)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from engine.inverse_engine.msc6fn import _mask, PROLOGUE, EPI

# The far-loadds prologue ships in two encodings that differ only in how DS reaches AX:
# PROLOGUE is `push ds;pop ax` (1e58), this variant is `mov ax,ds` (8cd8). Both are 2 bytes
# and leave DS in AX with no other effect, so the frame is byte-identical from offset 2 on
# (same 90 45 55 8bec 1e 8ed8), the same length, and the same far epilogue -- invert() can
# treat the two interchangeably by just skipping the right prologue.
PROLOGUE_MOVDS = bytes.fromhex("8cd89045558bec1e8ed8")
# The far frame tears down two ways: EPI (dec bp;dec bp;mov sp,bp;pop ds;pop bp;dec bp) and
# this lea-variant (lea sp,[bp-2];pop ds;pop bp;dec bp) that mixed-object DLLs also emit.
EPI_LEA = bytes.fromhex("8d66fe1f5d4d")

JCC_INV = {0x74: "!=", 0x75: "==", 0x7c: ">=", 0x7d: "<", 0x7e: ">", 0x7f: "<="}


def _param(off): return chr(ord('a') + (off - 6) // 2)


JCC_DIRECT = {0x75: "!= 0", 0x74: "== 0", 0x7c: "<", 0x7d: ">=", 0x7e: "<=", 0x7f: ">"}
# opposite conditional opcode (je<->jne, jl<->jge, jle<->jg): used to get the DIRECT
# (jcc-fires) condition from cond_of, which otherwise returns the inverse (skip) form.
_JCC_OPP = {0x74: 0x75, 0x75: 0x74, 0x7c: 0x7d, 0x7d: 0x7c, 0x7e: 0x7f, 0x7f: 0x7e}
_JCC_SET = ("je", "jne", "jl", "jge", "jle", "jg")

# general ALU op -> C operator, and the word registers we build expressions in
_ALU2 = {"add": "+", "sub": "-", "and": "&", "or": "|", "xor": "^",
         "shl": "<<", "shr": ">>", "sar": ">>", "imul": "*"}
_WREG = ("ax", "bx", "cx", "dx", "si", "di")
# raw register tokens: if one of these reaches an output expression it means an unmodeled
# value leaked (e.g. dx holding a long-call high word we don't yet fold) -> decline.
_RAWREG = frozenset(("ax", "bx", "cx", "dx", "si", "di", "bp", "sp",
                     "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh"))


def _u16le(op, i): return op[i] | (op[i+1] << 8)


def _layout_params(params):
    """params: [(name, 'int'|'far'|'long'), ...] in DECL order. Pascal puts the last-
    declared at bp+6. Far AND long both occupy two words (lo@off, hi@off+2). Return
    (lanes{off:(name,'lo'|'hi')}, pnames{off:name}, longs{lo_off:name})."""
    off = 6; lanes = {}; pnames = {}; longs = {}
    for nm, ty in reversed(params or []):
        if ty == 'far':
            lanes[off] = (nm, 'lo'); lanes[off+2] = (nm, 'hi'); off += 4
        elif ty == 'long':
            longs[off] = nm; off += 4                       # lo@off, hi@off+2 (one long value)
        else:
            pnames[off] = nm; off += 2
    return lanes, pnames, longs


class Dec:
    def __init__(s, md, endaddr, imports, syms=None, params=None, nearmap=None, func_base=0):
        s.md = md; s.end = endaddr; s.imports = imports; s.syms = syms or {}
        s.nearmap = nearmap or {}; s.func_base = func_base; s.has_nearcall = False
        s.nears_used = {}          # name -> True, sibling functions called intra-segment
        s.cdecl_imports = set()    # imports called cdecl (caller cleans via add sp)
        s.lanes, s.pnames, s.longs = _layout_params(params)
        s.a2i = {ins.address: i for i, ins in enumerate(md)}
        s.calls = 0; s.regvars = {}; s.decl_order = []; s.globs = {}
        s.farty = {}          # far-param name -> 'int' once a WORD deref proves the pointee width
        s.localdecl = {}      # loc_x -> 'int'|'char', stack locals discovered during the walk
        s.farptr = {}         # base-reg -> the far pointer expr currently in <seg>:<reg>
        # pre-scan back-edges (do-while loops): jcc whose target index < its own index
        s.loops = {}           # loop-head index -> (backedge jcc index, jcc opcode)
        s.counter = {}         # loop-head index -> counter register (inc/dec'd inside)
        for i, ins in enumerate(md):
            if ins.mnemonic in ("jne", "je", "jl", "jge", "jle", "jg") and ins.op_str.startswith("0x"):
                t = s.a2i.get(int(ins.op_str, 16))
                if t is not None and t <= i:
                    s.loops[t] = (i, ins.bytes[0])
                    for k in range(t, i):                 # find the induction var (dec/inc reg)
                        if md[k].mnemonic in ("dec", "inc") and md[k].op_str in ("si", "di"):
                            s.counter[t] = md[k].op_str
        s.cname = {}                                       # counter reg -> loop-var name (i, j)
        for reg in dict.fromkeys(s.counter.values()):
            s.cname[reg] = chr(ord('i') + len(s.cname))

    def _ti(s, addr):
        """Instruction index for a jump target; a target at/after the body end (the
        epilogue) maps to len(md) so `if`/branches that skip to the return resolve."""
        j = s.a2i.get(addr)
        if j is None and addr >= s.end:
            return len(s.md)
        return j

    def _resolve_jmp(s, addr):
        """Follow a chain of unconditional `jmp`s to its ultimate target address. Unoptimized
        codegen threads every `return` through jumps that hop to the shared epilogue via one
        or more intermediate `jmp`s, so a `jmp` whose chain ends at the epilogue IS a return."""
        seen = set()
        while addr not in seen:
            seen.add(addr)
            ti = s.a2i.get(addr)
            if ti is None or ti >= len(s.md):
                return addr
            ins = s.md[ti]
            if ins.mnemonic == "jmp" and ins.op_str.startswith("0x"):
                addr = int(ins.op_str, 16); continue
            return addr
        return addr

    def cond_of(s, test, jcc_op, reg):
        """(test insn, jcc opcode) -> C condition string (the INVERSE of the skip-jcc)."""
        cc = JCC_INV[jcc_op]
        m, ops = test.mnemonic, test.op_str
        if m == "or" and "," in ops:
            r1, r2 = ops.split(", ")
            if r1 == r2:
                return "%s %s 0" % (s._val(r1, reg), cc)                   # or r,r -> test r
            v1, v2 = reg.get(r1, ""), reg.get(r2, "")                      # far-null: or hi,lo
            if "#" in v1 and "#" in v2 and v1.split("#")[0] == v2.split("#")[0]:
                return "%s %s 0" % (v1.split("#")[0], cc)
        if m == "cmp":
            l, r = ops.split(", ")
            return "%s %s %s" % (s._val(l, reg), cc, s._val(r, reg))
        return None

    def _shortcircuit(s, i, hi, reg):
        """Recover a short-circuit compound conditional: an OR of AND-terms that all
        converge on ONE forward JOIN block (`if ((A&&B)||(C&&D)||...) { <join> } else...`).
        Each term is a run of `cmp/or/test; jcc` conditions. The fall-through after the
        whole chain IS the JOIN; a jcc to JOIN ends a term (success), a jcc to a later
        pair chains the AND, and the trailing term falls through to JOIN with its failures
        going to EXIT (the else). Returns (cond, join_idx, exit_idx) or None (decline)."""
        md = s.md
        pairs = []; idx = i
        while idx + 1 < hi and md[idx].mnemonic in ("cmp", "or", "test") \
                and md[idx+1].mnemonic in _JCC_SET:
            tgt = s._ti(int(md[idx+1].op_str, 16))
            if tgt is None or tgt <= idx:               # backward/unknown -> stop
                break
            pairs.append((idx, md[idx+1].bytes[0], tgt)); idx += 2
        if len(pairs) < 3:                              # need >=2 OR-terms
            return None
        join = idx                                      # fall-through after the chain
        exits = {t for _, _, t in pairs if t > join}
        if len(exits) != 1 or join >= hi:
            return None
        exit_i = exits.pop()
        starts = {p[0] for p in pairs}
        for _, _, t in pairs:                           # every target must be join / exit / a chain step
            if t != join and t != exit_i and t not in starts:
                return None
        terms = []; cur = []
        for ci, jop, t in pairs:
            if t == join:                               # success: jcc FIRES on true -> direct cond
                c = s.cond_of(md[ci], _JCC_OPP.get(jop, jop), reg)
                if c is None: return None
                cur.append(c); terms.append(cur); cur = []
            else:                                       # chain / exit: skip on false -> inverse cond
                c = s.cond_of(md[ci], jop, reg)
                if c is None: return None
                cur.append(c)
        if cur:                                         # trailing term falls through to join
            terms.append(cur)
        if len(terms) < 2:
            return None
        cond = " || ".join("(%s)" % " && ".join(g) for g in terms)
        return cond, join, exit_i

    def _bool_materialize(s, i, hi, reg):
        """Recover a MATERIALIZED boolean -- MSC 6.0a builds `(A || B || ...)` into a
        register as a literal 0/1 and tests it later, instead of branching directly:

            cmp A,0 ; jne T        (>=1 such, all jne to the SAME target T)
            [cmp B,0 ; jne T]...
            sub R,R ; jmp J        (false fall-through: R = 0)
          T:  mov R,1              (true, the jne target: R = 1)
          J:  or R,R ; jcc ...     (the caller now tests R, which holds (A||B))

        Returns (R, expr, J_index) so walk() can set reg[R]=expr and resume at J, where the
        existing `or R,R ; jcc` handler renders `if ((A||B) <cc> 0)`. None if it doesn't match."""
        md = s.md
        pairs = []; idx = i
        while idx + 1 < hi and md[idx].mnemonic == "cmp" \
                and md[idx].op_str.endswith(", 0") and md[idx+1].mnemonic == "jne":
            t = s._ti(int(md[idx+1].op_str, 16))
            if t is None or t <= idx:
                break
            pairs.append((idx, md[idx+1].bytes[0])); idx += 2
        if not pairs:
            return None
        T = s._ti(int(md[pairs[0][0]+1].op_str, 16))
        if any(s._ti(int(md[ci+1].op_str, 16)) != T for ci, _ in pairs):
            return None                                  # all jne must target the same true-block
        if idx + 1 >= hi or md[idx].mnemonic != "sub":   # sub R,R
            return None
        a, _, b = md[idx].op_str.partition(", ")
        if not b or a != b or a not in _WREG:
            return None
        R = a
        if md[idx+1].mnemonic != "jmp":                  # jmp J
            return None
        J = s._ti(int(md[idx+1].op_str, 16))
        if J is None or T is None or T >= len(md):
            return None
        if md[T].mnemonic != "mov" or md[T].op_str != "%s, 1" % R:   # T: mov R,1
            return None
        if T + 1 != J:                                   # J is the fall-through after `mov R,1`
            return None
        terms = []
        for ci, jop in pairs:                            # each jne fires when the operand != 0
            c = s.cond_of(md[ci], _JCC_OPP[jop], reg)
            if c is None:
                return None
            terms.append(c)
        return R, "(" + " || ".join(terms) + ")", J

    def pname(s, off):
        """The C name of the parameter at [bp+off], honoring a typed signature (s.pnames
        for int params, s.lanes for far) so the body agrees with the declared signature;
        falls back to the positional a,b,c,... when no signature was given."""
        if off in s.pnames:
            return s.pnames[off]
        if off in s.lanes:
            return s.lanes[off][0]
        return _param(off)

    def _local(s, off, byte=False):
        """A stack local at [bp-off] -> a C variable, declared on first use."""
        nm = "loc_%x" % off
        if nm not in s.localdecl:
            s.localdecl[nm] = "char" if byte else "int"
        return nm

    def _farmem(s, tok):
        """A segment-indexed operand `<seg>:[<base>(+<disp>)?]` -> a C far-pointer field
        access, using the far pointer tracked in s.farptr[base]. Returns (expr, is_byte)
        or None if the base register doesn't hold a known far pointer."""
        m = re.match(r"(word|byte) ptr \w+:\[(\w+)(?:\s*\+\s*(\w+))?\]$", tok)
        if not m:
            return None
        wty, base, disp = m.group(1), m.group(2), m.group(3)
        p = s.farptr.get(base)
        if p is None:
            return None
        cty = "char" if wty == "byte" else "WORD"
        d = int(disp, 0) if disp else 0
        if d:
            expr = "*(%s far *)((char far *)%s + %d)" % (cty, p, d)
        else:
            expr = "*(%s far *)%s" % (cty, p)
        return expr, (wty == "byte")

    def _val(s, tok, reg):
        if ":[" in tok:                                        # <seg>:[base+N] far-ptr field
            ff = s._farmem(tok if "ptr" in tok else "word ptr " + tok)
            if ff:
                return ff[0]
        byte = "byte ptr" in tok
        tok = tok.replace("word ptr ", "").replace("byte ptr ", "")
        if tok in reg:
            return reg[tok]
        m = re.match(r"\[bp \+ (\w+)\]", tok)
        if m:
            return s.pname(int(m.group(1), 0))
        ml = re.match(r"\[bp - (\w+)\]$", tok)                 # stack local [bp-off]
        if ml:
            return s._local(int(ml.group(1), 0), byte)
        mg = re.match(r"\[0x([0-9a-f]+)\]$", tok)              # direct [imm16]: a DGROUP global
        if mg:
            off = int(mg.group(1), 16); nm = s.syms.get(off); lo = s.syms.get(off - 2)
            if nm and nm[1] == "long":
                s.globs[nm[0]] = "long"; return nm[0]
            if lo and lo[1] == "long":
                s.globs[lo[0]] = "long"; return "(WORD)(%s >> 16)" % lo[0]
            name = nm[0] if nm else "gW_%x" % off
            s.globs[name] = "word"; return name
        mi = re.match(r"\[(bx|si|di)(?:\s*([+-])\s*(0x[0-9a-f]+|\d+))?\]$", tok)
        if mi:                                                 # near-indexed [base(+/-disp)]:
            bv = reg.get(mi.group(1))                          # pointer / array / struct field
            if bv is not None and bv not in _RAWREG:
                d = int(mi.group(3), 0) if mi.group(3) else 0
                if mi.group(2) == "-":
                    d = -d
                cty = "char" if byte else "WORD"
                if d:
                    return "*(%s *)((char *)%s + %d)" % (cty, bv, d)
                return "*(%s *)%s" % (cty, bv)
        if re.match(r"(-?0x[0-9a-f]+|-?\d+)$", tok):
            return str(int(tok, 0))
        return tok

    def _longload(s, off):
        """[bp+off] as a long-value expr: a long PARAM (lo lane) or None."""
        return s.longs.get(off)

    def _long(s, i, hi, reg):
        """Try to fold a DWORD/long-arith run starting at md[i]. dx:ax is the long
        accumulator (reg['dxax']). Returns new index, or i if nothing matched."""
        md = s.md
        m, ops = md[i].mnemonic, md[i].op_str
        # long param load:  mov ax,[bp+lo] ; mov dx,[bp+lo+2]
        g = re.match(r"ax, word ptr \[bp \+ (\w+)\]$", ops)
        if m == "mov" and g and i+1 < hi:
            off = int(g.group(1), 0); nm = s._longload(off)
            n2 = re.match(r"dx, word ptr \[bp \+ (\w+)\]$", md[i+1].op_str)
            if nm and md[i+1].mnemonic == "mov" and n2 and int(n2.group(1), 0) == off+2:
                reg["dxax"] = nm; return i+2
        # long shift left:  (shl ax,1 ; rcl dx,1) x k   ->  (expr << k)
        if m == "shl" and ops == "ax, 1" and "dxax" in reg:
            k = 0; j = i
            while j+1 < hi and md[j].mnemonic == "shl" and md[j].op_str == "ax, 1" \
                    and md[j+1].mnemonic == "rcl" and md[j+1].op_str == "dx, 1":
                k += 1; j += 2
            if k:
                reg["dxax"] = "(%s << %d)" % (reg["dxax"], k); return j
        # long add:  add ax,[bp+M] ; adc dx,[bp+M+2]
        g = re.match(r"ax, word ptr \[bp \+ (\w+)\]$", ops)
        if m == "add" and g and "dxax" in reg and i+1 < hi:
            off = int(g.group(1), 0); nm = s._longload(off)
            n2 = re.match(r"dx, word ptr \[bp \+ (\w+)\]$", md[i+1].op_str)
            if nm and md[i+1].mnemonic == "adc" and n2 and int(n2.group(1), 0) == off+2:
                reg["dxax"] = "%s + %s" % (reg["dxax"], nm); return i+2
        # zero-extend a word to long:  sub dx,dx  (dx=0, ax already a word value)
        if m == "sub" and ops == "dx, dx" and reg.get("ax"):
            reg["dxax"] = "(long)%s" % reg["ax"]; return i+1
        # (long)word << 16:  mov dx,[bp+N] ; sub ax,ax   (word into the high half)
        g = re.match(r"dx, word ptr \[bp \+ (\w+)\]$", ops)
        if m == "mov" and g and i+1 < hi and md[i+1].mnemonic == "sub" and md[i+1].op_str == "ax, ax":
            off = int(g.group(1), 0); nm = s.pnames.get(off) or _param(off)
            reg["dxax"] = "(long)%s << 16" % nm; return i+2
        return i

    def walk(s, blo, hi, reg):
        """Recover md[blo:hi] as C lines with register state `reg`. `blo` is the block
        start; only the top-level block (blo==0) emits the fall-through return."""
        out = []; i = blo; _guard = 0
        while i < hi:
            _guard += 1
            if _guard > 8 * len(s.md) + 64:                 # backstop: never spin forever
                s.fail = "iteration backstop (control-flow not modeled)"; return None
            ins = s.md[i]; op = ins.bytes; mn = ins.mnemonic; ops = ins.op_str
            s.fail = "%s %s" % (mn, ops)                     # last insn touched (for decline reason)
            j = s._long(i, hi, reg)                          # DWORD/long-arith fold
            if j != i:
                i = j; continue
            # do-while loop head: recover `do { body } while (cond);`
            if i in s.loops and i not in getattr(s, "active", set()):
                if not hasattr(s, "active"):
                    s.active = set()
                s.active.add(i)
                jcc_i, jop = s.loops[i]; ctr = s.counter.get(i); cn = s.cname.get(ctr)
                if cn:
                    reg[ctr] = cn
                flag = s.md[jcc_i - 1]                    # the instruction the back-edge tests
                if flag.mnemonic == "cmp":               # cmp A,B; jcc -> `A <cc> B` (condition only)
                    body_end = jcc_i - 1
                    l, r = flag.op_str.split(", ")
                    cond = "%s %s %s" % (s._val(l, reg), JCC_DIRECT[jop], s._val(r, reg))
                else:                                    # dec/inc counter; jcc -> `i != 0` (i-- is a stmt)
                    body_end = jcc_i
                    cond = "%s %s" % (cn, JCC_DIRECT[jop]) if cn else JCC_DIRECT[jop]
                body = s.walk(i, body_end, dict(reg))
                s.active.discard(i)
                if body is None:
                    return None
                out.append("do {")
                out += ["    " + t for t in body]
                out.append("} while (%s);" % cond)
                if cn:
                    reg[ctr] = cn
                i = jcc_i + 1; continue
            # counter increment/decrement in a loop -> i++ / i--
            if mn in ("dec", "inc") and ops in s.cname:
                out.append("%s%s;" % (s.cname[ops], "--" if mn == "dec" else "++"))
                i += 1; continue
            # memory inc/dec of a global -> gW_x++ / gW_x--
            if mn in ("dec", "inc"):
                dm = re.match(r"(?:word ptr )?\[0x([0-9a-f]+)\]$", ops)
                if dm:
                    off = int(dm.group(1), 16); nm = s.syms.get(off)
                    name = nm[0] if nm else "gW_%x" % off; s.globs[name] = nm[1] if nm else "word"
                    out.append("%s%s;" % (name, "--" if mn == "dec" else "++")); i += 1; continue
            # long-global test: a1 <hi>; or ax,[lo]; jcc  ->  if (name <cc> 0L) { then }
            if op[0] == 0xa1 and i+2 < hi and s.md[i+1].bytes[:2] == b"\x0b\x06" \
                    and s.md[i+2].mnemonic in ("je", "jne"):
                lo = _u16le(s.md[i+1].bytes, 2); nm = s.syms.get(lo)
                if nm and nm[1] == "long":
                    s.globs[nm[0]] = "long"; jcc = s.md[i+2]
                    cond = "%s %s 0L" % (nm[0], JCC_INV[jcc.bytes[0]])
                    skip_i = s._ti(int(jcc.op_str, 16))
                    if skip_i is None: return None
                    then = s.walk(i+3, skip_i, dict(reg))
                    if then is None:
                        return None
                    out.append("if (%s) {" % cond); out += ["    " + t for t in then]; out.append("}")
                    i = skip_i; continue
            # long-global store zero: sub ax,ax; mov [hi],ax; mov [lo],ax  ->  name = 0L;
            if op[:2] == b"\x2b\xc0" and i+2 < hi and s.md[i+1].bytes[0] == 0xa3 and s.md[i+2].bytes[0] == 0xa3:
                lo = _u16le(s.md[i+2].bytes, 1); nm = s.syms.get(lo)
                if nm and nm[1] == "long":
                    s.globs[nm[0]] = "long"; out.append("%s = 0L;" % nm[0]); reg["ax"] = "0"; i += 3; continue
            # short-circuit compound conditional: OR-of-ANDs converging on a forward JOIN
            if mn in ("cmp", "or", "test") and i + 1 < hi and s.md[i+1].mnemonic in _JCC_SET:
                sc = s._shortcircuit(i, hi, reg)
                if sc is not None:
                    cond, join, exit_i = sc
                    body = s.walk(join, exit_i, dict(reg))
                    if body is None:
                        return None
                    out.append("if (%s) {" % cond)
                    out += ["    " + t for t in body]
                    out.append("}")
                    i = exit_i; continue
            # materialized boolean: cmp A,0;jne T / sub R,R;jmp J / T: mov R,1  -> reg holds (A||B)
            if mn == "cmp" and i + 1 < hi and s.md[i+1].mnemonic == "jne":
                bm = s._bool_materialize(i, hi, reg)
                if bm is not None:
                    R, expr, J = bm
                    reg[R] = expr; i = J; continue
            # branch: (or r,r | cmp ..) followed by jcc -> if / if-else
            if i + 1 < hi and s.md[i+1].mnemonic in ("jne", "je", "jl", "jge", "jle", "jg") \
                    and mn in ("or", "cmp"):
                jcc = s.md[i+1]; jop = jcc.bytes[0]
                skip_i = s._ti(int(jcc.op_str, 16))
                if skip_i is None or skip_i <= i:
                    s.fail = "backward branch (goto): %s" % ops
                    return None                              # backward jcc = goto/loop, not an if
                # If the then-block ends in a FORWARD `jmp L2` to a MID-function label (strictly
                # before the epilogue), that jmp is the else-skip: then=[i+2:jmp], else=[skip_i:L2],
                # both join at L2. A `jmp` to the epilogue is a return and stays in the then-block
                # (walk() renders it), so exclude L2==len(md).
                els = None; cont = skip_i; then_end = skip_i; jx = skip_i - 1
                if jx > i + 1 and s.md[jx].mnemonic == "jmp" and s.md[jx].op_str.startswith("0x"):
                    l2 = s._ti(int(s.md[jx].op_str, 16))
                    if l2 is not None and skip_i < l2 < len(s.md):
                        then_end = jx; els = (skip_i, l2); cont = l2
                then = s.walk(i + 2, then_end, dict(reg))
                if then is None:
                    return None
                if els is not None and not then:
                    # empty then before the else-jmp = a lone jcc-over-jmp long conditional;
                    # emit it as the DIRECT-condition `if` with the else-block as its body.
                    cond = s.cond_of(ins, _JCC_OPP.get(jop, jop), reg)
                    body = s.walk(els[0], els[1], dict(reg))
                    if cond is None or body is None:
                        return None
                    out.append("if (%s) {" % cond)
                    out += ["    " + t for t in body]
                    out.append("}")
                    i = cont; continue
                cond = s.cond_of(ins, jop, reg)
                if cond is None:
                    return None
                out.append("if (%s) {" % cond)
                out += ["    " + t for t in then]
                if els is not None:
                    eb = s.walk(els[0], els[1], dict(reg))
                    if eb is None:
                        return None
                    out.append("} else {")
                    out += ["    " + t for t in eb]
                out.append("}")
                i = cont; continue
            # jmp to the epilogue (directly or via a jmp-chain) after ax set == return ax
            if mn == "jmp":
                tgt = s._resolve_jmp(int(ops, 16))
                if tgt >= s.end and "ax" in reg:
                    out.append("return %s;" % reg["ax"])
                i += 1; continue
            # intra-segment far call:  push cs ; call near <rel>  (linker-optimized far call
            # to a same-segment function). Target resolves in SEGMENT coords via func_base.
            if mn == "push" and ops == "cs" and i+1 < hi and s.md[i+1].mnemonic == "call":
                tgt = getattr(s, "func_base", 0) + int(s.md[i+1].op_str, 16)
                nm = s.nearmap.get(tgt, "sub_%x" % tgt); s.has_nearcall = True
                s.nears_used[nm] = True
                reg["ax"] = "%s(%s)" % (nm, ", ".join(s.args)); s.args = []
                reg["_lastcall"] = reg["ax"]
                nx = s.md[i+2] if i+2 < hi else None
                if nx is not None and nx.mnemonic == "add" and nx.op_str.startswith("sp,"):
                    nx = s.md[i+3] if i+3 < hi else None      # cdecl cleanup isn't the consumer
                cons = nx is not None and (
                    (nx.mnemonic == "mov" and nx.op_str.endswith(", ax")) or
                    (nx.mnemonic == "push" and nx.op_str == "ax") or
                    (nx.mnemonic in ("cmp", "test") and "ax" in nx.op_str.split(", ")) or
                    (nx.mnemonic == "or" and nx.op_str == "ax, ax") or
                    (nx.mnemonic == "jmp" and nx.op_str.startswith("0x") and int(nx.op_str, 16) >= s.end))
                if not cons and not (nx is None and not getattr(s, "void", False)):
                    out.append(reg["ax"] + ";"); reg.pop("_lastcall", None)
                elif cons and nx is not None and nx.mnemonic in ("cmp", "test", "or"):
                    reg.pop("_lastcall", None)
                s.nearcalls = getattr(s, "nearcalls", 0) + 1
                i += 2; continue
            # bare near call:  call <rel16>  (e8) -- a NEAR call to a same-segment sibling
            # (no push cs). Same handling as the intra-seg call; verified at the link.
            if mn == "call" and op[0] == 0xe8:
                tgt = getattr(s, "func_base", 0) + int(ops, 16)
                nm = s.nearmap.get(tgt, "sub_%x" % tgt); s.has_nearcall = True
                s.nears_used[nm] = True
                reg["ax"] = "%s(%s)" % (nm, ", ".join(s.args)); s.args = []
                reg["_lastcall"] = reg["ax"]
                nx = s.md[i+1] if i+1 < hi else None
                if nx is not None and nx.mnemonic == "add" and nx.op_str.startswith("sp,"):
                    nx = s.md[i+2] if i+2 < hi else None      # cdecl cleanup isn't the consumer
                cons = nx is not None and (
                    (nx.mnemonic == "mov" and nx.op_str.endswith(", ax")) or
                    (nx.mnemonic == "push" and nx.op_str == "ax") or
                    (nx.mnemonic in ("cmp", "test") and "ax" in nx.op_str.split(", ")) or
                    (nx.mnemonic == "or" and nx.op_str == "ax, ax") or
                    (nx.mnemonic == "jmp" and nx.op_str.startswith("0x") and int(nx.op_str, 16) >= s.end))
                if not cons and not (nx is None and not getattr(s, "void", False)):
                    out.append(reg["ax"] + ";"); reg.pop("_lastcall", None)
                elif cons and nx is not None and nx.mnemonic in ("cmp", "test", "or"):
                    reg.pop("_lastcall", None)
                i += 1; continue
            if op[0] == 0x9a:                                   # CALLF
                nm = s.imports.get(s.calls, "G%d" % s.calls); s.calls += 1
                nxt = s.md[i+1] if i+1 < hi else None
                is_cdecl = nxt is not None and nxt.mnemonic == "add" and nxt.op_str.startswith("sp,")
                if is_cdecl:
                    s.cdecl_imports.add(nm)                      # caller cleans -> cdecl import
                    argv = list(reversed(s.args))               # cdecl pushes args right-to-left
                    nxt = s.md[i+2] if i+2 < hi else None        # the add sp isn't the consumer
                else:
                    argv = s.args                               # pascal: push order == arg order
                reg["ax"] = "%s(%s)" % (nm, ", ".join(argv)); s.args = []
                reg["_lastcall"] = reg["ax"]
                consumed = nxt is not None and (
                    (nxt.mnemonic == "mov" and nxt.op_str.endswith(", ax")) or
                    (nxt.mnemonic == "push" and nxt.op_str == "ax") or
                    (nxt.mnemonic in ("cmp", "test") and "ax" in nxt.op_str.split(", ")) or
                    (nxt.mnemonic == "or" and nxt.op_str in ("ax, ax",)) or
                    (nxt.mnemonic == "jmp" and nxt.op_str.startswith("0x")
                     and int(nxt.op_str, 16) >= s.end))       # jmp to epilogue == return of ax
                if not consumed and not (nxt is None and not getattr(s, "void", False)):
                    out.append(reg["ax"] + ";"); reg.pop("_lastcall", None)
                elif consumed and nxt is not None and nxt.mnemonic in ("cmp", "test", "or"):
                    reg.pop("_lastcall", None)                   # embedded in a condition
            elif mn in ("les", "lds"):                       # les/lds <reg>, <far pointer source>
                dst, _, memt = ops.partition(", ")
                m = re.search(r"\[bp \+ (\w+)\]", memt)
                mg = re.search(r"\[0x([0-9a-f]+)\]", memt)
                ff = s._farmem("word ptr " + memt.split("ptr", 1)[-1].strip()) if ":" in memt else None
                if m:                                        # les reg, [bp+off]: a far param p
                    o = int(m.group(1), 0); lane = s.lanes.get(o)
                    p = lane[0] if lane else s.pname(o)
                    s.farptr[dst] = p; s._esbx = p
                elif ff:                                      # les reg, es:[base+N]: far ptr in a field
                    s.farptr[dst] = ff[0]; s._esbx = ff[0]
                elif mg:                                      # les reg, [imm]: a global far pointer
                    off = int(mg.group(1), 16); nm = s.syms.get(off)
                    p = nm[0] if nm else "gW_%x" % off; s.globs[p] = "word"
                    s.farptr[dst] = p; s._esbx = p
                else:
                    return None
            elif mn == "push" and ops in ("ss", "ds", "es"):  # segment half of a far-ptr arg:
                nx = s.md[i+1] if i+1 < hi else None           # push <seg> ; push <offset>
                if nx is not None and nx.mnemonic == "push":   # -> (T far *)<offset>
                    ov = nx.op_str.replace("word ptr ", "")
                    off = reg.get(ov, s._val(ov, reg))
                    if off is not None and off not in _RAWREG:
                        s.args.append("(char far *)%s" % off)
                        i += 2; continue
                s.fail = "push %s" % ops; return None
            elif mn == "push" and ":[" in ops:               # push <seg>:[base+N]: a far-ptr field
                ff = s._farmem(ops)
                if ff is None:
                    s.fail = "push %s" % ops; return None
                s.args.append(ff[0])
            elif mn == "push" and ops.startswith("dword ptr "):
                # 66 ff /6: a 32-bit push of a memory dword -- one `long` argument. The param
                # layout marks the source slot long (inferred above or from the signature).
                loc = ops[len("dword ptr "):]
                gp = re.match(r"\[bp \+ (\w+)\]$", loc)
                gl = re.match(r"\[bp - (\w+)\]$", loc)
                gg = re.match(r"\[(0x[0-9a-f]+)\]$", loc)
                if gp:
                    off = int(gp.group(1), 0); nm = s._longload(off)   # long param?
                    if nm is None and off in s.lanes and s.lanes[off][1] == "lo":
                        nm = s.lanes[off][0]                  # far-pointer param, pushed whole
                    if nm is None:                            # neither long nor far here: don't guess
                        s.fail = "push %s" % ops; return None
                    s.args.append(nm)
                elif gl:
                    nm = "loc_%x" % int(gl.group(1), 0); s.localdecl[nm] = "long"
                    s.args.append(nm)
                elif gg:
                    off = int(gg.group(1), 16); sy = s.syms.get(off)
                    nm = sy[0] if (sy and sy[1] == "long") else "gL_%x" % off
                    s.globs[nm] = "long"; s.args.append(nm)
                else:
                    s.fail = "push %s" % ops; return None
            elif mn == "push":
                r = ops.replace("word ptr ", "")
                mm = re.match(r"\[(0x[0-9a-f]+)\]$", r)
                if r == "es:[bx]":                            # push *p  (deref, pass value)
                    ep = getattr(s, "_esbx", "p"); s.farty[ep] = "int"
                    s.args.append("*%s" % ep)
                elif r in reg and "#" in reg[r]:
                    nm, ln = reg[r].split("#")
                    if ln == "hi":
                        s._pendfar = nm                                    # wait for the lo push
                    elif getattr(s, "_pendfar", None) == nm:
                        s.args.append(nm); s._pendfar = None               # hi+lo -> one far arg
                    else:
                        s.args.append(nm)
                elif r in reg:
                    # long/far-NULL constant idiom:  sub r,r ; push r ; push r  -> one 0L
                    # arg (hi-word then lo-word of a 32-bit zero). Two int args never reuse
                    # the SAME register for both consecutive pushes, so this is unambiguous.
                    if getattr(s, "_skip_push", None) == r:
                        s._skip_push = None                        # swallow the lo-word push
                    elif reg[r] == "0" and i + 1 < hi and s.md[i+1].mnemonic == "push" \
                            and s.md[i+1].op_str == r:
                        s.args.append("0L"); s._skip_push = r
                    else:
                        s.args.append(reg[r])
                elif re.match(r"\[bp \+ (\w+)\]", r):
                    s.args.append(s.pname(int(re.search(r"\+ (\w+)", r).group(1), 0)))
                elif re.match(r"\[bp - (\w+)\]$", r):             # push a local by value (unopt)
                    s.args.append(s._local(int(re.search(r"- (\w+)", r).group(1), 0)))
                elif re.match(r"(-?0x[0-9a-f]+|-?\d+)$", r):
                    v = int(r, 0)
                    if op[0] == 0x68 and v in getattr(s, "memoff", ()):   # push imm16 = &global
                        nm = s.syms.get(v)
                        name = nm[0] if nm else "gW_%x" % v
                        s.globs[name] = nm[1] if nm else "word"
                        s.args.append("&%s" % name)
                    else:
                        s.args.append(str(v))
                elif mm:                                          # push [off]: global word / long lane
                    off = int(mm.group(1), 16); nm = s.syms.get(off); lo = s.syms.get(off - 2)
                    if nm and nm[1] == "long":
                        s.globs[nm[0]] = "long"; s.args.append("(WORD)%s" % nm[0])
                    elif lo and lo[1] == "long":
                        s.globs[lo[0]] = "long"; s.args.append("(WORD)(%s >> 16)" % lo[0])
                    elif nm:
                        s.globs[nm[0]] = "word"; s.args.append(nm[0])
                    else:
                        g = "gW_%x" % off; s.globs[g] = "word"; s.args.append(g)
                else:
                    return None
            elif mn == "mov":
                dst, _, src = ops.partition(", ")
                if src in s.farptr:                          # mov si, bx : copy a far-ptr OFFSET
                    s.farptr[dst] = s.farptr[src]            # (segment stays in es/ds) -> es:[si] resolves
                elif dst in s.farptr and src not in s.farptr:
                    del s.farptr[dst]                        # dst reused for something else: drop alias
                if ":[" in src:                              # reg = <seg>:[base+N]  (field load)
                    ff = s._farmem(src)
                    if ff is None:
                        s.fail = "mov %s" % ops; return None
                    reg[dst] = ff[0]; i += 1; continue
                if ":[" in dst:                              # <seg>:[base+N] = src  (field store)
                    ff = s._farmem(dst)
                    if ff is None:
                        s.fail = "mov %s" % ops; return None
                    v = reg.get(src, s._val(src, reg))
                    if v in _RAWREG:                             # unmodeled reg (e.g. long-call hi word) -> decline
                        s.fail = "store raw %s" % v; return None
                    out.append("%s = %s;" % (ff[0], v)); i += 1; continue
                # compute the source value expr
                if src in reg:
                    val = reg[src]
                elif re.match(r"word ptr \[bp \+ (\w+)\]", src):
                    o = int(re.search(r"\+ (\w+)", src).group(1), 0)
                    if o in s.lanes:
                        nm, ln = s.lanes[o]; val = "%s#%s" % (nm, ln)   # far-ptr lane
                    elif o in s.pnames:
                        val = s.pnames[o]
                    else:
                        val = s.pname(o)
                elif re.match(r"(-?0x[0-9a-f]+|-?\d+)$", src):
                    val = str(int(src, 0))
                elif re.match(r"(?:word|byte) ptr \[(0x[0-9a-f]+|bp - \w+)\]$", src):
                    val = s._val(src, reg)                       # load: global / stack local
                else:
                    return None
                if "#" in val:                                   # a far param LANE was loaded into a reg:
                    base = val.split("#")[0]                      # form the far pointer in <seg>:<reg>
                    if dst in ("es", "ds"):                      # segment lane -> find the offset reg
                        for r2, rv in reg.items():
                            if rv == "%s#lo" % base:
                                s.farptr[r2] = base
                    elif val.endswith("#lo") and (reg.get("es") == "%s#hi" % base
                                                  or reg.get("ds") == "%s#hi" % base):
                        s.farptr[dst] = base                     # offset lane, segment already loaded
                # DX:AX 32-bit long store: mov [m],ax ; mov [m+2],dx -> *(long)m = <dx:ax>.
                # The paired adjacent word stores of ax (low) then dx (high) are MSC's long
                # store; emit it as one long assignment and consume both.
                if src == "ax" and i + 1 < hi and s.md[i + 1].mnemonic == "mov":
                    nxo = s.md[i + 1].op_str
                    lb = re.match(r"(?:word ptr )?\[bp - (\w+)\]$", dst)
                    lg = re.match(r"(?:word ptr )?\[0x([0-9a-f]+)\]$", dst)
                    if lb:
                        n = int(lb.group(1), 0)
                        h = re.match(r"(?:word ptr )?\[bp - (\w+)\], dx$", nxo)
                        if h and int(h.group(1), 0) == n - 2:
                            nm = s._local(n, False); s.localdecl[nm] = "long"
                            out.append("%s = %s;" % (nm, val)); i += 2; continue
                    elif lg:
                        g = int(lg.group(1), 16)
                        h = re.match(r"(?:word ptr )?\[0x([0-9a-f]+)\], dx$", nxo)
                        if h and int(h.group(1), 16) == g + 2:
                            sym = s.syms.get(g); name = sym[0] if sym else "gL_%x" % g
                            s.globs[name] = "long"
                            out.append("%s = %s;" % (name, val)); i += 2; continue
                ml = re.match(r"(?:(word|byte) ptr )?\[bp - (\w+)\]$", dst)
                if ml:                                           # mov [bp-off], <src>: local store
                    nm = s._local(int(ml.group(2), 0), ml.group(1) == "byte")
                    out.append("%s = %s;" % (nm, val)); i += 1; continue
                mg = re.match(r"(?:(word|byte) ptr )?\[0x([0-9a-f]+)\]$", dst)
                if mg:                                           # mov [imm16], <reg/imm>: global store
                    wdt = mg.group(1) or "word"; off = int(mg.group(2), 16)
                    nm = s.syms.get(off); lolane = s.syms.get(off - 2)
                    if (nm and nm[1] == "long") or (lolane and lolane[1] == "long"):
                        return None                              # a long-global lane: not modeled here
                    if wdt == "byte":                            # mov byte [off], imm8 -> gBxx = imm;
                        name = "gB_%x" % off; s.globs[name] = "byte"
                    else:
                        name = nm[0] if nm else "gW_%x" % off; s.globs[name] = "word"
                    out.append("%s = %s;" % (name, val))         # gxx = <val>; (ax stays live)
                    i += 1; continue
                if dst in s.cname:                               # counter init: i = <val>;
                    cn = s.cname[dst]
                    if cn not in s.decl_order:
                        s.decl_order.append(cn)
                    out.append("%s = %s;" % (cn, val)); reg[dst] = cn
                elif src in reg and dst in ("si", "di") and dst not in s.regvars and val == reg.get("_lastcall"):
                    vn = "v%d" % len(s.regvars); s.regvars[dst] = vn; s.decl_order.append(vn)
                    out.append("%s = %s;" % (vn, val))
                    s.args = [vn if a == val else a for a in s.args]
                    reg[dst] = vn; reg.pop("_lastcall", None)
                else:
                    reg[dst] = val
            elif mn in ("add", "sub") and ops.startswith("sp,"):
                pass                                             # stack adjust (cdecl cleanup / locals)
            elif mn in ("cwd", "cdq"):                           # sign-extend ax -> dx:ax (a long)
                reg["dxax"] = "(long)%s" % reg.get("ax", "0")
            elif mn == "nop":
                pass
            elif mn == "sub" and "," in ops and ops.split(", ")[0] == ops.split(", ")[1]:
                dst = ops.split(", ")[0]                          # sub X,X -> X = 0
                if dst in s.cname:                               # counter init: i = 0;
                    cn = s.cname[dst]
                    if cn not in s.decl_order:
                        s.decl_order.append(cn)
                    out.append("%s = 0;" % cn); reg[dst] = cn
                else:
                    reg[dst] = "0"
            elif mn in _ALU2 and "," in ops:                     # general binary ALU -> C expr
                dst, _, src = ops.partition(", ")
                if dst not in _WREG:
                    return None                                  # only word-register dests modeled
                reg[dst] = "(%s %s %s)" % (s._val(dst, reg), _ALU2[mn], s._val(src, reg))
            elif mn in ("neg", "not") and ops in _WREG:          # unary ALU
                reg[ops] = "(%s%s)" % ("-" if mn == "neg" else "~", s._val(ops, reg))
            elif mn == "lea":                                    # load effective address
                dst, _, mem = ops.partition(", ")
                lm = re.match(r"\[bp - (\w+)\]$", mem)
                gm = re.match(r"\[0x([0-9a-f]+)\]$", mem)
                if lm:
                    reg[dst] = "&%s" % s._local(int(lm.group(1), 0))
                elif gm:
                    off = int(gm.group(1), 16); nm = s.syms.get(off)
                    name = nm[0] if nm else "gW_%x" % off
                    s.globs[name] = nm[1] if nm else "word"; reg[dst] = "&%s" % name
                else:
                    return None                                  # indexed lea (arrays) not yet modeled
            else:
                s.fail = "%s %s" % (mn, ops); return None
            i += 1
        # fall-through to the epilogue: an implicit return only at TOP LEVEL of a non-void fn
        final = hi >= len(s.md)
        if reg.get("_lastcall") and reg["_lastcall"] not in "".join(out) and not (final and reg.get("ax") == reg["_lastcall"]):
            out.append(reg["_lastcall"] + ";")               # unconsumed void call
            reg.pop("_lastcall", None)
        if final and blo == 0 and not getattr(s, "void", False):
            if getattr(s, "rtype", None) == "long" and reg.get("dxax"):
                out.append("return %s;" % reg["dxax"])          # long return in dx:ax
            elif reg.get("ax"):
                out.append("return %s;" % reg["ax"])
        return out


def invert(code, imports=None, syms=None, rtype="int", params=None, nearmap=None, func_base=0):
    imports = imports or {}
    b = bytes(code)
    if b.startswith(PROLOGUE):
        p = len(PROLOGUE)
    elif b.startswith(PROLOGUE_MOVDS):                # mov-ax,ds encoding: identical frame after
        p = len(PROLOGUE_MOVDS)                       # the DS load, same length, same epilogue
    else:
        return None, "prologue miss"
    if b[p:p+2] == b"\x83\xec":              # sub sp, imm8  (optimized locals frame)
        p += 3
    elif b[p:p+2] == b"\x81\xec":            # sub sp, imm16 (UNoptimized locals frame -- the
        p += 4                               # 1.52c corpus default: /AM /G2 /Gsw with no /O)
    while b[p:p+1] in (b"\x57", b"\x56"):    # push di / push si (callee-saved register frame)
        p += 1
    ei = b.rfind(EPI); epilen = len(EPI)
    if ei < 0:                                        # lea-variant teardown (mixed-object DLLs)
        ei = b.rfind(EPI_LEA); epilen = len(EPI_LEA)
    if ei < 0:
        return None, "no epilogue"
    be = ei
    while be > p and b[be-1] in (0x5e, 0x5f):
        be -= 1
    md = list(_MD.disasm(b[p:be], p))
    # a `push imm16` whose value is also touched as [imm16] is the ADDRESS of a DGROUP
    # global passed as a near pointer (&gW_x). Collect those offsets so the push handler
    # renders `&gW_x` (the operand is a link-assigned offset, masked in _mask).
    memoff = set()
    for ins in md:
        for gm in re.finditer(r"\[0x([0-9a-f]+)\]", ins.op_str):
            memoff.add(int(gm.group(1), 16))
    # infer far/int params BEFORE building Dec, so the far-param LAYOUT (lanes) is set and
    # the body walk uses the right param names: a `les reg,[bp+off]` marks off far (4 bytes).
    if params is None:
        far_offs = set(); long_offs = set(); accessed = set(); maxo = 4
        for j, ins in enumerate(md):
            if ins.mnemonic in ("les", "lds"):
                lm = re.search(r"\[bp \+ (\w+)\]", ins.op_str)
                if lm:
                    far_offs.add(int(lm.group(1), 0))
            mm = re.match(r"(es|ds), word ptr \[bp \+ (\w+)\]", ins.op_str)
            if ins.mnemonic == "mov" and mm:                # mov es,[bp+M] -> far param at M-2
                far_offs.add(int(mm.group(2), 0) - 2)
            # a 32-bit push of a stack dword (66 ff /6) passes a `long` param by value
            dm = re.match(r"dword ptr \[bp \+ (\w+)\]$", ins.op_str)
            if ins.mnemonic == "push" and dm:
                o = int(dm.group(1), 0); long_offs.add(o); maxo = max(maxo, o + 2)
            # long-param load lanes:  mov ax,[bp+o] ; mov dx,[bp+o+2]
            g = re.match(r"ax, word ptr \[bp \+ (\w+)\]$", ins.op_str)
            if ins.mnemonic == "mov" and g and j + 1 < len(md):
                o = int(g.group(1), 0)
                n2 = re.match(r"dx, word ptr \[bp \+ (\w+)\]$", md[j+1].op_str)
                if md[j+1].mnemonic == "mov" and n2 and int(n2.group(1), 0) == o + 2:
                    long_offs.add(o); maxo = max(maxo, o + 2)
            for mm2 in re.finditer(r"\[bp \+ (\w+)\]", ins.op_str):
                oo = int(mm2.group(1), 0)
                if oo >= 6:
                    accessed.add(oo); maxo = max(maxo, oo)
        if far_offs or long_offs:                          # build a typed list when far/long params exist
            # bound by retf N (exact total param bytes) for pascal -- a hard constraint that
            # beats guessing the count from the max stack offset accessed.
            q0 = ei + epilen
            retf_total = (b[q0+1] | (b[q0+2] << 8)) if b[q0:q0+1] == b"\xca" else None
            limit = 6 + retf_total if retf_total else maxo + 2
            inferred = []; oo = 6
            while oo < limit:
                if oo in far_offs and oo + 4 <= limit:
                    inferred.append(("p", "far")); oo += 4
                elif oo in long_offs and oo + 4 <= limit:
                    inferred.append(("p", "long")); oo += 4
                elif oo in accessed or (retf_total and oo < limit):
                    inferred.append(("p", "int")); oo += 2
                else:
                    oo += 2
            # `inferred` is built low-offset first; _layout_params lays params out pascal
            # (last-declared at bp+6), so decl order is the reverse -- flip it, then name
            # positionally, so each type lands on the stack slot it was detected at.
            params = [("p%d" % k, ty) for k, (_n, ty) in enumerate(reversed(inferred))]
    d = Dec(md, be, imports, syms, params, nearmap=nearmap, func_base=func_base)
    d.memoff = memoff
    d.void = (rtype == "void"); d.rtype = rtype
    d.args = []
    lines = d.walk(0, len(md), {})
    if lines is None:
        return None, "unmodeled: %s" % getattr(d, "fail", "?")
    # calling convention + param count from the return instruction:
    #   ca NN -> pascal, NN/2 words cleaned by callee;  cb/c3 -> cdecl, caller cleans.
    q = ei + epilen
    pascal = True; conv = 0
    if b[q:q+1] == b"\xca":
        conv = (b[q+1] | (b[q+2] << 8)) // 2
    elif b[q:q+1] in (b"\xcb", b"\xc3"):
        pascal = False                                    # cdecl -- param count not in the retf
    # int-param count from the highest [bp + N] read (the far-param case was inferred above)
    ninfer = 0
    for ins in md:
        for mm2 in re.finditer(r"\[bp \+ (\w+)\]", ins.op_str):
            o = int(mm2.group(1), 0)
            if o >= 6:
                ninfer = max(ninfer, (o - 6) // 2 + 1)
    kw = "pascal " if pascal else ""                       # cdecl functions carry no keyword
    if params:                                            # caller gave (or we inferred) a typed sig
        sig = []
        for nm, ty in params:
            if ty == "far":
                sig.append("%s far *%s" % (d.farty.get(nm, "char"), nm))   # width from deref
            elif ty == "long":
                sig.append("long %s" % nm)
            else:
                sig.append("int %s" % nm)
        params = ", ".join(sig) or "void"
    else:                                                 # infer word params a,b,...
        nparam = conv if pascal else ninfer
        if pascal and ninfer > nparam:                    # body reads params the retf didn't clean
            nparam = ninfer
        params = ", ".join("int %s" % chr(ord('a')+i) for i in range(nparam)) or "void"
    seen = set()
    decls = "typedef unsigned int WORD;\n"
    for k in range(d.calls):
        nm = imports.get(k, "G%d" % k)
        if nm not in seen:
            pk = "" if nm in d.cdecl_imports else "pascal "     # cdecl imports: no keyword
            decls += "extern WORD far %s%s();\n" % (pk, nm); seen.add(nm)
    _cty = {"long": "long", "byte": "char"}
    for g, t in d.globs.items():
        decls += "extern %s %s;\n" % (_cty.get(t, "int"), g)
    for snm in d.nears_used:                               # sibling (same-segment) functions
        decls += "extern int far %s();\n" % snm
    regd = "".join("    register int %s;\n" % v for v in d.decl_order)
    locd = "".join("    %s %s;\n" % (t, nm) for nm, t in d.localdecl.items())
    body = regd + locd + "".join("    %s\n" % ln for ln in lines)
    return "%s\n%s far %sF(%s)\n{\n%s}" % (decls, rtype, kw, params, body), \
        {"calls": d.calls, "regvars": d.decl_order, "globals": dict(d.globs),
         "cdecl": not pascal, "has_nearcall": d.has_nearcall,
         "nears": list(d.nears_used)}


def _selftest():
    from engine.inverse_engine import msc6_oracle as ora
    code = bytes.fromhex("1e589045558bec1e8ed883ec0256ff76069a000000008bf00bf675042bc0eb08569a000000008bc65e4d4d8be51f5d4dca020090")
    src, meta = invert(code, imports={0: "G", 1: "H"})
    if src is None:
        print("DECLINED:", meta); return
    print(src)
    rc = ora.mc6_compile(src, "/AM /G2 /Gsw /Ox")
    print("\n// INTEGRATION driver: masked-equal = %s (%d vs %d B)"
          % (_mask(rc) == _mask(code) and len(rc) == len(code), len(rc), len(code)))
    # far-pointer battery: branch, deref load/pass/store (pointee width from usage)
    far = [
        ("fptr-branch", "1e589045558bec1e8ed883ec028b4e068b46080bc175052bc0eb0b908b460850519a000000004d4d8be51f5d4dca0400", "int", [("name", "far")]),
        ("deref-return", "1e589045558bec1e8ed883ec02c45e06268b074d4d8be51f5d4dca040090", "int", [("p", "far")]),
        ("deref-pass", "1e589045558bec1e8ed883ec02c45e0626ff379a000000004d4d8be51f5d4dca0400", "int", [("p", "far")]),
        ("deref-store", "1e589045558bec1e8ed883ec028b4606c45e082689074d4d8be51f5d4dca0600", "void", [("p", "far"), ("v", "int")]),
        ("dword-shl4", "1e589045558bec1e8ed883ec028b46068b5608d1e0d1d2d1e0d1d2d1e0d1d2d1e0d1d24d4d8be51f5d4dca040090", "long", [("a", "long")]),
        ("dword-add", "1e589045558bec1e8ed883ec028b46068b560803460a13560c4d4d8be51f5d4dca080090", "long", [("a", "long"), ("b", "long")]),
        ("dword-frword", "1e589045558bec1e8ed883ec028b56062bc04d4d8be51f5d4dca0200", "long", [("a", "int")]),
        # short-circuit OR-of-ANDs converging on a JOIN: (f==1&&g==0)||(f==2&&g==0)||(f==3&&g==0)
        ("shortcircuit-or-and", "1e589045558bec1e8ed883ec028b4e088b560683f90175040bd2741283f90275040bd2740983f903750c0bd27508519a00000000eb03b80d004d4d8be51f5d4dca040090", "int", [("f", "int"), ("g", "int")]),
        # materialized boolean: MSC builds (a||b) into a 0/1 register then tests it (olecli sub_28aa)
        ("bool-materialize", "1e589045558bec1e8ed883ec02c45e0626837f7f00750b26837f7d0075042bd2eb03ba01000bd274052bc0eb0490b805004d4d8be51f5d4dca040090", "int", [("p", "far")]),
    ]
    for label, hx, rt, pms in far:
        c = bytes.fromhex(hx)
        s2, _ = invert(c, imports={0: "G"}, rtype=rt, params=pms)
        if not s2:
            print("// %s: DECLINED" % label); continue
        r2 = ora.mc6_compile(s2, "/AM /G2 /Gsw /Ox")
        print("// %-12s masked-equal = %s (%d vs %d B)"
              % (label, _mask(r2) == _mask(c) and len(r2) == len(c), len(r2), len(c)))
    # real retail LZEXPAND functions (cdecl + param-inference + cmp-consumes-call);
    # padding-tolerant compare since a standalone recompile appends an alignment nop.
    real = [
        ("retail-cdecl(seg11)", "1e589045558bec1e8ed883ec02568b760cff7606ff760aff7608569a210500003bc67506b8ffffeb03902bc05e4d4d8be51f5d4dcb", {0: "G"}, None),
        ("retail-wstore(seg10)", "1e589045558bec1e8ed883ec02b8ffffa382004d4d8be51f5d4dcb", {}, None),
        ("retail-free(seg11)", "1e589045558bec1e8ed883ec02833e7e00007420ff367e009affff0000ff367e009affff0000c7067e0000002bc0a3ee00a3ec00c606800000" "2bc0a3ee00a3ec00a3f200a3f000a3f600a3f400a3fa00a3f800a3fe00a3fc00a30201a300014d4d8be51f5d4dcb", {0: "G", 1: "H"}, {0xec: ("gL_ec", "long")}),
        ("retail-cdecl2(seg12)", "1e589045558bec1e8ed883ec029affff0000833e8c01007412ff368c019a9b09000083c402c7068c010000833e8a01007412ff368a019affff000083c402c7068a010000833e8201007420ff3682019af4060000ff3682019afd060000c706820100002bc0a39801a396012bc0a38401a386014d4d8be51f5d4dcb", {}, {0x196: ("gL_196", "long"), 0x186: ("gL_186", "long")}),
        ("retail-addrglob(seg10)", "1e589045558bec1e8ed883ec02688203688003ff76069affff000083c4060bc07406b8ffffeb2090ff368203ff368003ff76089affff000083c4060bc07405b8feffeb03b801004d4d8be51f5d4dcb", {0: "G0", 1: "G1"}, None),
    ]
    for label, hx, imp, sy in real:
        c = bytes.fromhex(hx)
        s3, _ = invert(c, imports=imp, syms=sy)
        if not s3:
            print("// %s: DECLINED" % label); continue
        r3 = ora.mc6_compile(s3, "/AM /G2 /Gsw /Ox")
        eq = _mask(r3).rstrip(b"\x90\x00") == _mask(c).rstrip(b"\x90\x00")
        print("// %-20s masked-equal = %s" % (label, eq))


if __name__ == "__main__":
    _selftest()
