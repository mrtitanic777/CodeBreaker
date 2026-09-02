# -*- mode: python ; coding: utf-8 -*-
#
# CodeBreaker.spec -- package the unified workbench into a standalone hybrid exe.
#
#   pyinstaller CodeBreaker.spec     # -> dist/CodeBreaker.exe
#
# Hybrid: double-clicking the exe (no args) opens the native UI; running it with arguments
# (CodeBreaker.exe diff versions) behaves as the CLI, printing to the terminal it was launched
# from. It is a windowed build so a double-click shows no console; wt.py attaches the parent
# console on demand (see _attach_console_if_cli).
#
# Model: the ENTRY is wt.py (the --target dispatcher). wt.py runs each tool via
# runpy.run_path, so the tool CODE (engine/ + the per-target tool dirs) is bundled as
# DATA and executed from sys._MEIPASS at runtime -- not imported into the PYZ. The exe
# operates on an EXTERNAL data checkout (win95/src, files/, ...), which wt.py resolves
# from the working dir / $WT_REPO when frozen (see wt.py's frozen handling). Only the
# third-party dep (capstone) goes into the PYZ.
#
# COPYRIGHT-SAFE: this deliberately excludes vendored binaries (masm32bin/, dist/,
# build/) and caches (__pycache__, *.db), so no copyrighted Microsoft binary or large
# local artifact is ever baked into the shipped exe.
import os
from PyInstaller.utils.hooks import collect_all

HERE = SPECPATH                                          # CodeBreaker/ (dir of this spec)

_EXCLUDE_DIR = {"__pycache__", "masm32bin", "dist", "build"}
_KEEP_EXT = (".py", ".json", ".txt", ".html", ".sql", ".inc")

# The whole tool lives under engine/ (shared core + engine/targets/<era>/ submodules),
# so collecting engine/ recursively bundles every `wt --target <t>` toolchain.
_CODE_DIRS = ["engine"]


def collect_code(subdir):
    """Tool code + small data files under CodeBreaker/<subdir> as PyInstaller datas,
    preserving tree layout, skipping vendored binaries / build output / caches."""
    root = os.path.join(HERE, subdir)
    out = []
    if not os.path.isdir(root):
        return out
    for dp, dns, fns in os.walk(root):
        dns[:] = [d for d in dns if d not in _EXCLUDE_DIR]
        rel = os.path.relpath(dp, root)
        dest = subdir if rel == "." else os.path.join(subdir, rel)
        for f in fns:
            if f.endswith(_KEEP_EXT):
                out.append((os.path.join(dp, f), dest))
    return out


datas = []
for d in _CODE_DIRS:
    datas += collect_code(d)
if os.path.exists(os.path.join(HERE, "config.json")):
    datas.append((os.path.join(HERE, "config.json"), "."))

binaries = []
# clr = pythonnet, imported dynamically by the WinForms UI (bundled as data, so not seen by
# the import graph); listing it triggers pythonnet's own PyInstaller hook, which bundles
# Python.Runtime.dll + the .NET runtime so `wt ui` works from the frozen exe.
hiddenimports = ["clr"]
_c = collect_all("capstone")                            # the tools' one third-party dep
datas += _c[0]; binaries += _c[1]; hiddenimports += _c[2]

a = Analysis(
    ["wt.py"],
    pathex=[HERE, os.path.join(HERE, "engine")],
    binaries=binaries,
    datas=datas,
    hiddenimports=hiddenimports,
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)
pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name="CodeBreaker",
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,                                       # hybrid: windowed (no console flash on
                                                         # double-click -> UI); wt.py attaches the
                                                         # parent console when launched with args
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)
