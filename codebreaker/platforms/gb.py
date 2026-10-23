"""The Game Boy and Game Boy Color: SM83 code on a cartridge image. Their programs were written in
assembly, which is what comes back (source/gb_writer.py, RGBDS syntax); the cartridge built from
that source is compared with the original, and its functions are run in CodeBreaker's own SM83
(emulate/sm83.py)."""
from codebreaker.platforms import Platform


def _recognise(path):
    from codebreaker.formats import gb
    return gb.is_gb(path)


PLATFORM = Platform(
    name="gb", title="Game Boy / Game Boy Color", family="sm83", container="cartridge image (.gb, .gbc)",
    cpu="sm83:LE:16:default", source="asm",
    lane="codebreaker.verify.gb", runner=("codebreaker.emulate.sm83", "run"), recognise=_recognise)
