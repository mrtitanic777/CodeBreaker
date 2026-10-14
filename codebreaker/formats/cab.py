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

Decompression is done here for stored (NONE), MSZIP (zlib) and QUANTUM (the decoder below,
written from the published description of the format; it is plain Python and slow, between
0.1 and 0.25 MB of output per second). LZX cabinets are listed but not extracted (CabError).
Block checksums are verified where a cabinet carries them.

    python -m codebreaker cab list <file.cab>
    python -m codebreaker cab extract <file.cab> <outdir> [names...]
    python -m codebreaker cab create <out.cab> <file-or-folder>... [--store]
"""
import collections
import functools
import operator
import os
import struct
import sys
import time
import zlib

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
# a block is cut at 32756 bytes; see _mszip_blocks.
SHORT_BLOCK = 32756

Entry = collections.namedtuple(
    "Entry", "name size folder offset date time attributes compression")


class CabError(ValueError):
    pass


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


def _text(data, at):
    end = data.index(b"\0", at)
    return data[at:end], end + 1


def read(path):
    """Parse the headers of one cabinet (nothing is decompressed)."""
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] != b"MSCF" or len(data) < 36:
        raise CabError("%s: not a cabinet (no MSCF signature)" % path)
    (size, files_at, minor, major, n_folders, n_files, flags, set_id,
     index) = struct.unpack_from("<8xI4xI4xBBHHHHH", data, 0)
    at = 36
    header_reserve = folder_reserve = block_reserve = 0
    if flags & 4:                                # reserved areas present
        header_reserve, folder_reserve, block_reserve = struct.unpack_from("<HBB", data, at)
        at += 4 + header_reserve
    chain = {}
    for key, bit in (("prev", 1), ("next", 2)):  # neighbour cabinet name, then its disk label
        chain[key] = chain[key + "_disk"] = None
        if flags & bit:
            name, at = _text(data, at)
            disk, at = _text(data, at)
            chain[key], chain[key + "_disk"] = name.decode("latin-1"), disk.decode("latin-1")
    folders = []
    for _ in range(n_folders):
        folders.append(struct.unpack_from("<IHH", data, at))
        at += 8 + folder_reserve
    kinds = [COMPRESSION.get(t & 15, "?%d" % (t & 15)) for _o, _n, t in folders]
    files, at = [], files_at
    for _ in range(n_files):
        fsize, offset, folder, date, tm, attributes = struct.unpack_from("<IIHHHH", data, at)
        name, at = _text(data, at + 16)
        name = name.decode("utf-8" if attributes & 0x80 else "latin-1", "replace")
        k = {FROM_PREV: 0, PREV_AND_NEXT: 0, TO_NEXT: n_folders - 1}.get(folder, folder)
        kind = kinds[k] if 0 <= k < n_folders else "?"
        files.append(Entry(name, fsize, folder, offset, date, tm, attributes, kind))
    info = dict(chain, set_id=set_id, index=index, flags=flags, folders=kinds, size=size,
                version="%d.%d" % (major, minor))
    return Cabinet(path, info, files, folders, block_reserve)


# --- decompression -------------------------------------------------------------------------
def checksum(payload, uncompressed):
    """The CFDATA checksum: 32-bit little-endian words of the block header (two sizes) and
    the payload XORed together; the 1-3 odd bytes at the end count most-significant first."""
    data = struct.pack("<HH", len(payload), uncompressed) + payload
    whole = len(data) & ~3
    words = struct.unpack_from("<%dI" % (whole // 4), data)
    return functools.reduce(operator.xor, words, int.from_bytes(data[whole:], "big"))


def _neighbour(cab, key):
    name = cab.info[key]
    folder = os.path.dirname(os.path.abspath(cab.path))
    for candidate in os.listdir(folder) if name else ():
        if candidate.lower() == name.lower():
            return read(os.path.join(folder, candidate))
    raise CabError("%s: the %s cabinet of the set (%s) is not beside it"
                   % (cab.path, key, name))


def _folder_start(cab, k):
    """(cabinet, folder number) where the stream that folder `k` of `cab` belongs to begins."""
    while k == 0 and any(e.folder in (FROM_PREV, PREV_AND_NEXT) for e in cab.files):
        cab = _neighbour(cab, "prev")
        k = len(cab._folders) - 1
    return cab, k


def _blocks(cab, k):
    """Yield (payload, uncompressed size) for every block of a folder's stream, running on
    into the following cabinets. A block cut by a cabinet boundary has uncompressed size 0 in
    its first half; the halves are joined."""
    held = b""
    while True:
        offset, count, _kind = cab._folders[k]
        with open(cab.path, "rb") as f:
            f.seek(offset)
            for _ in range(count):
                stored, csize, usize = struct.unpack("<IHH", f.read(8))
                f.read(cab._block_reserve)
                part = f.read(csize)
                if len(part) != csize:
                    raise CabError("%s: truncated data block" % cab.path)
                if stored and stored != checksum(part, usize):   # 0 = not checksummed
                    raise CabError("%s: data block checksum mismatch" % cab.path)
                if usize == 0 and csize:
                    held += part
                    continue
                yield held + part, usize
                held = b""
        if k != len(cab._folders) - 1 or not any(
                e.folder in (TO_NEXT, PREV_AND_NEXT) for e in cab.files):
            return
        cab, k = _neighbour(cab, "next"), 0


def _unpack_stored(_kind):
    def step(payload, _usize, out):
        out += payload
    return step


def _unpack_mszip(_kind):
    """Each block is "CK" + a raw deflate stream, inflated at the start of a 32768-byte
    window that is NOT cleared between blocks, so a block may refer back into the one before
    it. After full blocks that window is simply the previous block; after a short block its
    tail still holds older bytes, exactly as in Microsoft's decoder."""
    window = [b""]

    def step(payload, usize, out):
        if payload[:2] != b"CK":
            raise CabError("MSZIP block without the CK signature")
        history = window[0]
        inflater = zlib.decompressobj(-15, zdict=history) if history else zlib.decompressobj(-15)
        got = inflater.decompress(payload[2:])
        if len(got) != usize:
            raise CabError("MSZIP block inflated to %d bytes, header says %d" % (len(got), usize))
        out += got
        window[0] = (got + history[len(got):])[-32768:].ljust(32768, b"\0")
    return step


# Quantum: an LZ77 coder whose symbols are arithmetic-coded with adaptive frequency models.
# Position and length "slots" give a base value plus a number of plain extra bits.
_Q_POS_EXTRA = [0, 0] + [i // 2 for i in range(40)]                    # 0,0,0,0,1,1,...,19,19
_Q_POS_BASE = [sum(1 << e for e in _Q_POS_EXTRA[:i]) for i in range(42)]
_Q_LEN_EXTRA = [0] * 6 + [n for n in range(1, 6) for _ in range(4)] + [0]
_Q_LEN_BASE = [sum(1 << e for e in _Q_LEN_EXTRA[:i]) for i in range(27)]


def _unpack_quantum(kind):
    """Decoder for one Quantum folder. State kept across blocks: the models and the output
    (which serves as the window); the arithmetic coder restarts with every 32768-byte frame,
    and in a cabinet a frame is exactly one CFDATA block."""
    window_bits = (kind >> 8) & 0x1F
    if not 10 <= window_bits <= 21:
        raise CabError("Quantum window of 2^%d bytes is not valid" % window_bits)

    def model(first, count):                     # [symbols, cumulative frequencies, countdown]
        return [list(range(first, first + count)), list(range(count, -1, -1)), 4]

    literals = [model(64 * i, 64) for i in range(4)]
    positions = [model(0, min(window_bits * 2, 24)), model(0, min(window_bits * 2, 36)),
                 model(0, window_bits * 2)]      # for matches of length 3, 4, 5 and more
    lengths, selector = model(0, 27), model(0, 7)
    produced = [0]                               # uncompressed bytes the frames so far cover

    def rescale(m):
        """Called when a model's total passes 3800: halve the counts; every 50th time (the
        first time after 4) also re-sort the symbols, most frequent first."""
        symbols, cum = m[0], m[1]
        count = len(symbols)
        m[2] -= 1
        if m[2]:
            for i in range(count - 1, -1, -1):
                cum[i] >>= 1
                if cum[i] <= cum[i + 1]:
                    cum[i] = cum[i + 1] + 1
            return
        m[2] = 50
        freq = [(cum[i] - cum[i + 1] + 1) >> 1 for i in range(count)]
        for i in range(count - 1):               # this exact exchange sort is part of the
            fi = freq[i]                         # format: ties must land where the packer's did
            for j in range(i + 1, count):
                if fi < freq[j]:
                    freq[i], freq[j] = freq[j], fi
                    symbols[i], symbols[j] = symbols[j], symbols[i]
                    fi = freq[i]
        for i in range(count - 1, -1, -1):
            cum[i] = cum[i + 1] + freq[i]

    def step(payload, usize, out):
        data = payload + b"\0" * 8               # reading past the end yields zero bits
        at, buf, have = 2, 0, 0                  # bit reader: `have` unread bits in `buf`
        high, low, code = 0xFFFF, 0, (data[0] << 8) | data[1]
        produced[0] += usize
        goal = produced[0]

        def bits(n):
            nonlocal at, buf, have
            if have < n:
                buf = ((buf & ((1 << have) - 1)) << 32) | int.from_bytes(data[at:at + 4], "big")
                at += 4
                have += 32
                if at > len(data) + 64:
                    raise CabError("Quantum block ran out of data")
            have -= n
            return (buf >> have) & ((1 << n) - 1)

        def symbol(m):
            nonlocal high, low, code
            cum = m[1]
            total = cum[0]
            span = high - low + 1
            target = ((code - low + 1) * total - 1) // span
            i = 1
            while cum[i] > target:
                i += 1
            value = m[0][i - 1]
            high = low + cum[i - 1] * span // total - 1
            low += cum[i] * span // total
            for j in range(i):
                cum[j] += 8
            if cum[0] > 3800:
                rescale(m)
            while True:                          # shift out the bits high and low agree on
                differ = high ^ low
                if differ & 0x8000:
                    if not (low & 0x4000 and not high & 0x4000):
                        break
                    code ^= 0x4000               # underflow: low = 01..., high = 10...
                    low &= 0x3FFF
                    high |= 0x4000
                    n = 1
                else:
                    n = 16 - differ.bit_length()
                low = (low << n) & 0xFFFF
                high = ((high << n) | ((1 << n) - 1)) & 0xFFFF
                code = ((code << n) | bits(n)) & 0xFFFF
            return value

        while len(out) < goal:
            which = symbol(selector)
            if which < 4:
                out.append(symbol(literals[which]))
                continue
            if which == 6:
                slot = symbol(lengths)
                length = _Q_LEN_BASE[slot] + bits(_Q_LEN_EXTRA[slot]) + 5
            else:
                length = which - 1               # selector 4 -> length 3, 5 -> length 4
            slot = symbol(positions[which - 4])
            distance = _Q_POS_BASE[slot] + bits(_Q_POS_EXTRA[slot]) + 1
            start = len(out) - distance
            if start < 0:
                raise CabError("Quantum match reaches before the start of the folder")
            if distance >= length:
                out += out[start:start + length]
            else:                                # overlapping copy repeats the last bytes
                piece = bytes(out[start:])
                out += (piece * (length // distance + 1))[:length]
    return step


def _unpack_lzx(_kind):
    raise CabError("LZX-compressed cabinets are not supported")


_UNPACK = {0: _unpack_stored, 1: _unpack_mszip, 2: _unpack_quantum, 3: _unpack_lzx}


def folder_bytes(cab, k, upto=None):
    """The uncompressed stream of the folder that folder `k` of `cab` is part of (starting
    in an earlier cabinet when it is continued from one), at least `upto` bytes of it."""
    cab, k = _folder_start(cab, k)
    kind = cab._folders[k][2]
    if kind & 15 not in _UNPACK:
        raise CabError("%s: unknown compression type %d" % (cab.path, kind & 15))
    step = _UNPACK[kind & 15](kind)
    out = bytearray()
    for payload, usize in _blocks(cab, k):
        if usize:
            step(payload, usize, out)
        if upto is not None and len(out) >= upto:
            break
    return out


def _safe_path(outdir, name):
    parts = [p for p in name.replace("\\", "/").split("/") if p not in ("", ".")]
    if not parts or ".." in parts or ":" in name:
        raise CabError("refusing to extract the unsafe name %r" % name)
    return os.path.join(outdir, *parts)


def _timestamp(entry):
    d, t = entry.date, entry.time
    try:
        return time.mktime((1980 + (d >> 9), (d >> 5) & 15, d & 31,
                            t >> 11, (t >> 5) & 63, (t & 31) * 2, 0, 0, -1))
    except (ValueError, OverflowError):
        return None


def extract(path, outdir, names=None):
    """Extract the files listed in one cabinet (all, or those in `names`, compared without
    regard to case) into `outdir`. Neighbouring cabinets are read as needed for files whose
    folder is continued. Returns {name: output path}."""
    cab = read(path)
    wanted = None if names is None else {n.lower() for n in names}
    groups = {}
    for entry in cab.files:
        if wanted is None or entry.name.lower() in wanted:
            groups.setdefault(cab.folder_of(entry), []).append(entry)
    written = {}
    for k, entries in sorted(groups.items()):
        stream = folder_bytes(cab, k, max(e.offset + e.size for e in entries))
        for entry in entries:
            data = stream[entry.offset:entry.offset + entry.size]
            if len(data) != entry.size:
                raise CabError("%s: %s is cut short" % (path, entry.name))
            target = _safe_path(outdir, entry.name)
            os.makedirs(os.path.dirname(target), exist_ok=True)
            with open(target, "wb") as f:
                f.write(data)
            stamp = _timestamp(entry)
            if stamp is not None:
                os.utime(target, (stamp, stamp))
            written[entry.name] = target
    return written


# --- writing -------------------------------------------------------------------------------
def _mszip_blocks(stream, short_blocks):
    """Cut a folder's stream into MSZIP blocks: [(payload, uncompressed size)].

    The decoder inflates each block into a window still holding the block before it, so a
    block is deflated with the previous block's bytes as preset dictionary; that is what
    makes the cabinet small. A block that will not compress to a 32768-byte payload is
    written short and stored instead (see SHORT_BLOCK). A short block leaves the decoder's
    window part old, part new, and decoders differ on how that reads, so the block after a
    short one is deflated with no dictionary: it refers only to itself and is read the same
    by all of them. 7-Zip alone still objects to a short block in mid-folder (Microsoft's own
    decoder does not); with `short_blocks` off every block is full and such a block's payload
    may run a few bytes over 32768, as in cabinets made by Microsoft's tools."""
    blocks, at, previous = [], 0, b""
    while at < len(stream):
        chunk = stream[at:at + BLOCK]
        if previous:
            packer = zlib.compressobj(9, zlib.DEFLATED, -15, 8, 0, previous)
        else:
            packer = zlib.compressobj(9, zlib.DEFLATED, -15)
        body = packer.compress(chunk) + packer.flush()
        previous = chunk
        if len(body) + 2 > 32768 and short_blocks:   # one stored deflate block: 5 bytes + the data
            chunk = stream[at:at + SHORT_BLOCK]
            body = struct.pack("<BHH", 1, len(chunk), len(chunk) ^ 0xFFFF) + chunk
            previous = b""
        blocks.append((b"CK" + body, len(chunk)))
        at += len(chunk)
    return blocks


def _chain_text(neighbour):
    if not neighbour:
        return b""
    name, disk = (neighbour, "") if isinstance(neighbour, str) else neighbour
    return name.encode("latin-1") + b"\0" + (disk or "").encode("latin-1") + b"\0"


def write(files, out_path, compress="mszip", set_id=0, index=0, prev=None, next=None,
          short_blocks=True):
    """Write a cabinet holding `files` = [(name, bytes)] in one folder; returns its size.

    `short_blocks` keeps every MSZIP data block within what Windows 95 SETUP can take, at the
    price of 7-Zip rejecting cabinets that hold incompressible data (see _mszip_blocks); pass
    False for a cabinet meant for present-day tools only.

    `set_id`, `index`, `prev` and `next` (a cabinet name, or (cabinet name, disk label)) are
    the fields that tie cabinets into a set; a program that walks the set, as Windows 95
    SETUP does, needs them carried over when a cabinet of a set is rebuilt. The output is a
    valid cabinet delivering the same files, not a byte copy of any original cabinet."""
    if compress not in ("mszip", "none"):
        raise CabError("compress must be 'mszip' or 'none', not %r" % (compress,))
    stream = b"".join(data for _name, data in files)
    if compress == "mszip":
        blocks = _mszip_blocks(stream, short_blocks)
    else:
        blocks = [(stream[at:at + BLOCK], min(BLOCK, len(stream) - at))
                  for at in range(0, len(stream), BLOCK)]
    if len(blocks) > 0xFFFF or len(files) > 0xFFFF:
        raise CabError("too much for one cabinet folder (%d blocks, %d files)"
                       % (len(blocks), len(files)))
    chain = _chain_text(prev) + _chain_text(next)
    flags = (1 if prev else 0) | (2 if next else 0)
    entries, offset = bytearray(), 0
    stamp = time.localtime()
    date = ((stamp.tm_year - 1980) << 9) | (stamp.tm_mon << 5) | stamp.tm_mday
    clock = (stamp.tm_hour << 11) | (stamp.tm_min << 5) | (stamp.tm_sec // 2)
    for name, data in files:
        try:
            raw, attributes = name.encode("ascii"), 0x20
        except UnicodeEncodeError:
            raw, attributes = name.encode("utf-8"), 0xA0
        entries += struct.pack("<IIHHHH", len(data), offset, 0, date, clock, attributes)
        entries += raw + b"\0"
        offset += len(data)
    files_at = 36 + len(chain) + 8
    data_at = files_at + len(entries)
    # Block checksum 0 means "not checksummed": every reader accepts it without checking.
    body = b"".join(struct.pack("<IHH", 0, len(payload), usize) + payload
                    for payload, usize in blocks)
    total = data_at + len(body)
    header = struct.pack("<4sIIIIIBBHHHHH", b"MSCF", 0, total, 0, files_at, 0, 3, 1,
                         1, len(files), flags, set_id, index)
    folder = struct.pack("<IHH", data_at, len(blocks), 1 if compress == "mszip" else 0)
    with open(out_path, "wb") as f:
        f.write(header + chain + folder + bytes(entries) + body)
    return total


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
