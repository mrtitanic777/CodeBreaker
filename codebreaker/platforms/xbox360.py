"""The Xbox 360: PowerPC code (Xenon, with VMX128) in a big-endian PE image (machine type
0x1F2), shipped inside a XEX2 container whose image -- the basefile -- native/xex unpacks.

The six slots: the XEX2 container opened to its basefile, read as a PowerPC image
(formats/powerpc_image.py); Xenon, 64-bit big-endian; the PowerPC calling convention (arguments
in r3-r10, the result in r3, r14-r31 kept); CodeBreaker's own PowerPC interpreter
(emulate/powerpc.py); the Xbox 360 SDK's cl.exe; its link.exe (build/functions.build_powerpc)."""
from codebreaker.platforms import (Compiler, Convention, Cpu, Emulator, Loader, Packager, Platform, Ref,
                                   lane_of, pe_machine)


def _recognise(path):
    from codebreaker.formats import xex
    return xex.is_xex(path) or (lane_of(path) == "pe" and pe_machine(path) == 0x01F2)


def _open(path):
    from codebreaker.formats import xex
    return xex.basefile(path) if xex.is_xex(path) else path


def compiler():
    from codebreaker.build import functions
    tools = functions.powerpc_tools()
    return tools[0] if tools else ""


def tools():
    from codebreaker.build import functions
    cl, link, lib = functions.powerpc_tools() or ("", "", "")
    return {"cl": cl, "link": link, "libcmt": lib}


PLATFORM = Platform(
    name="powerpc", title="Xbox 360 (PowerPC)", family="PowerPC", source="c", machine=0x01F2,
    lane="codebreaker.verify.lanes.powerpc", recognise=_recognise,
    loader=Loader(container="XEX2 -> PE", readable=_open, open=Ref("codebreaker.formats.powerpc_image", "read")),
    cpu=Cpu("PowerPC:BE:64:Xenon:homed", bits=64, endian="big", decoder="codebreaker.emulate.powerpc",
            registers=tuple("r%d" % i for i in range(32)), stack="r1"),
    convention=Convention(names=("__cdecl",), arguments=8, word=8, order="registers", results=("r3", "f1"),
                          pops="caller", preserved=tuple("r%d" % i for i in range(14, 32))),
    emulator=Emulator(engine="CodeBreaker's own interpreter", run=Ref("codebreaker.emulate.powerpc", "run")),
    compiler=Compiler(language="c", toolchain="the Xbox 360 SDK", tool=Ref(__name__, "compiler"),
                      tools=Ref(__name__, "tools"), flags=Ref("codebreaker.build.functions", "CFLAGS_POWERPC")),
    packager=Packager(functions=Ref("codebreaker.build.functions", "build_powerpc"), tools=Ref(__name__, "tools")))
