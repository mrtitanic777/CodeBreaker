"""unpack.py -- undo any on-disk compression of a DOS program by letting its unpacker run.

    from codebreaker.emulate import unpack
    module, relocations, cs, ip, ss, sp = unpack.unpacked(module, relocations, cs, ip, ss, sp, min_alloc)

A compressed program starts in a small unpacker that rebuilds the real program in memory, fixes
its segment references for where it was loaded, sets the stack, and jumps to the real entry
point. That is run here, in real mode, on a bare megabyte with a program prefix in front, and
stopped at that jump. Which jump it is follows from who wrote what: the unpacker's own code came
from the file (generation 0); most unpackers first copy themselves out of the way (generation 1,
written by generation 0) and the program they rebuild is then generation 2. Execution reaching
generation 2 is the real entry point, and so is a jump out of the moved copy down to bytes of the
file (a scheme that leaves the front of the program as it was stored). An unpacker that does not
move itself rebuilds the program as generation 1; it is recognised when the code it jumped to
-- code that is not a copy of bytes of the file -- makes its first call to DOS.

Some programs are not compressed but still begin by moving themselves (COMMAND.COM slides down
over its program prefix's unused end and continues there). `moved=True` runs such a start until
it continues in bytes it wrote, and returns memory as it then is.

The unpacker has by then applied the relocations for one load segment and thrown the table
away. It is run twice at two load segments: every word that differs by exactly the distance
between them is a relocation, and anything else that differs is reported as a failure.
"""
import array
import struct

_MEMORY = 0x110000
_TOP = 0xA000                       # first segment past conventional memory


class _Run(object):
    def __init__(self, module, relocations, cs, ip, ss, sp, load, limit, moved=False):
        self.moved, self.size, self.module = moved, len(module), bytes(module)
        self.copied = False                                   # the first written code to run is a copy of the file's
        import unicorn
        from unicorn import x86_const as X
        self.X, self.load, self.base = X, load, load * 16
        self.uc = uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_16)
        uc.mem_map(0, _MEMORY)
        img = bytearray(module)
        for at in relocations:
            struct.pack_into("<H", img, at, (struct.unpack_from("<H", img, at)[0] + load) & 0xFFFF)
        psp = load - 0x10
        prefix = bytearray(0x100)
        prefix[0:2] = b"\xCD\x20"
        struct.pack_into("<H", prefix, 2, _TOP)
        struct.pack_into("<H", prefix, 0x2C, psp - 0x20)      # an (empty) environment
        prefix[0x80:0x82] = b"\x00\x0D"
        uc.mem_write(psp * 16, bytes(prefix))
        uc.mem_write(self.base, bytes(img))
        for reg, value in ((X.UC_X86_REG_DS, psp), (X.UC_X86_REG_ES, psp), (X.UC_X86_REG_SS, (load + ss) & 0xFFFF),
                           (X.UC_X86_REG_SP, sp), (X.UC_X86_REG_CS, (load + cs) & 0xFFFF)):
            uc.reg_write(reg, value)
        self.gen = bytearray(_MEMORY)                         # who wrote each byte: 0 = the file
        self.now = 0                                          # generation of the code running
        self.first = None                                     # state when written code first ran
        self.found = None                                     # state at the real entry point
        self.exited = False
        self.stub_low = _MEMORY                               # start of the unpacker's moved copy
        self.when = array.array("I", bytes(4 * _MEMORY))      # ... and when (a count of writes)
        self.clock = 0
        uc.hook_add(unicorn.UC_HOOK_BLOCK, self._block)
        uc.hook_add(unicorn.UC_HOOK_MEM_WRITE, self._write)
        uc.hook_add(unicorn.UC_HOOK_INTR, self._interrupt)
        self.error = None
        try:
            uc.emu_start(((load + cs) & 0xFFFF) * 16 + ip, 0, count=limit)
        except unicorn.UcError as e:
            self.error = str(e)

    def _state(self, address):
        """Registers on entry to the block at `address`. (CS is worked out from the address: read
        inside a hook just after a far jump it can still be the old one.)"""
        X, r = self.X, self.uc.reg_read
        ip = r(X.UC_X86_REG_IP)
        return dict(cs=((address - ip) >> 4) & 0xFFFF, ip=ip, ss=r(X.UC_X86_REG_SS), sp=r(X.UC_X86_REG_SP))

    def _block(self, uc, address, size, _user):
        g = self.gen[address] if address < _MEMORY else 0
        left = self.now >= 1 and g == 0 and self.base <= address < self.stub_low   # the moved copy jumps
        self.now = g                                                              # down into the file's bytes
        if g >= 2 or left:
            self.found = self._state(address)
            uc.emu_stop()
        elif g == 1 and self.first is None:
            self.first = self._state(address)
            self.copied = bytes(uc.mem_read(address, 24)) in self.module
            if self.moved:                                    # all that was asked for
                self.found = self.first
                uc.emu_stop()
            low = address
            while low > self.base and self.gen[low - 1] == 1:
                low -= 1
            self.stub_low = low                               # where the unpacker's moved copy starts

    def _write(self, uc, _access, address, size, _value, _user):
        X = self.X
        top = uc.reg_read(X.UC_X86_REG_SS) * 16 + uc.reg_read(X.UC_X86_REG_SP)
        if -8 <= address - top < 64 or address + size > _MEMORY:    # the stack is not the program
            return
        g = min(self.now + 1, 3)
        self.clock += 1
        for i in range(address, address + size):
            self.gen[i] = g
            self.when[i] = self.clock

    def _interrupt(self, uc, number, _user):
        X = self.X
        ah = (uc.reg_read(X.UC_X86_REG_AX) >> 8) & 0xFF
        flags = uc.reg_read(X.UC_X86_REG_EFLAGS)
        if number == 0x20 or (number == 0x21 and ah in (0x00, 0x4C)):
            self.exited = True
            uc.emu_stop()
        elif number == 0x21 and ah == 0x30:                   # DOS version 7.10
            uc.reg_write(X.UC_X86_REG_AX, 0x0A07)
        elif number == 0x21 and ah in (0x09, 0x02, 0x4A, 0x25, 0x35, 0x1A):
            uc.reg_write(X.UC_X86_REG_EFLAGS, flags & ~1)
        elif self.now >= 1 and self.first is not None and not self.copied:    # the rebuilt program asks DOS for
                                                                             # something (a moved unpacker would
                                                                             # only do so to report an error)
            self.found = self.first
            uc.emu_stop()
        else:
            uc.reg_write(X.UC_X86_REG_EFLAGS, flags & ~1)

    def image(self):
        """The rebuilt program: from the load segment up to the unpacker's moved copy. An unpacker
        that writes the program front to back has often first moved the compressed data up out
        of the way, and the end of that is still there above the program: it was written before
        the program's first byte was, and is left off. (Front to back: the first bytes were
        written before the middle ones.)"""
        if self.found is self.first:                          # an unpacker that did not move
            end, want = _TOP * 16, 1
            if self.moved:                                    # ... or a program that only moved: all of it
                end = max(self.base + self.size, max((i for i in range(self.base, end) if self.gen[i]), default=0) + 1)
                return bytes(self.uc.mem_read(self.base, end - self.base))
        else:
            end, want = min(self.stub_low, _TOP * 16), 2
        gen, when = self.gen, self.when
        while end > self.base and gen[end - 1] < want:
            end -= 1
        middle = self.base + (end - self.base) // 2
        head = [when[i] for i in range(self.base, min(self.base + 16, end)) if gen[i] >= want]
        body = [when[i] for i in range(middle, min(middle + 16, end)) if gen[i] >= want]
        since = min(head) if len(head) == 16 and body and min(head) < min(body) else 0
        while end > self.base and (gen[end - 1] < want or when[end - 1] < since):
            end -= 1
        return bytes(self.uc.mem_read(self.base, end - self.base))


def unpacked(module, relocations, cs, ip, ss, sp, min_alloc=0, limit=60_000_000, moved=False):
    """Run the unpacker of a packed load module to the real entry point. With `moved`, run a
    program that only moves itself (it is not compressed) until it continues in the moved copy.
    -> (load module with no load segment applied, [relocation offsets], cs, ip, ss, sp), the
    segments relative to the load segment. RuntimeError when no real entry point is reached or
    the two runs do not agree."""
    runs = []
    for load in (0x1000, 0x1731):
        r = _Run(module, relocations, cs, ip, ss, sp, load, limit, moved)
        if r.found is None:
            raise RuntimeError("no entry point reached (%s)" % (r.error or ("the unpacker ended the program"
                                                                           if r.exited else "instruction limit")))
        runs.append(r)
    a, b = runs
    one, two = a.image(), b.image()
    if len(one) != len(two):
        raise RuntimeError("two runs rebuilt %d and %d bytes" % (len(one), len(two)))
    delta = (b.load - a.load) & 0xFFFF
    img, out, i, n, stray = bytearray(one), [], 0, len(one), 0
    while i < n:
        if one[i] == two[i]:
            i += 1
            continue
        w1 = struct.unpack_from("<H", one, i)[0] if i + 2 <= n else -1
        w2 = struct.unpack_from("<H", two, i)[0] if i + 2 <= n else -1
        if w1 >= 0 and (w2 - w1) & 0xFFFF == delta:
            out.append(i)
            struct.pack_into("<H", img, i, (w1 - a.load) & 0xFFFF)
            i += 2
        else:                                                # (the low bytes of a relocated word always differ)
            stray += 1
            i += 1
    if stray > 16:
        raise RuntimeError("%d bytes differ between two runs beyond relocations" % stray)
    fa, fb = a.found, b.found
    rel = [(f["cs"] - r.load) & 0xFFFF for f, r in ((fa, a), (fb, b))]
    if rel[0] != rel[1] or fa["ip"] != fb["ip"]:
        raise RuntimeError("two runs reached different entry points")
    return bytes(img), sorted(set(out)), rel[0], fa["ip"], (fa["ss"] - a.load) & 0xFFFF, fa["sp"]
