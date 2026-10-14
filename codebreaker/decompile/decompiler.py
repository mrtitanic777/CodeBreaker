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

REV = 16                                    # bump when what is fed to / taken from the native program changes


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
    # The description holds the module's bytes and every fact given with them, so what the
    # program says of it is kept under its hash (base/cache.py): a module is decompiled once.
    from codebreaker.base import cache
    with open(description, "rb") as f:
        k = cache.key(cache.tool(_exe()), _language_stamp(), f.read(), ["%x" % va for va in only])
    had = cache.get("decompiled", k)
    if had is not None:
        stdout, stderr = had.decode("utf-8", "replace").split("\x00", 1)
    else:
        stdout, stderr, whole = _run_resuming(description, only, timeout)
        if whole and stdout:
            cache.put("decompiled", k, (stdout + "\x00" + stderr).encode("utf-8", "replace"))
    out, order, by_va = {}, [], {}
    for m in re.finditer(r"(?ms)^/\*@@ (\S+) ([0-9a-f]+) \*/\n(.*?)(?=^/\*@@ |\Z)", stdout):
        name, va, text = m.group(1), int(m.group(2), 16), m.group(3).strip()
        if not text or text.startswith("/* decompile failed"):
            continue
        if name not in out:
            out[name] = text
            order.append(name)
            by_va[va] = name
    return out, order, by_va, stderr


_MARK = re.compile(r"(?m)^/\*@@ \S+ ([0-9a-f]+) \*/")


def _run_resuming(description, only, timeout):
    """Run the native program; when it dies part way (a crash that depends on what it did
    before -- the function it died on decompiles on its own), start it again from the next
    function it had not written, and so on. A function a fresh start dies on at once is left
    out, with a note on stderr. -> (stdout, stderr, whether every function was attempted)"""
    env = dict(os.environ, CB_LANGUAGES=_languages())
    if only:
        wanted = sorted(only)
    else:
        with open(description, encoding="utf-8", errors="replace") as f:
            wanted = sorted({int(line.split("\t")[1], 16) for line in f if line.startswith("F\t")})
    first = True
    outs, errs, left = [], [], list(wanted)
    while True:
        cmd = [_exe(), description] + (["--at"] + ["%x" % va for va in left] if (only or not first) else [])
        r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", timeout=timeout, env=env)
        out = r.stdout or ""
        outs.append(out)
        errs.append((r.stderr or "")[-1000:])
        if r.returncode == 0:
            return "".join(outs), "".join(errs)[-2000:], True
        done = {int(m.group(1), 16) for m in _MARK.finditer(out)}
        # (the last one written may be cut off by the crash: it is done again)
        last = max(done) if done else None
        left = [va for va in left if va not in done or va == last]
        if done and last is not None:
            outs[-1] = out[:list(_MARK.finditer(out))[-1].start()]
        if not done or done == {last} and left and left[0] == last:
            dead = left.pop(0)                               # dies at once on this one: left out
            errs.append("\n[decompiler died on %x; left out]\n" % dead)
        first = False
        if not left:
            return "".join(outs), "".join(errs)[-2000:], False


_STAMP = []


def _language_stamp():
    if not _STAMP:
        from codebreaker.base import cache
        _STAMP.append(cache.folder(_languages()))
    return _STAMP[0]


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
    out = _by_their_names(out, d.names)
    order = [by_va[va] for va in sorted(by_va)]
    return out, order, by_va, d


def _by_their_names(out, names):
    """A function found only because another called it was decompiled after its callers, whose
    text still calls it `func_0x<address>` -- a name nothing has. The callers are given its name."""
    known = {"%08x" % va: name for va, name in names.items()}
    if not known:
        return out
    placeholder = re.compile(r"\bfunc_0x([0-9a-f]{8})\b")
    return {n: placeholder.sub(lambda m: known.get(m.group(1), m.group(0)), t) for n, t in out.items()}


def _grow(d, line_for, rounds=4):
    """Decompile a description; then, while the output calls places not yet known as functions
    (`func_0x...`), add those inside the module's code and decompile them too."""
    out, order, by_va, _err = run(d.path)
    for _ in range(rounds):
        new = {int(x, 16) for t in out.values() for x in _CALLED.findall(t)} - set(d.names)
        new = {va for va in new if any(lo <= va < lo + n for lo, n in d.spans.values())}
        if not new:
            break
        with open(d.path, "a", newline="\n", encoding="utf-8") as f:
            for va in sorted(new):
                name, line = line_for(va)
                d.names[va] = name
                f.write(line + "\n")
        more, _o, more_va, _err = run(d.path, only=sorted(new))
        out.update(more)
        by_va.update(more_va)
    out = _by_their_names(out, d.names)
    return out, [by_va[va] for va in sorted(by_va)], by_va


def decompile_le(module_path, outdir=None):
    """Decompile a virtual device driver file: one LE image, or every member of a container.
    -> [(member name, {name: text}, [names], Description)]"""
    from codebreaker.build import watcom as B
    from codebreaker.formats import le
    outdir = outdir or B._scratch("decompiler_le", os.path.basename(module_path).lower())
    results = []
    for name, lf in le.open_vxds(module_path):
        d = describe.describe_le(lf, name, os.path.join(outdir, re.sub(r"\W", "_", name)))

        def line(va):
            n = "sub_%d_%05X" % ((va - describe.LE_BASE) // describe.LE_STRIDE + 1, (va - describe.LE_BASE) % describe.LE_STRIDE)
            return n, "F\t%x\t%s\t-1" % (va, n)
        out, order, _by_va = _grow(d, line)
        results.append((d.device, out, order, d))
    return results


def decompile_dos(module_path, outdir=None):
    """Decompile a DOS program. -> ({name: text}, [names], Description)"""
    from codebreaker.build import watcom as B
    outdir = outdir or B._scratch("decompiler_dos", os.path.basename(module_path).lower())
    d = describe.describe_dos(module_path, outdir)

    def line(va):
        kind, _pop = describe.returns(d.image, va - d.base)
        n = "sub_%05X" % (va - d.base)
        return n, "F\t%x\t%s\t-1\t__cdecl16%s" % (va, n, kind or "near")
    out, order, _by_va = _grow(d, line, rounds=8)
    from codebreaker.symbols import dos_interrupts
    out = {n: dos_interrupts.annotate(t, d.interrupts.get(n, [])) for n, t in out.items()}
    return out, order, d


def _is_x86(module_path):
    from codebreaker.formats.pe import open_pe
    return open_pe(module_path).machine == 0x014C


def as_c(path):
    """Any executable as C: (text, functions decompiled, functions found). Raises ValueError for
    a file that is not an executable this can read."""
    from codebreaker.formats import detect
    kind = detect.lane(path)
    if kind is None:                                         # .COM files and flat drivers have no header
        from codebreaker.formats import dos
        kind = "dos" if dos.kind(path) else None
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
    elif kind == "le":
        texts, order, header, total = {}, [], "", 0
        for member, out, names, d in decompile_le(path):
            total += len(d.names)
            for n in names:
                key = "%s.%s" % (member, n)
                texts[key] = "/* driver %s */\n%s" % (member, out[n])
                order.append(key)
    elif kind == "dos":
        texts, order, d = decompile_dos(path)
        header, total = "", len(d.names)
    else:
        raise ValueError("%s: not an executable this can read" % path)
    texts = {n: c_text._FACT.sub("", t).strip() for n, t in texts.items()}    # the decompiler's side notes
    text = "/* %s -- decompiled by CodeBreaker; not built, not judged. */\n\n" % os.path.basename(path) + \
        (header + "\n\n" if header else "") + "\n\n".join(texts[n] for n in order) + "\n"
    return text, len(order), total


def main(argv):
    """python -m codebreaker decompile <module> [--out FILE]

    Decompile any module the corpus or a path names -- a 32-bit or 16-bit Windows module, a
    virtual device driver, a DOS program, a PowerPC image -- and write its functions as C. Nothing is built or judged; `module` does that."""
    if not argv:
        print(main.__doc__)
        return 2
    from codebreaker.base import corpus
    path = argv[0] if os.path.isfile(argv[0]) else corpus.retail(argv[0])
    if not path:
        raise SystemExit("no module %r" % argv[0])
    try:
        text, decompiled, total = as_c(path)
    except ValueError as e:
        raise SystemExit(str(e))
    out = argv[argv.index("--out") + 1] if "--out" in argv else \
        os.path.splitext(os.path.basename(path))[0].lower() + ".c"
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    print("%s: %d of %d functions -> %s" % (os.path.basename(path), decompiled, total, out))
    return 0
