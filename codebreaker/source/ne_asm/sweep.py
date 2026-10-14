r"""ne_sweep.py -- run the idiomatic NE re-emitter (ne_idiom) across the whole
16-bit NE corpus, unattended.  The KERNEL/USER/GDI analog of the PE
`engine/targets/win95/l2sweep.py`: for every CODE segment of every NE module it
reconciles the segment to BYTE-EXACT idiomatic Microsoft SDK cMacros source
(via the DOSBox-free nasm-masm backend, db-pinning only what nasm re-encodes),
and banks a corpus coverage ledger.

This is the structural piece the NE track lacked -- ne_idiom.py was CLI-only,
one <module> <seg> <lo> <hi> at a time.  With this, the ~309 not-yet-idiomatic
NE modules reconstruct in one pass.

    python -m engine.inverse_engine.ne_sweep [--limit N] [--start K] [--emit DIR]
                                             [--list] [--backend nasm|ml]
"""
import argparse
import glob
import json
import os
import struct
import sys
from codebreaker.base import corpus as paths, settings

def _masm():
    from codebreaker.build import masm
    return masm


def _segdiff():
    from codebreaker.source.ne_asm import segdiff
    return segdiff


def _segment():
    from codebreaker.source.ne_asm import segment
    return segment


def _msvc152():
    from codebreaker.build import msvc152
    return msvc152


class _Corpus(object):
    """What the old target registry answered: where a module of the corpus is."""
    @staticmethod
    def resolve(module):
        from codebreaker.base import corpus
        return corpus.resolve(module) or corpus.retail(module) or module


def _target():
    return _Corpus


ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def _setup():
    os.chdir(ROOT)
    sys.path.insert(0, ".")
def _is_ne(path):
    """MZ -> e_lfanew -> 'NE' signature (a 16-bit New Executable)."""
    try:
        with open(path, "rb") as f:
            d = f.read(0x40)
            if d[:2] != b"MZ":
                return False
            off = struct.unpack_from("<I", d, 0x3c)[0]
            f.seek(off)
            return f.read(2) == b"NE"
    except Exception:
        return False


def ne_modules():
    """Every NE module under the system tree, de-duplicated by basename."""
    _setup()
    base = paths.root()
    seen, out = set(), []
    for sub in (base + "/SYSTEM", base + "/SYSTEM32", base, base + "/COMMAND"):
        for ext in ("exe", "dll", "drv", "386", "cpl", "scr", "fon"):
            for p in sorted(glob.glob(os.path.join(sub, "*." + ext))):
                nm = os.path.basename(p).lower()
                if nm in seen:
                    continue
                seen.add(nm)
                if _is_ne(p):
                    out.append((nm, p))
    return out


def _resolve(module):
    target = _target()
    try:
        return target.resolve(module)
    except Exception:
        h = [p for p in [paths.retail(module)] if p]
        return h[0] if h else module


def sweep_module(module, path, backend="nasm", emit=None):
    """Reconcile every CODE segment of one NE module to idiomatic cMacros.
    Returns a per-module record with per-segment coverage."""
    from codebreaker.formats import ne as ne_mod
    from codebreaker.source.ne_asm import segment as _t
    from codebreaker.source.ne_asm import idiom as ne_idiom
    from codebreaker.symbols import debug_build as debug_names
    ne = ne_mod.NEFile(path)
    o = _t.load_ordinals("ORDINALS.md")
    tag = os.path.splitext(os.path.basename(module))[0].lower()
    nj = settings.state("idiomatic", tag, "names.json")
    ov = debug_names.load_override(nj) if os.path.exists(nj) else {}
    segs = []
    for i, s in enumerate(ne.segments(), 1):
        if s.get("kind") != "CODE":
            continue
        sg = _t.Seg(ne, i, o, name_override=ov.get(i))
        n = len(sg.code)
        if n == 0:
            continue
        r = ne_idiom.verify_range(sg, 0, n, backend=backend)
        segs.append(dict(seg=i, n=n, ok=r["ok"], n_code=r["n_code"], db=r["db"], live=r["live"]))
        if emit and r["ok"]:
            d = os.path.join(emit, tag)
            os.makedirs(d, exist_ok=True)
            with open(os.path.join(d, "%s_seg%02d.asm" % (tag, i)), "w", newline="\r\n") as f:
                f.write(r["asm"])
    return dict(module=module, segs=segs,
                ok=all(s["ok"] for s in segs) if segs else False,
                nseg=len(segs), nok=sum(1 for s in segs if s["ok"]),
                code=sum(s["n_code"] for s in segs), live=sum(s["live"] for s in segs))


def sweep(limit=None, start=0, emit=None, backend="nasm"):
    mods = ne_modules()[start:]
    if limit:
        mods = mods[:limit]
    if emit:
        os.makedirs(emit, exist_ok=True)
    print("NE idiomatic sweep: %d NE modules (backend=%s)\n" % (len(mods), backend))
    tot = dict(mods=0, full=0, segs=0, segok=0, code=0, live=0, err=0)
    summary = []
    for nm, path in mods:
        try:
            rec = sweep_module(nm, path, backend=backend, emit=emit)
            tot["mods"] += 1; tot["full"] += 1 if rec["ok"] else 0
            tot["segs"] += rec["nseg"]; tot["segok"] += rec["nok"]
            tot["code"] += rec["code"]; tot["live"] += rec["live"]
            summary.append(rec)
            print("  %-16s segs %d/%d exact  %d instrs  %.0f%% live%s"
                  % (nm, rec["nok"], rec["nseg"], rec["code"],
                     100.0 * rec["live"] / max(1, rec["code"]),
                     "" if rec["ok"] else "  (partial)"))
        except Exception as e:
            tot["err"] += 1
            summary.append(dict(module=nm, error=str(e)[:200]))
            print("  %-16s ERROR %s" % (nm, str(e)[:70]))
    print("\n" + "=" * 60)
    print("NE IDIOMATIC SWEEP -- %d modules" % tot["mods"])
    print("  fully byte-exact modules : %d/%d" % (tot["full"], tot["mods"]))
    print("  CODE segments byte-exact : %d/%d" % (tot["segok"], tot["segs"]))
    print("  instructions             : %d  (%.1f%% live idiomatic, rest db-pinned)"
          % (tot["code"], 100.0 * tot["live"] / max(1, tot["code"])))
    print("  module errors            : %d" % tot["err"])
    if emit:
        with open(os.path.join(emit, "_ne_idiom_summary.json"), "w") as f:
            json.dump(dict(totals=tot, modules=summary), f, indent=1)
        print("  wrote _ne_idiom_summary.json")
    return summary


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--limit", type=int)
    ap.add_argument("--start", type=int, default=0)
    ap.add_argument("--emit")
    ap.add_argument("--backend", default="nasm", choices=("nasm", "ml"))
    ap.add_argument("--list", action="store_true")
    a = ap.parse_args()
    _setup()
    if a.list:
        for nm, _p in ne_modules():
            print(nm)
        return
    sweep(a.limit, a.start, a.emit, a.backend)


if __name__ == "__main__":
    main()
