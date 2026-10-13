"""period.py -- compilers of the original's time, as the repair search uses them.

    from codebreaker.build import period
    period.names()                              the period compilers set up on this machine
    p = period.get("xenix-c")
    built = p.compile(source_text, work)        (bytes, mask) of the code it gives, or None
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

    def compile(self, source, work):
        """Compile one source file's text. -> (bytes, mask) or None."""
        xcc = self._load()
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


def _scratch():
    from codebreaker.base import settings
    return settings.state("period")


def names():
    """The period compilers that are set up here."""
    from codebreaker.base import settings
    out = []
    if settings.get("period_compiler") and os.path.isfile(os.path.join(settings.get("period_compiler"), "CL.EXE")):
        out.append("msvc2")
    if settings.get("xenix_c") and os.path.isfile(os.path.join(settings.get("xenix_c"), "xcc.py")):
        out.append("xenix-c")
    return out


def get(name, target="link240"):
    from codebreaker.base import settings
    if name == "xenix-c" and "xenix-c" in names():
        return XenixC(settings.get("xenix_c"), target)
    raise ValueError("no period compiler %r set up (have: %s)" % (name, ", ".join(names()) or "none"))
