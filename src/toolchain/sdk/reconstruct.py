#!/usr/bin/env python3
r"""reconstruct.py -- whole-binary, PER-SEGMENT deconstruction + reconstruction.

Deconstructs a 16-bit NE the way the compiler actually laid it out: one code
segment at a time, each segment's functions recovered individually, so the
reconstruction mirrors the shipped segmentation (one <module>_TEXT per source
module, /NOPACKCODE) instead of lumping everything into a single blob.

For each CODE segment it:
  1. finds the function starts (entry-table exports landing in this segment +
     INTERNAL relocation targets + segment offset 0),
  2. bounds each function [start, next_start) and inverts it with msc6dec, giving
     it the RIGHT signature (from the SDK apidb, by export name) and the RIGHT
     import names (from this segment's relocation records),
  3. reports byte-exact / declined(+reason) per function, and can emit one .c per
     segment with recovered bodies and @RECON-UNKNOWN markers for declines.

No fabrication: a function msc6dec can't model is emitted as a stub with the reason,
never a guess; an import whose ordinal we can't name is labelled MODULE.<ord>.
"""
import os, sys, struct
from src.formats import ne
from src.engines.inverse import msc6dec
from src.engines.inverse.msc6fn import PROLOGUE

_EPI = bytes.fromhex("4d4d8be51f5d4d")           # dec bp;dec bp;mov sp,bp;pop ds;pop bp;dec bp
_EPI_LEA = bytes.fromhex("8d66fe1f5d4d")         # lea sp,[bp-2];pop ds;pop bp;dec bp (variant)

# The prologue FAMILY -- MSC-6 lays 16-bit functions with several standard frames; the
# corpus is dominated by the near-loadds form, not lzexpand's far-loadds. Longest first so
# the greedy span-consumer in analyze() takes the most specific match and skips the shorter
# ones that appear as substrings mid-frame.
_PROLOGUES = (
    PROLOGUE,                                     # 1e5890 45 55 8bec 1e 8ed8  far loadds (push ds;pop ax)
    bytes.fromhex("8cd89045558bec1e8ed8"),        # 8cd890 45 55 8bec 1e 8ed8  far loadds (mov ax,ds)
    bytes.fromhex("558bec1e8ed8"),                # 55 8bec 1e 8ed8            near loadds (push bp;mov bp,sp;push ds;mov ds,ax)
)
# Epilogue FAMILY, paired with the frames above. The near-loadds forms return NEAR (c3/c2,
# no dec bp) and restore ds but not the far-frame dec bp.
_EPIS = (
    _EPI, _EPI_LEA,
    bytes.fromhex("8be51f5d"),                    # mov sp,bp;pop ds;pop bp   (near loadds, with locals)
    bytes.fromhex("1f5d"),                        # pop ds;pop bp             (near loadds, no locals)
)

_NEAR_LOADDS = bytes.fromhex("558bec1e8ed8")      # near-loadds prologue (no inc bp)


def asm_origin(fb):
    """A framed function whose FRAME no C source reproduces at the compile oracle -- it belongs
    to the assembly/template path, not the C decompiler. Two shapes dominate the corpus:

      1. NEAR-LOADDS (55 8bec 1e 8ed8, no inc bp): 1.52c's cl emits the 8cd8+inc-bp far frame
         for every C declaration, so this form is a linker segment transform or hand-written asm.
      2. DRIVER SEGMENT-SWITCH THUNK: the far prologue (1e58/8cd8, 10 bytes) immediately followed
         by `mov ax,imm16 ; mov ds,ax` -- a second DS load to a FIXED (non-DGROUP, un-relocated)
         segment, which portable C cannot express. 87% of the 24-48 B bucket, ~938 near-identical
         printer-driver thunks.

    Tagging these keeps them out of msc6dec's C-inversion + the synthesizer, and marks them for
    a structural assembly template instead."""
    fb = bytes(fb)
    if fb.startswith(_NEAR_LOADDS):
        return "near-loadds"
    if len(fb) >= 15 and fb[:2] in (b"\x8c\xd8", b"\x1e\x58") \
            and fb[10] == 0xb8 and fb[13:15] == b"\x8e\xd8":
        return "seg-switch-thunk"
    return None


def _ordinal_names(f):
    """ordinal -> exported name, from the resident + non-resident name tables."""
    out = {}
    for nm, ordn in f.resident_names()[1:] + f.nonresident_names():
        out[ordn] = nm
    return out


def _import_name(f, mods, rec):
    if rec["kind"] == "IMPORTNAME":
        mod = mods[rec["module"]-1] if 0 < rec["module"] <= len(mods) else "?"
        return "%s.%s" % (mod, f.imported_name(rec["name_off"]))
    if rec["kind"] == "IMPORTORD":
        mod = mods[rec["module"]-1] if 0 < rec["module"] <= len(mods) else "?"
        return "%s.%d" % (mod, rec["ordinal"])
    if rec["kind"] == "INTERNAL":
        return "sub_%d_%04x" % (rec.get("seg", 0), rec.get("entry", 0))
    return "OSFIXUP"


def _func_imports(f, seg, relocs, start, end):
    """call-index -> import name, for the far calls inside [start,end)."""
    segb = f.data[seg["file_off"]:seg["file_off"]+seg["length"]]
    calls = []                                   # (offset, name)
    for r in relocs:
        o = r["offset"]
        if start <= o < end and o >= 1 and segb[o-1] == 0x9a:      # operand of a far call
            calls.append((o, _import_name(f, f.module_refs(), r)))
    calls.sort()
    return {i: nm for i, (o, nm) in enumerate(calls)}


def _func_starts(f, segs, seg, segb):
    """sorted unique function-start offsets within `seg`. Sources: the loadds
    prologue scan (ground truth for framed functions), the entry table (exports,
    incl. leaf functions), and INTERNAL relocation targets."""
    starts = set()
    for pro in _PROLOGUES:                       # every framed function starts at a frame prologue
        p = segb.find(pro)
        while p >= 0:                            # (a far frame's tail matches the near pattern too;
            starts.add(p); p = segb.find(pro, p + 1)   # analyze()'s greedy consumer drops the inner hit)
    for e in f.entries():
        if e.get("kind") in ("MOVEABLE", "FIXED") and e.get("seg") == seg["idx"]:
            starts.add(e["offset"])
    for r in f.relocations(seg):
        if r["kind"] == "INTERNAL" and r.get("seg") == seg["idx"]:
            starts.add(r.get("entry", 0))
    return sorted(o for o in starts if 0 <= o < seg["length"])


def _trim(b):
    while b and b[-1] in (0x00, 0x90):
        b = b[:-1]
    return b


def _u16(b, i): return b[i] | (b[i+1] << 8)


def _synth_syms(fb):
    """Synthesize a DGROUP symbol map from a function's own access patterns, so
    msc6dec can recognize long-globals without an external symbol table. A DWORD
    global reveals itself as a hi/lo lane pair (hi == lo+2); everything else touched
    by a direct [off] is a word global. Names are synthetic (gL/gW + offset) and
    MARKED synthetic -- honest placeholders, not invented real names."""
    syms = {}
    n = len(fb)
    i = 0
    while i < n - 6:
        # long test:  a1 <hi> ; 0b 06 <lo>   with hi == lo+2
        if fb[i] == 0xa1 and fb[i+3] == 0x0b and fb[i+4] == 0x06:
            hi = _u16(fb, i+1); lo = _u16(fb, i+5)
            if hi == lo + 2:
                syms[lo] = ("gL_%x" % lo, "long")
        # long store zero:  2b c0 ; a3 <hi> ; a3 <lo>
        if fb[i:i+2] == b"\x2b\xc0" and fb[i+2] == 0xa3 and fb[i+5] == 0xa3:
            lo = _u16(fb, i+6)
            syms.setdefault(lo, ("gL_%x" % lo, "long"))
        i += 1
    # word globals: any c7 06 <off> or ff 36 <off> or a1 <off> not already a long lane
    i = 0
    while i < n - 3:
        off = None
        if fb[i:i+2] == b"\xc7\x06":
            off = _u16(fb, i+2)
        elif fb[i:i+2] == b"\xff\x36":
            off = _u16(fb, i+2)
        if off is not None and off not in syms and (off - 2) not in syms:
            syms.setdefault(off, ("gW_%x" % off, "word"))
        i += 1
    return syms


def _func_end(segb, start, hardstop):
    """Exact end of a framed function: the earliest epilogue in the family that is
    terminated by its own return, scanning from `start`. Recognizes the far-loadds
    teardown and its lea-variant (retf/retf N) plus the near-loadds teardown (ret/ret N);
    a short near pattern that appears mid-body is rejected because it lacks the trailing
    return. Returns the offset just past the return, or None if none is found before
    `hardstop`."""
    best = None                                     # (pos, end) of the earliest epilogue+ret
    for pat in _EPIS:
        p = start
        while True:
            p = segb.find(pat, p)
            if p < 0 or p >= hardstop:
                break
            q = p + len(pat)
            r = segb[q:q+1]
            end = None
            if r in (b"\xca", b"\xc2"):             # retf imm16 / ret imm16
                end = q + 3
            elif r in (b"\xcb", b"\xc3"):           # retf / ret near
                end = q + 1
            if end is not None:                     # a real epilogue is terminated by its ret;
                if best is None or p < best[0]:     # the short near patterns (1f5d) appear mid-body
                    best = (p, end)                 # too, so only a ret-terminated hit counts
                break
            p += 1
    return best[1] if best else None


def analyze(path, apidb=None):
    d = open(path, "rb").read()
    ne_off = struct.unpack_from("<I", d, 0x3c)[0]
    f = ne.NEFile.from_buffer(d, ne_off, path)
    segs = f.segments()
    ordn = _ordinal_names(f)
    ent_by_off = {}                              # (seg,off) -> ordinal (exports)
    for e in f.entries():
        if e.get("kind") in ("MOVEABLE", "FIXED") and "seg" in e:
            ent_by_off[(e["seg"], e["offset"])] = e["ordinal"]
    report = {"module": f.module_name(), "path": path, "segments": []}
    for seg in segs:
        if seg["kind"] != "CODE":
            continue
        segb = d[seg["file_off"]:seg["file_off"]+seg["length"]]
        relocs = f.relocations(seg)
        starts = _func_starts(f, segs, seg, segb)
        try:                                              # call-graph inferred signatures
            from src.toolchain.sdk import callsig
            call_sigs = callsig.infer_segment_sigs(f, seg, segb)
        except Exception:
            call_sigs = {}
        # Bound framed functions GREEDILY by their own epilogue+retf, and skip any
        # start that falls INSIDE an already-consumed function -- that filters the
        # false-positive prologues (the 1e58 byte-pattern appearing mid-body).
        spans = []                                     # (start, end, framed)
        consumed = 0
        for s in starts:
            if s < consumed:
                continue
            if any(segb[s:s+len(pro)] == pro for pro in _PROLOGUES):
                e = _func_end(segb, s, seg["length"])
                if e is None:                          # a prologue with NO epilogue after it is
                    continue                            # spurious trailing bytes -> not a function
                spans.append((s, e, True)); consumed = e
            else:
                e = next((x for x in starts if x > s), seg["length"])
                e = min(e, _func_end(segb, s, seg["length"]) or e)
                if e - s < 6:                           # too small to be a real leaf function
                    continue                            # (a stray pop/retf tail fragment)
                spans.append((s, e, False))
        # name every function in this segment so intra-segment calls resolve to siblings
        nearmap = {}
        for s, e, framed in spans:
            ordv = ent_by_off.get((seg["idx"], s))
            nearmap[s] = (ordn.get(ordv) if ordv is not None else None) or "sub_%x" % s
        funcs = []
        for s, e, framed in spans:
            fb = segb[s:e] if framed else _trim(segb[s:e])
            if not fb:
                continue
            name = None; sig = None
            ordv = ent_by_off.get((seg["idx"], s))
            if ordv is not None:
                name = ordn.get(ordv, "ord%d" % ordv)
                if apidb:
                    sig = apidb.sig(name)
            imports = _func_imports(f, seg, relocs, s, s + len(fb))
            res = {"off": s, "size": len(fb), "name": name, "ordinal": ordv,
                   "framed": framed}
            origin = asm_origin(fb) if framed else None
            if origin:
                # frame no C source reproduces -> the assembly/template path, not the C
                # decompiler. Tag it and skip inversion so the synthesizer wastes no cycles.
                res["status"] = "asm-origin"; res["origin"] = origin
                funcs.append(res); continue
            if framed:
                rt = (sig["ret"] if sig and sig["ret"] in ("int", "long", "void") else "int")
                # SDK signature, else the call-graph-inferred one (full override: it gives EVERY
                # internal fn an exact param layout, which drove OLECLI +8; net-positive corpus-wide
                # even with a rare single-function regression -- verify gates everything).
                params = sig["params"] if sig else call_sigs.get(s)
                syms = _synth_syms(fb)
                try:
                    src, meta = msc6dec.invert(fb, imports=imports, syms=syms,
                                               rtype=rt, params=params,
                                               nearmap=nearmap, func_base=s)
                    if src:
                        # intra-segment calls can only be verified at the whole-DLL link
                        # stage (the linker rewrites the far call to push cs;call near),
                        # so mark them needs-link -- keeps "inverted" == per-function exact.
                        res["c"] = src; res["bytes"] = fb.hex()
                        res["status"] = "needs-link" if meta.get("has_nearcall") else "inverted"
                    else:
                        res["status"] = "declined"; res["reason"] = meta
                except Exception as ex:
                    res["status"] = "error"; res["reason"] = str(ex)
            else:
                res["status"] = "leaf/no-frame"
            funcs.append(res)
        report["segments"].append({
            "idx": seg["idx"], "length": seg["length"], "funcs": funcs,
            "n": len(funcs),
            "inverted": sum(1 for x in funcs if x["status"] == "inverted"),
            "needs_link": sum(1 for x in funcs if x["status"] == "needs-link"),
            "framed": sum(1 for x in funcs if x["framed"])})
    return report


# the recipe variants seen in shipped Win95 DLLs -- different modules used different
# optimization (Ox=speed emits a dummy frame slot; Os=size omits it). Verification
# accepts a function if ANY of these reproduces it, and records which one won.
RECIPES = ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os")


def verify_inverted(rep, recipes=RECIPES):
    """Recompile every inverted function under each candidate recipe and mark it
    byte-exact against the shipped bytes (relocs masked, padding ignored) if any
    recipe reproduces it. Records the winning recipe. Slow (a DOSBox run each)."""
    from src.engines.inverse import msc6_oracle as ora
    from src.engines.inverse.msc6fn import _mask
    from src.toolchain.sdk import mocklink
    n_ok = 0
    for s in rep["segments"]:
        for fn in s["funcs"]:
            st = fn.get("status")
            if st not in ("inverted", "needs-link") or "bytes" not in fn:
                continue
            fn["byte_exact"] = False
            if st == "needs-link":                            # verify via the mock linker
                r = mocklink.verify(bytes.fromhex(fn["bytes"]), fn["c"], recipes)
                if r.get("exact"):
                    fn["byte_exact"] = True; fn["recipe"] = r["recipe"]; fn["via"] = "mock-link"
                    n_ok += 1
                continue
            a = _mask(bytes.fromhex(fn["bytes"])).rstrip(b"\x90\x00")
            for rcp in recipes:
                try:
                    b = _mask(ora.mc6_compile(fn["c"], rcp)).rstrip(b"\x90\x00")
                except Exception as ex:
                    fn["verify_err"] = str(ex); continue
                if a == b:
                    fn["byte_exact"] = True; fn["recipe"] = rcp; n_ok += 1
                    break
    return n_ok


def reason_histogram(rep):
    from collections import Counter
    c = Counter()
    for s in rep["segments"]:
        for fn in s["funcs"]:
            if fn["status"] == "declined":
                c[str(fn.get("reason"))] += 1
            elif fn["status"] in ("leaf/no-frame", "error"):
                c[fn["status"]] += 1
    return c


def sweep(paths):
    """Analyze many NE binaries; return per-DLL coverage rows (no DOSBox verify -- fast).
    `paths` may include directories (walked for *.dll/*.exe/*.drv) or single files."""
    import struct
    files = []
    for p in paths:
        if os.path.isdir(p):
            for dp, _, fns in os.walk(p):
                for fn in fns:
                    if fn.lower().endswith((".dll", ".exe", ".drv")):
                        files.append(os.path.join(dp, fn))
        else:
            files.append(p)
    rows = []
    for p in files:
        try:
            d = open(p, "rb").read()
            if d[:2] != b"MZ":
                continue
            ne_off = struct.unpack_from("<I", d, 0x3c)[0]
            if ne_off + 2 > len(d) or d[ne_off:ne_off+2] != b"NE":
                continue
            rep = analyze(p)
            rows.append({"name": rep["module"], "path": p,
                         "funcs": sum(s["n"] for s in rep["segments"]),
                         "framed": sum(s["framed"] for s in rep["segments"]),
                         "produced": sum(s["inverted"] for s in rep["segments"]),
                         "needs_link": sum(s.get("needs_link", 0) for s in rep["segments"])})
        except Exception as ex:
            rows.append({"name": os.path.basename(p), "path": p, "error": str(ex)[:60]})
    return rows


def print_coverage(rep, reasons=False):
    print("=== %s  (%s) ===" % (rep["module"], os.path.basename(rep["path"])))
    tot = inv = fr = nl = 0
    for s in rep["segments"]:
        tot += s["n"]; inv += s["inverted"]; fr += s["framed"]; nl += s.get("needs_link", 0)
        print("  seg%-2d  %4dB  funcs=%-3d framed=%-3d inverted=%-3d needs-link=%-3d"
              % (s["idx"], s["length"], s["n"], s["framed"], s["inverted"], s.get("needs_link", 0)))
    print("  TOTAL functions=%d  framed=%d  inverted=%d  needs-link=%d  (recovered=%d/%d framed)"
          % (tot, fr, inv, nl, inv + nl, fr))
    if reasons:
        print("  -- framed declines (the remaining work-list) --")
        for why, k in reason_histogram(rep).most_common():
            print("     %3d  %s" % (k, why))


if __name__ == "__main__":
    from src.toolchain.sdk import apidb as _apidb
    from src.toolchain.sdk import w95paths
    binp = sys.argv[1] if len(sys.argv) > 1 else w95paths.input_dll("setup", "inf", "lzexpand.dll")
    hdrs = sys.argv[2:]
    db = _apidb.ApiDb.from_paths(hdrs) if hdrs else None
    print_coverage(analyze(binp, db))
