"""Windows 95 device drivers: LE images that run in ring 0 (VxDs), written in assembly.

The six slots: the LE reader (formats/le.py; a W3 container's members, vmm32.vxd's); the 386 in
flat ring 0; a driver's register convention (arguments and results in registers, the carry and
zero flags an answer too); the driver emulator (emulate/driver.py, on Unicorn); the kit's wasm
(build/driver.tool: the one that knows the DDK's macros); the driver written back as assembly and
linked as an LE by wlink (source/driver_writer.py, build/driver.py)."""
from codebreaker.platforms import Compiler, Convention, Cpu, Emulator, Loader, Packager, Platform, Ref


def _recognise(path):
    """An LE image, or a W3 container of them (vmm32.vxd: VMM and the drivers loaded with it)."""
    from codebreaker.formats import detect
    return detect.holds_drivers(path, bundles=(b"W3",))


def entries(path):
    """{label: (object, offset)} of the first driver in the file: its control procedure, its
    services, its API procedures (formats/le.entry_points)."""
    from codebreaker.formats import le
    return le.entry_points(le.members(path)[0][1])


def tools():
    from codebreaker.build import driver
    return {t: driver.tool(t) for t in ("wasm", "wlink")}


PLATFORM = Platform(
    name="le", title="device drivers", family="x86", source="asm",
    lane="codebreaker.verify.lanes.driver", recognise=_recognise,
    loader=Loader(container="LE", open=Ref("codebreaker.formats.le", "members"), entries=Ref(__name__, "entries"),
                  parts=Ref("codebreaker.formats.le", "members")),
    cpu=Cpu("x86:LE:32:default:windows", bits=32, endian="little", decoder=("capstone", "x86", 32),
            registers=("EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"), stack="ESP"),
    convention=Convention(names=("registers",), arguments="registers", word=4, order="registers",
                          results=("EAX", "CF", "ZF"), pops="callee",
                          preserved=("EBX", "ECX", "EDX", "ESI", "EDI", "EBP")),
    emulator=Emulator(engine="unicorn", run=Ref("codebreaker.emulate.driver", "recorded")),
    compiler=Compiler(language="asm", toolchain="openwatcom wasm, the DDK's macros",
                      tool=Ref("codebreaker.build.driver", "tool", "wasm"), tools=Ref(__name__, "tools")),
    packager=Packager(writer=Ref("codebreaker.source.driver_writer", "reconstruct"),
                      functions=Ref("codebreaker.build.driver", "build"), tools=Ref(__name__, "tools")))
