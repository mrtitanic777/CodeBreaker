#!/usr/bin/env python3
"""
datacode.py — separate DATA from CODE in a segment (readability track #3/#5).

Code segments carry embedded data — strings, jump tables, constant blobs — which
a linear disassembler (objdump) blindly decodes as bogus instructions
(`outs dx,...`, `arpl`, `imul bp,[di+0x61],0x7972`...). Those show up as `db`
byte-dumps and drag down the "live %" even though they were never instructions.

This classifier finds the data regions so asmrecon can emit them as READABLE data
(`db 'primary',0`, `dw L_case1,...`) and exclude them from the instruction/live
count. Two detectors to start:

  * strings  — runs of >= MINSTR printable ASCII (optionally NUL-terminated)
  * garbage  — clusters of the tell-tale "this is not code" mnemonics objdump
               emits over data (outs/ins/arpl/segment-prefix soup)

    python datacode.py MOUSE --seg 1        # list the data regions found
"""
import argparse
import re
import sys

MINSTR = 5                              # min printable run to call it a string
PRINTABLE = set(range(0x20, 0x7F)) | {0x09}
# mnemonics objdump tends to emit ONLY when decoding data as code
GARBAGE_MN = {"outs", "ins", "arpl", "bound", "aaa", "aas", "daa", "das",
              "lds", "les", "sahf", "lahf", "xlat", "hlt", "into"}
SEGPFX = {"fs", "gs", "es", "ds", "cs", "ss", "lock", "repz", "repnz"}


def _texty(run):
    """A run 'looks like text' (not a printable-opcode sequence or a jump table)
    if it contains a lowercase letter. Real Win95 strings almost always do;
    register push/pop runs (SQRVW) and arithmetic tables ($(,048<@) don't."""
    return any(0x61 <= b <= 0x7A for b in run)


def find_strings(code, minlen=MINSTR):
    """[(start, end, text)] for printable-ASCII runs that look like text."""
    out = []
    i, n = 0, len(code)
    while i < n:
        if code[i] in PRINTABLE:
            j = i
            while j < n and code[j] in PRINTABLE:
                j += 1
            run = code[i:j]
            if j - i >= minlen and _texty(run):
                end = j + 1 if j < n and code[j] == 0 else j   # eat a NUL
                out.append((i, end, bytes(run).decode("latin1")))
                i = end
                continue
        i += 1
    return out


_R32 = re.compile(r"\b(eax|ebx|ecx|edx|esi|edi|ebp|esp)\b")


def garbage_spans(rows, code):
    """[(start, end)] over runs of >=3 consecutive rows whose mnemonic is the
    kind objdump only produces over data — a strong 'this is data' signal."""
    def is_garbage(a):
        a = re.sub(r"\s+", " ", a).strip()
        mn = a.split(" ")[0]
        # a 32-bit register in 16-bit code is objdump decoding data (0x66/0x67
        # prefix bytes) — 16-bit modules never use eax/edi/...
        return (mn in GARBAGE_MN or (mn in SEGPFX and " " in a) or "(bad)" in a
                or bool(_R32.search(a)))
    spans, run = [], []
    srt = sorted(rows)
    idx = {o: (srt[k + 1] if k + 1 < len(srt) else None) for k, o in enumerate(srt)}
    for o in srt:
        if is_garbage(rows[o]):
            run.append(o)
        else:
            if len(run) >= 3:
                spans.append((run[0], idx[run[-1]] or run[-1]))
            run = []
    if len(run) >= 3:
        spans.append((run[0], idx[run[-1]] or run[-1]))
    return spans


def data_regions(sg):
    """Merge string + garbage detectors into sorted, non-overlapping data spans."""
    code = sg.code
    rows = {o: a for o, a in sg.rows}
    spans = [(s, e, "str") for s, e, _t in find_strings(code)]
    spans += [(s, e, "data") for s, e in garbage_spans(rows, code)]
    spans.sort()
    merged = []
    for s, e, k in spans:
        if merged and s <= merged[-1][1]:
            merged[-1] = (merged[-1][0], max(merged[-1][1], e), merged[-1][2])
        else:
            merged.append((s, e, k))
    return merged


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
    regs = data_regions(sg)
    tot = sum(e - s for s, e, _k in regs)
    print("%s seg%d: %d data region(s), %d bytes (%.1f%% of %d)"
          % (args.module, args.seg, len(regs), tot, 100.0 * tot / max(1, len(sg.code)),
             len(sg.code)))
    for s, e, k in regs[:40]:
        preview = bytes(sg.code[s:min(e, s + 24)])
        txt = "".join(chr(b) if 0x20 <= b < 0x7f else "." for b in preview)
        print("  %04x-%04x  %-4s  %3dB  |%s|" % (s, e, k, e - s, txt))


if __name__ == "__main__":
    main()
