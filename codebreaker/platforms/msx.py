"""The MSX: Z80 code on a cartridge (.rom). Its programs were written in assembly, which is what
comes back (source/msx_writer.py, sjasmplus / pasmo syntax); the cartridge built from that source
is compared with the original, and its functions are run in CodeBreaker's own Z80
(emulate/z80.py)."""
from codebreaker.platforms import Cpu, machine


def _recognise(path):
    from codebreaker.formats import msx
    return msx.is_msx(path)


PLATFORM = machine(
    "msx", "MSX", "z80", "cartridge image (.rom)",
    Cpu("z80:LE:16:default", bits=8, endian="little", decoder="codebreaker.decompile.z80",
        registers=("A", "F", "BC", "DE", "HL", "IX", "IY", "SP"), stack="SP"),
    _recognise, reader=("codebreaker.formats.msx", "read"), runner=("codebreaker.emulate.z80", "run"),
    assembler=("codebreaker.build.z80_asm", "build_flat"), writer=("codebreaker.source.msx_writer", "reconstruct"),
    results=("A", "F", "BC", "DE", "HL", "IX", "IY"), toolchain="CodeBreaker's own assembler, or sjasmplus / pasmo")
