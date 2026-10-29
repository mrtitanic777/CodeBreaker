"""platforms -- one description per platform, which every stage asks.

    from codebreaker import platforms
    p = platforms.identify(path)        # the Platform a file belongs to, or None
    p.name, p.title, p.cpu, p.source    # "pe", "32-bit Windows", "x86:LE:32:default:windows", "c"
    p.lane_module()                     # its kind module (verify/<kind>.py): profile, prepare, build, judge
    p.run_function()                    # what runs its code, or None
    p.readable(path)                    # the file the stages read (a .xex: its basefile)
    platforms.named("powerpc")          # by the name results are filed under (or its file's: "xbox360")
    platforms.for_machine(0x1F2)        # a PE machine type's platform

A platform is what a program was made for: the processor its code is for (the decompiler's
language for it), the container the code comes in, the language its functions come back as,
the module that reconstructs and judges it, and what runs its code beside the original's. Each
is one file here (win32.py, win16.py ...), so a new platform adds a description, not branches
in every stage (docs/PLATFORM_DESCRIPTOR.md).

The six slots (docs/ARCHITECTURE.md part 4). Every platform fills the same six, and the shared
machinery asks them instead of asking which platform it is:

    p.loader        open the file: its parts (segments, sections), entry points, exports, imports
    p.cpu           the processor: the SLEIGH language id (p.cpu is that string), the decoder,
                    the register model
    p.convention    how a function is called: its arguments, its result, who pops the arguments
    p.emulator      what runs a function: the judge's runner, and calling one export of a module
    p.compiler      what rebuilds a function from its source: the toolchain, its options
    p.packager      what makes the image of the rebuilt parts: the link, the assembly

A slot's work is done by functions of other modules, named (Ref) and imported only when used, so
describing a platform costs nothing. A slot a platform has no use for is the empty one of its
class (a DOS program is not run, so its emulator says so: p.emulator.run is None).

`name` is what results are filed under in the function store, so it keeps the name each lane
has always had ("pe" is 32-bit Windows). Platforms are tried in ORDER: the first that
recognises a file is its platform. `id` is the descriptor's file name (win16, win32, vxd ...),
which the engine's steps are filed under.
"""
import importlib
import os

ORDER = ("java", "xbox360", "xbox", "nes", "sms", "gb", "genesis", "zx", "msx", "win32", "win16", "vxd", "dos")
SLOTS = ("loader", "cpu", "convention", "emulator", "compiler", "packager")


class Ref(object):
    """A function or value of another module, named here and imported when first used.
    Leading arguments may be given, as functools.partial does: Ref("m", "f", "wcc")() is m.f("wcc")."""

    def __init__(self, module, name, *args, **kw):
        self.module, self.name, self.args, self.kw = module, name, args, kw

    def target(self):
        return getattr(importlib.import_module(self.module), self.name)

    def __call__(self, *args, **kw):
        return self.target()(*(self.args + args), **dict(self.kw, **kw))

    def pair(self):
        """(module, name), as the older tables wrote what runs a platform's code."""
        return self.module, self.name

    def __eq__(self, other):
        return isinstance(other, Ref) and (self.module, self.name, self.args, self.kw) == \
            (other.module, other.name, other.args, other.kw)

    def __hash__(self):
        return hash((self.module, self.name, self.args))

    def __repr__(self):
        args = ", ".join([repr(a) for a in self.args] + ["%s=%r" % kv for kv in sorted(self.kw.items())])
        return "%s.%s%s" % (self.module, self.name, "(%s)" % args if args else "")


class Slot(object):
    """One of a platform's six parts: what it says about itself are its fields (FIELDS, each None
    unless given), the functions that do its work Refs among them."""
    FIELDS = ()

    def __init__(self, **fields):
        unknown = set(fields) - set(self.FIELDS)
        if unknown:
            raise TypeError("%s has no %s" % (type(self).__name__, ", ".join(sorted(unknown))))
        for f in self.FIELDS:
            setattr(self, f, fields.get(f))

    def given(self):
        """{field: value} of the fields this platform filled."""
        return {f: getattr(self, f) for f in self.FIELDS if getattr(self, f) is not None}

    def __bool__(self):
        return bool(self.given())

    def __repr__(self):
        return "%s(%s)" % (type(self).__name__, ", ".join("%s=%r" % kv for kv in self.given().items()))


class Loader(Slot):
    """Open the file.
        container   what the code comes in ("NE", "PE", "LE", "MZ, COM, SYS", "iNES (.nes)" ...)
        open        path -> the parsed file (NEFile, the PE reader, the LE reader ...)
        readable    path -> the file the stages read (a container's image: a .xex's basefile)
        parts       path -> its parts as the stages want them (segments, sections, objects)
        entries     path -> its entry points
        exports     path -> what it exports (build/definitions.Export, for the Windows lanes)
        imports     path -> what it takes from other modules"""
    FIELDS = ("container", "open", "readable", "parts", "entries", "exports", "imports")


class Cpu(str):
    """The processor. The value IS the SLEIGH language id the decompiler reads the code as (so
    every stage that wrote `p.cpu` into a description still does); besides it:
        bits        the word the code works in (16, 32, 64, 8)
        endian      "little" | "big"
        decoder     what disassembles it outside the decompiler: ("capstone", arch, mode) or a
                    module of CodeBreaker's own
        registers   the register model: the registers a run starts from and is compared on
        stack       the stack pointer's name"""
    FIELDS = ("bits", "endian", "decoder", "registers", "stack")

    def __new__(cls, language="", **fields):
        self = str.__new__(cls, language)
        unknown = set(fields) - set(cls.FIELDS)
        if unknown:
            raise TypeError("Cpu has no %s" % ", ".join(sorted(unknown)))
        for f in cls.FIELDS:
            setattr(self, f, fields.get(f))
        return self

    @property
    def language(self):
        return str(self)

    def given(self):
        out = {"language": str(self)} if str(self) else {}
        out.update({f: getattr(self, f) for f in self.FIELDS if getattr(self, f) is not None})
        return out

    def disassembler(self, detail=False):
        """The decoder this processor declares, made once per thread (decoder() below). Only
        for a ("capstone", arch, bits) decoder; one of CodeBreaker's own modules is imported."""
        d = self.decoder
        if isinstance(d, tuple) and d and d[0] == "capstone":
            return decoder(d[2], arch=d[1], detail=detail)
        if isinstance(d, str):
            return importlib.import_module(d)
        raise ValueError("%r declares no decoder" % self)


# The capstone decoders, one per thread and option set. Making one is a thousand times dearer
# than decoding an instruction with it, and a decoder is not shared between threads. Its options
# (detail) are part of the key and are set once, here: a caller never changes a decoder it was
# handed, so what one caller asked for never changes what another's instructions say.
_DECODERS = __import__("threading").local()


def decoder(bits=16, arch="x86", detail=False):
    """A capstone decoder for `arch` ("x86": 16, 32 or 64 bits; "ppc": 32 or 64 bits, big-endian,
    as the Xbox 360's) or for a Cpu (its declared decoder), kept per thread. `detail`: with each
    instruction's operands and the registers it reads and writes."""
    if isinstance(bits, Cpu):
        return bits.disassembler(detail)
    key = (arch, bits, bool(detail))
    held = getattr(_DECODERS, "held", None)
    if held is None:
        held = _DECODERS.held = {}
    md = held.get(key)
    if md is None:
        import capstone
        if arch == "x86":
            md = capstone.Cs(capstone.CS_ARCH_X86, {16: capstone.CS_MODE_16, 32: capstone.CS_MODE_32,
                                                    64: capstone.CS_MODE_64}[bits])
        elif arch == "ppc":
            md = capstone.Cs(capstone.CS_ARCH_PPC, {32: capstone.CS_MODE_32, 64: capstone.CS_MODE_64}[bits]
                             | capstone.CS_MODE_BIG_ENDIAN)
        else:
            raise ValueError("no capstone decoder for %r" % arch)
        md.detail = bool(detail)
        held[key] = md
    return md


class Convention(Slot):
    """How a function is called, as the judge enters it and the decompiler writes it.
        names       the conventions the decompiler writes its prototypes with
        arguments   how many arguments (words) the judge gives a function whose count is not known,
                    or "registers" when they are in registers
        word        bytes per argument word
        order       "pascal" (the first pushed first) | "c" (the first pushed last) | "registers"
        results     where the result is: one register, or the pair a wide one is in
        pops        who removes the arguments: "callee" | "caller" | "either" (the function says)
        preserved   the registers a function must hand back as it found them"""
    FIELDS = ("names", "arguments", "word", "order", "results", "pops", "preserved")


class Emulator(Slot):
    """What runs a function.
        run         the judge's runner: runs one function of the original or the rebuild and
                    returns the record verify/<kind>.py makes a fingerprint of
        call        (path, export) -> (how it ended, its result): one export of a module called
                    with zero arguments (build/module.run's smoke test of a linked module)
        engine      what it is ("unicorn", or CodeBreaker's own interpreter)"""
    FIELDS = ("run", "call", "engine")


class Compiler(Slot):
    """What rebuilds a function from its source.
        language    what the source is: "c", "asm", "java" ...
        toolchain   whose tools ("openwatcom", "the Xbox 360 SDK", "cc65" ...)
        tool        () -> the compiler's (or assembler's) path
        tools       () -> every tool the build uses, {name: path} (stamps and profiles)
        flags       the options every function is compiled with (a value, or a Ref to one)
        env         () -> the environment the tools run in
        compile     (cc, flags, env, folder, stem, text) -> "" or why the compiler refused it
        library     () -> the runtime library linked with the rebuilt code"""
    FIELDS = ("language", "toolchain", "tool", "tools", "flags", "env", "compile", "library")

    def options(self):
        """The flags as a list (a Ref is read)."""
        f = self.flags
        return list(f.target() if isinstance(f, Ref) else f or ())


class Packager(Slot):
    """What makes the image of the rebuilt parts.
        link        the whole module linked from its reconstruction (build/module's builder for
                    the Windows lanes): (Built, path, prepared, folder) -> the image's path
        functions   the module the judge runs its functions in, built from their source
                    (build/functions.build16, build32 ...)
        writer      the module that writes a program's whole source and rebuilds it
                    (source/<machine>_writer.py: reconstruct(path, folder))
        tools       () -> {name: path} of the linker, assembler ... (profiles)
        linker      () -> the linker's path
        assembler   () -> the assembler's path (what is kept as assembly, the helpers C calls)
        resources   () -> the resource compiler's path"""
    FIELDS = ("link", "functions", "writer", "tools", "linker", "assembler", "resources")


_SLOT_CLASSES = {"loader": Loader, "cpu": Cpu, "convention": Convention, "emulator": Emulator,
                 "compiler": Compiler, "packager": Packager}


class Platform(object):
    def __init__(self, name, title, family, cpu, source, lane, recognise, runner=None, machine=None,
                 open=None, container="", loader=None, convention=None, emulator=None, compiler=None,
                 packager=None, steps=None):
        self.name, self.title, self.family, self.source = name, title, family, source
        self.lane, self.machine, self.steps = lane, machine, steps
        self._recognise = recognise
        self.id = None                                          # (set by all(): the descriptor's file)
        # the six slots; the older keywords (runner, open, container, a cpu string) fill theirs
        self.cpu = cpu if isinstance(cpu, Cpu) else Cpu(cpu or "")
        self.loader = loader or Loader()
        if container and not self.loader.container:
            self.loader.container = container
        if open and not self.loader.readable:
            self.loader.readable = open
        self.emulator = emulator or Emulator()
        if runner and not self.emulator.run:
            self.emulator.run = Ref(*runner)
        self.convention = convention or Convention()
        self.compiler = compiler or Compiler()
        self.packager = packager or Packager()

    @property
    def container(self):
        return self.loader.container or ""

    @property
    def runner(self):
        """(module, function) of what runs its code, or None (the emulator slot's run)."""
        return self.emulator.run.pair() if self.emulator.run else None

    def slots(self):
        """{slot: the slot} -- the six, in order."""
        return {s: getattr(self, s) for s in SLOTS}

    def recognises(self, path):
        try:
            return bool(self._recognise(path))
        except (OSError, ValueError):
            return False

    def readable(self, path):
        """The file the stages read: the image inside a container, else the file itself."""
        return self.loader.readable(path) if self.loader.readable else path

    def lane_module(self):
        return importlib.import_module(self.lane) if self.lane else None

    def steps_module(self):
        """The module of its steps on the engine (engine/<id>.py), or None."""
        return importlib.import_module(self.steps) if self.steps else None

    def run_function(self):
        return self.emulator.run.target() if self.emulator.run else None

    def __repr__(self):
        return "<platform %s: %s>" % (self.name, self.title)


def machine(name, title, family, container, cpu, recognise, reader, runner, assembler, writer, results,
            toolchain="CodeBreaker's own assembler"):
    """A machine whose programs were written in assembly and come back as assembly (the console
    and home-computer lanes: verify/lanes/image_lane.py): its reader, its processor, registers
    in and out, CodeBreaker's own interpreter, the assembler that rebuilds it and the writer of
    its source. `reader`, `runner`, `assembler`, `writer`: (module, function)."""
    return Platform(
        name=name, title=title, family=family, source="asm", lane="codebreaker.verify.lanes." + name,
        recognise=recognise, cpu=cpu,
        loader=Loader(container=container, open=Ref(*reader)),
        convention=Convention(names=("registers",), arguments="registers", order="registers", results=results,
                              pops="caller"),
        emulator=Emulator(engine="CodeBreaker's own interpreter", run=Ref(*runner)),
        compiler=Compiler(language="asm", toolchain=toolchain, tool=Ref(*assembler)),
        packager=Packager(writer=Ref(*writer)))


_ALL = None


def all():
    """Every platform, in the order they are tried."""
    global _ALL
    if _ALL is None:
        found = []
        for m in ORDER:
            p = importlib.import_module("codebreaker.platforms." + m).PLATFORM
            p.id = m
            found.append(p)
        _ALL = found
    return _ALL


def identify(path):
    """The platform `path` belongs to, or None when no platform recognises it."""
    for p in all():
        if p.recognises(path):
            return p
    return None


def named(name):
    """The platform results are filed under `name` (or whose descriptor is `name`.py), or None."""
    return next((p for p in all() if p.name == name), None) or next((p for p in all() if p.id == name), None)


def of(kind):
    """named(kind), which must exist: the platform a lane's KIND (or a Built's kind) is."""
    p = named(kind)
    if p is None:
        raise ValueError("no platform %r" % kind)
    return p


def for_machine(machine):
    """The platform of a PE image with this machine type, or None."""
    return next((p for p in all() if p.machine == machine), None)


def lanes():
    """{name: kind module} for every platform that has a lane."""
    return {p.name: p.lane for p in all() if p.lane}


def runners():
    """{name: (module, function)} for every platform whose code CodeBreaker can run."""
    return {p.name: p.runner for p in all() if p.runner}


def step_modules():
    """{id: the module of its steps} for every platform on the engine."""
    return {p.id: p.steps for p in all() if p.steps}


def lane_of(path):
    """The executable class of a file by its header (base/corpus.lane), cached there."""
    from codebreaker.base import corpus
    return corpus.lane(path)


def pe_machine(path):
    from codebreaker.formats.pe import open_pe
    return open_pe(path).machine


def is_folder_or_java(path):
    return os.path.isdir(path) or path.lower().endswith((".java", ".jar"))
