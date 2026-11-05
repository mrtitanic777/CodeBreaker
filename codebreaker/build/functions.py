"""functions.py -- compile a module's reconstructed functions and link a DLL from them.

One builder for every kind of module:

    dll, where, dropped = functions.build16(header, texts, order, imports, outdir, segments)
    dll, where, dropped = functions.build32(module_path, header, texts, order, imports, outdir, cflags)
    dll, where, dropped = functions.build_powerpc(header, texts, order, externs, outdir, base)
        where    {function name: where it is in the DLL} -- (segment, offset) for 16-bit,
                 an address for 32-bit
        dropped  {function name: why the compiler rejected it}

Each function is compiled on its own, against a header that declares every function's
convention and result. A function the compiler rejects cannot take others with it, it is known
exactly which one failed and why, and the compiles run side by side. A rejected function is
replaced by a stand-in with the same signature, so its callers still link (a match for such a
caller is then marked as weaker: it ran against a stand-in).

A 16-bit DLL is linked without the C runtime's start-up and with string constants in the code
segment, so it has no data of its own: when it is run for judging, its data segment is given
the original module's data and every global is then where the original had it.
"""
import os
import re
import struct
import subprocess

from codebreaker import platforms
from codebreaker.build import watcom as period_tools
from codebreaker.formats import omf
from codebreaker.jobs import pool

# Medium model (far code, near data), as the module build has it (build/module.py): the C
# runtime's helpers (long multiply, divide, shifts) are then called far, so a function reaches them
# from whichever physical segment the linker packs it into. In the small model they were called
# near, and a function packed into another 64K segment than the helpers could not be linked at all
# (51 functions of the 16-bit golden modules; which ones moved whenever code grew).
# -ecd: a function declared without a convention is __stdcall, the decompiler's default
CFLAGS16 = ["-bt=windows", "-mm", "-ecd", "-zu", "-zc", "-zl", "-zq", "-w0", "-s", "-3", "-zfp", "-zgp", "-zastd=c99", "-e=20",
            "-j"]   # (char is signed, as the decompiler means it: OpenWatcom's own is unsigned, and a byte
                    # compared with -1 never matched -- USER's dialog-item builder missed an icon's FFh mark)
_ERR = re.compile(r"\((\d+)\): Error! (E\d+: [^\r\n]*)")
_SYM = re.compile(r"(?m)^([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4,8})[+*]?\s+(\S+)\s*$")


def prototype(text):
    """The declaration of the function a text defines (everything before its body)."""
    head = text.split("{", 1)[0]
    head = head[:head.index(")") + 1] if ")" in head else head      # without old-style argument lines
    return " ".join(head.split()) + ";"


def stand_in(text):
    """A definition with the same signature that does nothing."""
    head = prototype(text).rstrip(";")
    # (with the old-style argument lines kept: a parameter they declare, left out, is an int --
    # not what the header's prototype of the function says)
    whole = text.split("{", 1)[0].rstrip()
    return whole + ("\n{\n}\n" if head.split()[0] == "void" else "\n{\n  return 0;\n}\n")


def _plain(symbol):
    """The C name behind a linker symbol (`_NAME@8`, `NAME_`, `_NAME`)."""
    return symbol.split("@")[0].strip("_")


def _adjust_flags(cflags, text):
    """The options one function's text asks for on top of the module's -- the Windows prologue for
    an export, the frame kept for a function that reads where SP stood on entry."""
    if "__export " in text and "-zW" not in cflags:
        cflags = list(cflags) + ["-zW"]        # (the Windows prologue, which the loader rewrites for an export)
    if ("codebreaker_entry_sp(" in text or "codebreaker_stack(" in text) and "-of+" not in cflags:
        cflags = list(cflags) + ["-of+"]       # (it reads the frame: BP+2 is where SP stood on entry)
    return cflags


def _run_compile(cc, cflags, env, outdir, stem, text):
    """Compile one function, running the compiler (no cache). `cflags` are already adjusted
    (_adjust_flags). -> (why, keepable): why is "" on success, else the first error; keepable says
    whether the outcome may be remembered (a rejection with a line is; a compiler that fell over is
    not). On success the object is at outdir/stem.obj."""
    with open(os.path.join(outdir, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
        f.write('#include "m.h"\n\n' + text + "\n")
    for ext in (".obj", ".err"):
        try:
            os.remove(os.path.join(outdir, stem + ext))
        except OSError:
            pass
    r = subprocess.run([cc] + cflags + [stem + ".c"], cwd=outdir, env=env, capture_output=True, text=True,
                       errors="replace", timeout=600)
    if r.returncode == 0 and os.path.isfile(os.path.join(outdir, stem + ".obj")):
        return "", True
    log = (r.stdout or "") + (r.stderr or "")
    try:
        log += open(os.path.join(outdir, stem + ".err"), errors="replace").read()
    except OSError:
        pass
    m = _ERR.search(log)
    why = (m.group(2) if m else log.strip()[-100:] or "the compiler failed")[:100]
    if m:                                        # with the line it is about
        src = ['#include "m.h"', ""] + text.splitlines()
        n = int(m.group(1))
        if 0 < n <= len(src):
            why += " -- " + src[n - 1].strip()[:200]
    return why, bool(m)


_TOOL_INPUTS = {}


def _dos_stamp(path):
    """(time, date) as DOS keeps them, of a file's modification (OpenWatcom's dependency record)."""
    import time
    t = time.gmtime(os.stat(path).st_mtime)           # (OpenWatcom's C library: with no TZ, its local time is UTC)
    return ((t.tm_hour << 11) | (t.tm_min << 5) | (t.tm_sec // 2),
            ((t.tm_year - 1980) << 9) | (t.tm_mon << 5) | t.tm_mday)


def omf_here(obj, folder):
    """An OpenWatcom object kept from a run in another folder, as the tool writes it run in
    `folder` -> bytes, or None when it cannot be said (then the tool is run). wasm and wcc name
    the source by its full path (THEADR: <the folder they ran in>\\<stem>.asm or .c) and, in
    their dependency records (COMENT class E9h), each file they read with its date: the folder
    named is the one of now, the date that of the file there now (written before this is asked),
    and every record's checksum again as the tool makes it."""
    if os.environ.get("TZ"):                             # (its dates are then of that zone: the tool says)
        return None
    recs, i, n = [], 0, len(obj)
    while i + 3 <= n:
        end = i + 3 + (obj[i + 1] | (obj[i + 2] << 8))
        if end > n or end < i + 4:
            return None
        recs.append((obj[i], bytearray(obj[i + 3:end - 1]), obj[end - 1]))
        i = end
    if i != n or not recs or recs[0][0] != 0x80 or not recs[0][1]:
        return None
    name = bytes(recs[0][1][1:1 + recs[0][1][0]])
    cut = name.rfind(b"\\")
    if cut < 0:
        return None
    was, now = name[:cut], os.path.abspath(folder).encode("latin1", "replace")
    out = bytearray()
    for kind, body, check in recs:
        if kind == 0x80:
            if len(now) + len(name) - cut > 255:
                return None
            body = bytearray([len(now) + len(name) - cut]) + now + name[cut:]
        elif kind == 0x88 and len(body) > 7 and body[1] == 0xE9:
            dep = bytes(body[7:7 + body[6]])
            if dep.startswith(was + b"\\"):
                dep = now + dep[len(was):]
                if len(dep) > 255:
                    return None
                there = dep.decode("latin1")
                if not os.path.isfile(there):
                    return None
                tm, dt = _dos_stamp(there)
                body = bytearray(body[:2]) + struct.pack("<HH", tm, dt) + bytes([len(dep)]) + dep + body[7 + body[6]:]
        rec = bytearray([kind]) + struct.pack("<H", len(body) + 1) + body
        out += rec + bytes([(-sum(rec)) & 0xFF if check else 0])
    return bytes(out)


_HERE = "\x00here\x00"


def message_kept(msg, folder):
    """A tool's message as the shared build cache keeps it: the folder it ran in (which a
    message may name) as a mark, put back by message_here -- bytes."""
    return msg.replace(os.path.abspath(folder), _HERE).encode("utf-8", "replace")


def message_here(kept, folder):
    """message_kept's message, naming `folder` as the tool run there names it."""
    return kept.decode("utf-8", "replace").replace(_HERE, os.path.abspath(folder))


def tool_inputs(cc, env, variable):
    """What a compiler reads besides its command line and the source, by content: the program's
    bytes, the headers it may include (every file of the INCLUDE folders, by its bytes) and the
    environment variable of options it reads too (OpenWatcom's WCC, WASM)."""
    from codebreaker.base import cache
    env = env or {}
    k = (cc, env.get("INCLUDE", ""), env.get(variable, ""))
    if k not in _TOOL_INPUTS:
        folders = [d for d in env.get("INCLUDE", "").split(os.pathsep) if d and os.path.isdir(d)]
        _TOOL_INPUTS[k] = cache.key(cache.tool_bytes(cc), cache.files_bytes(*folders), variable, env.get(variable, ""))
    return _TOOL_INPUTS[k]


def compile_kept(cc, cflags, env, outdir, stem, text, header_key):
    """_run_compile, kept in the shared build cache (base/cache.SHARED, kind "objects"). `cflags`
    are already adjusted. -> (why, keepable), the object at outdir/stem.obj on success.
    The key is every input of the object: the compiler, its INCLUDE headers and its WCC variable
    (tool_inputs), the options, the header m.h the function includes (`header_key`, the digest of
    its text), the function's text, and the stem (the object names its source file, stem.c, and
    an error names it too). The object also carries m.h's date (the compiler's dependency record),
    which is not an input of what is linked: a kept object has the date of the first compile."""
    from codebreaker.base import cache
    k = cache.key("wcc-object", tool_inputs(cc, env, "WCC"), list(cflags), header_key, stem, text) \
        if header_key else None
    had = cache.get("objects", k) if k else None
    if had is not None:
        with open(os.path.join(outdir, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
            f.write('#include "m.h"\n\n' + text + "\n")
        if had[:2] == b"ER":
            return message_here(had[2:], outdir), True
        here = omf_here(had[2:], os.path.dirname(os.path.join(outdir, stem + ".c")))  # (as wcc writes it here, now)
        if here is not None:
            with open(os.path.join(outdir, stem + ".obj"), "wb") as f:
                f.write(here)
            return "", True
    why, keep = _run_compile(cc, cflags, env, outdir, stem, text)
    if not why:
        if k:
            with open(os.path.join(outdir, stem + ".obj"), "rb") as f:
                cache.put("objects", k, b"OK" + f.read())
    elif k and keep:                             # a rejection is kept; a compiler that fell over is not
        cache.put("objects", k, b"ER" + message_kept(why, outdir))
    return why, keep


def _compile(cc, cflags, env, outdir, stem, text):
    """Compile one function. -> "" or the first error. What the compiler made of this text
    with this header and these options is kept (compile_kept: the shared build cache) and not
    made again. (The engine's function.object step goes through compile_kept too.)"""
    return compile_kept(cc, _adjust_flags(cflags, text), env, outdir, stem, text, _HEADER.get(outdir))[0]


_NEVER_SET = re.compile(r"W200: '(\w+)' has been referenced but never assigned a value")


def _run_never_set(cc, cflags, env, outdir, stem, text):
    """never_set, running the compiler (no cache). -> [name, ...]"""
    flags = [f for f in cflags if not f.startswith("-w")] + ["-w2", "-zs"]
    name = stem + "_w.c"
    with open(os.path.join(outdir, name), "w", newline="\n", encoding="utf-8") as f:
        f.write('#include "m.h"\n\n' + text + "\n")
    r = subprocess.run([cc] + flags + [name], cwd=outdir, env=env, capture_output=True, text=True,
                       errors="replace", timeout=600)
    try:
        os.remove(os.path.join(outdir, name))
    except OSError:
        pass
    return sorted(set(_NEVER_SET.findall((r.stdout or "") + (r.stderr or ""))))


def never_set(cc, cflags, env, outdir, stem, text):
    """The variables a function reads that nothing in it ever sets, as the compiler sees them
    (W200, its flow check, which the build's -w0 keeps quiet). -> [name, ...]
    One the decompiler made up -- a register on entry (`in_AX`, `unaff_ES`), or a stack slot it
    split off from a buffer whose address it passes on (`iStack_1b` beside `axStack_34`) -- is a
    value the original got from somewhere this C does not: what it reads is whatever is there.
    (The engine's function.never_set step is its own cache: it calls _run_never_set -- engine/win16.py.)"""
    return never_set_kept(cc, cflags, env, outdir, stem, text, _HEADER.get(outdir))


def never_set_kept(cc, cflags, env, outdir, stem, text, header_key):
    """_run_never_set, kept in the shared build cache (kind "never_set"). The key: the compiler,
    its INCLUDE headers and WCC variable (tool_inputs), the options it is run with, the header's
    digest, the function's text (the names come out of its warnings: the stem is not in them)."""
    from codebreaker.base import cache
    flags = [f for f in cflags if not f.startswith("-w")] + ["-w2", "-zs"]
    k = cache.key("wcc-never-set", tool_inputs(cc, env, "WCC"), flags, header_key, text) if header_key else None
    had = cache.get("never_set", k) if k else None
    if had is not None:
        return [s for s in had.decode().split(",") if s]
    found = _run_never_set(cc, cflags, env, outdir, stem, text)
    if k:
        cache.put("never_set", k, ",".join(found).encode())
    return found


_HEADER = {}                     # build folder -> the hash of the header in it
OBJECTS = {}                     # build folder -> {function: digest of its object code}, of the last build there


def object_digest(path):
    """A digest of what a compiled object file IS -- its code, data, the places to patch and the
    names it defines and refers to -- and of nothing else: not the source file's name or date,
    nor the compiler's notes. Two compilations that give the same machine code give the same
    digest, whatever was changed in the source to no effect. None when there is no file."""
    import hashlib
    try:
        with open(path, "rb") as f:
            data = f.read()
    except OSError:
        return None
    h = hashlib.sha1()
    if data[:1] == bytes([0x80]):                            # OMF: records; skip the header and comments
        p = 0
        while p + 3 <= len(data):
            kind = data[p]
            size = int.from_bytes(data[p + 1:p + 3], "little")
            if kind not in (0x80, 0x88, 0x8A, 0x8B):
                h.update(data[p:p + 3 + size - 1])           # (without the record's checksum)
            p += 3 + size
    else:                                                    # COFF: all but the time it was made
        h.update(data[:4])
        h.update(data[8:])
    return h.hexdigest()


def _note_objects(outdir, stems, dropped, gone=()):
    OBJECTS[outdir] = out = {}
    for n, stem in stems.items():
        out[n] = ("stand-in:" if n in dropped else "") + (object_digest(os.path.join(outdir, stem + ".obj")) or "none")
    for n in gone:
        out[n] = "the original's"


def compile_all(cc, cflags, env, header, texts, order, outdir, jobs=None, more=None):
    """Compile every function. -> ({name: object file stem}, {name: why it was rejected}).
    `more` gives switches of a function's own: {name: [switch, ...]}."""
    return compile_groups(cc, env, header, texts, [(cflags, order, outdir)], jobs=jobs, more=more)[0]


def compile_groups(cc, env, header, texts, groups, jobs=None, more=None):
    """compile_all for several folders at once -- `groups` [(cflags, order, outdir)], a 16-bit
    module's code segments each into a folder of its own -- every function of every folder in
    the one pool, so that a segment of a few functions does not leave the compilers idle while
    it ends. -> [({name: object file stem}, {name: why it was rejected})], one for each group."""
    more = more or {}
    from codebreaker.base import cache, parallel
    jobs = jobs or parallel.workers()
    work = []
    for cflags, order, outdir in groups:
        os.makedirs(outdir, exist_ok=True)
        _HEADER[outdir] = cache.key(header)
        with open(os.path.join(outdir, "m.h"), "w", newline="\n", encoding="utf-8") as f:
            f.write(header)
        stems = {n: "f%04d" % i for i, n in enumerate(order)}
        work += [(cflags, outdir, stems, n) for n in order]
    # (the largest first: a long function started last would be the one the rest wait for)
    order_run = sorted(range(len(work)), key=lambda i: -len(texts[work[i][3]]))
    got = pool.threads(lambda i: _compile(cc, work[i][0] + more.get(work[i][3], []), env, work[i][1],
                                          work[i][2][work[i][3]], texts[work[i][3]]), order_run, jobs=jobs)
    errors = dict(zip(order_run, got))
    out, k = [], 0
    for cflags, order, outdir in groups:
        stems = work[k][2] if order else {}
        dropped = {n: errors[k + i] for i, n in enumerate(order) if errors[k + i]}
        k += len(order)
        for n in list(dropped):                              # callers of a rejected function still link
            if _compile(cc, cflags + more.get(n, []), env, outdir, stems[n], stand_in(texts[n])):
                stems.pop(n)                                 # not even a stand-in: left unresolved
        out.append((stems, dropped))
    return out


_LINKED = {}                     # build folder -> what the linker said last


def _link(wlink, env, outdir, lines):
    with open(os.path.join(outdir, "m.lnk"), "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    for stale in ("m.dll", "m.map"):
        try:
            os.remove(os.path.join(outdir, stale))
        except OSError:
            pass
    r = subprocess.run([wlink, "@m.lnk"], cwd=outdir, env=env, capture_output=True, text=True,
                       errors="replace", timeout=900)
    dll = os.path.join(outdir, "m.dll")
    _LINKED[outdir] = (r.stdout or "") + (r.stderr or "")
    if os.path.isfile(dll):
        return dll, ""
    log = ((r.stdout or "") + (r.stderr or "")).strip()
    errors = [ln.strip() for ln in log.splitlines() if "Error!" in ln]      # not the warnings around them
    return None, (errors[0][:200] if errors else log[-160:]) or "the linker produced nothing"


def _interrupt_routines(wcc, env, outdir, interrupts):
    """The link lines for the routines the interrupt helpers call (decompile/c_text.py
    interrupt_routines, as build/module.py adds them) when the module's C takes an interrupt
    (the helpers are in the header): without them a helper's call went to 0:0 and its registers
    to offset 0 of the data segment."""
    if not interrupts:
        return []
    from codebreaker.decompile import c_text
    asm, c = c_text.interrupt_routines(interrupts)
    with open(os.path.join(outdir, "interrupts.asm"), "w", newline="\n") as f:
        f.write(asm)
    r = subprocess.run([platforms.of("ne").packager.assembler(), "-q", "interrupts.asm"], cwd=outdir, env=env,
                       capture_output=True, text=True)
    if r.returncode or _compile(wcc, CFLAGS16, env, outdir, "interrupt_registers", c.replace('#include "m.h"\n', "")):
        return []
    return ["file interrupts.obj", "file interrupt_registers.obj"]


def build16(header, texts, order, imports, outdir, segments=None, jobs=None, code_segment=None, interrupts=None,
            code_data=None, code_image=None):
    """`code_segment` {name: the number of the original's segment the function is in}: the
    functions of one segment are put in one segment again. A segment holds 64K at most, and
    functions of one segment call each other near, of different ones far -- as in the original.
    `interrupts` {vector}: the software interrupts the texts take (c_text: m.interrupts).
    `code_data` {segment: C}: the original's data its functions read through CS (c_text:
    m.code_data), compiled into that code segment. `code_image` {segment: bytes}: the original's
    whole segment, put at the start of the rebuilt one -- for C that still reads its code segment
    at the original's offsets (c_text: m.code_reads "left"), so that what it reads there is what
    the original has; the functions come after it. Left out again when the segment would not fit."""
    P = platforms.of("ne")                                    # (16-bit Windows: its compiler and packager)
    wcc, wlink = P.compiler.tool(), P.packager.linker()
    if not (wcc and wlink):
        raise RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)")
    env = P.compiler.env()
    more = {n: ["-nt=CODE%d" % s] for n, s in (code_segment or {}).items()}
    stems, dropped = compile_all(wcc, CFLAGS16, env, header, texts, order, outdir, jobs, more)
    live = [n for n in order if n not in dropped]
    # One initialised byte, so the DLL has a data segment at all: that segment is where the
    # original's data is put when the DLL is run for judging.
    data = ["char codebreaker_data_segment = 1;"]
    for n, image in sorted((segments or {}).items()):        # other segments the code reads from
        data.append('uint1 __based(__segname("CODEBREAKER_SEGMENT_%d")) codebreaker_segment_%d[] = {\n%s\n};' % (n, n, ",\n".join(
            ",".join(str(x) for x in image[i:i + 32]) for i in range(0, len(image), 32))))
    _compile(wcc, CFLAGS16, env, outdir, "data", "\n".join(data))
    lines = ["system windows dll", "name m.dll", "option quiet, map, nodefaultlibs, caseexact, undefsok",
             "libpath %s" % period_tools.ow_lib("ne")]
    images = []                                              # (first in their segments: at offset 0)
    for seg, image in sorted((code_image or {}).items()):
        if not _compile(wcc, CFLAGS16 + ["-nt=CODE%d" % seg], env, outdir, "image%02d" % seg,
                        'uint1 __based(__segname("_CODE")) codebreaker_code_image_%d[] = {\n%s\n};' % (seg, ",\n".join(
                            ",".join(str(x) for x in image[i:i + 32]) for i in range(0, len(image), 32)))):
            images.append((seg, "file image%02d.obj" % seg))
    if images:                                               # (each its own segment, not packed after another)
        lines.append("option packcode=0")
    lines += [line for _seg, line in images]
    lines += ["file %s.obj" % stems[n] for n in order if n in stems]
    for seg, text in sorted((code_data or {}).items()):     # (beside the functions of its segment)
        if not _compile(wcc, CFLAGS16 + ["-nt=CODE%d" % seg], env, outdir, "code%02d" % seg, text):
            lines.append("file code%02d.obj" % seg)
    lines += ["file data.obj", "library clibm.lib"]
    lines += _interrupt_routines(wcc, env, outdir, interrupts)
    # a C name compiled as __stdcall with an open argument list is the symbol `_name`
    lines += ["import _%s %s.%d" % (nm, mod, ordn) for nm, (mod, ordn) in sorted(imports.items())]
    dll, why = _link(wlink, env, outdir, lines)
    # A segment holds 64K. Where the rebuilt functions of one of the original's segments come to
    # more (other compiler; or the decompiler took a table for code and wrote pages of it), the
    # largest are replaced by stand-ins until the rest fit: those few are not built, the
    # others are judged.
    by_stem = {stem: n for n, stem in stems.items()}
    for _attempt in range(80):
        # ... and a function that reaches near for something that is in another segment (the
        # original called it far; the decompiler's text has lost that) cannot be linked at all
        apart = re.search(r"file (f[0-9]+)[.]obj.*not in the same segment", why or "")
        if not dll and apart and by_stem.get(apart.group(1)) in live:
            n = by_stem[apart.group(1)]
            if _compile(wcc, CFLAGS16 + more.get(n, []), env, outdir, stems[n], stand_in(texts[n])):
                break
            dropped[n] = "reaches near for something in another segment"
            live = [x for x in live if x != n]
            dll, why = _link(wlink, env, outdir, lines)
            continue
        over = re.search(r"size of segment CODE(\d+) exceeds 64k by (\d+) bytes", why or "")
        if dll or not over or not code_segment:
            break
        seg, need = int(over.group(1)), int(over.group(2)) + 512
        image = next((line for sg, line in images if sg == seg and line in lines), None)
        if image:                                            # (the original's image goes first)
            lines.remove(image)
            dll, why = _link(wlink, env, outdir, lines)
            continue
        sized = sorted(((os.path.getsize(os.path.join(outdir, stems[n] + ".obj")), n) for n in live
                        if code_segment.get(n) == seg and n in stems and n not in dropped), reverse=True)
        freed = 0
        for size, n in sized:
            if freed >= need:
                break
            if not _compile(wcc, CFLAGS16 + more.get(n, []), env, outdir, stems[n], stand_in(texts[n])):
                freed += size
                dropped[n] = "too large: the functions of its segment do not fit in 64K when rebuilt"
        if not freed:
            break
        live = [n for n in live if n not in dropped]
        dll, why = _link(wlink, env, outdir, lines)
    _note_objects(outdir, stems, dropped)
    if not dll:
        return None, {}, dict(dropped, **{n: "link: " + why for n in live})
    where = {}
    try:
        text = open(os.path.join(outdir, "m.map"), errors="replace").read()
    except OSError:
        text = ""
    live_set = set(live)
    for mm in _SYM.finditer(text):
        n = _plain(mm.group(3))
        if n in live_set and n not in where:
            where[n] = (int(mm.group(1), 16), int(mm.group(2), 16))
    return dll, where, dropped


def _in_the_original(outdir, env, undefined, originals):
    """An object file in which each name in `undefined` that is a function of the original
    (`originals` {name: address}, and `func_0x<address>`) is a jump to it. -> its name, or None.

    A function that is called but is not in the build -- the compiler rejected its text, the
    decompiler gave none, the decompiler did not know it was a function -- used to be nothing at
    all, and every caller that reached it "called into nothing" and was counted as behaving
    differently. The original's code is in memory beside the rebuilt image whenever that is run,
    so the call can simply go there: the caller is then judged with its real callee."""
    lines = [".386", ".model flat", "_TEXT segment dword public use32 'CODE'"]
    n = 0
    for symbol in sorted(set(undefined)):
        plain = _plain(symbol)
        at = originals.get(plain)
        named = re.fullmatch(r"func_0x([0-9a-fA-F]{6,8})", plain)
        if at is None and named:
            at = int(named.group(1), 16)
        if at is None or not re.fullmatch(r"[A-Za-z_@?$][\w@?$]*", symbol):
            continue
        lines += ["public %s" % symbol, "%s:" % symbol, "        push 0%08Xh" % at, "        ret"]
        n += 1
    if not n:
        return None
    lines += ["_TEXT ends", "end"]
    with open(os.path.join(outdir, "original.asm"), "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    wasm = platforms.of("pe").packager.assembler()
    if not wasm:
        return None
    try:
        os.remove(os.path.join(outdir, "original.obj"))
    except OSError:
        pass
    subprocess.run([wasm, "-zq", "-fo=original.obj", "original.asm"], cwd=outdir, env=env, capture_output=True,
                   text=True, errors="replace", timeout=300)
    return "original.obj" if os.path.isfile(os.path.join(outdir, "original.obj")) else None


def build32(module_path, header, texts, order, used_imports, outdir, cflags, jobs=None, originals=None):
    from codebreaker.build import watcom as B
    from codebreaker.formats.pe import open_pe
    ow = B._ow()
    if not os.path.exists(ow["wcc"]):
        raise RuntimeError("OpenWatcom's 32-bit compiler is not set up (settings.json: openwatcom)")
    env = B._ow_env(ow)
    imports = B.import_map(module_path)
    from codebreaker.decompile import describe
    numbered = describe.BY_NUMBER.get(module_path) or {}
    stems, dropped = compile_all(ow["wcc"], list(cflags) + ["-e=20"], env, header, texts, order, outdir, jobs)
    live = [n for n in order if n not in dropped]
    gone = [n for n in dropped if originals and n in originals and n in stems]
    for n in gone:                                           # no stand-in where the original itself can be called
        stems.pop(n, None)
    _note_objects(outdir, stems, dropped, gone)
    # undefsok: a call to a function that is not there is left unresolved (it then fails its test)
    lines = ["system nt_dll", "name m.dll", "option quiet", "option undefsok", B._BASE.strip()]
    lines += ["file %s.obj" % stems[n] for n in order if n in stems]
    lines += [x for x in B._RTL.strip().split("\n") if x]
    lines.append("library math387r.lib")                     # sin, sqrt ... for a function that uses them
    seen, exports = set(), []
    for n in order:
        if n not in stems:
            continue
        obj = open(os.path.join(outdir, stems[n] + ".obj"), "rb").read()
        for line in B._import_lines(obj, set(used_imports), imports, numbered).strip().split("\n"):
            if line and line not in seen:
                seen.add(line)
                lines.append(line)
        if n in dropped:
            continue
        # the public symbol as the compiler decorated it: _name (cdecl), _name@N (stdcall)
        sym = next((s for s in omf.publics(obj) if s == "_" + n or s == n or
                    re.fullmatch(r"[_@]%s@\d+" % re.escape(n), s)), None)
        if sym:
            exports.append("export %s='%s'" % (n, sym))
    dll, why = _link(ow["wlink"], env, outdir, lines + exports)
    if dll and originals:
        missing = re.findall(r"W1028: (\S+) is an undefined reference", _LINKED.get(outdir, ""))
        there = _in_the_original(outdir, env, missing, originals) if missing else None
        if there:
            dll, why = _link(ow["wlink"], env, outdir, lines + ["file " + there] + exports)
    if not dll:
        return None, {}, dict(dropped, **{n: "link: " + why for n in live})
    pb = open_pe(dll)
    at = pb.export_addrs()
    return dll, {n: pb.image_base + at[n] for n in live if n in at}, dropped


# PowerPC ---------------------------------------------------------------------------------------
CFLAGS_POWERPC = ["/nologo", "/c", "/TC", "/O2", "/GS-", "/Zl", "/Gy", "/W0", "/FIm.h"]
_CL_ERR = re.compile(r"\((\d+)\)\s*:\s*(?:fatal )?error (C\d+: [^\r\n]*)")
_MAP_SYM = re.compile(r"(?m)^ [0-9A-Fa-f]{4}:[0-9A-Fa-f]{8}\s+(\S+)\s+([0-9A-Fa-f]{8}) ")


def powerpc_tools():
    """(compiler, linker, runtime library), or None when settings.json does not name them."""
    from codebreaker.base import settings
    d, lib = settings.get("powerpc_tools"), settings.get("powerpc_lib")
    cl, link = os.path.join(d or "", "cl.exe"), os.path.join(d or "", "link.exe")
    return (cl, link, lib) if d and os.path.isfile(cl) and os.path.isfile(link) and os.path.isfile(lib or "") else None


def _compile_powerpc(cl, outdir, stem, text):
    from codebreaker.base import cache
    # (the key, every input of the object: the compiler by its bytes and its folder -- cl.exe runs
    # c1/c2 beside it -- the INCLUDE headers and CL variable it reads, the options, the header,
    # the stem the object and an error name, and the text; kept in the shared build cache)
    k = cache.key("cl-ppc-object", tool_inputs(cl, os.environ, "CL"), cache.folder(os.path.dirname(cl)),
                  CFLAGS_POWERPC, _HEADER[outdir], stem, text) if outdir in _HEADER else None
    had = cache.get("objects", k) if k else None
    if had is not None:
        if had[:2] == b"ER":
            return message_here(had[2:], outdir)
        with open(os.path.join(outdir, stem + ".obj"), "wb") as f:
            f.write(had[2:])
        return ""
    with open(os.path.join(outdir, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
        f.write(text + "\n")
    try:
        os.remove(os.path.join(outdir, stem + ".obj"))
    except OSError:
        pass
    r = subprocess.run([cl] + CFLAGS_POWERPC + ["/Fo" + stem + ".obj", stem + ".c"], cwd=outdir,
                       capture_output=True, text=True, errors="replace", timeout=600)
    if r.returncode == 0 and os.path.isfile(os.path.join(outdir, stem + ".obj")):
        if k:
            with open(os.path.join(outdir, stem + ".obj"), "rb") as f:
                cache.put("objects", k, b"OK" + f.read())
        return ""
    log = (r.stdout or "") + (r.stderr or "")
    m = _CL_ERR.search(log)
    why = (m.group(2) if m else log.strip()[-100:] or "the compiler failed")[:100]
    if k and m:
        cache.put("objects", k, b"ER" + message_kept(why, outdir))
    return why


def build_powerpc(header, texts, order, externs, outdir, base, jobs=None):
    """Compile each function with the PowerPC compiler and link one image at `base`.
    `externs` are the names of functions outside the module (imports): each gets a place of its
    own in the image, so a call to it can be seen when the image is run.
    -> (image path, {name: address} for functions and externs, {name: why it was rejected})"""
    tools = powerpc_tools()
    if not tools:
        raise RuntimeError("no PowerPC compiler is set up (settings.json: powerpc_tools, powerpc_lib)")
    cl, link, lib = tools
    from codebreaker.base import cache, parallel
    jobs = jobs or parallel.workers()
    os.makedirs(outdir, exist_ok=True)
    _HEADER[outdir] = cache.key(header)
    with open(os.path.join(outdir, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header)
    stems = {n: "f%04d" % i for i, n in enumerate(order)}
    errors = pool.threads(lambda n: _compile_powerpc(cl, outdir, stems[n], texts[n]), order, jobs=jobs)
    dropped = {n: e for n, e in zip(order, errors) if e}
    for n in list(dropped):
        if _compile_powerpc(cl, outdir, stems[n], stand_in(texts[n])):
            stems.pop(n)
    # every extern is a function of its own that does nothing; running stops at its address
    _compile_powerpc(cl, outdir, "externs", "\n".join(
        "#pragma optimize(\"\", off)\nuint8 %s() { return %d; }" % (n, i) for i, n in enumerate(sorted(externs))))
    _note_objects(outdir, stems, dropped)
    objs = [stems[n] + ".obj" for n in order if n in stems] + ["externs.obj"]
    with open(os.path.join(outdir, "m.rsp"), "w", newline="\n") as f:
        f.write("\n".join(objs) + "\n")
    for stale in ("m.dll", "m.map"):
        try:
            os.remove(os.path.join(outdir, stale))
        except OSError:
            pass
    r = subprocess.run([link, "/nologo", "/DLL", "/NOENTRY", "/NODEFAULTLIB", "/FIXED", "/XEX:NO", "/OPT:NOREF",
                        "/OPT:NOICF", "/FORCE", "/IGNORE:4006,4088", "/MAP:m.map", "/BASE:0x%X" % base, "/OUT:m.dll",
                        "@m.rsp", lib], cwd=outdir, capture_output=True, text=True, errors="replace", timeout=900)
    dll = os.path.join(outdir, "m.dll")
    live = [n for n in order if n not in dropped]
    if not os.path.isfile(dll):
        why = ((r.stdout or "") + (r.stderr or "")).strip()[-160:] or "the linker produced nothing"
        return None, {}, dict(dropped, **{n: "link: " + why for n in live})
    try:
        text = open(os.path.join(outdir, "m.map"), errors="replace").read()
    except OSError:
        text = ""
    want = set(order) | set(externs)
    where = {}
    for mm in _MAP_SYM.finditer(text):
        if mm.group(1) in want and mm.group(1) not in where:
            where[mm.group(1)] = int(mm.group(2), 16)
    return dll, where, dropped
