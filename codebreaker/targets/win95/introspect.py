#!/usr/bin/env python3
r"""introspect.py -- the unified function INTROSPECTION service (win95 32-bit PE).

One structured descriptor per function, consolidating the evidence the Layer-2
pipeline reads in scattered places (classify32's features/classify, emit_c &
emit_cpp's shape detectors, matchsearch's convention/signedness readers) behind a
single call:

    d = introspect.describe("kernel32.dll", 0x1b40)

This is the SUBSTRATE the higher systems compose on:
  * the signature factory manufactures a typed signature FROM a descriptor,
  * reflected delegation routes a function to an engine BY its descriptor,
  * the derived execution tree keys its nodes on it.

Doctrine: evidence only. Every field is read from the bytes or DECLINED
(None / 'unknown') -- never guessed. A descriptor states what the bytes prove and
marks what they don't.

    python introspect.py MODULE.dll[:RVA]     # one descriptor, or a shape histogram
"""
import os
import re
import sys
from dataclasses import dataclass, field, asdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import pe32util as H
import classify32

_ARG_EBP = re.compile(r"\[ebp \+ (0x[0-9a-f]+|\d+)\]")
_ARG_ESP = re.compile(r"\[esp(?: \+ (0x[0-9a-f]+|\d+))?\]")

# instruction signals that a value is treated as SIGNED vs UNSIGNED -- the only
# semantics (besides convention) that /O1 lets leak into the bytes.
_SIGNED = {"idiv", "imul", "sar", "movsx", "cdq", "cwde", "jl", "jle", "jg", "jge"}
_UNSIGNED = {"div", "shr", "movzx", "ja", "jae", "jb", "jbe"}


@dataclass
class FnDescriptor:
    module: str
    rva: int
    name: str | None
    size: int
    ninsn: int
    route: str                       # classify32 route: thunk|asm|c|human
    cat: str                         # classify32 category
    retbytes: int                    # bytes cleaned by a `ret N` (0 if plain ret / tail)
    convention: str                  # inferred outer: stdcall|cdecl|thiscall|stdcall0|unknown
    argc: int                        # explicit stack args (this NOT counted); best evidence
    argc_exact: bool                 # True when ret N pins it; False when inferred from slots
    thiscall: bool                   # ecx used as `this` (read before written)
    reads_caller_frame: bool         # [ebp+X] with no own prologue -> hand asm
    eax_convention: bool             # arg/index passed in eax (custom thunk)
    framed: bool                     # push ebp; mov ebp,esp prologue
    has_seh: bool
    signedness: str                  # signed|unsigned|mixed|unknown
    ncall: int
    calls: list = field(default_factory=list)   # [(kind, target_rva_or_iat, name)]
    shape: str = "unknown"           # refined single-word shape (see _shape)

    def to_dict(self):
        return asdict(self)


def _framed(ins):
    return len(ins) >= 2 and ins[0].mnemonic == "push" and ins[0].op_str == "ebp" \
        and ins[1].mnemonic == "mov" and ins[1].op_str == "ebp, esp"


def _thiscall(ins):
    """ecx used as `this`: read (as [ecx+..] or a source) before being written."""
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


def _writes_ebp(ins):
    """True if the function ever makes ebp its own value (frame pointer or a data
    pointer): mov/lea/add/sub/and/or/xor ebp,.. or pop ebp. If it does, an [ebp+X]
    read is through ITS ebp, not the caller's."""
    for i in ins:
        if i.mnemonic == "pop" and i.op_str == "ebp":
            return True
        if i.mnemonic in ("mov", "lea", "add", "sub", "and", "or", "xor", "movzx", "movsx") \
                and i.op_str.split(",", 1)[0].strip() == "ebp":
            return True
    return False


def _reads_caller_frame(ins, framed):
    # Reads the CALLER's frame only if it never establishes a frame AND never writes
    # ebp itself -- i.e. ebp is purely the inherited value (the SUnMapLS_IP_EBP_* idiom).
    return (not framed) and (not _writes_ebp(ins)) and any("[ebp" in i.op_str for i in ins)


def _sets_eax_from_imm_or_arg(i):
    """eax/ax/al loaded from an IMMEDIATE or a STACK slot (an index/arg to hand a
    callee in eax) -- NOT from a register deref like [ecx+k] (that's a field load in
    a normal method, not an eax-convention thunk)."""
    if i.mnemonic not in ("mov", "movzx", "movsx", "lea"):
        return False
    dst, _, src = i.op_str.partition(", ")
    if dst not in ("eax", "ax", "al"):
        return False
    if any("[%s" % r in src for r in ("ecx", "eax", "edx", "esi", "edi", "ebx")):
        return False                                      # register deref -> field/pointer load
    return bool(re.match(r"-?(0x[0-9a-f]+|\d+)$", src)) or "[esp" in src or "[ebp" in src


def _eax_convention(ins, ncall, heavy, thiscall):
    """arg/index handed to a callee in eax (no stack push) -- a custom thunk. Never a
    __thiscall method (those pass this in ecx and legitimately touch eax)."""
    if ncall < 1 or heavy or thiscall:
        return False
    sets_eax = any(_sets_eax_from_imm_or_arg(i) for i in ins)
    arg_push = any(i.mnemonic == "push" and i.op_str not in ("ebp", "esi", "edi", "ebx") for i in ins)
    return sets_eax and not arg_push


def _signedness(mn):
    s = any(m in _SIGNED for m in mn)
    u = any(m in _UNSIGNED for m in mn)
    return "mixed" if (s and u) else "signed" if s else "unsigned" if u else "unknown"


def _infer_argc(ins, retbytes, framed, thiscall):
    """Explicit stack-arg count. `ret N` pins it exactly (N/4). Otherwise (a plain
    ret / a tail jmp, i.e. __cdecl or a forward) infer from the highest arg slot
    referenced: framed -> [ebp+8+4k]; frameless -> [esp+4+4k] as a floor estimate."""
    if retbytes > 0:
        return retbytes // 4, True
    hi = -1
    for i in ins:
        if framed:
            m = _ARG_EBP.search(i.op_str)
            if m:
                off = int(m.group(1), 0)
                if off >= 8 and off % 4 == 0:
                    hi = max(hi, (off - 8) // 4)
        else:
            m = _ARG_ESP.search(i.op_str)
            if m:
                off = int(m.group(1), 0) if m.group(1) else 0
                if off >= 4 and off % 4 == 0:
                    hi = max(hi, (off - 4) // 4)
    return (hi + 1), False


def _convention(retbytes, argc, thiscall):
    if thiscall:
        return "thiscall"
    if retbytes > 0:
        return "stdcall"
    if argc > 0:
        return "cdecl"                # plain ret + args: stdcall-with-args would use ret N
    return "stdcall0"                 # 0-arg: stdcall/cdecl ABI-identical


def _shape(cat, route, thiscall, ncall, reads_caller_frame, eax_conv):
    if reads_caller_frame:
        return "caller_frame_asm"
    if eax_conv:
        return "eax_thunk"
    if route == "thunk":
        return "thunk"
    if cat in ("const_ret",):
        return "const"
    if thiscall:
        return "method"               # __thiscall: emit_cpp territory
    if cat in ("forwarder",):
        return "forwarder"
    if cat in ("wrapper", "wrapper2"):
        return "wrapper"
    if route == "c":
        return "leaf"
    return route                      # asm | human


def describe(module, rva):
    """Return a FnDescriptor for the function at `rva` in `module` (evidence only)."""
    c = H.carve(module); base = c.pe.image_base
    byva = {s: nm for _r, nm, s, _e in H.named_funcs(module)}
    iat = H.imports(module)
    fn = c.funcs.get(base + rva)
    end = fn["end"] if fn else base + rva + 1
    name = fn.get("name") if fn else None
    f = classify32.features(module, base + rva, end, byva, iat, base)
    cat, route = classify32.classify(f)
    ins, mn = f["ins"], f["mn"]
    framed = _framed(ins)
    thiscall = _thiscall(ins)
    ncall = sum(1 for m in mn if m in ("call", "jmp"))
    heavy = [m for m in mn if m not in ("push", "pop", "mov", "call", "jmp", "ret", "retn",
                                        "nop", "leave", "lea", "inc", "dec", "add", "sub", "xor",
                                        "test", "cmp", "sbb", "neg")]
    argc, exact = _infer_argc(ins, f["retbytes"], framed, thiscall)
    rcf = _reads_caller_frame(ins, framed)
    eax_conv = _eax_convention(ins, ncall, heavy, thiscall)
    return FnDescriptor(
        module=module, rva=rva, name=name, size=f["n"], ninsn=f["ni"],
        route=route, cat=cat, retbytes=f["retbytes"],
        convention=_convention(f["retbytes"], argc, thiscall),
        argc=max(argc, 0), argc_exact=exact,
        thiscall=thiscall, reads_caller_frame=rcf, eax_convention=eax_conv,
        framed=framed, has_seh=f["has_seh"], signedness=_signedness(mn),
        ncall=ncall, calls=list(f["calls"]),
        shape=_shape(cat, route, thiscall, ncall, rcf, eax_conv))


def describe_module(module):
    """Descriptors for every named function of a module (rva-sorted)."""
    H.ensure_target()
    c = H.carve(module); base = c.pe.image_base
    out = []
    for s, fn in sorted(c.funcs.items()):
        if not fn.get("name"):
            continue
        out.append(describe(module, s - base))
    return out


def main():
    H.ensure_target()
    arg = sys.argv[1] if len(sys.argv) > 1 else "kernel32.dll"
    if ":" in arg:
        mod, rva = arg.split(":"); d = describe(mod, int(rva, 0))
        for k, v in d.to_dict().items():
            print("  %-18s %s" % (k, v))
        return
    import collections
    ds = describe_module(arg)
    print("=== %s : %d named functions ===" % (arg, len(ds)))
    print("  shapes    :", dict(collections.Counter(d.shape for d in ds).most_common()))
    print("  conventions:", dict(collections.Counter(d.convention for d in ds).most_common()))
    print("  signedness:", dict(collections.Counter(d.signedness for d in ds).most_common()))


if __name__ == "__main__":
    main()
