#!/usr/bin/env python3
"""The file-format readers in C++ (native/formats: formats.dll) held to the Python readers' answers.

    python tests/test_native_formats.py [--record]

The Python readers of PE, NE, LE and MZ/DOS are deleted (docs/NATIVE.md, step 4); before they
went, every fact native/formats/gate.py collects (every attribute and method's answer, with
types, or the exception's name) was identical on every corpus file, and each sample's facts were
recorded here as a digest. The C++ must still give exactly those facts, and must have been asked.
A synthetic damaged file checks failures too. (--record printed the digests, from the Python
readers.) Skipped without formats.dll; the corpus cases are skipped without the corpus."""
import hashlib
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
sys.path.insert(0, os.path.join(ROOT, "native", "formats"))

# (reader, sample) -> the digest of its facts as the Python reader gave them (gate-identical, 2026-11-04)
DIGESTS = {
    ('pe', '<damaged>'): '6473bf4c0dc5f0b5',
    ('ne', '<damaged>'): 'c75b6244b7c0478f',
    ('le', '<damaged>'): 'd54603272ac9f0df',
    ('dos', '<damaged>'): 'b442c3cc0d1b7cda',
    ('pe', 'lz32.dll'): 'aceb3c08b4440233',
    ('pe', 'notepad.exe'): 'd40d565f930daa7b',
    ('pe', 'kernel32.dll'): '7fab66c75a025dea',
    ('ne', 'lzexpand.dll'): '0d93c4b410f74184',
    ('ne', 'keyboard.drv'): '1a92372ae10f30ed',
    ('ne', 'gdi.exe'): '084a4d3c357c2a28',
    ('le', 'vmouse.vxd'): 'b02cd86e15de1948',
    ('le', 'vmm32.vxd'): '1e1e73eeba8675f5',
    ('dos', 'mode.com'): '6a0d9372e2798ae0',
    ('dos', 'command.com'): '2af7b63fb298e894',
}
DAMAGED = "<damaged>"


def digest(facts, path):
    """The facts' digest, the file's place (which differs between checkouts) left out."""
    text = repr(facts).replace(repr(path)[1:-1], "<path>")
    return hashlib.sha1(text.encode("utf-8", "backslashreplace")).hexdigest()[:16]


SAMPLES = {"pe": ["lz32.dll", "notepad.exe", "kernel32.dll"], "ne": ["lzexpand.dll", "keyboard.drv", "gdi.exe"],
           "le": ["vmouse.vxd", "vmm32.vxd"], "dos": ["mode.com", "command.com", "io.sys"]}


def main(argv):
    from codebreaker.formats import native
    if native.why_not():
        print("SKIP: formats.dll not usable (%s)" % native.why_not())
        return 0
    import gate
    from codebreaker.base import corpus
    record = "--record" in argv
    results = []

    def check(name, ok, why=""):
        results.append(ok)
        print("  %s %s%s" % ("ok  " if ok else "FAIL", name, "" if ok else "  " + why))

    def facts(reader, path, key):
        before = gate.SIDE_CALLS[0]
        got = digest(gate.side(reader, not record, gate.READERS[reader], path), path)   # recorded: the Python reader
        if record:
            print("    (%r, %r): %r," % (reader, key, got))
            return
        check("%s: %s as recorded" % (reader, key), got == DIGESTS.get((reader, key)) and gate.SIDE_CALLS[0] > before,
              "digest %s" % got)

    work = os.path.join(ROOT, "state", "work", "test_native_formats")
    os.makedirs(work, exist_ok=True)
    damaged = os.path.join(work, "damaged.exe")                 # MZ, a PE header cut short
    with open(damaged, "wb") as f:
        f.write(b"MZ" + bytes(0x3A) + b"\x40\x00\x00\x00" + b"PE\0\0" + bytes(10))
    for reader in ("pe", "ne", "le", "dos"):
        facts(reader, damaged, DAMAGED)
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
            facts(reader, path, name)
    os.remove(damaged)
    os.rmdir(work)
    fails = results.count(False)
    print("%d passed, %d skipped, %d failed" % (results.count(True), skipped, fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
