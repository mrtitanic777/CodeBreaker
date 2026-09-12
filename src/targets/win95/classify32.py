#!/usr/bin/env python3
r"""classify32.py -- Layer-2 pipeline stage 1: the unified function-type router.

For every carved function in a 32-bit PE module, disassemble once, extract
features (call/jmp targets, imports, SEH use, ret size, constant return), and
assign a CATEGORY + ROUTE so the emitters can process a whole module unattended:

  ROUTE 'thunk' -- 16-bit flat-thunk stubs (mov cl/cx idx ...): mechanical asm
  ROUTE 'asm'   -- hand-written asm (SEH getters, pop/push shufflers, ptr-jmp)
  ROUTE 'c'     -- was C: const-return / forwarder / import-or-internal wrapper,
                   a clean-C template the cc32 oracle can confirm byte-exact
  ROUTE 'human' -- real multi-statement logic: byte-exact asm floor + review flag

This is the single classifier the pilot did by hand and the corpus lacked (the
prior pieces -- frontier.classify_body, triage.classify, matchbatch class -- were
partial and scattered).
"""
import re
import collections

import pe32util as H


def features(module, s, e, byva, iat, base):
    b = H.func_bytes(H.carve(module), s, e)
    ins = H.disasm(b, s)
    mn = [i.mnemonic for i in ins]
    ops = [i.op_str for i in ins]
    calls = []
    for i in ins:
        if i.mnemonic in ("call", "jmp") and i.op_str.startswith("0x"):
            t = int(i.op_str, 16)
            calls.append(("export" if t in byva else "internal", t - base, byva.get(t)))
        m = re.search(r"\[0x([0-9a-f]+)\]", i.op_str)
        if m and i.mnemonic in ("call", "jmp") and int(m.group(1), 16) in iat:
            a = int(m.group(1), 16)
            calls.append(("import", a, iat[a][1]))
    retn = next((int(i.op_str, 0) for i in ins if i.mnemonic in ("ret", "retn") and i.op_str), 0)
    return dict(n=len(b), ni=len(ins), ins=ins, mn=mn, ops=ops, calls=calls,
                has_seh=any("fs:[" in o for o in ops), retbytes=retn, bytes=b)


def classify(f):
    n, ins, mn, ops = f["n"], f["ins"], f["mn"], f["ops"]
    if n <= 2:
        return "tiny", "asm"
    o0 = ops[0] if ops else ""
    if mn[0] == "mov" and o0.startswith("cx,"):     # mov cx,<thunk index> -- flat-thunk stub
        return "thunk_cx", "thunk"
    if mn[0] == "mov" and o0.startswith("cl,"):
        return "thunk_cl", "thunk"
    if mn[0] == "xor" and len(ins) >= 2 and ins[1].op_str.startswith("cl,"):
        return "clstub", "thunk"
    if f["has_seh"]:
        return "seh_asm", "asm"
    if mn[0] == "pop":          # leading pop = return-addr/arg shuffle, then tail-forward
        return "handasm_shuffle", "asm"
    if mn[0] == "jmp" and "[0x" in o0:
        return "ptr_jmp", "asm"
    # edx-dispatch: load a code/data pointer into edx, then jmp (or fall into) a shared body
    if mn[0] == "mov" and o0.startswith("edx, 0x") and len(ins) <= 4:
        return "edx_dispatch", "asm"
    # call helper ; jmp worker -- in-place argument adapter
    if len(ins) >= 2 and mn[0] == "call" and mn[1] == "jmp":
        return "call_jmp", "asm"
    # DC attribute-getter offset stub: push default ; mov dl,<field offset> ; jmp shared body
    if len(ins) >= 2 and mn[0] == "push" and mn[1] == "mov" and ins[1].op_str.startswith("dl,"):
        return "attr_stub", "asm"
    # arg-adapter: mov edx,esp then rewrite a stack arg in place, then jmp the worker
    if mn[0] == "mov" and o0 == "edx, esp":
        return "argadapter", "asm"
    # code-sharing stub: push imm ; jmp into another export's body
    if mn == ["push", "jmp"]:
        return "codeshare", "asm"
    work = [m for m in mn if m not in ("push", "pop", "mov", "ret", "retn", "nop", "leave", "xor", "int3")]
    if not f["calls"] and not work and any(m in ("mov", "xor") for m in mn) and all("[" not in o for o in ops):
        return "const_ret", "c"
    ncall = sum(1 for m in mn if m in ("call", "jmp"))
    heavy = [m for m in mn if m not in ("push", "pop", "mov", "call", "jmp", "ret", "retn",
                                        "nop", "leave", "lea", "inc", "dec", "add", "sub", "xor",
                                        "test", "cmp", "sbb", "neg")]  # sbb/neg: the BOOL-massage idiom
    # Custom-convention asm thunks that LOOK like forwarders/wrappers but can never be clean C --
    # they pass an argument in eax, or read the CALLER's frame ([ebp+X] with no own prologue). These
    # inflate the 'c' route with functions emit_c can only decline; route them to the asm floor
    # (where they are covered byte-exact anyway) so 'c' means "reconstructable-C candidate".
    framed = len(ins) >= 2 and mn[0] == "push" and o0 == "ebp" and ins[1].op_str == "ebp, esp"
    writes_ebp = any((i.mnemonic == "pop" and i.op_str == "ebp") or
                     (i.mnemonic in ("mov", "lea", "add", "sub", "and", "or", "xor", "movzx", "movsx")
                      and i.op_str.split(",", 1)[0].strip() == "ebp") for i in ins)
    if ncall >= 1 and not framed and not writes_ebp and any("[ebp" in o for o in ops):
        return "caller_frame_asm", "asm"                  # reads the CALLER's frame (inherited ebp) -> hand asm
    if ncall >= 1 and not heavy:
        def _eax_imm_or_arg(i):                           # eax/ax/al <- immediate or stack slot
            if i.mnemonic not in ("mov", "movzx", "movsx", "lea"):
                return False
            dst, _, src = i.op_str.partition(", ")
            if dst not in ("eax", "ax", "al") or any("[%s" % r in src
                    for r in ("ecx", "eax", "edx", "esi", "edi", "ebx")):
                return False                              # register deref -> field load, not a thunk
            return bool(re.match(r"-?(0x[0-9a-f]+|\d+)$", src)) or "[esp" in src or "[ebp" in src
        sets_eax = any(_eax_imm_or_arg(i) for i in ins)
        arg_push = any(i.mnemonic == "push" and i.op_str not in ("ebp", "esi", "edi", "ebx") for i in ins)
        if sets_eax and not arg_push:
            return "eax_thunk_asm", "asm"                 # argument/index passed in eax -> custom convention
    if ncall == 1 and n <= 48 and not heavy:
        return "forwarder", "c"
    if ncall <= 3 and n <= 80 and not heavy:
        return "wrapper", "c"
    if not heavy and n <= 160:
        return "wrapper2", "c"
    return "logic", "human"


def classify_module(module):
    c = H.carve(module); base = c.pe.image_base
    byva = {s: nm for _r, nm, s, _e in H.named_funcs(module)}
    iat = H.imports(module)
    out = []
    for s, fn in c.funcs.items():
        f = features(module, s, fn["end"], byva, iat, base)
        cat, route = classify(f)
        out.append(dict(rva=s - base, name=fn.get("name"), size=f["n"], cat=cat, route=route,
                        calls=f["calls"], retbytes=f["retbytes"]))
    out.sort(key=lambda r: r["rva"])
    return out


def report(module, named_only=True):
    rows = classify_module(module)
    if named_only:
        rows = [r for r in rows if r["name"]]
    byroute = collections.Counter(r["route"] for r in rows)
    bycat = collections.Counter(r["cat"] for r in rows)
    auto = byroute["thunk"] + byroute["c"] + byroute["asm"]
    print("=== %s : %d %sfunctions ===" % (module, len(rows), "named " if named_only else ""))
    print("  ROUTES:", dict(byroute))
    print("  CATS  :", dict(bycat.most_common()))
    print("  auto-routable: %d/%d (%.0f%%), escalate: %d"
          % (auto, len(rows), 100.0 * auto / max(1, len(rows)), byroute["human"]))
    return rows


if __name__ == "__main__":
    import sys
    report(sys.argv[1] if len(sys.argv) > 1 else "gdi32.dll")
