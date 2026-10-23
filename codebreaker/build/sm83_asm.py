"""sm83_asm.py -- CodeBreaker's own Game Boy assembler, for the source the Game Boy lane writes.

    from codebreaker.build import sm83_asm
    rom = sm83_asm.build(source_text, size)     # the ROM image (size: its bytes; unwritten ones $FF)

It reads the part of RGBDS's language that source/gb_writer.py writes, and that Game Boy homebrew
is commonly written in, so the source also builds with RGBDS (rgbasm, rgblink -n / rgbfix):

    SECTION "name", ROM0[$addr]                 bank 0, at a fixed address
    SECTION "name", ROMX[$addr], BANK[n]        bank n (seen at $4000-$7FFF)
    db / dw                                     bytes, little-endian words: numbers, labels, "strings"
    ds n, value                                 n bytes of value
    INCBIN "file"
    DEF NAME EQU value  /  NAME EQU value       constants
    label:  /  .local:                          the address of the next byte
    ; comment
"""
import os
import re

from codebreaker.decompile import sm83
from codebreaker.build.z80_asm import AsmError, _strip, _items

BANK = 0x4000


def build(source, size, folder="."):
    lines = [(n, _strip(raw).strip()) for n, raw in enumerate(source.splitlines(), 1)]
    lines = [(n, l) for n, l in lines if l]
    labels = {}
    image = None
    for final in (False, True):
        image = bytearray(b"\xFF" * size)
        bank, base, pos = 0, 0, 0
        for n, line in lines:
            try:
                m = re.match(r"^([A-Za-z_.][\w.]*):{1,2}\s*(.*)$", line)
                if m:
                    labels[m.group(1)] = base + pos
                    line = m.group(2).strip()
                    if not line:
                        continue
                m = re.match(r"(?i)^(?:DEF\s+)?([A-Za-z_]\w*)\s+EQU\s+(.+)$", line)
                if m:
                    try:
                        labels[m.group(1)] = sm83.value(m.group(2), labels, base + pos)
                    except KeyError:
                        if final:
                            raise
                    continue
                m = re.match(r'(?i)^SECTION\s+"[^"]*"\s*,\s*(ROM0|ROMX)\s*\[\s*([^\]]+)\]\s*(?:,\s*BANK\s*\[\s*([^\]]+)\])?$', line)
                if m:
                    addr = sm83.value(m.group(2), labels)
                    bank = sm83.value(m.group(3), labels) if m.group(3) else (0 if m.group(1).upper() == "ROM0" else 1)
                    base = 0 if bank == 0 else BANK
                    pos = addr - base
                    continue
                m = re.match(r"(?i)^(DB|DW)\s+(.+)$", line)
                if m:
                    word = m.group(1).upper() == "DW"
                    data = bytearray()
                    for item in _items(m.group(2)):
                        if item.startswith('"') and item.endswith('"') and not word:
                            data += item[1:-1].encode("latin-1")
                            continue
                        try:
                            v = sm83.value(item, labels, base + pos + len(data))
                        except KeyError:
                            if final:
                                raise
                            v = 0
                        data += bytes([v & 0xFF, (v >> 8) & 0xFF]) if word else bytes([v & 0xFF])
                    _put(image, bank, pos, data)
                    pos += len(data)
                    continue
                m = re.match(r"(?i)^DS\s+([^,]+)(?:,\s*(.+))?$", line)
                if m:
                    count = sm83.value(m.group(1), labels)
                    v = sm83.value(m.group(2), labels) if m.group(2) else 0
                    _put(image, bank, pos, bytes([v & 0xFF]) * count)
                    pos += count
                    continue
                m = re.match(r'(?i)^INCBIN\s+"([^"]+)"', line)
                if m:
                    with open(os.path.join(folder, m.group(1)), "rb") as f:
                        data = f.read()
                    _put(image, bank, pos, data)
                    pos += len(data)
                    continue
                try:
                    code = sm83.assemble(line, base + pos, labels)
                except KeyError:
                    if final:
                        raise
                    code = _size_guess(line, base + pos)
                _put(image, bank, pos, code)
                pos += len(code)
            except (KeyError, ValueError) as e:
                raise AsmError("line %d: %s (%s)" % (n, line, e))
    return bytes(image)


def _size_guess(line, pc):
    class _Any(dict):
        def __contains__(self, k):
            return True

        def __getitem__(self, k):
            return 0xFF80 if "LDH" in line.upper() else pc
    return sm83.assemble(line, pc, _Any())


def _put(image, bank, pos, data):
    at = bank * BANK + pos
    if at < 0 or at + len(data) > len(image):
        raise AsmError("bytes outside the ROM (bank %d, offset $%X)" % (bank, pos))
    image[at:at + len(data)] = data
