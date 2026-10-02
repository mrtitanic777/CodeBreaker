#!/usr/bin/env python3
r"""oracle30.py -- the Microsoft C 3.00 (1985) compile oracle.

Twin of oracle16.py (MS-C 2.03 MC1/MC2), but drives the MS-C 3.00 toolchain -- the CL
driver running the P0..P3 passes -- so the inverse engine can be ported from 2.03 to 3.0
and prove MASM 4.00 (MS-C 3.0-built) reconstructions byte-exact. Period DOS binaries you
supply run headless under DOSBox-X. Point config.json at them:

    "dosbox_x":      "<path to dosbox-x.exe>"
    "msc300_oracle": "<dir with CL.EXE/P0-P3.EXE/MSC.EXE, include/, lib/>"

`cl -c CAND.C` -> CAND.OBJ is the verify primitive; only the final byte-exact check needs
this. When unconfigured, available() is False and compiles raise a clear message.
"""
import os
import subprocess
from src.core import config as _config

_cfg = _config.load_config()
_ENGINE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_ROOT = os.path.dirname(_ENGINE)

DBX = _cfg.get("dosbox_x") or os.path.join(
    _ROOT, "toolchains", "dosbox-x", "bin", "x64", "Release", "dosbox-x.exe")
ORACLE = _cfg.get("msc300_oracle") or os.path.join(_ROOT, "reference", "msc300")


def available():
    """True when DOSBox-X and a staged MS-C 3.0 toolchain are both present."""
    return bool(ORACLE and os.path.isdir(ORACLE)
                and os.path.exists(os.path.join(ORACLE, "CL.EXE"))
                and os.path.exists(os.path.join(ORACLE, "P1.EXE"))
                and DBX and os.path.exists(DBX))


def _compile(stem, csrc, timeout):
    """Compile a C string with `cl -c` in one headless DOSBox run; return the .OBJ bytes
    (or None if no object was produced)."""
    if not available():
        raise RuntimeError(
            "MS-C 3.0 oracle not configured. Set 'dosbox_x' and 'msc300_oracle' (a dir with "
            "CL.EXE/P0-P3.EXE/MSC.EXE + include/ + lib/) in config.json.\n  ORACLE=%s\n  DBX=%s"
            % (ORACLE, DBX))
    stem = stem.upper()
    open(os.path.join(ORACLE, stem + ".C"), "w", newline="\r\n").write(csrc)
    for e in ("OBJ", "obj"):
        p = os.path.join(ORACLE, stem + "." + e)
        if os.path.exists(p):
            os.remove(p)
    open(os.path.join(ORACLE, "dbx.conf"), "w", newline="").write("\r\n".join(
        ["[cpu]", "core=auto", "cputype=386", "[autoexec]",
         'mount c "%s"' % ORACLE, "c:",
         "set PATH=c:\\", "set INCLUDE=c:\\include", "set LIB=c:\\lib", "set TMP=c:\\",
         "cl -c %s.C >CL.LOG 2>CL.ERR" % stem, "echo DONE >D.TXT", "exit", ""]))
    subprocess.run([DBX, "-conf", os.path.join(ORACLE, "dbx.conf"), "-fastlaunch"],
                   capture_output=True, timeout=max(timeout, 40),
                   env=dict(os.environ, SDL_VIDEODRIVER="dummy"))
    p = os.path.join(ORACLE, stem + ".OBJ")
    return open(p, "rb").read() if os.path.exists(p) else None


def mc_compile(csrc):
    """Compile one candidate function; return its .OBJ bytes."""
    return _compile("CAND", csrc, 90)


def mc_compile_multi(csrc):
    """Compile a battery of probe functions in one run; return the .OBJ bytes."""
    return _compile("PROBE", csrc, 120)
