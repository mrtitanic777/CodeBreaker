#!/usr/bin/env python3
r"""recon32wrap.py -- reconstruct 32-bit validated-delegation wrappers (roadmap Phase C payoff).

The handle-gated gdi32/user32 API is overwhelmingly THIN WRAPPERS: a function loads a per-
function attribute INDEX and tail-delegates to a shared worker that validates the HDC/handle and
transitions to the GDI engine (the attribute storage + real logic live engine-side, not in ring-3
memory -- so these are NOT struct functions). The 32-bit analog of recon16's 16-bit template:

    SetBkMode:  mov cl, 0x4c ; jmp  <shared DC-attr worker>      ; index -> worker
    SetROP2:    mov cl, 0x46 ; jmp  <shared DC-attr worker>
    SetTextAlign: ... push args ; call [<engine dispatch ptr>] ; ret N   (inline form)

This recognizes the idiom from the disassembly -- the index constant, the delegate target (shared
worker tail-jump, or an engine-dispatch `call [ptr]`), and the arity (stdcall `ret N`) -- names the
function from the export table, and emits a faithful wrapper. The engine/worker itself is a
separate, deeper target.

    wt --target win95 recon32wrap <MODULE> [--limit N]
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pe32util as H


def _md():
    import capstone
    return capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def recognize(module, rva, name):
    """Return a reconstruction dict for the validated-delegation-wrapper at rva, or None."""
    pe = H.carve(module).pe
    off = pe._rva_to_off(rva)
    if off is None:
        return None
    ins = list(_md().disasm(pe.data[off:off + 96], pe.image_base + rva))
    if not ins:
        return None
    index = None            # the per-function attribute index (mov cl/dl/ecx, imm before delegate)
    delegate = None         # tail-jmp target (shared worker) or engine-dispatch pointer
    dkind = None            # 'worker'|'thunk' (tail-jmp) | 'engine' (short inline call [ptr]+ret)
    retn = None             # stdcall callee-clean byte count
    last_imm = {}
    # PRECISION GATE: a genuine wrapper is a pure delegation. Only prologue arg-setup may precede
    # the delegate (mov/xor/lea/push + a SEH frame setup). If REAL LOGIC appears first -- a
    # conditional branch, or a non-delegate call -- it's a real function, not a wrapper: reject.
    ARG_SETUP = {"mov", "xor", "lea", "push", "pop", "sub", "add", "and", "movzx", "movsx",
                 "nop", "leave", "cdq", "inc", "dec"}   # prologue arg-setup + epilogue (leave)
    saw_call = False
    for i in ins:
        m, ops = i.mnemonic, i.op_str
        if m.startswith("j") and m != "jmp":
            return None                              # conditional branch before delegate -> real logic
        if m == "mov":
            a = [o.strip() for o in ops.split(",")]
            if len(a) == 2 and (a[1].startswith("0x") or a[1].lstrip("-").isdigit()):
                try:
                    last_imm[a[0]] = int(a[1], 0)
                except ValueError:
                    pass
        if m == "jmp" and ("ptr [0x" in ops or ops.startswith("[0x")):
            try:
                delegate = int(ops.split("[")[1].split("]")[0], 0); dkind = "thunk"
            except ValueError:
                pass
            break
        elif m == "jmp" and not ops.startswith("dword") and not ops.startswith("["):
            try:
                delegate = int(ops, 0); dkind = "worker"
                index = last_imm.get("cl", last_imm.get("cx", last_imm.get("ecx")))
            except ValueError:
                pass
            break
        elif m == "call" and (ops.startswith("dword ptr [0x") or ops.startswith("[0x")):
            if saw_call:
                return None                          # a second call -> real body, not a pure wrapper
            saw_call = True
            try:
                delegate = int(ops.split("[")[1].split("]")[0], 0); dkind = "engine"
            except ValueError:
                pass
            index = last_imm.get("cl", last_imm.get("cx", last_imm.get("ecx")))
        elif m == "call":
            return None                              # a direct call to real code -> not a pure wrapper
        elif m == "ret":
            if ops.strip():
                try:
                    retn = int(ops, 0)
                except ValueError:
                    pass
            # engine inline form is valid ONLY if the ret comes right after the single call[ptr]
            if dkind == "engine":
                break
            return None                              # ret with no delegate seen -> not a wrapper
        elif m not in ARG_SETUP and m != "jmp":
            return None                              # any other real instruction before delegate -> reject
    if delegate is None:
        return None
    # arity: the stdcall `ret N` when present (inline/engine form); else the SDK prototype (the
    # worker-tail form has no own ret N -- the shared worker cleans the stack).
    argc = (retn // 4) if retn is not None else None
    if argc is None and name:
        try:
            from src.engines.symbol import symdb
            argc = symdb.arity(name)
        except Exception:
            argc = None
    argnames = ["a%d" % i for i in range(argc)] if argc else []
    params = ", ".join("unsigned " + a for a in argnames) or "void"
    body = []
    if dkind == "thunk":
        body.append("    /* forwarding thunk: tail-call the GDI/USER engine via its runtime dispatch slot */")
        call = "(*g_engine_%x)(%s)" % (delegate, ", ".join(argnames))
    elif dkind == "worker":
        body.append("    /* validated-delegation wrapper: delegate to the shared DC-attribute worker */")
        passed = argnames + (["0x%x /*index*/" % index] if index is not None else [])
        call = "sub_%x(%s)" % (delegate - pe.image_base if delegate > pe.image_base else delegate,
                               ", ".join(passed))
    else:
        body.append("    /* validated-delegation wrapper: validate handle, delegate to the GDI engine */")
        call = "GdiEngine_%x(%s)" % (delegate, ", ".join(argnames))
    body.append("    return %s;" % call)
    c = "unsigned %s(%s)\n{\n%s\n}\n" % (name or ("sub_%x" % rva), params, "\n".join(body))
    return {"rva": rva, "name": name, "index": index, "delegate": delegate, "kind": dkind,
            "argc": argc, "c": c}


def reconstruct_module(module, limit=None):
    pe = H.carve(module).pe
    exports = [(rva, nm) for _o, nm, rva in pe.exports() if nm]
    if limit:
        exports = exports[:limit]
    out = []
    for rva, nm in exports:
        try:
            r = recognize(module, rva, nm)
        except Exception:
            r = None
        if r:
            out.append(r)
    return out


from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    module = argv[0]
    limit = _opt(argv, "--limit"); limit = int(limit) if limit else None
    rs = reconstruct_module(module, limit)
    byw = sum(1 for r in rs if r["kind"] == "worker")
    bye = sum(1 for r in rs if r["kind"] == "engine")
    byt = sum(1 for r in rs if r["kind"] == "thunk")
    print("=== recon32wrap %s : %d validated-delegation wrappers (%d worker-tail, %d engine-dispatch, %d thunk) ===" %
          (module, len(rs), byw, bye, byt))
    emit = _opt(argv, "--emit")
    if emit:
        stem = module.replace(".", "_")
        outp = emit if emit.endswith(".c") else os.path.join(emit, stem + ".wrappers.c")
        with open(outp, "w", encoding="utf-8") as f:
            f.write("/* %s -- reconstructed ring-3 API: validated-delegation wrappers over the\n"
                    "   GDI/USER engine (thin wrappers that validate the handle + delegate; the\n"
                    "   real logic is in 16-bit GDI.EXE/USER.EXE via the flat thunk). Faithful\n"
                    "   structure: per-function index constants and arities are byte/SDK-derived. */\n\n" % module)
            for r in rs:
                tag = {"worker": "DC-attr worker", "engine": "engine dispatch", "thunk": "forwarding thunk"}[r["kind"]]
                f.write("/* %s  [%s%s] */\n%s\n" % (
                    r["name"], tag, (" idx 0x%x" % r["index"]) if r["index"] is not None else "", r["c"]))
        print("  wrote %s (%d functions)" % (outp, len(rs)))
    for r in rs[:16]:
        print("  %-20s [idx %s -> %s] :: %s" % (
            r["name"], ("0x%x" % r["index"]) if r["index"] is not None else "-",
            ("sub_%x" % ((r["delegate"] - H.carve(module).pe.image_base))) if r["kind"] == "worker" else "engine",
            r["c"].replace("\n", " ").strip()))


if __name__ == "__main__":
    main()
