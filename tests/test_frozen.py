#!/usr/bin/env python3
"""Frozen cases (tests/frozen.py): every pass of the C text on real inputs it was once given, and
each format reader on a small retail binary. Each must make what it made when it was recorded.
Run:  python tests/test_frozen.py     (exit code 0 = all passed)
      python tests/test_frozen.py --slow   the descriptions and whole modules' C as well
                                           (tests/test_frozen_slow.py runs that part)

A difference is either a bug or an intended change; for the latter, record again
(tools/freeze_tests.py) and say in the commit what changed and why. Format readers' cases need
the corpus and are SKIPPED without it (or when the corpus holds a different file)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HERE = os.path.dirname(os.path.abspath(__file__))
for p in (ROOT, HERE):
    if p not in sys.path:
        sys.path.insert(0, p)

import frozen                                       # noqa: E402
import frozen_subjects                              # noqa: E402

RESULTS = []


class Skip(Exception):
    pass


def check(name, fn):
    try:
        fn()
        RESULTS.append((name, "ok", ""))
    except Skip as s:
        RESULTS.append((name, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((name, "FAIL", "%s: %s" % (type(e).__name__, e)))


def replay_pass(name):
    """Every kept case of one pass of the C text."""
    from codebreaker.decompile import c_text
    data = frozen.load("c_text", name)
    fn = getattr(c_text, name)
    if not data["cases"]:
        raise Skip("no case was recorded")
    for k, case in enumerate(data["cases"]):
        args = frozen.decode(case["args"])
        got = fn(*args, **frozen.decode(case["kwargs"]))
        diff = frozen.first_difference(case["result"], frozen.encode(got), "result")
        if diff:
            raise AssertionError("case %d: %s" % (k, diff))
        if "args_after" in case:
            diff = frozen.first_difference(case["args_after"], frozen.encode(args), "arguments after")
            if diff:
                raise AssertionError("case %d: %s" % (k, diff))


def replay_subject(group, name, where, make):
    data = frozen.load(group, name)
    path = where()
    if not path or not os.path.isfile(path):
        raise Skip("%s is not in the corpus" % data["file"])
    if frozen_subjects.sha256(path) != data["sha256"]:
        raise Skip("the corpus holds a different %s" % data["file"])
    got = frozen.decode(frozen.encode(make(path)))       # (as the frozen copy was kept: tuples are lists)
    diff = frozen.first_difference(frozen.decode(data["output"]), got, "output")
    if diff:
        raise AssertionError(diff)


def main(argv):
    slow = "--slow" in argv
    if slow:
        os.environ["CB_NO_CACHE"] = "1"        # (made now from the code as it is, not a kept result)
    fast = "--only-slow" not in argv
    if fast:
        for name in frozen.names("c_text"):
            check("c_text." + name, lambda name=name: replay_pass(name))
        for group, name, where, make in frozen_subjects.SUBJECTS:
            if group == "formats":
                check("%s.%s" % (group, name), lambda a=(group, name, where, make): replay_subject(*a))
    if slow:
        for group, name, where, make in frozen_subjects.SUBJECTS:
            if group != "formats":
                check("%s.%s" % (group, name), lambda a=(group, name, where, make): replay_subject(*a))
        from codebreaker.decompile import decompiler
        for group, name, where, make in frozen_subjects.MODULE_SUBJECTS:
            if not decompiler.available():
                RESULTS.append(("%s.%s" % (group, name), "SKIP", "the native decompiler is not built"))
                continue
            check("%s.%s" % (group, name), lambda a=(group, name, where, make): replay_subject(*a))
    if not RESULTS:
        print("no frozen cases: record them with tools/freeze_tests.py")
        return 1
    for name, status, detail in RESULTS:
        if status != "ok" or "-v" in argv:
            print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
