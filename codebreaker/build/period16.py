"""period16.py -- a 16-bit Windows DLL built from its source with the tools of its time.

    python -m codebreaker build16 <source folder> [--original MODULE] [--work DIR] [--stub DONOR]

    from codebreaker.build import period16
    dll = period16.build(folder)                  the built DLL's path, or None
    same, total = period16.compare(dll, original)  segments identical to the original's
    period16.normalize(dll, donor, out)            the original's DOS stub put in front

The 16-bit modules of Windows 95 were compiled with Microsoft C 6.0a and linked with the
Windows SDK's LINK 5.60 against the Windows 3.1 DLL runtime. Microsoft C 6 here compiles but
has no linker or libraries, so the build uses both kits, in DOSBox-X:

    each module   Microsoft C 6.0a   cl /c /AM /G2 /Gsw /Ox            (the code)
    the DLL       Visual C++ 1.52c   link @<folder's .lnk> ; rc          (the NE file, its runtime)

A source folder holds the modules' .c files, the headers, the module-definition (.def) file
and the linker's response file (.lnk) that lists the objects and names the DLL. Objects with
no .c (the DLL entry, LIBENTRY.OBJ) are taken from the Windows 3.1 DLL kit
(toolchains/period/libs/win31-kit) or the DDK. That kit's lean runtime is the one the
originals were linked with; Visual C++ 1.52c's own is a third larger.

settings: dosbox_x, msc6 (Microsoft C 6.0a), msvc152 (Visual C++ 1.52c: LINK, RC, LIB).

The comparison is per segment, byte for byte: LINK lays out the code exactly as the original
was laid out when the source is the source. tests/fixtures/lzexpand is a reconstruction of
LZEXPAND.DLL made this way. Its notes said eight of twelve segments came out identical; against
the corpus's copies today it builds eleven segments, several the original's length, none
identical (the old tool gives the same) -- a measure of what is left, not of the build.
"""
import os
import re
import shutil
import struct
import subprocess
import tempfile

from codebreaker.base import corpus, settings


def _win(p):
    return p.replace("/", "\\")


def _kit():
    """The Windows 3.1 DLL kit: MDLLCEW.LIB, LIBW.LIB, LIBENTRY.OBJ -- or None."""
    d = os.path.join(settings.ROOT, "toolchains", "period", "libs", "win31-kit")
    return d if os.path.exists(os.path.join(d, "MDLLCEW.LIB")) else None


def _libentry():
    kit = _kit()
    if kit and os.path.exists(os.path.join(kit, "LIBENTRY.OBJ")):
        return os.path.join(kit, "LIBENTRY.OBJ")
    p = corpus.reference("Microsoft Windows 95 DDK", "LIB", "LIBENTRY.OBJ")
    return p if os.path.exists(p) else None


def _response_file(folder):
    found = sorted(f for f in os.listdir(folder) if f.lower().endswith(".lnk"))
    if not found:
        raise RuntimeError("no linker response file (.lnk) in %s" % folder)
    return found[0]


def build(folder, work=None, flags="/AM /G2 /Gsw /Ox"):
    """Compile and link the DLL a source folder describes. -> path of the built DLL, or None
    (the work folder keeps CL.TXT, LINK.TXT and RC.TXT)."""
    dbx, msc6, msvc = settings.get("dosbox_x"), settings.get("msc6"), settings.get("msvc152")
    for what, path in (("dosbox_x", dbx), ("msc6", msc6), ("msvc152", msvc)):
        if not path or not os.path.exists(path):
            raise RuntimeError("%s is not set up (settings.json: %s)" % (what, what))
    work = work or tempfile.mkdtemp(prefix="build16_")
    os.makedirs(work, exist_ok=True)
    for fn in os.listdir(folder):
        if fn.lower().endswith((".c", ".h", ".lnk", ".def", ".obj")):
            shutil.copy2(os.path.join(folder, fn), work)
    stub = os.path.join(msvc, "BIN", "WINSTUB.EXE")                 # the .def's STUB
    if os.path.exists(stub):
        shutil.copy2(stub, work)
    lnk_name = _response_file(work)
    with open(os.path.join(work, lnk_name)) as f:
        lnk = f.read()
    objs = re.findall(r"([A-Za-z_]\w*)\.obj", lnk)
    out_name = (re.findall(r"(\w+\.(?:dll|exe|drv))", lnk, re.I) or ["out.dll"])[0]
    if any(o.lower() == "libentry" for o in objs) and not os.path.exists(os.path.join(work, "LIBENTRY.OBJ")):
        entry = _libentry()
        if entry:
            shutil.copy2(entry, os.path.join(work, "LIBENTRY.OBJ"))
    kit = _kit()
    if kit and re.search(r"mdllcew\.lib", lnk, re.I):                 # LINK looks in the current folder first
        for lib in ("MDLLCEW.LIB", "LIBW.LIB"):
            if os.path.exists(os.path.join(kit, lib)):
                shutil.copy2(os.path.join(kit, lib), os.path.join(work, lib))
    modules = [m for m in objs if os.path.exists(os.path.join(work, m + ".c"))]
    with open(os.path.join(work, "BUILD.BAT"), "w", newline="") as f:
        f.write("@echo off\r\n")
        for m in modules:
            f.write("del %s.obj\r\n" % m)
        f.write("del %s\r\ndel done.txt\r\n" % out_name)
        for m in modules:
            f.write("cl /c %s %s.c >> CL.TXT\r\n" % (flags, m))
        f.write("link @%s > LINK.TXT\r\nrc %s > RC.TXT\r\necho DONE > done.txt\r\n" % (lnk_name, out_name))
    with open(os.path.join(work, "b.conf"), "w", newline="") as f:
        f.write("[cpu]\r\ncycles=max\r\n[autoexec]\r\n")
        f.write("mount c %s\r\nmount e %s\r\nmount d %s\r\n" % (_win(msc6), _win(msvc), _win(work)))
        # windows.h is in the SDK's (Visual C++ 1.52c's) INCLUDE, the C library's headers in Microsoft C 6's
        f.write("set PATH=C:\\BIN;E:\\BIN\r\nset INCLUDE=C:\\INCLUDE;E:\\INCLUDE\r\nset LIB=D:\\;E:\\LIB\r\n")
        f.write("d:\r\ncall BUILD.BAT\r\nexit\r\n")
    subprocess.run([dbx, "-conf", os.path.join(work, "b.conf"), "-fastlaunch"],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=600,
                   env=dict(os.environ, SDL_VIDEODRIVER="dummy"))
    dll = os.path.join(work, out_name)
    return dll if os.path.exists(dll) else None


def segments(path):
    """[(length, bytes)] of an NE file's segments, in order."""
    with open(path, "rb") as f:
        d = f.read()
    lfa = struct.unpack_from("<I", d, 0x3c)[0]
    n = struct.unpack_from("<H", d, lfa + 0x1c)[0]
    table = struct.unpack_from("<H", d, lfa + 0x22)[0] + lfa
    align = struct.unpack_from("<H", d, lfa + 0x32)[0] or 9
    out = []
    for i in range(n):
        sector, length, _flags, _minimum = struct.unpack_from("<HHHH", d, table + i * 8)
        out.append((length, d[sector << align:(sector << align) + length]))
    return out


def compare(built, original, log=print):
    """Segment by segment. -> (identical, segments of the original)"""
    b, o = segments(built), segments(original)
    if log:
        log("built: %d segments, %d bytes   original: %d segments, %d bytes" % (
            len(b), os.path.getsize(built), len(o), os.path.getsize(original)))
    same = 0
    for i in range(max(len(b), len(o))):
        bl = b[i][0] if i < len(b) else None
        ol = o[i][0] if i < len(o) else None
        if i < len(b) and i < len(o) and b[i][1] == o[i][1]:
            verdict = "identical"
            same += 1
        else:
            verdict = "same length, different" if bl == ol else "different length"
        if log:
            log("  segment %-2d built %-5s original %-5s %s" % (i + 1, bl if bl is not None else "-",
                                                              ol if ol is not None else "-", verdict))
    if log:
        log("%d of %d segments identical" % (same, len(o)))
    return same, len(o)


def normalize(built, donor, out):
    """Put the DOS stub of `donor` (an NE or PE file, or a raw stub) in front of `built`'s NE
    header and fix every file-absolute field that moves with it (e_lfanew, the segment
    sectors, the non-resident names, the fast-load area). LINK 5.20 pads the stub to 1024
    bytes; the retail modules have a shorter one. No segment byte moves relative to the header."""
    with open(built, "rb") as f:
        d = bytearray(f.read())
    with open(donor, "rb") as f:
        stub = f.read()
    if stub[:2] == b"MZ":
        lfanew = struct.unpack_from("<H", stub, 0x3c)[0]
        if 0 < lfanew <= len(stub):
            stub = stub[:lfanew]
    ne = struct.unpack_from("<H", d, 0x3c)[0]
    new_ne = len(stub)
    if new_ne % 16 or new_ne > ne:
        raise ValueError("the donor's stub (0x%x) cannot replace this one (0x%x)" % (new_ne, ne))
    shift = ne - new_ne
    align = struct.unpack_from("<H", d, ne + 0x32)[0] or 9
    if shift % (1 << align):
        raise ValueError("the stubs differ by 0x%x, not a whole number of sectors" % shift)
    sectors = shift >> align
    o = bytearray(stub) + d[ne:]
    struct.pack_into("<H", o, 0x3c, new_ne)
    n = struct.unpack_from("<H", o, new_ne + 0x1c)[0]
    table = struct.unpack_from("<H", o, new_ne + 0x22)[0]
    for i in range(n):
        p = new_ne + table + i * 8
        sector = struct.unpack_from("<H", o, p)[0]
        if sector:                                                     # 0: no image in the file
            struct.pack_into("<H", o, p, sector - sectors)
    nonresident = struct.unpack_from("<I", o, new_ne + 0x2c)[0]
    if nonresident:
        struct.pack_into("<I", o, new_ne + 0x2c, nonresident - shift)
    fast = struct.unpack_from("<H", o, new_ne + 0x38)[0]
    if fast:
        struct.pack_into("<H", o, new_ne + 0x38, fast - sectors)
    with open(out, "wb") as f:
        f.write(o)
    return bytes(o)


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    folder = argv[0]
    work = argv[argv.index("--work") + 1] if "--work" in argv else None
    dll = build(folder, work)
    if not dll:
        print("no DLL was made (see CL.TXT and LINK.TXT in the work folder)")
        return 1
    print("built: %s" % dll)
    original = argv[argv.index("--original") + 1] if "--original" in argv else os.path.basename(dll)
    original = original if os.path.isfile(original) else corpus.retail(original)
    if "--stub" in argv:
        donor = argv[argv.index("--stub") + 1]
        donor = donor if os.path.isfile(donor) else corpus.retail(donor)
        normalize(dll, donor, dll)
    if original:
        compare(dll, original)
    return 0
