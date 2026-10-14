"""floppy.py -- a FAT12 boot floppy made from rebuilt files, laid out as an original one is.

    python -m codebreaker boot-floppy [--image ORIGINAL.img] [--out FILE.img] [--extract DIR]

    from codebreaker.build import floppy
    disk = floppy.read(img_bytes)             .boot (512 bytes), .entries [Entry], .bpb
    img = floppy.make(disk, boot, contents)   contents {NAME.EXT: bytes}

The original floppy says what goes on the disk and how: its boot parameters (the BPB in the boot
sector), its root directory -- the files, in their order, with their attributes and times, and
the volume label. make() keeps all of that and puts in the boot sector given and each file's
contents given (a file not given keeps the original's), each file in one run of clusters, in
the order of the directory, both FATs written to match.

The command builds the Windows 95 Setup boot floppy (setup/media/boot.img of the corpus) from
what CodeBreaker rebuilt: the boot sector from Windows95/design/recon/bootdisk/msboot.asm
(nasm), every real-mode program from its source (`dos-source`, state/bootfloppy/src/<NAME>),
and the text files (CONFIG.SYS, AUTOEXEC.BAT, MSDOS.SYS ...) as they are -- they are source.
A file with no rebuilt program is the original's, and the command says which.
"""
import os
import struct
import subprocess

_ATTR_LABEL, _ATTR_DIR, _LFN = 0x08, 0x10, 0x0F


class Entry(object):
    def __init__(self, raw, data):
        self.raw = bytearray(raw)       # the 32-byte directory entry
        self.data = data                # the file's bytes (None for the volume label)

    @property
    def name(self):
        base, ext = self.raw[:8].decode("latin1").strip(), self.raw[8:11].decode("latin1").strip()
        return base + ("." + ext if ext else "")

    @property
    def is_file(self):
        return not self.raw[11] & (_ATTR_LABEL | _ATTR_DIR) and self.raw[11] != _LFN


class Disk(object):
    def __init__(self, boot, entries, bpb):
        self.boot, self.entries, self.bpb = boot, entries, bpb


def _bpb(boot):
    bps, spc, rsv, nfat, nroot, total, _media, spf = struct.unpack_from("<HBHBHHBH", boot, 11)
    return dict(bps=bps, spc=spc, rsv=rsv, nfat=nfat, nroot=nroot, total=total, spf=spf,
                root=(rsv + nfat * spf) * bps, data=(rsv + nfat * spf) * bps + nroot * 32)


def read(img):
    """A FAT12 floppy image. -> Disk (root directory only: a boot floppy has no folders)."""
    b = _bpb(img)
    fat = img[b["rsv"] * b["bps"]:(b["rsv"] + b["spf"]) * b["bps"]]
    cluster = b["spc"] * b["bps"]

    def chain(c):
        while 2 <= c < 0xFF8:
            yield c
            v = struct.unpack_from("<H", fat, c * 3 // 2)[0]
            c = (v >> 4) if c & 1 else (v & 0xFFF)
    entries = []
    for i in range(b["nroot"]):
        raw = img[b["root"] + 32 * i:b["root"] + 32 * i + 32]
        if raw[0] == 0:
            break
        if raw[0] == 0xE5:
            continue
        e = Entry(raw, None)
        if e.is_file:
            size, first = struct.unpack_from("<I", raw, 28)[0], struct.unpack_from("<H", raw, 26)[0]
            e.data = b"".join(img[b["data"] + (c - 2) * cluster:b["data"] + (c - 1) * cluster] for c in chain(first))[:size]
        entries.append(e)
    return Disk(bytes(img[:512]), entries, b)


def make(disk, boot, contents):
    """The image: `boot` as the boot sector, the directory of `disk` with `contents` in it."""
    b = disk.bpb
    if _bpb(boot) != b:
        raise ValueError("the boot sector's parameters are not the original floppy's")
    img = bytearray(b["total"] * b["bps"])
    img[:512] = boot
    cluster = b["spc"] * b["bps"]
    fat = bytearray(b["spf"] * b["bps"])

    def put(c, v):
        at = c * 3 // 2
        w = struct.unpack_from("<H", fat, at)[0]
        w = (w & 0x000F) | (v << 4) if c & 1 else (w & 0xF000) | v
        struct.pack_into("<H", fat, at, w)
    put(0, 0xF00 | boot[21])                                 # the media byte
    put(1, 0xFFF)
    nxt = 2
    for i, e in enumerate(disk.entries):
        raw = bytearray(e.raw)
        if e.is_file:
            data = contents.get(e.name.upper(), e.data)
            n = (len(data) + cluster - 1) // cluster
            if nxt + n - 2 > (b["total"] * b["bps"] - b["data"]) // cluster:
                raise ValueError("the files do not fit on the floppy")
            first = nxt if n else 0
            for k in range(n):
                put(nxt + k, nxt + k + 1 if k < n - 1 else 0xFFF)
            at = b["data"] + (nxt - 2) * cluster
            img[at:at + len(data)] = data
            nxt += n
            struct.pack_into("<HI", raw, 26, first, len(data))
        img[b["root"] + 32 * i:b["root"] + 32 * i + 32] = raw
    for k in range(b["nfat"]):
        at = (b["rsv"] + k * b["spf"]) * b["bps"]
        img[at:at + len(fat)] = fat
    return bytes(img)


def _boot_sector(design):
    """The boot sector assembled from its source (nasm). -> (bytes, source path)"""
    from codebreaker.base import settings
    src = os.path.join(design, "recon", "bootdisk", "msboot.asm")
    nasm = settings.get("nasm") or "nasm"
    if os.path.isdir(nasm):
        nasm = os.path.join(nasm, "nasm.exe")
    os.makedirs(settings.state("bootfloppy"), exist_ok=True)
    out = settings.state("bootfloppy", "msboot.bin")
    subprocess.run([nasm, "-f", "bin", src, "-o", out], check=True, capture_output=True)
    with open(out, "rb") as f:
        return f.read(), src


def _rebuilt(src_dir, name):
    """The file built from a program's source in state/bootfloppy/src/<NAME>, or None."""
    d = os.path.join(src_dir, name)
    if not os.path.isdir(d):
        return None
    stem = os.path.splitext(name)[0].lower()
    # the program laid out as the original file is (dos_writer: file/<NAME>), else as built
    for cand in (os.path.join("file", name.upper()), name.lower(), stem + ".exe", stem + ".com", stem + ".bin"):
        p = os.path.join(d, cand)
        if os.path.isfile(p):
            return p
    return None


def main(argv):
    """python -m codebreaker boot-floppy [--image ORIGINAL.img] [--out FILE.img] [--extract DIR]

    Build the Windows 95 Setup boot floppy from CodeBreaker's rebuilt files (see the top).
    --extract writes the original floppy's files to DIR and stops."""
    from codebreaker.base import corpus, settings
    from codebreaker.formats import dos
    from codebreaker.source import dos_writer
    image = argv[argv.index("--image") + 1] if "--image" in argv else \
        os.path.join(corpus.root(), "setup", "media", "boot.img")
    with open(image, "rb") as f:
        disk = read(f.read())
    if "--extract" in argv:
        out = argv[argv.index("--extract") + 1]
        os.makedirs(out, exist_ok=True)
        for e in disk.entries:
            if e.is_file:
                with open(os.path.join(out, e.name), "wb") as f:
                    f.write(e.data)
        print("%d files written to %s" % (sum(e.is_file for e in disk.entries), out))
        return 0
    original = settings.state("bootfloppy", "original")
    src_dir = settings.state("bootfloppy", "src")
    os.makedirs(original, exist_ok=True)
    boot, boot_src = _boot_sector(corpus.design_dir())
    print("boot sector: %s (%s the original's)" % (os.path.basename(boot_src), "the same as" if boot == disk.boot
                                                   else "DIFFERENT from"))
    contents, kept = {}, []
    for e in disk.entries:
        if not e.is_file:
            continue
        path = os.path.join(original, e.name)
        if not os.path.isfile(path):
            with open(path, "wb") as f:
                f.write(e.data)
        if not dos.kind(path):
            text = all(32 <= c < 127 or c in (9, 10, 13, 26) for c in e.data)
            print("  %-13s %s" % (e.name, "text: as it is" if text else "not a real-mode program: the original's"))
            if not text:
                kept.append(e.name)
            continue
        built = _rebuilt(src_dir, e.name)
        if not built:
            r = dos_writer.reconstruct(path, os.path.join(src_dir, e.name))
            built = (r.get("file") or r["image"]) if (r["same"] or r.get("equivalent")) else None
        if not built:
            print("  %-13s does not build back: the original's" % e.name)
            kept.append(e.name)
            continue
        with open(built, "rb") as f:
            contents[e.name.upper()] = f.read()
        print("  %-13s rebuilt from source (%s)" % (e.name, os.path.relpath(built, src_dir)))
    # a program the original stores packed by a packer CodeBreaker does not write (PKLITE ...)
    # is rebuilt unpacked, and bigger: when the disk is too small for them all, the biggest of
    # them go on as the original stores them, until everything fits
    unpacked = sorted((n for n in contents if dos.packer(os.path.join(original, n)) not in (None, "EXEPACK", "moves itself")),
                      key=lambda n: -len(contents[n]))
    while True:
        try:
            img = make(disk, boot, contents)
            break
        except ValueError as e:
            if "do not fit" not in str(e) or not unpacked:
                raise
            n = unpacked.pop(0)
            del contents[n]
            kept.append("%s (stored packed by %s; rebuilt unpacked it does not fit)" % (n, dos.packer(os.path.join(original, n))))
    out = argv[argv.index("--out") + 1] if "--out" in argv else settings.state("bootfloppy", "boot95.img")
    with open(out, "wb") as f:
        f.write(img)
    print("%s: %d programs rebuilt from source; the original's: %s" % (
        out, len(contents), ", ".join(kept) or "none"))
    return 0
