"""Windows 95 device drivers: LE images that run in ring 0 (VxDs), written in assembly."""
from codebreaker.platforms import Platform


def _recognise(path):
    """An LE image, or a W3 container of them (vmm32.vxd: VMM and the drivers loaded with it)."""
    from codebreaker.formats import detect
    return detect.holds_drivers(path, bundles=(b"W3",))

PLATFORM = Platform(
    name="le", title="device drivers", family="x86", container="LE",
    cpu="x86:LE:32:default:windows", source="asm",
    lane="codebreaker.verify.lanes.driver", runner=("codebreaker.emulate.driver", "run"),
    recognise=_recognise)
