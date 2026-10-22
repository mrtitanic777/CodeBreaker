"""16-bit Windows: NE modules (Windows 3.x and the 16-bit half of Windows 95). Their functions
come back as C or, where they were written in assembly, as assembly (analyze/origin.py)."""
from codebreaker.platforms import Platform, lane_of

PLATFORM = Platform(
    name="ne", title="16-bit Windows", family="x86", container="NE",
    cpu="x86:LE:16:Protected Mode:default", source="c or asm",
    lane="codebreaker.verify.win16", runner=("codebreaker.emulate.windows16", "probe"),
    recognise=lambda path: lane_of(path) == "ne")
