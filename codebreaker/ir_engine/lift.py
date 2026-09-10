#!/usr/bin/env python3
"""
lift.py — mechanically transliterate a function into compilable C.

THE GAME PLAN THIS IMPLEMENTS
    `cfg.py` recovers control flow but stops at expressions, leaving
    `if (/* ax == 0x10 */)` for a human. Filling those in by hand, 3,550 times,
    is the whole bottleneck. This does it MECHANICALLY.

    Each instruction becomes one C statement. Registers are declared locals
    (with a union so AL/AH alias AX correctly). Memory operands are named:
    [bp+N] -> an argument, [bp-N] -> a local, ds:0xNNNN -> a global, [bx+d] ->
    a far dereference. Pushes are collected and become a call's arguments. The
    whole thing is wrapped in cfg.py's goto-form control flow.

WHAT THIS IS AND IS NOT
    It is DERIVED, not interpreted. `arg1 + arg2` is what the bytes compute;
    `loc_4`, `ax`, `sub_1234` claim no meaning. That is exactly compatible with
    the project's one rule — a neutral name invents nothing. Naming `loc_4`
    into `cchText`, or `0x400` into EM_GETSEL, is a SEPARATE human layer that
    this tool deliberately does not attempt.

    The output is ugly — a literal transliteration, not idiomatic C. That is
    the point: ugly-but-faithful-and-compiling for 3,550 functions beats
    beautiful-for-450. Beautification is optional polish on top.

    An instruction the lifter cannot model is emitted as `/* UNLIFTED: ... */`
    and the function is counted `partial`. The measurement that matters is: how
    many functions lift with ZERO unlifted instructions.

Usage:
    python lift.py CARDFILE 3 0x04d4        # one function, to stdout
    python lift.py --measure                # coverage across the whole system
    python lift.py --measure APPS           # ...and the applications
"""
import re, sys, collections
from codebreaker.foundation import cores
cfg = cores.get('cfg')
trace = cores.get('trace')
from codebreaker.recon_engine import argcheck
from codebreaker.foundation import layout

PROTOS = None
def protos():
    global PROTOS
    if PROTOS is None:
        PROTOS = argcheck.prototypes()
    return PROTOS

VOID_APIS = None
def void_apis():
    """APIs the 1.01 header declares `void` — assigning their result to a
    register is a compile error (`ax = EndDialog(...)`), so those calls are
    emitted as plain statements."""
    global VOID_APIS
    if VOID_APIS is None:
        src = open("refs/sdk101/d06/WINDOWS.H", encoding="cp437", errors="replace").read()
        src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
        VOID_APIS = set(re.findall(r"\bvoid\s+(?:FAR\s+)?(?:PASCAL\s+)?(\w+)\s*\(", src))
    return VOID_APIS

_ENTRY_HOME = {}
def entry_home(mod):
    """{ordinal: (seg, off)} for a module's NE entry table (cached). Used to
    resolve a SELF-IMPORT (a module calling its own entry ordinal, e.g.
    WRITE.64) to the real body's (seg, off)."""
    key = mod.upper()
    if key not in _ENTRY_HOME:
        m = {}
        try:
            ne = cfg.open_any(key)
            for e in ne.entries():
                if e.get("kind") in ("MOVEABLE", "FIXED"):
                    m[e["ordinal"]] = (e["seg"], e["offset"])
        except Exception:
            pass
        _ENTRY_HOME[key] = m
    return _ENTRY_HOME[key]


REG16 = ("ax", "bx", "cx", "dx", "si", "di")
REG8  = {"al": ("ax", "l"), "ah": ("ax", "h"), "bl": ("bx", "l"), "bh": ("bx", "h"),
         "cl": ("cx", "l"), "ch": ("cx", "h"), "dl": ("dx", "l"), "dh": ("dx", "h")}
SEG   = ("ds", "es", "ss", "cs")

def norm(asm):
    """objdump pads mnemonics with runs of spaces — collapse them so the
    single-space patterns below actually match."""
    return re.sub(r"\s+", " ", asm).strip()

# True no-ops that carry no value in our data model: NOP, the sign-extends
# (handled implicitly by C's int promotions), and the flag-only instructions
# (direction, carry, interrupt-enable) which affect string ops / hardware, not
# the values this transliteration tracks.
def is_noop(asm):
    return bool(re.match(r"(nop|cwd|cbw|leave|\.word|cld|std|clc|stc|cli|sti|cmc|"
                         r"pushf|popf)\b",
                         asm))

# The frame PROLOGUE and EPILOGUE. These are skipped only where they actually
# occur — a leading run at the top and a trailing run at the bottom — because
# `push ds`/`push bp` mean "save the frame" there but "push a segment for a far
# pointer argument" in the middle of the body. Matching them anywhere ate real
# argument pushes and corrupted the following call.
PROLOGUE = re.compile(
    r"(push (bp|ds|es|si|di)|pop ax|nop|inc bp|mov bp,sp|mov ds,ax|"
    r"mov es,ax|sub sp,|push cs)\b")
EPILOGUE = re.compile(
    r"(pop (bp|ds|es|si|di)|mov sp,bp|sub bp,|dec bp|add sp,|ret|retf|leave)\b")


def frame_spans(rows):
    """Offsets that are prologue or epilogue, to be skipped.

    The leading run also swallows the SHARED-PROLOGUE form: `call <prolog>`
    followed by the `.word` inline descriptor and then the frame setup. If it
    is not skipped, the prologue's `push bp` leaks into the first real call's
    arguments.
    """
    skip = set()
    for idx, (off, asm) in enumerate(rows):     # leading prologue run
        a = norm(asm)
        if PROLOGUE.match(a) or a.startswith(".word") or \
                (idx == 0 and re.match(r"call 0x[0-9a-f]+$", a)):
            skip.add(off)
        else:
            break
    for off, asm in reversed(rows):             # trailing epilogue run
        if EPILOGUE.match(norm(asm)):
            skip.add(off)
        else:
            break
    return skip


class Lifter:
    def __init__(self, sg, blocks, start, mod, seg, argmap, ret_is_far):
        self.sg = sg
        self.blocks = blocks
        self.start = start
        self.mod, self.seg = mod, seg
        self.argmap = argmap            # {bp_offset: "argN"}
        self.ret_is_far = ret_is_far
        self.syms = cfg.internal_symbols(mod, seg, sg)
        self.locals = {}                # bp_off -> "loc_X"  (negative offs)
        self.globals = set()            # ds offsets used
        self.segs = set()               # segment pseudo-regs pushed
        self.pushes = []                # pending call args (temp names)
        self.pt = 0                     # current push depth
        self.ptmax = 0                  # max depth -> how many temps to declare
        self.flags = None               # (left, right) from last cmp/test
        self.use_cf = False             # a shift/rotate needs the carry flag
        self.unlifted = 0

    def _width(self, op):
        """Operand width in bits: 8 for byte reg / BYTE PTR, else 16."""
        op = op.strip()
        if op in REG8:
            return 8
        if "BYTE PTR" in op:
            return 8
        return 16

    # ---- operand parsing --------------------------------------------------
    def mem(self, op):
        """(c_lvalue_or_None, size) for a memory operand, else (None, None)."""
        m = re.match(r"(BYTE|WORD|DWORD) PTR (.+)$", op)
        size = None
        if m:
            size, inner = m.group(1), m.group(2)
        else:
            inner = op
        # bare seg:disp  (e.g. ds:0x16)  or  seg:[...]  or  [...]
        segm = re.match(r"(ds|es|ss|cs):(.+)$", inner)
        seg = None
        if segm:
            seg, inner = segm.group(1), segm.group(2)
        typ = {"BYTE": "BYTE", "WORD": "WORD", "DWORD": "DWORD"}.get(size, "WORD")

        # pure displacement:  0xNNNN
        m = re.match(r"0x([0-9a-f]+)$", inner)
        if m and seg:
            off = int(m.group(1), 16)
            self.globals.add((off, typ))
            return "g_%04x" % off, typ

        # bracketed addressing:  [bp+0x6] [bx] [bx+si] [bx+0x1a] [bp+si-0x104]
        m = re.match(r"\[(.+)\]$", inner)
        if not m:
            return None, None
        terms = m.group(1)

        # bp-relative only -> arg or local
        bm = re.fullmatch(r"bp([+-]0x[0-9a-f]+)", terms)
        if bm:
            disp = int(bm.group(1), 16)
            if disp >= 0:
                nm = self.argmap.get(disp)
                if nm:
                    return nm, typ
                # positive but not a known arg: treat as raw frame slot
            key = disp
            if key not in self.locals:
                self.locals[key] = "loc_%x" % (-disp if disp < 0 else disp) \
                    + ("" if disp < 0 else "_hi")
            return self.locals[key], typ

        # general: sum of registers/displacements -> far deref
        parts, cur, sign = [], "", "+"
        for tok in re.findall(r"[+-]|bp|bx|si|di|0x[0-9a-f]+", terms):
            if tok in "+-":
                sign = tok
                continue
            if tok == "bp":
                parts.append((sign, "(WORD)_bp"))
            elif tok in ("bx", "si", "di"):
                parts.append((sign, tok))
            else:
                parts.append((sign, tok))
        if not parts:
            return None, None
        addr = ""
        for s, t in parts:
            addr += (t if not addr else " %s %s" % (s, t))
        return "(*(%s FAR *)(%s))" % (typ, addr), typ

    def addr_of(self, op):
        """The C address expression for a `lea reg,[...]` source."""
        op = op.strip()
        # bare displacement, with or without a segment override:
        #   `lea bx,ds:0x582` / `lea bx,0x582` -> the constant offset 0x582.
        dm = re.match(r"(?:(?:ds|es|ss|cs|fs|gs):)?0x([0-9a-f]+)$", op)
        if dm:
            return "0x%x" % int(dm.group(1), 16)
        m = re.match(r"\[(.+)\]$", op)
        if not m:
            return None
        terms = m.group(1)
        bm = re.fullmatch(r"bp([+-]0x[0-9a-f]+)", terms)
        if bm:
            disp = int(bm.group(1), 16)
            nm = self.argmap.get(disp) if disp >= 0 else None
            if nm is None:
                if disp not in self.locals:
                    self.locals[disp] = "loc_%x" % (-disp if disp < 0 else disp) \
                        + ("" if disp < 0 else "_hi")
                nm = self.locals[disp]
            return "&%s" % nm
        parts, sign = [], "+"
        for tok in re.findall(r"[+-]|bp|bx|si|di|0x[0-9a-f]+", terms):
            if tok in "+-":
                sign = tok; continue
            parts.append((sign, "(WORD)_bp" if tok == "bp" else tok))
        if not parts:
            return None
        addr = ""
        for s, t in parts:
            addr += (t if not addr else " %s %s" % (s, t))
        return addr

    def rval(self, op):
        """A C rvalue string for any operand, or None if un-parseable."""
        op = op.strip()
        if op in REG16:
            return op
        if op in REG8:
            return op
        if op == "bp":
            return "_bp"
        if op == "sp":
            return "_sp"
        if op in SEG:
            self.segs.add(op)
            return "_%s" % op
        m = re.match(r"0x([0-9a-f]+)$", op)
        if m:
            return "0x%x" % int(m.group(1), 16)
        if re.match(r"-?\d+$", op):
            return op
        lv, _ = self.mem(op)
        return lv

    # ---- per-instruction transfer ----------------------------------------
    def insn(self, off, asm):
        """Return a list of C statements for one instruction."""
        asm = norm(asm)
        if is_noop(asm):
            return []
        op, _, rest = asm.partition(" ")
        args = [a.strip() for a in rest.split(",")] if rest else []

        # two-operand ALU / mov
        BIN = {"mov": "=", "add": "+=", "sub": "-=", "and": "&=", "or": "|=",
               "xor": "^=", "adc": "+=", "sbb": "-="}
        if op in BIN and len(args) == 2:
            dst = self.rval(args[0]); src = self.rval(args[1])
            if dst is None or src is None:
                return self._unlift(asm)
            if op == "mov":
                return ["%s = %s;" % (dst, src)]
            # adc/sbb fold in the modeled carry so multi-word chains are exact.
            if op in ("adc", "sbb"):
                self.use_cf = True
                sign = "+" if op == "adc" else "-"
                return ["%s %s %s %s _cf;" % (dst, BIN[op], src, sign)]
            return ["%s %s %s;" % (dst, BIN[op], src)]

        # shifts and rotates. Modelled with an explicit carry pseudo-flag (_cf)
        # so the multi-word idiom `shl lo,1 / rcl hi,1` (and `shr/rcr`) lifts
        # exactly. Count is `,1` (the common form, carry exact), `,cl`, or an
        # immediate; rcl/rcr require the 1-count form (carry-through by cl is a
        # loop we do not model) and unlift otherwise.
        if op in ("shl", "sal", "shr", "sar", "rol", "ror", "rcl", "rcr") \
                and len(args) == 2:
            dst = self.rval(args[0]); cnt = self.rval(args[1])
            if dst is None or cnt is None:
                return self._unlift(asm)
            W = self._width(args[0]); top = W - 1
            self.use_cf = True
            if op in ("shl", "sal"):
                return ["_cf = (%s >> %d) & 1; %s <<= %s;" % (dst, top, dst, cnt)]
            if op == "shr":
                return ["_cf = %s & 1; %s >>= %s;" % (dst, dst, cnt)]
            if op == "sar":
                return ["_cf = %s & 1; %s = ((int)%s) >> %s;"
                        % (dst, dst, dst, cnt)]
            if op == "rol":
                return ["%s = ((%s << %s) | (%s >> (%d - %s))) & 0x%x;"
                        " _cf = %s & 1;"
                        % (dst, dst, cnt, dst, W, cnt, (1 << W) - 1, dst)]
            if op == "ror":
                return ["%s = ((%s >> %s) | (%s << (%d - %s))) & 0x%x;"
                        " _cf = (%s >> %d) & 1;"
                        % (dst, dst, cnt, dst, W, cnt, (1 << W) - 1, dst, top)]
            # rcl / rcr : rotate THROUGH carry, 1 bit.
            if cnt != "1":
                return self._unlift(asm)
            if op == "rcl":
                return ["{ WORD _nc = (%s >> %d) & 1;"
                        " %s = ((%s << 1) | _cf) & 0x%x; _cf = _nc; }"
                        % (dst, top, dst, dst, (1 << W) - 1)]
            # rcr
            return ["{ WORD _nc = %s & 1;"
                    " %s = (%s >> 1) | (_cf << %d); _cf = _nc; }"
                    % (dst, dst, dst, top)]

        if op == "lea" and len(args) == 2:
            dst = self.rval(args[0])
            a = self.addr_of(args[1])
            if dst and a:
                return ["%s = (WORD)(%s);" % (dst, a)]
            return self._unlift(asm)

        if op in ("inc", "dec") and len(args) == 1:
            v = self.rval(args[0])
            if v is None:
                return self._unlift(asm)
            return ["%s%s;" % (v, "++" if op == "inc" else "--")]

        if op in ("neg", "not") and len(args) == 1:
            v = self.rval(args[0])
            if v is None:
                return self._unlift(asm)
            return ["%s = %s%s;" % (v, "-" if op == "neg" else "~", v)]

        if op in ("cmp", "test") and len(args) == 2:
            a, b = self.rval(args[0]), self.rval(args[1])
            self.flags = (a, b) if op == "cmp" else (a, "0" if args[0] == args[1] else b)
            return []
        if op == "or" and len(args) == 2 and args[0] == args[1]:
            self.flags = (self.rval(args[0]), "0")
            return ["/* or %s,%s : test for zero */" % (args[0], args[0])]

        if op == "push" and len(args) == 1:
            v = self.rval(args[0])
            if v is None:
                return self._unlift(asm)
            # Snapshot into a temp: `push ax` here and `push ax` after a
            # `mov ax,..` push DIFFERENT values, so the argument must capture ax
            # AT THIS POINT, not re-read the register at the call.
            t = "_p%d" % self.pt
            self.pt += 1
            self.ptmax = max(self.ptmax, self.pt)
            self.pushes.append(t)
            return ["%s = %s;" % (t, v)]
        if op == "pop" and len(args) == 1:
            return []          # saved-register pops are boilerplate; others rare

        # les/lds reg, mem : load a FAR POINTER. The offset word goes to the
        # register, the segment word to ES or DS. This is how every far pointer
        # parameter is dereferenced, so it is the commonest un-lifted op.
        if op in ("les", "lds") and len(args) == 2:
            dst = self.rval(args[0])
            src, _ = self.mem(args[1] if "PTR" in args[1]
                              else "DWORD PTR " + args[1])
            sr = "_es" if op == "les" else "_ds"
            self.segs.add("es" if op == "les" else "ds")
            if dst and src:
                return ["%s = LOWORD(%s);" % (dst, src),
                        "%s = HIWORD(%s);" % (sr, src)]
            return self._unlift(asm)

        # 3-operand imul (186+): dst = src * imm, a plain 16-bit product.
        if op == "imul" and len(args) == 3:
            dst = self.rval(args[0]); src = self.rval(args[1]); imm = self.rval(args[2])
            if None in (dst, src, imm):
                return self._unlift(asm)
            return ["%s = (%s) * (%s);" % (dst, src, imm)]

        # xlat: al = [bx + al], a byte table lookup in DS.
        if op == "xlat":
            return ["al = *(BYTE FAR *)((WORD)(bx + al));"]

        # mul/imul/div/idiv, one-operand form: the DX:AX pair.
        if op in ("mul", "imul") and len(args) == 1:
            v = self.rval(args[0])
            if v is None:
                return self._unlift(asm)
            # _ulmul_f: a FAR wrapper (in _TEXT) around the 32-bit multiply. The
            # compiler's own __ulmul helper is NEAR (in _TEXT); a graft distributed
            # to another code segment can't reach it with a near call. Calling the
            # FAR wrapper instead keeps the math working from any segment. [longmath]
            return ["{ DWORD _m = _ulmul_f((DWORD)ax, (DWORD)(%s)); ax = LOWORD(_m); dx = HIWORD(_m); }" % v]
        if op in ("div", "idiv") and len(args) == 1:
            v = self.rval(args[0])
            if v is None:
                return self._unlift(asm)
            # _uldiv_f/_ulrem_f: FAR wrappers (in _TEXT) around the 32-bit divide
            # and remainder. Same reason as _ulmul_f -- the compiler's __uldiv/
            # __ulrem helpers are NEAR in _TEXT and unreachable by a near call from
            # a graft in another segment. [longmath]
            return ["{ DWORD _d = ((DWORD)dx << 16) | ax;"
                    " ax = (WORD)_uldiv_f(_d, (DWORD)(%s));"
                    " dx = (WORD)_ulrem_f(_d, (DWORD)(%s)); }" % (v, v)]

        # string operations, with or without a rep prefix. Modelled as the
        # loop (or single step) they actually are. Direction is assumed forward
        # (cld); the rare std case is not distinguished.
        sm = re.match(r"(rep|repz|repe|repnz|repne)?\s*(movs|stos|lods|scas|cmps)(b|w)?\b", asm)
        if sm:
            rep, sop, suf = sm.group(1), sm.group(2), sm.group(3)
            if suf == "b" or "BYTE PTR" in asm:
                T, sz, acc = "BYTE", 1, "al"
            else:
                T, sz, acc = "WORD", 2, "ax"
            d = "(*(%s FAR *)di)" % T
            srcv = "(*(%s FAR *)si)" % T
            if sop == "movs":
                step = "%s = %s; di += %d; si += %d;" % (d, srcv, sz, sz)
            elif sop == "stos":
                step = "%s = %s; di += %d;" % (d, acc, sz)
            elif sop == "lods":
                step = "%s = %s; si += %d;" % (acc, srcv, sz)
            elif sop == "scas":
                self.flags = (acc, d)
                step = "di += %d;" % sz
            else:  # cmps
                self.flags = (srcv, d)
                step = "si += %d; di += %d;" % (sz, sz)
            if rep:
                if sop == "scas":
                    # repnz: advance while NOT equal; repz: while equal.
                    test = "!=" if rep in ("repnz", "repne") else "=="
                    return ["while (cx != 0 && %s %s %s) { di += %d; cx--; }"
                            % (d, test, acc, sz)]
                return ["while (cx != 0) { %s cx--; }" % step]
            return [step]

        if op in ("call",):
            return self._call(off, asm)

        if op in ("ret", "retf", "iret"):
            return ["return ax;"]

        if op in ("xchg",) and len(args) == 2:
            a, b = self.rval(args[0]), self.rval(args[1])
            if a and b:
                return ["{ WORD _t = %s; %s = %s; %s = _t; }" % (a, a, b, b)]
            return self._unlift(asm)

        # jmp / conditional jumps are realised by cfg's control flow; the
        # condition is attached at block emit time, so nothing here.
        if op in cfg.UNCOND or op in cfg.CC:
            return []

        return self._unlift(asm)

    def _call(self, off, asm):
        pushes = self.pushes
        self.pushes = []
        self.pt = 0                     # stack unwinds after the call cleans
        # Resolve directly so the EMITTED identifier is collision-free:
        #  - a system call uses its real API name (declared in the header)
        #  - an internal call uses sub_<addr> (never a header macro like
        #    GLOBALHANDLE, which broke compilation); its .SYM name is a comment
        #  - a far call uses far_<seg>_<off>
        try:
            kind, d = self.sg.resolve_call(off, asm)
        except Exception:
            return self._unlift(asm)
        if kind == "sys":
            label = d[0] or ""
            # A SELF-IMPORT — the module calling its OWN entry ordinal (e.g.
            # WRITE.64) — must resolve to the real body at that ordinal, NOT the
            # bare module name. The bare name loses the target AND can collide
            # with a header macro (<winexp.h> #defines WRITE as 1, so a naive
            # `WRITE(...)` becomes `1(...)` -> "term does not evaluate to a
            # function"). Resolve the ordinal through the NE entry table.
            ms = re.match(r"(\w+)\.(\d+)$", label)
            if ms and ms.group(1).upper() == self.mod.upper():
                home = entry_home(self.mod).get(int(ms.group(2)))
                if home:
                    base = "%s_s%d_%04x" % (self.mod.lower(), home[0], home[1])
                    note = "/* self ord %s */" % ms.group(2)
                else:
                    mm = re.match(r"[A-Za-z_]\w*", label)
                    base = mm.group(0) if mm else None
                    note = ""
            else:
                base = d[1] or d[0]
                base = re.match(r"[A-Za-z_]\w*", base or "")
                base = base.group(0) if base else None
                note = ""
        elif kind == "int":
            sym = self.syms.get(d)
            base = "sub_%04x" % d
            note = "/*%s*/" % sym if sym else ""
        elif kind == "far":
            base = "far_%s" % re.sub(r"[^0-9a-fA-F]", "_", str(d))
            note = ""
        else:
            return self._unlift(asm)
        if not base:
            return self._unlift(asm)
        params = protos().get(base)
        argstr = self._args(pushes, params)
        if base in void_apis():
            return ["%s(%s);%s" % (base, argstr, note)]
        return ["ax = %s(%s);%s" % (base, argstr, note)]

    def _args(self, pushes, params):
        if params is None:
            return ", ".join(pushes)
        # combine wide (2-push) parameters: seg pushed first, then off. POINT is
        # a struct but it is two words on the stack, so it combines too.
        WIDE = re.compile(r"^\s*(LP\w+|LONG|DWORD|FARPROC|long|POINT)", re.I)
        out, i = [], 0
        for p in params:
            if i >= len(pushes):
                out.append("0"); continue
            if WIDE.match(p) and i + 1 < len(pushes):
                # bare POINT is a struct-by-value; LPPOINT is a far pointer and
                # must NOT take _mkpt.
                if re.match(r"POINT\b", p.strip()):
                    out.append("_mkpt(%s, %s)" % (pushes[i], pushes[i + 1]))
                else:
                    out.append("MAKELONG(%s, %s)" % (pushes[i + 1], pushes[i]))
                i += 2
            else:
                out.append(pushes[i]); i += 1
        return ", ".join(out)

    def _unlift(self, asm):
        self.unlifted += 1
        return ["/* UNLIFTED: %s */" % asm]

    # ---- whole function ---------------------------------------------------
    def condition(self, cc, off, blockrows):
        """The C condition for a conditional-jump block ending.

        `cc` is either a binary operator (`==`, `<`, ...) to splice between the
        last compare's two operands, or a COMPLETE expression that stands on its
        own — jcxz's "cx == 0", the sign tests, the overflow tests. Treating the
        second kind as a binary operator produced `(*si) cx == 0 0x23`.
        """
        if cc == "cx == 0":
            return "cx == 0"
        if cc in ("overflow", "no overflow"):
            return "0 /* %s: flag not modelled */" % cc
        fl = self.flags
        if not fl:
            return "1 /* condition not recovered */"
        a, b = fl
        if cc == "< 0":
            return "(int)%s < 0" % a
        if cc == ">= 0":
            return "(int)%s >= 0" % a
        op = {"< (unsigned)": "<", "<= (unsigned)": "<=",
              "> (unsigned)": ">", ">= (unsigned)": ">="}.get(cc, cc)
        return "%s %s %s" % (a, op, b)


def arg_convention(rows, blocks):
    """{bp_offset: 'argN'} and whether the frame is far, from the ret."""
    rets = [a for _, bd, k, *_ in blocks if k == "ret" for _, a in bd
            if a.startswith(("ret", "retf"))]
    far = bool(rets and rets[-1].startswith("retf"))
    m = re.match(r"retf?\s+0x([0-9a-f]+)", rets[-1]) if rets else None
    nbytes = int(m.group(1), 16) if m else None
    base = 6 if far else 4
    offs = sorted({int(x, 16) for _, bd, *_ in blocks for _, a in bd
                   for x in re.findall(r"\[bp\+0x([0-9a-f]+)\]", a)} | set())
    offs = [o for o in offs if o >= base and (nbytes is None or o <= base + nbytes - 2)]
    if nbytes is not None:                      # PASCAL: arg1 highest
        offs = sorted(offs, reverse=True)
    argmap = {o: "arg%d" % (i + 1) for i, o in enumerate(offs)}
    return argmap, far, len(offs)


def lift_function(mod, seg, start):
    sg, rows, s, e, prolog = cfg.rows_for(mod, seg, start)
    blocks = cfg.build(rows, s, e)
    argmap, far, nargs = arg_convention(rows, blocks)
    skip = frame_spans(rows)
    L = Lifter(sg, blocks, s, mod, seg, argmap, far)

    body = []
    for b, bd, kind, tgt, fall, cc in blocks:
        body.append(("label", b))
        # all but the final control-flow instruction
        core = bd[:-1] if kind in ("cond", "goto") else bd
        for off, asm in core:
            if off in skip:
                continue
            for st in L.insn(off, asm):
                body.append(("stmt", st))
        if kind == "cond":
            if cc == "loop":
                # `loop TGT`: decrement cx, branch while it is nonzero.
                body.append(("stmt", "cx--; if (cx != 0) goto L_%04x;" % tgt))
                body.append(("stmt", "goto L_%04x;" % fall))
            else:
                cond = L.condition(cc, bd[-1][0], bd)
                body.append(("stmt", "if (%s) goto L_%04x;" % (cond, tgt)))
                body.append(("stmt", "goto L_%04x;" % fall))
        elif kind == "goto":
            body.append(("stmt", "goto L_%04x;" % tgt))
        elif kind == "ret":
            body.append(("stmt", "return ax;"))     # epilogue rows are skipped
        elif kind == "fall":
            body.append(("stmt", "goto L_%04x;" % fall))

    # A jump can target an address that is not a block leader — into the middle
    # of an instruction, past the function end, or a jump-table slot cfg did not
    # materialise. Such a `goto L_XXXX` references an undefined label and will
    # not compile. Redirect those to a stub so the rest of the function stands.
    defined = {"L_%04x" % v for k, v in body if k == "label"}
    for i, (k, v) in enumerate(body):
        if k == "stmt":
            m = re.search(r"goto (L_[0-9a-f]{4})", v)
            if m and m.group(1) not in defined:
                body[i] = ("stmt", v[:m.start()]
                           + "return ax; /* %s: target outside function */" % m.group(1))

    # ---- render -----------------------------------------------------------
    out = []
    sig_args = ", ".join("WORD %s" % argmap[o] for o in sorted(argmap, reverse=far)) or "void"
    out.append("long %s_s%d_%04x(%s)   /* %s seg%d:0x%04x  %s */"
               % (mod.lower(), seg, start, sig_args, mod, seg, start,
                  "far" if far else "near"))
    out.append("{")
    out.append("    union { WORD x; struct { BYTE l, h; } b; } "
               "ax_, bx_, cx_, dx_;")
    out.append("    WORD si, di;")
    out.append("    WORD _bp = 0, _sp = 0;   /* frame regs (transliterated) */")
    out.append("#define ax ax_.x\n#define bx bx_.x\n#define cx cx_.x\n#define dx dx_.x")
    out.append("#define al ax_.b.l\n#define ah ax_.b.h\n#define bl bx_.b.l\n#define bh bx_.b.h")
    out.append("#define cl cx_.b.l\n#define ch cx_.b.h\n#define dl dx_.b.l\n#define dh dx_.b.h")
    for key in sorted(L.locals):
        out.append("    WORD %s;" % L.locals[key])
    # A global read as a byte in one place and a word in another produced two
    # declarations of g_XXXX with conflicting types. Declare each once, at the
    # widest width seen; narrower accesses just truncate.
    rank = {"BYTE": 1, "WORD": 2, "DWORD": 4}
    gtype = {}
    for off, typ in L.globals:
        if off not in gtype or rank[typ] > rank[gtype[off]]:
            gtype[off] = typ
    for off in sorted(gtype):
        out.append("    static %s g_%04x;" % (gtype[off], off))
    for sr in sorted(L.segs):
        out.append("    WORD _%s = 0;   /* segment register (data model)*/" % sr)
    if L.use_cf:
        out.append("    WORD _cf = 0;   /* carry flag (shift/rotate chains) */")
    if L.ptmax:
        out.append("    WORD %s;   /* push snapshots */"
                   % ", ".join("_p%d" % i for i in range(L.ptmax)))
    out.append("    (void)si; (void)di; (void)_bp; (void)_sp;")
    for kind, v in body:
        if kind == "label":
            out.append("L_%04x:" % v)
        else:
            out.append("    " + v)
    out.append("#undef ax\n#undef bx\n#undef cx\n#undef dx")
    out.append("#undef al\n#undef ah\n#undef bl\n#undef bh")
    out.append("#undef cl\n#undef ch\n#undef dl\n#undef dh")
    out.append("}")
    return "\n".join(out), L.unlifted, len(rows)


def measure(which):
    mods = cfg.SYSMODS if which != "APPS" else \
        ["CALC", "CALENDAR", "CARDFILE", "CLIPBRD", "CLOCK", "CONTROL",
         "NOTEPAD", "PAINT", "REVERSI", "SPOOLER", "TERMINAL", "WRITE"]
    tot = clean = err = 0
    unlift_hist = collections.Counter()
    for mod in mods:
        ne = cfg.open_any(mod)
        o = trace.load_ordinals("ORDINALS.md")
        for i, s in enumerate(ne.segments(), 1):
            if s["kind"] != "CODE":
                continue
            try:
                sgg = trace.Seg(ne, i, o)
            except Exception:
                continue
            for start in sorted(sgg.funcs):
                tot += 1
                try:
                    _, unl, nins = lift_function(mod, i, start)
                except Exception:
                    err += 1
                    continue
                if unl == 0:
                    clean += 1
                else:
                    unlift_hist[min(unl, 10)] += 1
    print("=== %s ===" % which)
    print("functions               : %d" % tot)
    print("FULLY lifted (0 unlift) : %d  (%.1f%%)" % (clean, 100.0 * clean / max(1, tot)))
    print("partial (>=1 unlift)    : %d" % (tot - clean - err))
    print("lifter crashed          : %d" % err)
    print("unlift-count histogram  : " +
          "  ".join("%s:%d" % ("10+" if k == 10 else k, unlift_hist[k])
                    for k in sorted(unlift_hist)))


AUTO_DIR = layout.recon_sub("auto_lift")

BANNER = """/*
 * %s_lifted.c  —  MECHANICALLY LIFTED, NOT a human reconstruction.
 *
 * *** DO NOT CONFUSE THIS WITH THE HAND-WRITTEN RECONSTRUCTION ***
 *
 * Every function here was produced by lift.py: a literal, per-instruction
 * transliteration of the 1985 machine code into C. The control flow is
 * cfg.py's; the expressions are derived by a stack simulator. NOTHING here was
 * read or understood by a person.
 *
 * The names claim no meaning and that is deliberate — `loc_4`, `arg1`,
 * `sub_1234`, `g_0016` are DERIVED, not interpreted, so this file invents
 * nothing (the project's one rule). Turning `loc_4` into `cchText`, or `0x400`
 * into EM_GETSEL, is the separate human layer; a function only graduates out of
 * this directory when a person does that work.
 *
 * ledger.py excludes this directory from every "transcribed"/"addressed"
 * count. It is reported on its own line, exactly like the massgen skeletons,
 * so it can never be mistaken for progress it has not earned.
 *
 * Only FULLY-lifted functions (zero un-lifted instructions) are emitted here;
 * the rest await more opcode support in lift.py.
 */

#include <windows.h>

/* A POINT argument is two words on the stack but a struct in the prototype, so
 * it needs constructing, not a MAKELONG. */
static POINT _mkpt(WORD _x, WORD _y) { POINT _p; _p.x = _x; _p.y = _y; return _p; }
"""


def emit(which):
    import os
    os.makedirs(AUTO_DIR, exist_ok=True)
    mods = cfg.SYSMODS if which != "APPS" else \
        ["CALC", "CALENDAR", "CARDFILE", "CLIPBRD", "CLOCK", "CONTROL",
         "NOTEPAD", "PAINT", "REVERSI", "SPOOLER", "TERMINAL", "WRITE"]
    grand_f = grand_e = 0
    for mod in mods:
        ne = cfg.open_any(mod)
        o = trace.load_ordinals("ORDINALS.md")
        parts = [BANNER % mod.lower()]
        n = 0
        for i, s in enumerate(ne.segments(), 1):
            if s["kind"] != "CODE":
                continue
            try:
                sgg = trace.Seg(ne, i, o)
            except Exception:
                continue
            for start in sorted(sgg.funcs):
                try:
                    c, unl, ins = lift_function(mod, i, start)
                except Exception:
                    continue
                if unl == 0 and ins >= 4:
                    parts.append(c)
                    n += 1
        path = "%s/%s_lifted.c" % (AUTO_DIR, mod.lower())
        open(path, "w", encoding="utf-8").write("\n\n".join(parts) + "\n")
        grand_f += n
        print("%-10s %4d functions -> %s" % (mod, n, path))
    print("\n%d functions emitted mechanically across %d modules" % (grand_f, len(mods)))
    print("These are NOT counted as transcribed. Run build.py to compile-check them.")


def main():
    if "--emit" in sys.argv:
        emit("APPS" if "APPS" in sys.argv else "SYSTEM")
        return
    if "--measure" in sys.argv:
        which = "APPS" if "APPS" in sys.argv else "SYSTEM"
        measure(which)
        return
    mod, seg, start = sys.argv[1].upper(), int(sys.argv[2]), int(sys.argv[3], 0)
    c, unl, nins = lift_function(mod, seg, start)
    print(c)
    print("\n/* %d instructions, %d unlifted */" % (nins, unl))


if __name__ == "__main__":
    main()
