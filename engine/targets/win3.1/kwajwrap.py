#!/usr/bin/env python3
r"""
kwajwrap.py — wrap our source-rebuilt binaries as KWAJ method-0 (stored) install
files, so the distribution media literally CARRIES our bytes (the win95
cabwrite.py analog for 3.1's KWAJ media).

The retail 3.1 media stores each file KWAJ-compressed (method 3, LZ+Huffman) under
a `.??_` name (GDI.EX_, VGA.DR_). We do not have a method-3 *compressor*, but KWAJ
method 0 is *stored* (no compression) and SETUP's KWAJ decompressor handles it. We
reuse the SHIPPED header verbatim (its flags/name/ext/outlen already describe the
right output file) and only switch the method field to 0 and store our bytes — a
valid KWAJ file whose expansion is exactly our binary.

Every wrap is round-tripped through mscomp.kwaj_decompress and must equal the
input, so a bad wrap fails loudly here, never at install time.

    python kwajwrap.py --stage <outdir>   # build extracted/ mirror, our .EX_ swapped
    python kwajwrap.py FILE.EX_ ours.bin -o OUT.EX_    # wrap one
"""
import os
import struct
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)
# mscomp.py (KWAJ/SZDD codec) lives in the win3.1 DATA root, not the tool dir.
sys.path.insert(0, os.environ.get("WT_DATA_ROOT", os.getcwd()))

# staged binary (basename, lower) -> shipped compressed name in extracted/
BOOT_MAP = {
    "krnl386.exe": "KRNL386.EX_", "gdi.exe": "GDI.EX_", "user.exe": "USER.EX_",
    "system.drv": "SYSTEM.DR_", "vga.drv": "VGA.DR_", "keyboard.drv": "KEYBOARD.DR_",
    "mouse.drv": "MOUSE.DR_", "comm.drv": "COMM.DR_", "progman.exe": "PROGMAN.EX_",
}


def kwaj_store(template_exz, our_bytes):
    """Build a KWAJ method-0 (stored) file carrying our_bytes, reusing the shipped
    header from template_exz (its outlen/name/ext fields already describe the right
    output). Returns the wrapped bytes; raises if the header's outlen disagrees with
    our length (i.e. we are wrapping the wrong file)."""
    import mscomp
    ship = open(template_exz, "rb").read()
    info = mscomp.kwaj_parse_header(ship)
    if info.get("outlen") not in (None, len(our_bytes)):
        raise ValueError("outlen %s != our %d (wrong file?)" % (info["outlen"], len(our_bytes)))
    hdr = bytearray(ship[:info["data_offset"]])
    struct.pack_into("<H", hdr, 8, 0)              # method field -> 0 (stored)
    out = bytes(hdr) + our_bytes
    # round-trip: must decompress back to exactly our bytes
    back, _ = mscomp.kwaj_decompress(out)
    if back != our_bytes:
        raise ValueError("round-trip mismatch: stored KWAJ did not restore our bytes")
    return out


def stage(outdir, staged="staged/win31", src="extracted"):
    """Mirror `src` (the shipped compressed setup set) into `outdir`, replacing the
    boot-chain .EX_ files with our-authored KWAJ-stored versions. Everything else is
    shipped-passthrough (provably our bytes already). Reports provenance."""
    import shutil
    os.makedirs(outdir, exist_ok=True)
    ours = swapped = passthrough = 0
    for f in os.listdir(src):
        shutil.copy2(os.path.join(src, f), os.path.join(outdir, f))
        passthrough += 1
    for base, exz in sorted(BOOT_MAP.items()):
        op = os.path.join(staged, base)
        tp = os.path.join(src, exz)
        if not (os.path.exists(op) and os.path.exists(tp)):
            print("  [skip] %-14s (missing staged or template)" % base); continue
        wrapped = kwaj_store(tp, open(op, "rb").read())
        open(os.path.join(outdir, exz), "wb").write(wrapped)
        ours += 1; swapped += 1; passthrough -= 1
        print("  [ours] %-12s -> %-12s  %d B stored, round-trip OK"
              % (base, exz, len(wrapped)))
    print("\nstaged media: %s" % os.path.abspath(outdir))
    print("  our-authored KWAJ files: %d   shipped-passthrough: %d   total: %d"
          % (ours, passthrough, ours + passthrough))


def main():
    if "--stage" in sys.argv:
        stage(sys.argv[sys.argv.index("--stage") + 1])
        return
    if len(sys.argv) >= 4 and "-o" in sys.argv:
        tmpl, ours = sys.argv[1], sys.argv[2]
        out = sys.argv[sys.argv.index("-o") + 1]
        open(out, "wb").write(kwaj_store(tmpl, open(ours, "rb").read()))
        print("wrote %s (round-trip verified)" % out)
        return
    sys.exit(__doc__)


if __name__ == "__main__":
    main()
