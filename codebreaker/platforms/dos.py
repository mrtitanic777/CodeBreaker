"""Real-mode DOS programs: MZ executables, and .COM files and device drivers, which have no
header (formats/dos.py tells them apart). Their source is assembly, rebuilt and compared."""
from codebreaker.platforms import Platform, lane_of


def _recognise(path):
    lane = lane_of(path)
    if lane is None:                    # .COM files and plain driver images have no header
        from codebreaker.formats import dos
        return bool(dos.kind(path))
    return lane == "dos"


PLATFORM = Platform(
    name="dos", title="DOS programs", family="x86", container="MZ, COM, SYS",
    cpu="x86:LE:16:Real Mode:default", source="asm",
    lane="codebreaker.verify.dos", recognise=_recognise)
