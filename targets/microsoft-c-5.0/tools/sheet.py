#!/usr/bin/env python3
"""sheet.py -- per-binary recovery worksheet: list app functions (carved minus CRT),
each with a compact disassembly, resolved near-call targets (named if known), and any
string/global refs. The analyst's view for Phase-5 reconstruction.

    python sheet.py <name> [--all] [--fn 0xADDR]
"""
import sys, os, json, struct, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16, x86
MD = Cs(CS_ARCH_X86, CS_MODE_16); MD.detail = True
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def load(name):
    d = open(os.path.join(ROOT, "unpacked", name+".EXE"), "rb").read()
    hdr = struct.unpack_from("<H", d, 8)[0]*16
    img = d[hdr:]
    cs = struct.unpack_from("<H", d, 22)[0]; ip = struct.unpack_from("<H", d, 20)[0]
    funcs = json.load(open(os.path.join(ROOT,"work",name,"functions.json")))
    ident = json.load(open(os.path.join(ROOT,"work",name,"identified.json")))
    return img, cs*16+ip, funcs, ident

def dgroup_base(img, name):
    # crude: DGROUP begins after _TEXT; use the reloc-target clustering is overkill here.
    return None

def main():
    name = sys.argv[1]
    img, entry, funcs, ident = load(name)
    idset = {int(k,16): v for k,v in ident.items()}
    fnstart = {f["off"] for f in funcs}
    def nameof(t):
        if t in idset: return idset[t]["name"]
        if t == entry: return "__astart"
        if t in fnstart: return f"sub_{t:x}"
        return f"loc_{t:x}"
    only = None
    if "--fn" in sys.argv: only = int(sys.argv[sys.argv.index("--fn")+1],16)
    app = [f for f in funcs if f["off"] not in idset]
    print(f"# {name}: {len(funcs)} carved, {len(idset)} CRT-identified, {len(app)} app functions, entry@{entry:#x}\n")
    show = funcs if "--all" in sys.argv else app
    if only is not None: show = [f for f in funcs if f["off"]==only]
    for f in show:
        o, s = f["off"], f["size"]
        tag = idset[o]["name"] if o in idset else nameof(o)
        calls = []
        for ins in MD.disasm(bytes(img[o:o+s]), o):
            if ins.mnemonic == "call":
                for op in ins.operands:
                    if op.type == x86.X86_OP_IMM:
                        calls.append(op.imm)
        cnames = sorted(set(nameof(c) for c in calls))
        print(f"=== {tag} @ {o:#06x}  size={s}  frame={f['frame']}"
              f"{' probe' if f['probe'] else ''}  calls: {', '.join(cnames) if cnames else '-'}")
        if o in idset:
            print("    [CRT — identified]\n"); continue
        for ins in MD.disasm(bytes(img[o:o+min(s,220)]), o):
            ann = ""
            if ins.mnemonic == "call":
                for op in ins.operands:
                    if op.type == x86.X86_OP_IMM: ann = "  ; " + nameof(op.imm)
            print(f"    {ins.address:04x}: {ins.bytes.hex():14} {ins.mnemonic} {ins.op_str}{ann}")
        print()

if __name__ == "__main__":
    main()
