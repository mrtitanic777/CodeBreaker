"""functions.py -- compile a module's reconstructed functions and link a DLL from them.

One builder for every kind of module:

    dll, where, dropped = functions.build16(header, texts, order, imports, outdir, segments)
    dll, where, dropped = functions.build32(module_path, header, texts, order, imports, outdir, cflags)
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

from codebreaker.base import period_tools

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
    """Compile one function. -> "" or the first error."""
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
        return ""
    log = (r.stdout or "") + (r.stderr or "")
    try:
        log += open(os.path.join(outdir, stem + ".err"), errors="replace").read()
    except OSError:
        pass
    m = _ERR.search(log)
    return (m.group(2) if m else log.strip()[-100:] or "the compiler failed")[:100]


def compile_all(cc, cflags, env, header, texts, order, outdir, jobs=6):
    """Compile every function. -> ({name: object file stem}, {name: why it was rejected})"""
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header)
    stems = {n: "f%04d" % i for i, n in enumerate(order)}
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        errors = list(ex.map(lambda n: _compile(cc, cflags, env, outdir, stems[n], texts[n]), order))
    dropped = {n: e for n, e in zip(order, errors) if e}
    for n in list(dropped):                                  # callers of a rejected function still link
        if _compile(cc, cflags, env, outdir, stems[n], stand_in(texts[n])):
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


def build16(header, texts, order, imports, outdir, segments=None, jobs=6):
    wcc, wlink = period_tools.ow_tool("wcc"), period_tools.ow_tool("wlink")
    if not (wcc and wlink):
        raise RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)")
    env = period_tools.ow_env("ne")
    stems, dropped = compile_all(wcc, CFLAGS16, env, header, texts, order, outdir, jobs)
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


def build32(module_path, header, texts, order, used_imports, outdir, cflags, jobs=6):
    from codebreaker.build import watcom as B
    from codebreaker.formats.pe import open_pe
    from codebreaker.targets.win95 import flat_thunk_build as A
    ow = B._ow()
    if not os.path.exists(ow["wcc"]):
        raise RuntimeError("OpenWatcom's 32-bit compiler is not set up (settings.json: openwatcom)")
    env = B._ow_env(ow)
    imports = B.import_map(module_path)
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
        for line in B._import_lines(obj, set(used_imports), imports).strip().split("\n"):
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
