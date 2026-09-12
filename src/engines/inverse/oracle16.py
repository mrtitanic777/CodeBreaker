#!/usr/bin/env python3
r"""oracle16.py -- the MC1/MC2 compile oracle (Microsoft C 2.03 = rebranded Lattice C).

The inverse engine proves a reconstruction byte-exact by RECOMPILING it with the very
compiler whose codegen it inverts, then diffing the bytes. MC1.EXE / MC2.EXE are period
DOS binaries you supply; they run headless under DOSBox-X. Point config.json at them:

    "dosbox_x":      "<path to dosbox-x.exe>"
    "msc203_oracle": "<dir containing MC1.EXE and MC2.EXE>"

No compiler ships with CodeBreaker -- only this harness that drives one you own. The rest
of the engine (inverse, forward model, disassembly) needs no oracle; only the verify step
here does. When it is not configured, `available()` is False and the compile calls raise a
clear message rather than guessing.
"""
import os
import subprocess
from src.core import config as _config

_cfg = _config.load_config()
_ENGINE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))   # CodeBreaker/engine
_ROOT = os.path.dirname(_ENGINE)                                         # CodeBreaker (repo root)

# Both paths come from config.json (see config.json.example); the defaults are only sensible
# repo-relative placeholders for a fresh checkout.
DBX = _cfg.get("dosbox_x") or os.path.join(_ROOT, "reference", "dosbox-x", "dosbox-x.exe")
ORACLE = _cfg.get("msc203_oracle") or os.path.join(_ROOT, "reference", "msc203")


def available():
    """True when DOSBox-X and an MC1/MC2 directory are both present."""
    return bool(ORACLE and os.path.isdir(ORACLE)
                and os.path.exists(os.path.join(ORACLE, "MC1.EXE"))
                and os.path.exists(os.path.join(ORACLE, "MC2.EXE"))
                and DBX and os.path.exists(DBX))


def _compile(stem, csrc, timeout):
    """Compile a C string through MC1->MC2 in one headless DOSBox run; return the .OBJ bytes
    (or None if the compile produced no object)."""
    if not available():
        raise RuntimeError(
            "MC1/MC2 oracle not configured. Set 'dosbox_x' and 'msc203_oracle' (a directory "
            "holding MC1.EXE/MC2.EXE) in config.json.\n  ORACLE=%s\n  DBX=%s" % (ORACLE, DBX))
    open(os.path.join(ORACLE, stem + ".c"), "w", newline="\r\n").write(csrc)
    for e in ("OBJ", "Q", "q"):
        p = os.path.join(ORACLE, stem + "." + e)
        if os.path.exists(p):
            os.remove(p)
    open(os.path.join(ORACLE, "dbx.conf"), "w", newline="").write("\r\n".join(
        ["[cpu]", "core=auto", "cputype=386", "[autoexec]",
         'mount c "%s"' % ORACLE, "c:",
         "mc1 %s.c >NUL" % stem, "mc2 %s >NUL" % stem, "echo DONE >D.TXT", "exit", ""]))
    subprocess.run([DBX, "-conf", os.path.join(ORACLE, "dbx.conf"), "-fastlaunch"],
                   capture_output=True, timeout=max(timeout, 30),
                   env=dict(os.environ, SDL_VIDEODRIVER="dummy"))
    p = os.path.join(ORACLE, stem + ".OBJ")
    return open(p, "rb").read() if os.path.exists(p) else None


def mc_compile(csrc):
    """Compile one candidate function; return its .OBJ bytes."""
    return _compile("cand", csrc, 90)


def mc_compile_multi(csrc):
    """Compile a battery of probe functions in one run; return the .OBJ bytes."""
    return _compile("probe", csrc, 120)
