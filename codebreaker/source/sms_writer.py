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

# the console's I/O ports and the mapper's registers, by the names the hardware documentation gives them
PORTS = {0x3E: "MEMORY_CONTROL", 0x3F: "IO_CONTROL", 0x7E: "VCOUNTER", 0x7F: "PSG",
         0xBE: "VDP_DATA", 0xBF: "VDP_CONTROL", 0xDC: "JOYPAD_1", 0xDD: "JOYPAD_2"}
GG_PORTS = {0x00: "GG_START", 0x06: "GG_STEREO"}
REGISTERS = {0xFFFC: "MAPPER_CONTROL", 0xFFFD: "MAPPER_SLOT0", 0xFFFE: "MAPPER_SLOT1", 0xFFFF: "MAPPER_SLOT2"}


class Function(object):
    def __init__(self, name):
        self.name = name


def data_lines(data, db=".DB", fill=".DSB %d,$%02X"):
    """Bytes as source lines: rows of 16, and a run of 16 or more of one value as one fill line."""
    out, i = [], 0
    while i < len(data):
        run = 1
        while i + run < len(data) and data[i + run] == data[i]:
            run += 1
        if run >= 16:
            out.append("        " + fill % (run, data[i]))
            i += run
            continue
        row = []
        while i < len(data) and len(row) < 16:
            same = 1
            while i + same < len(data) and data[i + same] == data[i]:
                same += 1
            if same >= 16:
                break
            row.append(data[i])
            i += 1
        out.append("        %s %s" % (db, ",".join(_items(row))))
    return out


def _printable(b):
    return 32 <= b < 127 and b not in (0x22, 0x5C)         # (no quote or backslash: every assembler reads it alike)


def _items(row):
    """A row's bytes as items: runs of 4 or more printable characters as one string, the rest as $XX."""
    items, i = [], 0
    while i < len(row):
        j = i
        while j < len(row) and _printable(row[j]):
            j += 1
        if j - i >= 4:
            items.append('"%s"' % bytes(row[i:j]).decode("ascii"))
            i = j
        else:
            items.append("$%02X" % row[i])
            i += 1
    return items


def _walk(rom):
    """{ROM offset: Insn} for what flow reaches in the power-on mapping, its targets, the starts."""
    mem = rom.memory()
    limit = min(len(rom.data), 0xC000)
    starts = rom.functions()
    code, targets, todo, seen = {}, set(), list(starts), set()
    while todo:
        pc = todo.pop()
        while 0 <= pc < limit and pc not in seen:
            seen.add(pc)
            insn = z80.decode(mem, pc, pc)
            if insn is None or pc + insn.size > limit:
                break
            code[pc] = insn
            if insn.target is not None and insn.target < limit and insn.flow:
                targets.add(insn.target)
                todo.append(insn.target)
            if insn.flow in ("jump", "ret", "indirect"):     # (HALT resumes after an interrupt)
                break
            pc += insn.size
    return code, targets, starts


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
    functions = {}
    for a in starts:
        names.setdefault(a, "sub_%04X" % a)
        functions[a] = Function(names[a])
    covered = set()
    for off, insn in code.items():
        covered.update(range(off + 1, off + insn.size))
    limit = min(len(rom.data), 0xC000)
    line_at = {a for a in range(limit) if a not in covered}
    labels = {}
    for a in sorted(targets):
        if a in line_at:
            labels[a] = names.get(a, "L_%04X" % a)
    for a, n in names.items():
        if a in line_at:
            labels[a] = n
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
    ports = dict(PORTS, **(GG_PORTS if rom.system == "Game Gear" else {}))
    used_ports, used_regs = set(), set()
    for insn in code.values():
        if insn.pattern in ("OUT (n),A", "IN A,(n)") and insn.values[0] in ports:
            used_ports.add(insn.values[0])
        if "(nn)" in insn.pattern and insn.values and insn.values[-1] in REGISTERS:
            used_regs.add(insn.values[-1])
    constants = {**{a: REGISTERS[a] for a in used_regs}}
    port_names = {a: ports[a] for a in used_ports}
    banks = len(rom.pages)
    out = ["; %s -- written by CodeBreaker from %s" % (stem + ".asm", os.path.basename(path)),
           "; %s, %d KB ROM%s" % (rom.system, len(rom.data) // 1024,
                                  ", header product %s version %d (%s)" % (rom.header["product"], rom.header["version"],
                                                                           rom.header["region"]) if rom.header else ""),
           "",
           ".MEMORYMAP", "SLOTSIZE $4000", "DEFAULTSLOT 2", "SLOT 0 $0000", "SLOT 1 $4000", "SLOT 2 $8000",
           ".ENDME", "",
           ".ROMBANKMAP", "BANKSTOTAL %d" % banks, "BANKSIZE $4000", "BANKS %d" % banks, ".ENDRO", ""]
    for a in sorted(port_names):
        out.append(".DEFINE %-16s $%02X" % (port_names[a], a))
    for a in sorted(constants):
        out.append(".DEFINE %-16s $%04X" % (constants[a], a))
    if port_names or constants:
        out.append("")
    text_names = {**labels, **constants}
    for page in range(banks):
        base = [0x0000, 0x4000, 0x8000][_slot(page)]
        out += [".BANK %d SLOT %d" % (page, _slot(page)), ".ORG $0000"]
        data = []

        def flush():
            out.extend(data_lines(data))
            del data[:]
        offset = page * sms.PAGE
        end = offset + len(rom.pages[page])
        mapped = page <= 2 and offset < limit
        while offset < end:
            addr = base + offset - page * sms.PAGE
            if mapped and addr in labels:
                flush()
                if addr in functions:
                    out.append("")
                out.append("%s:" % labels[addr])
            insn = code.get(addr) if mapped else None
            if insn is not None and offset + insn.size <= end:
                flush()
                t = z80.text(insn, text_names)
                if insn.pattern in ("OUT (n),A", "IN A,(n)") and insn.values[0] in port_names:
                    t = t.replace("($%02X)" % insn.values[0], "(%s)" % port_names[insn.values[0]])
                out.append("        " + t)
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
    from codebreaker.build import z80_asm
    try:
        built = z80_asm.build(source, folder)
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
