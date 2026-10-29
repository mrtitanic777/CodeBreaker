"""The Sega Genesis / Mega Drive: 68000 code on a cartridge image (the Z80 beside it runs sound
code the 68000 copies in). Its programs were mostly written in assembly, which is what comes back
(source/genesis_writer.py, Motorola syntax); the cartridge built from that source is compared
with the original, and its functions are run in CodeBreaker's own 68000 (emulate/m68k.py)."""
from codebreaker.platforms import Cpu, machine


def _recognise(path):
    from codebreaker.formats import genesis
    return genesis.is_genesis(path)


PLATFORM = machine(
    "genesis", "Genesis / Mega Drive", "68000", "cartridge image (.md, .bin, .gen, .smd)",
    Cpu("68000:BE:32:default", bits=32, endian="big", decoder="codebreaker.decompile.m68k",
        registers=tuple("D%d" % i for i in range(8)) + tuple("A%d" % i for i in range(8)) + ("SR",), stack="A7"),
    _recognise, reader=("codebreaker.formats.genesis", "read"), runner=("codebreaker.emulate.m68k", "run"),
    assembler=("codebreaker.build.m68k_asm", "build"), writer=("codebreaker.source.genesis_writer", "reconstruct"),
    results=tuple("D%d" % i for i in range(8)) + tuple("A%d" % i for i in range(7)), toolchain="CodeBreaker's own assembler")
