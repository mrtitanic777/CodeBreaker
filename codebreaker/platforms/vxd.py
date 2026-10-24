"""Windows 95 device drivers: LE images that run in ring 0 (VxDs), written in assembly."""
from codebreaker.platforms import Platform, lane_of


def _recognise(path):
    """An LE image, or a W3 container of them (vmm32.vxd: VMM and the drivers loaded with it)."""
    if lane_of(path) == "le":
        return True
    with open(path, "rb") as f:
        head = f.read(0x40)
        if head[:2] != b"MZ" or len(head) < 0x40:
            return False
        f.seek(int.from_bytes(head[0x3C:0x40], "little"))
        return f.read(2) == b"W3"

PLATFORM = Platform(
    name="le", title="device drivers", family="x86", container="LE",
    cpu="x86:LE:32:default:windows", source="asm",
    lane="codebreaker.verify.driver", runner=("codebreaker.emulate.driver", "run"),
    recognise=_recognise)
