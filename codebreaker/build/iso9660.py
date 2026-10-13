#!/usr/bin/env python3
"""Dependency-light ISO9660 reader/extractor.

Written because 7-Zip refuses WINDOWS_3.1.ISO even though its volume
descriptors are well-formed. Walks the primary (or supplementary/Joliet)
descriptor's directory tree and extracts every file.

    python3 iso9660.py WINDOWS_3.1.ISO --list
    python3 iso9660.py WINDOWS_3.1.ISO --extract extracted
"""
import argparse
import os
import struct
import sys

SECTOR = 2048


class DirRecord:
    __slots__ = ("name", "extent", "length", "flags", "is_dir")

    def __init__(self, name, extent, length, flags):
        self.name = name
        self.extent = extent
        self.length = length
        self.flags = flags
        self.is_dir = bool(flags & 0x02)

    def __repr__(self):
        kind = "DIR " if self.is_dir else "FILE"
        return f"<{kind} {self.name} @{self.extent} {self.length}b>"


class Iso9660:
    def __init__(self, path):
        self.f = open(path, "rb")
        self.f.seek(0, os.SEEK_END)
        self.size = self.f.tell()
        self.joliet = False
        self.root = self._find_root()

    def sector(self, n, count=1):
        self.f.seek(n * SECTOR)
        return self.f.read(SECTOR * count)

    def _find_root(self):
        """Scan volume descriptors; prefer Joliet (long names) if present."""
        primary = None
        supplementary = None
        n = 16
        while n * SECTOR < self.size:
            vd = self.sector(n)
            if vd[1:6] != b"CD001":
                break
            vtype = vd[0]
            if vtype == 0xFF:  # terminator
                break
            if vtype == 1:
                primary = vd
            elif vtype == 2 and self._is_joliet(vd):
                supplementary = vd
            n += 1

        vd = supplementary if supplementary is not None else primary
        if vd is None:
            raise ValueError("no ISO9660 primary volume descriptor found")
        self.joliet = supplementary is not None

        rec = vd[156:190]
        extent = struct.unpack("<I", rec[2:6])[0]
        length = struct.unpack("<I", rec[10:14])[0]
        return DirRecord("", extent, length, 0x02)

    @staticmethod
    def _is_joliet(vd):
        """Joliet is signalled by a UCS-2 escape sequence at offset 88."""
        esc = vd[88:120]
        return esc.startswith((b"%/@", b"%/C", b"%/E"))

    def _decode(self, raw):
        if self.joliet:
            try:
                return raw.decode("utf-16-be")
            except UnicodeDecodeError:
                pass
        return raw.decode("latin-1")

    def read_dir(self, rec):
        """Parse a directory extent into DirRecords, skipping '.' and '..'."""
        nsec = (rec.length + SECTOR - 1) // SECTOR
        data = self.sector(rec.extent, nsec)[: rec.length]
        out = []
        pos = 0
        while pos < len(data):
            rlen = data[pos]
            if rlen == 0:
                # padding to next sector boundary
                pos = (pos // SECTOR + 1) * SECTOR
                continue
            entry = data[pos : pos + rlen]
            pos += rlen
            if len(entry) < 33:
                continue
            extent = struct.unpack("<I", entry[2:6])[0]
            length = struct.unpack("<I", entry[10:14])[0]
            flags = entry[25]
            namelen = entry[32]
            raw = entry[33 : 33 + namelen]
            if namelen == 1 and raw in (b"\x00", b"\x01"):
                continue  # . and ..
            name = self._decode(raw).split(";")[0].rstrip(".")
            out.append(DirRecord(name, extent, length, flags))
        return out

    def walk(self, rec=None, prefix=""):
        """Yield (relative_path, DirRecord) for every file, depth-first."""
        if rec is None:
            rec = self.root
        for e in self.read_dir(rec):
            path = f"{prefix}/{e.name}" if prefix else e.name
            if e.is_dir:
                yield from self.walk(e, path)
            else:
                yield path, e

    def read_file(self, rec):
        self.f.seek(rec.extent * SECTOR)
        return self.f.read(rec.length)

    def close(self):
        self.f.close()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("iso")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--extract", metavar="OUTDIR")
    args = ap.parse_args()

    iso = Iso9660(args.iso)
    print(f"# {args.iso}  ({iso.size} bytes, joliet={iso.joliet})", file=sys.stderr)

    count = total = 0
    for path, rec in iso.walk():
        count += 1
        total += rec.length
        if args.list or not args.extract:
            print(f"{rec.length:>9}  {path}")
        if args.extract:
            out = os.path.join(args.extract, path.replace("/", os.sep))
            os.makedirs(os.path.dirname(out) or ".", exist_ok=True)
            with open(out, "wb") as fh:
                fh.write(iso.read_file(rec))
            if not args.list:
                print(f"{rec.length:>9}  {path}")
    print(f"\n{count} files, {total} bytes", file=sys.stderr)
    iso.close()


if __name__ == "__main__":
    main()
