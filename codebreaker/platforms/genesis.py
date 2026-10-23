"""The Sega Genesis / Mega Drive: 68000 code on a cartridge image (the Z80 beside it runs sound
code the 68000 copies in). Its programs were mostly written in assembly, which is what comes back
(source/genesis_writer.py, Motorola syntax); the cartridge built from that source is compared
with the original, and its functions are run in CodeBreaker's own 68000 (emulate/m68k.py)."""
from codebreaker.platforms import Platform


def _recognise(path):
    from codebreaker.formats import genesis
    return genesis.is_genesis(path)


PLATFORM = Platform(
    name="genesis", title="Genesis / Mega Drive", family="68000", container="cartridge image (.md, .bin, .gen, .smd)",
    cpu="68000:BE:32:default", source="asm",
    lane="codebreaker.verify.genesis", runner=("codebreaker.emulate.m68k", "run"), recognise=_recognise)
