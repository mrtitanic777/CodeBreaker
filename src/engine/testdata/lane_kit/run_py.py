"""run_py.py <module path> <state folder> <out json> -- the Python's reconstruct.run on one module, its rows written out: the reference the lane kit's
proof (engine.lane_kit.proof) is compared with. Run from anywhere; the module is a path in the corpus, CB_CORPUS names the corpus's folder (default: the Windows 95
corpus beside the main checkout) and CB_MAIN_CHECKOUT the main checkout (its settings and built native libraries; default: this one's).

    python run_py.py <corpus>/WINDOWS/SYSTEM/lz32.dll <a fresh folder> lz32.dll.python.json
"""
import json
import os
import sys
import time

WT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "..", ".."))
MAIN = os.environ.get("CB_MAIN_CHECKOUT", WT)
sys.path.insert(0, WT)
os.makedirs(sys.argv[2], exist_ok=True)
os.environ["CB_STATE"] = sys.argv[2]
# (set before codebreaker is imported: its __init__ exports the settings it finds in THIS checkout)
for k, v in {"CB_ENGINE_BIN": MAIN + "/native/engine/build/bin", "CB_TRACER_BIN": MAIN + "/build",
             "CB_DECOMPILER_BIN": MAIN + "/native/decompiler/build/bin", "CB_LANGUAGES_DIR": MAIN + "/native/decompiler/languages",
             "CB_FORMATS_BIN": MAIN + "/native/formats/build/bin", "CB_ANALYZE_BIN": MAIN + "/native/analyze/build/bin",
             "CB_SYMBOLS_BIN": MAIN + "/native/symbols/build/bin", "CB_INVERT_BIN": MAIN + "/native/invert/build/bin",
             "CB_DOSEXEC_BIN": MAIN + "/native/dosexec/build/bin", "CB_L2_BIN": MAIN + "/native/l2/build/bin",
             "CB_CORPUS": os.environ.get("CB_CORPUS", os.path.join(MAIN, "..", "Windows95", "design", "corpus", "files"))}.items():
    os.environ[k] = v
from codebreaker.base import settings as _s

_s.ROOT = MAIN
_s.FILE = os.path.join(MAIN, "settings.json")
_s._loaded = None
from codebreaker.verify import reconstruct
from codebreaker.store import functions

if os.environ.get("PY_NO_APIARITY"):
    # the older tables beside the corpus (design/apiarity/*.json) are carried into the Python's knowledge base; the C++ kit does not carry them
    from codebreaker.symbols import api_arguments as _aa

    _aa._apiarity = lambda stem: {}
path = sys.argv[1]
t0 = time.time()
sha, digest = reconstruct.run(path)
fs = functions.open()
known = fs.judged(sha, digest, reconstruct.SOURCE)
out = {"module": os.path.basename(path), "seconds": time.time() - t0, "functions": {}}
for addr, r in known.items():
    out["functions"][addr] = {k: r.get(k) for k in ("name", "verdict", "reason", "limits", "detail")}
json.dump(out, open(sys.argv[3], "w"), indent=1, sort_keys=True)
print("done", len(known), "functions in %.0fs" % out["seconds"])
