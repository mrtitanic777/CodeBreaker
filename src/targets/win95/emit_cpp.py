#!/usr/bin/env python3
r"""emit_cpp.py -- Layer-2 pipeline: auto-generate clean C++ for the mechanical
__thiscall METHOD classes and confirm byte-exact with the cc32 MSVC 2.0 oracle
(compiled as C++, so member functions get __thiscall: `this` in ecx).

The C sibling emit_c models __cdecl/__stdcall/__fastcall free functions and, by
design, BAILS whenever a register (ecx/edx) is live into a call or read as a base
without first being loaded from a stack slot -- i.e. exactly the C++ method case
where ecx is the incoming `this` pointer. In MFC modules (mfc40: ~2,800 such
functions, ALL previously falling to the ASM floor) that is where the coverage
lives, and none of it is a demangling problem: MFC exports by ordinal, so the
methods are unnamed internal `sub_<rva>` with `this` in ecx, not mangled exports.

emit_cpp closes the tractable slice of that gap:

  * getter   `mov eax,[ecx+k]` / `movzx/movsx` (byte/word/dword field)  -> return this->field
  * address  `lea eax,[ecx+k]`                                          -> return &field
  * this     `mov eax,ecx`                                              -> return this
  * setter   `[ecx+k] = imm` or `[ecx+k] = arg`                         -> this->field = v

These are the leaf method classes with a canonical, single-shape codegen. The
call-bearing classes (forwarders, virtual dispatch, multi-call) were measured
against the corpus and do NOT reduce to a byte-exact template: MFC methods
overwhelmingly do real work (frame + `this` saved in a callee-saved reg + args
computed from fields), so they need full body/expression reconstruction, not a
fixed template -- out of scope here, left to the ASM floor / behavior engine.

Each method is emitted with `this`-relative pointer arithmetic
(`*(T*)((char*)this + k)`), which MSVC 2.0 /O1 compiles to `[ecx+k]` for ANY
offset with no struct-layout/packing concern. Callee-cleanup `ret N` fixes the
stack-arg count (N/4; `this` rides in ecx and is not counted). The emitted method
is read back by its sole code symbol (mangling is irrelevant -- calls/relocs are
masked at verify time, exactly as in emit_c). Whatever the oracle confirms
byte-exact is done; the rest stays on the ASM floor. A modeler that cannot
represent a shape returns None, so a wrong body is never emitted.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))  # target dir on path for -m / bare imports

import pe32util as H
import cc32
from src.engines.recon import segdiff

# a lone empty struct + the out-of-line method definition; `this` arithmetic keeps
# us free of field-offset alignment/packing entirely.
_PRE = "struct %s { %s };\n"
_FIELD = re.compile(r"\[ecx(?: \+ (0x[0-9a-f]+|\d+))?\]$")
_ESP = re.compile(r"(?:dword ptr )?\[esp(?: \+ (0x[0-9a-f]+|\d+))?\]$")


def _body(ins):
    return [i for i in ins if i.mnemonic not in ("nop", "int3")]


def _is_thiscall(ins):
    """True when ecx is used as `this` -- READ (as a base [ecx+..] or a source)
    before being written. A method whose first ecx touch is a write is not using
    an incoming this; bail so we never model a non-thiscall function as one."""
    for i in ins:
        o = i.op_str
        if "ecx" not in o:
            continue
        if "[ecx" in o:
            return True
        dst = o.split(",", 1)[0].strip()
        is_w = i.mnemonic in ("mov", "xor", "lea", "movzx", "movsx", "pop") and dst == "ecx"
        return not is_w
    return False


def _foff(operand):
    """Offset k from an `[ecx + k]` operand (0 if bare [ecx])."""
    m = _FIELD.search(operand)
    if not m:
        return None
    return int(m.group(1), 0) if m.group(1) else 0


def _deref(ctype, k):
    return "*(%s *)((char *)this + %d)" % (ctype, k) if k else "*(%s *)this" % ctype


def _getter(body):
    """Single field load returned in eax: `mov eax,[ecx+k]` (int),
    `movzx/movsx eax, byte/word ptr [ecx+k]` (u/char, u/short), or
    `lea eax,[ecx+k]` (field address). Returns (rettype, expr) or None."""
    if len(body) != 2 or body[1].mnemonic not in ("ret", "retn"):
        return None
    i0 = body[0]
    m, o = i0.mnemonic, i0.op_str
    if m == "mov" and o == "eax, ecx":                             # mov eax,ecx; ret -> return this
        return "void *", "return this;"
    if not o.startswith("eax, "):
        return None
    src = o.split(", ", 1)[1]
    k = _foff(src)
    if k is None:
        return None
    if m == "mov" and "ptr" not in src.replace("dword ptr", ""):   # plain dword field
        return "int", "return %s;" % _deref("int", k)
    if m == "movzx" and src.startswith("byte ptr"):
        return "int", "return %s;" % _deref("unsigned char", k)
    if m == "movzx" and src.startswith("word ptr"):
        return "int", "return %s;" % _deref("unsigned short", k)
    if m == "movsx" and src.startswith("byte ptr"):
        return "int", "return %s;" % _deref("signed char", k)
    if m == "movsx" and src.startswith("word ptr"):
        return "int", "return %s;" % _deref("short", k)
    if m == "lea":                                                 # lea eax,[ecx+k] -> &field
        return "void *", "return (char *)this + %d;" % k
    return None


def _argidx(src):
    """Stack-arg index for a `[esp + X]` operand at method entry: [esp+4]=a0.
    __thiscall passes `this` in ecx, so the first stack slot is the first
    explicit argument. Returns index or None."""
    m = _ESP.search(src)
    if not m:
        return None
    x = int(m.group(1), 0) if m.group(1) else 0
    off = x - 4
    return off // 4 if off >= 0 and off % 4 == 0 else None


def _setter(body):
    """Straight-line field store(s), each value a constant or an incoming stack
    arg: `mov [ecx+k], imm`, or `mov reg,[esp+X]` then `mov [ecx+k], reg`.
    Returns (stmts, maxarg) or None. Bails on any calls, control flow, or a value
    that isn't a literal/argument, so a wrong body is never emitted."""
    regarg = {}                                           # reg -> a<idx> (loaded from a stack arg)
    stmts = []
    maxa = -1
    for i in body:
        m, o = i.mnemonic, i.op_str
        if m in ("ret", "retn"):
            break
        if m != "mov":
            return None
        dst, _, src = o.partition(", ")
        km = _foff(dst)
        if km is not None:                                # a store to [ecx+k]
            w = ("char" if dst.startswith("byte ptr") else       # store WIDTH from the ptr prefix:
                 "short" if dst.startswith("word ptr") else "int")  # byte/word/dword field
            if re.match(r"-?(0x[0-9a-f]+|\d+)$", src):
                v = int(src, 0); v = v - 0x100000000 if v > 0x7fffffff else v
                stmts.append("%s = %d;" % (_deref(w, km), v))
            elif src in regarg and w == "int":            # arg store: dword only (byte/word-arg
                stmts.append("%s = %s;" % (_deref(w, km), regarg[src]))   # marshalling is v2)
                maxa = max(maxa, int(regarg[src][1:]))
            else:
                return None
        else:                                             # a reg load: only reg <- [esp+arg]
            if "[ecx" in src or "[" in dst:
                return None
            ai = _argidx(src)
            if ai is None or dst not in ("eax", "ecx", "edx", "esi", "edi", "ebx"):
                return None
            regarg[dst] = "a%d" % ai
    if not stmts:
        return None
    return stmts, maxa


def _args(n):
    return "" if n == 0 else ", ".join("int a%d" % i for i in range(n))


def gen(module, row):
    """Return [(cpp_src, structtag)] candidates for a __thiscall method, or []. The
    method takes retbytes/4 explicit args (callee-cleanup; `this` rides in ecx)."""
    c = H.carve(module); base = c.pe.image_base
    s = base + row["rva"]
    ins = H.disasm(H.func_bytes(c, s, s + row["size"]), s)
    if not ins or not _is_thiscall(ins):
        return []
    body = _body(ins)
    argc = (row["retbytes"] // 4)
    tag = "C_%05x" % row["rva"]                           # unique per function -> unique mangling

    def wrap(rettype, defn):
        decl = "%s m(%s);" % (rettype, _args(argc))
        return _PRE % (tag, decl) + "%s %s::m(%s){ %s }\n" % (rettype, tag, _args(argc), defn), tag

    g = _getter(body)
    if g is not None:
        rettype, stmt = g
        return [wrap(rettype, stmt)]
    st = _setter(body)
    if st is not None:
        stmts, maxa = st
        if maxa + 1 > argc:                               # a referenced arg beyond ret N -> inconsistent
            return []
        return [wrap("void", " ".join(stmts))]
    return []


def _one_func(objfuncs, tag):
    """The compiled method's (bytes, mask): the sole code symbol carrying this
    struct tag in its mangled name (`?m@C_xxxxx@@...`)."""
    key = "@%s@@" % tag
    for sym, cm in objfuncs.items():
        if key in sym:
            return cm
    return None


def emit_module(module, rows):
    """Batch clean-C++ for the __thiscall-method candidates among `rows` in
    chunked cl invocations. Returns {rva: (status, cppsrc, sym, flags, note)} with
    status in byte-exact|draft|None -- the same contract as emit_c.emit_module, so
    l2pipe consumes both uniformly."""
    prim = {}                                             # rva -> (cppsrc, tag)
    rowmap = {r["rva"]: r for r in rows}
    for r in rows:
        try:
            cs = gen(module, r)
        except Exception:
            cs = []
        if cs:
            prim[r["rva"]] = cs[0]
    res = {r["rva"]: (None, None, None, None, None) for r in rows}
    if not prim:
        return res
    rvas = list(prim.keys())
    CHUNK = 200
    for c0 in range(0, len(rvas), CHUNK):
        chunk = rvas[c0:c0 + CHUNK]
        tu = "".join(prim[rva][0] for rva in chunk)       # each struct tag is unique -> no clashes
        try:
            r = cc32.compile_c(tu, flags="/c /O1", cpp=True)
            funcs = cc32.Obj(r["obj"]).functions() if "obj" in r else None
        except Exception:
            funcs = None
        if funcs is None:                                 # chunk won't compile -> per-fn (bounded)
            for rva in chunk:
                res[rva] = emit_best(module, rowmap[rva])
            continue
        for rva in chunk:
            cppsrc, tag = prim[rva]
            cm = _one_func(funcs, tag)
            if cm is None:
                continue
            ob, om = cm
            sb, sm = cc32.shipped_pe_function(module, rva)
            if segdiff.diff(ob, om, sb, sm)["exact"]:
                res[rva] = ("byte-exact", cppsrc, tag, "/c /O1 (c++)", None)
            else:
                res[rva] = ("draft", cppsrc, tag, "/c /O1 (c++)",
                            "compiles as a __thiscall method; codegen differs (review)")
    return res


def emit_best(module, row):
    """Best clean-C++ result for one function (per-function compile), same 5-tuple
    contract as emit_c.emit_best. Used as the bounded fallback when a batch chunk
    fails to compile."""
    try:
        cands = gen(module, row)
    except Exception:
        cands = []
    draft = None
    sb = sm = None
    for cppsrc, tag in cands:
        try:
            r = cc32.compile_c(cppsrc, flags="/c /O1", cpp=True)
            if "obj" not in r:
                continue
            cm = _one_func(cc32.Obj(r["obj"]).functions(), tag)
            if cm is None:
                continue
            ob, om = cm
            if sb is None:
                sb, sm = cc32.shipped_pe_function(module, row["rva"])
            if segdiff.diff(ob, om, sb, sm)["exact"]:
                return "byte-exact", cppsrc, tag, "/c /O1 (c++)", None
            if draft is None:
                draft = (cppsrc, tag)
        except Exception:
            continue
    if draft:
        return "draft", draft[0], draft[1], "/c /O1 (c++)", \
               "compiles as a __thiscall method; codegen differs (review)"
    return None, None, None, None, None


def sweep(module, limit=None):
    """Standalone measurement: classify the module, run emit_cpp over every
    __thiscall-shaped function, report the byte-exact clean-C++ yield."""
    import classify32
    H.ensure_target()
    rows = classify32.classify_module(module)
    if limit:
        rows = rows[:limit]
    res = emit_module(module, rows)
    nx = sum(1 for v in res.values() if v[0] == "byte-exact")
    nd = sum(1 for v in res.values() if v[0] == "draft")
    print("=== %s : %d functions ===" % (module, len(rows)))
    print("  clean C++ (byte-exact) : %d" % nx)
    print("  drafts (review)        : %d" % nd)
    return res


def main():
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    ap.add_argument("--limit", type=int)
    a = ap.parse_args()
    sweep(a.module, a.limit)


if __name__ == "__main__":
    main()
