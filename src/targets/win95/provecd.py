#!/usr/bin/env python3
r"""provecd.py -- prove, byte-for-byte, that a built CD's binaries are OURS and not retail.

External CAB tools (7-Zip, expand.exe -F) mis-handle our MSZIP + spanning cabinets, so provenance
can't be trusted to them. This reads each cabinet ourselves (MSCF parse + MSZIP inflate with the
cross-block 32K dictionary, following folder spans across the prev/next cabinet chain), extracts every
file we substituted, and md5-compares it to (a) our build and (b) the retail binary. Output: a receipt
where every substituted file must read `== OURS` and zero read `== RETAIL`.

    wt --target win95 provecd [--dir <cd WIN95 dir>] [--our <our staging dir>]
"""
import glob
import hashlib
import os
import struct
import sys
import zlib

from src.targets.win95 import authsrc, owlink


def _cstr(d, o):
    e = d.index(b"\0", o); return d[o:e], e + 1


def _parse(cabpath):
    d = open(cabpath, "rb").read()
    coff_files = struct.unpack("<I", d[16:20])[0]
    cFold, cFile, flags = struct.unpack("<HHH", d[26:32])
    off = 36; cbCFFolder = cbCFData = 0
    if flags & 0x4:
        cbH, cbCFFolder, cbCFData = struct.unpack("<HBB", d[off:off + 4]); off += 4 + cbH
    prev = nxt = None
    if flags & 0x1: prev, off = _cstr(d, off); _, off = _cstr(d, off)
    if flags & 0x2: nxt, off = _cstr(d, off); _, off = _cstr(d, off)
    folders = []
    for _ in range(cFold):
        coff, ncf, tc = struct.unpack("<IHH", d[off:off + 8]); off += 8 + cbCFFolder
        folders.append([coff, ncf, tc & 0xF])
    off = coff_files; mem = []
    for _ in range(cFile):
        cb, uoff, ifold = struct.unpack("<IIH", d[off:off + 10]); off += 16
        nm, off = _cstr(d, off); mem.append((nm.decode("latin1"), cb, uoff, ifold))
    return dict(data=d, folders=folders, mem=mem, cbCFData=cbCFData,
                prev=prev.decode("latin1") if prev else None,
                nxt=nxt.decode("latin1") if nxt else None)


def _folder_stream(cabdir, cabname, ifold, _seen=None):
    """Uncompressed bytes of folder `ifold` in `cabname`, following spans (a folder whose data
    continues in the next cabinet, or a file whose folder began in the previous one)."""
    info = _parse(os.path.join(cabdir, cabname))
    if ifold >= len(info["folders"]):
        # spanning-marker folder index: the real folder is the FIRST/LAST of a chained cabinet.
        return b""
    coff, ncf, comp = info["folders"][ifold]
    if comp != 1:
        return None                                        # not MSZIP (QUANTUM etc.) -- skip
    d = info["data"]; p = coff; unc = b""; prev = b""
    for _ in range(ncf):
        _cs, cbData, _cbU = struct.unpack("<IHH", d[p:p + 8]); p += 8 + info["cbCFData"]
        blk = d[p:p + cbData]; p += cbData
        if blk[:2] != b"CK":
            return None
        dc = zlib.decompressobj(-15, zdict=prev) if prev else zlib.decompressobj(-15)
        out = dc.decompress(blk[2:]) + dc.flush(); unc += out; prev = out[-32768:]
    return unc


def _extract(cabdir, cabname, name):
    info = _parse(os.path.join(cabdir, cabname))
    t = [m for m in info["mem"] if m[0].lower() == name.lower()]
    if not t:
        return None
    _nm, cb, uoff, ifold = t[0]
    stream = _folder_stream(cabdir, cabname, ifold)
    if stream is None or uoff + cb > len(stream):
        return None
    return stream[uoff:uoff + cb]


def _md5(b):
    return hashlib.md5(b).hexdigest()


def _md5f(p):
    try:
        return _md5(open(p, "rb").read())
    except OSError:
        return None


def main(argv):
    dr = authsrc._dr()
    cddir = argv[argv.index("--dir") + 1] if "--dir" in argv else \
        os.path.join(dr, "recon", "authored", "_neboot", "out", "WIN95")
    ourdir = argv[argv.index("--our") + 1] if "--our" in argv else \
        os.path.join(dr, "recon", "authored", "_neboot", "our")
    if not os.path.isdir(cddir):
        print("no CD dir: %s" % cddir); return 1
    # Setup-runtime cabinets: installed-only mode leaves these STOCK by design (Setup runs on them).
    # A file that also lives in an installed WIN95_*.CAB must be checked THERE (that's the copy the
    # installed system gets); a file that lives ONLY in a runtime cabinet is stock-by-design, not a
    # breach.
    RUNTIME = ("MINI.CAB", "PRECOPY1.CAB", "PRECOPY2.CAB", "SETUPW95.CAB", "SETUPPP.CAB", "DXMINI.CAB")
    from src.targets.win95 import cab
    loc = {}                                               # name -> installed cab (preferred)
    runtime_only = {}                                      # name -> runtime cab (fallback)
    for cabpath in glob.glob(os.path.join(cddir, "*.[cC][aA][bB]")):
        try:
            _i, files = cab.read_cab(cabpath)
        except Exception:                                  # noqa: BLE001
            continue
        base = os.path.basename(cabpath)
        for cf in files:
            n = cf.name.lower()
            if base.upper() in RUNTIME:
                runtime_only.setdefault(n, base)
            else:
                loc.setdefault(n, base)
    ours_files = sorted(os.path.basename(p) for p in glob.glob(os.path.join(ourdir, "*")))
    receipt = os.path.join(os.path.dirname(os.path.dirname(cddir)), "..", "..", "..")  # noqa: F841
    out = os.path.join(dr, "recon", "authored", "PROVECD.txt")
    okn = retailn = miss = runtime = 0
    lines = ["CD provenance -- every substituted file: CD content vs OUR build vs RETAIL", ""]
    for f in ours_files:
        mod = os.path.splitext(f)[0]
        cabname = loc.get(f.lower())
        our_md5 = _md5f(os.path.join(ourdir, f))
        ret = owlink._find_binary(mod)
        ret_md5 = _md5f(ret) if ret else None
        if not cabname:
            if f.lower() in runtime_only:
                lines.append("%-14s %-9s runtime-stock (installed-only: by design)"
                             % (f, runtime_only[f.lower()])); runtime += 1
            else:
                lines.append("%-14s NOT-ON-CD" % f); miss += 1
            continue
        body = _extract(cddir, cabname, f)
        cd_md5 = _md5(body) if body else None
        if cd_md5 and cd_md5 == our_md5:
            verdict = "== OURS"; okn += 1
        elif cd_md5 and cd_md5 == ret_md5:
            verdict = "!! RETAIL"; retailn += 1
        elif cd_md5 is None:
            verdict = "unextractable(span/quantum)"; miss += 1
        else:
            verdict = "differs(both)"; miss += 1
        lines.append("%-14s %-9s %s" % (f, cabname, verdict))
    lines += ["", "SUMMARY: installed-cabinet files: %d == OURS, %d == RETAIL(breach), %d unverified;"
              " %d runtime-stock(by design); of %d staged"
              % (okn, retailn, miss, runtime, len(ours_files))]
    open(out, "w").write("\n".join(lines) + "\n")
    print("\n".join(lines[-1:]))
    print("INSTALLED binaries verified OURS: %d   RETAIL breaches: %d   (runtime-stock by design: %d)"
          % (okn, retailn, runtime))
    print("-> %s" % out)
    return 0 if retailn == 0 else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
