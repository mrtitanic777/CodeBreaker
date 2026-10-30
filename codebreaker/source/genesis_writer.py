"""genesis_writer.py -- a Genesis / Mega Drive program as assembly source that builds back into it.

    python -m codebreaker.source.genesis_writer <game.md> [<out folder>]
    from codebreaker.source import genesis_writer
    r = genesis_writer.reconstruct(path, folder)
    r["source"]                     game.asm (Motorola syntax: asm68k, vasm)
    r["same"]                       the source, built, is the cartridge image byte for byte
    r["functions"]                  {address: Function(name)}

Genesis programs were written in 68000 assembly (most of them; some in C, compiled), and
assembly is what comes back. Code is what the program's own flow reaches from the reset vector
and the exception and interrupt handlers the vector table names (formats/genesis.py); every
byte it reaches is written as an instruction, every other byte as data. The vector table is
written as `dc.l` lines naming the handlers, the header at $100 as data with its fields named.
Functions are named for their vector (`reset`, `vblank`, `hblank`, ...) or `sub_<address>`, jump
targets `L_<address>`, and absolute and PC-relative operands naming the start of a line get that
line's label -- so the source can be moved and edited, not only rebuilt. The console's ports
(VDP, I/O, Z80 bus, TMSS) are written by name.

It builds with CodeBreaker's own assembler (build/m68k_asm.py), or with asm68k / vasm.
"""
import os
import sys

from codebreaker.decompile import m68k
from codebreaker.formats import genesis
from codebreaker.source import image_writer as w
from codebreaker.source.image_writer import data_lines

# the console's registers, by the names the hardware documentation gives them
HARDWARE = {0xC00000: "VDP_DATA", 0xC00004: "VDP_CTRL", 0xC00008: "VDP_HVCOUNTER", 0xC00011: "PSG",
            0xA00000: "Z80_RAM", 0xA04000: "YM2612", 0xA10001: "IO_VERSION", 0xA10003: "IO_DATA1",
            0xA10005: "IO_DATA2", 0xA10007: "IO_DATA3", 0xA10009: "IO_CTRL1", 0xA1000B: "IO_CTRL2",
            0xA1000D: "IO_CTRL3", 0xA11100: "Z80_BUSREQ", 0xA11200: "Z80_RESET", 0xA14000: "TMSS",
            0xA130F1: "SRAM_CTRL"}
_HEADER = [(0x100, 16, "system"), (0x110, 16, "copyright"), (0x120, 48, "domestic name"), (0x150, 48, "overseas name"),
           (0x180, 14, "serial and version"), (0x18E, 2, "checksum"), (0x190, 16, "devices"),
           (0x1A0, 8, "ROM start and end"), (0x1A8, 8, "RAM start and end"), (0x1B0, 12, "backup RAM"),
           (0x1BC, 12, "modem"), (0x1C8, 40, "notes"), (0x1F0, 16, "regions")]


_HEADER_AT = {at: (n, what) for at, n, what in _HEADER}


def _length(insn):
    return insn.length


def _walk(rom):
    mem = rom.data
    limit = len(mem)
    return w.walk(rom.functions(), lambda pc: m68k.decode(mem, pc, pc),
                  inside=lambda pc: 0x200 <= pc < limit,
                  fits=lambda pc, insn: pc + insn.length <= limit,
                  follows=lambda insn: 0x200 <= insn.target < limit and insn.target % 2 == 0 and insn.flow
                  and insn.mnemonic != "TRAP",
                  stops=lambda insn: insn.flow in ("jump", "ret", "indirect", "halt"), size=_length)


def _refs(insn):
    """Addresses an instruction's operands name (absolute, PC-relative, long immediates)."""
    for o in insn.operands:
        if o.kind in ("absw", "absl", "pcdisp", "pcindex"):
            yield o.value & 0xFFFFFFFF, o.kind
        elif o.kind == "imm" and o.size == "L":
            yield o.value, "imm"


def reconstruct(path, folder):
    rom = genesis.read(path)
    os.makedirs(folder, exist_ok=True)
    stem = os.path.splitext(os.path.basename(path))[0]
    code, targets, starts = _walk(rom)
    # a handler is named for its vector when one vector names it (or it is the reset, vblank or hblank
    # handler); one many vectors share is the program's catch-all
    sharing = {}
    for name, at in rom.vectors.items():
        sharing.setdefault(at, []).append(name)
    names = {}
    for at, vecs in sharing.items():
        if at not in starts or "ssp" in vecs and len(vecs) == 1:
            continue
        main = [v for v in vecs if v in ("reset", "vblank", "hblank", "external")]
        names[at] = "exception" if len(vecs) > 2 else main[0] if main else vecs[0]
    functions = w.functions(starts, names, "sub_%06X")
    covered = w.covered(code, _length)
    limit = len(rom.data)

    def line_start(a):
        return 0x200 <= a < limit and a not in covered and a % 2 == 0
    labels = w.labels(targets, names, line_start, "L_%06X")
    used = set()
    for insn in code.values():
        for v, kind in _refs(insn):
            if v in HARDWARE:
                used.add(v)
            elif line_start(v) and (kind != "imm" or v in code or v >= 0x400):
                labels.setdefault(v, names.get(v, ("L_%06X" if v in code else "D_%06X") % v))
    hardware = {a: HARDWARE[a] for a in sorted(used)}
    h = rom.header
    out = w.lines(["; %s -- written by CodeBreaker from %s" % (stem + ".asm", os.path.basename(path)),
                   '; "%s" / "%s", %s, %d KB ROM%s' % (h["domestic name"], h["overseas name"], h["serial"],
                                                      len(rom.data) // 1024, ", from an .smd dump" if rom.smd else ""),
                   ""], "asm68k", os.path.basename(path))
    for a, n in hardware.items():
        out.append("%-16s equ $%06X" % (n, a))
    if hardware:
        out.append("")
    text_names = {**labels, **hardware}
    # the vector table: 64 longs
    out.part("vectors", "vector table", length=0x100)
    out.append("        org $0")
    for i in range(64):
        v = int.from_bytes(rom.data[i * 4:i * 4 + 4], "big")
        name = labels.get(v) if i else None
        what = {0: "initial stack pointer", 1: "reset"}.get(i) or genesis.VECTOR_NAMES.get(i, "")
        out.append("        dc.l %s%s" % (name or "$%08X" % v, ("  ; " + what) if what else ""))

    def lines(data):
        return data_lines(data, "dc.b", "dcb.b %d,$%02X")

    def header(offset, addr, flush):
        if offset not in _HEADER_AT:
            return 0
        flush()
        n, what = _HEADER_AT[offset]
        field = rom.data[offset:offset + n]
        if all(32 <= b < 127 and b != 0x22 for b in field):
            text = ['        dc.b "%s"' % field.decode("ascii")]          # a text field as its text
        else:
            text = lines(list(field))
        out.extend(w.commented(text, what))
        return n
    w.emit(out, 0x100, limit, rom.data.__getitem__, lambda offset: offset, lambda offset, addr: code.get(offset),
           labels, functions, lambda insn: m68k.text(insn, text_names), lines, special=header, size=_length)
    source = w.text(out) + "\n"
    r = w.result(folder, stem + ".asm", source, functions, code, _length, **{"rom bytes": len(rom.data)})
    from codebreaker.build import m68k_asm
    w.assembled(r, lambda: m68k_asm.build(source), rom.data, os.path.join(folder, stem + ".built.md"))
    return r


def main(argv):
    return w.main(argv, reconstruct, __doc__)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
