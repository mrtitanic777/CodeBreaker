"""msc6.py -- Microsoft C 6.0a, the compiler of the 16-bit Windows modules, run for comparison.

    from codebreaker.build import msc6
    msc6.available()
    built = msc6.compile_many({"f": c_text, ...}, header, "/AM /Gsw /Os")   # {"f": (bytes, mask) or None}

The compiler is a DOS program. It is run in DOSBox-X (settings: dosbox_x -- the emulator's
program; msc6 -- the folder holding the compiler's BIN and INCLUDE), every source of a call in
ONE session: starting the emulator costs about two seconds, compiling a function a fraction of
one. Each function is compiled as a file of its own and its code read out of the object file,
with the bytes the linker fills in (an address, a segment, the distance of a call to another
file) marked in `mask` and left out of every comparison -- as build/period.py's other compilers.

What the reconstruction is written in for judging is OpenWatcom's dialect; `dialect()` is the
same text said the way this compiler takes it (its keywords for far and the calling
conventions, no 64-bit integers).
"""
import os
import re
import struct
import subprocess

# /Gc: a function declared without a convention is pascal, as the reconstruction's own default
FLAG_SETS = ("/AM /Gcsw /Os", "/AM /Gcsw /Ox", "/AS /Gcsw /Os", "/AL /Gcsw /Os", "/AM /Gcsw /Od", "/AM /Gc2sw /Os",
             "/AM /Gc2sw /Ox", "/AM /Gcs /Os", "/AS /Gcs /Os")   # (/G2: SUWIN has push imm and leave)

_CONF = ("[sdl]\noutput=surface\n[cpu]\ncore=dynamic\ncputype=386\ncycles=max\n[autoexec]\n"
         "mount c \"%s\"\nmount d \"%s\"\nset PATH=C:\\BIN\nset INCLUDE=C:\\INCLUDE\nset LIB=C:\\LIB\nd:\ncall BLD.BAT\nexit\n")


def _tools(compiler="msc6"):
    """(DOSBox-X, the compiler's folder). compiler: "msc6" (Microsoft C 6.0a) or "msvc152"
    (Microsoft C/C++ 8.00c of Visual C++ 1.52c, which built a few of the 16-bit modules); both
    are a BIN\\CL.EXE run the same way."""
    from codebreaker.base import settings
    return settings.get("dosbox_x"), settings.get(compiler)


def available(compiler="msc6"):
    emulator, home = _tools(compiler)
    return bool(emulator and os.path.isfile(emulator) and home and os.path.isfile(os.path.join(home, "BIN", "CL.EXE")))


def object_code(obj):
    """(bytes, mask) of the first code segment of an OMF object file: the data records of that
    segment laid end to end, and 1 for every byte a fix-up record names."""
    code, mask, p = bytearray(), bytearray(), 0
    last = None                                              # (segment, offset of the record's data in `code`)
    while p + 3 <= len(obj):
        kind = obj[p]
        size = struct.unpack_from("<H", obj, p + 1)[0]
        body = obj[p + 3:p + 3 + size - 1]
        p += 3 + size
        if kind in (0xA0, 0xA1):                             # LEDATA: segment, offset, bytes
            i = 0
            seg = body[i]
            i += 1
            if seg & 0x80:
                seg = ((seg & 0x7F) << 8) | body[i]
                i += 1
            width = 4 if kind == 0xA1 else 2
            off = int.from_bytes(body[i:i + width], "little")
            i += width
            if seg != 1:
                last = None
                continue
            if off > len(code):
                code.extend(bytes(off - len(code)))
                mask.extend(bytes(off - len(mask)))
            code[off:off + len(body) - i] = body[i:]
            mask[off:off + len(body) - i] = bytes(len(body) - i)
            last = (seg, off)
        elif kind in (0x9C, 0x9D) and last:                  # FIXUPP: the places in the record before it
            i = 0
            while i < len(body):
                b = body[i]
                if not b & 0x80:                             # a thread: one byte, and an index for most methods
                    i += 1
                    if (b >> 2) & 7 < 4 and i < len(body):
                        i += 2 if body[i] & 0x80 else 1
                    continue
                where = ((b & 3) << 8) | body[i + 1]
                loc = (b >> 2) & 0xF
                fix = body[i + 2]
                i += 3
                if not fix & 0x80 and (fix >> 4) & 7 < 3:    # the frame is given here
                    i += 2 if body[i] & 0x80 else 1
                if not fix & 0x08:                           # the target is given here
                    i += 2 if body[i] & 0x80 else 1
                if not fix & 0x04:                           # with a displacement
                    i += 4 if kind == 0x9D else 2
                n = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2, 9: 4, 11: 6, 13: 4}.get(loc, 2)
                at = last[1] + where
                mask[at:at + n] = b"\1" * min(n, max(0, len(mask) - at))
        elif kind in (0x8A, 0x8B):
            break
    return bytes(code), bytes(mask)


def dialect(header, text):
    """(header, text) as Microsoft C 6 takes them."""
    said = ["#define __far _far", "#define __near _near", "#define __stdcall _pascal", "#define __cdecl _cdecl",
            "#define codebreaker_ss() 0", "#define codebreaker_ds() 0", "#define codebreaker_cs() 0",
            # OpenWatcom's spellings of what Microsoft C 6 has with one underscore, and the far-pointer
            # macros OpenWatcom's i86.h gives (here without a header)
            "#define __based _based", "#define __segname _segname", "#define __export _export",
            "#define __loadds _loadds", "#define __interrupt _interrupt", "#define __saveregs _saveregs",
            "#define MK_FP(s,o) ((void _far *)(((unsigned long)(unsigned)(s) << 16) | (unsigned)(o)))",
            "#define FP_OFF(p) ((unsigned)(unsigned long)(void _far *)(p))",
            "#define FP_SEG(p) ((unsigned)((unsigned long)(void _far *)(p) >> 16))"]
    keep = []
    for line in header.split("\n"):
        if "#pragma aux" in line or re.match(r"uint2 codebreaker_[sdc]s\(void\);", line.strip()):
            continue
        line = line.replace("unsigned long long", "unsigned long").replace("long long", "long").replace(
            "long double", "double")
        keep.append(line)
    return "\n".join(said + keep) + "\n", text


def function_code(obj, name):
    """(bytes, mask) of the public function `name` in an OMF object file -- cut from its code segment
    between its public name and the next -- or None. A header's own functions (static helpers, the
    reconstruction's stubs) come first in the segment, so the first code segment is not the function."""
    from codebreaker.invert import omf
    want = name.lstrip("_").lower()
    for pub, (code, holes) in omf.functions(obj).items():
        if pub.lstrip("_").lower() == want:
            mask = bytearray(len(code))
            for at in holes:
                if at < len(mask):
                    mask[at] = 1
            return bytes(code), bytes(mask)
    return None


def compile_many(sources, header, flags, work=None, timeout=900, compiler="msc6", names=None):
    """Compile each of `sources` {key: C text} with `header` before it. -> {key: (bytes, mask) or None}
    names: {key: function name} -- each key's code is that public function's (function_code), not the
    object's first code segment."""
    from codebreaker.base import cache, settings
    emulator, home = _tools(compiler)
    if not available(compiler):
        raise RuntimeError("%s is not set up (settings.json: dosbox_x, %s)" % (compiler, compiler))
    out, todo = {}, {}
    stamp = cache.tool(os.path.join(home, "BIN", "CL.EXE"))
    for key, text in sources.items():
        k = cache.key(stamp, flags, header, text, "function " + names[key] if names and key in names else "")
        had = cache.get("msc6", k)
        if had is not None:
            out[key] = None if had == b"-" else (had[4:4 + int.from_bytes(had[:4], "little")],
                                                 had[4 + int.from_bytes(had[:4], "little"):])
        else:
            todo[key] = (k, text)
    if not todo:
        return out
    work = os.path.abspath(work or settings.state(compiler, "work"))
    os.makedirs(work, exist_ok=True)
    for f in os.listdir(work):
        if f.lower().endswith((".obj", ".c", ".txt")):
            os.remove(os.path.join(work, f))
    with open(os.path.join(work, "M.H"), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
        f.write(header)
    lines, files = ["@echo off"], {}
    for i, (key, (_k, text)) in enumerate(todo.items()):
        files[key] = "F%d" % i
        with open(os.path.join(work, "F%d.C" % i), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
            f.write('#include "m.h"\n\n' + text + "\n")
        lines.append("cl /nologo /c /W0 %s /FoF%d.OBJ F%d.C >nul" % (flags, i, i))
    lines.append("echo DONE > DONE.TXT")
    with open(os.path.join(work, "BLD.BAT"), "w", newline="") as f:
        f.write("\r\n".join(lines) + "\r\n")
    with open(os.path.join(work, "R.CONF"), "w", newline="\n") as f:
        f.write(_CONF % (home, work))
    try:
        subprocess.run([emulator, "-conf", os.path.join(work, "R.CONF"), "-exit", "-nopromptfolder", "-silent"],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=timeout)
    except subprocess.TimeoutExpired:
        pass
    finished = os.path.isfile(os.path.join(work, "DONE.TXT"))
    for key, (k, _text) in todo.items():
        path = os.path.join(work, files[key] + ".OBJ")
        built = None
        if os.path.isfile(path):
            with open(path, "rb") as f:
                obj = f.read()
            if names and key in names:
                built = function_code(obj, names[key])
            else:
                code, mask = object_code(obj)
                if code:
                    built = (code, mask)
        out[key] = built
        if built:
            cache.put("msc6", k, len(built[0]).to_bytes(4, "little") + built[0] + built[1])
        elif finished:
            cache.put("msc6", k, b"-")                       # the compiler refused it (not: the session was cut off)
    return out
