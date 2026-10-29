"""zx.py -- what is particular to reconstructing a ZX Spectrum program: its 16 KB ROM at $0000. Its
reader (formats/zx.py) and writer (source/zx_writer.py) are its platform's (platforms/zx.py). The
lane itself is verify/lanes/z80_flat.py's."""
from codebreaker.verify.lanes import z80_flat

z80_flat.lane("zx", 2, (0x0000, 0x4000)).export(globals())
