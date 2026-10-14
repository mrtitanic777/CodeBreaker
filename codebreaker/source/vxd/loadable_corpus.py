r"""loadable_corpus.py -- drive the whole VxD corpus through the from-source LOADABLE build.

The object-level test (vxd_corpus) proves byte-exact CODE recovery. This proves the next thing: that the
recovered source rebuilds, through the AUTHENTIC period toolchain (ML -> LINK 2.60 /VXD -> RC + ADRC2VXD),
into a container the Win95 loader accepts -- device id stamped from the DDB, identity page map, version
resource embedded. It runs vxd_from_source.build_from_source on every module and records the build stage
reached and the container validation (device id, identity map, object count, resource length).

Results checkpoint to JSON after every module (resumable). A boot is a separate, human-driven step; this
measures BUILDABILITY and container validity at corpus scale, the static precondition for a boot.

    python -m engine.inverse_engine.vxd_loadable_corpus <corpus>/oracle recovered/vxd_loadable.json
    python -m engine.inverse_engine.vxd_loadable_corpus ... --redo vkd --workers 4
"""
import os
import sys
import json
import time
from codebreaker.base import corpus as paths

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)


def _one(name, v, root):
    from codebreaker.source.vxd import from_source as F
    t0 = time.time()
    try:
        r = F.build_from_source(v, root=root)
        out = {"ok": bool(r.get("ok")), "stage": r.get("stage"), "seconds": round(time.time() - t0, 1)}
        if r.get("ok"):
            out.update({"objects": r["objects"], "device_id": r["device_id"],
                        "device_id_ok": r["device_id_ok"], "identity": r["page_map_identity"],
                        "resource_len": r["resource_len"], "vxd_len": r["vxd_len"]})
        else:
            out["log"] = (r.get("log") or "")[-240:]
        return out
    except Exception as e:
        import traceback
        return {"ok": False, "stage": "exception", "log": str(e)[:200],
                "trace": traceback.format_exc()[-300:], "seconds": round(time.time() - t0, 1)}


def run(srcdir, out_json, redo=None, workers=4, root="."):
    import threading
    from concurrent.futures import ThreadPoolExecutor, as_completed
    from codebreaker.formats import le
    from codebreaker.source.vxd import corpus as vxd_corpus
    import glob
    root = os.path.abspath(root)
    redo = set(redo or [])
    results = json.load(open(out_json)) if os.path.exists(out_json) else {}
    cand = []
    for pat in vxd_corpus._VXD_EXTS:
        cand += glob.glob(os.path.join(srcdir, pat))
    for f in vxd_corpus._EXTRA_FILES:
        cand.append(os.path.join(srcdir, f))
    files = sorted(p for p in set(cand) if os.path.exists(p) and vxd_corpus._is_le_vxd(p))
    todo = []
    for p in files:
        try:
            mods = le.open_vxds(p)
        except Exception as e:
            results[os.path.basename(p)] = {"ok": False, "stage": "open", "log": str(e)[:80]}
            continue
        for name, v in mods:
            key = name.split(".")[0].lower()
            if key in results and key not in redo:
                continue
            todo.append((key, name, v))
    lock = threading.Lock()
    t_all = time.time()
    print("== %d modules to build, %d workers ==" % (len(todo), workers), flush=True)
    with ThreadPoolExecutor(max_workers=workers) as ex:
        futs = {ex.submit(_one, name, v, root): key for key, name, v in todo}
        for fut in as_completed(futs):
            key = futs[fut]
            r = fut.result()
            with lock:
                results[key] = r
                _save(results, out_json)
            if r["ok"]:
                print("  %-14s OK   objs=%-2d dev=0x%04x dev_ok=%-5s ident=%-5s res=%-4d [%ss]" % (
                    key, r["objects"], r["device_id"], r["device_id_ok"], r["identity"],
                    r["resource_len"], r["seconds"]), flush=True)
            else:
                print("  %-14s FAIL@%-10s %s" % (key, r["stage"], (r.get("log") or "")[-90:].replace("\n", " ")),
                      flush=True)
    _summary(results, time.time() - t_all)
    return results


def _save(results, path):
    tmp = path + ".tmp"
    json.dump(results, open(tmp, "w"), indent=1)
    os.replace(tmp, path)


def _summary(results, secs):
    ok = [r for r in results.values() if r.get("ok")]
    dev_ok = sum(1 for r in ok if r.get("device_id_ok"))
    ident = sum(1 for r in ok if r.get("identity"))
    from collections import Counter
    stages = Counter(r.get("stage") for r in results.values() if not r.get("ok"))
    print("\n== loadable corpus: %d/%d BUILD | %d device-id-correct | %d identity-map | %.0fs ==" % (
        len(ok), len(results), dev_ok, ident, secs), flush=True)
    if stages:
        print("   failures by stage: %s" % dict(stages), flush=True)


if __name__ == "__main__":
    from codebreaker.base import settings
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    srcdir = args[0] if args else paths.root()
    out = args[1] if len(args) > 1 else settings.state("vxd", "loadable.json")
    redo = sys.argv[sys.argv.index("--redo") + 1].split(",") if "--redo" in sys.argv else []
    workers = int(sys.argv[sys.argv.index("--workers") + 1]) if "--workers" in sys.argv else 4
    run(srcdir, out, redo=redo, workers=workers, root=ROOT)
