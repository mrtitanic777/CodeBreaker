"""32-bit Windows: PE images for the 386 and later (machine type 0x14C).

The six slots: the PE reader (formats/pe.py); the 386 in flat 32-bit mode; the Win32 conventions
(__stdcall, __cdecl, __fastcall, __thiscall: EAX the result, EBX ESI EDI EBP kept); the 32-bit
emulator (emulate/emulator.py trace_function, on Unicorn, as a run record); OpenWatcom's wcc386 with the options
c_text gives; the judge's build by
build/functions.build32."""
from codebreaker.platforms import (Compiler, Convention, Cpu, Emulator, Loader, Packager, Platform, Ref,
                                   lane_of, pe_machine)


def sections(path):
    """[(address, bytes)] of its code sections."""
    from codebreaker.formats.pe import open_pe
    return [(va, raw) for va, raw, _name in open_pe(path).code_sections()]


def tools():
    from codebreaker.build import watcom
    return {t: watcom.ow_tool(t) or "" for t in ("wcc386", "wlink")}


PLATFORM = Platform(
    name="pe", title="32-bit Windows", family="x86", source="c", machine=0x014C,
    lane="codebreaker.verify.lanes.win32",
    recognise=lambda path: lane_of(path) == "pe" and pe_machine(path) != 0x01F2,
    loader=Loader(container="PE", open=Ref("codebreaker.formats.pe", "open_pe"), parts=Ref(__name__, "sections"),
                  exports=Ref("codebreaker.build.definitions", "exports"),
                  imports=Ref("codebreaker.build.definitions", "imports")),
    cpu=Cpu("x86:LE:32:default:windows", bits=32, endian="little", decoder=("capstone", "x86", 32),
            registers=("EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"), stack="ESP"),
    convention=Convention(names=("__stdcall", "__cdecl", "__fastcall", "__thiscall"), arguments=4, word=4,
                          order="c", results=("EAX", "EDX:EAX"), pops="either",
                          preserved=("EBX", "ESI", "EDI", "EBP")),
    emulator=Emulator(engine="unicorn", run=Ref("codebreaker.emulate.emulator", "recorded")),
    compiler=Compiler(language="c", toolchain="openwatcom", tool=Ref("codebreaker.build.watcom", "ow_tool", "wcc386"),
                      tools=Ref(__name__, "tools"), flags=Ref("codebreaker.decompile.c_text", "CFLAGS"),
                      env=Ref("codebreaker.build.watcom", "ow_env", "pe")),
    packager=Packager(functions=Ref("codebreaker.build.functions", "build32"), tools=Ref(__name__, "tools"),
                      linker=Ref("codebreaker.build.watcom", "ow_tool", "wlink"),
                      assembler=Ref("codebreaker.build.watcom", "ow_tool", "wasm"),
                      resources=Ref("codebreaker.build.watcom", "ow_tool", "wrc")))
