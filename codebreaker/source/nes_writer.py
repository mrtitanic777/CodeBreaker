"""nes_writer.py -- a NES program as assembly source that builds back into it.

    python -m codebreaker.source.nes_writer <game.nes> [<out folder>]
    from codebreaker.source import nes_writer
    r = nes_writer.reconstruct(path, folder)
    r["source"], r["config"]        game.s (ca65 syntax) and game.cfg (ld65), beside the CHR as game.chr
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

NES programs were written in assembly almost without exception, so assembly is what comes back
(docs/PLATFORM_DESCRIPTOR.md). Code is what the program's own flow reaches from its three
vectors (formats/nes.py); every byte it reaches is written as an instruction, every other byte
as data. Function starts are named for their vector (`reset`, `nmi`, `irq`) or `sub_<address>`,
branch and jump targets `L_<address>`, and an absolute operand naming the start of a line gets
that line's label -- so the source can be moved and edited, not only rebuilt. The last six
bytes of the bank that holds them are the vectors, written as `.word nmi, reset, irq`.

It builds with CodeBreaker's own assembler (build/m6502_asm.py) or with the cc65 suite
(`ca65 game.s && ld65 -C game.cfg -o game.nes game.o`).
"""
import os
import sys

from codebreaker.decompile import m6502
from codebreaker.formats import nes


# the console's registers, by the names the hardware documentation gives them
HARDWARE = {0x2000: "PPUCTRL", 0x2001: "PPUMASK", 0x2002: "PPUSTATUS", 0x2003: "OAMADDR", 0x2004: "OAMDATA",
            0x2005: "PPUSCROLL", 0x2006: "PPUADDR", 0x2007: "PPUDATA",
            0x4000: "SQ1_VOL", 0x4001: "SQ1_SWEEP", 0x4002: "SQ1_LO", 0x4003: "SQ1_HI",
            0x4004: "SQ2_VOL", 0x4005: "SQ2_SWEEP", 0x4006: "SQ2_LO", 0x4007: "SQ2_HI",
            0x4008: "TRI_LINEAR", 0x400A: "TRI_LO", 0x400B: "TRI_HI", 0x400C: "NOISE_VOL",
            0x400E: "NOISE_LO", 0x400F: "NOISE_HI", 0x4010: "DMC_FREQ", 0x4011: "DMC_RAW",
            0x4012: "DMC_START", 0x4013: "DMC_LEN", 0x4014: "OAMDMA", 0x4015: "SND_CHN",
            0x4016: "JOY1", 0x4017: "JOY2"}


class Function(object):
    def __init__(self, name):
        self.name = name


def _banks(rom):
    """[(segment name, CPU address of its first byte, PRG offset, size)]"""
    if len(rom.prg) == 0x4000:
        return [("BANK0", 0xC000, 0, 0x4000)]
    if rom.mapper == 0 or len(rom.prg) <= 0x8000:
        return [("BANK0", 0x8000, 0, len(rom.prg))]
    out = []
    for i in range(len(rom.banks)):
        at = 0xC000 if i == len(rom.banks) - 1 else 0x8000
        out.append(("BANK%d" % i, at, i * 0x4000, 0x4000))
    return out


def _walk(rom):
    """{PRG offset of an instruction's first byte: Insn} for what flow reaches, and {target}"""
    mem = rom.memory()
    starts = rom.functions()
    code, targets, todo, seen = {}, set(), list(starts), set()
    while todo:
        pc = todo.pop()
        while 0x8000 <= pc <= 0xFFFF and pc not in seen:
            seen.add(pc)
            insn = m6502.decode(mem, pc, pc)
            off = rom.mapped(pc)
            if insn is None or off is None or rom.mapped(pc + insn.size - 1) != off + insn.size - 1:
                break
            code[off] = insn
            if insn.target is not None and insn.target >= 0x8000:
                targets.add(insn.target)
                todo.append(insn.target)
            if insn.mnemonic in m6502.ENDS:
                break
            pc += insn.size
    return code, targets, starts


def reconstruct(path, folder):
    rom = nes.read(path)
    os.makedirs(folder, exist_ok=True)
    stem = os.path.splitext(os.path.basename(path))[0]
    code, targets, starts = _walk(rom)
    banks = _banks(rom)
    names = {rom.vectors["reset"]: "reset"}
    names.setdefault(rom.vectors["nmi"], "nmi")
    names.setdefault(rom.vectors["irq"], "irq")
    functions = {}
    for a in starts:
        names.setdefault(a, "sub_%04X" % a)
        functions[a] = Function(names[a])
    covered = set()                              # PRG offsets inside an instruction
    for off, insn in code.items():
        covered.update(range(off + 1, off + insn.size))
    # a label can only stand where a line starts, in the bank shown at that address
    line_at = {}
    for seg, base, off0, size in banks:
        for off in range(off0, off0 + size):
            if off not in covered:
                line_at.setdefault(base + off - off0, off)
    labels = {}
    for a in sorted(targets):
        if a in line_at:
            labels[a] = names.get(a, "L_%04X" % a)
    for a, n in names.items():
        if a in line_at:
            labels[a] = n
    for insn in code.values():                    # absolute operands that name a line's start
        if insn.mode in ("abs", "abx", "aby", "ind") and insn.mnemonic not in ("JMP", "JSR") \
                and insn.operand in line_at and insn.operand >= 0x8000:
            labels.setdefault(insn.operand, "D_%04X" % insn.operand)
    used = sorted({i.operand for i in code.values() if i.mode in ("abs", "abx", "aby") and i.operand in HARDWARE})
    hardware = {a: HARDWARE[a] for a in used}
    out = ["; %s -- written by CodeBreaker from %s" % (stem + ".s", os.path.basename(path)),
           "; mapper %d, %d KB PRG, %d KB CHR, %s mirroring%s" % (
               rom.mapper, len(rom.prg) // 1024, len(rom.chr) // 1024, rom.mirroring,
               ", battery-backed RAM" if rom.battery else ""),
           ""] + ["%-10s = $%04X" % (hardware[a], a) for a in used] + (["" ] if used else []) + [
           '.segment "HEADER"', "        .byte " + ", ".join("$%02X" % b for b in rom.header[:16]), ""]
    if rom.trainer:
        out += ['.segment "TRAINER"', "        .byte " + ", ".join("$%02X" % b for b in rom.trainer), ""]
    for seg, base, off0, size in banks:
        out.append('.segment "%s"' % seg)
        off, end = off0, off0 + size
        vectors_here = base + size == 0x10000
        data = []

        def flush():
            for i in range(0, len(data), 16):
                out.append("        .byte " + ", ".join("$%02X" % b for b in data[i:i + 16]))
            del data[:]
        while off < end:
            addr = base + off - off0
            if vectors_here and addr == 0xFFFA:
                flush()
                vec = [rom.vectors[k] for k in ("nmi", "reset", "irq")]
                out.append("        .word " + ", ".join(labels.get(v, "$%04X" % v) for v in vec))
                off += 6
                continue
            if addr in labels:
                flush()
                if addr in functions:
                    out.append("")
                out.append("%s:" % labels[addr])
            insn = code.get(off)
            if insn is not None and off + insn.size <= end and not (vectors_here and addr + insn.size > 0xFFFA):
                flush()
                out.append("        " + m6502.text(insn, {**hardware, **labels}))
                off += insn.size
            else:
                data.append(rom.prg[off])
                if len(data) == 16 or (base + off + 1 - off0) in labels:
                    flush()
                off += 1
        flush()
        out.append("")
    if rom.chr:
        with open(os.path.join(folder, stem + ".chr"), "wb") as f:
            f.write(rom.chr)
        out += ['.segment "CHARS"', '        .incbin "%s.chr"' % stem, ""]
    source = "\n".join(out) + "\n"
    cfg = ["MEMORY {", "    HEADER: start = $0000, size = $0010, fill = yes, file = %O;"]
    if rom.trainer:
        cfg.append("    TRAINER: start = $7000, size = $0200, fill = yes, file = %O;")
    for seg, base, _off, size in banks:
        cfg.append("    PRG%s: start = $%04X, size = $%04X, fill = yes, file = %%O;" % (seg[4:], base, size))
    if rom.chr:
        cfg.append("    CHR: start = $0000, size = $%04X, fill = yes, file = %%O;" % len(rom.chr))
    cfg += ["}", "SEGMENTS {", "    HEADER: load = HEADER, type = ro;"]
    if rom.trainer:
        cfg.append("    TRAINER: load = TRAINER, type = ro;")
    for seg, _b, _o, _s in banks:
        cfg.append("    %s: load = PRG%s, type = ro;" % (seg, seg[4:]))
    if rom.chr:
        cfg.append("    CHARS: load = CHR, type = ro;")
    cfg.append("}")
    cfg_text = "\n".join(cfg) + "\n"
    src_path, cfg_path = os.path.join(folder, stem + ".s"), os.path.join(folder, stem + ".cfg")
    with open(src_path, "w", newline="\n") as f:
        f.write(source)
    with open(cfg_path, "w", newline="\n") as f:
        f.write(cfg_text)
    r = {"source": src_path, "config": cfg_path, "functions": functions, "notes": [], "linked": False,
         "same": False, "code bytes": sum(i.size for i in code.values()), "prg bytes": len(rom.prg)}
    from codebreaker.build import m6502_asm
    try:
        built = m6502_asm.build(source, cfg_text, folder)
    except ValueError as e:
        r["notes"].append("does not assemble: %s" % e)
        return r
    r["linked"] = True
    with open(os.path.join(folder, stem + ".built.nes"), "wb") as f:
        f.write(built)
    original = rom.header + rom.trainer + rom.prg + rom.chr
    r["built"] = os.path.join(folder, stem + ".built.nes")
    r["same"] = built == original
    if not r["same"]:
        first = next((i for i in range(min(len(built), len(original))) if built[i] != original[i]),
                     min(len(built), len(original)))
        r["notes"].append("the built image differs from the original at byte %#x" % first)
    return r


def main(argv):
    if not argv:
        print(__doc__.split("\n\n")[0])
        return 2
    folder = argv[1] if len(argv) > 1 else os.path.splitext(argv[0])[0] + "_source"
    r = reconstruct(argv[0], folder)
    print("%s: %d functions, %d of %d PRG bytes reached as code -> %s" % (
        os.path.basename(argv[0]), len(r["functions"]), r["code bytes"], r["prg bytes"], r["source"]))
    print("  rebuilt: %s" % ("identical to the original" if r["same"] else "; ".join(r["notes"]) or "differs"))
    return 0 if r["same"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
