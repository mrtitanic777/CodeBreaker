"""msx.py -- what is particular to reconstructing an MSX cartridge: the BIOS ROM at $0000. Its reader
(formats/msx.py) and writer (source/msx_writer.py) are its platform's (platforms/msx.py). The lane
itself is verify/lanes/z80_flat.py's."""
from codebreaker.verify.lanes import z80_flat

z80_flat.lane("msx", 2, (0x0000, 0x4000)).export(globals())
