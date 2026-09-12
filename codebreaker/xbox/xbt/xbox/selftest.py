#!/usr/bin/env python3
"""selftest -- self-contained regression checks for the Xbox 360 content tools.

Locks in the byte-exact guarantees so nothing silently regresses. The core
checks are fully self-contained -- they exercise the real STFS pack and resign
code paths using the committed empty_live.bin template and a SYNTHETIC keyvault
built from a fixed test RSA key (no real console key, no user data). Point it at
directories of real sample files (arguments, or $XBT_SAMPLES) to additionally run
golden checks: byte-identical resigns, package reads, account decrypts.

    selftest [sample-dir ...]

Exit code is non-zero if any check FAILs (SKIPs don't fail the run).
"""
DESCRIPTION = "run built-in regression checks (resign crypto, round-trips, goldens)"

import hashlib
import os
import struct
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
# sibling subpackages (crypto/, core/, …) so checks can import across them
# whether run standalone or via the unified CLI (which pre-injects them).
_PKG = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
for _s in ("crypto", "core", "ppc", "win", "match", "assets"):
    _d = os.path.join(_PKG, _s)
    if os.path.isdir(_d) and _d not in sys.path:
        sys.path.insert(0, _d)
import stfs  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
EMPTY_LIVE = os.path.join(HERE, "empty_live.bin")

# A fixed test RSA-1024 keypair. Generated once for this suite; it is NOT a real
# console key and signs nothing that any console would accept -- it exists purely
# to drive the resign code path deterministically without a real keyvault.
_TP = 0xf4b80c2e03aea244733b38ca5f750a8796ce783b02571aa42d2de77c5ca0fa9e4271fc24773a7faa0bf5836d0ca8b1bf1644c988b23b44cb65035e33d3ab7441
_TQ = 0xe1dcd0fb015e7f1e4cc275b9154c66ef6077e99f821a1a82003d2eaa026ceae8ef7f4e59c8d72573a4e6ab8c3ad031f3d61959e0419989a5fdd33c2b170b10bd
_TN = _TP * _TQ
_E = 0x10001


def _bn_encode(v, size):
    """int -> XeCrypt BnQw bytes (inverse of stfs._bn)."""
    swapped = v.to_bytes(size, "little")
    return b"".join(swapped[i:i + 8][::-1] for i in range(0, size, 8))


def _synth_kv(console_id=b"\xAB\xCD\xEF\x01\x23"):
    """A synthetic 0x4000 keyvault carrying the test keypair -- console cert at
    0x9C8 and console private key at 0x298 -- laid out per the real KV struct."""
    kv = bytearray(0x4000)
    c = stfs.KV_CERT_OFF                                   # 0x9C8
    struct.pack_into(">H", kv, c + 0x00, 0x01A8)
    kv[c + 0x02:c + 0x07] = console_id
    kv[c + 0x07:c + 0x1B] = b"X000000-000".ljust(0x14, b"\x00")
    kv[c + 0x1B] = 0x02
    kv[c + 0x1C:c + 0x24] = b"01-01-13"
    struct.pack_into(">I", kv, c + 0x24, _E)
    kv[c + 0x28:c + 0xA8] = _bn_encode(_TN, 0x80)         # modulus (BnQw)
    k = stfs.KV_PRV_OFF                                    # 0x298
    struct.pack_into(">I", kv, k + 0x00, 0x10)            # cqw
    struct.pack_into(">I", kv, k + 0x04, _E)             # e
    kv[k + 0x10:k + 0x90] = _bn_encode(_TN, 0x80)        # n
    kv[k + 0x90:k + 0xD0] = _bn_encode(_TP, 0x40)        # p
    kv[k + 0xD0:k + 0x110] = _bn_encode(_TQ, 0x40)       # q
    return bytes(kv)


# --------------------------------------------------------------- self-contained
def check_pack_roundtrip():
    d = tempfile.mkdtemp()
    payload = {"alpha.bin": b"A" * 5000, "beta.txt": b"hello world" * 100}
    for nm, data in payload.items():
        open(os.path.join(d, nm), "wb").write(data)
    out = os.path.join(d, "packed.con")
    stfs.build(d, out, EMPTY_LIVE)
    s = stfs.STFS(open(out, "rb").read())
    if sorted(s.files) != sorted(payload):
        return "FAIL", "file listing mismatch: %s" % sorted(s.files)
    for nm, data in payload.items():
        if s.read_file(nm) != data:
            return "FAIL", "content mismatch for %s" % nm
    return "PASS", "pack %d files -> read back byte-identical" % len(payload)


def _verify_con(out):
    """Independent PKCS#1 verification of a CON package's console signature."""
    region = bytes(out[0x22C:0x344])
    sig = bytes(out[0x1AC:0x1AC + 0x80])
    n = stfs._bn(bytes(out[0x2C:0x2C + 0x80]))
    blk = pow(int.from_bytes(sig[::-1], "big"), _E, n).to_bytes(0x80, "big")
    want = stfs._SHA1_DER + hashlib.sha1(region).digest()
    return blk.endswith(want) and blk[:2] == b"\x00\x01"


def check_resign_crypto():
    kv = _synth_kv()
    out = stfs.resign(open(EMPTY_LIVE, "rb").read(), kv)   # LIVE -> CON
    if out[:4] != b"CON ":
        return "FAIL", "magic not CON after resign"
    if not _verify_con(out):
        return "FAIL", "signature does not decode to PKCS#1(sha1(region))"
    return "PASS", "resign path signs + self-verifies (synthetic KV)"


def check_live_to_con():
    kv = _synth_kv()
    live = open(EMPTY_LIVE, "rb").read()
    out = stfs.resign(live, kv)
    checks = [
        (out[:4] == b"CON ", "magic flipped to CON"),
        (out[0x22C:0x234] == b"\xFF" * 8, "license[0] set to Unrestricted"),
        (out[0x344:] == live[0x344:], "metadata body (0x344+) untouched"),
        (out[0x04:0x1AC] == kv[0x9C8:0x9C8 + 0x1A8], "console cert copied from KV"),
    ]
    bad = [d for ok, d in checks if not ok]
    return ("FAIL", "; ".join(bad)) if bad else ("PASS", "LIVE->CON conversion correct")


def check_resign_idempotent():
    """Resigning twice yields the same bytes (deterministic)."""
    kv = _synth_kv()
    a = stfs.resign(open(EMPTY_LIVE, "rb").read(), kv)
    b = stfs.resign(a, kv)
    return ("PASS", "second resign is byte-stable") if a == b else \
           ("FAIL", "resign is not idempotent")


def check_god_roundtrip():
    """Build a synthetic XDVDFS volume, write it as a GoD/SVOD package, then read
    it back through the hash-block de-interleaver -- exercises write_god +
    SvodReader + GDF end to end."""
    import gdf
    import god
    payload = {"default.xex": b"XEX2" + b"\x00" * 3000, "readme.txt": b"hello GoD" * 64}
    vol = gdf.build_xdvdfs(payload)
    d = tempfile.mkdtemp()
    _con, data_dir, parts = god.write_god(
        lambda o, l: vol[o:o + l], len(vol), d, open(EMPTY_LIVE, "rb").read(),
        title_id=0x12345678, media_id=0xABCDEF01)
    g = gdf.GDF(god.SvodReader(data_dir), base=0)
    got = dict(g.files())
    for nm, data in payload.items():
        if nm not in got:
            return "FAIL", "missing %s after GoD round-trip" % nm
        if g.read(nm) != data:
            return "FAIL", "content mismatch for %s" % nm
    return "PASS", "disc -> GoD -> read back %d files (%d part)" % (len(payload), parts)


def _xzp():
    for c in (os.path.join(HERE, "..", "cli", "plugins"),
              os.path.join(getattr(sys, "_MEIPASS", HERE), "plugins")):
        if os.path.isfile(os.path.join(c, "xzp.py")):
            sys.path.insert(0, c)
            break
    import xzp
    return xzp


def check_xzp_roundtrip():
    """Pack files into a XUIZ container from a synthesized template, then parse."""
    xzp = _xzp()
    tpl = (b"XUIZ" + struct.pack(">I", 1) + struct.pack(">I", 0) +
           struct.pack(">I", 1) + struct.pack(">I", 0) + struct.pack(">H", 0) + b"\x00")
    d = tempfile.mkdtemp()
    payload = {"logo.png": bytes.fromhex("89504e470d0a1a0a") + b"\x00" * 40,
               "data.bin": b"xyz" * 200}
    for nm, data in payload.items():
        open(os.path.join(d, nm), "wb").write(data)
    out = os.path.join(d, "packed.xzp")
    xzp.build(d, out, tpl)
    r = xzp.parse(out)
    names = {e["name"] for e in r["entries"]}
    if names != set(payload):
        return "FAIL", "entry names %s != %s" % (sorted(names), sorted(payload))
    for e in r["entries"]:
        blob = r["data"][r["base"] + e["offset"]:r["base"] + e["offset"] + e["size"]]
        if blob != payload[e["name"]]:
            return "FAIL", "payload mismatch for %s" % e["name"]
    return "PASS", "pack %d entries -> parse byte-identical" % len(payload)


# Golden LZX-compress digests, captured while byte-exact against XexTool-RE's
# reference encoder (both -c c and -c c with E8). These lock the native encoder
# without needing the reference tool present -- a bit-level regression changes
# the SHA-256 even when the output still decodes correctly.
_LZX_GOLDEN = {
    0:     "9a95916f6a2fc9df558e42b57108f9137b057a564085ac6feefb0cf9565966e8",
    24756: "531d3788463178e8bc85c353931ade542de6219781e3a508515b2cccb5488c82",
}


def _lzx_golden_buf():
    """Deterministic buffer exercising literals, matches, R0/R1/R2 repeats, a
    block split (>0x1800 tokens) and E8 windows."""
    out = bytearray(); x = 7
    prng = bytearray()
    for _ in range(4000):
        x = (1103515245 * x + 12345) & 0x7fffffff
        prng.append((x >> 16) & 0xFF)
    return (bytes(range(256)) * 10 +
            b"The quick brown fox jumps over the lazy dog. " * 180 +
            b"\x00" * 4096 + bytes((i * 37) & 0xFF for i in range(6000)) + bytes(prng))


def check_lzx_roundtrip():
    """LZX encoder end-to-end: compress a mixed buffer, strip the XEX chunk
    framing, LZXD-decode, and require a byte-identical image back; then assert the
    compressed bytes match the golden digests (byte-exact vs the xextool encoder).
    Exercises the native optimal-parse encoder with no reference tool present."""
    import hashlib
    import lzx_encode
    import _lzx
    buf = _lzx_golden_buf()
    stream = lzx_encode.lzx_compress(buf, 0x8000, 0)
    # strip the u16-BE chunk length prefixes to recover the raw LZXD bitstream
    raw = bytearray()
    o = 0
    while o + 2 <= len(stream):
        clen = (stream[o] << 8) | stream[o + 1]
        o += 2
        raw += stream[o:o + clen]
        o += clen
    back = _lzx.lzxd_decompress(bytes(raw), 15, len(buf))
    if back != buf:
        n = min(len(back), len(buf))
        i = 0
        while i < n and back[i] == buf[i]:
            i += 1
        return "FAIL", "round-trip mismatch @%d (len in=%d out=%d)" % (i, len(buf), len(back))
    for e8, want in _LZX_GOLDEN.items():
        got = hashlib.sha256(lzx_encode.lzx_compress(buf, 0x8000, e8)).hexdigest()
        if got != want:
            return "FAIL", "golden mismatch (e8=%d): %s != %s" % (e8, got[:16], want[:16])
    ratio = 100 * len(stream) // len(buf)
    return "PASS", "%d B -> %d B (%d%%); round-trip + 2 golden digests match" % (
        len(buf), len(stream), ratio)


def _build_fatx_image():
    """A tiny valid big-endian (Xbox 360) FATX partition: root has a live file,
    a deleted file (with its data intact), and a subdir with a nested file."""
    import fatx
    SPC = 1
    BPC = SPC * 0x200
    DATA = 0x2000
    img = bytearray(DATA + 8 * BPC)
    struct.pack_into(">4sLLL", img, 0, b"FATX", 0x12345678, SPC, 1)
    ts = ((2015 - 1980) << 25) | (6 << 21) | (15 << 16) | (12 << 11) | (30 << 5) | 10

    def dirent(nl, attr, name, first, size):
        nb = name.encode() + b"\xff" * (42 - len(name))
        return struct.pack(">BB42sLLLLL", nl, attr, nb, first, size, ts, ts, ts)

    def coff(c):
        return DATA + BPC * (c - 1)

    root = (dirent(9, 0x20, "hello.txt", 2, 15) +
            dirent(0xE5, 0x20, "deleted.bin", 3, 20) +          # deleted
            dirent(6, 0x10, "subdir", 4, 0) +
            struct.pack(">B", 0xFF) + b"\x00" * 63)
    img[coff(1):coff(1) + len(root)] = root
    img[coff(2):coff(2) + 15] = b"Hello FATX 360!"
    img[coff(3):coff(3) + 20] = b"SECRET deleted data!"
    sub = dirent(10, 0x20, "nested.dat", 5, 11) + struct.pack(">B", 0xFF) + b"\x00" * 63
    img[coff(4):coff(4) + len(sub)] = sub
    img[coff(5):coff(5) + 11] = b"nested data"
    fat = [0xFFF8, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF] + [0] * (0x800 - 6)
    struct.pack_into(">" + "H" * len(fat), img, 0x1000, *fat)
    return bytes(img)


def check_fatx_write():
    """FATX write: inject a file, mkdir + nested file, delete; verify each
    round-trips byte-identical against the reader."""
    import fatx
    d = tempfile.mkdtemp(); path = os.path.join(d, "w.fatx")
    open(path, "wb").write(_build_fatx_image())
    payload = b"INJECTED CONTENT " * 20
    r = fatx.FatxReader(path); p = r.detect_partitions()[0]
    r.add_file(p, "newfile.bin", payload)
    r.mkdir(p, "newdir"); r.add_file(p, "newdir/nested.txt", b"hello nested")
    r.close()
    r2 = fatx.FatxReader(path); p2 = r2.detect_partitions()[0]
    files = {e.path: e for e in r2._flatten(r2.list_directory(p2))}
    def rd(e):
        out = bytearray()
        for cl in r2.cluster_chain(p2, e.first_cluster):
            out += r2.read_cluster(p2, cl)
        return bytes(out[:e.file_size])
    if "newfile.bin" not in files or rd(files["newfile.bin"]) != payload:
        return "FAIL", "injected file not byte-identical"
    if "newdir/nested.txt" not in files or rd(files["newdir/nested.txt"]) != b"hello nested":
        return "FAIL", "nested dir/file not byte-identical"
    r2.close()
    r3 = fatx.FatxReader(path); p3 = r3.detect_partitions()[0]
    r3.remove(p3, "newfile.bin"); r3.close()
    r4 = fatx.FatxReader(path); p4 = r4.detect_partitions()[0]
    if "newfile.bin" in {e.path for e in r4._flatten(r4.list_directory(p4))}:
        return "FAIL", "deleted file still listed"
    return "PASS", "inject + mkdir + nested + delete all round-trip"


def check_fatx_roundtrip():
    """FATX reader + deleted-file recovery: build a synthetic Xbox 360 (big-
    endian) partition, auto-detect its geometry, list the live tree, and recover
    the deleted file's contents. Locks the dual-endian dirent/timestamp layout
    and the orphan-recovery path (facts per FATX-Tools, MIT)."""
    import fatx
    d = tempfile.mkdtemp()
    path = os.path.join(d, "test.fatx")
    open(path, "wb").write(_build_fatx_image())
    r = fatx.FatxReader(path)
    parts = r.detect_partitions()
    if not parts or parts[0].endian != ">" or parts[0].epoch != 1980:
        return "FAIL", "endianness auto-detect failed (%s)" % (parts and parts[0].endian)
    p = parts[0]
    flat = {e.path: e for e in r._flatten(r.list_directory(p))}
    if set(flat) != {"hello.txt", "subdir", "subdir/nested.dat"}:
        return "FAIL", "live listing wrong: %s" % sorted(flat)
    if flat["hello.txt"].modified is None or flat["hello.txt"].modified.year != 2015:
        return "FAIL", "timestamp decode wrong"
    out = os.path.join(d, "rec")
    r.recover_all(p, out)
    rec = os.path.join(out, "deleted.bin")
    if not os.path.exists(rec) or open(rec, "rb").read() != b"SECRET deleted data!":
        return "FAIL", "deleted-file recovery failed"
    return "PASS", "mount+list+recover (X360-BE, 3 live + 1 recovered deleted)"


def check_gpd_edit():
    """XDBF/GPD write side: build a synthetic profile with two locked
    achievements, unlock one, rebuild, and confirm the flag + FILETIME persist
    and the other records survive untouched."""
    import xdbf

    def rec(ach_id, score, name):
        strs = (name.encode("utf-16-be") + b"\x00\x00" +
                "Unlocked".encode("utf-16-be") + b"\x00\x00" +
                "Locked".encode("utf-16-be") + b"\x00\x00")
        body = struct.pack(">IIIIIQ", 0, ach_id, 5, score, 0, 0) + strs
        return struct.pack(">I", len(body)) + body[4:]

    ents = [(1, 1, rec(1, 10, "First Blood")), (1, 2, rec(2, 25, "Marathon"))]
    cap, free_cap = len(ents), 1
    heap, table = b"", b""
    for ns, eid, p in ents:
        table += struct.pack(">HQII", ns, eid, len(heap), len(p))
        heap += p
    gpd = (b"XDBF" + struct.pack(">IIIII", 0x10000, cap, len(ents), free_cap, 1) +
           table + struct.pack(">II", len(heap), 0) + heap)
    x = xdbf.XDBF(gpd)
    x.unlock_achievement(1, True)
    y = xdbf.XDBF(x.build())
    a = {r["id"]: r for r in y.achievements()}
    if not a[1]["unlocked"] or a[2]["unlocked"]:
        return "FAIL", "unlock flag wrong (1=%s 2=%s)" % (a[1]["unlocked"], a[2]["unlocked"])
    if struct.unpack_from(">Q", y.payload(1, 1), 0x14)[0] != xdbf.XDBF.UNLOCK_FILETIME:
        return "FAIL", "unlock FILETIME not stamped"
    if a[1]["name"] != "First Blood" or a[2]["score"] != 25:
        return "FAIL", "records corrupted on rebuild"
    return "PASS", "unlock 1/2 achievements + rebuild (names/scores preserved)"


def check_nand_ecc():
    """NAND spare/ECC (nandpro/xeBuild class): add ECC to a raw image, verify
    every page, strip back to the exact raw data, and confirm a corrupted page
    is detected and repaired."""
    import nand_tool as N

    def prng(n, s=7):
        o = bytearray()
        x = s
        for _ in range(n):
            x = (1103515245 * x + 12345) & 0x7fffffff
            o.append((x >> 16) & 0xff)
        return bytes(o)

    raw = prng(8 * 512)
    img = N.add_spare(raw)
    if len(img) != 8 * 528 or N.verify_ecc(img):
        return "FAIL", "ecc build/verify failed"
    if N.strip_spare(img) != raw:
        return "FAIL", "unecc did not round-trip"
    bad = bytearray(img)
    bad[3 * 528 + 10] ^= 0xFF
    if N.verify_ecc(bytes(bad)) != [3]:
        return "FAIL", "corruption not detected on the right page"
    if N.verify_ecc(N.fix_all_ecc(bytes(bad))):
        return "FAIL", "fix did not repair ECC"
    return "PASS", "ecc/verify/unecc round-trip + corrupt-detect + fix (8 pages)"


def check_rte_wire():
    """Real-time-editing peek/poke wire logic (no console needed): typed big-
    endian pack/unpack round-trips and the exact memory_read/write ops a poke
    and peek issue against a fake console."""
    import jrpc2

    if jrpc2.pack_typed("u32", "0xDEADBEEF") != b"\xde\xad\xbe\xef":
        return "FAIL", "u32 pack not big-endian"
    if jrpc2.unpack_typed("u32", b"\xde\xad\xbe\xef") != 0xDEADBEEF:
        return "FAIL", "u32 unpack wrong"
    if jrpc2.pack_typed("float", "1.5") != b"\x3f\xc0\x00\x00":
        return "FAIL", "float pack wrong"

    class Fake:
        def __init__(self):
            self.mem = {}
            self.log = []

        def memory_write(self, a, d):
            self.log.append(("W", a, d))
            self.mem[a] = d

        def memory_read(self, a, n):
            return self.mem.get(a, b"\x00" * n)

    f = Fake()
    jrpc2.poke(f, 0x82000000, "u32", "0x1337")
    if f.log[-1] != ("W", 0x82000000, b"\x00\x00\x13\x37"):
        return "FAIL", "poke issued wrong write %r" % (f.log[-1],)
    if jrpc2.peek(f, 0x82000000, "u32") != 0x1337:
        return "FAIL", "peek read back wrong value"
    return "PASS", "typed BE pack/unpack + peek/poke wire ops (u32/float)"


def check_disc_stealth():
    """Disc stealth cross-checks (abgx360-class): a synthetic matched SS/DMI/PFI
    passes, a media-ID mismatch between DMI and SS fails, and an SS whose two
    XGD-version indicators disagree is caught."""
    import stealth as S

    mid = bytes.fromhex("0123456789ABCDEF" "FEDCBA9876543210")

    def dmi(m):
        d = bytearray(0x800)
        d[0] = 0x02
        d[0x18] = 2
        d[0x20:0x30] = m
        d[0x7E4:0x7F0] = S.DMI_MAGIC
        return bytes(d)

    def ss(m, xgd=2, sec=None):
        d = bytearray(0x800)
        d[0x0D:0x10] = {1: bytes([0x20, 0x33, 0xAF]), 2: bytes([0x20, 0x33, 0x9F]),
                        3: bytes([0x23, 0x8E, 0x0F])}[xgd]
        d[0x4BA] = sec if sec is not None else (1 if xgd == 1 else 2)
        d[0x460:0x470] = m
        return bytes(d)

    def passed(checks):
        return all(ok for _n, ok, _d, crit in checks if crit)

    good = S.verify(S.parse_ss(ss(mid)), S.parse_dmi(dmi(mid)), None, mid)
    if not passed(good):
        return "FAIL", "matched disc did not pass: %s" % [n for n, ok, _d, c in good if c and not ok]
    wrong = bytes(16)
    bad = S.verify(S.parse_ss(ss(wrong)), S.parse_dmi(dmi(mid)), None, mid)
    if passed(bad) or "Media ID: DMI == SS" not in [n for n, ok, _d, c in bad if not ok]:
        return "FAIL", "media-ID mismatch not caught"
    inc = S.verify(S.parse_ss(ss(mid, 3, sec=1)), S.parse_dmi(dmi(mid)), None, mid)
    if passed(inc):
        return "FAIL", "SS version inconsistency not caught"
    return "PASS", "matched disc passes; media-ID mismatch + XGD inconsistency caught"


def check_texture():
    """Xbox 360 texture untiling: the tiled-address map must be bijective and
    tile()/untile() must be exact inverses across sizes and DXT formats, and the
    DDS wrapper must be a valid 128-byte header."""
    import texture as T

    for fmt in ("dxt1", "dxt5"):
        bb = T.FORMATS[fmt][0]
        for w, h in [(64, 64), (128, 256), (32, 32), (256, 128)]:
            bw, bh = T._blocks(w, h)
            linear = bytes((i * 7 + 3) & 0xff for i in range(bw * bh * bb))
            if T.untile(T.tile(linear, w, h, fmt), w, h, fmt) != linear:
                return "FAIL", "%s %dx%d tile/untile not inverse" % (fmt, w, h)
    bw, bh = T._blocks(256, 256)
    offs = [T.tiled_offset_2d(x, y, bw, 4) for y in range(bh) for x in range(bw)]
    if len(set(offs)) != len(offs):
        return "FAIL", "tiled address map not bijective"
    hdr = T.dds_header(128, 128, "dxt5")
    if hdr[:4] != b"DDS " or len(hdr) != 128 or hdr[84:88] != b"DXT5":
        return "FAIL", "DDS header malformed"
    if T.swap16(T.swap16(bytes(range(256)))) != bytes(range(256)):
        return "FAIL", "swap16 not involutive"
    return "PASS", "tile/untile inverse (dxt1/5, 4 sizes) + bijective + valid DDS"


def check_xpr():
    """XPR resource bundle: build an XPR0 around a known DXT1 surface, read it
    back, confirm the format/size/pixels + DDS wrap, and parse an XPR2 header."""
    import xpr
    ok, msg = xpr._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_xwb():
    """XACT wave bank: build a synthetic big-endian (Xbox 360) XWB with a PCM
    and an XMA entry, parse it, and confirm the Format bit-field decodes, the
    XMA stream extracts byte-exact, and PCM wraps into a valid little-endian WAV."""
    import xwb as X

    def fmt(codec, ch, rate):
        return codec | (ch << 2) | (rate << 5)

    pcm, xma = bytes(range(32)), bytes([0xAA] * 16)
    wavedata = pcm + xma
    h = bytearray(b"WBND" + struct.pack(">II", 46, 1))
    segpos = len(h)
    h += b"\x00" * 40
    seg0 = len(h)
    wbd = struct.pack(">II", 0, 2) + b"TestBank".ljust(64, b"\x00") + struct.pack(">III", 24, 0, 4)
    h += wbd
    seg1 = len(h)
    h += struct.pack(">IIIIII", 0, fmt(0, 2, 44100), 0, 32, 0, 0)
    h += struct.pack(">IIIIII", 0, fmt(1, 2, 48000), 32, 16, 0, 0)
    seg2 = seg3 = seg4 = len(h)
    h += wavedata
    struct.pack_into(">10I", h, segpos, seg0, len(wbd), seg1, 48, seg2, 0, seg3, 0, seg4, len(wavedata))

    x = X.XWB(bytes(h))
    if x.en != ">" or x.version != 46 or x.entry_count != 2:
        return "FAIL", "header parse wrong"
    e0, e1 = x.entries
    if e0["codec_name"] != "PCM" or e0["rate"] != 44100 or e0["channels"] != 2:
        return "FAIL", "PCM format decode wrong: %s" % e0
    if e1["codec_name"] != "XMA" or e1["rate"] != 48000:
        return "FAIL", "XMA format decode wrong: %s" % e1
    if x.stream(e1) != xma:
        return "FAIL", "XMA stream not extracted byte-exact"
    wav = X.pcm_to_wav(pcm, 2, 44100, 16)
    if wav[:4] != b"RIFF" or wav[8:12] != b"WAVE" or wav[36:40] != b"data":
        return "FAIL", "WAV header malformed"
    if wav[44:] != bytes(pcm[i ^ 1] for i in range(len(pcm))):
        return "FAIL", "PCM not byte-swapped to LE"
    return "PASS", "parse BE bank + format decode + XMA extract + valid WAV"


def check_xur():
    """XUR (compiled XUI) container: build a synthetic XUR5 with a STRN string
    table, parse the big-endian header + section table, and recover the strings
    (element names / texture paths / text) byte-for-byte."""
    import xur as X

    strs = ["XuiCanvas", "file://game:/skin.png", "Press A to continue", ""]
    strn = X.build_strn(strs)
    count = 1
    data_base = 20 + count * 12
    blob = (struct.pack(">III", X.MAGIC, 5, 0) +
            struct.pack(">HIH", 0x0C, data_base + len(strn), count) +
            struct.pack(">III", X.STRN, 0, len(strn)) + strn)
    x = X.XUR(blob)
    if x.version != 5 or x.section_count != 1:
        return "FAIL", "header parse wrong"
    if x.strings() != strs:
        return "FAIL", "STRN strings not recovered: %s" % x.strings()
    # writer: unchanged re-emit is byte-identical; an edit round-trips
    if X.set_strings(blob, strs) != blob:
        return "FAIL", "faithful re-emit not byte-identical"
    edited = X.set_string(blob, 2, "Press START instead")
    back = X.XUR(edited).strings()
    if back != ["XuiCanvas", "file://game:/skin.png", "Press START instead", ""]:
        return "FAIL", "string edit did not round-trip: %s" % back
    return "PASS", "parse + faithful re-emit + string edit round-trip"


def check_ppcasm():
    """Native PowerPC assembler vs hand-checked golden encodings (self-contained,
    no capstone needed). Locks the injection-critical instruction encoders."""
    import ppcasm
    gold = {
        "nop": 0x60000000, "blr": 0x4E800020, "bctr": 0x4E800420,
        "bctrl": 0x4E800421, "li r3, 0": 0x38600000, "li r0, 1": 0x38000001,
        "lis r11, 0x8200": 0x3D608200, "mtlr r0": 0x7C0803A6,
        "mflr r0": 0x7C0802A6, "mr r3, r31": 0x7FE3FB78,
        "add r3, r4, r5": 0x7C642A14, "stw r31, 8(r1)": 0x93E10008,
        "lwz r3, 0(r4)": 0x80640000, "ori r3, r3, 1": 0x60630001,
        "cmpwi r3, 0": 0x2C030000, "addi r1, r1, -32": 0x3821FFE0,
    }
    for asm, exp in gold.items():
        got = ppcasm.assemble_one(asm)
        if got != exp:
            return "FAIL", "%s -> 0x%08X != 0x%08X" % (asm, got, exp)
    if ppcasm.assemble_one("b 0x82001000", 0x82000000) != 0x48001000:
        return "FAIL", "relative branch encoding wrong"
    return "PASS", "%d golden encodings + relative branch" % len(gold)


def check_shader():
    """Xenos shader disassembler: encode a synthetic microcode program (an exec
    running one ALU + one fetch) and confirm the control-flow, ALU vector+scalar
    opcodes, and fetch decode back correctly."""
    import shader

    def cf(op, address=0, count=0, sequence=0):
        return (address & 0xFFF) | ((count & 7) << 12) | ((sequence & 0xFFF) << 16) | ((op & 0xF) << 44)
    cf0 = cf(1, address=1, count=2, sequence=0b0100)
    cf1 = cf(2)
    w0 = cf0 & 0xFFFFFFFF
    w1 = ((cf0 >> 32) & 0xFFFF) | ((cf1 & 0xFFFF) << 16)
    w2 = (cf1 >> 16) & 0xFFFFFFFF
    alu = [(3 & 0x3F) | (0b1111 << 16), 0, (2 << 16) | (1 << 8) | (1 << 24)]
    fetch = [0 | (2 << 5) | (5 << 12), 0, 0]
    dwords = [w0, w1, w2] + alu + fetch
    data = b"".join(struct.pack(">I", x & 0xFFFFFFFF) for x in dwords)
    lines, cfl = shader.disassemble(data, little=False)
    d0 = shader.decode_cf(cf0)
    a = shader.decode_alu(*alu)
    f = shader.decode_fetch(*fetch)
    if not (d0["mn"] == "exec" and d0["address"] == 1 and d0["count"] == 2):
        return "FAIL", "exec CF decode wrong"
    if a["vector"] != "mul" or a["scalar"] != "adds" or a["vector_dest"] != 3:
        return "FAIL", "ALU decode wrong (%s/%s)" % (a["vector"], a["scalar"])
    if f["mn"] != "vfetch_full" or f["dst_reg"] != 5 or f["src_reg"] != 2:
        return "FAIL", "fetch decode wrong"
    return "PASS", "CF + ALU (mul+adds) + vertex-fetch decode correctly"


def check_og_xbox():
    """Original Xbox: XBE header/cert/entry-decrypt on a synthetic XBE, and the
    EEPROM keyed-hash crypto — custom-IV SHA-1 matches hashlib, and all four
    version IVs (1.0 / 1.1-1.5 / 1.6 / debug) detect + decrypt a round-tripped
    security block."""
    import hashlib
    import xbe as XBE
    import eeprom as EE
    std = (0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0)
    for m in (b"", b"abc", bytes(range(200))):
        if EE._sha1_iv(std, m) != hashlib.sha1(m).digest():
            return "FAIL", "custom-IV SHA-1 != hashlib"
    for ver in EE.EEPROM_KEYS:
        hdd = bytes(range(0x10, 0x20))
        block = b"\xC3" * 8 + hdd + struct.pack("<I", 1)
        ee = bytearray(256)
        ee[0x00:0x30] = EE.encrypt_security(block, ver)
        struct.pack_into("<I", ee, 0x58, 0x00014000)
        d = EE.parse(bytes(ee))
        if d["version"] != ver or d["hdd_key"] != hdd.hex().upper():
            return "FAIL", "EEPROM %s did not round-trip" % ver
    img = bytearray(0x600)
    img[0:4] = b"XBEH"
    B = 0x10000
    struct.pack_into("<I", img, 0x104, B)
    struct.pack_into("<I", img, 0x10C, 0x2000)
    struct.pack_into("<I", img, 0x118, B + 0x200)
    struct.pack_into("<I", img, 0x11C, 0)
    struct.pack_into("<I", img, 0x120, B + 0x300)
    struct.pack_into("<I", img, 0x128, 0x11030 ^ 0xA8FC57AB)
    struct.pack_into("<I", img, 0x148, 0x11500 ^ 0x5B6D40B6)
    struct.pack_into("<I", img, 0x200 + 0x08, 0x4D530037)
    img[0x200 + 0x0C:0x200 + 0x0C + 8] = "Halo".encode("utf-16-le")
    struct.pack_into("<I", img, 0x200 + 0xA0, 0x1)
    x = XBE.XBE(bytes(img))
    if x.title_name != "Halo" or x.entry_point() != (0x11030, "retail"):
        return "FAIL", "XBE parse/entry-decrypt wrong (%r %s)" % (x.title_name, x.entry_point())
    return "PASS", "XBE cert+entry decrypt + EEPROM 4-version keyed-hash round-trip"


def check_bindiff():
    """Binary diff/patch round-trips exactly for a localized edit, a non-aligned
    insertion, and produces a tiny patch for the common (mostly-unchanged) case."""
    import bindiff
    src = bytes((i * 7) & 0xFF for i in range(100000))
    dst = bytearray(src); dst[50000:50010] = b"MODIFIED!!!"; dst = bytes(dst)
    p = bindiff.make_patch(src, dst)
    if bindiff.apply_patch(src, p) != dst:
        return "FAIL", "localized-edit round-trip mismatch"
    if len(p) > 200:
        return "FAIL", "patch too large for a localized edit (%d bytes)" % len(p)
    dst2 = src[:1000] + b"INSERTED_" * 4 + src[1000:]
    if bindiff.apply_patch(src, bindiff.make_patch(src, dst2)) != dst2:
        return "FAIL", "insertion round-trip mismatch"
    return "PASS", "round-trips exact; %d-byte patch for a localized 100 KB edit" % len(p)


def check_report():
    """Unified analysis report: timestamp/size formatting and format-dispatch
    (identifies a FATX blob and produces a markdown report skeleton)."""
    import report
    ok, msg = report._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_identify():
    """Format auto-identifier: magic + structural detection for the core Xbox/360
    formats, STFS content-type decode, COFF machine word, and the XGD media
    offset."""
    import identify
    ok, msg = identify._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_view():
    """In-app viewers: hex dump (+identical-row collapse), byte diff (locates the
    first changed byte + marks columns), and a PowerPC disassembly view."""
    import view
    ok, msg = view._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_smc():
    """SMC 'BuNy' cipher is a clean inverse pair (encrypt∘decrypt = identity both
    ways) and the NAND flash-header SMC fields locate the firmware."""
    import smc
    data = bytes((i * 131 + 7) & 0xFF for i in range(4096))
    if smc.smc_encrypt(smc.smc_decrypt(data)) != data:
        return "FAIL", "decrypt->encrypt not identity"
    plain = bytes(range(256)) * 4
    if smc.smc_decrypt(smc.smc_encrypt(plain)) != plain:
        return "FAIL", "encrypt->decrypt not identity"
    nand = bytearray(0x10000)
    nand[0:2] = b"\xff\x4f"
    fw = bytes((i * 7) & 0xFF for i in range(0x2000))
    struct.pack_into(">I", nand, 0x78, len(fw))
    struct.pack_into(">I", nand, 0x7C, 0x8000)
    nand[0x8000:0x8000 + len(fw)] = fw
    got, off, ln = smc.extract_from_nand(bytes(nand))
    if got != fw or off != 0x8000:
        return "FAIL", "NAND SMC extraction wrong"
    return "PASS", "cipher inverse both ways + NAND header extraction"


def check_savegame():
    """Save-game toolkit: build a synthetic STFS save, poke a value in the inner
    blob, rebuild + re-read, and confirm the edit landed while the title id /
    content type / other bytes are preserved."""
    import savegame
    d = tempfile.mkdtemp()
    inner = bytearray(0x400)
    inner[0x20:0x24] = (1000).to_bytes(4, "big")
    open(os.path.join(d, "savedata.bin"), "wb").write(inner)
    save = os.path.join(d, "save.con")
    stfs.build(d, save, open(EMPTY_LIVE, "rb").read())
    before = savegame.info(open(save, "rb").read())
    out = os.path.join(d, "edited.con")
    savegame.patch(save, "savedata.bin", 0x20, (999999).to_bytes(4, "big"), out)
    s2 = stfs.STFS(open(out, "rb").read())
    got = s2.read_file("savedata.bin")
    after = savegame.info(open(out, "rb").read())
    if int.from_bytes(got[0x20:0x24], "big") != 999999:
        return "FAIL", "patched value not read back"
    if after["content_type"] != before["content_type"] or after["title_id"] != before["title_id"]:
        return "FAIL", "metadata not preserved through rebuild"
    if got[:0x20] != bytes(0x20) or got[0x24:] != bytes(0x400 - 0x24):
        return "FAIL", "unrelated bytes changed"
    return "PASS", "edit inner blob + rebuild; metadata + other bytes preserved"


def check_fuzz():
    """Parser fuzzer: a fragile parser (unchecked index) must be flagged as
    crashing, and a robust one (rejects all input with ValueError) must be
    reported clean."""
    import fuzz
    def fragile(d):
        return d[100000]
    def robust(d):
        raise ValueError("bad input")
    seed = bytes(200)
    if not fuzz.fuzz(fragile, seed, iterations=200, seed_value=1):
        return "FAIL", "fuzzer missed crashes in a fragile parser"
    if fuzz.fuzz(robust, seed, iterations=200, seed_value=1):
        return "FAIL", "fuzzer flagged a handled ValueError as a crash"
    return "PASS", "detects unhandled crashes, ignores handled errors"


def check_xeniapatch():
    """Xenia .patch.toml parser + value encoders: parse a document exercising
    every op type and confirm the big-endian encodings and enabled-filter.
    Facts reimplemented clean-room from the Xenia patch spec."""
    import xeniapatch as xp
    doc = ('title_id = "584108A8"\n'
           'media_id = ["12345678"]\n\n'
           '[[patch]]\nname = "On"\nis_enabled = true\n'
           '  [[patch.be8]]\n  address = 0x82000000\n  value = 0xAB\n'
           '  [[patch.be16]]\n  address = 0x82000002\n  value = 0x1234\n'
           '  [[patch.be32]]\n  address = 0x82000004\n  value = 0xDEADBEEF\n'
           '  [[patch.be64]]\n  address = 0x82000008\n  value = 0x1122334455667788\n'
           '  [[patch.f32]]\n  address = 0x82000010\n  value = 1.5\n'
           '  [[patch.array]]\n  address = 0x82000014\n  value = "001122"\n'
           '  [[patch.u16string]]\n  address = 0x82000018\n  value = "Hi"\n\n'
           '[[patch]]\nname = "Off"\nis_enabled = false\n'
           '  [[patch.be32]]\n  address = 0x82000100\n  value = 0\n')
    pf = xp.parse(doc)
    if pf.title_id != "584108A8" or pf.media_ids != ["12345678"] or len(pf.enabled()) != 1:
        return "FAIL", "header/enabled parse wrong (%s, %d)" % (pf.title_id, len(pf.enabled()))
    ops = {o.op: o.blob for o in pf.patches[0].ops}
    want = {"be8": b"\xab", "be16": b"\x12\x34", "be32": b"\xde\xad\xbe\xef",
            "be64": bytes.fromhex("1122334455667788"), "f32": struct.pack(">f", 1.5),
            "array": b"\x00\x11\x22", "u16string": "Hi".encode("utf-16-be")}
    for k, v in want.items():
        if ops.get(k) != v:
            return "FAIL", "%s encoding %r != %r" % (k, ops.get(k), v)
    return "PASS", "2 patches (1 enabled); all 7 op encodings big-endian correct"


def check_memscan():
    """Memory scanner / pointer / cave engine over a synthetic memory: scan
    for a value, narrow by dec + eq to the single address; find an isolated
    code cave; resolve a static pointer chain."""
    import memscan
    BASE = 0x40000000
    mem = bytearray(b"".join(struct.pack(">I", 0x7C0802A6) for _ in range(0x4000)))
    def wr(off, v):
        struct.pack_into(">I", mem, off, v & 0xFFFFFFFF)
    def read(addr, size):
        o = addr - BASE
        return bytes(mem[o:o + size]) if 0 <= o and o + size <= len(mem) else b""
    wr(0x4000, 100); wr(0x2000, 100); wr(0x6000, 100)
    sc = memscan.Scanner(read)
    sc.first_scan(BASE, BASE + 0x10000, 100, "u32")
    wr(0x4000, 80); wr(0x2000, 55); wr(0x6000, 100)
    sc.next_scan("dec"); sc.next_scan("eq", 80)
    if list(sc.hits) != [0x40004000]:
        return "FAIL", "scan did not narrow to the target: %s" % list(sc.hits)
    for i in range(40):
        wr(0x8000 + i * 4, memscan.PPC_NOP)
    if not any(a == BASE + 0x8000 and ln == 160 for a, ln in sc.find_caves(BASE, BASE + 0x10000, 0x40)):
        return "FAIL", "isolated code cave not found"
    wr(0x0100, BASE + 0x4000 - 0x10)
    paths = sc.pointer_paths(BASE + 0x4000, BASE, BASE + 0x1000, max_depth=1)
    if not any(b == BASE + 0x100 and offs == [0x10] for b, offs in paths):
        return "FAIL", "pointer chain not resolved"
    return "PASS", "scan->dec->eq narrows to target; cave + pointer chain found"


def check_xinput():
    """Controller-automation command builders: XInput button masks + the XBDM
    autoinput/setpacket/queuepackets wire strings a macro compiles to."""
    import xinput as xi
    if xi.buttons_mask("A B") != 0x3000 or xi.buttons_mask("X,Y,START") != 0xC010:
        return "FAIL", "button mask wrong (A+B=%X)" % xi.buttons_mask("A B")
    cmds = xi.build_macro(0, "press A 200\nhold LB RB\ntrigger R 255\nwait 50\nrelease\n")
    if "buttons=0x1000" not in cmds[0] or "queuepackets count=1 duration=200" not in cmds[1]:
        return "FAIL", "press macro compiled wrong: %s" % cmds[:2]
    if not any("buttons=0x0300" in c for c in cmds) or not any("rt=0xFF" in c for c in cmds):
        return "FAIL", "hold/trigger not in compiled macro"
    if not any("queuepackets count=1 duration=50" in c for c in cmds):
        return "FAIL", "wait not compiled"
    return "PASS", "button masks + autoinput macro wire strings correct (%d cmds)" % len(cmds)


def check_install2god():
    """NXE-installed STFS -> GoD/LIVE header transform (NXE2GOD MakeGOD): apply
    to a synthetic STFS header and confirm every retype byte + the recomputed
    header hash SHA1(buf[0x344:0x344+0xACBC]) -> 0x32C."""
    import god
    import hashlib
    src = bytearray(0xB000)
    src[0:4] = b"CON "
    for i in range(0x344, 0x344 + 0xACBC):        # deterministic body
        src[i] = (i * 31) & 0xFF
    src[0x344:0x348] = b"\x00\x00\x40\x00"        # installed content type (0x4000)
    if not god.is_nxe_install(bytes(src)):
        return "FAIL", "NXE-install detection failed"
    out = god.install_to_god(bytes(src))
    if out[0:4] != b"LIVE":
        return "FAIL", "magic not retyped to LIVE"
    checks = (all(out[i] == 0xFF for i in range(0x22C, 0x234)) and
              out[0x342] == 0xAD and out[0x343] == 0x0E and out[0x346] == 0x70 and
              out[0x379] == 0x24 and out[0x37C] == 0x11 and
              all(out[i] == 0 for i in range(0x3FD, 0x412)))
    if not checks:
        return "FAIL", "retype byte patches wrong"
    want = hashlib.sha1(out[0x344:0x344 + 0xACBC]).digest()
    if out[0x32C:0x32C + 20] != want:
        return "FAIL", "header hash @0x32C mismatch"
    return "PASS", "retype to LIVE/0x7000 + header hash over 0xACBC bytes correct"


def check_databreak():
    """XBDM hardware data-breakpoint wire strings ('what accesses this addr'):
    the read/write/readwrite/execute command forms and code-breakpoint form."""
    import xbdm
    if xbdm.cmd_data_break(0x82000000, "write", 4) != "break write=0x82000000 size=4":
        return "FAIL", "write bp wrong: %s" % xbdm.cmd_data_break(0x82000000, "write", 4)
    if xbdm.cmd_data_break(0x830100AB, "readwrite", 8, clear=True) != "break readwrite=0x830100AB size=8 clear":
        return "FAIL", "readwrite/clear bp wrong"
    if xbdm.cmd_code_break(0x82012340) != "break addr=0x82012340":
        return "FAIL", "code bp wrong"
    try:
        xbdm.cmd_data_break(0, "bogus")
        return "FAIL", "invalid access type accepted"
    except ValueError:
        pass
    return "PASS", "read/write/readwrite/execute + code breakpoint wire strings correct"


def check_flashplan():
    """RGH/JTAG flash-planner mappings: RGH-type -> xeBuild image type,
    motherboard -> secure-boot block, and CPLD IDCODE -> glitch chip.
    Facts reimplemented clean-room from Genexis."""
    import flashplan as fp
    for hack, want in [("RGH 1", "glitch"), ("RGH 2", "glitch2"), ("RGH 3", "glitch3"),
                       ("R-JTAG", "jtag")]:
        if fp.image_type("FreeBoot", hack) != want:
            return "FAIL", "image_type(%s) != %s" % (hack, want)
    if fp.secure_boot_block("Corona 4G") != "KSB" or fp.secure_boot_block("Jasper") != "PSB" \
            or fp.secure_boot_block("Falcon") != "XSB":
        return "FAIL", "secure-boot block mapping wrong"
    if not fp.cpld_part(0x06E5E093).startswith("XC2C64A"):
        return "FAIL", "CPLD IDCODE map wrong"
    return "PASS", "image-type + secure-boot-block + CPLD IDCODE mappings correct"


def check_media():
    """XMA RIFF-XMA2 wrap (native) produces a valid container, and the Bink
    header parser decodes a synthetic .bik header."""
    import xma, bink
    riff = xma.wrap_xma2(bytes(5000), 2, 44100)
    if riff[:4] != b"RIFF" or riff[8:12] != b"WAVE":
        return "FAIL", "RIFF-XMA2 container malformed"
    if struct.unpack_from("<H", riff, 20)[0] != 0x166:
        return "FAIL", "fmt tag is not WAVE_FORMAT_XMA2"
    d = tempfile.mkdtemp(); bp = os.path.join(d, "t.bik")
    open(bp, "wb").write(b"BIKi" + struct.pack("<10I", 1000000, 300, 8192, 300, 1280, 720, 30, 1, 0, 1) + bytes(20))
    h = bink.read_header(bp)
    if h["width"] != 1280 or h["height"] != 720 or h["fps"] != 30.0 or h["audio_tracks"] != 1:
        return "FAIL", "Bink header parse wrong: %s" % h
    return "PASS", "XMA RIFF-XMA2 wrap valid + Bink header decoded"


def check_il_capture():
    """MSVC IL-bundle capture (the c2-rs /Bd /d2nop technique) via ppccc.
    Skips when the XDK PowerPC cl.exe is unavailable."""
    import ppc_cc
    if not ppc_cc.available():
        return "SKIP", "XDK PowerPC cl.exe not configured"
    r = ppc_cc.capture_il("int f(int a){return a+1;}", flags="/c /O2")
    got = set(r["bundle"])
    if not {"ex", "gl", "sy"}.issubset(got):
        return "FAIL", "captured only %s of the IL bundle" % sorted(got)
    return "PASS", "captured MSVC IL bundle (%s)" % ",".join(sorted(got))


def check_tu():
    """Title-Update manager: input detection (raw .xexp vs STFS TU package)
    and that a raw XEX2 patch is returned verbatim by the patch extractor.
    The delta-apply itself is covered by the delta-patch corpus."""
    import tu
    xexp = bytes([0x58, 0x45, 0x58, 0x32, 0, 0, 0, 0x40])  # "XEX2" + patch flag
    if not (tu._is_xexp(xexp) and not tu._is_stfs(xexp)):
        return "FAIL", "xexp detection wrong"
    if not (tu._is_stfs(b"CON ") and tu._is_stfs(b"LIVE") and tu._is_stfs(b"PIRS")):
        return "FAIL", "STFS magic detection wrong"
    d = tempfile.mkdtemp()
    fp = os.path.join(d, "u.xexp")
    open(fp, "wb").write(xexp + bytes(32))
    patch, inner = tu._patch_bytes_from(fp)
    if inner is not None or patch[:4] != b"XEX2":
        return "FAIL", "raw .xexp not passed through"
    return "PASS", "TU/DLC input detection + patch passthrough correct"


def check_ppctrace():
    """PPC interpreter/tracer integer core: assemble and run a sum-1..10 loop
    (with a multiply and a store/load) and confirm the computed results."""
    import ppctrace
    return ("PASS", "integer loop (sum=55) + float compute (2.5*4+1.5=11.5) interpreted") if         ppctrace.selftest() else ("FAIL", "interpreter produced wrong result")


def check_xexdiff():
    """Address-invariant XEX function diff: normalizing two copies of a routine
    placed at different addresses (with different call targets + data addresses)
    must yield identical fingerprints, while a real opcode change must not; plus
    the fingerprint set-diff bookkeeping."""
    import xexdiff
    ok, msg = xexdiff._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_xexrun():
    """Minimal XEX runner: load a program into guest memory, run from its entry,
    intercept a call into an import thunk (HLE-stub + log), and confirm the
    stubbed return value flows through and the program exits cleanly."""
    import xexrun
    ok, msg = xexrun._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_pipeline():
    """Pipeline runner: parse a script with variables, {VAR} substitution and
    the ignore-error prefix into the correct step list."""
    import pipeline as pl
    script = chr(10).join(["set X = foo.xex", "vmx128 0x1", "-callgraph {X} --hot 3"])
    steps, variables = pl.parse_script(script)
    if steps[0] != (["vmx128", "0x1"], False):
        return "FAIL", "step 0 wrong: %s" % (steps[0],)
    if steps[1] != (["callgraph", "foo.xex", "--hot", "3"], True):
        return "FAIL", "substitution/ignore wrong: %s" % (steps[1],)
    return "PASS", "script parse + {VAR} substitution + ignore-flag correct"


def check_demangle():
    """MSVC C++ demangler: decode known decorated names to readable C++."""
    import msvc_demangle as dm
    cases = {
        "?foo@bar@@YAHXZ": "int __cdecl bar::foo(void)",
        "?x@@3HA": "int x",
        "?g@@YAXPAH@Z": "void __cdecl g(int *)",
        "?Update@Player@Game@@QAEXH@Z": "public: void __thiscall Game::Player::Update(int)",
    }
    for m, want in cases.items():
        if dm.demangle(m) != want:
            return "FAIL", "%s -> %s (want %s)" % (m, dm.demangle(m), want)
    return "PASS", "4 MSVC decorated names demangled to correct C++"


def check_c2model():
    """c2 codegen predictor: the ALLOC (register) + ORDER (store-order) models
    reproduce every killer cell from the RE'd specs (ALLOC.md/ORDER.md)."""
    import c2model
    alloc = [([1,2,3,1],{1:"r11",2:"r10",3:"r9"}),
             ([1,2,3,2,1],{1:"r10",2:"r11",3:"r9"}),
             ([1,2,1,2],{1:"r10",2:"r11"}),
             ([1,1,2,2,2],{1:"r10",2:"r11"})]
    order = [([1,2,2],[1,0,2]),([1,2,1,2],[0,1,2,3]),([1,1,2,2,2],[2,0,1,3,4])]
    for sv,want in alloc:
        if c2model.allocate(sv)!=want:
            return "FAIL","ALLOC %s -> %s != %s"%(sv,c2model.allocate(sv),want)
    for sv,want in order:
        if c2model.store_order(sv)!=want:
            return "FAIL","ORDER %s -> %s != %s"%(sv,c2model.store_order(sv),want)
    if c2model.allocate([1,2,3,4,5]) is not None:
        return "FAIL","should refuse >3 producers"
    return "PASS","ALLOC + ORDER models reproduce all 7 killer cells; refuses >3 producers"


def check_pm4():
    """Xenos PM4 parser: decode a synthetic command buffer (draw, register
    write, NOP, constant-load) and confirm packet types/opcodes/counts."""
    import pm4
    def t3(op, body):
        return [(3 << 30) | ((len(body) - 1) << 16) | (op << 8)] + list(body)
    def t0(base, body):
        return [((len(body) - 1) << 16) | base] + list(body)
    words = t3(0x2A, [0xAABBCCDD, 6]) + t0(0x100, [1, 2, 3]) + [(2 << 30)] + t3(0x36, [1, 2, 3, 4])
    data = struct.pack(">%dI" % len(words), *words)
    pkts = pm4.parse(data)
    names = [p.name.split()[0] for p in pkts]
    if names != ["DRAW_INDX", "REG_WRITE", "NOP", "SET_CONSTANT"]:
        return "FAIL", "packet decode wrong: %s" % names
    if pkts[0].count != 2 or pkts[3].count != 4 or pm4.summary(pkts)["draws"] != 1:
        return "FAIL", "counts/summary wrong"
    return "PASS", "PM4 draw/regwrite/nop/constant packets decoded"


def check_pdata():
    """.pdata RUNTIME_FUNCTION decode: pack a synthetic entry and confirm the
    extent + prolog length + 32-bit/EH flags are recovered."""
    import pdata
    packed = 5 | (40 << 8) | (1 << 30) | (1 << 31)
    blob = struct.pack(">II", 0x82001000, packed) + struct.pack(">II", 0, 0)
    f = pdata.parse(blob)[0]
    if f.begin != 0x82001000 or f.func_bytes != 160 or f.prolog_len != 5:
        return "FAIL", "extent/prolog wrong"
    if not (f.has_handler and f.is_32bit):
        return "FAIL", "flags wrong"
    return "PASS", "RUNTIME_FUNCTION extent + prolog + EH/32-bit flags decoded"


def check_callgraph():
    """Call-graph builder: assemble 3 functions with known bl edges and
    confirm the callees/callers are recovered exactly."""
    import ppcasm, callgraph
    base = 0x82001000
    def asm(l, a):
        return struct.pack(">I", ppcasm.assemble_one(l, a))
    code = (asm("bl 0x82001010", base) + asm("bl 0x82001020", base+4) + asm("blr", base+8) + asm("nop", base+12) +
            asm("bl 0x82001020", base+16) + asm("blr", base+20) + asm("nop", base+24) + asm("nop", base+28) +
            asm("blr", base+32) + asm("nop", base+36) + asm("nop", base+40) + asm("nop", base+44))
    funcs = {base: 0x10, base+0x10: 0x10, base+0x20: 0x10}
    callees, callers = callgraph.build(code, base, funcs)
    if callees[base] != {base+0x10, base+0x20} or callees[base+0x10] != {base+0x20}:
        return "FAIL", "callees wrong"
    if callers[base+0x20] != {base, base+0x10}:
        return "FAIL", "callers wrong"
    return "PASS", "3-function call graph recovered (callees + callers exact)"


def check_ppcobj():
    """PowerPC COFF object writer: emit a .obj with a bl to an external, read it
    back through our own reader, and confirm the function bytes, the REL24
    relocation, its ignore-mask, and masked_eq all round-trip."""
    import ppc_obj
    ok, msg = ppc_obj._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_server():
    """Headless REST server: start on an ephemeral port and verify /health,
    /commands and /run (including the allowlist rejecting an unknown command)."""
    import server
    ok, msg = server._selfcheck()
    return ("PASS" if ok else "FAIL"), msg


def check_ppcdecomp():
    """Ghidra-independent decompiler: assemble a known function, decompile it,
    and confirm the pseudo-C carries the intended semantics (arith, memory,
    compare/branch, negate, return)."""
    import ppcdecomp
    ok, c = ppcdecomp._selfcheck()
    if not ok:
        return "FAIL", "decompiled C missing an expected statement:\n" + c
    # a second, memory-oriented shape
    import ppcasm
    src = "lwz r3, 8(r4)\nstw r3, 0xC(r5)\nblr\n"
    code = ppcasm.assemble(src, 0x82000000)
    c2 = ppcdecomp.decompile_code(code, 0x82000000, "m")
    if "r3 = *(unsigned int*)(r4 + 0x8);" not in c2 or \
       "*(unsigned int*)(r5 + 0xC) = r3;" not in c2:
        return "FAIL", "memory access not lifted:\n" + c2
    return "PASS", "assemble -> decompile -> semantic C verified (arith+mem+branch+float)"


def check_vmx128():
    """VMX128 decoder (Xbox 360 vector ISA): every one of the 77 opcode rows
    decodes to its own mnemonic, the spec's worked example decodes exactly, and
    a plain-AltiVec word is not misclaimed. Knowledge ported from the VMX128
    decode spec (public-table + compiler-listing verified)."""
    import vmx128
    bad = [m for m, p, _ in vmx128._TABLE if (vmx128.decode(p) or (None,))[0] != m]
    if bad:
        return "FAIL", "%d rows misdecode: %s" % (len(bad), ",".join(bad[:5]))
    if vmx128.format_insn(0x138050CF) != "lvx128 vr124,r0,r10":
        return "FAIL", "worked example wrong: %s" % vmx128.format_insn(0x138050CF)
    if vmx128.is_vmx128(0x114A4AEE):                 # plain AltiVec vmaddfp
        return "FAIL", "plain AltiVec misclaimed as VMX128"
    return "PASS", "all 77 VMX128 rows + worked example decode; no false positive"


def check_chd():
    """CHD v5 native header parse (no chdman): build a synthetic DVD-CHD header
    and confirm version / logical size / hunk / compressor decode, plus a clean
    error when chdman is needed but absent."""
    import chd
    h = bytearray(124)
    h[0:8] = b"MComprHD"
    struct.pack_into(">II", h, 8, 124, 5)
    struct.pack_into(">4I", h, 16, 0x63647A6C, 0, 0, 0)     # 'cdzl'
    struct.pack_into(">QQQ", h, 32, 7825162240, 0, 0)
    struct.pack_into(">II", h, 56, 4096, 2048)
    d = tempfile.mkdtemp()
    p = os.path.join(d, "t.chd")
    open(p, "wb").write(bytes(h) + b"\x00" * 512)
    if not chd.is_chd(p):
        return "FAIL", "is_chd() false on valid CHD"
    info = chd.read_header(p)
    if info["version"] != 5 or info["logical_bytes"] != 7825162240 or info["compressors"] != ["cdzl"]:
        return "FAIL", "header decode wrong: %s" % info
    return "PASS", "CHD v5 header (size/hunk/compressor) parsed natively"


def check_zar():
    """ZArchive (.zar): pack a small tree, verify determinism + the embedded
    SHA-256 integrity hash, and that read-back content + extraction are byte-
    identical. Stdlib-only (Python 3.14 zstd); MIT, adapted from xVerter."""
    import zar
    import hashlib
    if not zar.HAVE_ZSTD:
        return "SKIP", "compression.zstd unavailable (needs Python 3.14+)"
    d = tempfile.mkdtemp()
    src = os.path.join(d, "src")
    os.makedirs(os.path.join(src, "sub"))
    open(os.path.join(src, "a.txt"), "wb").write(b"hello " * 4000)
    open(os.path.join(src, "sub", "b.bin"), "wb").write(bytes((i * 97) & 0xFF for i in range(70000)))
    want = {}
    for dp, _dn, fs in os.walk(src):
        for f in fs:
            p = os.path.join(dp, f)
            want[os.path.relpath(p, src).replace(os.sep, "/")] = hashlib.sha1(open(p, "rb").read()).hexdigest()
    z1 = os.path.join(d, "a.zar")
    z2 = os.path.join(d, "b.zar")
    zar.pack(src, z1)
    zar.pack(src, z2)
    if open(z1, "rb").read() != open(z2, "rb").read():
        return "FAIL", "pack is not deterministic"
    with zar.ZarReader(z1) as zr:
        if not zr.verify_integrity():
            return "FAIL", "integrity hash failed"
        if zr.hash_walk("sha1") != want:
            return "FAIL", "content hashes differ from source"
        zr.extract_all(os.path.join(d, "ext"))
    for rel, h in want.items():
        got = hashlib.sha1(open(os.path.join(d, "ext", rel.replace("/", os.sep)), "rb").read()).hexdigest()
        if got != h:
            return "FAIL", "extracted %s not byte-identical" % rel
    return "PASS", "deterministic pack + SHA-256 integrity + byte-identical extract"


def check_datcheck():
    """Redump/No-Intro DAT verification: build a file + a Logiqx DAT holding its
    hashes, confirm the match (by sha1) and that a bogus hash misses; also that
    a .cso of the same content authenticates via its decoded-ISO hash."""
    import datcheck
    import ciso
    import _lz4
    saved = _lz4.HAVE_LZ4
    _lz4.HAVE_LZ4 = False
    try:
        d = tempfile.mkdtemp()
        iso = bytes((i * 13) & 0xFF for i in range(2048 * 16))
        ip = os.path.join(d, "g.iso")
        open(ip, "wb").write(iso)
        crc, md5, sha1, size = datcheck.hashes(ip)
        dat = ('<?xml version="1.0"?><datafile><header><name>T</name></header>'
               '<game name="My Game (USA)"><rom name="g.iso" size="%d" crc="%s" md5="%s" sha1="%s"/>'
               '</game></datafile>') % (size, crc, md5, sha1)
        dp = os.path.join(d, "t.dat")
        open(dp, "w", encoding="utf-8").write(dat)
        dat_obj = datcheck.Dat.load(dp)
        hit, via = dat_obj.lookup(sha1=sha1)
        if not hit or hit[0] != "My Game (USA)" or via != "sha1":
            return "FAIL", "DAT match failed (%s)" % (hit,)
        if dat_obj.lookup(sha1="0" * 40)[0] is not None:
            return "FAIL", "bogus hash matched"
        ciso.write_cso(ip, os.path.join(d, "g.cso"))
        _, _, s2, _ = datcheck.hashes(os.path.join(d, "g.cso"))
        if s2 != sha1 or dat_obj.lookup(sha1=s2)[0][0] != "My Game (USA)":
            return "FAIL", "CSO did not authenticate as the ISO's DAT entry"
        return "PASS", "DAT match by sha1 + CSO authenticates + bogus rejected"
    finally:
        _lz4.HAVE_LZ4 = saved


def check_ciso():
    """CSO + CCI block-compressed disc images: LZ4 raw-block codec round-trip
    and ISO -> CSO/CCI -> ISO byte-identity, all via the pure-Python LZ4 path
    that ships in the EXE. Formats clean-room from CISO/CCI (facts per xVerter)."""
    import _lz4
    import ciso
    saved = _lz4.HAVE_LZ4
    _lz4.HAVE_LZ4 = False                          # exercise the shipped path
    try:
        for blob in (b"\x00" * 2048, bytes((i * 7) & 0xFF for i in range(2048)),
                     b"AB" * 1024, b"hello world " * 170):
            if _lz4.decode_block(_lz4.compress_block(blob), len(blob)) != blob:
                return "FAIL", "LZ4 block round-trip failed"
        d = tempfile.mkdtemp()
        iso = (b"\x00" * (2048 * 20) + bytes((i * 13) & 0xFF for i in range(2048 * 12)) +
               b"MICROSOFT*XBOX*MEDIA".ljust(2048, b"\x00") * 8)
        ip = os.path.join(d, "t.iso")
        open(ip, "wb").write(iso)
        for wr, ext in ((ciso.write_cso, "cso"), (ciso.write_cci, "cci")):
            img = os.path.join(d, "t." + ext)
            back = os.path.join(d, "b_" + ext + ".iso")
            wr(ip, img)
            ciso.to_iso(img, back)
            if open(back, "rb").read() != iso:
                return "FAIL", "%s ISO round-trip not byte-identical" % ext.upper()
        return "PASS", "LZ4 codec + CSO/CCI ISO round-trip byte-identical (pure-Python)"
    finally:
        _lz4.HAVE_LZ4 = saved


SELF_CONTAINED = [
    ("xenia patch.toml", check_xeniapatch),
    ("parser fuzzer", check_fuzz),
    ("controller autoinput", check_xinput),
    ("memory scanner", check_memscan),
    ("nxe install->god", check_install2god),
    ("xbdm data breakpoint", check_databreak),
    ("rgh flash planner", check_flashplan),
    ("cso/cci compressed iso", check_ciso),
    ("zarchive pack/verify", check_zar),
    ("vmx128 decoder", check_vmx128),
    ("call graph", check_callgraph),
    ("ppc decompiler", check_ppcdecomp),
    ("ppc coff object writer", check_ppcobj),
    ("headless rest server", check_server),
    ("pdata unwind", check_pdata),
    ("pm4 command buffer", check_pm4),
    ("c2 codegen model", check_c2model),
    ("msvc demangler", check_demangle),
    ("pipeline runner", check_pipeline),
    ("ppc interpreter", check_ppctrace),
    ("minimal xex runner", check_xexrun),
    ("xex function diff", check_xexdiff),
    ("title-update manager", check_tu),
    ("msvc il capture", check_il_capture),
    ("xma/bink media", check_media),
    ("chd header parse", check_chd),
    ("redump dat verify", check_datcheck),
    ("ppc assembler golden", check_ppcasm),
    ("xenos shader disasm", check_shader),
    ("og xbox (xbe+eeprom)", check_og_xbox),
    ("bindiff round-trip", check_bindiff),
    ("hex/diff/disasm viewers", check_view),
    ("format auto-identify", check_identify),
    ("analysis report", check_report),
    ("smc cipher + extract", check_smc),
    ("savegame edit round-trip", check_savegame),
    ("gpd edit round-trip", check_gpd_edit),
    ("nand ecc round-trip", check_nand_ecc),
    ("rte peek/poke wire", check_rte_wire),
    ("disc stealth checks", check_disc_stealth),
    ("texture untile + dds", check_texture),
    ("xpr texture bundle", check_xpr),
    ("xwb wave bank", check_xwb),
    ("xur compiled-xui", check_xur),
    ("stfs pack round-trip", check_pack_roundtrip),
    ("stfs resign crypto", check_resign_crypto),
    ("LIVE->CON conversion", check_live_to_con),
    ("resign idempotent", check_resign_idempotent),
    ("GoD disc round-trip", check_god_roundtrip),
    ("xzp pack round-trip", check_xzp_roundtrip),
    ("lzx encode round-trip", check_lzx_roundtrip),
    ("fatx read + recover", check_fatx_roundtrip),
    ("fatx write", check_fatx_write),
]


# --------------------------------------------------------------- sample-gated
def _discover(dirs):
    """Classify sample files under the given dirs by magic / name."""
    found = {"con": [], "live": [], "kv": [], "xzp": [], "account": [], "gpd": []}
    for root in dirs:
        if not os.path.isdir(root):
            continue
        for dp, dn, fn in os.walk(root):
            dn[:] = [x for x in dn if not x.lower().startswith("ghidra") and x != ".git"]
            for f in fn:
                p = os.path.join(dp, f)
                try:
                    sz = os.path.getsize(p)
                    if sz < 0x400:
                        continue
                    with open(p, "rb") as fh:
                        head = fh.read(4)
                    low = f.lower()
                    if head == b"CON ":
                        found["con"].append(p)
                    elif head in (b"LIVE", b"PIRS"):
                        found["live"].append(p)
                    elif head == b"XUIZ":                 # real container magic,
                        found["xzp"].append(p)            # not just a .xzp name
                    elif low.endswith(".gpd"):
                        found["gpd"].append(p)
                    elif f == "Account" and sz >= 0x194:
                        found["account"].append(p)
                    elif sz == 0x4000:
                        with open(p, "rb") as fh:
                            fh.seek(stfs.KV_CERT_OFF)
                            if struct.unpack(">H", fh.read(2))[0] == 0x1A8:
                                found["kv"].append(p)
                except Exception:
                    pass
            if sum(len(v) for v in found.values()) > 400:
                return found
    return found


def check_golden_resign(samples):
    """For any real CON whose signing console matches a discovered KV, resigning
    must reproduce the original bytes exactly."""
    kvs = {}
    for k in samples["kv"]:
        d = open(k, "rb").read()
        kvs[d[stfs.KV_CERT_OFF + 2:stfs.KV_CERT_OFF + 7]] = d
    for con in samples["con"]:
        d = open(con, "rb").read()
        cid = d[0x06:0x0B]
        if cid in kvs:
            out = stfs.resign(d, kvs[cid])
            if out == d:
                return "PASS", "%s resigns byte-identical" % os.path.basename(con)
            return "FAIL", "%s resign != original" % os.path.basename(con)
    return "SKIP", "no CON + matching KV pair found"


def check_stfs_read(samples):
    pkgs = samples["con"] + samples["live"]
    if not pkgs:
        return "SKIP", "no STFS packages found"
    s = stfs.STFS(open(pkgs[0], "rb").read())
    return "PASS", "%s: %d files, %s" % (os.path.basename(pkgs[0]), len(s.files),
                                         s.magic.decode("latin-1"))


def check_account(samples):
    if not samples["account"]:
        return "SKIP", "no Account file found"
    import account
    a = account.Account(open(samples["account"][0], "rb").read())
    if a.environment == "unverified":
        return "FAIL", "Account MAC did not verify"
    return "PASS", "Account decrypts (%s), gamertag %r" % (a.environment, a.gamertag)


def check_xdbf(samples):
    if not samples["gpd"]:
        return "SKIP", "no .gpd file found"
    import xdbf
    x = xdbf.XDBF(open(samples["gpd"][0], "rb").read())
    return "PASS", "%s: %d entries" % (os.path.basename(samples["gpd"][0]), len(x.entries))


def check_xzp_read(samples):
    """Read every discovered .xzp (v1 Minecraft or v3 dashboard): it must parse
    to a non-empty entry table and every PNG payload must carry a valid PNG
    signature. Guards against the reader silently returning zero entries."""
    if not samples["xzp"]:
        return "SKIP", "no .xzp file found"
    xzp = _xzp()
    checked = 0
    for src in samples["xzp"]:
        r = xzp.parse(src)
        if not r["entries"]:
            return "FAIL", "%s: parsed 0 entries" % os.path.basename(src)
        npng, hit, _ = xzp.verify(r)
        if npng and hit != npng:
            return "FAIL", "%s: %d/%d PNG signatures valid" % (
                os.path.basename(src), hit, npng)
        checked += 1
    first = xzp.parse(samples["xzp"][0])
    return "PASS", "%d file(s) read; %s v%d has %d entries" % (
        checked, os.path.basename(samples["xzp"][0]), first["version"], len(first["entries"]))


SAMPLE_GATED = [
    ("golden resign (byte-identical)", check_golden_resign),
    ("stfs read", check_stfs_read),
    ("account decrypt", check_account),
    ("xdbf read", check_xdbf),
    ("xzp read (v1/v3)", check_xzp_read),
]


# ------------------------------------------------------------------------ CLI
def main():
    dirs = [a for a in sys.argv[1:] if not a.startswith("-")]
    dirs += [p for p in os.environ.get("XBT_SAMPLES", "").split(os.pathsep) if p]
    # the repo's samples/ dir (user-local, git-ignored) is discovered by default
    repo_samples = os.path.join(os.path.dirname(os.path.dirname(HERE)), "samples")
    if os.path.isdir(repo_samples) and repo_samples not in dirs:
        dirs.append(repo_samples)
    results = []

    print("XboxTranspiler content-tool selftest\n")
    print("  self-contained (synthetic KV, committed fixtures):")
    for name, fn in SELF_CONTAINED:
        try:
            status, detail = fn()
        except Exception as e:
            status, detail = "FAIL", "%s: %s" % (type(e).__name__, e)
        results.append(status)
        print("    [%-4s] %-26s %s" % (status, name, detail))

    print("\n  golden (real samples%s):" %
          (" from %d dir(s)" % len(dirs) if dirs else " -- none given"))
    samples = _discover(dirs) if dirs else {k: [] for k in
                                            ("con", "live", "kv", "xzp", "account", "gpd")}
    for name, fn in SAMPLE_GATED:
        try:
            status, detail = fn(samples)
        except Exception as e:
            status, detail = "FAIL", "%s: %s" % (type(e).__name__, e)
        results.append(status)
        print("    [%-4s] %-26s %s" % (status, name, detail))

    npass = results.count("PASS")
    nfail = results.count("FAIL")
    nskip = results.count("SKIP")
    print("\n  %d passed, %d failed, %d skipped" % (npass, nfail, nskip))
    if not dirs:
        print("  (pass sample dir(s) or set $XBT_SAMPLES to run the golden checks)")
    sys.exit(1 if nfail else 0)


if __name__ == "__main__":
    main()
