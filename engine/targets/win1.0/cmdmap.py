#!/usr/bin/env python3
"""
cmdmap.py — locate an application's command handlers using its own resources.

THE EVIDENCE
    A MENU resource pairs a command id with the text the user sees. A
    STRINGTABLE pairs a string id with its text. Both were written by the
    original authors and both survive verbatim in the binary.

    A WM_COMMAND handler must compare the incoming wParam against those exact
    command ids, so each id appears in the code as a 16-bit immediate. Finding
    the immediate therefore locates the handler — WITHOUT deciding in advance
    what the handler does.

WHAT THIS TOOL CLAIMS AND WHAT IT DOES NOT
    It claims: "the constant 0x1041 occurs at seg37:0x0148, inside the function
    starting at seg37:0x00c4, and MW_MENU says 0x1041 is Character > Bold".
    All four of those are facts.

    It does NOT claim the function IS the Bold handler. A 16-bit immediate is
    only two bytes; it can occur inside an unrelated constant, an offset, or
    data-in-code. The tool reports every hit and its context and leaves the
    judgement to a reader. Ids under 0x0100 are especially weak — a small
    integer occurs everywhere — so they are reported but flagged NOISY.

    The signal that is actually strong is DENSITY: a function containing eight
    distinct menu ids is a command dispatcher, and no other kind of function
    looks like that.

Usage:
    python cmdmap.py APP                 # functions ranked by distinct ids hit
    python cmdmap.py APP --id 0x1041     # every site of one id
    python cmdmap.py APP --func 37:0x00c4
"""
import sys, re, struct, collections
import trace, rsrc
from engine.disasm_engine.disasm import open_ne

NOISY_BELOW = 0x0100


def menu_ids(ne):
    """{id: 'Popup > Item'} from every MENU resource."""
    out = {}
    for r in ne.resources():
        if r["type"] != "MENU":
            continue
        d = ne.data[r["off"]:r["off"] + r["length"]]
        try:
            items = rsrc.menu(d)
        except Exception:
            continue
        path = []
        for depth, ident, txt, fl in items:
            del path[depth:]
            if ident is None:
                path.append(txt)
                continue
            if ident and txt:
                out[ident] = " > ".join(path + [txt.replace("\t", " / ")])
    return out


def string_ids(ne):
    return {i: t for i, t in rsrc.strings(ne, ne.resources())}


def scan(app):
    ne = open_ne("files/WF/%s.EXE" % app)
    ids = menu_ids(ne)
    strs = string_ids(ne)
    o = trace.load_ordinals("ORDINALS.md")

    # site -> (seg, off, func_start, id, kind)
    hits = []
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            sg = trace.Seg(ne, i, o)
        except Exception:
            continue
        code = sg.code
        starts = sorted(sg.funcs)

        def owner(off):
            lo, hi = 0, len(starts)
            while lo < hi:
                mid = (lo + hi) // 2
                if starts[mid] <= off:
                    lo = mid + 1
                else:
                    hi = mid
            return starts[lo - 1] if lo else None

        # An immediate is only meaningful if it is really an immediate, so the
        # search runs over DISASSEMBLED rows, not raw bytes. A raw-byte scan
        # matches inside opcodes and addresses and produces mostly rubbish.
        for off, row in sg.rows:
            txt = row if isinstance(row, str) else getattr(row, "op_str", str(row))
            for mo in re.finditer(r"0x([0-9a-fA-F]{1,4})\b", txt):
                v = int(mo.group(1), 16)
                if v in ids:
                    hits.append((i, off, owner(off), v, "MENU"))
                elif v in strs and v >= NOISY_BELOW:
                    hits.append((i, off, owner(off), v, "STRING"))
    return ne, ids, strs, hits


def main():
    app = sys.argv[1].upper()
    ne, ids, strs, hits = scan(app)

    if "--id" in sys.argv:
        want = int(sys.argv[sys.argv.index("--id") + 1], 0)
        print("# %s: id 0x%04x = %r" % (app, want, ids.get(want) or strs.get(want)))
        for seg, off, fn, v, kind in hits:
            if v == want:
                print("   seg%-3d:0x%04x   in function seg%d:0x%04x" % (seg, off, seg, fn or 0))
        return

    byfn = collections.defaultdict(set)
    for seg, off, fn, v, kind in hits:
        if fn is not None:
            byfn[(seg, fn)].add(v)
    print("# %s: %d menu ids, %d strings, %d immediate sites"
          % (app, len(ids), len(strs), len(hits)))
    print("# functions ranked by DISTINCT resource ids referenced — a dispatcher")
    print("# is the only kind of function that references many.")
    for (seg, fn), vs in sorted(byfn.items(), key=lambda kv: -len(kv[1]))[:25]:
        noisy = sum(1 for v in vs if v < NOISY_BELOW)
        tag = "  (%d of these are < 0x100, weak)" % noisy if noisy else ""
        print("\n  seg%-3d:0x%04x   %2d distinct ids%s" % (seg, fn, len(vs), tag))
        for v in sorted(vs):
            what = ids.get(v)
            print("        0x%04x  %s" % (v, what if what else "str %r" % strs.get(v)))


if __name__ == "__main__":
    main()
