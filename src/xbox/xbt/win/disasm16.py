#!/usr/bin/env python3
"""
disasm16.py — capstone recursive-descent code/data classifier (readability #5).

objdump does a LINEAR sweep: it decodes every byte as an instruction, so data
(strings, jump tables, blobs) comes out as garbage (`outs`, `arpl`, `imul
bp,[di+0x61],0x7972`). A real disassembler follows control flow — it only decodes
what is actually REACHED, so data is identified by never being decoded at all,
not by guessing from what it looks like.

`classify()` does exactly that with capstone in 16-bit mode: from the entry
points, decode an instruction, mark its bytes as code, and queue its branch
target + fall-through (unless it's a jmp/ret). Everything never covered is data.

HONEST LIMITATION (measured): pure recursive descent UNDER-covers code for
externally-dispatched code. A 16-bit driver's functions are reached by far calls
from other modules and by jump tables, so they're not reached from the
intra-segment entry set — and get wrongly called data (MOUSE seg1: 52% "data",
mostly real code). So this is NOT wired into the byte-exact #3 path (it would
emit real code as db and wreck live%); #3 stays on the linear-disasm + datacode
+ reachability(external-entry) classifier, which does not under-cover. capstone
here is a DIAGNOSTIC/cross-check: accurate instruction boundaries (no objdump
`...` collapse) and a high-confidence "definitely code" set. A full
objdump->capstone swap in the byte-exact path is a separate, per-module byte-
exact-validated effort (re-tuning asmrecon.to_masm for capstone syntax).

    python disasm16.py MOUSE --seg 1        # code vs data breakdown (diagnostic)
"""
import argparse
import sys

try:
    import capstone
    from capstone import x86
    _MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    _MD.detail = True
except Exception:
    _MD = None

_ENDERS = {"jmp", "ljmp", "ret", "retf", "iret", "iretf", "iretd", "iretw"}


def classify(code, entries):
    """Set of byte offsets that are reachable CODE (recursive descent from
    entries). Everything else in the segment is data."""
    covered = set()
    if _MD is None:
        return covered
    n = len(code)
    seen = set()
    wl = [e for e in entries if 0 <= e < n]
    while wl:
        a = wl.pop()
        if a in seen or not (0 <= a < n):
            continue
        seen.add(a)
        ins = next(iter(_MD.disasm(bytes(code[a:a + 15]), a, count=1)), None)
        if ins is None:                         # undecodable here -> not code
            continue
        for k in range(a, min(a + ins.size, n)):
            covered.add(k)
        mn = ins.mnemonic
        is_ret = ins.group(capstone.CS_GRP_RET) or ins.group(capstone.CS_GRP_IRET)
        if ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL):
            for op in ins.operands:             # follow a direct (immediate) target
                if op.type == x86.X86_OP_IMM and 0 <= op.imm < n:
                    wl.append(op.imm)
        if not (mn in _ENDERS or is_ret):        # fall-through
            wl.append(a + ins.size)
    return covered


def entries_of(sg):
    """Entry points: carved function starts, exports, intra-segment reloc targets."""
    import re
    ents = set(getattr(sg, "funcs", {}) or {})
    ents |= set(getattr(sg, "export_names", {}) or {})
    si = getattr(sg, "si", 0)
    for site, tgt in (getattr(sg, "fixups", {}) or {}).items():
        m = re.match(r"seg(\d+):0x([0-9a-f]+)$", str(tgt))
        if m and int(m.group(1)) == si:
            ents.add(int(m.group(2), 16))
    return ents


def data_spans(sg, minrun=4):
    """[(start, end)] maximal runs of non-code bytes >= minrun — control flow
    never reaches them, so they are data."""
    code = sg.code
    covered = classify(code, entries_of(sg))
    out = []
    i, n = 0, len(code)
    while i < n:
        if i not in covered:
            j = i
            while j < n and j not in covered:
                j += 1
            if j - i >= minrun:
                out.append((i, j))
            i = j
        else:
            i += 1
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    ap.add_argument("--seg", type=int, default=1)
    args = ap.parse_args()
    sys.path.insert(0, __import__("os").path.dirname(__import__("os").path.abspath(__file__)))
    import ne as ne_mod, trace, target
    o = trace.load_ordinals("ORDINALS.md")
    ne = ne_mod.NEFile(target.resolve(args.module))
    sg = trace.Seg(ne, args.seg, o)
    if _MD is None:
        print("capstone not available"); return
    spans = data_spans(sg)
    db = sum(e - s for s, e in spans)
    print("%s seg%d: %d bytes, %d data span(s), %d data bytes (%.1f%%)"
          % (args.module, args.seg, len(sg.code), len(spans), db,
             100.0 * db / max(1, len(sg.code))))
    for s, e in spans[:30]:
        prev = "".join(chr(b) if 0x20 <= b < 0x7F else "." for b in sg.code[s:min(e, s + 28)])
        print("  %04x-%04x  %3dB  |%s|" % (s, e, e - s, prev))


if __name__ == "__main__":
    main()
