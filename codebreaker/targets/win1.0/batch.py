#!/usr/bin/env python3
"""
batch.py — decode a QUEUE of functions in one call.

The project's rate limit has not been analysis, it has been round-trips: each
function cost a call to disassemble, another to resolve its dispatch table,
another to chase the interesting arms. Three or four exchanges per function,
one function per turn.

This does all of that for a whole list at once. For each address it prints the
annotated disassembly with every call resolved to a name, AND — the part that
actually saves the round-trips — it recognises the four dispatch forms this
binary uses and expands the table inline, so the message map arrives with the
code instead of after it:

    1. compare chain          cmp ax,K / je ARM
    2. range jump table       sub ax,LO / cmp ax,N / jmp cs:[bx+TBL]
    3. scan table             repnz scasw / jmp cs:[bx+DISP]
    4. direct memory compare

    python batch.py USER 1 0x2c92 0x4ffa 0x5126     three functions, one call
    python batch.py USER 1 --gaps 6                 the six largest uncited
    python batch.py USER 1 0x2c92 --len 0x180       override the extent

    python batch.py USER 1 --gaps 40 --digest       FORTY functions, one call

DIGEST MODE is the answer to "can this do many more functions". Full
disassembly costs ~150 lines each, so the limit was never the loop — it was the
output. `--digest` prints only what is actually needed to decide what a
function IS and to write its header:

    the RETF (so the argument count), the calls it makes with names resolved,
    the globals it reads and writes, the struct fields it touches, the
    constants it compares against, and its dispatch table if it has one.

That is ~10 lines instead of ~150, so forty functions cost what three used to.
The workflow it enables is two-tier: DIGEST forty to find the interesting five,
then FULL-DECODE those five. The digest is also exactly the evidence
`verify.py` checks a reconstruction against — the call list — so a digest is
enough to write a correct file for the simpler functions.

Extent: to the next known function start, capped by --len (default 0x200), so
a big function is truncated rather than swamping the output. `--full` lifts it.
"""
import re
import sys

import idord
import trace

WM = getattr(trace, "WM", {})


def resolve(sg, off, asm):
    """A call/jmp target as a name where one is known."""
    if not asm.startswith(("call", "jmp")):
        return ""
    try:
        kind, d = sg.resolve_call(off, asm)
    except Exception:
        return ""
    if kind == "sys":
        return d[1] or d[0]
    if kind == "int":
        return "-> 0x%04x" % d
    if kind == "far":
        return str(d)
    return ""


def decode_dispatch(sg, rows, base):
    """Recognise a dispatch and return printable lines describing it.

    Form 2 is the one worth automating: the handler base for a `repnz scasw`
    table is `table + displacement + 2` — the scan leaves DI PAST the match.
    Writing that as arithmetic rather than as a step is what stops it being
    mis-applied (it was, once, to the list box)."""
    out = []
    code = sg.code
    for i, (off, asm) in enumerate(rows):
        # ---- form 2: range jump table -----------------------------------
        m = re.search(r"jmp\s+WORD PTR cs:\[bx\+0x([0-9a-f]+)\]", asm)
        if m:
            tbl = int(m.group(1), 16)
            lo, n = 0, 0
            scan = False
            for off2, asm2 in rows[max(0, i - 12):i]:
                mm = re.match(r"sub\s+ax,0x([0-9a-f]+)", asm2)
                if mm:
                    lo = int(mm.group(1), 16)
                mm = re.match(r"cmp\s+ax,0x([0-9a-f]+)", asm2)
                if mm:
                    n = int(mm.group(1), 16)
                if "scas" in asm2:
                    scan = True
            if scan:
                out.append("    [scan table] handler = 0x%04x + 2 + 2*i "
                           "(repnz scasw leaves DI PAST the match)" % tbl)
                base_ = tbl + 2
                for k in range(min(n or 24, 40)):
                    try:
                        h = int.from_bytes(code[base_ + 2 * k:base_ + 2 * k + 2], "little")
                    except Exception:
                        break
                    out.append("       i=%-3d -> 0x%04x" % (k, h))
            else:
                out.append("    [jump table at 0x%04x] messages 0x%02x..0x%02x"
                           % (tbl, lo, lo + n))
                for k in range(n + 1):
                    try:
                        h = int.from_bytes(code[tbl + 2 * k:tbl + 2 * k + 2], "little")
                    except Exception:
                        break
                    msg = lo + k
                    out.append("       0x%04x %-20s -> 0x%04x"
                               % (msg, WM.get(msg, ""), h))
    # ---- form 1: compare chain ------------------------------------------
    chain = []
    pending = None
    for off, asm in rows:
        m = re.match(r"cmp\s+ax,0x([0-9a-f]+)", asm)
        if m:
            pending = int(m.group(1), 16)
            continue
        m = re.match(r"je\s+0x([0-9a-f]+)", asm)
        if m and pending is not None:
            chain.append((pending, int(m.group(1), 16)))
            pending = None
        m = re.match(r"jne\s+0x([0-9a-f]+)", asm)
        if m and pending is not None:
            pending = pending  # jne skips the arm; the arm follows
    if len(chain) >= 3:
        out.append("    [compare chain] %d arms" % len(chain))
        for v, t in chain:
            out.append("       0x%04x %-20s -> 0x%04x" % (v, WM.get(v, ""), t))
    return out


def one(sg, start, end, label=""):
    rows = trace.disasm_whole(sg.code[start:end], base=start)
    print("\n/* ===== %s0x%04x  (%d bytes) ===== */" % (label, start, end - start))
    for line in decode_dispatch(sg, rows, start):
        print(line)
    for off, asm in rows:
        t = resolve(sg, off, asm)
        m = re.match(r"cmp\s+ax,0x([0-9a-f]+)", asm)
        if m and not t:
            t = WM.get(int(m.group(1), 16), "")
        print("   %04x: %-34s %s" % (off, asm, t))


def digest(sg, start, end, label, ords_, mod):
    """~10 lines per function: what it IS, not what it says."""
    rows = trace.disasm_whole(sg.code[start:end], base=start)
    calls, globals_r, globals_w, fields, consts = [], set(), set(), {}, set()
    retf = None
    for off, asm in rows:
        if asm.startswith(("call", "jmp")):
            t = resolve(sg, off, asm)
            if t and not t.startswith("-> 0x"):
                calls.append(t)
            elif t:
                calls.append(t.replace("-> ", "sub_"))
        m = re.match(r"ret[f]?\s+0x([0-9a-f]+)", asm)
        if m:
            retf = int(m.group(1), 16)
        for m in re.finditer(r"ds:0x([0-9a-f]+)", asm):
            a = int(m.group(1), 16)
            head = asm.split()[0]
            wrote = (asm.split(",")[0].find("ds:") >= 0
                     and head in ("mov", "inc", "dec", "add", "sub", "or", "and", "xor"))
            (globals_w if wrote else globals_r).add(a)
        for m in re.finditer(r"\[(si|di|bx|bp)\+0x([0-9a-f]+)\]", asm):
            if m.group(1) != "bp":
                k = "%s+0x%s" % (m.group(1), m.group(2))
                fields[k] = fields.get(k, 0) + 1
        m = re.match(r"cmp\s+\w+,0x([0-9a-f]+)", asm)
        if m:
            consts.add(int(m.group(1), 16))

    print("\n%s0x%04x  %d bytes%s" % (label, start, end - start,
          "  retf %d (%d args)" % (retf, retf // 2) if retf else ""))
    d = decode_dispatch(sg, rows, start)
    if d:
        print("   DISPATCH:")
        for line in d[:26]:
            print("   " + line)
        if len(d) > 26:
            print("      ... %d more entries" % (len(d) - 26))
    if calls:
        seen = []
        for c in calls:
            if c not in seen:
                seen.append(c)
        print("   CALLS:   " + ", ".join(seen[:14]))
    if globals_w:
        print("   WRITES:  " + ", ".join("ds:0x%x" % a for a in sorted(globals_w)))
    if globals_r:
        print("   READS:   " + ", ".join("ds:0x%x" % a for a in sorted(globals_r))[:200])
    if fields:
        top = sorted(fields.items(), key=lambda kv: -kv[1])[:10]
        print("   FIELDS:  " + ", ".join("%s x%d" % (k, n) for k, n in top))
    if consts:
        cs = sorted(consts)[:14]
        print("   CONSTS:  " + ", ".join(
            "0x%x%s" % (c, "(%s)" % WM[c] if c in WM else "") for c in cs))


def main():
    a = [x for x in sys.argv[1:] if not x.startswith("--")]
    if len(a) < 2:
        print(__doc__)
        return
    mod, seg = a[0].upper(), int(a[1])
    ords_ = trace.load_ordinals("ORDINALS.md")
    ne = idord.open_module(mod)
    sg = trace.Seg(ne, seg, ords_)
    starts = sorted(sg.funcs)

    names = {}
    for e in ne.entries():
        if e.get("seg") == seg and "offset" in e:
            nm = ords_.get("%s.%d" % (mod, e["ordinal"]))
            names[e["offset"]] = "%s  " % (nm or "%s.%d" % (mod, e["ordinal"]))

    cap = 0x200
    if "--len" in sys.argv:
        cap = int(sys.argv[sys.argv.index("--len") + 1], 0)
    if "--full" in sys.argv:
        cap = 1 << 20

    if "--gaps" in sys.argv:
        n = int(sys.argv[sys.argv.index("--gaps") + 1])
        sizes = []
        for i, s in enumerate(starts):
            e = starts[i + 1] if i + 1 < len(starts) else len(sg.code)
            sizes.append((e - s, s))
        targets = [s for _, s in sorted(sizes, reverse=True)[:n]]
    else:
        targets = [int(x, 0) for x in a[2:]]

    dig = "--digest" in sys.argv
    if dig:
        print("# %s seg%d — DIGEST of %d functions" % (mod, seg, len(targets)))
    for t in targets:
        i = next((k for k, s in enumerate(starts) if s >= t), None)
        nxt = starts[i + 1] if i is not None and i + 1 < len(starts) else len(sg.code)
        end = min(nxt, t + (cap if not dig else 0x400))
        if dig:
            digest(sg, t, end, names.get(t, ""), ords_, mod)
        else:
            one(sg, t, end, names.get(t, ""))


if __name__ == "__main__":
    main()
