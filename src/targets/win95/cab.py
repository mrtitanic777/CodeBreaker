"""One copy: this is CodeBreaker's src/formats/cab.py -- the win95 tools keep its flat import name."""
import importlib
import os
import runpy
import sys

_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
if _ROOT not in sys.path:
    sys.path.append(_ROOT)
if __name__ == "__main__":
    runpy.run_module("src.formats.cab", run_name="__main__")
else:
    sys.modules[__name__] = importlib.import_module("src.formats.cab")
