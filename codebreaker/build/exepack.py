"""exepack.py -- store a real-mode program the way LINK /EXEPACK did, and read how a stored one was.

    from codebreaker.build import exepack
    layout = exepack.layout(stored_module, cs, ip)     how the original was packed: .fills, .stub_code
    module, cs, ip = exepack.pack(image, relocs, entry, stack, fills, stub_code)

The format (formats/dos.unpack_exepack undoes it): the stored load module is the program's
bytes with stretches of one repeated byte ("fills") taken out, followed by a paragraph-aligned
segment the file's entry point names. That segment starts with eight words -- the program's IP,
CS, a scratch word, the segment's own size, SP, SS, the program's size in paragraphs, "RB" --
then the unpacker's code and its message ("Packed file is corrupt"), then the relocation table
as sixteen lists, one per 64 KB, each a count and that many offsets.

The packed stream is read backwards by the unpacker, from just below that segment: each command
is a length word and a command byte (B0 fill, B2 copy; bit 0 marks the last), a fill preceded by
its one byte, a copy by its bytes. The program's first bytes, up to the first fill, are stored
as they are -- no command is spent on them -- and FF pads the stream to the paragraph.

Which stretches are fills is the linker's choice, made from how the program was written: LINK
packed what the object file gave it as ITERATED data (`db 218 dup (0)`), not every run of a
byte -- a run written out byte by byte stays as it is. The fills of a rebuilt program are
therefore stated: they are the `dup`s of its source, listed beside it (layout() reads them from
the original).
"""
import struct

MESSAGE = b"Packed file is corrupt"
FILL, COPY = 0xB0, 0xB2


class Layout(object):
    def __init__(self, fills, stub_code, size, words):
        self.fills = fills              # [(offset, length, byte)] in the program, ascending
        self.stub_code = stub_code      # the unpacker's code and message: what follows the header words
        self.size = size                # the program's size in bytes (paragraphs * 16)
        self.words = words              # 8, or 9 in the later header (a skip count before "RB")


def layout(module, cs, ip):
    """How a stored load module was packed. -> Layout. ValueError when it is not EXEPACK."""
    at = cs * 16
    words = struct.unpack_from("<%dH" % (ip // 2), module, at)
    if ip not in (0x10, 0x12) or words[-1] != 0x4252 or (ip == 0x12 and words[7] != 1):
        raise ValueError("not the EXEPACK this writes (a header of eight or nine words ending in RB)")
    own_size, paras = words[3], words[6]
    src, dst, fills = at, paras * 16, []
    while module[src - 1] == 0xFF:
        src -= 1
    while True:
        command, length = module[src - 1], struct.unpack_from("<H", module, src - 3)[0]
        src -= 3
        if command & 0xFE == FILL:
            src -= 1
            dst -= length
            fills.append((dst, length, module[src]))
        elif command & 0xFE == COPY:
            src -= length
            dst -= length
        else:
            raise ValueError("EXEPACK command %#x" % command)
        if command & 1:
            break
    if src != dst:
        raise ValueError("the stored program's first bytes are not stored as they are")
    table = module.find(MESSAGE, at, at + own_size)
    if table < 0:
        raise ValueError("EXEPACK message not found")
    return Layout(sorted(fills), bytes(module[at + ip:table + len(MESSAGE)]), paras * 16, ip // 2)


def pack(image, relocs, entry, stack, fills, stub_code, words=8):
    """The program `image` (its relocation offsets in the order the table lists them, entry
    (cs, ip), stack (ss, sp), all relative to the program) stored packed, with a header of
    `words` words. -> (stored load module, (cs, ip, ss, sp) of the file's header): the unpacker
    runs from its own copy behind the program, with a stack of 80h bytes there."""
    image = bytes(image) + bytes(-len(image) % 16)
    fills = sorted(fills)
    for off, n, byte in fills:
        if image[off:off + n] != bytes([byte]) * n or not 0 < n <= 0xFFFF:
            raise ValueError("a fill at %#x is not %d bytes of %02X" % (off, n, byte))
    # the commands, from the first fill to the end of the program
    commands, at = [], fills[0][0] if fills else len(image)
    for off, n, byte in fills + [(len(image), 0, None)]:
        while at < off:
            k = min(off - at, 0xFFFF)
            commands.append((COPY, image[at:at + k]))
            at += k
        if n:
            commands.append((FILL, bytes([byte]), n))
            at += n
    stream = bytearray(image[:fills[0][0] if fills else len(image)])
    for i, c in enumerate(commands):
        last = 1 if i == 0 else 0                          # read last: the lowest command
        if c[0] == FILL:
            stream += c[1] + struct.pack("<HB", c[2], FILL | last)
        else:
            stream += c[1] + struct.pack("<HB", len(c[1]), COPY | last)
    stream += b"\xFF" * (-len(stream) % 16)
    lists = [[] for _ in range(16)]
    for r in relocs:                                         # in the order given: LINK's was its fixups'
        lists[r >> 16].append(r & 0xFFFF)
    table = b"".join(struct.pack("<H%dH" % len(l), len(l), *l) for l in lists)
    own = 2 * words + len(stub_code) + len(table)
    (cs, ip), (ss, sp) = entry, stack
    head = struct.pack("<7H", ip, cs, 0, own, sp, ss, len(image) // 16) + (struct.pack("<H", 1) if words == 9 else b"") + b"RB"
    stub_cs = len(stream) // 16
    return bytes(stream) + head + stub_code + table, (stub_cs, 2 * words, len(image) // 16 + (own + 15) // 16, 0x80)
