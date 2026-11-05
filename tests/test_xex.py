#!/usr/bin/env python3
"""The XEX engine and its Python front end (codebreaker/formats/xex.py).

    python tests/test_xex.py

No .xex is in the repository, so what is checked here needs none: telling a .xex from other
files, reading the engine's listing (from a canned one), the header kept beside an extracted
basefile, corpus.retail leaving other files alone, and -- when native/xex is built -- the
engine's own crypto self-test and its command line."""
import json
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def main():
    from codebreaker.base import corpus
    from codebreaker.formats import xex
    work = tempfile.mkdtemp(prefix="cb_xex_")
    results = []

    def check(name, ok, why=""):
        results.append(ok)
        print("  %s %s%s" % ("ok  " if ok else "FAIL", name, "" if ok else "  " + why))

    x, pe = os.path.join(work, "a.xex"), os.path.join(work, "b.exe")
    with open(x, "wb") as f:
        f.write(b"XEX2" + bytes(60))
    with open(pe, "wb") as f:
        f.write(b"MZ" + bytes(62))
    check("is_xex", xex.is_xex(x) and not xex.is_xex(pe) and not xex.is_xex(os.path.join(work, "none")))

    with open(pe + ".json", "w", encoding="utf-8") as f:
        json.dump({"imports": ["xam.xex", "xboxkrnl.exe"]}, f)
    check("import_libraries", xex.import_libraries(pe) == ["xam.xex", "xboxkrnl.exe"] and
          xex.import_libraries(x) is None)
    check("corpus.retail leaves other files", corpus.retail(pe) == pe)

    import importlib
    importlib.reload(xex)
    try:
        exe = xex.program()
    except RuntimeError:
        exe = None
    if exe:
        real = subprocess.run
        r = real([exe, "selftest"], capture_output=True, text=True)
        check("engine selftest", r.returncode == 0 and "All crypto self-tests passed" in r.stdout, r.stdout[-200:])
        r = real([exe], capture_output=True, text=True)
        check("engine usage", "codebreaker xex <command>" in r.stdout, r.stdout[:200])
    else:
        print("  SKIP engine checks (native/xex not built)")

    passed, failed = results.count(True), results.count(False)
    print("%d passed, %d skipped, %d failed" % (passed, 0 if exe else 1, failed))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
