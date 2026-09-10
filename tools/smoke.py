#!/usr/bin/env python3
r"""smoke.py -- the restructure regression oracle.

Verifies the tool still imports and dispatches after each restructure stage. It checks the
failure mode a restructure actually risks -- broken imports / path resolution -- not deep
behavior (that needs the external toolchains + corpus and is the user's end-to-end check).

Three layers:
  1. `python test/selftest.py` exits 0 (import health of the 7 core subsystems).
  2. Every shared subpackage + a representative module imports cleanly.
  3. Every `wt <subsystem> list`/help + `wt doctor`/`targets` runs without a Python traceback
     (non-zero exit is tolerated -- missing external tools are fine; a traceback is not).

    python tools/smoke.py        # exit 0 = all green, 1 = a regression

Run it from the repo root. Safe: no external toolchains, no data mutation, no GUI.
"""
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

# (2) representative import per subpackage -- adjust the dotted path if modules are renamed.
IMPORTS = [
    "codebreaker.foundation.config", "codebreaker.foundation.cores", "codebreaker.foundation.layout",
    "codebreaker.format_loaders.pe", "codebreaker.format_loaders.ne",
    "codebreaker.disasm_engine.disasm", "codebreaker.ir_engine.il",
    "codebreaker.inverse_engine", "codebreaker.recon_engine", "codebreaker.behavior_engine.tracer",
    "codebreaker.behavior_engine.synth", "codebreaker.match_engine", "codebreaker.symbol_engine",
    "codebreaker.build_engine", "codebreaker.backbone", "codebreaker.sdk.apidb", "codebreaker.diff.cli",
    "codebreaker.coverage", "codebreaker.studio.app",
]

# (3) command-surface checks. (argv, needs_no_data). A traceback in output = FAIL.
COMMANDS = [
    ["doctor"], ["targets"],
    ["behavior", "list"], ["recompile", "list"], ["inverse", "list"], ["msc6", "list"],
    ["sdk"], ["backbone"], ["--target", "win95", "list"],
]

WT = os.path.join(ROOT, "wt.py")


def check_imports():
    out = []
    for mod in IMPORTS:
        try:
            __import__(mod)
            out.append((mod, True, ""))
        except Exception as e:
            out.append((mod, False, "%s: %s" % (type(e).__name__, str(e)[:120])))
    return out


def check_commands():
    out = []
    for argv in COMMANDS:
        try:
            r = subprocess.run([sys.executable, WT] + argv, capture_output=True, text=True,
                               timeout=120, cwd=ROOT)
            blob = (r.stdout or "") + (r.stderr or "")
            tb = "Traceback (most recent call last)" in blob or "ModuleNotFoundError" in blob \
                or "ImportError" in blob
            out.append((" ".join(argv), not tb, "" if not tb else blob.strip().splitlines()[-1][:140]))
        except Exception as e:
            out.append((" ".join(argv), False, "%s: %s" % (type(e).__name__, str(e)[:120])))
    return out


def check_selftest():
    try:
        r = subprocess.run([sys.executable, os.path.join(ROOT, "test", "selftest.py")],
                           capture_output=True, text=True, timeout=300, cwd=ROOT)
        return r.returncode == 0, (r.stdout or r.stderr).strip().splitlines()[-1][:140]
    except Exception as e:
        return False, "%s: %s" % (type(e).__name__, str(e)[:120])


def main():
    fails = 0
    print("== selftest ==")
    ok, msg = check_selftest()
    print("  [%s] test/selftest.py   %s" % ("PASS" if ok else "FAIL", msg))
    fails += not ok

    print("== imports ==")
    for mod, ok, msg in check_imports():
        if not ok:
            print("  [FAIL] %-38s %s" % (mod, msg)); fails += 1
    print("  %d imports checked" % len(IMPORTS))

    print("== commands (no traceback expected) ==")
    for cmd, ok, msg in check_commands():
        print("  [%s] wt %-22s %s" % ("PASS" if ok else "FAIL", cmd, msg))
        fails += not ok

    print("\n%s  (%d failure%s)" % ("ALL GREEN" if not fails else "REGRESSION",
                                    fails, "" if fails == 1 else "s"))
    sys.exit(1 if fails else 0)


if __name__ == "__main__":
    main()
