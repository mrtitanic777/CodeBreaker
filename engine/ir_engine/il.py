#!/usr/bin/env python3
r"""
il.py -- CodeBreaker Intermediate Language: the IR engine that sits between our
byte-exact disassembly and clean C/C++.

Unlike lift.py (which transliterates each asm STRING straight to a C STRING), il.py
lifts each instruction into a small, typed, register-transfer IR in which every
implicit side effect -- above all the x86 condition flags -- is made EXPLICIT. That
explicitness is what makes analysis possible: once `xor eax,eax` is spelled out as
`eax=0; ZF=1; SF=0; CF=0; OF=0; PF=...`, a liveness pass can prove the flag writes
are never read and delete them, leaving `eax = 0` -- the textbook decompiler result.

Pipeline:  lift_block(asm[])  ->  [Stmt]         (explicit-flag IR)
           run_passes([Stmt]) ->  [Stmt]         (dead-flag elim, const/copy prop, DSE)
           emit_c([Stmt])     ->  C source

This is the engine core (v0): straight-line semantics for the common x86 subset, the
four marquee passes, and a C emitter, with a self-test that reproduces the canonical
`xor eax,eax -> eax = 0` collapse. CFG/SSA structuring and the differential validator
(IR interpreter vs. real semantics) build on this; see run_passes()'s TODO markers.

    python il.py --selftest
"""
import re
import sys

# ---------------------------------------------------------------- IR value model
FLAGS = ("ZF", "SF", "CF", "OF", "PF")

# full 16-bit register set of the era; subreg (al/ah) handled at lift time
WREGS = ("ax", "bx", "cx", "dx", "si", "di", "bp", "sp")
BREGS = {"al": ("ax", 0), "ah": ("ax", 8), "bl": ("bx", 0), "bh": ("bx", 8),
         "cl": ("cx", 0), "ch": ("cx", 8), "dl": ("dx", 0), "dh": ("dx", 8)}


class Node:
    """Base IR expression node. Subclasses are pure-data; equality is structural."""
    def children(self):
        return []


class Const(Node):
    def __init__(self, val, width=16):
        self.val = val & ((1 << width) - 1)
        self.width = width

    def __repr__(self):
        return "0x%x" % self.val


class Reg(Node):
    def __init__(self, name, width=16):
        self.name = name
        self.width = width

    def __repr__(self):
        return self.name


class Flag(Node):
    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return self.name


class Mem(Node):
    def __init__(self, addr, width=16):
        self.addr = addr
        self.width = width

    def children(self):
        return [self.addr]

    def __repr__(self):
        return "mem%d[%r]" % (self.width, self.addr)


class Bin(Node):
    def __init__(self, op, a, b, width=16):
        self.op, self.a, self.b, self.width = op, a, b, width

    def children(self):
        return [self.a, self.b]

    def __repr__(self):
        return "(%r %s %r)" % (self.a, self.op, self.b)


class Un(Node):
    def __init__(self, op, a, width=16):
        self.op, self.a, self.width = op, a, width

    def children(self):
        return [self.a]

    def __repr__(self):
        return "%s%r" % (self.op, self.a)


# ---------------------------------------------------------------- IR statements
class Set(Node):
    """dst <- expr.  dst is a Reg / Flag / Mem location."""
    def __init__(self, dst, expr):
        self.dst, self.expr = dst, expr

    def __repr__(self):
        return "%r = %r" % (self.dst, self.expr)


class Label(Node):
    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return "%s:" % self.name


class Goto(Node):
    def __init__(self, target):
        self.target = target

    def __repr__(self):
        return "goto %s" % self.target


class CBranch(Node):
    def __init__(self, cond, target, fall):
        self.cond, self.target, self.fall = cond, target, fall

    def __repr__(self):
        return "if %r goto %s else %s" % (self.cond, self.target, self.fall)


class Call(Node):
    def __init__(self, target, args=()):
        self.target, self.args = target, list(args)

    def children(self):
        cs = list(self.args)
        if isinstance(self.target, Node):
            cs.append(self.target)
        return cs

    def __repr__(self):
        return "call %s(%s)" % (self.target, ", ".join(map(repr, self.args)))


class Ret(Node):
    def __init__(self, expr=None):
        self.expr = expr

    def __repr__(self):
        return "return" + (" %r" % self.expr if self.expr is not None else "")


# ------------------------------------------------------------------- the lifter
def _reg(tok):
    """Parse a register token into (Reg, subreg-info) or None."""
    tok = tok.strip().lower()
    if tok in WREGS:
        return Reg(tok, 16), None
    if tok in BREGS:
        base, shift = BREGS[tok]
        return Reg(base, 16), (base, shift)          # 8-bit view of a 16-bit reg
    return None, None


def _imm(tok):
    tok = tok.strip()
    try:
        return Const(int(tok, 0), 16)
    except ValueError:
        return None


def _rval(tok):
    """Right-hand value: register or immediate (memory handled by caller)."""
    r, sub = _reg(tok)
    if r is not None:
        if sub is None:
            return r
        _base, shift = sub
        # 8-bit read = (reg >> shift) & 0xff
        return Bin("&", Bin(">>", r, Const(shift, 16), 16), Const(0xff, 16), 8)
    return _imm(tok)


def _set_flags(result, a, b, op, width=16):
    """Emit the explicit flag definitions an ALU op produces. `result` is the IR
    expression for the destination's new value; a,b the operands (for CF/OF).
    `width` is the operand width (16 for the legacy lift_insn path, 32 for the
    32-bit lifter) -- it governs SF's sign bit and ZF's mask, and the CF/OF ops
    carry it in their Bin width. CF/OF are add-vs-subtract specific (a shared
    'carry' op could not tell an add's carry-out from a subtract's borrow)."""
    m = (1 << width) - 1
    out = [
        Set(Flag("ZF"), Bin("==", Bin("&", result, Const(m, width), width), Const(0, width), 1)),
        Set(Flag("SF"), Bin("&", Bin(">>", result, Const(width - 1, width), width),
                            Const(1, width), 1)),
        Set(Flag("PF"), Un("parity", result, 1)),
    ]
    if op == "+":
        out += [Set(Flag("CF"), Bin("ucarry", a, b, width)),
                Set(Flag("OF"), Bin("sovf_add", a, b, width))]
    elif op == "-":
        out += [Set(Flag("CF"), Bin("borrow", a, b, width)),
                Set(Flag("OF"), Bin("sovf_sub", a, b, width))]
    elif op == "inc":                                # inc/dec set OF but PRESERVE CF
        out += [Set(Flag("OF"), Bin("sovf_add", a, b, width))]
    elif op == "dec":
        out += [Set(Flag("OF"), Bin("sovf_sub", a, b, width))]
    else:                                            # logic / shift ops clear CF/OF
        out += [Set(Flag("CF"), Const(0, 1)), Set(Flag("OF"), Const(0, 1))]
    return out


ALU = {"add": "+", "sub": "-", "and": "&", "or": "|", "xor": "^"}
_SHIFT = {"shl": "<<", "sal": "<<", "shr": ">>", "sar": ">>s"}  # >>s = arithmetic (signed)
# x87 FPU (lifted to structured __f* helper calls -- the 80-bit stack semantics live
# in named runtime helpers, so these are readable/analyzable but not differentially
# validated the way the integer core is; see validate_il.py's scope note).
_FPU_CONST = {"fldz", "fld1", "fldpi", "fldl2e", "fldl2t", "fldln2", "fldlg2"}
_FPU_ARITH = {"fadd", "fsub", "fmul", "fdiv", "fsubr", "fdivr",
              "fiadd", "fisub", "fimul", "fidiv", "fisubr", "fidivr",
              "fcom", "fcomp", "fucom", "fucomp", "ficom", "ficomp", "fxch", "ffree"}
_FPU_NOARG = {"faddp", "fsubp", "fmulp", "fdivp", "fsubrp", "fdivrp",
              "fchs", "fabs", "fsqrt", "fsin", "fcos", "fptan", "fpatan", "f2xm1",
              "fyl2x", "fyl2xp1", "frndint", "fscale", "fprem", "fprem1", "fxtract",
              "fcompp", "fucompp", "ftst", "fxam", "fnstsw", "fstsw", "fnstcw",
              "fstcw", "fdecstp", "fincstp", "fldcw", "fnstenv", "fstenv", "fldenv",
              "fnsave", "frstor"}
_FPU_CMOV = {"fcmovb", "fcmove", "fcmovbe", "fcmovu",      # x87 conditional moves (P6)
             "fcmovnb", "fcmovne", "fcmovnbe", "fcmovnu"}
_FPU = (_FPU_CONST | _FPU_ARITH | _FPU_NOARG | _FPU_CMOV |
        {"fld", "fild", "fbld", "fstp", "fistp", "fbstp", "fst", "fist",
         "fwait", "wait", "fnop", "fclex", "fnclex", "finit", "fninit"})
CC = {                                               # jcc -> flag expression
    "jz": Flag("ZF"), "je": Flag("ZF"),
    "jnz": Un("!", Flag("ZF"), 1), "jne": Un("!", Flag("ZF"), 1),
    "js": Flag("SF"), "jns": Un("!", Flag("SF"), 1),
    "jc": Flag("CF"), "jb": Flag("CF"), "jnc": Un("!", Flag("CF"), 1),
    "jae": Un("!", Flag("CF"), 1),
}


def lift_insn(asm):
    """One asm line -> list[Stmt] with explicit flags. Unknown ops -> a raw stub."""
    asm = re.sub(r"\s+", " ", asm.strip().lower())
    asm = re.sub(r";.*$", "", asm).strip()
    if not asm:
        return []
    op, _, rest = asm.partition(" ")
    args = [a.strip() for a in rest.split(",")] if rest else []

    def wb(dst_reg, sub, value):
        """Write-back helper honouring 8-bit subregisters."""
        if sub is None:
            return Set(dst_reg, value)
        _base, shift = sub
        keep = ~(0xff << shift) & 0xffff
        merged = Bin("|", Bin("&", dst_reg, Const(keep, 16), 16),
                     Bin("<<", Bin("&", value, Const(0xff, 16), 16),
                         Const(shift, 16), 16), 16)
        return Set(dst_reg, merged)

    if op in ALU and len(args) == 2:
        d, sub = _reg(args[0])
        s = _rval(args[1])
        if d is None or s is None:
            return [("raw", asm)]
        cop = ALU[op]
        # xor/sub of a reg with itself is the canonical zero idiom
        if op in ("xor", "sub") and args[0] == args[1]:
            result = Const(0, 16)
        else:
            lhs = _rval(args[0])
            result = Bin(cop, lhs, s, 16)
        return [wb(d, sub, result)] + _set_flags(result, _rval(args[0]), s, cop)

    if op in ("inc", "dec") and len(args) == 1:
        d, sub = _reg(args[0])
        if d is None:
            return [("raw", asm)]
        cop = "+" if op == "inc" else "-"
        result = Bin(cop, _rval(args[0]), Const(1, 16), 16)
        # inc/dec leave CF untouched; only ZF/SF/PF/OF
        return [wb(d, sub, result),
                Set(Flag("ZF"), Bin("==", result, Const(0, 16), 1)),
                Set(Flag("SF"), Bin(">>", result, Const(15, 16), 1)),
                Set(Flag("PF"), Un("parity", result, 1))]

    if op in ("cmp", "test") and len(args) == 2:
        a = _rval(args[0]); b = _rval(args[1])
        if a is None or b is None:
            return [("raw", asm)]
        result = Bin("-" if op == "cmp" else "&", a, b, 16)
        return _set_flags(result, a, b, "-" if op == "cmp" else "&")

    if op == "mov" and len(args) == 2:
        d, sub = _reg(args[0]); s = _rval(args[1])
        if d is None or s is None:
            return [("raw", asm)]
        return [wb(d, sub, s)]

    if op in ("neg", "not") and len(args) == 1:
        d, sub = _reg(args[0])
        if d is None:
            return [("raw", asm)]
        result = Un("-" if op == "neg" else "~", _rval(args[0]), 16)
        stmts = [wb(d, sub, result)]
        if op == "neg":
            stmts += _set_flags(result, Const(0, 16), _rval(args[0]), "-")
        return stmts

    if op in CC and len(args) == 1:
        return [CBranch(CC[op], args[0], None)]       # fall filled by block builder
    if op == "jmp" and len(args) == 1:
        return [Goto(args[0])]
    if op in ("ret", "retn", "retf", "retd", "retfd", "retnd", "retfw", "retnw"):
        return [Ret(Reg("ax", 16))]
    if op == "nop":
        return []
    if op == "push" and len(args) == 1:
        return [("raw", "push " + args[0])]           # modelled by frame pass later
    if op == "pop" and len(args) == 1:
        return []
    return [("raw", asm)]


def lift_block(lines):
    out = []
    for ln in lines:
        out.extend(lift_insn(ln))
    return out


# ------------------------------------------------------------------- IR passes
def _reads_flags(node, acc):
    if isinstance(node, Flag):
        acc.add(node.name)
    for c in getattr(node, "children", lambda: [])():
        _reads_flags(c, acc)


def dead_flag_elimination(stmts):
    """Backward liveness over flags (straight-line/conservative). Drop a Set(Flag)
    whose flag is not read before it is next written. This is the pass that turns
    the explicit `xor` flag cloud back into a bare assignment."""
    live = set()
    out = []
    for st in reversed(stmts):
        if isinstance(st, Set) and isinstance(st.dst, Flag):
            if st.dst.name not in live:
                continue                              # dead flag write -> drop
            live.discard(st.dst.name)                 # this def satisfies later reads
            out.append(st)
            continue
        # any other statement: collect the flags it reads
        rd = set()
        if isinstance(st, Set):
            _reads_flags(st.expr, rd)
        elif isinstance(st, CBranch):
            _reads_flags(st.cond, rd)
        elif isinstance(st, Ret) and st.expr is not None:
            _reads_flags(st.expr, rd)
        live |= rd
        out.append(st)
    out.reverse()
    return out


def _fold(node):
    """Constant-fold an expression bottom-up."""
    if isinstance(node, Bin):
        a, b = _fold(node.a), _fold(node.b)
        if isinstance(a, Const) and isinstance(b, Const):
            v = _eval_bin(node.op, a.val, b.val, node.width)
            if v is not None:
                return Const(v, node.width)
        # arithmetic identities (readability). NB: | and & are deliberately NOT
        # simplified -- the subreg write `(reg & keep) | (val<<sh)` must keep its
        # shape so the al/ah/ax emitter peephole still recognises it.
        z = isinstance(b, Const) and b.val == 0
        za = isinstance(a, Const) and a.val == 0
        if node.op in ("+", "^") and z:
            return a
        if node.op in ("+", "^") and za:
            return b
        if node.op in ("-", "<<", ">>", ">>s") and z:
            return a
        if node.op == "*" and isinstance(b, Const) and b.val == 1:
            return a
        if node.op == "*" and isinstance(a, Const) and a.val == 1:
            return b
        return Bin(node.op, a, b, node.width)
    if isinstance(node, Un):
        a = _fold(node.a)
        if isinstance(a, Const):
            if node.op == "~":
                return Const(~a.val, node.width)
            if node.op == "-":
                return Const(-a.val, node.width)
        return Un(node.op, a, node.width)
    return node


def _eval_bin(op, a, b, w):
    m = (1 << w) - 1
    if op == "+":
        return (a + b) & m
    if op == "-":
        return (a - b) & m
    if op == "&":
        return a & b
    if op == "|":
        return a | b
    if op == "^":
        return a ^ b
    if op == "<<":
        return (a << b) & m
    if op == ">>":
        return a >> b
    if op == "==":
        return 1 if a == b else 0
    return None


def const_copy_prop(stmts, guard_addr=False):
    """Forward constant/copy propagation with folding. Tracks reg->Const|Reg and
    substitutes into later uses; invalidated on redefinition or Call.

    guard_addr (byte-exact/faithful path): do NOT propagate a value into an address-of
    `&x`. `&(local holding v)` and `&v` compile to different code (the local must be
    materialised in memory), so substituting there collapsed the Win32 `tmp=v; f(&tmp)`
    output-param idiom. Off by default so the readable ledger stays byte-stable (that
    path has the same latent bug; fixing it there needs a deliberate goldens rebaseline)."""
    env = {}

    def subst(node):
        if isinstance(node, Reg) and node.name in env:
            return env[node.name]
        if isinstance(node, Bin):
            return _fold(Bin(node.op, subst(node.a), subst(node.b), node.width))
        if isinstance(node, Un):
            if node.op == "&" and guard_addr:
                return node          # keep &local intact (see docstring)
            return _fold(Un(node.op, subst(node.a), node.width))
        if isinstance(node, Mem):
            return Mem(subst(node.addr), node.width)
        if isinstance(node, Call):
            return Call(node.target, [subst(a) for a in node.args])
        return node

    out = []
    for st in stmts:
        if isinstance(st, Set):
            expr = _fold(subst(st.expr))
            st = Set(st.dst, expr)
            if _has_call(expr):
                env.clear()                           # call clobbers all registers
            if isinstance(st.dst, Reg):
                if isinstance(expr, (Const, Reg)):
                    env[st.dst.name] = expr
                else:
                    env.pop(st.dst.name, None)
                # any reg that aliased this one is now stale
                for k in [k for k, v in env.items()
                          if isinstance(v, Reg) and v.name == st.dst.name]:
                    env.pop(k, None)
        elif isinstance(st, (CBranch, Ret)):
            if isinstance(st, CBranch):
                st = CBranch(subst(st.cond), st.target, st.fall)
            elif st.expr is not None:
                st = Ret(subst(st.expr))
        elif isinstance(st, Call):
            env.clear()                               # call clobbers everything
        out.append(st)
    return out


def dead_store_elimination(stmts):
    """Drop Set(Reg) whose value is never read afterward (straight-line). Registers
    live at exit (ax as return value) are kept. Memory/flags left to their passes."""
    live = {"ax"}                                     # return-value convention
    out = []
    for st in reversed(stmts):
        if isinstance(st, Set) and isinstance(st.dst, Reg):
            if st.dst.name not in live:
                # still must keep if the RHS has side effects; ours don't
                continue
            live.discard(st.dst.name)
            rd = _reads_regs(st.expr)
            live |= rd
            out.append(st)
            continue
        # collect reg reads of any other statement
        rd = set()
        if isinstance(st, Set):
            rd = _reads_regs(st.expr)
            if isinstance(st.dst, Mem):
                rd |= _reads_regs(st.dst.addr)
        elif isinstance(st, CBranch):
            rd = _reads_regs(st.cond)
        elif isinstance(st, Ret) and st.expr is not None:
            rd = _reads_regs(st.expr)
        elif isinstance(st, Call):
            live |= set(WREGS)                        # conservative
        live |= rd
        out.append(st)
    out.reverse()
    return out


def _reads_regs(node, acc=None):
    acc = set() if acc is None else acc
    if isinstance(node, Reg):
        acc.add(node.name)
    for c in getattr(node, "children", lambda: [])():
        _reads_regs(c, acc)
    return acc


def _has_call(node):
    if isinstance(node, Call):
        return True
    return any(_has_call(c) for c in getattr(node, "children", lambda: [])())


def run_passes(stmts, rounds=3):
    """Fixpoint the marquee passes. (TODO: SSA rename + CFG structuring + the
    IR-interpreter differential validator hook.)"""
    for _ in range(rounds):
        stmts = const_copy_prop(stmts)
        stmts = dead_flag_elimination(stmts)
        stmts = dead_store_elimination(stmts)
    return stmts


# ------------------------------------------------------------------- C emitter
_STRUCT_PTRS = set()          # param names typed as struct pointers (render argN->fN)


def _struct_field(node):
    """If `node` is a struct-pointer access -> `argN->f<off>`, else None. Set by
    lift_asm_function's type recovery via _STRUCT_PTRS for the current function."""
    if not isinstance(node, Mem):
        return None
    a = node.addr
    if isinstance(a, Reg) and a.name in _STRUCT_PTRS:
        return "%s->f0" % a.name
    if (isinstance(a, Bin) and a.op == "+" and isinstance(a.a, Reg)
            and a.a.name in _STRUCT_PTRS and isinstance(a.b, Const)):
        return "%s->f%x" % (a.a.name, a.b.val)
    return None


def _c(node):
    if isinstance(node, Const):
        return "0x%x" % node.val if node.val > 9 else str(node.val)
    if isinstance(node, Reg):
        return node.name
    if isinstance(node, Flag):
        return node.name
    if isinstance(node, Mem):
        fld = _struct_field(node)                         # argN->f8 rather than *(u32*)(argN+8)
        return fld if fld is not None else "*(u%d*)(%s)" % (node.width, _c(node.addr))
    if isinstance(node, Bin):
        sub = _subreg_read(node)                          # al/ah/ax rather than the mask algebra
        if sub is not None:
            return sub
        if node.op == ">>s":                             # arithmetic (signed) shift right
            return "((i32)%s >> %s)" % (_c(node.a), _c(node.b))
        sgn = {"<s": "<", ">s": ">", "<=s": "<=", ">=s": ">="}   # signed relational
        if node.op in sgn:
            return "((i32)%s %s (i32)%s)" % (_c(node.a), sgn[node.op], _c(node.b))
        cop = {"==": "==", "ucarry": "__carry", "borrow": "__borrow",
               "sovf_add": "__ovf_add", "sovf_sub": "__ovf_sub"}.get(node.op, node.op)
        if cop.startswith("__"):
            return "%s(%s, %s)" % (cop, _c(node.a), _c(node.b))
        return "(%s %s %s)" % (_c(node.a), cop, _c(node.b))
    if isinstance(node, Un):
        if node.op == "parity":
            return "__parity(%s)" % _c(node.a)
        return "%s%s" % (node.op, _c(node.a))
    if isinstance(node, Call):
        a = ", ".join(_c(x) for x in node.args)
        if isinstance(node.target, str):
            return "%s(%s)" % (node.target, a)
        return "(*(fnptr)%s)(%s)" % (_c(node.target), a)
    return repr(node)


def emit_c(stmts):
    out = []
    for st in stmts:
        if isinstance(st, tuple) and st[0] == "raw":
            out.append("    asm(%r);" % st[1])
        elif isinstance(st, tuple) and st[0] == "data":
            out.append("    /* data: %s */" % st[1])
        elif isinstance(st, Set):
            sw = _subreg_write(st)                         # `al = v` rather than the merge
            if sw is not None:
                out.append("    %s = %s;" % (sw[0], _c(sw[1])))
            else:
                out.append("    %s = %s;" % (_c(st.dst), _c(st.expr)))
        elif isinstance(st, Label):
            out.append("%s:" % st.name)
        elif isinstance(st, Goto):
            out.append("    goto %s;" % st.target)
        elif isinstance(st, CBranch):
            out.append("    if (%s) goto %s;" % (_c(st.cond), st.target))
        elif isinstance(st, Call):
            out.append("    %s(%s);" % (st.target, ", ".join(_c(a) for a in st.args)))
        elif isinstance(st, Ret):
            out.append("    return %s;" % _c(st.expr) if st.expr is not None else "    return;")
    return "\n".join(out)


# ---------------------------------------------------------------------- selftest
def _show(title, stmts):
    print("--- %s ---" % title)
    for st in stmts:
        print("   ", repr(st) if not (isinstance(st, tuple)) else "raw %s" % st[1])


def _selftest():
    print("=" * 68)
    print("CASE 1: the canonical `xor eax,eax` (here ax) flag collapse")
    print("=" * 68)
    ir = lift_block(["xor ax, ax"])
    _show("lifted (flags explicit)", ir)
    opt = run_passes(ir)
    _show("after passes", opt)
    print("C: ", emit_c(opt).strip())
    assert len(opt) == 1 and isinstance(opt[0], Set), "flags not eliminated"
    assert "ax = 0" in emit_c(opt), "did not collapse to ax = 0"
    print("PASS: collapsed to `ax = 0;`\n")

    print("=" * 68)
    print("CASE 2: const-prop + fold + dead-store across a block")
    print("=" * 68)
    ir = lift_block(["mov ax, 5", "add ax, 3", "mov bx, ax", "mov cx, 99", "ret"])
    _show("lifted", ir)
    opt = run_passes(ir)
    _show("after passes", opt)
    print("C:\n" + emit_c(opt))
    c = emit_c(opt)
    assert "ax = 8" in c, "5+3 not folded to 8"
    assert "cx" not in c, "dead store to cx not eliminated"
    print("\nPASS: folded ax=8, dropped dead cx, kept returned ax\n")

    print("=" * 68)
    print("CASE 3: a compare feeds a branch -> its flags MUST survive")
    print("=" * 68)
    ir = lift_block(["cmp ax, 10", "jz somewhere"])
    opt = run_passes(ir)
    _show("after passes", opt)
    reads = any(isinstance(s, CBranch) for s in opt)
    keptzf = any(isinstance(s, Set) and isinstance(s.dst, Flag) and s.dst.name == "ZF"
                 for s in opt)
    assert reads and keptzf, "live ZF was wrongly eliminated"
    print("PASS: ZF kept because the branch reads it (liveness is correct)\n")
    print("ALL SELFTESTS PASSED")
    return True


# ==========================================================================
# Real-function path: lift one of OUR reconstructed .asm files (32-bit) all the
# way to structured C. Reuses the IR nodes / passes / emitter above; adds a
# width-aware, memory-aware operand parser, an .asm parser, a block builder, a
# small if-then/if-then-else structurer, and a signature recoverer.
# ==========================================================================
REG32 = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp")
SEG = ("es", "ds", "fs", "gs", "ss", "cs")            # segment registers (16-bit)
BREG32 = {"al": ("eax", 0), "ah": ("eax", 8), "bl": ("ebx", 0), "bh": ("ebx", 8),
          "cl": ("ecx", 0), "ch": ("ecx", 8), "dl": ("edx", 0), "dh": ("edx", 8)}
WSUB = {"ax": "eax", "bx": "ebx", "cx": "ecx", "dx": "edx",     # 16-bit = low 16 of
        "si": "esi", "di": "edi", "bp": "ebp", "sp": "esp"}     # the 32-bit parent
_NEG = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", "<=": ">", ">": "<=",
        "< (unsigned)": ">= (unsigned)", ">= (unsigned)": "< (unsigned)",
        "<= (unsigned)": "> (unsigned)", "> (unsigned)": "<= (unsigned)"}

# --- readability peephole: recognise the byte/word subregister IR shapes the
# lifters emit and render them as al/ah/ax rather than the mask-merge algebra.
# PURELY COSMETIC: the IR is unchanged, so the differential validator (which runs
# the IR, not this text) still guarantees the semantics. A subreg name denotes the
# byte/word lane of its 32-bit parent (see the module preamble note in decompile).
_BREG_BY = {}                                          # (parent, shift) -> byte name
for _n, (_p32, _sh) in BREG32.items():
    _BREG_BY[(_p32, _sh)] = _n                         # ("eax", 0) -> "al"   (32-bit lifter)
    _BREG_BY[(_p32[1:], _sh)] = _n                     # ("ax",  0) -> "al"   (16-bit lifter)
_WREG_BY = {p: n for n, p in WSUB.items()}             # "eax" -> "ax"


def _subreg_read(node):
    """If `node` is a byte/word subregister READ, return its name (al/ah/ax/...)."""
    if not (isinstance(node, Bin) and node.op == "&" and isinstance(node.b, Const)):
        return None
    if node.b.val == 0xff:                             # 8-bit lane: (parent >> shift) & 0xff
        inner = node.a
        if (isinstance(inner, Bin) and inner.op == ">>" and isinstance(inner.a, Reg)
                and isinstance(inner.b, Const)):
            return _BREG_BY.get((inner.a.name, inner.b.val))
        if isinstance(inner, Reg):                     # shift 0 folded away: parent & 0xff
            return _BREG_BY.get((inner.name, 0))
    if node.b.val == 0xffff and isinstance(node.a, Reg):   # 16-bit lane: parent & 0xffff
        return _WREG_BY.get(node.a.name)
    return None


def _byte_value(rhs, shift):
    """Unwrap the value written into a byte lane: ((v & 0xff) << shift) -> v."""
    if isinstance(rhs, Bin) and rhs.op == "<<" and isinstance(rhs.b, Const) and rhs.b.val == shift:
        rhs = rhs.a
    elif shift != 0:
        if isinstance(rhs, Const):                     # ((c & 0xff) << shift) const-folded
            return Const((rhs.val >> shift) & 0xff, rhs.width) if not (rhs.val & ~(0xff << shift)) else None
        return None
    if isinstance(rhs, Bin) and rhs.op == "&" and isinstance(rhs.b, Const) and rhs.b.val == 0xff:
        return rhs.a                                   # (v & 0xff) -> v
    if isinstance(rhs, Const):                          # already the folded byte constant
        return rhs if rhs.val <= 0xff else None
    return None


def _subreg_write(st):
    """If `st` is a Set implementing a byte/word subregister WRITE, return
    (name, value_node) so the emitter can print `al = value` instead of the merge."""
    if not (isinstance(st, Set) and isinstance(st.dst, Reg) and isinstance(st.expr, Bin)
            and st.expr.op == "|"):
        return None
    base = st.dst.name
    keep_part, rhs = st.expr.a, st.expr.b
    if not (isinstance(keep_part, Bin) and keep_part.op == "&"
            and isinstance(keep_part.a, Reg) and keep_part.a.name == base
            and isinstance(keep_part.b, Const)):
        return None
    keep = keep_part.b.val
    if base in _WREG_BY and keep in (0xffff0000,):     # 16-bit lane write: (v & 0xffff)
        v = rhs.a if (isinstance(rhs, Bin) and rhs.op == "&"
                      and isinstance(rhs.b, Const) and rhs.b.val == 0xffff) else None
        if v is not None:
            return (_WREG_BY[base], v)
    for shift in (0, 8):                               # 8-bit lane write
        if keep in (~(0xff << shift) & 0xffffffff, ~(0xff << shift) & 0xffff) \
                and (base, shift) in _BREG_BY:
            v = _byte_value(rhs, shift)
            if v is not None:
                return (_BREG_BY[(base, shift)], v)
    return None


def _imm2(tok):
    tok = tok.strip()
    neg, t = (tok[0] == "-"), tok.lstrip("+-")
    try:
        if re.match(r"^[0-9a-fA-F]+h$", t):
            v = int(t[:-1], 16)
        else:
            v = int(t, 0)
        return Const(-v if neg else v, 32)
    except ValueError:
        return None


def _mem_addr(inner, bits=32):
    """Parse the inside of [ ] -> an IR address node: the full effective address
    base + index*scale + disp (any subset). e.g. [eax], [ebx+8], [eax+ecx*4+0x10],
    [edi*2-4]. Returns None if a term is unrecognisable (caller falls back to opaque)."""
    s = inner.replace(" ", "")
    if not s:
        return None
    regs = REG32 if bits == 32 else (WREGS + REG32)   # 16-bit: [bx+si], and 386 [edi] etc.
    node = None
    for term in re.findall(r"[+\-]?[^+\-]+", s):
        neg = term.startswith("-")
        term = term.lstrip("+-")
        msc = re.match(r"^([a-z]{2,3})\*([1248])$", term)
        if msc and msc.group(1) in regs:
            t = Bin("*", Reg(msc.group(1), bits), Const(int(msc.group(2)), bits), bits)
        elif term in regs:
            t = Reg(term, bits)
        else:
            t = _imm2(term)
            if t is None:
                return None
        if neg:
            node = Bin("-", node if node is not None else Const(0, bits), t, bits)
        else:
            node = t if node is None else Bin("+", node, t, bits)
    return node


def _operand(tok, params, width_hint=None, bits=32):
    """Parse one operand -> (IR node, width). Registers, immediates, and memory.
    `bits` selects the register file / addressing: 32-bit (eax, dword ptr,
    [esp/ebp+K] arg slots, [ebp-K] locals) or 16-bit (ax, word ptr, [bp+K] args,
    [bp-K] locals, ds:imm absolutes). Stack slots become named args in `params`."""
    t = tok.strip().lower()
    width = bits if width_hint is None else width_hint
    m = re.match(r"(dword|qword|tbyte|word|byte|xmmword|mmword|oword)\s+ptr\s+(.*)", t)
    if m:
        width = {"dword": 32, "qword": 64, "tbyte": 80, "word": 16, "byte": 8,
                 "xmmword": 128, "oword": 128, "mmword": 64}.get(m.group(1), bits)
        t = m.group(2).strip()
    msimd = re.match(r"^(mm|xmm|ymm|zmm)(\d+)$", t)        # MMX/SSE/AVX vector registers
    if msimd:
        w = {"mm": 64, "xmm": 128, "ymm": 256, "zmm": 512}[msimd.group(1)]
        return Reg(t, w), w
    if bits == 32:                                        # 32-bit: e-regs primary, ax/al = subregs
        if t in REG32:
            return Reg(t, 32), 32
        if t in WSUB:                                     # ax = low 16 of eax
            return Bin("&", Reg(WSUB[t], 32), Const(0xffff, 32), 16), 16
        if t in SEG:
            return Reg(t, 16), 16
        if t in BREG32:                                   # al = (eax >> 0) & 0xff
            base, shift = BREG32[t]
            return Bin("&", Bin(">>", Reg(base, 32), Const(shift, 32), 32), Const(0xff, 32), 8), 8
    else:                                                 # 16-bit: 16-bit regs primary, but a
        if t in REG32:                                    # 386 in a 16-bit segment uses e-regs too
            return Reg(t, 32), 32
        if t in WREGS:
            return Reg(t, 16), 16
        if t in SEG:
            return Reg(t, 16), 16
        if t in BREGS:                                    # al = (ax >> 0) & 0xff
            base, shift = BREGS[t]
            return Bin("&", Bin(">>", Reg(base, 16), Const(shift, 16), 16), Const(0xff, 16), 8), 8
    # segment-override memory: fs:[0] (SEH/TIB), es:[bx], fs:[eax+4], ...
    mseg = re.match(r"^(es|ds|fs|gs|ss|cs):\[(.+)\]$", t)
    if mseg:
        seg, inner = mseg.group(1), mseg.group(2).strip()
        off = _mem_addr(inner, bits) or Const(0, bits)
        # segmented address = segment base + offset (flat model: fs -> TIB, etc.)
        return Mem(Bin("+", Reg(seg, 16), off, bits), width), width
    # seg:imm absolute (e.g. `ds:52h` in 16-bit; `mov ax, ds:134eh` thunks in 32-bit).
    # Flat model: the segment base is 0, so the linear address is the offset.
    msa = re.match(r"^(es|ds|fs|gs|ss|cs):(0x[0-9a-f]+|[0-9a-f]+h|\d+)$", t)
    if msa:
        return Mem(_imm2(msa.group(2)), width), width
    if t.startswith("[") and t.endswith("]"):
        inner = t[1:-1].strip()
        fp, argdiv = ("esp|ebp", 4) if bits == 32 else ("bp|sp", 2)
        loc = "ebp" if bits == 32 else "bp"
        # stack slot -> argument (a sane frame only; a huge disp is a mis-disassembled
        # db byte, not a 32KB+ arg -- leave those as an honest memory access).
        ms = re.match(r"^(%s)\s*\+\s*(0x[0-9a-f]+|[0-9a-f]+h|\d+)$" % fp, inner)
        if ms and _imm2(ms.group(2)).val < 0x8000:
            disp = _imm2(ms.group(2)).val
            off_in = disp % argdiv               # byte offset WITHIN the containing arg slot
            base = disp - off_in                 # the aligned dword-arg this access lands in
            name = "arg%d" % (base // argdiv)    # ret addr low; args above the frame ptr
            params[base] = name
            if off_in and width < argdiv * 8:    # sub-dword access -> byte/word LANE of the arg
                # e.g. `mov cl, [ebp+9]` reads byte 1 of arg2, i.e. (arg2 >> 8) & 0xff -- not
                # the whole arg. Dropping off_in collapsed the lane and read byte 0 instead.
                return (Bin("&", Bin(">>", Reg(name, 32), Const(off_in * 8, 32), 32),
                             Const((1 << width) - 1, 32), width), width)
            return Reg(name, width), width
        # frame-negative slot -> a named stack local (declared at function top).
        ml = re.match(r"^%s\s*-\s*(0x[0-9a-f]+|[0-9a-f]+h|\d+)$" % loc, inner)
        if ml:
            return Reg("loc_%x" % _imm2(ml.group(1)).val, width), width
        # absolute global
        mg = re.match(r"^(0x[0-9a-f]+|[0-9a-f]+h)$", inner)
        if mg:
            return Mem(_imm2(mg.group(1)), width), width
        # general effective address: base + index*scale + disp
        addr = _mem_addr(inner, bits)
        return Mem(addr if addr is not None else Const(0, bits), width), width
    v = _imm2(t)
    if v is not None:
        return v, width
    return None, width


def _is_csr(low, bits=32):
    """True if `low` is a push of a callee-saved register (a prologue save)."""
    csr = ("ebp", "ebx", "esi", "edi") if bits == 32 else ("bp", "bx", "si", "di", "ds")
    parts = low.split(None, 1)
    return len(parts) == 2 and parts[1].strip() in csr


def _call_target(tok, params, bits=32):
    """Resolve a call operand -> a direct name 'sub_XXXX' or an indirect IR node.
    NE far calls keep their RESOLVED target in the disasm (carried through in the operand):
      `0h:0a1h -> USER.POSTAPPMESSAGE [USER.116]`   (import: module.name [module.ordinal])
      `0ffffh:1d2h -> seg2:0x0000`                  (internal far call to another segment)
    Use it -- an import becomes a named MODULE_NAME symbol, an internal far call resolves to
    sub_<target-offset> -- instead of collapsing every far call to the opaque sub_indirect."""
    mv = re.match(r"vxd:(\w+)_(\d+)$", tok.strip())       # synthetic VxD service call
    if mv:                                                # (see _resolve_vxdcalls); tok is lowercased,
        return "%s_ord_%s" % (mv.group(1).upper(), mv.group(2))  # device names are UPPER -> restore
    ma = re.search(r"->\s*(\S.*)$", tok)
    if ma:
        tgt = ma.group(1).strip()
        mi = re.match(r"([a-z0-9_]+)\.([a-z0-9_$?@]+)\s*\[", tgt)      # MODULE.NAME [...]
        if mi:
            return "%s_%s" % (mi.group(1).upper(), re.sub(r"\W", "_", mi.group(2)).upper())
        mo = re.match(r"\[?\s*([a-z0-9_]+)\.(\d+)\s*\]", tgt)          # by-ordinal only [MOD.n]
        if mo:
            return "%s_ord_%s" % (mo.group(1).upper(), mo.group(2))
        ms = re.match(r"seg(\d+):0x([0-9a-f]+)", tgt)                  # internal far call
        if ms:
            return "sub_%x" % int(ms.group(2), 16)
    t = re.sub(r"^(near|far|short)\s+(ptr\s+)?", "", tok.strip().lower()).strip()
    m = re.match(r"^(0x[0-9a-f]+|[0-9a-f]+h)$", t)
    if m:
        return "sub_%x" % _imm2(t).val
    ml = re.match(r"^l_([0-9a-f]+)$", t)                   # intra-section label target (`call L_08341`)
    if ml:                                                 # -> its offset. A DIRECT call the disassembler
        return "sub_%x" % int(ml.group(1), 16)            # rendered as a label, NOT an indirect call.
    node, _w = _operand(t, params, bits=bits)
    return node if node is not None else "sub_indirect"


def _wb32(tok, params, value, bits=32):
    """Write-back to a destination operand, honouring 8-bit subregisters. Writing
    al/ah/... merges the byte into its parent (16- or 32-bit per `bits`, rest kept).
    Returns a Set, or None if the destination cannot be parsed."""
    t = re.sub(r"(dword|word|byte)\s+ptr\s+", "", tok.strip().lower()).strip()
    breg = BREG32 if bits == 32 else BREGS
    if t in breg:
        base, shift = breg[t]
        keep = ~(0xff << shift) & ((1 << bits) - 1)
        merged = Bin("|", Bin("&", Reg(base, bits), Const(keep, bits), bits),
                     Bin("<<", Bin("&", value, Const(0xff, bits), bits),
                         Const(shift, bits), bits), bits)
        return Set(Reg(base, bits), merged)
    if bits == 32 and t in WSUB:                         # 16-bit write merges low 16 into parent
        base = WSUB[t]
        return Set(Reg(base, 32),
                   Bin("|", Bin("&", Reg(base, 32), Const(0xffff0000, 32), 32),
                       Bin("&", value, Const(0xffff, 32), 32), 32))
    d, _w = _operand(tok, params, bits=bits)
    return Set(d, value) if d is not None else None


def _far_ptr_slot(memtok, params, bits):
    """les/lds of a far pointer held in a STACK SLOT ([bp+K]). The slot parsed as an
    operand resolves to a param/local (arg3), not a Mem, so the normal les path bails.
    Here the offset is that slot and the segment is the NEXT word (slot+2). Returns
    (offset_node, segment_node), or (None, None) if it isn't a stack-slot far ptr."""
    mm, _ = _operand("word ptr " + memtok, params, bits=bits)
    if mm is None or isinstance(mm, Mem):
        return None, None
    m = re.match(r"^\[\s*(e?bp|e?sp)\s*([+-])\s*(0x[0-9a-f]+|[0-9a-f]+h|\d+)\s*\]$",
                 memtok.strip())
    if not m:
        return None, None
    base_r, sign, ds = m.groups()
    disp = _imm2(ds).val * (1 if sign == "+" else -1)
    seg, _ = _operand("[%s%+d]" % (base_r, disp + 2), params, bits=bits)
    return mm, seg


def _is_simd(op):
    """A MMX/SSE/AVX vector op (reached only after every scalar op is handled, so
    stragglers starting with p.. or ending ps/pd/ss/sd are genuinely SIMD). We don't
    model vector semantics; they render as __<op>(..) helpers on the xmm/mm regs."""
    return (op.startswith(("p", "cvt", "unpck", "shuf", "pack", "movq", "movd", "movnt",
                           "mova", "movu", "movh", "movl", "movmsk", "vp", "vmov"))
            or op.endswith(("ps", "pd", "ss", "sd"))
            or op in ("emms", "femms", "ldmxcsr", "stmxcsr", "lfence", "mfence", "sfence",
                      "maskmovq", "maskmovdqu"))


def _lift_simd(op, args, params, bits):
    """dst = __op(operands) for a SIMD instruction (opaque helper, honest scaffold)."""
    ops = [_operand(a, params, bits=bits)[0] for a in args]
    if any(o is None for o in ops):
        return [("raw", op + " " + ", ".join(args))]
    if not args or op in ("emms", "femms", "lfence", "mfence", "sfence"):
        return [Call("__" + op, ops)]
    wb = _wb32(args[0], params, Call("__" + op, ops), bits=bits)  # dst = helper(operands)
    return [wb] if wb is not None else [Call("__" + op, ops)]


def _is_data(insn):
    """A reconstructed line that is inline DATA, not code: the disassembler's data
    markers (`data`, `data -> seg..`, `string`) or a run of raw hex bytes (`00 00`).
    These belong to the function's asm but are not un-lifted instructions."""
    s = insn.strip()
    op = s.split()[0] if s.split() else ""
    # objdump -mi386 sometimes decodes data-carved-as-code as an opcode that cannot
    # exist on the 8086/286 target (e.g. `0f 01 ff` -> `tlbsync`, a 2007-era AMD
    # instruction). Those are misdecodes, not real instructions: treat as data.
    _ANACHRONISTIC = ("tlbsync", "invlpga", "rdtscp", "monitor", "mwait")
    return (op == "data" or op.startswith("data/") or op in ("string", "(bad)", "???")
            or op in _ANACHRONISTIC
            or s.startswith((";", ".")) or not s
            or bool(re.match(r"^[0-9a-f]{2}( [0-9a-f]{2})*$", s)))


_SETCC = {"sete": "==", "setz": "==", "setne": "!=", "setnz": "!=",     # dst = (a <op> b)
          "setb": "<", "setc": "<", "setnae": "<", "setae": ">=", "setnb": ">=",
          "setbe": "<=", "setna": "<=", "seta": ">", "setnbe": ">",      # unsigned / eq
          "setl": "<s", "setnge": "<s", "setge": ">=s", "setnl": ">=s",  # signed
          "setle": "<=s", "setng": "<=s", "setg": ">s", "setnle": ">s"}


def _lift_fpu(op, args, params, bits=32):
    """x87 FPU -> structured __f* helper calls (shared by both lifters). The 80-bit
    stack semantics live in the named helpers -- readable/analyzable, but not
    differentially validated the way the integer core is (see validate_il's scope)."""
    def _fop():
        if not args:
            return None
        a = args[-1].strip()
        ms = re.match(r"st\(?(\d)\)?$", a)
        return Reg("st%s" % ms.group(1), 64) if ms else _operand(a, params, bits=bits)[0]
    if op in ("fld", "fild", "fbld"):                 # push a value onto the FP stack
        s = _fop()
        return [Call("__fpush", [s] if s is not None else [])]
    if op in ("fstp", "fistp", "fbstp"):              # store top, then pop
        d = _fop()
        return [Set(d, Call("__fpop", []))] if isinstance(d, Mem) else [Call("__fpop", [])]
    if op in ("fst", "fist"):                         # store top (no pop)
        d = _fop()
        return [Set(d, Call("__ftop", []))] if isinstance(d, Mem) else []
    if op in _FPU_CONST:                              # fldz/fld1/fldpi... push a constant
        return [Call("__" + op, [])]
    if op in _FPU_ARITH:                              # st0 op= operand
        s = _fop()
        return [Call("__" + op, [s] if s is not None else [])]
    if op in _FPU_NOARG:                              # p-forms / unary on st0 / control-w-effect
        d = _fop()
        if op in ("fnstsw", "fstsw", "fnstcw", "fstcw") and isinstance(d, Mem):
            return [Set(d, Call("__" + op, []))]
        return [Call("__" + op, [d] if isinstance(d, Mem) else [])]
    if op in _FPU_CMOV:                               # conditional move st(i) -> st0 (P6)
        s = _fop()
        return [Call("__" + op, [s] if s is not None else [])]
    return []                                         # FPU control (fwait/fnop/finit/...)


def _lift32(insn, params, last_cmp):
    """Lift one 32-bit instruction to IR; update last_cmp[0]=(a,b) for cmp/test."""
    insn = re.sub(r"\s*->.*$", "", re.sub(r"\s+", " ", insn.strip().lower()))  # drop annotations
    op, _, rest = insn.partition(" ")
    args = [a.strip() for a in _split_ops(rest)] if rest else []

    if _is_data(insn):                                   # inline data, not un-lifted code
        return [("data", insn)]
    if op in SEG and rest:                               # a segment prefix -> lift the rest
        return _lift32(rest, params, last_cmp)
    if op in ALU and len(args) == 2:
        d, _w = _operand(args[0], params)
        s, _w2 = _operand(args[1], params)
        if d is None or s is None:
            return [("raw", insn)]
        if op in ("xor", "sub") and args[0] == args[1]:
            result = Const(0, 32)
        else:
            result = Bin(ALU[op], d, s, 32)
        wb = _wb32(args[0], params, result)
        if wb is None:
            return [("raw", insn)]
        return _set_flags(result, d, s, ALU[op], _w) + [wb]
    if op == "mov" and len(args) == 2:
        s, _w2 = _operand(args[1], params)
        if s is None:
            return [("raw", insn)]
        wb = _wb32(args[0], params, s)
        if wb is None:
            return [("raw", insn)]
        return [wb]
    if op in ("cmp", "test") and len(args) == 2:
        a, aw = _operand(args[0], params)
        b, _ = _operand(args[1], params)
        if a is None or b is None:
            return [("raw", insn)]
        last_cmp[0] = (a, b)
        result = Bin("-" if op == "cmp" else "&", a, b, 32)
        return _set_flags(result, a, b, "-" if op == "cmp" else "&", aw)
    if op in ("inc", "dec") and len(args) == 1:
        d, dw = _operand(args[0], params)
        if d is None:
            return [("raw", insn)]
        result = Bin("+" if op == "inc" else "-", d, Const(1, 32), 32)
        wb = _wb32(args[0], params, result)
        if wb is None:
            return [("raw", insn)]
        return _set_flags(result, d, Const(1, dw), op, dw) + [wb]
    if op == "lea" and len(args) == 2:
        d, _ = _operand(args[0], params)
        mm, _ = _operand("dword ptr " + args[1], params)
        if isinstance(mm, Mem):                          # lea of a raw address -> the addr expr
            return [Set(d, mm.addr)] if d is not None else [("raw", insn)]
        if d is not None and mm is not None:             # [esp+x]/[ebp+x] became a named local:
            return [Set(d, Un("&", mm, 32))]             # lea takes its ADDRESS -> &arg
        return [("raw", insn)]
    if op in ("movzx", "movsx") and len(args) == 2:
        s, sw = _operand(args[1], params)
        if s is None:
            return [("raw", insn)]
        if op == "movsx":                                # sign-extend from source width
            m = 1 << ((sw or 8) - 1)                      # ((s ^ signbit) - signbit)
            s = Bin("-", Bin("^", s, Const(m, 32), 32), Const(m, 32), 32)
        wb = _wb32(args[0], params, s)
        if wb is None:
            return [("raw", insn)]
        return [wb]
    if op in _SHIFT and len(args) == 2:
        d, dw = _operand(args[0], params)
        cnt, _ = _operand(args[1], params)
        if d is None or cnt is None:
            return [("raw", insn)]
        result = Bin(_SHIFT[op], d, cnt, 32)
        wb = _wb32(args[0], params, result)
        if wb is None:
            return [("raw", insn)]
        return _set_flags(result, d, cnt, _SHIFT[op], dw) + [wb]
    if op in ("shld", "shrd") and len(args) == 3:        # double-precision shift
        d, _ = _operand(args[0], params); s, _ = _operand(args[1], params)
        cnt, _ = _operand(args[2], params)
        if d is None or s is None or cnt is None:
            return [("raw", insn)]
        rest = Bin("-", Const(32, 32), cnt, 32)          # bits shifted in from `src`
        result = (Bin("|", Bin("<<", d, cnt, 32), Bin(">>", s, rest, 32), 32) if op == "shld"
                  else Bin("|", Bin(">>", d, cnt, 32), Bin("<<", s, rest, 32), 32))
        wb = _wb32(args[0], params, result)
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("neg", "not") and len(args) == 1:
        d, dw = _operand(args[0], params)
        if d is None:
            return [("raw", insn)]
        result = Un("-" if op == "neg" else "~", d, 32)
        wb = _wb32(args[0], params, result)
        if wb is None:
            return [("raw", insn)]
        if op == "neg":
            return _set_flags(result, Const(0, dw), d, "-", dw) + [wb]
        return [wb]
    if op in ("adc", "sbb") and len(args) == 2:
        d, w = _operand(args[0], params)
        s, _ = _operand(args[1], params)
        if d is None or s is None:
            return [("raw", insn)]
        # adc/sbb READ CF into the result and WRITE CF out. Snapshot the incoming CF
        # into a temp so neither the CF-out write nor the dest write-back corrupts it.
        cf0, m = Reg("__cf0", 1), Const((1 << w) - 1, w)

        def _ovf_bits(p, q, r, t):                    # sign bit of ((p^q) & (r^t))
            return Bin("&", Bin(">>", Bin("&", Bin("^", p, q, w), Bin("^", r, t, w), w),
                               Const(w - 1, w), w), Const(1, w), 1)
        if op == "adc":                               # d + s + CF
            result = Bin("+", Bin("+", d, s, w), cf0, w)
            cout = Bin("|", Bin("ucarry", d, s, w), Bin("ucarry", Bin("+", d, s, w), cf0, w), 1)
            of = _ovf_bits(d, result, s, result)
        else:                                         # sbb: d - s - CF
            result = Bin("-", Bin("-", d, s, w), cf0, w)
            cout = Bin("|", Bin("borrow", d, s, w), Bin("borrow", Bin("-", d, s, w), cf0, w), 1)
            of = _ovf_bits(d, s, d, result)
        wb = _wb32(args[0], params, result)
        if wb is None:
            return [("raw", insn)]
        return [Set(cf0, Flag("CF")),                 # snapshot before anything is written
                Set(Flag("ZF"), Bin("==", Bin("&", result, m, w), Const(0, w), 1)),
                Set(Flag("SF"), Bin("&", Bin(">>", result, Const(w - 1, w), w), Const(1, w), 1)),
                Set(Flag("PF"), Un("parity", result, 1)),
                Set(Flag("CF"), cout), Set(Flag("OF"), of)] + [wb]
    if op == "imul" and len(args) in (2, 3):
        ops = [_operand(a, params)[0] for a in args]
        if any(o is None for o in ops):
            return [("raw", insn)]
        result = Bin("*", ops[1], ops[2], 32) if len(args) == 3 else Bin("*", ops[0], ops[1], 32)
        wb = _wb32(args[0], params, result)          # flags (CF/OF) left implicit
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("rol", "ror") and len(args) == 2:
        d, _ = _operand(args[0], params)
        cnt, _ = _operand(args[1], params)
        if d is None or cnt is None:
            return [("raw", insn)]
        rest = Bin("-", Const(32, 32), cnt, 32)       # width - count (32-bit dest)
        if op == "rol":
            result = Bin("|", Bin("<<", d, cnt, 32), Bin(">>", d, rest, 32), 32)
        else:
            result = Bin("|", Bin(">>", d, cnt, 32), Bin("<<", d, rest, 32), 32)
        wb = _wb32(args[0], params, result)
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("rcl", "rcr") and len(args) == 2:          # rotate through carry (33-bit) -> helper
        d, _ = _operand(args[0], params); cnt, _ = _operand(args[1], params)
        if d is None or cnt is None:
            return [("raw", insn)]
        wb = _wb32(args[0], params, Call("__" + op, [d, cnt, Flag("CF")]))
        return [wb] if wb is not None else [("raw", insn)]
    if op in _FPU:                                    # x87 FPU -> structured helper calls
        return _lift_fpu(op, args, params, 32)
    if op in ("les", "lds", "lfs", "lgs", "lss") and len(args) == 2:
        d, _ = _operand(args[0], params)               # far pointer: 32-bit off + 16-bit seg
        memtok = re.sub(r"^(dword|fword|qword|word)?\s*ptr\s+", "", args[1].strip())
        mm, _ = _operand("dword ptr " + memtok, params)
        seg = {"les": "es", "lds": "ds", "lfs": "fs", "lgs": "gs", "lss": "ss"}[op]
        if d is not None and isinstance(mm, Mem):
            off_wb = _wb32(args[0], params, Mem(mm.addr, 32))
            if off_wb is not None:
                return [off_wb, Set(Reg(seg, 16),
                                    Mem(Bin("+", mm.addr, Const(4, 32), 32), 16))]
        off, sg = _far_ptr_slot(memtok, params, 32)    # far ptr passed in a stack slot
        if d is not None and off is not None:
            off_wb = _wb32(args[0], params, off)
            if off_wb is not None:
                return [off_wb] + ([Set(Reg(seg, 16), sg)] if sg is not None else [])
        return [("raw", insn)]
    if op == "cdq":                                    # edx:eax = sign-extend(eax)
        return [Set(Reg("edx", 32), Bin(">>s", Reg("eax", 32), Const(31, 32), 32))]
    if op == "cwde":                                   # eax = sign-extend(ax)
        ax = Bin("&", Reg("eax", 32), Const(0xffff, 32), 32)
        m = Const(1 << 15, 32)
        return [Set(Reg("eax", 32), Bin("-", Bin("^", ax, m, 32), m, 32))]
    if op in ("cwd", "cbw"):                            # dx=sext(ax) / ax=sext(al)
        src = Bin("&", Reg("eax", 32), Const(0xffff if op == "cwd" else 0xff, 32), 32)
        m = Const(1 << (15 if op == "cwd" else 7), 32)
        sx = Bin("-", Bin("^", src, m, 32), m, 32)      # sign-extended value
        val = Bin(">>", sx, Const(16, 32), 32) if op == "cwd" else sx
        wb = _wb32("dx" if op == "cwd" else "ax", params, val)
        return [wb] if wb is not None else [("raw", insn)]
    m_str = re.match(r"^(movs|stos|lods|scas|cmps)([bwd])$", op)
    if m_str or (op in ("rep", "repe", "repz", "repne", "repnz")
                 and args and re.match(r"^(movs|stos|lods|scas|cmps)[bwd]$", args[0])):
        return _string_op(op if m_str else args[0], None if m_str else op)
    ps = re.match(r"^(outs|ins)([bwd]?)$", op)          # port string I/O
    if ps or (op in ("rep", "repe", "repz", "repne", "repnz")
              and args and re.match(r"^(outs|ins)[bwd]?$", args[0])):
        return _port_string(op if ps else args[0], None if ps else op, 32)
    if op in ("clc", "stc", "cmc"):                     # carry-flag ops
        return [Set(Flag("CF"), Const(0, 1) if op == "clc" else
                    Const(1, 1) if op == "stc" else Un("!", Flag("CF"), 1))]
    if op in ("sahf", "lahf"):                          # AH<->flags transfer
        return [Call("__" + op, [])]
    if op in ("daa", "das", "aaa", "aas"):              # BCD adjust of al/ax -> helper
        wb = _wb32("al" if op in ("daa", "das") else "ax", params,
                   Call("__" + op, [_operand("ax", params)[0]]))
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("aad", "aam"):                            # ax adjust for BCD mul/div
        base = _operand(args[0], params)[0] if args else Const(10, 32)
        wb = _wb32("ax", params, Call("__" + op, [_operand("ax", params)[0], base]))
        return [wb] if wb is not None else [("raw", insn)]
    if op == "bound" and len(args) == 2:                # array-index bounds check (traps)
        a, _ = _operand(args[0], params); b, _ = _operand(args[1], params)
        return [Call("__bound", [a, b])] if a is not None and b is not None else [("raw", insn)]
    if op == "xchg" and len(args) == 2:
        a, _ = _operand(args[0], params)
        b, _ = _operand(args[1], params)
        if a is None or b is None:
            return [("raw", insn)]
        t = Reg("__xch", 32)
        wa = _wb32(args[0], params, b)                  # a = b
        wb = _wb32(args[1], params, t)                  # b = (old a)
        if wa is not None and wb is not None:
            return [Set(t, a), wa, wb]
        return [("raw", insn)]
    if op in ("int3", "int1", "icebp", "ud0", "ud2"):
        return [Call("__debugbreak", [])]
    if op == "into":
        return [Call("__into", [])]                    # trap on overflow
    if op == "int" and len(args) == 1:
        n, _ = _operand(args[0], params)
        return [Call("__int", [n])] if n is not None else [("raw", insn)]
    if op in ("bt", "bts", "btr", "btc") and len(args) == 2:   # bit test (+set/reset/complement)
        d, _ = _operand(args[0], params); bit, _ = _operand(args[1], params)
        if d is None or bit is None:
            return [("raw", insn)]
        cf = [Set(Flag("CF"), Bin("&", Bin(">>", d, bit, 32), Const(1, 32), 1))]
        if op == "bt":
            return cf
        one = Bin("<<", Const(1, 32), bit, 32)
        res = {"bts": Bin("|", d, one, 32), "btr": Bin("&", d, Un("~", one, 32), 32),
               "btc": Bin("^", d, one, 32)}[op]
        wb = _wb32(args[0], params, res)
        return cf + [wb] if wb is not None else cf
    if op in ("sldt", "str", "smsw") and len(args) == 1:      # store system reg -> dst
        wb = _wb32(args[0], params, Call("__" + op, []))
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("lldt", "ltr", "lgdt", "lidt", "sgdt", "sidt", "lmsw", "invlpg", "clts",
              "invd", "wbinvd", "cpuid", "rdtsc", "rdmsr", "wrmsr", "ud2", "sysenter", "sysexit",
              "syscall", "sysret", "rsm", "sgdtw", "sidtw"):
        return [Call("__" + op, [x for x in (_operand(a, params)[0] for a in args) if x is not None])]
    if op in ("in", "out") and len(args) == 2:
        port = args[1] if op == "in" else args[0]
        acc = args[0] if op == "in" else args[1]
        p, _ = _operand(port, params)
        if op == "in":
            wb = _wb32(acc, params, Call("__inp", [p] if p is not None else []))
            return [wb] if wb is not None else [("raw", insn)]
        a, _ = _operand(acc, params)
        return [Call("__outp", [x for x in (p, a) if x is not None])]
    if op in ("mul", "imul") and len(args) == 1:       # edx:eax = eax * src
        s, _ = _operand(args[0], params)
        if s is None:
            return [("raw", insn)]
        return [Set(Reg("edx", 32), Call("__" + op + "hi", [Reg("eax", 32), s])),
                Set(Reg("eax", 32), Bin("*", Reg("eax", 32), s, 32))]
    if op in ("div", "idiv") and len(args) == 1:       # eax=quot, edx=rem of edx:eax / src
        s, _ = _operand(args[0], params)
        if s is None:
            return [("raw", insn)]
        hi, lo = Reg("__dvh", 32), Reg("__dvl", 32)     # snapshot edx,eax before overwrite
        return [Set(hi, Reg("edx", 32)), Set(lo, Reg("eax", 32)),
                Set(Reg("eax", 32), Call("__" + op, [hi, lo, s])),
                Set(Reg("edx", 32), Call("__" + op + "_rem", [hi, lo, s]))]
    if op == "bswap" and len(args) == 1:
        d, _ = _operand(args[0], params)
        if d is None:
            return [("raw", insn)]
        wb = _wb32(args[0], params, Call("__bswap", [d]))
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("lock", "addr32", "data32", "addr16", "data16", "rex") and rest:
        return _lift32(rest, params, last_cmp)         # a prefix, not an op: lift the rest
    if op in ("xlat", "xlatb"):                         # al = [ds:ebx + al]
        idx = Bin("+", Reg("ebx", 32), Bin("&", Reg("eax", 32), Const(0xff, 32), 32), 32)
        wb = _wb32("al", params, Mem(idx, 8))
        return [wb] if wb is not None else [("raw", insn)]
    if op in _SETCC and len(args) == 1 and last_cmp[0] is not None:   # dst = (a <op> b)
        a, b = last_cmp[0]
        wb = _wb32(args[0], params, Bin(_SETCC[op], a, b, 8))
        return [wb] if wb is not None else [("raw", insn)]
    if op == "arpl" and len(args) == 2:                 # adjust RPL of dst (rare, prot mode)
        d, _ = _operand(args[0], params); s, _ = _operand(args[1], params)
        wb = _wb32(args[0], params, Call("__arpl", [d, s])) if d is not None and s is not None else None
        return [wb] if wb is not None else [("raw", insn)]
    if op == "salc":                                    # al = CF ? 0xff : 0
        wb = _wb32("al", params, Un("-", Flag("CF"), 8))
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("lar", "lsl") and len(args) == 2:         # load access-rights / segment-limit (prot mode)
        s, _ = _operand(args[1], params)                # dst = rights/limit of the selector in src
        wb = _wb32(args[0], params, Call("__" + op, [s])) if s is not None else None
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("bsf", "bsr") and len(args) == 2:         # bit scan fwd/rev -> index of first/last set bit
        s, _ = _operand(args[1], params)
        wb = _wb32(args[0], params, Call("__" + op, [s])) if s is not None else None
        return [wb] if wb is not None else [("raw", insn)]
    if op in ("iret", "iretd", "iretw"):               # interrupt return
        return [Ret(Reg("eax", 32))]
    if op in ("ret", "retn", "retf", "retd", "retfd", "retnd", "retfw", "retnw"):
        return [Ret(Reg("eax", 32))]                   # retd/retfd = 32-bit operand-size returns
    if op == "nop" or op.startswith("align") or op in ("cld", "std", "sti", "cli",
                                                        "wait", "fwait", "hlt"):
        return []                                    # cld: DF=forward, which we assume
    if op in ("push", "pop", "leave", "enter", "pusha", "popa", "pushad", "popad",
              "pushf", "popf", "pushfd", "popfd", "pushd", "popd", "popal", "pushal",
              "pushaw", "popaw"):
        return []                                    # frame boilerplate (esp/ebp modeled away)
    if op.startswith("cmov") and len(args) == 2 and last_cmp[0] is not None:  # dst = cond ? src : dst
        setmn = "set" + op[4:]
        if setmn in _SETCC:
            a, b = last_cmp[0]
            src = _operand(args[1], params, bits=32)[0]; dst = _operand(args[0], params, bits=32)[0]
            if src is not None and dst is not None:
                w = _wb32(args[0], params, Call("__cmov", [Bin(_SETCC[setmn], a, b, 8), src, dst]))
                return [w] if w is not None else [("raw", insn)]
        return [("raw", insn)]
    if _is_simd(op):                                 # MMX/SSE vector op -> opaque helper
        return _lift_simd(op, args, params, 32)
    return [("raw", insn)]


def _lift16(insn, params, last_cmp):
    """Lift one 16-bit instruction to IR (the win1.0/win3.1 eras). Covers the common
    instruction set via the width-parametric operand parser / write-back (bits=16);
    anything else falls to a raw stub, same honesty model as the 32-bit path."""
    B = 16
    insn = re.sub(r"\s*->.*$", "", re.sub(r"\s+", " ", insn.strip().lower()))  # drop annotations
    op, _, rest = insn.partition(" ")
    args = [a.strip() for a in _split_ops(rest)] if rest else []
    if _is_data(insn):                                   # inline data, not un-lifted code
        return [("data", insn)]
    if (op in SEG or op in ("lock", "addr32", "data32", "addr16", "data16")) and rest:
        return _lift16(rest, params, last_cmp)           # a prefix, not an op
    if op in _FPU:                                        # x87 FPU (same helper model as 32-bit)
        return _lift_fpu(op, args, params, 16)
    opd = lambda a: _operand(a, params, bits=B)
    wb = lambda dst, v: _wb32(dst, params, v, bits=B)

    if op in ALU and len(args) == 2:
        d, dw = opd(args[0]); s, _ = opd(args[1])
        if d is None or s is None:
            return [("raw", insn)]
        result = Const(0, B) if op in ("xor", "sub") and args[0] == args[1] else Bin(ALU[op], d, s, B)
        w = wb(args[0], result)
        return _set_flags(result, d, s, ALU[op], B) + [w] if w is not None else [("raw", insn)]
    if op == "mov" and len(args) == 2:
        s, _ = opd(args[1])
        if s is None:
            return [("raw", insn)]
        w = wb(args[0], s)
        return [w] if w is not None else [("raw", insn)]
    if op in ("cmp", "test") and len(args) == 2:
        a, _ = opd(args[0]); b, _ = opd(args[1])
        if a is None or b is None:
            return [("raw", insn)]
        last_cmp[0] = (a, b)
        result = Bin("-" if op == "cmp" else "&", a, b, B)
        return _set_flags(result, a, b, "-" if op == "cmp" else "&", B)
    if op in ("inc", "dec") and len(args) == 1:
        d, _ = opd(args[0])
        if d is None:
            return [("raw", insn)]
        result = Bin("+" if op == "inc" else "-", d, Const(1, B), B)
        w = wb(args[0], result)
        return _set_flags(result, d, Const(1, B), op, B) + [w] if w is not None else [("raw", insn)]
    if op in ("neg", "not") and len(args) == 1:
        d, _ = opd(args[0])
        if d is None:
            return [("raw", insn)]
        result = Un("-" if op == "neg" else "~", d, B)
        w = wb(args[0], result)
        if w is None:
            return [("raw", insn)]
        return (_set_flags(result, Const(0, B), d, "-", B) + [w]) if op == "neg" else [w]
    if op in _SHIFT and len(args) == 2:
        d, _ = opd(args[0]); cnt, _ = opd(args[1])
        if d is None or cnt is None:
            return [("raw", insn)]
        result = Bin(_SHIFT[op], d, cnt, B)
        w = wb(args[0], result)
        return _set_flags(result, d, cnt, _SHIFT[op], B) + [w] if w is not None else [("raw", insn)]
    if op in ("shld", "shrd") and len(args) == 3:        # double-precision shift
        d, _ = opd(args[0]); s, _ = opd(args[1]); cnt, _ = opd(args[2])
        if d is None or s is None or cnt is None:
            return [("raw", insn)]
        rest = Bin("-", Const(B, B), cnt, B)
        result = (Bin("|", Bin("<<", d, cnt, B), Bin(">>", s, rest, B), B) if op == "shld"
                  else Bin("|", Bin(">>", d, cnt, B), Bin("<<", s, rest, B), B))
        w = wb(args[0], result)
        return [w] if w is not None else [("raw", insn)]
    if op in ("movzx", "movsx") and len(args) == 2:
        s, sw = opd(args[1])
        if s is None:
            return [("raw", insn)]
        if op == "movsx":
            m = 1 << ((sw or 8) - 1)
            s = Bin("-", Bin("^", s, Const(m, B), B), Const(m, B), B)
        w = wb(args[0], s)
        return [w] if w is not None else [("raw", insn)]
    if op == "lea" and len(args) == 2:
        d, _ = opd(args[0]); mm, _ = _operand("word ptr " + args[1], params, bits=B)
        if isinstance(mm, Mem):
            return [Set(d, mm.addr)] if d is not None else [("raw", insn)]
        if d is not None and mm is not None:
            return [Set(d, Un("&", mm, B))]
        return [("raw", insn)]
    if op in ("les", "lds", "lfs", "lgs", "lss") and len(args) == 2:
        d, _ = opd(args[0])                            # far ptr: 16-bit off + 16-bit seg
        memtok = re.sub(r"^(dword|fword|word)?\s*ptr\s+", "", args[1].strip())
        mm, _ = _operand("word ptr " + memtok, params, bits=B)
        seg = {"les": "es", "lds": "ds", "lfs": "fs", "lgs": "gs", "lss": "ss"}[op]
        if d is not None and isinstance(mm, Mem):
            off_wb = wb(args[0], Mem(mm.addr, B))
            if off_wb is not None:
                return [off_wb, Set(Reg(seg, 16), Mem(Bin("+", mm.addr, Const(2, B), B), 16))]
        off, sg = _far_ptr_slot(memtok, params, B)     # far ptr passed in a stack slot
        if d is not None and off is not None:
            off_wb = wb(args[0], off)
            if off_wb is not None:
                return [off_wb] + ([Set(Reg(seg, 16), sg)] if sg is not None else [])
        return [("raw", insn)]
    if op == "cbw":                                    # ax = sign-extend(al)
        al = Bin("&", Reg("ax", 16), Const(0xff, 16), 16); m = Const(0x80, 16)
        return [Set(Reg("ax", 16), Bin("-", Bin("^", al, m, 16), m, 16))]
    if op == "cwd":                                    # dx = sign-extend(ax) high half
        return [Set(Reg("dx", 16), Bin(">>s", Reg("ax", 16), Const(15, 16), 16))]
    if op == "cwde":                                   # 386-in-16: eax = sign-extend(ax)
        ax = Bin("&", Reg("eax", 32), Const(0xffff, 32), 32); m = Const(0x8000, 32)
        return [Set(Reg("eax", 32), Bin("-", Bin("^", ax, m, 32), m, 32))]
    if op == "cdq":
        return [Set(Reg("edx", 32), Bin(">>s", Reg("eax", 32), Const(31, 32), 32))]
    if op in ("adc", "sbb") and len(args) == 2:        # with carry: full CF-out + OF
        d, _ = opd(args[0]); s, _ = opd(args[1])
        if d is None or s is None:
            return [("raw", insn)]
        cf0, m = Reg("__cf0", 1), Const(0xffff, B)

        def _ovf(p, q, r, u):
            return Bin("&", Bin(">>", Bin("&", Bin("^", p, q, B), Bin("^", r, u, B), B),
                               Const(15, B), B), Const(1, B), 1)
        if op == "adc":
            result = Bin("+", Bin("+", d, s, B), cf0, B)
            cout = Bin("|", Bin("ucarry", d, s, B), Bin("ucarry", Bin("+", d, s, B), cf0, B), 1)
            of = _ovf(d, result, s, result)
        else:
            result = Bin("-", Bin("-", d, s, B), cf0, B)
            cout = Bin("|", Bin("borrow", d, s, B), Bin("borrow", Bin("-", d, s, B), cf0, B), 1)
            of = _ovf(d, s, d, result)
        w = wb(args[0], result)
        if w is None:
            return [("raw", insn)]
        return [Set(cf0, Flag("CF")),
                Set(Flag("ZF"), Bin("==", Bin("&", result, m, B), Const(0, B), 1)),
                Set(Flag("SF"), Bin("&", Bin(">>", result, Const(15, B), B), Const(1, B), 1)),
                Set(Flag("PF"), Un("parity", result, 1)),
                Set(Flag("CF"), cout), Set(Flag("OF"), of), w]
    if op == "xchg" and len(args) == 2:
        a, _ = opd(args[0]); b, _ = opd(args[1])
        if a is None or b is None:
            return [("raw", insn)]
        t = Reg("__xch", B)
        wa = wb(args[0], b); wbk = wb(args[1], t)
        return [Set(t, a), wa, wbk] if wa is not None and wbk is not None else [("raw", insn)]
    if op in ("rol", "ror") and len(args) == 2:
        d, _ = opd(args[0]); cnt, _ = opd(args[1])
        if d is None or cnt is None:
            return [("raw", insn)]
        rest = Bin("-", Const(B, B), cnt, B)
        result = (Bin("|", Bin("<<", d, cnt, B), Bin(">>", d, rest, B), B) if op == "rol"
                  else Bin("|", Bin(">>", d, cnt, B), Bin("<<", d, rest, B), B))
        w = wb(args[0], result)
        return [w] if w is not None else [("raw", insn)]
    if op in ("rcl", "rcr") and len(args) == 2:          # rotate through carry -> helper
        d, _ = opd(args[0]); cnt, _ = opd(args[1])
        if d is None or cnt is None:
            return [("raw", insn)]
        w = wb(args[0], Call("__" + op, [d, cnt, Flag("CF")]))
        return [w] if w is not None else [("raw", insn)]
    if op == "imul" and len(args) in (2, 3):
        ops = [opd(a)[0] for a in args]
        if any(o is None for o in ops):
            return [("raw", insn)]
        result = Bin("*", ops[1], ops[2], B) if len(args) == 3 else Bin("*", ops[0], ops[1], B)
        w = wb(args[0], result)
        return [w] if w is not None else [("raw", insn)]
    if op in ("mul", "imul") and len(args) == 1:       # dx:ax = ax * src
        s, _ = opd(args[0])
        if s is None:
            return [("raw", insn)]
        return [Set(Reg("dx", 16), Call("__" + op + "hi", [Reg("ax", 16), s])),
                Set(Reg("ax", 16), Bin("*", Reg("ax", 16), s, 16))]
    if op in ("div", "idiv") and len(args) == 1:       # ax=quot, dx=rem of dx:ax / src
        s, _ = opd(args[0])
        if s is None:
            return [("raw", insn)]
        hi, lo = Reg("__dvh", 16), Reg("__dvl", 16)     # snapshot dx,ax before overwrite
        return [Set(hi, Reg("dx", 16)), Set(lo, Reg("ax", 16)),
                Set(Reg("ax", 16), Call("__" + op, [hi, lo, s])),
                Set(Reg("dx", 16), Call("__" + op + "_rem", [hi, lo, s]))]
    m_str = re.match(r"^(movs|stos|lods|scas)([bw])$", op)
    if m_str or (op in ("rep", "repe", "repz", "repne", "repnz")
                 and args and re.match(r"^(movs|stos|lods|scas)[bw]$", args[0])):
        return _string_op16(op if m_str else args[0], None if m_str else op)
    if op in ("in", "out") and len(args) == 2:         # port I/O
        port = args[1] if op == "in" else args[0]
        a = args[0] if op == "in" else args[1]
        p, _ = opd(port)
        if op == "in":
            w = wb(a, Call("__inp", [p] if p is not None else []))
            return [w] if w is not None else [("raw", insn)]
        av, _ = opd(a)
        return [Call("__outp", [x for x in (p, av) if x is not None])]
    if op in ("int3", "int1", "icebp", "ud2"):
        return [Call("__debugbreak", [])]
    if op in ("sysenter", "sysexit", "syscall", "sysret", "rsm", "cpuid", "rdtsc"):
        return [Call("__" + op, [])]
    if op == "int" and len(args) == 1:
        n, _ = opd(args[0])
        return [Call("__int", [n])] if n is not None else [("raw", insn)]
    if op in ("iret", "iretw", "iretd"):
        return [Ret(Reg("ax", 16))]
    if op == "bswap" and len(args) == 1:
        w = wb(args[0], Call("__bswap", [opd(args[0])[0]]))
        return [w] if w is not None else [("raw", insn)]
    if op == "arpl" and len(args) == 2:
        d, _ = opd(args[0]); s, _ = opd(args[1])
        w = wb(args[0], Call("__arpl", [d, s])) if d is not None and s is not None else None
        return [w] if w is not None else [("raw", insn)]
    if op in _SETCC and len(args) == 1 and last_cmp[0] is not None:   # dst = (a <op> b)
        a, b = last_cmp[0]
        w = wb(args[0], Bin(_SETCC[op], a, b, 8))
        return [w] if w is not None else [("raw", insn)]
    if op in ("bt", "bts", "btr", "btc") and len(args) == 2:
        d, _ = opd(args[0]); bit, _ = opd(args[1])
        if d is None or bit is None:
            return [("raw", insn)]
        cf = [Set(Flag("CF"), Bin("&", Bin(">>", d, bit, B), Const(1, B), 1))]
        if op == "bt":
            return cf
        one = Bin("<<", Const(1, B), bit, B)
        res = {"bts": Bin("|", d, one, B), "btr": Bin("&", d, Un("~", one, B), B),
               "btc": Bin("^", d, one, B)}[op]
        w = wb(args[0], res)
        return cf + [w] if w is not None else cf
    if op in ("clc", "stc", "cmc"):                    # carry-flag ops
        return [Set(Flag("CF"), Const(0, 1) if op == "clc" else
                    Const(1, 1) if op == "stc" else Un("!", Flag("CF"), 1))]
    if op in ("sahf", "lahf"):                          # AH<->flags transfer
        return [Call("__" + op, [])]
    if op in ("daa", "das", "aaa", "aas"):              # BCD adjust of al/ax
        w = wb("al" if op in ("daa", "das") else "ax", Call("__" + op, [Reg("ax", 16)]))
        return [w] if w is not None else [("raw", insn)]
    if op in ("aad", "aam"):
        base = opd(args[0])[0] if args else Const(10, 16)
        w = wb("ax", Call("__" + op, [Reg("ax", 16), base]))
        return [w] if w is not None else [("raw", insn)]
    if op == "bound" and len(args) == 2:
        a, _ = opd(args[0]); b, _ = opd(args[1])
        return [Call("__bound", [a, b])] if a is not None and b is not None else [("raw", insn)]
    if op in ("lock", "addr32", "data32", "addr16", "data16") and rest:
        return _lift16(rest, params, last_cmp)          # a prefix, not an op
    if op in ("xlat", "xlatb"):                          # al = [ds:bx + al]
        idx = Bin("+", Reg("bx", 16), Bin("&", Reg("ax", 16), Const(0xff, 16), 16), 16)
        w = wb("al", Mem(idx, 8))
        return [w] if w is not None else [("raw", insn)]
    ps = re.match(r"^(outs|ins)([bwd]?)$", op)          # port string I/O
    if ps or (op in ("rep", "repe", "repz", "repne", "repnz")
              and args and re.match(r"^(outs|ins)[bwd]?$", args[0])):
        return _port_string(op if ps else args[0], None if ps else op, 16)
    cs = re.match(r"^cmps([bwd])$", op)                 # string compare (si vs di)
    if cs or (op in ("rep", "repe", "repz", "repne", "repnz")
              and args and re.match(r"^cmps[bwd]$", args[0])):
        return _string_op16(op if cs else args[0], None if cs else op)
    if op in ("ret", "retn", "retf", "retd", "retfd", "retnd", "retfw", "retnw"):
        return [Ret(Reg("ax", 16))]                       # retd/retfd = 32-bit operand-size returns
    if op in ("lar", "lsl") and len(args) == 2:           # load access-rights / segment-limit
        s, _ = opd(args[1])
        w = wb(args[0], Call("__" + op, [s])) if s is not None else None
        return [w] if w is not None else [("raw", insn)]
    if op in ("bsf", "bsr") and len(args) == 2:           # bit scan fwd/rev
        s, _ = opd(args[1])
        w = wb(args[0], Call("__" + op, [s])) if s is not None else None
        return [w] if w is not None else [("raw", insn)]
    if op == "into":                                      # INT 4 if OF set (signed-overflow trap)
        return [Call("__into", [])]
    if op == "nop" or op.startswith("align") or op in ("cld", "std", "sti", "cli",
                                                        "wait", "fwait", "hlt", "nop66"):
        return []
    if op in ("push", "pop", "pusha", "popa", "pushf", "popf", "leave", "enter",
              "pushd", "popd", "pushad", "popad", "popal", "pushal", "pushfd", "popfd",
              "pushaw", "popaw"):
        return []                                    # frame boilerplate (sp/bp modeled away)
    if op.startswith("cmov") and len(args) == 2 and last_cmp[0] is not None:  # dst = cond ? src : dst
        setmn = "set" + op[4:]
        if setmn in _SETCC:
            a, b = last_cmp[0]
            src = _operand(args[1], params, bits=16)[0]; dst = _operand(args[0], params, bits=16)[0]
            if src is not None and dst is not None:
                w = _wb32(args[0], params, Call("__cmov", [Bin(_SETCC[setmn], a, b, 8), src, dst]), bits=16)
                return [w] if w is not None else [("raw", insn)]
        return [("raw", insn)]
    if _is_simd(op):                                 # MMX/SSE vector op -> opaque helper
        return _lift_simd(op, args, params, 16)
    return [("raw", insn)]


def _string_op16(mn, rep):
    """16-bit si/di string primitives (movsw/stosw/lodsw/scasw + byte forms). DF
    assumed forward; a rep prefix -> a __rep_* block helper (see _string_op)."""
    base, suf = mn[:4], mn[4]
    W, n = (16, 2) if suf == "w" else (8, 1)
    si, di = Reg("si", 16), Reg("di", 16)
    acc = Reg("ax", 16) if W == 16 else Bin("&", Reg("ax", 16), Const(0xff, 16), 16)
    bump = lambda r: Set(r, Bin("+", r, Const(n, 16), 16))
    if rep:
        ptrs = {"movs": [di, si], "stos": [di], "lods": [si], "scas": [di],
                "cmps": [si, di]}[base]
        return [Call("__rep_%s" % mn, [Reg("cx", 16)] + ptrs)]
    if base == "movs":
        return [Set(Mem(di, W), Mem(si, W)), bump(si), bump(di)]
    if base == "stos":
        return [Set(Mem(di, W), acc), bump(di)]
    if base == "lods":
        return [_wb32("al" if W == 8 else "ax", {}, Mem(si, W), bits=16), bump(si)]
    if base == "scas":
        return _set_flags(Bin("-", acc, Mem(di, W), W), acc, Mem(di, W), "-", W) + [bump(di)]
    if base == "cmps":                                 # cmp [si], [di]; si += n; di += n
        return _set_flags(Bin("-", Mem(si, W), Mem(di, W), W),
                          Mem(si, W), Mem(di, W), "-", W) + [bump(si), bump(di)]
    return [("raw", mn)]


def _port_string(mn, rep, bits):
    """Port string I/O: outs* -> `out dx, [si]`; ins* -> `[di] = in dx` (+ ptr bump).
    A rep prefix collapses to a __rep_* block helper, like the memory string ops."""
    m = re.match(r"(outs|ins)([bwd]?)", mn)
    io, suf = m.group(1), m.group(2)
    W, n = {"b": (8, 1), "w": (16, 2), "d": (32, 4), "": (8, 1)}[suf]
    si = Reg("si" if bits == 16 else "esi", bits)
    di = Reg("di" if bits == 16 else "edi", bits)
    cx = Reg("cx" if bits == 16 else "ecx", bits)
    dx = Reg("dx", 16)
    bump = lambda r: Set(r, Bin("+", r, Const(n, bits), bits))
    if rep:
        return [Call("__rep_%s" % mn, [cx, dx, si if io == "outs" else di])]
    if io == "outs":
        return [Call("__outp", [dx, Mem(si, W)]), bump(si)]
    return [Set(Mem(di, W), Call("__inp", [dx])), bump(di)]


# esi/edi string primitives. DF is assumed forward (the overwhelmingly common
# `cld` state); a `rep` prefix becomes a __rep_* block-move helper call rather than
# an inlined loop (keeps _lift32 straight-line; the helper reads as one operation).
def _string_op(mn, rep):
    base, suf = mn[:4], mn[4]
    W, n = {"b": (8, 1), "w": (16, 2), "d": (32, 4)}[suf]
    esi, edi = Reg("esi", 32), Reg("edi", 32)
    acc = (Reg("eax", 32) if W == 32 else
           Bin("&", Reg("eax", 32), Const((1 << W) - 1, 32), 32))
    bump = lambda r: Set(r, Bin("+", r, Const(n, 32), 32))
    if rep:                                            # whole block in one helper call
        ptrs = {"movs": [edi, esi], "stos": [edi], "lods": [esi], "scas": [edi],
                "cmps": [esi, edi]}[base]
        return [Call("__rep_%s" % mn, [Reg("ecx", 32)] + ptrs)]
    if base == "movs":
        return [Set(Mem(edi, W), Mem(esi, W)), bump(esi), bump(edi)]
    if base == "stos":
        return [Set(Mem(edi, W), acc), bump(edi)]
    if base == "lods":
        wb = _wb32({8: "al", 16: "ax", 32: "eax"}[W], {}, Mem(esi, W))
        return [wb, bump(esi)]
    if base == "scas":                                 # cmp acc, [edi]; edi += n
        return _set_flags(Bin("-", acc, Mem(edi, W), W), acc, Mem(edi, W), "-", W) + [bump(edi)]
    if base == "cmps":                                 # cmp [esi], [edi]; esi/edi += n
        return _set_flags(Bin("-", Mem(esi, W), Mem(edi, W), W),
                          Mem(esi, W), Mem(edi, W), "-", W) + [bump(esi), bump(edi)]
    return [("raw", mn)]


def _split_ops(rest):
    """Split an operand list on commas that are not inside brackets."""
    out, depth, cur = [], 0, ""
    for ch in rest:
        if ch == "[":
            depth += 1
        elif ch == "]":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur); cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur)
    return out


_JCC = {"je": "==", "jz": "==", "jne": "!=", "jnz": "!=",
        "jl": "<", "jnge": "<", "jle": "<=", "jng": "<=",
        "jg": ">", "jge": ">=", "jnl": ">=",
        "jb": "< (unsigned)", "jbe": "<= (unsigned)",
        "ja": "> (unsigned)", "jae": ">= (unsigned)", "jnb": ">= (unsigned)"}


def parse_asm(path):
    """Our reconstructed .asm -> (rows[(off,insn)], labels{name:off}). Handles the
    `db ...; <addr> <real instruction>` escape (bytes we emit verbatim carry the
    decoded instruction in the trailing comment)."""
    rows, labels = [], {}
    in_macro = False
    for line in open(path, encoding="latin1"):
        s = line.rstrip("\n")
        st = s.strip()
        # skip MASM macro definitions (e.g. the `retf macro n / ifb / else / dw /
        # endif / endm` boilerplate that heads most functions) -- not instructions.
        if in_macro:
            if st.lower() == "endm":
                in_macro = False
            continue
        if re.match(r"^\w+\s+macro\b", st, re.I):
            in_macro = True
            continue
        if not st or re.match(r"(?i)(title|page|assume|\.\d86|\.model|public|extrn)\b", st) \
                or re.search(r"(?i)\bsegment\b", st) or st.endswith("ends") or st == "end":
            continue                                  # assembler directives, not instructions
        m = re.match(r"^(L_[0-9a-fA-F]+):", st)
        if m:
            labels[m.group(1)] = None
            rows.append(("label", m.group(1)))
            continue
        if st.startswith("db "):                     # verbatim-bytes escape
            cm = st.split(";", 1)
            if len(cm) == 2:
                toks = cm[1].split(None, 1)
                if len(toks) == 2:
                    try:                              # comment = "<off> <disasm>"
                        off = int(toks[0], 16)
                    except ValueError:                # malformed offset -> keep the insn anyway
                        off = -1
                    rows.append((off, toks[1].strip()))
            continue
        code, _, comment = st.partition(";")
        code = code.strip()
        if not code:
            continue
        off = None
        mo = re.match(r"^([0-9a-fA-F]{3,})\b", comment.strip())
        if mo:
            off = int(mo.group(1), 16)
        rows.append((off if off is not None else -1, code))

    # The disassembler occasionally carves a function-terminal `ret`/`retf` as data
    # (e.g. `.byte 0cah / .byte 2h` for `retf 2`). Collapse those opcode bytes back
    # into the return mnemonic so the lifter emits a real `return` (not a data stub).
    def _bval(insn):
        m = re.match(r"\.byte\s+([0-9a-fA-F]+)h?\s*$", str(insn).strip())
        return int(m.group(1), 16) if m else None
    fixed, i = [], 0
    while i < len(rows):
        off, insn = rows[i]
        b = _bval(insn)
        if b in (0xc3, 0xcb):                          # ret / retf (no operand)
            fixed.append((off, "retf" if b == 0xcb else "ret")); i += 1; continue
        if b in (0xc2, 0xca):                          # ret / retf imm16 (2 op bytes)
            lo = _bval(rows[i + 1][1]) if i + 1 < len(rows) else None
            hi = _bval(rows[i + 2][1]) if i + 2 < len(rows) else None
            if lo is not None:
                n = lo | ((hi or 0) << 8)
                fixed.append((off, ("retf %d" if b == 0xca else "ret %d") % n))
                i += 3 if hi is not None else 2; continue
            fixed.append((off, "retf" if b == 0xca else "ret"))   # operand not carved
            i += 1; continue
        fixed.append((off, insn)); i += 1
    return fixed


_VXD_DEVIDS = None


def _vxd_devids():
    """{device_id: NAME} from the VxD device-ID table (vmmsvc, win95-only). Empty when
    vmmsvc isn't importable (non-win95 targets) -- which disables VxDCall recovery, the
    intended no-op there. Lazy + cached so il.py stays import-order-independent."""
    global _VXD_DEVIDS
    if _VXD_DEVIDS is None:
        try:
            import vmmsvc
            _VXD_DEVIDS = {v: k for k, v in vmmsvc.DEVICE_IDS.items()}
        except Exception:
            _VXD_DEVIDS = {}
    return _VXD_DEVIDS


def _asm_byte_map(path):
    """{offset: [raw bytes]} for the reconstructed .asm -- the byte column parse_asm drops.
    Normal lines carry it in the trailing `; <off> <hex bytes>` comment; the `db` escape
    carries it in its operand list. Best-effort: used only to read data that trails an
    instruction (a VxDCall service dword)."""
    out = {}
    for line in open(path, encoding="latin1"):
        st = line.strip()
        if not st:
            continue
        code, _, comment = st.partition(";")
        cm = comment.strip().split()
        if not cm:
            continue
        try:
            off = int(cm[0], 16)
        except ValueError:
            continue
        if code.strip().lower().startswith("db "):        # bytes are the db operands
            bs = []
            for t in re.findall(r"[0-9a-fA-F]+", code.split(None, 1)[1] if " " in code else ""):
                try:
                    bs.append(int(t, 16) & 0xff)
                except ValueError:
                    pass
            if bs:
                out[off] = bs
        else:                                             # comment = "<off> <b0> <b1> ..."
            bs = []
            for t in cm[1:]:
                if re.fullmatch(r"[0-9a-fA-F]{2}", t):
                    bs.append(int(t, 16))
                else:
                    break
            if bs:
                out[off] = bs
    return out


def _read_dword(bmap, start):
    """The little-endian dword at file offset `start`, gathered from the byte map's
    consecutive rows (a mis-decoded inline dword spans 1-4 rows). None if incomplete."""
    bs, o, guard = [], start, 0
    while len(bs) < 4 and guard < 8:
        chunk = bmap.get(o)
        if not chunk:
            return None
        bs.extend(chunk)
        o += len(chunk)
        guard += 1
    if len(bs) < 4:
        return None
    return bs[0] | (bs[1] << 8) | (bs[2] << 16) | (bs[3] << 24)


def _resolve_vxdcalls(rows, path):
    """Rewrite each `int 20h` VxD service call -> a named `call vxd:<DEVICE>_<index>`.
    A VxD invokes a service with `int 20h` (cd 20) immediately followed by an INLINE dword
    (Device_ID<<16)|index, which the linear disassembler mis-decodes as 4 code bytes. We
    read that dword from the raw bytes, name the target from the device-ID table -- ONLY
    when the device is RECOGNISED (a self-validating gate: a stray 32-bit `int 20h` in a
    non-VxD image has random trailing bytes that won't match, and 16-bit `int 20h` -- DOS
    terminate -- never reaches here, bits==32 only) -- and drop the 4 data rows (the
    disassembler always realigns immediately after them). The service entry points are
    exported as <DEVICE>_ord_<index> via the DDB service table (link95.sym_maps_le), so
    these calls resolve cross-module. Returns rows unchanged when nothing matches."""
    devid = _vxd_devids()
    if not devid:
        return rows
    bmap = None
    out, drop_until = [], None
    for off, insn in rows:
        if drop_until is not None and isinstance(off, int) and 0 <= off < drop_until:
            continue                                      # a service-dword byte row: drop it
        drop_until = None
        if isinstance(off, int) and re.fullmatch(r"int\s+20h", insn.strip(), re.I):
            if bmap is None:
                bmap = _asm_byte_map(path)
            svc = _read_dword(bmap, off + 2)
            if svc is not None:
                # low word is the service index; bit 15 (0x8000) is a call flag some VxDs set
                # (e.g. CONFIGMG), never part of the index (service tables are < 4000 entries),
                # so mask it to match the 0-based DDB service-table index the export side uses.
                dev, ix = (svc >> 16) & 0xffff, svc & 0x7fff
                nm = devid.get(dev)
                if nm:
                    out.append((off, "call vxd:%s_%d" % (nm, ix)))
                    drop_until = off + 6
                    continue
        out.append((off, insn))
    return out


def lift_asm_function(path, name=None, faithful=False, clean=False, reg_inputs=None):
    """Lift a reconstructed .asm function -> structured C. Returns the C source.

    faithful=False (default): the READABLE path -- cross-block dead-store elimination
    prunes register/local defs whose value never escapes, giving clean decompiler C.
    This is what the reconstructed-C ledger renders; its output must stay byte-stable
    (goldens.py guards it).

    faithful=True: the LOSSLESS path for byte-exact recompilation. DSE is skipped so
    EVERY value-producing instruction survives into the C -- no dropped register defs
    (`mov esi,ecx`), no pruned write-only frame locals. Flag clouds are still elided
    (implicit in C / recomputed by the compiler) and copy-prop still runs (it never
    drops a def), so the output is complete but not swamped. This is the material the
    raiser needs: you cannot recompile to the exact bytes from C that is missing
    instructions. See engine/raiser.py."""
    rows = parse_asm(path)
    params = {}
    # 32-bit segments carry `use32`; its absence marks the 16-bit eras (win1.0/3.1).
    bits = 32 if "use32" in open(path, encoding="latin1").read(2000) else 16
    if bits == 32:                                     # VxD `int 20h`+dword -> named service call
        rows = _resolve_vxdcalls(rows, path)
    acc = "eax" if bits == 32 else "ax"                # accumulator / return register

    def _lift(txt):
        return (_lift16 if bits == 16 else _lift32)(txt, params, last_cmp)

    # SYNTHESIZE a label at every jump target that lands on a real instruction but has none.
    # il.py used to leave such `jmp 3e7h` as an "external" computed jump (a huge defect class);
    # but an intra-function jump to a valid instruction offset is just an unlabelled goto. Adding
    # the missing label turns `goto *(void*)0x3e7 /* external */` into a clean `goto L_003e7`.
    _pfx = re.compile(r"^\s*((es|cs|ss|ds|fs|gs|lock|addr32|data32|addr16|data16)\s+)+", re.I)
    _insn_off = set(t for t, _v in rows if t != "label" and isinstance(t, int) and t >= 0)
    _have = set()
    for _t, _v in rows:
        if _t == "label":
            try:
                _have.add(int(_v[2:], 16))
            except ValueError:
                pass
    _targets = set()
    for _t, _v in rows:
        if _t == "label":
            continue
        _low = _pfx.sub("", _v).strip().lower().split()
        if _low and (_low[0] in COND_JUMPS or _low[0] in ("jmp", "ljmp", "jmpf")):
            _m = re.match(r"(?i)^([0-9a-f]+)h?$", _pfx.sub("", _v).split()[-1])
            if _m:
                try:
                    _targets.add(int(_m.group(1), 16))
                except ValueError:
                    pass
    _need = (_targets & _insn_off) - _have
    if _need:
        _nr = []
        for _r in rows:
            if _r[0] != "label" and isinstance(_r[0], int) and _r[0] in _need:
                _nr.append(("label", "L_%05x" % _r[0]))
            _nr.append(_r)
        rows = _nr
    # map code offsets -> their L_ label, to resolve bare-hex jump targets: the db
    # escape writes `jmp 3e7h` (a raw offset) rather than `jmp L_003e7`.
    label_off = {}
    for _tag, _val in rows:
        if _tag == "label":
            try:
                label_off[int(_val[2:], 16)] = _val
            except ValueError:
                pass

    def _resolve(tok):
        """Jump-target token -> label name. L_ labels pass through; a bare-hex offset
        maps to its label if one exists here, else stays (external/unlabelled target)."""
        if re.match(r"(?i)^L_[0-9a-f]+$", tok):
            return tok
        v = _imm2(tok)
        return label_off.get(v.val, tok) if v is not None else tok
    # ---- split into basic blocks; terminators are jcc/jmp/ret. Along the way,
    #      recover call arguments: prologue register-saves are skipped, body
    #      `push`es accumulate as pending args, and a `call` consumes them
    #      (reversed: args are pushed right-to-left) into a Call expression. ----
    last_cmp = [None]
    blocks = []
    cur_name, cur = "entry", []
    pending = []          # push'd values awaiting the next call
    in_prologue = True     # leading push-saves / frame setup are not arguments

    def _boundary():       # control-flow edge: pending args cannot cross it
        pending.clear()

    for row in rows:
        tag, val = row
        if tag == "label":
            blocks.append([cur_name, cur, ("fall", val)])
            cur_name, cur = val, []
            in_prologue = False
            _boundary()
            continue
        off, txt = tag, val
        # strip leading segment / size prefixes here so a PREFIXED terminator
        # (e.g. `gs jae 1308h`, `es jmp ..`) is still recognised as a jcc/jmp/ret.
        txt = re.sub(r"^\s*((es|cs|ss|ds|fs|gs|lock|addr32|data32|addr16|data16)\s+)+",
                     "", txt, flags=re.I)
        low = re.sub(r"\s+", " ", txt.strip().lower())
        op = low.split()[0] if low.split() else ""
        if op in COND_JUMPS:
            tgt = _resolve(txt.split()[-1])
            blocks.append([cur_name, cur, ("cond", CANON.get(op, op), tgt)])
            cur_name, cur = "_after_%s" % tgt, []
            _boundary()
            continue
        if op in ("jmp", "ljmp", "jmpf"):
            tgt = _resolve(txt.split()[-1])
            if re.match(r"(?i)^L_[0-9a-f]+$", tgt):        # direct jump to a known label
                blocks.append([cur_name, cur, ("goto", tgt)])
            else:                                          # computed / external / far / jump-table
                operand = low.partition(" ")[2].strip().split(",")[-1].strip()
                node, _ = _operand(operand, params, bits=bits)
                blocks.append([cur_name, cur, ("ijmp", node)])
            cur_name, cur = "_dead", []
            _boundary()
            continue
        if op in ("loop", "loopw", "loopd", "loope", "loopz", "loopne", "loopnz"):
            cxr = "cx" if bits == 16 else "ecx"          # cx--; if (cx != 0) goto tgt
            tgt = _resolve(txt.split()[-1])              # (loope/loopne's ZF refinement dropped)
            cur.append(Set(Reg(cxr, bits), Bin("-", Reg(cxr, bits), Const(1, bits), bits)))
            blocks.append([cur_name, cur, ("cond", "jne", tgt)])
            cur_name, cur = "_after_%s" % tgt, []
            _boundary()
            continue
        if op in ("jcxz", "jecxz"):                      # if (cx == 0) goto tgt
            tgt = _resolve(txt.split()[-1])
            blocks.append([cur_name, cur, ("cond", "je", tgt)])
            cur_name, cur = "_after_%s" % tgt, []
            _boundary()
            continue
        if op in ("ret", "retn", "retf", "retd", "retfd", "retnd", "retfw", "retnw"):
            cur.extend(_lift(txt))
            blocks.append([cur_name, cur, ("ret",)])
            cur_name, cur = "_dead", []
            _boundary()
            continue
        # ---- prologue: skip register saves + frame setup, don't treat as args --
        if in_prologue:
            if (op == "push" and _is_csr(low, bits)) or re.match(r"mov e?bp, ?e?sp", low) \
                    or re.match(r"sub e?sp,", low) or op == "enter" \
                    or (bits == 16 and op == "inc" and low.split()[-1] == "bp"):
                continue
            in_prologue = False
        # ---- call-argument recovery ------------------------------------------
        if op == "push":
            node, _w = _operand(low.partition(" ")[2].strip(), params, bits=bits)
            if node is not None:
                pending.append(node)
            continue
        if op == "pop":
            continue
        if op in ("call", "lcall", "callf"):             # near/far call
            operand = low.partition(" ")[2].strip()
            operand = operand.split(",")[-1].strip() if op != "call" else operand  # far: seg,off
            tgt = _call_target(operand, params, bits)
            args = list(reversed(pending))
            pending.clear()
            cur.append(Set(Reg(acc, bits), Call(tgt, args)))
            continue
        cur.extend(_lift(txt))
    if cur or cur_name not in [b[0] for b in blocks]:
        blocks.append([cur_name, cur, ("end",)])

    # ---- per-block optimisation (block-local const/copy-prop + fold) ----------
    for b in blocks:
        b[1] = const_copy_prop(b[1], guard_addr=faithful)
        b[1] = dead_flag_elimination(b[1])   # flags unused (relational conds) -> gone
    # ---- global register liveness -> safe cross-block dead-store elimination ---
    # (faithful mode keeps live_out for the SSA rename below, but does NOT prune:
    #  losslessness for byte-exact recompilation trumps readability here.)
    conds0 = _recover_conditions(rows, params, bits)
    live_out, live_in = _reg_liveness(blocks, conds=conds0)
    if not faithful:
        # A block's own conditional branch reads its cmp operands, so those registers are
        # live at the block's end -- even when EVERY successor redefines them first (then
        # they are absent from live_out). Seed DSE with them too, or the store feeding the
        # branch is wrongly dropped -> use-before-def. This is the dropped-subregister-load
        # class (e.g. mmfmig32 0x16830: `mov cl,[ebp+9]; cmp ..,cl; jbe` where the next
        # block reloads cl, so cl left live_out and the first load vanished).
        cond_reads, ci = {}, 0
        for (nm, _st, term) in blocks:
            if term[0] == "cond":
                cc = conds0[ci] if ci < len(conds0) else None
                ci += 1
                rd = set()
                if cc and cc[2]:
                    _reads_regs(cc[2][0], rd); _reads_regs(cc[2][1], rd)
                cond_reads[nm] = rd
        for b in blocks:
            b[1] = _dse_blocklocal(b[1], live_out.get(b[0], set()) | cond_reads.get(b[0], set()))

    # ---- structurer: recover if-then for `cond` blocks whose fall body rejoins -
    names = [b[0] for b in blocks]
    idx = {n: k for k, n in enumerate(names)}
    # recompute relational conditions per cond block from its own last cmp
    # (re-lift to capture cmp operands in order)
    conds = _recover_conditions(rows, params, bits)

    # ---- light type recovery: pointer/struct params + void/int return --------
    ptr_params, structs, produces_eax = _recover_types(blocks, acc)
    if clean:
        produces_eax = True                          # DEREGISTERIZED: eax is a LOCAL, so a caller that
        # reads eax after the call must capture the RETURN (`eax = sub(...)`). The register-machine
        # model threads eax via the global, so it can be void there -- but clean fns must return int.
    if not produces_eax:                             # no eax result -> void; drop `return eax`
        for b in blocks:
            b[1] = [Ret(None) if isinstance(s, Ret) else s for s in b[1]]
    # a struct param gets a synthesised type `struct <fn>_argN {..}` and renders as
    # argN->f<off>; the emitter learns the struct-param names via _STRUCT_PTRS.
    fn = name or ("fn_" + re.sub(r"\W", "_", path.split("/")[-1].split(".")[0]))
    struct_decls, ptype = [], {}
    for p, flds in sorted(structs.items()):
        sname = "%s_%s" % (fn, p)
        members = "".join(" u%d f%x;" % (min(w, 32), off) for off, w in sorted(flds.items()))
        struct_decls.append("struct %s {%s };" % (sname, members))
        ptype[p] = "struct %s *%s" % (sname, p)
    global _STRUCT_PTRS
    _STRUCT_PTRS = set(structs)

    # ---- SSA-lite: rename block-local register temporaries to distinct v<N> ----
    # skip anything a rename could desync: regs feeding a condition (conds carry
    # register names), and regs with a subregister access (the al/ah/ax peephole
    # keys off the parent's name).
    cond_regs = set()
    for _t, _mn, cmpops in conds:
        if cmpops:
            for nd in cmpops[:2]:
                if nd is not None:
                    cond_regs |= {x.name for x in _walk(nd) if isinstance(x, Reg)}
    for b in blocks:                                  # regs read by a computed jump
        if b[2][0] == "ijmp" and b[2][1] is not None and not isinstance(b[2][1], Const):
            cond_regs |= {x.name for x in _walk(b[2][1]) if isinstance(x, Reg)}
    subreg_regs = set()
    _sr = re.compile(r"\b(%s)\b" % "|".join(set(BREG32) | set(WSUB) | set(BREGS)))
    for row in rows:
        if row[0] == "label":
            continue
        for tok in _sr.findall(row[1].lower()):
            if tok in BREG32:
                subreg_regs.add(BREG32[tok][0])
            if tok in WSUB:
                subreg_regs.add(WSUB[tok])
            if tok in BREGS:
                subreg_regs.add(BREGS[tok][0])
    _ssa_rename(blocks, live_out, cond_regs | subreg_regs, keep_locs=faithful)

    body = _emit_structured(blocks, conds)
    txt = "\n".join(body)
    decls = sorted(set(re.findall(r"\bloc_[0-9a-f]+\b", txt)), key=lambda s: int(s[4:], 16)) \
        + sorted(set(re.findall(r"\bv\d+\b", txt)), key=lambda s: int(s[1:]))
    reg_params = []
    if clean:
        # DEREGISTERIZE -- undo the compiler's register allocation. A GP register that is NOT a
        # genuine INPUT is a pure local: declare it function-local so the lift's global eax..edi (and
        # their lane macros al/ah/ax = *(u8/u16*)&eax) resolve to it by name-shadowing, making the fn
        # self-contained. A genuine input becomes a real register-PARAM. Which registers are inputs:
        # `reg_inputs` when supplied (the empirical unicorn detector -- EVIDENCE, not guess); else
        # fall back to entry live-in (static, conservative). frame/seg regs stay global.
        _LANES = {"eax": ("al", "ah", "ax"), "ebx": ("bl", "bh", "bx"), "ecx": ("cl", "ch", "cx"),
                  "edx": ("dl", "dh", "dx"), "esi": ("si",), "edi": ("di",)}
        def _parent(r):
            for _p, _ls in _LANES.items():
                if r == _p or r in _ls:
                    return _p
            return r
        input_parents = ({_parent(r) for r in reg_inputs} if reg_inputs is not None
                         else {_parent(r) for r in live_in.get("entry", set())})
        used = [p for p, ls in _LANES.items() if re.search(r"\b(%s)\b" % "|".join((p,) + ls), txt)]
        decls = decls + sorted(p for p in used if p not in input_parents)   # -> function locals
        reg_params = sorted(p for p in used if p in input_parents)          # -> real register inputs
    C = list(struct_decls)
    sig_names, _seen = [], set()                     # sub-dword slots share an arg name;
    for p in reg_params:                             # genuine register inputs (ecx=this, fastcall, ..)
        sig_names.append("u32 %s" % p)
    for d in sorted(params):                         # then the recovered stack args, each once
        nm = params[d]
        if nm not in _seen:
            _seen.add(nm)
            # `char *` (not `void *`) for raw pointer params: the lift does byte-arithmetic on them
            # (`argN + off`), which gcc allows on void* but MSVC rejects (C2036 'void*: unknown size').
            sig_names.append(ptype.get(nm, ("char *%s" if nm in ptr_params else "u32 %s") % nm))
    sig = ", ".join(sig_names) or "void"
    C.append("%s %s(%s)" % ("int" if produces_eax else "void", fn, sig))
    C.append("{")
    if decls:                                         # recovered stack locals + SSA temps (+ clean reg-locals)
        C.append("    u32 %s;" % ", ".join(decls))
    for ln in body:
        C.append("    " + ln)
    C.append("}")
    return "\n".join(C)


# dest-writing ALU ops whose result lands in operand 0; after them the ZF/SF a
# following jcc reads is a test of that result against zero (`dec ecx; jnz` etc).
# CF/OF are also set but are NOT a value-vs-zero relation, so a "zero" condition
# is only surfaced to the ZF/SF jumps below -- carry/overflow jumps stay honest.
_ZERO_ALU = {"add", "sub", "or", "and", "xor", "adc", "sbb", "inc", "dec", "neg",
             "shl", "sal", "shr", "sar"}
_ZERO_JUMPS = {"je", "jne", "js", "jns"}                 # decide purely on ZF/SF


def _walk(node):
    """Yield node and every sub-expression (uses each node's children())."""
    yield node
    for c in node.children():
        yield from _walk(c)


def _recover_types(blocks, acc="eax"):
    """Light type recovery from the lifted IR. Returns (ptr_params, structs, produces_eax):
      * ptr_params -- params used as the BASE of a memory access (`void *argN`).
      * structs -- {param: {offset: width}} for params accessed ONLY at constant
        offsets: those become a synthesised struct (argN->f<off>). A param with any
        indexed/computed access (array-like) stays a plain void* (not in structs).
      * produces_eax -- the function computes a return value. True if any statement
        assigns eax, OR any `return` yields something other than the bare ENTRY eax
        (copy-prop folds `eax=ecx; return eax` into `return ecx`, dropping the eax
        store, so the store alone isn't reliable). `return eax` where eax was never
        redefined means eax is the untouched entry value -> void."""
    fields, complex_ = {}, set()                    # param -> {off: width}; array-like params
    produces_eax = False
    for _n, stmts, _t in blocks:
        for st in stmts:
            exprs = []
            if isinstance(st, Set):
                exprs.append(st.expr)
                if isinstance(st.dst, Mem):
                    exprs.append(st.dst.addr)
                elif isinstance(st.dst, Reg) and st.dst.name == acc:
                    produces_eax = True
            elif isinstance(st, Ret) and st.expr is not None:
                exprs.append(st.expr)
                if not (isinstance(st.expr, Reg) and st.expr.name == acc):
                    produces_eax = True             # returns a real value, not entry acc
            for e in exprs:
                for nd in _walk(e):
                    if not isinstance(nd, Mem):
                        continue
                    base = nd.addr
                    if isinstance(base, Reg) and base.name.startswith("arg"):
                        f = fields.setdefault(base.name, {})
                        f[0] = max(f.get(0, 0), nd.width)
                    elif (isinstance(base, Bin) and base.op == "+"
                          and isinstance(base.a, Reg) and base.a.name.startswith("arg")):
                        if isinstance(base.b, Const):
                            f = fields.setdefault(base.a.name, {})
                            f[base.b.val] = max(f.get(base.b.val, 0), nd.width)
                        else:
                            complex_.add(base.a.name)          # indexed -> array-like
    structs = {p: f for p, f in fields.items() if p not in complex_}
    ptr = set(fields) | complex_
    return ptr, structs, produces_eax


_NORENAME = set(SEG) | {"esp", "ebp", "sp", "bp"}          # frame / segment regs stay put


def _subst_regs(node, mp):
    """Rebuild an expression with Reg names remapped per `mp` (for SSA renaming)."""
    if isinstance(node, Reg):
        return Reg(mp[node.name], node.width) if node.name in mp else node
    if isinstance(node, Mem):
        return Mem(_subst_regs(node.addr, mp), node.width)
    if isinstance(node, Bin):
        return Bin(node.op, _subst_regs(node.a, mp), _subst_regs(node.b, mp), node.width)
    if isinstance(node, Un):
        return Un(node.op, _subst_regs(node.a, mp), node.width)
    if isinstance(node, Call):
        tgt = node.target if isinstance(node.target, str) else _subst_regs(node.target, mp)
        return Call(tgt, [_subst_regs(a, mp) for a in node.args])
    return node


def _ssa_rename(blocks, live_out, skip, keep_locs=False):
    """Block-local SSA-lite. A register whose value lives and dies within one block
    (not live-out) -- and is neither in `skip` (feeds a condition, or has a
    subregister access the al/ah peephole needs) nor a frame/segment reg nor a param
    -- has each definition renamed to a fresh v<N>, so reused register names become
    distinct named values. Live-out / cross-block values keep their register name, so
    no phi is needed and the rename is a semantics-preserving alpha-rename. Returns
    the v-names created (declared as locals by the caller).

    keep_locs (byte-exact/faithful path): never rename `loc_` STACK SLOTS. Splitting a
    reused stack slot ([ebp-4] written several times, non-overlapping lifetimes) into
    distinct v-temps makes the compiler allocate a separate frame slot per temp instead
    of coalescing them into the one slot the original reused -- inflating `sub esp,N` and
    every local offset. Keeping the loc_<offset> name preserves the single-slot reuse."""
    ctr = [0]
    made = []
    for blk in blocks:
        name, stmts, _term = blk
        lout = live_out.get(name, set())
        last_def = {}
        for i, st in enumerate(stmts):
            if isinstance(st, Set) and isinstance(st.dst, Reg):
                last_def[st.dst.name] = i
        cur, new = {}, []
        for i, st in enumerate(stmts):
            if isinstance(st, Set):
                expr = _subst_regs(st.expr, cur)
                if isinstance(st.dst, Mem):
                    new.append(Set(Mem(_subst_regs(st.dst.addr, cur), st.dst.width), expr))
                    continue
                if isinstance(st.dst, Reg):
                    r = st.dst.name
                    if (r in skip or r in _NORENAME or r.startswith("arg")
                            or r.startswith("__") or (keep_locs and r.startswith("loc_"))):
                        cur.pop(r, None)                 # keep the register name here
                        new.append(Set(st.dst, expr))
                        continue
                    if r in lout and last_def[r] == i:   # final def of a live-out value
                        cur[r] = r
                        new.append(Set(st.dst, expr))
                        continue
                    ctr[0] += 1
                    v = "v%d" % ctr[0]; made.append(v)
                    cur[r] = v
                    new.append(Set(Reg(v, st.dst.width), expr))
                    continue
                new.append(Set(st.dst, expr))            # Flag dst
            elif isinstance(st, Ret) and st.expr is not None:
                new.append(Ret(_subst_regs(st.expr, cur)))
            elif isinstance(st, Call):
                new.append(Call(st.target, [_subst_regs(a, cur) for a in st.args]))
            else:
                new.append(st)
        blk[1] = new
    return made


def _recover_conditions(rows, params, bits=32):
    """Map each cond terminator (by target label) to its relational C condition,
    from the flag-setting instruction that precedes it. Handles the plain
    `cmp`/`test`, the `test/or/and reg,reg` and `dec/inc/..`-then-`jz` zero-test
    idioms (very common), and leaves anything else honestly unrecovered."""
    conds = []
    last = [None]                                        # (a, b, kind); kind: cmp|test|zero
    for tag, val in rows:
        if tag == "label":
            continue
        off, txt = tag, val
        op = txt.split()[0].lower()
        parts = _split_ops(txt.partition(" ")[2]) if " " in txt.strip() else []
        if op == "cmp" and len(parts) == 2:
            a, _ = _operand(parts[0], params, bits=bits); b, _ = _operand(parts[1], params, bits=bits)
            last[0] = (a, b, "cmp") if a is not None and b is not None else None
        elif op == "test" and len(parts) == 2:
            a, _ = _operand(parts[0], params, bits=bits); b, _ = _operand(parts[1], params, bits=bits)
            if a is None or b is None:
                last[0] = None
            elif parts[0].strip().lower() == parts[1].strip().lower():
                last[0] = (a, Const(0, 32), "zero")      # test reg,reg -> reg vs 0
            else:
                last[0] = (a, b, "test")                 # genuine bit test
        elif op in _ZERO_ALU and len(parts) >= 1:
            d, _ = _operand(parts[0], params, bits=bits)  # dest holds the result post-op
            last[0] = (d, Const(0, 32), "zero") if d is not None else None
        elif op in ("loop", "loopw", "loopd", "loope", "loopz", "loopne", "loopnz"):
            cxr = "cx" if bits == 16 else "ecx"          # matches the block builder's cx-- cond
            conds.append((txt.split()[-1], "jne", (Reg(cxr, bits), Const(0, bits), "cmp")))
        elif op in ("jcxz", "jecxz"):                    # if (cx == 0) goto tgt
            cxr = "cx" if bits == 16 else "ecx"
            conds.append((txt.split()[-1], "je", (Reg(cxr, bits), Const(0, bits), "cmp")))
        elif op in COND_JUMPS:
            tgt = txt.split()[-1]
            cur = last[0]
            if cur is not None and cur[2] == "zero" and CANON.get(op, op) not in _ZERO_JUMPS:
                cur = None                               # CF/OF jump after a value op: honest
            kind = "cmp" if cur and cur[2] == "zero" else (cur[2] if cur else None)
            conds.append((tgt, CANON.get(op, op),
                          (cur[0], cur[1], kind) if cur else None))
    return conds


def _stmt_reads(st):
    rd = set()
    if isinstance(st, Set):
        _reads_regs(st.expr, rd)
        if isinstance(st.dst, Mem):
            _reads_regs(st.dst.addr, rd)
    elif isinstance(st, Ret) and st.expr is not None:
        _reads_regs(st.expr, rd)
    return rd


def _reg_liveness(blocks, conds):
    """Iterative backward liveness over register names -> {block_name: live_out}."""
    order = [b[0] for b in blocks]
    idx = {n: i for i, n in enumerate(order)}
    # successor edges from terminators
    succ = {}
    cond_ops = {}
    ci = 0
    for i, (name, stmts, term) in enumerate(blocks):
        nxt = order[i + 1] if i + 1 < len(blocks) else None
        if term[0] == "cond":
            s = [t for t in (term[2], nxt) if t in idx]
            succ[name] = s
            if ci < len(conds):
                cond_ops[name] = conds[ci][2]      # (a,b,op) for this cond
                ci += 1
        elif term[0] == "goto":
            succ[name] = [term[1]] if term[1] in idx else []
        elif term[0] == "fall":
            succ[name] = [term[1]] if term[1] in idx else []
        else:
            succ[name] = []
    # per-block use/def
    use, dfn = {}, {}
    for name, stmts, term in blocks:
        defined, used = set(), set()
        for st in stmts:
            for r in _stmt_reads(st):
                if r not in defined:
                    used.add(r)
            if isinstance(st, Set) and isinstance(st.dst, Reg):
                defined.add(st.dst.name)
        if name in cond_ops and cond_ops[name]:      # branch reads its cmp operands
            a, b, _op = cond_ops[name]
            _reads_regs(a, used); _reads_regs(b, used)
        use[name], dfn[name] = used, defined
    live_in = {n: set() for n in order}
    live_out = {n: set() for n in order}
    changed = True
    while changed:
        changed = False
        for name in reversed(order):
            lo = set()
            for s in succ.get(name, []):
                lo |= live_in[s]
            li = use[name] | (lo - dfn[name])
            if lo != live_out[name] or li != live_in[name]:
                live_out[name], live_in[name] = lo, li
                changed = True
    return live_out, live_in


def _dse_blocklocal(stmts, live_out):
    """Drop reg stores dead at block scope (seeded with the block's live-out).
    Memory stores are always kept (observable side effect)."""
    live = set(live_out)
    out = []
    for st in reversed(stmts):
        if isinstance(st, Set) and isinstance(st.dst, Reg):
            if st.dst.name not in live and not _has_call(st.expr):
                continue                              # dead register store (no side effect)
            live.discard(st.dst.name)
            live |= _stmt_reads(st)
            out.append(st)
        else:
            live |= _stmt_reads(st)
            out.append(st)
    out.reverse()
    return out


# canonical jcc set + alias folding + logical negation, all keyed by mnemonic so
# signedness (jl vs jb) and zero/sign semantics (test x,x) are decided precisely.
CANON = {"jz": "je", "jnz": "jne", "jnge": "jl", "jng": "jle", "jnle": "jg",
         "jnl": "jge", "jnae": "jb", "jna": "jbe", "jnbe": "ja", "jnb": "jae",
         "jc": "jb", "jnc": "jae", "jpe": "jp", "jpo": "jnp"}
COND_JUMPS = set(CANON) | {"je", "jne", "jl", "jle", "jg", "jge", "jb", "jbe",
                           "ja", "jae", "js", "jns", "jo", "jno", "jp", "jnp"}
NEG_MN = {"je": "jne", "jne": "je", "jl": "jge", "jge": "jl", "jle": "jg",
          "jg": "jle", "jb": "jae", "jae": "jb", "jbe": "ja", "ja": "jbe",
          "js": "jns", "jns": "js", "jo": "jno", "jno": "jo", "jp": "jnp", "jnp": "jp"}


def _opw(n):
    """The bit-width of a comparison operand (8/16/32); 32 if untagged."""
    w = getattr(n, "width", None)
    return w if w in (8, 16, 32) else 32


def _cmp_width(a, b):
    """Width of a cmp: the non-immediate operand carries the true size (`cmp al,0x39`
    is an 8-bit compare even though 0x39 fits a dword). x86 requires both the same
    width, so either non-const answers; fall back to 32 (a bare imm-vs-imm never occurs)."""
    if not isinstance(a, Const):
        return _opw(a)
    if not isinstance(b, Const):
        return _opw(b)
    return 32


def _wc(s, w, signed):
    """Cast an already-rendered operand to its compare width so the C relational has the
    SAME signedness/width as the x86 flags. At w==32 this reproduces the historical output
    exactly ((i32) for signed, bare for unsigned); at w<32 it sign-/zero-extends from the
    real subregister -- e.g. `cmp al,imm; jle` is a SIGNED int8 test, so (i8)al (0x8d ->
    -115), and `cmp word[..],imm; ja` is UNSIGNED 16-bit, so (u16)."""
    if w >= 32:
        return "(i32)%s" % s if signed else s
    return "(%s%d)%s" % ("i" if signed else "u", w, s)


def _cc_to_c(mn, a, b, kind):
    """Render a jcc as a C boolean, honouring signedness, the compare kind, AND the
    operand WIDTH.

    `test x,x` (and `test a,b`) set flags off `x` (or `a&b`) vs 0, so je/js/jl there
    mean == 0 / < 0 -- NOT `x < x`. cmp a,b compares the two operands, signed for
    jl/jg and unsigned for jb/ja, each at the operands' subregister width."""
    if kind == "test":
        lhs = a if _c(a) == _c(b) else Bin("&", a, b, 32)
        rhs = Const(0, 32)
    else:
        lhs, rhs = a, b
    w = _cmp_width(lhs, rhs)
    L, R = _c(lhs), _c(rhs)
    unsigned = {"je": "==", "jne": "!=", "jb": "<", "jbe": "<=", "ja": ">", "jae": ">="}
    signed = {"jl": "<", "jle": "<=", "jg": ">", "jge": ">="}
    if mn in unsigned:                                    # ==,!= and unsigned relationals
        return "%s %s %s" % (_wc(L, w, False), unsigned[mn], _wc(R, w, False))
    if mn in signed:                                      # signed relationals (i8/i16/i32)
        return "%s %s %s" % (_wc(L, w, True), signed[mn], _wc(R, w, True))
    if mn in ("js", "jns"):
        inner = L if R in ("0", "0x0") else "(%s - %s)" % (L, R)
        return "%s %s 0" % (_wc(inner, w, True), "<" if mn == "js" else ">=")
    if mn in ("jo", "jno"):
        if kind == "test":                               # test clears OF
            return "0" if mn == "jo" else "1"
        return ("__ovf_sub(%s, %s)" if mn == "jo" else "!__ovf_sub(%s, %s)") % (L, R)
    if mn in ("jp", "jnp"):                              # parity of the compared value
        val = L if kind == "test" else "(%s - %s)" % (L, R)
        return "%s__parity(%s)" % ("" if mn == "jp" else "!", val)
    return "1 /* %s not modelled */" % mn


def _cond_c(mn, cmp_operands, negate=False):
    if cmp_operands is None:
        return "1 /* cond not recovered */"
    a, b, kind = cmp_operands
    m = NEG_MN.get(mn, mn) if negate else mn
    return _cc_to_c(m, a, b, kind)


def _goto(tgt):
    """Render `goto tgt;`. A real label passes through; a bare-offset target that
    isn't a label in this function is a cross-function jump -> a computed goto to
    that absolute address (honest, and valid GCC computed-goto syntax)."""
    if re.match(r"(?i)^(L_[0-9a-f]+|_\w+)$", tgt):
        return "goto %s;" % tgt
    v = _imm2(tgt)
    return ("goto *(void *)0x%x; /* external */" % v.val) if v is not None else "goto %s;" % tgt


def _emit_structured(blocks, conds):
    """Structure the basic blocks into C. Recovers if-then and if-then-else where
    a forward branch's arm(s) are entered ONLY by fall-through (so inlining them is
    provably faithful); everything else -- back-edges (loops), multi-entry blocks,
    external targets -- falls back to a labelled `goto`, which is always correct.
    Target labels ARE emitted, so the fallback control flow is valid, not dangling."""
    n = len(blocks)
    names = [b[0] for b in blocks]
    idx = {nm: k for k, nm in enumerate(names)}
    # how many explicit branches (cond/goto) target each in-function label
    tgt_count = {}
    for (nm, st, term) in blocks:
        t = term[2] if term[0] == "cond" else (term[1] if term[0] == "goto" else None)
        if t is not None and t in idx:
            tgt_count[t] = tgt_count.get(t, 0) + 1
    # positionally pair each cond-block with its recovered condition (block order)
    conds = list(conds)
    condmap = {}
    ci = 0
    for bi, (nm, st, term) in enumerate(blocks):
        if term[0] == "cond":
            condmap[bi] = conds[ci] if ci < len(conds) else (None, term[2], None)
            ci += 1

    out = []
    PAD = "    "

    def stmt_lines(stmts, ind):
        for st in stmts:
            if isinstance(st, tuple) and st and st[0] == "raw":
                out.append(PAD * ind + "__asm(%r);" % st[1])
            elif isinstance(st, tuple) and st and st[0] == "data":
                out.append(PAD * ind + "/* data: %s */" % st[1])
            elif isinstance(st, Set):
                sw = _subreg_write(st)                    # `al = v` rather than the merge
                if sw is not None:
                    out.append(PAD * ind + "%s = %s;" % (sw[0], _c(sw[1])))
                else:
                    out.append(PAD * ind + "%s = %s;" % (_c(st.dst), _c(st.expr)))
            elif isinstance(st, Ret):
                out.append(PAD * ind + ("return %s;" % _c(st.expr) if st.expr is not None else "return;"))

    def only_fallthrough(lo, hi):
        """True if no block in [lo,hi) is reached by an explicit branch -- i.e. the
        range is entered solely by falling into blocks[lo]. Safe to inline."""
        return all(names[j] not in tgt_count for j in range(lo, hi))

    def pure_compare(stmts):
        """A block whose only effect is setting flags (a bare cmp/test after DFE) --
        so folding its condition into a || chain skips nothing observable."""
        return all(isinstance(s, Set) and isinstance(s.dst, Flag) for s in stmts)

    def emit_seq(lo, hi, ind, join, skip_label_at=None):
        """Emit blocks[lo:hi]. A `goto join` is suppressed (it's the implicit fall
        out of a structured region). `skip_label_at` omits one region-entry label."""
        i = lo
        while i < hi:
            nm, stmts, term = blocks[i]
            # do-while: block i is a loop header entered only by fall-in, whose sole
            # explicit in-edge is a conditional back-edge from a straight-line latch
            # (the ubiquitous `dec ecx; jnz top` / `...; cmp; jl top` shape). Anything
            # with internal branches or extra entries stays a labelled goto.
            lj = None
            for j in range(i, hi):
                t2 = blocks[j][2]
                if t2[0] == "cond" and t2[2] == nm:
                    lj = j; break
                if t2[0] != "fall":                    # body must be straight-line
                    break
            if (lj is not None and tgt_count.get(nm, 0) == 1
                    and only_fallthrough(i + 1, lj + 1)):
                out.append(PAD * ind + "do {")
                for j in range(i, lj + 1):
                    stmt_lines(blocks[j][1], ind + 1)
                _t, ccop, cmpops = condmap[lj]
                out.append(PAD * ind + "} while (%s);" % _cond_c(ccop, cmpops))
                i = lj + 1
                continue
            if nm in tgt_count and i != skip_label_at:
                out.append(PAD * ind + nm + ":")
            stmt_lines(stmts, ind)
            if term[0] == "cond":
                _t, ccop, cmpops = condmap[i]
                tgt = term[2]
                tj = idx.get(tgt)
                # short-circuit ||: consecutive cond blocks that all branch to the
                # SAME target, each follower a pure compare -> one `if (c0 || c1 ..)`.
                # A follower runs iff every prior cond was false, which is exactly ||.
                if cmpops is not None:
                    chain = [(ccop, cmpops)]
                    j = i + 1
                    while (j < hi and blocks[j][2][0] == "cond" and blocks[j][2][2] == tgt
                           and pure_compare(blocks[j][1]) and condmap[j][2] is not None
                           and only_fallthrough(i + 1, j + 1)):
                        chain.append((condmap[j][1], condmap[j][2]))
                        j += 1
                    if len(chain) > 1:
                        cond_str = " || ".join(_cond_c(c, m) for c, m in chain)
                        out.append(PAD * ind + "if (%s) %s" % (cond_str, _goto(tgt)))
                        i = j
                        continue
                # if-then / if-then-else: forward branch whose arm(s) only fall in
                if tj is not None and i + 1 < tj <= hi and only_fallthrough(i + 1, tj):
                    then_lo, then_hi = i + 1, tj
                    else_lo = else_hi = None
                    then_join = tgt                        # plain if-then joins at the target
                    # the then-body's exit `jmp JOIN` marks an if-else (the block
                    # splitter leaves an empty `_dead` block after a jmp, so scan back
                    # past those to the real goto).
                    gj = None
                    for j in range(tj - 1, i, -1):
                        if blocks[j][2][0] == "goto":
                            gj = j; break
                        if blocks[j][1]:                   # real work with no goto exit
                            break
                    if gj is not None and tgt_count.get(tgt, 0) == 1:
                        JOIN = blocks[gj][2][1]
                        jj = idx.get(JOIN)                 # else arm = [tj, jj)
                        if jj is not None and tj < jj <= hi and only_fallthrough(tj + 1, jj):
                            else_lo, else_hi = tj, jj
                            then_join = JOIN               # both arms fall out to JOIN
                    # body runs when branch NOT taken -> negate the jcc
                    out.append(PAD * ind + "if (%s) {" % _cond_c(ccop, cmpops, negate=True))
                    emit_seq(then_lo, then_hi, ind + 1, join=then_join)
                    if else_lo is not None:
                        out.append(PAD * ind + "} else {")
                        emit_seq(else_lo, else_hi, ind + 1, join=then_join,
                                 skip_label_at=else_lo)
                        out.append(PAD * ind + "}")
                        i = else_hi
                    else:
                        out.append(PAD * ind + "}")
                        i = then_hi
                    continue
                out.append(PAD * ind + "if (%s) %s" % (_cond_c(ccop, cmpops), _goto(tgt)))
            elif term[0] == "goto":
                if term[1] != join:                        # suppress the implicit fall-out
                    out.append(PAD * ind + "goto %s;" % term[1])
            elif term[0] == "ijmp":                        # computed / jump-table / external
                nd = term[1]
                if nd is None:
                    out.append(PAD * ind + "goto *0; /* indirect jump (unresolved) */")
                elif isinstance(nd, Const):
                    out.append(PAD * ind + "goto *(void *)0x%x; /* external */" % nd.val)
                else:
                    out.append(PAD * ind + "goto *(%s); /* computed jump / jump table */"
                               % _c(nd))
            i += 1

    emit_seq(0, n, 0, join=None)
    # drop labels no surviving goto references (if-then sugar removed their jumps)
    referenced = set()
    for ln in out:
        m = re.search(r"goto (\w+);", ln)
        if m:
            referenced.add(m.group(1))
    out = [ln for ln in out
           if not (re.match(r"^\s*\w+:$", ln) and ln.strip()[:-1] not in referenced)]
    return out


_CTRL_MN = (COND_JUMPS | {"jmp", "ljmp", "jmpf", "loop", "loopw", "loopd", "loope",
                          "loopz", "loopne", "loopnz", "jcxz", "jecxz"})
_FRAME_MN = {"push", "pop", "leave", "enter", "ret", "retn", "retf", "retd", "retfd",
             "retnd", "retfw", "retnw", "nop", "cld", "std", "cli", "sti", "wait",
             "fwait", "hlt", "cmc", "clc", "stc"}


def _value_defs(path, faithful):
    """Count the VALUE-producing statements the lift emits (reg/mem assignments and
    calls) -- the material a byte-exact recompile needs. Control flow (jcc/jmp/loop)
    and frame ops (push/pop/leave/no-C-effect flags) are excluded: they always lift
    to goto/if or are re-synthesised by the compiler, so they are never the drop we
    care about. Returns the emitted-statement count."""
    c = lift_asm_function(path, "fn", faithful=faithful)
    body = c.split("{", 1)[1] if "{" in c else c
    n = 0
    for ln in body.splitlines():
        s = ln.strip()
        if not s or s.endswith(":") or s in ("{", "}"):
            continue
        if s.startswith(("if", "goto", "return", "}", "/*")):
            continue
        if "=" in s or re.match(r"\w+\(", s):        # assignment or bare call statement
            n += 1
    return n


def lossless_report(path, faithful=True):
    """Instruction-accounting for the lift. Splits the asm into value-producing
    instructions (mov/add/lea/mul/... -> must leave a statement) vs control/frame
    instructions (jcc/jmp/loop/push/pop/leave/flag-clears -> lifted to goto/if or
    re-synthesised by the compiler, legitimately not body statements). A value insn
    is 'dropped' if the emitted C has fewer value statements than value insns.
    Returns (value_insns, value_stmts_emitted, dropped_estimate, readable_stmts)."""
    rows = parse_asm(path)
    insns = [v for (t, v) in rows if t != "label" and not _is_data(v)]
    val_insns = 0
    for ins in insns:
        mn = ins.split()[0].lower() if ins.split() else ""
        if mn in _CTRL_MN or mn in _FRAME_MN or mn in ("call", "lcall", "callf"):
            continue
        val_insns += 1
    stmts = _value_defs(path, faithful=faithful)
    readable = _value_defs(path, faithful=False)
    return val_insns, stmts, max(0, val_insns - stmts), readable


def main():
    argv = [a for a in sys.argv[1:] if a not in ("--faithful", "--lossless")]
    faithful = "--faithful" in sys.argv
    if "--selftest" in sys.argv or not sys.argv[1:]:
        sys.exit(0 if _selftest() else 1)
    if "--lossless" in sys.argv:
        path = argv[0]
        val, stmts, drop, readable = lossless_report(path, faithful=True)
        print("LOSSLESS CHECK  %s" % path)
        print("  value-producing insns: %d" % val)
        print("  readable-mode stmts:   %d" % readable)
        print("  faithful-mode stmts:   %d   (+%d recovered vs readable)"
              % (stmts, stmts - readable))
        print("  coverage: %.1f%%   est-dropped: %d" % (100 * stmts / max(1, val), drop))
        sys.exit(0)
    # otherwise: lift a reconstructed .asm file to C
    path = argv[0]
    name = argv[1] if len(argv) > 1 else None
    print(lift_asm_function(path, name, faithful=faithful))


if __name__ == "__main__":
    main()
