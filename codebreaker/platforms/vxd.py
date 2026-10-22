"""Windows 95 device drivers: LE images that run in ring 0 (VxDs), written in assembly."""
from codebreaker.platforms import Platform, lane_of

PLATFORM = Platform(
    name="le", title="device drivers", family="x86", container="LE",
    cpu="x86:LE:32:default:windows", source="asm",
    lane="codebreaker.verify.driver", runner=("codebreaker.emulate.driver", "run"),
    recognise=lambda path: lane_of(path) == "le")
