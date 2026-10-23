"""The ZX Spectrum: Z80 programs in snapshots (.sna, .z80) and on tapes (.tap). Its programs were
written in assembly (or BASIC around a machine-code core), and assembly is what comes back
(source/zx_writer.py, sjasmplus / pasmo syntax); the block built from that source is compared with
the original, and its functions are run in CodeBreaker's own Z80 (emulate/z80.py)."""
from codebreaker.platforms import Platform


def _recognise(path):
    from codebreaker.formats import zx
    return zx.is_zx(path)


PLATFORM = Platform(
    name="zx", title="ZX Spectrum", family="z80", container="snapshot or tape (.sna, .z80, .tap)",
    cpu="z80:LE:16:default", source="asm",
    lane="codebreaker.verify.zx", runner=("codebreaker.emulate.z80", "run"), recognise=_recognise)
