#!/usr/bin/env python3
"""libsig: identify statically-linked CRT/library functions in a 16-bit MZ image
by matching byte signatures extracted from the shipped OMF .LIB (fixup sites
wildcarded). CodeBreaker origin-triage: name library code, don't reconstruct it.

Usage: python libsig_scan.py <image.exe|unpacked> <lib1.LIB> [lib2.LIB ...]
"""
import struct, sys, os
sys.path.insert(0, r"C:/Users/mrtit/Documents/GitHub/CodeBreaker")
from src.formats.objextract import Obj

MINLEN = 8          # require at least this many leading bytes in a signature
MIN_MATCH = 10      # require at least this many bytes compared for a hit


def lib_modules(data):
    """Yield each module's OBJ byte-slice from an OMF .LIB."""
    assert data[0] == 0xF0, "not a library"
    pagelen = struct.unpack_from("<H", data, 1)[0] + 3
    i = pagelen
    while i < len(data):
        if data[i] == 0xF1:
            break
        if data[i] != 0x80:            # padding before next page-aligned module
            i += 1; continue
        # find MODEND (0x8A/0x8B) then align up to page boundary
        j = i
        end = None
        while j < len(data):
            rt = data[j]
            rlen = struct.unpack_from("<H", data, j + 1)[0]
            nxt = j + 3 + rlen
            if rt in (0x8A, 0x8B):
                end = nxt; break
            j = nxt
        if end is None:
            break
        yield data[i:end]
        i = ((end + pagelen - 1) // pagelen) * pagelen


def build_sigs(libpaths):
    sigs = {}   # name -> (bytes, frozenset(mask))
    for lp in libpaths:
        data = open(lp, "rb").read()
        for mod in lib_modules(data):
            try:
                fns = Obj(mod).functions()
            except Exception:
                continue
            for name, (code, mask) in fns.items():
                # leading unmasked run length
                lead = 0
                while lead < len(code) and lead not in mask:
                    lead += 1
                if len(code) >= MINLEN and lead >= 4:
                    # keep the longest signature seen for a name
                    if name not in sigs or len(code) > len(sigs[name][0]):
                        sigs[name] = (code, frozenset(mask))
    return sigs


def load_image(path):
    b = open(path, "rb").read()
    if b[:2] in (b"MZ", b"ZM"):
        hdr = struct.unpack("<H", b[8:10])[0] * 16
        return b[hdr:]
    return b


def match_at(img, a, code, mask):
    n = min(len(code), len(img) - a)
    if n < MIN_MATCH:
        return 0
    cmpd = 0
    for i in range(n):
        if i in mask:
            continue
        if img[a + i] != code[i]:
            return 0
        cmpd += 1
    return cmpd if cmpd >= MIN_MATCH else 0


def scan(img, sigs, starts=None):
    hits = {}
    positions = sorted(starts) if starts else range(len(img))
    for a in positions:
        best = None
        for name, (code, mask) in sigs.items():
            c = match_at(img, a, code, mask)
            if c and (best is None or c > best[1]):
                best = (name, c, len(code))
        if best:
            hits[a] = best
    return hits


def main():
    img = load_image(sys.argv[1])
    sigs = build_sigs(sys.argv[2:])
    # use carve function starts if available
    stem = os.path.splitext(os.path.basename(sys.argv[1]))[0]
    fpath = os.path.join("workspace", "disasm", stem, "functions.txt")
    starts = None
    if os.path.exists(fpath):
        starts = [int(l.split()[0], 16) for l in open(fpath)
                  if l[:2] == "0x" or l.startswith("0x")]
    print(f"{len(sigs)} signatures; scanning {sys.argv[1]}"
          f" ({'carved starts' if starts else 'every offset'})")
    hits = scan(img, sigs, starts)
    print(f"== {len(hits)} library functions identified ==")
    for a in sorted(hits):
        nm, c, ln = hits[a]
        print(f"  {a:#06x}  {nm:24s} ({c}/{ln} bytes)")


if __name__ == "__main__":
    main()
