#!/usr/bin/env python3
"""
carryover.py — cross-version reuse mapper: how much of the Windows 95
reconstruction already matches Windows 98, byte-for-byte.

The point of this tool is to NOT rewrite the OS twice. Windows 98 is largely
Windows 95 recompiled: whole modules are byte-identical, and inside the modules
that did change, most individual functions did not. Wherever a 95 function
matches its 98 counterpart at the byte level, the 95 reconstruction of that
function — the cited C/asm we already wrote — is already correct for 98. This
tool finds those functions and reports what is free.

The comparison is MASK-AWARE, and that is the whole trick (borrowed from
segdiff.py). Two functions can be the *same code* yet differ at every relocation
site: a 16-bit far call carries a segment/selector that the loader fixes up, a
PE reference carries an absolute address. Those bytes legitimately differ
between builds. So we mask the relocation sites on BOTH sides (NE:
ne.relocations following the fixup chains; PE: the base-reloc table) and compare
only the real opcode stream. An unmasked compare would call every module
"changed" and find nothing reusable.

EVIDENCE RULE: a match is only ever proposed from the retail bytes of
BOTH corpora. Nothing here invents a name or a body. A "reconstructed & matches"
verdict means the 95 reconstruction cited these exact bytes and the 98 binary
carries them unchanged — the strongest kind of carry-over there is.

Levels:
    carryover                     corpus scan: one line per shared module,
                                  code-byte match %, and whether 95 reconstructed it
    carryover --module USER.EXE   function-level: carve both, match masked
                                  fingerprints; list identical / changed / new,
                                  and which identical funcs are already reconstructed
    carryover --reconstructed     scan only the modules 95 actually reconstructed
                                  (the immediately-actionable set)
    carryover --port [--yes]      copy the transferable 95 reconstructed/*.c for
                                  fully-matching modules into ours (report-only
                                  without --yes)
    carryover --selftest          prove the mechanism (a module matches itself 100%)

The 95 project is located via `config win95_project <path>`; its `files/` (retail
95 binaries) and `reconstructed/` must be present.
"""
import argparse
import hashlib
import json
import os
import re
import sys

import target
from codebreaker.recon_engine import segdiff
from codebreaker.foundation import layout

# --------------------------------------------------------------------------
# locating the Windows 95 project (its files/ and reconstructed/)
# --------------------------------------------------------------------------

def _config():
    # win98re writes CodeBreaker.config.json next to the EXE / tools dir.
    for base in (os.path.dirname(os.path.abspath(__file__)),
                 os.getcwd(), os.path.dirname(os.getcwd())):
        p = os.path.join(base, "CodeBreaker.config.json")
        if os.path.exists(p):
            try:
                return json.load(open(p))
            except (OSError, ValueError):
                pass
    return {}


def w95_root():
    """Path to the Windows 95 project root (holds files/ + reconstructed/)."""
    cfg = _config()
    if cfg.get("win95_project"):
        return cfg["win95_project"]
    # default: the sibling win95 project directory
    here = os.getcwd()
    cand = os.path.join(os.path.dirname(here), "win95")
    return cand


def _need(path, what):
    if not os.path.isdir(path):
        sys.exit(f"carryover: {what} not found at {path!r}\n"
                 f"  set it with:  config win95_project <path-to-win95>\n"
                 f"  (its files/ must be extracted — run `extract` there if missing)")
    return path


# --------------------------------------------------------------------------
# module indexes
# --------------------------------------------------------------------------

def index(files_dir):
    """{NAME.UPPER: path} for every file in a corpus's files/ dir."""
    out = {}
    for f in os.listdir(files_dir):
        p = os.path.join(files_dir, f)
        if os.path.isfile(p):
            out[f.upper()] = p
    return out


def md5(path):
    with open(path, "rb") as f:
        return hashlib.md5(f.read()).hexdigest()


# --------------------------------------------------------------------------
# NE: mask-aware per-segment and per-function comparison
# --------------------------------------------------------------------------

def _ne_code_segs(path):
    """[(idx, code, mask)] for every CODE segment, mask = relocation-affected
    byte offsets (reuses segdiff's fixup-chain-following extractor)."""
    from codebreaker.format_loaders import ne as ne_mod
    ne = ne_mod.NEFile(path)
    out = []
    for i, s in enumerate(ne.segments(), 1):
        if s.get("kind") != "CODE":
            continue
        code, mask = segdiff.shipped_ne_segment(path, i)
        out.append((i, code, mask))
    return out


def _masked_diff(a, amask, b, bmask):
    """(matched_bytes, compared_bytes) over the region both share, ignoring
    positions masked on either side. compared excludes masked positions."""
    skip = amask | bmask
    n = min(len(a), len(b))
    compared = matched = 0
    for i in range(n):
        if i in skip:
            continue
        compared += 1
        if a[i] == b[i]:
            matched += 1
    return matched, compared


def compare_ne(p95, p98):
    """Segment-aligned mask-aware compare. Returns dict with per-segment verdicts
    and the code-byte match fraction. Segments align by index (the common case
    for a recompiled module); a length mismatch on a segment is reported, not
    silently matched."""
    s95 = {i: (c, m) for i, c, m in _ne_code_segs(p95)}
    s98 = {i: (c, m) for i, c, m in _ne_code_segs(p98)}
    segs = sorted(set(s95) | set(s98))
    total = matched = 0
    per = []
    for i in segs:
        if i not in s95 or i not in s98:
            only = "95" if i in s95 else "98"
            code = (s95.get(i) or s98.get(i))[0]
            per.append((i, "only-%s" % only, 0, len(code)))
            total += len(code)
            continue
        c95, m95 = s95[i]
        c98, m98 = s98[i]
        mt, cmp = _masked_diff(c95, m95, c98, m98)
        exact = (len(c95) == len(c98) and mt == cmp)
        # code-byte accounting keyed on the larger segment so shrink/growth counts
        seglen = max(len(c95), len(c98))
        seg_matched = mt if not exact else seglen
        per.append((i, "exact" if exact else "%.0f%%" % (100.0 * mt / cmp if cmp else 0),
                    seg_matched, seglen))
        total += seglen
        matched += seg_matched
    return {"per_seg": per, "matched": matched, "total": total,
            "pct": (100.0 * matched / total) if total else 0.0}


def _carve_ne(path):
    """{(seg, off): (masked_bytes, size, name)} for every carved NE function."""
    from codebreaker.format_loaders import ne as ne_mod
    from codebreaker.disasm_engine import trace
    ne = ne_mod.NEFile(path)
    try:
        ordinals = trace.load_ordinals("ORDINALS.md")
    except Exception:
        ordinals = {}
    out = {}
    for i, s in enumerate(ne.segments(), 1):
        if s.get("kind") != "CODE":
            continue
        code, mask = segdiff.shipped_ne_segment(path, i)
        try:
            sg = trace.Seg(ne, i, ordinals)
        except Exception:
            continue
        for start, f in sg.funcs.items():
            end = f.get("end", start)
            if end <= start:
                continue
            b = bytearray(code[start:end])
            for k in range(len(b)):                    # zero the masked sites
                if (start + k) in mask:
                    b[k] = 0
            out[(i, start)] = (bytes(b), end - start, f.get("name") or "")
    return out


def _carve_pe(path):
    """{('R', rva): (masked_bytes, size, name)} for every carved PE function.

    PE masking has two parts, the analogue of NE's fixup mask:
      * base relocations (pe.base_relocs) — the absolute-address fields the loader
        would patch; they differ between builds. Masked precisely from the .reloc
        table, 4 bytes each.
      * relative branch displacements (E8/E9 rel32, 0F8x jcc rel32, EB/7x rel8) —
        a call/jmp to a *moved* neighbour has a different displacement even when
        the code is the same, exactly like NE's self-segment far calls. Found by
        capstone from the ORIGINAL bytes (so masking never corrupts the decode),
        then zeroed. This over-masks intra-function jumps slightly — safe: it can
        only cost matches, never invent one.
    """
    import capstone
    from codebreaker.format_loaders import pe_trace
    c = pe_trace.PeCarve(target.resolve(path))
    base = c.pe.image_base
    reloc_va = {base + r for r in c.pe.base_relocs()}
    secs = c.sections                                  # [(va, bytes, name)]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = False

    def slice_of(start, end):
        for sva, data, _nm in secs:
            if sva <= start < sva + len(data):
                return bytes(data[start - sva:end - sva])
        return None

    out = {}
    for start, f in c.funcs.items():
        end = f.get("end", start)
        if end <= start:
            continue
        raw = slice_of(start, end)
        if not raw:
            continue
        b = bytearray(raw)
        # (1) relative-branch displacements, located on the ORIGINAL bytes
        for insn in md.disasm(raw, 0):
            bs = insn.bytes
            a, sz = insn.address, insn.size
            if not bs:
                continue
            if bs[0] in (0xE8, 0xE9) and sz >= 5:                 # call/jmp rel32
                for k in range(a + 1, a + 5):
                    b[k] = 0
            elif bs[0] == 0x0F and sz >= 6 and 0x80 <= bs[1] <= 0x8F:   # jcc rel32
                for k in range(a + 2, a + 6):
                    b[k] = 0
            elif (bs[0] == 0xEB or 0x70 <= bs[0] <= 0x7F or bs[0] in (0xE0, 0xE1, 0xE2, 0xE3)) and sz >= 2:
                b[a + 1] = 0                                       # rel8 branch
        # (2) base-reloc absolute-address fields
        for va in reloc_va:
            if start <= va < end:
                for k in range(4):
                    idx = va - start + k
                    if 0 <= idx < len(b):
                        b[idx] = 0
        out[("R", start - base)] = (bytes(b), end - start, f.get("name") or "")
    return out


def _carve(path):
    """Format-dispatched function carve. NE -> (seg,off) keys, PE -> ('R',rva)."""
    fmt = target.format_of(path)
    if fmt == "NE":
        return _carve_ne(path)
    if fmt == "PE":
        return _carve_pe(path)
    return {}


def _fingerprint(b):
    return hashlib.md5(b).hexdigest()


def match_functions(p95, p98):
    """Match carved functions across versions by masked fingerprint (works for NE
    and PE — the carve is format-dispatched).
    Returns dict: identical [(k95,k98,size,name)], changed [k95...], new [k98...]."""
    f95 = _carve(p95)
    f98 = _carve(p98)
    by_fp98 = {}
    for k, (b, sz, nm) in f98.items():
        by_fp98.setdefault(_fingerprint(b), []).append((k, sz, nm))
    used98 = set()
    identical, changed = [], []
    for k95, (b, sz, nm) in sorted(f95.items()):
        fp = _fingerprint(b)
        cands = [c for c in by_fp98.get(fp, []) if c[0] not in used98]
        if cands:
            # prefer the candidate at the same (seg,off); else first free
            same = [c for c in cands if c[0] == k95]
            pick = (same or cands)[0]
            used98.add(pick[0])
            identical.append((k95, pick[0], sz, nm or pick[2]))
        else:
            changed.append((k95, sz, nm))
    matched98 = {p[1] for p in identical}
    new = [(k, sz, nm) for k, (b, sz, nm) in sorted(f98.items()) if k not in matched98]
    return {"identical": identical, "changed": changed, "new": new,
            "n95": len(f95), "n98": len(f98)}


# --------------------------------------------------------------------------
# PE: coarse .text compare — the FAST-scan fallback only. The accurate path is
# function-level (_carve_pe + match_functions), used by --module and --reconstructed.
# --------------------------------------------------------------------------

def compare_pe(p95, p98):
    """Coarse: compare the code section mask-aware when sizes line up. PE code is
    usually rebased/reordered, so a non-match here means 'needs a function-level
    pass', not 'nothing reusable'. Kept deliberately honest."""
    try:
        from codebreaker.format_loaders import pe as pe_mod
    except Exception:
        return None
    def text(path):
        p = pe_mod.open_pe(path)
        for s in p.sections():
            if s.get("name", "").rstrip("\x00").lower() in (".text", "text") or (s.get("flags", 0) & 0x20):
                data = p.data[s["file_off"]:s["file_off"] + s["raw_size"]]
                return data
        return b""
    try:
        t95, t98 = text(p95), text(p98)
    except Exception:
        return None
    if not t95 or not t98:
        return None
    if len(t95) != len(t98):
        return {"pct": 0.0, "matched": 0, "total": max(len(t95), len(t98)),
                "note": "text size differs (needs function-level pass)"}
    same = sum(1 for a, b in zip(t95, t98) if a == b)
    return {"pct": 100.0 * same / len(t95), "matched": same, "total": len(t95),
            "note": "unmasked .text compare (coarse)"}


# --------------------------------------------------------------------------
# 95 reconstruction cross-reference
# --------------------------------------------------------------------------

_MODULE_RX = re.compile(r"MODULE:\s*([A-Za-z0-9_.]+)", re.I)
_CITE_RX = re.compile(r"seg(\d+):0x([0-9a-fA-F]+)")           # NE: (seg, off)
_RVA_CITE_RX = re.compile(r"\brva:0x([0-9a-fA-F]+)", re.I)    # PE: ('R', rva)


def recon_index(w95):
    """{MODULE.UPPER: {"files": [names], "cites": {(seg, off), ...}}} scanned from
    the 95 reconstructed/*.c hand-written reconstructions."""
    rdir = os.path.join(w95, "reconstructed")
    out = {}
    if not os.path.isdir(rdir):
        return out
    for fn in os.listdir(rdir):
        if not fn.endswith(".c"):
            continue
        path = os.path.join(rdir, fn)
        try:
            txt = open(path, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        m = _MODULE_RX.search(txt)
        if not m:
            continue
        mod = m.group(1).upper()
        rec = out.setdefault(mod, {"files": [], "cites": set()})
        rec["files"].append(fn)
        for seg, off in _CITE_RX.findall(txt):
            rec["cites"].add((int(seg), int(off, 16)))
        for rva in _RVA_CITE_RX.findall(txt):
            rec["cites"].add(("R", int(rva, 16)))
    return out


def reconstructed_funcs(rec_entry, min_cites=2):
    """The (seg, off) a reconstruction actually READ: cited >= min_cites distinct
    addresses inside the function's span. Here we approximate a function's span by
    grouping citations — but the caller intersects against real carved starts, so
    we return the raw cited set and let the carve decide membership."""
    return rec_entry["cites"]


# --------------------------------------------------------------------------
# classification + reporting
# --------------------------------------------------------------------------

def reuse_accurate(p95, p98):
    """Byte-weighted reuse via function fingerprints (location-independent, so it
    counts moved/reorganized functions the segment-index compare would miss).
    Works for NE and PE. Returns % of 98's carved code bytes that byte-match some
    95 function."""
    mm = match_functions(p95, p98)
    # identical entries are (k95, k98, size, name); byte-equal so 95/98 sizes agree
    matched = sum(t[2] for t in mm["identical"])
    total = matched + sum(t[1] for t in mm["new"])   # new = 98 funcs with no match
    return {"pct": (100.0 * matched / total) if total else 0.0,
            "matched": matched, "total": total, "mm": mm}


def classify(name, p95, p98, accurate=False):
    """One module's cross-version verdict. `accurate` uses function-level
    fingerprint matching for NE (slower: disassembles both), which counts moved
    functions; the default fast path aligns segments by index (a safe LOWER bound
    for reorganized modules — it never over-claims reuse)."""
    fmt95, fmt98 = target.format_of(p95), target.format_of(p98)
    r = {"name": name, "fmt": fmt98 or fmt95, "verdict": "?", "pct": 0.0}
    if md5(p95) == md5(p98):
        r.update(verdict="FILE-IDENTICAL", pct=100.0)
        return r
    if fmt95 != fmt98:
        r.update(verdict="FORMAT-CHANGED (%s->%s)" % (fmt95, fmt98))
        return r
    if fmt98 == "NE":
        c = reuse_accurate(p95, p98) if accurate else compare_ne(p95, p98)
        r["pct"] = c["pct"]
        r["detail"] = c
        r["approx"] = not accurate
        r["verdict"] = ("CODE-IDENTICAL" if c["pct"] >= 99.95 else
                        "PARTIAL" if c["pct"] >= 1.0 else "DIVERGED")
    elif fmt98 == "PE":
        # accurate: function-level fingerprint match (counts moved functions).
        # fast: coarse .text compare — a safe lower bound / "needs function pass".
        if accurate:
            c = reuse_accurate(p95, p98)
            r["pct"] = c["pct"]; r["detail"] = c
            r["verdict"] = ("CODE-IDENTICAL" if c["pct"] >= 99.95 else
                            "PARTIAL" if c["pct"] >= 1.0 else "DIVERGED")
        else:
            c = compare_pe(p95, p98)
            if c and c["pct"] > 0:
                r["pct"] = c["pct"]; r["detail"] = c; r["approx"] = True
                r["verdict"] = ("CODE-IDENTICAL" if c["pct"] >= 99.95 else
                                "PARTIAL" if c["pct"] >= 1.0 else "DIVERGED")
            else:
                r["verdict"] = "PE (needs function pass)"
    else:
        r["verdict"] = "%s (compare not implemented)" % fmt98
    return r


def scan(only_reconstructed=False, accurate=None):
    w95 = _need(w95_root(), "Windows 95 project")
    d95 = _need(os.path.join(w95, "files"), "Windows 95 files/")
    d98 = _need(os.path.join(target.FILES), "Windows 98 files/")
    a, b = index(d95), index(d98)
    recon = recon_index(w95)
    names = sorted(set(a) & set(b))
    if only_reconstructed:
        # a reconstructed module may be keyed by DLL/EXE name in the banner
        recon_names = set(recon)
        names = [n for n in names if n in recon_names]
    # default: accurate function-level metric for the small reconstructed set,
    # fast segment-index lower bound for the full corpus (thousands of modules).
    if accurate is None:
        accurate = only_reconstructed
    rows = []
    for n in names:
        try:
            r = classify(n, a[n], b[n], accurate=accurate)
        except Exception as e:
            r = {"name": n, "fmt": "?", "verdict": "ERROR: %s" % e, "pct": 0.0}
        r["reconstructed"] = n in recon
        rows.append(r)
    return rows, recon, a, b


def render_scan(rows, recon):
    order = {"FILE-IDENTICAL": 0, "CODE-IDENTICAL": 1, "PARTIAL": 2}
    def key(r):
        v = r["verdict"].split()[0]
        return (0 if r.get("reconstructed") else 1, order.get(v, 3), -r["pct"], r["name"])
    rows = sorted(rows, key=key)
    print("%-16s %-5s %-26s %6s  %s" % ("module", "fmt", "verdict", "match", "95-recon"))
    print("-" * 74)
    rec_shown = 0
    for r in rows:
        star = "RECON" if r.get("reconstructed") else ""
        if star:
            rec_shown += 1
        pct = ("%5.1f%%%s" % (r["pct"], "+" if r.get("approx") and r["pct"] > 0
                              and r["verdict"] not in ("CODE-IDENTICAL", "FILE-IDENTICAL")
                              else " "))
        print("%-16s %-5s %-26s %7s  %s"
              % (r["name"][:16], r["fmt"] or "?", r["verdict"][:26], pct, star))
    # summary
    fi = sum(1 for r in rows if r["verdict"] == "FILE-IDENTICAL")
    ci = sum(1 for r in rows if r["verdict"] == "CODE-IDENTICAL")
    pa = sum(1 for r in rows if r["verdict"] == "PARTIAL")
    print("-" * 74)
    print("shared modules: %d   file-identical: %d   code-identical: %d   partial: %d"
          % (len(rows), fi, ci, pa))
    print("reconstructed-in-95 & shared: %d  (these are the immediate carry-overs)"
          % rec_shown)
    if any(r.get("approx") and r["pct"] > 0 for r in rows):
        print("note: '+' = segment-index lower bound (fast scan); run"
              " `carryover --module <M>` for the exact function-level figure.")


def _kfmt(k):
    """Render a carve key: NE (seg, off) or PE ('R', rva)."""
    if isinstance(k, tuple) and len(k) == 2 and k[0] == "R":
        return "rva:0x%x" % k[1]
    return "seg%d:0x%04x" % (k[0], k[1])


def render_module(name, w95, a, b, recon):
    if name.upper() not in a:
        sys.exit(f"{name} not in the 95 corpus")
    if name.upper() not in b:
        sys.exit(f"{name} not in the 98 corpus")
    p95, p98 = a[name.upper()], b[name.upper()]
    fmt = target.format_of(p98)
    print(f"# {name}  [{fmt}]   95={os.path.basename(p95)}  98={os.path.basename(p98)}")
    if fmt not in ("NE", "PE"):
        print(f"  format {fmt}: function-level compare not implemented"
              " (NE and PE only). LE/VxD is future work.")
        return
    mm = match_functions(p95, p98)
    cites = recon.get(name.upper(), {}).get("cites", set())
    # a reconstructed function: its carved start is cited (>=1 here; the ledger's
    # >=2-distinct rule is applied there — this flags recon *presence*).
    ident = mm["identical"]
    ident_recon = [t for t in ident if t[0] in cites]
    transfer_bytes = sum(t[2] for t in ident_recon)
    ident_bytes = sum(t[2] for t in ident)
    total98 = ident_bytes + sum(t[1] for t in mm["new"])
    print(f"  functions: 95={mm['n95']}  98={mm['n98']}")
    print(f"  IDENTICAL (byte-match, mask-aware): {len(ident)}  ({ident_bytes} B,"
          f" {100.0*ident_bytes/total98 if total98 else 0:.1f}% of 98 code)"
          f"   CHANGED: {len(mm['changed'])}   NEW-in-98: {len(mm['new'])}")
    if name.upper() in recon:
        print(f"  of the identical, ALREADY RECONSTRUCTED in 95: {len(ident_recon)}"
              f"  ({transfer_bytes} bytes transfer for free)")
        print(f"  95 reconstruction files: {', '.join(recon[name.upper()]['files'])}")
    else:
        print("  (no 95 reconstruction on record for this module; the match count"
              " still shows how much 98 code is unchanged from 95)")
    # show a sample of the transferable functions if reconstructed, else the
    # largest byte-identical functions (the reuse opportunities).
    show = ident_recon if ident_recon else sorted(ident, key=lambda t: -t[2])
    label = ("transferable (reconstructed in 95 AND byte-identical in 98)"
             if ident_recon else "largest byte-identical functions (reuse candidates)")
    if show:
        print(f"\n  {label}:")
        for (k95, k98, sz, nm) in show[:20]:
            moved = "" if k95 == k98 else "  (moved %s->%s)" % (_kfmt(k95), _kfmt(k98))
            print("    %-14s %-24s %5d b%s" % (_kfmt(k95), (nm or "-")[:24], sz, moved))
        if len(show) > 20:
            print("    ... %d more" % (len(show) - 20))


# --------------------------------------------------------------------------
# selftest
# --------------------------------------------------------------------------

def selftest():
    print("carryover selftest\n")
    d98 = target.FILES
    if not os.path.isdir(d98):
        print("  no files/ — run `extract` first; skipping"); return True
    # pick a small NE module and compare it against itself: must be 100%.
    ne_pick = None
    for f in sorted(os.listdir(d98)):
        p = os.path.join(d98, f)
        if os.path.isfile(p) and target.format_of(p) == "NE":
            if 0 < os.path.getsize(p) < 60000 and target.code_bytes(p) > 0:
                ne_pick = p; break
    if not ne_pick:
        print("  no small code-bearing NE module found; skipping"); return True
    print("  NE self-compare:", os.path.basename(ne_pick))
    c = compare_ne(ne_pick, ne_pick)
    mm = match_functions(ne_pick, ne_pick)
    ok = (c["pct"] > 99.99 and mm["n95"] == mm["n98"] == len(mm["identical"])
          and not mm["changed"])
    print("    segment match: %.2f%%   functions: %d, all identical: %s"
          % (c["pct"], mm["n95"], len(mm["identical"]) == mm["n95"]))
    # PE leg: a module must fingerprint-match itself completely (masking is
    # deterministic, so identical == every function).
    pe_pick = None
    for f in sorted(os.listdir(d98)):
        p = os.path.join(d98, f)
        if os.path.isfile(p) and target.format_of(p) == "PE" and 0 < os.path.getsize(p) < 80000:
            pe_pick = p; break
    if pe_pick:
        print("  PE self-compare:", os.path.basename(pe_pick))
        pm = match_functions(pe_pick, pe_pick)
        pe_ok = (pm["n95"] == pm["n98"] == len(pm["identical"]) and not pm["changed"]
                 and not pm["new"])
        print("    functions: %d, all identical: %s" % (pm["n98"], pe_ok))
        ok = ok and pe_ok
    print("\nSELFTEST:", "PASS" if ok else "FAIL")
    return ok


# --------------------------------------------------------------------------
# --port: copy transferable reconstructions
# --------------------------------------------------------------------------

def do_port(dry=True):
    import shutil
    w95 = _need(w95_root(), "Windows 95 project")
    rows, recon, a, b = scan(only_reconstructed=True)
    dest = layout.recon_sub("from95", os.getcwd())
    picks = [r for r in rows if r.get("reconstructed")
             and r["verdict"] in ("FILE-IDENTICAL", "CODE-IDENTICAL")]
    print("# %d reconstructed-in-95 modules are FILE/CODE-IDENTICAL in 98:"
          % len(picks))
    n = 0
    for r in picks:
        files = recon.get(r["name"], {}).get("files", [])
        for fn in files:
            src = os.path.join(w95, "reconstructed", fn)
            print("  %-16s %-16s <- %s" % (r["name"], r["verdict"], fn))
            if not dry and os.path.exists(src):
                os.makedirs(dest, exist_ok=True)
                shutil.copy2(src, os.path.join(dest, fn))
                n += 1
    if dry:
        print("\n(report only) re-run with --port --yes to copy these into"
              " reconstructed/from95/ for review before adoption.")
    else:
        print("\ncopied %d reconstruction files -> %s" % (n, dest))
        print("REVIEW before adopting: a CODE-IDENTICAL verdict means the retail"
              " bytes match mask-aware; re-verify each with verify/segdiff on 98.")


def write_report(rows, recon, a, b):
    """Emit reports/carryover.md — the reconstructed-module carry-over ledger,
    with the free-bytes total (reconstructed in 95 AND byte-identical in 98)."""
    os.makedirs("reports", exist_ok=True)
    path = os.path.join("reports", "carryover.md")
    rec_rows = sorted([r for r in rows if r.get("reconstructed")],
                      key=lambda r: -r["pct"])
    free_bytes = free_mods = 0
    lines = ["# Carry-over ledger — Windows 95 reconstruction → Windows 98",
             "",
             "How much of the existing 95 reconstruction is already byte-exact in",
             "98 (mask-aware). CODE-IDENTICAL modules transfer verbatim; PARTIAL",
             "modules carry their matching functions (see `carryover --module <M>`).",
             "",
             "| module | fmt | verdict | reuse | 95 recon files |",
             "|---|---|---|---:|---|"]
    for r in rec_rows:
        files = ", ".join(recon.get(r["name"], {}).get("files", []))
        lines.append("| %s | %s | %s | %.1f%% | %s |"
                     % (r["name"], r["fmt"], r["verdict"], r["pct"],
                        files[:80] + ("…" if len(files) > 80 else "")))
        if r["verdict"] in ("FILE-IDENTICAL", "CODE-IDENTICAL"):
            free_mods += 1
            free_bytes += target.code_bytes(b[r["name"]])
    lines += ["",
              "## Free carry-over (transfers verbatim)",
              "- **%d modules** are FILE/CODE-IDENTICAL and already reconstructed in 95."
              % free_mods,
              "- **%s code bytes** of 98 are already done by those reconstructions."
              % f"{free_bytes:,}",
              "",
              "Adopt with `carryover --port --yes` (copies the 95 reconstruction",
              "into reconstructed/from95/ for review), then re-gate each with",
              "`verify`/`segdiff` against the 98 bytes before counting it.",
              ""]
    open(path, "w", encoding="utf-8").write("\n".join(lines))
    print("wrote", path, "  (%d free modules, %s free bytes)"
          % (free_mods, f"{free_bytes:,}"))


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--module", help="function-level deep dive on one NE module")
    ap.add_argument("--reconstructed", action="store_true",
                    help="scan only the modules 95 actually reconstructed")
    ap.add_argument("--port", action="store_true",
                    help="list/copy transferable reconstructions for identical modules")
    ap.add_argument("--yes", action="store_true", help="with --port: actually copy")
    ap.add_argument("--report", action="store_true",
                    help="write reports/carryover.md (the carry-over ledger)")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.selftest:
        sys.exit(0 if selftest() else 1)
    if args.port:
        do_port(dry=not args.yes); return
    if args.report:
        rows, recon, a, b = scan(only_reconstructed=True)
        write_report(rows, recon, a, b); return
    if args.module:
        w95 = _need(w95_root(), "Windows 95 project")
        _need(os.path.join(w95, "files"), "Windows 95 files/")
        a, b = index(os.path.join(w95, "files")), index(target.FILES)
        render_module(args.module, w95, a, b, recon_index(w95))
        return
    rows, recon, a, b = scan(only_reconstructed=args.reconstructed)
    render_scan(rows, recon)


if __name__ == "__main__":
    main()
