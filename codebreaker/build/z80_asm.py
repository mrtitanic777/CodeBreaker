"""z80_asm.py -- CodeBreaker's own Z80 assembler, for the source the Master System lane writes.

    from codebreaker.build import z80_asm
    rom = z80_asm.build(source_text)            # the ROM image the source describes
    rom = z80_asm.build_file(path)              # .INCLUDE / .INCBIN resolve beside it

It reads the part of WLA-DX's language that source/sms_writer.py writes, and that Master System
and Game Gear homebrew is commonly written in, so the source also builds with WLA-DX itself:

    .MEMORYMAP ... .ENDME           the slots (only SLOTSIZE and SLOT n $addr are used)
    .ROMBANKMAP ... .ENDRO          BANKSTOTAL, BANKSIZE, BANKS (the ROM's size)
    .BANK n SLOT s                  where the following bytes go: ROM page n, seen at slot s's address
    .ORG offset / .ORGA address     within the bank: from its start / at a CPU address
    .DB / .BYTE  .DW / .WORD        bytes and little-endian words: numbers, labels, "strings"
    .DSB n value                    n bytes of value
    .INCBIN "file"                  a file's bytes
    .DEFINE NAME value / NAME = value     constants
    label:                          the address of the next byte
    ; comment

Numbers: $1F 1Fh %0101 31 'c'. Expressions + - * / & | ^ << >> ( ), and <x >x (low and high byte).
Unwritten bytes of the ROM are $FF, as WLA-DX leaves them by default (.EMPTYFILL).
"""
import os
import re

from codebreaker.decompile import z80


class AsmError(ValueError):
    pass


def _strip(line):
    out, quoted = "", False
    for c in line:
        if c == '"':
            quoted = not quoted
        if c == ";" and not quoted:
            break
        out += c
    return out.rstrip()


def _items(text):
    """Comma-separated items of a data directive (strings kept whole)."""
    out, cur, quoted, depth = [], "", False, 0
    for c in text:
        if c == '"':
            quoted = not quoted
        if not quoted:
            depth += c == "("
            depth -= c == ")"
        if c == "," and not quoted and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur.strip())
    return out


def build_file(path):
    with open(path, encoding="latin-1") as f:
        return build(f.read(), os.path.dirname(os.path.abspath(path)))


def build(source, folder="."):
    lines = []
    for n, raw in enumerate(source.splitlines(), 1):
        line = _strip(raw).strip()
        if line:
            lines.append((n, line))
    slots, banks_total, bank_size = {}, None, 0x4000
    for _n, line in lines:
        m = re.match(r"(?i)SLOT\s+(\d+)\s+(\S+)", line)
        if m:
            slots[int(m.group(1))] = z80.value(m.group(2), {})
        m = re.match(r"(?i)BANKSTOTAL\s+(\S+)", line)
        if m:
            banks_total = z80.value(m.group(1), {})
        m = re.match(r"(?i)BANKSIZE\s+(\S+)", line)
        if m:
            bank_size = z80.value(m.group(1), {})
    labels = {}
    image = None
    for final in (False, True):
        image = bytearray(b"\xFF" * ((banks_total or 1) * bank_size))
        bank, slot_base, pos = 0, slots.get(0, 0), 0
        in_map = False
        for n, line in lines:
            u = line.upper()
            if u.startswith(".MEMORYMAP") or u.startswith(".ROMBANKMAP"):
                in_map = True
                continue
            if u.startswith(".ENDME") or u.startswith(".ENDRO"):
                in_map = False
                continue
            if in_map:
                continue
            try:
                m = re.match(r"^([A-Za-z_.@][\w.@]*):\s*(.*)$", line)
                if m:
                    labels[m.group(1)] = slot_base + pos
                    line = m.group(2).strip()
                    if not line:
                        continue
                    u = line.upper()
                m = re.match(r"(?i)^\.DEFINE\s+(\S+)\s+(.+)$", line) or re.match(r"^([A-Za-z_][\w]*)\s*=\s*(.+)$", line)
                if m:
                    try:
                        labels[m.group(1)] = z80.value(m.group(2), labels, slot_base + pos)
                    except KeyError:
                        if final:
                            raise
                    continue
                m = re.match(r"(?i)^\.BANK\s+(\S+)(?:\s+SLOT\s+(\S+))?$", line)
                if m:
                    bank = z80.value(m.group(1), labels)
                    slot_base = slots.get(z80.value(m.group(2), labels), 0) if m.group(2) else slots.get(0, 0)
                    pos = 0
                    continue
                m = re.match(r"(?i)^\.ORG\s+(.+)$", line)
                if m:
                    pos = z80.value(m.group(1), labels)
                    continue
                m = re.match(r"(?i)^\.ORGA\s+(.+)$", line)
                if m:
                    pos = z80.value(m.group(1), labels) - slot_base
                    continue
                m = re.match(r"(?i)^\.(DB|BYTE|DW|WORD)\s+(.+)$", line)
                if m:
                    word = m.group(1).upper() in ("DW", "WORD")
                    data = bytearray()
                    for item in _items(m.group(2)):
                        if item.startswith('"') and item.endswith('"') and not word:
                            data += item[1:-1].encode("latin-1")
                            continue
                        try:
                            v = z80.value(item, labels, slot_base + pos + len(data))
                        except KeyError:
                            if final:
                                raise
                            v = 0
                        data += bytes([v & 0xFF, (v >> 8) & 0xFF]) if word else bytes([v & 0xFF])
                    _put(image, bank, bank_size, pos, data)
                    pos += len(data)
                    continue
                m = re.match(r"(?i)^\.DSB\s+([^,]+),\s*(.+)$", line)
                if m:
                    count, v = z80.value(m.group(1), labels), z80.value(m.group(2), labels)
                    _put(image, bank, bank_size, pos, bytes([v & 0xFF]) * count)
                    pos += count
                    continue
                m = re.match(r'(?i)^\.INCBIN\s+"([^"]+)"', line)
                if m:
                    with open(os.path.join(folder, m.group(1)), "rb") as f:
                        data = f.read()
                    _put(image, bank, bank_size, pos, data)
                    pos += len(data)
                    continue
                if u.startswith(".EMPTYFILL") or u.startswith(".SDSCTAG") or u.startswith(".COMPUTESMSCHECKSUM"):
                    continue
                if line.startswith("."):
                    raise AsmError("directive not understood")
                try:
                    code = z80.assemble(line, slot_base + pos, labels)
                except KeyError:
                    if final:
                        raise
                    code = _size_guess(line, slot_base + pos)
                _put(image, bank, bank_size, pos, code)
                pos += len(code)
            except (KeyError, ValueError) as e:
                raise AsmError("line %d: %s (%s)" % (n, line, e))
    return bytes(image)


def _size_guess(line, pc):
    """An instruction's size on the first pass, when a label it names is not defined yet: assembled
    with every unknown name taken as a value that fits each operand kind."""
    class _Any(dict):
        def __contains__(self, k):
            return True

        def __getitem__(self, k):
            return pc
    return z80.assemble(line, pc, _Any())


def _put(image, bank, bank_size, pos, data):
    at = bank * bank_size + pos
    if at < 0 or at + len(data) > len(image):
        raise AsmError("bytes outside the ROM (bank %d, offset $%X)" % (bank, pos))
    image[at:at + len(data)] = data
