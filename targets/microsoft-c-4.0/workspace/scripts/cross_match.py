#!/usr/bin/env python3
"""cross_match - identify shared source modules across tools by matching function
code. Two functions built from the same .c (same rev/model) are byte-identical up
to relocated segment refs and layout-dependent call/jump displacements. We
normalize those away (mask reloc sites; blank immediates/displacements in the
disasm) and cluster functions with identical normalized signatures.

Usage: python cross_match.py <A.exe> <B.exe> [C.exe ...]
"""
import sys, os, struct, re, hashlib
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
MD = Cs(CS_ARCH_X86, CS_MODE_16)
MINSIG = 8      # require >= this many instructions to count (cut false positives)


def load(path):
    b = open(path, 'rb').read()
    hdr = struct.unpack('<H', b[8:10])[0] * 16
    return b[hdr:]


def funcs_of(stem):
    fp = os.path.join("workspace", "disasm", stem, "functions.txt")
    out = []
    for l in open(fp):
        m = re.match(r"(0x[0-9a-f]+)\s+size=\s*(\d+)", l)
        if m:
            out.append((int(m.group(1), 16), int(m.group(2))))
    return out


def norm_sig(img, off, size):
    """Normalized signature: mnemonic + operand shape with all numeric literals
    (addresses, displacements, immediates) blanked -> layout-independent."""
    insns = list(MD.disasm(img[off:off+size], off))
    if len(insns) < MINSIG:
        return None, 0
    lines = []
    for ins in insns:
        ops = re.sub(r"0x[0-9a-f]+", "#", ins.op_str)
        lines.append(ins.mnemonic + " " + ops)
    sig = "\n".join(lines)
    return hashlib.sha1(sig.encode()).hexdigest(), len(insns)


def main():
    bins = sys.argv[1:]
    table = {}   # sighash -> {stem: [offsets]}
    ninsn = {}
    for p in bins:
        stem = os.path.splitext(os.path.basename(p))[0]
        img = load(p)
        for off, size in funcs_of(stem):
            h, n = norm_sig(img, off, size)
            if h:
                table.setdefault(h, {}).setdefault(stem, []).append(off)
                ninsn[h] = n
    stems = [os.path.splitext(os.path.basename(p))[0] for p in bins]
    shared = {h: v for h, v in table.items() if len(v) >= 2}
    print(f"binaries: {', '.join(stems)}")
    print(f"shared signatures (appear in >=2 tools): {len(shared)}")
    # pairwise overlap counts
    for i in range(len(stems)):
        for j in range(i+1, len(stems)):
            c = sum(1 for v in table.values() if stems[i] in v and stems[j] in v)
            print(f"  {stems[i]} ^ {stems[j]}: {c} matching functions")
    # biggest shared clusters
    big = sorted(shared.items(), key=lambda kv: -ninsn[kv[0]])[:12]
    print("\nlargest shared functions (insns : tools @ offsets):")
    for h, v in big:
        locs = "  ".join(f"{s}@{','.join('%#x'%o for o in offs)}" for s, offs in v.items())
        print(f"  {ninsn[h]:4d} insn : {locs}")


if __name__ == "__main__":
    main()
