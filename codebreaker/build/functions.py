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
    if k and m:                                  # a rejection is kept; a compiler that fell over is not
        cache.put("objects", k, b"ER" + why.encode("utf-8", "replace"))
    return why


_HEADER = {}                     # build folder -> the hash of the header in it


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
    if os.path.isfile(dll):
        return dll, ""
    return None, ((r.stdout or "") + (r.stderr or "")).strip()[-160:] or "the linker produced nothing"


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
        data.append("uint1 __far codebreaker_segment_%d[] = {\n%s\n};" % (n, ",\n".join(
            ",".join(str(x) for x in image[i:i + 32]) for i in range(0, len(image), 32))))
    _compile(wcc, CFLAGS16, env, outdir, "data", "\n".join(data))
    lines = ["system windows dll", "name m.dll", "option quiet, map, nodefaultlibs, caseexact, undefsok",
             "libpath %s" % period_tools.ow_lib("ne")]
    lines += ["file %s.obj" % stems[n] for n in order if n in stems]
    lines += ["file data.obj", "library clibs.lib"]
    # a C name compiled as __stdcall with an open argument list is the symbol `_name`
    lines += ["import _%s %s.%d" % (nm, mod, ordn) for nm, (mod, ordn) in sorted(imports.items())]
    dll, why = _link(wlink, env, outdir, lines)
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


def build32(module_path, header, texts, order, used_imports, outdir, cflags, jobs=None):
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
    # undefsok: a call to a function that is not there is left unresolved (it then fails its test)
    lines = ["system nt_dll", "name m.dll", "option quiet", "option undefsok", B._BASE.strip()]
    lines += ["file %s.obj" % stems[n] for n in order if n in stems]
    lines += [x for x in B._RTL.strip().split("\n") if x]
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
