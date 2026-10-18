"""java.py -- what is particular to reconstructing a Java program as C++03.

One of the kinds reconstruct.py works with (see there for what a kind provides). The module is a
folder of Java source (a decompiled game's src/, say); its "functions" are its classes. The source
is transpiled by codebreaker/java (transpile.convert, for no particular platform) and each class's
C++ is compiled on its own with the host compiler (settings.json `host_cxx`, else g++ on PATH).

Nothing is run. There is no judge for Java yet that runs the original beside its translation, so a
class that compiles is NOT JUDGED -- it says no more than that -- and one that does not is
DOES NOT COMPILE, with the compiler's first complaint. A jar is not a module of this kind: decompile
it first (`codebreaker java decompile`).
"""
import hashlib
import os
import re
import shutil
import subprocess

KIND = "java"
REV = 1                      # bump when what a verdict means changes (flags, the judge, the transpile call)
SOURCE_REV = "java-%d" % REV  # who made the candidates: the transpiler, not the native decompiler
LIBRARY = False              # no runtime-library functions to identify
PER_FUNCTION = True          # a class's verdict depends on its text, the support headers and the classes it names
STD = "c++03"
PLATFORM = None              # no platform: the generic stdafx.h, nothing a host compiler lacks
FLAGS = ("-std=" + STD, "-fsyntax-only", "-w")
COMPILES = "compiles (%s %s); not run -- there is no judge yet that runs Java beside its translation"
_ERROR = re.compile(r"\berror\b")


def is_module(path):
    """A folder holding .java files (at any depth), or a single .java file's folder."""
    if os.path.isfile(path):
        return path.lower().endswith(".java")
    if not os.path.isdir(path):
        return False
    for _dp, _dirs, files in os.walk(path):
        if any(f.endswith(".java") for f in files):
            return True
    return False


def source_dir(path):
    return os.path.dirname(os.path.abspath(path)) if os.path.isfile(path) else path


def sha(path):
    """What results are filed under: the .java files' names and contents, in a fixed order."""
    from codebreaker.java import transpile
    h = hashlib.sha256()
    root = source_dir(path)
    for p in transpile.discover(root):
        h.update(os.path.relpath(p, root).replace("\\", "/").encode() + b"\0")
        with open(p, "rb") as f:
            h.update(f.read())
        h.update(b"\0")
    return h.hexdigest()


def compiler():
    """The host C++ compiler, or "" when there is none."""
    from codebreaker.base import settings
    cxx = settings.get("host_cxx")
    if cxx and os.path.isfile(cxx):
        return cxx
    return shutil.which("g++") or ""


def check(cxx, include_dir, unit):
    """Compile one translation unit for syntax and types. -> (ok, [error lines])"""
    # -iquote, not -I: the emitted tree has headers named like the C library's (Math.h, Locale.h) and on a
    # case-insensitive disk -I would put them in front of <math.h> and <locale.h>
    r = subprocess.run([cxx] + list(FLAGS) + ["-iquote", include_dir, unit], stdout=subprocess.DEVNULL,
                       stderr=subprocess.PIPE)
    err = r.stderr.decode("utf-8", "replace")
    lines = [ln for ln in err.splitlines() if _ERROR.search(ln)]
    return r.returncode == 0 and not lines, lines


def profile(module_path=None):
    from codebreaker.base import cache
    cxx = compiler()
    if not cxx:
        raise RuntimeError("no C++ compiler to judge Java with (settings.json: host_cxx, or g++ on PATH)")
    settings = repr(("java", REV, cache.tool(cxx), FLAGS, PLATFORM))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


class Prepared(object):
    pass


def prepare(module_path, work):
    """Transpile the module into work/cpp. Classes are named by their .java file."""
    if module_path.lower().endswith(".jar"):
        raise SystemExit("%s: a jar -- decompile it to Java source first (codebreaker java decompile)"
                         % os.path.basename(module_path))
    from codebreaker.java import transpile
    src = source_dir(module_path)
    out = os.path.join(work, "cpp")
    if os.path.isdir(out):
        shutil.rmtree(out)
    m = Prepared()
    m.out = out
    m.summary = transpile.convert(src, out, platform=PLATFORM, write_report=False)
    classes = [os.path.splitext(os.path.basename(p))[0] for p in transpile.discover(src)]
    emitted = set(f for f in os.listdir(out))
    m.texts = {}
    for c in classes:
        parts = [_read(os.path.join(out, c + ext)) for ext in (".h", ".cpp") if c + ext in emitted]
        if parts:
            m.texts[c] = "\n".join(parts)
    support = sorted(f for f in emitted if f.endswith(".h") and os.path.splitext(f)[0] not in m.texts)
    m.header = "".join("// ---- %s\n%s\n" % (f, _read(os.path.join(out, f))) for f in support)
    m.order = sorted(m.texts)
    m.key = {c: c for c in m.order}
    m.all_keys = sorted(classes)
    m.sig = hashlib.sha1(("%d\0%s" % (REV, m.header)).encode("utf-8", "replace")).hexdigest()
    return m


def _read(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        return f.read()


def build(m, module_path, work):
    """One translation unit per class: its .cpp, or for a header-only class a unit that includes it.
    -> (context for judge, {name: item}, {name: why it is not in the build})"""
    cxx = compiler()
    units = os.path.join(work, "units")
    os.makedirs(units, exist_ok=True)
    items = {}
    for c in m.order:
        unit = os.path.join(m.out, c + ".cpp")
        if not os.path.isfile(unit):
            unit = os.path.join(units, c + ".cpp")
            with open(unit, "w", encoding="utf-8") as f:
                f.write('#include "stdafx.h"\n#include "%s.h"\n' % c)
        items[c] = (c, unit)
    return (cxx, m.out), items, {}


def judge(context, item):
    cxx, include_dir = context
    _name, unit = item
    ok, errors = check(cxx, include_dir, unit)
    if ok:
        return "inconclusive", "not-run", [], COMPILES % (os.path.basename(cxx), " ".join(FLAGS))
    first = errors[0] if errors else "the compiler rejected it"
    return "no-compile", first[:200], [], "\n".join(errors[:20])
