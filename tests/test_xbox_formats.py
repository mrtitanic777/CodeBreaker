#!/usr/bin/env python3
"""The Xbox and Xbox 360 formats (codebreaker/formats/xbox), what they are (detect.identify), and
how media and info read them.

    python tests/test_xbox_formats.py

Self-contained: every container and asset is built here, and the STFS signing path runs on a
SYNTHETIC keyvault carrying a fixed test RSA key (no real console key, no user data). Real
sample files, when present on this machine (Projects/...), add read checks; without them those
are SKIPPED."""
import hashlib
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.formats import detect, inspect, media       # noqa: E402
from codebreaker.formats.xbox import (bink, fatx, gdf, god, stfs, texture, xbe, xdbf, xma,  # noqa: E402
                                      xpr, xur, xwb, xzp)

EMPTY_LIVE = os.path.join(ROOT, "codebreaker", "formats", "xbox", "empty_live.bin")
SAMPLES = os.path.join(ROOT, "Projects")
RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _tmp(name):
    return os.path.join(tempfile.mkdtemp(prefix="cb_xbox_"), name)


# ---- a synthetic keyvault (a fixed TEST key: signs nothing a console accepts) --------------------
_TP = 0xf4b80c2e03aea244733b38ca5f750a8796ce783b02571aa42d2de77c5ca0fa9e4271fc24773a7faa0bf5836d0ca8b1bf1644c988b23b44cb65035e33d3ab7441
_TQ = 0xe1dcd0fb015e7f1e4cc275b9154c66ef6077e99f821a1a82003d2eaa026ceae8ef7f4e59c8d72573a4e6ab8c3ad031f3d61959e0419989a5fdd33c2b170b10bd
_TN, _E = _TP * _TQ, 0x10001


def _bn(v, size):
    swapped = v.to_bytes(size, "little")
    return b"".join(swapped[i:i + 8][::-1] for i in range(0, size, 8))


def _synth_kv(console_id=b"\xAB\xCD\xEF\x01\x23"):
    kv = bytearray(0x4000)
    c = stfs.KV_CERT_OFF
    struct.pack_into(">H", kv, c, 0x01A8)
    kv[c + 0x02:c + 0x07] = console_id
    kv[c + 0x07:c + 0x1B] = b"X000000-000".ljust(0x14, b"\x00")
    kv[c + 0x1B] = 0x02
    kv[c + 0x1C:c + 0x24] = b"01-01-13"
    struct.pack_into(">I", kv, c + 0x24, _E)
    kv[c + 0x28:c + 0xA8] = _bn(_TN, 0x80)
    k = stfs.KV_PRV_OFF
    struct.pack_into(">I", kv, k, 0x10)
    struct.pack_into(">I", kv, k + 0x04, _E)
    kv[k + 0x10:k + 0x90] = _bn(_TN, 0x80)
    kv[k + 0x90:k + 0xD0] = _bn(_TP, 0x40)
    kv[k + 0xD0:k + 0x110] = _bn(_TQ, 0x40)
    return bytes(kv)


# ---- STFS -------------------------------------------------------------------------------------
def test_stfs_pack_round_trip():
    d = tempfile.mkdtemp(prefix="cb_stfs_")
    payload = {"alpha.bin": b"A" * 5000, "beta.txt": b"hello world" * 100}
    for nm, data in payload.items():
        open(os.path.join(d, nm), "wb").write(data)
    out = os.path.join(d, "packed.con")
    stfs.build(d, out, EMPTY_LIVE)
    s = stfs.STFS(open(out, "rb").read())
    assert sorted(s.files) == sorted(payload), sorted(s.files)
    for nm, data in payload.items():
        assert s.read_file(nm) == data, nm
    v = media.volumes(out)                                       # and through media
    assert v[0].kind == "stfs" and {e.path: v[0].read(e) for e in v[0].files()} == payload


def test_stfs_resign_signs_and_verifies():
    kv = _synth_kv()
    live = open(EMPTY_LIVE, "rb").read()
    out = stfs.resign(live, kv)
    assert out[:4] == b"CON "
    region, sig = bytes(out[0x22C:0x344]), bytes(out[0x1AC:0x1AC + 0x80])
    n = stfs._bn(bytes(out[0x2C:0x2C + 0x80]))
    blk = pow(int.from_bytes(sig[::-1], "big"), _E, n).to_bytes(0x80, "big")
    assert blk[:2] == b"\x00\x01" and blk.endswith(stfs._SHA1_DER + hashlib.sha1(region).digest())
    assert out[0x22C:0x234] == b"\xFF" * 8 and out[0x344:] == live[0x344:]     # LIVE -> CON
    assert out[0x04:0x1AC] == kv[0x9C8:0x9C8 + 0x1A8]
    assert stfs.resign(out, kv) == out                                         # deterministic


# ---- discs, Games on Demand ------------------------------------------------------------------
def test_god_round_trip():
    payload = {"default.xex": b"XEX2" + b"\x00" * 3000, "readme.txt": b"hello GoD" * 64}
    vol = gdf.build_xdvdfs(payload)
    d = tempfile.mkdtemp(prefix="cb_god_")
    _con, data_dir, _parts = god.write_god(lambda o, l: vol[o:o + l], len(vol), d, open(EMPTY_LIVE, "rb").read(),
                                           title_id=0x12345678, media_id=0xABCDEF01)
    g = gdf.GDF(god.SvodReader(data_dir), base=0)
    for nm, data in payload.items():
        assert g.read(nm) == data, nm
    v = media.volumes(d)                                          # the folder, through media
    assert v[0].kind == "god" and {e.path: v[0].read(e) for e in v[0].files()} == payload


def test_nxe_install_to_god():
    src = bytearray(0xB000)
    src[0:4] = b"CON "
    for i in range(0x344, 0x344 + 0xACBC):
        src[i] = (i * 31) & 0xFF
    src[0x344:0x348] = b"\x00\x00\x40\x00"
    assert god.is_nxe_install(bytes(src))
    out = god.install_to_god(bytes(src))
    assert out[0:4] == b"LIVE" and all(out[i] == 0xFF for i in range(0x22C, 0x234))
    assert out[0x342] == 0xAD and out[0x343] == 0x0E and out[0x346] == 0x70
    assert out[0x32C:0x32C + 20] == hashlib.sha1(out[0x344:0x344 + 0xACBC]).digest()


# ---- FATX ---------------------------------------------------------------------------------------
def _fatx_image():
    """A tiny big-endian (Xbox 360) FATX partition: a live file, a deleted file with its data,
    a folder with a file in it."""
    BPC, DATA = 0x200, 0x2000
    img = bytearray(DATA + 8 * BPC)
    struct.pack_into(">4sLLL", img, 0, b"FATX", 0x12345678, 1, 1)
    ts = ((2015 - 1980) << 25) | (6 << 21) | (15 << 16) | (12 << 11) | (30 << 5) | 10

    def dirent(nl, attr, name, first, size):
        return struct.pack(">BB42sLLLLL", nl, attr, name.encode() + b"\xff" * (42 - len(name)), first, size, ts, ts, ts)

    def at(c):
        return DATA + BPC * (c - 1)
    root = (dirent(9, 0x20, "hello.txt", 2, 15) + dirent(0xE5, 0x20, "deleted.bin", 3, 20) +
            dirent(6, 0x10, "subdir", 4, 0) + b"\xff" + b"\x00" * 63)
    img[at(1):at(1) + len(root)] = root
    img[at(2):at(2) + 15] = b"Hello FATX 360!"
    img[at(3):at(3) + 20] = b"SECRET deleted data!"
    sub = dirent(10, 0x20, "nested.dat", 5, 11) + b"\xff" + b"\x00" * 63
    img[at(4):at(4) + len(sub)] = sub
    img[at(5):at(5) + 11] = b"nested data"
    fat = [0xFFF8, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF] + [0] * (0x800 - 6)
    struct.pack_into(">" + "H" * len(fat), img, 0x1000, *fat)
    return bytes(img)


def test_fatx_read_and_recover():
    path = _tmp("test.fatx")
    open(path, "wb").write(_fatx_image())
    v = media.volumes(path)
    assert v and v[0].kind == "fatx"
    files = {e.path: e for e in v[0].files()}
    assert set(files) == {"hello.txt", "subdir", "subdir/nested.dat"}, sorted(files)
    assert v[0].read(files["hello.txt"]) == b"Hello FATX 360!"
    assert v[0].read(files["subdir/nested.dat"]) == b"nested data"
    r = fatx.FatxReader(path)
    p = r.detect_partitions()[0]
    out = os.path.join(os.path.dirname(path), "rec")
    r.recover_all(p, out)
    r.close()
    assert open(os.path.join(out, "deleted.bin"), "rb").read() == b"SECRET deleted data!"


def test_fatx_write():
    path = _tmp("w.fatx")
    open(path, "wb").write(_fatx_image())
    r = fatx.FatxReader(path)
    p = r.detect_partitions()[0]
    r.add_file(p, "newfile.bin", b"INJECTED " * 20)
    r.mkdir(p, "newdir")
    r.add_file(p, "newdir/nested.txt", b"hello nested")
    r.close()
    v = media.volumes(path)[0]
    files = {e.path: e for e in v.files()}
    assert v.read(files["newfile.bin"]) == b"INJECTED " * 20 and v.read(files["newdir/nested.txt"]) == b"hello nested"


# ---- assets -------------------------------------------------------------------------------------
def test_gpd_unlock():
    def rec(ach_id, score, name):
        strs = b"".join(s.encode("utf-16-be") + b"\x00\x00" for s in (name, "Unlocked", "Locked"))
        body = struct.pack(">IIIIIQ", 0, ach_id, 5, score, 0, 0) + strs
        return struct.pack(">I", len(body)) + body[4:]
    ents = [(1, 1, rec(1, 10, "First Blood")), (1, 2, rec(2, 25, "Marathon"))]
    heap, table = b"", b""
    for ns, eid, p in ents:
        table += struct.pack(">HQII", ns, eid, len(heap), len(p))
        heap += p
    gpd = b"XDBF" + struct.pack(">IIIII", 0x10000, len(ents), len(ents), 1, 1) + table + struct.pack(">II", len(heap), 0) + heap
    x = xdbf.XDBF(gpd)
    x.unlock_achievement(1, True)
    y = xdbf.XDBF(x.build())
    a = {r["id"]: r for r in y.achievements()}
    assert a[1]["unlocked"] and not a[2]["unlocked"]
    assert struct.unpack_from(">Q", y.payload(1, 1), 0x14)[0] == xdbf.XDBF.UNLOCK_FILETIME
    assert a[1]["name"] == "First Blood" and a[2]["score"] == 25


def test_texture_untile():
    for fmt in ("dxt1", "dxt5"):
        bb = texture.FORMATS[fmt][0]
        for w, h in [(64, 64), (128, 256), (32, 32), (256, 128)]:
            bw, bh = texture._blocks(w, h)
            linear = bytes((i * 7 + 3) & 0xff for i in range(bw * bh * bb))
            assert texture.untile(texture.tile(linear, w, h, fmt), w, h, fmt) == linear, (fmt, w, h)
    bw, bh = texture._blocks(256, 256)
    offs = [texture.tiled_offset_2d(x, y, bw, 4) for y in range(bh) for x in range(bw)]
    assert len(set(offs)) == len(offs)
    hdr = texture.dds_header(128, 128, "dxt5")
    assert hdr[:4] == b"DDS " and len(hdr) == 128 and hdr[84:88] == b"DXT5"


def test_xpr():
    ok, msg = xpr._selfcheck()
    assert ok, msg


def test_xwb():
    def fmt(codec, ch, rate):
        return codec | (ch << 2) | (rate << 5)
    pcm, x_ma = bytes(range(32)), bytes([0xAA] * 16)
    h = bytearray(b"WBND" + struct.pack(">II", 46, 1))
    segpos = len(h)
    h += b"\x00" * 40
    seg0 = len(h)
    wbd = struct.pack(">II", 0, 2) + b"TestBank".ljust(64, b"\x00") + struct.pack(">III", 24, 0, 4)
    h += wbd
    seg1 = len(h)
    h += struct.pack(">IIIIII", 0, fmt(0, 2, 44100), 0, 32, 0, 0)
    h += struct.pack(">IIIIII", 0, fmt(1, 2, 48000), 32, 16, 0, 0)
    seg2 = len(h)
    h += pcm + x_ma
    struct.pack_into(">10I", h, segpos, seg0, len(wbd), seg1, 48, seg2, 0, seg2, 0, seg2, len(pcm + x_ma))
    x = xwb.XWB(bytes(h))
    assert x.en == ">" and x.version == 46 and x.entry_count == 2
    e0, e1 = x.entries
    assert e0["codec_name"] == "PCM" and e0["rate"] == 44100 and e1["codec_name"] == "XMA" and e1["rate"] == 48000
    assert x.stream(e1) == x_ma
    wav = xwb.pcm_to_wav(pcm, 2, 44100, 16)
    assert wav[:4] == b"RIFF" and wav[8:12] == b"WAVE" and wav[44:] == bytes(pcm[i ^ 1] for i in range(len(pcm)))


def test_xur():
    strs = ["XuiCanvas", "file://game:/skin.png", "Press A to continue", ""]
    strn = xur.build_strn(strs)
    blob = (struct.pack(">III", xur.MAGIC, 5, 0) + struct.pack(">HIH", 0x0C, 32 + len(strn), 1) +
            struct.pack(">III", xur.STRN, 0, len(strn)) + strn)
    x = xur.XUR(blob)
    assert x.version == 5 and x.section_count == 1 and x.strings() == strs
    assert xur.set_strings(blob, strs) == blob
    assert xur.XUR(xur.set_string(blob, 2, "Press START")).strings()[2] == "Press START"


def test_xbe():
    img = bytearray(0x600)
    img[0:4] = b"XBEH"
    B = 0x10000
    for off, v in ((0x104, B), (0x10C, 0x2000), (0x118, B + 0x200), (0x11C, 0), (0x120, B + 0x300),
                   (0x128, 0x11030 ^ 0xA8FC57AB), (0x148, 0x11500 ^ 0x5B6D40B6), (0x200 + 0x08, 0x4D530037),
                   (0x200 + 0xA0, 0x1)):
        struct.pack_into("<I", img, off, v)
    img[0x20C:0x20C + 8] = "Halo".encode("utf-16-le")
    x = xbe.XBE(bytes(img))
    assert x.title_name == "Halo" and x.entry_point() == (0x11030, "retail")
    path = _tmp("default.xbe")
    open(path, "wb").write(bytes(img))
    info = inspect.describe(path)
    assert info["summary"]["title"] == "Halo" and info["summary"]["title id"] == "4D530037", info["summary"]


def test_xma_and_bink():
    riff = xma.wrap_xma2(bytes(5000), 2, 44100)
    assert riff[:4] == b"RIFF" and riff[8:12] == b"WAVE" and struct.unpack_from("<H", riff, 20)[0] == 0x166
    bp = _tmp("t.bik")
    open(bp, "wb").write(b"BIKi" + struct.pack("<10I", 1000000, 300, 8192, 300, 1280, 720, 30, 1, 0, 1) + bytes(20))
    h = bink.read_header(bp)
    assert h["width"] == 1280 and h["height"] == 720 and h["fps"] == 30.0 and h["audio_tracks"] == 1


def test_xzp_round_trip():
    tpl = b"XUIZ" + struct.pack(">IIII", 1, 0, 1, 0) + struct.pack(">H", 0) + b"\x00"
    d = tempfile.mkdtemp(prefix="cb_xzp_")
    payload = {"logo.png": bytes.fromhex("89504e470d0a1a0a") + b"\x00" * 40, "data.bin": b"xyz" * 200}
    for nm, data in payload.items():
        open(os.path.join(d, nm), "wb").write(data)
    out = os.path.join(d, "packed.xzp")
    xzp.build(d, out, tpl)
    v = media.volumes(out)[0]
    assert v.kind == "xzp" and {e.path: v.read(e) for e in v.files()} == payload


# ---- identify, info, the folder map --------------------------------------------------------------
def test_identify():
    def ident(blob, name="f.bin"):
        p = _tmp(name)
        open(p, "wb").write(blob)
        got = detect.identify(p)
        return got[0] if got else {}
    assert ident(b"XEX2" + bytes(60))["command"] == "xex"
    assert ident(b"FATX" + bytes(60))["command"] == "media"
    assert "CSO" in ident(b"CISO" + bytes(60))["format"]
    assert "XPR2" in ident(b"XPR2" + bytes(60))["format"]
    assert "CHD" in ident(b"MComprHD" + bytes(8) + struct.pack(">I", 5))["format"]
    assert "Bink" in ident(b"BIKi" + bytes(60))["format"]
    con = bytearray(0x400)
    con[0:4] = b"CON "
    struct.pack_into(">I", con, 0x344, 0x7000)
    assert ident(bytes(con))["detail"] == "Games on Demand"
    disc = bytearray(0x10000 + 20)
    disc[0x10000:0x10000 + 20] = b"MICROSOFT*XBOX*MEDIA"
    assert "XDVDFS" in ident(bytes(disc))["format"]
    assert "PowerPC" in ident(struct.pack("<HH", 0x01F2, 1) + bytes(40))["detail"]
    assert ident(b"\xca\xfe\xba\xbe" + struct.pack(">I", 2) + bytes(40))["format"] == "Mach-O universal binary"
    assert ident(b"\xca\xfe\xba\xbe" + struct.pack(">HH", 0, 52) + bytes(40))["format"] == "Java class file"
    assert ident(b"NES\x1a" + bytes(60))["family"] == "nes"
    assert ident(b"hello", "x.zzz") == {}


def test_folder_map():
    d = tempfile.mkdtemp(prefix="cb_map_")
    open(os.path.join(d, "a.xex"), "wb").write(b"XEX2" + bytes(60))
    open(os.path.join(d, "b.xex"), "wb").write(b"XEX2" + bytes(60))
    open(os.path.join(d, "main.c"), "w").write("int main(void) { return 0; }\n")
    open(os.path.join(d, "blob.zzz"), "wb").write(b"\x01\x02\x03")
    m = inspect.folder_map(d)
    by = {r["format"]: r for r in m["formats"]}
    assert m["files"] == 4 and by["Xbox 360 executable (XEX2)"]["files"] == 2
    assert by["source / text: C"]["files"] == 1
    assert [r["extension"] for r in m["unrecognised"]] == [".zzz"]
    assert "| Xbox 360 executable (XEX2) |" in inspect.markdown(m)


# ---- real samples (when this machine has them) ----------------------------------------------------
def _sample(rel):
    p = os.path.join(SAMPLES, *rel.split("/"))
    if not os.path.exists(p):
        raise Skip("no %s here" % rel)
    return p


def test_real_stfs_theme():
    v = media.volumes(_sample("UP/PixarUP/assets/nxeart"))[0]
    files = {e.path: e for e in v.files()}
    assert v.kind == "stfs" and set(files) == {"DashStyle", "nxebg.jpg", "nxeslot.jpg"}
    assert v.read(files["nxebg.jpg"])[:3] == b"\xff\xd8\xff"


def test_real_xzp():
    v = media.volumes(_sample("XboxTranspiler/samples/dashboard-17559/.dash/Consoles/consoles.xzp"))[0]
    assert len(v.files()) == 668


def test_real_pck():
    v = media.volumes(_sample("XboxTranspiler/samples/tu14/Tutorial.pck"))[0]
    assert sorted(e.path for e in v.files()) == ["GameRules.grf", "languages.loc"]


def test_texture_offsets_and_gfx_bits():
    """The tiled offset is in bytes (every block of a 8x8-block surface at its own block-aligned
    byte); the GFx bit reader reads most-significant first, signed by two's complement."""
    from codebreaker.formats import gfx
    offsets = {texture.tiled_offset_2d(x, y, 32, 4) for y in range(8) for x in range(8)}
    assert texture.tiled_offset_2d(0, 0, 32, 4) == 0 and len(offsets) == 64 and all(o % 16 == 0 for o in offsets)
    b = gfx.Bits(bytes([0b10110000]))
    assert b.read(1) == 1 and b.read(3) == 0b011
    assert gfx.Bits(bytes([0b11110000])).reads(4) == -1


def test_real_heavyiron_and_gfx():
    """Up's own archives (the recompiled title's files, when present): the archive, a DXT5 texture,
    a GFx font rebuilt as TrueType."""
    from codebreaker.formats import gfx
    from codebreaker.formats.xbox import heavyiron
    scenes = os.path.join(ROOT, "Converted", "Up", "assets", "Up", "Global_Scenes")
    if not os.path.isfile(os.path.join(scenes, "FONT.ho")):
        raise Skip("Up's files are not in Converted/Up")
    data = open(os.path.join(scenes, "FONT.ho"), "rb").read()
    P = heavyiron.parse(data)
    assert len(P["layers"]) > 0 and len(heavyiron.all_assets(P)) > 0
    image, w, h, fmt = heavyiron.texture(data, P, 0x000000F00000036B)
    assert (w, h, fmt) == (512, 512, 0x54) and image.getpixel((256, 200))[:3] != (0, 0, 0)
    assert any(name == "Merit Badge" for _v, _id, name, _n, _b, _l in
               (f for _o, _m, body in gfx.find_movies(data) for f in gfx.fonts(body)))
    out = os.path.join(tempfile.mkdtemp(prefix="cb_gfx_"), "font.ttf")
    assert gfx.font_to_ttf(data, 1, out) == ("Merit Badge", 243) and os.path.getsize(out) > 10000


def test_nand_flash():
    """A raw NAND dump built to the layout nand.py reads: a root block (allocation table on even
    pages, directory on odd), an older root copy that must lose, and a file over two blocks, the
    second remapped to another physical block by its spare id."""
    from codebreaker.formats.xbox import nand
    blocks = 0x1000000 // nand.BLOCK + 8
    raw = bytearray(b"\xFF" * (blocks * nand.PAGES_PER_BLOCK * (nand.PAGE + nand.SPARE)))

    def put_block(phys, block_id, data, root_seq=None):
        for i in range(nand.PAGES_PER_BLOCK):
            o = (phys * nand.PAGES_PER_BLOCK + i) * (nand.PAGE + nand.SPARE)
            raw[o:o + nand.PAGE] = data[i * nand.PAGE:(i + 1) * nand.PAGE].ljust(nand.PAGE, b"\0")
            spare = bytearray(nand.SPARE)
            spare[1], spare[2] = block_id & 0xFF, block_id >> 8
            if root_seq is not None:
                spare[12] = 0x30
                spare[0], spare[3], spare[4], spare[6] = root_seq >> 24, (root_seq >> 16) & 0xFF, (root_seq >> 8) & 0xFF, root_seq & 0xFF
            raw[o + nand.PAGE:o + nand.PAGE + nand.SPARE] = spare
    content = bytes(range(256)) * 80                          # 20480 bytes: two blocks
    alloc = [0x1FFE] * 256
    alloc[3], alloc[4] = 4, 0x1FFF
    table = struct.pack(">256H", *alloc)
    entry = b"kernel.bin".ljust(22, b"\0") + struct.pack(">HII", 3, len(content), 0x12345678)
    root = bytearray()
    for i in range(nand.PAGES_PER_BLOCK):
        root += table if i % 2 == 0 else entry.ljust(nand.PAGE, b"\0")
    old = bytearray(root)
    old[nand.PAGE:nand.PAGE + 10] = b"stale.bin\0"
    put_block(1, 1, bytes(old), root_seq=5)
    put_block(2, 2, bytes(root), root_seq=9)
    put_block(3, 3, content[:nand.BLOCK])
    put_block(40, 4, content[nand.BLOCK:])                     # block 4 lives at physical block 40
    raw[:2] = b"\xFF\x4F"
    raw[0x1C:0x1C + 9] = b"Microsoft"
    path = os.path.join(tempfile.mkdtemp(prefix="cb_nand_"), "flash.bin")
    with open(path, "wb") as f:
        f.write(raw)
    assert nand.is_nand(path)
    vols = media.volumes(path)
    assert vols and vols[0].kind == "nand"
    files = {e.path: e for e in vols[0].files()}
    assert set(files) == {"kernel.bin"} and vols[0].read(files["kernel.bin"]) == content


def main():
    for fn in (test_texture_offsets_and_gfx_bits, test_real_heavyiron_and_gfx, test_nand_flash, test_stfs_pack_round_trip, test_stfs_resign_signs_and_verifies, test_god_round_trip,
               test_nxe_install_to_god, test_fatx_read_and_recover, test_fatx_write, test_gpd_unlock,
               test_texture_untile, test_xpr, test_xwb, test_xur, test_xbe, test_xma_and_bink,
               test_xzp_round_trip, test_identify, test_folder_map, test_real_stfs_theme, test_real_xzp,
               test_real_pck):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
