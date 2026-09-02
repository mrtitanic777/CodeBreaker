"""LZX compressor for XEX 'normal' compression -- a faithful, byte-exact port of
XexTool-RE's lzx_encode.cpp (itself a line-by-line reverse-engineering of the
original xextool binary). Operates on a flat byte "arena" with the original's
exact memory offsets; Python bigints stand in for the C++ pointer arithmetic.

Public: lzx_compress(data, window_size, e8_filesize) -> bytes  (the concatenated
[u16be complen][data] chunk stream; the XEX block/SHA-1 framing is the caller's).
"""
import struct

# ---- static .rdata tables (verbatim from xextool_unpacked.exe) -------------
kExtraBits = bytes([
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12,
    13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 0])
kFooterMask = [
    0, 0, 0, 0, 1, 1, 3, 3, 7, 7, 0xf, 0xf, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x7f, 0xff, 0xff,
    0x1ff, 0x1ff, 0x3ff, 0x3ff, 0x7ff, 0x7ff, 0xfff, 0xfff, 0x1fff, 0x1fff, 0x3fff, 0x3fff,
    0x7fff, 0x7fff, 0xffff, 0xffff, 0x1ffff, 0x1ffff, 0x3ffff, 0x3ffff, 0x7ffff, 0x7ffff,
    0xfffff, 0xfffff, 0x1fffff, 0x1fffff, 0x3fffff, 0x3fffff, 0x7fffff, 0x7fffff, 0x1ffffff]
kDelta = bytes([
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16])
kSplitMag = bytes(
    [0, 1, 2, 2] + [3] * 4 + [4] * 8 + [5] * 16 + [6] * 32 + [7] * 64 + [8] * 128)
kSplitWeight = [0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225, 256, 0]


def split_mag(f):
    return kSplitMag[f] if f < 0x100 else kSplitMag[f >> 8] + 8


def _s32(v):
    v &= 0xFFFFFFFF
    return v - 0x100000000 if v & 0x80000000 else v


def _s16(v):
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def _s8(v):
    v &= 0xFF
    return v - 0x100 if v & 0x80 else v


class LzxEnc:
    def __init__(self):
        self.arena = bytearray()
        self.output = bytearray()
        self.input = b""

    # ---- arena accessors (little-endian, matching x86) --------------------
    def i32(self, o):
        return struct.unpack_from("<i", self.arena, o)[0]

    def u32(self, o):
        return struct.unpack_from("<I", self.arena, o)[0]

    def i16(self, o):
        return struct.unpack_from("<h", self.arena, o)[0]

    def u16(self, o):
        return struct.unpack_from("<H", self.arena, o)[0]

    def i8(self, o):
        return struct.unpack_from("<b", self.arena, o)[0]

    def u8(self, o):
        return self.arena[o]

    def s32(self, o, v):
        struct.pack_into("<I", self.arena, o, v & 0xFFFFFFFF)

    def s16(self, o, v):
        struct.pack_into("<H", self.arena, o, v & 0xFFFF)

    def s8(self, o, v):
        self.arena[o] = v & 0xFF

    def E(self, n):
        return struct.unpack_from("<i", self.arena, n * 4)[0]

    def Eu(self, n):
        return struct.unpack_from("<I", self.arena, n * 4)[0]

    def sE(self, n, v):
        struct.pack_into("<I", self.arena, n * 4, v & 0xFFFFFFFF)

    def P(self, o, idx=0):
        return (self.i32(o) + idx) & 0xFFFFFFFF

    def memset(self, o, val, n):
        self.arena[o:o + n] = bytes([val & 0xFF]) * n

    def memmove(self, dst, src, n):
        self.arena[dst:dst + n] = bytes(self.arena[src:src + n])

    def WIN(self, k):
        return self.arena[(self.E(0) + k) & 0xFFFFFFFF]

    def btL_off(self, k):
        return (self.E(3) + k * 4) & 0xFFFFFFFF

    def btR_off(self, k):
        return (self.E(4) + k * 4) & 0xFFFFFFFF

    def BTL(self, k):
        return self.i32(self.btL_off(k))

    def BTR(self, k):
        return self.i32(self.btR_off(k))

    def sBTL(self, k, v):
        self.s32(self.btL_off(k), v)

    def sBTR(self, k, v):
        self.s32(self.btR_off(k), v)

    # ---- canonical Huffman builder (FUN_00424e80 family) ------------------
    def huff_sift(self, param_1):
        fq = self.u32(0x9a0)
        sVar1 = self.i16(0x9ac + param_1 * 2)
        sVar2 = param_1 * 2
        sVar4 = self.i16(0x252c)
        if sVar4 < sVar2:
            self.s16(0x9ac + param_1 * 2, sVar1)
            return
        while True:
            sVar3 = sVar2
            if (sVar2 < sVar4 and
                    self.u16(fq + self.u16(0x9ae + sVar2 * 2) * 2) <
                    self.u16(fq + self.u16(0x9ac + sVar2 * 2) * 2)):
                sVar3 = sVar2 + 1
            sVar4 = self.i16(0x9ac + sVar3 * 2)
            if self.u16(fq + (sVar1 & 0xFFFF) * 2) <= self.u16(fq + (sVar4 & 0xFFFF) * 2):
                break
            self.s16(0x9ac + param_1 * 2, sVar4)
            sVar4 = self.i16(0x252c)
            sVar2 = sVar3 * 2
            param_1 = sVar3
            if sVar2 > sVar4:
                break
        self.s16(0x9ac + param_1 * 2, sVar1)

    def huff_countdepth(self, node):
        if node < self.i32(0x2528):
            d = _s8(self.u8(0x252e))
            if _s8(self.u8(0x252e)) > 0xf:
                d = 0x10
            self.s16(0x2504 + d * 2, self.i16(0x2504 + d * 2) + 1)
            return
        self.s8(0x252e, self.u8(0x252e) + 1)
        self.huff_countdepth(self.i16(0xf28 + node * 4))
        self.huff_countdepth(self.i16(0xf2a + node * 4))
        self.s8(0x252e, self.u8(0x252e) - 1)

    def huff_lengths(self, rootNode):
        for o in range(0x2504, 0x2521, 4):
            self.s32(o, 0)
        self.s16(0x2524, 0)
        if rootNode < self.i32(0x2528):
            d = _s8(self.u8(0x252e))
            if _s8(self.u8(0x252e)) > 0xf:
                d = 0x10
            self.s16(0x2504 + d * 2, self.i16(0x2504 + d * 2) + 1)
        else:
            self.s8(0x252e, self.u8(0x252e) + 1)
            self.huff_countdepth(self.i16(0xf28 + rootNode * 4))
            self.huff_countdepth(self.i16(0xf2a + rootNode * 4))
            self.s8(0x252e, self.u8(0x252e) - 1)
        bVar3 = 0
        psVar7 = 0x2524
        cVar2 = 0x10
        sVar8 = 0
        psVar6 = psVar7
        while True:
            bVar1 = bVar3 & 0x1f
            cVar2 -= 1
            bVar3 = (bVar3 + 1) & 0xFF
            sVar8 = _s16(sVar8 + (self.i16(psVar6) << bVar1))
            psVar6 -= 2
            if cVar2 == 0:
                break
        while True:
            if sVar8 == 0:
                cVar2 = 0x10
                while True:
                    sVar8 = self.i16(psVar7)
                    while True:
                        sVar8 = _s16(sVar8 - 1)
                        if sVar8 < 0:
                            break
                        lenBase = self.u32(0x9a8)
                        sym = self.u16(self.u32(0x9a4))
                        self.s8(lenBase + sym, cVar2)
                        self.s32(0x9a4, self.i32(0x9a4) + 2)
                    cVar2 -= 1
                    psVar7 -= 2
                    if cVar2 == 0:
                        break
                return
            self.s16(psVar7, self.i16(psVar7) - 1)
            uVar4 = 0xf
            while True:
                if self.i16(0x2504 + uVar4 * 2) != 0:
                    self.s16(0x2504 + uVar4 * 2, self.i16(0x2504 + uVar4 * 2) - 1)
                    self.s16(0x2506 + uVar4 * 2, self.i16(0x2506 + uVar4 * 2) + 2)
                    break
                bVar3 = (uVar4 - 1) & 0xFF
                uVar4 = bVar3
                if bVar3 == 0:
                    break
            sVar8 = _s16(sVar8 - 1)

    def huff_treebuild(self, count, freqOff, codeOff):
        s = self.i16(0x252c) >> 1
        while s > 0:
            self.huff_sift(s)
            s -= 1
        self.s32(0x9a4, codeOff)
        param_1 = count
        sVar5 = count
        while True:
            sVar5 = param_1
            uVar2 = self.u16(0x9ae)
            if _s16(uVar2) < self.i32(0x2528):
                self.s16(self.u32(0x9a4), uVar2)
                self.s32(0x9a4, self.i32(0x9a4) + 2)
            uVar1 = self.u16(0x9ac + self.i16(0x252c) * 2)
            self.s16(0x252c, self.i16(0x252c) - 1)
            self.s16(0x9ae, uVar1)
            self.huff_sift(1)
            sVar3 = self.i16(0x9ae)
            if sVar3 < self.i32(0x2528):
                self.s16(self.u32(0x9a4), sVar3 & 0xFFFF)
                self.s32(0x9a4, self.i32(0x9a4) + 2)
            iVar6 = sVar5
            self.s16(freqOff + (sVar5 & 0xFFFF) * 2,
                     self.u16(freqOff + (sVar3 & 0xFFFF) * 2) + self.u16(freqOff + uVar2 * 2))
            self.s16(0x9ae, sVar5)
            self.huff_sift(1)
            self.s16(0xf28 + iVar6 * 4, uVar2)
            self.s16(0xf2a + iVar6 * 4, sVar3)
            param_1 = _s16(sVar5 + 1)
            if not (1 < self.i16(0x252c)):
                break
        self.s32(0x9a4, codeOff)
        self.huff_lengths(sVar5)

    def huff_codes(self, count, codeOff, lenOff):
        nc = [0] * 18
        iVar4 = 1
        p = 0x2508
        while True:
            sVar1 = _s16((nc[iVar4] + self.i16(p - 2)) * 2)
            sVar3 = _s16((self.i16(p) + sVar1) * 2)
            nc[iVar4 + 1] = sVar1
            s1 = self.i16(p + 2)
            nc[iVar4 + 2] = sVar3
            s2 = self.i16(p + 4)
            s1 = _s16((s1 + sVar3) * 2)
            nc[iVar4 + 3] = s1
            nc[iVar4 + 4] = _s16((s2 + s1) * 2)
            iVar4 += 4
            p += 8
            if not (iVar4 < 0x11):
                break
        for i in range(count):
            length = self.u8(lenOff + i)
            self.s16(codeOff + i * 2, nc[length])
            nc[length] = _s16(nc[length] + 1)

    def huff_build(self, count, freqOff, lenOff, codeOff, doCodes):
        while True:
            self.s32(0x9a8, lenOff)
            sVar2 = 0
            iVar1 = 0
            self.s32(0x2528, count)
            self.s32(0x9a0, freqOff)
            self.s8(0x252e, 0)
            self.s16(0x252c, 0)
            self.s16(0x9ae, 0)
            if count < 1:
                break
            while True:
                self.s8(lenOff + iVar1, 0)
                if self.u16(freqOff + iVar1 * 2) != 0:
                    self.s16(0x252c, self.i16(0x252c) + 1)
                    self.s16(0x9ac + self.i16(0x252c) * 2, sVar2)
                sVar2 += 1
                iVar1 = sVar2
                if not (iVar1 < count):
                    break
            if 1 < self.i16(0x252c):
                self.huff_treebuild(count & 0xFFFF, freqOff, codeOff)
                if doCodes != 0:
                    self.huff_codes(count, codeOff, lenOff)
                return
            if self.i16(0x252c) == 0:
                break
            if self.i16(0x9ae) == 0:
                self.s16(freqOff + 2, 1)
            else:
                self.s16(freqOff + 0, 1)
        self.s16(codeOff + (self.i16(0x9ae) & 0xFFFF) * 2, 0)

    # ---- binary-tree match finder -----------------------------------------
    def bt_insert(self, param_1, param_2):
        iVar6 = self.E(2)
        self.sE(2, param_1)
        if iVar6 <= param_2:
            self.sBTR(param_1, 0)
            self.sBTL(param_1, 0)
            return
        local_14 = self.btL_off(param_1)
        iVar5 = 0
        local_10 = self.btR_off(param_1)
        local_8 = 0
        local_c = 0
        while True:
            iVar1 = iVar5 + iVar6
            iVar2 = self.WIN(iVar1) - self.WIN(iVar5 + param_1)
            iVar3 = iVar5
            if iVar2 == 0:
                iVar4 = (iVar1 - iVar6) + param_1
                while True:
                    iVar3 += 1
                    iVar4 += 1
                    if 0x31 < iVar3:
                        break
                    iVar2 = self.WIN((iVar1 - iVar5) + iVar3) - self.WIN(iVar4)
                    if iVar2 != 0:
                        break
            if iVar2 < 0:
                if local_c < iVar3:
                    if 0x31 < iVar3:
                        self.s32(local_14, self.BTL(iVar6))
                        self.s32(local_10, self.BTR(iVar6))
                        return
                    iVar5 = local_8
                    local_c = iVar3
                    if iVar3 <= local_8:
                        iVar5 = iVar3
                self.s32(local_10, iVar6)
                local_10 = self.btL_off(iVar6)
                iVar6 = self.i32(local_10)
            else:
                if local_8 < iVar3:
                    if 0x31 < iVar3:
                        self.s32(local_14, self.BTL(iVar6))
                        self.s32(local_10, self.BTR(iVar6))
                        return
                    iVar5 = iVar3
                    local_8 = iVar3
                    if local_c <= iVar3:
                        iVar5 = local_c
                self.s32(local_14, iVar6)
                local_14 = self.btR_off(iVar6)
                iVar6 = self.i32(local_14)
            if iVar6 <= param_2:
                self.s32(local_14, 0)
                self.s32(local_10, 0)
                return

    def bt_search(self, param_1):
        local_1c = self.Eu(2)
        uVar1 = ((param_1 - self.Eu(1)) + 4) & 0xFFFFFFFF
        self.sE(2, param_1)
        if local_1c <= uVar1:
            self.sBTR(param_1, 0)
            self.sBTL(param_1, 0)
            return 0
        local_18 = self.btL_off(param_1)
        iVar2 = 0
        iVar6 = 0
        local_14 = self.btR_off(param_1)
        local_10 = 0
        local_c = 0
        toC9F = False
        while True:
            iVar9 = _s32(local_1c) + iVar6
            iVar5 = self.WIN(iVar9) - self.WIN(param_1 + iVar6)
            iVar3 = iVar6
            toC7C = False
            if iVar5 == 0:
                iVar8 = (iVar9 - _s32(local_1c)) + param_1
                while True:
                    iVar3 += 1
                    iVar8 += 1
                    if 0x100 < iVar3:
                        toC7C = True
                        break
                    iVar5 = self.WIN((iVar9 - iVar6) + iVar3) - self.WIN(iVar8)
                    if iVar5 == 0:
                        continue
                    break
            if (not toC7C) and iVar5 < 0:
                if local_c < iVar3:
                    if iVar2 < iVar3:
                        toC7C = True
                    else:
                        iVar6 = local_10
                        local_c = iVar3
                        if iVar3 <= local_10:
                            iVar6 = iVar3
                if not toC7C:
                    self.s32(local_14, local_1c & 0xFFFFFFFF)
                    local_14 = self.btL_off(local_1c)
                    local_1c = self.u32(local_14)
            elif not toC7C:                              # iVar5 >= 0
                if local_10 < iVar3:
                    if iVar2 < iVar3:
                        piVar4 = (iVar2 + 0x14) * 4
                        while True:
                            iVar2 += 1
                            piVar4 += 4
                            self.s32(piVar4, ((param_1 - local_1c) + 2) & 0xFFFFFFFF)
                            if not (iVar2 < iVar3):
                                break
                        if 0x31 < iVar3:
                            self.s32(local_18, self.BTL(local_1c))
                            self.s32(local_14, self.BTR(local_1c))
                            toC9F = True
                    if not toC9F:
                        iVar6 = local_c
                        local_10 = iVar3
                        if iVar3 < local_c:
                            iVar6 = iVar3
                if not toC9F:
                    self.s32(local_18, local_1c & 0xFFFFFFFF)
                    local_18 = self.btR_off(local_1c)
                    local_1c = self.u32(self.btR_off(local_1c)) if False else (self.BTR(local_1c) & 0xFFFFFFFF)
            if toC7C:
                piVar4 = (iVar2 + 0x14) * 4
                while True:
                    iVar2 += 1
                    piVar4 += 4
                    self.s32(piVar4, ((param_1 - local_1c) + 2) & 0xFFFFFFFF)
                    if not (iVar2 < iVar3):
                        break
                if 0x31 < iVar3:
                    self.s32(local_18, self.BTL(local_1c))
                    self.s32(local_14, self.BTR(local_1c))
                    toC9F = True
                else:
                    iVar6 = local_10
                    local_c = iVar3
                    if iVar3 <= local_10:
                        iVar6 = iVar3
                    self.s32(local_14, local_1c & 0xFFFFFFFF)
                    local_14 = self.btL_off(local_1c)
                    local_1c = self.u32(local_14)
            if toC9F:
                break
            if not (uVar1 < local_1c):
                self.s32(local_18, 0)
                self.s32(local_14, 0)
                break
        # repeat-offset (R0/R1/R2) extension
        if iVar2 < 2:
            return 0
        iVar6 = 0
        if 0 < iVar2:
            b = param_1 - self.E(0x11)
            while self.WIN(param_1 + iVar6) == self.WIN(b + iVar6):
                iVar6 += 1
                if not (iVar6 < iVar2):
                    break
        goto_e2e = False
        if 1 < iVar6:
            piVar4 = (iVar6 + 0x14) * 4
            iVar3 = iVar6
            while True:
                self.s32(piVar4, 0)
                iVar3 -= 1
                piVar4 -= 4
                if not (1 < iVar3):
                    break
            if 0x32 < iVar6:
                goto_e2e = True
        if not goto_e2e:
            iVar3 = 0
            if 0 < iVar2:
                b = param_1 - self.E(0x12)
                while self.WIN(param_1 + iVar3) == self.WIN(b + iVar3):
                    iVar3 += 1
                    if not (iVar3 < iVar2):
                        break
            if iVar6 < iVar3:
                piVar4 = (iVar6 + 0x14) * 4
                while True:
                    iVar6 += 1
                    piVar4 += 4
                    self.s32(piVar4, 1)
                    if not (iVar6 < iVar3):
                        break
            iVar3 = 0
            if 0 < iVar2:
                b = param_1 - self.E(0x13)
                while self.WIN(param_1 + iVar3) == self.WIN(b + iVar3):
                    iVar3 += 1
                    if not (iVar3 < iVar2):
                        break
            if iVar6 < iVar3:
                piVar4 = (iVar6 + 0x14) * 4
                while True:
                    iVar6 += 1
                    piVar4 += 4
                    self.s32(piVar4, 2)
                    if not (iVar6 < iVar3):
                        break
        iVar6 = 0x7fff - (param_1 & 0x7fff)
        if iVar2 <= iVar6:
            return iVar2
        if iVar6 < 2:
            return 0
        return iVar6

    def bt_slide_delete(self, pos, thresh):
        puVar5 = 8
        if self.Eu(2) != pos:
            return
        if self.Eu(2) <= thresh:
            self.sE(2, 0)
            self.sBTR(pos, 0)
            self.sBTL(pos, 0)
            return
        uVar4 = self.BTL(pos) & 0xFFFFFFFF
        if uVar4 <= thresh:
            self.sBTL(pos, 0)
            uVar4 = 0
        uVar3 = self.BTR(pos) & 0xFFFFFFFF
        if uVar3 <= thresh:
            self.sBTR(pos, 0)
            uVar3 = 0
        while True:
            while uVar4 <= uVar3:
                if uVar3 <= thresh:
                    uVar3 = 0
                self.s32(puVar5, uVar3)
                if uVar3 == 0:
                    return
                puVar5 = self.btL_off(uVar3)
                uVar3 = self.BTL(uVar3) & 0xFFFFFFFF
            if uVar4 <= thresh:
                uVar4 = 0
            self.s32(puVar5, uVar4)
            if uVar4 == 0:
                break
            puVar5 = self.btR_off(uVar4)
            uVar4 = self.u32(puVar5)

    # ---- bit output -------------------------------------------------------
    def write_bits(self, val, nbits):
        c = _s8(self.i8(0x18) - nbits)
        self.s32(0x14, self.u32(0x14) | ((val << ((self.i8(0x18) - nbits) & 0x1f)) & 0xFFFFFFFF))
        self.s8(0x18, c)
        while c < 0x11:
            if self.u32(0x864) <= self.u32(0x860):
                self.s32(0x1c, 1)
                self.s32(0x860, self.i32(0x85c))
            self.s8(self.P(0x860), self.u8(0x16))
            self.s32(0x860, self.i32(0x860) + 1)
            self.s8(self.P(0x860), self.u8(0x17))
            self.s32(0x860, self.i32(0x860) + 1)
            self.s32(0x14, (self.i32(0x14) << 0x10) & 0xFFFFFFFF)
            self.s8(0x18, self.i8(0x18) + 0x10)
            c = self.i8(0x18)

    def off_slot(self, off):
        if off < 0x400:
            return self.u8(0x45c + off)
        if off < 0x80000:
            return (self.u8(0x45c + (off >> 9)) + 0x12) & 0xFF
        return ((off >> 0x11) + 0x22) & 0xFF

    def delta_sym(self, prevOff, curOff, i):
        return kDelta[16 + self.u8(prevOff + i) - self.u8(curOff + i)]

    def build_slot_table(self):
        self.s8(0x45c, 0)
        self.s8(0x45d, 1)
        self.s8(0x45e, 2)
        self.s8(0x45f, 3)
        size = 2
        pos = 4
        val = 4
        while True:
            self.memset(0x45c + pos, val, size)
            pos += size
            cur = val
            val = (val + 1) & 0xFF
            self.memset(0x45c + pos, val, size)
            pos += size
            val = (cur + 2) & 0xFF
            size *= 2
            if not (pos < 0x400):
                break

    def init_run(self):
        win = self.E(1)
        self.sE(0, self.E(0x10cc) - win)
        self.sE(3, self.E(0x10cd) - win * 4)
        self.sE(0x116, win)
        self.sE(0x21b, win)
        self.sE(4, self.E(0x10ce) - win * 4)
        self.sE(2, 0)
        self.sE(0x11, 1); self.sE(0x12, 1); self.sE(0x13, 1)
        self.sE(0xe, 1); self.sE(0xf, 1); self.sE(0x10, 1)
        self.s8(0x97d, 1)
        self.sE(0x260, 1)
        self.s8(0x18, 0x20)
        self.sE(5, 0); self.sE(7, 0)
        self.memset(0x3960, 0, self.E(0x21c) * 8 + 0x100)
        self.memset(0x41f4, 0, 0xf9)
        self.memset(0x2540, 8, 0x100)
        self.memset(0x2640, 9, self.E(0x21c) * 8)
        self.memset(0x27fd, 6, 0xf9)
        self.s32(0x431e, 0x03030303)
        self.s32(0x4322, 0x03030303)
        uv = self.u8(0x55c) + 0x12
        if uv < self.Eu(0x21c):
            off = 0x2640 + uv * 8
            while True:
                self.s8(off, 100)
                uv += 1
                off += 8
                if not (uv < self.Eu(0x21c)):
                    break
        self.sE(0x264, self.E(0x116)); self.sE(0x263, self.E(0x116))
        self.sE(0x21a, 0); self.sE(0x261, 1)
        self.memset(self.P(0x34), 0, 0x2000)
        self.sE(9, 0); self.sE(10, 0); self.s8(0x87c, 0)
        self.sE(0xe, 1); self.sE(0xf, 1); self.sE(0x10, 1)
        self.sE(0x267, 0); self.sE(0x10cf, 0)
        self.memset(0x28f8, 0, 0xaf0)
        self.memset(0x3c1e, 0, 0x3e4)
        for o in range(0x42ee, 0x430b, 4):
            self.s32(o, 0)
        self.memset(self.P(0x4330), 0, self.E(0x262) + 0x1101 + self.E(1))

    def setup(self, window, e8size):
        if window & 0x7fff:
            window &= 0xffff8000
        if window < 0x8000:
            window = 0x8000
        W = window
        C = window
        slots = 4
        acc = 4
        while True:
            eb = kExtraBits[slots]
            slots += 1
            acc += (1 << eb)
            if not (acc < W):
                break
        off = [0x4350]

        def place(sz):
            off[0] = (off[0] + 15) & ~15
            p = off[0]
            off[0] += sz
            return p
        Lwindow = place(C + 0x1101 + W)
        LbtL = place((C + W) * 4 + 0x4404)
        LbtR = place((C + W) * 4 + 0x4404)
        Llenbuf = place(0x10000)
        Loffbuf = place(0x20000)
        Lflagbuf = place(0x2000)
        Lnodes = place(0x18150)
        Loutbuf = place(0x9800)
        self.ptFreq = place(0x60)
        self.ptLen = place(0x20)
        self.ptCode = place(0x40)
        total = (off[0] + 15) & ~15
        self.arena = bytearray(total)
        self.sE(1, W)
        self.sE(0x262, C)
        self.sE(0x21c, slots)
        self.sE(0x21d, e8size)
        self.sE(0x10cc, Lwindow); self.sE(0x10cd, LbtL); self.sE(0x10ce, LbtR)
        self.sE(0xb, Loffbuf); self.sE(0xc, Llenbuf); self.sE(0xd, Lflagbuf)
        self.sE(0x94f, Lnodes)
        self.sE(0x217, Loutbuf); self.sE(0x218, Loutbuf); self.sE(0x219, Loutbuf + 0x97c0)
        self.build_slot_table()
        for v in range(0x100):
            c = 0
            x = v
            while x:
                c += (x & 1)
                x >>= 1
            self.s8(0x87d + v, c)
        self.init_run()

    # ---- token frequency / tree building ----------------------------------
    def count_tokens(self, offIdx, param_1, param_3):
        iVar5 = 0
        pos = param_1 & 7
        bVar4 = ((1 << pos) | ((1 >> (8 - pos)) if (8 - pos) < 8 else 0)) & 0xFF
        if param_1 < param_3:
            iVar1 = offIdx * 4
            while True:
                if (self.u8((param_1 >> 3) + self.u32(0x34)) & bVar4) == 0:
                    lit = self.u8(self.u32(0x30) + param_1)
                    self.s16(0x28f8 + lit * 2, self.i16(0x28f8 + lit * 2) + 1)
                    iVar5 += 1
                else:
                    uVar2 = self.u32(self.u32(0x2c) + iVar1)
                    lenHdr = self.u8(param_1 + self.u32(0x30))
                    slot = self.off_slot(uVar2)
                    if lenHdr < 7:
                        self.s16(0x2af8 + (lenHdr + slot * 8) * 2,
                                 self.i16(0x2af8 + (lenHdr + slot * 8) * 2) + 1)
                    else:
                        self.s16(slot * 0x10 + 0x2b06, self.i16(slot * 0x10 + 0x2b06) + 1)
                        self.s16(0x3c10 + lenHdr * 2, self.i16(0x3c10 + lenHdr * 2) + 1)
                    iVar5 = lenHdr + 2 + iVar5
                    iVar1 += 4
                param_1 += 1
                bVar4 = ((bVar4 << 1) | (1 if (bVar4 & 0x80) else 0)) & 0xFF
                if not (param_1 < param_3):
                    break
        return iVar5

    def build_codes(self, doMain):
        n = self.E(0x21c) * 8 + 0x100
        self.huff_build(n, 0x28f8, 0x2540, 0x33e8, doMain)
        self.huff_build(0xf9, 0x3c1e, 0x27fd, 0x4002, doMain)
        self.huff_build(8, 0x42ee, 0x431e, 0x430e, 1)

    def fill_default_lengths(self):
        uVar1 = 0
        while True:
            if self.i8(0x2540 + uVar1) == 0:
                self.s8(0x2540 + uVar1, 0xb)
            uVar1 += 1
            if not (uVar1 < 0x100):
                break
        mainN = self.E(0x21c) * 8 + 0x100
        if uVar1 < mainN:
            while True:
                if self.i8(0x2540 + uVar1) == 0:
                    self.s8(0x2540 + uVar1, 0xc)
                uVar1 += 1
                if not (uVar1 < mainN):
                    break
        off = 0x27fd
        iVar3 = 0xf9
        while True:
            if self.i8(off) == 0:
                self.s8(off, 8)
            off += 1
            iVar3 -= 1
            if iVar3 == 0:
                break
        uVar1 = self.u8(0x55c) + 0x12
        if uVar1 < self.Eu(0x21c):
            p = 0x2640 + uVar1 * 8
            while True:
                self.s8(p, 100)
                uVar1 += 1
                p += 8
                if not (uVar1 < self.Eu(0x21c)):
                    break

    def emit_partial(self):
        uVar1 = self.Eu(9)
        if uVar1 != 0:
            if self.E(0x260) == 0:
                self.count_tokens(self.u32(0x2538), self.u32(0x2534), uVar1)
            else:
                self.memset(0x28f8, 0, (self.E(0x21c) + 0x20) * 0x10)
                self.memset(0x3c1e, 0, 0x1f2)
                self.count_tokens(0, 0, uVar1)
                self.sE(0x260, 0)
            self.build_codes(0)
            self.fill_default_lengths()
            self.s32(0x2534, self.E(9))
            self.s32(0x2538, self.E(10))

    def flush_pending(self, pos):
        self.s8(0x97d, 0)
        self.sE(0x260, 1)
        self.build_emit_block()
        self.sE(0x21b, pos)
        if self.Eu(9) < 0x1000:
            self.sE(0x94c, 0x1000)
        else:
            self.sE(0x94c, self.E(9) + 0x1000)

    def block_boundary(self, param_1):     # param_1 is [value]; returns (int, newval)
        uVar1 = self.Eu(1)
        iVar2 = self.E(0x21b)
        iVar3 = param_1[0]
        self.s8(0x97d, 0)
        if ((iVar2 - uVar1) & 0xFFFFFFFF) < (uVar1 & 0xFFFFFFFF):
            uVar1 = (iVar2 - uVar1) & 0xFFFFFFFF
        if _s32(self.E(0) - self.E(0x10cc) + iVar3) < _s32((iVar3 - iVar2) + uVar1):
            return 0
        local_4 = [0]
        self.find_split(local_4, self.Eu(9), self.Eu(10), None)
        self.memset(0x28f8, 0, (self.E(0x21c) + 0x20) * 0x10)
        self.memset(0x3c1e, 0, 0x1f2)
        self.count_tokens(0, 0, local_4[0])
        self.build_codes(0)
        self.fill_default_lengths()
        self.sE(2, 0)
        self.memset(self.P(0x34), 0, 0x2000)
        param_1[0] = iVar2 & 0xFFFFFFFF
        self.sE(0x21a, 0); self.sE(9, 0); self.sE(10, 0)
        self.sE(0x11, 1); self.sE(0x12, 1); self.sE(0x13, 1)
        self.sE(0xe, 1); self.sE(0xf, 1); self.sE(0x10, 1)
        self.sE(0x260, 1)
        self.sE(0x94c, local_4[0])
        return 1

    def build_emit_block(self):
        self.s8(0x97d, 0)
        local_4 = [0]
        local_8 = [0]
        self.find_split(local_4, self.Eu(9), self.Eu(10), local_8)
        self.emit_one_block(local_4[0], local_8[0])
        if local_4[0] == self.Eu(9):
            self.memset(self.P(0x34), 0, 0x2000)
            self.sE(9, 0); self.sE(10, 0)
            self.fill_default_lengths()
            return
        self.memmove(self.P(0x34), self.P(0x34) + (local_4[0] >> 3),
                     ((self.Eu(9) >> 3) - (local_4[0] >> 3)) + 1)
        uVar1 = (self.Eu(9) - local_4[0]) >> 3
        self.memset(uVar1 + 1 + self.P(0x34), 0, 0x1fff - uVar1)
        self.memmove(self.P(0x30), self.P(0x30) + local_4[0], self.Eu(9) - local_4[0])
        self.memmove(self.P(0x2c), self.P(0x2c) + local_8[0] * 4, (self.Eu(10) - local_8[0]) * 4)
        self.sE(9, self.Eu(9) - local_4[0])
        self.sE(10, self.Eu(10) - local_8[0])
        self.fill_default_lengths()

    def emit_one_block(self, param_1, param_2):
        self.memset(0x28f8, 0, (self.E(0x21c) + 0x20) * 0x10)
        self.memset(0x3c1e, 0, 0x1f2)
        uVar1 = self.count_tokens(0, 0, param_1) & 0xFFFFFFFF
        local_4 = self.decide_aligned(param_2) & 0xFFFFFFFF
        self.build_codes(1)
        uVar2 = self.cost_uncompressed()
        if uVar1 <= uVar2 and self.u32(0x98c) <= self.u32(0x990):
            local_4 = 3
        self.write_bits(local_4 & 0xff, 3)
        self.write_bits((uVar1 >> 0x10) & 0xff, 8)
        self.write_bits((uVar1 >> 8) & 0xff, 8)
        self.write_bits(uVar1 & 0xff, 8)
        if local_4 == 1:
            self.emit_pretrees()
            self.emit_tokens_verbatim(param_1)
            self.update_repeats(param_2)
            self.s32(0x990, self.i32(0x990) + uVar1)
            return
        if local_4 == 2:
            self.emit_aligned_tree()
            self.emit_pretrees()
            self.emit_tokens_aligned(param_1)
            self.update_repeats(param_2)
            self.s32(0x990, self.i32(0x990) + uVar1)
            return
        if local_4 == 3:
            self.update_repeats(param_2)
            self.store_uncompressed(self.u32(0x990), uVar1)
        self.s32(0x990, self.i32(0x990) + uVar1)

    def count_aligned(self, offCount):
        off = self.P(0x2c)
        n = offCount
        if n != 0:
            while True:
                uVar2 = self.u32(off)
                off += 4
                if 0xf < uVar2:
                    self.s16(0x42ee + (uVar2 & 7) * 2, self.i16(0x42ee + (uVar2 & 7) * 2) + 1)
                n -= 1
                if n == 0:
                    break

    def decide_aligned(self, param_3):
        self.s16(0x42ee, 0); self.s16(0x42f0, 0)
        self.s32(0x42f2, 0); self.s32(0x42f6, 0); self.s32(0x42fa, 0)
        uVar6 = 0
        uVar4 = 0
        self.count_aligned(param_3)
        off = 0x42ee
        iVar3 = 8
        iVar2 = 0
        while True:
            iVar2 = iVar3
            uVar1 = self.u16(off)
            if uVar4 < uVar1:
                uVar4 = uVar1
            uVar6 += uVar1
            off += 2
            iVar3 = iVar2 - 1
            if iVar3 == 0:
                break
        if uVar4 <= uVar6 // 5:
            iVar2 = 1
        else:
            iVar2 = iVar2 + 1
            if param_3 < 100:
                iVar2 = 1
        return iVar2

    def pretree_encode(self, srcLen, count, prevLen):
        fF, fL, fC = self.ptFreq, self.ptLen, self.ptCode
        self.memset(fF, 0, 0x60)
        saved = self.u8(srcLen + count)
        self.s8(srcLen + count, 0x7b)
        iVar3 = 0
        if 0 < count:
            while True:
                cVar1 = self.i8(srcLen + iVar3)
                iVar7 = 0
                pc = srcLen + iVar3 + 1
                goto_single = False
                goto_next = False
                if self.i8(pc) == cVar1:
                    while True:
                        pc += 1
                        iVar7 += 1
                        if self.i8(pc) != cVar1:
                            break
                    if iVar7 < 4:
                        goto_single = True
                    elif cVar1 == 0:
                        if iVar7 < 0x34:
                            if iVar7 < 0x14:
                                self.s16(fF + 17 * 2, self.i16(fF + 17 * 2) + 1)
                                iVar3 = iVar3 - 1 + iVar7
                                goto_next = True
                        else:
                            iVar7 = 0x33
                        if not goto_next:
                            self.s16(fF + 18 * 2, self.i16(fF + 18 * 2) + 1)
                            iVar3 = iVar3 - 1 + iVar7
                    else:
                        if 5 < iVar7:
                            iVar7 = 5
                        bVar2 = self.delta_sym(prevLen, srcLen, iVar3)
                        self.s16(fF + bVar2 * 2, self.i16(fF + bVar2 * 2) + 1)
                        self.s16(fF + 19 * 2, self.i16(fF + 19 * 2) + 1)
                        iVar3 = iVar3 - 1 + iVar7
                else:
                    goto_single = True
                if goto_single:
                    bVar2 = self.delta_sym(prevLen, srcLen, iVar3)
                    self.s16(fF + bVar2 * 2, self.i16(fF + bVar2 * 2) + 1)
                iVar3 += 1
                if not (iVar3 < count):
                    break
        self.huff_build(0x14, fF, fL, fC, 1)
        for i in range(0x14):
            self.write_bits(self.u8(fL + i), 4)
        iVar3 = 0
        if 0 < count:
            while True:
                b = self.u8(srcLen + iVar3)
                pb = srcLen + iVar3 + 1
                iVar7 = 0
                local_b6 = 0
                goto_single = False
                if self.u8(pb) == b:
                    while True:
                        pb += 1
                        iVar7 += 1
                        if self.u8(pb) != b:
                            break
                    if iVar7 < 4:
                        goto_single = True
                    elif b == 0:
                        if iVar7 < 0x34:
                            if iVar7 < 0x14:
                                local_b6 = 0x11
                        else:
                            iVar7 = 0x33
                        if local_b6 != 0x11:
                            local_b6 = 0x12
                    else:
                        if 5 < iVar7:
                            iVar7 = 5
                        local_b6 = 0x13
                else:
                    goto_single = True
                if goto_single:
                    local_b6 = self.delta_sym(prevLen, srcLen, iVar3) & 0xFF
                lb = local_b6 & 0xFF
                self.write_bits(self.u16(fC + lb * 2), self.u8(fL + lb))
                if local_b6 == 0x11:
                    self.write_bits(iVar7 - 4, 4)
                    iVar3 = iVar3 - 1 + iVar7
                elif local_b6 == 0x12:
                    self.write_bits(iVar7 - 0x14, 5)
                    iVar3 = iVar3 - 1 + iVar7
                elif local_b6 == 0x13:
                    self.write_bits(iVar7 - 4, 1)
                    dm = self.delta_sym(prevLen, srcLen, iVar3)
                    self.write_bits(self.u16(fC + dm * 2), self.u8(fL + dm))
                    iVar3 = iVar3 - 1 + iVar7
                iVar3 += 1
                if not (iVar3 < count):
                    break
        self.s8(srcLen + count, saved)
        self.arena[prevLen:prevLen + count] = bytes(self.arena[srcLen:srcLen + count])

    def emit_pretrees(self):
        self.pretree_encode(0x2540, 0x100, 0x3960)
        self.pretree_encode(0x2640, self.E(0x21c) * 8, 0x3a60)
        self.pretree_encode(0x27fd, 0xf9, 0x41f4)

    def emit_aligned_tree(self):
        self.huff_build(8, 0x42ee, 0x431e, 0x430e, 1)
        for i in range(8):
            self.write_bits(self.u8(0x431e + i), 3)

    def out_chunk(self, srcOff, complen, declen):
        self.output.append((complen >> 8) & 0xFF)
        self.output.append(complen & 0xFF)
        self.output += self.arena[srcOff:srcOff + complen]

    def chunk_flush(self):
        if self.i32(0x868) != 0:
            if _s8(self.i8(0x18)) < 0x20:
                self.write_bits(0, _s8(self.i8(0x18) - 0x10))
            iVar1 = self.i32(0x860) - self.i32(0x85c)
            if 0 < iVar1:
                self.out_chunk(self.P(0x85c), iVar1, self.i32(0x868))
        self.s32(0x868, 0)
        self.s32(0x860, self.i32(0x85c))
        self.s8(0x18, 0x20)
        self.s32(0x14, 0)

    def update_repeats(self, offCount):
        unaff_EDI = offCount
        iVar3 = unaff_EDI - 1
        bVar4 = 0
        done = False
        if -1 < iVar3:
            puVar5 = self.u32(0x2c) + iVar3 * 4
            while True:
                if self.u32(puVar5) < 3:
                    bVar4 = 0
                else:
                    bVar4 += 1
                    if 2 < bVar4:
                        done = True
                        break
                iVar3 -= 1
                puVar5 -= 4
                if not (-1 < iVar3):
                    break
            if 2 < bVar4:
                done = True
        if not done:
            iVar3 = 0
        while iVar3 < unaff_EDI:
            uVar1 = self.u32(self.u32(0x2c) + iVar3 * 4)
            if uVar1 != 0:
                if uVar1 < 3:
                    t = self.i32(0x38 + uVar1 * 4)
                    self.s32(0x38 + uVar1 * 4, self.i32(0x38))
                    self.s32(0x38, t)
                else:
                    self.s32(0x40, self.i32(0x3c))
                    self.s32(0x3c, self.i32(0x38))
                    self.s32(0x38, uVar1 - 2)
            iVar3 += 1

    def store_uncompressed(self, param_1, param_2):
        self.write_bits(0, _s8(self.i8(0x18) - 0x10))
        piVar3 = 0x38
        iVar4 = 3
        while True:
            iVar1 = self.i32(piVar3)
            self.s8(self.P(0x860), iVar1); self.s32(0x860, self.i32(0x860) + 1)
            self.s8(self.P(0x860), iVar1 >> 8); self.s32(0x860, self.i32(0x860) + 1)
            self.s8(self.P(0x860), iVar1 >> 0x10); self.s32(0x860, self.i32(0x860) + 1)
            self.s8(self.P(0x860), iVar1 >> 0x18); self.s32(0x860, self.i32(0x860) + 1)
            piVar3 += 4
            iVar4 -= 1
            if iVar4 == 0:
                break
        uVar2 = param_2 & 1
        if param_2 != 0:
            while True:
                self.s8(self.P(0x860), self.WIN(param_1)); self.s32(0x860, self.i32(0x860) + 1)
                self.s32(0x868, self.i32(0x868) + 1)
                param_1 += 1
                param_2 -= 1
                if self.i32(0x868) == 0x8000:
                    if _s8(self.i8(0x18)) < 0x20:
                        self.write_bits(0, _s8(self.i8(0x18) - 0x10))
                    iVar4b = self.i32(0x860) - self.i32(0x85c)
                    if 0 < iVar4b:
                        self.out_chunk(self.P(0x85c), iVar4b, self.i32(0x868))
                    self.s32(0x868, 0)
                    self.s32(0x860, self.i32(0x85c))
                    self.s8(0x18, 0x20)
                    self.s32(0x14, 0)
                    self.s8(0x87c, 0)
                if param_2 == 0:
                    break
        if uVar2 != 0:
            self.s8(self.P(0x860), 0)
            self.s32(0x860, self.i32(0x860) + 1)
        self.s32(0x14, 0)
        self.s8(0x18, 0x20)

    def cost_uncompressed(self):
        iVar6 = 0
        iVar5 = 0x4b0
        local_8 = 0
        local_c = 0
        pb = 0x2541
        pw = 0x28fa
        local_4 = 0x40
        while True:
            iVar5 += self.u16(pw - 2) * self.u8(pb - 1)
            iVar6 += self.u16(pw) * self.u8(pb)
            local_c += self.u16(pw + 2) * self.u8(pb + 1)
            local_8 += self.u16(pw + 4) * self.u8(pb + 2)
            pw += 8
            pb += 4
            local_4 -= 1
            if local_4 == 0:
                break
        iVar5 = iVar5 + local_8 + local_c + iVar6
        uVar3 = 0
        local_d = 0
        if self.E(0x21c) != 0:
            while True:
                iVar7 = uVar3 * 8
                ib = uVar3 * 8 + 0x100
                uVar4 = kExtraBits[uVar3]
                local_d += 1
                uVar3 = local_d
                iVar5 = ((self.u8(0x2542 + ib) + uVar4) * self.u16(0x28fc + ib * 2) + iVar5
                         + (self.u8(0x2641 + iVar7) + uVar4) * self.u16(0x28fa + ib * 2)
                         + (self.u8(0x2540 + ib) + uVar4) * self.u16(0x28f8 + ib * 2)
                         + (self.u8(0x2544 + ib) + uVar4) * self.u16(0x2900 + ib * 2)
                         + (self.u8(0x2545 + ib) + uVar4) * self.u16(0x2902 + ib * 2)
                         + (self.u8(0x2546 + ib) + uVar4) * self.u16(0x2904 + ib * 2)
                         + (self.u8(0x2547 + ib) + uVar4) * self.u16(0x2906 + ib * 2)
                         + (self.u8(0x2543 + ib) + uVar4) * self.u16(0x28fe + ib * 2))
                if not (uVar3 < self.Eu(0x21c)):
                    break
        iVar7 = 0
        iVar6 = 0
        pb2 = 0x27fe
        pw2 = 0x3c20
        local_4 = 0x53
        while True:
            iVar5 += self.u16(pw2 - 2) * self.u8(pb2 - 1)
            iVar6 += self.u8(pb2) * self.u16(pw2)
            iVar7 += self.u16(pw2 + 2) * self.u8(pb2 + 1)
            pw2 += 6
            pb2 += 3
            local_4 -= 1
            if local_4 == 0:
                break
        return ((iVar5 + iVar7 + iVar6 + 7) >> 3) & 0xFFFFFFFF

    def emit_tokens_verbatim(self, param_1):
        uVar7 = 0
        if param_1 != 0:
            local_8 = 0
            while True:
                if (self.u8((uVar7 >> 3) + self.u32(0x34)) & (1 << (uVar7 & 7))) == 0:
                    uVar5 = self.u8(uVar7 + self.u32(0x30))
                    self.write_bits(self.u16(0x33e8 + uVar5 * 2), self.u8(0x2540 + uVar5))
                    self.s32(0x868, self.i32(0x868) + 1)
                else:
                    bVar2 = self.u8(uVar7 + self.u32(0x30))
                    uVar5 = self.u32(local_8 + self.u32(0x2c))
                    local_8 += 4
                    uVar6 = self.off_slot(uVar5)
                    if bVar2 < 7:
                        self.write_bits(self.u16(0x35e8 + (bVar2 + uVar6 * 8) * 2),
                                        self.u8(bVar2 + 0x2640 + uVar6 * 8))
                    else:
                        self.write_bits(self.u16(uVar6 * 0x10 + 0x35f6), self.u8(0x2647 + uVar6 * 8))
                        self.write_bits(self.u16(0x3ff4 + bVar2 * 2), self.u8(bVar2 + 0x27f6))
                    if kExtraBits[uVar6] != 0:
                        self.write_bits(kFooterMask[uVar6] & uVar5, kExtraBits[uVar6])
                    self.s32(0x868, self.i32(0x868) + bVar2 + 2)
                uVar7 += 1
                if self.i32(0x868) == 0x8000:
                    self.chunk_flush()
                    self.s8(0x87c, 0)
                if not (uVar7 < param_1):
                    break

    def emit_tokens_aligned(self, param_1):
        local_10 = 0
        if param_1 != 0:
            local_4 = 0
            while True:
                if (self.u8((local_10 >> 3) + self.u32(0x34)) & (1 << (local_10 & 7))) == 0:
                    uVar5 = self.u8(self.u32(0x30) + local_10)
                    self.write_bits(self.u16(0x33e8 + uVar5 * 2), self.u8(0x2540 + uVar5))
                    self.s32(0x868, self.i32(0x868) + 1)
                else:
                    bVar3 = self.u8(self.u32(0x30) + local_10)
                    uVar5 = self.u32(local_4 + self.u32(0x2c))
                    local_4 += 4
                    uVar6 = self.off_slot(uVar5)
                    if bVar3 < 7:
                        self.write_bits(self.u16(0x35e8 + (bVar3 + uVar6 * 8) * 2),
                                        self.u8(bVar3 + 0x2640 + uVar6 * 8))
                    else:
                        self.write_bits(self.u16(uVar6 * 0x10 + 0x35f6), self.u8(0x2647 + uVar6 * 8))
                        self.write_bits(self.u16(0x3ff4 + bVar3 * 2), self.u8(bVar3 + 0x27f6))
                    local_8 = bVar3
                    eb = kExtraBits[uVar6]
                    if eb < 3:
                        if eb != 0:
                            self.write_bits(kFooterMask[uVar6] & uVar5, eb)
                    else:
                        if 3 < eb:
                            self.write_bits(((1 << (eb - 3)) - 1) & (uVar5 >> 3), eb - 3)
                        al = uVar5 & 7
                        self.write_bits(self.u16(0x430e + al * 2), self.u8(0x431e + al))
                    self.s32(0x868, self.i32(0x868) + local_8 + 2)
                local_10 += 1
                if self.i32(0x868) == 0x8000:
                    self.chunk_flush()
                    self.s8(0x87c, 0)
                if not (local_10 < param_1):
                    break

    def feed_window(self, destOff):
        sz = self.i32(0x998)
        if 0x7fff < sz:
            self.arena[destOff:destOff + 0x8000] = self.input[self.u32(0x994):self.u32(0x994) + 0x8000]
            self.s32(0x998, self.i32(0x998) - 0x8000)
            self.s32(0x994, self.i32(0x994) + 0x8000)
            return 0x8000
        if sz < 1:
            return 0
        self.arena[destOff:destOff + sz] = self.input[self.u32(0x994):self.u32(0x994) + sz]
        self.s32(0x994, self.i32(0x994) + self.i32(0x998))
        self.s32(0x998, 0)
        return sz

    def feed_chunk(self):
        iVar1 = self.E(0x10cc)
        iVar2 = self.E(0x116)
        iVar3 = self.E(0)
        dest = (iVar1 + (iVar2 - iVar1) + iVar3) & 0xFFFFFFFF
        sVar4 = self.feed_window(dest)
        if -1 < sVar4:
            if self.E(0x21d) != 0 and self.Eu(0x10cf) < 0x8000:
                self.e8_translate(sVar4, dest)
            self.sE(0x10cf, self.E(0x10cf) + 1)
            if 0 < sVar4:
                self.parse(sVar4)

    def e8_translate(self, param_2, param_3):
        if 6 < param_2:
            local_c = self.i32(param_3 + param_2 - 6)
            local_8 = self.u16(param_3 + param_2 - 2)
            for k in range(6):
                self.s8(param_3 + param_2 - 6 + k, 0xe8)
            local_10 = (self.u32(0x99c) - 10 + param_2) & 0xFFFFFFFF
            pcVar5 = param_3
            while True:
                cVar1 = self.i8(pcVar5)
                while cVar1 != _s8(0xe8):
                    self.s32(0x99c, self.i32(0x99c) + 1)
                    pcVar5 += 1
                    cVar1 = self.i8(pcVar5)
                uVar2 = self.u32(0x99c)
                if local_10 <= uVar2:
                    break
                iVar4 = self.i32(pcVar5 + 1)
                uVar6 = (uVar2 + (iVar4 & 0xFFFFFFFF)) & 0xFFFFFFFF
                if _s32(uVar6) > -1:
                    uVar3 = self.u32(0x874)
                    if uVar6 < ((uVar2 + uVar3) & 0xFFFFFFFF):
                        if uVar3 <= uVar6:
                            uVar6 = ((iVar4 & 0xFFFFFFFF) - uVar3) & 0xFFFFFFFF
                        self.s8(pcVar5 + 2, uVar6 >> 8)
                        self.s8(pcVar5 + 1, uVar6)
                        self.s8(pcVar5 + 3, uVar6 >> 0x10)
                        self.s8(pcVar5 + 4, uVar6 >> 0x18)
                pcVar5 += 5
                self.s32(0x99c, self.i32(0x99c) + 5)
            self.s32(param_3 + param_2 - 6, local_c)
            self.s16(param_3 + param_2 - 2, local_8)
            self.s32(0x99c, (local_10 + 10) & 0xFFFFFFFF)
        else:
            self.s32(0x99c, self.i32(0x99c) + param_2)

    def eval_split(self, param_2, param_3, param_4, param_5):
        iVar2 = self.E(0x21c) + 0x20
        local_c88 = iVar2 * 8
        if 799 < local_c88:
            return 0
        h1 = [0] * 800
        h2 = [0] * 800
        c1 = param_2
        c2 = param_3
        o1 = param_4 * 4
        o2 = param_5 * 4
        m1 = (1 << (param_2 & 7)) & 0xFF
        m2 = (1 << (param_3 & 7)) & 0xFF
        n = 0x400
        while True:
            if (self.u8((c1 >> 3) + self.u32(0x34)) & m1) == 0:
                s1 = self.u8(self.u32(0x30) + c1)
            else:
                b = self.u8(self.u32(0x30) + c1)
                off = self.u32(o1 + self.u32(0x2c))
                slot = self.off_slot(off)
                s1 = (b + 0x100 + slot * 8) if b < 7 else (slot * 8 + 0x107)
                o1 += 4
            h1[s1] += 1
            c1 += 1
            m1 = ((m1 << 1) | (1 if (m1 & 0x80) else 0)) & 0xFF
            if (self.u8((c2 >> 3) + self.u32(0x34)) & m2) == 0:
                s2 = self.u8(self.u32(0x30) + c2)
            else:
                b = self.u8(self.u32(0x30) + c2)
                off = self.u32(self.u32(0x2c) + o2)
                slot = self.off_slot(off)
                s2 = (b + 0x100 + slot * 8) if b < 7 else (slot * 8 + 0x107)
                o2 += 4
            h2[s2] += 1
            c2 += 1
            m2 = ((m2 << 1) | (1 if (m2 & 0x80) else 0)) & 0xFF
            n -= 1
            if n == 0:
                break
        edi = 0
        for s in range(local_c88):
            v = kSplitWeight[split_mag(h1[s] & 0xFFFF)] - kSplitWeight[split_mag(h2[s] & 0xFFFF)]
            edi += (-v if v < 0 else v)
        return edi & 0xFFFFFFFF

    def find_split(self, outTok, param_2, param_3, outOff):
        outTok[0] = param_2
        if outOff is not None:
            outOff[0] = param_3
        if param_2 < 0x1800:
            return
        local_829 = self.u8(0x87c)
        if 3 < local_829:
            return
        local_814 = [0] * 1032
        uVar6 = 0
        uVar3 = 0
        if param_2 >> 3 != 0:
            flag = self.u32(0x34)
            j = 0
            while True:
                if (uVar3 & 7) == 0:
                    local_814[j] = uVar6
                    j += 1
                fb = self.u8(flag + uVar3)
                uVar3 += 1
                uVar6 = (uVar6 + self.u8(0x87d + fb)) & 0xFFFF
                if not (uVar3 < (param_2 >> 3)):
                    break
        local_824 = param_2 - 0x1000
        if 0x800 < local_824:
            uVar3 = 0xc00
            local_818 = 0
            while True:
                uVar4 = self.eval_split(uVar3 - 0x400, uVar3, local_814[(uVar3 - 0x400) >> 6], local_814[uVar3 >> 6])
                if (0x578 < uVar4 and
                        (self.eval_split(uVar3 - 0x800, uVar3 + 0x400, local_814[(uVar3 - 0x800) >> 6], local_814[(uVar3 + 0x400) >> 6]) > 0x578) and
                        (self.eval_split(uVar3 - 0xc00, uVar3 + 0x800, local_814[(uVar3 - 0xc00) >> 6], local_814[(uVar3 + 0x800) >> 6]) > 0x578)):
                    uVar4b = uVar3 - 0x200
                    uVar7 = 0
                    if uVar4b < uVar3 + 0x600:
                        uVar8 = uVar3 - 0x600
                        while True:
                            uVar5 = self.eval_split(uVar8, uVar4b, local_814[uVar8 >> 6], local_814[uVar4b >> 6])
                            if uVar7 < uVar5:
                                uVar7 = uVar5
                                local_818 = uVar4b
                            uVar4b += 0x40
                            uVar8 += 0x40
                            if not (uVar4b < uVar3 + 0x600):
                                break
                        if 0x6a3 < uVar7 and 0xfff < local_818:
                            self.s8(0x87c, (local_829 + 1) & 0xFF)
                            outTok[0] = local_818
                            if outOff is not None:
                                outOff[0] = local_814[local_818 >> 6]
                            break
                cont = uVar3 < local_824
                uVar3 += 0x400
                if not cont:
                    break

    def parse(self, param_1):
        NB = self.E(0x94f)
        uVar10 = self.Eu(0x116)
        local_c = (param_1 + uVar10) & 0xFFFFFFFF
        local_30 = uVar10
        if self.E(0x261) == 0:
            local_24 = uVar10 - 0x32
            for _ in range(0x32):
                self.bt_insert(local_24, ((uVar10 - self.Eu(1)) + 4) & 0xFFFFFFFF)
                local_24 += 1
        else:
            self.sE(0x261, 0)
            self.sE(0x94c, 10000)
            if self.E(0x21d) == 0:
                self.write_bits(0, 1)
            else:
                self.write_bits(1, 1)
                self.write_bits(self.u16(0x876), 0x10)
                self.write_bits(self.u16(0x874), 0x10)
        while True:                                        # LAB_520 re-entry target (no prologue re-run)
            to_tail = False
            while True:                                        # LAB_520
                local_24 = (uVar10 + 0x8000) & 0xFFFFFFFF
                while True:                                    # LAB_530
                    if local_c <= uVar10:
                        to_tail = True
                        break
                    uVar8u = local_24 & 0xffff8000
                    if local_c < uVar8u:
                        uVar8u = local_c
                    local_20 = uVar8u
                    uVar2 = self.bt_search(uVar10)
                    is_literal = False
                    if uVar2 < 2:
                        is_literal = True
                    elif uVar8u < ((uVar2 + uVar10) & 0xFFFFFFFF):
                        uVar2 = (uVar8u - uVar10) & 0xFFFFFFFF
                        if _s32(uVar2) < 2:
                            is_literal = True
                    if is_literal:
                        self.s8(self.Eu(0xc) + self.E(9), self.WIN(uVar10))
                        self.sE(9, self.E(9) + 1)
                        local_24 += 1
                        uVar10 += 1
                        local_30 = uVar10
                        if 0xfff7 < self.Eu(9):
                            self.flush_pending(uVar10)
                        continue                               # goto LAB_530
                    if 0x31 < uVar2:
                        local_4 = self.Eu(uVar2 + 0x14)
                        if local_4 == 3 and 0x10 < uVar2:
                            self.bt_insert(uVar10 + 1, ((uVar10 - self.Eu(1)) + 5) & 0xFFFFFFFF)
                        else:
                            for local_1c in range(1, uVar2):
                                self.bt_insert(local_1c + uVar10, ((local_1c - self.Eu(1)) + 4 + uVar10) & 0xFFFFFFFF)
                        pbVar11 = (self.Eu(9) >> 3) + self.E(0xd)
                        uVar10 += uVar2
                        self.s8(pbVar11, self.u8(pbVar11) | (1 << (self.Eu(9) & 7)))
                        self.s8(self.Eu(0xc) + self.E(9), (uVar2 - 2) & 0xFF)
                        self.sE(9, self.E(9) + 1)
                        self.s32(self.Eu(0xb) + self.Eu(10) * 4, local_4)
                        self.sE(10, self.E(10) + 1)
                        if local_4 < 3:
                            if local_4 != 0:
                                t = self.E(0x11)
                                self.sE(0x11, self.E(local_4 + 0x11))
                                self.sE(local_4 + 0x11, t)
                        else:
                            self.sE(0x13, self.E(0x12))
                            self.sE(0x12, self.E(0x11))
                            self.sE(0x11, local_4 - 2)
                        local_30 = uVar10
                        if 0xfff7 < self.Eu(9) or 0x7ff7 < self.Eu(10):
                            self.flush_pending(uVar10)
                        break                                  # goto LAB_520
                    # ---- optimal parse over a short match (2..0x31) ------------
                    local_28 = uVar2 + uVar10
                    local_8 = uVar10 + 0xefd
                    self.s32(NB + 0x2c, self.u8(0x2540 + self.WIN(uVar10)))
                    self.s32(NB + 0x1c, uVar10)
                    uVar7 = 2
                    if 1 < uVar2:
                        iVar6 = 0x30
                        local_2cb = 0x16 * 4
                        while True:
                            uVar8u = self.u32(local_2cb)
                            bVar1 = self.off_slot(uVar8u)
                            if uVar7 < 9:
                                self.s32(iVar6 + 0x14 + NB, self.u8(uVar7 + bVar1 * 8 + 0x263e) + kExtraBits[bVar1])
                            else:
                                self.s32(iVar6 + 0x14 + NB, self.u8(bVar1 * 8 + 0x2647) + self.u8(uVar7 + 0x27f4) + kExtraBits[bVar1])
                            self.s32(iVar6 + 4 + NB, uVar10)
                            self.s32(iVar6 + NB, self.i32(local_2cb))
                            uVar7 += 1
                            iVar6 += 0x18
                            local_2cb += 4
                            if not (uVar7 <= uVar2):
                                break
                    self.s32(NB + 0x14, 0)
                    self.s32(NB + 8, self.E(0x11))
                    self.s32(NB + 0xc, self.E(0x12))
                    self.s32(NB + 0x10, self.E(0x13))
                    local_2c = NB - uVar10 * 0x18
                    local_18 = uVar10 - 1
                    local_1c = (uVar8u - uVar10) & 0xFFFFFFFF
                    uVar8 = uVar10 * 0x18 + 4 + local_2c
                    local_10 = uVar10
                    iVar9 = uVar8 + 0x18
                    entry = "top"
                    result = None
                    while True:                                # optimal-parse inner (multi-entry)
                        if entry == "top":
                            iVar6 = self.i32(uVar8 + 0x18)
                            local_1c = (local_1c - 1) & 0xFFFFFFFF
                            iVar9 = uVar8 + 0x18
                            local_18 += 1
                            uVar2 = uVar10 + 1
                            if iVar6 != local_18:
                                uVar7 = self.u32(uVar8 + 0x14)
                                if uVar7 < 3:
                                    if uVar7 == 0:
                                        iVar4 = local_2c + iVar6 * 0x18
                                        self.sE(0x11, self.i32(local_2c + 8 + iVar6 * 0x18))
                                        self.sE(0x12, self.i32(iVar4 + 0xc))
                                        iVar6 = self.i32(iVar4 + 0x10)
                                    else:
                                        iVar6 = local_2c + iVar6 * 0x18
                                        if uVar7 == 1:
                                            self.sE(0x11, self.i32(iVar6 + 0xc))
                                            self.sE(0x12, self.i32(iVar6 + 8))
                                            iVar6 = self.i32(iVar6 + 0x10)
                                        else:
                                            self.sE(0x11, self.i32(iVar6 + 0x10))
                                            self.sE(0x12, self.i32(iVar6 + 0xc))
                                            iVar6 = self.i32(iVar6 + 8)
                                else:
                                    self.sE(0x11, uVar7 - 2)
                                    iVar6 = local_2c + iVar6 * 0x18
                                    self.sE(0x12, self.i32(iVar6 + 8))
                                    iVar6 = self.i32(iVar6 + 0xc)
                                self.sE(0x13, iVar6)
                            self.s32(uVar8 + 0x1c, self.E(0x11))
                            self.s32(uVar8 + 0x20, self.E(0x12))
                            self.s32(uVar8 + 0x24, self.E(0x13))
                            local_24 = iVar9
                            if local_28 == uVar2:
                                result = "9e0"
                                break
                            local_30 = self.bt_search(uVar2)
                            brk = False
                            if local_30 + uVar2 <= local_20:
                                brk = True
                            else:
                                local_30 = local_1c
                                if 1 < _s32(local_1c):
                                    brk = True
                            if brk:
                                if _s32(local_30) < 0x33:
                                    entry = "7f5"
                                    continue
                                result = "93c"
                                break
                            local_30 = 0
                            entry = "7f5"
                            continue
                        else:                                  # LAB_7f5
                            if local_8 <= local_30 + uVar2:
                                result = "93c"
                                break
                            if (((2 < _s32(local_30)) or (local_30 == 2 and self.Eu(0x16) < 0x800))
                                    and local_28 < local_30 + uVar2):
                                uVar10 = (local_30 - local_10) + uVar2
                                if 0xefb < uVar10:
                                    uVar10 = 0xefc
                                ii6 = (local_28 - local_10) + 1
                                if ii6 <= uVar10:
                                    iVar4b = ii6 * 0x18
                                    cnt = (uVar10 - ii6) + 1
                                    while cnt != 0:
                                        self.s32(NB + 0x14 + iVar4b, 0xffffffff)
                                        iVar4b += 0x18
                                        cnt -= 1
                                local_28 = local_30 + uVar2
                            local_4 = self.u32(uVar8 + 0x28)
                            uVar10 = self.u8(0x2540 + self.WIN(uVar2)) + local_4
                            if uVar10 < self.u32(uVar8 + 0x40):
                                self.s32(uVar8 + 0x40, uVar10)
                                self.s32(uVar8 + 0x30, uVar2)
                            uVar7 = 2
                            uVar8 = iVar9
                            uVar10 = uVar2
                            if 1 < local_30:
                                local_14 = 0x16 * 4
                                puVar3 = local_24 + 0x40
                                while True:
                                    uVar8u = self.u32(local_14)
                                    bVar1 = self.off_slot(uVar8u)
                                    if uVar7 < 9:
                                        uVar5 = self.u8(uVar7 + bVar1 * 8 + 0x263e)
                                    else:
                                        uVar5 = self.u8(bVar1 * 8 + 0x2647) + self.u8(uVar7 + 0x27f4)
                                    uVar5 = uVar5 + kExtraBits[bVar1] + local_4
                                    if uVar5 < self.u32(puVar3):
                                        self.s32(puVar3, uVar5)
                                        self.s32(puVar3 - 16, uVar2)
                                        self.s32(puVar3 - 20, self.i32(local_14))
                                    uVar7 += 1
                                    local_14 += 4
                                    puVar3 += 24
                                    if not (uVar7 <= local_30):
                                        break
                            entry = "top"
                            continue
                    if result == "93c":
                        local_8 = self.Eu(local_30 + 0x14)
                        local_4 = local_30 + uVar2
                        puVar3 = local_2c + local_4 * 0x18
                        self.s32(puVar3, local_8)
                        self.s32(puVar3 + 4, uVar2)
                        if local_8 == 3 and 0x10 < _s32(local_30):
                            self.bt_insert(uVar10 + 2, ((uVar2 - self.Eu(1)) + 5) & 0xFFFFFFFF)
                        else:
                            uVar10 = 1
                            while uVar10 < local_30:
                                self.bt_insert(uVar10 + uVar2, ((uVar10 - self.Eu(1)) + 4 + uVar2) & 0xFFFFFFFF)
                                uVar10 += 1
                        uVar2 = local_4
                        if local_8 < 3:
                            if local_8 != 0:
                                t = self.E(0x11)
                                self.sE(0x11, self.E(local_8 + 0x11))
                                self.sE(local_8 + 0x11, t)
                        else:
                            self.sE(0x13, self.E(0x12))
                            self.sE(0x12, self.E(0x11))
                            self.sE(0x11, local_8 - 2)
                    # LAB_9e0
                    puVar3 = local_2c
                    local_1c = 0
                    uVar10 = self.u32(local_2c + 4 + uVar2 * 0x18)
                    while True:
                        local_30 = uVar10
                        uVar10 = self.u32(local_30 * 0x18 + 4 + local_2c)
                        local_1c += 1
                        self.s32(local_30 * 0x18 + 4 + local_2c, uVar2)
                        uVar2 = local_30
                        if local_30 == local_10:
                            break
                    while 0xfff7 < self.Eu(9) + local_1c or 0x7ff7 < self.Eu(10) + local_1c:
                        self.flush_pending(local_30)
                    while True:
                        puVar3 = puVar3 + 4 + local_30 * 0x18
                        if local_30 + 1 < self.u32(puVar3):
                            pbVar11 = (self.Eu(9) >> 3) + self.E(0xd)
                            self.s8(pbVar11, self.u8(pbVar11) | (1 << (self.Eu(9) & 7)))
                            self.s8(self.Eu(9) + self.Eu(0xc),
                                    ((self.i32(puVar3) & 0xFF) - (local_30 & 0xFF) - 2) & 0xFF)
                            self.sE(9, self.E(9) + 1)
                            self.s32(self.Eu(0xb) + self.Eu(10) * 4,
                                     self.i32(local_2c + self.i32(puVar3) * 0x18))
                            self.sE(10, self.E(10) + 1)
                            local_30 = self.u32(puVar3)
                        else:
                            self.s8(self.Eu(9) + self.Eu(0xc), self.WIN(local_30))
                            self.sE(9, self.E(9) + 1)
                            local_30 = local_30 + 1
                        local_1c -= 1
                        puVar3 = local_2c
                        if local_1c == 0:
                            break
                    if self.Eu(0x94c) <= self.Eu(9):
                        self.emit_partial()
                        self.sE(0x94c, self.E(0x94c) + 0x1000)
                    uVar10 = local_30
                    if self.u8(0x97d) != 0 and (0xfdff < self.Eu(9) or 0x7dff < self.Eu(10)):
                        box = [local_30]
                        r = self.block_boundary(box)
                        local_30 = box[0]
                        uVar10 = local_30
                        if r == 0:
                            self.flush_pending(local_30)
                            uVar10 = local_30
                    # fall through -> LAB_520 (reset local_24)
                    break
                if to_tail:
                    break
            # ---- tail -------------------------------------------------------
            self.sE(0x263, (uVar10 - self.Eu(1)) & 0xFFFFFFFF)
            if 0x7fff < param_1:
                ebp = uVar10
                thresh = ((uVar10 - self.Eu(1)) + 0x36) & 0xFFFFFFFF
                ebp -= 1
                for _ in range(0x32):
                    self.bt_slide_delete(ebp, thresh)
                    ebp -= 1
                uVar10 = local_30
                if (self.Eu(0x262) + self.Eu(1)) <= (_s32(self.E(0) - self.E(0x10cc) + local_30) & 0xFFFFFFFF):
                    if self.u8(0x97d) != 0:
                        box = [local_30]
                        r = self.block_boundary(box)
                        local_30 = box[0]
                        uVar10 = local_30
                        if r != 0:
                            continue                               # goto LAB_520 (re-enter, NO prologue)
                    uVar10 = local_30
                    self.memmove(self.P(0x4330), (self.E(0x262) + self.E(0x10cc)) & 0xFFFFFFFF, self.E(1))
                    self.memmove(self.P(0x4334), (self.E(0x10cd) + self.E(0x262) * 4) & 0xFFFFFFFF, self.E(1) * 4)
                    self.memmove(self.P(0x4338), (self.E(0x10ce) + self.E(0x262) * 4) & 0xFFFFFFFF, self.E(1) * 4)
                    self.sE(0x263, (uVar10 - self.Eu(1)) & 0xFFFFFFFF)
                    iv = self.E(0x262)
                    self.sE(0, self.E(0) - iv)
                    self.sE(3, self.E(3) + iv * -4)
                    self.sE(4, self.E(4) + iv * -4)
                self.sE(0x116, uVar10)
                return
            if self.u8(0x97d) == 0:
                self.sE(0x116, uVar10)
                return
            box = [local_30]
            r = self.block_boundary(box)
            local_30 = box[0]
            uVar10 = local_30
            if r == 0:
                self.sE(0x116, local_30)
                return
            continue                               # goto LAB_520 (re-enter, NO prologue)


def lzx_compress(data, window_size, e8_filesize):
    enc = LzxEnc()
    enc.setup(window_size, e8_filesize)
    enc.init_run()
    enc.input = bytes(data)
    enc.s32(0x994, 0)
    enc.s32(0x998, len(data))
    while enc.i32(0x998) > 0:
        enc.feed_chunk()
        if enc.i32(0x1c) == 0:
            enc.build_codes(0)
            enc.fill_default_lengths()
    while enc.i32(0x24) != 0:
        enc.build_emit_block()
    enc.chunk_flush()
    return bytes(enc.output)
