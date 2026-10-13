"""decompiler.py -- CodeBreaker's decompiler: a module's functions as C.

    from codebreaker.decompile import decompiler
    module = decompiler.decompile(module_path)
    module.header                  types, declarations and globals the functions share
    module.functions               {address: (name, C text)}, compilable with c_text.CFLAGS
    module.order                   the names, in address order

One decompiler for everything. The native program (native/decompiler) knows processors, not
file formats: this module takes the file apart with the readers in codebreaker.formats and
describes it -- where the bytes go in memory, and the facts read from the binary rather than
guessed:

  * every function's start and name, and the bytes of arguments it removes on return (its own
    `ret N`), which fixes its calling convention and argument count;
  * every import slot, the function it holds, and the same for that function (from the module
    that exports it, checked against the headers).

What comes back is then made into C a compiler accepts and that means what the original meant:
base types defined, globals bound to their addresses in the original, every function and import
declared before use with its real convention, and stack locals kept where the original had them
whenever an address to one can escape.
"""
import os
import re
import subprocess

from codebreaker.decompile import c_text, describe

REV = 15                                    # bump when what is fed to / taken from the native program changes


def _exe():
    from codebreaker.base import settings
    return os.path.join(settings.get("decompiler_bin"), "decompiler.exe")


def _languages():
    from codebreaker.base import settings
    return os.path.join(settings.ROOT, "native", "decompiler", "languages")


def available():
    return os.path.isfile(_exe()) and os.path.isdir(_languages())


def run(description, only=(), timeout=3600):
    """Run the native program on a description. -> ({name: text}, [names], {va: name}, stderr)
    Works for any processor it has a language for; `only` limits it to those addresses."""
    env = dict(os.environ, CB_LANGUAGES=_languages())
    cmd = [_exe(), description] + (["--at"] + ["%x" % va for va in only] if only else [])
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", timeout=timeout, env=env)
    out, order, by_va = {}, [], {}
    for m in re.finditer(r"(?ms)^/\*@@ (\S+) ([0-9a-f]+) \*/\n(.*?)(?=^/\*@@ |\Z)", r.stdout or ""):
        name, va, text = m.group(1), int(m.group(2), 16), m.group(3).strip()
        if not text or text.startswith("/* decompile failed"):
            continue
        if name not in out:
            out[name] = text
            order.append(name)
            by_va[va] = name
    return out, order, by_va, (r.stderr or "")[-2000:]


def raw(module_path, funcs, outdir, timeout=3600):
    """The native program's own output for a PE module, before it is made compilable."""
    description, _names = describe.describe32(module_path, funcs, outdir)
    return run(description, timeout=timeout)


def decompile(module_path, funcs=None, outdir=None):
    """Decompile a module (every function, or the addresses in `funcs`). -> Module"""
    from codebreaker.build import watcom as B
    from codebreaker.verify import win32 as pipeline
    if funcs is None:
        funcs = pipeline.LANES["pe"].functions(module_path)
    outdir = outdir or B._scratch("decompiler", os.path.basename(module_path).lower())
    return c_text.Module(*c_text.compilable32(module_path, funcs, outdir))


# 16-bit modules ---------------------------------------------------------------------------------
_CALLED = re.compile(r"\bfunc_0x([0-9a-f]{8})\s*\(")


def decompile16(module_path, outdir=None, rounds=4):
    """Decompile a 16-bit Windows module. -> ({name: text}, [names], {address: name}, Description)

    Function starts come first from what the file states (entry table, relocations, agreed
    near-call targets). Decompiling then shows calls to places not yet known as functions
    (`func_0x...`); those are added and the module is done again, until no new ones appear."""
    from codebreaker.build import watcom as B
    outdir = outdir or B._scratch("decompiler16", os.path.basename(module_path).lower())
    d = describe.describe16(module_path, outdir)
    out, order, by_va, _err = run(d.path)
    for _ in range(rounds):
        new = {int(x, 16) for t in out.values() for x in _CALLED.findall(t)} - set(d.names)
        new = {va for va in new if describe.inside(d, va)}
        if not new:
            break
        with open(d.path, "a", newline="\n", encoding="utf-8") as f:
            for va in sorted(new):
                seg, off = describe.where(va)
                d.names[va], d.where[va] = "sub_%d_%04X" % (seg, off), (seg, off)
                f.write(describe.model_line(d, va, d.names[va]) + "\n")
        more, _o, more_va, _err = run(d.path, only=sorted(new))      # only the new ones
        out.update(more)
        by_va.update(more_va)
    order = [by_va[va] for va in sorted(by_va)]
    return out, order, by_va, d


def _is_x86(module_path):
    from codebreaker.formats.pe import open_pe
    return open_pe(module_path).machine == 0x014C


def main(argv):
    """python -m codebreaker decompile <module> [--out FILE]

    Decompile any module the corpus or a path names -- 32-bit or 16-bit x86, or PowerPC -- and
    write its functions as C. Nothing is built or judged; `module` does that."""
    if not argv:
        print(main.__doc__)
        return 2
    from codebreaker.base import corpus
    from codebreaker.formats import detect
    path = argv[0] if os.path.isfile(argv[0]) else corpus.retail(argv[0])
    if not path:
        raise SystemExit("no module %r" % argv[0])
    kind = detect.lane(path)
    if kind == "ne":
        texts, order, _by_va, d = decompile16(path)
        header, total = "", len(d.names)
    elif kind == "pe" and _is_x86(path):
        m = decompile(path)
        texts, order, header, total = m.texts, m.order, m.header, len(m.by_va)
    elif kind == "pe":                                       # PowerPC: as the decompiler gives it
        from codebreaker.build import watcom as B
        starts = describe.function_starts(path)
        texts, order, _by_va, _err = raw(path, starts, B._scratch("decompiler", os.path.basename(path).lower()))
        header, total = "", len(starts)
    else:
        raise SystemExit("%s: a %s file; only 32-bit (PE) and 16-bit (NE) modules so far" % (
            path, kind or "non-executable"))
    out = argv[argv.index("--out") + 1] if "--out" in argv else \
        os.path.splitext(os.path.basename(path))[0].lower() + ".c"
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("/* %s -- decompiled by CodeBreaker; not built, not judged. */\n\n" % os.path.basename(path))
        f.write(header + "\n\n" if header else "")
        f.write("\n\n".join(texts[n] for n in order) + "\n")
    print("%s: %d of %d functions -> %s" % (os.path.basename(path), len(order), total, out))
    return 0
