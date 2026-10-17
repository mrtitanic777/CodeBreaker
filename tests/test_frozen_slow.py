#!/usr/bin/env python3
"""The slower frozen cases: descriptions (decompile/describe.py) of small retail modules, and two
whole modules' compilable C (decompile/c_text.py, through the native decompiler). See
tests/test_frozen.py.
Run:  python tests/test_frozen_slow.py     (exit code 0 = all passed)"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import test_frozen                                  # noqa: E402

if __name__ == "__main__":
    sys.exit(test_frozen.main(["--slow", "--only-slow"] + sys.argv[1:]))
