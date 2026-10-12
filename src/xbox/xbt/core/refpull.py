"""One copy: this is CodeBreaker's src/toolchain/reference/refpull.py -- the Xbox tool keeps its flat import name."""
import importlib
import os
import runpy
import sys

_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if _ROOT not in sys.path:
    sys.path.append(_ROOT)
from src.core import cores                                        # noqa: E402
cores.ensure_target([os.path.join(_ROOT, "src", "xbox", "xbt", d) for d in ("core", "win", "match")])
if __name__ == "__main__":
    runpy.run_module("src.toolchain.reference.refpull", run_name="__main__")
else:
    sys.modules[__name__] = importlib.import_module("src.toolchain.reference.refpull")
