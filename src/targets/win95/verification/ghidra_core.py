#!/usr/bin/env python3
r"""ghidra_core.py -- the embedded Ghidra decompiler core (native/ghidra_core), fed CodeBreaker's facts.

`cbdecomp` is the Ghidra decompiler core linked directly, with no Java and no Ghidra application.
Left alone it guesses what it cannot see from one function. This module tells it what CodeBreaker
has read from the binaries:

  * every function's start and name, and the bytes of arguments it removes on return (its own
    `ret N`) -- which fixes its calling convention and argument count;
  * every import-table slot, the function it holds, and the same for that function (from the DLL
    that exports it, checked against the Win32 headers -- apiarity).

    facts(module_path)        -> path of the facts file for a module (written on demand)
    decompile(module_path)    -> ({name: C text}, [names in address order], {va: name})
"""
import os
import re
import subprocess

from src.core import paths

REV = 3                                    # bump when what is fed to / taken from the core changes

# The core prints a function in the module's default convention (stdcall) without saying so, and
# spells the others out. Compile with stdcall as the default so the text means what it says.
CFLAGS = ["-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", "-ecs"]

_TYPES = """typedef signed char int1; typedef short int2; typedef int int4; typedef long long int8;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned int uint4;
typedef unsigned long long uint8;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned int xunknown4;
typedef unsigned long long xunknown8;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned int undefined4; typedef unsigned long long undefined8;
typedef unsigned char bool; typedef float float4; typedef double float8; typedef long double float10;
typedef int code();
#define true 1
#define false 0
"""
_BY_SIZE = {1: "uint1", 2: "uint2", 4: "uint4", 8: "uint8"}


def _exe():
    root = paths.repo_root()
    return os.path.join(root, "native", "ghidra_core", "build", "bin", "cbdecomp.exe")


def _specs():
    return os.path.join(paths.repo_root(), "native", "ghidra_core", "languages")


def available():
    return os.path.isfile(_exe()) and os.path.isdir(_specs())


def _ident(name):
    return re.sub(r"\W", "_", name)


def function_names(module_path, funcs):
    """{va: name} -- an export's own name, else sub_<rva>. A name also used by an import of this
    module stays with the import (the call has to bind to it)."""
    from src.formats.pe import open_pe
    from src.targets.win95 import bodyverify as B
    pe = open_pe(module_path)
    imported = set(B.import_map(module_path))
    by_rva = {}
    for nm, rva in sorted(pe.export_addrs().items()):
        if re.fullmatch(r"[A-Za-z_]\w*", nm) and nm not in imported:
            by_rva.setdefault(rva, nm)
    return {va: by_rva.get(va - pe.image_base, "sub_%06X" % (va - pe.image_base)) for va in funcs}


def facts(module_path, funcs, outdir):
    """Write the facts file for `funcs` (VAs) of a module; return (path, {va: name})."""
    from src.engines.behavior import apiarity
    from src.formats.pe import open_pe
    pe = open_pe(module_path)
    names = function_names(module_path, funcs)
    mod = apiarity._Mod(module_path)
    lines = []
    for va in funcs:
        try:
            pop = apiarity._scan(mod, va - pe.image_base, 4)
        except Exception:
            pop = None
        lines.append("F\t%x\t%s\t%d" % (va - pe.image_base, names[va], -1 if pop is None else pop))
    for slot, (dll, name) in sorted(pe.imports().items()):
        stem = os.path.splitext(dll)[0].upper()
        if not name or name.startswith("#"):
            lines.append("I\t%x\t%s\t-1" % (slot - pe.image_base, _ident("%s_ord_%s" % (stem, (name or "#0")[1:]))))
            continue
        pop = apiarity.stack_bytes(stem, name)
        lines.append("I\t%x\t%s\t%d" % (slot - pe.image_base, _ident(name), -1 if pop is None else pop))
    os.makedirs(outdir, exist_ok=True)
    path = os.path.join(outdir, "facts.txt")
    with open(path, "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    return path, names


def decompile(module_path, funcs, outdir, timeout=3600):
    """Decompile `funcs` with the embedded core. -> ({name: text}, [names], {va: name}, stderr)."""
    from src.formats.pe import open_pe
    fpath, names = facts(module_path, funcs, outdir)
    env = dict(os.environ, CB_GHIDRA_SPECS=_specs())
    r = subprocess.run([_exe(), module_path, "--facts", fpath], capture_output=True, text=True,
                       errors="replace", timeout=timeout, env=env)
    base = open_pe(module_path).image_base
    out, order, by_va = {}, [], {}
    for m in re.finditer(r"(?ms)^/\*@@ (\S+) ([0-9a-f]+) \*/\n(.*?)(?=^/\*@@ |\Z)", r.stdout or ""):
        name, rva, text = m.group(1), int(m.group(2), 16), m.group(3).strip()
        if not text or text.startswith("/* decompile failed"):
            continue
        if name not in out:
            out[name] = text
            order.append(name)
            by_va[base + rva] = name
    return out, order, by_va, (r.stderr or "")[-2000:]


def adapt(module_path, funcs, outdir):
    """-> (preamble, {name: text}, [names], {va: name}, [import names used]) for ghidra_engine:
    the core's output with its base types defined, its globals bound to the original's addresses,
    and every function and import declared before use with its real convention."""
    from src.engines.behavior import apiarity
    from src.targets.win95 import bodyverify as B
    out, order, by_va, _err = decompile(module_path, funcs, outdir)
    imports = B.import_map(module_path)
    sizes = {}
    for n in order:
        body = []
        for ln in out[n].split("\n"):
            m = re.match(r"/\*@G ([0-9a-f]+) (\d+) \*/$", ln)
            if m:
                sizes.setdefault(int(m.group(1), 16), set()).add(int(m.group(2)))
            else:
                body.append(ln)
        # every function returns a value in EAX whether or not the core saw it used
        out[n] = re.sub(r"\Avoid(\s)", r"int\1", "\n".join(body).strip())
    text = "\n".join(out.values())
    macros = []
    for pre, addr in sorted(set(re.findall(r"\b([a-z]+)Ram([0-9a-f]{8})\b", text))):
        szs = sizes.get(int(addr, 16)) or {4}
        size = min(szs) if len(szs) > 1 and pre[0] in "cb" else max(s for s in szs if s <= 8) if any(s <= 8 for s in szs) else 4
        ty = "code *" if pre == "pc" else ("int%d" % size if pre[0] == "i" else _BY_SIZE.get(size, "uint4"))
        macros.append("#define %sRam%s (*(%s *)0x%s)" % (pre, addr, ty, addr))
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    used = sorted(n for n in called if n in imports and n not in out)
    decls = []
    for n in used:
        conv = "__cdecl" if apiarity.stack_bytes(imports[n], n) == 0 else "__stdcall"
        decls.append("int %s %s();" % (conv, n))
    for n in order:                                    # each function, declared before any use
        head = out[n].split("{", 1)[0].strip()
        if head.endswith(")"):
            decls.append(" ".join(head.split()) + ";")
    preamble = _TYPES + "\n".join(decls) + "\n" + "\n".join(macros) + "\n"
    return preamble, out, order, by_va, used
