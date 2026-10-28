"""Real-mode DOS programs: MZ executables, and .COM files and device drivers, which have no
header (formats/dos.py tells them apart). Their source is assembly, rebuilt and compared."""
from codebreaker.platforms import Platform


def _recognise(path):
    from codebreaker.formats import detect
    return detect.kind(path) == "dos"   # (.COM files and plain driver images too, which have no header)


PLATFORM = Platform(
    name="dos", title="DOS programs", family="x86", container="MZ, COM, SYS",
    cpu="x86:LE:16:Real Mode:default", source="asm",
    lane="codebreaker.verify.lanes.dos", recognise=_recognise)
