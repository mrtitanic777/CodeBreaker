#!/usr/bin/env python3
"""Tests for real-mode programs and boot media: the reader (formats/dos.py), unpacking, function
discovery, interrupt names, the description for the decompiler, and disk / CD images.
Run:  python tests/test_dos.py     (exit code 0 = all passed)

Checks that need the retail corpus, the built decompiler or an image file are SKIPPED when
those are not present."""
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.analyze import dos_functions      # noqa: E402
from codebreaker.base import corpus                # noqa: E402
from codebreaker.formats import dos, media         # noqa: E402
from codebreaker.symbols import dos_interrupts     # noqa: E402

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


def _retail(name):
    path = corpus.retail(name) if os.path.isdir(corpus.root()) else None
    if not path:
        raise Skip("%s is not in the corpus" % name)
    return path


# --- the reader ------------------------------------------------------------------------------
def test_kinds():
    assert dos.kind(_retail("find.exe")) == "mz"
    assert dos.kind(_retail("more.com")) == "com"
    assert dos.kind(_retail("himem.sys")) == "driver"
    assert dos.kind(_retail("ne2000.dos")) == "driver"          # a driver linked as an .EXE
    assert dos.kind(_retail("winboot.sys")) == "kernel"
    assert dos.kind(_retail("country.sys")) is None             # data with a program's extension
    assert dos.kind(_retail("kernel32.dll")) is None            # a Windows module is not a DOS program


def test_com_and_driver_layout():
    p = dos.read(_retail("more.com"))
    assert p.base == dos.LOAD * 16 and p.entries == [(dos.LOAD * 16 + 0x100, "entry", "near")]
    assert p.image[:2] == b"\xCD\x20" and p.image[0x100:] == open(p.path, "rb").read()
    d = dos.read(_retail("himem.sys"))
    assert [x.name for x in d.devices] == ["XMSXXXX0"] and d.devices[0].character
    names = [n for _va, n, _dist in d.entries]
    assert names == ["XMSXXXX0_strategy", "XMSXXXX0_interrupt"], names


def test_relocations_are_applied():
    path = _retail("snapshot.exe")
    data = open(path, "rb").read()
    p = dos.read(path)
    nreloc, header = struct.unpack_from("<H", data, 6)[0], struct.unpack_from("<H", data, 8)[0] * 16
    table = struct.unpack_from("<H", data, 0x18)[0]
    assert len(p.relocations) == nreloc == 7
    for i in range(nreloc):
        off, seg = struct.unpack_from("<HH", data, table + 4 * i)
        stored = struct.unpack_from("<H", data, header + seg * 16 + off)[0]
        assert p.word(dos.LOAD * 16 + seg * 16 + off) == (stored + dos.LOAD) & 0xFFFF


def test_exepack_static_equals_emulated():
    from codebreaker.emulate import unpack
    path = _retail("find.exe")
    assert dos.packer(path) == "EXEPACK"
    data = open(path, "rb").read()
    h = dos._mz(data)
    module = bytes(data[h["header"]:h["size"]])
    static = dos.unpack_exepack(module, h["cs"], h["ip"])
    assert len(static[0]) == 12016 and static[2:4] == (0, 0x18DA), (len(static[0]), static[2:])
    ran = unpack.unpacked(module, [], h["cs"], h["ip"], h["ss"], h["sp"], h["min_alloc"])
    assert ran[0] == static[0] and ran[1] == sorted(static[1]) and ran[2:] == static[2:]
    p = dos.read(path)
    assert p.unpacked == "static" and p.image == static[0] and p.entries[0][0] == dos.LOAD * 16 + 0x18DA


def test_unpacked_file():
    """A packed program written back unpacked reads as the same program, no longer packed."""
    p = dos.read(_retail("find.exe"))
    out = os.path.join(tempfile.mkdtemp(prefix="cb_unpack_"), "find.exe")
    with open(out, "wb") as f:
        f.write(dos.unpacked_file(p))
    q = dos.read(out)
    assert dos.packer(out) is None and q.unpacked is None
    assert q.image == p.image and q.relocations == p.relocations
    assert (q.cs, q.ip, q.ss, q.sp) == (p.cs, p.ip, p.ss, p.sp)


def test_kernel_image_parts():
    p = dos.read(_retail("winboot.sys"))
    assert [label for _lo, _n, label in p.spans] == ["bios and dos", "sysinit", "loader"]
    assert p.unpacked == "static"                               # SYSINIT is stored with EXEPACK
    names = [n for _va, n, _dist in p.entries]
    assert names[:3] == ["bios_entry", "loader_entry", "sysinit_entry"] and "CON_interrupt" in names
    assert [d.name for d in p.devices][:4] == ["CON", "AUX", "PRN", "CLOCK$"]
    assert p.image[dos.LOADER_SEGMENT * 16 - p.base:][:2] == b"MZ"


# --- functions and interrupts --------------------------------------------------------------------
def test_function_discovery():
    a = dos_functions.discover(dos.read(_retail("find.exe")))
    sure = [f for f in a.functions.values() if f.sure]
    assert len(sure) == 60 and {f.evidence for f in sure} == {"entry", "called"}, len(sure)
    assert all(f.evidence in ("pointer", "prologue", "follows") or not f.sure or f.evidence in ("entry", "called")
               for f in a.functions.values())
    assert len(a.functions) - len(sure) <= 3                    # guesses are few here, and marked
    entry = a.functions[dos.LOAD * 16 + 0x18DA]
    assert entry.name == "entry" and entry.evidence == "entry"
    c = a.census()
    assert c["total"] == 12016 and c["code"] > 4000 and c["guessed code"] < 100
    assert c["code"] + c["guessed code"] + c["not classified"] + c["jump tables"] == c["total"]
    named = [dos_interrupts.name(i.number, i.ah, i.al) for i in a.interrupts]
    assert len(named) >= 30 and sum(1 for n in named if n.exact) >= 25
    assert "DOS_Exit" in {n.name for n in named}


def test_installed_handler_is_found():
    a = dos_functions.discover(dos.read(_retail("dlchlp.exe")))
    handlers = [f for f in a.functions.values() if f.evidence == "vector"]
    assert len(handlers) == 1 and handlers[0].name.startswith("int_5C_handler") and handlers[0].distance == "far"


def test_interrupt_names():
    assert dos_interrupts.name(0x21, 0x4C).name == "DOS_Exit"
    assert dos_interrupts.name(0x21, 0x44, 0x00).name == "DOS_IoctlGetDeviceInformation"
    assert dos_interrupts.name(0x21, 0x44).name == "DOS_Ioctl"
    assert dos_interrupts.name(0x2F, 0x16, 0x00).name == "Multiplex_WindowsEnhancedModeCheck"
    assert dos_interrupts.name(0x33, 0x00, 0x03).name == "Mouse_GetPositionAndButtons"
    assert dos_interrupts.name(0x10, 0x0E).arguments.startswith("AL = character")
    assert not dos_interrupts.name(0x21).exact and not dos_interrupts.name(0x99).exact
    i = dos_functions.Interrupt(0x1234, 0x21, 0x09, None, None)
    text = dos_interrupts.annotate("x = swi(0x21);", [i])
    assert "DOS_WriteString" in text and "swi(0x21) /* DOS_WriteString */" in text


def test_description_for_the_decompiler():
    from codebreaker.decompile import decompiler, describe
    d = describe.describe_dos(_retail("find.exe"), tempfile.mkdtemp())
    lines = open(d.path).read().splitlines()
    assert lines[0] == "arch\tx86:LE:16:Real Mode:default" and lines[2] == "reg\tDS\t%x" % d.program.ds
    functions = [x.split("\t") for x in lines if x.startswith("F\t")]
    assert len(functions) == len(d.names) >= 50
    assert all(f[4] in ("__cdecl16far", "__cdecl16near", "__stdcall16far", "__stdcall16near") for f in functions)
    assert ["%x" % (dos.LOAD * 16 + 0x18DA), "entry"] in [f[1:3] for f in functions]
    if not decompiler.available():
        raise Skip("description written; native/decompiler is not built")
    three = sorted(d.names)[:3]
    out, order, _by_va, _err = decompiler.run(d.path, only=three)
    assert len(order) >= 2, order


# --- media ---------------------------------------------------------------------------------------
def test_boot_floppy():
    path = os.path.join(corpus.root(), "setup", "media", "boot.img")
    if not os.path.isfile(path):
        raise Skip("no boot floppy image in the corpus")
    v, = media.volumes(path)
    assert v.kind == "fat12" and v.label == "BOOT95B" and v.boot["system"] == "MSWIN4.1"
    files = {e.path: e for e in v.files()}
    assert len(files) == 31 and files["IO.SYS"].size == 214836
    assert v.read(files["IO.SYS"]) == open(_retail("winboot.sys"), "rb").read()
    out = tempfile.mkdtemp()
    assert media.extract(path, out, ["*.sys"]) == 8 and os.path.getsize(os.path.join(out, "HIMEM.SYS")) == 33191


def test_cd_image():
    path = os.path.join(os.path.dirname(corpus.design_dir()), "..", "win1.0", "Win1.0.iso")
    if not os.path.isfile(path):
        raise Skip("no small CD image on this machine")
    v = media.volumes(path)[0]
    files = [e for e in v.files() if not e.is_dir]
    assert v.kind == "iso9660" and len(files) == 35
    assert all(len(v.read(e)) == e.size for e in files[:5])


def test_not_an_image():
    p = os.path.join(tempfile.mkdtemp(), "x.img")
    with open(p, "wb") as f:
        f.write(b"this is not a disk" * 4000)
    try:
        media.volumes(p)
    except ValueError:
        return
    raise AssertionError("garbage was read as an image")


def main():
    for fn in (test_kinds, test_com_and_driver_layout, test_relocations_are_applied,
               test_exepack_static_equals_emulated, test_unpacked_file, test_kernel_image_parts, test_function_discovery,
               test_installed_handler_is_found, test_interrupt_names, test_description_for_the_decompiler,
               test_boot_floppy, test_cd_image, test_not_an_image):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
