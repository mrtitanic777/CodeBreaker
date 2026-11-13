#!/usr/bin/env python3
"""Makes the fixtures and goldens of native/cb/formats/xbox/{xbe,xdbf,xpr,xur,bink,chd}_test.cpp.

    python make_asset_goldens.py              writes the fixtures (*.bin) and python_goldens.txt beside this file
    python make_asset_goldens.py dump <dir>   prints the same dump of every .xdbf / .xur / .xbe / .xpr / .bik / .chd file of <dir>
                                              (the proof against real files: the C++ test with CB_XBOX_PROOF_DIR=<dir> compares)

The inputs are the ones tests/test_xbox_formats.py and tests/test_xbe.py build; the dump is what the Python readers
(codebreaker/formats/xbox/*.py, through formats.dll) answer for them, one `name.key=value` line each. Run from a checkout with
CB_FORMATS_BIN set to a built formats.dll (native/formats/build/bin).
"""
import hashlib
import math
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", "..", "..", "..", ".."))
sys.path.insert(0, ROOT)

from codebreaker.formats.xbox import bink, chd, xbe, xdbf, xpr, xur  # noqa: E402


def sha(data):
    return hashlib.sha1(data).hexdigest()[:12]


def esc(s):
    return s.replace("\\", "\\\\").replace("\n", "\\n").replace("\r", "\\r")


# ---- the dumps --------------------------------------------------------------------------------------------------
def dump_xdbf(n, data):
    x = xdbf.XDBF(data)
    out = ["%s.version=0x%X" % (n, x.version), "%s.entries=%d" % (n, len(x.entries)), "%s.table=%d" % (n, x.entry_table_len),
           "%s.free=%d" % (n, x.free_count), "%s.data_start=0x%X" % (n, x.data_start)]
    for ns, c in sorted(x.counts().items()):
        out.append("%s.ns%d=%d" % (n, ns, c))
    for i, (ns, eid, off, length) in enumerate(x.entries):
        out.append("%s.entry%d=%d,%016X,%d,%s" % (n, i, ns, eid, length, sha(x.payload(ns, eid))))
    for i, a in enumerate(x.achievements()):
        out.append("%s.ach%d=%d,%d,%d,%d,%d,%s|%s|%s" % (n, i, a["id"], a["image"], a["score"], a["flags"], a["unlocked"],
                                                          esc(a["name"]), esc(a["unlocked_desc"]), esc(a["locked_desc"])))
    out.append("%s.build=%s" % (n, sha(x.build())))
    first = x.achievements()[0]["id"] if x.achievements() else None
    if first is not None:
        y = xdbf.XDBF(data)
        ok = y.unlock_achievement(first, True)
        out.append("%s.unlock_first=%d,%s" % (n, ok, sha(y.build())))
        y.unlock_achievement(first, False)
        out.append("%s.lock_first=%s" % (n, sha(y.build())))
    z = xdbf.XDBF(data)
    out.append("%s.unlock_all=%d,%s" % (n, z.unlock_all(), sha(z.build())))
    w = xdbf.XDBF(data)
    w.set_payload(5, 0x1234, b"hello")
    out.append("%s.set_new=%s" % (n, sha(w.build())))
    return out


def dump_xbe(n, data):
    x = xbe.XBE(data)
    ep, epk = x.entry_point()
    kt, ktk = x.kernel_thunk()
    out = ["%s.title=%s" % (n, esc(x.title_name)), "%s.title_id=%08X,%s" % (n, x.title_id, x.title_id_str()),
           "%s.base=0x%X" % (n, x.base), "%s.size=%d" % (n, x.size_of_image), "%s.sections=%d" % (n, x.section_count),
           "%s.region=0x%X,%s" % (n, x.region, x.region_str()), "%s.media=0x%X,%s" % (n, x.allowed_media, x.media_str()),
           "%s.entry=%08X,%s" % (n, ep, epk), "%s.thunk=%08X,%s" % (n, kt, ktk),
           "%s.ratings=0x%X,%d,%d" % (n, x.ratings, x.version, x.disk_number)]
    for i, s in enumerate(x.sections()):
        out.append("%s.section%d=%s,0x%X,0x%X,%d,0x%X,%d" % (n, i, esc(s["name"]), s["flags"], s["vaddr"], s["vsize"], s["raw_addr"], s["raw_size"]))
    try:
        pe = xbe.pe_image(x)
        out.append("%s.pe=%d,%s" % (n, len(pe), sha(pe)))
    except Exception as e:                       # the verdict only: the messages differ
        out.append("%s.pe=error" % n)
    return out


def dump_xpr(n, data):
    info = xpr.parse(data)
    out = []
    for k in ("container", "format_name", "dds_fmt", "width", "height", "mipmaps", "cubemap", "data_offset", "header_size", "data_size", "total"):
        if k in info:
            v = info[k]
            out.append("%s.%s=%s" % (n, k, int(v) if isinstance(v, bool) else v))
    out.append("%s.pixels=%d,%s" % (n, len(info["pixels"]), sha(info["pixels"])))
    try:
        if info["container"] == "XPR2":
            # texture.untile answers only as much of the surface as the (short) pixel data reaches; ours the whole surface: compare the part both have
            dds = xpr.to_dds(info, 64, 32)
            out.append("%s.dds_prefix=%s" % (n, sha(dds[:1408])))
        else:
            dds = xpr.to_dds(info)
            out.append("%s.dds=%d,%s" % (n, len(dds), sha(dds)))
    except Exception:
        out.append("%s.dds=error" % n)
    return out


def dump_xur(n, data):
    x = xur.XUR(data)
    out = ["%s.version=%d" % (n, x.version), "%s.flags=%d" % (n, x.flags), "%s.tool=%d" % (n, x.tool_version),
           "%s.file_size=%d" % (n, x.file_size), "%s.sections=%d" % (n, x.section_count)]
    for i, (m, off, length) in enumerate(x.sections):
        out.append("%s.section%d=%s,%d,%d" % (n, i, xur._tag(m), off, length))
    strs = x.strings()
    out.append("%s.strings=%d" % (n, len(strs)))
    for i, s in enumerate(strs):
        out.append("%s.string%d=%s" % (n, i, esc(s)))
    try:
        out.append("%s.identity=%d" % (n, xur.set_strings(data, strs) == data))
        if strs:
            out.append("%s.set0=%s" % (n, sha(xur.set_string(data, 0, "Hello w\u00f6rld"))))
    except Exception:
        out.append("%s.identity=error" % n)
    return out


def dump_bink(n, path):
    h = bink.read_header(path)
    return ["%s.%s=%s" % (n, k, h[k]) for k in ("generation", "codec", "file_size", "frames", "largest_frame", "width", "height", "fps",
                                                   "video_flags", "audio_tracks")]


def dump_chd(n, path):
    h = chd.read_header(path)
    return ["%s.%s=%s" % (n, k, ",".join(h[k]) if isinstance(h[k], list) else h[k])
            for k in ("version", "header_len", "logical_bytes", "hunk_bytes", "unit_bytes", "compressors", "meta_offset", "map_offset")]


# ---- the inputs (what the Python tests build) -------------------------------------------------------------------
def gpd_blob():
    def rec(ach_id, score, name):
        strs = b"".join(s.encode("utf-16-be") + b"\x00\x00" for s in (name, "Unlocked", "Locked"))
        body = struct.pack(">IIIIIQ", 0, ach_id, 5, score, 0, 0) + strs
        return struct.pack(">I", len(body)) + body[4:]
    ents = [(1, 1, rec(1, 10, "First Blood")), (1, 2, rec(2, 25, "Marathon"))]
    heap, table = b"", b""
    for ns, eid, p in ents:
        table += struct.pack(">HQII", ns, eid, len(heap), len(p))
        heap += p
    return b"XDBF" + struct.pack(">IIIII", 0x10000, len(ents), len(ents), 1, 1) + table + struct.pack(">II", len(heap), 0) + heap


def xbe_small():
    img = bytearray(0x600)
    img[0:4] = b"XBEH"
    B = 0x10000
    for off, v in ((0x104, B), (0x10C, 0x2000), (0x118, B + 0x200), (0x11C, 0), (0x120, B + 0x300),
                   (0x128, 0x11030 ^ 0xA8FC57AB), (0x148, 0x11500 ^ 0x5B6D40B6), (0x200 + 0x08, 0x4D530037), (0x200 + 0xA0, 0x1)):
        struct.pack_into("<I", img, off, v)
    img[0x20C:0x20C + 8] = "Halo".encode("utf-16-le")
    return bytes(img)


def xbe_demo():
    BASE, TEXT, DATA = 0x10000, 0x11000, 0x12000
    code = (b"\x68" + struct.pack("<I", DATA + 0x10) + b"\xff\x15" + struct.pack("<I", DATA) + b"\x83\xc4\x04\x33\xc0\xc3")
    data = struct.pack("<II", 0x80000008, 0) + bytes(8) + b"hello, kernel\0"
    hdr = bytearray(0x1000)
    hdr[0:4] = b"XBEH"
    struct.pack_into("<I", hdr, 0x104, BASE)
    struct.pack_into("<I", hdr, 0x108, 0x1000)
    struct.pack_into("<I", hdr, 0x10C, 0x3000)
    struct.pack_into("<I", hdr, 0x118, BASE + 0x200)
    struct.pack_into("<I", hdr, 0x11C, 2)
    struct.pack_into("<I", hdr, 0x120, BASE + 0x400)
    struct.pack_into("<I", hdr, 0x128, TEXT ^ xbe.ENTRY_KEYS["retail"])
    struct.pack_into("<I", hdr, 0x148, DATA ^ xbe.THUNK_KEYS["retail"])
    struct.pack_into("<I", hdr, 0x200 + 0x08, 0x4D530001)
    hdr[0x20C:0x20C + 8] = "Demo".encode("utf-16-le")
    for i, (va, body, flags, name_at) in enumerate(((TEXT, code, 0x6, 0x480), (DATA, data, 0x1, 0x488))):
        struct.pack_into("<IIIIII", hdr, 0x400 + 0x38 * i, flags, va, 0x1000, 0x1000 * (i + 1), len(body), BASE + name_at)
    hdr[0x480:0x486] = b".text\0"
    hdr[0x488:0x48E] = b".data\0"
    return bytes(hdr) + code.ljust(0x1000, b"\0") + data.ljust(0x1000, b"\0")


def xur_blob():
    strs = ["XuiCanvas", "file://game:/skin.png", "Press A to continue", ""]
    strn = xur.build_strn(strs)
    return (struct.pack(">III", xur.MAGIC, 5, 0) + struct.pack(">HIH", 0x0C, 32 + len(strn), 1) +
            struct.pack(">III", xur.STRN, 32, len(strn)) + strn)


def xpr_blobs():
    w, h = 64, 32
    pixels = bytes((i * 7) & 0xFF for i in range(w * h // 2))
    return xpr.build_xpr0(pixels, w, h, xfmt=0x0C), b"XPR2" + struct.pack(">II", 0x14, len(pixels)) + b"\x00" * 8 + pixels


def chd_blob():
    def tag(s):
        return struct.unpack(">I", s.encode("latin-1"))[0] if s else 0
    head = b"MComprHD" + struct.pack(">II", 124, 5) + struct.pack(">4I", tag("zlib"), tag("lzma"), 0, 0)
    head += struct.pack(">QQQ", 4700000256, 0x1234, 0x7F00)
    head += struct.pack(">II", 19584, 2048)
    return head.ljust(124, b"\0")


def xwb_blob():
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
    return bytes(h)


def xzp_blob():
    import tempfile
    from codebreaker.formats.xbox import xzp
    tpl = b"XUIZ" + struct.pack(">IIII", 1, 0, 1, 0) + struct.pack(">H", 0) + b"\x00"
    d = tempfile.mkdtemp(prefix="cb_xzp_")
    payload = {"logo.png": bytes.fromhex("89504e470d0a1a0a") + b"\x00" * 40, "data.bin": b"xyz" * 200}
    for nm, data in payload.items():
        open(os.path.join(d, nm), "wb").write(data)
    out = os.path.join(d, "packed.xzp")
    xzp.build(d, out, tpl)
    return open(out, "rb").read()


def dump_summary(name, path):
    from codebreaker.formats import detect
    from codebreaker.formats import xbox as xbox_package
    got = detect.identify(path)
    return ["summary.%s.%s=%s" % (name, k, v) for k, v in xbox_package.summary(path, got[0] if got else {}).items()]


def bink_blob():
    return b"BIKi" + struct.pack("<10I", 1000000, 300, 8192, 300, 1280, 720, 30, 1, 0, 1) + bytes(20)


FIXTURES = {
    "gpd.bin": gpd_blob(), "xbe_small.bin": xbe_small(), "xbe_demo.bin": xbe_demo(), "xur.bin": xur_blob(),
    "xpr0.bin": xpr_blobs()[0], "xpr2.bin": xpr_blobs()[1], "t.bik": bink_blob(), "t.chd": chd_blob(), "xwb.bin": xwb_blob(), "pack.xzp": xzp_blob(),
}


def main():
    if len(sys.argv) > 2 and sys.argv[1] == "dump":
        folder = sys.argv[2]
        for name in sorted(os.listdir(folder)):
            path = os.path.join(folder, name)
            ext = os.path.splitext(name)[1].lower()
            data = open(path, "rb").read() if os.path.isfile(path) else b""
            if ext == ".xdbf":
                lines = dump_xdbf(name, data)
            elif ext == ".xur":
                lines = dump_xur(name, data)
            elif ext == ".xbe":
                lines = dump_xbe(name, data)
            elif ext == ".xpr":
                lines = dump_xpr(name, data)
            elif ext == ".bik":
                lines = dump_bink(name, path)
            elif ext == ".chd":
                lines = dump_chd(name, path)
            else:
                continue
            sys.stdout.buffer.write(("\n".join(lines) + "\n").encode("utf-8"))
        return
    for name, blob in FIXTURES.items():
        open(os.path.join(HERE, name), "wb").write(blob)
    lines = []
    lines += dump_xdbf("gpd.bin", FIXTURES["gpd.bin"])
    lines += dump_xbe("xbe_small.bin", FIXTURES["xbe_small.bin"])
    lines += dump_xbe("xbe_demo.bin", FIXTURES["xbe_demo.bin"])
    lines += dump_xur("xur.bin", FIXTURES["xur.bin"])
    lines += dump_xpr("xpr0.bin", FIXTURES["xpr0.bin"])
    lines += dump_xpr("xpr2.bin", FIXTURES["xpr2.bin"])
    lines += dump_bink("t.bik", os.path.join(HERE, "t.bik"))
    lines += dump_chd("t.chd", os.path.join(HERE, "t.chd"))
    for name in ("gpd.bin", "xbe_demo.bin", "xpr0.bin", "xpr2.bin", "xur.bin", "t.bik", "t.chd", "xwb.bin", "pack.xzp"):
        lines += dump_summary(name, os.path.join(HERE, name))
    open(os.path.join(HERE, "python_goldens.txt"), "w", newline="\n").write("\n".join(lines) + "\n")
    print("wrote %d fixtures and %d golden lines" % (len(FIXTURES), len(lines)))


if __name__ == "__main__":
    main()
