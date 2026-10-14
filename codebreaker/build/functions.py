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
import subprocess
from concurrent.futures import ThreadPoolExecutor

from codebreaker.build import watcom as period_tools

# -ecd: a function declared without a convention is __stdcall, the decompiler's default
CFLAGS16 = ["-bt=windows", "-ms", "-ecd", "-zu", "-zc", "-zl", "-zq", "-w0", "-s", "-3", "-zastd=c99", "-e=20"]
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
    return head + ("\n{\n}\n" if head.split()[0] == "void" else "\n{\n  return 0;\n}\n")


def _plain(symbol):
    """The C name behind a linker symbol (`_NAME@8`, `NAME_`, `_NAME`)."""
    return symbol.split("@")[0].strip("_")


def _compile(cc, cflags, env, outdir, stem, text):
    """Compile one function. -> "" or the first error. What the compiler made of this text
    with this header and these options is kept (base/cache.py) and not made again."""
    from codebreaker.base import cache
    if "codebreaker_entry_sp(" in text and "-of+" not in cflags:
        cflags = list(cflags) + ["-of+"]       # (it reads the frame: BP+2 is where SP stood on entry)
    k = cache.key(cache.tool(cc), cflags, _HEADER.get(outdir, ""), text) if outdir in _HEADER else None
    had = cache.get("objects", k) if k else None
    if had is not None:
        if had[:2] == b"ER":
            return had[2:].decode("utf-8", "replace")
        with open(os.path.join(outdir, stem + ".obj"), "wb") as f:
            f.write(had[2:])
        return ""
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
        if k:
            with open(os.path.join(outdir, stem + ".obj"), "rb") as f:
                cache.put("objects", k, b"OK" + f.read())
        return ""
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
    if k and m:                                  # a rejection is kept; a compiler that fell over is not
        cache.put("objects", k, b"ER" + why.encode("utf-8", "replace"))
    return why


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
    more = more or {}
    from codebreaker.base import cache, parallel
    jobs = jobs or parallel.workers()
    os.makedirs(outdir, exist_ok=True)
    _HEADER[outdir] = cache.key(header)
    with open(os.path.join(outdir, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header)
    stems = {n: "f%04d" % i for i, n in enumerate(order)}
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        errors = list(ex.map(lambda n: _compile(cc, cflags + more.get(n, []), env, outdir, stems[n], texts[n]), order))
    dropped = {n: e for n, e in zip(order, errors) if e}
    for n in list(dropped):                                  # callers of a rejected function still link
        if _compile(cc, cflags + more.get(n, []), env, outdir, stems[n], stand_in(texts[n])):
            stems.pop(n)                                     # not even a stand-in: left unresolved
    return stems, dropped


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


def build16(header, texts, order, imports, outdir, segments=None, jobs=None, code_segment=None):
    """`code_segment` {name: the number of the original's segment the function is in}: the
    functions of one segment are put in one segment again. A segment holds 64K at most, and
    functions of one segment call each other near, of different ones far -- as in the original."""
    wcc, wlink = period_tools.ow_tool("wcc"), period_tools.ow_tool("wlink")
    if not (wcc and wlink):
        raise RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)")
    env = period_tools.ow_env("ne")
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
    lines += ["file %s.obj" % stems[n] for n in order if n in stems]
    lines += ["file data.obj", "library clibs.lib"]
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
    wasm = period_tools.ow_tool("wasm")
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
    from codebreaker.build import watcom as A
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
        sym = next((s for s in A._omf_publics(obj) if s == "_" + n or s == n or
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
    k = cache.key(cache.tool(cl), CFLAGS_POWERPC, _HEADER.get(outdir, ""), text) if outdir in _HEADER else None
    had = cache.get("objects", k) if k else None
    if had is not None:
        if had[:2] == b"ER":
            return had[2:].decode("utf-8", "replace")
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
        cache.put("objects", k, b"ER" + why.encode("utf-8", "replace"))
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
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        errors = list(ex.map(lambda n: _compile_powerpc(cl, outdir, stems[n], texts[n]), order))
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
