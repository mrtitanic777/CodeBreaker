#!/usr/bin/env python3
"""
ledger.py — the coverage ledger: how much of Windows 3.1 is reconstructed.

Measures the project instead of estimating it. Design carried over from the
Windows 1.0 ledger, with three changes forced by the target.

  BYTES      for every address the reconstruction CITES (`seg3:0x0a6f`,
             `USER.107`, `@41`) the size of the carved function at that
             address, summed per module. The honest "how much of the binary
             has someone actually looked at" figure, and the headline metric.
  FUNCTIONS  every function carved out of the binaries (the denominator)
             versus the C written against them, split by depth:
               transcribed — a real body, instruction-derived
               partial     — structure + evidence, body marked
                             "[documented partial]"
               prototype   — declared and described, not implemented
  EXPORTS    named vs unnamed entry-table slots. New for 3.1 and the best
             pointer at what is left: 3.1 exports names, so the ordinal->name
             problem that dominated the 1.0 project is gone — but the modules
             still carry hundreds of *unnamed* internal entry points, and
             those are exactly where behavioural reconstruction still applies.

Citations are the reconstruction's own comments, which is what makes this
checkable: a file claiming to reconstruct USER.107 has to name it.

WHAT CHANGED FROM THE 1.0 LEDGER
  * **Tiered.** 4.92 MB of code across 193 modules, ~24x Windows 1.0. One flat
    percentage would read ~0.1% for years and carry no information.
  * **Keyed on file, not module name.** Twelve 3.1 files call themselves
    DISPLAY, six call themselves MOUSE, seven call themselves GRABBER.
  * **Lazy.** Carving runs objdump per segment. Carving all 193 modules on
    every invocation is not usable, so by default only core + system + any
    module a reconstruction actually cites gets carved. Everything else
    contributes its code-byte denominator and a coverage of zero, which is
    true. `--carve all` forces the full sweep.

Usage:
    python ledger.py                  # the full tally
    python ledger.py --gaps USER.EXE  # that module's uncited functions,
                                      # biggest first — the work queue
    python ledger.py --carve all      # carve every tier (slow)
"""
import collections
import glob
import os
import re
import sys

import target
from engine.disasm_engine import trace
from engine.foundation import layout

RECON = layout.written_c_globs()

FNDEF = re.compile(r"^[A-Za-z_][A-Za-z0-9_ \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{", re.M)
PROTO = re.compile(r"^(?:static\s+|extern\s+)?[A-Za-z_][\w \t\*]*\b\w+\s*\([^;{]*\)\s*;\s*$", re.M)

# A reconstruction declares which file it is about. Explicit beats inferred:
# the 1.0 ledger guessed ownership from filename prefixes and that logic was
# fragile even with nine unique module names. With 3.1's duplicates it would
# be wrong outright.
#
# Matches the MODULE: declaration wherever it appears, and does NOT require the
# comment to close (`*/`) right after it. The banners are multi-line —
# `/* MODULE: KRNL386.EXE\n * ...` — so an anchored `.../ */` pattern silently
# matched nothing and left every seg:off citation uncredited (only the
# MODULE.ordinal citations counted). That masked all internal-function work.
OWNER_RX = re.compile(r"MODULE:\s*([A-Za-z0-9_.]+)")


def load_ordinals():
    return trace.load_ordinals("ORDINALS.md")


# A cited address is credited to a function only when the reconstruction cites
# at least this many DISTINCT addresses inside that function's carved span.
# One citation of a function's entry is how the reconstruction NAMES a call
# target (e.g. `[seg1:0x3ca8]` = gcheckfree) — naming a callee is not reading
# it. A function actually read produces many instruction-level citations across
# its body. This threshold is what keeps "named" from inflating into "read".
MIN_CITES_TO_CREDIT = 2


def read_funcs(fns, seen):
    """Carved functions the reconstruction demonstrably READ.

    fns: {(seg, start): size}.  seen: set of every cited (seg, off).
    Returns the subset of fns whose span contains >= MIN_CITES_TO_CREDIT cited
    addresses. Sorting citations per segment keeps this near-linear.
    """
    by_seg = collections.defaultdict(list)
    for (seg, off) in seen:
        by_seg[seg].append(off)
    for seg in by_seg:
        by_seg[seg].sort()
    out = {}
    import bisect
    for (seg, start), size in fns.items():
        offs = by_seg.get(seg)
        if not offs:
            continue
        lo = bisect.bisect_left(offs, start)
        hi = bisect.bisect_left(offs, start + max(size, 1))
        if hi - lo >= MIN_CITES_TO_CREDIT:
            out[(seg, start)] = size
    return out


def conduct_exact():
    """Functions the CONDUCTOR proved byte-exact (reports/conduct/<mod>.json).
    These are stronger than cited — reassembled to the shipped bytes — so they
    are credited directly, bypassing the citation threshold. Returns
    ({module-basename-lower: {(seg, off): size}}, {resolved paths})."""
    import json
    out, paths = {}, set()
    for f in glob.glob(os.path.join(layout.runtime_dir(), "conduct", "*.json")):
        name = os.path.basename(f)[:-5]
        if name.startswith("_"):
            continue
        try:
            led = json.load(open(f))
            path = target.resolve(name)
        except Exception:
            continue
        # PE conductor results carry an "rva" so they credit against carve_pe's
        # ('R', rva) keys; NE results key by (seg, off) as before.
        d = {(("R", v["rva"]) if "rva" in v else (v["seg"], v["off"])): v.get("size", 0)
             for v in led.values()
             if v.get("status") == "exact" and "seg" in v and "off" in v}
        if d:
            out[os.path.basename(path).lower()] = d
            paths.add(path)
    return out, paths


def carve(ne, ordinals):
    """{(seg, off): size} for every function in a module.

    NE-only. `trace.Seg` disassembles 16-bit NE segments; Win95's PE and LE
    modules have no NE segment table, so they are not carved here — they still
    contribute their code-byte denominator (target.code_bytes) with zero
    coverage, which is true until a 32-bit/LE carver exists (PLAN.md). Callers
    guard with target.format_of == 'NE'; this is a belt-and-braces check.
    """
    if not hasattr(ne, "segments"):
        return {}
    out = {}
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            sg = trace.Seg(ne, i, ordinals)
        except Exception:
            continue
        for start, f in sg.funcs.items():
            out[(i, start)] = max(0, f["end"] - start)
    return out


def carve_pe(path):
    """{('R', rva): size} for every carved PE function, keyed by RVA (stable
    across rebasing). Uses pe_trace (capstone). The ('R', ...) pseudo-segment
    lets read_funcs treat PE citations exactly like NE seg:off ones."""
    from engine.format_loaders import pe_trace
    try:
        c = pe_trace.PeCarve(path)
    except Exception:
        return {}
    base = c.pe.image_base
    return {("R", s - base): max(0, f["end"] - f["start"])
            for s, f in c.funcs.items()}


def conduct_readability():
    """Aggregate LIVE vs db across every conductor byte-exact function — the
    'how much is readable code vs opaque db byte-dumps' metric, distinct from
    byte-exact coverage. Global + per tier. What's 'remaining' to make readable
    is the db count. Reads reports/conduct/<mod>.json."""
    import json
    agg = {"total": 0, "live": 0, "db": 0, "tiers": {}}
    for f in glob.glob(os.path.join(layout.runtime_dir(), "conduct", "*.json")):
        name = os.path.basename(f)[:-5]
        if name.startswith("_"):
            continue
        try:
            led = json.load(open(f))
            tier = target.tier_of(target.resolve(name))
        except Exception:
            continue
        t = agg["tiers"].setdefault(tier, {"total": 0, "live": 0, "db": 0})
        for v in led.values():
            if v.get("status") != "exact":
                continue
            for k in ("total", "live", "db"):
                agg[k] += v.get(k, 0)
                t[k] += v.get(k, 0)
    return agg


def carve_le(path):
    """{(obj_index, off): size} for every carved function in an LE module, keyed
    (object, object-relative offset) — the convention the LE conductor records,
    so conduct_exact credits them directly (no rva; LE objects base at 0)."""
    try:
        from le import LEFile
        from le_seg import LeSeg
    except Exception:
        return {}
    out = {}
    try:
        objs = LEFile(path).objects()
    except Exception:
        return {}
    for i, ob in enumerate(objs, 1):
        if not ob["exec"]:
            continue
        try:
            sg = LeSeg(path, obj_index=i)
        except Exception:
            continue
        for start, f in sg.funcs.items():
            out[(i, start)] = max(0, f["end"] - start)
    return out


def export_counts(ne):
    """(named, unnamed) exports. PE: from the export directory; NE below."""
    if hasattr(ne, "sections"):                    # PE
        exp = ne.exports()
        named = sum(1 for _o, n, _r in exp if n)
        return named, len(exp) - named
    """(named, unnamed) REAL entry-table slots.

    Only FIXED and MOVEABLE entries are counted. The entry table also encodes
    gaps in the ordinal space as UNUSED bundles (segment index 0), and ne.py
    faithfully emits one dict per skipped ordinal. Counting those as exports
    inflates the figure wildly — MMSYSTEM.DLL appears to have 1090 unnamed
    entry points when it has 37 KB of code and 25 real ones.
    """
    named = set()
    for tbl in (ne.resident_names(), ne.nonresident_names()):
        for _n, o in tbl:
            if o:
                named.add(o)
    total = set()
    try:
        for e in ne.entries():
            if e.get("ordinal") and e.get("kind") in ("FIXED", "MOVEABLE"):
                total.add(e["ordinal"])
    except Exception:
        pass
    return len(named & total), len(total - named)


def owner_of(path, src):
    """Which module file a reconstruction source is about."""
    m = OWNER_RX.search(src)
    if m:
        try:
            return target.resolve(m.group(1))
        except ValueError:
            return None
    base = os.path.basename(path)[:-2].upper()      # strip .c
    for cand in target.CORE + target.SYSTEM + target.APPS:
        if base.startswith(cand.split(".")[0]):
            return target.resolve(cand)
    return None


def cited_addresses(modules, ordinals):
    """{module_path: {(seg, off)}} named by the reconstruction.

    DELIBERATELY EXCLUDES reconstructed/auto/. Those are massgen.py skeletons:
    mechanically true about calls and arguments, but nobody has read them.
    Counting them here would overstate understanding: a skeleton is not a
    reconstruction, so they get their own line at the bottom of this report
    instead.
    """
    cites = collections.defaultdict(set)
    by_modname = {}
    for p, ne in modules.items():
        by_modname.setdefault(ne.module_name().upper(), p)

    # module keys come from glob (real, lower-case filename); owner_of ->
    # target.resolve echoes the banner's case (e.g. GDI.EXE) on the
    # case-insensitive FS. Canonicalise so seg:off / @ord citations aren't
    # silently dropped by a case mismatch.
    _by_lower = {p.lower(): p for p in modules}

    for pattern in RECON:
        for f in glob.glob(pattern):
            src = open(f, encoding="utf-8", errors="replace").read()
            owner = owner_of(f, src)
            if owner:
                owner = _by_lower.get(owner.lower(), owner)

            # MODULE.ordinal — NE only (find_seg_of_entry reads the NE entry
            # table). PE ordinal citations are covered by the rva: form below.
            for mo in re.finditer(r"\b([A-Z][A-Z0-9_]{2,11})\.(\d+)\b", src):
                p = by_modname.get(mo.group(1))
                if p is None or not hasattr(modules[p], "segments"):
                    continue
                seg, off = trace.find_seg_of_entry(modules[p], int(mo.group(2)))
                if seg is not None:
                    cites[p].add((seg, off))

            if not owner or owner not in modules:
                continue
            mod = modules[owner]
            if hasattr(mod, "sections"):                 # PE owner
                # PE reconstructions cite by RVA (`rva:0x1a2f`), stable across
                # rebasing; carve_pe keys functions the same ('R', rva) way.
                for mo in re.finditer(r"\brva:0x([0-9a-fA-F]{2,8})", src):
                    cites[owner].add(("R", int(mo.group(1), 16)))
            else:                                        # NE owner
                for mo in re.finditer(r"\bseg(\d+):0x([0-9a-fA-F]{2,4})", src):
                    cites[owner].add((int(mo.group(1)), int(mo.group(2), 16)))
                for mo in re.finditer(r"@(\d+)\b", src):
                    seg, off = trace.find_seg_of_entry(mod, int(mo.group(1)))
                    if seg is not None:
                        cites[owner].add((seg, off))
    return cites


def classify_sources(files):
    """(transcribed, partial, prototype) counts across a set of .c files."""
    full = part = proto = 0
    for f in files:
        src = open(f, encoding="utf-8", errors="replace").read()
        for m in FNDEF.finditer(src):
            i = src.index("{", m.start())
            d, j = 0, i
            while j < len(src):
                if src[j] == "{":
                    d += 1
                elif src[j] == "}":
                    d -= 1
                    if d == 0:
                        break
                j += 1
            body = src[i:j + 1]
            code = re.sub(r"/\*.*?\*/", "", body, flags=re.S).strip()
            stmts = [x for x in re.split(r"[;{}]", code) if x.strip()]
            if "documented partial" in body.lower() or len(stmts) <= 1:
                part += 1
            else:
                full += 1
        proto += len(PROTO.findall(src))
    return full, part, proto


def cited_module_paths():
    """Modules any reconstruction refers to — carved even outside core/system."""
    out = set()
    for pattern in RECON:
        for f in glob.glob(pattern):
            src = open(f, encoding="utf-8", errors="replace").read()
            o = owner_of(f, src)
            if o:
                out.add(o)
    return out


# A module is "source-backed" when a reconstruction anchors it to real DDK
# driver/VxD source (reference/ddk) that was byte-verified against the shipped
# binary — i.e. we hold and checked the module's ACTUAL source, not just some
# hand-cited bytes. That is a stronger claim than cited% and the honest way to
# score the driver/VxD layer, whose real source exists in full. Deliberately
# keyed on DDK anchors only: core files cite wow16/sdk as NAMING oracles (not
# rebuildable module source), so they are NOT counted here and cover% stays the
# governing metric for KRNL/GDI/USER.
SRCBACK_RX = re.compile(r"ref ddk:|DDK SOURCE ANCHOR|reference/ddk", re.I)


def source_backed_owners():
    """{module_path} anchored to verified real DDK source (rebuildable)."""
    out = set()
    for pattern in RECON:
        for f in glob.glob(pattern):
            src = open(f, encoding="utf-8", errors="replace").read()
            if SRCBACK_RX.search(src):
                o = owner_of(f, src)
                if o:
                    out.add(o)
    return out


def main():
    argv = sys.argv[1:]
    from engine.foundation import parallel
    jobs = parallel.resolve_jobs(argv)     # -j/--jobs N; strips it from argv
    carve_all = "all" in argv[argv.index("--carve") + 1:argv.index("--carve") + 2] \
        if "--carve" in argv else False

    tiers = target.tiers()
    ordinals = load_ordinals()

    # which modules get the expensive treatment. Some names in target.SYSTEM are
    # not present on this OSR2.5 media (e.g. COMMCTRL.DLL); skip those cleanly.
    # Compare case-insensitively: resolve() echoes the (upper-case) list name on
    # the case-insensitive Windows FS, while tiers()/glob yields the real lower-
    # case filename, so exact string membership would never match.
    want = set()
    for p in target.CORE + target.SYSTEM:
        try:
            want.add(target.resolve(p).lower())
        except ValueError:
            pass
    want |= {c.lower() for c in cited_module_paths()}
    bex, bex_paths = conduct_exact()          # conductor byte-exact functions
    want |= {p.lower() for p in bex_paths}     # ensure those modules get carved
    if carve_all:
        want = set(p.lower() for ps in tiers.values() for p in ps)

    if "--gaps" in argv:
        name = argv[argv.index("--gaps") + 1]
        path = target.resolve(name)
        ne = target.open_module(path)
        fns = carve(ne, ordinals)
        seen = cited_addresses({path: ne}, ordinals).get(path, set())
        read = read_funcs(fns, seen)
        rows = sorted(((sz, seg, off) for (seg, off), sz in fns.items()
                       if (seg, off) not in read), reverse=True)
        print(f"# {os.path.basename(path)}: {len(rows)} functions not yet cited "
              f"by any reconstruction, largest first")
        for sz, seg, off in rows[:40]:
            print(f"  seg{seg:<3}:0x{off:04x}  {sz:6d} B")
        return

    # ---- open + carve ----------------------------------------------------
    # Opening a module (header parse) is cheap; CARVING (linear disasm) is the
    # expensive part and is independent per module. The dominant cost is the PE
    # tier (32 MB through capstone). That carve is GIL-bound Python, so real
    # speedup needs a PROCESS pool, not threads (measured: threads were slower).
    # carve_pe(path)->dict is picklable and needs no shared state, so it fans
    # out cleanly across cores. NE carve is cheap pure-Python; keep it serial.
    import ledger as _self
    wanted = [p for ps in tiers.values() for p in ps
              if p.lower() in want and target.format_of(p) in ("NE", "PE", "LE")]

    opened, carved = {}, {}
    for p in wanted:
        try:
            opened[p] = target.open_module(p)     # cheap; needed for exports/cites
        except Exception:
            pass
    ne_paths = [p for p in opened if target.format_of(p) == "NE"]
    # largest-first: start the long-pole modules early so a single big module
    # can't gate the whole pool at the tail (load balance across workers).
    pe_paths = sorted((p for p in opened if target.format_of(p) == "PE"),
                      key=lambda x: -target.code_bytes(x))
    le_paths = [p for p in wanted if target.format_of(p) == "LE"]   # carve by path
    for p in ne_paths:
        carved[p] = carve(opened[p], ordinals)
    for p, c in zip(pe_paths, parallel.pmap(_self.carve_pe, pe_paths, jobs=jobs,
                                            label="PE carve (%d jobs)" % jobs)):
        carved[p] = c
    for p in le_paths:
        carved[p] = carve_le(p)

    cites = cited_addresses(opened, ordinals)

    show_src = "--source-backed" in argv
    srcback = source_backed_owners() if show_src else set()

    if show_src:
        print("TIER          mods    fns   cited      code B     cited B    cover"
              "    src-bk B   src-bk%")
    else:
        print("TIER          mods    fns   cited      code B     cited B    cover")
    gf = gc = gb = gcb = gsb = 0
    for tname in target.TIER_ORDER:
        paths = tiers[tname]
        tf = tc = tb = tcb = tsb = 0
        for p in paths:
            cb = target.code_bytes(p)
            tb += cb
            if p in srcback:
                tsb += cb
            fns = carved.get(p)
            if fns is None:
                continue
            seen = cites.get(p, set())
            hit = read_funcs(fns, seen)
            hit.update(bex.get(os.path.basename(p).lower(), {}))   # byte-exact: credit directly
            tf += len(fns)
            tc += len(hit)
            tcb += sum(hit.values())
        gf += tf; gc += tc; gb += tb; gcb += tcb; gsb += tsb
        shown = "-" if tf == 0 and not any(p in carved for p in paths) else str(tf)
        row = (f"  {tname:<10} {len(paths):>5} {shown:>6} {tc:>7} {tb:>11,} "
               f"{tcb:>11,} {100*tcb/max(1,tb):>7.2f}%")
        if show_src:
            row += f" {tsb:>11,} {100*tsb/max(1,tb):>7.2f}%"
        print(row)
    total = (f"  {'TOTAL':<10} {sum(len(v) for v in tiers.values()):>5} {gf:>6} {gc:>7} "
             f"{gb:>11,} {gcb:>11,} {100*gcb/max(1,gb):>7.2f}%")
    if show_src:
        total += f" {gsb:>11,} {100*gsb/max(1,gb):>7.2f}%"
    print(total)
    if bex:
        bf = sum(len(d) for d in bex.values())
        bb = sum(sum(d.values()) for d in bex.values())
        print(f"  byte-exact (conductor): {bf} functions, {bb:,} bytes across "
              f"{len(bex)} module(s) - reassemble to the shipped bytes, credited above")
        r = conduct_readability()
        if r["total"]:
            print(f"  readability (live): {r['live']:,} / {r['total']:,} instructions "
                  f"live ({100*r['live']/r['total']:.2f}%), {r['db']:,} db remaining")
            for tname in target.TIER_ORDER:
                tt = r["tiers"].get(tname)
                if tt and tt["total"]:
                    print(f"    {tname:<10} {tt['live']:>8,} / {tt['total']:<8,} live "
                          f"({100*tt['live']/tt['total']:>6.2f}%)  {tt['db']:,} db")
    if not carve_all:
        print(f"  (carved {len(carved)} of {sum(len(v) for v in tiers.values())} "
              f"modules; uncarved modules count their code bytes and zero "
              f"coverage. --carve all for the full sweep)")
    if show_src:
        print(f"  src-bk = code bytes of modules anchored to VERIFIED real DDK source "
              f"(reference/ddk); {len(srcback)} modules. A stronger claim than cited%: "
              f"we hold and byte-checked the module's actual source. cover% stays the "
              f"metric for core (wow16/sdk are naming oracles, not rebuildable source).")

    # ---- exports ---------------------------------------------------------
    print("\nEXPORTS (carved modules)      named   unnamed   "
          "unnamed = the reconstruction frontier")
    tn = tu = 0
    for p in sorted(opened, key=lambda x: -target.code_bytes(x))[:12]:
        n, u = export_counts(opened[p])
        tn += n; tu += u
        print(f"  {os.path.basename(p):<24} {n:>7} {u:>9}")
    for p in list(opened)[12:]:
        n, u = export_counts(opened[p])
        tn += n; tu += u
    print(f"  {'TOTAL':<24} {tn:>7} {tu:>9}")

    # ---- written C -------------------------------------------------------
    if layout.has_tree():                                # component tree: no dir-based
        sysc = classify_sources([])                      # system/apps split; count all
        appc = classify_sources(layout.written_c_files())  # hand-C as one layer
    else:
        sysc = classify_sources(glob.glob(os.path.join(layout.recon_sub("system"), "*.c")))
        appc = classify_sources(glob.glob(os.path.join(layout.recon_dir(), "*.c")))
    print("\nWRITTEN C            transcribed  partial  proto  addressed   of layer")
    for nm, c in (("SYSTEM", sysc), ("APPS", appc)):
        a = sum(c)
        print(f"  {nm:9s} {c[0]:14d} {c[1]:8d} {c[2]:6d} {a:10d} "
              f"{100*a/max(1,gf):9.1f}%")
    a = sum(sysc) + sum(appc)
    print(f"  {'TOTAL':9s} {sysc[0]+appc[0]:14d} {sysc[1]+appc[1]:8d} "
          f"{sysc[2]+appc[2]:6d} {a:10d} {100*a/max(1,gf):9.1f}%")

    # NOTE: the W3/VxD byte view (VMM32.VXD LE submodules) is a future section —
    # it needs an LE loader + W4 decompressor (PLAN.md Phase 3). Until then the
    # 222 VxDs contribute no denominator and appear nowhere here; say so when
    # quoting any coverage % (CONTEXT.md denominator honesty).

    auto_coverage()


def auto_coverage():
    """Report the massgen.py skeletons SEPARATELY, and never as progress."""
    tot = read = 0
    for f in layout.generated_c_files():
        src = open(f, encoding="utf-8", errors="replace").read()
        # anchored to line start: the marker also appears in each function's
        # `return` comment, which would double the count
        tot += len(re.findall(r"^/\* \[AUTO\] ", src, re.M))
        read += len(re.findall(r"^/\* (?!\[AUTO\])\w+  \w+ seg", src, re.M))
    if tot:
        print(f"\nAUTO skeletons (massgen.py): {tot} generated, {read} promoted "
              f"by a human")
        print("  These are NOT counted as reconstructed. They carry the binary's"
              " true call")
        print("  graph, argument counts and global usage — everything except"
              " understanding.")


if __name__ == "__main__":
    main()
