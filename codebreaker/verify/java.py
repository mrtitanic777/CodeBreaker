"""java.py -- what is particular to reconstructing a Java program as C++03.

One of the kinds reconstruct.py works with (see there for what a kind provides). The module is a
folder of Java source (a decompiled game's src/, say); its "functions" are its classes. The source
is transpiled by codebreaker/java (transpile.convert, for no particular platform) and each class's
C++ is compiled on its own with the host compiler (settings.json `host_cxx`, else g++ on PATH).

A class that does not compile is DOES NOT COMPILE, with the compiler's first complaint. One that
compiles is then run beside the original where it can be (codebreaker/java/probe.py): its methods
with primitive parameters and results (static ones directly, instance ones on an object made with a
constructor that takes primitives) are called on the same inputs in the JVM -- the Java
source compiled by javac (settings.json `java`; javac beside it, else on PATH) -- and natively, and
the results compared exactly. All alike: VERIFIED, or VERIFIED (weak) "methods-partial" when only some
of the class's methods could be run. Any different: UNVERIFIED, with the method, the input and both
results. Nothing it could run (no such method, no JDK): NOT JUDGED, saying why. A jar is not a module
of this kind: decompile it first (`codebreaker java decompile`).
"""
import hashlib
import os
import re
import shutil
import subprocess

KIND = "java"
REV = 3                      # bump when what a verdict means changes (flags, the judge, the transpile call)
SOURCE_REV = "java-%d" % REV  # who made the candidates: the transpiler, not the native decompiler
LIBRARY = False              # no runtime-library functions to identify
PER_FUNCTION = True          # a class's verdict depends on its text, the support headers and the classes it names
STD = "c++03"
PLATFORM = None              # no platform: the generic stdafx.h, nothing a host compiler lacks
FLAGS = ("-std=" + STD, "-fsyntax-only", "-w")
COMPILES = "compiles (%s %s); not run: %s"
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


def jdk():
    """(javac, java) -- the JDK the behavioural judge runs the original with -- or ("", "")."""
    from codebreaker.decompile import jar
    java = jar.find_java() or ""
    javac = ""
    if java:
        sibling = os.path.join(os.path.dirname(java), "javac" + (".exe" if os.name == "nt" else ""))
        javac = sibling if os.path.isfile(sibling) else (shutil.which("javac") or "")
    return (javac, java) if javac else ("", "")


def compiler():
    """The host C++ compiler, or "" when there is none."""
    from codebreaker.base import settings
    cxx = settings.get("host_cxx")
    if cxx and os.path.isfile(cxx):
        return cxx
    return shutil.which("g++") or ""


def check(cxx, include_dir, unit, prefix=None):
    """Compile one translation unit for syntax and types. -> (ok, [error lines]). `prefix` is a header
    precompiled by `precompile` -- the support headers every unit starts with -- included first."""
    # -iquote, not -I: the emitted tree has headers named like the C library's (Math.h, Locale.h) and on a
    # case-insensitive disk -I would put them in front of <math.h> and <locale.h>
    first = ["-include", prefix] if prefix else []
    r = subprocess.run([cxx] + list(FLAGS) + ["-iquote", include_dir] + first + [unit], stdout=subprocess.DEVNULL,
                       stderr=subprocess.PIPE)
    err = r.stderr.decode("utf-8", "replace")
    lines = [ln for ln in err.splitlines() if _ERROR.search(ln)]
    return r.returncode == 0 and not lines, lines


def profile(module_path=None):
    from codebreaker.base import cache
    cxx = compiler()
    if not cxx:
        raise RuntimeError("no C++ compiler to judge Java with (settings.json: host_cxx, or g++ on PATH)")
    from codebreaker.java import probe
    javac, java = jdk()
    settings = repr(("java", REV, cache.tool(cxx), FLAGS, PLATFORM, probe.REV,
                     cache.tool(javac) if javac else "", cache.tool(java) if java else ""))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


def precompile(cxx, include_dir, work):
    """stdafx.h and the Java runtime (JavaRT.h and what it includes) precompiled once for the module: every
    class unit starts with them, and parsing them was four fifths of each class's compile. -> the header
    to pass as `prefix`, or None (a compiler that is not GCC's, or the header did not precompile; the units
    are then compiled without it, as before)."""
    if "g++" not in os.path.basename(cxx).lower():
        return None
    folder = os.path.join(work, "pch")
    os.makedirs(folder, exist_ok=True)
    head = os.path.join(folder, "j2x_pch.h")
    with open(head, "w", encoding="utf-8") as f:
        f.write('#include "stdafx.h"\n#include "JavaRT.h"\n')
    flags = [x for x in FLAGS if x != "-fsyntax-only"]
    r = subprocess.run([cxx] + flags + ["-iquote", include_dir, "-x", "c++-header", head, "-o", head + ".gch"],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return head if r.returncode == 0 and os.path.isfile(head + ".gch") else None


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
    for stale in (os.path.join(work, "probe"), os.path.join(work, "javasrc")):   # the last run's objects and
        if os.path.isdir(stale):                                                  # archive are of other C++
            shutil.rmtree(stale)
    if os.path.isdir(out):
        shutil.rmtree(out)
    m = Prepared()
    m.out = out
    m.summary = transpile.convert(src, out, platform=PLATFORM, write_report=False)
    m.src = src
    m.java = dict((os.path.splitext(os.path.basename(p))[0], p) for p in transpile.discover(src))
    classes = sorted(set(m.java) - set(m.summary.get('markers') or ()))   # not the dropped empty markers
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
        items[c] = (c, unit, m.java.get(c, ""))
    javac, java = jdk()
    staged = ""
    if javac:
        from codebreaker.java import probe
        staged = probe.stage_sources(m.src, os.path.join(work, "javasrc"))
    prefix = precompile(cxx, m.out, work)
    compiled = {}
    if javac:
        probe.library(cxx, m.out, work, prefix)         # the whole program, once, before the judges link to it
        # every class's Java harness, compiled in batches here rather than one javac per class there
        compiled = probe.compile_java(staged, probe.plan(staged, m.out, list(items)), work, javac)
    return (cxx, m.out, prefix, staged, javac, java, work, compiled), items, {}


def judge(context, item):
    cxx, include_dir, prefix, staged, javac, java, work, compiled = context
    _name, unit, source = item
    ok, errors = check(cxx, include_dir, unit, prefix)
    if not ok:
        first = errors[0] if errors else "the compiler rejected it"
        return "no-compile", first[:200], [], "\n".join(errors[:20])
    how = (os.path.basename(cxx), " ".join(FLAGS))
    if not javac or not source:
        return "inconclusive", "not-run", [], COMPILES % (how + ("no JDK (settings.json: java)",))
    from codebreaker.java import parse, probe
    try:
        cls = parse.parse_file(source)
    except Exception as e:                                   # the transpiler parsed it; say so if this did not
        return "inconclusive", "not-run", [], COMPILES % (how + ("could not read %s: %s" % (source, e),))
    r = probe.run(staged, include_dir, cls, work, cxx, javac, java, prefix, compiled)
    if not r["judged"]:
        return "inconclusive", "not-run", [], COMPILES % (how + (r["why"],))
    said = "%d of %d methods run beside the JVM on %d inputs" % (r["judged"], r["total"], r["cases"])
    if r["differs"]:
        lines = ["%s(%s): Java %s, C++ %s" % (m, ", ".join(a), j, c) for m, a, j, c in r["differs"][:12]]
        return "divergent", "result-differs", [], said + "; %d differ:\n" % len(r["differs"]) + "\n".join(lines)
    limits = ["methods-partial"] if r["judged"] < r["total"] else []
    return "equivalent", "", limits, said + ": every result alike"
