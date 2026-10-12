#!/usr/bin/env python3
r"""sharedruntime.py -- step 2.1: the canonical shared-runtime source.

origintriage found 1,359 unique bodies statically linked across many modules (41,959 instances) --
the CRT, compiler helpers, and shared statically-linked libraries. Reconstructing each per-module is
wasted work; they must be recovered ONCE into a canonical runtime the whole tree links. This does
that: for every shared body it recovers a single canonical source (from a representative module's
decompile), classifies it (crt / sharedlib / runtime), emits the canonical tree + a manifest, and
flips those bodies to `sourced` in the master ledger -- removing 22.9% of the corpus from the
per-module reconstruction load in one pass.

    wt --target win95 sharedruntime            # build the canonical shared-runtime tree
    wt --target win95 sharedruntime --limit N

Named bodies get their canonical source extracted by name from the representative module's --module
output; unnamed shared bodies (sub_) are cataloged with their representative location for follow-on
extraction. Output: <design>/../base/runtime/{crt,sharedlib,runtime}/ + shared_runtime_manifest.csv;
the master body_ledger.csv is updated in place (shared -> sourced / shared-cataloged).
"""
import os
import re
import csv
import sys
import subprocess

from src.core import paths
from src.targets.win95 import origintriage as OT

# CRT / compiler-runtime name signatures (identify-and-source, never hand-reconstruct).
_CRT = OT._CRT


def _design():
    from src.core import corpus
    d = corpus.design_dir()
    return d if os.path.isdir(d) else None


_MOD_INDEX = None


def _corpus_module(name):
    """Locate a corpus binary by filename (filename->path index built once)."""
    global _MOD_INDEX
    if _MOD_INDEX is None:
        _MOD_INDEX = {}
        for dp, _d, fs in os.walk(OT._corpus_root()):
            for fn in fs:
                _MOD_INDEX.setdefault(fn.lower(), os.path.join(dp, fn))
    return _MOD_INDEX.get(name.lower())


def _module_blocks(path):
    """{addr_int: raised-source-block} from --module. The emitter names functions `sub_<hexaddr>`;
    key each block by that address so it can be matched to a body by structural hash (via triage's
    per-function address), independent of whether the function has a recovered name."""
    lane, exe = OT._detect(path)
    if lane is None:
        return {}
    text = OT._dec(exe, path, "--module")
    if not text:
        return {}
    out = {}
    for m in re.finditer(r"(?m)^([A-Za-z_][\w\* ]*?\b([A-Za-z_]\w*)\s*\([^;{]*\))\s*\{", text):
        name = m.group(2)
        sm = re.match(r"sub_([0-9a-fA-F]+)$", name)
        i = text.find("{", m.end() - 1)
        depth, j = 0, i
        while j < len(text):
            c = text[j]
            if c == "{":
                depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        if depth != 0:
            continue
        block = text[m.start():j + 1]
        if sm:
            out[int(sm.group(1), 16)] = block
        out[name] = block                                # also key by the literal name (named exports)
    return out


def _module_recovery(path):
    """(hash16 -> addr, addr -> source-block) for one module: triage gives each function's structural
    hash + start address, --module gives each function's source keyed by address -- so a shared body
    is recovered by matching its body_id to the hash, then pulling the block at that address."""
    _lane, fs = OT.triage_module(path)
    hash_addr = {}
    if fs:
        for f in fs:
            if f["sh"] and f.get("addr") is not None:
                hash_addr.setdefault(f["sh"][:16], f["addr"])
    return hash_addr, _module_blocks(path)


def _classify(name):
    n = name.strip().lower().lstrip("_").split("@")[0]
    if n in {c.lower().lstrip("_") for c in _CRT}:
        return "crt"
    if name and (name[0].islower() or name[:2] in ("__",)) and not name.isupper():
        return "sharedlib"           # mixed/lower-case named helper (statically-linked library)
    return "sharedlib"


def build(limit=None):
    design = _design()
    if not design:
        return {"error": "design dir not found"}
    catalog = os.path.join(design, "origin_shared_catalog.csv")
    if not os.path.isfile(catalog):
        return {"error": "run origintriage first (no origin_shared_catalog.csv)"}
    rows = list(csv.DictReader(open(catalog)))
    if limit:
        rows = rows[:limit]
    outroot = os.path.join(os.path.dirname(design), "base", "runtime")
    for sub in ("crt", "sharedlib", "runtime"):
        os.makedirs(os.path.join(outroot, sub), exist_ok=True)

    # each shared body carries its representative module in the ledger; recover its ONE canonical
    # source by matching body_id -> structural hash -> address -> --module block (works named+unnamed)
    ledger_path = os.path.join(design, "body_ledger.csv")
    lrows = list(csv.DictReader(open(ledger_path))) if os.path.isfile(ledger_path) else []
    mod_of = {r["body_id"]: r["example_module"] for r in lrows}

    manifest = []
    cache = {}
    recovered = {"crt": 0, "sharedlib": 0}
    named = unnamed = 0
    for r in rows:
        bid, nm = r["struct_hash"], r["example_name"].strip()
        mod = mod_of.get(bid, "")
        origin = _classify(nm) if nm else "runtime"
        if nm:
            named += 1
        else:
            unnamed += 1
        block = None
        if mod:
            if mod not in cache:
                p = _corpus_module(mod)
                cache[mod] = _module_recovery(p) if p else ({}, {})
            hash_addr, blocks = cache[mod]
            addr = hash_addr.get(bid)
            if addr is not None:
                block = blocks.get(addr)
        fname = re.sub(r"\W", "_", nm) if nm else ("body_" + bid)
        target = os.path.join(outroot, origin, fname + ".c")
        with open(target, "w", encoding="utf-8") as f:
            f.write("/* %s -- canonical shared-runtime body (origin: %s).\n"
                    " * Linked into %s modules (%s instances) across Win95 -- recovered ONCE here so\n"
                    " * the whole tree references this, not %s copies. body_id %s; from %s. */\n"
                    % (nm or ("body_" + bid), origin, r["in_modules"], r["instances"],
                       r["instances"], bid, mod or "?"))
            if block:
                f.write(block + "\n")
                if origin in recovered:
                    recovered[origin] += 1
            else:
                f.write("/* TODO: canonical body not extractable from %s; recover (size ~%s instrs). */\n"
                        % (mod or "?", r["max_size"]))
        manifest.append((bid, nm, origin, r["in_modules"], r["instances"],
                         os.path.relpath(target, outroot), bool(block)))

    # manifest
    with open(os.path.join(design, "shared_runtime_manifest.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["body_id", "name", "origin", "in_modules", "instances", "source", "recovered"])
        for row in manifest:
            w.writerow(row)

    # update the master ledger in place: shared -> sourced (named w/ source) / shared-cataloged
    sourced = {bid for bid, _n, _o, _m, _i, _s, ok in manifest if ok}
    if lrows:
        for r in lrows:
            if r["origin"] == "shared-runtime":
                if r["body_id"] in sourced:
                    r["state"] = "sourced"
                    r["language"] = "c"
                else:
                    r["state"] = "shared-cataloged"
        with open(ledger_path, "w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=lrows[0].keys())
            w.writeheader(); w.writerows(lrows)

    return {"outroot": outroot, "named": named, "unnamed": unnamed, "total": len(rows),
            "recovered_crt": recovered["crt"], "recovered_sharedlib": recovered["sharedlib"],
            "sourced": len(sourced), "manifest": os.path.join(design, "shared_runtime_manifest.csv")}


def main(argv):
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    r = build(limit=limit)
    if r.get("error"):
        print("sharedruntime: " + r["error"]); return
    print("=== canonical shared-runtime (step 2.1) ===")
    print("  shared bodies        %d  (%d named + %d unnamed)" % (r["total"], r["named"], r["unnamed"]))
    print("  canonical source      %d recovered  (%d crt + %d sharedlib)" % (
        r["sourced"], r["recovered_crt"], r["recovered_sharedlib"]))
    print("  canonical tree ->    %s" % r["outroot"])
    print("  manifest ->          %s" % r["manifest"])
    print("  master ledger updated: shared bodies -> sourced / shared-cataloged")


if __name__ == "__main__":
    main(sys.argv[1:])
