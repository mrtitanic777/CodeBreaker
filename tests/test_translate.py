#!/usr/bin/env python3
"""Tests for the universal translator (codebreaker/translate). Run:  python tests/test_translate.py
(exit code 0 = all passed)

The suite itself lives in tests/translate/ and is written for pytest, like tests/java/;
this script runs it and reports in the form tests/run_all.py reads. Checks that need a JDK or g++ are
skipped when they are not there.

Every test file of the folder runs as a pytest job of its own, `workers` of them side by side
(run_all.run_pytest_folder), under the compile cache (run_all.py, `--no-cache` or CB_TEST_CACHE=0 to bypass).
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import run_all                                              # noqa: E402

if __name__ == "__main__":
    sys.exit(run_all.run_pytest_folder("translate"))
