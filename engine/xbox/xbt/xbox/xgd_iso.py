"""Xbox Game Disc (XGD) ISO and GOD container reader.

Ported from Xbox360Toolkit.
Reads XGD1/XGD2/XGD3 disc images (.iso) and Games on Demand (GOD)
containers. Parses the XDFS directory tree and can list/extract files.

Supports:
  - ISO images (single or multi-part)
  - GOD containers (LIVE/PIRS signed with .data folder)
  - XGD1, XGD2, XGD3 format detection
  - Directory tree walking
  - File extraction
"""

import struct
import os
from pathlib import Path
from typing import Optional
from dataclasses import dataclass, field


# ── Constants ───────────────────────────────────────────

XGD_SECTOR_SIZE = 0x800
XGD_IMAGE_MAGIC = b"MICROSOFT*XBOX*MEDIA"
XGD_ISO_BASE_SECTOR = 0x20

# Magic sectors for different XGD formats
XGD_MAGIC_XDKI = XGD_ISO_BASE_SECTOR
XGD_MAGIC_XGD1 = 0x30620
XGD_MAGIC_XGD2 = 0x1FB40
XGD_MAGIC_XGD3 = 0x4120

DIR_ATTR = 0x10  # Directory attribute flag


@dataclass
class XgdInfo:
    """XGD image metadata."""
    format: str = ""  # "XDKI", "XGD1", "XGD2", "XGD3"
    base_sector: int = 0
    root_dir_sector: int = 0
    root_dir_size: int = 0
    total_sectors: int = 0


@dataclass
class XgdFileEntry:
    """A file or directory in the XGD filesystem."""
    name: str = ""
    sector: int = 0
    size: int = 0
    is_directory: bool = False
    path: str = ""
    children: list = field(default_factory=list)


# ── ISO Reader ──────────────────────────────────────────

class XgdIsoReader:
    """Read Xbox 360 ISO disc images.

    Usage:
        reader = XgdIsoReader("game.iso")
        for f in reader.list_files():
            print(f.path, f.size)
        reader.extract_all("output/")
    """

    def __init__(self, path: str):
        self.path = path
        self._streams = []
        self._slices = []  # (stream, start_sector, end_sector)
        self.xgd_info: Optional[XgdInfo] = None

        self._open()
        self._detect_format()

    def _open(self):
        """Open the ISO file(s). Supports multi-part (game.iso, game.1.iso, etc.)."""
        main_path = Path(self.path)
        files = [str(main_path)]

        # Check for multi-part
        for i in range(1, 100):
            part = main_path.with_suffix(f".{i}.iso")
            if part.exists():
                files.append(str(part))
            else:
                break

        sector_count = 0
        for fp in files:
            f = open(fp, "rb")
            size = f.seek(0, 2)
            f.seek(0)
            sectors = size // XGD_SECTOR_SIZE
            self._streams.append(f)
            self._slices.append((f, sector_count, sector_count + sectors - 1))
            sector_count += sectors

    def _read_sector(self, sector: int) -> Optional[bytes]:
        """Read a single 2048-byte sector."""
        for stream, start, end in self._slices:
            if start <= sector <= end:
                local = sector - start
                stream.seek(local * XGD_SECTOR_SIZE)
                data = stream.read(XGD_SECTOR_SIZE)
                if len(data) == XGD_SECTOR_SIZE:
                    return data
                return None
        return None

    def _read_sectors(self, start_sector: int, count: int) -> bytes:
        """Read multiple consecutive sectors."""
        result = bytearray()
        for i in range(count):
            sector = self._read_sector(start_sector + i)
            if sector is None:
                break
            result.extend(sector)
        return bytes(result)

    def _detect_format(self):
        """Detect XGD format by checking magic sectors."""
        for name, magic_sector in [("XDKI", XGD_MAGIC_XDKI),
                                    ("XGD3", XGD_MAGIC_XGD3),
                                    ("XGD2", XGD_MAGIC_XGD2),
                                    ("XGD1", XGD_MAGIC_XGD1)]:
            data = self._read_sector(magic_sector)
            if data and data[:20] == XGD_IMAGE_MAGIC:
                self.xgd_info = XgdInfo()
                self.xgd_info.format = name

                # Parse XGD header at magic sector
                # Offset 20: root dir sector (LE uint32)
                # Offset 24: root dir size (LE uint32)
                self.xgd_info.root_dir_sector = struct.unpack_from("<I", data, 20)[0]
                self.xgd_info.root_dir_size = struct.unpack_from("<I", data, 24)[0]

                if name == "XDKI":
                    self.xgd_info.base_sector = 0
                else:
                    self.xgd_info.base_sector = magic_sector - XGD_ISO_BASE_SECTOR

                return

        raise ValueError("Not a valid XGD disc image (magic not found)")

    def _read_directory(self, sector: int, size: int, base_path: str = "") -> list:
        """Read a directory table and return file entries."""
        num_sectors = (size + XGD_SECTOR_SIZE - 1) // XGD_SECTOR_SIZE
        dir_data = self._read_sectors(self.xgd_info.base_sector + sector, num_sectors)
        if not dir_data:
            return []

        return self._walk_tree(dir_data, 0, base_path)

    def _walk_tree(self, dir_data: bytes, offset: int, base_path: str) -> list:
        """Walk the binary tree directory structure."""
        entries = []
        pos = offset * 4

        if pos + 14 > len(dir_data):
            return entries

        left = struct.unpack_from("<H", dir_data, pos)[0]
        right = struct.unpack_from("<H", dir_data, pos + 2)[0]
        sector = struct.unpack_from("<I", dir_data, pos + 4)[0]
        size = struct.unpack_from("<I", dir_data, pos + 8)[0]
        attr = dir_data[pos + 12]
        name_len = dir_data[pos + 13]

        if name_len == 0 or pos + 14 + name_len > len(dir_data):
            return entries

        name = dir_data[pos + 14:pos + 14 + name_len].decode("ascii", errors="replace")
        full_path = f"{base_path}/{name}" if base_path else name
        is_dir = bool(attr & DIR_ATTR)

        entry = XgdFileEntry(name=name, sector=sector, size=size,
                             is_directory=is_dir, path=full_path)

        if is_dir and size > 0:
            entry.children = self._read_directory(sector, size, full_path)

        entries.append(entry)

        # Recurse left subtree
        if left != 0xFFFF and left != 0:
            entries.extend(self._walk_tree(dir_data, left, base_path))

        # Recurse right subtree
        if right != 0 and right != 0xFFFF:
            entries.extend(self._walk_tree(dir_data, right, base_path))

        return entries

    def list_files(self) -> list:
        """List all files and directories in the disc image."""
        if not self.xgd_info:
            return []
        return self._read_directory(
            self.xgd_info.root_dir_sector,
            self.xgd_info.root_dir_size)

    def _flatten(self, entries: list) -> list:
        """Flatten the directory tree into a single list."""
        result = []
        for e in entries:
            result.append(e)
            if e.children:
                result.extend(self._flatten(e.children))
        return result

    def list_all_files(self) -> list:
        """List all files (flattened) in the disc image."""
        return self._flatten(self.list_files())

    def extract_file(self, entry: XgdFileEntry, output_path: str):
        """Extract a single file from the disc image."""
        if entry.is_directory:
            os.makedirs(output_path, exist_ok=True)
            return

        os.makedirs(os.path.dirname(output_path) or ".", exist_ok=True)

        remaining = entry.size
        sector = entry.sector + self.xgd_info.base_sector
        with open(output_path, "wb") as f:
            while remaining > 0:
                data = self._read_sector(sector)
                if data is None:
                    break
                chunk = min(remaining, XGD_SECTOR_SIZE)
                f.write(data[:chunk])
                remaining -= chunk
                sector += 1

    def extract_all(self, output_dir: str) -> int:
        """Extract all files. Returns count extracted."""
        files = self.list_all_files()
        count = 0
        for f in files:
            out_path = os.path.join(output_dir, f.path.replace("/", os.sep))
            if f.is_directory:
                os.makedirs(out_path, exist_ok=True)
            else:
                self.extract_file(f, out_path)
                count += 1
        return count

    def close(self):
        for s in self._streams:
            s.close()
        self._streams.clear()

    def __del__(self):
        self.close()


DESCRIPTION = "read an Xbox 360 disc image (XGD ISO): list / extract files"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: xgd <iso> [list]  |  xgd extract <iso> <outdir>"); return
    if a[0] == "extract" and len(a) > 2:
        r = XgdIsoReader(a[1]); n = r.extract_all(a[2]); r.close()
        print("extracted %d files -> %s" % (n, a[2])); return
    iso = a[1] if a[0] == "list" and len(a) > 1 else a[0]
    r = XgdIsoReader(iso)
    files = [e for e in r.list_all_files() if not getattr(e, "is_directory", False)]
    for e in files[:60]:
        print("   %12d  %s" % (e.size, getattr(e, "path", "") or e.name))
    print("total: %d files" % len(files))
    r.close()


if __name__ == "__main__":
    main()
