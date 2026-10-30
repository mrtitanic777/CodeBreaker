"""z80_writer.py -- a flat Z80 program (one block of memory at one address) as assembly source that
builds back into it. The ZX Spectrum and MSX lanes write through it (source/zx_writer.py's and
msx's callers give the block, its entry points and the machine's names).

    from codebreaker.source import z80_writer
    r = z80_writer.write(image, origin, entries, folder, stem, ports=..., names=..., header=[...])
    r["source"]                     <stem>.asm (sjasmplus / pasmo syntax: ORG, DEFB, DEFW, DEFS, EQU)
    r["same"]                       the source, built, is the block byte for byte
    r["functions"]                  {address: Function(name)}

Code is what the program's own flow reaches from its entry points; every byte it reaches is
written as an instruction, every other byte as data. Entry points keep the names given,
called functions are `sub_<address>`, jump targets `L_<address>`, and pointers loaded into HL
DE IX IY (or read and written through) that name the start of a line get that line's label, so
the source can be moved and edited, not only rebuilt. Ports and fixed addresses (system
variables, ROM routines) are written by the names the machine's documentation gives them.
"""
import os

from codebreaker.decompile import z80
from codebreaker.source import image_writer as w


def walk(image, origin, entries):
    """({address: Insn} reached from the entries, {targets}, [function starts])"""
    end = origin + len(image)
    mem = bytearray(0x10000)
    mem[origin:end] = image[:0x10000 - origin]
    return w.walk([a for a in entries if origin <= a < end], lambda pc: z80.decode(mem, pc, pc),
                  inside=lambda pc: origin <= pc < end,
                  fits=lambda pc, insn: pc + insn.size <= end,
                  follows=lambda insn: origin <= insn.target < end and insn.flow,
                  stops=lambda insn: insn.flow in ("jump", "ret", "indirect"),
                  calls=lambda insn: insn.flow in ("call", "ccall") and not insn.pattern.startswith("RST"))


def write(image, origin, entries, folder, stem, ports=None, names=None, header=()):
    """`entries` {address: name}; `ports` {port: name}; `names` {address: name} for fixed addresses
    outside the block (system variables, ROM routines)."""
    ports, names = ports or {}, names or {}
    os.makedirs(folder, exist_ok=True)
    code, targets, starts = walk(image, origin, entries)
    end = origin + len(image)
    fnames = dict(entries)
    functions = w.functions(starts, fnames, "sub_%04X")
    covered = w.covered(code)

    def line_at(a):
        return origin <= a < end and a not in covered
    labels = w.labels(targets, fnames, line_at, "L_%04X")
    pointer = ("LD HL,nn", "LD DE,nn", "LD IX,nn", "LD IY,nn", "LD SP,nn")
    used_names, used_ports = {}, {}
    for insn in code.values():
        v = insn.values[-1] if insn.values else None
        if insn.pattern in ("OUT (n),A", "IN A,(n)") and insn.values[0] in ports:
            used_ports[insn.values[0]] = ports[insn.values[0]]
        if v is None:
            continue
        if not (origin <= v < end) and v in names and ("nn" in insn.pattern or insn.flow):
            used_names[v] = names[v]
        elif line_at(v) and (insn.pattern in pointer or (insn.flow == "" and "(nn)" in insn.pattern)):
            labels.setdefault(v, fnames.get(v, "L_%04X" % v) if v in code else "D_%04X" % v)
    out = w.lines(["; %s.asm -- written by CodeBreaker" % stem] + ["; " + h for h in header] + [""], "sjasmplus", stem)
    for p, n in sorted(used_ports.items()):
        out.append("%-16s EQU $%02X" % (n, p))
    for a, n in sorted(used_names.items()):
        out.append("%-16s EQU $%04X" % (n, a))
    if used_ports or used_names:
        out.append("")
    out.append("        ORG $%04X" % origin)
    text_names = {**labels, **used_names}

    def text(insn):
        t = z80.text(insn, text_names)
        if t.startswith(".db "):                 # bytes an assembler would not write so (z80.text), in this syntax
            t = "DEFB " + t[4:]
        if insn.pattern in ("OUT (n),A", "IN A,(n)") and insn.values[0] in used_ports:
            t = t.replace("($%02X)" % insn.values[0], "(%s)" % used_ports[insn.values[0]])
        return t
    w.emit(out, origin, end, lambda a: image[a - origin], lambda a: a, lambda a, addr: code.get(a), labels,
           functions, text, lambda data: w.data_lines(data, "DEFB", "DEFS %d,$%02X"))
    source = w.text(out) + "\n"
    r = w.result(folder, stem + ".asm", source, functions, code, bytes=len(image))
    from codebreaker.build import z80_asm
    placed = {}

    def build():
        placed["at"], built = z80_asm.build_flat(source, folder)
        return built
    built = w.assembled(r, build, bytes(image), what="block",
                        same=lambda built: placed["at"] == origin and built == bytes(image))
    if built is not None:
        r["built image"] = built
    return r
