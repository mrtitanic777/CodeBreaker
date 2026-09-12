#!/usr/bin/env python3
"""
build_exe.py — freeze the whole workbench into one standalone XboxTranspiler.exe.

Runs PyInstaller with xboxtranspiler.py as the entry point and BUNDLES every tool
script + plugin + our small derived data into the binary. Auto-discovers the
tool set, so adding a new tool (top-level .py) or plugin (plugins/*.py) and
re-running this is all it takes to bake it into the EXE — the whole point of
the extensible design.

    python build_exe.py            # produce dist/WinTranspiler.exe (onefile)
    python build_exe.py --onedir   # faster-starting folder build instead

NOT bundled (driven by path — see `win95re config`): Ghidra, DOSBox-X, the
period compilers, and the ~300 MB copyrighted reference corpus. Baking those
in is neither wise (size) nor legal (MS source). The EXE finds them at runtime.
"""
import glob
import os
import subprocess
import sys

CLI_DIR = os.path.dirname(os.path.abspath(__file__))     # xbt/cli/
PKG = os.path.dirname(CLI_DIR)                          # xbt/
ROOT = os.path.dirname(PKG)                             # repo root
SEP = ";" if os.name == "nt" else ":"          # PyInstaller add-data separator

# Every module across the package's domain subpackages. They are bundled FLAT
# into the frozen _MEIPASS so the same flat import namespace the source tree's
# path-injection provides is reproduced at runtime (the entry point scans
# _MEIPASS for its module index). Absolute source paths, flat destination.
EXCLUDE = {"build_exe.py", "__init__.py"}
TOOL_PY = []
DATA_BLOBS = []          # non-.py files a tool loads at runtime (e.g. templates)
for sub in ("core", "xbox", "ppc", "win", "match", "assets", "crypto", "cli", "gui"):
    d = os.path.join(PKG, sub)
    if os.path.isdir(d):
        TOOL_PY += [p for p in glob.glob(os.path.join(d, "*.py"))
                    if os.path.basename(p) not in EXCLUDE]
        # tools resolve these next to their .py (dirname(__file__)); the frozen
        # EXE flattens everything into _MEIPASS, so bundle them flat too, or the
        # tool can't find its template at runtime (e.g. god.py's empty_live.bin).
        DATA_BLOBS += glob.glob(os.path.join(d, "*.bin"))

# small, ours, worth embedding so the EXE is self-sufficient for them.
DATA_FILES = [os.path.join(CLI_DIR, f) for f in ("dashboard.html", "ORDINALS.md") if os.path.exists(os.path.join(CLI_DIR, f))]
# masm32bin ships the 386-capable MASM 5.10A the PE (asmrecon32) path drives; it
# must ride along so the frozen EXE can reconstruct 32-bit modules.
DATA_DIRS = [os.path.join(CLI_DIR, d) for d in ("plugins", "ghidra_scripts", "assets", "masm32bin")
             if os.path.isdir(os.path.join(CLI_DIR, d))]
# crypto DLLs (LZX) live next to the crypto modules; bundle flat under bin/.
if os.path.isdir(os.path.join(PKG, "crypto", "bin")):
    DATA_DIRS.append(os.path.join(PKG, "crypto", "bin"))
# a curated slice of reference/ that's ours + tiny (derived tables), not the corpus.
REF_KEEP = [os.path.join(ROOT, f) for f in ("reference/vmm_services.txt",) if os.path.exists(os.path.join(ROOT, f))]


def main():
    onefile = "--onedir" not in sys.argv
    add = []
    # PyInstaller 6.x: single '--add-data=SRC{sep}DST' token; DST is RELATIVE.
    for f in TOOL_PY + DATA_FILES + DATA_BLOBS:
        add += ["--add-data=%s%s." % (f, SEP)]                       # flat into _MEIPASS root
    for d in DATA_DIRS:
        add += ["--add-data=%s%s%s" % (d, SEP, os.path.basename(d))] # dir -> _MEIPASS/<name>
    for f in REF_KEEP:
        add += ["--add-data=%s%sreference" % (f, SEP)]

    exe_icon = os.path.join(ROOT, "assets", "w2k_defragment.ico")   # the app's file icon
    icon_args = ["--icon", exe_icon] if os.path.exists(exe_icon) else []
    # Ship a fast AES for the XEX decrypt front-end if one is installed; the
    # tool still runs without it (xex.py has a pure-Python AES fallback) but the
    # frozen EXE is much faster on retail images with a real crypto lib bundled.
    crypto_args = []
    for m in ("Crypto", "cryptography"):
        try:
            __import__(m)
            crypto_args += ["--collect-all", m]
            break
        except Exception:
            pass

    # The GUI (gui.py) is bundled as a tool and imported at runtime, so its Tk
    # dependency must be pulled in explicitly -- otherwise the frozen EXE's Python
    # has no tkinter and `gui` silently falls back to the progress monitor. Fail
    # the build loudly if Tk is missing here rather than shipping a GUI-less EXE.
    try:
        import tkinter, _tkinter  # noqa: F401
    except Exception as e:
        print("WARNING: this Python has no working tkinter (%s).\n"
              "         The built EXE will not be able to open the GUI.\n"
              "         Install Tk (Windows: the python.org installer includes it) "
              "and rebuild." % e)
    gui_imports = []
    for m in ("tkinter", "tkinter.ttk", "tkinter.filedialog",
              "tkinter.messagebox", "tkinter.font", "_tkinter", "PIL.ImageTk"):
        gui_imports += ["--hidden-import", m]

    cmd = [sys.executable, "-m", "PyInstaller",
           "--onefile" if onefile else "--onedir",
           "--name", "XboxTranspiler",
           "--console",
           "--noconfirm",
           "--collect-all", "capstone",       # C-ext package (x86 + PowerPC), via runpy
           "--collect-all", "PIL",            # lazily imported for the working gif
           "--collect-all", "cryptography",   # xbt/crypto tools (GoobyCorp) need pyca
           "--collect-all", "tkinter",        # GUI: pull the Tcl/Tk runtime + data
           ] + gui_imports + crypto_args + icon_args + add + [
           # tools are import-ed by each other at runtime; ship them in the PYZ too
           ] + sum((["--hidden-import", os.path.basename(f)[:-3]] for f in TOOL_PY), []) + [
           os.path.join(ROOT, "XboxTranspiler.py")]

    print("bundling %d tools + %d plugins-dirs + %d data files"
          % (len(TOOL_PY), len(DATA_DIRS), len(DATA_FILES) + len(REF_KEEP)))
    print(" ".join(cmd), "\n")
    r = subprocess.run(cmd, cwd=ROOT)
    if r.returncode == 0:
        exe = os.path.join(ROOT, "dist", "XboxTranspiler.exe" if os.name == "nt" else "XboxTranspiler")
        print("\nBUILT:", exe)
        print("smoke-test it:  %s doctor" % exe)
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
