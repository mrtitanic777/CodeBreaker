"""msx_writer.py -- an MSX cartridge as assembly source that builds back into it.

    python -m codebreaker.source.msx_writer <game.rom> [<out folder>]
    from codebreaker.source import msx_writer
    r = msx_writer.reconstruct(path, folder)    # as source/z80_writer.py's write()

The cartridge (formats/msx.py) is written through source/z80_writer.py from its header's entry
points, with the MSX's names: the VDP, PSG and PPI ports and the BIOS's documented entry points
and system variables, as the MSX2 Technical Handbook names them.
"""
import os
import sys

from codebreaker.formats import msx
from codebreaker.source import image_writer, z80_writer

PORTS = {0x98: "VDP_DATA", 0x99: "VDP_CTRL", 0xA0: "PSG_ADDR", 0xA1: "PSG_WRITE", 0xA2: "PSG_READ",
         0xA8: "PPI_A", 0xA9: "PPI_B", 0xAA: "PPI_C", 0xAB: "PPI_CTRL"}
NAMES = {0x000C: "RDSLT", 0x0014: "WRSLT", 0x001C: "CALSLT", 0x0024: "ENASLT", 0x0030: "CALLF",
         0x0041: "DISSCR", 0x0044: "ENASCR", 0x0047: "WRTVDP", 0x004A: "RDVRM", 0x004D: "WRTVRM",
         0x0050: "SETRD", 0x0053: "SETWRT", 0x0056: "FILVRM", 0x0059: "LDIRMV", 0x005C: "LDIRVM",
         0x005F: "CHGMOD", 0x0062: "CHGCLR", 0x0069: "CLRSPR", 0x006C: "INITXT", 0x006F: "INIT32",
         0x0072: "INIGRP", 0x0075: "INIMLT", 0x0090: "GICINI", 0x0093: "WRTPSG", 0x0096: "RDPSG",
         0x009C: "CHSNS", 0x009F: "CHGET", 0x00A2: "CHPUT", 0x00C3: "CLS", 0x00D5: "GTSTCK", 0x00D8: "GTTRIG",
         0x0138: "RSLREG", 0x013B: "WSLREG", 0xF3AE: "LINL40", 0xF3DF: "RG0SAV", 0xFC9E: "JIFFY",
         0xFCC1: "EXPTBL", 0xFD9F: "H_TIMI"}


def reconstruct(path, folder):
    p = msx.read(path)
    stem = os.path.splitext(os.path.basename(path))[0]
    h = p.header
    header = ["MSX cartridge %s; %d KB at $%04X; INIT $%04X STATEMENT $%04X DEVICE $%04X TEXT $%04X" % (
        os.path.basename(path), len(p.image) // 1024, p.origin, h["init"], h["statement"], h["device"], h["text"])]
    return z80_writer.write(p.image, p.origin, p.entries, folder, stem, PORTS, NAMES, header)


def main(argv):
    return image_writer.main(argv, reconstruct, __doc__, "bytes", "bytes")


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
