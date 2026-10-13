"""module16.py -- compile a 16-bit module's reconstructed functions and link a 16-bit Windows
DLL from them.

    dll, where, dropped = module16.build(header, texts, order, imports, outdir)
        where    {function name: (segment, offset)} in the DLL that was built
        dropped  {function name: why the compiler rejected it}

Each function is compiled on its own, against a header that declares every function's
convention and result but leaves its arguments open. The decompiler recovers each call on its
own evidence, and a caller that pushes three words where the definition takes a far pointer and
a word puts the same bytes on the stack; compiled apart, each call is taken as written, and a
function the compiler rejects cannot take others with it. A rejected function is replaced by a
stand-in with the same signature so its callers still link.

The DLL is linked without the C runtime's start-up and with string constants in the code
segment, so it has no data of its own: when it is run for judging, its data segment is given
the original module's data and every global is then where the original had it.
"""
import os
import re
import subprocess
from concurrent.futures import ThreadPoolExecutor

from codebreaker.base import period_tools

# -ecd: a function declared without a convention is __stdcall, the decompiler's default for 16-bit code
CFLAGS = ["-bt=windows", "-ms", "-ecd", "-zu", "-zc", "-zl", "-zq", "-w0", "-s", "-3", "-zastd=c99", "-e=20"]
_ERR = re.compile(r"\((\d+)\): Error! (E\d+: [^\r\n]*)")
_SYM = re.compile(r"(?m)^([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4,8})[+*]?\s+(\S+)\s*$")


def _stand_in(proto):
    """A definition with the same signature that does nothing."""
    head = proto.rstrip(";")
    return head + ("\n{\n}\n" if head.split()[0] == "void" else "\n{\n  return 0;\n}\n")


def _plain(symbol):
    """The C name behind a linker symbol (`_NAME@8`, `NAME_`, `_NAME`)."""
    return symbol.split("@")[0].strip("_")


def _compile(wcc, env, outdir, stem, text):
    """Compile one function. -> "" or the first error."""
    with open(os.path.join(outdir, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
        f.write('#include "m.h"\n\n' + text + "\n")
    for ext in (".obj", ".err"):
        try:
            os.remove(os.path.join(outdir, stem + ext))
        except OSError:
            pass
    r = subprocess.run([wcc] + CFLAGS + [stem + ".c"], cwd=outdir, env=env, capture_output=True, text=True,
                       errors="replace", timeout=300)
    if r.returncode == 0 and os.path.isfile(os.path.join(outdir, stem + ".obj")):
        return ""
    log = (r.stdout or "") + (r.stderr or "")
    try:
        log += open(os.path.join(outdir, stem + ".err"), errors="replace").read()
    except OSError:
        pass
    m = _ERR.search(log)
    return (m.group(2) if m else log.strip()[-100:] or "the compiler failed")[:100]


def build(header, texts, order, imports, outdir, jobs=6, segments=None):
    from codebreaker.decompile.c16 import prototype
    wcc, wlink = period_tools.ow_tool("wcc"), period_tools.ow_tool("wlink")
    if not (wcc and wlink):
        raise RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)")
    os.makedirs(outdir, exist_ok=True)
    env = period_tools.ow_env("ne")
    with open(os.path.join(outdir, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header)
    stems = {n: "f%04d" % i for i, n in enumerate(order)}
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        errors = list(ex.map(lambda n: _compile(wcc, env, outdir, stems[n], texts[n]), order))
    dropped = {n: e for n, e in zip(order, errors) if e}
    for n in list(dropped):                                  # callers of a rejected function still link
        if _compile(wcc, env, outdir, stems[n], _stand_in(prototype(texts[n]))):
            stems.pop(n)                                     # not even a stand-in: left unresolved
    live = [n for n in order if n not in dropped]
    # One initialised byte, so the DLL has a data segment at all: that segment is where the
    # original's data is put when the DLL is run for judging.
    data = ["char codebreaker_data_segment = 1;"]
    for n, image in sorted((segments or {}).items()):        # other segments the code reads from
        data.append("uint1 __far codebreaker_segment_%d[] = {\n%s\n};" % (n, ",\n".join(
            ",".join(str(x) for x in image[i:i + 32]) for i in range(0, len(image), 32))))
    _compile(wcc, env, outdir, "data", "\n".join(data))
    lines = ["system windows dll", "name m.dll", "option quiet, map, nodefaultlibs, caseexact, undefsok",
             "libpath %s" % period_tools.ow_lib("ne")]
    lines += ["file %s.obj" % stems[n] for n in order if n in stems]
    lines += ["file data.obj", "library clibs.lib"]
    # a C name compiled as __stdcall with an open argument list is the symbol `_name`
    lines += ["import _%s %s.%d" % (nm, mod, ordn) for nm, (mod, ordn) in sorted(imports.items())]
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
    if not os.path.isfile(dll):
        why = ((r.stdout or "") + (r.stderr or "")).strip()[-160:] or "the linker produced nothing"
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
