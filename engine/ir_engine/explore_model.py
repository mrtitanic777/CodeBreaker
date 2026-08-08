#!/usr/bin/env python3
r"""
explore_model.py — one unified per-module ANALYSIS MODEL for the explorer.

The parsers each expose a slice (pe.py sections/exports/imports/relocs,
pe_trace/trace/le_seg the carved functions + call edges, the conduct ledger the
byte-exact live/db verdicts). This façade aggregates them into ONE dict per
module so the explorer (and whole-file linking) has a single object to read:

    m = analyze_module("NOTEPAD.EXE")
    m["functions"][0]  -> {addr,end,size,name,status,live,total,db,section,callees}

Everything here is EVIDENCE straight from the binary + our own byte-exact ledger.
The only inference is `strings` (a literal scan) — labelled, never a name source.

    python explore_model.py NOTEPAD.EXE          # dump a compact model summary
    python explore_model.py SYSTEM.DRV --json     # full model as JSON
"""
import json
from engine.foundation import cores
import os
import re
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
LEDGER_DIR = os.path.join("reports", "conduct")


# --------------------------------------------------------------------------
# strings — the one genuinely new backend piece (ASCII + UTF-16LE runs)
# --------------------------------------------------------------------------
_ASCII = re.compile(rb"[\x20-\x7e]{4,}")
_UTF16 = re.compile(rb"(?:[\x20-\x7e]\x00){4,}")


def extract_strings(data, base=0, min_len=4, limit=4000):
    """Printable ASCII and UTF-16LE literal runs in `data`. Returns
    [{addr, text, enc}] — an inference layer (a literal scan), never a name source."""
    out = []
    for m in _ASCII.finditer(data):
        s = m.group()
        if len(s) >= min_len:
            out.append({"addr": base + m.start(), "enc": "ascii",
                        "text": s.decode("latin1")})
            if len(out) >= limit:
                return out
    for m in _UTF16.finditer(data):
        try:
            txt = m.group().decode("utf-16-le")
        except Exception:
            continue
        out.append({"addr": base + m.start(), "enc": "utf16", "text": txt})
        if len(out) >= limit:
            break
    out.sort(key=lambda s: s["addr"])
    return out


# --------------------------------------------------------------------------
# ledger merge — attach each function's byte-exact verdict (status/live/db)
# --------------------------------------------------------------------------
def _load_ledger(module_basename):
    p = os.path.join(LEDGER_DIR, module_basename.lower() + ".json")
    try:
        return json.load(open(p))
    except Exception:
        return {}


def _ledger_index(ledger):
    """Two lookups: by RVA (PE records carry 'rva') and by 'seg:off' key."""
    by_rva, by_key = {}, {}
    for k, v in ledger.items():
        if not isinstance(v, dict):
            continue
        by_key[k] = v
        if "rva" in v:
            by_rva[v["rva"]] = v
    return by_rva, by_key


def _verdict(rec):
    if not rec:
        return {"status": "not-reconstructed", "live": 0, "total": 0, "db": 0}
    return {"status": rec.get("status", "?"), "live": rec.get("live", 0),
            "total": rec.get("total", 0), "db": rec.get("db", 0)}


# --------------------------------------------------------------------------
# per-format builders
# --------------------------------------------------------------------------
def _pe_model(path, base_name):
    from engine.format_loaders import pe
    from engine.format_loaders.pe_trace import PeCarve
    pf = pe.open_pe(path)
    ib = getattr(pf, "image_base", 0)
    model = {"format": "PE", "image_base": ib,
             "module_name": _safe(lambda: pf.module_name(), base_name),
             "entry": {"addr": ib + pf.entry, "kind": "entrypoint"}}
    model["sections"] = [
        {"name": s["name"], "addr": ib + s["vaddr"], "vsize": s["vsize"],
         "rawsize": s["rawsize"], "kind": _pe_kind(s["chars"])}
        for s in pf.sections]
    model["exports"] = [{"ordinal": o, "name": n, "addr": ib + rva}
                        for (o, n, rva) in _safe(lambda: pf.exports(), [])]
    imp = _safe(lambda: pf.imports(), {})
    model["imports"] = [{"site": site, "dll": dll, "name": nm}
                        for site, (dll, nm) in sorted(imp.items())]
    relocs = _safe(lambda: pf.base_relocs(), set())
    model["relocations"] = {"count": len(relocs), "kind": "HIGHLOW"}
    # functions + call edges via the PE carve (image-VA keyed; takes a PATH)
    carve = _safe(lambda: PeCarve(path), None)
    funcs = []
    edges = {}
    if carve is not None:
        by_rva, _bk = _ledger_index(_load_ledger(base_name))
        for start in sorted(carve.funcs):
            f = carve.funcs[start]
            rec = by_rva.get(start - ib)
            callees = _safe(lambda: list(dict.fromkeys(carve.calls_in(start))), [])
            edges[start] = callees
            sec = _sec_of(model["sections"], start)
            funcs.append({"addr": start, "end": f["end"], "size": f["end"] - f["start"],
                          "name": f.get("name") or ("sub_%X" % start),
                          "section": sec, "off": start - _sec_addr(model["sections"], sec, start),
                          **_verdict(rec), "callees": callees})
    model["functions"] = funcs
    model["callgraph"] = _callers_from_edges(edges, {f["addr"]: f["name"] for f in funcs})
    model["strings"] = _pe_strings(pf, ib)
    return model


def _pe_strings(pf, ib):
    out = []
    for s in pf.sections:
        if _pe_kind(s["chars"]) == "data":
            raw = _safe(lambda: pf.section_bytes(s) if hasattr(pf, "section_bytes") else None, None)
            if raw is None:
                raw = _read_raw(pf, s)
            if raw:
                out += extract_strings(raw, ib + s["vaddr"])
    return out[:4000]


def _read_raw(pf, s):
    try:
        data = pf.data if hasattr(pf, "data") else open(pf.path, "rb").read()
        return data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
    except Exception:
        return b""


def _pe_kind(chars):
    return "code" if (chars & 0x20) or (chars & 0x20000000) else "data"


def _ne_model(path, base_name):
    from engine.format_loaders import ne
    trace = cores.get('trace')
    nf = ne.open_ne(path) if hasattr(ne, "open_ne") else ne.NEFile(path)
    ordinals = _safe(lambda: trace.load_ordinals("ORDINALS.md"), {})
    model = {"format": "NE", "image_base": 0,
             "module_name": _safe(lambda: nf.resident_names()[0][0], base_name),
             "entry": {"addr": _safe(lambda: nf.h.get("csip", 0), 0), "kind": "csip"}}
    segs = _safe(lambda: nf.segments(), [])
    model["sections"] = [
        {"name": "seg%d" % s["idx"], "addr": s["idx"], "vsize": s["length"],
         "rawsize": s["length"], "kind": s.get("kind", "?").lower()} for s in segs]
    model["exports"] = [{"ordinal": ordn, "name": nm, "addr": None}
                        for (nm, ordn) in _safe(lambda: nf.resident_names(), [])[1:]]
    model["imports"] = [{"site": None, "dll": d, "name": None}
                        for d in _safe(lambda: nf.module_refs(), [])]
    model["relocations"] = {"count": sum(len(_safe(lambda: nf.relocations(s["idx"]), []))
                                         for s in segs), "kind": "NE-fixup"}
    funcs, edges = [], {}
    _byr, by_key = _ledger_index(_load_ledger(base_name))
    for s in segs:
        if s.get("kind") != "CODE":
            continue
        sg = _safe(lambda: trace.Seg(nf, s["idx"], ordinals), None)
        if sg is None:
            continue
        for start in sorted(sg.funcs):
            f = sg.funcs[start]
            rec = by_key.get("%d:%04x" % (s["idx"], start))
            callees = [c[1] if isinstance(c, tuple) else str(c)
                       for c in _safe(lambda: sg.calls_in(start), [])]
            key = (s["idx"], start)
            edges[key] = callees
            funcs.append({"addr": start, "seg": s["idx"], "end": f["end"],
                          "size": f["end"] - f["start"], "off": start,
                          "name": f.get("name") or ("sub_%d_%04X" % (s["idx"], start)),
                          "section": "seg%d" % s["idx"], **_verdict(rec),
                          "callees": [str(c) for c in callees]})
    model["functions"] = funcs
    model["callgraph"] = {}
    model["strings"] = _ne_strings(path, segs)
    return model


def _ne_strings(path, segs):
    """Read each NE DATA segment's raw bytes straight from the file and scan it."""
    out = []
    try:
        data = open(path, "rb").read()
    except Exception:
        return out
    for s in segs:
        if s.get("kind") == "DATA" and s.get("file_off") and s.get("length"):
            raw = data[s["file_off"]:s["file_off"] + s["length"]]
            if raw:
                out += extract_strings(raw, 0)
    return out[:4000]


def _le_model(path, base_name):
    le = cores.get('le')
    lf = le.LEFile(path)
    model = {"format": "LE", "image_base": 0,
             "module_name": _safe(lambda: lf.module_name(), base_name),
             "entry": {"addr": _safe(lambda: lf.eip, 0), "kind": "eip"}}
    objs = _safe(lambda: lf.objects(), [])
    model["sections"] = [
        {"name": "obj%d" % i, "addr": o.get("base", 0), "vsize": o.get("vsize", 0),
         "rawsize": o.get("vsize", 0), "kind": "code" if o.get("exec") else "data"}
        for i, o in enumerate(objs, 1)]
    model["exports"] = []
    model["imports"] = []
    model["relocations"] = {"count": 0, "kind": "LE-fixup (not extracted)"}
    funcs = []
    _byr, by_key = _ledger_index(_load_ledger(base_name))
    try:
        from engine.format_loaders.le_seg import LeSeg
        for i, o in enumerate(objs, 1):
            if not o.get("exec"):
                continue
            sg = _safe(lambda i=i: LeSeg(path, obj_index=i), None)
            if sg is None:
                continue
            for start in sorted(getattr(sg, "funcs", {})):
                f = sg.funcs[start]
                rec = by_key.get("%d:%04x" % (i, start))
                funcs.append({"addr": start, "seg": i, "end": f["end"],
                              "size": f["end"] - f["start"], "off": start,
                              "name": f.get("name") or ("sub_%d_%04X" % (i, start)),
                              "section": "obj%d" % i, **_verdict(rec), "callees": []})
    except Exception:
        pass
    model["functions"] = funcs
    model["callgraph"] = {}
    model["strings"] = extract_strings(_safe(lambda: lf.code_bytes and b"", b""), 0)
    return model


# --------------------------------------------------------------------------
# helpers
# --------------------------------------------------------------------------
def _safe(fn, default):
    try:
        return fn()
    except Exception:
        return default


def _sec_of(sections, addr):
    for s in sections:
        if s["addr"] <= addr < s["addr"] + max(s["vsize"], s["rawsize"]):
            return s["name"]
    return "?"


def _sec_addr(sections, name, addr):
    """Base address of the named section (for within-section offset of the .asm)."""
    for s in sections:
        if s["name"] == name:
            return s["addr"]
    return addr - addr        # 0: fall back to treating addr as the offset


def _callers_from_edges(edges, name_by_addr):
    """Invert callees -> callers, resolving local sub_ targets back to addresses."""
    callers = {a: [] for a in edges}
    name_to_addr = {n: a for a, n in name_by_addr.items()}
    for a, callees in edges.items():
        for c in callees:
            ta = name_to_addr.get(c)
            if ta is not None and ta in callers and a not in callers[ta]:
                callers[ta].append(a)
    return {a: {"callees": edges.get(a, []),
                "callers": callers.get(a, [])} for a in edges}


def analyze_module(name):
    """Unified per-module analysis model. Dispatch by format."""
    target = cores.get('target')
    path = target.resolve(name)
    fmt = target.format_of(path)
    base = os.path.basename(path)
    if fmt == "PE":
        m = _pe_model(path, base)
    elif fmt == "NE":
        m = _ne_model(path, base)
    elif fmt in ("LE", "LX"):
        m = _le_model(path, base)
    else:
        raise ValueError("unsupported format %s for %s" % (fmt, base))
    m["name"] = base
    m["path"] = path
    m["n_functions"] = len(m.get("functions", []))
    fs = m.get("functions", [])
    m["n_exact"] = sum(1 for f in fs if f.get("status") == "exact")
    m["n_strings"] = len(m.get("strings", []))
    return m


def _summary(m):
    print("=" * 60)
    print("%s  [%s]  module=%s" % (m["name"], m["format"], m.get("module_name")))
    print("=" * 60)
    print("entry:        0x%X (%s)" % (m["entry"]["addr"], m["entry"]["kind"]))
    print("sections:     %d" % len(m["sections"]))
    for s in m["sections"][:12]:
        print("   %-10s 0x%-8X vsize=%-7d %s" % (s["name"], s["addr"], s["vsize"], s["kind"]))
    print("functions:    %d  (%d byte-exact)" % (m["n_functions"], m["n_exact"]))
    print("exports:      %d" % len(m["exports"]))
    print("imports:      %d" % len(m["imports"]))
    print("relocations:  %d (%s)" % (m["relocations"]["count"], m["relocations"]["kind"]))
    print("strings:      %d" % m["n_strings"])
    for f in m["functions"][:8]:
        print("   fn 0x%-8X %-28s %-16s %d/%d live  %d callees"
              % (f["addr"], f["name"][:28], f["status"], f["live"], f["total"],
                 len(f.get("callees", []))))
    for s in m["strings"][:6]:
        print("   str 0x%-8X %-6s %r" % (s["addr"], s["enc"], s["text"][:50]))


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: explore_model.py MODULE [--json]")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    sys.path.append(ROOT)
    m = analyze_module(sys.argv[1])
    if "--json" in sys.argv:
        print(json.dumps(m, indent=1, default=str))
    else:
        _summary(m)


if __name__ == "__main__":
    main()
