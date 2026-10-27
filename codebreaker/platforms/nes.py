"""The NES (Famicom): 6502 code (the 2A03, without decimal mode) on an iNES cartridge image.
Its programs were written in assembly, which is what comes back (source/nes_writer.py); the
cartridge built from that source is compared with the original, and its functions are run
in CodeBreaker's own 6502 (emulate/m6502.py)."""
from codebreaker.platforms import Platform


def _recognise(path):
    from codebreaker.formats import nes
    return nes.is_nes(path)


PLATFORM = Platform(
    name="nes", title="NES", family="6502", container="iNES (.nes)",
    cpu="6502:LE:16:NES:default", source="asm",
    lane="codebreaker.verify.lanes.nes", runner=("codebreaker.emulate.m6502", "run"), recognise=_recognise)
