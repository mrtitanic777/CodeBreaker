"""powerpc.py -- run a PowerPC function and record what it did.

    from codebreaker.emulate import powerpc
    r = powerpc.run(regions, entry, args, externs={address: name}, span=(lo, hi))
    r["exit"]        "ret" | "exception:<why>" | "none" (the step limit was reached)
    r["ret"]         r3 on return
    r["calls"]       [(name, [r3..r10])]: calls that left the code given, in order
    r["writes"]      {address: (byte before, byte after)} outside the stack
    r["path"]        addresses inside `span` that ran
    r["saved"]       whether r14-r31 and the stack pointer came back as they went in

The emulator library used for x86 cannot run 64-bit PowerPC code (its one selectable 64-bit
processor model faults on the first instruction fetch), so this is an interpreter of our own:
the 64-bit PowerPC user instruction set with integer, branch, condition-register and
floating-point instructions, as compilers for 32-bit-address PowerPC targets use it -- 64-bit
registers and arithmetic, effective addresses cut to 32 bits, big-endian memory.

What it does not do, and says so: vector instructions (the run ends "exception:unsupported"),
overflow recording (XER[OV] is never set), floating-point status and rounding modes other than
round-to-nearest, and anything privileged. tests/test_powerpc.py checks it against functions
compiled from C whose results are known.

Memory is pages created on first touch (zero-filled), with a limit on how many, so code that
reaches into memory it was not given keeps running the same way on both sides of a comparison.
"""
import math
import struct

M64 = 0xFFFFFFFFFFFFFFFF
M32 = 0xFFFFFFFF
RETURN = 0x7FFF0000                     # where the function returns to
STACK_TOP = 0x70100000                  # stack: the megabyte below this
STACK_LOW = 0x70000000
ARGS_BASE = 0x60000000                  # memory pointer-shaped inputs can point into
PAGE = 0x1000
MAX_PAGES = 2048
MAX_STEPS = 20000
MAX_CALLS = 400


class Stop(Exception):
    pass


def _s64(v):
    return v - (1 << 64) if v & (1 << 63) else v


def _s32(v):
    v &= M32
    return v - (1 << 32) if v & 0x80000000 else v


def _sext(v, bits):
    v &= (1 << bits) - 1
    return (v - (1 << bits)) & M64 if v & (1 << (bits - 1)) else v


def _mask(mb, me):
    """Bits mb..me set, bit 0 the most significant of 64 (wrapping when mb > me)."""
    if mb <= me:
        return ((1 << (64 - mb)) - 1) & ~((1 << (63 - me)) - 1) & M64
    return (((1 << (64 - mb)) - 1) | (~((1 << (63 - me)) - 1))) & M64


def _rotl64(v, n):
    n &= 63
    return ((v << n) | (v >> (64 - n))) & M64 if n else v & M64


def _rotl32(v, n):
    v &= M32
    n &= 31
    r = ((v << n) | (v >> (32 - n))) & M32 if n else v
    return r | (r << 32)                                     # the 32-bit result, doubled (as the hardware does)


def _f(bits):
    return struct.unpack(">d", struct.pack(">Q", bits & M64))[0]


def _b(x):
    try:
        return struct.unpack(">Q", struct.pack(">d", x))[0]
    except (OverflowError, struct.error):
        return 0x7FF0000000000000 if x > 0 else 0xFFF0000000000000


def _single(x):
    """A double rounded to single precision (still held as a double)."""
    try:
        return struct.unpack(">f", struct.pack(">f", x))[0]
    except (OverflowError, struct.error):
        return math.inf if x > 0 else -math.inf


def pages(regions):
    """{page number: bytes} for [(address, bytes)] -- an image prepared once and shared by every
    run of it (a run copies a page only when it writes to it)."""
    out = {}
    for va, data in regions:
        pos = 0
        while pos < len(data):
            a = (va + pos) & M32
            off = a & (PAGE - 1)
            n = min(PAGE - off, len(data) - pos)
            page = out.setdefault(a >> 12, bytearray(PAGE))
            page[off:off + n] = data[pos:pos + n]
            pos += n
    return {k: bytes(v) for k, v in out.items()}


class Machine(object):
    def __init__(self, regions, shared=None):
        self.pages = {}                                      # pages this run has written or created
        self.shared = shared or {}                           # pages it only reads so far
        self.r = [0] * 32
        self.f = [0] * 32                                    # floating registers, as 64 raw bits
        self.lr = self.ctr = self.cr = 0
        self.ca = 0
        self.pc = 0
        self.writes = {}                                     # address -> (first old byte, last new byte)
        self.reserve = None
        for va, data in regions:
            self.load(va, data)

    # -- memory --------------------------------------------------------------------------------
    def load(self, va, data):
        pos = 0
        while pos < len(data):
            a = (va + pos) & M32
            page = self._page(a)
            off = a & (PAGE - 1)
            n = min(PAGE - off, len(data) - pos)
            page[off:off + n] = data[pos:pos + n]
            pos += n

    def _page(self, a, write=True):
        key = a >> 12
        page = self.pages.get(key)
        if page is None:
            base = self.shared.get(key)
            if base is not None and not write:
                return base
            if len(self.pages) >= MAX_PAGES:
                raise Stop("memory")
            page = self.pages[key] = bytearray(base) if base is not None else bytearray(PAGE)
        return page

    def read(self, a, n):
        a &= M32
        off = a & (PAGE - 1)
        if off + n <= PAGE:
            return int.from_bytes(self._page(a, False)[off:off + n], "big")
        return int.from_bytes(bytes(self._page((a + i) & M32, False)[(a + i) & (PAGE - 1)] for i in range(n)), "big")

    def write(self, a, n, v):
        a &= M32
        data = (v & ((1 << (8 * n)) - 1)).to_bytes(n, "big")
        track = not (STACK_LOW <= a < STACK_TOP)
        for i in range(n):
            x = (a + i) & M32
            page = self._page(x)
            off = x & (PAGE - 1)
            if track:
                old = self.writes.get(x)
                self.writes[x] = (page[off] if old is None else old[0], data[i])
            page[off] = data[i]

    # -- condition register -----------------------------------------------------------------------
    def set_cr(self, field, bits):
        shift = 28 - 4 * field
        self.cr = (self.cr & ~(0xF << shift) | (bits << shift)) & M32

    def record(self, v):
        """CR0 from a result, as a signed 64-bit number."""
        s = _s64(v & M64)
        self.set_cr(0, 8 if s < 0 else 4 if s > 0 else 2)

    def compare(self, field, a, b):
        self.set_cr(field, 8 if a < b else 4 if a > b else 2)

    def bit(self, i):
        return (self.cr >> (31 - i)) & 1


def _add(m, a, b, carry=0):
    total = (a & M64) + (b & M64) + carry
    return total & M64, 1 if total >> 64 else 0


def _step(m, w):
    """Execute one instruction word at m.pc; set m.pc for the next."""
    op = w >> 26
    rt, ra, rb = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    r = m.r
    nxt = (m.pc + 4) & M32
    simm = _sext(w & 0xFFFF, 16)
    uimm = w & 0xFFFF

    if op == 14:                                             # addi
        r[rt] = ((r[ra] if ra else 0) + simm) & M64
    elif op == 15:                                           # addis
        r[rt] = ((r[ra] if ra else 0) + _sext(uimm << 16, 32)) & M64
    elif 32 <= op <= 55 or op in (58, 62):
        _memory(m, op, w, rt, ra, simm)
    elif op == 18:                                           # b
        d = _sext(w & 0x03FFFFFC, 26)
        if w & 1:
            m.lr = nxt
        nxt = (d if w & 2 else m.pc + d) & M32
    elif op == 16:                                           # bc
        if _taken(m, rt, ra):
            d = _sext(w & 0xFFFC, 16)
            if w & 1:
                m.lr = nxt
            nxt = (d if w & 2 else m.pc + d) & M32
        elif w & 1:
            m.lr = nxt
    elif op == 19:
        xo = (w >> 1) & 0x3FF
        if xo == 16:                                         # bclr
            target = m.lr
            if _taken(m, rt, ra):
                if w & 1:
                    m.lr = nxt
                nxt = target & 0xFFFFFFFC
            elif w & 1:
                m.lr = nxt
        elif xo == 528:                                      # bcctr
            if _taken(m, rt | 4, ra):
                if w & 1:
                    m.lr = nxt
                nxt = m.ctr & 0xFFFFFFFC
            elif w & 1:
                m.lr = nxt
        elif xo == 0:                                        # mcrf
            m.set_cr(rt >> 2, (m.cr >> (28 - 4 * (ra >> 2))) & 0xF)
        elif xo in _CR_OPS:
            a, b = m.bit(ra), m.bit(rb)
            v = _CR_OPS[xo](a, b) & 1
            m.cr = (m.cr & ~(1 << (31 - rt)) | (v << (31 - rt))) & M32
        elif xo == 150:                                      # isync
            pass
        else:
            raise Stop("unsupported")
    elif op == 31:
        _extended(m, w, rt, ra, rb)
    elif op == 24:
        r[ra] = r[rt] | uimm
    elif op == 25:
        r[ra] = r[rt] | (uimm << 16)
    elif op == 26:
        r[ra] = r[rt] ^ uimm
    elif op == 27:
        r[ra] = r[rt] ^ (uimm << 16)
    elif op == 28:
        r[ra] = r[rt] & uimm
        m.record(r[ra])
    elif op == 29:
        r[ra] = r[rt] & (uimm << 16)
        m.record(r[ra])
    elif op == 11:                                           # cmpi
        if rt & 1:
            m.compare(rt >> 2, _s64(r[ra]), _s64(simm))
        else:
            m.compare(rt >> 2, _s32(r[ra]), _s64(simm))
    elif op == 10:                                           # cmpli
        m.compare(rt >> 2, r[ra] if rt & 1 else r[ra] & M32, uimm)
    elif op == 12 or op == 13:                               # addic, addic.
        r[rt], m.ca = _add(m, r[ra], simm)
        if op == 13:
            m.record(r[rt])
    elif op == 8:                                            # subfic
        r[rt], m.ca = _add(m, ~r[ra] & M64, simm, 1)
    elif op == 7:                                            # mulli
        r[rt] = (_s64(r[ra]) * _s64(simm)) & M64
    elif op == 21:                                           # rlwinm
        mb, me = (w >> 6) & 31, (w >> 1) & 31
        r[ra] = _rotl32(r[rt], rb) & _mask(mb + 32, me + 32)
        if w & 1:
            m.record(r[ra])
    elif op == 20:                                           # rlwimi
        mb, me = (w >> 6) & 31, (w >> 1) & 31
        k = _mask(mb + 32, me + 32)
        r[ra] = (_rotl32(r[rt], rb) & k) | (r[ra] & ~k & M64)
        if w & 1:
            m.record(r[ra])
    elif op == 23:                                           # rlwnm
        mb, me = (w >> 6) & 31, (w >> 1) & 31
        r[ra] = _rotl32(r[rt], r[rb] & 31) & _mask(mb + 32, me + 32)
        if w & 1:
            m.record(r[ra])
    elif op == 30:                                           # 64-bit rotates
        sh = rb | ((w >> 1) & 1) << 5
        mbe = ((w >> 6) & 31) | ((w >> 5) & 1) << 5
        kind = (w >> 2) & 7
        if kind == 0:                                        # rldicl
            v = _rotl64(r[rt], sh) & _mask(mbe, 63)
        elif kind == 1:                                      # rldicr
            v = _rotl64(r[rt], sh) & _mask(0, mbe)
        elif kind == 2:                                      # rldic
            v = _rotl64(r[rt], sh) & _mask(mbe, 63 - sh)
        elif kind == 3:                                      # rldimi
            k = _mask(mbe, 63 - sh)
            v = (_rotl64(r[rt], sh) & k) | (r[ra] & ~k & M64)
        else:
            n = r[rb] & 63
            if (w >> 1) & 0xF == 8:                          # rldcl
                v = _rotl64(r[rt], n) & _mask(mbe, 63)
            elif (w >> 1) & 0xF == 9:                        # rldcr
                v = _rotl64(r[rt], n) & _mask(0, mbe)
            else:
                raise Stop("unsupported")
        r[ra] = v
        if w & 1:
            m.record(v)
    elif op == 59 or op == 63:
        _float(m, op, w, rt, ra, rb)
    elif op == 3 or op == 2:                                 # twi, tdi: a trap is taken only if its test holds
        a, b = (_s32(r[ra]), _s64(simm)) if op == 3 else (_s64(r[ra]), _s64(simm))
        ua, ub = (r[ra] & M32, simm & M32) if op == 3 else (r[ra], simm)
        if (rt & 16 and a < b) or (rt & 8 and a > b) or (rt & 4 and a == b) or (rt & 2 and ua < ub) or \
                (rt & 1 and ua > ub):
            raise Stop("trap")
    elif op == 17:                                           # sc
        raise Stop("syscall")
    else:
        raise Stop("unsupported")
    m.pc = nxt


_CR_OPS = {257: lambda a, b: a & b, 449: lambda a, b: a | b, 193: lambda a, b: a ^ b, 225: lambda a, b: ~(a & b),
           33: lambda a, b: ~(a | b), 289: lambda a, b: ~(a ^ b), 129: lambda a, b: a & ~b, 417: lambda a, b: a | ~b}


def _taken(m, bo, bi):
    """Does a conditional branch with these BO and BI fields go? (decrements CTR when BO says so)"""
    ok = True
    if not bo & 4:
        m.ctr = (m.ctr - 1) & M64
        ok = (m.ctr != 0) != bool(bo & 2)
    if ok and not bo & 16:
        ok = m.bit(bi) == ((bo >> 3) & 1)
    return ok


_LOADS = {32: (4, 0, 0), 33: (4, 0, 1), 34: (1, 0, 0), 35: (1, 0, 1), 40: (2, 0, 0), 41: (2, 0, 1),
          42: (2, 1, 0), 43: (2, 1, 1)}
_STORES = {36: (4, 0), 37: (4, 1), 38: (1, 0), 39: (1, 1), 44: (2, 0), 45: (2, 1)}


def _memory(m, op, w, rt, ra, simm):
    r = m.r
    if op in (58, 62):
        simm = _sext(w & 0xFFFC, 16)
    base = r[ra] if ra else 0
    ea = (base + simm) & M32
    if op in _LOADS:
        size, signed, update = _LOADS[op]
        v = m.read(ea, size)
        r[rt] = _sext(v, 8 * size) if signed else v
        if update:
            r[ra] = (base + simm) & M64
    elif op in _STORES:
        size, update = _STORES[op]
        m.write(ea, size, r[rt])
        if update:
            r[ra] = (base + simm) & M64
    elif op == 58:
        kind = w & 3
        if kind == 2:                                        # lwa
            r[rt] = _sext(m.read(ea, 4), 32)
        else:                                                # ld, ldu
            r[rt] = m.read(ea, 8)
            if kind == 1:
                r[ra] = (base + simm) & M64
    elif op == 62:                                           # std, stdu
        m.write(ea, 8, r[rt])
        if w & 3 == 1:
            r[ra] = (base + simm) & M64
    elif op == 46:                                           # lmw
        for i in range(rt, 32):
            r[i] = m.read(ea + 4 * (i - rt), 4)
    elif op == 47:                                           # stmw
        for i in range(rt, 32):
            m.write(ea + 4 * (i - rt), 4, r[i])
    elif op in (48, 49):                                     # lfs, lfsu
        m.f[rt] = _b(struct.unpack(">f", m.read(ea, 4).to_bytes(4, "big"))[0])
        if op == 49:
            r[ra] = (base + simm) & M64
    elif op in (50, 51):                                     # lfd, lfdu
        m.f[rt] = m.read(ea, 8)
        if op == 51:
            r[ra] = (base + simm) & M64
    elif op in (52, 53):                                     # stfs, stfsu
        m.write(ea, 4, int.from_bytes(_pack_single(_f(m.f[rt])), "big"))
        if op == 53:
            r[ra] = (base + simm) & M64
    elif op in (54, 55):                                     # stfd, stfdu
        m.write(ea, 8, m.f[rt])
        if op == 55:
            r[ra] = (base + simm) & M64
    else:
        raise Stop("unsupported")


def _pack_single(x):
    try:
        return struct.pack(">f", x)
    except (OverflowError, struct.error):
        return struct.pack(">f", math.inf if x > 0 else -math.inf)


# indexed loads and stores of opcode 31: xo -> (size, signed, update, byte-reversed)
_XLOAD = {23: (4, 0, 0, 0), 55: (4, 0, 1, 0), 87: (1, 0, 0, 0), 119: (1, 0, 1, 0), 279: (2, 0, 0, 0),
          311: (2, 0, 1, 0), 343: (2, 1, 0, 0), 375: (2, 1, 1, 0), 341: (4, 1, 0, 0), 373: (4, 1, 1, 0),
          21: (8, 0, 0, 0), 53: (8, 0, 1, 0), 534: (4, 0, 0, 1), 790: (2, 0, 0, 1), 20: (4, 0, 0, 0),
          84: (8, 0, 0, 0)}
_XSTORE = {151: (4, 0, 0), 183: (4, 1, 0), 215: (1, 0, 0), 247: (1, 1, 0), 407: (2, 0, 0), 439: (2, 1, 0),
           149: (8, 0, 0), 181: (8, 1, 0), 662: (4, 0, 1), 918: (2, 0, 1)}
_LOGIC = {28: lambda a, b: a & b, 60: lambda a, b: a & ~b, 444: lambda a, b: a | b, 412: lambda a, b: a | ~b,
          316: lambda a, b: a ^ b, 124: lambda a, b: ~(a | b), 476: lambda a, b: ~(a & b), 284: lambda a, b: ~(a ^ b)}


def _extended(m, w, rt, ra, rb):
    r = m.r
    xo = (w >> 1) & 0x3FF
    rc = w & 1
    base = r[ra] if ra else 0
    if xo in _XLOAD:
        size, signed, update, swap = _XLOAD[xo]
        ea = (base + r[rb]) & M32
        v = m.read(ea, size)
        if swap:
            v = int.from_bytes(v.to_bytes(size, "big"), "little")
        r[rt] = _sext(v, 8 * size) if signed else v
        if update:
            r[ra] = (base + r[rb]) & M64
        if xo in (20, 84):                                   # lwarx, ldarx: a reservation
            m.reserve = ea
        return
    if xo in _XSTORE:
        size, update, swap = _XSTORE[xo]
        ea = (base + r[rb]) & M32
        v = r[rt] & ((1 << (8 * size)) - 1)
        if swap:
            v = int.from_bytes(v.to_bytes(size, "big"), "little")
        m.write(ea, size, v)
        if update:
            r[ra] = (base + r[rb]) & M64
        return
    if xo in _LOGIC:
        r[ra] = _LOGIC[xo](r[rt], r[rb]) & M64
        if rc:
            m.record(r[ra])
        return
    if xo == 150 or xo == 214:                               # stwcx., stdcx.: stored while the reservation
        if m.reserve is not None:                            # lwarx/ldarx made holds (one processor: it
            m.write((base + r[rb]) & M32, 4 if xo == 150 else 8, r[rt])   # does, until this)
            m.set_cr(0, 2)
        else:
            m.set_cr(0, 0)
        m.reserve = None
        return
    if xo == 0:                                              # cmp
        if rt & 1:
            m.compare(rt >> 2, _s64(r[ra]), _s64(r[rb]))
        else:
            m.compare(rt >> 2, _s32(r[ra]), _s32(r[rb]))
        return
    if xo == 32:                                             # cmpl
        if rt & 1:
            m.compare(rt >> 2, r[ra], r[rb])
        else:
            m.compare(rt >> 2, r[ra] & M32, r[rb] & M32)
        return
    if xo == 339:                                            # mfspr
        spr = ((w >> 16) & 31) | ((w >> 11) & 31) << 5
        if spr == 8:
            r[rt] = m.lr
        elif spr == 9:
            r[rt] = m.ctr
        elif spr == 1:
            r[rt] = m.ca << 29
        else:
            r[rt] = 0                                        # time base and the like read as zero
        return
    if xo == 467:                                            # mtspr
        spr = ((w >> 16) & 31) | ((w >> 11) & 31) << 5
        if spr == 8:
            m.lr = r[rt] & M32
        elif spr == 9:
            m.ctr = r[rt]
        elif spr == 1:
            m.ca = (r[rt] >> 29) & 1
        return
    if xo == 19:                                             # mfcr
        r[rt] = m.cr
        return
    if xo == 144:                                            # mtcrf
        fxm = (w >> 12) & 0xFF
        for i in range(8):
            if fxm & (0x80 >> i):
                m.set_cr(i, (r[rt] >> (28 - 4 * i)) & 0xF)
        return
    if xo in (371, 83):                                      # mftb, mfmsr
        r[rt] = 0
        return
    if xo in (278, 246, 86, 54, 598, 982, 854):              # cache hints, sync, eieio
        return
    if xo == 1014:                                           # dcbz: the 32-byte block is cleared
        ea = (base + r[rb]) & M32 & ~31
        for i in range(0, 32, 8):
            m.write(ea + i, 8, 0)
        return
    if xo in (535, 567, 599, 631, 663, 695, 727, 759, 983):
        ea = (base + r[rb]) & M32
        if xo in (535, 567):                                 # lfsx, lfsux
            m.f[rt] = _b(struct.unpack(">f", m.read(ea, 4).to_bytes(4, "big"))[0])
        elif xo in (599, 631):                               # lfdx, lfdux
            m.f[rt] = m.read(ea, 8)
        elif xo in (663, 695):                               # stfsx, stfsux
            m.write(ea, 4, int.from_bytes(_pack_single(_f(m.f[rt])), "big"))
        elif xo in (727, 759):                               # stfdx, stfdux
            m.write(ea, 8, m.f[rt])
        else:                                                # stfiwx
            m.write(ea, 4, m.f[rt] & M32)
        if xo in (567, 631, 695, 759):
            r[ra] = (base + r[rb]) & M64
        return
    if xo == 4 or xo == 68:                                  # tw, td
        wide = xo == 68
        a, b = (_s64(r[ra]), _s64(r[rb])) if wide else (_s32(r[ra]), _s32(r[rb]))
        ua, ub = (r[ra], r[rb]) if wide else (r[ra] & M32, r[rb] & M32)
        if (rt & 16 and a < b) or (rt & 8 and a > b) or (rt & 4 and a == b) or (rt & 2 and ua < ub) or \
                (rt & 1 and ua > ub):
            raise Stop("trap")
        return
    # shifts
    if xo == 24:                                             # slw
        n = r[rb] & 63
        v = (r[rt] << n) & M32 if n < 32 else 0
    elif xo == 536:                                          # srw
        n = r[rb] & 63
        v = (r[rt] & M32) >> n if n < 32 else 0
    elif xo == 792 or xo == 824:                             # sraw, srawi
        n = (r[rb] & 63) if xo == 792 else rb
        s = _s32(r[rt])
        if n > 31:
            v = M64 if s < 0 else 0
            m.ca = 1 if s < 0 else 0
        else:
            v = (s >> n) & M64
            m.ca = 1 if s < 0 and (r[rt] & ((1 << n) - 1)) else 0
    elif xo == 27:                                           # sld
        n = r[rb] & 127
        v = (r[rt] << n) & M64 if n < 64 else 0
    elif xo == 539:                                          # srd
        n = r[rb] & 127
        v = r[rt] >> n if n < 64 else 0
    elif xo == 794 or (xo >> 1) == 413:                      # srad, sradi
        n = (r[rb] & 127) if xo == 794 else (rb | ((w >> 1) & 1) << 5)
        s = _s64(r[rt])
        if n > 63:
            v = M64 if s < 0 else 0
            m.ca = 1 if s < 0 else 0
        else:
            v = (s >> n) & M64
            m.ca = 1 if s < 0 and (r[rt] & ((1 << n) - 1)) else 0
    elif xo == 26:                                           # cntlzw
        v = 32 - (r[rt] & M32).bit_length()
    elif xo == 58:                                           # cntlzd
        v = 64 - r[rt].bit_length()
    elif xo == 954:
        v = _sext(r[rt], 8)
    elif xo == 922:
        v = _sext(r[rt], 16)
    elif xo == 986:
        v = _sext(r[rt], 32)
    else:
        _arithmetic(m, w, xo & 0x1FF, rt, ra, rb, rc)
        return
    r[ra] = v
    if rc:
        m.record(v)


def _arithmetic(m, w, xo, rt, ra, rb, rc):
    r = m.r
    a, b = r[ra], r[rb]
    if xo == 266:                                            # add
        v = (a + b) & M64
    elif xo == 40:                                           # subf
        v = (b - a) & M64
    elif xo == 10:                                           # addc
        v, m.ca = _add(m, a, b)
    elif xo == 8:                                            # subfc
        v, m.ca = _add(m, ~a & M64, b, 1)
    elif xo == 138:                                          # adde
        v, m.ca = _add(m, a, b, m.ca)
    elif xo == 136:                                          # subfe
        v, m.ca = _add(m, ~a & M64, b, m.ca)
    elif xo == 202:                                          # addze
        v, m.ca = _add(m, a, 0, m.ca)
    elif xo == 200:                                          # subfze
        v, m.ca = _add(m, ~a & M64, 0, m.ca)
    elif xo == 234:                                          # addme
        v, m.ca = _add(m, a, M64, m.ca)
    elif xo == 232:                                          # subfme
        v, m.ca = _add(m, ~a & M64, M64, m.ca)
    elif xo == 104:                                          # neg
        v = (-a) & M64
    elif xo == 235:                                          # mullw
        v = (_s32(a) * _s32(b)) & M64
    elif xo == 75:                                           # mulhw
        v = ((_s32(a) * _s32(b)) >> 32) & M32
    elif xo == 11:                                           # mulhwu
        v = ((a & M32) * (b & M32)) >> 32
    elif xo == 233:                                          # mulld
        v = (_s64(a) * _s64(b)) & M64
    elif xo == 73:                                           # mulhd
        v = ((_s64(a) * _s64(b)) >> 64) & M64
    elif xo == 9:                                            # mulhdu
        v = (a * b) >> 64
    elif xo == 491:                                          # divw
        x, y = _s32(a), _s32(b)
        v = 0 if y == 0 or (x == -0x80000000 and y == -1) else int(abs(x) // abs(y)) * (1 if (x < 0) == (y < 0) else -1) & M32
    elif xo == 459:                                          # divwu
        v = 0 if b & M32 == 0 else (a & M32) // (b & M32)
    elif xo == 489:                                          # divd
        x, y = _s64(a), _s64(b)
        v = 0 if y == 0 or (x == -(1 << 63) and y == -1) else (abs(x) // abs(y)) * (1 if (x < 0) == (y < 0) else -1) & M64
    elif xo == 457:                                          # divdu
        v = 0 if b == 0 else a // b
    else:
        raise Stop("unsupported")
    r[rt] = v
    if rc:
        m.record(v)


def _float(m, op, w, rt, ra, rb):
    f = m.f
    rc5 = (w >> 6) & 31
    xo5 = (w >> 1) & 31
    xo = (w >> 1) & 0x3FF
    single = op == 59
    a, b, c = _f(f[ra]), _f(f[rb]), _f(f[rc5])
    try:
        if xo5 == 21:
            v = a + b
        elif xo5 == 20:
            v = a - b
        elif xo5 == 25:
            v = a * c
        elif xo5 == 18:
            v = a / b if b else (math.nan if a == 0 or a != a else math.copysign(math.inf, a) * math.copysign(1, b))
        elif xo5 == 29:
            v = a * c + b
        elif xo5 == 28:
            v = a * c - b
        elif xo5 == 31:
            v = -(a * c + b)
        elif xo5 == 30:
            v = -(a * c - b)
        elif xo5 == 22:
            v = math.sqrt(b) if b >= 0 else math.nan
        elif xo5 == 24:
            v = 1.0 / b if b else math.copysign(math.inf, b)
        elif xo5 == 26:
            v = 1.0 / math.sqrt(b) if b > 0 else (math.inf if b == 0 else math.nan)
        elif xo5 == 23 and not single:                       # fsel
            f[rt] = f[rc5] if a >= 0.0 else f[rb]
            return
        elif single:
            raise Stop("unsupported")
        else:
            _float_other(m, w, xo, rt, ra, rb)
            return
    except (OverflowError, ValueError):
        v = math.nan
    f[rt] = _b(_single(v) if single else v)


def _float_other(m, w, xo, rt, ra, rb):
    f = m.f
    b = _f(f[rb])
    if xo == 72:                                             # fmr
        f[rt] = f[rb]
    elif xo == 40:                                           # fneg
        f[rt] = f[rb] ^ (1 << 63)
    elif xo == 264:                                          # fabs
        f[rt] = f[rb] & ~(1 << 63) & M64
    elif xo == 136:                                          # fnabs
        f[rt] = f[rb] | (1 << 63)
    elif xo == 12:                                           # frsp
        f[rt] = _b(_single(b))
    elif xo in (0, 32):                                      # fcmpu, fcmpo
        a = _f(f[ra])
        m.set_cr(rt >> 2, 1 if a != a or b != b else 8 if a < b else 4 if a > b else 2)
    elif xo in (14, 15):                                     # fctiw, fctiwz
        f[rt] = _to_int(b, 32, xo == 15) & M32
    elif xo in (814, 815):                                   # fctid, fctidz
        f[rt] = _to_int(b, 64, xo == 815) & M64
    elif xo == 846:                                          # fcfid
        f[rt] = _b(float(_s64(f[rb])))
    elif xo in (583, 711, 134, 70, 38, 64):                  # status register moves: no status is kept
        if xo == 583:
            f[rt] = 0
    else:
        raise Stop("unsupported")


def _to_int(x, bits, toward_zero):
    lo, hi = -(1 << (bits - 1)), (1 << (bits - 1)) - 1
    if x != x:
        return lo
    if x >= hi:
        return hi
    if x <= lo:
        return lo
    return int(x) if toward_zero else int(round(x))


def filled(kind, size=0x10000):
    """Contents for the memory pointer-shaped inputs point into (ARGS_BASE upward), so that what
    a function reads through a pointer is not always zero:
      "pointers"  every word holds the address 0x100 past itself: any chain of pointers leads on
      "counts"    words 1, 2, 3, ...
      "text"      a run of letters ending in a zero every 24 bytes"""
    if kind == "pointers":
        return b"".join(struct.pack(">I", ARGS_BASE + a + 0x100) for a in range(0, size, 4))
    if kind == "counts":
        return b"".join(struct.pack(">I", a // 4 + 1) for a in range(0, size, 4))
    if kind == "text":
        return bytes(0 if a % 24 == 23 else 0x41 + (a % 24) for a in range(size))
    return b""


_FILLED = {}


def run(regions, entry, args=(), externs=None, span=None, stub_ret=0, steps=MAX_STEPS, fargs=(), shared=None,
        fill=None, branches=False, absent=None):
    """Run the function at `entry`. `regions` is [(address, bytes)] to place in memory; `args`
    go in r3..r10 and `fargs` (floats) in f1..; `externs` {address: name} are functions outside
    what was given: a call to one is recorded with its eight argument registers and returns
    `stub_ret`. `shared` is memory prepared with pages(), used instead of (or under) `regions`;
    `fill` names what the memory behind pointer-shaped inputs holds (see filled()). With
    `branches`, each conditional branch inside `span` is logged in out["branches"] (see _Branches).
    `absent` (low, high): where code the image calls may be missing -- a call that lands on nothing
    there is a call out, recorded as "@<address>" (a partial image calls into the rest of its title)."""
    m = Machine(regions, shared)
    if fill:
        if fill not in _FILLED:
            _FILLED[fill] = pages([(ARGS_BASE, filled(fill))])
        m.shared = {**_FILLED[fill], **m.shared} if m.shared else _FILLED[fill]
    externs = externs or {}
    for i, v in enumerate(args[:8]):
        m.r[3 + i] = v & M64
    for i, v in enumerate(fargs[:13]):
        m.f[1 + i] = _b(float(v))
    for i in range(14, 32):
        m.r[i] = 0xA5A50000 + i                              # values a function must hand back unchanged
    m.r[1] = STACK_TOP - 0x4000
    m.r[13] = 0x7E000000                                     # per-thread data pointer of these targets
    m.r[2] = 0
    m.lr = RETURN
    m.pc = entry & M32
    entry_saved = (list(m.r[14:32]), m.r[1])
    calls, path = [], set()
    lo, hi = span or (0, 0)
    log = _Branches() if branches else None
    last, linked = None, None                                # the last instruction; where a call from it went
    out = {"exit": "none", "truncated": False, "calls": calls}
    count = 0
    try:
        while True:
            pc = m.pc
            if pc == RETURN:
                out["exit"] = "ret"
                break
            name = externs.get(pc)
            if name is not None:
                if len(calls) >= MAX_CALLS:
                    out["truncated"] = True
                    break
                calls.append((name, [m.r[3 + i] for i in range(8)]))
                m.r[3] = stub_ret & M64
                m.pc = m.lr & M32
                continue
            if count >= steps:
                out["truncated"] = True
                break
            count += 1
            if lo <= pc < hi:
                path.add(pc)
            if pc & 3:
                raise Stop("alignment")
            w = m.read(pc, 4)
            if w == 0:
                if absent and absent[0] <= pc < absent[1] and linked == pc and m.lr == (last + 4) & M32:
                    # Called, and nothing is there: code outside what was given (a partial image
                    # calls into the rest of its title). A call out, like one to `externs`.
                    if len(calls) >= MAX_CALLS:
                        out["truncated"] = True
                        break
                    calls.append(("@%08x" % pc, [m.r[3 + i] for i in range(8)]))
                    m.r[3] = stub_ret & M64
                    m.pc = m.lr & M32
                    continue
                raise Stop("invalid")
            last, linked = pc, None
            if log is not None and lo <= pc < hi:
                log.step(m, pc, w)
            else:
                _step(m, w)
            if w & 1 and (w >> 26) in (16, 18, 19):          # a branch that links: where it went
                linked = m.pc
    except Stop as e:
        out["exit"] = "exception:%s" % e
    out.update(ret=m.r[3], fret=m.f[1], steps=count, path=path, pc=m.pc,
               writes={a: v for a, v in m.writes.items() if v[0] != v[1]},
               saved=(list(m.r[14:32]), m.r[1]) == entry_saved, machine=m)
    if log is not None:
        out["branches"] = log.events
    return out


class _Branches(object):
    """The branch log of run(branches=True), for an input generator (verify/guided.py): each
    conditional branch -- (address, taken, compare) -- where compare is what the last compare
    into the condition field it tests compared: ("cmp", size, a, b, where a was loaded from or
    None, where b was), or None when that field was last set some other way. A register's
    "where" is the memory it was last loaded from, if nothing has written it since."""
    CAP = 4096

    def __init__(self):
        self.events, self.field, self.loaded = [], {}, {}

    def step(self, m, pc, w):
        op = w >> 26
        rt, ra, rb = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        xo = (w >> 1) & 0x3FF
        r = m.r
        compare = branch = load = None
        if op in (10, 11) or (op == 31 and xo in (0, 32)):
            size = 8 if rt & 1 else 4
            mask = M64 if size == 8 else M32
            if op == 11:
                b, at_b = _sext(w & 0xFFFF, 16) & mask, None
            elif op == 10:
                b, at_b = w & 0xFFFF, None
            else:
                b, at_b = r[rb] & mask, self._where(rb, size)
            compare = (rt >> 2, ("cmp", size, r[ra] & mask, b, self._where(ra, size), at_b))
        elif (op == 16 or (op == 19 and xo == 16)) and (rt & 0x14) != 0x14:
            branch = None if rt & 16 else ra >> 2                # (the field it tests; None: the counter alone)
        elif op in _LOADS or (op == 58 and w & 3 in (0, 2)):
            size = _LOADS[op][0] if op in _LOADS else (8 if w & 3 == 0 else 4)
            d = _sext(w & (0xFFFC if op == 58 else 0xFFFF), 16)
            load = (rt, ((r[ra] if ra else 0) + d) & M32, size)
        cr = m.cr
        _step(m, w)
        if compare is not None:
            self.field[compare[0]] = compare[1]
        elif cr != m.cr:                                     # a field set by something else
            for f in range(8):
                if ((cr ^ m.cr) >> (28 - 4 * f)) & 0xF:
                    self.field[f] = None
        if op == 16 or (op == 19 and xo == 16):
            if (rt & 0x14) != 0x14 and len(self.events) < self.CAP:
                self.events.append((pc, m.pc != (pc + 4) & M32, self.field.get(branch) if branch is not None else None))
        elif compare is None:
            if not (op in _STORES and not _STORES[op][1]):       # (a plain store writes no register)
                self.loaded.pop(rt, None)
            self.loaded.pop(ra, None)
            if load is not None and load[0] != ra:
                self.loaded[load[0]] = load[1:]

    def _where(self, reg, size):
        at = self.loaded.get(reg)
        return at[0] + (at[1] - size if at[1] > size else 0) if at and at[1] >= size else None
