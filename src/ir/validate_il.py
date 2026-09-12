#!/usr/bin/env python3
r"""
validate_il.py -- differential validator for il.py's instruction lifting.

THE IDEA (the "verified, not guessed" backbone, adapted to what's on this box):
we have no CPU emulator (no unicorn), so instead of "IR vs real silicon" this runs
"IR vs an INDEPENDENT reference interpreter" -- a second, from-scratch textbook
implementation of each instruction's register/flag effect. For a given instruction
on random 32-bit register inputs:

    ref_state  = ref_step(insn, inputs)          # hand-written reference semantics
    il_state   = ir_interp(_lift32(insn), inputs) # execute il.py's lifted IR

and the two register files must agree. Any disagreement is a lifting bug -- caught
mechanically, not by eye. Two independent implementations agreeing is real evidence.

SCOPE (v1): straight-line register instructions -- mov, add/sub/and/or/xor,
inc/dec, neg/not, shl/sal/shr/sar, movzx/movsx, imul(2/3), rol/ror -- over the
32-bit GPRs and their 8-bit subregisters (al/ah/.../dl/dh). No memory, no control
flow, no 16-bit ax/segment regs yet (those carry known aliasing gaps in il.py's
model; validating them needs those gaps closed first -- flagged, not pretended).

    python validate_il.py            # run the randomized differential suite
    python validate_il.py -n 5000    # more iterations per form

Deterministic: the RNG is seeded, so a failure reproduces exactly.
"""
import random
import sys

sys.path.insert(0, __import__("os").path.dirname(__import__("os").path.abspath(__file__)))
from src.ir import il

M32 = 0xFFFFFFFF
GPR = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp")
BSUB = {"al": ("eax", 0), "ah": ("eax", 8), "bl": ("ebx", 0), "bh": ("ebx", 8),
        "cl": ("ecx", 0), "ch": ("ecx", 8), "dl": ("edx", 0), "dh": ("edx", 8)}
WORD = {"ax": "eax", "bx": "ebx", "cx": "ecx", "dx": "edx",
        "si": "esi", "di": "edi", "bp": "ebp", "sp": "esp"}    # 16-bit = low 16 of parent


# ------------------------------------------------------------------- reference
def _rd(st, name):
    """Read a register operand's *value* from a reference state."""
    if name in GPR:
        return st[name] & M32
    if name in BSUB:
        base, sh = BSUB[name]
        return (st[base] >> sh) & 0xFF
    if name in WORD:
        return st[WORD[name]] & 0xFFFF
    raise KeyError(name)


def _wr(st, name, val):
    """Write a register operand, honouring 8-bit / 16-bit subregister merges."""
    if name in GPR:
        st[name] = val & M32
    elif name in BSUB:
        base, sh = BSUB[name]
        st[base] = (st[base] & ~(0xFF << sh) | ((val & 0xFF) << sh)) & M32
    elif name in WORD:
        base = WORD[name]
        st[base] = (st[base] & 0xFFFF0000 | (val & 0xFFFF)) & M32
    else:
        raise KeyError(name)


def _val(st, tok):
    """A source operand: a register or an immediate."""
    tok = tok.strip().lower()
    if tok in GPR or tok in BSUB or tok in WORD:
        return _rd(st, tok)
    if tok.endswith("h"):
        return int(tok[:-1], 16) & M32
    return int(tok, 0) & M32


def _width(tok):
    t = tok.strip().lower()
    if t in BSUB:
        return 8
    if t in WORD:
        return 16
    return 32


def _ovf(d, w):
    return d > (1 << (w - 1)) - 1 or d < -(1 << (w - 1))


def _rflags(st, res, a, b, op, w):
    """Independent flag semantics (ZF/SF/PF always; CF/OF per op)."""
    m = (1 << w) - 1
    st["ZF"] = 1 if (res & m) == 0 else 0
    st["SF"] = (res >> (w - 1)) & 1
    st["PF"] = 1 - (bin(res & 0xFF).count("1") & 1)
    if op == "add":
        st["CF"] = (((a & m) + (b & m)) >> w) & 1
        st["OF"] = 1 if _ovf(_sgn_w(a, w) + _sgn_w(b, w), w) else 0
    elif op in ("sub", "neg"):
        st["CF"] = 1 if (a & m) < (b & m) else 0
        st["OF"] = 1 if _ovf(_sgn_w(a, w) - _sgn_w(b, w), w) else 0
    elif op in ("and", "or", "xor"):
        st["CF"] = 0
        st["OF"] = 0
    elif op == "inc":                                # CF preserved
        st["OF"] = 1 if _ovf(_sgn_w(a, w) + _sgn_w(b, w), w) else 0
    elif op == "dec":
        st["OF"] = 1 if _ovf(_sgn_w(a, w) - _sgn_w(b, w), w) else 0


def ref_step(insn, st):
    """Independent textbook execution of ONE instruction -> new state (copy)."""
    st = dict(st)
    parts = insn.replace(",", " ").split()
    op, args = parts[0], parts[1:]
    if op == "mov":
        _wr(st, args[0], _val(st, args[1]))          # mov: no flag change
    elif op in ("add", "sub", "and", "or", "xor"):
        w = _width(args[0])
        a, b = _rd(st, args[0]), _val(st, args[1])
        r = {"add": a + b, "sub": a - b, "and": a & b,
             "or": a | b, "xor": a ^ b}[op]
        _wr(st, args[0], r)
        _rflags(st, r, a, b, op, w)
    elif op in ("inc", "dec"):
        w = _width(args[0])
        a = _rd(st, args[0])
        r = a + (1 if op == "inc" else -1)
        _wr(st, args[0], r)
        _rflags(st, r, a, 1, op, w)
    elif op == "neg":
        w = _width(args[0])
        a = _rd(st, args[0])
        _wr(st, args[0], -a)
        _rflags(st, -a, 0, a, "neg", w)
    elif op == "not":
        _wr(st, args[0], ~_rd(st, args[0]))          # not: no flag change
    elif op in ("adc", "sbb"):
        w = _width(args[0])
        a, b, cf = _rd(st, args[0]), _val(st, args[1]), st.get("CF", 0)
        mm = (1 << w) - 1
        if op == "adc":
            full = (a & mm) + (b & mm) + cf
            st["CF"] = (full >> w) & 1
            st["OF"] = 1 if _ovf(_sgn_w(a, w) + _sgn_w(b, w) + cf, w) else 0
        else:
            full = (a & mm) - (b & mm) - cf
            st["CF"] = 1 if full < 0 else 0
            st["OF"] = 1 if _ovf(_sgn_w(a, w) - _sgn_w(b, w) - cf, w) else 0
        r = full & mm
        _wr(st, args[0], r)
        st["ZF"] = 1 if r == 0 else 0
        st["SF"] = (r >> (w - 1)) & 1
        st["PF"] = 1 - (bin(r & 0xFF).count("1") & 1)
    elif op in ("shl", "sal", "shr", "sar"):
        a, n = _rd(st, args[0]), _val(st, args[1]) & 0xFF
        if op in ("shl", "sal"):
            r = a << n
        elif op == "shr":
            r = a >> n
        else:                                    # sar: arithmetic (sign-extended)
            w = _width(args[0])
            sign = a >> (w - 1) & 1
            r = a >> n | (((1 << n) - 1) << (w - n) if sign and n else 0)
        _wr(st, args[0], r)
    elif op == "movzx":
        _wr(st, args[0], _val(st, args[1]))
    elif op == "movsx":
        v, w = _val(st, args[1]), _width(args[1])
        sign = 1 << (w - 1)
        _wr(st, args[0], (v ^ sign) - sign)
    elif op == "imul":
        if len(args) == 2:
            _wr(st, args[0], _rd(st, args[0]) * _val(st, args[1]))
        else:
            _wr(st, args[0], _val(st, args[1]) * _val(st, args[2]))
    elif op in ("rol", "ror"):
        a, n = _rd(st, args[0]), _val(st, args[1]) & 31
        r = ((a << n) | (a >> (32 - n))) if op == "rol" else ((a >> n) | (a << (32 - n)))
        _wr(st, args[0], r)
    elif op == "cdq":                                # edx = sign-extend(eax) high half
        st["edx"] = M32 if (_rd(st, "eax") >> 31) & 1 else 0
    elif op == "cwde":                               # eax = sign-extend(ax)
        ax = _rd(st, "eax") & 0xFFFF
        _wr(st, "eax", (ax ^ 0x8000) - 0x8000)
    elif op == "cwd":                                # dx = sign-extend(ax) high half
        ax = _rd(st, "eax") & 0xFFFF
        _wr(st, "dx", ((((ax ^ 0x8000) - 0x8000) & M32) >> 16) & 0xFFFF)
    elif op == "cbw":                                # ax = sign-extend(al)
        al = _rd(st, "eax") & 0xFF
        _wr(st, "ax", (al ^ 0x80) - 0x80)
    elif op == "xchg":
        t = _rd(st, args[0])
        _wr(st, args[0], _rd(st, args[1]))
        _wr(st, args[1], t)
    else:
        raise NotImplementedError(op)
    return st


# ------------------------------------------------------------------ IR interp
def memfn(addr):
    """Deterministic, address-sensitive default byte (so a wrong address reads a
    different value -> address bugs surface as value mismatches)."""
    addr &= M32
    return ((addr * 2654435761) ^ (addr >> 15) ^ 0x9E) & 0xFF


def mem_read(mem, addr, width):
    v = 0
    for i in range(width // 8):
        b = mem.get((addr + i) & M32)
        v |= (memfn(addr + i) if b is None else b) << (8 * i)
    return v & M32


def mem_write(mem, addr, width, val):
    for i in range(width // 8):
        mem[(addr + i) & M32] = (val >> (8 * i)) & 0xFF


def ir_eval(node, st):
    T = type(node).__name__
    if T == "Const":
        return node.val & M32
    if T == "Reg":
        return st.get(node.name, 0) & M32
    if T == "Mem":
        return mem_read(st.get("mem") or {}, ir_eval(node.addr, st), node.width)
    if T == "Flag":
        return st.get(node.name, 0) & 1
    if T == "Un":
        a = ir_eval(node.a, st)
        if node.op == "parity":
            return 1 - (bin(a & 0xFF).count("1") & 1)     # PF: 1 if even # of low-byte 1s
        if node.op == "!":
            return 0 if a else 1
        return {"-": -a, "~": ~a}[node.op] & ((1 << node.width) - 1)
    if T == "Bin":
        a, b = ir_eval(node.a, st), ir_eval(node.b, st)
        o, w = node.op, node.width
        m = (1 << w) - 1
        if o == "+": r = a + b
        elif o == "-": r = a - b
        elif o == "*": r = a * b
        elif o == "&": r = a & b
        elif o == "|": r = a | b
        elif o == "^": r = a ^ b
        elif o == "<<": r = a << (b & 0xFF)
        elif o == ">>": r = a >> (b & 0xFF)
        elif o == ">>s":                                 # arithmetic (signed) shift right
            n = b & 0xFF
            r = a >> n
            if (a >> 31) & 1 and n:
                r |= ((1 << n) - 1) << (32 - n)
        elif o == "==": return 1 if a == b else 0
        elif o == "ucarry": return (((a & m) + (b & m)) >> w) & 1
        elif o == "borrow": return 1 if (a & m) < (b & m) else 0
        elif o == "sovf_add":
            d = _sgn_w(a, w) + _sgn_w(b, w)
            return 1 if (d > (1 << (w - 1)) - 1 or d < -(1 << (w - 1))) else 0
        elif o == "sovf_sub":
            d = _sgn_w(a, w) - _sgn_w(b, w)
            return 1 if (d > (1 << (w - 1)) - 1 or d < -(1 << (w - 1))) else 0
        else:
            raise NotImplementedError("Bin op %r" % o)
        return r & m                                      # mask to the node's own width (16/32)
    raise NotImplementedError(T)


def _sgn_w(x, w):
    x &= (1 << w) - 1
    return x - (1 << w) if x & (1 << (w - 1)) else x


def ir_interp(stmts, st):
    """Execute lifted IR Set-statements (flags/branches ignored) -> new state."""
    st = dict(st)
    st["mem"] = dict(st.get("mem") or {})
    for s in stmts:
        if type(s).__name__ != "Set":
            continue
        if type(s.dst).__name__ == "Reg":
            st[s.dst.name] = ir_eval(s.expr, st) & M32
        elif type(s.dst).__name__ == "Flag":
            st[s.dst.name] = ir_eval(s.expr, st) & 1
        elif type(s.dst).__name__ == "Mem":
            mem_write(st["mem"], ir_eval(s.dst.addr, st), s.dst.width, ir_eval(s.expr, st))
    return st


# --------------------------------------------------------------------- driver
FORMS = [
    "mov {r}, {r2}", "mov {r}, {imm}", "mov {b}, {b2}", "mov {b}, {imm8}",
    "add {r}, {r2}", "sub {r}, {r2}", "and {r}, {r2}", "or {r}, {r2}",
    "xor {r}, {r2}", "add {b}, {imm8}", "add {b}, {b2}",
    "inc {r}", "dec {r}", "neg {r}", "not {r}", "inc {b}",
    "shl {r}, {sh}", "shr {r}, {sh}", "sar {r}, {sh}", "sal {r}, {sh}",
    "movzx {r}, {b}", "movsx {r}, {b}",
    "imul {r}, {r2}", "imul {r}, {r2}, {imm8}",
    "rol {r}, {sh}", "ror {r}, {sh}",
    "adc {r}, {r2}", "sbb {r}, {r2}", "adc {r}, {imm}", "adc {b}, {b2}",
    # 16-bit register forms (aliasing: ax = low 16 of eax)
    "mov {w}, {w2}", "mov {w}, {imm16}", "add {w}, {w2}", "sub {w}, {w2}",
    "and {w}, {w2}", "or {w}, {w2}", "xor {w}, {w2}", "inc {w}", "dec {w}",
    "neg {w}", "not {w}", "movzx {r}, {w}", "movsx {r}, {w}",
    # sign-extend accumulators + exchange (deterministic register transforms)
    "cdq", "cwde", "cwd", "cbw", "xchg {r}, {r2}", "xchg {w}, {w2}",
]


def _fill(form, rnd):
    return form.format(
        r=rnd.choice(GPR), r2=rnd.choice(GPR),
        b=rnd.choice(list(BSUB)), b2=rnd.choice(list(BSUB)),
        w=rnd.choice(list(WORD)), w2=rnd.choice(list(WORD)),
        imm="%xh" % rnd.randint(0, M32), imm8="%xh" % rnd.randint(0, 255),
        imm16="%xh" % rnd.randint(0, 0xFFFF), sh=rnd.randint(0, 31))


def _ea(st, inner):
    """Independent effective-address computation for the inside of [ ]."""
    total = 0
    for term in __import__("re").findall(r"[+\-]?[^+\-]+", inner.replace(" ", "")):
        neg = term.startswith("-")
        term = term.lstrip("+-")
        msc = __import__("re").match(r"^(e[a-z][a-z])\*([1248])$", term)
        if msc:
            v = _rd(st, msc.group(1)) * int(msc.group(2))
        elif term in GPR:
            v = _rd(st, term)
        else:
            v = int(term[:-1], 16) if term.endswith("h") else int(term, 0)
        total += -v if neg else v
    return total & M32


EA_FORMS = ["[{r}]", "[{r}+{d}]", "[{r}-{d}]", "[{r}+{x}*{sc}]",
            "[{r}+{x}*{sc}+{d}]", "[{x}*{sc}+{d}]", "[{d}]", "[{r}+{x}]"]
EA_BASE = ("eax", "ebx", "ecx", "edx", "esi", "edi")   # skip esp/ebp (arg-slot model)


def _check_addresses(n, rnd):
    total = mism = 0
    fails = []
    for form in EA_FORMS:
        for _ in range(n):
            inner = form.format(r=rnd.choice(EA_BASE), x=rnd.choice(EA_BASE),
                                sc=rnd.choice((1, 2, 4, 8)),
                                d="%xh" % rnd.randint(0, 0xFFFF))[1:-1]
            st = {g: rnd.randint(0, M32) for g in GPR}
            node, _w = il._operand("dword ptr [%s]" % inner, {})
            if type(node).__name__ != "Mem":
                continue
            total += 1
            il_addr = ir_eval(node.addr, st)
            ref_addr = _ea(st, inner)
            if il_addr != ref_addr:
                mism += 1
                if len(fails) < 10:
                    fails.append((inner, ref_addr, il_addr))
    print("\n  effective-address computation (scaled-index etc.):")
    print("    checked: %d   AGREE: %d (%.2f%%)   MISMATCH: %d"
          % (total, total - mism, 100 * (total - mism) / max(1, total), mism))
    for inner, r, i in fails:
        print("    [%-22s] ref %08x / il %08x" % (inner, r, i))
    return mism


def _split_operands(rest):
    out, depth, cur = [], 0, ""
    for ch in rest:
        if ch == "[":
            depth += 1
        elif ch == "]":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def _memtok(tok):
    import re
    m = re.match(r"(dword|word|byte) ptr \[(.+)\]$", tok.strip())
    return ({"dword": 32, "word": 16, "byte": 8}[m.group(1)], m.group(2)) if m else None


def _ref_mem_step(st, insn):
    """Independent execution of a mov/ALU that may touch memory -> new state."""
    st = dict(st)
    st["mem"] = dict(st.get("mem") or {})
    op, _, rest = insn.partition(" ")
    args = _split_operands(rest)

    def rd(tok):
        mt = _memtok(tok)
        return mem_read(st["mem"], _ea(st, mt[1]), mt[0]) if mt else _val(st, tok)

    def wr(tok, val):
        mt = _memtok(tok)
        if mt:
            mem_write(st["mem"], _ea(st, mt[1]), mt[0], val)
        else:
            _wr(st, tok, val)

    if op == "mov":
        wr(args[0], rd(args[1]))
    elif op in ("add", "sub", "and", "or", "xor"):
        a, b = rd(args[0]), rd(args[1])
        wr(args[0], {"add": a + b, "sub": a - b, "and": a & b,
                     "or": a | b, "xor": a ^ b}[op])
    else:
        return None
    return st


def _check_memory(n, rnd):
    reg6 = ("eax", "ebx", "ecx", "edx", "esi", "edi")
    forms = ["mov {r}, dword ptr [{b}]", "mov {r}, dword ptr [{b}+{d}]",
             "mov {r}, word ptr [{b}]", "mov {r}, byte ptr [{b}+{x}*4]",
             "add {r}, dword ptr [{b}+{d}]", "xor {r}, dword ptr [{b}]",
             "mov dword ptr [{b}], {r}", "mov dword ptr [{b}+{d}], {r}",
             "mov word ptr [{b}], {r}", "mov byte ptr [{b}], {r}"]
    tot = mism = 0
    fails = []
    for form in forms:
        for _ in range(n):
            insn = form.format(r=rnd.choice(reg6), b=rnd.choice(reg6),
                               x=rnd.choice(reg6), d="%xh" % rnd.randint(0, 0xFFFF))
            st = {g: rnd.randint(0, M32) for g in GPR}
            st["mem"] = {}
            ref = _ref_mem_step(st, insn)
            if ref is None:
                continue
            il_st = ir_interp(il._lift32(insn, {}, [None]), st)
            tot += 1
            reg_ok = all(ref.get(g, st[g]) == il_st.get(g, st[g]) for g in GPR)
            mem_ok = ref["mem"] == il_st.get("mem", {})
            if not (reg_ok and mem_ok):
                mism += 1
                if len(fails) < 8:
                    fails.append((insn, "reg" if not reg_ok else "mem"))
    print("\n  memory loads/stores (value + width, vs independent execution):")
    print("    checked: %d   AGREE: %d (%.2f%%)   MISMATCH: %d"
          % (tot, tot - mism, 100 * (tot - mism) / max(1, tot), mism))
    for insn, kind in fails:
        print("    %-34s (%s mismatch)" % (insn, kind))
    return mism


def _sgn(x):
    return x - (1 << 32) if x >> 31 else x


def _ref_jcc(op, a, b):
    """Textbook: is jCC taken after `cmp a, b`? (flag semantics, independent)."""
    res = (a - b) & M32
    ZF, SF, CF = res == 0, (res >> 31) & 1, 1 if a < b else 0
    d = _sgn(a) - _sgn(b)
    OF = 1 if (d > 0x7FFFFFFF or d < -0x80000000) else 0
    t = {"je": ZF, "jz": ZF, "jne": not ZF, "jnz": not ZF,
         "jl": SF != OF, "jnge": SF != OF, "jge": SF == OF, "jnl": SF == OF,
         "jle": ZF or SF != OF, "jng": ZF or SF != OF,
         "jg": (not ZF) and SF == OF, "jnle": (not ZF) and SF == OF,
         "jb": CF, "jc": CF, "jnae": CF, "jae": not CF, "jnb": not CF, "jnc": not CF,
         "jbe": CF or ZF, "jna": CF or ZF, "ja": (not CF) and (not ZF),
         "jnbe": (not CF) and (not ZF), "js": SF, "jns": not SF,
         "jo": OF, "jno": not OF}
    return bool(t[op]) if op in t else None


def _ceval(c, a, b):
    """Evaluate a _cc_to_c() condition string on concrete (eax, ebx) values."""
    def S(x):
        x &= M32
        return x - (1 << 32) if x & 0x80000000 else x

    def ovf(x, y):
        d = S(x) - S(y)
        return 1 if (d > 0x7FFFFFFF or d < -0x80000000) else 0

    import re
    s = c.replace("!=", "\x00")                          # protect != from the ! swap
    s = re.sub(r"\(i32\)(\([^()]*\)|\w+)", r"S(\1)", s)
    s = re.sub(r"\(i64\)(\([^()]*\)|\w+)", r"S(\1)", s)
    s = s.replace("__ovf_sub", "ovf").replace("!", " not ").replace("\x00", "!=")
    return bool(eval(s, {"__builtins__": {}},
                     {"eax": a, "ebx": b, "S": S, "ovf": ovf}))


def _check_conditions(n, rnd):
    """Validate the REAL condition builder (_cc_to_c) over every COND_JUMP: it must
    model each (no '1 /* not modelled */') and match the textbook flag semantics."""
    from src.ir.il import Reg
    EAX, EBX = Reg("eax", 32), Reg("ebx", 32)
    tot = mism = 0
    fails = []
    unmodelled = [mn for mn in sorted(il.COND_JUMPS)
                  if "not modelled" in il._cc_to_c(il.CANON.get(mn, mn), EAX, EBX, "cmp")]
    for mn in sorted(il.COND_JUMPS):
        canon = il.CANON.get(mn, mn)
        cstr = il._cc_to_c(canon, EAX, EBX, "cmp")
        if "not modelled" in cstr:
            continue
        for _ in range(n * 2):
            a, b = rnd.randint(0, M32), rnd.randint(0, M32)
            if rnd.random() < 0.2:
                b = a
            ref = _ref_jcc(canon, a, b)
            if ref is None:
                continue
            tot += 1
            if bool(_ceval(cstr, a, b)) != ref:
                mism += 1
                if len(fails) < 8:
                    fails.append((mn, a, b, ref, cstr))
    print("\n  jcc condition builder (_cc_to_c, evaluated vs flag semantics):")
    print("    jumps: %d   unmodelled: %d %s" % (len(il.COND_JUMPS), len(unmodelled),
                                                 unmodelled or ""))
    print("    checked: %d   AGREE: %d (%.2f%%)   MISMATCH: %d"
          % (tot, tot - mism, 100 * (tot - mism) / max(1, tot), mism))
    for mn, a, b, r, c in fails:
        print("    %-5s a=%08x b=%08x ref=%s  [%s]" % (mn, a, b, r, c))
    return mism + len(unmodelled)


_JCCSET = ("je", "jz", "jne", "jnz", "jl", "jnge", "jge", "jnl", "jle", "jng",
           "jg", "jnle", "jb", "jc", "jae", "jnc", "jbe", "ja", "js", "jns", "jo", "jno")


def _jcc_taken(op, st):
    ZF, SF, CF, OF = st["ZF"], st["SF"], st["CF"], st["OF"]
    t = {"je": ZF, "jz": ZF, "jne": not ZF, "jnz": not ZF,
         "jl": SF != OF, "jnge": SF != OF, "jge": SF == OF, "jnl": SF == OF,
         "jle": ZF or SF != OF, "jng": ZF or SF != OF,
         "jg": (not ZF) and SF == OF, "jnle": (not ZF) and SF == OF,
         "jb": CF, "jc": CF, "jae": not CF, "jnc": not CF,
         "jbe": CF or ZF, "ja": (not CF) and (not ZF),
         "js": SF, "jns": not SF, "jo": OF, "jno": not OF}
    return bool(t[op])


def _ref_full(ln, st):
    """Reference execution of one line incl. cmp/test (sets flags, no reg write)."""
    op = ln.split()[0]
    if op in ("cmp", "test"):
        st = dict(st)
        args = ln.replace(",", " ").split()[1:]
        w = _width(args[0])
        a, b = _val(st, args[0]), _val(st, args[1])
        if op == "cmp":
            _rflags(st, (a - b) & M32, a, b, "sub", w)
        else:
            _rflags(st, (a & b) & M32, a, b, "and", w)
        return st
    return ref_step(ln, st)


def _run_fn(lines, st, use_il):
    labels = {ln.strip()[:-1]: i for i, ln in enumerate(lines) if ln.strip().endswith(":")}
    pc = steps = 0
    while pc < len(lines) and steps < 2000:
        steps += 1
        ln = lines[pc].strip()
        if not ln or ln.endswith(":"):
            pc += 1
            continue
        op = ln.split()[0]
        if op == "ret":
            break
        if op in _JCCSET:
            pc = labels[ln.split()[-1]] if _jcc_taken(op, st) else pc + 1
            continue
        st = ir_interp(il._lift32(ln, {}, [None]), st) if use_il else _ref_full(ln, st)
        pc += 1
    return st


def _gen_fn(rnd):
    """A small forward-branching function: ALU/mov blocks separated by cmp+jcc to
    forward labels, ending in ret. No back-edges -> guaranteed to terminate."""
    reg = ("eax", "ebx", "ecx", "edx")
    alu = ["add", "sub", "and", "or", "xor"]
    jcc = ["je", "jne", "jl", "jle", "jg", "jge", "jb", "jbe", "ja", "jae", "js", "jns"]

    def instr():
        o = rnd.choice(alu + ["mov", "inc", "dec", "neg", "adc", "sbb"])
        if o in ("inc", "dec", "neg"):
            return "%s %s" % (o, rnd.choice(reg))
        return "%s %s, %s" % (o, rnd.choice(reg), rnd.choice(reg))

    nb = rnd.randint(2, 4)
    body = []
    for bi in range(nb - 1):
        for _ in range(rnd.randint(1, 3)):
            body.append(instr())
        body.append("cmp %s, %s" % (rnd.choice(reg), rnd.choice(reg)))
        body.append("%s L%d" % (rnd.choice(jcc), bi))
    tail = []
    for bi in range(nb - 1):
        tail.append("L%d:" % bi)
        tail.append(instr())
    tail.append("ret")
    return body + tail


def _check_functions(n, rnd):
    reg = ("eax", "ebx", "ecx", "edx")
    tot = mism = 0
    fails = []
    for _ in range(n):
        lines = _gen_fn(rnd)
        st0 = {g: rnd.randint(0, M32) for g in GPR}
        for f in ("ZF", "SF", "CF", "OF", "PF"):
            st0[f] = rnd.randint(0, 1)
        st0["mem"] = {}
        try:
            ref = _run_fn(lines, dict(st0), False)
            ilr = _run_fn(lines, dict(st0), True)
        except Exception as e:
            mism += 1
            if len(fails) < 6:
                fails.append(("; ".join(lines), "EXC:" + str(e)[:50]))
            continue
        tot += 1
        bad = [g for g in reg if ref.get(g) != ilr.get(g)]
        if bad:
            mism += 1
            if len(fails) < 6:
                fails.append(("; ".join(lines), ",".join(bad)))
    print("\n  whole-function control flow (lifted IR vs raw asm, end-to-end):")
    print("    functions: %d   AGREE: %d (%.2f%%)   MISMATCH: %d"
          % (tot, tot - mism, 100 * (tot - mism) / max(1, tot), mism))
    for prog, why in fails:
        print("    [%s]  (%s)" % (prog[:90], why))
    return mism


# ------------------------------------------------------- 16-bit lifting (_lift16)
GPR16 = ("ax", "bx", "cx", "dx", "si", "di")               # independent 16-bit regs


def _sgn16(x):
    x &= 0xFFFF
    return x - 0x10000 if x & 0x8000 else x


def _ovf16(d):
    return d > 0x7FFF or d < -0x8000


def _rflags16(st, res, a, b, op):
    m = 0xFFFF
    st["ZF"] = 1 if (res & m) == 0 else 0
    st["SF"] = (res >> 15) & 1
    st["PF"] = 1 - (bin(res & 0xFF).count("1") & 1)
    if op == "add":
        st["CF"] = (((a & m) + (b & m)) >> 16) & 1
        st["OF"] = 1 if _ovf16(_sgn16(a) + _sgn16(b)) else 0
    elif op == "sub":
        st["CF"] = 1 if (a & m) < (b & m) else 0
        st["OF"] = 1 if _ovf16(_sgn16(a) - _sgn16(b)) else 0
    elif op in ("and", "or", "xor"):
        st["CF"] = st["OF"] = 0
    elif op == "inc":
        st["OF"] = 1 if _ovf16(_sgn16(a) + _sgn16(b)) else 0
    elif op == "dec":
        st["OF"] = 1 if _ovf16(_sgn16(a) - _sgn16(b)) else 0


def _rd16(st, tok):
    tok = tok.strip().lower()
    if tok in GPR16:
        return st[tok] & 0xFFFF
    return (int(tok[:-1], 16) if tok.endswith("h") else int(tok, 0)) & 0xFFFF


def _ref16(insn, st):
    """Independent textbook execution of one 16-bit instruction (regs mask 0xffff)."""
    st = dict(st)
    parts = insn.replace(",", " ").split()
    op, args = parts[0], parts[1:]
    if op == "mov":
        st[args[0]] = _rd16(st, args[1])
    elif op in ("add", "sub", "and", "or", "xor"):
        a, b = st[args[0]] & 0xFFFF, _rd16(st, args[1])
        r = {"add": a + b, "sub": a - b, "and": a & b, "or": a | b, "xor": a ^ b}[op]
        st[args[0]] = r & 0xFFFF
        _rflags16(st, r, a, b, op)
    elif op == "cmp":
        a, b = st[args[0]] & 0xFFFF, _rd16(st, args[1])
        _rflags16(st, a - b, a, b, "sub")
    elif op in ("inc", "dec"):
        a = st[args[0]] & 0xFFFF
        r = a + (1 if op == "inc" else -1)
        st[args[0]] = r & 0xFFFF
        _rflags16(st, r, a, 1, op)
    elif op == "neg":
        a = st[args[0]] & 0xFFFF
        st[args[0]] = (-a) & 0xFFFF
        _rflags16(st, (-a) & 0xFFFF, 0, a, "sub")
    elif op == "not":
        st[args[0]] = (~st[args[0]]) & 0xFFFF
    elif op in ("adc", "sbb"):
        a, b, cf = st[args[0]] & 0xFFFF, _rd16(st, args[1]), st.get("CF", 0)
        full = (a + b + cf) if op == "adc" else (a - b - cf)
        st[args[0]] = full & 0xFFFF
        st["ZF"] = 1 if (full & 0xFFFF) == 0 else 0
        st["SF"] = (full >> 15) & 1
        st["PF"] = 1 - (bin(full & 0xFF).count("1") & 1)
        st["CF"] = ((full >> 16) & 1) if op == "adc" else (1 if full < 0 else 0)
        d = (_sgn16(a) + _sgn16(b) + cf) if op == "adc" else (_sgn16(a) - _sgn16(b) - cf)
        st["OF"] = 1 if _ovf16(d) else 0
    elif op == "xchg":
        st[args[0]], st[args[1]] = _rd16(st, args[1]), st[args[0]] & 0xFFFF
    else:
        raise NotImplementedError(op)
    return st


def _check_16bit(n, rnd):
    forms = ["mov {r}, {r2}", "mov {r}, {imm}", "add {r}, {r2}", "sub {r}, {r2}",
             "and {r}, {r2}", "or {r}, {r2}", "xor {r}, {r2}", "cmp {r}, {r2}",
             "add {r}, {imm}", "sub {r}, {imm}", "inc {r}", "dec {r}", "neg {r}", "not {r}",
             "adc {r}, {r2}", "sbb {r}, {r2}", "adc {r}, {imm}", "xchg {r}, {r2}"]
    FLAG_EXACT16 = {"add", "sub", "and", "or", "xor", "inc", "dec", "neg", "mov", "cmp",
                    "adc", "sbb"}
    FLAGS = ("ZF", "SF", "CF", "OF", "PF")
    tot = mism = 0
    fails = []
    def imm16():                                          # MASM: a hex literal starting with
        h = "%xh" % rnd.randint(0, 0xFFFF)                # a letter needs a leading 0 (else
        return ("0" + h) if h[0].isalpha() else h         # `dh` is the register, not 0x0d)
    for form in forms:
        for _ in range(n):
            insn = form.format(r=rnd.choice(GPR16), r2=rnd.choice(GPR16), imm=imm16())
            st = {g: rnd.randint(0, 0xFFFF) for g in GPR16}
            for f in FLAGS:
                st[f] = rnd.randint(0, 1)
            try:
                ref = _ref16(insn, st)
            except NotImplementedError:
                continue
            ir = ir_interp(il._lift16(insn, {}, [None]), st)
            tot += 1
            bad = [g for g in GPR16 if ref[g] != (ir.get(g, st[g]) & 0xFFFF)]
            if insn.split()[0] in FLAG_EXACT16:
                bad += ["/" + f for f in FLAGS if ref.get(f, st[f]) != ir.get(f, st[f])]
            if bad:
                mism += 1
                if len(fails) < 8:
                    fails.append((insn, bad))
    print("\n  16-bit lifting (_lift16 vs independent 16-bit reference):")
    print("    checked: %d   AGREE: %d (%.2f%%)   MISMATCH: %d"
          % (tot, tot - mism, 100 * (tot - mism) / max(1, tot), mism))
    for insn, bad in fails:
        print("    %-24s %s" % (insn, bad))
    return mism


def main():
    n = int(sys.argv[sys.argv.index("-n") + 1]) if "-n" in sys.argv else 2000
    rnd = random.Random(0xC0FFEE)                # seeded: failures reproduce
    total = mism = skipped = 0
    fails = []
    FLAG_EXACT = {"add", "sub", "and", "or", "xor", "inc", "dec", "neg", "not",
                  "mov", "movzx", "movsx", "adc", "sbb"}   # ops whose flags il claims exactly
    FLAGS = ("ZF", "SF", "CF", "OF", "PF")
    for form in FORMS:
        for _ in range(n):
            insn = _fill(form, rnd)
            state = {g: rnd.randint(0, M32) for g in GPR}
            for fl in FLAGS:
                state[fl] = rnd.randint(0, 1)         # seed flags so 'preserved' is testable
            try:
                ref = ref_step(insn, state)
            except NotImplementedError:
                skipped += 1
                continue
            ir = ir_interp(il._lift32(insn, {}, [None]), state)
            total += 1
            bad = [g for g in GPR if ref[g] != ir.get(g, state[g])]
            if insn.split()[0] in FLAG_EXACT:
                bad += ["/" + f for f in FLAGS if ref.get(f, state[f]) != ir.get(f, state[f])]
            if bad:
                mism += 1
                if len(fails) < 12:
                    fails.append((insn, {g: (state.get(g), ref.get(g), ir.get(g)) for g in bad}))
    print("=" * 68)
    print("il.py differential validation  (IR vs independent reference)")
    print("=" * 68)
    print("  instruction forms: %d   iterations/form: %d" % (len(FORMS), n))
    print("  checked:  %d" % total)
    print("  AGREE:    %d  (%.2f%%)" % (total - mism, 100 * (total - mism) / max(1, total)))
    print("  MISMATCH: %d" % mism)
    if fails:
        print("\n  first mismatches (reg: input -> ref / il):")
        def _h(x):
            return "None" if x is None else ("%08x" % x if x > 1 else str(x))
        for insn, regs in fails:
            print("    %-26s %s" % (insn, "  ".join(
                "%s: %s -> ref %s / il %s" % (g, _h(i), _h(r), _h(v))
                for g, (i, r, v) in regs.items())))
    addr_mism = _check_addresses(n, rnd)
    mem_mism = _check_memory(n, rnd)
    cond_mism = _check_conditions(n, rnd)
    fn_mism = _check_functions(n, rnd)
    bit16_mism = _check_16bit(n, rnd)
    bad = mism + addr_mism + mem_mism + cond_mism + fn_mism + bit16_mism
    print("\n%s" % ("ALL AGREE — lifting matches the reference." if not bad
                    else "DISAGREEMENTS FOUND — see above (a lifting bug or a ref bug)."))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
