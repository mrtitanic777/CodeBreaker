"""FATX filesystem reader, deleted-file recovery, and signature carver.

Reads Xbox 360 (big-endian) and original Xbox (little-endian) FATX partitions,
disk images, and raw partition dumps. The on-disk layout, dirent format, FAT
handling, timestamp packing, orphan-recovery and file-carving approach follow
**FATX-Tools** by aerosoul94 (MIT) — read for format facts and reimplemented
here as original Python driven by this project's own XEX/STFS parsers.

Authoritative facts (per FATX-Tools):
  - volume header  endian+'LLLL' : magic, serial, sectors_per_cluster, root_cluster
  - dirent         endian+'BB42sLLLLL' (0x40 bytes): name_len, attr, name[42],
                   first_cluster, size, created, modified, accessed
  - timestamp      year=((t>>25)&0x7F)+epoch, month, day, hour, minute, second*2;
                   epoch 1980 (Xbox 360) / 2000 (original Xbox)
  - FAT            16-bit if max_clusters<0xFFF0 else 32-bit; FAT aligned to 0x1000
  - offsets        fat @ vol+0x1000 ; data @ vol+0x1000+align(max_clusters*sz,0x1000)

Commands: info / list / extract / recover / carve.
"""

import os
import struct
from dataclasses import dataclass, field
from datetime import datetime
from typing import Optional, BinaryIO

# ── Constants ───────────────────────────────────────────
FATX_MAGIC = b"FATX"
FATX_SECTOR_SIZE = 0x200
FATX_PAGE_SIZE = 0x1000
FATX_DIRENT_SIZE = 0x40
FATX_MAX_FILENAME = 42

DIRENT_NEVER_USED = 0x00
DIRENT_NEVER_USED2 = 0xFF
DIRENT_DELETED = 0xE5

FILE_ATTRIBUTE_DIRECTORY = 0x10
VALID_FILE_ATTRIBUTES = 0x37            # RO|HIDDEN|SYSTEM|DIRECTORY|ARCHIVE

# Characters a real FATX file name may contain (plus 0xFF padding).
_VALID_NAME = frozenset(
    b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
    b" !#$%&'()-.@[]^_`{}~" + bytes([0xFF]))

# Standard Xbox 360 retail HDD partition map (offset, size; size 0 = to end).
PARTITIONS = [
    {"name": "SystemCache",    "offset": 0x80000,     "size": 0x80000000},
    {"name": "GameCache",      "offset": 0x80080000,  "size": 0xA0E30000},
    {"name": "SystemAux",      "offset": 0x8000400,   "size": 0x6E000000},
    {"name": "SystemExtended", "offset": 0x12000400,  "size": 0xDFFFC00},
    {"name": "Content",        "offset": 0x10C080000, "size": 0},
]
USB_PARTITIONS = [{"name": "Data", "offset": 0x8000, "size": 0}]


# ── Timestamp ───────────────────────────────────────────
def decode_timestamp(t: int, epoch: int) -> Optional[datetime]:
    """Decode a packed FATX timestamp; None if it is not a valid date."""
    if t == 0:
        return None
    year = ((t & 0xFE000000) >> 25) + epoch
    month = (t & 0x01E00000) >> 21
    day = (t & 0x001F0000) >> 16
    hour = (t & 0x0000F800) >> 11
    minute = (t & 0x000007E0) >> 5
    second = (t & 0x0000001F) * 2
    try:
        return datetime(year, month, day, hour, minute, min(second, 59))
    except ValueError:
        return None


# ── Data classes ────────────────────────────────────────
@dataclass
class FatxPartition:
    name: str = ""
    offset: int = 0
    size: int = 0
    endian: str = ">"                    # '>' Xbox 360, '<' original Xbox
    epoch: int = 1980
    sectors_per_cluster: int = 32
    bytes_per_cluster: int = 0x4000
    root_cluster: int = 1
    max_clusters: int = 0
    fat16x: bool = True
    fat_offset: int = 0
    data_offset: int = 0
    valid: bool = False


@dataclass
class FatxEntry:
    name: str = ""
    attributes: int = 0
    first_cluster: int = 0
    file_size: int = 0
    created: Optional[datetime] = None
    modified: Optional[datetime] = None
    accessed: Optional[datetime] = None
    is_directory: bool = False
    is_deleted: bool = False
    path: str = ""
    cluster: int = 0                     # physical cluster the dirent was found in
    children: list = field(default_factory=list)


# ── Volume reader ───────────────────────────────────────
class FatxReader:
    def __init__(self, path: str):
        self.path = path
        self._stream: Optional[BinaryIO] = open(path, "rb")
        self._stream.seek(0, 2)
        self._size = self._stream.tell()
        self._fat_cache: dict = {}       # partition offset -> FAT list

    def close(self):
        if self._stream:
            self._stream.close()
            self._stream = None

    def __del__(self):
        self.close()

    # -- partition discovery --------------------------------------------
    def detect_partitions(self) -> list:
        """Every FATX partition we can find: the standard maps, a raw partition
        at offset 0, or a whole-file single partition."""
        found = []
        seen = set()
        candidates = [{"name": "Partition", "offset": 0, "size": 0}]
        candidates += PARTITIONS + USB_PARTITIONS
        for pdef in candidates:
            if pdef["offset"] in seen:
                continue
            p = self._mount(pdef)
            if p and p.valid:
                found.append(p)
                seen.add(pdef["offset"])
        return found

    def _read(self, off: int, n: int) -> bytes:
        self._stream.seek(off)
        return self._stream.read(n)

    def _mount(self, pdef: dict) -> Optional[FatxPartition]:
        off = pdef["offset"]
        if off + 0x10 > self._size or self._read(off, 4) != FATX_MAGIC:
            return None
        p = FatxPartition(name=pdef["name"], offset=off)
        p.size = pdef["size"] if pdef["size"] > 0 else (self._size - off)

        # Auto-detect endianness: a real sectors_per_cluster is a small power of
        # two, so whichever byte order yields one in {1..256} is correct. Xbox
        # 360 is big-endian (epoch 1980); original Xbox little-endian (2000).
        hdr = self._read(off, 0x10)
        for endian, epoch in ((">", 1980), ("<", 2000)):
            _, _serial, spc, root = struct.unpack(endian + "LLLL", hdr)
            if spc in (1, 2, 4, 8, 16, 32, 64, 128, 256):
                p.endian, p.epoch, p.sectors_per_cluster = endian, epoch, spc
                p.root_cluster = root if root else 1
                break
        else:
            return None
        p.bytes_per_cluster = p.sectors_per_cluster * FATX_SECTOR_SIZE
        p.max_clusters = (p.size // p.bytes_per_cluster) + 1
        p.fat16x = p.max_clusters < 0xFFF0
        bytes_per_fat = p.max_clusters * (2 if p.fat16x else 4)
        bytes_per_fat = (bytes_per_fat + FATX_PAGE_SIZE - 1) & ~(FATX_PAGE_SIZE - 1)
        p.fat_offset = off + FATX_PAGE_SIZE
        p.data_offset = p.fat_offset + bytes_per_fat
        p.valid = True
        return p

    # -- FAT + clusters -------------------------------------------------
    def _fat(self, p: FatxPartition) -> list:
        cached = self._fat_cache.get(p.offset)
        if cached is not None:
            return cached
        n = p.max_clusters
        width = 2 if p.fat16x else 4
        raw = self._read(p.fat_offset, n * width)
        fmt = p.endian + (("H" if p.fat16x else "L") * (len(raw) // width))
        fat = list(struct.unpack(fmt, raw[:len(raw) // width * width]))
        self._fat_cache[p.offset] = fat
        return fat

    def cluster_offset(self, p: FatxPartition, cluster: int) -> int:
        return p.data_offset + p.bytes_per_cluster * (cluster - 1)

    def read_cluster(self, p: FatxPartition, cluster: int) -> bytes:
        return self._read(self.cluster_offset(p, cluster), p.bytes_per_cluster)

    def cluster_chain(self, p: FatxPartition, first: int) -> list:
        """Follow the FAT from `first` until the end/reserved marker."""
        fat = self._fat(p)
        reserved = 0xFFF0 if p.fat16x else 0xFFFFFFF0
        chain = [first]
        cur = first
        while True:
            if cur >= len(fat):
                break
            nxt = fat[cur]
            if nxt >= reserved or nxt == 0 or nxt >= len(fat):
                break
            chain.append(nxt)
            cur = nxt
            if len(chain) > p.max_clusters:
                break
        return chain

    # -- dirent parsing -------------------------------------------------
    def _parse_dirent(self, p: FatxPartition, data: bytes, base: str,
                      cluster: int) -> Optional[FatxEntry]:
        name_len = data[0]
        if name_len in (DIRENT_NEVER_USED, DIRENT_NEVER_USED2):
            return None                              # end of directory
        nl, attr, name_bytes, first, size, ct, mt, at = struct.unpack(
            p.endian + "BB42sLLLLL", data)
        e = FatxEntry(cluster=cluster)
        e.is_deleted = (name_len == DIRENT_DELETED)
        n = FATX_MAX_FILENAME if e.is_deleted else min(name_len & 0x3F, FATX_MAX_FILENAME)
        e.name = name_bytes[:n].split(b"\xff")[0].split(b"\x00")[0].decode(
            "ascii", errors="replace")
        e.attributes = attr
        e.first_cluster = first
        e.file_size = size
        e.created = decode_timestamp(ct, p.epoch)
        e.modified = decode_timestamp(mt, p.epoch)
        e.accessed = decode_timestamp(at, p.epoch)
        e.is_directory = bool(attr & FILE_ATTRIBUTE_DIRECTORY)
        e.path = f"{base}/{e.name}" if base else e.name
        return e

    def list_directory(self, p: FatxPartition, cluster: int = 0,
                       base: str = "", include_deleted: bool = False) -> list:
        if cluster == 0:
            cluster = p.root_cluster
        entries = []
        for cl in self.cluster_chain(p, cluster):
            data = self.read_cluster(p, cl)
            stop = False
            for i in range(0, len(data), FATX_DIRENT_SIZE):
                slot = data[i:i + FATX_DIRENT_SIZE]
                if len(slot) < FATX_DIRENT_SIZE:
                    break
                e = self._parse_dirent(p, slot, base, cl)
                if e is None:
                    stop = True
                    break
                if e.is_deleted and not include_deleted:
                    continue
                entries.append(e)
            if stop:
                break
        for e in entries:
            if e.is_directory and 0 < e.first_cluster <= p.max_clusters and not e.is_deleted:
                e.children = self.list_directory(p, e.first_cluster, e.path,
                                                 include_deleted)
        return entries

    # -- extraction -----------------------------------------------------
    def extract_file(self, p: FatxPartition, e: FatxEntry, out: str,
                     contiguous: bool = False):
        os.makedirs(os.path.dirname(out) or ".", exist_ok=True)
        remaining = e.file_size
        if contiguous or e.is_deleted:
            # deleted files have a cleared FAT chain: read clusters in order.
            need = (remaining + p.bytes_per_cluster - 1) // p.bytes_per_cluster
            chain = list(range(e.first_cluster, e.first_cluster + need))
        else:
            chain = self.cluster_chain(p, e.first_cluster)
        with open(out, "wb") as f:
            for cl in chain:
                if remaining <= 0:
                    break
                chunk = self.read_cluster(p, cl)[:min(remaining, p.bytes_per_cluster)]
                f.write(chunk)
                remaining -= len(chunk)

    def _flatten(self, entries: list) -> list:
        out = []
        for e in entries:
            out.append(e)
            if e.children:
                out.extend(self._flatten(e.children))
        return out

    def extract_all(self, p: FatxPartition, out_dir: str) -> int:
        n = 0
        for e in self._flatten(self.list_directory(p)):
            dst = os.path.join(out_dir, e.path.replace("/", os.sep))
            if e.is_directory:
                os.makedirs(dst, exist_ok=True)
            else:
                self.extract_file(p, e, dst)
                n += 1
        return n

    # -- deleted-file recovery (metadata / orphan analysis) -------------
    def _dirent_valid(self, p: FatxPartition, e: FatxEntry) -> bool:
        if not (0 < e.first_cluster <= p.max_clusters):
            return False
        if not e.name or any(c not in _VALID_NAME for c in e.name.encode("latin-1", "replace")):
            return False
        if (e.attributes & ~VALID_FILE_ATTRIBUTES) != 0:
            return False
        return True

    def recover(self, p: FatxPartition) -> list:
        """Scan every cluster for orphaned/deleted dirents and rebuild what can
        be recovered. Returns a flat list of recoverable FatxEntry."""
        orphans = []
        for cl in range(1, p.max_clusters):
            off = self.cluster_offset(p, cl)
            if off + p.bytes_per_cluster > self._size:
                break
            data = self._read(off, p.bytes_per_cluster)
            for i in range(0, len(data), FATX_DIRENT_SIZE):
                slot = data[i:i + FATX_DIRENT_SIZE]
                if len(slot) < FATX_DIRENT_SIZE:
                    break
                nl = slot[0]
                if nl in (DIRENT_NEVER_USED, 0x01, DIRENT_NEVER_USED2):
                    continue
                if nl != DIRENT_DELETED and (nl & 0x3F) > FATX_MAX_FILENAME:
                    continue
                if (slot[1] & ~VALID_FILE_ATTRIBUTES) != 0:
                    continue
                try:
                    e = self._parse_dirent(p, slot, "", cl)
                except struct.error:
                    continue
                if e and self._dirent_valid(p, e):
                    orphans.append(e)
        # rebuild parent/child by cluster-chain membership
        by_cluster = {}
        for o in orphans:
            by_cluster.setdefault(o.cluster, []).append(o)
        for o in orphans:
            if o.is_directory and 0 < o.first_cluster <= p.max_clusters:
                for member in self.cluster_chain(p, o.first_cluster):
                    for child in by_cluster.get(member, []):
                        if child is not o:
                            child.path = f"{o.name}/{child.name}"
                            o.children.append(child)
        return orphans

    def recover_all(self, p: FatxPartition, out_dir: str,
                    deleted_only: bool = True) -> int:
        n = 0
        for e in self.recover(p):
            if deleted_only and not e.is_deleted:
                continue
            if e.is_directory or e.file_size == 0:
                continue
            dst = os.path.join(out_dir, e.path.replace("/", os.sep))
            try:
                self.extract_file(p, e, dst, contiguous=True)
                n += 1
            except OSError:
                pass
        return n

    # -- signature carver -----------------------------------------------
    def carve(self, p: FatxPartition, out_dir: str,
              interval: int = FATX_PAGE_SIZE, decrypt: bool = False) -> list:
        """Step the partition at `interval` and carve files by magic even where
        no dirent survives (XEX / XBE / STFS / PE). With `decrypt`, encrypted
        normal XEX are measured exactly by decrypting the block chain (slower)."""
        results = []
        base = p.data_offset
        end = min(p.offset + p.size, self._size)
        pos = base
        n = 0
        while pos + 4 <= end:
            length = _carve_at(self._stream, pos, end - pos, decrypt)
            if length:
                kind, ln = length
                self._stream.seek(pos)
                blob = self._stream.read(ln)
                dst = os.path.join(out_dir, "carved", "%08X_%s.%s" % (
                    pos - base, kind, _EXT.get(kind, "bin")))
                os.makedirs(os.path.dirname(dst), exist_ok=True)
                with open(dst, "wb") as f:
                    f.write(blob)
                results.append((pos - base, kind, ln, dst))
                n += 1
                pos += ((ln + interval - 1) // interval) * interval
            else:
                pos += interval
        return results

    # -- write side: inject / mkdir / delete ----------------------------
    def _writable(self):
        if "r+b" not in getattr(self._stream, "mode", ""):
            self._stream.close()
            self._stream = open(self.path, "r+b")

    def _write(self, off, data):
        self._stream.seek(off)
        self._stream.write(data)

    def _set_fat(self, p, cluster, value):
        width = 2 if p.fat16x else 4
        fmt = p.endian + ("H" if p.fat16x else "L")
        self._write(p.fat_offset + cluster * width, struct.pack(fmt, value & (0xFFFF if p.fat16x else 0xFFFFFFFF)))
        fat = self._fat(p)
        if cluster < len(fat):
            fat[cluster] = value & (0xFFFF if p.fat16x else 0xFFFFFFFF)

    def _end(self, p):
        return 0xFFFF if p.fat16x else 0xFFFFFFFF

    def _alloc(self, p, n):
        fat = self._fat(p)
        free = [c for c in range(1, p.max_clusters) if c < len(fat) and fat[c] == 0][:n]
        if len(free) < n:
            raise OSError("not enough free clusters (%d needed, %d free)" % (n, len(free)))
        return free

    def _encode_ts(self, p, dt=None):
        import datetime
        dt = dt or datetime.datetime(2020, 1, 1, 0, 0, 0)
        y = max(0, dt.year - p.epoch)
        return ((y << 25) | (dt.month << 21) | (dt.day << 16) |
                (dt.hour << 11) | (dt.minute << 5) | (dt.second // 2)) & 0xFFFFFFFF

    def _resolve_dir(self, p, path):
        """Return the first cluster of the directory named by `path` ('' = root)."""
        cluster = p.root_cluster
        for part in path.replace("\\", "/").split("/"):
            if not part:
                continue
            for e in self.list_directory(p, cluster):
                if e.is_directory and e.name.lower() == part.lower():
                    cluster = e.first_cluster
                    break
            else:
                raise OSError("directory not found: %s" % part)
        return cluster

    def _write_dirent(self, p, dir_cluster, name, attr, first, size):
        ts = self._encode_ts(p)
        nb = name.encode("ascii")[:FATX_MAX_FILENAME]
        rec = struct.pack(p.endian + "BB42sLLLLL", len(nb), attr,
                          nb + b"\xff" * (FATX_MAX_FILENAME - len(nb)),
                          first, size, ts, ts, ts)
        chain = self.cluster_chain(p, dir_cluster)
        for cl in chain:
            base = self.cluster_offset(p, cl)
            data = self._read(base, p.bytes_per_cluster)
            for i in range(0, len(data), FATX_DIRENT_SIZE):
                nl = data[i]
                if nl in (DIRENT_NEVER_USED, DIRENT_NEVER_USED2, DIRENT_DELETED):
                    self._write(base + i, rec)
                    return
        # directory full — extend it with a fresh cluster
        new = self._alloc(p, 1)[0]
        self._set_fat(p, chain[-1], new)
        self._set_fat(p, new, self._end(p))
        self._write(self.cluster_offset(p, new), b"\xff" * p.bytes_per_cluster)
        self._write(self.cluster_offset(p, new), rec)

    def add_file(self, p, dest_path, data):
        """Write `data` as a new file at dest_path ('dir/sub/name' or 'name')."""
        self._writable()
        dest_path = dest_path.replace("\\", "/")
        parent, _, name = dest_path.rpartition("/")
        dir_cluster = self._resolve_dir(p, parent)
        n = max(1, (len(data) + p.bytes_per_cluster - 1) // p.bytes_per_cluster)
        clusters = self._alloc(p, n)
        for i, cl in enumerate(clusters):                  # write data
            chunk = data[i * p.bytes_per_cluster:(i + 1) * p.bytes_per_cluster]
            self._write(self.cluster_offset(p, cl), chunk.ljust(p.bytes_per_cluster, b"\x00"))
        for i, cl in enumerate(clusters):                  # link the chain
            self._set_fat(p, cl, clusters[i + 1] if i + 1 < len(clusters) else self._end(p))
        self._write_dirent(p, dir_cluster, name, 0x20, clusters[0], len(data))

    def mkdir(self, p, dest_path):
        """Create a directory at dest_path."""
        self._writable()
        dest_path = dest_path.replace("\\", "/")
        parent, _, name = dest_path.rpartition("/")
        dir_cluster = self._resolve_dir(p, parent)
        new = self._alloc(p, 1)[0]
        self._set_fat(p, new, self._end(p))
        self._write(self.cluster_offset(p, new), b"\xff" * p.bytes_per_cluster)   # empty dir
        self._write_dirent(p, dir_cluster, name, FILE_ATTRIBUTE_DIRECTORY, new, 0)

    def remove(self, p, path):
        """Delete a file/empty-dir: mark its dirent deleted and free its chain."""
        self._writable()
        path = path.replace("\\", "/")
        parent, _, name = path.rpartition("/")
        dir_cluster = self._resolve_dir(p, parent)
        for cl in self.cluster_chain(p, dir_cluster):
            base = self.cluster_offset(p, cl)
            data = self._read(base, p.bytes_per_cluster)
            for i in range(0, len(data), FATX_DIRENT_SIZE):
                e = self._parse_dirent(p, data[i:i + FATX_DIRENT_SIZE], "", cl)
                if e is None:
                    break
                if not e.is_deleted and e.name.lower() == name.lower():
                    self._write(base + i, b"\xE5")          # deleted marker
                    c = e.first_cluster                     # free the FAT chain
                    fat = self._fat(p)
                    while 0 < c < len(fat) and fat[c] < (0xFFF0 if p.fat16x else 0xFFFFFFF0):
                        nxt = fat[c]
                        self._set_fat(p, c, 0)
                        c = nxt
                    if 0 < c < len(fat):
                        self._set_fat(p, c, 0)
                    return True
        raise OSError("not found: %s" % path)


# ── carver signatures ───────────────────────────────────
_EXT = {"XEX": "xex", "XBE": "xbe", "STFS": "stfs", "PE": "exe"}


def _u32be(b, o):
    return struct.unpack_from(">I", b, o)[0]


def _carve_at(stream, pos, avail, decrypt=False):
    """Return (kind, length) if a known file starts at `pos`, else None."""
    stream.seek(pos)
    head = stream.read(min(0x400, avail))
    if len(head) < 0x18:
        return None
    if head[:4] == b"XEX2":
        ln = _xex_length(stream, pos, head, avail, decrypt)
        if ln and 0x1000 <= ln <= avail:
            return ("XEX", ln)
        return None
    if head[:4] == b"XBEH":
        stream.seek(pos + 0x10C)
        ln = _rd_u32le(stream)                       # XBE SizeOfImage (LE)
        if 0x1000 <= ln <= avail:
            return ("XBE", ln)
        return None
    if head[:4] in (b"CON ", b"LIVE", b"PIRS"):
        ln = _stfs_length(head)
        if ln and ln <= avail:
            return ("STFS", ln)
        return None
    if head[:2] == b"MZ":
        ln = _pe_length(stream, pos, head, avail)
        if ln and 0x200 <= ln <= avail:
            return ("PE", ln)
        return None
    return None


def _rd_u32be(stream):
    d = stream.read(4)
    return struct.unpack(">I", d)[0] if len(d) == 4 else 0


def _xex_length(stream, pos, head, avail, decrypt=False):
    """Exact on-disk XEX2 file length = data_offset + stored-data length, derived
    from the file-format-info: walk the block chain (normal), sum data runs
    (basic), or add the image size (none). Returns None if the header is short."""
    if len(head) < 0x18:
        return None
    data_offset = _u32be(head, 0x08)
    sec = _u32be(head, 0x10)
    opt_count = _u32be(head, 0x14)
    if data_offset == 0 or data_offset > avail or opt_count > 0x400:
        return None
    hdr = head if len(head) >= data_offset else _reread(stream, pos, data_offset)
    if len(hdr) < min(data_offset, 0x18 + opt_count * 8):
        return None
    def _img_size():
        img = sec + 8 + 256                          # image-info block
        return _u32be(hdr, img + 0x10) if img + 0x14 <= len(hdr) else 0

    def _page(n):
        return (n + FATX_PAGE_SIZE - 1) & ~(FATX_PAGE_SIZE - 1)

    ffi = None
    for i in range(opt_count):
        if _u32be(hdr, 0x18 + i * 8) == 0x000003FF:  # K_FILE_FORMAT
            ffi = _u32be(hdr, 0x18 + i * 8 + 4)
            break
    if ffi is None or ffi + 8 > len(hdr):            # uncompressed, no FFI
        sz = _img_size()
        return _page(data_offset + sz) if sz else None
    enc = struct.unpack_from(">H", hdr, ffi + 4)[0]
    comp = struct.unpack_from(">H", hdr, ffi + 6)[0]
    if comp == 1:                                    # basic: run sizes (in header)
        info_size = _u32be(hdr, ffi)
        total, o = 0, ffi + 8
        while o + 8 <= ffi + info_size and o + 8 <= len(hdr):
            total += _u32be(hdr, o)                  # data size (skip zero size)
            o += 8
        return _page(data_offset + total)
    if comp == 2:                                    # normal
        first_block = _u32be(hdr, ffi + 12)
        if enc:                                      # block headers are encrypted
            if not decrypt:
                return None
            total = _xex_decrypt_datalen(stream, pos, data_offset, first_block, avail)
            return _page(data_offset + total) if total else None
        block = first_block
        total, cur, guard = 0, pos + data_offset, 0
        while block and guard < 0x100000:
            total += block
            stream.seek(cur)
            nxt = stream.read(4)
            if len(nxt) < 4:
                break
            cur += block
            block = struct.unpack(">I", nxt)[0]
            guard += 1
        return _page(data_offset + total)
    sz = _img_size()                                 # comp == 0 (none)
    return _page(data_offset + sz) if sz else None


def _reread(stream, pos, n):
    stream.seek(pos)
    return stream.read(n)


def _xex_decrypt_datalen(stream, pos, data_offset, first_block, avail):
    """Exact stored-data length of an ENCRYPTED normal XEX: let xex.py find the
    title key (it validates by decoding to a real PE), then walk the decrypted
    block chain summing block sizes. Returns 0 if it isn't a decodable XEX."""
    try:
        from xex import XEXFile, _aes_cbc_decrypt
    except Exception:
        return 0
    cap = min(avail, 64 * 1024 * 1024)
    stream.seek(pos)
    blob = stream.read(cap)
    try:
        x = XEXFile(blob)
        x.basefile()                                 # sets x._title_key or raises
    except Exception:
        return 0
    key = getattr(x, "_title_key", None)
    if not key or data_offset >= len(blob):
        return 0
    dec = _aes_cbc_decrypt(key, blob[data_offset:], iv=b"\x00" * 16)
    block, cur, total, guard = first_block, 0, 0, 0
    while block and guard < 0x100000:
        if cur + 4 > len(dec):
            return 0
        nxt = struct.unpack_from(">I", dec, cur)[0]
        total += block
        cur += block
        block = nxt
        guard += 1
    return total


def _rd_u32le(stream):
    d = stream.read(4)
    return struct.unpack("<I", d)[0] if len(d) == 4 else 0


def _stfs_length(head):
    """STFS package size = 0xB000 metadata + allocated blocks * 0x1000. The
    (little-endian) total-block count sits in the volume descriptor at 0x395."""
    try:
        total_blocks = struct.unpack_from("<I", head, 0x395)[0] & 0xFFFFFF
        if 0 < total_blocks < 0x100000:
            return 0xB000 + total_blocks * 0x1000
    except struct.error:
        pass
    return None


def _pe_length(stream, pos, head, avail):
    """PE image length from SizeOfHeaders + section raw sizes (best effort)."""
    try:
        e_lfanew = struct.unpack_from("<I", head, 0x3C)[0]
        if e_lfanew + 0x40 > avail:
            return None
        stream.seek(pos + e_lfanew)
        sig = stream.read(4)
        if sig != b"PE\x00\x00":
            return None
        coff = stream.read(20)
        num_sec = struct.unpack_from("<H", coff, 2)[0]
        opt_size = struct.unpack_from("<H", coff, 16)[0]
        stream.seek(pos + e_lfanew + 24 + opt_size)
        end = 0
        for _ in range(min(num_sec, 96)):
            sh = stream.read(40)
            if len(sh) < 40:
                break
            raw_size = struct.unpack_from("<I", sh, 16)[0]
            raw_ptr = struct.unpack_from("<I", sh, 20)[0]
            end = max(end, raw_ptr + raw_size)
        return end or None
    except struct.error:
        return None


DESCRIPTION = ("read an Xbox 360 FATX filesystem (info/list/extract) + recover "
               "deleted files and carve by signature")


# ── CLI ─────────────────────────────────────────────────
def _print_tree(entries, depth=0):
    for e in entries:
        tag = "/" if e.is_directory else ""
        ts = e.modified.strftime("%Y-%m-%d %H:%M") if e.modified else ""
        print("  %s%-*s %10s  %s" % ("  " * depth, 40 - 2 * depth,
              e.name + tag, "" if e.is_directory else e.file_size, ts))
        if e.children:
            _print_tree(e.children, depth + 1)


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  fatx <image> [list]            list live files\n"
              "  fatx info    <image>           partition + geometry summary\n"
              "  fatx extract <image> <outdir>  extract all live files\n"
              "  fatx recover <image> <outdir>  recover deleted/orphaned files\n"
              "  fatx carve   <image> <outdir> [--decrypt]\n"
              "                                 carve files by signature (XEX/XBE/STFS/PE);\n"
              "                                 --decrypt measures encrypted XEX exactly\n"
              "  fatx add   <image> <src-file> <dest-path>   inject a file\n"
              "  fatx mkdir <image> <dest-path>              create a directory\n"
              "  fatx rm    <image> <path>                   delete a file/empty dir")
        return
    if a and a[0] in ("add", "mkdir", "rm"):
        r = FatxReader(a[1])
        parts = r.detect_partitions()
        if not parts:
            print("no FATX partition found"); return
        p = parts[0]
        if a[0] == "add":
            r.add_file(p, a[3], open(a[2], "rb").read())
            print("injected %s -> %s" % (a[2], a[3]))
        elif a[0] == "mkdir":
            r.mkdir(p, a[2]); print("created directory %s" % a[2])
        else:
            r.remove(p, a[2]); print("deleted %s" % a[2])
        r.close()
        return
    op = a[0] if a[0] in ("info", "list", "extract", "recover", "carve") else "list"
    img = a[1] if op != "list" or (len(a) > 1 and a[0] == "list") else a[0]
    if op == "list" and a[0] != "list":
        img = a[0]
    r = FatxReader(img)
    parts = r.detect_partitions()
    if not parts:
        print("no FATX partition found in %s" % img)
        return

    if op == "info":
        for p in parts:
            print("partition %-14s @0x%X  %s  cluster=%dKB  clusters=%d  FAT%d  root=%d" % (
                p.name, p.offset, "X360-BE" if p.endian == ">" else "Xbox-LE",
                p.bytes_per_cluster // 1024, p.max_clusters, 16 if p.fat16x else 32,
                p.root_cluster))
        return
    if op == "extract":
        n = sum(r.extract_all(p, a[2]) for p in parts)
        print("extracted %d files -> %s" % (n, a[2])); return
    if op == "recover":
        n = sum(r.recover_all(p, a[2]) for p in parts)
        print("recovered %d deleted/orphaned files -> %s" % (n, a[2])); return
    if op == "carve":
        decrypt = "--decrypt" in a
        total = 0
        for p in parts:
            for off, kind, ln, dst in r.carve(p, a[2], decrypt=decrypt):
                print("  %-5s +0x%08X  %8d bytes  %s" % (kind, off, ln, os.path.basename(dst)))
                total += 1
        print("carved %d files -> %s/carved%s" % (
            total, a[2], "  (+decrypt)" if decrypt else "")); return

    for p in parts:
        print("partition %-14s @0x%X  (%s)" % (
            p.name, p.offset, "X360-BE" if p.endian == ">" else "Xbox-LE"))
        try:
            _print_tree(r.list_directory(p))
        except Exception as ex:
            print("   (list error: %s)" % ex)
    r.close()


if __name__ == "__main__":
    main()
