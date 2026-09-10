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
from codebreaker.formats import ne as ne_mod
from codebreaker.core import paths

DATA_ROOT = corpus.DATA_ROOT
_SEGOFF = re.compile(r"se[gc](\w+?):0x([0-9a-fA-F]+)", re.I)  # "seg1:0x051c" (NE) / "sec1:0x0012f40" (PE)

# win95 (and the other 32-bit targets) ship PE binaries; their exports live in the PE
# export directory, addressed by RVA. The corpus keys a function by its code-section-relative
# offset (RVA minus the code section's RVA), so PE enrichment joins on that same offset.
_PE_TARGET = {"win95_gold": "win95", "win98_gold": "win98"}
_pe_target_set = None


def _ensure_pe_target(version_id):
    """Point cores at the version's target once, so names.export_names / the lazy binary
    cache resolve through WT_DATA_ROOT (split repo/data tree aware). Returns False if the
    version isn't a wired PE target."""
    global _pe_target_set
    tgt = _PE_TARGET.get(version_id)
    if not tgt:
        return False
    if _pe_target_set != tgt:
        from codebreaker.core import cores
        here = os.path.dirname(os.path.abspath(__file__))
        cb = os.path.dirname(os.path.dirname(here))            # engine/ -> repo root
        cores.set_target(paths.target_dir(tgt))
        _pe_target_set = tgt
    return True


def _target_root(version_id):
    """Absolute data root for a PE target (the dir the target resolves `files/`, `src/` under)."""
    return os.path.join(DATA_ROOT, _PE_TARGET[version_id])


def _pe_names(module, version_id):
    """{(seg, offset_int) -> (name, ordinal)} for a PE module's exports, addressed in the
    corpus's code-section-relative offset space. Named exports come from the PE export table
    (ground truth); ordinal-only exports are filled from the period SDK's ordinal map where it
    has one (16-bit heritage libs; hypothesis-tier), else left for structural propagation.
    The target resolves `files/` relative to its data root, so we resolve with cwd parked there."""
    from codebreaker.engines.symbol import names
    cwd = os.getcwd()
    try:
        os.chdir(_target_root(version_id))
        ne, _exp = names.export_names(module)
        if not hasattr(ne, "exports"):
            return {}
        cs = ne.code_sections()
        base = (cs[0][0] - ne.image_base) if cs else 0
        exports = list(ne.exports())
    finally:
        os.chdir(cwd)
    seg = 1                                                    # code section == sec1 in the carve
    out = {}
    unnamed = []
    for o, n, rva in exports:
        key = (seg, rva - base)
        if n:
            out[key] = (n, o)                                 # PE export table -- ground truth
        else:
            unnamed.append((key, o))
    if unnamed:                                               # try the SDK ordinal map (16-bit libs)
        try:
            from codebreaker.engines.symbol import symdb
            omap = symdb.ordinal_names(module)                # {ordinal: name} or {}
        except Exception:
            omap = {}
        for key, o in unnamed:
            nm = omap.get(o)
            if nm:
                out[key] = (nm, o)                            # SDK-supplied (hypothesis-tier)
    return out


def _binary_for(version_id, module):
    """Absolute path to the shipped binary a corpus module was carved from, or None."""
    m = module.upper()
    if version_id == "win310":
        p = os.path.join(DATA_ROOT, "win3.1", "files", m)
        return p if os.path.exists(p) else None
    if version_id == "win101":
        # apps ship as ordinary NE .EXE under files/WF; system modules live in the
        # WIN100.BIN bundle (not handled here).
        for cand in (m if m.endswith(".EXE") else m + ".EXE",):
            p = os.path.join(DATA_ROOT, "win1.0", "files", "WF", cand)
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

    is_pe = _ensure_pe_target(version_id)                 # 32-bit PE target (win95/98/nt4)?
    named = done = missing = 0
    for module, addr_to_fid in sorted(by_mod.items()):
        if is_pe:
            try:
                names = _pe_names(module, version_id)      # PE export table + SDK ordinal fill
            except Exception:
                missing += 1
                continue
            if not names:
                missing += 1
                continue
        else:
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
