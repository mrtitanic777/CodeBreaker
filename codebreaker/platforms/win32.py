"""32-bit Windows: PE images for the 386 and later (machine type 0x14C)."""
from codebreaker.platforms import Platform, lane_of, pe_machine

PLATFORM = Platform(
    name="pe", title="32-bit Windows", family="x86", container="PE",
    cpu="x86:LE:32:default:windows", source="c", machine=0x014C,
    lane="codebreaker.verify.lanes.win32", runner=("codebreaker.emulate.emulator", "trace_function"),
    recognise=lambda path: lane_of(path) == "pe" and pe_machine(path) != 0x01F2)
