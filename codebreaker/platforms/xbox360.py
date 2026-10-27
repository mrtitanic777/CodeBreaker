"""The Xbox 360: PowerPC code (Xenon, with VMX128) in a big-endian PE image (machine type
0x1F2), shipped inside a XEX2 container whose image -- the basefile -- native/xex unpacks."""
from codebreaker.platforms import Platform, lane_of, pe_machine


def _recognise(path):
    from codebreaker.formats import xex
    return xex.is_xex(path) or (lane_of(path) == "pe" and pe_machine(path) == 0x01F2)


def _open(path):
    from codebreaker.formats import xex
    return xex.basefile(path) if xex.is_xex(path) else path


PLATFORM = Platform(
    name="powerpc", title="Xbox 360 (PowerPC)", family="PowerPC", container="XEX2 -> PE",
    cpu="PowerPC:BE:64:Xenon:homed", source="c", machine=0x01F2,
    lane="codebreaker.verify.lanes.powerpc", runner=("codebreaker.emulate.powerpc", "run"),
    recognise=_recognise, open=_open)
