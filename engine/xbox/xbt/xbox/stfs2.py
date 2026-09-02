"""STFS (Secure Transacted File System) package reader/writer.

Ported from STFS-Forge and xbox360-lib StfsDevice.
Handles CON, LIVE, and PIRS Xbox 360 content packages.

Supports:
  - Reading package metadata (title, description, title ID, etc.)
  - Listing files inside the package
  - Extracting files from the package
  - Replacing files and rehashing the block chain
"""

import struct
import hashlib
import os
from pathlib import Path
from typing import Optional
from dataclasses import dataclass, field


# ── Constants ───────────────────────────────────────────

MAGIC_CON  = 0x434F4E20  # "CON "
MAGIC_LIVE = 0x4C495645  # "LIVE"
MAGIC_PIRS = 0x50495253  # "PIRS"

SECTOR_SIZE = 0x1000
ENTRIES_PER_HASH_TABLE = 0xAA
INT24_MAX = 0xFFFFFF
ENTRY_SIZE = 0x40


def _be32(data: bytes, off: int = 0) -> int:
    return struct.unpack_from(">I", data, off)[0]

def _be16(data: bytes, off: int = 0) -> int:
    return struct.unpack_from(">H", data, off)[0]

def _le16(data: bytes, off: int = 0) -> int:
    return struct.unpack_from("<H", data, off)[0]

def _le24(data: bytes, off: int = 0) -> int:
    return data[off] | (data[off+1] << 8) | (data[off+2] << 16)

def _be24(data: bytes, off: int = 0) -> int:
    return (data[off] << 16) | (data[off+1] << 8) | data[off+2]

def _put_be24(buf: bytearray, off: int, val: int):
    buf[off] = (val >> 16) & 0xFF
    buf[off+1] = (val >> 8) & 0xFF
    buf[off+2] = val & 0xFF


# ── Data structures ─────────────────────────────────────

@dataclass
class StfsMetadata:
    magic: int = 0
    magic_str: str = ""
    display_name: str = ""
    description: str = ""
    title_name: str = ""
    title_id: int = 0
    content_type: int = 0
    header_size: int = 0

@dataclass
class VolumeDescriptor:
    size: int = 0
    block_separation: int = 0
    file_table_block_count: int = 0
    file_table_block_num: int = 0
    top_hash: bytes = b""
    allocated_count: int = 0
    unallocated_count: int = 0

@dataclass
class FileEntry:
    name: str = ""
    name_length: int = 0
    flags: int = 0
    valid_data_blocks: int = 0
    allocation_blocks: int = 0
    starting_block: int = 0
    path_indicator: int = -1
    file_size: int = 0
    update_time: int = 0
    access_time: int = 0
    is_directory: bool = False
    is_valid: bool = False

@dataclass
class ListedEntry:
    index: int = 0
    file_address: int = 0
    entry: FileEntry = field(default_factory=FileEntry)
    path: str = ""

@dataclass
class HashEntry:
    block_hash: bytes = b""
    status: int = 0
    next_block: int = 0


# Content type names
CONTENT_TYPES = {
    0x000001: "SavedGame", 0x000002: "Marketplace", 0x000003: "Publisher",
    0x001000: "Xbox360Title", 0x002000: "IPTV", 0x004000: "ArcadeTitle",
    0x005000: "XNA", 0x009000: "License", 0x010000: "MovieTrailer",
    0x020000: "GameDemo", 0x030000: "GameTitle", 0x040000: "Installer",
    0x050000: "InstallerGame", 0x060000: "AvatarItem", 0x070000: "Profile",
    0x080000: "GamerPicture", 0x090000: "ThemePC", 0x0A0000: "CacheFile",
    0x0B0000: "StorageDownload", 0x0C0000: "XboxSavedGame",
    0x0D0000: "XboxDownload", 0x0E0000: "GameTrailer",
    0x00B000: "InstalledGame", 0xD0000: "XboxOriginalGame",
}


# ── STFS Package ────────────────────────────────────────

class StfsPackage:
    """STFS package reader/writer.

    Usage:
        pkg = StfsPackage("package.bin")
        print(pkg.metadata)
        for f in pkg.files:
            print(f.path, f.entry.file_size)
        pkg.extract_all("output_dir/")
        pkg.replace_file("path/in/pkg", "local_file.bin")
    """

    def __init__(self, path: str):
        self.path = path
        self._data = bytearray(Path(path).read_bytes())
        self.metadata = StfsMetadata()
        self.volume = VolumeDescriptor()
        self.files: list[ListedEntry] = []

        self._first_hash_addr = 0
        self._package_sex = 0
        self._block_step = [0, 0]
        self._top_level = 0
        self._tables_per_level = [0, 0, 0]

        self._parse_header()
        self._parse_volume()
        self._init_state()
        self._read_file_listing()

    # ── Header parsing ──────────────────────────────────

    def _parse_header(self):
        magic = _be32(self._data, 0)
        if magic not in (MAGIC_CON, MAGIC_LIVE, MAGIC_PIRS):
            raise ValueError(f"Not an STFS package (magic: 0x{magic:08X})")

        self.metadata.magic = magic
        names = {MAGIC_CON: "CON", MAGIC_LIVE: "LIVE", MAGIC_PIRS: "PIRS"}
        self.metadata.magic_str = names.get(magic, "???")

        # Display name (UTF-16BE at 0x411, 128 chars max)
        try:
            self.metadata.display_name = self._data[0x411:0x411+256].decode("utf-16-be", errors="replace").split("\x00")[0]
        except Exception:
            pass

        # Description (UTF-16BE at 0x611)
        try:
            self.metadata.description = self._data[0x611:0x611+256].decode("utf-16-be", errors="replace").split("\x00")[0]
        except Exception:
            pass

        # Title name (UTF-16BE at 0x1691)
        try:
            self.metadata.title_name = self._data[0x1691:0x1691+128].decode("utf-16-be", errors="replace").split("\x00")[0]
        except Exception:
            pass

        self.metadata.title_id = _be32(self._data, 0x360)
        self.metadata.content_type = _be32(self._data, 0x344)
        self.metadata.header_size = _be32(self._data, 0x340)

    def _parse_volume(self):
        off = 0x379
        self.volume.size = self._data[off]
        self.volume.block_separation = self._data[off + 2]
        self.volume.file_table_block_count = _le16(self._data, off + 3)
        self.volume.file_table_block_num = _le24(self._data, off + 5)
        self.volume.top_hash = bytes(self._data[off + 8:off + 8 + 0x14])
        self.volume.allocated_count = _be32(self._data, off + 0x1C)
        self.volume.unallocated_count = _be32(self._data, off + 0x20)

    def _init_state(self):
        self._first_hash_addr = _align_up(self.metadata.header_size, SECTOR_SIZE)
        self._package_sex = 0 if (self.volume.block_separation & 1) else 1

        if self._package_sex == 0:
            self._block_step = [0xAB, 0x718F]
        else:
            self._block_step = [0xAC, 0x723A]

        ac = self.volume.allocated_count
        self._tables_per_level[0] = _div_round_up(ac, ENTRIES_PER_HASH_TABLE)
        self._tables_per_level[1] = _div_round_up(self._tables_per_level[0], ENTRIES_PER_HASH_TABLE)
        self._tables_per_level[2] = _div_round_up(self._tables_per_level[1], ENTRIES_PER_HASH_TABLE)

        if ac <= ENTRIES_PER_HASH_TABLE:
            self._top_level = 0
            self._tables_per_level[1] = 0
            self._tables_per_level[2] = 0
        elif ac <= 0x70E4:
            self._top_level = 1
            self._tables_per_level[2] = 0
        else:
            self._top_level = 2

    # ── Block addressing ────────────────────────────────

    def _block_to_address(self, block: int) -> int:
        """Convert a block number to a file offset."""
        if block >= self.volume.allocated_count:
            raise ValueError(f"Block {block} out of range (max {self.volume.allocated_count})")

        # Number of hash tables before this block
        if block < ENTRIES_PER_HASH_TABLE:
            hash_tables_before = 0
        else:
            hash_tables_before = ((block // ENTRIES_PER_HASH_TABLE) * self._block_step[0])
            if block >= 0x70E4:
                hash_tables_before += ((block // 0x70E4) + 1) * self._block_step[0]

        return self._first_hash_addr + ((block + hash_tables_before + self._block_step[0]) * SECTOR_SIZE)

    def _get_hash_address_of_block(self, block: int) -> int:
        """Get the file address of the hash entry for a given block."""
        level0_table = block // ENTRIES_PER_HASH_TABLE
        record_in_table = block % ENTRIES_PER_HASH_TABLE

        # Compute the backing block number for the level-0 hash table
        base = level0_table * self._block_step[0]
        if level0_table > 0 and self._top_level >= 1:
            base += (((level0_table - 1) // ENTRIES_PER_HASH_TABLE) + 1) * self._block_step[0]

        table_addr = self._first_hash_addr + (base * SECTOR_SIZE)
        if self._package_sex == 1:
            table_addr += ((self.volume.block_separation >> 1) & 1) * SECTOR_SIZE

        return table_addr + (record_in_table * 0x18)

    def _get_hash_entry(self, block: int) -> HashEntry:
        addr = self._get_hash_address_of_block(block)
        h = HashEntry()
        h.block_hash = bytes(self._data[addr:addr + 0x14])
        h.status = self._data[addr + 0x14]
        h.next_block = _be24(self._data, addr + 0x15)
        return h

    # ── File listing ────────────────────────────────────

    def _read_file_entry(self, offset: int) -> FileEntry:
        e = FileEntry()
        name_len_flags = self._data[offset]
        e.name_length = name_len_flags & 0x3F
        e.flags = name_len_flags

        if e.name_length == 0:
            e.is_valid = False
            return e

        raw_name = self._data[offset + 1:offset + 1 + e.name_length]
        e.name = raw_name.decode("ascii", errors="replace").rstrip("\x00\xff")
        e.is_valid = len(e.name) > 0

        e.valid_data_blocks = _le24(self._data, offset + 0x27)
        e.allocation_blocks = _le24(self._data, offset + 0x2A)
        e.starting_block = _le24(self._data, offset + 0x2F)
        e.path_indicator = _le16(self._data, offset + 0x32)
        if e.path_indicator == 0xFFFF:
            e.path_indicator = -1
        e.file_size = _be32(self._data, offset + 0x34)
        e.is_directory = bool(e.flags & 0x80)

        return e

    def _read_file_listing(self):
        self.files = []
        block = self.volume.file_table_block_num

        for _ in range(self.volume.file_table_block_count):
            if block == INT24_MAX:
                break
            block_addr = self._block_to_address(block)

            for i in range(0x40):  # 64 entries per block
                entry_addr = block_addr + (i * ENTRY_SIZE)
                if entry_addr + ENTRY_SIZE > len(self._data):
                    break
                entry = self._read_file_entry(entry_addr)
                if not entry.is_valid:
                    break
                self.files.append(ListedEntry(
                    index=len(self.files),
                    file_address=entry_addr,
                    entry=entry))

            h = self._get_hash_entry(block)
            block = h.next_block

        # Resolve paths
        path_cache = {}
        for le in self.files:
            le.path = self._resolve_path(le.index, path_cache)

    def _resolve_path(self, idx: int, cache: dict) -> str:
        if idx in cache:
            return cache[idx]
        entry = self.files[idx].entry
        name = entry.name
        if entry.path_indicator == -1 or entry.path_indicator >= len(self.files):
            cache[idx] = name
            return name
        parent = self._resolve_path(entry.path_indicator, cache)
        path = parent + "\\" + name
        cache[idx] = path
        return path

    # ── Extract ─────────────────────────────────────────

    def extract_file(self, listed_entry: ListedEntry, output_path: str):
        """Extract a single file from the package."""
        entry = listed_entry.entry
        if entry.is_directory:
            os.makedirs(output_path, exist_ok=True)
            return

        os.makedirs(os.path.dirname(output_path) or ".", exist_ok=True)
        remaining = entry.file_size
        block = entry.starting_block
        chunks = []

        while remaining > 0 and block != INT24_MAX:
            addr = self._block_to_address(block)
            chunk_size = min(remaining, SECTOR_SIZE)
            chunks.append(bytes(self._data[addr:addr + chunk_size]))
            remaining -= chunk_size
            if remaining > 0:
                h = self._get_hash_entry(block)
                block = h.next_block

        with open(output_path, "wb") as f:
            for chunk in chunks:
                f.write(chunk)

    def extract_all(self, output_dir: str) -> int:
        """Extract all files. Returns count of files extracted."""
        count = 0
        for le in self.files:
            if le.entry.is_directory:
                continue
            rel_path = le.path.replace("\\", os.sep)
            out_path = os.path.join(output_dir, rel_path)
            self.extract_file(le, out_path)
            count += 1
        return count

    # ── Replace + Rehash ────────────────────────────────

    def replace_file(self, pkg_path: str, local_path: str):
        """Replace a file inside the package and rehash."""
        target = None
        for le in self.files:
            if le.path.lower() == pkg_path.lower().replace("/", "\\"):
                target = le
                break
        if target is None:
            raise ValueError(f"File not found in package: {pkg_path}")

        new_data = Path(local_path).read_bytes()
        if len(new_data) > 0xFFFFFFFF:
            raise ValueError("File too large (>4GB)")

        # Write new data to existing block chain
        block = target.entry.starting_block
        remaining = len(new_data)
        offset = 0

        while remaining > 0 and block != INT24_MAX:
            addr = self._block_to_address(block)
            chunk = min(remaining, SECTOR_SIZE)
            self._data[addr:addr + chunk] = new_data[offset:offset + chunk]
            # Zero-pad if partial block
            if chunk < SECTOR_SIZE:
                self._data[addr + chunk:addr + SECTOR_SIZE] = b"\x00" * (SECTOR_SIZE - chunk)
            offset += chunk
            remaining -= chunk
            if remaining > 0:
                h = self._get_hash_entry(block)
                block = h.next_block

        if remaining > 0:
            raise ValueError(f"New file is larger than allocated blocks ({remaining} bytes remaining)")

        # Update file size in directory entry
        struct.pack_into(">I", self._data, target.file_address + 0x34, len(new_data))

        # Rehash level 0
        self._rehash()

        # Save
        Path(self.path).write_bytes(self._data)

    def _rehash(self):
        """Recompute SHA-1 hashes for all data blocks in level 0."""
        for block in range(self.volume.allocated_count):
            try:
                addr = self._block_to_address(block)
                block_data = self._data[addr:addr + SECTOR_SIZE]
                sha = hashlib.sha1(block_data).digest()
                hash_addr = self._get_hash_address_of_block(block)
                self._data[hash_addr:hash_addr + 0x14] = sha
            except (ValueError, IndexError):
                break

    def save(self, output_path: str = None):
        """Save the package to disk."""
        out = output_path or self.path
        Path(out).write_bytes(self._data)


# ── Helpers ─────────────────────────────────────────────

def _align_up(val: int, alignment: int) -> int:
    return (val + alignment - 1) & ~(alignment - 1)

def _div_round_up(a: int, b: int) -> int:
    return (a + b - 1) // b
