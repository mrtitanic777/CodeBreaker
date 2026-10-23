"""m68k_asm.py -- CodeBreaker's own 68000 assembler, for the source the Genesis lane writes.

    from codebreaker.build import m68k_asm
    image = m68k_asm.build(source_text)         # the bytes from address 0 to the last one written

It reads the part of the Motorola assemblers' language (asm68k, SNASM68K, vasm's mot syntax) that
source/genesis_writer.py writes, so the source also builds with those:

    org $addr                       where the next bytes go
    dc.b / dc.w / dc.l              bytes, words and longs, big-endian: numbers, labels, "strings"
    dcb.b n,value                   n bytes of value (dcb.w / dcb.l: words, longs)
    NAME equ value                  constants
    label:                          the address of the next byte
    ; comment  (or * at the start of a line)

Unwritten bytes between two org's are zero.
"""
import re

from codebreaker.decompile import m68k
from codebreaker.build.z80_asm import AsmError, _items


def _strip(line):
    if line.lstrip().startswith("*"):
        return ""
    out, quoted = "", False
    for c in line:
        if c == '"':
            quoted = not quoted
        if c == ";" and not quoted:
            break
        out += c
    return out.rstrip()


def build(source):
    lines = [(n, _strip(raw).strip()) for n, raw in enumerate(source.splitlines(), 1)]
    lines = [(n, l) for n, l in lines if l]
    labels = {}
    image = bytearray()
    sizes = {}                                       # line -> bytes on the first pass (branches settle on pass 2)
    for final in (False, True):
        image = bytearray()
        pc = 0
        for n, line in lines:
            try:
                m = re.match(r"^([A-Za-z_.@][\w.@]*):\s*(.*)$", line)
                if m:
                    labels[m.group(1)] = pc
                    line = m.group(2).strip()
                    if not line:
                        continue
                m = re.match(r"(?i)^([A-Za-z_]\w*)\s+equ\s+(.+)$", line)
                if m:
                    labels[m.group(1)] = m68k._value(m.group(2), labels, pc)
                    continue
                m = re.match(r"(?i)^org\s+(.+)$", line)
                if m:
                    pc = m68k._value(m.group(1), labels, pc)
                    continue
                m = re.match(r"(?i)^dc\.([bwl])\s+(.+)$", line)
                if m:
                    width = {"b": 1, "w": 2, "l": 4}[m.group(1).lower()]
                    data = bytearray()
                    for item in _items(m.group(2)):
                        if item.startswith('"') and item.endswith('"') and width == 1:
                            data += item[1:-1].encode("latin-1")
                            continue
                        try:
                            v = m68k._value(item, labels, pc + len(data))
                        except KeyError:
                            if final:
                                raise
                            v = 0
                        data += (v & ((1 << (8 * width)) - 1)).to_bytes(width, "big")
                    _put(image, pc, data)
                    pc += len(data)
                    continue
                m = re.match(r"(?i)^dcb\.([bwl])\s+([^,]+),\s*(.+)$", line)
                if m:
                    width = {"b": 1, "w": 2, "l": 4}[m.group(1).lower()]
                    count, v = m68k._value(m.group(2), labels, pc), m68k._value(m.group(3), labels, pc)
                    _put(image, pc, (v & ((1 << (8 * width)) - 1)).to_bytes(width, "big") * count)
                    pc += width * count
                    continue
                if re.match(r"(?i)^even$", line):
                    pc += pc & 1
                    continue
                try:
                    code = m68k.assemble(line, pc, labels)
                except KeyError:
                    if final:
                        raise
                    code = _size_guess(line, pc, labels)
                if not final:
                    sizes[n] = len(code)
                elif len(code) != sizes.get(n, len(code)):
                    raise AsmError("the instruction's size changed between passes")
                _put(image, pc, code)
                pc += len(code)
            except (KeyError, ValueError) as e:
                raise AsmError("line %d: %s (%s)" % (n, line, e))
    return bytes(image)


def _size_guess(line, pc, labels):
    """On the first pass, an instruction naming a label not defined yet: assembled with every
    unknown name standing for a nearby address (the source gives each operand's size: BRA.S / .W,
    (x).W / (x).L), so its size is the size it will have."""
    class _Any(dict):
        def __contains__(self, k):
            return True

        def __getitem__(self, k):
            return labels[k] if dict.__contains__(labels, k) else pc + 4
    return m68k.assemble(line, pc, _Any())


def _put(image, at, data):
    if at + len(data) > len(image):
        image.extend(bytes(at + len(data) - len(image)))
    image[at:at + len(data)] = data
