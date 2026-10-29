"""The Sega Master System and Game Gear (and the SG-1000 before them): Z80 code on a cartridge
image. Their programs were written in assembly, which is what comes back (source/sms_writer.py,
WLA-DX syntax); the cartridge built from that source is compared with the original, and its
functions are run in CodeBreaker's own Z80 (emulate/z80.py)."""
from codebreaker.platforms import Cpu, machine


def _recognise(path):
    from codebreaker.formats import sms
    return sms.is_sms(path)


PLATFORM = machine(
    "sms", "Master System / Game Gear", "z80", "cartridge image (.sms, .gg, .sg)",
    Cpu("z80:LE:16:default", bits=8, endian="little", decoder="codebreaker.decompile.z80",
        registers=("A", "F", "BC", "DE", "HL", "IX", "IY", "SP"), stack="SP"),
    _recognise, reader=("codebreaker.formats.sms", "read"), runner=("codebreaker.emulate.z80", "run"),
    assembler=("codebreaker.build.z80_asm", "build"), writer=("codebreaker.source.sms_writer", "reconstruct"),
    results=("A", "F", "BC", "DE", "HL", "IX", "IY"), toolchain="CodeBreaker's own assembler, or WLA-DX")
