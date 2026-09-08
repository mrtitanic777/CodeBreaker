#!/usr/bin/env python3
r"""msvc2model.py - FORWARD MODEL for the MSVC 2.0 (win95 C2.EXE) code generator, 32-bit.

The 32-bit sibling of mc2model.py: a deterministic `construct -> bytes` predictor of what
MSVC 2.0's backend emits at the retail flags (`/O1 /Oy-`), built the proven way -- probe the
real compiler, find the exact rule, encode it, prove it round-trips byte-exact. This is the
load-bearing first step toward an MSVC-2.0 *inverse* (bytes -> byte-exact C) that reaches the
below-C residual (scheduling / stack-displacement / register choice) which C-shape search
provably cannot -- see win95 docs/reports/MSVC20_ALLOCATOR_MODEL.md.

FIRST CLASS MODELED (this milestone): a single-`return` __stdcall leaf function over int
args/consts --  `int __stdcall f(<n int args>) { return EXPR; }`  where EXPR is a constant, an
argument, `arg <op> const`, or `arg <op> arg`  (op in + - & | ^ *).

Measured MSVC-2.0 rules encoded here (each verified byte-exact against the oracle):
  * const 0 -> `xor eax,eax` (33 c0); other const -> `mov eax,imm32` (b8 id); no frame.
  * any arg use sets up the /Oy- frame: `push ebp`(55) ... `mov ebp,esp`(8b ec) ... `pop ebp`
    `ret 4*nargs` (5d c2 iw).  `mov ebp,esp` sits right AFTER the first load, EXCEPT for imul,
    which defers it to AFTER the multiply.
  * arg is loaded to eax from [esp + 4 + 4*K] (K = 1-based arg index; the +4 is the pushed ebp).
  * arg<op>const:  add 83/0, sub 83/5, and 83/4 (eax,ib);  or/xor use the 2-byte AL form
    0c/34 ib (upper bytes pass through, so the byte op is exact + shorter);  mul 6b c0 ib.
  * arg<op>arg:  add/sub load the LEFT operand first (03 / 2b eax,[right]);  and/or/xor/mul
    load the RIGHT first then op with the left (23 / 0b / 33 / 0f af  eax,[left]) -- MSVC's
    commutative-operand canonicalization.

    wt inverse msvc2model                 # self-test: model vs oracle, byte-exact count
    wt inverse msvc2model --show          # dump each case: predicted vs shipped

Out-of-class constructs are declined (emit returns None) -- never mis-modeled.
"""
import struct
import sys

try:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    _MD = Cs(CS_ARCH_X86, CS_MODE_32)
except Exception:
    _MD = None                                          # only the frame self-test needs it

# ---- construct IR (tiny; the "quad" analogue for this class) ---------------
# node = {"nargs": N, "expr": E}
#   E = ("const", V) | ("arg", K) | ("binop", op, L, R)   with L,R = ("arg",K)|("const",V)
OPS = {"+", "-", "&", "|", "^", "*"}


def _imm32(v):
    return struct.pack("<i", v)


def _argoff(k):
    """Frame offset of the k-th int arg (1-based) once ebp is pushed: [esp+4+4k]."""
    return 4 + 4 * k


# ---- ebp-relative encoders (MSVC 2.0 /O1 /Oy- keeps the frame pointer) -------
# Every arg-using function sets up the standard frame FIRST -- `push ebp`(55)
# `mov ebp,esp`(8b ec) -- then reads args ebp-relative at [ebp + 4 + 4k].  There
# is no esp-relative pre-frame load and no deferred `mov ebp,esp` (an earlier
# model probed the wrong flags and encoded an FPO-style frame; the shipped 9x
# DLLs and the /Oy- oracle both use this standard frame -- verified byte-exact).
_R = {"eax": 0, "ecx": 1, "edx": 2}
_PRO = bytes([0x55, 0x8B, 0xEC])                         # push ebp; mov ebp,esp
_MEMOP = {"+": 0x03, "-": 0x2B, "&": 0x23, "|": 0x0B, "^": 0x33}   # op r32, r/m32


def _ebp_rm(reg, off):
    """ModRM for `<reg>,[ebp+disp8]` (mod=01, rm=101)."""
    return bytes([0x40 | (_R[reg] << 3) | 0x05, off & 0xFF])


def _esp_rm(reg, off):
    """ModRM+SIB for `<reg>,[esp+disp8]` (mod=01, rm=100=SIB, base+index=esp/none)."""
    return bytes([0x40 | (_R[reg] << 3) | 0x04, 0x24, off & 0xFF])


def _ld(reg, k):
    """mov <reg>,[ebp + arg k]  ->  8b /r  (eax:8b 45 ib, ecx:8b 4d ib)."""
    return bytes([0x8B]) + _ebp_rm(reg, _argoff(k))


def _push_arg(k):
    """push dword ptr [ebp + arg k]  ->  ff 75 ib."""
    return bytes([0xFF, 0x75, _argoff(k) & 0xFF])


def _memop(op, reg, k):
    """<op> <reg>,[ebp + arg k]  (imul is the 0f af two-byte form)."""
    if op == "*":
        return bytes([0x0F, 0xAF]) + _ebp_rm(reg, _argoff(k))
    return bytes([_MEMOP[op]]) + _ebp_rm(reg, _argoff(k))


def _epi(nargs):
    """pop ebp; ret 4*nargs  (the __stdcall callee-clean epilogue)."""
    return bytes([0x5D, 0xC2]) + struct.pack("<H", 4 * nargs)


def _op_const_bytes(op, c):
    """Bytes for `eax <op>= c` (c an imm8), or None if unmodeled / out of imm8 range.
    Measured MSVC 2.0 idioms: +/-1 -> inc/dec, a-c -> add eax,(-c), or via al, *5 via lea."""
    if not (-128 <= c <= 127):
        return None
    if op == "+":
        return bytes([0x40]) if c == 1 else bytes([0x48]) if c == -1 else bytes([0x83, 0xC0, c & 0xFF])
    if op == "-":
        return bytes([0x48]) if c == 1 else bytes([0x40]) if c == -1 else bytes([0x83, 0xC0, (-c) & 0xFF])
    if op == "&":
        return bytes([0x83, 0xE0, c & 0xFF])
    if op == "|":
        return bytes([0x0C, c & 0xFF])                       # or al,ib
    if op == "^":
        return bytes([0x83, 0xF0, c & 0xFF])                 # xor eax,ib
    if op == "*":
        return bytes([0x8D, 0x04, 0x80]) if c == 5 else bytes([0x6B, 0xC0, c & 0xFF])  # *5 -> lea; else imul
    return None


def _flatten_chain(E):
    """If E is a left-associative chain of ONE operator over arg leaves -- ((a op b) op c) ... --
    return (op, [argidx leftmost..rightmost]); else None. Used to recognise n-ary chains."""
    if E[0] != "binop":
        return None
    op = E[1]
    leaves, node = [], E
    while node[0] == "binop" and node[1] == op:
        if node[3][0] != "arg":                          # right operand must be an arg leaf
            return None
        leaves.append(node[3][1])
        node = node[2]
    if node[0] != "arg":
        return None
    leaves.append(node[1])
    leaves.reverse()
    return op, leaves


def _ret_body(E):
    """Bytes that compute a simple return expression into eax (NO frame / epilogue) -- the tail
    of a multi-statement function.  Covers const / arg / arg<op>{arg,const} / square.  None if
    out of this small class (the multi-statement milestone keeps return exprs simple)."""
    k = E[0]
    if k == "const":
        v = E[1]
        if v == 0:
            return bytes([0x33, 0xC0])
        if -128 <= v <= 127:
            return bytes([0x6A, v & 0xFF, 0x58])
        return bytes([0xB8]) + _imm32(v)
    if k == "arg":
        return _ld("eax", E[1])
    if k == "binop":
        op, L, R = E[1], E[2], E[3]
        if L[0] == "arg" and R[0] == "const":
            ob = _op_const_bytes(op, R[1])
            return (_ld("eax", L[1]) + ob) if ob else None
        if L[0] == "arg" and R[0] == "arg":
            li, ri = L[1], R[1]
            if op == "*" and li == ri:
                return _ld("eax", li) + bytes([0x0F, 0xAF, 0xC0])
            if op == "+":
                return _ld("eax", ri) + _ld("ecx", li) + bytes([0x03, 0xC1])
            if op == "-":
                return _ld("eax", li) + _memop("-", "eax", ri)
            return _ld("eax", ri) + _memop(op, "eax", li)
    return None


def _materialize(V):
    """Bytes that leave Value V in eax (framed context), or None if not modelable.  The general
    emit primitive the register-dataflow interpreter needs: a value may be an arg, const, global,
    a (indirect) call, a memory load/field off another value, or `X <op> {const|arg}`.  A load
    chain flows through eax: materialize the base into eax, then `mov eax,[eax+disp]`."""
    k = V[0]
    if k == "arg":
        return _ld("eax", V[1])
    if k == "const":
        v = V[1]
        if v == 0:
            return bytes([0x33, 0xC0])
        if -128 <= v <= 127:
            return bytes([0x6A, v & 0xFF, 0x58])
        return bytes([0xB8]) + _imm32(v)
    if k == "global":
        return bytes([0xA1, 0, 0, 0, 0])                     # mov eax,[disp32] (reloc masked)
    if k == "call":
        return _emit_call_seq(V[2])
    if k in ("icall", "icall_ret"):
        return _emit_call_seq(V[1], indirect=True)
    if k in ("load", "deref", "index"):
        if k == "deref":
            base, d = V[1], 0
        elif k == "index":
            base, d = V[1], (V[2][1] * 4 if V[2][0] == "const" else None)
        else:
            base, d = V[1], V[2]
        if d is None:
            return None
        b = _materialize(base)
        if b is None:
            return None
        if d == 0:
            mem = bytes([0x8B, 0x00])                        # mov eax,[eax]
        elif -0x80 <= d < 0x80:
            mem = bytes([0x8B, 0x40, d & 0xFF])              # mov eax,[eax+disp8]
        else:
            mem = bytes([0x8B, 0x80]) + struct.pack("<i", d)  # mov eax,[eax+disp32]
        return b + mem
    if k == "callbinret":                                    # (call) <op> {const|arg}
        _, op, call, other = V
        b = _materialize(call)
        if b is None:
            return None
        if other[0] == "arg":
            return b + _memop(op, "eax", other[1])
        ob = _op_const_bytes(op, other[1])
        return (b + ob) if ob is not None else None
    if k == "binop":
        op, L, R = V[1], V[2], V[3]
        if op not in OPS:
            return None
        if R[0] == "const":                                  # <materialize L> ; op eax,const
            b = _materialize(L)
            ob = _op_const_bytes(op, R[1])
            return (b + ob) if (b is not None and ob is not None) else None
        return None
    return None


def _emit_call_seq(cargs, indirect=False):
    """push args right-to-left, then the call (target reloc = 0, masked).  An arg is a stack
    parameter ('arg', k) -> push [ebp+argk], or an immediate ('const', v) -> push imm8/imm32.
    indirect=True emits `call dword ptr [disp32]` (ff 15, an IAT/import slot) instead of rel32."""
    seq = b""
    for a in reversed(cargs):
        if a[0] == "arg":
            seq += _push_arg(a[1])                           # ff 75 off
        elif a[0] == "const":
            v = a[1]
            seq += bytes([0x6A, v & 0xFF]) if -128 <= v <= 127 else bytes([0x68]) + struct.pack("<i", v)
        else:
            return None
    return seq + (bytes([0xFF, 0x15, 0, 0, 0, 0]) if indirect else bytes([0xE8, 0, 0, 0, 0]))


def _emit_stmt(st):
    """One statement -> bytes (no frame).  voidcall/ivoidcall (a call) or an `if(cond) {body}`
    guard: `cmp [arg],0; je/jne +len(body); <body>` (je guards if(arg), jne guards if(!arg))."""
    if st[0] == "voidcall" and st[1] == "__stdcall":
        return _emit_call_seq(st[2])
    if st[0] == "ivoidcall":
        return _emit_call_seq(st[1], indirect=True)
    if st[0] == "if":
        cond, then = st[1], st[2]
        body = b""
        for s in then:
            bs = _emit_stmt(s)
            if bs is None:
                return None
            body += bs
        if not body or len(body) > 127:                      # rel8 guard only, this milestone
            return None
        if cond[0] in ("nz", "z"):                            # if(arg) / if(!arg): cmp [arg],0
            condb = bytes([0x83, 0x7D, _argoff(cond[1]) & 0xFF, 0x00])
            je = cond[0] == "nz"
        elif cond[0] in ("callnz", "callz"):                  # if(g(args)) : push;call; test eax,eax
            cs = _emit_call_seq(cond[2])
            if cs is None:
                return None
            condb = cs + bytes([0x85, 0xC0])
            je = cond[0] == "callnz"
        elif cond[0] in ("icallnz", "icallz"):               # if((*p)(args)) : push;call [P]; test eax,eax
            cs = _emit_call_seq(cond[1], indirect=True)
            if cs is None:
                return None
            condb = cs + bytes([0x85, 0xC0])
            je = cond[0] == "icallnz"
        else:
            return None
        jcc = bytes([0x74 if je else 0x75, len(body)])        # je -> body runs when true, jne -> when false
        return condb + jcc + body
    return None


def _emit_seq(node):
    """Multi-statement body: a run of statements (void calls / if-guards), then an optional
    trailing return (a simple expr, or a returning call).  frame; <stmts>; [ret]; epi."""
    nargs = node["nargs"]
    seq = _PRO
    for st in node["stmts"]:
        cs = _emit_stmt(st)
        if cs is None:
            return None
        seq += cs
    ret = node.get("ret")
    if ret is not None:
        cs = _materialize(ret)                               # general: call/load/global/binop-const/...
        if cs is None:
            cs = _ret_body(ret)                              # arg<op>arg two-register forms
        if cs is None:
            return None
        seq += cs
    if node.get("leave"):                                    # `leave` teardown (indirect calls, unknown
        epi = bytes([0xC9]) + (bytes([0xC3]) if nargs == 0    # callee convention -> restore esp via ebp)
                               else bytes([0xC2]) + struct.pack("<H", 4 * nargs))
    else:
        epi = _epi(nargs)
    return seq + epi


def _emit_fastcall(node):
    """__fastcall / __thiscall register-convention leaf: arg1->ecx, arg2->edx (>2 args
    decline -- arg3+ would spill to the stack), result in eax, no frame, `ret` (c3).
    Covers arg / deref / index(const) / a+b (lea edx+ecx) / (*arg1)<op>arg2."""
    nargs, E = node["nargs"], node["expr"]
    if nargs > 2:
        return None
    kind = E[0]
    ret = bytes([0xC3])
    if kind == "arg":                                        # mov eax,ecx / mov eax,edx
        if E[1] == 1:
            return bytes([0x8B, 0xC1]) + ret
        if E[1] == 2:
            return bytes([0x8B, 0xC2]) + ret
        return None
    if kind == "deref":                                      # mov eax,[ecx] / [edx]
        a = E[1]
        if a[0] != "arg" or a[1] not in (1, 2):
            return None
        return bytes([0x8B, 0x01 if a[1] == 1 else 0x02]) + ret
    if kind == "index":                                      # mov eax,[ecx+d] / [edx+d]
        p, idx = E[1], E[2]
        if p[0] != "arg" or p[1] not in (1, 2) or idx[0] != "const":
            return None
        d = idx[1] * 4
        if d == 0:
            mm = bytes([0x01 if p[1] == 1 else 0x02])
        elif 0 < d < 0x80:
            mm = bytes([0x41 if p[1] == 1 else 0x42, d])
        else:
            return None
        return bytes([0x8B]) + mm + ret
    if kind == "binop":
        op, L, R = E[1], E[2], E[3]
        if op == "+" and L == ("arg", 1) and R == ("arg", 2):        # a+b -> lea eax,[edx+ecx]
            return bytes([0x8D, 0x04, 0x0A]) + ret
        if (op in _MEMOP and L[0] == "deref" and L[1] == ("arg", 1)  # (*arg1) <op> arg2
                and R == ("arg", 2)):
            return bytes([0x8B, 0x01, _MEMOP[op], 0xC2]) + ret       # mov eax,[ecx]; op eax,edx
        return None
    return None


def _emit_fpo(node):
    """Frameless (/Oy, frame-pointer omission) leaf shapes: NO frame, args read
    esp-relative at [esp + 4k] (SIB 44 24), and the body is otherwise identical to
    the framed form.  Covers the single-return non-call subset (arg / arg<op>{arg,
    const} / chains / deref / index / unary / square); call/global/store frameless
    are declined -- a push shifts esp and the arg offsets with it."""
    nargs, E = node["nargs"], node["expr"]
    kind = E[0]
    ret = bytes([0xC3]) if nargs == 0 else bytes([0xC2]) + struct.pack("<H", 4 * nargs)

    def ld(reg, k):
        return bytes([0x8B]) + _esp_rm(reg, 4 * k)          # mov reg,[esp+4k]

    def memop(op, reg, k):
        if op == "*":
            return bytes([0x0F, 0xAF]) + _esp_rm(reg, 4 * k)
        return bytes([_MEMOP[op]]) + _esp_rm(reg, 4 * k)

    if kind == "arg":
        return ld("eax", E[1]) + ret
    if kind == "unary":
        uop, a = E[1], E[2]
        if a[0] != "arg":
            return None
        if uop == "neg":
            return ld("eax", a[1]) + bytes([0xF7, 0xD8]) + ret
        if uop == "not":
            return ld("eax", a[1]) + bytes([0xF7, 0xD0]) + ret
        if uop == "lognot":
            return bytes([0x33, 0xC0]) + bytes([0x39]) + _esp_rm("eax", 4 * a[1]) + bytes([0x0F, 0x94, 0xC0]) + ret
        return None
    if kind == "deref":
        if E[1][0] != "arg":
            return None
        return ld("eax", E[1][1]) + bytes([0x8B, 0x00]) + ret
    if kind == "index":
        p, idx = E[1], E[2]
        if p[0] != "arg":
            return None
        if idx[0] == "arg":
            return ld("eax", idx[1]) + ld("ecx", p[1]) + bytes([0x8B, 0x04, 0x81]) + ret
        if idx[0] == "const":
            d = idx[1] * 4
            if d == 0:
                mem = bytes([0x8B, 0x00])
            elif 0 < d < 0x80:
                mem = bytes([0x8B, 0x40, d])
            else:
                return None
            return ld("eax", p[1]) + mem + ret
        return None
    if kind == "binop":
        op, L, R = E[1], E[2], E[3]
        if op not in OPS:
            return None
        chain = _flatten_chain(E)
        if chain and len(chain[1]) >= 3:
            cop, lv = chain
            if cop in ("*", "&", "|", "^"):
                seq = ld("eax", lv[-1])
                for k in reversed(lv[:-1]):
                    seq += memop(cop, "eax", k)
                return seq + ret
            if cop == "-":
                head, sub = lv[0], lv[1:]
                seq = ld("eax", head)
                for k in sub[1:] + sub[:1]:
                    seq += memop("-", "eax", k)
                return seq + ret
            return None
        if L[0] == "arg" and R[0] == "const":
            ob = _op_const_bytes(op, R[1])
            if ob is None:
                return None
            return ld("eax", L[1]) + ob + ret
        if L[0] == "arg" and R[0] == "arg":
            li, ri = L[1], R[1]
            if op == "*" and li == ri:
                return ld("eax", li) + bytes([0x0F, 0xAF, 0xC0]) + ret
            if op == "+":
                return ld("eax", ri) + ld("ecx", li) + bytes([0x03, 0xC1]) + ret
            if op == "-":
                return ld("eax", li) + memop("-", "eax", ri) + ret
            return ld("eax", ri) + memop(op, "eax", li) + ret
        return None
    return None


def emit(node):
    """Forward model: construct -> exact MSVC-2.0 bytes, or None if out of class.  A node with
    "fpo":True is the frameless (/Oy) form; otherwise the standard /Oy- framed form."""
    if isinstance(node, dict) and node.get("stmts") is not None:
        return _emit_seq(node)
    try:
        nargs, E = node["nargs"], node["expr"]
    except (KeyError, TypeError):
        return None
    if E is None:
        return None
    if node.get("cc") == "fastcall":
        return _emit_fastcall(node)
    if node.get("fpo"):
        return _emit_fpo(node)
    # M2 callee-saved: return callL <op> callR -- park the right call's result in esi across
    # the left call.  push esi; <eval R>; push L.args; mov esi,eax; call L; op eax,esi; pop esi.
    if (E[0] == "binop" and E[1] in OPS and E[2][0] == "call" and E[3][0] == "call"
            and E[2][1] == "__stdcall" and E[3][1] == "__stdcall"
            and all(a[0] == "arg" for a in E[2][2]) and all(a[0] == "arg" for a in E[3][2])):
        op, cargsL, cargsR = E[1], E[2][2], E[3][2]
        seq = _PRO + bytes([0x56])                           # frame; push esi
        cr = _emit_call_seq(cargsR)                          # push R.args; call R -> eax
        if cr is None:
            return None
        seq += cr
        for a in reversed(cargsL):                           # push L.args
            seq += _push_arg(a[1])
        seq += bytes([0x8B, 0xF0, 0xE8, 0, 0, 0, 0])         # mov esi,eax (save R); call L -> eax
        seq += bytes([0x0F, 0xAF, 0xC6]) if op == "*" else bytes([_MEMOP[op], 0xC6])  # op eax,esi
        return seq + bytes([0x5E]) + _epi(nargs)             # pop esi; epi
    kind = E[0]
    ret_n = bytes([0xC3]) if nargs == 0 else bytes([0xC2]) + struct.pack("<H", 4 * nargs)

    # --- pure constant: no frame -------------------------------------------
    if kind == "const":
        v = E[1]
        if v == 0:
            body = bytes([0x33, 0xC0])                       # xor eax,eax
        elif -128 <= v <= 127:
            body = bytes([0x6A, v & 0xFF, 0x58])             # push imm8; pop eax  (short const)
        else:
            body = bytes([0xB8]) + _imm32(v)                 # mov eax,imm32
        return body + ret_n

    # --- empty body: void f(...) {}  (frame only when it takes args) -------
    if kind == "empty":
        return bytes([0xC3]) if nargs == 0 else _PRO + _epi(nargs)

    # --- unary on one arg:  -a (neg) / ~a (not) / !a (lognot -> a==0) -------
    if kind == "unary":
        uop, a = E[1], E[2]
        if a[0] != "arg":
            return None
        if uop == "neg":
            return _PRO + _ld("eax", a[1]) + bytes([0xF7, 0xD8]) + _epi(nargs)   # neg eax
        if uop == "not":
            return _PRO + _ld("eax", a[1]) + bytes([0xF7, 0xD0]) + _epi(nargs)   # not eax
        if uop == "lognot":                                                      # xor eax,eax; cmp [a],eax; sete al
            return (_PRO + bytes([0x33, 0xC0]) + bytes([0x39]) + _ebp_rm("eax", _argoff(a[1]))
                    + bytes([0x0F, 0x94, 0xC0]) + _epi(nargs))
        return None

    # --- global read / write / inc-dec (leaf; disp32 reloc is masked) ------
    if kind == "gread":                                      # return G
        return None if nargs != 0 else bytes([0xA1, 0, 0, 0, 0]) + ret_n
    if kind == "gwrite":                                     # G = value
        val = E[2]
        if val[0] == "const":                                # mov dword[G],imm32
            return bytes([0xC7, 0x05, 0, 0, 0, 0]) + struct.pack("<i", val[1]) + ret_n
        if val[0] == "arg":                                  # mov eax,[a]; mov [G],eax
            return _PRO + _ld("eax", val[1]) + bytes([0xA3, 0, 0, 0, 0]) + _epi(nargs)
        return None
    if kind == "gincdec":                                    # G++ / G--
        return bytes([0xFF, 0x05 if E[2] > 0 else 0x0D, 0, 0, 0, 0]) + ret_n

    # --- call / indirect call: frame first, push args right-to-left (all ebp) --
    # the rel32 / disp32 target bytes are emitted 0 and MASKED by the verifier.
    if kind in ("call", "icall"):
        conv, cargs = E[1], E[2]
        if not all(a[0] == "arg" for a in cargs):
            return None
        if kind == "call" and not cargs:                     # return g(): tail-call jmp rel32
            return bytes([0xE9, 0, 0, 0, 0])
        seq = _PRO
        for a in reversed(cargs):
            seq += _push_arg(a[1])                           # push dword [ebp+arg]
        seq += bytes([0xE8, 0, 0, 0, 0]) if kind == "call" else bytes([0xFF, 0x15, 0, 0, 0, 0])
        if conv == "__cdecl":
            seq += bytes([0x59])                             # pop ecx  (caller cleans one cdecl arg)
        return seq + _epi(nargs)

    # --- call result combined with one operand:  g(args) <op> {arg|const} --
    # (__stdcall extern only; the result in eax is then op'd with an arg [mem] or a const.)
    if kind == "callbin":
        op, conv, cargs, other = E[1], E[2], E[3], E[4]
        if conv != "__stdcall" or op not in OPS or not all(a[0] == "arg" for a in cargs):
            return None
        seq = _PRO
        for a in reversed(cargs):
            seq += _push_arg(a[1])
        seq += bytes([0xE8, 0, 0, 0, 0])                     # call rel32 (reloc)
        if other[0] == "arg":                                # eax=g(); op eax,[other]  (memory form)
            seq += _memop(op, "eax", other[1])
        elif other[0] == "const":                            # eax=g(); op eax,const
            ob = _op_const_bytes(op, other[1])
            if ob is None:
                return None
            seq += ob
        else:
            return None
        return seq + _epi(nargs)

    # --- everything below sets up the standard /Oy- frame FIRST ------------
    if kind == "arg":                                        # return a
        return _PRO + _ld("eax", E[1]) + _epi(nargs)

    if kind == "deref":                                      # return *p
        if E[1][0] != "arg":
            return None
        return _PRO + _ld("eax", E[1][1]) + bytes([0x8B, 0x00]) + _epi(nargs)

    if kind == "index":
        p, idx = E[1], E[2]
        if p[0] != "arg":
            return None
        if idx[0] == "arg":                                  # p[i]: eax=[i]; ecx=[p]; mov eax,[ecx+eax*4]
            return _PRO + _ld("eax", idx[1]) + _ld("ecx", p[1]) + bytes([0x8B, 0x04, 0x81]) + _epi(nargs)
        if idx[0] == "const":
            d = idx[1] * 4
            if d == 0:
                mem = bytes([0x8B, 0x00])                     # mov eax,[eax]
            elif 0 < d < 0x80:
                mem = bytes([0x8B, 0x40, d])                 # mov eax,[eax+disp8]
            else:
                return None
            return _PRO + _ld("eax", p[1]) + mem + _epi(nargs)
        return None

    if kind == "store":                                      # *p = value  (void)
        p, val = E[1], E[2]
        if p[0] != "arg":
            return None
        if val[0] == "arg":                                  # mov ecx,[p]; mov eax,[v]; mov [ecx],eax
            return _PRO + _ld("ecx", p[1]) + _ld("eax", val[1]) + bytes([0x89, 0x01]) + _epi(nargs)
        if val[0] == "const" and val[1] == 0:                # *p = 0  ->  and dword[eax],0
            return _PRO + _ld("eax", p[1]) + bytes([0x83, 0x20, 0x00]) + _epi(nargs)
        if val[0] == "const":                                # mov dword[eax],imm32
            return _PRO + _ld("eax", p[1]) + bytes([0xC7, 0x00]) + struct.pack("<i", val[1]) + _epi(nargs)
        return None

    if kind == "cmp":                                        # a <rel> b -> 0/1 via setcc
        op, L, R = E[1], E[2], E[3]
        if L[0] != "arg" or R[0] != "arg":
            return None
        SET = {"<": 0x9C, "<=": 0x9E, ">": 0x9F, ">=": 0x9D, "==": 0x94, "!=": 0x95}
        if op not in SET:
            return None
        return (_PRO + _ld("ecx", L[1]) + bytes([0x33, 0xC0])       # mov ecx,[a]; xor eax,eax
                + bytes([0x3B]) + _ebp_rm("ecx", _argoff(R[1]))     # cmp ecx,[b]
                + bytes([0x0F, SET[op], 0xC0]) + _epi(nargs))       # setcc al

    if kind == "binop":
        op, L, R = E[1], E[2], E[3]
        if op not in OPS:
            return None
        # same-op chain over >=3 arg leaves: single accumulator, load last, fold in reverse
        chain = _flatten_chain(E)
        if chain and len(chain[1]) >= 3:
            cop, lv = chain
            if cop in ("*", "&", "|", "^"):                  # commutative: eax=last; op the rest (reverse)
                seq = _ld("eax", lv[-1])
                for k in reversed(lv[:-1]):
                    seq += _memop(cop, "eax", k)
                return _PRO + seq + _epi(nargs)
            if cop == "-":                                   # a-b-c[-..]: subtract 2nd..last first, 1st last
                head, sub = lv[0], lv[1:]
                seq = _ld("eax", head)
                for k in sub[1:] + sub[:1]:
                    seq += _memop("-", "eax", k)
                return _PRO + seq + _epi(nargs)
            if cop == "+":                                   # a+b+c[+..]: eax=last; ecx=2nd-last; add; then +rest
                seq = _ld("eax", lv[-1]) + _ld("ecx", lv[-2]) + bytes([0x03, 0xC1])
                for k in reversed(lv[:-2]):
                    seq += _memop("+", "eax", k)
                return _PRO + seq + _epi(nargs)
            return None

        # --- mixed-precedence / mixed +/- three-operand shapes (two-register) ---
        def _argprod(X):
            if X[0] == "binop" and X[1] == "*" and X[2][0] == "arg" and X[3][0] == "arg":
                return X[2][1], X[3][1]
            return None
        if op in ("+", "-") and _argprod(L) and _argprod(R):     # (a*b) +/- (c*d)
            a, b = _argprod(L)
            c, d = _argprod(R)
            seq = (_ld("eax", b) + _ld("ecx", d) + _memop("*", "eax", a) + _memop("*", "ecx", c)
                   + (bytes([0x03, 0xC1]) if op == "+" else bytes([0x2B, 0xC1])))
            return _PRO + seq + _epi(nargs)
        if op == "+":                                            # (a*b) + c  /  a + (b*c)  -- product first
            for X, Y in ((L, R), (R, L)):
                pr = _argprod(X)
                if pr and Y[0] == "arg":
                    return (_PRO + _ld("eax", pr[1]) + _memop("*", "eax", pr[0])
                            + _memop("+", "eax", Y[1]) + _epi(nargs))
        if (op == "*" and L[0] == "binop" and L[1] == "+"        # (a+b) * c
                and L[2][0] == "arg" and L[3][0] == "arg" and R[0] == "arg"):
            return (_PRO + _ld("eax", L[3][1]) + _ld("ecx", L[2][1]) + bytes([0x03, 0xC1])
                    + _memop("*", "eax", R[1]) + _epi(nargs))
        if (op in ("+", "-") and L[0] == "binop" and L[1] in ("+", "-")   # (a op1 b) op2 c, one '+' one '-'
                and L[2][0] == "arg" and L[3][0] == "arg" and R[0] == "arg"):
            tail = [(L[1], L[3][1]), (op, R[1])]
            plus = [k for o, k in tail if o == "+"]
            minus = [k for o, k in tail if o == "-"]
            if len(plus) == 1 and len(minus) == 1:               # seed the '+' operand, sub the '-', add head
                seq = _ld("eax", plus[0]) + _memop("-", "eax", minus[0]) + _memop("+", "eax", L[2][1])
                return _PRO + seq + _epi(nargs)
        # arg <op> const
        if L[0] == "arg" and R[0] == "const":
            ob = _op_const_bytes(op, R[1])
            if ob is None:
                return None
            return _PRO + _ld("eax", L[1]) + ob + _epi(nargs)
        # arg <op> arg
        if L[0] == "arg" and R[0] == "arg":
            li, ri = L[1], R[1]
            if op == "*" and li == ri:                       # a*a -> imul eax,eax (compiler reuses the load)
                return _PRO + _ld("eax", li) + bytes([0x0F, 0xAF, 0xC0]) + _epi(nargs)
            if op == "+":                                    # eax=b; ecx=a; add eax,ecx  (two-register)
                return _PRO + _ld("eax", ri) + _ld("ecx", li) + bytes([0x03, 0xC1]) + _epi(nargs)
            if op == "-":                                    # eax=a; sub eax,[b]
                return _PRO + _ld("eax", li) + _memop("-", "eax", ri) + _epi(nargs)
            return _PRO + _ld("eax", ri) + _memop(op, "eax", li) + _epi(nargs)  # &|^*: eax=b; op eax,[a]
        return None
    return None


# --- C-source renderer (for oracle validation) ------------------------------
def _c_operand(o):
    return ("arg%d" % o[1]) if o[0] == "arg" else str(o[1])


def _c_expr(E):
    if E[0] == "const":
        return str(E[1])
    if E[0] == "arg":
        return "arg%d" % E[1]
    if E[0] == "deref":
        return "(*%s)" % _c_expr(E[1])
    if E[0] == "index":
        return "%s[%s]" % (_c_expr(E[1]), _c_expr(E[2]))
    return "(%s %s %s)" % (_c_expr(E[2]), E[1], _c_expr(E[3]))   # binop / cmp, left-assoc


def _ptr_args(E, s):
    """Collect arg indices used as pointers (deref/index base) so the signature types them."""
    if E[0] == "deref":
        if E[1][0] == "arg":
            s.add(E[1][1])
        _ptr_args(E[1], s)
    elif E[0] == "index":
        if E[1][0] == "arg":
            s.add(E[1][1])
    elif E[0] == "store":
        if E[1][0] == "arg":
            s.add(E[1][1])
    elif E[0] in ("binop", "cmp"):
        _ptr_args(E[2], s)
        _ptr_args(E[3], s)


def _to_c_seq(node):
    """C for a straight-line multi-statement node: extern decls + a void/int body."""
    nargs = node["nargs"]
    params = "void" if nargs == 0 else ", ".join("int arg%d" % (i + 1) for i in range(nargs))
    externs, body = [], []
    ctr = [0]

    def callC(cargs, indirect=False):
        gsig = ", ".join("int" for _ in cargs) or "void"
        if indirect:                                         # call through a function pointer (IAT slot)
            externs.append("extern int (__stdcall *p%d)(%s);" % (ctr[0], gsig))
            nm = "(*p%d)" % ctr[0]
        else:
            externs.append("extern int __stdcall g%d(%s);" % (ctr[0], gsig))
            nm = "g%d" % ctr[0]
        argsC = ", ".join(("arg%d" % a[1]) if a[0] == "arg" else str(a[1]) for a in cargs)
        ctr[0] += 1
        return "%s(%s)" % (nm, argsC)

    def stmtC(st):
        if st[0] == "ivoidcall":
            return callC(st[1], True) + ";"
        if st[0] == "voidcall":
            return callC(st[2]) + ";"
        if st[0] == "if":
            cond, then = st[1], st[2]
            if cond[0] == "nz":
                c = "arg%d" % cond[1]
            elif cond[0] == "z":
                c = "!arg%d" % cond[1]
            elif cond[0] == "callnz":
                c = callC(cond[2])
            elif cond[0] == "callz":
                c = "!" + callC(cond[2])
            elif cond[0] == "icallnz":
                c = callC(cond[1], True)
            elif cond[0] == "icallz":
                c = "!" + callC(cond[1], True)
            else:
                c = "0"
            return "if (%s) { %s }" % (c, " ".join(stmtC(s) for s in then))
        return ""

    for st in node["stmts"]:
        body.append(stmtC(st))
    def valC(V):
        k = V[0]
        if k == "arg":
            return "arg%d" % V[1]
        if k == "const":
            return str(V[1])
        if k == "global":
            if "extern int G;" not in externs:
                externs.insert(0, "extern int G;")
            return "G"
        if k == "call":
            return callC(V[2])
        if k in ("icall", "icall_ret"):
            return callC(V[1], True)
        if k == "deref":
            return "(*%s)" % valC(V[1])
        if k == "index":
            return "%s[%d]" % (valC(V[1]), V[2][1])
        if k == "load":
            return "(*%s)" % valC(V[1]) if V[2] == 0 else "(*(int*)((char*)%s + %d))" % (valC(V[1]), V[2])
        if k == "callbinret":
            _, op, call, other = V
            oc = ("arg%d" % other[1]) if other[0] == "arg" else str(other[1])
            return "(%s %s %s)" % (valC(call), op, oc)
        if k == "binop":
            return "(%s %s %s)" % (valC(V[2]), V[1], valC(V[3]))
        return "0"

    ret = node.get("ret")
    rtype = "void"
    if ret is not None:
        rtype = "int"
        body.append("return " + valC(ret) + ";")
    return "%s %s __stdcall f(%s) { %s }" % (" ".join(externs), rtype, params, " ".join(body))


def to_c(node):
    if isinstance(node, dict) and node.get("stmts") is not None:
        return _to_c_seq(node)
    nargs, E = node["nargs"], node["expr"]
    ptr = set()
    _ptr_args(E, ptr)
    params = "void" if nargs == 0 else ", ".join(
        "int%s arg%d" % ("*" if (i + 1) in ptr else "", i + 1) for i in range(nargs))
    if node.get("cc") == "fastcall":                 # register-convention leaf (arg/deref/index/binop)
        return "int __fastcall f(%s) { return %s; }" % (params, _c_expr(E))
    if E[0] == "binop" and E[2][0] == "call" and E[3][0] == "call":   # return g0(..) op g1(..)
        L, R = E[2], E[3]
        return ("extern int __stdcall g0(%s); extern int __stdcall g1(%s); int __stdcall f(%s)"
                " { return g0(%s) %s g1(%s); }" % (
                    ", ".join("int" for _ in L[2]) or "void", ", ".join("int" for _ in R[2]) or "void",
                    params, ", ".join("arg%d" % a[1] for a in L[2]), E[1],
                    ", ".join("arg%d" % a[1] for a in R[2])))
    if E[0] == "store":                              # void: *argP = value;
        return "void __stdcall f(%s) { *arg%d = %s; }" % (params, E[1][1], _c_expr(E[2]))
    if E[0] == "call":                               # extern g(...); return g(args);
        conv, cargs = E[1], E[2]
        gsig = ", ".join("int" for _ in cargs) or "void"
        callargs = ", ".join("arg%d" % a[1] for a in cargs)
        return ("extern int %s g(%s); int __stdcall f(%s) { return g(%s); }"
                % (conv, gsig, params, callargs))
    if E[0] == "gread":
        return "extern int G; int __stdcall f(void) { return G; }"
    if E[0] == "gwrite":
        return "extern int G; void __stdcall f(%s) { G = %s; }" % (params, _c_expr(E[2]))
    if E[0] == "gincdec":
        return "extern int G; void __stdcall f(void) { G %s; }" % ("++" if E[2] > 0 else "--")
    if E[0] == "icall":
        conv, cargs = E[1], E[2]
        gsig = ", ".join("int" for _ in cargs) or "void"
        callargs = ", ".join("arg%d" % a[1] for a in cargs)
        return ("extern int (%s *P)(%s); int __stdcall f(%s) { return P(%s); }"
                % (conv, gsig, params, callargs))
    if E[0] == "callbin":                            # return g(args) <op> {arg|const};
        op, conv, cargs, other = E[1], E[2], E[3], E[4]
        gsig = ", ".join("int" for _ in cargs) or "void"
        callargs = ", ".join("arg%d" % a[1] for a in cargs)
        oc = ("arg%d" % other[1]) if other[0] == "arg" else str(other[1])
        return ("extern int %s g(%s); int __stdcall f(%s) { return g(%s) %s %s; }"
                % (conv, gsig, params, callargs, op, oc))
    if E[0] == "empty":                              # void f(...) {}
        return "void __stdcall f(%s) {}" % params
    if E[0] == "unary":                              # return -a / ~a / !a
        return "int __stdcall f(%s) { return %sarg%d; }" % (
            params, {"neg": "-", "not": "~", "lognot": "!"}[E[1]], E[2][1])
    return "int __stdcall f(%s) { return %s; }" % (params, _c_expr(E))


# --- the battery: every construct in the class ------------------------------
def battery():
    cases = []
    cases.append(("ret0", {"nargs": 0, "expr": ("const", 0)}))
    cases.append(("ret1", {"nargs": 0, "expr": ("const", 1)}))
    cases.append(("retBig", {"nargs": 0, "expr": ("const", 0x1234)}))
    cases.append(("retNeg", {"nargs": 0, "expr": ("const", -7)}))
    cases.append(("arg1", {"nargs": 1, "expr": ("arg", 1)}))
    cases.append(("arg2of2", {"nargs": 2, "expr": ("arg", 2)}))
    for op in ("+", "-", "&", "|", "^", "*"):
        cases.append(("a%sc" % op, {"nargs": 1, "expr": ("binop", op, ("arg", 1), ("const", 5))}))
        cases.append(("a%sb" % op, {"nargs": 2, "expr": ("binop", op, ("arg", 1), ("arg", 2))}))
    # multi-op: multiplicative chains (modeled byte-exact), left-associative over arg leaves
    def _chain(op, idxs):
        e = ("arg", idxs[0])
        for i in idxs[1:]:
            e = ("binop", op, e, ("arg", i))
        return e
    cases.append(("a*b*c", {"nargs": 3, "expr": _chain("*", [1, 2, 3])}))
    cases.append(("a*b*c*d", {"nargs": 4, "expr": _chain("*", [1, 2, 3, 4])}))
    # two-register additive / mixed-precedence (modeled byte-exact)
    A = lambda i: ("arg", i)
    cases.append(("a+b+c", {"nargs": 3, "expr": ("binop", "+", ("binop", "+", A(1), A(2)), A(3))}))
    cases.append(("a+b-c", {"nargs": 3, "expr": ("binop", "-", ("binop", "+", A(1), A(2)), A(3))}))
    cases.append(("a-b+c", {"nargs": 3, "expr": ("binop", "+", ("binop", "-", A(1), A(2)), A(3))}))
    cases.append(("a*b+c", {"nargs": 3, "expr": ("binop", "+", ("binop", "*", A(1), A(2)), A(3))}))
    cases.append(("a+b*c", {"nargs": 3, "expr": ("binop", "+", A(1), ("binop", "*", A(2), A(3)))}))
    # pointer / deref
    cases.append(("*p", {"nargs": 1, "expr": ("deref", A(1))}))
    cases.append(("p[2]", {"nargs": 1, "expr": ("index", A(1), ("const", 2))}))
    cases.append(("p[0]", {"nargs": 1, "expr": ("index", A(1), ("const", 0))}))
    # comparison -> 0/1
    for o in ("<", "<=", ">", ">=", "==", "!="):
        cases.append(("a%sb" % o, {"nargs": 2, "expr": ("cmp", o, A(1), A(2))}))
    # inc/dec (+/-1)  and  store *p = value  (void)
    cases.append(("a+1", {"nargs": 1, "expr": ("binop", "+", A(1), ("const", 1))}))
    cases.append(("a-1", {"nargs": 1, "expr": ("binop", "-", A(1), ("const", 1))}))
    cases.append(("store", {"nargs": 2, "expr": ("store", A(1), A(2))}))
    cases.append(("store0", {"nargs": 1, "expr": ("store", A(1), ("const", 0))}))
    # pop-flip family (base-flip cracked): sub-chains + indexed load
    def _sub(idxs):
        e = A(idxs[0])
        for i in idxs[1:]:
            e = ("binop", "-", e, A(i))
        return e
    cases.append(("a-b-c", {"nargs": 3, "expr": _sub([1, 2, 3])}))
    cases.append(("a-b-c-d", {"nargs": 4, "expr": _sub([1, 2, 3, 4])}))
    cases.append(("a-b-c-d-e", {"nargs": 5, "expr": _sub([1, 2, 3, 4, 5])}))
    cases.append(("p[i]", {"nargs": 2, "expr": ("index", A(1), A(2))}))
    # remaining pop-flip family (rules known)
    cases.append(("a+b+c+d", {"nargs": 4, "expr": _chain("+", [1, 2, 3, 4])}))
    cases.append(("a+b+c+d+e", {"nargs": 5, "expr": _chain("+", [1, 2, 3, 4, 5])}))
    cases.append(("(a+b)*c", {"nargs": 3, "expr": ("binop", "*", ("binop", "+", A(1), A(2)), A(3))}))
    cases.append(("a*b+c*d", {"nargs": 4, "expr": ("binop", "+", ("binop", "*", A(1), A(2)),
                                                   ("binop", "*", A(3), A(4)))}))
    cases.append(("a&b&c", {"nargs": 3, "expr": _chain("&", [1, 2, 3])}))
    cases.append(("a*b-c*d", {"nargs": 4, "expr": ("binop", "-", ("binop", "*", A(1), A(2)),
                                                   ("binop", "*", A(3), A(4)))}))
    # function calls (#1) -- reloc bytes masked
    cases.append(("g()", {"nargs": 0, "expr": ("call", "__stdcall", [])}))
    cases.append(("g(a)", {"nargs": 1, "expr": ("call", "__stdcall", [A(1)])}))
    cases.append(("g(a,b)", {"nargs": 2, "expr": ("call", "__stdcall", [A(1), A(2)])}))
    cases.append(("g(a,b,c)", {"nargs": 3, "expr": ("call", "__stdcall", [A(1), A(2), A(3)])}))
    cases.append(("g_cdecl(a)", {"nargs": 1, "expr": ("call", "__cdecl", [A(1)])}))
    # globals + indirect call (#2 foundation)
    cases.append(("getG", {"nargs": 0, "expr": ("gread", "G")}))
    cases.append(("setGc", {"nargs": 0, "expr": ("gwrite", "G", ("const", 5))}))
    cases.append(("setG", {"nargs": 1, "expr": ("gwrite", "G", A(1))}))
    cases.append(("incG", {"nargs": 0, "expr": ("gincdec", "G", 1)}))
    cases.append(("P(a)", {"nargs": 1, "expr": ("icall", "__stdcall", [A(1)])}))
    cases.append(("P(a,b)", {"nargs": 2, "expr": ("icall", "__stdcall", [A(1), A(2)])}))
    # empty bodies + unary
    cases.append(("empty0", {"nargs": 0, "expr": ("empty",)}))
    cases.append(("empty1", {"nargs": 1, "expr": ("empty",)}))
    cases.append(("empty2", {"nargs": 2, "expr": ("empty",)}))
    cases.append(("neg", {"nargs": 1, "expr": ("unary", "neg", A(1))}))
    cases.append(("not", {"nargs": 1, "expr": ("unary", "not", A(1))}))
    cases.append(("lognot", {"nargs": 1, "expr": ("unary", "lognot", A(1))}))
    # square (imul eax,eax) + call result combined with an operand
    cases.append(("a*a", {"nargs": 1, "expr": ("binop", "*", A(1), A(1))}))
    cases.append(("g(a)+b", {"nargs": 2, "expr": ("callbin", "+", "__stdcall", [A(1)], A(2))}))
    cases.append(("g(a)+c", {"nargs": 1, "expr": ("callbin", "+", "__stdcall", [A(1)], ("const", 1))}))
    cases.append(("g(a,b)&c", {"nargs": 2, "expr": ("callbin", "&", "__stdcall", [A(1), A(2)], ("const", 7))}))
    return cases


def declined_battery():
    """Cases the model must DECLINE (emit -> None) until their scheduling is modeled: additive
    chains (2nd-register reassociation), subtraction chains (esp/ebp base flip), mixed precedence."""
    def _chain(op, idxs):
        e = ("arg", idxs[0])
        for i in idxs[1:]:
            e = ("binop", op, e, ("arg", i))
        return e
    # everything probed so far is now modeled; keep a couple of genuinely-unmodeled shapes as guards
    A = lambda i: ("arg", i)
    return [
        ("a/b", {"nargs": 2, "expr": ("binop", "/", A(1), A(2))}),          # division not modeled
        ("a<<b", {"nargs": 2, "expr": ("binop", "<<", A(1), A(2))}),        # shifts not modeled
    ]


# ===========================================================================
# 32-BIT FRAME-LAYOUT MODEL  (the mc2 framemodel.py analogue)
# ---------------------------------------------------------------------------
# Measured MSVC-2.0 /O1 /Oy- stack-frame allocator (address-taken locals -> forced to stack):
#   * frame:  push ebp; mov ebp,esp; sub esp,N   /   epilogue: mov esp,ebp; pop ebp; ret[4*nargs]
#   * each local's size rounds UP to 4 (char[10] -> 12).
#   * ORDERING: scalars first (top, nearest ebp), then arrays; within each class, DECLARATION
#     order. (`char buf[6]; int a;` -> a at [ebp-4], buf at [ebp-0xc] -- the scalar wins the top
#     slot despite being declared second. All-scalar or all-array => plain declaration order.)
#   * offset(local) = -(cumulative rounded size through that local); framesize = that cumulative.
# ===========================================================================
def _roundup4(n):
    return (n + 3) & ~3


def frame_layout(locals_):
    """locals_ = [(size_bytes, is_array), ...] in DECLARATION order.
    Returns (framesize, {decl_index: ebp_offset}) -- offset negative, i.e. [ebp+offset]."""
    order = ([i for i, (_, arr) in enumerate(locals_) if not arr] +
             [i for i, (_, arr) in enumerate(locals_) if arr])       # scalars first, then arrays
    cum = 0
    off = {}
    for i in order:
        cum += _roundup4(locals_[i][0])
        off[i] = -cum
    return cum, off                                                  # cum already a multiple of 4


def _frame_battery():
    # (name, C source, locals-in-decl-order[(size,is_array)])
    G = "extern void __stdcall g(void*); "
    return [
        ("1int",   G + "int __stdcall f(void){ int a; g(&a); return a; }", [(4, False)]),
        ("2int",   G + "int __stdcall f(void){ int a,b; g(&a); g(&b); return a+b; }", [(4, False), (4, False)]),
        ("3int",   G + "int __stdcall f(void){ int a,b,c; g(&a); g(&b); g(&c); return a+b+c; }",
         [(4, False), (4, False), (4, False)]),
        ("2int_rev", G + "int __stdcall f(void){ int a,b; g(&b); g(&a); return a+b; }", [(4, False), (4, False)]),
        ("buf8",   G + "int __stdcall f(void){ char buf[8]; g(buf); return 0; }", [(8, True)]),
        ("buf10",  G + "int __stdcall f(void){ char buf[10]; g(buf); return 0; }", [(10, True)]),
        ("int_buf", G + "int __stdcall f(void){ int a; char buf[8]; g(&a); g(buf); return a; }",
         [(4, False), (8, True)]),
        ("buf_int", G + "int __stdcall f(void){ char buf[6]; int a; g(buf); g(&a); return a; }",
         [(6, True), (4, False)]),
    ]


def _extract_frame(code):
    """From compiled bytes: (framesize from `sub esp,N`, sorted set of distinct [ebp-X] offsets)."""
    import re
    fs = 0
    offs = set()
    for ins in _MD.disasm(code, 0):
        if ins.mnemonic == "sub" and ins.op_str.startswith("esp,"):
            fs = int(ins.op_str.split(",")[1], 0)
        m = re.findall(r"\[ebp - (0x[0-9a-f]+|\d+)\]", ins.op_str)
        for x in m:
            offs.add(-int(x, 0))
    return fs, sorted(offs)


def frame_selftest(show=False):
    cc32 = _oracle()
    ok = tot = 0
    for name, src, locals_ in _frame_battery():
        tot += 1
        pred_fs, pred_off = frame_layout(locals_)
        pred_set = sorted(set(pred_off.values()))
        r = cc32.compile_c(src, flags="/c /O1 /Oy-")
        if "obj" not in r:
            print("  %-8s ORACLE-FAIL" % name); continue
        fns = cc32.Obj(r["obj"]).functions()
        fn = next((v for k, v in fns.items() if k.startswith("_f")), None)
        real_fs, real_off = _extract_frame(bytes(fn[0])) if fn else (-1, [])
        match = (pred_fs == real_fs) and (pred_set == real_off)
        ok += match
        if show or not match:
            print("  %-8s %s  pred(fs=%d,off=%s) real(fs=%d,off=%s)" %
                  (name, "OK " if match else "MISS", pred_fs, pred_set, real_fs, real_off))
    print("\nmsvc2 frame-layout self-test: %d/%d byte-exact  (MSVC 2.0 /O1 /Oy-)" % (ok, tot))
    return ok == tot


def _oracle():
    """Wire MSVC-2.0 cc32 via the win95 target. cc32._vc_root() finds the vendored msvc20
    toolchain under the win95 DATA root's docs/reference/toolchain -- and it keys off cwd, so
    chdir there (data lives outside the tool repo; point at it with WT_DATA_ROOT)."""
    import os
    from engine.foundation import cores
    here = os.path.dirname(os.path.abspath(__file__))
    cb = os.path.dirname(os.path.dirname(here))
    if cores._active_dir is None:
        cores.set_target(os.path.join(cb, "engine", "targets", "win95"))
    # locate the win95 data root (has docs/reference/toolchain/msvc20/VC/BIN/CL.EXE)
    roots = []
    if os.environ.get("WT_DATA_ROOT"):
        roots.append(os.path.join(os.environ["WT_DATA_ROOT"], "win95"))
    roots += [os.path.join(os.path.dirname(cb), "win95"), os.getcwd()]
    for r in roots:
        if os.path.exists(os.path.join(r, "docs", "reference", "toolchain",
                                       "msvc20", "VC", "BIN", "CL.EXE")):
            os.chdir(r)
            break
    return cores.get("cc32")


def selftest(show=False):
    cc32 = _oracle()
    ok = tot = 0
    for name, node in battery():
        pred = emit(node)
        tot += 1
        r = cc32.compile_c(to_c(node), flags="/c /O1 /Oy-")
        if "obj" not in r:
            print("  %-8s ORACLE-FAIL %s" % (name, str(r.get("err", ""))[:80])); continue
        fns = cc32.Obj(r["obj"]).functions()
        fn = next((v for k, v in fns.items() if k.startswith("_f")), None)
        real = bytes(fn[0]) if fn else b""
        mask = set(fn[1]) if fn else set()               # reloc byte positions -> ignore
        match = pred is not None and len(pred) == len(real) and all(
            pred[i] == real[i] for i in range(len(real)) if i not in mask)
        ok += match
        if show or not match:
            tag = "OK " if match else "MISS"
            print("  %-8s %s  pred=%s  real=%s" % (name, tag,
                  pred.hex(" ") if pred else "(declined)", real.hex(" ")))
    print("\nmsvc2model self-test: %d/%d byte-exact  (MSVC 2.0 /O1 /Oy-)" % (ok, tot))
    # correctly-declined check: unmodeled shapes must return None (never a wrong guess)
    dec_ok = dec_tot = 0
    for name, node in declined_battery():
        dec_tot += 1
        d = emit(node) is None
        dec_ok += d
        if show or not d:
            print("  declined %-8s %s" % (name, "OK (None)" if d else "LEAK: emitted bytes!"))
    print("declined-correctly: %d/%d  (unmodeled shapes return None, never mis-modeled)"
          % (dec_ok, dec_tot))
    return ok == tot and dec_ok == dec_tot


def main():
    args = sys.argv[1:]
    show = "--show" in args
    if "--frame" in args:
        ok = frame_selftest(show=show)
    else:
        ok = selftest(show=show)
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
