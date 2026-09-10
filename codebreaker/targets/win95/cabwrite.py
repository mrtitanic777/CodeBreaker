#!/usr/bin/env python3
r"""
cabwrite.py -- write a FUNCTIONAL Microsoft cabinet (MSCF) from a set of files.

The inverse of cab.py's reader. Packs files into a valid MSCF cabinet using MSZIP
(raw deflate framed with the 'CK' signature) or stored (uncompressed) blocks, so
period tools -- Windows EXTRACT.EXE / expand.exe and Win95 SETUP -- can extract our
byte-exact reconstructed binaries. NOT byte-identical to Microsoft's cabinets:
QUANTUM is unreproducible and MS's exact deflate isn't matched. The point is
functional media that DELIVERS byte-identical files, not a bit-identical CD.

    python cabwrite.py OUT.CAB file1 file2 ...
    python cabwrite.py --selftest
"""
import os
import struct
import sys
import zlib

MSZIP, STORED = 1, 0
# CFDATA uncompressed block size. The MSZIP CFDATA payload ('CK' + raw deflate) MUST NOT
# exceed 32768 bytes -- Win95 SETUP's MSZIP decompressor reads into a fixed 32768-byte
# window and OVERRUNS on any larger block, reporting "corrupt Setup (.CAB) file". A full
# 32768-byte INCOMPRESSIBLE block can't satisfy that: even deflate's STORED encoding adds
# ~5 bytes + the 2-byte 'CK', so it lands at ~32775. So cap the uncompressed block low
# enough that the worst case (stored) still fits: 32756 = 32768 - 12. (MS avoided this
# entirely by using QUANTUM, not MSZIP, for the incompressible setup*.exe span.)
_BLOCK = 32756


def _mszip_block(chunk):
    """One MSZIP CFDATA payload: 'CK' + a self-contained raw-deflate stream, guaranteed
    <= 32768 bytes. If normal compression EXPANDS an (incompressible) block, re-emit it
    with deflate STORED blocks (level 0) so it can't grow past the decode window."""
    co = zlib.compressobj(9, zlib.DEFLATED, -15)   # raw deflate, no zlib header
    body = co.compress(chunk) + co.flush(zlib.Z_FINISH)
    if len(body) > len(chunk):                     # compression expanded it -> store raw
        co = zlib.compressobj(0, zlib.DEFLATED, -15)   # level 0 = deflate STORED blocks
        body = co.compress(chunk) + co.flush(zlib.Z_FINISH)
    return b"CK" + body


def _chk(data, seed):
    """The CAB checksum primitive (libmspack cabd_checksum): XOR little-endian
    32-bit words, fold the 1-3 byte tail little-endian, XOR the running seed."""
    ul = 0
    n = len(data)
    i = 0
    for _ in range(n >> 2):
        ul ^= data[i] | (data[i + 1] << 8) | (data[i + 2] << 16) | (data[i + 3] << 24)
        i += 4
    rem = n & 3                                    # tail folds BIG-endian: the first
    if rem == 3:                                    # tail byte goes to the highest slot
        ul ^= (data[i] << 16) | (data[i + 1] << 8) | data[i + 2]
    elif rem == 2:
        ul ^= (data[i] << 8) | data[i + 1]
    elif rem == 1:
        ul ^= data[i]
    return (seed ^ ul) & 0xFFFFFFFF


def _cfdata_cksum(payload, cbUncomp):
    """CFDATA checksum: ONE pass over the 4-byte (cbData, cbUncomp) block header
    FOLLOWED BY the data, seed 0 -- the order matters (the primitive is word-
    aligned), and it's exactly what EXTRACT.EXE/expand.exe verify."""
    return _chk(struct.pack("<HH", len(payload), cbUncomp) + payload, 0)


def write_cab(files, out_path, compress=MSZIP, set_id=0, cab_index=0,
              prev=None, nxt=None):
    """files: [(name, bytes)]. Writes an MSCF cabinet to out_path.

    set_id / cab_index and prev/nxt = (cabinet_name, disk_name) preserve a
    cabinet SET so period tools that walk the chain (Win95 SETUP's precopy) find
    files split across cabinets. Omit for a standalone cabinet."""
    folder = b"".join(data for _n, data in files)   # one folder, files concatenated
    # split into CFDATA blocks
    blocks = []
    for i in range(0, len(folder) or 1, _BLOCK):
        chunk = folder[i:i + _BLOCK]
        payload = _mszip_block(chunk) if compress == MSZIP else chunk
        blocks.append((payload, len(chunk)))
    if not folder:
        blocks = [(b"CK" if compress == MSZIP else b"", 0)]

    n_folders, n_files = 1, len(files)
    # cabinet-set chain strings (prev/next cabinet name + disk name), right after
    # the 36-byte header, before the folder records.
    flags = (0x0001 if prev else 0) | (0x0002 if nxt else 0)
    chain = b""
    for pair in (prev, nxt):
        if pair:
            cab, disk = pair
            chain += cab.encode("ascii", "replace") + b"\0" + disk.encode("ascii", "replace") + b"\0"
    cffile_off = 36 + len(chain) + 8 * n_folders
    # CFFILE records
    cffiles = bytearray()
    uoff = 0
    for name, data in files:
        nm = name.encode("ascii", "replace") + b"\0"
        cffiles += struct.pack("<IIHHHH", len(data), uoff, 0, 0x21, 0, 0x20) + nm
        uoff += len(data)
    cfdata_off = cffile_off + len(cffiles)
    # CFDATA blocks
    cfdata = bytearray()
    for payload, ucomp in blocks:
        cfdata += struct.pack("<IHH", _cfdata_cksum(payload, ucomp), len(payload), ucomp) + payload

    total = cfdata_off + len(cfdata)
    hdr = struct.pack("<4sIIIIIBBHHHHH", b"MSCF", 0, total, 0, cffile_off, 0,
                      3, 1, n_folders, n_files, flags, set_id, cab_index)
    folder_rec = struct.pack("<IHH", cfdata_off, len(blocks), compress)
    with open(out_path, "wb") as f:
        f.write(hdr + chain + folder_rec + bytes(cffiles) + bytes(cfdata))
    return total


def _selftest():
    import tempfile, subprocess
    files = [("hello.txt", b"Hello, Windows 95!\n" * 100),
             ("bin.dat", bytes(range(256)) * 300)]
    d = tempfile.mkdtemp()
    cabp = os.path.join(d, "TEST.CAB")
    write_cab(files, cabp)
    print("wrote %s (%d bytes)" % (cabp, os.path.getsize(cabp)))
    # round-trip via our own reader + verify contents
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import cab
    info, cabfiles = cab.read_cab(cabp)
    print("read back: %d files, compress=%s" % (len(cabfiles), info["compress"]))
    # extract with the real Windows expand.exe (not Git-Bash's unix `expand`)
    ok = True
    exp = os.path.join(os.environ.get("SystemRoot", r"C:\Windows"), "System32", "expand.exe")
    outd = os.path.join(d, "out")
    os.makedirs(outd, exist_ok=True)
    r = subprocess.run([exp, cabp, "-F:*", outd], capture_output=True, text=True)
    for name, data in files:
        p = os.path.join(outd, name)
        got = open(p, "rb").read() if os.path.exists(p) else None
        match = got == data
        ok = ok and match
        print("  expand %-10s -> %s" % (name, "BYTE-IDENTICAL" if match else "MISMATCH/missing"))
    print("SELFTEST:", "PASS" if ok else "FAIL", "(expand.exe:", r.returncode, ")")
    return ok


def main():
    if "--selftest" in sys.argv:
        sys.exit(0 if _selftest() else 1)
    if len(sys.argv) < 3:
        sys.exit("usage: cabwrite.py OUT.CAB file1 [file2 ...]  |  --selftest")
    out = sys.argv[1]
    files = [(os.path.basename(p), open(p, "rb").read()) for p in sys.argv[2:]]
    n = write_cab(files, out)
    print("wrote %s: %d files, %d bytes" % (out, len(files), n))


if __name__ == "__main__":
    main()
