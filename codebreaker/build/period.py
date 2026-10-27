"""period.py -- compilers of the original's time, as the repair search uses them.

    from codebreaker.build import period
    period.names()                              the period compilers set up on this machine
    p = period.get("xenix-c")                   (or "msvc2", "msc6", "msvc152", "msc203"; flags=...)
    built = p.compile(source_text, work)        (bytes, mask) of the code it gives, or None
                                                (header=..., function=name: that function's code)
    for unit in p.units():                      what there is to compare with: Unit(name, source
        unit.original                           text or None, (bytes, mask) of the original)

A reconstruction is judged by behaviour with a modern compiler (build/functions.py). Whether it
is the source that was WRITTEN is a different question, and only a compiler of the original's
time can speak to it: the authored source gives the original's bytes back, another source that
behaves the same does not. verify/repair.py searches with that distance; this module is the one
place that knows how each such compiler is run and how its output is read.

    msvc2      Microsoft's 32-bit C compiler of 1994 (settings: period_compiler -- the folder of
               CL.EXE). One function at a time; see verify/match.py.
    xenix-c    the C compiler of XENIX for the 8086 (cpp, c0, c1, c2, as), with which Microsoft's
               linker and librarian of 1983-84 were compiled (settings: xenix_c -- the folder
               holding xcc.py, which runs the compiler's own passes in an emulator). One source
               file at a time; a file's code is one stretch of the executable.
    msc6       Microsoft C 6.0a, the 16-bit Windows modules' compiler, in DOSBox-X (settings: msc6,
    msvc152    dosbox_x); and Visual C++ 1.52c's C/C++ 8.00c (settings: msvc152), which built a few
               of them. Many sources in one emulator session: build/msc6.py.
    msc203     Microsoft C 2.03 of 1984, MC1 then MC2, in DOSBox-X (settings: msc203).

`mask` marks the bytes the linker fills in (an address, the distance of a call): they are not
the compiler's to get right, and are left out of every comparison.
"""
import importlib
import os
import sys


class Unit(object):
    """Something that can be compiled and compared: a function, or a source file."""
    def __init__(self, name, source, original, address=0):
        self.name, self.source, self.original, self.address = name, source, original, address


class XenixC(object):
    """The XENIX C compiler, run by the project that reconstructed LINK and LIB with it."""
    name = "xenix-c"
    bits = 16

    def __init__(self, tools, target="link240"):
        self.tools, self.target = tools, target
        self._xcc = None

    def _load(self):
        if self._xcc is None:
            os.environ["TARGET"] = self.target
            if self.tools not in sys.path:
                sys.path.insert(0, self.tools)
            self._xcc = importlib.import_module("xcc")
        return self._xcc

    def compile(self, source, work, header="", function=""):
        """Compile one source file's text (a file's code is one stretch: `function` is not used).
        -> (bytes, mask) or None."""
        xcc = self._load()
        source = header + source
        os.makedirs(work, exist_ok=True)
        path = os.path.join(work, "unit.c")
        with open(path, "wb") as f:
            f.write(source.replace("\r\n", "\n").encode("latin1", "replace"))
        try:
            o = xcc.compile_c(path, include=os.path.join(xcc.WORK, "src"))
        except Exception:
            return None
        text = getattr(o, "text", None)
        if not text:
            return None
        return bytes(text), self._mask(len(text), o)

    @staticmethod
    def _mask(size, o):
        mask = bytearray(size)
        for r in getattr(o, "treloc", None) or ():
            at = r[0] if isinstance(r, (tuple, list)) else r
            if isinstance(at, int) and 0 <= at < size:
                mask[at:at + 2] = b"\1\1"[:size - at]
        return bytes(mask)

    def units(self):
        """The source files of the reconstruction, each with the stretch of the executable it
        was compiled into."""
        xcc = self._load()
        image = bytes(xcc.image())
        src = os.path.join(xcc.WORK, "src")
        order = []
        with open(os.path.join(src, "ORDER"), encoding="latin1") as f:
            for line in f:
                parts = line.split()
                if len(parts) >= 2 and not line.startswith("#"):
                    order.append((parts[0], int(parts[1], 16)))
        # a function kept in a file of its own is named for its address: f01034.c
        listed = set(n for n, _a in order)
        for name in sorted(os.listdir(src)):
            if name not in listed and name[:1] == "f" and name.endswith(".c"):
                try:
                    order.append((name, int(name[1:-2], 16)))
                except ValueError:
                    pass
        out = []
        for name, at in order:
            path = os.path.join(src, name)
            if not os.path.isfile(path):
                continue
            with open(path, "rb") as f:
                text = f.read().decode("latin1")
            built = self.compile(text, os.path.join(_scratch(), "units"))
            if built is None:
                continue
            code, mask = built
            out.append(Unit(name, text, (image[at:at + len(code)], mask), at))
        return out


class Msvc2(object):
    """Microsoft's 32-bit C compiler of 1994, one function at a time (verify/match.py)."""
    name = "msvc2"
    bits = 32

    def __init__(self, cl, flags="/O2 /Gz"):
        from codebreaker.verify import match
        self.cl, self.flags, self.env = cl, flags, match.compiler_env(cl)

    def compile(self, source, work, header="", function=""):
        """-> (bytes, mask) of the function (the one named, or the only one), or None"""
        from codebreaker.verify import match
        return match.compile_function(self.cl, self.env, header, source, function, work, self.flags)

    def units(self):
        return []


class MicrosoftC16(object):
    """Microsoft C 6.0a ("msc6") or the C/C++ 8.00c of Visual C++ 1.52c ("msvc152"), the 16-bit
    Windows modules' compilers (build/msc6.py: one emulator session for many sources)."""
    bits = 16

    def __init__(self, name, flags="/AM /Gcsw /Os"):
        self.name, self.flags = name, flags

    def compile(self, source, work=None, header="", function=""):
        """-> (bytes, mask) of the public function named, else of the first code segment; or None"""
        from codebreaker.build import msc6
        names = {"x": function} if function else None
        return msc6.compile_many({"x": source}, header, self.flags, work=work, compiler=self.name, names=names)["x"]

    def compile_many(self, sources, header="", **kw):
        from codebreaker.build import msc6
        return msc6.compile_many(sources, header, self.flags, compiler=self.name, **kw)

    def units(self):
        return []


class Msc203(object):
    """Microsoft C 2.03 (1984): MC1 then MC2, in DOSBox-X (settings: msc203 -- their folder)."""
    name = "msc203"
    bits = 16

    def __init__(self, home):
        self.home = home

    def object(self, source, stem="cand", timeout=120):
        """-> the .OBJ's bytes, or None"""
        import shutil
        from codebreaker.build import dos_tools
        _ok, made, work = dos_tools.run({"c": self.home}, ["c:mc1 %s.c >NUL" % stem, "c:mc2 %s >NUL" % stem],
                                        [stem + ".OBJ"], timeout=max(timeout, 30), files={stem + ".c": source},
                                        cpu={"core": "auto", "cputype": "386"})
        if work:
            shutil.rmtree(work, ignore_errors=True)
        return made.get(stem + ".OBJ")

    def compile(self, source, work=None, header="", function=""):
        """-> (bytes, mask) of the public function named, else of the first code segment; or None"""
        from codebreaker.build import msc6
        obj = self.object(header + source)
        if obj is None:
            return None
        if function:
            return msc6.function_code(obj, function)
        code, mask = msc6.object_code(obj)
        return (bytes(code), bytes(mask)) if code else None

    def units(self):
        return []


def _scratch():
    from codebreaker.base import settings
    return settings.state("period")


def _set_up():
    """{name: the setting it needs, and whether that is there}"""
    from codebreaker.base import settings
    from codebreaker.build import dos_tools, msc6
    folder = lambda key, *files: bool(settings.get(key)) and all(
        os.path.isfile(os.path.join(settings.get(key), f)) for f in files)
    return {"msvc2": folder("period_compiler", "CL.EXE"),
            "xenix-c": folder("xenix_c", "xcc.py"),
            "msc6": msc6.available("msc6"),
            "msvc152": msc6.available("msvc152"),
            "msc203": bool(dos_tools.emulator()) and folder("msc203", "MC1.EXE", "MC2.EXE")}


NAMES = ("msvc2", "xenix-c", "msc6", "msvc152", "msc203")


def names():
    """The period compilers that are set up here."""
    have = _set_up()
    return [n for n in NAMES if have[n]]


def get(name, target=None, flags=None):
    """The period compiler `name` (NAMES): .compile(source, work, header="", function="") ->
    (bytes, mask) or None, .units(), .bits. target: the XENIX reconstruction's (default link240);
    flags: the compiler's options, else its usual ones."""
    from codebreaker.base import settings
    if name not in NAMES or not _set_up()[name]:
        raise ValueError("no period compiler %r set up (have: %s)" % (name, ", ".join(names()) or "none"))
    if name == "xenix-c":
        return XenixC(settings.get("xenix_c"), target or "link240")
    if name == "msvc2":
        return Msvc2(os.path.join(settings.get("period_compiler"), "CL.EXE"), *([flags] if flags else []))
    if name == "msc203":
        return Msc203(settings.get("msc203"))
    return MicrosoftC16(name, *([flags] if flags else []))
