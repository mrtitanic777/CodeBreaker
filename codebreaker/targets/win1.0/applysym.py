#!/usr/bin/env python3
"""
applysym.py — record Microsoft's own symbol names into ORDINALS.md.

Appends ONE new section holding every ordinal whose address falls on a symbol in
a `.SYM` segment whose retail mapping `symscore.correlate()` proved. Ordinals in
unmapped segments are skipped and counted, never guessed.

APPEND ONLY. This file was destroyed once by a `str.replace()` called with an
empty marker, which inserted its payload between every character and grew 77 KB
into 119 MB; worse, every later `replace()` then silently did nothing while
reporting success, and an entire session's records were lost. So:

    - never replace() into this file
    - append, then re-read and assert the new text is present
    - assert the file grew by roughly what was added, and that the ORIGINAL
      content is still byte-identical at the front

`trace.load_ordinals` lets later lines override earlier ones, so an appended row
wins over anything above it — which is what we want, since these names are
Microsoft's and ours were coined.

Usage:
    python applysym.py --dry
    python applysym.py
"""
import os, re, sys
import symscore, trace

LEDGER = "ORDINALS.md"
MARK = "## Microsoft symbol names recovered from the SDK .SYM files"


def collect():
    rows, skipped, matched = [], 0, 0
    for mod in ("KERNEL", "GDI", "USER"):
        m, d, new, data, _syms = symscore.score(mod)
        matched += len(m)
        for od, s, off, ms in new:
            rows.append((mod, od, s, off, ms, "code"))
        for od, s, off, ours, ms in data:
            rows.append((mod, od, s, off, ms, "DATA, not a function"))
        # DIFFER: Microsoft's name supersedes ours, but say so explicitly.
        for od, s, off, ours, ms in d:
            rows.append((mod, od, s, off, ms, "supersedes coined name `%s`" % ours))
    return rows, matched, skipped


def main():
    before = open(LEDGER, encoding="utf-8").read()
    assert MARK not in before, "section already appended — refusing to duplicate"
    n_before = len(before)

    rows, matched, _ = collect()
    if not rows:
        print("nothing to add"); return

    out = []
    out.append("")
    out.append(MARK)
    out.append("")
    out.append("Source: `refs/sdk101/d04/SYM/{KERNEL,GDI,USER}.SYM` — SYMDEB symbol")
    out.append("files shipped by Microsoft with the Windows 1.01 SDK. These are the")
    out.append("original internal names, not names coined by this project.")
    out.append("")
    out.append("They belong to the DEBUG build. They are applied to retail addresses")
    out.append("only where `symscore.correlate()` PROVED the segment mapping by")
    out.append("testing which retail segment's independently carved function starts")
    out.append("the symbol offsets land on. Ordinals in unmapped segments are absent")
    out.append("from this table rather than guessed. See SCORECARD.md.")
    out.append("")
    out.append("%d names below. A further %d ordinals already carried a coined name"
               % (len(rows), matched))
    out.append("that MATCHES the symbol exactly and are therefore unchanged.")
    out.append("")
    out.append("Case is as SYMDEB records it: the assembler uppercased public")
    out.append("symbols, so `PAINTBITS` was very likely `PaintBits` in the source.")
    out.append("")
    out.append("| ordinal | name | address | note |")
    out.append("|---|---|---|---|")
    for mod, od, s, off, ms, note in sorted(rows, key=lambda r: (r[0], r[1])):
        out.append("| %s.%d | `%s` | seg%d:0x%04x | %s |" % (mod, od, ms, s, off, note))
    out.append("")
    add = "\n".join(out)

    if "--dry" in sys.argv:
        print(add[:3000])
        print("\n... %d rows total" % len(rows))
        return

    with open(LEDGER, "a", encoding="utf-8") as f:
        f.write(add)

    # ---- verify, because "it printed ok" is not evidence -------------------
    after = open(LEDGER, encoding="utf-8").read()
    assert after[:n_before] == before, "ORIGINAL CONTENT CHANGED — restore from git"
    assert MARK in after, "marker missing after append"
    grew = len(after) - n_before
    assert abs(grew - len(add)) <= 2, "grew by %d, expected %d" % (grew, len(add))
    for mod, od, s, off, ms, note in rows[:5]:
        assert ("| %s.%d | `%s` |" % (mod, od, ms)) in after, "row missing: %s.%d" % (mod, od)

    named_before = len([v for v in trace.load_ordinals(LEDGER).values()
                        if v and re.match(r"^[A-Za-z_]\w*$", v)])
    print("appended %d rows (%d bytes); original %d bytes intact"
          % (len(rows), grew, n_before))
    print("ORDINALS named now: %d" % named_before)


if __name__ == "__main__":
    main()
