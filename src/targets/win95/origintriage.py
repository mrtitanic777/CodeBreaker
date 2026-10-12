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
import re
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


import struct as _struct

# format -> native decompiler (one engine family, four loaders). Same --names/--disasm interface.
_DECOMP = {"pe": "win95pedec.exe", "ne": "win95dec.exe", "le": "win95ledec.exe", "dos": "win95mzdec.exe"}


def _detect(path):
    """(lane, decompiler_exe) for a binary by its header -- PE/NE/LE(VxD)/DOS -- or (None, None)."""
    from src.formats import detect
    lane = detect.lane(path)
    return (lane, _DECOMP[lane]) if lane else (None, None)


def _dec_cache_path(dec, path, mode):
    """Where the cached output of `dec path mode` lives: keyed by the decompiler executable
    (size + mtime), the input file (size + mtime) and the mode."""
    import hashlib
    import tempfile
    try:
        a, b = os.stat(dec), os.stat(path)
    except OSError:
        return None
    key = hashlib.sha1(("%s|%d|%d|%s|%d|%d|%s" % (
        os.path.basename(dec), a.st_size, int(a.st_mtime), os.path.abspath(path).lower(),
        b.st_size, int(b.st_mtime), mode)).encode()).hexdigest()
    return os.path.join(tempfile.gettempdir(), "codebreaker", "dec", key[:2], key + ".txt")


def _dec(exe, path, mode):
    """Output of the native decompiler in whole-module `mode` (--module, --names, --disasm).
    Deterministic for a given binary and decompiler build, and re-run by every worker process of
    a sweep (once per chunk of a big module), so it is cached on disk. CB_NO_DEC_CACHE=1 skips
    the cache."""
    from src.core import toolchain
    dec = toolchain.decompiler(exe)
    if not dec:
        return None
    cp = None if os.environ.get("CB_NO_DEC_CACHE") else _dec_cache_path(dec, path, mode)
    if cp and os.path.isfile(cp):
        try:
            with open(cp, encoding="utf-8", errors="replace") as f:
                return f.read()
        except OSError:
            pass
    try:
        r = subprocess.run([dec, path, mode], capture_output=True, text=True, timeout=240)
    except Exception:
        return None
    if cp and r.returncode == 0 and r.stdout:
        try:
            os.makedirs(os.path.dirname(cp), exist_ok=True)
            tmp = "%s.%d.tmp" % (cp, os.getpid())
            with open(tmp, "w", encoding="utf-8", errors="replace") as f:
                f.write(r.stdout)
            os.replace(tmp, cp)
        except OSError:
            pass
    return r.stdout


def _addr_key(tok):
    """Normalise an address token to one integer key across formats: 'SSSS:OOOO' (NE/LE seg:off) ->
    (seg<<16)|off; a flat hex 'RVA' -> its int. So --names starts and --disasm lines share a key."""
    tok = tok.strip()
    if ":" in tok:
        s, o = tok.split(":", 1)
        try:
            return (int(s, 16) << 16) | int(o, 16)
        except ValueError:
            return None
    try:
        return int(tok, 16)
    except ValueError:
        return None


_INS_RE = re.compile(r"^\s*([0-9A-Fa-f]{4}:[0-9A-Fa-f]+|[0-9A-Fa-f]+)\s+[0-9A-Fa-f]{2,}\s+(.*)$")
_LBL_RE = re.compile(r"^([A-Za-z_]\w*):\s*$")
_NUM_RE = re.compile(r"0x[0-9A-Fa-f]+|\b\d+\b")


def triage_module(path):
    """Format-agnostic: parse the decompiler's --disasm into functions (delimited by the --names
    starts and by labels), and structural-hash each function's NORMALISED instruction stream
    (mnemonic+operands with numeric literals masked, comments dropped) -- so the same code linked at
    different addresses in different modules hashes identically. Returns (lane, [func...])."""
    lane, exe = _detect(path)
    if lane is None:
        return (None, None)
    names = _dec(exe, path, "--names")
    disasm = _dec(exe, path, "--disasm")
    if not disasm:
        return (lane, None)
    starts, name_of = set(), {}
    for line in (names or "").splitlines():
        parts = line.split("\t")
        k = _addr_key(parts[0]) if parts else None
        if k is not None:
            starts.add(k)
            if len(parts) > 1 and parts[1].strip():
                name_of[k] = parts[1].strip()
    funcs = []
    cur_name, cur, cur_addr = None, [], None

    def flush():
        if cur:
            h = hashlib.sha1("\n".join(cur).encode()).hexdigest()
            funcs.append({"name": cur_name or "", "size": len(cur), "sh": h, "addr": cur_addr})

    for line in disasm.splitlines():
        if line.startswith("; ----"):                    # segment/section header -> boundary
            flush(); cur_name, cur, cur_addr = None, [], None
            continue
        ml = _LBL_RE.match(line)
        if ml:
            flush(); cur_name, cur, cur_addr = ml.group(1), [], None
            continue
        mi = _INS_RE.match(line)
        if not mi:
            continue
        key = _addr_key(mi.group(1))
        if key in starts and cur:                        # a --names start begins a new function
            nm = name_of.get(key, "")
            flush(); cur_name, cur, cur_addr = nm, [], None
        elif key in starts and not cur and not cur_name:
            cur_name = name_of.get(key, "")
        if cur_addr is None:
            cur_addr = key                               # first instruction's address = function start
        instr = _NUM_RE.sub("#", mi.group(2).split(";")[0]).strip()
        cur.append(instr)
    flush()
    return (lane, funcs)


def _corpus_root():
    """The Win95 binary corpus (…/Windows95/design/corpus/files). wt.py repoints WT_DATA_ROOT at its
    own win95 data dir, so resolve the corpus canonically from the sibling repo unless it holds PEs."""
    from src.core import corpus
    return corpus.root()


def run(limit=None, out=None, root=None, lanes=None):
    root = root or _corpus_root()
    lanes = lanes or ("pe", "ne", "le", "dos")
    # discover EVERY shipping binary and detect its lane by header (not extension alone)
    bins = []
    seen = set()
    exts = (".dll", ".exe", ".drv", ".ocx", ".cpl", ".vxd", ".386", ".mpd", ".pdr", ".com", ".sys")
    for dp, _d, fs in os.walk(root):
        for fn in fs:
            if not fn.lower().endswith(exts) or fn.lower() in seen:
                continue
            p = os.path.join(dp, fn)
            lane, _exe = _detect(p)
            if lane in lanes:
                seen.add(fn.lower()); bins.append((fn, p, lane))
    if limit:
        bins = bins[:limit]

    # SDK-documented export names (genuine but spec'd) -- from both proto tables
    protos = set()
    for w16 in (False, True):
        protos |= set(A._load_sdk_protos(win16=w16).keys())

    per_module = []
    lane_funcs = {}            # lane -> total function count
    bodies = {}                # struct-hash -> per-unique-body record
    total = 0
    for fn, p, lane in bins:
        _lane, fs = triage_module(p)
        if not fs:
            continue
        exps = set()
        if lane == "pe":
            try:
                exps = {n for _o, n in A.PE(p).exports()}
            except Exception:
                exps = set()
        for f in fs:
            total += 1
            sh = f["sh"]
            if not sh:
                continue
            b = bodies.get(sh)
            if b is None:
                b = bodies[sh] = {"modules": set(), "instances": 0, "name": "", "module": fn,
                                  "lane": lane, "size": 0, "documented": False}
            b["modules"].add(fn)
            b["instances"] += 1
            b["size"] = max(b["size"], f["size"])
            if f["name"] and not b["name"]:
                b["name"] = f["name"]
            if f["name"] in exps and f["name"] in protos:
                b["documented"] = True
        per_module.append((fn, len(fs)))
        lane_funcs[lane] = lane_funcs.get(lane, 0) + len(fs)

    # per-unique-body origin + initial pipeline state
    def _origin(b):
        if len(b["modules"]) >= _DUP_MODULES:
            return "shared-runtime"
        if _canon(b["name"]) in _CRT:
            return "crt"
        if b["name"] and any(t.lower() in b["name"].lower() for t in _THUNK):
            return "thunk"
        if b["documented"]:
            return "documented-api"
        return "genuine"

    _STATE0 = {"shared-runtime": "shared", "crt": "crt", "thunk": "thunk",
               "documented-api": "documented", "genuine": "decompiled"}
    buckets = {k: 0 for k in ("shared-runtime", "crt", "thunk", "documented-api", "genuine")}
    genuine_bodies = 0
    for sh, b in bodies.items():
        o = _origin(b)
        b["origin"] = o
        b["state"] = _STATE0[o]
        buckets[o] += 1
        if o in ("genuine", "documented-api"):
            genuine_bodies += 1

    unique_bodies = len(bodies)
    body_modules = {sh: b["modules"] for sh, b in bodies.items()}
    body_meta = {sh: [b["instances"], b["name"], b["size"]] for sh, b in bodies.items()}
    if not out:
        # root is .../design/corpus/files -> write the ledger up in .../design
        design = os.path.dirname(os.path.dirname(root))
        out = os.path.join(design, "origin_triage.csv")
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

    # THE MASTER TRACKER (step 2.0): one row per unique body -- identity + origin + pipeline state.
    # State machine: decompiled -> raised -> typed -> verified (genuine); shared/crt/thunk/documented
    # carry their identify-once state. Re-running triage refreshes it; body-recovery passes update
    # `state`/`verified`/`language` per body_id. This is the auditable denominator for "% to boot".
    ledger = os.path.join(os.path.dirname(out), "body_ledger.csv")
    with open(ledger, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["body_id", "lane", "origin", "state", "verified", "language",
                    "instances", "in_modules", "size", "example_name", "example_module"])
        for sh, b in sorted(bodies.items(), key=lambda kv: (-kv[1]["instances"], kv[0])):
            w.writerow([sh[:16], b["lane"], b["origin"], b["state"], 0, "",
                        b["instances"], len(b["modules"]), b["size"], b["name"], b["module"]])

    return {"modules": len(per_module), "total_functions": total, "unique_bodies": unique_bodies,
            "buckets": buckets, "genuine_unique_targets": genuine_bodies, "out": out,
            "catalog": catalog, "n_shared_bodies": len(shared), "lane_funcs": lane_funcs,
            "ledger": ledger}


def main(argv):
    limit = None
    if "--limit" in argv:
        limit = int(argv[argv.index("--limit") + 1])
    out = None
    if "--out" in argv:
        out = argv[argv.index("--out") + 1]
    lanes = None
    if "--lanes" in argv:
        lanes = tuple(argv[argv.index("--lanes") + 1].split(","))
    r = run(limit=limit, out=out, lanes=lanes)
    print("=== origin triage (all lanes) ===")
    print("  modules analysed        %d   %s" % (
        r["modules"], "  ".join("%s=%d" % (k, v) for k, v in sorted(r["lane_funcs"].items()))))
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
    print("  MASTER BODY LEDGER      %d rows -> %s" % (r["unique_bodies"], r.get("ledger", "")))
    print("  triage ledger -> %s" % r["out"])


if __name__ == "__main__":
    main(sys.argv[1:])
