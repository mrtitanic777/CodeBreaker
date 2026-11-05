"""zx.py -- ZX Spectrum programs: snapshots (`.sna`, `.z80`) and tapes (`.tap`).

    from codebreaker.formats import zx
    p = zx.read(path)
    p.origin, p.image           the program's block of memory and the address it starts at
    p.entries                   {address: name}: where it starts (a snapshot's PC, a tape's USR calls,
                                an IM 2 interrupt handler)
    p.registers                 a snapshot's registers (a f b c d e h l ix iy sp i im), else {}
    p.kind                      "snapshot" / "tape"

A snapshot is the machine stopped: the registers, and RAM from $4000 (16 KB of it the screen and
the system variables, the program above). .sna (48K) is a 27-byte header and the 48 KB, with the
program counter on the stack; .z80 holds the registers and the memory, compressed (ED ED n b runs)
in its first version and in 16 KB pages from the second on (48K: page 8 at $4000, 4 at $8000, 5 at
$C000; 128K: pages 3-10 are banks 0-7: bank 5 at $4000, 2 at $8000, the one paged in at $C000).
The program block taken from a snapshot is RAM from $5B00 (after the screen and the printer
buffer) to its end.

A tape is a list of blocks: a 17-byte header (type: 0 a BASIC program, 3 code; name; length; the
load address or the BASIC autostart line) then the data. The program is the code block; its
entry is the address BASIC's USR calls (`RANDOMIZE USR 32768`), read from the tokenised BASIC.
"""
import os
import struct

from codebreaker.formats import native

EXTENSIONS = (".sna", ".z80", ".tap")


class Program(object):
    pass


def is_zx(path):
    if not path.lower().endswith(EXTENSIONS):
        return False
    try:
        return read(path) is not None
    except (OSError, ValueError, struct.error, IndexError):
        return False


def _program(answer, _data, _path):
    p = Program()
    answer["memory"] = bytearray(answer["memory"])
    p.__dict__.update(answer)
    return p


@native.answered("zx.read", args_of=lambda path: (None, (os.fspath(path),)), result=_program)
def read(path):
    """The Program of a .sna, .z80 or .tap file (formats.dll: native/formats/zx)."""
