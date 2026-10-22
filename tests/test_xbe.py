#!/usr/bin/env python3
"""The original Xbox: an XBE goes down the 32-bit Windows lane as the PE image
formats/xbox/xbe.py makes of it, its kernel calls named from the kernel's export map.

    python tests/test_xbe.py

The XBE is built here: a header, a code section that calls the kernel through the thunk table,
a data section holding the table and a string. The decompile check needs the native decompiler
(native/decompiler/build/bin); without it, it is SKIPPED."""
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker import platforms                 # noqa: E402
from codebreaker.formats.pe import open_pe        # noqa: E402
from codebreaker.formats.xbox import xbe          # noqa: E402

BASE, TEXT, DATA = 0x10000, 0x11000, 0x12000
RESULTS = []


def make_xbe(path):
    """A minimal retail XBE: entry `push offset msg; call [DbgPrint]; add esp,4; xor eax,eax; ret`."""
    code = (b"\x68" + struct.pack("<I", DATA + 0x10) + b"\xff\x15" + struct.pack("<I", DATA) +
            b"\x83\xc4\x04\x33\xc0\xc3")
    data = struct.pack("<II", 0x80000008, 0) + bytes(8) + b"hello, kernel\0"
    hdr = bytearray(0x1000)
    hdr[0:4] = b"XBEH"
    struct.pack_into("<I", hdr, 0x104, BASE)
    struct.pack_into("<I", hdr, 0x108, 0x1000)                  # size of headers
    struct.pack_into("<I", hdr, 0x10C, 0x3000)                  # size of image
    struct.pack_into("<I", hdr, 0x118, BASE + 0x200)            # certificate
    struct.pack_into("<I", hdr, 0x11C, 2)                       # sections
    struct.pack_into("<I", hdr, 0x120, BASE + 0x400)            # section headers
    struct.pack_into("<I", hdr, 0x128, TEXT ^ xbe.ENTRY_KEYS["retail"])
    struct.pack_into("<I", hdr, 0x148, DATA ^ xbe.THUNK_KEYS["retail"])
    struct.pack_into("<I", hdr, 0x200 + 0x08, 0x4D530001)
    hdr[0x20C:0x20C + 8] = "Demo".encode("utf-16-le")
    for i, (va, body, flags, name_at) in enumerate(((TEXT, code, 0x6, 0x480), (DATA, data, 0x1, 0x488))):
        struct.pack_into("<IIIIII", hdr, 0x400 + 0x38 * i, flags, va, 0x1000, 0x1000 * (i + 1), len(body),
                         BASE + name_at)
    hdr[0x480:0x486] = b".text\0"
    hdr[0x488:0x48E] = b".data\0"
    blob = bytes(hdr) + code.ljust(0x1000, b"\0") + data.ljust(0x1000, b"\0")
    open(path, "wb").write(blob)


def main():
    path = os.path.join(tempfile.mkdtemp(prefix="cb_xbe_"), "default.xbe")
    make_xbe(path)
    checks = []
    p = platforms.identify(path)
    checks.append(("an XBE is the original Xbox", p is not None and p.name == "xbe"))
    pe_path = p.readable(path)
    pe = open_pe(pe_path)
    checks.append(("the PE: x86, the XBE's base and entry", (pe.machine, pe.image_base, pe.image_base + pe.entry) ==
                   (0x014C, BASE, TEXT)))
    imports = pe.imports()
    checks.append(("the kernel thunk table is xboxkrnl.exe's imports",
                   ("xboxkrnl.exe", "#8") in set(imports.values())))
    from codebreaker.symbols import ordinals
    checks.append(("kernel ordinal 8 is DbgPrint", ordinals.name("XBOXKRNL", 8) == "DbgPrint"))
    from codebreaker.verify import reconstruct
    checks.append(("its image goes down the 32-bit lane", reconstruct.kind_name(pe_path) == "pe"))
    exe = os.path.join(ROOT, "native", "decompiler", "build", "bin", "decompiler" + (".exe" if os.name == "nt" else ""))
    if os.path.isfile(exe):
        from codebreaker.decompile import decompiler
        text, done, _total = decompiler.as_c(pe_path)
        checks.append(("decompiles, the call named", done >= 1 and "DbgPrint(" in text))
    else:
        RESULTS.append("  SKIP decompiles (no decompiler built)")
    for name, ok in checks:
        print("  %-4s %s" % ("ok" if ok else "FAIL", name))
    for line in RESULTS:
        print(line)
    fails = sum(1 for _n, ok in checks if not ok)
    print("%d passed, %d skipped, %d failed" % (len(checks) - fails, len(RESULTS), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
