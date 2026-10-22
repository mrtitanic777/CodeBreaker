"""switches.py -- the switch tables of an Xbox 360 image, for XenonRecomp.

    from codebreaker.recompile import switches
    tables, report = switches.find(xex_or_image_path)
    switches.write(path, tables)            # XenonRecomp's switch-table file

XenonAnalyse matches fixed instruction sequences, which miss compilers whose instruction order
differs or whose linker interleaves nops. This scanner walks back from every `bctr`, evaluates
CTR symbolically and classifies the table by the expression's shape:

    absolute : ctr = load32(T + (idx << 2))
    byte     : ctr = B + (load8(T + idx) << s)            (s may be 0)
    short    : ctr = B + (load16(T + (idx << 1)) << s)

`idx` is the register the guarding `cmplwi crN, rI, max` + `bgt/ble crN` checks (or, with no
check, the table's length comes from its entries). Every label is checked against the function
bounds .pdata gives. Every executable section is scanned.
"""
import struct



class Image:
    def __init__(self, data, base):
        self.data = data
        self.base = base

    def u32(self, addr):
        return struct.unpack_from('>I', self.data, addr - self.base)[0]

    def u16(self, addr):
        return struct.unpack_from('>H', self.data, addr - self.base)[0]

    def u8(self, addr):
        return self.data[addr - self.base]

    def load(self, size, addr):
        # A computed table address outside the image (a value the scanner could not follow): that jump is
        # unresolved, not the end of the scan.
        if not 0 <= addr - self.base <= len(self.data) - size:
            raise Unresolved(f'load of {size} bytes at {addr:08X} is outside the image')
        return {1: self.u8, 2: self.u16, 4: self.u32}[size](addr)


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def decode(w):
    """Decode only the instructions jump-table sequences use. Returns a tuple or None."""
    op = w >> 26
    rd, ra, rb = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    imm = w & 0xFFFF
    if w == 0x60000000:
        return ('nop',)
    if op == 15:
        return ('addis', rd, ra, sext16(imm))
    if op == 14:
        return ('addi', rd, ra, sext16(imm))
    if op == 24:
        return ('ori', ra, rd, imm)  # dest is rA
    if op == 21:
        sh, mb, me = rb, (w >> 6) & 31, (w >> 1) & 31
        return ('rlwinm', ra, rd, sh, mb, me)
    if op == 10 and not (w >> 21) & 1:
        return ('cmplwi', rd >> 2, ra, imm)
    if op == 11 and not (w >> 21) & 1:
        return ('cmpwi', rd >> 2, ra, sext16(imm))
    if op == 32:
        return ('lwz', rd, ra, sext16(imm))
    if op == 16:
        return ('bc', rd, ra, w & 3)  # BO, BI, AA|LK
    if op == 19 and ((w >> 1) & 0x3FF) == 16:
        return ('bclr', rd, ra)
    if op == 18:
        return ('b',)
    if op == 31:
        xo = (w >> 1) & 0x3FF
        if xo == 23:
            return ('loadx', rd, ra, rb, 4)
        if xo == 279:
            return ('loadx', rd, ra, rb, 2)
        if xo == 87:
            return ('loadx', rd, ra, rb, 1)
        if (xo & 0x1FF) == 266:
            return ('add', rd, ra, rb)
        if xo == 444:
            return ('or', ra, rd, rb)  # mr when rS == rB
        if xo == 467 and (((w >> 16) & 31) | (((w >> 11) & 31) << 5)) == 9:
            return ('mtctr', rd)
    return ('other', w)


X_STORES = {151, 183, 215, 247, 407, 439, 149, 181, 663, 695, 727, 759, 983}  # stwx/stbx/sthx/stdx/stf*x (+u)
X_WRITES_RA = {28, 60, 124, 284, 316, 412, 444, 476, 24, 536, 792, 824, 922, 954, 26, 27, 58, 539, 794}
D_LOADS = {32: False, 33: True, 34: False, 35: True, 40: False, 41: True, 42: False, 43: True}  # value: update form
D_STORES = {36: False, 37: True, 38: False, 39: True, 44: False, 45: True, 52: False, 53: True, 54: False, 55: True}
FP_LOADS = {48: False, 49: True, 50: False, 51: True}


def gpr_writes(w):
    """Set of GPRs an instruction word writes, or None if unknown (treated as clobbering anything)."""
    op = w >> 26
    rd, ra = (w >> 21) & 31, (w >> 16) & 31
    if w == 0x60000000:
        return set()
    if op in (14, 15, 7, 12, 13, 8):  # addi addis mulli addic addic. subfic
        return {rd}
    if op in (24, 25, 26, 27, 28, 29, 20, 21, 23):  # ori oris xori xoris andi. andis. rlwimi rlwinm rlwnm
        return {ra}
    if op in (10, 11, 16, 18, 19):  # cmpli cmpi, branches (write LR/CTR only)
        return set()
    if op in D_LOADS:
        return {rd, ra} if D_LOADS[op] else {rd}
    if op in D_STORES or op in FP_LOADS:
        return {ra} if (D_STORES.get(op) or FP_LOADS.get(op)) else set()
    if op == 58:  # ld/ldu/lwa
        return {rd, ra} if (w & 3) == 1 else {rd}
    if op == 62:  # std/stdu
        return {ra} if (w & 3) == 1 else set()
    if op in (59, 63, 4):  # FPU / VMX (no GPR destinations in the forms seen here)
        xo = (w >> 1) & 0x3FF
        return None if op == 63 and xo in (583,) else set()  # mffs writes FPR; fine
    if op == 31:
        xo = (w >> 1) & 0x3FF
        if xo in X_STORES:
            return {ra} if xo in (183, 247, 439, 181, 695, 759) else set()
        if xo in (0, 32):  # cmpw cmplw
            return set()
        if xo in X_WRITES_RA:
            return {ra}
        if xo in (467, 144):  # mtspr mtcrf
            return set()
        return {rd}
    return None


def writes(insn, w):
    """Register written by insn (decoded) or 'unknown'/None."""
    kind = insn[0]
    if kind in ('addis', 'addi', 'ori', 'rlwinm', 'loadx', 'add', 'or', 'lwz'):
        return insn[1]
    regs = gpr_writes(w)
    if regs is None:
        return 'unknown'
    return regs


class Unresolved(Exception):
    pass


class Scanner:
    WINDOW = 40

    def __init__(self, image, text_start, text_end):
        self.img = image
        self.text_start, self.text_end = text_start, text_end

    def insn(self, addr):
        return decode(self.img.u32(addr))

    def branch_over(self, a):
        """`ble crN, a+4` at a-4 skipping `b default` at a: returns (crN, default) or None."""
        w = self.img.u32(a)
        if (w >> 26) != 18 or w & 3:  # b, relative, no link
            return None
        prev = self.img.u32(a - 4)
        d = decode(prev)
        if d[0] != 'bc' or d[1] != 4 or d[2] & 3 != 1 or prev & 3:  # ble crN (relative, no link)
            return None
        if (a - 4 + (sext16(prev & 0xFFFC))) & 0xFFFFFFFF != a + 4:
            return None
        off = w & 0x03FFFFFC
        if off & 0x02000000:
            off -= 0x04000000
        return d[2] >> 2, (a + off) & 0xFFFFFFFF

    def find_guard(self, bctr):
        """Locate `cmplwi crN, rI, max` guarded by `bgt/ble crN`. Returns (addr, rI, count, default).

        Two guard shapes occur: `bgt crN, default` falling through to the table code, and
        `ble crN, L; b default; L:` (the conditional branch skips an unconditional one)."""
        cr = None
        default = 0
        i = 0
        while i < self.WINDOW - 1:
            i += 1
            a = bctr - 4 * i
            if a < self.text_start:
                return None
            d = self.insn(a)
            if d[0] == 'b':
                over = self.branch_over(a) if cr is None else None
                if over is None:
                    return None  # otherwise the guard and the bctr must be straight-line code
                cr, default = over
                i += 1  # skip the ble
                continue
            if cr is None and d[0] in ('bc', 'bclr'):
                bo, bi = d[1], d[2]
                if bi & 3 == 1 and bo in (12, 4):  # gt bit, branch-if-true (bgt) / branch-if-false (ble)
                    cr = bi >> 2
                    if d[0] == 'bc' and bo == 12:
                        w = self.img.u32(a)
                        default = sext16(w & 0xFFFC) & 0xFFFFFFFF
                        if not w & 2:  # relative
                            default = (a + default) & 0xFFFFFFFF
                    continue
                return None
            if cr is not None and d[0] == 'cmplwi' and d[1] == cr:
                return a, d[2], d[3] + 1, default
        return None

    def eval_reg(self, reg, addr, guard):
        """Symbolic value of `reg` just before the instruction at addr."""
        g_addr, g_reg = guard[0], guard[1]
        a = addr - 4
        while a >= g_addr - 4 * self.WINDOW:
            if a == g_addr and reg == g_reg:
                return ('idx',)
            word = self.img.u32(a)
            d = decode(word)
            w = writes(d, word)
            if w == 'unknown':
                if a > g_addr:
                    raise Unresolved(f'unknown insn {word:#010x} at {a:#x}')
            elif isinstance(w, int):
                if w == reg:
                    return self.eval_insn(d, a, guard)
            elif reg in w:
                raise Unresolved(f'r{reg} defined by unsupported insn {word:#010x} at {a:#x}')
            a -= 4
        raise Unresolved(f'r{reg} not resolved')

    def eval_insn(self, d, a, guard):
        kind = d[0]
        if kind == 'addis':
            base = ('const', 0) if d[2] == 0 else self.eval_reg(d[2], a, guard)
            return simplify(('add', base, ('const', (d[3] << 16) & 0xFFFFFFFF)))
        if kind == 'addi':
            base = ('const', 0) if d[2] == 0 else self.eval_reg(d[2], a, guard)
            return simplify(('add', base, ('const', d[3] & 0xFFFFFFFF)))
        if kind == 'ori':
            v = self.eval_reg(d[2], a, guard)
            if v[0] == 'const':
                return ('const', v[1] | d[3])
            if d[3] == 0:
                return v
            raise Unresolved('ori on non-const')
        if kind == 'rlwinm':
            _, _, rs, sh, mb, me = d
            if mb == 0 and me == 31 - sh:
                return simplify(('shl', self.eval_reg(rs, a, guard), sh))
            raise Unresolved(f'rlwinm {sh},{mb},{me}')
        if kind == 'loadx':
            _, _, ra, rb, size = d
            base = ('const', 0) if ra == 0 else self.eval_reg(ra, a, guard)
            return ('load', size, simplify(('add', base, self.eval_reg(rb, a, guard))))
        if kind == 'add':
            return simplify(('add', self.eval_reg(d[2], a, guard), self.eval_reg(d[3], a, guard)))
        if kind == 'or' and d[2] == d[3]:
            # `mr rX, rI` before the guard: rX is a copy of the index if rI is unchanged up to the cmplwi.
            if d[2] == guard[1] and a < guard[0] and self.unchanged(guard[1], a + 4, guard[0]):
                return ('idx',)
            return self.eval_reg(d[2], a, guard)
        if kind == 'lwz' and d[2] == 1:
            # Unoptimised (debug) code keeps the switch value in a stack slot and loads it again after the guard:
            # `lwz rG, off(r1); cmplwi rG, max; bgt default; lwz rI, off(r1)`. rI is the index if the guard's
            # register came from the same slot and nothing stored to it (or moved r1) in between.
            slot = self.guard_slot(guard)
            if slot is not None and slot[1] == d[3] and self.slot_unchanged(d[3], slot[0] + 4, a):
                return ('idx',)
        raise Unresolved(f'cannot evaluate {d}')

    def guard_slot(self, guard):
        """(address, offset) of `lwz rG, offset(r1)` loading the guard's register just before the cmplwi."""
        g_addr, g_reg = guard[0], guard[1]
        for i in range(1, 9):
            a = g_addr - 4 * i
            word = self.img.u32(a)
            d = decode(word)
            if d[0] == 'lwz' and d[1] == g_reg:
                return (a, d[3]) if d[2] == 1 else None
            w = writes(d, word)
            if w == 'unknown' or w == g_reg or (isinstance(w, set) and g_reg in w):
                return None
        return None

    def slot_unchanged(self, offset, start, end):
        """No store to offset(r1) and no write to r1 in [start, end)."""
        for a in range(start, end, 4):
            word = self.img.u32(a)
            op = word >> 26
            if op in D_STORES and (word >> 16) & 31 == 1 and sext16(word & 0xFFFF) == offset:
                return False
            w = writes(decode(word), word)
            if w == 'unknown' or w == 1 or (isinstance(w, set) and 1 in w):
                return False
        return True

    def unchanged(self, reg, start, end):
        for a in range(start, end, 4):
            word = self.img.u32(a)
            w = writes(decode(word), word)
            if w == 'unknown' or w == reg or (isinstance(w, set) and reg in w):
                return False
        return True

    def index_reg_live(self, guard, bctr):
        """The index register must hold the same value at the bctr as at the cmplwi."""
        return self.unchanged(guard[1], guard[0] + 4, bctr)

    def analyze_unguarded(self, bctr, extent):
        """Jump table with no bounds check before it (a `switch` whose default the compiler knows is
        unreachable, e.g. a leaf function switching on an argument, or `default: __assume(0)`).

        The index is the register the table load scales (or uses directly for byte tables), taken
        where it is used; it must still hold that value at the bctr. Without a bound, the table runs
        while its entries are word-aligned addresses inside the function `extent` (lo, hi): an entry
        equal to hi is MSVC's slot for the unreachable default. An inline table (data right after the
        bctr) ends exactly where the first case's code begins. Extra trailing entries only add case
        labels that are never taken; a missing one would fall back to an indirect jump and be reported."""
        a = bctr - 4
        load = mt = None
        for _ in range(12):
            d = self.insn(a)
            if d[0] in ('b', 'bc', 'bclr'):
                break
            if d[0] == 'mtctr' and mt is None:
                mt = a
            if d[0] == 'loadx' and mt is not None:
                load = a
                break
            a -= 4
        if load is None:
            return None
        _, _, ra, rb, size = self.insn(load)
        for cand in (rb, ra):
            use, reg = load, cand
            # The scaling `slwi rX, rI, n` (if any) is where the index is used.
            u = load - 4
            while u > load - 4 * 8:
                word = self.img.u32(u)
                d = decode(word)
                if d[0] in ('b', 'bc', 'bclr'):
                    break
                w = writes(d, word)
                if w == cand or (isinstance(w, set) and cand in w) or w == 'unknown':
                    if d[0] == 'rlwinm' and d[4] == 0 and d[5] == 31 - d[3]:
                        use, reg = u, d[2]
                    break
                u -= 4
            guard = (use - 4, reg)
            if not self.unchanged(reg, use, bctr):
                continue
            try:
                shape = table_shape(self.eval_reg(self.insn(mt)[1], mt, guard))
            except Unresolved:
                continue
            lo, hi = extent
            inline = bctr < shape[2] < bctr + 16
            labels = []
            for i in range(4096):
                if inline and shape[2] + shape[1] * i >= min(labels, default=0xFFFFFFFF):
                    break
                v = table_entry(self.img, shape, i)
                if v & 3 or not lo <= v <= hi:
                    break
                labels.append(v)
            if len(labels) < 2:
                return None
            return dict(bctr=bctr, r=reg, kind=shape[0], labels=labels, default=0, guard=None,
                        table=shape[2], inline=inline)
        return None

    def analyze(self, bctr):
        guard = self.find_guard(bctr)
        if guard is None:
            return None
        d = self.insn(bctr - 4)
        # mtctr may not be immediately before bctr in every sequence; search a little.
        mt_addr = bctr - 4
        while self.insn(mt_addr)[0] != 'mtctr':
            mt_addr -= 4
            if mt_addr <= guard[0]:
                raise Unresolved('no mtctr')
        ctr = self.eval_reg(self.insn(mt_addr)[1], mt_addr, guard)
        if not self.index_reg_live(guard, bctr):
            raise Unresolved('index register clobbered before bctr')
        count = guard[2]
        kind, labels = classify(self.img, ctr, count)
        return dict(bctr=bctr, r=guard[1], kind=kind, labels=labels, default=guard[3], guard=guard[0])


def simplify(e):
    if e[0] == 'add':
        a, b = e[1], e[2]
        if a[0] == 'const' and b[0] == 'const':
            return ('const', (a[1] + b[1]) & 0xFFFFFFFF)
        if a[0] == 'add' and a[2][0] == 'const' and b[0] == 'const':
            return simplify(('add', a[1], ('const', (a[2][1] + b[1]) & 0xFFFFFFFF)))
        if b[0] == 'const' and b[1] == 0:
            return a
        if a[0] == 'const' and a[1] == 0:
            return b
        if a[0] != 'const' and b[0] == 'const':
            return ('add', b, a)  # canonical: const first
    if e[0] == 'shl' and e[2] == 0:
        return e[1]
    return e


def split_const(e):
    """Return (const, rest) for const + rest, else (0, e)."""
    if e[0] == 'add' and e[1][0] == 'const':
        return e[1][1], e[2]
    return 0, e


def index_scale(e):
    if e == ('idx',):
        return 1
    if e[0] == 'shl' and e[1] == ('idx',):
        return 1 << e[2]
    return None


def table_shape(ctr):
    """(kind, size, table, base, shift) for a CTR expression that reads a jump table."""
    base, rest = split_const(ctr)
    shift = 0
    if rest[0] == 'shl' and rest[1][0] == 'load':
        shift, rest = rest[2], rest[1]
    if rest[0] != 'load':
        raise Unresolved(f'ctr shape {ctr}')
    size, addr = rest[1], rest[2]
    table, idx = split_const(addr)
    if index_scale(idx) != size:
        raise Unresolved(f'table index scale mismatch {addr} size {size}')
    if size == 4 and (base or shift):
        raise Unresolved('absolute table with offset')
    return {1: 'byte', 2: 'short', 4: 'absolute'}[size], size, table, base, shift


def table_entry(img, shape, i):
    _, size, table, base, shift = shape
    return (base + (img.load(size, table + size * i) << shift)) & 0xFFFFFFFF


def classify(img, ctr, count):
    shape = table_shape(ctr)
    return shape[0], [table_entry(img, shape, i) for i in range(count)]


def indexed_load_before(img, bctr, text_start, window=12):
    """True if an indexed load (lwzx/lhzx/lbzx) precedes the bctr in straight-line code."""
    for i in range(1, window):
        a = bctr - 4 * i
        if a < text_start:
            break
        d = decode(img.u32(a))
        if d[0] in ('b', 'bc', 'bclr'):
            break
        if d[0] == 'loadx':
            return True
    return False


def extent(funcs, addr, text_start, text_end):
    """Bounds of the .pdata function containing addr, else the gap between .pdata functions."""
    import bisect
    fn = containing(funcs, addr)
    if fn:
        return fn
    i = bisect.bisect_right(funcs, (addr, 0xFFFFFFFF))
    lo = funcs[i - 1][1] if i else text_start
    hi = funcs[i][0] if i < len(funcs) else text_end
    return lo, hi


def read_pdata(img, start, size):
    funcs = []
    for off in range(0, size, 8):
        begin, packed = struct.unpack_from('>II', img.data, start - img.base + off)
        if begin == 0:
            continue
        length = ((packed >> 8) & 0x3FFFFF) * 4
        funcs.append((begin, begin + length))
    funcs.sort()
    return funcs


def containing(funcs, addr):
    import bisect
    i = bisect.bisect_right(funcs, (addr, 0xFFFFFFFF)) - 1
    if i >= 0 and funcs[i][0] <= addr < funcs[i][1]:
        return funcs[i]
    return None


def find(path):
    """-> ([table], {counts}) for an image or .xex. A table is a dict: bctr, r (the index
    register), default, labels, kind, table, guard, inline, func."""
    import os
    from codebreaker.formats import powerpc_image, xex
    image = powerpc_image.read(xex.basefile(path) if xex.is_xex(path) else path)
    lo, hi = image.code
    data = image.bytes_at(image.base, hi - image.base)
    img = Image(data, image.base)
    funcs = sorted((b, b + n) for b, (n, _p, _h) in image.extents.items())
    sc = Scanner(img, lo, hi)
    found, failed, outside, unguarded = [], [], [], []
    for a in range(lo, hi, 4):
        if img.u32(a) != 0x4E800420:
            continue
        try:
            t = sc.analyze(a)
        except Unresolved as e:
            failed.append((a, str(e)))
            continue
        if t is None:
            t = sc.analyze_unguarded(a, extent(funcs, a, lo, hi))
            if t is None:
                if indexed_load_before(img, a, lo):
                    unguarded.append(a)
                continue
        fn = containing(funcs, a)
        if fn and any(not (fn[0] <= l <= fn[1]) for l in t['labels']):  # fn[1]: unreachable-default slot
            outside.append((a, fn))
        t['func'] = fn
        found.append(t)
    kinds = {}
    for t in found:
        kinds[t['kind']] = kinds.get(t['kind'], 0) + 1
    return found, {"tables": len(found), "kinds": kinds, "unresolved": failed, "outside": outside,
                   "pointer_calls": unguarded}




def write(path, found):
    """XenonRecomp's switch-table file."""
    lines = ['# Switch tables, found by CodeBreaker (codebreaker/recompile/switches.py)', '']
    for t in found:
        if t['guard'] is None:
            where = 'inline after the bctr' if t['inline'] else f"at 0x{t['table']:X}"
            lines.append(f"# {t['kind']} table {where}, no bounds check (length from its entries)")
        else:
            lines.append(f"# {t['kind']} table, guard at 0x{t['guard']:X}")
        lines += ['[[switch]]', f"base = 0x{t['bctr']:X}", f"r = {t['r']}", f"default = 0x{t['default']:X}",
                  'labels = [']
        lines += [f'    0x{l:X},' for l in t['labels']]
        lines += [']', '']
    with open(path, 'w', newline='\n') as f:
        f.write('\n'.join(lines) + '\n')
