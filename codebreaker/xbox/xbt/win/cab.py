#!/usr/bin/env python3
"""
cab.py — Windows 95 cabinet-set reader and extractor.

The medium changed again. Windows 1.0 shipped one concatenated boot image;
Windows 3.1 shipped separate KWAJ/SZDD-compressed files on a flat CD. Windows 95
ships its whole system inside a *spanning multi-cabinet set*:

    PRECOPY1.CAB PRECOPY2.CAB  MINI.CAB  WIN95_02.CAB ... WIN95_28.CAB

Two facts drive this tool:

  * **Folders span cabinet boundaries.** A CFFOLDER's compressed data can start
    in WIN95_03.CAB and continue into WIN95_04.CAB; a CFFILE with folder index
    0xFFFD/0xFFFE/0xFFFF is continued-from-prev / continued-to-next / both. So a
    single file cannot be decompressed from one .CAB in isolation — the chain
    matters, and WIN95_02.CAB is the head that indexes it.

  * **The compression is QUANTUM, not MSZIP.** 3.1's KWAJ/SZDD we decode
    ourselves (`mscomp.py`); Quantum is an arithmetic-coded LZ variant that is a
    genuine research project to reimplement. Unlike 3.1's ISO — which 7-Zip
    refused, forcing us to write `iso9660.py` — 7-Zip *does* handle these
    cabinets correctly, spanning and all. The retail bytes are identical no
    matter who decompresses them, so the evidence rule is not weakened by
    delegating the Quantum step: we own the cabinet catalog (the manifest this
    tool builds by parsing MSCF ourselves) and hand only the decompress to 7-Zip.

So this module does two separable things:

  * `manifest()` / `--list` — parse the MSCF headers ourselves. This is the
    evidence: every file name, size, folder, and compression method, read from
    the cabinet structure directly, with no external tool involved.
  * `extract()` / `--extract` — shell out to 7-Zip for the Quantum decode,
    pointed at the head cabinet so it follows the whole span.

Usage:
    python3 cab.py <dir-of-cabs> --list
    python3 cab.py <dir-of-cabs> --extract files
    python3 cab.py <win95-iso> --from-iso --extract files   # ISO -> cabs -> files
"""
import argparse
import glob
import os
import struct
import subprocess
import sys
import tempfile

# --------------------------------------------------------------------------
# MSCF structures — parsed here so the file catalog is our own evidence.
# --------------------------------------------------------------------------

_COMPRESS = {0: "NONE", 1: "MSZIP", 2: "QUANTUM", 3: "LZX"}

# Special CFFILE folder indices (spanning markers).
_IFOLD_CONTINUED_FROM_PREV = 0xFFFD
_IFOLD_CONTINUED_TO_NEXT = 0xFFFE
_IFOLD_CONTINUED_PREV_AND_NEXT = 0xFFFF


class CabFile:
    __slots__ = ("name", "size", "ifolder", "uoffset", "compress", "cabinet")

    def __init__(self, name, size, ifolder, uoffset, compress, cabinet):
        self.name = name
        self.size = size
        self.ifolder = ifolder
        self.uoffset = uoffset
        self.compress = compress
        self.cabinet = cabinet

    def __repr__(self):
        span = {
            _IFOLD_CONTINUED_FROM_PREV: " <-prev",
            _IFOLD_CONTINUED_TO_NEXT: " ->next",
            _IFOLD_CONTINUED_PREV_AND_NEXT: " <-prev->next",
        }.get(self.ifolder, "")
        return f"<{self.name} {self.size}b {self.compress}{span}>"


def _cstr(buf, off):
    end = buf.index(b"\0", off)
    return buf[off:end].decode("latin1"), end + 1


def read_cab(path):
    """Parse one .CAB's MSCF header. Returns (info, [CabFile])."""
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] != b"MSCF":
        raise ValueError(f"{path}: not an MSCF cabinet")
    (sig, res1, cb_cab, res2, coff_files, res3,
     v_min, v_maj, c_folders, c_files, flags, set_id, i_cab) = struct.unpack(
        "<4sIIIIIBBHHHHH", data[:36])
    off = 36
    ab_reserve = cbCFHeader = cbCFFolder = cbCFData = 0
    if flags & 0x0004:  # cfhdrRESERVE_PRESENT
        cbCFHeader, cbCFFolder, cbCFData = struct.unpack("<HBB", data[off:off + 4])
        off += 4 + cbCFHeader
    prev = nxt = None
    if flags & 0x0001:  # cfhdrPREV_CABINET
        prev, off = _cstr(data, off)
        _prevdisk, off = _cstr(data, off)
    if flags & 0x0002:  # cfhdrNEXT_CABINET
        nxt, off = _cstr(data, off)
        _nextdisk, off = _cstr(data, off)

    folders = []
    for _ in range(c_folders):
        coff_start, c_cfdata, type_comp = struct.unpack("<IHH", data[off:off + 8])
        off += 8 + cbCFFolder
        folders.append(_COMPRESS.get(type_comp & 0x0F, f"?{type_comp & 0x0F}"))

    files = []
    off = coff_files
    base = os.path.basename(path)
    for _ in range(c_files):
        cb_file, uoff, ifolder, date, time, attribs = struct.unpack(
            "<IIHHHH", data[off:off + 16])
        off += 16
        name, off = _cstr(data, off)
        comp = folders[ifolder] if ifolder < len(folders) else "SPAN"
        files.append(CabFile(name, cb_file, ifolder, uoff, comp, base))

    info = dict(cabinet=base, cb_cabinet=cb_cab, version=f"{v_maj}.{v_min}",
                folders=c_folders, files=c_files, flags=flags,
                set_id=set_id, index=i_cab, prev=prev, next=nxt,
                compress=sorted(set(folders)))
    return info, files


def cab_set(directory):
    """Every .CAB in a directory, parsed. Returns {name: (info, files)}."""
    out = {}
    for p in sorted(glob.glob(os.path.join(directory, "*.[cC][aA][bB]"))):
        try:
            out[os.path.basename(p)] = read_cab(p)
        except ValueError:
            pass
    return out


def manifest(directory):
    """Flat list of every CabFile across the whole set, de-duplicated by name."""
    seen = {}
    for _name, (_info, files) in cab_set(directory).items():
        for cf in files:
            # keep the first (head-cabinet) catalog entry; spanning dupes agree
            seen.setdefault(cf.name.upper(), cf)
    return sorted(seen.values(), key=lambda c: c.name.upper())


# --------------------------------------------------------------------------
# Decompression — delegated to 7-Zip (handles Quantum + spanning).
# --------------------------------------------------------------------------

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "core"))
try:
    import config as _config
    _cfg_7z = _config.tool("sevenzip")
except Exception:
    _cfg_7z = None
_SEVENZIP_CANDIDATES = [p for p in (
    _cfg_7z,
    r"C:\Program Files\7-Zip\7z.exe",
    r"C:\Program Files (x86)\7-Zip\7z.exe",
) if p]


def find_7zip():
    for c in _SEVENZIP_CANDIDATES:
        if os.path.exists(c):
            return c
    from shutil import which
    for name in ("7z", "7za", "7zz"):
        p = which(name)
        if p:
            return p
    raise RuntimeError(
        "7-Zip not found. Install it (handles the Quantum-compressed spanning "
        "cabinets) or point cab.py at a 7z.exe.")


def _head_cabinet(directory):
    """The cabinet that indexes the main span: the WIN95_* with no PREV, or
    the lowest-numbered one. 7-Zip pointed here follows the whole chain."""
    infos = {n: i for n, (i, _f) in cab_set(directory).items()}
    win95 = sorted(n for n in infos if n.upper().startswith("WIN95_"))
    for n in win95:
        if not infos[n]["prev"]:
            return os.path.join(directory, n)
    if win95:
        return os.path.join(directory, win95[0])
    # no WIN95_* span: just return any head
    for n, i in infos.items():
        if not i["prev"]:
            return os.path.join(directory, n)
    raise RuntimeError(f"no cabinet head found in {directory}")


def extract(directory, outdir, head=None):
    """Expand the whole cabinet set into outdir via 7-Zip."""
    os.makedirs(outdir, exist_ok=True)
    sz = find_7zip()
    head = head or _head_cabinet(directory)
    cmd = [sz, "x", "-y", f"-o{outdir}", head]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        sys.stderr.write(r.stdout + r.stderr)
        raise RuntimeError(f"7-Zip failed on {head} (exit {r.returncode})")
    n = len([f for f in os.listdir(outdir) if os.path.isfile(os.path.join(outdir, f))])
    return n


# --------------------------------------------------------------------------
# ISO convenience — pull the WIN95/*.CAB out of an install ISO first.
# --------------------------------------------------------------------------

def cabs_from_iso(iso_path, outdir):
    """Extract just the installable cabinets from a Win95 ISO into outdir."""
    import iso9660
    os.makedirs(outdir, exist_ok=True)
    iso = iso9660.Iso9660(iso_path)
    n = 0
    for path, rec in iso.walk():
        u = path.upper()
        base = os.path.basename(u)
        if u.endswith(".CAB") and base.startswith(("WIN95_", "PRECOPY", "MINI")):
            with open(os.path.join(outdir, base), "wb") as f:
                f.write(iso.read_file(rec))
            n += 1
    return n


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src", help="directory of .CAB files, or a Win95 ISO with --from-iso")
    ap.add_argument("--from-iso", action="store_true",
                    help="src is an install ISO; pull WIN95/*.CAB out first")
    ap.add_argument("--list", action="store_true", help="print the file catalog")
    ap.add_argument("--extract", metavar="OUTDIR", help="expand the set into OUTDIR")
    args = ap.parse_args()

    tmp = None
    directory = args.src
    if args.from_iso:
        tmp = tempfile.mkdtemp(prefix="win95cabs_")
        n = cabs_from_iso(args.src, tmp)
        print(f"# pulled {n} cabinets from {args.src} -> {tmp}")
        directory = tmp

    if args.list or not args.extract:
        cs = cab_set(directory)
        print(f"# {len(cs)} cabinets")
        comps = sorted({c for _n, (i, _f) in cs.items() for c in i["compress"]})
        print(f"# compression methods present: {', '.join(comps)}")
        files = manifest(directory)
        print(f"# {len(files)} distinct files")
        for cf in files:
            print(f"  {cf.size:>10}  {cf.compress:8} {cf.name}")

    if args.extract:
        n = extract(directory, args.extract)
        print(f"# extracted {n} files -> {args.extract}")


if __name__ == "__main__":
    main()
