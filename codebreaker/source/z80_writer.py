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
from codebreaker.source.sms_writer import data_lines


class Function(object):
    def __init__(self, name):
        self.name = name


def walk(image, origin, entries):
    """({address: Insn} reached from the entries, {targets}, [function starts])"""
    end = origin + len(image)
    mem = bytearray(0x10000)
    mem[origin:end] = image[:0x10000 - origin]
    starts = set(a for a in entries if origin <= a < end)
    code, targets, todo, seen = {}, set(), list(starts), set()
    while todo:
        pc = todo.pop()
        while origin <= pc < end and pc not in seen:
            seen.add(pc)
            insn = z80.decode(mem, pc, pc)
            if insn is None or pc + insn.size > end:
                break
            code[pc] = insn
            t = insn.target
            if t is not None and origin <= t < end and insn.flow:
                targets.add(t)
                todo.append(t)
                if insn.flow in ("call", "ccall") and not insn.pattern.startswith("RST"):
                    starts.add(t)
            if insn.flow in ("jump", "ret", "indirect"):
                break
            pc += insn.size
    return code, targets, sorted(starts)


def write(image, origin, entries, folder, stem, ports=None, names=None, header=()):
    """`entries` {address: name}; `ports` {port: name}; `names` {address: name} for fixed addresses
    outside the block (system variables, ROM routines)."""
    ports, names = ports or {}, names or {}
    os.makedirs(folder, exist_ok=True)
    code, targets, starts = walk(image, origin, entries)
    end = origin + len(image)
    fnames = dict(entries)
    functions = {}
    for a in starts:
        fnames.setdefault(a, "sub_%04X" % a)
        functions[a] = Function(fnames[a])
    covered = set()
    for a, insn in code.items():
        covered.update(range(a + 1, a + insn.size))
    line_at = lambda a: origin <= a < end and a not in covered
    labels = {}
    for a in sorted(targets):
        if line_at(a):
            labels[a] = fnames.get(a, "L_%04X" % a)
    for a, n in fnames.items():
        if line_at(a):
            labels[a] = n
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
    out = ["; %s.asm -- written by CodeBreaker" % stem] + ["; " + h for h in header] + [""]
    for p, n in sorted(used_ports.items()):
        out.append("%-16s EQU $%02X" % (n, p))
    for a, n in sorted(used_names.items()):
        out.append("%-16s EQU $%04X" % (n, a))
    if used_ports or used_names:
        out.append("")
    out.append("        ORG $%04X" % origin)
    text_names = {**labels, **used_names}
    data = []

    def flush():
        out.extend(data_lines(data, "DEFB", "DEFS %d,$%02X"))
        del data[:]
    a = origin
    while a < end:
        if a in labels:
            flush()
            if a in functions:
                out.append("")
            out.append("%s:" % labels[a])
        insn = code.get(a)
        if insn is not None:
            flush()
            t = z80.text(insn, text_names)
            if insn.pattern in ("OUT (n),A", "IN A,(n)") and insn.values[0] in used_ports:
                t = t.replace("($%02X)" % insn.values[0], "(%s)" % used_ports[insn.values[0]])
            out.append("        " + t)
            a += insn.size
        else:
            data.append(image[a - origin])
            if (a + 1) in labels:
                flush()
            a += 1
    flush()
    source = "\n".join(out) + "\n"
    src_path = os.path.join(folder, stem + ".asm")
    with open(src_path, "w", newline="\n") as f:
        f.write(source)
    r = {"source": src_path, "functions": functions, "notes": [], "linked": False, "same": False,
         "code bytes": sum(i.size for i in code.values()), "bytes": len(image)}
    from codebreaker.build import z80_asm
    try:
        at, built = z80_asm.build_flat(source, folder)
    except ValueError as e:
        r["notes"].append("does not assemble: %s" % e)
        return r
    r["linked"] = True
    r["built image"] = built
    r["same"] = at == origin and built == bytes(image)
    if not r["same"]:
        first = next((i for i in range(min(len(built), len(image))) if built[i] != image[i]), min(len(built), len(image)))
        r["notes"].append("the built block differs from the original at byte %#x" % first)
    return r
