#!/usr/bin/env python3
"""
msgfwd.py — name USER ordinals by the WINDOW MESSAGE they forward.

A whole family of USER APIs is nothing but "validate the window handle, then
SendMessage(hWnd, WM_xxx, wParam, lParam)". In the Win1.0 USER binary those
bodies literally load the message number into AX and tail-jump into the shared
SendMessage worker. So the WM_ constant NAMES the ordinal, definitively and
without guessing — the same class of evidence as the RT_* constant naming the
Load* family, or the INT 21h AH naming the KERNEL wrappers.

  e.g. USER.38 -> mov ax,0xe (WM_GETTEXTLENGTH) -> worker  =  GetWindowTextLength
       USER.37 -> mov ax,0xc (WM_SETTEXT)       -> worker  =  SetWindowText

Usage:
    python msgfwd.py [MOD] [SEG] [--worker 0xADDR]
      default MOD=USER SEG=1; the worker offset is auto-detected as the
      most-jumped-to target that is preceded by a message-constant load.
"""
import re, sys, collections
import idord, trace

CAP = 80            # instructions to follow from an entry before giving up


def scan(mod="USER", seg_index=1, worker=None):
    ne = idord.open_module(mod)
    ordinals = trace.load_ordinals("ORDINALS.md")
    sg = trace.Seg(ne, seg_index, ordinals)
    rows = sg.rows
    idx = sg.off2idx

    # entry-table offsets in this segment -> ordinal
    ent = {}
    for e in ne.entries():
        if e.get("seg") == seg_index and "offset" in e:
            ent.setdefault(e["offset"], e["ordinal"])

    def follow(start):
        """From an entry, follow straight-line code looking for the exact
        SendMessage-forward shape, which is what makes this evidence and not a
        guess about a stray constant:

            push <hWnd reg>            ; the validated window handle
            mov  ax,IMM ; push ax      ; the MESSAGE number
            <3 more pushes>            ; wParam, lParam(lo,hi)
            jmp/call WORKER            ; the shared SendMessage worker

        Anything looser (a bare `mov ax,1` somewhere in the body) is rejected.
        """
        i = idx.get(start)
        if i is None:
            return None
        n = len(rows)
        for j in range(i, min(i + CAP, n - 6)):
            off, asm = rows[j]
            m = re.match(r"mov    ax,0x([0-9a-f]+)$", asm)
            if not m:
                if asm.startswith("ret"):
                    return None
                continue
            if not rows[j + 1][1].startswith("push   ax"):
                continue
            if not rows[j - 1][1].startswith("push  "):      # the hWnd push
                continue
            msg = int(m.group(1), 16)
            pushes, k = 0, j + 2
            while k < n and (rows[k][1].startswith("push  ")
                             or rows[k][1].startswith("xor    ax,ax")
                             or rows[k][1] == "nop"):
                if rows[k][1].startswith("push  "):
                    pushes += 1
                k += 1
            if pushes < 3 or k >= n:
                continue
            m2 = re.match(r"(jmp|call)   \s*0x([0-9a-f]+)$", rows[k][1])
            if m2:
                return (msg, int(m2.group(2), 16))
        return None

    hits = {}
    for off, o in ent.items():
        r = follow(off)
        if r:
            hits[o] = (off, r[0], r[1])

    if worker is None:                     # auto-detect the SendMessage worker
        tally = collections.Counter(t for _, _, t in hits.values())
        worker = tally.most_common(1)[0][0] if tally else None

    print(f"# {mod} seg{seg_index}: message-forwarding ordinals "
          f"(worker = 0x{worker:04x})\n")
    for o in sorted(hits):
        off, msg, tgt = hits[o]
        if tgt != worker:
            continue
        nm = trace.WM.get(msg, f"msg 0x{msg:04x} (unknown constant)")
        print(f"  {mod}.{o:<4} seg{seg_index}:0x{off:04x}  ->  {nm}")
    print("\n# other constant->call sites (not the shared worker):")
    for o in sorted(hits):
        off, msg, tgt = hits[o]
        if tgt == worker:
            continue
        nm = trace.WM.get(msg)
        if nm:
            print(f"  {mod}.{o:<4} seg{seg_index}:0x{off:04x}  ->  {nm}"
                  f"   via 0x{tgt:04x}")


if __name__ == "__main__":
    a = [x for x in sys.argv[1:] if not x.startswith("--")]
    w = None
    if "--worker" in sys.argv:
        w = int(sys.argv[sys.argv.index("--worker") + 1], 0)
    scan(a[0].upper() if a else "USER", int(a[1]) if len(a) > 1 else 1, w)
