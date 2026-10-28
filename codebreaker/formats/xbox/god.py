#!/usr/bin/env python3
"""GoD / SVOD — Xbox 360 Games-on-Demand container read + write.

A GoD (a.k.a. SVOD) package is a CON/LIVE header plus a `<mediaid>.data/`
directory of `Data####` "part" files. Each part interleaves hash blocks among
the data: a 4096-byte master hash block, then 0xCB subparts, each a 4096-byte
subpart hash block (204 SHA-1s) followed by 204 data blocks of 0x1000. The
concatenated, de-interleaved data blocks form an XDVDFS/GDF disc volume.

Clean-room Python read (de-interleave) + write (ISO -> GoD) of that format.
The block/hash layout and CON-header field map were learned from iso2god-rs by
Ilia Pozdnyakov (MIT), reimplemented here as original Python; format constants
and offsets are facts, not copied code.

Commands:
    god info    <god-dir|con-header>     summary of a GoD package
    god list    <god-dir>                on-disc file listing (via GDF)
    iso2god     <image.iso> <out-dir> [--title NAME]   convert an ISO to GoD
"""
DESCRIPTION = "Xbox 360 GoD/SVOD: read (de-interleave) and convert ISO -> GoD"

import hashlib
import os
import struct
import sys

BLOCK = 0x1000
BLOCKS_PER_SUBPART = 0xCC          # 204 data blocks per subpart
SUBPARTS_PER_PART = 0xCB           # 203 subparts per part
BLOCKS_PER_PART = 0xA1C4           # = 0xCB * 0xCC data blocks per part
SUBPART_SIZE = BLOCK * BLOCKS_PER_SUBPART


def _phys_offset_in_part(block_in_part):
    """Physical byte offset of a data block within its part file (past hashes)."""
    subpart = block_in_part // BLOCKS_PER_SUBPART
    in_sub = block_in_part % BLOCKS_PER_SUBPART
    return (BLOCK                                    # master hash block
            + subpart * (BLOCK + SUBPART_SIZE)       # prior subparts
            + BLOCK                                   # this subpart's hash block
            + in_sub * BLOCK)


class SvodReader:
    """De-interleaves a GoD `.data` directory into the logical disc volume,
    exposing read_at(logical_offset, length) so a GDF reader can walk it."""
    def __init__(self, data_dir):
        parts = sorted(n for n in os.listdir(data_dir)
                       if n.lower().startswith("data"))
        if not parts:
            raise ValueError("no Data#### part files in %r" % data_dir)
        self.paths = [os.path.join(data_dir, n) for n in parts]
        self._fh = {}

    def _f(self, idx):
        if idx not in self._fh:
            self._fh[idx] = open(self.paths[idx], "rb")
        return self._fh[idx]

    def read_at(self, offset, length):
        out = bytearray()
        while length > 0:
            block = offset // BLOCK
            in_block = offset % BLOCK
            part = block // BLOCKS_PER_PART
            if part >= len(self.paths):
                break
            block_in_part = block % BLOCKS_PER_PART
            phys = _phys_offset_in_part(block_in_part) + in_block
            take = min(length, BLOCK - in_block)
            f = self._f(part)
            f.seek(phys)
            chunk = f.read(take)
            if not chunk:
                break
            out += chunk
            offset += len(chunk)
            length -= len(chunk)
        return bytes(out)

    def close(self):
        for f in self._fh.values():
            f.close()


# ------------------------------------------------------------------- writer
def _con_header(template, *, title_id, media_id, content_type, blocks_allocated,
                part_count, parts_total_size, mht_hash, title=""):
    """Build the GoD CON/LIVE header from the empty_live template (0xB000)."""
    b = bytearray(template)
    struct.pack_into(">I", b, 0x0344, content_type)
    struct.pack_into(">I", b, 0x0354, media_id)
    struct.pack_into(">I", b, 0x0360, title_id)
    # block counts: u24 BE allocated @0x392, u16 BE not-allocated @0x395
    b[0x0392] = (blocks_allocated >> 16) & 0xFF
    b[0x0393] = (blocks_allocated >> 8) & 0xFF
    b[0x0394] = blocks_allocated & 0xFF
    struct.pack_into(">H", b, 0x0395, 0)
    struct.pack_into("<I", b, 0x03A0, part_count)        # part count is LE (sic)
    struct.pack_into(">I", b, 0x03A4, parts_total_size // 0x100)
    b[0x037D:0x037D + 20] = mht_hash
    if title:
        t = title.encode("utf-16-be")[:0x7E]
        b[0x0411:0x0411 + len(t)] = t
        b[0x1691:0x1691 + len(t)] = t
    b[0x035B] = 0; b[0x035F] = 0; b[0x0391] = 0
    b[0x032C:0x032C + 20] = hashlib.sha1(bytes(b[0x0344:0x0344 + 0xACBC])).digest()
    return bytes(b)


def write_god(volume_read, volume_size, out_dir, template, *, title_id, media_id,
              content_type=0x7000, title=""):
    """Write a GoD package: `<out_dir>/<mediaid>.data/Data####` parts plus the
    CON header file `<out_dir>/<mediaid>`. `volume_read(off, len)` streams the
    de-interleaved (logical) disc volume to pack.
    """
    tid = "%08X" % title_id
    mid = "%08X" % media_id
    base = os.path.join(out_dir, tid, "%08X" % content_type)
    data_dir = os.path.join(base, mid + ".data")
    os.makedirs(data_dir, exist_ok=True)

    total_blocks = (volume_size + BLOCK - 1) // BLOCK
    part_count = (total_blocks + BLOCKS_PER_PART - 1) // BLOCKS_PER_PART
    first_master_hash = None
    parts_total_size = 0

    for pidx in range(part_count):
        part_path = os.path.join(data_dir, "Data%04d" % pidx)
        master = bytearray(BLOCK)
        subpart_blobs = []
        for sidx in range(SUBPARTS_PER_PART):
            base_block = pidx * BLOCKS_PER_PART + sidx * BLOCKS_PER_SUBPART
            if base_block >= total_blocks:
                break
            sub_hash = bytearray(BLOCK)
            data_blocks = bytearray()
            filled = 0
            for k in range(BLOCKS_PER_SUBPART):
                gblk = base_block + k
                if gblk >= total_blocks:
                    break
                chunk = volume_read(gblk * BLOCK, BLOCK)
                chunk = chunk.ljust(BLOCK, b"\x00")
                sub_hash[k * 20:k * 20 + 20] = hashlib.sha1(chunk).digest()
                data_blocks += chunk
                filled += 1
            master[sidx * 20:sidx * 20 + 20] = hashlib.sha1(bytes(sub_hash)).digest()
            subpart_blobs.append((bytes(sub_hash), bytes(data_blocks)))
            if filled < BLOCKS_PER_SUBPART:
                break
        with open(part_path, "wb") as f:
            f.write(bytes(master))
            for sub_hash, data_blocks in subpart_blobs:
                f.write(sub_hash)
                f.write(data_blocks)
        parts_total_size += os.path.getsize(part_path)
        if pidx == 0:
            first_master_hash = hashlib.sha1(bytes(master)).digest()

    header = _con_header(template, title_id=title_id, media_id=media_id,
                         content_type=content_type, blocks_allocated=total_blocks,
                         part_count=part_count, parts_total_size=parts_total_size,
                         mht_hash=first_master_hash, title=title)
    con_path = os.path.join(base, mid)
    with open(con_path, "wb") as f:
        f.write(header)
    return con_path, data_dir, part_count


def _template():
    """The empty LIVE header template (from iso2god-rs, MIT). Ships alongside."""
    here = os.path.dirname(os.path.abspath(__file__))
    p = os.path.join(here, "empty_live.bin")
    if os.path.exists(p):
        return open(p, "rb").read()
    return None


# ---- NXE-installed STFS package -> GoD/LIVE header (NXE2GOD transform) ----
import hashlib as _hashlib


def install_to_god(data):
    """Convert an NXE-installed game's STFS/CON header into a GoD/LIVE header
    (the NXE2GOD 'MakeGOD' transform): retype the package to LIVE / content-type
    0x7000 and recompute the header hash. `data` is the full package-header file
    (>= 0xB000); the sibling .data block folder is copied unchanged by the user.
    Facts reimplemented clean-room from NXE2GOD; on modded consoles this needs no
    RSA signature (the header hash is what is checked)."""
    if len(data) < 0x413:
        raise ValueError("not an STFS package header (need >= 0x413 bytes)")
    b = bytearray(data)
    b[0x0:0x4] = b"LIVE"
    for i in range(0x22C, 0x234):
        b[i] = 0xFF
    b[0x340] = 0x00
    b[0x341] = 0x00
    b[0x342] = 0xAD
    b[0x343] = 0x0E
    b[0x346] = 0x70
    b[0x379] = 0x24
    b[0x37A] = 0x05
    b[0x37B] = 0x05
    b[0x37C] = 0x11
    for i in range(0x3FD, 0x412):
        b[i] = 0x00
    digest = _hashlib.sha1(bytes(b[0x344:0x344 + 0xACBC])).digest()
    b[0x32C:0x32C + 20] = digest
    return bytes(b)


def is_nxe_install(data):
    """True if the header looks like an NXE-installed game (content type 0x4000)."""
    return len(data) >= 0x348 and data[0x344:0x348] == b"\x00\x00\x40\x00"


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: god {info|list} <god-dir>  |  god2iso <god.data-dir> <out.iso>  |  "
              "iso2god <image.iso> <out-dir>  |  install2god <con-header> <out-header>")
        return
    op = a[0]
    if op == "install2god":
        if len(a) < 3:
            print("usage: god install2god <installed-STFS-header> <out-GoD-header>"); sys.exit(2)
        data = open(a[1], "rb").read()
        if not is_nxe_install(data):
            print("warning: header content-type is not 0x00004000 (NXE-installed); converting anyway")
        open(a[2], "wb").write(install_to_god(data))
        print("GoD/LIVE header written -> %s (copy the sibling .data folder alongside)" % a[2])
        return
    if op in ("info", "list"):
        from codebreaker.formats.xbox import gdf
        data_dir = a[1]
        if not os.path.isdir(data_dir):
            print("give the .data directory of a GoD package"); sys.exit(2)
        g = gdf.GDF(SvodReader(data_dir), base=0)
        fs = g.files()
        if op == "info":
            print("GoD .data : %s" % data_dir)
            print("root sector: %d  files: %d  total: %d bytes"
                  % (g.root_sector, len(fs), sum(s for _, s in fs)))
        else:
            for p, sz in fs:
                print("  %12d  %s" % (sz, p))
            print("total: %d files" % len(fs))
    elif op == "god2iso":
        if len(a) < 3:
            print("usage: god2iso <god.data-dir> <out.iso>"); sys.exit(2)
        data_dir, out = a[1], a[2]
        sv = SvodReader(data_dir)
        # total logical size = data blocks across all parts
        total = 0
        for path in sv.paths:
            phys = os.path.getsize(path)
            # each part holds (phys - master) / (subpart_hash + subpart_data) subparts
            body = phys - BLOCK
            n_sub = body // (BLOCK + SUBPART_SIZE)
            rem = body - n_sub * (BLOCK + SUBPART_SIZE)
            total += n_sub * SUBPART_SIZE + max(0, rem - BLOCK)
        with open(out, "wb") as f:
            off = 0
            while off < total:
                chunk = sv.read_at(off, 8 * 1024 * 1024)
                if not chunk:
                    break
                f.write(chunk); off += len(chunk)
        print("wrote %d bytes -> %s" % (off, out))
    elif op == "iso2god" or op == "convert":
        if len(a) < 3:
            print("usage: iso2god <image.iso> <out-dir> [--title NAME]"); sys.exit(2)
        from codebreaker.formats.xbox import gdf
        from codebreaker.formats import xex
        iso, outdir = a[1], a[2]
        title = ""
        if "--title" in a:
            title = a[a.index("--title") + 1]
        tpl = _template()
        if tpl is None:
            print("error: empty_live.bin template not found next to god.py"); sys.exit(1)
        disc = gdf.GDF(gdf._FileDevice(iso))          # locate the XDVDFS volume
        base = disc.base
        size = os.path.getsize(iso) - base
        dev = gdf._FileDevice(iso)

        def vol(off, length):
            return dev.read_at(base + off, length)
        # title/media id: read from the disc's default.xex if present, else 0
        title_id = media_id = 0
        try:
            media_id, title_id = xex.ids(disc.read("DEFAULT.XEX"))
        except Exception:
            pass
        print("  title_id=%08X  media_id=%08X" % (title_id, media_id))
        con, ddir, n = write_god(vol, size, outdir, tpl, title_id=title_id,
                                 media_id=media_id, title=title)
        print("wrote GoD: %d part(s)" % n)
        print("  con header: %s" % con)
        print("  data dir  : %s" % ddir)
    elif op == "xex2god":
        if len(a) < 3:
            print("usage: xex2god <default.xex> <out-dir> [--title NAME]"); sys.exit(2)
        from codebreaker.formats.xbox import gdf
        from codebreaker.formats import xex
        xexpath, outdir = a[1], a[2]
        title = a[a.index("--title") + 1] if "--title" in a else ""
        data = open(xexpath, "rb").read()
        media_id, title_id = xex.ids(data)
        vol = gdf.build_xdvdfs({"default.xex": data})
        con, ddir, n = write_god(lambda o, l: vol[o:o + l], len(vol), outdir, _template(),
                                 title_id=title_id, media_id=media_id, title=title)
        print("wrote GoD from XEX: title_id=%08X media_id=%08X, %d part(s)" % (title_id, media_id, n))
        print("  con header: %s" % con)
    else:
        print("unknown op %r" % op); sys.exit(2)


if __name__ == "__main__":
    main()
