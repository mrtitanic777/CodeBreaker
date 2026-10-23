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
from codebreaker.source.sms_writer import data_lines

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


class Function(object):
    def __init__(self, name):
        self.name = name


def _walk(rom):
    mem = rom.memory()
    limit = min(len(rom.data), 0x8000)
    starts = rom.functions()
    code, targets, todo, seen = {}, set(), list(starts), set()
    while todo:
        pc = todo.pop()
        while 0 <= pc < limit and pc not in seen and not 0x104 <= pc < 0x150:
            seen.add(pc)
            insn = sm83.decode(mem, pc, pc)
            if insn is None or pc + insn.size > limit or (pc < 0x104 < pc + insn.size):
                break
            code[pc] = insn
            if insn.target is not None and insn.target < limit and insn.flow:
                targets.add(insn.target)
                todo.append(insn.target)
            if insn.flow in ("jump", "ret", "indirect") or insn.pattern == "STOP":
                break
            pc += insn.size
    return code, targets, starts


def reconstruct(path, folder):
    rom = gb.read(path)
    os.makedirs(folder, exist_ok=True)
    stem = os.path.splitext(os.path.basename(path))[0]
    code, targets, starts = _walk(rom)
    names = {}
    for name, at in rom.vectors.items():
        if at in starts:
            names[at] = name
    functions = {}
    for a in starts:
        names.setdefault(a, "sub_%04X" % a)
        functions[a] = Function(names[a])
    covered = set()
    for off, insn in code.items():
        covered.update(range(off + 1, off + insn.size))
    limit = min(len(rom.data), 0x8000)
    line_at = {a for a in range(limit) if a not in covered}
    labels = {}
    for a in sorted(targets):
        if a in line_at:
            labels[a] = names.get(a, "L_%04X" % a)
    for a, n in names.items():
        if a in line_at:
            labels[a] = n
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
    for bank in range(len(rom.banks)):
        base = 0 if bank == 0 else gb.BANK
        out.append('SECTION "bank%d", %s' % (bank, "ROM0[$0000]" if bank == 0 else "ROMX[$4000], BANK[%d]" % bank))
        data = []

        def flush():
            out.extend(data_lines(data, "db", "ds %d, $%02X"))
            del data[:]
        offset, end = bank * gb.BANK, bank * gb.BANK + len(rom.banks[bank])
        mapped = bank <= 1
        while offset < end:
            addr = base + offset - bank * gb.BANK
            if bank == 0 and addr in dict((h[0], 1) for h in _HEADER):
                flush()
                at, size, what = next(h for h in _HEADER if h[0] == addr)
                out.extend(["%s  ; %s" % (line, what) if i == 0 else line
                            for i, line in enumerate(data_lines(list(rom.data[at:at + size]), "db", "ds %d, $%02X"))])
                offset += size
                continue
            if mapped and addr in labels:
                flush()
                if addr in functions:
                    out.append("")
                out.append("%s:" % labels[addr])
            insn = code.get(addr) if mapped else None
            if insn is not None and offset + insn.size <= end:
                flush()
                out.append("        " + sm83.text(insn, names_for_text))
                offset += insn.size
            else:
                data.append(rom.data[offset])
                if mapped and (addr + 1) in labels:
                    flush()
                offset += 1
        flush()
        out.append("")
    source = "\n".join(out) + "\n"
    src_path = os.path.join(folder, stem + ".asm")
    with open(src_path, "w", newline="\n") as f:
        f.write(source)
    r = {"source": src_path, "functions": functions, "notes": [], "linked": False, "same": False,
         "code bytes": sum(i.size for i in code.values()), "rom bytes": len(rom.data)}
    from codebreaker.build import sm83_asm
    try:
        built = sm83_asm.build(source, len(rom.data), folder)
    except ValueError as e:
        r["notes"].append("does not assemble: %s" % e)
        return r
    r["linked"] = True
    built_path = os.path.join(folder, stem + ".built" + os.path.splitext(path)[1])
    with open(built_path, "wb") as f:
        f.write(built)
    r["built"] = built_path
    r["same"] = built == rom.data
    if not r["same"]:
        first = next((i for i in range(min(len(built), len(rom.data))) if built[i] != rom.data[i]),
                     min(len(built), len(rom.data)))
        r["notes"].append("the built image differs from the original at byte %#x" % first)
    return r


def main(argv):
    if not argv:
        print(__doc__.split("\n\n")[0])
        return 2
    folder = argv[1] if len(argv) > 1 else os.path.splitext(argv[0])[0] + "_source"
    r = reconstruct(argv[0], folder)
    print("%s: %d functions, %d of %d ROM bytes reached as code -> %s" % (
        os.path.basename(argv[0]), len(r["functions"]), r["code bytes"], r["rom bytes"], r["source"]))
    print("  rebuilt: %s" % ("identical to the original" if r["same"] else "; ".join(r["notes"]) or "differs"))
    return 0 if r["same"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
