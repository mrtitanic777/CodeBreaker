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
            # (OpenWatcom's _Packed: the frame structure c_text writes is of words -- no padding to lose)
            "#define _Packed",
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


# The reconstruction says a callee-pops convention as OpenWatcom's __stdcall, whose arguments go on
# the stack right to left: the C lists them in that order. Microsoft's _pascal pushes them left to
# right, so the same list said as _pascal is backwards -- the Windows API and every pascal function of
# a 1990s module were written with the first-pushed argument first.
_PASCAL = re.compile(r"__stdcall(?:16(?:far|near))?\b[^;(){}=]*?\b(\w+)\s*\(")
_SEGS = ("unaff_SS", "codebreaker_ss()", "unaff_DS", "codebreaker_ds()")


def pascal_names(*texts):
    """the functions the texts declare or define with a callee-pops (__stdcall) convention"""
    return {m.group(1) for t in texts for m in _PASCAL.finditer(t)}


def _close(text, i):
    """the index past the parenthesis that closes the one before `i` (string and character literals
    skipped), or -1"""
    depth = 1
    while i < len(text):
        c = text[i]
        if c in "\"'":
            j = i + 1
            while j < len(text) and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        depth += c == "("
        depth -= c == ")"
        i += 1
        if not depth:
            return i
    return -1


def _arguments(inner):
    """an argument list cut at its top-level commas"""
    out, depth, start, i = [], 0, 0, 0
    while i < len(inner):
        c = inner[i]
        if c in "\"'":
            j = i + 1
            while j < len(inner) and inner[j] != c:
                j += 2 if inner[j] == "\\" else 1
            i = j + 1
            continue
        depth += c in "([{"
        depth -= c in ")]}"
        if c == "," and not depth:
            out.append(inner[start:i])
            start = i + 1
        i += 1
    if inner.strip():
        out.append(inner[start:])
    return out


def _far_pairs(args):
    """an offset followed by the segment register it is in (right-to-left order: the offset is pushed
    last, so it comes first) is one far pointer, as the source had it"""
    out, k = [], 0
    while k < len(args):
        if k + 1 < len(args) and args[k + 1].strip() in _SEGS:
            off = re.sub(r"^\s*\(\s*u?int2\s*\)\s*", "", args[k]).strip()
            out.append(" (void __far *)%s" % (off if off.startswith("&") else "(char __near *)(%s)" % off))
            k += 2
        else:
            out.append(args[k])
            k += 1
    return out


def pascal_order(text, names):
    """`text` with the arguments of every call to, and the parameters of every definition of, a
    function in `names` in Pascal order: first pushed first, an offset and its segment one far pointer."""
    out, i = [], 0
    call = re.compile(r"\b(\w+)\s*\(")
    while True:
        m = call.search(text, i)
        if not m:
            out.append(text[i:])
            return "".join(out)
        end = _close(text, m.end())
        if m.group(1) not in names or end < 0:
            out.append(text[i:m.end()])
            i = m.end()
            continue
        args = _far_pairs([pascal_order(a, names) for a in _arguments(text[m.end():end - 1])])
        args.reverse()
        out.append(text[i:m.end()] + ", ".join(a.strip() for a in args) + ")")
        i = end


_DEREF = re.compile(r"\*\s*\(\s*((?:unsigned |signed )?\w+)\s*\*\s*\)\s*(0x[0-9a-fA-F]+|\d+)\b")


def ds_globals(text):
    """(declarations, text): every read or write through a constant near address, `*(T *)0x63b2`,
    made a variable of the data segment, `codebreaker_d63b2_T`. Microsoft C addresses a variable
    directly (`mov ax, [63b2]`, the offset a fix-up); a constant pointer it loads into a register first
    -- the source had the variable. One variable per address and type."""
    seen = {}

    def var(m):
        typ, at = m.group(1), int(m.group(2), 0)
        if at < 0x10 or typ in ("void", "code"):
            return m.group(0)
        nm = "codebreaker_d%04x_%s" % (at, re.sub(r"\W", "_", typ))
        seen[nm] = typ
        return nm
    out = _DEREF.sub(var, text)
    return "".join("extern %s %s;\n" % (t, n) for n, t in sorted(seen.items())), out


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


def _session(work, header, files, flags, compiler, timeout):
    """One emulator session compiling `files` [(stem, C text)] -> whether it ran to its end."""
    emulator, home = _tools(compiler)
    os.makedirs(work, exist_ok=True)
    for f in os.listdir(work):
        if f.lower().endswith((".obj", ".c", ".txt")):
            os.remove(os.path.join(work, f))
    with open(os.path.join(work, "M.H"), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
        f.write(header)
    lines = ["@echo off"]
    for stem, text in files:
        with open(os.path.join(work, stem + ".C"), "w", newline="\r\n", encoding="latin1", errors="replace") as f:
            f.write('#include "m.h"\n\n' + text + "\n")
        lines.append("cl /nologo /c /W0 %s /Fo%s.OBJ %s.C > %s.TXT" % (flags, stem, stem, stem))
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
    return os.path.isfile(os.path.join(work, "DONE.TXT"))


def _read(path):
    if os.path.isfile(path):
        with open(path, "rb") as f:
            return f.read()
    return None


BATCH = 40          # functions to a source file when each is cut out by name


def compile_many(sources, header, flags, work=None, timeout=900, compiler="msc6", names=None, batch=BATCH):
    """Compile each of `sources` {key: C text} with `header` before it. -> {key: (bytes, mask) or None}
    names: {key: function name} -- each key's code is that public function's (function_code), not the
    object's first code segment.

    With names, `batch` functions share one source file: starting the compiler's passes (CL, C1, C2)
    is most of the cost of compiling a function in the emulator, and Microsoft C compiles each
    function of a file on its own (a near call to a neighbour in the file is resolved without a
    fix-up, but those bytes are the linker's in the original too, and not compared). A file the
    compiler refuses is compiled again one function to a file, so one function cannot sink the rest."""
    from codebreaker.base import cache, settings
    if not available(compiler):
        raise RuntimeError("%s is not set up (settings.json: dosbox_x, %s)" % (compiler, compiler))
    out, todo = {}, {}
    _emulator, home = _tools(compiler)
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
    keys = list(todo)
    built, unsettled = {}, set()             # (unsettled: no verdict -- the session was cut off -- so not cached)
    single, retry = keys, []
    if names and batch > 1 and len(keys) > 1:
        groups = [g for g in ([k for k in keys[i:i + batch] if k in names] for i in range(0, len(keys), batch)) if g]
        files, starts = _batch_files(groups, todo, "B")
        finished = _session(work, header, files, flags, compiler, timeout)
        single = [k for k in keys if k not in names]
        for i, g in enumerate(groups):
            obj = _read(os.path.join(work, "B%d.OBJ" % i))
            if obj is not None:
                built.update((k, function_code(obj, names[k])) for k in g)
            elif not finished:
                unsettled.update(g)
            else:                                    # refused: the functions its errors are in go on their
                bad = _refused(os.path.join(work, "B%d.TXT" % i), starts[i])     # own, the rest together
                single += bad or g
                rest = [k for k in g if k not in bad] if bad else []
                if rest:
                    retry.append(rest)
    if single or retry:
        files, _ = _batch_files(retry, todo, "R")
        files += [("F%d" % j, todo[k][1]) for j, k in enumerate(single)]
        finished = _session(work, header, files, flags, compiler, timeout)
        for i, g in enumerate(retry):
            obj = _read(os.path.join(work, "R%d.OBJ" % i))
            if obj is not None:
                built.update((k, function_code(obj, names[k])) for k in g)
            else:
                unsettled.update(g)                  # (refused again: an error the first file's hid)
        for j, k in enumerate(single):
            obj = _read(os.path.join(work, "F%d.OBJ" % j))
            if obj is None:
                built[k] = None
                if not finished:
                    unsettled.add(k)
            elif names and k in names:
                built[k] = function_code(obj, names[k])
            else:
                code, mask = object_code(obj)
                built[k] = (code, mask) if code else None
    for key, (k, _text) in todo.items():
        got = out[key] = built.get(key)
        if got:
            cache.put("msc6", k, len(got[0]).to_bytes(4, "little") + got[0] + got[1])
        elif key in built and key not in unsettled:
            cache.put("msc6", k, b"-")                       # the compiler refused it (not: the session was cut off)
    return out


def _batch_files(groups, todo, prefix):
    """([(stem, text)], [[(first line, key)]]): each group's functions in one file, and the line each
    begins on (the file starts with the #include and a blank line)"""
    files, starts = [], []
    for i, g in enumerate(groups):
        at, here = 3, []
        for k in g:
            here.append((at, k))
            at += todo[k][1].count("\n") + 2         # (its lines, and the blank one between)
        files.append(("%s%d" % (prefix, i), "\n\n".join(todo[k][1] for k in g)))
        starts.append(here)
    return files, starts


def _refused(path, starts):
    """the keys of the functions a refused file's errors are in -- [] when an error is not in the file
    itself (the header's: then none can be told from another)"""
    try:
        text = open(path, encoding="latin1").read()
    except OSError:
        return []
    bad = []
    for name, line in re.findall(r"^(\S+)\((\d+)\) : (?:fatal )?error", text, re.M):
        if not re.match(r"[BR]\d+\.C$", os.path.basename(name), re.I):
            return []
        at = int(line)
        k = max((s for s in starts if s[0] <= at), default=None)
        if k and k[1] not in bad:
            bad.append(k[1])
    return bad
