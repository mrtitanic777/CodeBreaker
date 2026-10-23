"""The Sega Master System and Game Gear (and the SG-1000 before them): Z80 code on a cartridge
image. Their programs were written in assembly, which is what comes back (source/sms_writer.py,
WLA-DX syntax); the cartridge built from that source is compared with the original, and its
functions are run in CodeBreaker's own Z80 (emulate/z80.py)."""
from codebreaker.platforms import Platform


def _recognise(path):
    from codebreaker.formats import sms
    return sms.is_sms(path)


PLATFORM = Platform(
    name="sms", title="Master System / Game Gear", family="z80", container="cartridge image (.sms, .gg, .sg)",
    cpu="z80:LE:16:default", source="asm",
    lane="codebreaker.verify.sms", runner=("codebreaker.emulate.z80", "run"), recognise=_recognise)
