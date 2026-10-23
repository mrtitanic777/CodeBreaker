"""platforms -- one description per platform, which every stage asks.

    from codebreaker import platforms
    p = platforms.identify(path)        # the Platform a file belongs to, or None
    p.name, p.title, p.cpu, p.source    # "pe", "32-bit Windows", "x86:LE:32:default:windows", "c"
    p.lane_module()                     # its kind module (verify/<kind>.py): profile, prepare, build, judge
    p.run_function()                    # what runs its code, or None
    p.readable(path)                    # the file the stages read (a .xex: its basefile)
    platforms.named("powerpc")          # by the name results are filed under
    platforms.for_machine(0x1F2)        # a PE machine type's platform

A platform is what a program was made for: the processor its code is for (the decompiler's
language for it), the container the code comes in, the language its functions come back as,
the module that reconstructs and judges it, and what runs its code beside the original's. Each
is one file here (win32.py, win16.py ...), so a new platform adds a description, not branches
in every stage (docs/PLATFORM_DESCRIPTOR.md).

`name` is what results are filed under in the function store, so it keeps the name each lane
has always had ("pe" is 32-bit Windows). Platforms are tried in ORDER: the first that
recognises a file is its platform.
"""
import importlib
import os

ORDER = ("java", "xbox360", "xbox", "nes", "sms", "gb", "genesis", "zx", "msx", "win32", "win16", "vxd", "dos")


class Platform(object):
    def __init__(self, name, title, family, cpu, source, lane, recognise, runner=None, machine=None,
                 open=None, container=""):
        self.name, self.title, self.family, self.cpu, self.source = name, title, family, cpu, source
        self.lane, self.runner, self.machine, self.container = lane, runner, machine, container
        self._recognise, self._open = recognise, open

    def recognises(self, path):
        try:
            return bool(self._recognise(path))
        except (OSError, ValueError):
            return False

    def readable(self, path):
        """The file the stages read: the image inside a container, else the file itself."""
        return self._open(path) if self._open else path

    def lane_module(self):
        return importlib.import_module(self.lane) if self.lane else None

    def run_function(self):
        if not self.runner:
            return None
        module, name = self.runner
        return getattr(importlib.import_module(module), name)

    def __repr__(self):
        return "<platform %s: %s>" % (self.name, self.title)


_ALL = None


def all():
    """Every platform, in the order they are tried."""
    global _ALL
    if _ALL is None:
        _ALL = [importlib.import_module("codebreaker.platforms." + m).PLATFORM for m in ORDER]
    return _ALL


def identify(path):
    """The platform `path` belongs to, or None when no platform recognises it."""
    for p in all():
        if p.recognises(path):
            return p
    return None


def named(name):
    """The platform results are filed under `name`, or None."""
    return next((p for p in all() if p.name == name), None)


def for_machine(machine):
    """The platform of a PE image with this machine type, or None."""
    return next((p for p in all() if p.machine == machine), None)


def lanes():
    """{name: kind module} for every platform that has a lane."""
    return {p.name: p.lane for p in all() if p.lane}


def runners():
    """{name: (module, function)} for every platform whose code CodeBreaker can run."""
    return {p.name: p.runner for p in all() if p.runner}


def lane_of(path):
    """The executable class of a file by its header (base/corpus.lane), cached there."""
    from codebreaker.base import corpus
    return corpus.lane(path)


def pe_machine(path):
    from codebreaker.formats.pe import open_pe
    return open_pe(path).machine


def is_folder_or_java(path):
    return os.path.isdir(path) or path.lower().endswith((".java", ".jar"))
