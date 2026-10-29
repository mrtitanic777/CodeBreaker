#!/usr/bin/env python3
"""Run every test suite and exit non-zero if any of them fails.

    python tests/run_all.py            # all suites
    python tests/run_all.py --quick    # skip the slower end-to-end suites

Use this as the gate before committing:  python tests/run_all.py && git commit ...
(Piping a single suite through `tail` hides its exit code; this script does not.)"""
import os
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
SUITES = [("../tools/check_imports.py", False), ("../tools/tidy.py", False), ("test_plumbing.py", False), ("test_ui.py", False), ("test_mcp.py", False), ("test_service.py", False), ("test_symbols.py", False), ("test_dos.py", False), ("test_analysis.py", False), ("test_drivers.py", False), ("test_versions.py", False), ("test_powerpc.py", False), ("test_xex.py", False), ("test_vmx128.py", False), ("test_match_powerpc.py", False), ("test_xenos.py", False), ("test_platforms.py", False), ("test_nes.py", False), ("test_z80.py", False), ("test_m68k.py", False), ("test_xbe.py", False), ("test_recompile.py", False), ("test_assembler.py", False), ("test_media.py", False), ("test_xbox_formats.py", False), ("test_xbox_crypto.py", False), ("test_xex_edit.py", False), ("test_ppc_calls.py", False), ("test_xna.py", False), ("test_xbox_effect.py", False), ("test_whole_module.py", False), ("test_invert.py", False), ("test_readable.py", False), ("test_java.py", False), ("test_translate.py", False),
          ("test_frozen.py", False), ("test_port.py", False), ("test_converted.py", False), ("test_exepack.py", False),
          ("test_pklite.py", False), ("test_x360.py", True), ("test_frozen_slow.py", True), ("test_verification.py", True)]


def main(argv):
    quick = "--quick" in argv
    env = dict(os.environ, CB_NO_FP_CACHE="1")        # verdicts must reflect the current code
    failed = []
    for name, slow in SUITES:
        if quick and slow:
            print("-- %-28s skipped (--quick)" % name)
            continue
        t0 = time.time()
        path = os.path.join(HERE, name)
        with open(path, encoding="utf-8") as f:                  # a file of pytest tests only: run by pytest
            own = "__main__" in f.read()
        command = [sys.executable, path] if own else [sys.executable, "-m", "pytest", "-q", "-p", "no:cacheprovider", path]
        r = subprocess.run(command, capture_output=True, text=True, env=env, cwd=os.path.dirname(HERE))
        lines = [ln for ln in (r.stdout or "").strip().splitlines() if ln.strip()]
        summary = next((ln.strip() for ln in reversed(lines) if "passed" in ln), "(no summary line)")
        print("%s %-28s %s  (%.0fs)" % ("ok" if r.returncode == 0 else "FAIL", name, summary,
                                        time.time() - t0))
        if r.returncode != 0:
            failed.append(name)
            for ln in lines:
                if "FAIL" in ln or ln.startswith("       "):
                    print("     " + ln.strip()[:200])
            if r.stderr.strip():
                print("     " + r.stderr.strip().splitlines()[-1][:200])
    print("ALL SUITES PASSED" if not failed else "FAILED: " + ", ".join(failed))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
