#!/usr/bin/env python3
r"""origintriage.py -- corpus-wide ORIGIN TRIAGE + cross-module dedup for the reconstruction roadmap.

"Map every body correctly" is only scoped once we know how many bodies are GENUINE reconstruction
targets (OS/app-specific code) versus code that should be IDENTIFIED, not reconstructed: the CRT,
compiler helpers, flat-thunk machinery, and runtime/library code statically linked into many modules
(reconstruct it once, not 300x). This pass triages the whole PE corpus by origin and collapses
duplicates, turning "all of Windows 95" into a real, counted work-list.

Signals (read-only; nothing is modified -- one ledger is written):
  * every function's start comes from the native decompiler's `--names`;
  * a STRUCTURAL body hash (opcode + instruction-length sequence, operands masked so link-time
    addresses don't matter) finds the same function statically linked across modules;
  * a body that appears in many modules is shared runtime/library -> identify, don't reconstruct;
  * names matching the CRT/compiler-helper set, and thunk-machinery imports, are tagged too.

    wt --target win95 origintriage [--limit N] [--out FILE]

Writes design/origin_triage_pe.csv (per-module counts) and prints the corpus summary + the genuine
unique-target count -- the number the full-reconstruction roadmap is actually sized against.
"""
import os
import sys
import csv
import glob
import hashlib
import subprocess

from src.core import paths
from src.targets.win95 import autobuild95 as A

# CRT / C++ runtime / compiler-helper names (identify-and-source, never reconstruct). Matched
# case-insensitively as whole names or with the usual leading underscores/@decoration stripped.
_CRT = {
    "memcpy", "memmove", "memset", "memcmp", "strlen", "strcpy", "strncpy", "strcat", "strncat",
    "strcmp", "strncmp", "strchr", "strrchr", "strstr", "strtok", "sprintf", "vsprintf", "sscanf",
    "malloc", "calloc", "realloc", "free", "new", "delete", "_msize", "_expand", "_heapchk",
    "printf", "fprintf", "fwrite", "fread", "fopen", "fclose", "atoi", "atol", "strtol", "qsort",
    "bsearch", "abs", "labs", "div", "ldiv", "rand", "srand", "wcslen", "wcscpy", "wcscmp",
    "__chkstk", "_chkstk", "_alloca_probe", "__alloca_probe", "_ftol", "__ftol", "_ftol2",
    "_aulldiv", "_alldiv", "_aullrem", "_allrem", "_aullshr", "_allshl", "_aullshl", "_allmul",
    "__cxxframehandler", "_cxxthrowexception", "__ehprolog", "_eh_prolog", "_eh_epilog",
    "_setargv", "_amsg_exit", "_cinit", "__crtgetenvironmentstringsa", "_initterm", "_onexit",
    "_dllmaincrtstartup", "_wincrtstartup", "maincrtstartup", "__security_init_cookie",
    "_purecall", "__local_unwind", "__global_unwind", "_except_handler3", "_except_handler2",
    "_controlfp", "_fpreset", "atexit", "exit", "_exit", "abort", "signal", "longjmp", "setjmp",
}
_THUNK = ("SMapLS", "SUnMapLS", "ThunkConnect32", "FT_Prolog", "QT_Thunk", "ThunkInitLSF",
          "FT_Thunk", "C16ThkSL", "ThunkData")
_DUP_MODULES = 5              # a body in >= this many modules is shared runtime/library


def _canon(name):
    n = name.strip().lower()
    if n.startswith("_"):
        n = n.lstrip("_")
    n = n.split("@")[0]
    return n


def _decompiler_for(path):
    b = A.load(path)
    if isinstance(b, A.PE):
        return "win95pedec.exe"
    return None                                          # PE-only for this pass (flat address space)


def _func_starts(exe, path):
    """(rva, name) for every function, from the native decompiler's --names."""
    dec = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin", exe)
    if not os.path.exists(dec):
        return None
    try:
        r = subprocess.run([dec, path, "--names"], capture_output=True, text=True, timeout=180)
    except Exception:
        return None
    out = []
    for line in r.stdout.splitlines():
        parts = line.split("\t")
        try:
            rva = int(parts[0].strip(), 16)
        except ValueError:
            continue
        name = parts[1].strip() if len(parts) > 1 else ""
        out.append((rva, name))
    return out


def _struct_hash(pe, lo, hi):
    """Address-independent structural hash of a function body [lo,hi): the sequence of opcode bytes
    + instruction lengths, with all operand bytes dropped. Two copies of the same function linked at
    different addresses hash identically -- so statically-linked shared code is detectable."""
    d = pe.read_at(lo, hi - lo)
    if not d:
        return None
    h = hashlib.sha1()
    i, n = 0, len(d)
    count = 0
    while i < n:
        ln = A._ilen(d, i)
        if ln <= 0:
            h.update(b"\xff" + d[i:i + 1])               # undecoded byte: record it, advance one
            i += 1
            count += 1
            continue
        op = d[i:i + 2] if d[i] == 0x0F else d[i:i + 1]  # opcode (1-2 bytes), operands dropped
        h.update(op + bytes([ln & 0xFF]))
        i += ln
        count += 1
    return (h.hexdigest(), count)


def triage_module(path, exports_set):
    """-> list of {name, size, origin, sh} for each function in one PE, plus its export names."""
    pe = A.PE(path)
    if not pe.is_pe:
        return None
    raw = _func_starts("win95pedec.exe", path)
    if not raw:
        return None
    # section VA ranges (a function's body ends at the next function start, capped at its section end)
    secs = [(va, va + max(vs, rs)) for nm, va, vs, rs, pr in pe.secs if max(vs, rs) > 0]

    def sec_end(r):
        return next((b for a, b in secs if a <= r < b), None)
    name_of = {rva: nm for rva, nm in raw}
    starts = sorted({rva for rva, _n in raw if sec_end(rva) is not None})
    funcs = []
    for i, lo in enumerate(starts):
        se = sec_end(lo)
        nxt = starts[i + 1] if i + 1 < len(starts) else se
        hi = min(nxt, se) if (nxt and se) else (nxt or se or lo)
        if hi <= lo or hi - lo > 1 << 16:
            hi = min(lo + 4096, se or (lo + 4096))
        sh = _struct_hash(pe, lo, hi)
        funcs.append({"rva": lo, "name": name_of.get(lo, ""), "size": hi - lo,
                      "sh": sh[0] if sh else None})
    return funcs


def _corpus_root():
    """The Win95 binary corpus (…/Windows95/design/corpus/files). wt.py repoints WT_DATA_ROOT at its
    own win95 data dir, so resolve the corpus canonically from the sibling repo unless it holds PEs."""
    cands = [os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design", "corpus", "files"),
             r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files"]
    for c in cands:
        if os.path.isdir(c):
            return c
    return cands[0]


def run(limit=None, out=None, root=None):
    root = root or _corpus_root()
    # discover PE binaries
    bins = []
    seen = set()
    for dp, _d, fs in os.walk(root):
        for fn in fs:
            if not fn.lower().endswith((".dll", ".exe", ".drv", ".ocx", ".cpl")) or fn.lower() in seen:
                continue
            p = os.path.join(dp, fn)
            b = A.load(p)
            if isinstance(b, A.PE):
                seen.add(fn.lower()); bins.append((fn, p))
    if limit:
        bins = bins[:limit]

    # SDK-documented export names (genuine but spec'd) -- from both proto tables
    protos = set()
    for w16 in (False, True):
        protos |= set(A._load_sdk_protos(win16=w16).keys())

    per_module = []
    body_modules = {}          # struct-hash -> set(modules)
    body_meta = {}             # struct-hash -> [func_count, a representative name, max size]
    all_funcs = []             # (module, name, size, sh)
    for fn, p in bins:
        fs = triage_module(p, None)
        if fs is None:
            continue
        exps = {n for _o, n in A.PE(p).exports()}
        for f in fs:
            all_funcs.append((fn, f["name"], f["size"], f["sh"], f["name"] in exps))
            if f["sh"]:
                body_modules.setdefault(f["sh"], set()).add(fn)
                m = body_meta.setdefault(f["sh"], [0, "", 0])
                m[0] += 1
                if f["name"] and not m[1]:
                    m[1] = f["name"]
                m[2] = max(m[2], f["size"])
        per_module.append((fn, len(fs)))

    # classify
    buckets = {"shared-runtime": 0, "crt": 0, "thunk": 0, "documented-api": 0, "genuine": 0}
    genuine_bodies = set()
    for mod, name, size, sh, is_exp in all_funcs:
        cn = _canon(name) if name else ""
        nmods = len(body_modules.get(sh, ())) if sh else 1
        if sh and nmods >= _DUP_MODULES:
            buckets["shared-runtime"] += 1
        elif cn in _CRT:
            buckets["crt"] += 1
        elif name and any(t.lower() in name.lower() for t in _THUNK):
            buckets["thunk"] += 1
        elif is_exp and name in protos:
            buckets["documented-api"] += 1
            genuine_bodies.add(sh or (mod + name))       # documented is still a genuine body to build
        else:
            buckets["genuine"] += 1
            genuine_bodies.add(sh or (mod + name))

    total = len(all_funcs)
    unique_bodies = len({sh for _m, _n, _s, sh, _e in all_funcs if sh})
    if not out:
        # root is .../design/corpus/files -> write the ledger up in .../design
        design = os.path.dirname(os.path.dirname(root))
        out = os.path.join(design, "origin_triage_pe.csv")
    with open(out, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["module", "functions"])
        for mod, nf in sorted(per_module):
            w.writerow([mod, nf])
    # shared-body catalog: the identify-once list -- every body linked into >= _DUP_MODULES modules,
    # ranked by reach (module count x size = the reconstruction effort SAVED by not repeating it).
    catalog = os.path.join(os.path.dirname(out), "origin_shared_catalog.csv")
    shared = [(sh, len(mods), body_meta[sh][0], body_meta[sh][1], body_meta[sh][2])
              for sh, mods in body_modules.items() if len(mods) >= _DUP_MODULES]
    shared.sort(key=lambda r: (r[1], r[4]), reverse=True)
    with open(catalog, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["struct_hash", "in_modules", "instances", "example_name", "max_size"])
        for sh, nmods, insts, nm, sz in shared:
            w.writerow([sh[:16], nmods, insts, nm, sz])
    return {"modules": len(per_module), "total_functions": total, "unique_bodies": unique_bodies,
            "buckets": buckets, "genuine_unique_targets": len(genuine_bodies), "out": out,
            "catalog": catalog, "n_shared_bodies": len(shared)}


def main(argv):
    limit = None
    if "--limit" in argv:
        limit = int(argv[argv.index("--limit") + 1])
    out = None
    if "--out" in argv:
        out = argv[argv.index("--out") + 1]
    r = run(limit=limit, out=out)
    print("=== origin triage (PE corpus) ===")
    print("  modules analysed        %d" % r["modules"])
    print("  total functions         %d" % r["total_functions"])
    print("  unique bodies (dedup)   %d   (%.1f%% collapse)" % (
        r["unique_bodies"], 100.0 * (1 - r["unique_bodies"] / max(r["total_functions"], 1))))
    print("  --- by origin ---")
    for k in ("shared-runtime", "crt", "thunk", "documented-api", "genuine"):
        v = r["buckets"][k]
        print("    %-16s %8d  (%.1f%%)" % (k, v, 100.0 * v / max(r["total_functions"], 1)))
    print("  GENUINE UNIQUE TARGETS  %d   <- the real reconstruction work-list" %
          r["genuine_unique_targets"])
    print("  shared-body catalog     %d bodies (identify once) -> %s" %
          (r.get("n_shared_bodies", 0), r.get("catalog", "")))
    print("  ledger -> %s" % r["out"])


if __name__ == "__main__":
    main(sys.argv[1:])
