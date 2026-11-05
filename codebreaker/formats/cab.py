"""cab.py -- Microsoft cabinet (MSCF) files: list, extract and write, with no outside tool.

A cabinet holds FOLDERS (one compressed stream each, cut into CFDATA blocks of at most 32768
uncompressed bytes) and FILES (a name plus an offset and size inside one folder's stream). A
folder may run on into the next cabinet of a set: a file whose folder index is 0xFFFD is
continued FROM the previous cabinet, 0xFFFE continues INTO the next one, 0xFFFF both; the
cabinets name their neighbours (prev/next) and are looked up in the same directory.

    from codebreaker.formats import cab
    c = cab.read(path)             # c.info: set_id, index, prev, next, flags, folders
                                   # c.files: Entry(name, size, folder, offset, date, time,
                                   #                attributes, compression)
    cab.extract(path, outdir)      # -> {name: output path}; names=[...] picks some
    cab.write([(name, data)], out_path, compress="mszip")       # or "none"; -> size

Decompression is done for stored (NONE), MSZIP (zlib) and QUANTUM (written from the published
description of the format) by formats.dll (native/formats/cab, docs/NATIVE.md step 4). LZX cabinets are listed but not extracted (CabError).
Block checksums are verified where a cabinet carries them.

    python -m codebreaker cab list <file.cab>
    python -m codebreaker cab extract <file.cab> <outdir> [names...]
    python -m codebreaker cab create <out.cab> <file-or-folder>... [--store]
"""
import collections
import os
import sys

from codebreaker.formats import native

COMPRESSION = {0: "NONE", 1: "MSZIP", 2: "QUANTUM", 3: "LZX"}
FROM_PREV, TO_NEXT, PREV_AND_NEXT = 0xFFFD, 0xFFFE, 0xFFFF

# Blocks hold 32768 uncompressed bytes (all but the last of a folder). Microsoft's decoder
# and 7-Zip count on that: each MSZIP block is inflated at the START of a 32768-byte window
# that still holds the block before it, so a block's back-references only land where the packer
# meant when the block before it filled the window exactly.
BLOCK = 32768
# The one exception made when WRITING. A CFDATA payload must not exceed 32768 bytes: the
# MSZIP decompressor in Windows 95 SETUP reads the payload into a fixed 32768-byte buffer and
# overruns on anything larger ("corrupt Setup (.CAB) file"). A full block of incompressible
# data cannot meet that (deflate's stored form adds 5 bytes, the "CK" signature 2), so such
# a block is cut at 32756 bytes and stored.
SHORT_BLOCK = 32756

Entry = collections.namedtuple(
    "Entry", "name size folder offset date time attributes compression")


class CabError(ValueError):
    pass


native.RAISES["CabError"] = CabError


class Cabinet:
    """One parsed cabinet: `path`, `info` (dict) and `files` (list of Entry)."""

    def __init__(self, path, info, files, folders, block_reserve):
        self.path, self.info, self.files = path, info, files
        self._folders = folders                  # [(offset of first CFDATA, block count, type)]
        self._block_reserve = block_reserve

    def folder_of(self, entry):
        """The real folder number of a file (the continuation markers name the first or the
        last folder of the cabinet)."""
        if entry.folder in (FROM_PREV, PREV_AND_NEXT):
            return 0
        return len(self._folders) - 1 if entry.folder == TO_NEXT else entry.folder


def _by_path(path, *rest):
    return None, (os.fspath(path),) + rest


def _cabinet(answer, _data, path):
    info, files, folders, block_reserve = answer
    return Cabinet(path, info, [Entry(*e) for e in files], folders, block_reserve)


@native.answered("cab.read", args_of=_by_path, result=_cabinet)
def read(path):
    """Parse the headers of one cabinet (nothing is decompressed)."""


@native.answered("cab.checksum", data_arg=None)
def checksum(payload, uncompressed):
    """The CFDATA checksum: 32-bit little-endian words of the block header (two sizes) and
    the payload XORed together; the 1-3 odd bytes at the end count most-significant first."""


@native.answered("cab.extract", args_of=lambda path, outdir, names=None: (
    None, (os.fspath(path), os.fspath(outdir), None if names is None else list(names))))
def extract(path, outdir, names=None):
    """Extract the files listed in one cabinet (all, or those in `names`, compared without
    regard to case) into `outdir`. Neighbouring cabinets are read as needed for files whose
    folder is continued. Returns {name: output path}."""


@native.answered("cab.write", args_of=lambda files, out_path, compress="mszip", set_id=0, index=0, prev=None,
                 next=None, short_blocks=True: (
    None, ([(name, bytes(data)) for name, data in files], os.fspath(out_path), compress, set_id, index,
           tuple(prev) if isinstance(prev, (list, tuple)) else prev,
           tuple(next) if isinstance(next, (list, tuple)) else next, short_blocks)))
def write(files, out_path, compress="mszip", set_id=0, index=0, prev=None, next=None,
          short_blocks=True):
    """Write a cabinet holding `files` = [(name, bytes)] in one folder; returns its size.

    `short_blocks` keeps every MSZIP data block within what Windows 95 SETUP can take, at the
    price of 7-Zip rejecting cabinets that hold incompressible data; pass False for a cabinet
    meant for present-day tools only. A block that will not compress to a 32768-byte payload is
    written short and stored (SHORT_BLOCK); the block after a short one is deflated with no
    dictionary, so it is read the same by every decoder.

    `set_id`, `index`, `prev` and `next` (a cabinet name, or (cabinet name, disk label)) are
    the fields that tie cabinets into a set; a program that walks the set, as Windows 95
    SETUP does, needs them carried over when a cabinet of a set is rebuilt. The output is a
    valid cabinet delivering the same files, not a byte copy of any original cabinet."""


# --- command -------------------------------------------------------------------------------
def _gather(paths):
    files = []
    for path in paths:
        if os.path.isdir(path):
            for folder, _dirs, names in sorted(os.walk(path)):
                for name in sorted(names):
                    full = os.path.join(folder, name)
                    with open(full, "rb") as f:
                        files.append((os.path.relpath(full, path).replace("/", "\\"), f.read()))
        else:
            with open(path, "rb") as f:
                files.append((os.path.basename(path), f.read()))
    return files


def main(argv):
    """python -m codebreaker cab list <file.cab>
    python -m codebreaker cab extract <file.cab> <outdir> [names...]
    python -m codebreaker cab create <out.cab> <file-or-folder>... [--store]"""
    store = "--store" in argv
    args = [a for a in argv if a != "--store"]
    try:
        if len(args) == 2 and args[0] == "list":
            cab = read(args[1])
            info = cab.info
            print("%s: set %d, cabinet %d, %d bytes, folders: %s" % (
                os.path.basename(args[1]), info["set_id"], info["index"], info["size"],
                ", ".join(info["folders"]) or "none"))
            for key in ("prev", "next"):
                if info[key]:
                    print("  %s cabinet: %s" % (key, info[key]))
            marks = {FROM_PREV: "  (from previous cabinet)", TO_NEXT: "  (into next cabinet)",
                     PREV_AND_NEXT: "  (from previous, into next)"}
            for e in cab.files:
                print("%10d  %04d-%02d-%02d  %-8s %s%s" % (
                    e.size, 1980 + (e.date >> 9), (e.date >> 5) & 15, e.date & 31,
                    e.compression, e.name, marks.get(e.folder, "")))
            print("%d files, %d bytes" % (len(cab.files), sum(e.size for e in cab.files)))
            return 0
        if len(args) >= 3 and args[0] == "extract":
            done = extract(args[1], args[2], args[3:] or None)
            missing = [n for n in args[3:] if n.lower() not in {d.lower() for d in done}]
            for name in missing:
                print("not in the cabinet: %s" % name, file=sys.stderr)
            print("extracted %d files to %s" % (len(done), args[2]))
            return 1 if missing else 0
        if len(args) >= 3 and args[0] == "create":
            files = _gather(args[2:])
            size = write(files, args[1], compress="none" if store else "mszip")
            print("wrote %s: %d files, %d bytes" % (args[1], len(files), size))
            return 0
    except (CabError, OSError) as problem:
        print("cab: %s" % problem, file=sys.stderr)
        return 1
    print(main.__doc__, file=sys.stderr)
    return 2
