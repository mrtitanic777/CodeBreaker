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
from codebreaker.source import image_writer as w

# the console's registers, by the names the hardware documentation gives them
HARDWARE = {0x2000: "PPUCTRL", 0x2001: "PPUMASK", 0x2002: "PPUSTATUS", 0x2003: "OAMADDR", 0x2004: "OAMDATA",
            0x2005: "PPUSCROLL", 0x2006: "PPUADDR", 0x2007: "PPUDATA",
            0x4000: "SQ1_VOL", 0x4001: "SQ1_SWEEP", 0x4002: "SQ1_LO", 0x4003: "SQ1_HI",
            0x4004: "SQ2_VOL", 0x4005: "SQ2_SWEEP", 0x4006: "SQ2_LO", 0x4007: "SQ2_HI",
            0x4008: "TRI_LINEAR", 0x400A: "TRI_LO", 0x400B: "TRI_HI", 0x400C: "NOISE_VOL",
            0x400E: "NOISE_LO", 0x400F: "NOISE_HI", 0x4010: "DMC_FREQ", 0x4011: "DMC_RAW",
            0x4012: "DMC_START", 0x4013: "DMC_LEN", 0x4014: "OAMDMA", 0x4015: "SND_CHN",
            0x4016: "JOY1", 0x4017: "JOY2"}


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
    return w.walk(rom.functions(), lambda pc: m6502.decode(mem, pc, pc),
                  inside=lambda pc: 0x8000 <= pc <= 0xFFFF,
                  fits=lambda pc, insn: rom.mapped(pc) is not None and
                  rom.mapped(pc + insn.size - 1) == rom.mapped(pc) + insn.size - 1,
                  follows=lambda insn: insn.target >= 0x8000,
                  stops=lambda insn: insn.mnemonic in m6502.ENDS,
                  key=rom.mapped)


def _lines(data):
    return ["        .byte " + ", ".join("$%02X" % b for b in data[i:i + 16]) for i in range(0, len(data), 16)]


def reconstruct(path, folder):
    rom = nes.read(path)
    os.makedirs(folder, exist_ok=True)
    stem = os.path.splitext(os.path.basename(path))[0]
    code, targets, starts = _walk(rom)
    banks = _banks(rom)
    names = {rom.vectors["reset"]: "reset"}
    names.setdefault(rom.vectors["nmi"], "nmi")
    names.setdefault(rom.vectors["irq"], "irq")
    functions = w.functions(starts, names, "sub_%04X")
    covered = w.covered(code)                    # PRG offsets inside an instruction
    # a label can only stand where a line starts, in the bank shown at that address
    line_at = {}
    for seg, base, off0, size in banks:
        for off in range(off0, off0 + size):
            if off not in covered:
                line_at.setdefault(base + off - off0, off)
    labels = w.labels(targets, names, line_at.__contains__, "L_%04X")
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
           ""] + ["%-10s = $%04X" % (hardware[a], a) for a in used] + ([""] if used else []) + [
           '.segment "HEADER"', "        .byte " + ", ".join("$%02X" % b for b in rom.header[:16]), ""]
    if rom.trainer:
        out += ['.segment "TRAINER"', "        .byte " + ", ".join("$%02X" % b for b in rom.trainer), ""]
    for seg, base, off0, size in banks:
        out.append('.segment "%s"' % seg)
        end = off0 + size
        vectors_here = base + size == 0x10000

        def vectors(off, addr, flush, vectors_here=vectors_here):
            if not (vectors_here and addr == 0xFFFA):
                return 0
            flush()
            vec = [rom.vectors[k] for k in ("nmi", "reset", "irq")]
            out.append("        .word " + ", ".join(labels.get(v, "$%04X" % v) for v in vec))
            return 6

        def code_at(off, addr, end=end, vectors_here=vectors_here):
            insn = code.get(off)
            if insn is None or off + insn.size > end or (vectors_here and addr + insn.size > 0xFFFA):
                return None
            return insn
        w.emit(out, off0, end, rom.prg.__getitem__, lambda off, base=base, off0=off0: base + off - off0, code_at,
               labels, functions, lambda insn: m6502.text(insn, {**hardware, **labels}), _lines, special=vectors)
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
    cfg_path = os.path.join(folder, stem + ".cfg")
    with open(cfg_path, "w", newline="\n") as f:
        f.write(cfg_text)
    r = w.result(folder, stem + ".s", source, functions, code, config=cfg_path, **{"prg bytes": len(rom.prg)})
    from codebreaker.build import m6502_asm
    w.assembled(r, lambda: m6502_asm.build(source, cfg_text, folder), rom.header + rom.trainer + rom.prg + rom.chr,
                os.path.join(folder, stem + ".built.nes"))
    return r


def main(argv):
    return w.main(argv, reconstruct, __doc__, "PRG bytes", "prg bytes")


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
