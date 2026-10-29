#!/usr/bin/env python3
"""Tests for installation media: cabinet files (formats/cab.py) and CD images
(build/disc_image.py). Run:  python tests/test_media.py     (exit code 0 = all passed)

The checks against real cabinets are SKIPPED when the original disc contents (a folder
`disc/src/WIN95` beside the corpus folder) or the corpus itself are not present."""
import os
import random
import struct
import subprocess
import sys
import tempfile
import zlib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus                 # noqa: E402
from codebreaker.build import disc_image            # noqa: E402
from codebreaker.formats import media                # noqa: E402
from codebreaker.formats import cab                 # noqa: E402

RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _sample_files():
    rnd = random.Random(7)
    text = b"".join(b"line %d of a compressible file\r\n" % i for i in range(9000))
    return [("small.txt", b"hello, cabinet\r\n"),
            ("empty.bin", b""),
            ("text.log", text),                                   # > 100 KB, compressible
            ("noise.bin", bytes(rnd.getrandbits(8) for _ in range(70000))),   # incompressible
            ("sub\\tail.dat", bytes(range(256)) * 40)]


# --- cabinets: write, read back, extract -----------------------------------------------------
def _round_trip(compress):
    files = _sample_files()
    d = tempfile.mkdtemp()
    path = os.path.join(d, "t.cab")
    size = cab.write(files, path, compress=compress, set_id=77, index=1,
                     prev="first.cab", next=("third.cab", "Disk 3"))
    assert size == os.path.getsize(path)
    c = cab.read(path)
    assert c.info["folders"] == [compress.upper()], c.info
    assert (c.info["set_id"], c.info["index"]) == (77, 1)
    assert (c.info["prev"], c.info["next"], c.info["next_disk"]) == ("first.cab", "third.cab",
                                                                    "Disk 3")
    assert [(e.name, e.size) for e in c.files] == [(n, len(b)) for n, b in files]
    assert all(e.compression == compress.upper() and e.folder == 0 for e in c.files)
    with open(path, "rb") as f:                      # no data block payload over 32768 bytes
        f.seek(c._folders[0][0])
        for _ in range(c._folders[0][1]):
            _sum, csize, usize = struct.unpack("<IHH", f.read(8))
            assert csize <= 32768 and usize <= cab.BLOCK
            f.seek(csize, 1)
    out = cab.extract(path, os.path.join(d, "out"))
    assert set(out) == {n for n, _b in files}
    for name, data in files:
        with open(out[name], "rb") as f:
            assert f.read() == data, name
    assert os.path.isfile(os.path.join(d, "out", "sub", "tail.dat"))
    one = cab.extract(path, os.path.join(d, "one"), names=["TEXT.LOG"])
    assert list(one) == ["text.log"] and os.listdir(os.path.join(d, "one")) == ["text.log"]
    return size, sum(len(b) for _n, b in files)


def test_cabinet_round_trip_stored():
    size, total = _round_trip("none")
    assert size > total


def test_cabinet_round_trip_mszip():
    size, total = _round_trip("mszip")
    assert size < total * 0.6, (size, total)        # the text really was compressed


def test_cabinet_accepted_by_windows_expand():
    """Windows' own decoder must read what we write, across block boundaries too (a writer
    that cut MSZIP blocks short without care produced files expand.exe got wrong)."""
    expand = os.path.join(os.environ.get("SystemRoot", r"C:\Windows"), "System32", "expand.exe")
    if not os.path.isfile(expand):
        raise Skip("no Windows expand.exe")
    files = [(n.replace("\\", "_"), b) for n, b in _sample_files()]
    d = tempfile.mkdtemp()
    path, out = os.path.join(d, "t.cab"), os.path.join(d, "out")
    os.makedirs(out)
    cab.write(files, path)
    subprocess.run([expand, path, "-F:*", out], capture_output=True, timeout=60)
    for name, data in files:
        with open(os.path.join(out, name), "rb") as f:
            assert f.read() == data, name


def test_cabinet_rejects_what_it_cannot_do():
    d = tempfile.mkdtemp()
    not_cab = os.path.join(d, "x.cab")
    with open(not_cab, "wb") as f:
        f.write(b"PK\3\4 not a cabinet at all, but long enough to hold a header")
    for bad in (lambda: cab.read(not_cab),
                lambda: cab.write([("a", b"a")], os.path.join(d, "y.cab"), compress="lzx")):
        try:
            bad()
        except cab.CabError:
            continue
        raise AssertionError("no CabError")
    escape = os.path.join(d, "z.cab")
    cab.write([("..\\outside.txt", b"x")], escape)
    try:
        cab.extract(escape, os.path.join(d, "out"))
    except cab.CabError:
        return
    raise AssertionError("a name with .. was extracted")


def test_checksum_matches_known_block():
    # the documented algorithm: header words and payload words XORed, odd tail big-endian
    assert cab.checksum(b"", 0) == 0
    assert cab.checksum(b"\x01\x02\x03\x04", 4) == (0x00040004 ^ 0x04030201)
    assert cab.checksum(b"\x01\x02\x03\x04\xAA\xBB\xCC", 7) == (
        0x00070007 ^ 0x04030201 ^ 0xAABBCC)


# --- real cabinets -----------------------------------------------------------------------------
def _disc_folder():
    folder = os.path.join(os.path.dirname(corpus.root()), "disc", "src", "WIN95")
    if not os.path.isdir(folder):
        raise Skip("original disc contents not present")
    return folder


def _corpus_copies(name):
    """Paths in the corpus whose file name is `name` (any case)."""
    global _BY_NAME
    if _BY_NAME is None:
        _BY_NAME = {}
        for folder, _dirs, names in os.walk(corpus.root()):
            for n in names:
                _BY_NAME.setdefault(n.lower(), []).append(os.path.join(folder, n))
    return _BY_NAME.get(name.lower(), [])


_BY_NAME = None


def _same_as_a_corpus_copy(path, name):
    """True / False when a corpus copy of the same size exists; None when there is none."""
    with open(path, "rb") as f:
        data = f.read()
    verdict = None
    for copy in _corpus_copies(name):
        if os.path.getsize(copy) == len(data):
            with open(copy, "rb") as f:
                if f.read() == data:
                    return True
            verdict = False
    return verdict


def test_real_mszip_cabinet():
    path = os.path.join(_disc_folder(), "MINI.CAB")
    if not os.path.isfile(path):
        raise Skip("MINI.CAB not present")
    c = cab.read(path)
    assert c.info["folders"] == ["MSZIP"] and len(c.files) > 5
    out = cab.extract(path, tempfile.mkdtemp())
    assert len(out) == len(c.files)
    assert all(os.path.getsize(out[e.name]) == e.size for e in c.files)
    verdicts = [_same_as_a_corpus_copy(out[e.name], e.name) for e in c.files]
    if not any(v is not None for v in verdicts):
        raise Skip("no corpus copies to compare with")
    assert verdicts.count(True) >= 5, verdicts     # (some corpus copies were edited by SETUP)


def test_real_quantum_cabinet():
    path = os.path.join(_disc_folder(), "PRECOPY1.CAB")
    if not os.path.isfile(path):
        raise Skip("PRECOPY1.CAB not present")
    c = cab.read(path)
    assert c.info["folders"] == ["QUANTUM"] and c.info["next"]
    assert any(e.folder == cab.TO_NEXT for e in c.files)        # its folder runs on
    name = "drvspace.sys"                            # 2 KB, 160 KB into the folder
    entry = [e for e in c.files if e.name.lower() == name][0]
    out = cab.extract(path, tempfile.mkdtemp(), names=[name])
    assert os.path.getsize(out[entry.name]) == entry.size
    verdict = _same_as_a_corpus_copy(out[entry.name], name)
    if verdict is None:
        raise Skip("no corpus copy of %s" % name)
    assert verdict


# --- CD images -------------------------------------------------------------------------------
def _make_tree():
    """A folder to put on a disc. Returns (scratch folder, source folder, {name on disc: bytes})."""
    d = tempfile.mkdtemp()
    src = os.path.join(d, "src")
    rnd = random.Random(3)
    tree = [("ReadMe.txt", "README.TXT", b"read me\r\n" * 30),
            ("a long file name.text", "A_LONG_F.TEX", b"long name, cut to 8.3"),
            ("noext", "NOEXT", b"no extension"),
            ("System/kernel.bin", "SYSTEM/KERNEL.BIN",
             bytes(rnd.getrandbits(8) for _ in range(150000))),
            ("System/empty.dat", "SYSTEM/EMPTY.DAT", b""),
            ("System/Deep/z.z", "SYSTEM/DEEP/Z.Z", zlib.compress(b"deep" * 999))]
    for i in range(120):                              # a directory longer than one sector
        tree.append(("many/f%04d.dat" % i, "MANY/F%04d.DAT" % i, b"%d" % i * i))
    for on_disk, _name, data in tree:
        path = os.path.join(src, *on_disk.split("/"))
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "wb") as f:
            f.write(data)
    return d, src, {name: data for _on_disk, name, data in tree}


def test_disc_image_round_trip():
    d, src, made = _make_tree()
    iso = os.path.join(d, "t.iso")
    size = disc_image.write(src, iso, volume_id="my disc")
    assert size == os.path.getsize(iso) and size % disc_image.SECTOR == 0
    v, = media.volumes(iso)
    listing = {e.path.upper(): e for e in v.files()}
    files = {n for n, e in listing.items() if not e.is_dir}
    assert files == set(made), files ^ set(made)
    assert {n for n, e in listing.items() if e.is_dir} == {"SYSTEM", "SYSTEM/DEEP", "MANY"}
    for name, data in made.items():
        assert listing[name].size == len(data)
        assert v.read(listing[name]) == data, name
    assert v.label == "MY_DISC" and v.boot_image() is None
    out = os.path.join(d, "out")
    assert media.extract(iso, out) == len(made)
    with open(os.path.join(out, "SYSTEM", "DEEP", "Z.Z"), "rb") as f:
        assert f.read() == made["SYSTEM/DEEP/Z.Z"]


def test_disc_image_bootable():
    d, src, made = _make_tree()
    boot = os.path.join(d, "boot.img")
    image = bytes(random.Random(5).getrandbits(8) for _ in range(4096)) * 360     # 1474560
    with open(boot, "wb") as f:
        f.write(image)
    iso = os.path.join(d, "b.iso")
    disc_image.write(src, iso, boot_image=boot)
    vols = media.volumes(iso)
    offset, size, kind = vols[0].boot_image()
    assert kind == 2 and size == len(image), (kind, size)                 # a 1.44 MB floppy
    with open(iso, "rb") as f:
        f.seek(17 * disc_image.SECTOR)
        assert f.read(30)[7:] == b"EL TORITO SPECIFICATION"
        f.seek(offset)
        assert f.read(len(image)) == image
    listing = {e.path.upper(): e for e in vols[0].files()}
    assert vols[0].read(listing["SYSTEM/KERNEL.BIN"]) == made["SYSTEM/KERNEL.BIN"]


def test_disc_image_reports_name_clashes():
    d, src, _made = _make_tree()
    for name in ("longname_one.txt", "longname_two.txt"):
        with open(os.path.join(src, name), "wb") as f:
            f.write(b"x")
    try:
        disc_image.write(src, os.path.join(d, "c.iso"))
    except disc_image.DiscError as e:
        assert "longname_one.txt" in str(e) and "LONGNAME.TXT" in str(e)
        return
    raise AssertionError("two files with one 8.3 name were written")


# --- writing into a FAT volume ---------------------------------------------------------------
def _blank_fat(path, bits):
    """An empty FAT volume made by hand: a FAT16 one inside a partition table (as an installed
    hard disk has it, the partition at sector 63) or a bare FAT32 one."""
    if bits == 16:
        first, spc, total, roots, spf = 63, 4, 32768, 512, 33
    else:
        first, spc, total, roots, spf = 0, 1, 70000, 0, 548
    b = bytearray(512)
    b[0:3], b[3:11] = b"\xEB\x3C\x90", b"MSWIN4.1"
    struct.pack_into("<HBHBHHBHHHII", b, 11, 512, spc, 1 if bits == 16 else 32, 2, roots, 0, 0xF8,
                     spf if bits == 16 else 0, 63, 16, first, total)
    if bits == 32:
        struct.pack_into("<IHHI", b, 36, spf, 0, 0, 2)                # root folder at cluster 2
        b[66] = 0x29
    else:
        b[38] = 0x29
    b[510:512] = b"\x55\xAA"
    reserved = struct.unpack_from("<H", b, 14)[0]
    fat = bytearray(spf * 512)
    struct.pack_into("<HH" if bits == 16 else "<III", fat, 0, *((0xFFF8, 0xFFFF) if bits == 16 else (0x0FFFFFF8, 0x0FFFFFFF, 0x0FFFFFFF)))
    with open(path, "wb") as f:
        f.truncate((first + total) * 512)
        if first:
            mbr = bytearray(512)
            mbr[446 + 4] = 0x06
            struct.pack_into("<II", mbr, 446 + 8, first, total)
            mbr[510:512] = b"\x55\xAA"
            f.write(mbr)
        f.seek(first * 512)
        f.write(b)
        for i in range(2):
            f.seek((first + reserved + i * spf) * 512)
            f.write(fat)


def _put_round_trip(bits):
    d = tempfile.mkdtemp()
    img = os.path.join(d, "hd.img")
    _blank_fat(img, bits)
    big = bytes(random.Random(11).getrandbits(8) for _ in range(70000))
    done = media.put(img, {"WINDOWS/SYSTEM/VMM32/VTD.VXD": big, "msdos.sys": b"[Options]\r\n",
                           "WINDOWS/SYSTEM/empty.dat": b""})
    assert [h for _p, h in done] == ["added"] * 3, done
    for i in range(40):                                   # more than a cluster of entries: the folder grows
        media.put(img, {"WINDOWS/SYSTEM/VMM32/F%02d.VXD" % i: b"%d" % i * 300})
    assert media.put(img, {"WINDOWS/SYSTEM/VMM32/VTD.VXD": b"short now"}) == [("WINDOWS/SYSTEM/VMM32/VTD.VXD", "replaced")]
    media.put(img, {"MSDOS.SYS": big[:5000]})
    v, = media.volumes(img)
    listing = {e.path.upper(): e for e in v.files()}
    assert v.read(listing["WINDOWS/SYSTEM/VMM32/VTD.VXD"]) == b"short now"
    assert v.read(listing["MSDOS.SYS"]) == big[:5000]
    assert listing["WINDOWS/SYSTEM/EMPTY.DAT"].size == 0 and listing["WINDOWS/SYSTEM/VMM32"].is_dir
    for i in range(40):
        assert v.read(listing["WINDOWS/SYSTEM/VMM32/F%02d.VXD" % i]) == b"%d" % i * 300
    used = sum(1 for c in range(2, v.clusters + 2) if v._next(c))     # the replaced chains were let go
    step = v.spc * v.bps
    want = sum((e.size + step - 1) // step for e in listing.values() if not e.is_dir)
    want += sum(len(v._chain(e.start)) // step for e in listing.values() if e.is_dir) + (bits == 32)
    assert used == want, (used, want)
    try:
        media.put(img, {"WINDOWS/SYSTEM/a long name.vxd": b"x"})
    except ValueError:
        return
    raise AssertionError("a name that is not 8.3 was written")


def test_fat16_put_in_partition():
    _put_round_trip(16)


def test_fat32_put():
    _put_round_trip(32)


def main():
    for fn in (test_cabinet_round_trip_stored, test_cabinet_round_trip_mszip,
               test_cabinet_accepted_by_windows_expand,
               test_cabinet_rejects_what_it_cannot_do, test_checksum_matches_known_block,
               test_real_mszip_cabinet, test_real_quantum_cabinet,
               test_disc_image_round_trip, test_disc_image_bootable,
               test_disc_image_reports_name_clashes, test_fat16_put_in_partition, test_fat32_put):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
