"""The Game Boy and Game Boy Color: SM83 code on a cartridge image. Their programs were written in
assembly, which is what comes back (source/gb_writer.py, RGBDS syntax); the cartridge built from
that source is compared with the original, and its functions are run in CodeBreaker's own SM83
(emulate/sm83.py)."""
from codebreaker.platforms import Cpu, machine


def _recognise(path):
    from codebreaker.formats import gb
    return gb.is_gb(path)


PLATFORM = machine(
    "gb", "Game Boy / Game Boy Color", "sm83", "cartridge image (.gb, .gbc)",
    Cpu("sm83:LE:16:default", bits=8, endian="little", decoder="codebreaker.decompile.sm83",
        registers=("A", "F", "BC", "DE", "HL", "SP"), stack="SP"),
    _recognise, reader=("codebreaker.formats.gb", "read"), runner=("codebreaker.emulate.sm83", "run"),
    assembler=("codebreaker.build.sm83_asm", "build"), writer=("codebreaker.source.gb_writer", "reconstruct"),
    results=("A", "F", "BC", "DE", "HL"), toolchain="CodeBreaker's own assembler, or RGBDS")
