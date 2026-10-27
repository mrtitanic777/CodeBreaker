"""image_writer.py -- what the writers of a program image share: a cartridge, a tape block, a ROM.

    from codebreaker.source import image_writer as w
    code, targets, starts = w.walk(starts, decode, inside, fits, follows, stops)
    functions = w.functions(starts, names, "sub_%04X")
    labels = w.labels(targets, names, line_start, "L_%04X")
    w.emit(out, start, end, byte, address, code_at, labels, functions, text, lines)
    r = w.result(folder, stem + ".asm", source, functions, code, size)
    w.assembled(r, build, original, built_path)

Every one of these programs comes back the same way. Code is what the program's own flow reaches
from its entry points; every byte it reaches is written as an instruction, every other byte as
data. Function starts keep their entry point's name or are `sub_<address>`, jump targets
`L_<address>`, and operands naming the start of a line get that line's label. The source is then
assembled and the result compared with the original byte for byte.

What differs from one machine to the next -- its processor, the assembler's spelling, how the
image is laid out in memory, which hardware has names -- stays in its writer (source/nes_writer.py,
sms_writer.py, gb_writer.py, genesis_writer.py, z80_writer.py).
"""
import os

from codebreaker.compare import image


class Function(object):
    def __init__(self, name):
        self.name = name


def _size(insn):
    return insn.size


def walk(starts, decode, inside, fits, follows, stops, key=None, calls=None, size=_size):
    """What flow reaches from `starts`. -> ({key: Insn}, {target}, [function starts])

    decode(pc) -> the instruction at pc, or None; inside(pc): may code stand at pc; fits(pc, insn):
    the whole instruction lies in the program; follows(insn): its target (not None) is a place
    flow goes; stops(insn): flow does not go on to the next instruction. key(pc): where the
    instruction is filed (default: pc); calls(insn): the target followed is a function's start."""
    found = set(starts)
    code, targets, todo, seen = {}, set(), list(found), set()
    while todo:
        pc = todo.pop()
        while inside(pc) and pc not in seen:
            seen.add(pc)
            insn = decode(pc)
            if insn is None or not fits(pc, insn):
                break
            code[key(pc) if key else pc] = insn
            if insn.target is not None and follows(insn):
                targets.add(insn.target)
                todo.append(insn.target)
                if calls and calls(insn):
                    found.add(insn.target)
            if stops(insn):
                break
            pc += size(insn)
    return code, targets, sorted(found) if calls else list(starts)


def functions(starts, names, fmt):
    """{start: Function}, each named from `names` or `fmt` % address (the name is added to `names`)."""
    out = {}
    for a in starts:
        names.setdefault(a, fmt % a)
        out[a] = Function(names[a])
    return out


def covered(code, size=_size):
    """The addresses inside an instruction, past its first byte: no line starts there."""
    inside = set()
    for at, insn in code.items():
        inside.update(range(at + 1, at + size(insn)))
    return inside


def labels(targets, names, line_start, fmt):
    """{address: label} for each flow target and each named address where a line starts."""
    out = {}
    for a in sorted(targets):
        if line_start(a):
            out[a] = names.get(a, fmt % a)
    for a, n in names.items():
        if line_start(a):
            out[a] = n
    return out


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


def commented(lines, what):
    """`lines` with `what` as a comment on the first."""
    return ["%s  ; %s" % (line, what) if i == 0 else line for i, line in enumerate(lines)]


def emit(out, start, end, byte, address, code_at, labels, functions, text, lines, special=None, size=_size):
    """Append the lines for the program's bytes [start, end) to `out`.

    byte(offset) -> the byte there; address(offset) -> where the processor sees it (labels and
    functions are by address); code_at(offset, address) -> the instruction to write there, or None
    for data; text(insn) -> its line; lines(bytes) -> the data's lines. special(offset, address,
    flush), when given, writes what stands at a place of its own (a header field, the vectors) and
    returns how many bytes it took, or 0."""
    data = []

    def flush():
        out.extend(lines(data))
        del data[:]
    offset = start
    while offset < end:
        addr = address(offset)
        took = special(offset, addr, flush) if special else 0
        if took:
            offset += took
            continue
        if addr in labels:
            flush()
            if addr in functions:
                out.append("")
            out.append("%s:" % labels[addr])
        insn = code_at(offset, addr)
        if insn is not None:
            flush()
            out.append("        " + text(insn))
            offset += size(insn)
        else:
            data.append(byte(offset))
            if (addr + 1) in labels:
                flush()
            offset += 1
    flush()


def result(folder, name, source, functions, code, size=_size, **more):
    """Write the source; -> the result every writer gives (`more`: its own fields)."""
    path = os.path.join(folder, name)
    with open(path, "w", newline="\n") as f:
        f.write(source)
    r = {"source": path, "functions": functions, "notes": [], "linked": False, "same": False,
         "code bytes": sum(size(i) for i in code.values())}
    r.update(more)
    return r


def assembled(r, build, original, built_path=None, what="image", same=None):
    """Assemble with build() -> bytes, keep what came out at `built_path`, and say in `r` whether it
    is the original. same(built), when given, is that test. -> the bytes built, or None."""
    try:
        built = build()
    except ValueError as e:
        r["notes"].append("does not assemble: %s" % e)
        return None
    r["linked"] = True
    if built_path:
        with open(built_path, "wb") as f:
            f.write(built)
        r["built"] = built_path
    r["same"] = same(built) if same else built == original
    if not r["same"]:
        at = image.first_difference(built, original)
        r["notes"].append("the built %s differs from the original at byte %#x" % (
            what, len(original) if at is None else at))
    return built


def main(argv, reconstruct, doc, unit="ROM bytes", total="rom bytes"):
    """The command line every writer has: <program> [<out folder>]."""
    if not argv:
        print(doc.split("\n\n")[0])
        return 2
    folder = argv[1] if len(argv) > 1 else os.path.splitext(argv[0])[0] + "_source"
    r = reconstruct(argv[0], folder)
    print("%s: %d functions, %d of %d %s reached as code -> %s" % (
        os.path.basename(argv[0]), len(r["functions"]), r["code bytes"], r[total], unit, r["source"]))
    print("  rebuilt: %s" % ("identical to the original" if r["same"] else "; ".join(r["notes"]) or "differs"))
    return 0 if r["same"] else 1

