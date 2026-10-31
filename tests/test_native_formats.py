#!/usr/bin/env python3
"""The file-format readers in C++ (native/formats: formats.dll) against the Python readers.

    python tests/test_native_formats.py

Each reader's facts (native/formats/gate.py: every attribute and method's answer, with types,
or the exception's name) on a few corpus files, with the reader's switch off and on: they must
be identical, and the C++ side must have been asked. A synthetic damaged file checks failures
match too. Skipped without formats.dll; the corpus cases are skipped without the corpus."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
sys.path.insert(0, os.path.join(ROOT, "native", "formats"))

SAMPLES = {"pe": ["lz32.dll", "notepad.exe", "kernel32.dll"], "ne": ["lzexpand.dll", "keyboard.drv", "gdi.exe"],
           "le": ["vmouse.vxd", "vmm32.vxd"], "dos": ["mode.com", "command.com", "io.sys"],
           "omf": [], "xex": []}


def main():
    from codebreaker.formats import native
    if native.why_not():
        print("SKIP: formats.dll not usable (%s)" % native.why_not())
        return 0
    import gate
    from codebreaker.base import corpus
    results = []

    def check(name, ok, why=""):
        results.append(ok)
        print("  %s %s%s" % ("ok  " if ok else "FAIL", name, "" if ok else "  " + why))

    def same(reader, path):
        before = gate.SIDE_CALLS[0]
        a = gate.side(reader, False, gate.READERS[reader], path)
        b = gate.side(reader, True, gate.READERS[reader], path)
        return a == b and gate.SIDE_CALLS[0] > before

    work = tempfile.mkdtemp(prefix="cb_natfmt_", dir=os.environ.get("TEMP"))
    damaged = os.path.join(work, "damaged.exe")                 # MZ, a PE header cut short
    with open(damaged, "wb") as f:
        f.write(b"MZ" + bytes(0x3A) + b"\x40\x00\x00\x00" + b"PE\0\0" + bytes(10))
    for reader in ("pe", "ne", "le", "dos"):
        check("%s: a damaged file fails alike" % reader, same(reader, damaged))
    skipped = 0
    for reader, names in SAMPLES.items():
        for name in names:
            try:
                path = corpus.resolve(name)
            except Exception:                                   # noqa: BLE001
                path = None
            if not path or not os.path.isfile(path):
                skipped += 1
                continue
            check("%s: %s identical" % (reader, name), same(reader, path))
    os.remove(damaged)
    os.rmdir(work)
    fails = results.count(False)
    print("%d passed, %d skipped, %d failed" % (results.count(True), skipped, fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
