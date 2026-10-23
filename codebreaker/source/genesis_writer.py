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
from codebreaker.source.sms_writer import data_lines

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


class Function(object):
    def __init__(self, name):
        self.name = name


def _walk(rom):
    mem = rom.data
    limit = len(mem)
    starts = rom.functions()
    code, targets, todo, seen = {}, set(), list(starts), set()
    while todo:
        pc = todo.pop()
        while 0x200 <= pc < limit and pc not in seen:
            seen.add(pc)
            insn = m68k.decode(mem, pc, pc)
            if insn is None or pc + insn.length > limit:
                break
            code[pc] = insn
            t = insn.target
            if t is not None and 0x200 <= t < limit and t % 2 == 0 and insn.flow and insn.mnemonic != "TRAP":
                targets.add(t)
                todo.append(t)
            if insn.flow in ("jump", "ret", "indirect", "halt"):
                break
            pc += insn.length
    return code, targets, starts


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
    functions = {}
    for a in starts:
        names.setdefault(a, "sub_%06X" % a)
        functions[a] = Function(names[a])
    covered = set()
    for off, insn in code.items():
        covered.update(range(off + 1, off + insn.length))
    limit = len(rom.data)
    labels = {}

    def line_start(a):
        return 0x200 <= a < limit and a not in covered and a % 2 == 0
    for a in sorted(targets):
        if line_start(a):
            labels[a] = names.get(a, "L_%06X" % a)
    for a, n in names.items():
        if line_start(a):
            labels[a] = n
    used = set()
    for insn in code.values():
        for v, kind in _refs(insn):
            if v in HARDWARE:
                used.add(v)
            elif line_start(v) and (kind != "imm" or v in code or v >= 0x400):
                labels.setdefault(v, names.get(v, ("L_%06X" if v in code else "D_%06X") % v))
    hardware = {a: HARDWARE[a] for a in sorted(used)}
    h = rom.header
    out = ["; %s -- written by CodeBreaker from %s" % (stem + ".asm", os.path.basename(path)),
           '; "%s" / "%s", %s, %d KB ROM%s' % (h["domestic name"], h["overseas name"], h["serial"],
                                              len(rom.data) // 1024, ", from an .smd dump" if rom.smd else ""), ""]
    for a, n in hardware.items():
        out.append("%-16s equ $%06X" % (n, a))
    if hardware:
        out.append("")
    text_names = {**labels, **hardware}
    # the vector table: 64 longs
    out.append("        org $0")
    for i in range(64):
        v = int.from_bytes(rom.data[i * 4:i * 4 + 4], "big")
        name = labels.get(v) if i else None
        what = {0: "initial stack pointer", 1: "reset"}.get(i) or genesis.VECTOR_NAMES.get(i, "")
        out.append("        dc.l %s%s" % (name or "$%08X" % v, ("  ; " + what) if what else ""))
    offset = 0x100
    header_at = {at: (n, what) for at, n, what in _HEADER}
    data = []

    def flush():
        out.extend(data_lines(data, "dc.b", "dcb.b %d,$%02X"))
        del data[:]
    while offset < limit:
        if offset in header_at:
            flush()
            n, what = header_at[offset]
            field = rom.data[offset:offset + n]
            if all(32 <= b < 127 and b != 0x22 for b in field):
                lines = ['        dc.b "%s"' % field.decode("ascii")]          # a text field as its text
            else:
                lines = data_lines(list(field), "dc.b", "dcb.b %d,$%02X")
            out.extend(["%s  ; %s" % (line, what) if i == 0 else line for i, line in enumerate(lines)])
            offset += n
            continue
        if offset in labels:
            flush()
            if offset in functions:
                out.append("")
            out.append("%s:" % labels[offset])
        insn = code.get(offset)
        if insn is not None:
            flush()
            out.append("        " + m68k.text(insn, text_names))
            offset += insn.length
        else:
            data.append(rom.data[offset])
            if (offset + 1) in labels:
                flush()
            offset += 1
    flush()
    source = "\n".join(out) + "\n"
    src_path = os.path.join(folder, stem + ".asm")
    with open(src_path, "w", newline="\n") as f:
        f.write(source)
    r = {"source": src_path, "functions": functions, "notes": [], "linked": False, "same": False,
         "code bytes": sum(i.length for i in code.values()), "rom bytes": len(rom.data)}
    from codebreaker.build import m68k_asm
    try:
        built = m68k_asm.build(source)
    except ValueError as e:
        r["notes"].append("does not assemble: %s" % e)
        return r
    r["linked"] = True
    built_path = os.path.join(folder, stem + ".built.md")
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
