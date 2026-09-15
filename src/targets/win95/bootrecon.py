#!/usr/bin/env python3
r"""bootrecon.py -- reverse-engineer a Win95 boot floppy image end to end.

The Win95 Setup boot floppy (design/corpus/files/setup/media/boot.img, volume "BOOT95B") is the
one place in the corpus that carries the AUTHENTIC real-mode boot chain: a genuine "MSWIN4.1"
FAT12 volume boot record (VBR) plus the exact IO.SYS (== winboot.sys) it is built to load. It is
the missing link the DOS reconstruction needed -- our own DOS 6-derived VBR jumps to IO.SYS
offset 0 (the MZ header) and hangs, because the real Win95 VBR jumps to 0070:0200 (the MSLOAD
entry, past the header).

This pass fully reverse-engineers such an image:

  * parse the BPB, walk both FAT copies, and extract every root-directory file (FAT12/16);
  * cross-reference each extracted file against the corpus by content hash, so already-
    reconstructed components (IO.SYS, COMMAND.COM, HIMEM, DRVSPACE, ...) are identified and the
    genuinely-new DOS utilities are flagged;
  * carve the 512-byte VBR and REGRESSION-VERIFY the readable reconstruction
    (design/recon/bootdisk/msboot.asm) reassembles byte-identical to it (needs nasm on PATH);
  * document the disk layout, the file inventory, and the recovered boot contract.

    wt --target win95 bootrecon [--img PATH] [--out DIR] [--no-extract]

Read-only against the corpus; writes only under design/recon/bootdisk/.
"""
import os
import sys
import csv
import struct
import hashlib
import subprocess

from src.core import paths


# --- design arena / corpus resolution (mirror dosrecon) -------------------------------------
def _design_root():
    dr = paths.design_root()
    if os.path.isdir(os.path.join(dr, "corpus")):
        return dr
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design")
    if os.path.isdir(os.path.join(sib, "corpus")):
        return sib
    return dr


def _default_img():
    return os.path.join(_design_root(), "corpus", "files", "setup", "media", "boot.img")


def _corpus_hashes():
    """md5 -> relative corpus path, for every regular file under corpus/files (identify extracted
    files against the already-reconstructed corpus)."""
    root = os.path.join(_design_root(), "corpus", "files")
    out = {}
    if not os.path.isdir(root):
        return out
    for dp, _dn, fn in os.walk(root):
        for f in fn:
            p = os.path.join(dp, f)
            try:
                with open(p, "rb") as fh:
                    out.setdefault(hashlib.md5(fh.read()).hexdigest(), os.path.relpath(p, root))
            except OSError:
                pass
    return out


# --- FAT12/16 volume walk -------------------------------------------------------------------
def _u16(b, o):
    return b[o] | (b[o + 1] << 8)


def _u32(b, o):
    return b[o] | (b[o + 1] << 8) | (b[o + 2] << 16) | (b[o + 3] << 24)


class Bpb:
    """Parsed BIOS Parameter Block + derived geometry of a FAT12/16 volume."""
    def __init__(self, d):
        self.oem       = d[3:11].decode("latin1")
        self.bps       = _u16(d, 0x0B)
        self.spc       = d[0x0D]
        self.resv      = _u16(d, 0x0E)
        self.nfat      = d[0x10]
        self.nroot     = _u16(d, 0x11)
        self.tot16     = _u16(d, 0x13)
        self.media     = d[0x15]
        self.spf       = _u16(d, 0x16)
        self.spt       = _u16(d, 0x18)
        self.heads     = _u16(d, 0x1A)
        self.hidden    = _u32(d, 0x1C)
        self.tot32     = _u32(d, 0x20)
        self.drive     = d[0x24]
        self.extsig    = d[0x26]
        self.serial    = _u32(d, 0x27)
        self.label     = d[0x2B:0x36].decode("latin1")
        self.fstype    = d[0x36:0x3E].decode("latin1")
        self.total     = self.tot16 or self.tot32
        self.fat_start = self.resv
        self.root_start = self.resv + self.nfat * self.spf
        self.root_sects = (self.nroot * 32 + self.bps - 1) // self.bps
        self.data_start = self.root_start + self.root_sects
        clusters = (self.total - self.data_start) // self.spc if self.spc else 0
        self.is_fat16 = clusters >= 4085


def _walk(img):
    """Return (Bpb, [ (name, size, cluster, nclusters, attr, bytes) ]) for a FAT12/16 image."""
    d = open(img, "rb").read()
    bpb = Bpb(d)
    fat = d[bpb.fat_start * bpb.bps: (bpb.fat_start + bpb.spf) * bpb.bps]

    def nextcl(n):
        if bpb.is_fat16:
            o = n * 2
            return _u16(fat, o)
        o = (n * 3) // 2
        v = fat[o] | (fat[o + 1] << 8)
        return (v & 0x0FFF) if (n & 1) == 0 else (v >> 4)

    end = 0xFFF0 if bpb.is_fat16 else 0x0FF0

    def chain(start):
        cl, out = start, []
        while 2 <= cl < end and len(out) < 65536:
            out.append(cl)
            cl = nextcl(cl)
        return out

    ents = []
    rootoff = bpb.root_start * bpb.bps
    for i in range(bpb.nroot):
        e = d[rootoff + i * 32: rootoff + i * 32 + 32]
        if not e or e[0] == 0:
            break
        if e[0] == 0xE5:
            continue
        attr = e[0x0B]
        if attr & 0x08:                    # volume label
            continue
        name = e[0:8].decode("latin1").rstrip()
        ext = e[8:11].decode("latin1").rstrip()
        fn = name + ("." + ext if ext else "")
        clus = _u16(e, 0x1A)
        size = _u32(e, 0x1C)
        ch = chain(clus) if clus >= 2 else []
        raw = b"".join(
            d[(bpb.data_start + (c - 2) * bpb.spc) * bpb.bps:
              (bpb.data_start + (c - 2) * bpb.spc) * bpb.bps + bpb.spc * bpb.bps]
            for c in ch
        )[:size]
        ents.append((fn, size, clus, len(ch), attr, raw))
    return bpb, d[:512], ents


# --- VBR regression check -------------------------------------------------------------------
def _verify_vbr(vbr_bytes, outdir):
    """Assemble the readable reconstruction (msboot.asm) with nasm and compare to the carved VBR.
    Returns (status, detail)."""
    asm = os.path.join(outdir, "msboot.asm")
    if not os.path.isfile(asm):
        return "MISSING", "design/recon/bootdisk/msboot.asm not present"
    nasm = None
    for c in ("nasm", r"C:\Program Files\NASM\nasm.exe", "/usr/bin/nasm"):
        try:
            subprocess.run([c, "-v"], capture_output=True)
            nasm = c
            break
        except OSError:
            continue
    if not nasm:
        return "SKIP", "nasm not on PATH (source present, not re-verified this run)"
    out = os.path.join(outdir, "_msboot_check.bin")
    r = subprocess.run([nasm, "-f", "bin", asm, "-o", out], capture_output=True, text=True)
    if r.returncode != 0:
        return "FAIL", "nasm error: " + (r.stderr.strip()[:200])
    built = open(out, "rb").read()
    try:
        os.remove(out)
    except OSError:
        pass
    if built == vbr_bytes:
        return "BYTE-IDENTICAL", "%d/%d bytes; md5 %s" % (
            len(built), len(vbr_bytes), hashlib.md5(built).hexdigest())
    nd = sum(1 for a, b in zip(built, vbr_bytes) if a != b) + abs(len(built) - len(vbr_bytes))
    return "DIFF", "%d bytes differ" % nd


# --- report emitters ------------------------------------------------------------------------
def _emit(bpb, ents, vbr_bytes, vstatus, vdetail, chashes, outdir, do_extract):
    filesdir = os.path.join(outdir, "files")
    if do_extract:
        os.makedirs(filesdir, exist_ok=True)

    rows = []
    for fn, size, clus, ncl, attr, raw in sorted(ents, key=lambda x: -x[1]):
        md5 = hashlib.md5(raw).hexdigest()
        corpus = chashes.get(md5, "")
        rows.append((fn, size, clus, ncl, attr, md5, corpus))
        if do_extract:
            with open(os.path.join(filesdir, fn), "wb") as fh:
                fh.write(raw)

    with open(os.path.join(outdir, "MANIFEST.csv"), "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["name", "size", "cluster", "nclusters", "attr", "md5", "corpus_match"])
        for r in rows:
            w.writerow([r[0], r[1], r[2], r[3], "0x%02X" % r[4], r[5], r[6]])

    total = sum(r[1] for r in rows)
    matched = sum(1 for r in rows if r[6])
    lines = []
    lines.append("# Win95 boot floppy -- full reverse-engineering (`bootrecon`)\n")
    lines.append("Reconstructed by CodeBreaker from the authentic Win95 Setup boot image.\n")
    lines.append("## Volume\n")
    lines.append("| field | value |")
    lines.append("|---|---|")
    lines.append("| OEM | `%s` |" % bpb.oem)
    lines.append("| label | `%s` |" % bpb.label.rstrip())
    lines.append("| filesystem | %s (%s) |" % (bpb.fstype.rstrip(), "FAT16" if bpb.is_fat16 else "FAT12"))
    lines.append("| serial | %08X |" % bpb.serial)
    lines.append("| bytes/sector | %d |" % bpb.bps)
    lines.append("| sectors/cluster | %d |" % bpb.spc)
    lines.append("| reserved sectors | %d |" % bpb.resv)
    lines.append("| FATs | %d x %d sectors |" % (bpb.nfat, bpb.spf))
    lines.append("| root entries | %d |" % bpb.nroot)
    lines.append("| total sectors | %d (%d KB) |" % (bpb.total, bpb.total * bpb.bps // 1024))
    lines.append("| geometry | %d spt x %d heads |" % (bpb.spt, bpb.heads))
    lines.append("| FAT @ sector | %d (0x%X) |" % (bpb.fat_start, bpb.fat_start * bpb.bps))
    lines.append("| root @ sector | %d (0x%X) |" % (bpb.root_start, bpb.root_start * bpb.bps))
    lines.append("| data @ sector | %d (0x%X) |" % (bpb.data_start, bpb.data_start * bpb.bps))
    lines.append("")
    lines.append("## Boot chain (recovered)\n")
    lines.append("1. **BIOS** loads the VBR (sector 0) to `0000:7C00` and jumps to it.")
    lines.append("2. **VBR** (`msboot.asm`, MSWIN4.1) relocates the disk-parameter table, scans the")
    lines.append("   root directory for `IO      SYS`, reads its first **4 sectors** to `0000:0700`")
    lines.append("   (== `0070:0000`), and far-jumps to **`0070:0200`** -- the MSLOAD entry, past the")
    lines.append("   MZ header at `0070:0000`. Contract handed to IO.SYS: `DL`=drive, `SS:BP`=0000:7C00")
    lines.append("   (BPB in place), `[BP-4:BP-2]`=IO.SYS data LBA (MSLOAD reads it to continue loading).")
    lines.append("3. **IO.SYS** (MSLOAD -> MSBIO -> MSDOS -> SYSINIT) brings up the MS-DOS 7.10 kernel,")
    lines.append("   processes `CONFIG.SYS`, loads `COMMAND.COM`, runs `AUTOEXEC.BAT`.")
    lines.append("")
    lines.append("### VBR reconstruction: **%s** (%s)\n" % (vstatus, vdetail))
    lines.append("## Files (%d; %d bytes; %d already reconstructed in the corpus)\n"
                 % (len(rows), total, matched))
    lines.append("| name | size | clusters | md5 | corpus match |")
    lines.append("|---|---:|---:|---|---|")
    for fn, size, clus, ncl, attr, md5, corpus in rows:
        lines.append("| `%s` | %d | %d | `%s` | %s |"
                     % (fn, size, ncl, md5[:12], ("`%s`" % corpus) if corpus else "*(new)*"))
    lines.append("")
    with open(os.path.join(outdir, "README.md"), "w") as fh:
        fh.write("\n".join(lines))

    return rows, matched, total


def main(argv):
    img = _default_img()
    outdir = os.path.join(_design_root(), "recon", "bootdisk")
    do_extract = True
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--img" and i + 1 < len(argv):
            img = argv[i + 1]; i += 1
        elif a == "--out" and i + 1 < len(argv):
            outdir = argv[i + 1]; i += 1
        elif a == "--no-extract":
            do_extract = False
        elif a in ("-h", "--help"):
            print(__doc__)
            return 0
        i += 1

    if not os.path.isfile(img):
        print("bootrecon: image not found: %s" % img)
        return 1
    os.makedirs(outdir, exist_ok=True)

    bpb, vbr, ents = _walk(img)
    chashes = _corpus_hashes()
    vstatus, vdetail = _verify_vbr(vbr, outdir)
    rows, matched, total = _emit(bpb, ents, vbr, vstatus, vdetail, chashes, outdir, do_extract)

    print("bootrecon: %s  (%s, %s)" % (os.path.basename(img), bpb.label.rstrip(), bpb.fstype.rstrip()))
    print("  %d files, %d bytes; %d match the corpus, %d new"
          % (len(rows), total, matched, len(rows) - matched))
    print("  VBR reconstruction: %s -- %s" % (vstatus, vdetail))
    print("  -> %s (README.md, MANIFEST.csv%s)"
          % (outdir, ", files/" if do_extract else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
