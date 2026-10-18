#!/usr/bin/env python3
"""Tests for the Java transpiler (codebreaker/java). Run:  python tests/test_java.py
(exit code 0 = all passed)

The suite itself lives in tests/java/ and is written for pytest (it came with the transpiler);
this script runs it and reports in the form tests/run_all.py reads. Checks that need g++ are
skipped when it is not there."""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    r = subprocess.run([sys.executable, "-m", "pytest", "-q", "-p", "no:cacheprovider",
                        os.path.join("tests", "java")], cwd=ROOT, capture_output=True, text=True)
    out = (r.stdout or "") + (r.stderr or "")
    if "No module named pytest" in out:
        print(out.strip().splitlines()[-1])
        print("0 passed, 0 skipped, 1 failed")
        return 1
    for line in out.splitlines():
        if line.startswith(("FAILED", "ERROR")):
            print("  FAIL " + line.split(" ", 1)[1])
    def count(word):
        m = re.search(r"(\d+) %s" % word, out.strip().splitlines()[-1] if out.strip() else "")
        return int(m.group(1)) if m else 0
    failed = count("failed") + count("error")
    print("%d passed, %d skipped, %d failed" % (count("passed"), count("skipped"), failed))
    return 1 if r.returncode or failed else 0


if __name__ == "__main__":
    sys.exit(main())
