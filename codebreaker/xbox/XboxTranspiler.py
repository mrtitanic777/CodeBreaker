#!/usr/bin/env python3
"""XboxTranspiler — top-level launcher.

Runs the packaged entry point. In source form it lives at xbt/cli/; in the
frozen (PyInstaller) build every module is unpacked flat into _MEIPASS, so the
entry is resolved differently for the two cases. All logic lives in the xbt/
package; this just gives `python XboxTranspiler.py <cmd>` a stable entry and
PyInstaller a stable script to freeze.
"""
import os, runpy, sys

if getattr(sys, "frozen", False):
    _ENTRY = os.path.join(sys._MEIPASS, "xboxtranspiler.py")
else:
    _HERE = os.path.dirname(os.path.abspath(__file__))
    _ENTRY = os.path.join(_HERE, "xbt", "cli", "xboxtranspiler.py")

sys.argv[0] = _ENTRY
runpy.run_path(_ENTRY, run_name="__main__")
