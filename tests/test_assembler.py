#!/usr/bin/env python3
"""The assembler engine (native/nasm): its MASM-mode regression suite, fixtures against their
golden bytes, flat and through the object formats (native/nasm/test/masm/run.py).

    python tests/test_assembler.py

Skipped when the engine is not built. A fixture listed in KNOWN fails today on the engine's
current source as on every build before it; it is reported, not counted as a regression."""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
SUITE = os.path.join(ROOT, "native", "nasm", "test", "masm", "run.py")
KNOWN = {"masm_paren_operand"}            # a parenthesised operand naming a later EQU: not yet


def main():
    from codebreaker.base import settings
    nasm = settings.get("nasm")
    if not nasm or not os.path.isfile(nasm):
        print("  SKIP the engine is not built (native/nasm/CMakeLists.txt)")
        print("0 passed, 1 skipped, 0 failed")
        return 0
    passed = failed = 0
    for extra in ([], ["--objects"]):
        r = subprocess.run([sys.executable, SUITE, "--nasm", nasm] + extra, capture_output=True, text=True)
        for line in r.stdout.splitlines():
            m = re.match(r"(PASS|FAIL) (\w+)", line)
            if not m:
                continue
            if m.group(1) == "PASS":
                passed += 1
            elif m.group(2) in KNOWN:
                print("  known %s" % line.strip()[:160])
            else:
                failed += 1
                print("  FAIL %s" % line.strip()[5:165])
    print("%d passed, 0 skipped, %d failed" % (passed, failed))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
