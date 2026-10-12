#!/usr/bin/env python3
r"""verify95.py -- step 2.2: the verification oracle. A reconstructed body is CORRECT only when it
BEHAVES like the original, proven on the Unicorn oracle -- not merely when it compiles. This is the
definition of "done" for the full reconstruction and the throughput gate for the whole program.

For a function it runs the ORIGINAL and the REBUILT under the same oracle with identical inputs and
compares a codegen-INDEPENDENT behavioral fingerprint: (return value, clean-vs-fault exit, and the
sequence of imported API calls). Instruction path and count are ignored -- MSC-vs-OpenWatcom codegen
differs there without any behavioral difference. PE/DOS bodies run on the behavior-engine tracer;
NE (16-bit) bodies on ne_dynoracle. Divergence on identical inputs is a real reconstruction defect.

    wt --target win95 verify95 <retail> <rebuilt> [--arity N] [--max N]   # pair-verify two modules
    wt --target win95 verify95 --ledger                                   # batch: verify every body
                                                                          # whose module is rebuilt,
                                                                          # update body_ledger.csv

No fabrication: a fingerprint records only what the oracle observed; a trace error is recorded as
itself, never scored as a pass.
"""
import os
import sys
import csv

from src.core import cores, paths
from src.engines.behavior import tracer
from src.targets.win95 import ne_dynoracle, origintriage as OT

# compiler/CRT-runtime imports the toolchain emits (not the body's own logic) -- ignored so
# MSC-vs-OpenWatcom startup noise is not scored as a behavioral divergence.
_TOOLCHAIN = {"KERNEL.GETVERSION", "KERNEL.GETWINFLAGS", "KERNEL.LOCALINIT", "KERNEL.GETVERSION16",
              "KERNEL32.GETVERSION", "KERNEL32.GETVERSIONEXA", "KERNEL32.GETCURRENTTHREADID"}


def _norm_dep(d):
    """Canonicalise an imported-call name so the two builds' import-table SPELLINGS of the same
    import compare equal: uppercase, drop a .DLL/.EXE on the module half, unify the separator.
    ('KERNEL32.dll!#21' and 'KERNEL32!#21' are the same ordinal import -> same behavior)."""
    s = str(d).upper().replace("!", ".").replace(":", ".")
    parts = s.split(".")
    parts = [p for p in parts if p not in ("DLL", "EXE", "DRV")]
    return ".".join(parts)


def _is_noise(api):
    """Toolchain/thunk imports that differ by CODEGEN (OpenWatcom vs MS), not by the body's logic --
    excluded from the behavioral fingerprint so they don't read as a divergence."""
    a = str(api).upper()
    if a in _TOOLCHAIN:
        return True
    return ("SMAPLS" in a or "SUNMAPLS" in a or "FT_" in a or "QT_" in a or "THUNK" in a
            or a.endswith(".@0") or "GETVERSION" in a or "GETWINFLAGS" in a)


def _exit_norm(e):
    if e is None:
        return "none"
    e = str(e)
    return "ret" if e == "ret" else ("fault" if e.startswith("exception") else e.split(":")[0])


import struct as _struct

# provisioned environment (mirrors envprov): a region of pointers that all resolve to a safe RET, so
# a pointer/handle argument that gets dereferenced OR called through lands on a clean return instead
# of a null fault. Both regions live inside the tracer's pre-mapped scratch window (0x200000..0x210000).
_OBJ, _SAFE = 0x206000, 0x207800


def _prov_meminit():
    return [(_OBJ, _struct.pack("<I", _SAFE) * 0x400),   # 4 KB of "-> _SAFE" pointers
            (_SAFE, b"\xc3" * 16)]                        # RET landing pad


# --- behavioral-fingerprint cache -------------------------------------------------------------
# A PE fingerprint is a pure function of (module bytes, func addr, input vector): the trace is
# deterministic given those. The RETAIL side (the corpus binary) is reconstruction-independent, so
# its fingerprint is identical across every body and every sweep re-run -- yet it was re-traced each
# time, making it ~half of the oracle cost (4 of 8 traces/body). Cache fingerprints keyed by the
# module's content hash so a re-run (the common case after any synth/lifter change) skips the retail
# traces entirely. Keying on content hash (not path) makes it correct for both sides and for shared
# bodies: a rebuilt DLL gets a fresh hash each build, so it never stales. Disabled by CB_NO_FP_CACHE.
import hashlib as _hashlib
import pickle as _pickle
import sqlite3 as _sqlite3

_SHA_MEMO = {}
_FP_DB = None


def _fp_cache_path():
    try:
        from src.core import paths
        return os.path.join(paths.design_root(), "verify_fp_cache.sqlite")
    except Exception:
        return os.path.join(os.path.dirname(os.path.abspath(__file__)), "verify_fp_cache.sqlite")


def _fp_db():
    """Open (once per process) the WAL-mode sqlite fp cache, or False if it cannot be used. WAL lets
    the 10 sweep workers read concurrently; a write that loses the lock is swallowed (re-traced next
    time), so the cache never affects correctness."""
    global _FP_DB
    if _FP_DB is None:
        try:
            db = _sqlite3.connect(_fp_cache_path(), timeout=5.0)
            db.execute("PRAGMA journal_mode=WAL")
            db.execute("PRAGMA synchronous=NORMAL")
            db.execute("CREATE TABLE IF NOT EXISTS fp(k TEXT PRIMARY KEY, v BLOB)")
            db.commit()
            _FP_DB = db
        except Exception:
            _FP_DB = False
    return _FP_DB or None


def _mod_sha(path):
    """sha256 of a module file, memoized by (path, mtime, size) so a stable corpus binary is hashed
    once per process."""
    try:
        st = os.stat(path)
    except OSError:
        return None
    key = (path, int(st.st_mtime), st.st_size)
    s = _SHA_MEMO.get(key)
    if s is None:
        h = _hashlib.sha256()
        try:
            with open(path, "rb") as f:
                for chunk in iter(lambda: f.read(1 << 20), b""):
                    h.update(chunk)
        except OSError:
            return None
        s = h.hexdigest()
        _SHA_MEMO[key] = s
    return s


def _fp_key(sha, func, spec):
    args = tuple(int(a) for a in spec.get("args", []))
    mi = tuple((int(a), bytes(b)) for a, b in (spec.get("mem_init") or []))
    return sha + ":" + _hashlib.sha1(repr((func, args, mi)).encode()).hexdigest()


def _fp_cache_get(key):
    db = _fp_db()
    if not db:
        return None
    try:
        row = db.execute("SELECT v FROM fp WHERE k=?", (key,)).fetchone()
        return _pickle.loads(row[0]) if row else None
    except Exception:
        return None


def _fp_cache_put(key, fp):
    db = _fp_db()
    if not db:
        return
    try:
        db.execute("INSERT OR REPLACE INTO fp(k,v) VALUES(?,?)", (key, _pickle.dumps(fp)))
        db.commit()
    except Exception:
        pass


def _arm():
    """The oracle resolves its engines through the active target. Called from a library (not
    through wt) nothing had set one, every trace failed inside the registry, and the failure
    was reported as an 'inconclusive' verdict. Default to win95 when no target is active."""
    cores.ensure_target(paths.target_dir("win95"))


def _fp_pe(path, func, spec):
    _arm()
    key = None
    if not os.environ.get("CB_NO_FP_CACHE"):
        sha = _mod_sha(path)
        if sha:
            key = _fp_key(sha, func, spec)
            hit = _fp_cache_get(key)
            if hit is not None:
                return hit
    try:
        r = tracer.trace_function(path, func=func, args=spec["args"], mem_init=spec.get("mem_init"),
                                  stub_externals=True)
    except ImportError:
        raise                                     # a broken setup is not an observation
    except Exception as e:
        return {"err": type(e).__name__}          # transient trace errors are never cached
    deps = tuple(_norm_dep(d) for d in r.get("api_deps", []) if not _is_noise(d))
    fp = {"ret": r.get("ret"), "exit": _exit_norm(r.get("exit")), "deps": deps,
          "truncated": bool(r.get("truncated"))}
    # Cache only settled results. A truncated trace can differ by wall-clock (the tracer's 2s cap),
    # so caching it could record a machine-dependent fingerprint -- re-trace those instead.
    if key and not fp["truncated"]:
        _fp_cache_put(key, fp)
    return fp


def _fp_ne(path, ordn, spec):
    _arm()
    try:
        r = ne_dynoracle.probe(path, ordinal=ordn, args=spec["args"])
    except ImportError:
        raise
    except Exception as e:
        return {"err": type(e).__name__}
    deps = tuple(_norm_dep(h.get("api")) for h in r.get("imports_hit", []) if not _is_noise(h.get("api")))
    return {"ret": r.get("ret"), "exit": _exit_norm(r.get("exit")), "deps": deps, "truncated": False}


def _vectors(arity, lane):
    """Input vectors to exercise a function past the immediate null-fault (no per-function signatures
    needed). Each is {args, mem_init}. Beyond zeros/handles, the PROVISIONED vector points every arg
    at a valid object region whose contents resolve dereference/call chains to a safe RET -- so a
    pointer-taking function runs its real path to completion instead of faulting (env-provisioned)."""
    if arity <= 0:
        return [{"args": []}]
    n = arity
    vs = [{"args": [0] * n}]
    if lane in ("pe", "dos"):
        vs.append({"args": [0x200000] * n})                      # bare scratch pointer
        vs.append({"args": [_OBJ] * n, "mem_init": _prov_meminit()})   # provisioned objects
    else:                                                        # NE controlled env: valid handles
        vs.append({"args": [0x0400] * n})                        # LZ handle base
        vs.append({"args": [0x0080] * n})                        # open-file handle
    vs.append({"args": [1] * n})
    return vs


def _observed(fp):
    """We observed behavior on this vector iff the side traced at all (a fault or truncation is still
    an observation -- identical faults on identical inputs are evidence of identical behavior)."""
    return "err" not in fp


def _match(a, b):
    return a["ret"] == b["ret"] and a["exit"] == b["exit"] and a["deps"] == b["deps"]


def _body_verdict(rs, bs):
    """Aggregate per-vector fingerprints. On each vector both sides get the SAME deterministic input,
    so an identical fingerprint (return, exit, imported-call sequence -- including an identical fault
    or truncation) is evidence they behave identically on that input; a difference is a real
    behavioral divergence. Equivalent iff >=1 vector was observed on both sides and every observed
    vector matches; divergent if any observed vector differs; inconclusive if none was observed."""
    obs = [(a, b) for a, b in zip(rs, bs) if _observed(a) and _observed(b)]
    if not obs:
        return "inconclusive"
    if all(_match(a, b) for a, b in obs):
        clean = any(a.get("exit") == "ret" and not a.get("truncated") for a, _b in obs)
        return "equivalent" if clean else "consistent"     # consistent = matches only via fault/trunc
    return "divergent"


def _fmt(path):
    lane, _exe = OT._detect(path)
    return lane


def verify_pair(retail, rebuilt, arity=0, maxn=64):
    """Per-export behavioral verdicts for retail vs rebuilt. Returns [(name, verdict, fp_r, fp_b)]."""
    lane = _fmt(retail)
    from src.targets.win95 import autobuild95 as A
    vecs = _vectors(arity, lane)
    out = []
    if lane == "ne":
        # NE exports are mostly BY ORDINAL (the LZ APIs have no resident name). Probe only the
        # ordinals the entry table ACTUALLY exports -- blindly probing 1..maxn tests non-exports
        # (unused ordinals resolve to different garbage in a smaller rebuild) and floods the verdict
        # with false divergences. Fall back to 1..maxn only if the entry table can't be read.
        names = {o: n for o, n in ne_dynoracle_exports(retail)}
        ords = A.NE(retail).export_ordinals() if A.NE(retail).is_ne else []
        for ordn in (ords[:maxn] or list(range(1, maxn + 1))):
            rs = [_fp_ne(retail, ordn, v) for v in vecs]
            bs = [_fp_ne(rebuilt, ordn, v) for v in vecs]
            if all("err" in a for a in rs) and all("err" in b for b in bs):
                continue                                 # ordinal not present in either -> skip
            out.append((names.get(ordn, "@%d" % ordn), _body_verdict(rs, bs), rs[0], bs[0]))
    else:
        pr, pb = A.PE(retail), A.PE(rebuilt)
        rbase, bbase = A._imagebase(pr), A._imagebase(pb)   # tracer wants a VA (imagebase+rva)
        raddr, baddr = pr.export_addrs(), pb.export_addrs()
        for nm in list(raddr)[:maxn]:
            if nm not in baddr:
                continue
            rs = [_fp_pe(retail, rbase + raddr[nm], v) for v in vecs]
            bs = [_fp_pe(rebuilt, bbase + baddr[nm], v) for v in vecs]
            out.append((nm, _body_verdict(rs, bs), rs[0], bs[0]))
    return out


def ne_dynoracle_exports(path):
    from src.targets.win95 import autobuild95 as A
    ne = A.NE(path)
    return [(o, n) for o, n in ne.resident_names()] if ne.is_ne else []


def _print_pair(retail, rebuilt, verdicts):
    from collections import Counter
    c = Counter(v for _n, v, _a, _b in verdicts)
    _TAG = {"equivalent": "OK  ", "consistent": "~   ", "divergent": "DIFF", "inconclusive": "?   ", "error": "err "}
    print("=== verify %s  vs  %s ===" % (os.path.basename(retail), os.path.basename(rebuilt)))
    for nm, v, a, b in verdicts:
        note = ""
        if v == "divergent":
            note = "  ret %s/%s exit %s/%s deps %d/%d" % (a.get("ret"), b.get("ret"),
                    a.get("exit"), b.get("exit"), len(a.get("deps", ())), len(b.get("deps", ())))
        elif v == "inconclusive":
            note = "  (trace truncated -- unfollowable, e.g. 32->16 thunk)"
        print("  %-28s %s%s" % (nm, _TAG[v], note))
    print("  --- %d equivalent, %d divergent, %d inconclusive, %d error (of %d) ---" % (
        c["equivalent"], c["divergent"], c["inconclusive"], c["error"], len(verdicts)))  # noqa
    return c["equivalent"], len(verdicts)


# --------------------------------------------------------------- ledger batch
def _design():
    from src.core import paths
    for c in (os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design"),
              r"C:\Users\mrtit\Documents\GitHub\Windows95\design"):
        if os.path.isdir(c):
            return c
    return None


def _rebuilt_index():
    """Map retail module name -> a rebuilt artifact path we can verify against (the from-source
    builds we've produced: base/<mod>/_build, base/<mod>/_ow, design/scaffold/<mod>)."""
    base = os.path.join(os.path.dirname(_design()), "base")
    idx = {}
    for root in (base,):
        if not os.path.isdir(root):
            continue
        for dp, _d, fs in os.walk(root):
            for fn in fs:
                if fn.lower().endswith((".dll", ".exe", ".drv", ".vxd", ".386")):
                    idx.setdefault(fn.lower(), os.path.join(dp, fn))
    return idx


def verify_ledger(maxmods=None):
    design = _design()
    ledger = os.path.join(design, "body_ledger.csv")
    rows = list(csv.DictReader(open(ledger)))
    rebuilt = _rebuilt_index()
    # group genuine bodies by their example module; verify modules that have a rebuilt artifact
    by_mod = {}
    for r in rows:
        if r["origin"] in ("genuine", "documented-api"):
            by_mod.setdefault(r["example_module"], []).append(r)
    verified_ids = set()
    done_mods = 0
    for mod, brs in by_mod.items():
        rb = rebuilt.get(mod.lower())
        if not rb:
            continue
        retail = OT._corpus_root()
        from src.targets.win95 import autobuild95 as A  # locate retail path
        rp = None
        for dp, _d, fs in os.walk(retail):
            if mod in fs:
                rp = os.path.join(dp, mod); break
        if not rp:
            continue
        try:
            verdicts = verify_pair(rp, rb, maxn=256)
        except Exception:
            continue
        okset = {nm for nm, v, _a, _b in verdicts if v == "equivalent"}
        # map each verified export to the ACTUAL body in THIS module by re-triaging it
        # (name->body_id), rather than the ledger's first-seen example_name -- precise crediting.
        try:
            _lane, fns = OT.triage_module(rp)
        except Exception:
            fns = None
        name2id = {f["name"]: f["sh"][:16] for f in (fns or []) if f.get("name") and f.get("sh")}
        present = {r["body_id"] for r in rows}
        for nm in okset:
            bid = name2id.get(nm)
            if bid and bid in present:
                verified_ids.add(bid)
        done_mods += 1
        if maxmods and done_mods >= maxmods:
            break
    # update ledger
    for r in rows:
        if r["body_id"] in verified_ids:
            r["verified"] = "1"
            r["state"] = "verified"
    with open(ledger, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=rows[0].keys())
        w.writeheader(); w.writerows(rows)
    return {"modules_verified": done_mods, "bodies_verified": len(verified_ids), "ledger": ledger}


def main(argv):
    if "--ledger" in argv:
        mm = int(argv[argv.index("--max") + 1]) if "--max" in argv else None
        r = verify_ledger(maxmods=mm)
        print("=== verify95 --ledger ===")
        print("  modules verified against a rebuild: %d" % r["modules_verified"])
        print("  bodies marked VERIFIED:             %d" % r["bodies_verified"])
        print("  ledger -> %s" % r["ledger"])
        return
    pos = [a for a in argv if not a.startswith("--")]
    if len(pos) < 2:
        print(__doc__); return
    arity = int(argv[argv.index("--arity") + 1]) if "--arity" in argv else 0
    maxn = int(argv[argv.index("--max") + 1]) if "--max" in argv else 64
    verdicts = verify_pair(pos[0], pos[1], arity=arity, maxn=maxn)
    _print_pair(pos[0], pos[1], verdicts)


if __name__ == "__main__":
    main(sys.argv[1:])
