"""sms_writer.py -- a Master System / Game Gear program as assembly source that builds back into it.

    python -m codebreaker.source.sms_writer <game.sms> [<out folder>]
    from codebreaker.source import sms_writer
    r = sms_writer.reconstruct(path, folder)
    r["source"]                     game.asm (WLA-DX syntax)
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

These programs were written in assembly, so assembly is what comes back. Code is what the
program's own flow reaches from the reset, interrupt and NMI vectors in the pages mapped at
power-on (formats/sms.py); every byte it reaches is written as an instruction, every other byte
as data. Function starts are named for their vector (`reset`, `irq`, `nmi`) or `sub_<address>`,
jump targets `L_<address>`, and absolute words naming the start of a line get that line's label,
so the source can be moved and edited, not only rebuilt. The console's ports and the mapper's
registers are written by name.

It builds with CodeBreaker's own assembler (build/z80_asm.py) or with WLA-DX
(`wla-z80 -o game.o game.asm && wlalink linkfile game.sms`).
"""
import os
import sys

from codebreaker.decompile import z80
from codebreaker.formats import sms
from codebreaker.source import image_writer as w

# the console's I/O ports and the mapper's registers, by the names the hardware documentation gives them
PORTS = {0x3E: "MEMORY_CONTROL", 0x3F: "IO_CONTROL", 0x7E: "VCOUNTER", 0x7F: "PSG",
         0xBE: "VDP_DATA", 0xBF: "VDP_CONTROL", 0xDC: "JOYPAD_1", 0xDD: "JOYPAD_2"}
GG_PORTS = {0x00: "GG_START", 0x06: "GG_STEREO"}
REGISTERS = {0xFFFC: "MAPPER_CONTROL", 0xFFFD: "MAPPER_SLOT0", 0xFFFE: "MAPPER_SLOT1", 0xFFFF: "MAPPER_SLOT2"}


def _walk(rom):
    """{ROM offset: Insn} for what flow reaches in the power-on mapping, its targets, the starts."""
    mem = rom.memory()
    limit = min(len(rom.data), 0xC000)
    return w.walk(rom.functions(), lambda pc: z80.decode(mem, pc, pc),
                  inside=lambda pc: 0 <= pc < limit,
                  fits=lambda pc, insn: pc + insn.size <= limit,
                  follows=lambda insn: insn.target < limit and insn.flow,
                  stops=lambda insn: insn.flow in ("jump", "ret", "indirect"))     # (HALT resumes after an interrupt)


def _slot(page):
    return 0 if page == 0 else 1 if page == 1 else 2


def reconstruct(path, folder):
    rom = sms.read(path)
    os.makedirs(folder, exist_ok=True)
    stem = os.path.splitext(os.path.basename(path))[0]
    code, targets, starts = _walk(rom)
    names = {0x0000: "reset"}
    for name, at in (("irq", 0x38), ("nmi", 0x66)):
        if at in starts:
            names.setdefault(at, name)
    functions = w.functions(starts, names, "sub_%04X")
    covered = w.covered(code)
    limit = min(len(rom.data), 0xC000)
    line_at = {a for a in range(limit) if a not in covered}
    labels = w.labels(targets, names, line_at.__contains__, "L_%04X")
    # pointers into the program: a word loaded into a pointer register (HL DE IX IY, not BC, which mostly
    # holds counts), or read and written through (nn), that is the start of a line past the vectors
    pointer = ("LD HL,nn", "LD DE,nn", "LD IX,nn", "LD IY,nn", "LD SP,nn")
    for insn in code.values():
        v = insn.values[-1] if insn.values else None
        if v is None or v not in line_at or v < 0x0100:
            continue
        if insn.pattern in pointer or (insn.flow == "" and "(nn)" in insn.pattern):
            labels.setdefault(v, names.get(v, "L_%04X" % v) if v in code else "D_%04X" % v)
    # the hardware the code names
    ports = {**PORTS, **(GG_PORTS if rom.system == "Game Gear" else {})}
    used_ports, used_regs = set(), set()
    for insn in code.values():
        if insn.pattern in ("OUT (n),A", "IN A,(n)") and insn.values[0] in ports:
            used_ports.add(insn.values[0])
        if "(nn)" in insn.pattern and insn.values and insn.values[-1] in REGISTERS:
            used_regs.add(insn.values[-1])
    constants = {**{a: REGISTERS[a] for a in used_regs}}
    port_names = {a: ports[a] for a in used_ports}
    banks = len(rom.pages)
    out = w.lines(["; %s -- written by CodeBreaker from %s" % (stem + ".asm", os.path.basename(path)),
           "; %s, %d KB ROM%s" % (rom.system, len(rom.data) // 1024,
                                  ", header product %s version %d (%s)" % (rom.header["product"], rom.header["version"],
                                                                           rom.header["region"]) if rom.header else ""),
           "",
           ".MEMORYMAP", "SLOTSIZE $4000", "DEFAULTSLOT 2", "SLOT 0 $0000", "SLOT 1 $4000", "SLOT 2 $8000",
           ".ENDME", "",
           ".ROMBANKMAP", "BANKSTOTAL %d" % banks, "BANKSIZE $4000", "BANKS %d" % banks, ".ENDRO", ""],
                  "wla-z80", os.path.basename(path))
    for a in sorted(port_names):
        out.append(".DEFINE %-16s $%02X" % (port_names[a], a))
    for a in sorted(constants):
        out.append(".DEFINE %-16s $%04X" % (constants[a], a))
    if port_names or constants:
        out.append("")
    text_names = {**labels, **constants}

    def text(insn):
        t = z80.text(insn, text_names)
        if insn.pattern in ("OUT (n),A", "IN A,(n)") and insn.values[0] in port_names:
            t = t.replace("($%02X)" % insn.values[0], "(%s)" % port_names[insn.values[0]])
        return t
    for page in range(banks):
        base = [0x0000, 0x4000, 0x8000][_slot(page)]
        out.part("bank%d" % page, "bank", length=len(rom.pages[page]))
        out += [".BANK %d SLOT %d" % (page, _slot(page)), ".ORG $0000"]
        first = page * sms.PAGE
        end = first + len(rom.pages[page])
        mapped = page <= 2 and first < limit          # (the pages seen at power-on: where flow was followed)

        def code_at(offset, addr, end=end, mapped=mapped):
            insn = code.get(addr) if mapped else None
            return insn if insn is not None and offset + insn.size <= end else None
        w.emit(out, first, end, rom.data.__getitem__, lambda offset, base=base, first=first: base + offset - first,
               code_at, labels if mapped else {}, functions, text, w.data_lines)
        out.append("")
    source = w.text(out) + "\n"
    r = w.result(folder, stem + ".asm", source, functions, code, **{"rom bytes": len(rom.data)})
    from codebreaker.build import z80_asm
    w.assembled(r, lambda: z80_asm.build(source, folder), rom.data,
                os.path.join(folder, stem + ".built" + os.path.splitext(path)[1]))
    return r


def main(argv):
    return w.main(argv, reconstruct, __doc__)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
