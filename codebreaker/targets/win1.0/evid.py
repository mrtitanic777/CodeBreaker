#!/usr/bin/env python3
"""
evid.py — one compact EVIDENCE line per still-unnamed ordinal.

Now that idord.open_module() reads both halves of the system image
(WIN100.BIN + WIN100.OVL — see FINDINGS.md §2a), every ordinal's real body is
readable and its calls RESOLVE to names. That turns naming from guesswork into
reading: this tool gathers, per ordinal, the facts that have historically been
decisive in this project —

    retf N        the PASCAL arg-byte count (N/2 words)
    -> names      which ALREADY-NAMED APIs the body calls (the strongest signal:
                  a body that calls GetDlgItem + SetWindowText is SetDlgItemText)
    msg 0xNN      a window-message constant pushed into a SendMessage worker
    RT=n          a resource type constant (names the Load* family)
    AH=nn         the INT 21h DOS function (names the KERNEL wrappers)
    apps=N/x      how many apps call it and how often
    ret           whether callers keep the return value

It NEVER proposes a name. It prints evidence; a human reads it and decides.

Usage:
    python evid.py [MOD ...] [--ord N] [--min-calls N] [--named]
"""
import re, sys, glob, collections
import idord, trace
from codebreaker.foundation import layout

RT = {1: "RT_CURSOR", 2: "RT_BITMAP", 3: "RT_ICON", 4: "RT_MENU",
      5: "RT_DIALOG", 6: "RT_STRING", 8: "RT_FONT", 10: "RT_ACCEL"}


def app_calls():
    """(MOD, ord) -> call count, counted from the reconstructed app sources'
    remaining raw markers (they mirror the binaries' fixups 1:1)."""
    c = collections.Counter()
    for f in glob.glob(layout.recon_dir() + "/*.c"):
        s = open(f, encoding="utf-8", errors="replace").read()
        for mod, o in re.findall(r"Call_([A-Z]+)_(\d+)", s):
            c[(mod, int(o))] += 1
    return c


def evidence(mod, ne, sg, ordn, off):
    f = sg.funcs.get(off)
    if not f:
        return None
    rows = sg.rows[sg.off2idx[off]:sg.off2idx.get(f["end"], len(sg.rows))]
    bits = []
    for o, asm in rows:
        m = re.match(r"retf?\s+0x([0-9a-f]+)", asm)
        if m:
            bits.append(f"retf {int(m.group(1),16)}(={int(m.group(1),16)//2}w)")
            break
        if asm.strip() in ("retf", "ret"):
            bits.append("retf 0(=0w)")
            break
    names = []
    for o, asm in rows:
        if not (asm.startswith("call") or asm.startswith("jmp")):
            continue
        kind, det = sg.resolve_call(o, asm)
        if kind == "sys":
            lbl, nm = det
            names.append(nm or lbl)
        elif kind == "far":
            names.append(str(det))
    seen = []
    for n in names:
        if n not in seen:
            seen.append(n)
    consts = []
    for o, asm in rows:
        m = re.match(r"mov    ax,0x([0-9a-f]+)$", asm)
        if m:
            v = int(m.group(1), 16)
            if v in trace.WM:
                consts.append(trace.WM[v])
        m = re.match(r"mov    ah,0x([0-9a-f]+)", asm)
        if m and int(m.group(1), 16) in trace.DOS21:
            consts.append("AH=%s %s" % (m.group(1), trace.DOS21[int(m.group(1), 16)]))
        m = re.match(r"push   0x([0-9a-f]+)$", asm)
        if m and int(m.group(1), 16) in RT:
            consts.append(RT[int(m.group(1), 16)])
    return dict(size=f["end"] - off, args=bits, calls=seen[:6],
                consts=sorted(set(consts))[:3])


def main():
    a = sys.argv[1:]
    mods = [x.upper() for x in a if not x.startswith("--") and not x.isdigit()]
    if not mods:
        mods = ["USER", "GDI", "KERNEL"]
    only = int(a[a.index("--ord") + 1], 0) if "--ord" in a else None
    minc = int(a[a.index("--min-calls") + 1]) if "--min-calls" in a else 1
    show_named = "--named" in a

    ordinals = trace.load_ordinals("ORDINALS.md")
    counts = app_calls()
    for mod in mods:
        ne = idord.open_module(mod)
        segcache = {}
        rows = []
        ents = [(e["ordinal"], e["seg"], e["offset"]) for e in ne.entries()
                if e.get("kind") != "UNUSED" and "offset" in e]
        for ordn, seg, off in sorted(ents):
            if only is not None and ordn != only:
                continue
            named = ordinals.get(f"{mod}.{ordn}")
            named = named if named and not named.startswith("0x") else None
            n = counts.get((mod, ordn), 0)
            if only is None:
                if named and not show_named:
                    continue
                if n < minc:
                    continue
            if seg not in segcache:
                try:
                    segcache[seg] = trace.Seg(ne, seg, ordinals)
                except Exception:
                    segcache[seg] = None
            sg = segcache[seg]
            if sg is None:
                continue
            ev = evidence(mod, ne, sg, ordn, off)
            if not ev:
                continue
            img = ne.segments()[seg - 1].get("image", "?")
            rows.append((n, ordn, seg, off, img, ev, named))
        rows.sort(key=lambda r: -r[0])
        print(f"\n===== {mod}: {len(rows)} ordinals "
              f"({'named too' if show_named else 'still unnamed'}, "
              f">= {minc} app call sites)")
        for n, ordn, seg, off, img, ev, named in rows:
            head = f"{mod}.{ordn:<4} seg{seg:<2}[{img}]:0x{off:04x} {ev['size']:5d}B calls={n:<3}"
            det = " ".join(ev["args"])
            if ev["consts"]:
                det += "  {" + ", ".join(ev["consts"]) + "}"
            if ev["calls"]:
                det += "  -> " + ", ".join(ev["calls"])
            if named:
                det = f"[{named}] " + det
            print(f"  {head} {det}")


if __name__ == "__main__":
    main()
