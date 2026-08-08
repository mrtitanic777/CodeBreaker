#!/usr/bin/env python3
"""
build_exe.py — freeze the whole workbench into one standalone CodeBreaker.exe.

Runs PyInstaller with win95re.py as the entry point and BUNDLES every tool
script + plugin + our small derived data into the binary. Auto-discovers the
tool set, so adding a new tool (top-level .py) or plugin (plugins/*.py) and
re-running this is all it takes to bake it into the EXE — the whole point of
the extensible design.

    python build_exe.py            # produce dist/CodeBreaker.exe (onefile)
    python build_exe.py --onedir   # faster-starting folder build instead

NOT bundled (driven by path — see `win95re config`): Ghidra, DOSBox-X, the
period compilers, and the ~300 MB copyrighted reference corpus. Baking those
in is neither wise (size) nor legal (MS source). The EXE finds them at runtime.
"""
import glob
import os
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "engine"))
from engine.foundation import layout

ROOT = os.path.dirname(os.path.abspath(__file__))
SEP = ";" if os.name == "nt" else ":"          # PyInstaller add-data separator

# top-level .py that are NOT the entry or the build tooling: every tool + lib.
# NOTE: the CLI dispatcher is now the shared ../wt.py (win95re.py retired). This
# per-target EXE builder still needs a rework to bundle wt.py's multi-target model
# (engine/ + all target dirs); the entry below points at wt.py so it references a
# live file, but a full frozen build has not been re-validated post-integration.
EXCLUDE = {"build_exe.py"}
TOOL_PY = [os.path.basename(p) for p in glob.glob(os.path.join(ROOT, "*.py"))
           if os.path.basename(p) not in EXCLUDE]

# small, ours, worth embedding so the EXE is self-sufficient for them.
DATA_FILES = [f for f in ("dashboard.html", "ORDINALS.md") if os.path.exists(os.path.join(ROOT, f))]
# masm32bin ships the 386-capable MASM 5.10A the PE (asmrecon32) path drives; it
# must ride along so the frozen EXE can reconstruct 32-bit modules.
DATA_DIRS = [d for d in ("plugins", "ghidra_scripts", "assets", "masm32bin")
             if os.path.isdir(os.path.join(ROOT, d))]
# a curated slice of reference/ that's ours + tiny (derived tables), not the corpus.
REF_KEEP = [f for f in (layout.reference_sub("vmm_services.txt"),) if os.path.exists(os.path.join(ROOT, f))]


def main():
    onefile = "--onedir" not in sys.argv
    add = []
    for f in TOOL_PY + DATA_FILES:
        add += ["--add-data", "%s%s." % (f, SEP)]
    for d in DATA_DIRS:
        add += ["--add-data", "%s%s%s" % (d, SEP, d)]
    for f in REF_KEEP:
        add += ["--add-data", "%s%s%s" % (f, SEP, os.path.dirname(f))]

    exe_icon = os.path.join(ROOT, "assets", "w2k_defragment.ico")   # the app's file icon
    icon_args = ["--icon", exe_icon] if os.path.exists(exe_icon) else []
    cmd = [sys.executable, "-m", "PyInstaller",
           "--onefile" if onefile else "--onedir",
           "--name", "CodeBreaker",
           # WINDOWED: no console window is ever spawned. All report/log text is
           # streamed into the app's own UI log pane (progress.capture_output).
           # A console appears only for debugging (WT_CONSOLE=1) or when launched
           # from an existing terminal for a CLI command (attaches to the parent).
           "--windowed",
           "--noconfirm",
           "--collect-all", "capstone",       # C-ext package, imported via runpy
           "--collect-all", "PIL",            # lazily imported for the working gif
           ] + icon_args + add + [
           # tools are import-ed by each other at runtime; ship them in the PYZ too
           ] + sum((["--hidden-import", f[:-3]] for f in TOOL_PY), []) + [
           os.path.join(os.path.dirname(ROOT), "wt.py")]   # entry: the shared wt.py dispatcher

    print("bundling %d tools + %d plugins-dirs + %d data files"
          % (len(TOOL_PY), len(DATA_DIRS), len(DATA_FILES) + len(REF_KEEP)))
    print(" ".join(cmd), "\n")
    r = subprocess.run(cmd, cwd=ROOT)
    if r.returncode == 0:
        exe = os.path.join(ROOT, "dist", "CodeBreaker.exe" if os.name == "nt" else "CodeBreaker")
        print("\nBUILT:", exe)
        print("smoke-test it:  %s doctor" % exe)
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
