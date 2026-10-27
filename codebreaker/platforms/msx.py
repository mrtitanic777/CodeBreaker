"""The MSX: Z80 code on a cartridge (.rom). Its programs were written in assembly, which is what
comes back (source/msx_writer.py, sjasmplus / pasmo syntax); the cartridge built from that source
is compared with the original, and its functions are run in CodeBreaker's own Z80
(emulate/z80.py)."""
from codebreaker.platforms import Platform


def _recognise(path):
    from codebreaker.formats import msx
    return msx.is_msx(path)


PLATFORM = Platform(
    name="msx", title="MSX", family="z80", container="cartridge image (.rom)",
    cpu="z80:LE:16:default", source="asm",
    lane="codebreaker.verify.lanes.msx", runner=("codebreaker.emulate.z80", "run"), recognise=_recognise)
