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


def _load_eax(k):
    """mov eax, [esp + off]  ->  8b 44 24 ib   (off always < 0x80 for our arg range)."""
    return bytes([0x8B, 0x44, 0x24, _argoff(k)])


# arg <op> const  -> the byte sequence for the op (operand already in eax)
def _op_const(op, c):
    # +/-1 use the 1-byte inc/dec, not add/sub imm (measured)
    if (op == "+" and c == 1) or (op == "-" and c == -1):
        return bytes([0x40])                            # inc eax
    if (op == "-" and c == 1) or (op == "+" and c == -1):
        return bytes([0x48])                            # dec eax
    if op == "+":
        return bytes([0x83, 0xC0, c & 0xFF])            # add eax, ib
    if op == "-":
        return bytes([0x83, 0xE8, c & 0xFF])            # sub eax, ib
    if op == "&":
        return bytes([0x83, 0xE0, c & 0xFF])            # and eax, ib
    if op == "|":
        return bytes([0x0C, c & 0xFF])                  # or  al, ib
    if op == "^":
        return bytes([0x34, c & 0xFF])                  # xor al, ib
    if op == "*":
        return bytes([0x6B, 0xC0, c & 0xFF])            # imul eax, eax, ib
    return None


# arg <op> arg  -> (which operand to LOAD first, the op-with-memory bytes vs the other operand)
_MEMOP = {"+": 0x03, "-": 0x2B, "&": 0x23, "|": 0x0B, "^": 0x33}   # op eax, [esp+off]
def _binargs_plan(op, li, ri):
    """Return (load_idx, other_idx, op_encoder). add/sub load LEFT first; commutative
    (and/or/xor/mul) load RIGHT first, then op against the LEFT."""
    if op in ("+", "-"):
        load, other = li, ri
    else:                                                # & | ^ *  -> right first
        load, other = ri, li
    def enc(off):
        if op == "*":
            return bytes([0x0F, 0xAF, 0x44, 0x24, off])  # imul eax, [esp+off]
        return bytes([_MEMOP[op], 0x44, 0x24, off])      # op eax, [esp+off]
    return load, other, enc


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


def _emit_mul_chain(nargs, idxs):
    """a*b*c[*d...] -> eax = rightmost; imul the rest RIGHT-TO-LEFT; frame deferred past the
    imuls (all operands read esp-relative, before `mov ebp,esp`). Measured byte-exact."""
    pro = bytes([0x55])
    seq = _load_eax(idxs[-1])                             # eax = rightmost leaf
    for k in reversed(idxs[:-1]):                         # imul eax, [esp+arg]  (reverse order)
        seq += bytes([0x0F, 0xAF, 0x44, 0x24, _argoff(k)])
    seq += bytes([0x8B, 0xEC])                            # mov ebp, esp  (deferred, imul style)
    return pro + seq + bytes([0x5D]) + bytes([0xC2]) + struct.pack("<H", 4 * nargs)


# --- a small ModRM encoder for the two-register evaluator --------------------
_R = {"eax": 0, "ecx": 1, "edx": 2}
_ALU = {"+": 0x03, "-": 0x2B, "&": 0x23, "|": 0x0B, "^": 0x33}   # op r32, r/m32


def _rm(reg, base, off):
    """ModRM(+SIB) for `<reg>, [base+off]` with a disp8. base in {'esp','ebp'} -- MSVC uses the
    esp (SIB) form for a ref emitted BEFORE `mov ebp,esp`, the ebp (no-SIB) form for one after."""
    rb = _R[reg]
    if base == "esp":
        return bytes([0x40 | (rb << 3) | 0x04, 0x24, off & 0xFF])
    return bytes([0x40 | (rb << 3) | 0x05, off & 0xFF])          # ebp


def _push_mem(base, off):
    """push dword ptr [base+off]  (esp form has a SIB byte, ebp form doesn't)."""
    return bytes([0xFF, 0x74, 0x24, off & 0xFF]) if base == "esp" else bytes([0xFF, 0x75, off & 0xFF])


def _mov(reg, base, off):    return bytes([0x8B]) + _rm(reg, base, off)
def _alu(op, reg, base, off): return bytes([_ALU[op]]) + _rm(reg, base, off)
def _imul(reg, base, off):   return bytes([0x0F, 0xAF]) + _rm(reg, base, off)
def _add_rr(dst, src):       return bytes([0x03, 0xC0 | (_R[dst] << 3) | _R[src]])
_FRAME = bytes([0x8B, 0xEC])


def _flatten_addsub(E):
    """A left-assoc +/- chain over ARG leaves -> (head_argidx, [(op, argidx) ...tail]) or None.
    The head carries an implicit '+'. Any non-arg leaf (e.g. a product) makes it None."""
    if E[0] != "binop" or E[1] not in ("+", "-"):
        return None
    tail, node = [], E
    while node[0] == "binop" and node[1] in ("+", "-"):
        if node[3][0] != "arg":
            return None
        tail.append((node[1], node[3][1]))
        node = node[2]
    if node[0] != "arg":
        return None
    tail.reverse()
    return node[1], tail


def _as_arg_product(X):
    """X == argA * argB  ->  (A, B)  else None."""
    if X[0] == "binop" and X[1] == "*" and X[2][0] == "arg" and X[3][0] == "arg":
        return X[2][1], X[3][1]
    return None


def _epi(nargs):
    return bytes([0x5D]) + bytes([0xC2]) + struct.pack("<H", 4 * nargs)


def _emit_add3(nargs, head, tail):
    """3-operand +/- chain with >=1 '+': the '+' tail operand becomes the ecx base (loaded
    esp-relative, pre-frame), the other tail operand is applied with its op (ebp-relative,
    post-frame), then eax = head, eax += ecx. Uniform schedule, measured byte-exact."""
    (opb, b), (opc, c) = tail
    if opc == "+":
        base, other, opo = c, b, opb
    else:                                                 # opb == '+'
        base, other, opo = b, c, opc
    seq = (_mov("ecx", "esp", _argoff(base)) + _FRAME
           + _alu(opo, "ecx", "ebp", _argoff(other))
           + _mov("eax", "ebp", _argoff(head)) + _add_rr("eax", "ecx"))
    return bytes([0x55]) + seq + _epi(nargs)


def _emit_bitchain(op, nargs, args):
    """a&b&c (also |, ^): commutative chain -- load LAST, op the rest in reverse; the frame is set
    right after the load (first-2-esp then ebp), unlike mul which defers the frame to the end."""
    seq = _load_eax(args[-1]) + _FRAME + _alu(op, "eax", "esp", _argoff(args[-2]))
    for k in reversed(args[:-2]):
        seq += _alu(op, "eax", "ebp", _argoff(k))
    return bytes([0x55]) + seq + _epi(nargs)


def _pairswap(xs):
    """Swap adjacent elements in pairs: [a,b,c,d]->[b,a,d,c] (self-inverse). The generic
    scheduler's reorder of independent subtracted operands in a sub-chain."""
    out, i = [], 0
    while i < len(xs):
        if i + 1 < len(xs):
            out += [xs[i + 1], xs[i]]; i += 2
        else:
            out.append(xs[i]); i += 1
    return out


def _emit_sub_chain(nargs, head, subargs):
    """a-b-c[-d..]: eax=head; frame; sub the rest. Base-flip rule for the eax accumulator: the
    FIRST subtract reads esp-relative, the rest ebp. Subtracted operands are pair-swapped."""
    order = _pairswap(subargs)
    seq = _load_eax(head) + _FRAME + _alu("-", "eax", "esp", _argoff(order[0]))
    for k in order[1:]:
        seq += _alu("-", "eax", "ebp", _argoff(k))
    return bytes([0x55]) + seq + _epi(nargs)


def _emit_add_chain(nargs, args):
    """a+b+c+d[+...] (n>=4, all '+'): ecx = last; add (n-1)th; eax = head; add the remaining
    middle operands (reverse); pop; add eax,ecx. Base: only the ecx load is esp, rest ebp.
    pop comes BEFORE the final combine for n>=4 (n=3 is _emit_add3, combine-then-pop)."""
    seq = (_mov("ecx", "esp", _argoff(args[-1])) + _FRAME
           + _alu("+", "ecx", "ebp", _argoff(args[-2]))
           + _mov("eax", "ebp", _argoff(args[0])))
    for i in range(len(args) - 3, 0, -1):                # middle operands args[n-3]..args[1], reverse
        seq += _alu("+", "ecx", "ebp", _argoff(args[i]))
    seq += bytes([0x5D]) + _add_rr("eax", "ecx")         # pop; add eax,ecx (pop before combine)
    return bytes([0x55]) + seq + bytes([0xC2]) + struct.pack("<H", 4 * nargs)


def _emit_prod_leaf(nargs, pL, pR, H):
    """(argpL * argpR) + argH  (either source order): product in ecx (ecx=pR, imul pL --
    right-first), leaf in eax, frame deferred past all loads, then add eax,ecx. Measured
    byte-exact.  Combine is '+' only (subtraction-combine unprobed -> declined by caller)."""
    seq = (_mov("ecx", "esp", _argoff(pR)) + _imul("ecx", "esp", _argoff(pL))
           + _mov("eax", "esp", _argoff(H)) + _FRAME + _add_rr("eax", "ecx"))
    return bytes([0x55]) + seq + _epi(nargs)


def emit(node):
    """Forward model: construct -> exact MSVC-2.0 /O1 /Oy- bytes, or None if out of class."""
    try:
        nargs, E = node["nargs"], node["expr"]
    except (KeyError, TypeError):
        return None
    kind = E[0]

    # --- pure constant: no frame -------------------------------------------
    if kind == "const":
        v = E[1]
        body = bytes([0x33, 0xC0]) if v == 0 else (bytes([0xB8]) + _imm32(v))
        ret = bytes([0xC3]) if nargs == 0 else bytes([0xC2]) + struct.pack("<H", 4 * nargs)
        return body + ret

    # --- everything else uses an arg -> /Oy- frame -------------------------
    setframe = bytes([0x8B, 0xEC])                       # mov ebp, esp
    epi = bytes([0x5D]) + bytes([0xC2]) + struct.pack("<H", 4 * nargs)   # pop ebp; ret 4n
    pro = bytes([0x55])                                  # push ebp

    if kind == "arg":
        seq = _load_eax(E[1]) + setframe                # load; mov ebp,esp
        return pro + seq + epi

    # --- pointer load: *p  ->  mov eax,[esp+p]; mov ebp,esp; mov eax,[eax] ------
    if kind == "deref":
        if E[1][0] != "arg":
            return None
        return pro + _load_eax(E[1][1]) + setframe + bytes([0x8B, 0x00]) + epi

    # --- index load: p[k] (const) or p[i] (arg, scaled) ------------------------
    if kind == "index":
        p, idx = E[1], E[2]
        if p[0] != "arg":
            return None
        if idx[0] == "arg":          # p[i]: mov ecx,[i]; frame; mov eax,[p]; pop; mov eax,[eax+ecx*4]
            seq = (_mov("ecx", "esp", _argoff(idx[1])) + setframe + _load_eax(p[1])
                   + bytes([0x5D, 0x8B, 0x04, 0x88]))
            return pro + seq + bytes([0xC2]) + struct.pack("<H", 4 * nargs)
        if idx[0] == "const":
            d = idx[1] * 4
            if d == 0:
                mem = bytes([0x8B, 0x00])                # mov eax,[eax]
            elif 0 < d < 0x80:
                mem = bytes([0x8B, 0x40, d])             # mov eax,[eax+disp8]
            else:
                return None
            return pro + _load_eax(p[1]) + setframe + mem + epi
        return None

    # --- call an extern:  g(args)  ---------------------------------------------
    # value = the call's eax result; reloc bytes (rel32 target) are emitted as 0 and MASKED.
    if kind == "call":
        conv, cargs = E[1], E[2]
        if not all(a[0] == "arg" for a in cargs):
            return None
        if not cargs:                                   # return g(): tail-call jmp rel32 (no frame)
            return bytes([0xE9, 0, 0, 0, 0])
        seq = bytes([0x55, 0x8B, 0xEC])                 # push ebp; mov ebp,esp
        for i, a in enumerate(reversed(cargs)):         # push args right-to-left; first esp, rest ebp
            seq += _push_mem("esp" if i == 0 else "ebp", _argoff(a[1]))
        seq += bytes([0xE8, 0, 0, 0, 0])                # call rel32 (reloc)
        if conv == "__cdecl":
            seq += bytes([0x8B, 0xE5])                  # mov esp,ebp  (caller cleans cdecl args)
        seq += _epi(nargs)
        return seq

    # --- store *p = value  (void) ---------------------------------------------
    if kind == "store":
        p, val = E[1], E[2]
        if p[0] != "arg":
            return None
        if val[0] == "arg":            # mov ecx,[esp+v]; frame; mov eax,[esp+p]; pop; mov [eax],ecx
            seq = (_mov("ecx", "esp", _argoff(val[1])) + setframe
                   + _load_eax(p[1]) + bytes([0x5D, 0x89, 0x08]))
            return pro + seq + bytes([0xC2]) + struct.pack("<H", 4 * nargs)
        if val[0] == "const":          # mov eax,[esp+p]; frame; mov dword[eax],imm; pop; ret
            return pro + _load_eax(p[1]) + setframe + bytes([0xC7, 0x00]) + struct.pack("<i", val[1]) + epi
        return None

    # --- comparison returning 0/1 ---------------------------------------------
    if kind == "cmp":
        op, L, R = E[1], E[2], E[3]
        if L[0] != "arg" or R[0] != "arg":
            return None
        if op in ("<", "<=", ">", ">="):                # branch idiom: cmp [R],eax(=L); mov 1; jcc +2; xor
            JCC = {"<": 0x7F, "<=": 0x7D, ">": 0x7C, ">=": 0x7E}   # inverse-relation on (R-L)
            seq = (_load_eax(L[1]) + setframe
                   + bytes([0x39, 0x44, 0x24, _argoff(R[1])])
                   + bytes([0xB8, 1, 0, 0, 0, JCC[op], 0x02, 0x33, 0xC0]))
            return pro + seq + epi
        if op in ("==", "!="):                          # ecx=R-L; pop; cmp ecx,1; sbb eax,eax; neg/inc
            seq = (_mov("ecx", "esp", _argoff(R[1])) + setframe
                   + _alu("-", "ecx", "esp", _argoff(L[1]))
                   + bytes([0x5D, 0x83, 0xF9, 0x01, 0x1B, 0xC0])
                   + (bytes([0xF7, 0xD8]) if op == "==" else bytes([0x40])))
            return pro + seq + bytes([0xC2]) + struct.pack("<H", 4 * nargs)
        return None

    if kind == "binop":
        op, L, R = E[1], E[2], E[3]
        if op not in OPS:
            return None
        # multiplicative chain (same-op '*' over >=3 arg leaves) -- single-accumulator, clean.
        chain = _flatten_chain(E)
        if chain and chain[0] == "*" and len(chain[1]) >= 3:
            return _emit_mul_chain(nargs, chain[1])
        if chain and chain[0] in ("&", "|", "^") and len(chain[1]) >= 3:   # bitwise chain
            return _emit_bitchain(chain[0], nargs, chain[1])
        # TWO-REGISTER additive / mixed-precedence (the modeled clean-schedule shapes):
        if op in ("+", "-"):
            fa = _flatten_addsub(E)                       # pure +/- chain over arg leaves?
            if fa and len(fa[1]) == 2 and any(o == "+" for o, _ in fa[1]):
                return _emit_add3(nargs, fa[0], fa[1])    # a+b+c / a+b-c / a-b+c
            if fa and len(fa[1]) >= 2 and all(o == "-" for o, _ in fa[1]):
                return _emit_sub_chain(nargs, fa[0], [k for _, k in fa[1]])   # a-b-c[-d..]
            if fa and len(fa[1]) >= 3 and all(o == "+" for o, _ in fa[1]):    # a+b+c+d[+..] (n>=4)
                return _emit_add_chain(nargs, [fa[0]] + [k for _, k in fa[1]])
            if op == "+":                                 # product + leaf (mixed precedence)
                pl, pr = _as_arg_product(L), _as_arg_product(R)
                if pl and R[0] == "arg":
                    return _emit_prod_leaf(nargs, pl[0], pl[1], R[1])
                if pr and L[0] == "arg":
                    return _emit_prod_leaf(nargs, pr[0], pr[1], L[1])
                if pl and pr:                             # two products: (a*b)+(c*d) -> lea combine
                    seq = (_mov("edx", "esp", _argoff(pr[1])) + _imul("edx", "esp", _argoff(pr[0]))
                           + _mov("ecx", "esp", _argoff(pl[1])) + setframe
                           + _imul("ecx", "ebp", _argoff(pl[0]))
                           + bytes([0x5D, 0x8D, 0x04, 0x0A]))   # pop; lea eax,[edx+ecx]
                    return pro + seq + bytes([0xC2]) + struct.pack("<H", 4 * nargs)
        # two products (a*b) - (c*d): eax=a*b, edx=c*d, sub eax,edx (no lea, order matters)
        if op == "-" and _as_arg_product(L) and _as_arg_product(R):
            a, b = _as_arg_product(L)
            c, d = _as_arg_product(R)
            seq = (_mov("eax", "esp", _argoff(b)) + _imul("eax", "esp", _argoff(a))
                   + _mov("edx", "esp", _argoff(d)) + setframe + _imul("edx", "ebp", _argoff(c))
                   + bytes([0x2B, 0xC2]))               # sub eax, edx
            return pro + seq + epi
        # left-fold: (a <io> b) <op2> c  -- inner sum/diff in eax (esp), then op2 c (ebp)
        if (L[0] == "binop" and L[1] in ("+", "-") and L[2][0] == "arg" and L[3][0] == "arg"
                and R[0] == "arg" and op not in ("+", "-")):
            inner = _alu(L[1], "eax", "esp", _argoff(L[3][1]))
            if op == "*":
                outer = _imul("eax", "ebp", _argoff(R[1]))
            elif op in _ALU:
                outer = _alu(op, "eax", "ebp", _argoff(R[1]))
            else:
                return None
            return pro + _load_eax(L[2][1]) + setframe + inner + outer + epi
        # arg <op> const
        if L[0] == "arg" and R[0] == "const":
            k, c = L[1], R[1]
            opb = _op_const(op, c)
            if opb is None or not (-128 <= c <= 127):
                return None                              # imm8-range only, this milestone
            if op == "*":                                # imul defers mov ebp,esp past the op
                return pro + _load_eax(k) + opb + setframe + epi
            return pro + _load_eax(k) + setframe + opb + epi
        # arg <op> arg
        if L[0] == "arg" and R[0] == "arg":
            load, other, enc = _binargs_plan(op, L[1], R[1])
            opb = enc(_argoff(other))
            if op == "*":
                return pro + _load_eax(load) + opb + setframe + epi
            return pro + _load_eax(load) + setframe + opb + epi
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


def to_c(node):
    nargs, E = node["nargs"], node["expr"]
    ptr = set()
    _ptr_args(E, ptr)
    params = "void" if nargs == 0 else ", ".join(
        "int%s arg%d" % ("*" if (i + 1) in ptr else "", i + 1) for i in range(nargs))
    if E[0] == "store":                              # void: *argP = value;
        return "void __stdcall f(%s) { *arg%d = %s; }" % (params, E[1][1], _c_expr(E[2]))
    if E[0] == "call":                               # extern g(...); return g(args);
        conv, cargs = E[1], E[2]
        gsig = ", ".join("int" for _ in cargs) or "void"
        callargs = ", ".join("arg%d" % a[1] for a in cargs)
        return ("extern int %s g(%s); int __stdcall f(%s) { return g(%s); }"
                % (conv, gsig, params, callargs))
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
