#!/usr/bin/env python3
"""oldcomp.py - is a procedure of M86.EXE the kit compiler's code but for the older compiler's habits?

M86.EXE was built by a compiler a little older than the one in the kit (PAS1/PAS2 of November
1981). For the same source the two differ in a few places, each a fixed pattern:

  FOR entry test     older:  mov dx,[lo]  cmp dx,[hi]  ja skip  xchg dx,ax
                     kit:    mov dx,[hi]  cmp dx,[lo]  jb skip  mov ax,[lo]
  a >= b, a > b      older:  mov dx,[a]   cmp dx,[b]   mov cx,0  jb / jbe
  as a value         kit:    mov dx,[b]   cmp dx,[a]   mov cx,0  ja / jae
  upper bound of a   older:  lea di,[bp+d]  push / mov dx, [di+2]
  string parameter   kit:    push / mov dx, [bp+d+2]
  RETURN             older:  lea sp,[bp-n]  pop bp  ret       (the exit, in place)
                     kit:    jmp exit
  a pointer kept     older:  mov [bp-t],di  call ...  mov di,[bp-t]      (and a frame two bytes larger)
  over a call        kit:    call ...  mov di,[bp]  mov di,[di+d]
  a value kept       older:  add dx,[bp+d]  xchg dx,ax                  (DX still holds [bp+a])
  in DX              kit:    mov ax,[bp+a]  add ax,[bp+d]
  a jump out of      older:  jcc target                 (the code is two bytes shorter, still in reach)
  short reach        kit:    jncc +3 ; jmp target

equivalent(want, got, fixups) lays the two procedures side by side byte for byte. Every difference
must be one of these patterns, or the displacement of a relative jump or call; and every such jump
must go to the same place once the two procedures are aligned. Nothing is decoded loosely: a
byte that fits no pattern fails the procedure.
"""
import struct

def equivalent(want, got, fx):
    """-> (True, [what was found]) or (False, [where it stopped])"""
    i = j = 0; n, m = len(want), len(got); amap = [(0, 0)]; jumps = []; notes = []; keep = {}
    def s8(b): return b - 256 if b > 127 else b
    while i < n and j < m:
        if want[i] == got[j] or j in fx: i += 1; j += 1; continue
        a, b = i - 2, j - 2
        # FOR entry test
        if a >= 0 and want[a:a + 2] == b"\x8b\x56" and want[a + 3:a + 5] == b"\x3b\x56" and want[a + 6] == 0x77 and want[a + 8] == 0x92 \
           and got[b:b + 2] == b"\x8b\x56" and got[b + 3:b + 5] == b"\x3b\x56" and got[b + 6] == 0x72 and got[b + 8:b + 10] == b"\x8b\x46" \
           and want[a + 2] == got[b + 5] == got[b + 10] and want[a + 5] == got[b + 2]:
            jumps.append((a + 8 + s8(want[a + 7]), b + 8 + s8(got[b + 7]))); notes.append("FOR entry test at +%04x" % a)
            i = a + 9; j = b + 11; amap.append((i, j)); continue
        # a >= b / a > b as a value
        if a >= 0 and want[a:a + 2] == b"\x8b\x56" and want[a + 3:a + 5] == b"\x3b\x56" and want[a + 6:a + 9] == b"\xb9\x00\x00" \
           and got[b:b + 2] == b"\x8b\x56" and got[b + 3:b + 5] == b"\x3b\x56" and got[b + 6:b + 9] == b"\xb9\x00\x00" \
           and want[a + 2] == got[b + 5] and want[a + 5] == got[b + 2] and (want[a + 9], got[b + 9]) in ((0x72, 0x77), (0x76, 0x73), (0x77, 0x72), (0x73, 0x76)):
            notes.append("comparison as a value at +%04x" % a); i = a + 10; j = b + 10; continue
        # the upper bound of a string parameter: the older compiler goes through the address of the pair
        if want[i:i + 2] == b"\x8d\x7e" and want[i + 3] in (0xFF, 0x8B) and want[i + 4] in (0x75, 0x55) and want[i + 5] == 2 \
           and got[j] == want[i + 3] and got[j + 1] == want[i + 4] + 1 and s8(got[j + 2]) == s8(want[i + 2]) + 2:
            notes.append("upper bound of a string parameter at +%04x" % i); i += 6; j += 3; amap.append((i, j)); continue
        # RETURN: the older compiler writes the procedure's exit in place, the kit's jumps to it
        r0 = 3 if want[i:i + 2] == b"\x8b\x46" else 0          # a function first loads its result: mov ax,[bp-n]
        if want[i + r0:i + r0 + 2] == b"\x8d\x66" and want[i + r0 + 3] == 0x5D and want[i + r0 + 4] in (0xCA, 0xC2, 0xCB, 0xC3) and got[j] in (0xE9, 0xEB):
            ln = r0 + (7 if want[i + r0 + 4] in (0xCA, 0xC2) else 5); jl = 3 if got[j] == 0xE9 else 2
            tg = j + jl + (struct.unpack_from("<h", got, j + 1)[0] if jl == 3 else s8(got[j + 1]))
            if got[tg:tg + ln] == want[i:i + ln]:
                notes.append("RETURN at +%04x" % i); i += ln; j += jl; amap.append((i, j)); continue
        # a conditional jump the kit compiler had to make long
        if 0x70 <= want[i] <= 0x7F and got[j] == want[i] ^ 1 and got[j + 1] == 3 and got[j + 2] == 0xE9:
            jumps.append((i + 2 + s8(want[i + 1]), j + 5 + struct.unpack_from("<h", got, j + 3)[0])); notes.append("long jump at +%04x" % i)
            i += 2; j += 5; amap.append((i, j)); continue
        # a value still in DX: mov dx,[bp+a]  mov di,[m]  mov [di],dx  and then the older compiler goes on
        # with  add dx,[bp+d]  xchg dx,ax  where the kit's loads it again:  mov ax,[bp+a]  add ax,[bp+d]
        if want[i:i + 2] == b"\x03\x56" and want[i + 3] == 0x92 and got[j:j + 2] == b"\x8b\x46" and got[j + 3:j + 5] == b"\x03\x46" \
           and got[j + 5] == want[i + 2] and i >= 9 and want[i - 9:i - 7] == b"\x8b\x56" and want[i - 7] == got[j + 2] \
           and want[i - 6:i - 4] == b"\x8b\x3e" and want[i - 2:i] == b"\x89\x15":
            notes.append("value kept in DX at +%04x" % i); i += 4; j += 6; amap.append((i, j)); continue
        # a pointer kept over a call: the older compiler saves DI (an outer procedure's variable, loaded by
        # mov di,[bp]  mov di,[di+d]) in a temporary of its own before the call and takes it from there
        # afterwards; the kit's loads the variable again. One temporary more: the frame is two bytes larger.
        if i == 4 and want[0] == got[0] == 0xE8 and (want[4] | want[5] << 8) == (got[4] | got[5] << 8) + 2:
            keep["t"] = (-(want[4] | want[5] << 8)) & 0xFF; i = j = 6; continue
        if "t" in keep and want[i:i + 2] == b"\x89\x7e" and want[i + 2] == keep["t"]:
            k = want.rfind(b"\x8b\x7e\x00\x8b\x7d", max(0, i - 24), i)
            if k >= 0: keep["d"] = want[k + 5]; notes.append("pointer kept over a call at +%04x" % i); i += 3; amap.append((i, j)); continue
        if "d" in keep and a >= 0 and want[a:a + 2] == b"\x8b\x7e" and want[a + 2] == keep["t"] and got[b:b + 6] == b"\x8b\x7e\x00\x8b\x7d" + bytes([keep["d"]]):
            i = a + 3; j = b + 6; amap.append((i, j)); continue
        # ... and the other way round, where the older compiler's longer code put the target out of reach
        if 0x70 <= got[j] <= 0x7F and want[i] == got[j] ^ 1 and want[i + 1] == 3 and want[i + 2] == 0xE9:
            jumps.append((i + 5 + struct.unpack_from("<h", want, i + 3)[0], j + 2 + s8(got[j + 1]))); notes.append("long jump at +%04x" % i)
            i += 5; j += 2; amap.append((i, j)); continue
        # last: the displacement of a short jump (opcode just before), of a near jump or call - the
        # patterns above come first, because a ModRM byte can look like a jump opcode
        if i >= 1 and want[i - 1] == got[j - 1] and (0x70 <= want[i - 1] <= 0x7F or want[i - 1] in (0xEB, 0xE2, 0xE3)):
            jumps.append((i + 1 + s8(want[i]), j + 1 + s8(got[j]))); i += 1; j += 1; continue
        k = 1 if (i >= 1 and want[i - 1] == got[j - 1] and want[i - 1] in (0xE8, 0xE9)) else (2 if (i >= 2 and want[i - 2] == got[j - 2] and want[i - 2] in (0xE8, 0xE9) and want[i - 1] == got[j - 1]) else 0)
        if k:
            a, b = i - k, j - k; tw = a + 3 + struct.unpack_from("<h", want, a + 1)[0]; tg = b + 3 + struct.unpack_from("<h", got, b + 1)[0]
            if want[a] == 0xE9: jumps.append((tw, tg))            # a call's target is compared by name in build.py
            i = a + 3; j = b + 3; continue
        return False, ["no pattern at +%04x (original %s, compiled %s)" % (i, want[i:i + 6].hex(" "), got[j:j + 6].hex(" "))]
    if i != n or j != m: return False, ["lengths do not close: %d of %d, %d of %d" % (i, n, j, m)]
    def mp(o):
        a, b = [p for p in amap if p[0] <= o][-1]; return b + (o - a)
    for tw, tg in jumps:
        if 0 <= tw <= n and mp(tw) != tg: return False, ["a jump to +%04x goes to +%04x in the compiled code, not +%04x" % (tw, tg, mp(tw))]
    return (True, notes) if notes else (False, ["differs only in jump displacements?"])
