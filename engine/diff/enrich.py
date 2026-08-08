#!/usr/bin/env python3
r"""enrich.py -- replace `sub_<seg>_<off>` placeholders with real export names.

The fingerprint/match layer works on structure alone, so it never needed names.
Propagation does: a name can only travel A->B if A actually has one. This pass
reads each module's own export tables (ground truth in the binary) and writes the
recovered name onto the matching corpus row, joined by (module, segment, offset).

Sources, in order of authority:
  * NE name tables (resident + non-resident) joined with the entry table
    (ordinal -> seg:offset) -- win3.1 and win1.0 apps name their exports in the
    binary itself. This is ground truth.
  * ORDINALS.md (recovered map) -- for modules whose binary carries no names
    (win1.0's boot-image system modules). Recovered, not read; lower authority.

win95 PE export enrichment and win1.0's WIN100.BIN system modules (behind the
bundle carve) are not wired yet -- they need the PE loader / idord bundle path;
this pass reports what it could and could not name, honestly.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # engine/
import corpus  # noqa: E402
from engine.format_loaders import ne as ne_mod

REPO = corpus.REPO
_SEGOFF = re.compile(r"seg?(\w+?):0x([0-9a-fA-F]+)", re.I)   # "seg1:0x051c" / "sec1:0x0012f40"


def _binary_for(version_id, module):
    """Absolute path to the shipped binary a corpus module was carved from, or None."""
    m = module.upper()
    if version_id == "win310":
        p = os.path.join(REPO, "win3.1", "files", m)
        return p if os.path.exists(p) else None
    if version_id == "win101":
        # apps ship as ordinary NE .EXE under files/WF; system modules live in the
        # WIN100.BIN bundle (not handled here).
        for cand in (m if m.endswith(".EXE") else m + ".EXE",):
            p = os.path.join(REPO, "win1.0", "files", "WF", cand)
            if os.path.exists(p):
                return p
    return None


def _ne_names(path):
    """(seg, offset_int) -> (name, ordinal) from an NE binary's own export tables."""
    o = ne_mod.NEFile(path)
    ord_addr = {e["ordinal"]: (e["seg"], e["offset"])
                for e in o.entries() if "offset" in e and "seg" in e}
    out = {}
    for name, ordinal in list(o.resident_names()) + list(o.nonresident_names()):
        if ordinal == 0 or not name:            # ordinal 0 is the module's own name
            continue
        addr = ord_addr.get(ordinal)
        if addr:
            out[addr] = (name, ordinal)
    return out


def enrich(conn, version_id, apply=True):
    """Name every corpus function whose module binary exports it. Returns
    (named, modules_done, modules_missing)."""
    rows = conn.execute(
        "SELECT function_id, module_name, segment_offset FROM functions WHERE"
        " version_id=? AND symbol_name LIKE 'sub_%'", (version_id,)).fetchall()
    by_mod = {}
    for r in rows:
        m = _SEGOFF.search(r["segment_offset"] or "")
        if not m:
            continue
        try:
            seg = int(m.group(1), 16) if not m.group(1).isdigit() else int(m.group(1))
        except ValueError:
            seg = m.group(1)
        key = (seg, int(m.group(2), 16))
        by_mod.setdefault(r["module_name"], {})[key] = r["function_id"]

    named = done = missing = 0
    for module, addr_to_fid in sorted(by_mod.items()):
        binary = _binary_for(version_id, module)
        if not binary:
            missing += 1
            continue
        try:
            names = _ne_names(binary)
        except Exception:
            missing += 1
            continue
        done += 1
        for (seg, off), (name, ordinal) in names.items():
            fid = addr_to_fid.get((seg, off))
            if fid and apply:
                conn.execute("UPDATE functions SET symbol_name=?, ordinal=? WHERE"
                             " function_id=?", (name, ordinal, fid))
                named += 1
            elif fid:
                named += 1
    if apply:
        conn.commit()
    return named, done, missing
