"""Real-mode DOS programs: MZ executables, and .COM files and device drivers, which have no
header (formats/dos.py tells them apart). Their source is assembly, rebuilt and compared.

The six slots: the DOS reader (formats/dos.py: the load module, its relocations, a packed one
unpacked); the 8086 in real mode; the near and far C conventions the decompiler writes; no
emulator -- nothing is run: a program that comes back byte for byte IS the original; OpenWatcom's
wasm; the program written back as assembly and linked by wlink (source/dos_writer.py)."""
from codebreaker.platforms import Compiler, Convention, Cpu, Emulator, Loader, Packager, Platform, Ref


def _recognise(path):
    from codebreaker.formats import detect
    return detect.kind(path) == "dos"   # (.COM files and plain driver images too, which have no header)


def tools():
    from codebreaker.build import driver
    return {t: driver.tool(t) for t in ("wasm", "wlink")}


PLATFORM = Platform(
    name="dos", title="DOS programs", family="x86", source="asm",
    lane="codebreaker.verify.lanes.dos", recognise=_recognise,
    loader=Loader(container="MZ, COM, SYS", open=Ref("codebreaker.formats.dos", "read"),
                  parts=Ref("codebreaker.formats.dos", "chain")),
    cpu=Cpu("x86:LE:16:Real Mode:default", bits=16, endian="little", decoder=("capstone", "x86", 16),
            registers=("AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS"), stack="SP"),
    convention=Convention(names=("__cdecl16near", "__cdecl16far"), word=2, order="c", results=("AX", "DX:AX"),
                          pops="caller"),
    emulator=Emulator(),
    compiler=Compiler(language="asm", toolchain="openwatcom wasm", tool=Ref("codebreaker.build.driver", "tool", "wasm"),
                      tools=Ref(__name__, "tools")),
    packager=Packager(writer=Ref("codebreaker.source.dos_writer", "reconstruct"), tools=Ref(__name__, "tools")))
