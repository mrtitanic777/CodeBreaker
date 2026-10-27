"""msx.py -- what is particular to reconstructing an MSX cartridge: its reader (formats/msx.py), its
writer (source/msx_writer.py) and the BIOS ROM at $0000. The lane itself is verify/z80_flat.py's."""
from codebreaker.formats import msx
from codebreaker.source import msx_writer
from codebreaker.verify import z80_flat

z80_flat.lane("msx", 2, msx.read, msx_writer, (0x0000, 0x4000)).export(globals())
