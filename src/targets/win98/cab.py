#!/usr/bin/env python3
"""
cab.py — Windows 98 cabinet-set reader and extractor.

The medium changed again. Windows 1.0 shipped one concatenated boot image;
Windows 3.1 shipped separate KWAJ/SZDD-compressed files on a flat CD; Windows 95
shipped a Quantum-compressed spanning set. Windows 98 (Second Edition, volume
label `WIN98 SE`) again ships its whole system inside a *spanning multi-cabinet
set* in the CD's `win98/` directory — 77 cabinets, ~6,956 files — but with a
larger, differently-chained layout than 95:

    PRECOPY1.CAB -> PRECOPY2.CAB                       (pre-copy pair)
    CATALOG3.CAB -> BASE4..6 -> NET7..10 -> DRIVER11..20
                 -> WIN98_21.CAB -> ... -> WIN98_71.CAB (the main content chain)
    MINI.CAB   WIN98_OL.CAB   CHL99.CAB                 (standalone)

Three facts drive this tool:

  * **Folders span cabinet boundaries.** A CFFOLDER's compressed data can start
    in WIN98_22.CAB and continue into WIN98_23.CAB; a CFFILE with folder index
    0xFFFD/0xFFFE/0xFFFF is continued-from-prev / continued-to-next / both. So a
    single file cannot be decompressed from one .CAB in isolation — the chain
    matters.

  * **The chain is found by NEXT pointers, not "no PREV".** The main content
    chain's true head (CATALOG3) still carries a PREV pointer to PRECOPY2, while
    PRECOPY2's NEXT is null — the two are separate sets. Keying head detection on
    "no NEXT points here" (see `_head_cabinets`) is what makes 7-Zip walk the
    whole ~7,000-file corpus instead of just the ~500-file PRECOPY pair.

  * **The compression is LZX, not Quantum.** 95 used Quantum; 98 SE's spanning
    folders are LZX (with a few small MSZIP cabinets: WIN98_OL, MINI, CHL99).
    LZX is a research project to reimplement; 7-Zip *does* handle these cabinets
    correctly, spanning and all. The retail bytes are identical no matter who
    decompresses them, so the evidence rule is not weakened by delegating the
    LZX/MSZIP decode: we own the cabinet catalog (the manifest this tool builds
    by parsing MSCF ourselves) and hand only the decompress to 7-Zip.

So this module does two separable things:

  * `manifest()` / `--list` — parse the MSCF headers ourselves. This is the
    evidence: every file name, size, folder, and compression method, read from
    the cabinet structure directly, with no external tool involved.
  * `extract()` / `--extract` — shell out to 7-Zip for the LZX/MSZIP decode,
    pointed at each chain head so it follows every span.

Usage:
    python3 cab.py <dir-of-cabs> --list
    python3 cab.py <dir-of-cabs> --extract files
    python3 cab.py <win98-iso> --from-iso --extract files   # ISO -> cabs -> files
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
# Decompression — delegated to 7-Zip (handles LZX/MSZIP + spanning).
# --------------------------------------------------------------------------

_SEVENZIP_CANDIDATES = [
    r"C:\Program Files\7-Zip\7z.exe",
    r"C:\Program Files (x86)\7-Zip\7z.exe",
]


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
        "7-Zip not found. Install it (handles the LZX/MSZIP-compressed spanning "
        "cabinets) or point cab.py at a 7z.exe.")


def _head_cabinets(directory):
    """Every cabinet that STARTS a chain: one that no other cabinet forward-links
    to via its NEXT field (nothing points into it). 7-Zip pointed at a head
    follows that chain's NEXT pointers to the end, so extracting every head
    covers the whole medium.

    Why NEXT-based, not "no PREV": on a Windows 98 CD the files sit in one long
    content chain —

        CATALOG3 -> BASE4 -> BASE5 -> BASE6 -> NET7…10 -> DRIVER11…20
                 -> WIN98_21 -> WIN98_22 -> … -> WIN98_71

    but its true start, CATALOG3, still carries a PREV pointer (to PRECOPY2),
    while PRECOPY2's NEXT is null — the PRECOPY1/2 pair is a separate 2-cab set.
    A "no PREV" test therefore misses CATALOG3, and 7-Zip walks only PRECOPY1->
    PRECOPY2 (~500 of the ~7000 files). Keying on "not anyone's NEXT" finds the
    real heads: PRECOPY1, CATALOG3, and the standalone MINI / WIN98_OL / CHL99.
    (Win95's single WIN95_* span is the degenerate one-head case.)"""
    cs = cab_set(directory)
    real = {n.upper(): n for n in cs}                       # UPPER -> on-disk name
    infos = {n.upper(): i for n, (i, _f) in cs.items()}
    nexts = {i["next"].upper() for i in infos.values() if i["next"]}
    heads = [n for n in infos if n not in nexts]
    return [os.path.join(directory, real[h]) for h in sorted(heads)]


def _head_cabinet(directory):
    """The first span head (back-compat shim; prefer `_head_cabinets`)."""
    heads = _head_cabinets(directory)
    if not heads:
        raise RuntimeError(f"no cabinet head found in {directory}")
    return heads[0]


def extract(directory, outdir, head=None):
    """Expand the cabinet set(s) into outdir via 7-Zip. Runs 7-Zip once per span
    head so all of Win98's parallel spans (WIN98_/BASE/DRIVER/NET/…) land in the
    same corpus directory; identical files across spans overwrite harmlessly."""
    os.makedirs(outdir, exist_ok=True)
    sz = find_7zip()
    heads = [head] if head else _head_cabinets(directory)
    if not heads:
        raise RuntimeError(f"no cabinet head found in {directory}")
    for h in heads:
        cmd = [sz, "x", "-y", f"-o{outdir}", h]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode != 0:
            sys.stderr.write(r.stdout + r.stderr)
            raise RuntimeError(f"7-Zip failed on {h} (exit {r.returncode})")
    n = len([f for f in os.listdir(outdir) if os.path.isfile(os.path.join(outdir, f))])
    return n


# --------------------------------------------------------------------------
# ISO convenience — pull the win98/*.CAB out of an install ISO first.
# --------------------------------------------------------------------------

# The install cabinets live in the OS directory on the CD (win98/ for a Win98
# medium). Filtering on that directory grabs exactly the system span set and
# skips unrelated cabinets elsewhere on the disc (add-ons/pws, drivers/cpu,
# tsclient). Name-prefix fallback keeps it working on oddly-laid-out images.
_INSTALL_DIR = "win98"
_INSTALL_PREFIXES = ("WIN98_", "BASE", "DRIVER", "NET", "CATALOG",
                     "PRECOPY", "MINI", "CHL")


def cabs_from_iso(iso_path, outdir):
    """Extract just the installable cabinets from a Win98 ISO into outdir."""
    from src.engines.build import iso9660
    os.makedirs(outdir, exist_ok=True)
    iso = iso9660.Iso9660(iso_path)
    n = 0
    for path, rec in iso.walk():
        u = path.upper()
        base = os.path.basename(u)
        if not u.endswith(".CAB"):
            continue
        parent = os.path.basename(os.path.dirname(u.replace("\\", "/")))
        in_install_dir = parent == _INSTALL_DIR.upper()
        if in_install_dir or base.startswith(_INSTALL_PREFIXES):
            with open(os.path.join(outdir, base), "wb") as f:
                f.write(iso.read_file(rec))
            n += 1
    return n


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src", help="directory of .CAB files, or a Win98 ISO with --from-iso")
    ap.add_argument("--from-iso", action="store_true",
                    help="src is an install ISO; pull win98/*.CAB out first")
    ap.add_argument("--list", action="store_true", help="print the file catalog")
    ap.add_argument("--extract", metavar="OUTDIR", help="expand the set into OUTDIR")
    args = ap.parse_args()

    tmp = None
    directory = args.src
    if args.from_iso:
        tmp = tempfile.mkdtemp(prefix="win98cabs_")
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
