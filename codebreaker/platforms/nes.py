"""The NES (Famicom): 6502 code (the 2A03, without decimal mode) on an iNES cartridge image.
Its programs were written in assembly, which is what comes back (source/nes_writer.py); the
cartridge built from that source is compared with the original, and its functions are run
in CodeBreaker's own 6502 (emulate/m6502.py)."""
from codebreaker.platforms import Cpu, machine


def _recognise(path):
    from codebreaker.formats import nes
    return nes.is_nes(path)


PLATFORM = machine(
    "nes", "NES", "6502", "iNES (.nes)",
    Cpu("6502:LE:16:NES:default", bits=8, endian="little", decoder="codebreaker.decompile.m6502",
        registers=("A", "X", "Y", "P", "S"), stack="S"),
    _recognise, reader=("codebreaker.formats.nes", "read"), runner=("codebreaker.emulate.m6502", "recorded"),
    assembler=("codebreaker.build.m6502_asm", "build"), writer=("codebreaker.source.nes_writer", "reconstruct"),
    results=("A", "X", "Y", "P"), toolchain="CodeBreaker's own assembler, or cc65")
