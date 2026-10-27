"""gb_writer.py -- a Game Boy program as assembly source that builds back into it.

    python -m codebreaker.source.gb_writer <game.gb> [<out folder>]
    from codebreaker.source import gb_writer
    r = gb_writer.reconstruct(path, folder)
    r["source"]                     game.asm (RGBDS syntax)
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

Game Boy programs were written in assembly, so assembly is what comes back. Code is what the
program's own flow reaches from the entry at $0100 and the interrupt vectors in the two banks
mapped at power-on (formats/gb.py); every byte it reaches is written as an instruction, every
other byte as data. The cartridge header ($0104-$014F) is written as data with its fields named
in comments. Functions are named for their vector (`entry`, `vblank`, `stat`, `timer`, `serial`,
`joypad`) or `sub_<address>`, jump targets `L_<address>`; the I/O registers by the names
Pan Docs gives them (rLCDC, rSTAT, ...), as hardware.inc does.

It builds with CodeBreaker's own assembler (build/sm83_asm.py) or with RGBDS
(`rgbasm -o game.o game.asm && rgblink -o game.gb game.o`).
"""
import os
import sys

from codebreaker.decompile import sm83
from codebreaker.formats import gb
from codebreaker.source import image_writer as w
from codebreaker.source.image_writer import data_lines

# the I/O registers, as hardware.inc names them
REGISTERS = {0xFF00: "rP1", 0xFF01: "rSB", 0xFF02: "rSC", 0xFF04: "rDIV", 0xFF05: "rTIMA", 0xFF06: "rTMA",
             0xFF07: "rTAC", 0xFF0F: "rIF", 0xFF10: "rNR10", 0xFF11: "rNR11", 0xFF12: "rNR12", 0xFF13: "rNR13",
             0xFF14: "rNR14", 0xFF16: "rNR21", 0xFF17: "rNR22", 0xFF18: "rNR23", 0xFF19: "rNR24",
             0xFF1A: "rNR30", 0xFF1B: "rNR31", 0xFF1C: "rNR32", 0xFF1D: "rNR33", 0xFF1E: "rNR34",
             0xFF20: "rNR41", 0xFF21: "rNR42", 0xFF22: "rNR43", 0xFF23: "rNR44", 0xFF24: "rNR50",
             0xFF25: "rNR51", 0xFF26: "rNR52", 0xFF40: "rLCDC", 0xFF41: "rSTAT", 0xFF42: "rSCY", 0xFF43: "rSCX",
             0xFF44: "rLY", 0xFF45: "rLYC", 0xFF46: "rDMA", 0xFF47: "rBGP", 0xFF48: "rOBP0", 0xFF49: "rOBP1",
             0xFF4A: "rWY", 0xFF4B: "rWX", 0xFF4D: "rKEY1", 0xFF4F: "rVBK", 0xFF51: "rHDMA1", 0xFF52: "rHDMA2",
             0xFF53: "rHDMA3", 0xFF54: "rHDMA4", 0xFF55: "rHDMA5", 0xFF56: "rRP", 0xFF68: "rBCPS", 0xFF69: "rBCPD",
             0xFF6A: "rOCPS", 0xFF6B: "rOCPD", 0xFF70: "rSVBK", 0xFFFF: "rIE"}
# the bank controller's registers (writes into ROM space), as hardware.inc names them
MBC_REGISTERS = {0x0000: "rRAMG", 0x2000: "rROMB0", 0x3000: "rROMB1", 0x4000: "rRAMB"}
_HEADER = [(0x0104, 0x30, "Nintendo's logo (the boot ROM checks it)"), (0x0134, 0x0F, "title"),
           (0x0143, 1, "Color support"), (0x0144, 2, "licensee"), (0x0146, 1, "Super Game Boy support"),
           (0x0147, 1, "cartridge type"), (0x0148, 1, "ROM size"), (0x0149, 1, "RAM size"),
           (0x014A, 1, "destination"), (0x014B, 1, "old licensee"), (0x014C, 1, "version"),
           (0x014D, 1, "header checksum"), (0x014E, 2, "global checksum")]


_HEADER_AT = {at: (size, what) for at, size, what in _HEADER}


def _walk(rom):
    mem = rom.memory()
    limit = min(len(rom.data), 0x8000)
    return w.walk(rom.functions(), lambda pc: sm83.decode(mem, pc, pc),
                  inside=lambda pc: 0 <= pc < limit and not 0x104 <= pc < 0x150,      # (not the header)
                  fits=lambda pc, insn: pc + insn.size <= limit and not pc < 0x104 < pc + insn.size,
                  follows=lambda insn: insn.target < limit and insn.flow,
                  stops=lambda insn: insn.flow in ("jump", "ret", "indirect") or insn.pattern == "STOP")


def reconstruct(path, folder):
    rom = gb.read(path)
    os.makedirs(folder, exist_ok=True)
    stem = os.path.splitext(os.path.basename(path))[0]
    code, targets, starts = _walk(rom)
    names = {at: name for name, at in rom.vectors.items() if at in starts}
    functions = w.functions(starts, names, "sub_%04X")
    covered = w.covered(code)
    limit = min(len(rom.data), 0x8000)
    line_at = {a for a in range(limit) if a not in covered}
    labels = w.labels(targets, names, line_at.__contains__, "L_%04X")
    pointer = ("LD HL,nn", "LD DE,nn", "LD SP,nn")
    for insn in code.values():
        v = insn.values[-1] if insn.values else None
        if v is None or v not in line_at or v < 0x0150:
            continue
        if insn.pattern.startswith("LD (nn)"):
            continue                      # a write into ROM space goes to the bank controller, not to data
        if insn.pattern in pointer or (insn.flow == "" and "(nn)" in insn.pattern):
            labels.setdefault(v, names.get(v, "L_%04X" % v) if v in code else "D_%04X" % v)
    known = {**REGISTERS, **MBC_REGISTERS}
    used = sorted({i.values[0] for i in code.values() if i.pattern.startswith("LDH") and i.values[0] in known} |
                  {i.values[-1] for i in code.values() if "(nn)" in i.pattern and i.values and i.values[-1] in known})
    hardware = {a: known[a] for a in used}
    out = ["; %s -- written by CodeBreaker from %s" % (stem + ".asm", os.path.basename(path)),
           '; "%s", %s, %d KB ROM%s%s' % (rom.title, rom.mbc, len(rom.data) // 1024,
                                         ", Game Boy Color (%s)" % rom.cgb if rom.cgb else "",
                                         ", Super Game Boy" if rom.sgb else ""), ""]
    for a in used:
        out.append("DEF %-8s EQU $%04X" % (hardware[a], a))
    if used:
        out.append("")
    names_for_text = {**labels, **hardware}

    def lines(data):
        return data_lines(data, "db", "ds %d, $%02X")
    for bank in range(len(rom.banks)):
        base = 0 if bank == 0 else gb.BANK
        first = bank * gb.BANK
        end = first + len(rom.banks[bank])
        mapped = bank <= 1                   # (the banks seen at power-on: where flow was followed)
        out.append('SECTION "bank%d", %s' % (bank, "ROM0[$0000]" if bank == 0 else "ROMX[$4000], BANK[%d]" % bank))

        def header(offset, addr, flush, bank=bank):
            if bank != 0 or addr not in _HEADER_AT:
                return 0
            flush()
            size, what = _HEADER_AT[addr]
            out.extend(w.commented(lines(list(rom.data[addr:addr + size])), what))
            return size

        def code_at(offset, addr, end=end, mapped=mapped):
            insn = code.get(addr) if mapped else None
            return insn if insn is not None and offset + insn.size <= end else None
        w.emit(out, first, end, rom.data.__getitem__, lambda offset, base=base, first=first: base + offset - first,
               code_at, labels if mapped else {}, functions, lambda insn: sm83.text(insn, names_for_text), lines,
               special=header)
        out.append("")
    source = "\n".join(out) + "\n"
    r = w.result(folder, stem + ".asm", source, functions, code, **{"rom bytes": len(rom.data)})
    from codebreaker.build import sm83_asm
    w.assembled(r, lambda: sm83_asm.build(source, len(rom.data), folder), rom.data,
                os.path.join(folder, stem + ".built" + os.path.splitext(path)[1]))
    return r


def main(argv):
    return w.main(argv, reconstruct, __doc__)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
