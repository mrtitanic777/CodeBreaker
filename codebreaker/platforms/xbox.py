"""The original Xbox: x86 code in an XBE. The program goes down the 32-bit Windows lane as the PE
image formats/xbox/xbe.py makes of it (the same bytes at the same addresses, the kernel's
functions as imports of xboxkrnl.exe), so it has no lane of its own: corpus.retail hands every
stage that image."""
from codebreaker.platforms import Platform


def _recognise(path):
    with open(path, "rb") as f:
        return f.read(4) == b"XBEH"


def _open(path):
    from codebreaker.formats.xbox import xbe
    return xbe.to_pe(path) if _recognise(path) else path


PLATFORM = Platform(
    name="xbe", title="original Xbox", family="x86", container="XBE -> PE",
    cpu="x86:LE:32:default:windows", source="c", lane=None, recognise=_recognise, open=_open)
