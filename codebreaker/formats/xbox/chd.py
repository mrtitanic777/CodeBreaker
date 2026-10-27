"""CHD (MAME Compressed Hunks of Data) — Xbox disc images.

`info` parses the CHD v5 header natively (no external tool). Conversions
(chd<->iso, verify) are delegated to MAME's `chdman`, exactly as xVerter and
every other CHD tool does — CHD's multi-codec hunk format (zlib/lzma/flac/…) is
maintained upstream and not worth reimplementing. chdman is found via `config`
(key `chdman`) or PATH, the same driven-not-embedded policy as Ghidra/DOSBox.

Commands: info <chd> | toiso <chd> <out.iso> | tochd <in.iso> <out.chd> | verify <chd>.
"""

import os
import struct
import subprocess
import sys

from codebreaker.base import settings

DESCRIPTION = "CHD (MAME) Xbox disc images: native header info + chdman-driven chd<->iso/verify"

CHD_MAGIC = b"MComprHD"


class ChdError(Exception):
    pass


# ── native CHD v5 header ───────────────────────────────────────────────
def read_header(path):
    """Parse a CHD v5 header (big-endian). Returns a dict; raises for other
    versions (chdman can upgrade them with `chdman copy`)."""
    with open(path, "rb") as f:
        head = f.read(124)
    if head[:8] != CHD_MAGIC:
        raise ChdError("not a CHD file (bad magic)")
    hlen, version = struct.unpack_from(">II", head, 8)
    if version != 5:
        raise ChdError("CHD v%d not supported here (chdman copy can upgrade to v5)" % version)
    comps = struct.unpack_from(">4I", head, 16)
    logicalbytes, mapoffset, metaoffset = struct.unpack_from(">QQQ", head, 32)
    hunkbytes, unitbytes = struct.unpack_from(">II", head, 56)

    def tag(v):
        if v == 0:
            return "none"
        return bytes([(v >> 24) & 0xFF, (v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF]).decode("latin-1")

    return {
        "version": version, "header_len": hlen,
        "logical_bytes": logicalbytes, "hunk_bytes": hunkbytes, "unit_bytes": unitbytes,
        "compressors": [tag(c) for c in comps if c],
        "meta_offset": metaoffset, "map_offset": mapoffset,
    }


def is_chd(path):
    try:
        return open(path, "rb").read(8) == CHD_MAGIC
    except OSError:
        return False


# ── chdman driver ──────────────────────────────────────────────────────
def _need_chdman():
    exe = settings.tool("chdman")
    if not exe:
        raise ChdError("`chdman` not found — install MAME tools (Linux: mame-tools / "
                       "rom-tools; Windows: chdman.exe ships with MAME) and, if it is "
                       'not on PATH; set "chdman" in settings.json')
    return exe


def _run(argv):
    p = subprocess.run(argv, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if p.returncode != 0:
        raise ChdError("chdman failed:\n%s" % (p.stdout or "").strip())
    return p.stdout


def extract(chd_path, out_iso):
    """chd -> ISO (chdman extractdvd)."""
    _run([_need_chdman(), "extractdvd", "-i", chd_path, "-o", out_iso, "-f"])
    if not os.path.exists(out_iso) or os.path.getsize(out_iso) == 0:
        raise ChdError("chdman reported success but wrote no output")
    return os.path.getsize(out_iso)


def build(iso_path, out_chd):
    """ISO -> chd (chdman createdvd)."""
    if os.path.exists(out_chd):
        os.remove(out_chd)
    _run([_need_chdman(), "createdvd", "-i", iso_path, "-o", out_chd])
    if not os.path.exists(out_chd) or os.path.getsize(out_chd) == 0:
        raise ChdError("chdman reported success but wrote no output")
    return os.path.getsize(out_chd)


def verify(chd_path):
    return _run([_need_chdman(), "verify", "-i", chd_path])


# ── CLI ────────────────────────────────────────────────────────────────
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  chd info   <file.chd>            native header summary (no chdman)\n"
              "  chd toiso  <file.chd> <out.iso>  extract to ISO (needs chdman)\n"
              "  chd tochd  <in.iso>  <out.chd>   build a DVD CHD (needs chdman)\n"
              "  chd verify <file.chd>            chdman integrity verify")
        return
    op = a[0]
    if op == "info":
        h = read_header(a[1])
        print("CHD v%d: %s" % (h["version"], a[1]))
        print("logical size : %d bytes (%.2f GB)" % (h["logical_bytes"], h["logical_bytes"] / (1 << 30)))
        print("hunk / unit  : %d / %d bytes" % (h["hunk_bytes"], h["unit_bytes"]))
        print("compressors  : %s" % (", ".join(h["compressors"]) or "none"))
        print("on-disk size : %d bytes" % os.path.getsize(a[1]))
        return
    if op == "toiso" and len(a) >= 3:
        print("extracted %d bytes -> %s" % (extract(a[1], a[2]), a[2])); return
    if op == "tochd" and len(a) >= 3:
        print("built CHD (%d bytes) -> %s" % (build(a[1], a[2]), a[2])); return
    if op == "verify":
        print(verify(a[1]).strip()); return
    print("unknown/short command; run 'chd' for usage"); sys.exit(2)


if __name__ == "__main__":
    main()
