#!/usr/bin/env python3
r"""Golden-output regression harness for CodeBreaker.

Two independent guards, because two kinds of change must be caught:

  * resolve-hash -- for Phase-2 convergence (#1). For every (target, core) it
    records the content hash of the module that `import <core>` RESOLVES to under
    that target (target tool dir first, then the shared engine/ -- exactly `wt`'s
    rule). Hoisting a core to engine/ and deleting the per-target duplicates
    changes WHICH file sits on disk; this proves it never changes WHICH bytes a
    target executes. Any drift here means a relocation altered real behaviour.

  * behavioural -- for engine changes (#3 types, #4 structurer). A hash of
    il.lift_asm_function output over a FIXED win95 asm sample (win95's corpus is
    frozen -- its conduct is byte-exact/done -- so the sample is deterministic;
    the other eras are still being ground by conduct and would be unstable). An
    unintended change to the emitted C trips this; an intended improvement is
    accepted with `rebaseline`.

    python goldens.py capture      # write baseline -> engine/goldens/*.json
    python goldens.py check        # compare to baseline; exit 1 on drift
    python goldens.py rebaseline   # accept current behavioural output as baseline
                                   #   (resolve-hash is never auto-accepted)
"""
import hashlib
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))       # CodeBreaker/engine
WT_DIR = os.path.dirname(HERE)                          # CodeBreaker
DATA_ROOT = os.path.dirname(WT_DIR)                          # repo parent
STORE = os.path.join(HERE, "goldens")

CORES = ["masmlift", "trace", "ne", "disasm", "cfg", "lift",
         "verify", "ledger", "idord", "names", "argcheck"]
BEHAVIOUR_TARGET = "win95"                              # the frozen corpus
BEHAVIOUR_N = 300                                       # deterministic asm sample size

sys.path.insert(0, WT_DIR)
import wt                                               # reuse its target resolution


def _hash_file(p):
    return hashlib.md5(open(p, "rb").read()).hexdigest() if os.path.exists(p) else None


def _resolve(target, core):
    """The path `import <core>` resolves to for `target`: tool dir wins, else engine."""
    tools_dir, _data = wt.TARGETS[target]
    for d in (tools_dir, wt.ENGINE):
        cand = os.path.join(d, core + ".py")
        if os.path.exists(cand):
            return cand
    return None


def resolve_hashes():
    """{target: {core: (relpath, md5)}} -- what each target actually imports."""
    out = {}
    for target in sorted(wt.TARGETS):
        row = {}
        for core in CORES:
            p = _resolve(target, core)
            row[core] = None if p is None else [os.path.relpath(p, DATA_ROOT).replace("\\", "/"),
                                                _hash_file(p)]
        out[target] = row
    return out


def _sample_asm():
    """A fixed, deterministic list of win95 asm files (sorted, evenly strided)."""
    from codebreaker.core import layout
    import glob
    _tools, data = wt.TARGETS[BEHAVIOUR_TARGET]
    # sort by (module-dir, asm filename) NOT full path, so the strided sample is the
    # SAME set of files whether asm/ is flat or sorted into the component tree — the
    # behavioural hash tracks il.py, never the directory layout.
    files = sorted(glob.glob(os.path.join(layout.asm_dir(data), "**", "*.asm"),
                             recursive=True),
                   key=lambda f: (os.path.basename(os.path.dirname(f)).lower(),
                                  os.path.basename(f).lower()))
    if not files:
        return []
    stride = max(1, len(files) // BEHAVIOUR_N)
    return [os.path.relpath(f, DATA_ROOT).replace("\\", "/") for f in files[::stride][:BEHAVIOUR_N]]


def behaviour_hash(sample):
    """Hash of il's lifted C over the given asm sample (order-stable)."""
    sys.path.insert(0, HERE)
    from codebreaker.ir import il
    h = hashlib.md5()
    n = 0
    for rel in sample:
        p = os.path.join(DATA_ROOT, rel)
        if not os.path.exists(p):
            continue
        try:
            c = il.lift_asm_function(p, "fn")
        except Exception as e:
            c = "ERR:" + str(e)[:80]
        # key by <module-dir>/<filename>, NOT the full relpath: il output is derived
        # purely from file CONTENT, so this hash tracks lifting behaviour and stays
        # stable when asm/ is re-sorted into the component tree (same files, same key).
        key = "%s/%s" % (os.path.basename(os.path.dirname(rel)), os.path.basename(rel))
        h.update(key.lower().encode()); h.update(b"\0")
        h.update(c.encode("utf-8", "replace")); h.update(b"\0")
        n += 1
    return h.hexdigest(), n


def capture():
    os.makedirs(STORE, exist_ok=True)
    rh = resolve_hashes()
    json.dump(rh, open(os.path.join(STORE, "resolve.json"), "w"), indent=1, sort_keys=True)
    sample = _sample_asm()
    bh, n = behaviour_hash(sample)
    json.dump({"target": BEHAVIOUR_TARGET, "n": n, "hash": bh, "sample": sample},
              open(os.path.join(STORE, "behaviour.json"), "w"), indent=1)
    print("captured: resolve-hash for %d targets x %d cores; behaviour over %d win95 fns (%s)"
          % (len(rh), len(CORES), n, bh[:12]))


def check():
    ok = True
    # ---- resolve-hash ----
    base = _load("resolve.json")
    if base is None:
        print("no baseline -- run `goldens.py capture` first"); return False
    cur = resolve_hashes()
    for target in sorted(base):
        for core in CORES:
            b, c = base[target].get(core), cur.get(target, {}).get(core)
            bh = b[1] if b else None
            ch = c[1] if c else None
            if bh != ch:
                ok = False
                print("  DRIFT %-7s %-9s  %s (%s)  ->  %s (%s)"
                      % (target, core, b and b[0], _sh(bh), c and c[0], _sh(ch)))
    # ---- behavioural ----
    bb = _load("behaviour.json")
    if bb is not None:
        cur_hash, n = behaviour_hash(bb["sample"])
        if cur_hash != bb["hash"]:
            ok = False
            print("  DRIFT behaviour  il over %d win95 fns: %s -> %s  (intended? `rebaseline`)"
                  % (n, bb["hash"][:12], cur_hash[:12]))
    print("GOLDENS OK -- no drift" if ok else "GOLDENS: drift detected (see above)")
    return ok


def rebaseline():
    """Accept current behavioural output; resolve-hash still recaptured verbatim."""
    os.makedirs(STORE, exist_ok=True)
    sample = _sample_asm()
    bh, n = behaviour_hash(sample)
    json.dump({"target": BEHAVIOUR_TARGET, "n": n, "hash": bh, "sample": sample},
              open(os.path.join(STORE, "behaviour.json"), "w"), indent=1)
    print("behaviour re-baselined: %d win95 fns (%s)" % (n, bh[:12]))


def _load(name):
    p = os.path.join(STORE, name)
    return json.load(open(p)) if os.path.exists(p) else None


def _sh(x):
    return "--" if x is None else x[:8]


def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else "check"
    if cmd == "capture":
        capture()
    elif cmd == "rebaseline":
        rebaseline()
    elif cmd == "check":
        sys.exit(0 if check() else 1)
    else:
        print(__doc__); sys.exit(2)


if __name__ == "__main__":
    main()
