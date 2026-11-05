"""media.py -- read media and containers: floppy, hard-disk and CD images, module bundles, and
the Xbox / Xbox 360 containers (formats/xbox/volumes.py: STFS, discs, GoD, FATX, ZAR, XZP, PCK).

    from codebreaker.formats import media
    for v in media.volumes(path):          every volume inside an image file (or GoD folder)
        v.kind                             "fat12" | "fat16" | "fat32" | "iso9660" | "bundle" |
                                           "stfs" | "xdvdfs" | "god" | "fatx" | "zar" | "xzp" | "pck"
        v.label, v.where                   its name; where in the image it is
        v.boot                             {field: value} of its boot sector / volume descriptor
        for e in v.files():                every file and folder: e.path  e.size  e.is_dir  e.when
            data = v.read(e)
    media.extract(path, folder, names=())  write the files out; -> number written
    media.put(path, {"WINDOWS/SYSTEM/X.VXD": data})   write files into an image's FAT volume in
                                           place (replaced or added; folders made as needed)

    python -m codebreaker media list <image> [--boot] [--plain]
    python -m codebreaker media extract <image> <folder> [name ...]

One image file can hold several volumes: a floppy image is one FAT volume; a hard-disk image
(also a "flat" virtual disk) is a partition table and a FAT volume per partition, logical
drives included; a CD image is an ISO-9660 volume and, when it is bootable by floppy emulation
(El Torito), the boot floppy inside it as a FAT volume of its own; and a Windows 1.x boot image
(WIN100.BIN with its .OVL) is a bundle of system modules, each written out as an ordinary
one-file executable (codebreaker.formats.ne does the taking apart).

FAT: 12-, 16- and 32-bit tables told apart by the number of clusters, as DOS does; long file
names are read (`--plain` shows the 8.3 names) and put() writes by the 8.3 ones; floppies from before DOS 2.0 carry no parameter
block, so their layout is taken from the media byte and the image size. ISO-9660: the primary
names, or the long (Joliet) names when the disc has them (`--plain` for the primary ones);
images with raw 2352-byte sectors are read too. Files are read on demand, never the whole image.
The FAT and ISO-9660 readers, the partition table and put() are formats.dll's (native/formats/media,
docs/NATIVE.md step 4).
"""
import fnmatch
import os
import struct

from codebreaker.formats import native


class Entry(object):
    """One file or folder of a volume."""
    def __init__(self, path, size, is_dir, start, when="", short=None, attributes=0):
        self.path, self.size, self.is_dir, self.start = path, size, is_dir, start
        self.when, self.short, self.attributes = when, short or path, attributes

    def __repr__(self):
        return "Entry(%r, %d)" % (self.path, self.size)


def _entry(t):
    e = Entry(*t[:7])
    if len(t) > 7:
        e.extents = list(t[7])
    return e


class _Source(object):
    """Bytes of an image file from an offset on, read as asked for."""
    def __init__(self, path, offset=0):
        self.path, self.offset = path, offset

    def read(self, at, n):
        with open(self.path, "rb") as f:
            f.seek(self.offset + at)
            return f.read(n)


# FAT ------------------------------------------------------------------------------------------
class FatVolume(object):
    """A FAT12/16/32 volume (its attributes: the boot sector's fields, `boot`, the table `fat`)."""

    def __init__(self, source, where="", size=None, _attrs=None):
        self.src = source
        if _attrs is None:
            _attrs = native.call("media.fat", None, os.fspath(source.path), source.offset, where, size)
        self.__dict__.update(_attrs)
        self._entries = None

    def files(self):
        """Every file and folder, folders before what is in them."""
        if self._entries is None:
            entries, self.label = native.call("media.fat_files", self.fat, os.fspath(self.src.path), self.src.offset,
                                              self.bits, self.spc, self.bps, self.data_at, self.root_at, self.roots,
                                              self.root_cluster, self.label)
            self._entries = [_entry(t) for t in entries]
        return self._entries

    def _next(self, c):
        return native.call("media.fat_next", self.fat, self.bits, c)

    def _chain(self, cluster, size=None):
        """The bytes of a cluster chain (all of it, or the first `size` bytes)."""
        return native.call("media.fat_read", self.fat, os.fspath(self.src.path), self.src.offset, self.bits,
                           self.spc, self.bps, self.data_at, cluster, size)

    def read(self, entry):
        return b"" if entry.is_dir else self._chain(entry.start, entry.size)


# ISO 9660 ---------------------------------------------------------------------------------------
class IsoVolume(object):
    kind = "iso9660"

    def __init__(self, source, sector=2048, skip=0, long_names=True, _attrs=None):
        """`sector`/`skip`: 2048/0 for a plain image; 2352/16 or 2352/24 for raw sectors."""
        self.src = source
        if _attrs is None:
            _attrs = native.call("media.iso", None, os.fspath(source.path), source.offset, sector, skip, long_names)
        self.__dict__.update(_attrs)
        self._entries = None

    def files(self):
        if self._entries is None:
            self._entries = [_entry(t) for t in native.call(
                "media.iso_files", None, os.fspath(self.src.path), self.src.offset, self.sector, self.skip,
                self.long_names, self.root[0], self.root[1])]
        return self._entries

    def read(self, entry):
        if entry.is_dir:
            return b""
        return native.call("media.iso_read", None, os.fspath(self.src.path), self.src.offset, self.sector,
                           self.skip, [tuple(x) for x in entry.extents])

    def boot_image(self):
        """(offset in the image file, bytes, emulated media) of the El Torito boot image: media
        0 none (a loader), 1 / 2 / 3 a 1.2 / 1.44 / 2.88 MB floppy, 4 a hard disk. None when
        the disc is not bootable."""
        return native.call("media.iso_boot_image", None, os.fspath(self.src.path), self.src.offset, self.sector,
                           self.skip, self.boot_catalog)


# a bundle of system modules ---------------------------------------------------------------------
class BundleVolume(object):
    """A Windows 1.x boot image: system modules stored together (see formats.ne.open_boot_image)."""
    kind = "bundle"

    def __init__(self, path):
        from codebreaker.formats import ne
        self._ne, self.where = ne, ""
        self.modules = ne.open_boot_image(path)
        if len(self.modules) < 2:
            raise ValueError("not a bundle of modules")
        self.label = os.path.basename(path)
        self.boot = {"modules": len(self.modules)}
        self._built = {}

    def files(self):
        return [Entry(name + ".EXE", len(self._bytes(name)), False, m.ne_off, short=name + ".EXE")
                for name, m in self.modules.items()]

    def _bytes(self, name):
        if name not in self._built:
            self._built[name] = self._ne.standalone(self.modules[name])
        return self._built[name]

    def read(self, entry):
        return self._bytes(entry.path[:-4])


# any image ------------------------------------------------------------------------------------
def volumes(path, long_names=True):
    """Every volume inside an image file. ValueError when nothing in it is recognised."""
    from codebreaker.formats.xbox import volumes as xbox             # first: an Xbox 360 disc also
    found = xbox.volumes(path, long_names)                           # carries an ISO-9660 video part
    if found:
        return found
    if os.path.isdir(path):
        raise ValueError("%s: a folder, and not a Games-on-Demand package" % os.path.basename(path))
    out = []
    for kind, offset, attrs in native.call("media.volumes", None, os.fspath(path), long_names):
        if kind == "iso":
            out.append(IsoVolume(_Source(path, offset), _attrs=attrs))
        else:
            out.append(FatVolume(_Source(path, offset), _attrs=attrs))
    if out:
        return out
    try:
        return [BundleVolume(path)]
    except (ValueError, struct.error, IndexError, KeyError):
        raise ValueError("%s: no FAT volume, partition table, ISO-9660 volume or module bundle found" % os.path.basename(path))


def put(path, files, volume=0):
    """Write `files` {"FOLDER/NAME.EXT": bytes} into FAT volume number `volume` of the image at
    `path`, in place. Paths are 8.3 names in any case, folder by folder. A file that is there is
    replaced -- its clusters freed; its directory entry, attributes and long name kept, with the
    new start, size and time -- and one that is not is added, a missing folder made. Every copy
    of the FAT is written. -> [(path, "replaced" | "added")]"""
    vol = volumes(path)[volume]
    if not isinstance(vol, FatVolume) or vol.old:
        raise ValueError("not a FAT volume with a parameter block")
    return native.call("media.put", None, os.fspath(path), vol.src.offset,
                       [(name, bytes(data)) for name, data in files.items()])


def _safe(path):
    parts = [p for p in path.replace("\\", "/").split("/") if p not in ("", ".", "..")]
    return os.path.join(*["".join(c if c not in '<>:"|?*' and ord(c) >= 32 else "_" for c in p) for p in parts])


def extract(path, folder, names=(), long_names=True):
    """Write the files of an image under `folder` (each volume after the first in a folder of
    its own). `names` limits it to matching names or paths (wildcards allowed). -> files written"""
    n = 0
    for i, v in enumerate(volumes(path, long_names)):
        base = folder if i == 0 else os.path.join(folder, "volume%d" % (i + 1))
        for e in v.files():
            wanted = not names or any(fnmatch.fnmatch(e.path.lower(), w.lower()) or
                                      fnmatch.fnmatch(e.path.rsplit("/", 1)[-1].lower(), w.lower()) for w in names)
            if e.is_dir or not wanted:
                continue
            out = os.path.join(base, _safe(e.path))
            os.makedirs(os.path.dirname(out) or ".", exist_ok=True)
            with open(out, "wb") as f:
                f.write(v.read(e))
            n += 1
    return n


def main(argv):
    """python -m codebreaker media list <image> [--boot] [--plain]
    python -m codebreaker media extract <image> <folder> [name ...] [--plain]

    List or extract the files of a floppy, hard-disk or CD image, or of a Windows 1.x boot
    image. --boot also prints each volume's boot sector fields; --plain uses 8.3 / primary names."""
    flags = [a for a in argv if a.startswith("--")]
    args = [a for a in argv if not a.startswith("--")]
    if len(args) < 2 or args[0] not in ("list", "extract") or (args[0] == "extract" and len(args) < 3):
        print(main.__doc__)
        return 2
    long_names = "--plain" not in flags
    if args[0] == "extract":
        print("%d files written to %s" % (extract(args[1], args[2], args[3:], long_names), args[2]))
        return 0
    for v in volumes(args[1], long_names):
        files = v.files()
        print("%s volume %r%s: %d files, %d folders, %d bytes" % (
            v.kind, v.label, " (%s)" % v.where if v.where else "", sum(1 for e in files if not e.is_dir),
            sum(1 for e in files if e.is_dir), sum(e.size for e in files if not e.is_dir)))
        if "--boot" in flags:
            for k, value in v.boot.items():
                print("    %-20s %s" % (k, value))
        for e in files:
            print("  %10s  %-16s  %s" % ("<folder>" if e.is_dir else e.size, e.when, e.path if long_names else e.short))
    return 0
