"""zx.py -- what is particular to reconstructing a ZX Spectrum program: its reader (formats/zx.py),
its writer (source/zx_writer.py) and its 16 KB ROM at $0000. The lane itself is verify/z80_flat.py's."""
from codebreaker.formats import zx
from codebreaker.source import zx_writer
from codebreaker.verify import z80_flat

z80_flat.lane("zx", 2, zx.read, zx_writer, (0x0000, 0x4000)).export(globals())
