"""16-bit Windows: NE modules (Windows 3.x and the 16-bit half of Windows 95). Their functions
come back as C or, where they were written in assembly, as assembly (analyze/origin.py).

The six slots: the NE reader (formats/ne.py); the x86 in 16-bit protected mode; the Pascal and C
conventions of 16-bit Windows (describe16 writes them: __stdcall16far ... ); the 16-bit emulator
(emulate/windows16.py, the judge's runner); OpenWatcom's wcc; the module linked by
build/module._build16 (wlink, with what is kept as assembly assembled by wasm), the judge's
build of the functions by build/functions.build16. Its steps on the engine: engine/win16.py."""
import os

from codebreaker.platforms import (Compiler, Convention, Cpu, Emulator, Loader, Packager, Platform, Ref,
                                   lane_of)


def segments(path):
    """The segment table: [{idx, kind, file_off, length, flags, minalloc, ...}]."""
    from codebreaker.formats.ne import NEFile
    return NEFile(path).segments()


def entries(path):
    from codebreaker.formats.ne import NEFile
    return NEFile(path).entries()


def call(path, export):
    """One export of a module called with zero arguments: (how it ended, its result)."""
    try:
        r = PLATFORM.emulator.run(path, ordinal=export.ordinal, args=[0] * PLATFORM.convention.arguments)
    except ImportError:
        raise
    except Exception as e:                                    # the emulator giving up is a result too
        return "error: %s" % type(e).__name__, None
    return r.ended, r.results.get("AX")


def library():
    """The medium model's C library (clibm.lib), linked with the rebuilt C."""
    from codebreaker.build import watcom
    return os.path.join(watcom.ow_lib("ne"), "clibm.lib")


def tools():
    from codebreaker.build import watcom
    return {t: watcom.ow_tool(t) or "" for t in ("wcc", "wlink", "wasm")}


PLATFORM = Platform(
    name="ne", title="16-bit Windows", family="x86", source="c or asm",
    lane="codebreaker.verify.lanes.win16", steps="codebreaker.engine.win16",
    recognise=lambda path: lane_of(path) == "ne",
    loader=Loader(container="NE", open=Ref("codebreaker.formats.ne", "NEFile"),
                  parts=Ref(__name__, "segments"), entries=Ref(__name__, "entries"),
                  exports=Ref("codebreaker.build.definitions", "exports"),
                  imports=Ref("codebreaker.build.definitions", "imports")),
    cpu=Cpu("x86:LE:16:Protected Mode:default", bits=16, endian="little", decoder=("capstone", "x86", 16),
            registers=("AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS"), stack="SP"),
    convention=Convention(names=("__stdcall16far", "__stdcall16near", "__cdecl16far", "__cdecl16near",
                                 "__fastcall16near", "__fastcall16far"),
                          arguments=8, word=2, order="pascal", results=("AX", "DX:AX"), pops="either",
                          preserved=("SI", "DI", "DS")),
    emulator=Emulator(engine="unicorn", run=Ref("codebreaker.emulate.windows16", "recorded"),
                      call=Ref(__name__, "call")),
    compiler=Compiler(language="c", toolchain="openwatcom", tool=Ref("codebreaker.build.watcom", "ow_tool", "wcc"),
                      tools=Ref(__name__, "tools"), flags=Ref("codebreaker.build.functions", "CFLAGS16"),
                      env=Ref("codebreaker.build.watcom", "ow_env", "ne"),
                      compile=Ref("codebreaker.build.functions", "_compile"), library=Ref(__name__, "library")),
    packager=Packager(link=Ref("codebreaker.build.module", "_build16"),
                      functions=Ref("codebreaker.build.functions", "build16"), tools=Ref(__name__, "tools"),
                      linker=Ref("codebreaker.build.watcom", "ow_tool", "wlink"),
                      assembler=Ref("codebreaker.build.watcom", "ow_tool", "wasm"),
                      resources=Ref("codebreaker.build.watcom", "ow_tool", "wrc")))
