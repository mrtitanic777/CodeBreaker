"""m6502_asm.py -- assemble and link 6502 source in ca65 syntax, with an ld65 configuration.

    from codebreaker.build import m6502_asm
    image = m6502_asm.build(source_text, config_text, folder)   # the bytes ld65 would write

The subset the NES lane writes (source/nes_writer.py), enough that it rebuilds without the
cc65 suite on the machine; the same files build with ca65 and ld65 (`ca65 game.s` and
`ld65 -C game.cfg game.o`). Understood:

    .segment "NAME"         switch to a segment (the configuration says where it loads)
    name:                   a label at the current address
    NAME = $2000            a constant
    .byte 1, $02, "text"    bytes and strings          .word $1234, label    little-endian words
    .incbin "file"          a file's bytes, from `folder`
    LDA $0200,x ...         an instruction (decompile/m6502.assemble)
    ; ...                   a comment

The configuration's MEMORY areas, in order, are the output file: each starts at its `start`,
holds the segments that `load` into it, and is filled to its `size` with `fill` value 0
(fill=yes), as ld65 does.
"""
import os
import re

from codebreaker.decompile import m6502

_AREA = re.compile(r"(\w+)\s*:\s*([^;]*);")


def _number(s):
    s = s.strip()
    if s.startswith("$"):
        return int(s[1:], 16)
    if s.startswith("%"):
        return int(s[1:], 2)
    return int(s, 0)


def config(text):
    """(memory areas [(name, start, size, fill)], {segment: area})"""
    text = re.sub(r"#.*", "", text)
    blocks = dict(re.findall(r"(MEMORY|SEGMENTS)\s*\{([^}]*)\}", text))
    areas, segments = [], {}
    for name, body in _AREA.findall(blocks.get("MEMORY", "")):
        attrs = dict(a.split("=", 1) for a in re.sub(r"\s+", "", body).split(",") if "=" in a)
        areas.append((name, _number(attrs.get("start", "0")), _number(attrs.get("size", "0")),
                      attrs.get("fill", "no") == "yes"))
    for name, body in _AREA.findall(blocks.get("SEGMENTS", "")):
        attrs = dict(a.split("=", 1) for a in re.sub(r"\s+", "", body).split(",") if "=" in a)
        segments[name] = attrs["load"]
    return areas, segments


def _split_args(s):
    out, cur, quoted = [], "", False
    for ch in s:
        if ch == '"':
            quoted = not quoted
        if ch == "," and not quoted:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def _strip_comment(line):
    quoted = False
    for i, ch in enumerate(line):
        if ch == '"':
            quoted = not quoted
        elif ch == ";" and not quoted:
            return line[:i]
    return line


def build(source, config_text, folder="."):
    """-> the linked image (bytes). Raises ValueError with the line on an error."""
    areas, segments = config(config_text)
    starts = {name: start for name, start, _size, _fill in areas}
    labels = {}
    for pass_ in (1, 2):
        out = {name: bytearray() for name, _s, _z, _f in areas}
        area = None
        for n, raw in enumerate(source.splitlines(), 1):
            line = _strip_comment(raw).strip()
            if not line:
                continue
            try:
                m = re.match(r"^([A-Za-z_]\w*):\s*(.*)$", line)
                if m:
                    if area is None:
                        raise ValueError("a label outside any segment")
                    labels[m.group(1)] = starts[area] + len(out[area])
                    line = m.group(2).strip()
                    if not line:
                        continue
                c = re.match(r"^([A-Za-z_]\w*)\s*=\s*(\S+)$", line)
                if c:                                    # a constant: NAME = value
                    labels[c.group(1)] = _number(c.group(2))
                    continue
                if line.startswith(".segment"):
                    seg = re.search(r'"(\w+)"', line).group(1)
                    area = segments[seg]
                    continue
                if area is None:
                    raise ValueError("code outside any segment")
                here = out[area]
                pc = starts[area] + len(here)
                if line.startswith((".byte", ".byt")):
                    for a in _split_args(line.split(None, 1)[1]):
                        here += a[1:-1].encode("latin1") if a.startswith('"') else bytes([_number(a) & 0xFF])
                elif line.startswith((".word", ".addr")):
                    for a in _split_args(line.split(None, 1)[1]):
                        v = labels.get(a, 0) if re.match(r"^[A-Za-z_]\w*$", a) else _number(a)
                        if pass_ == 2 and re.match(r"^[A-Za-z_]\w*$", a) and a not in labels:
                            raise ValueError("unknown label %s" % a)
                        here += bytes([v & 0xFF, v >> 8 & 0xFF])
                elif line.startswith(".incbin"):
                    with open(os.path.join(folder, re.search(r'"([^"]+)"', line).group(1)), "rb") as f:
                        here += f.read()
                elif line.startswith("."):
                    raise ValueError("directive not understood")
                else:
                    if pass_ == 1:                       # (sizes only: every label is far, every branch near)
                        known = dict(labels)
                        for word in re.findall(r"[A-Za-z_]\w*", line.split(None, 1)[1] if " " in line else ""):
                            if word.lower() not in ("a", "x", "y") and word not in known:
                                known[word] = 0xFFFF
                        mnemonic = line.split()[0].upper()
                        if mnemonic in m6502.BRANCHES:
                            here += b"\0\0"
                            continue
                        here += m6502.assemble(line, pc, known)
                    else:
                        here += m6502.assemble(line, pc, labels)
            except (ValueError, KeyError, AttributeError) as e:
                raise ValueError("line %d: %s (%s)" % (n, raw.strip(), e))
    image = bytearray()
    for name, _start, size, fill in areas:
        body = out[name]
        if len(body) > size:
            raise ValueError("%s holds %d bytes, more than its %d" % (name, len(body), size))
        image += body + (bytes(size - len(body)) if fill else b"")
    return bytes(image)
