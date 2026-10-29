"""The ZX Spectrum: Z80 programs in snapshots (.sna, .z80) and on tapes (.tap). Its programs were
written in assembly (or BASIC around a machine-code core), and assembly is what comes back
(source/zx_writer.py, sjasmplus / pasmo syntax); the block built from that source is compared with
the original, and its functions are run in CodeBreaker's own Z80 (emulate/z80.py)."""
from codebreaker.platforms import Cpu, machine


def _recognise(path):
    from codebreaker.formats import zx
    return zx.is_zx(path)


PLATFORM = machine(
    "zx", "ZX Spectrum", "z80", "snapshot or tape (.sna, .z80, .tap)",
    Cpu("z80:LE:16:default", bits=8, endian="little", decoder="codebreaker.decompile.z80",
        registers=("A", "F", "BC", "DE", "HL", "IX", "IY", "SP"), stack="SP"),
    _recognise, reader=("codebreaker.formats.zx", "read"), runner=("codebreaker.emulate.z80", "recorded"),
    assembler=("codebreaker.build.z80_asm", "build_flat"), writer=("codebreaker.source.zx_writer", "reconstruct"),
    results=("A", "F", "BC", "DE", "HL", "IX", "IY"), toolchain="CodeBreaker's own assembler, or sjasmplus / pasmo")
