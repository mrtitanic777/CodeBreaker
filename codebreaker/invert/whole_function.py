r"""whole_function.py -- whole-function template synthesis (deterministic C code generation).

The reconstruction is INVERSE code generation: machine code -> C that recompiles to the
same bytes. This module is the "template-based generation" layer (cf. Jinja/M4 for normal
codegen): a library of parameterized C templates, each with a structural MATCHER over the
disassembly and an EMITTER that fills the holes. A match yields candidate C; the oracle
(refine/mocklink) proves it byte-exact -- so unlike an LLM guess, a template either
recompiles to the exact bytes or it is rejected.

This is the fast-path for common exact shapes and the vehicle for CROSS-DLL reuse: a
template proven on one DLL auto-reconstructs the same shape everywhere (verified: the
lzexpand thunk template reconstructs suexpand.dll's thunk byte-exact). It complements the
instruction-level inverter (msc6dec): templates catch whole-function shapes instantly,
msc6dec inverts the arbitrary bodies statement-by-statement.

To grow the database: add a Template. Every hand-reconstructed function whose SHAPE recurs
should become one here, so it is never re-derived on another DLL.

    from codebreaker.invert import whole_function as templates
    c = templates.synthesize(func_bytes)     # -> candidate C str, or None
"""
from capstone import x86
from codebreaker import platforms

_MD = platforms.decoder(16, detail=True)
# the Win16 exported far-function loadds prologue
_PROLOGUE = ["push ds", "pop ax", "nop", "inc bp", "push bp", "mov bp, sp",
             "push ds", "mov ds, ax"]


def _skip_prologue(ins):
    """Index just past the loadds prologue (+ optional `sub sp,N`), or None."""
    i = 0
    for p in _PROLOGUE:
        if i >= len(ins) or (ins[i].mnemonic + " " + ins[i].op_str).strip() != p:
            return None
        i += 1
    if i < len(ins) and ins[i].mnemonic == "sub":
        i += 1
    return i


# ---- Template: pure forwarding thunk -------------------------------------------
# prologue; pushes (imm zeros and/or [bp+off] params); push cs; call <sibling>;
# [add sp,N]; straight to epilogue. Forwards its args (+ padding) to one sibling.
def _match_thunk(ins):
    i = _skip_prologue(ins)
    if i is None:
        return None
    pushes = []
    while i < len(ins) and ins[i].mnemonic == "push":
        op = ins[i].operands[0]
        if op.type == x86.X86_OP_REG:                # push cs -> end of arg pushes
            break
        if op.type == x86.X86_OP_MEM and op.mem.base != 0:
            pushes.append(("bp", op.mem.disp))
        elif op.type == x86.X86_OP_IMM:
            pushes.append(("imm", op.imm))
        else:
            return None
        i += 1
    if not (i + 1 < len(ins) and ins[i].operands and
            ins[i].operands[0].type == x86.X86_OP_REG and ins[i + 1].mnemonic == "call"):
        return None
    i += 2
    if i < len(ins) and ins[i].mnemonic == "add":    # cdecl call cleanup
        i += 1
    for j in range(i, len(ins)):                     # tail must be epilogue only
        m = ins[j].mnemonic
        if m in ("call", "les", "lea") or (m == "mov" and "sp" not in ins[j].op_str):
            return None
    if not any(p[0] == "bp" for p in pushes):
        return None
    ret = ins[-1]
    return {"pushes": pushes, "conv": "pascal" if (ret.mnemonic == "retf" and ret.op_str) else "cdecl"}


def _emit_thunk(m, rev=False):
    offs = [p[1] for p in m["pushes"] if p[0] == "bp"]
    nz = sum(1 for p in m["pushes"] if p[0] == "imm" and p[1] == 0)
    uniq = sorted(set(offs))
    conv = "pascal " if m["conv"] == "pascal" else ""
    params = ", ".join("int p%x" % o for o in uniq) or "void"
    fwd = ", ".join(["0"] * nz + ["p%x" % o for o in (offs[::-1] if rev else offs)])
    return "extern int far SIB();\nint far %sF(%s){ return SIB(%s); }" % (conv, params, fwd)


# a Template is (name, matcher, [emitter variants]). Emitters return candidate C.
TEMPLATES = [
    ("thunk", _match_thunk, [lambda m: _emit_thunk(m, False), lambda m: _emit_thunk(m, True)]),
]


def candidates(func_bytes):
    """Yield candidate C strings for `func_bytes` from every matching template."""
    ins = list(_MD.disasm(bytes(func_bytes), 0))
    if len(ins) < 6:
        return
    for name, match, emitters in TEMPLATES:
        m = match(ins)
        if m is not None:
            for emit in emitters:
                yield name, emit(m)


def synthesize(func_bytes, verify=None):
    """Return the first candidate C that `verify(func_bytes, c)` accepts (returns exact),
    or -- if no verifier -- the first candidate produced. `verify` is refine.check or
    mocklink.verify style: callable(bytes, c) -> {'exact': bool}."""
    for name, c in candidates(func_bytes):
        if verify is None:
            return c
        r = verify(bytes(func_bytes), c)
        if r.get("exact"):
            return c
    return None
