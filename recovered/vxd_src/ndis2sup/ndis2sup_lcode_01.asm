; ndis2sup_lcode_01.asm -- NDIS2SUP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   0
        push   12h
        call   sub_1_3414
        add    esp, 8
        db  0A3h, 084h, 037h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0028
        mov    word ptr [eax], 0
        db  0A1h, 084h, 037h, 000h, 000h
        mov    word ptr [eax + 2], 0
        sub    eax, eax
        ret
loc_1_0028:
        mov    eax, 0c0000001h
        ret
        db  08Bh, 0FFh, 06Ah, 000h, 0A1h, 084h, 037h, 000h, 000h, 050h, 0E8h, 019h, 034h, 000h, 000h, 083h
        db  0C4h, 008h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_1_0050:
        push   esi
        push   edi
        db  0A1h, 084h, 037h, 000h, 000h
        cmp    word ptr [eax], 0
        jne    SHORT loc_1_00A4
        lea    ecx, [ecx]
loc_1_0060:
        db  08Bh, 035h, 084h, 037h, 000h, 000h
        mov    eax, esi
        push   0
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        lea    eax, [eax*2 + 0ah]
        lea    eax, [eax + eax*8]
        push   eax
        call   sub_1_3414
        add    esp, 8
        db  03Bh, 035h, 084h, 037h, 000h, 000h
        je     SHORT loc_1_00D9
        push   0
        push   eax
        call   sub_1_3456
        db  0A1h, 084h, 037h, 000h, 000h
        add    esp, 8
        cmp    word ptr [eax], 0
        je     SHORT loc_1_0060
loc_1_00A4:
        db  0A1h, 084h, 037h, 000h, 000h
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        lea    ecx, [eax*2]
        pop    edi
        lea    esi, [ecx + ecx*8]
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        mov    dx, word ptr [esi + ecx]
        mov    word ptr [ecx], dx
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        mov    dword ptr [esi + ecx + 0eh], 0
        pop    esi
        ret
loc_1_00D9:
        or     eax, eax
        jne    SHORT loc_1_00E2
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_00E2:
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        xor    edx, edx
        mov    dx, word ptr [ecx + 2]
        lea    edx, [edx*2 + 2]
        mov    edi, eax
        lea    edx, [edx + edx*8]
        mov    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  08Bh, 015h, 084h, 037h, 000h, 000h
        mov    cx, word ptr [eax + 2]
        inc    cx
        mov    word ptr [eax], cx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        lea    esi, [ecx + 1]
        add    ecx, 4
        cmp    ecx, esi
        jbe    SHORT loc_1_014D
        lea    ecx, [esi*2]
        lea    ecx, [ecx + ecx*8]
        lea    edi, [eax + ecx]
loc_1_0136:
        lea    ecx, [esi + 1]
        mov    word ptr [edi], cx
        add    edi, 12h
        inc    esi
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        add    ecx, 4
        cmp    ecx, esi
        ja     SHORT loc_1_0136
loc_1_014D:
        mov    cx, word ptr [eax + 2]
        add    cx, 4
        mov    word ptr [eax + 2], cx
        and    ecx, 0ffffh
        add    ecx, ecx
        xor    esi, esi
        lea    ecx, [ecx + ecx*8]
        mov    word ptr [eax + ecx], 0
        db  0A3h, 084h, 037h, 000h, 000h
        mov    si, word ptr [eax]
        lea    ecx, [esi*2]
        push   0
        lea    edi, [ecx + ecx*8]
        mov    cx, word ptr [eax + edi]
        mov    word ptr [eax], cx
        push   edx
        call   sub_1_3456
        add    esp, 8
        db  0A1h, 084h, 037h, 000h, 000h
        mov    dword ptr [edi + eax + 0eh], 0
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01B0:
        push   ebp
        mov    ebp, esp
        push   esi
        db  0A1h, 084h, 037h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi*2]
        mov    ax, word ptr [eax]
        lea    ecx, [ecx + ecx*8]
        db  08Bh, 015h, 084h, 037h, 000h, 000h
        mov    word ptr [ecx + edx], ax
        db  0A1h, 084h, 037h, 000h, 000h
        mov    word ptr [eax], si
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_01E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cmp    eax, 23h
        jg     SHORT loc_1_0200
        je     SHORT loc_1_0270
        cmp    eax, 0dh
        ja     SHORT loc_1_0216
        shl    eax, 2
        db  0FFh, 0A0h, 098h, 002h, 000h, 000h
loc_1_0200:
        cmp    eax, 24h
        je     SHORT loc_1_0277
        cmp    eax, 2ah
        je     SHORT loc_1_0280
        cmp    eax, 2eh
        je     SHORT loc_1_0284
        cmp    eax, 0ffh
        je     SHORT loc_1_0290
loc_1_0216:
        sub    eax, eax
        pop    ebp
        ret
loc_1_021A:
        sub    eax, eax
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0220:
        mov    eax, 103h
        pop    ebp
        ret
loc_1_0227:
        mov    eax, 10001h
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0230:
        mov    eax, 0c000009ah
        pop    ebp
        ret
loc_1_0237:
        mov    eax, 0c0000001h
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0240:
        mov    eax, 0c001000dh
        pop    ebp
        ret
loc_1_0247:
        mov    eax, 0c00000bbh
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0250:
        mov    eax, 80010004h
        pop    ebp
        ret
loc_1_0257:
        mov    eax, 80010003h
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0260:
        mov    eax, 10001h
        pop    ebp
        ret
loc_1_0267:
        mov    eax, 0c0010016h
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0270:
        mov    eax, 0c0010006h
        pop    ebp
        ret
loc_1_0277:
        mov    eax, 0c0010008h
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0280:
        sub    eax, eax
        pop    ebp
        ret
loc_1_0284:
        mov    eax, 0c000009ah
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_0290:
        mov    eax, 0c0000001h
        pop    ebp
        ret
        db  090h
loc_1_0298:
        sbb    al, byte ptr [edx]
        add    byte ptr [eax], al
        push   ss
        add    al, byte ptr [eax]
        db  000h, 020h
        add    al, byte ptr [eax]
        db  000h, 027h
        add    al, byte ptr [eax]
        db  000h, 027h
        add    al, byte ptr [eax]
        db  000h, 027h
        add    al, byte ptr [eax]
        db  000h, 030h
        add    al, byte ptr [eax]
        db  000h, 037h
        add    al, byte ptr [eax]
        db  000h, 040h, 002h
        add    byte ptr [eax], al
        inc    edi
        add    al, byte ptr [eax]
        db  000h, 050h, 002h
        add    byte ptr [eax], al
        push   edi
        add    al, byte ptr [eax]
        db  000h, 060h, 002h
        add    byte ptr [eax], al
        add    al, byte ptr [bx + si]
        add    byte ptr [ebp - 75h], dl
        in     al, dx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    eax, 2
        jne    SHORT loc_1_02E5
        mov    eax, 0c001000dh
        pop    ebp
        ret
loc_1_02E5:
        or     eax, eax
        jne    SHORT loc_1_02F0
        mov    eax, 0c0000001h
        pop    ebp
        ret
loc_1_02F0:
        sub    eax, eax
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0300:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    ecx, dword ptr [ecx + 48h]
        cmp    ecx, eax
        je     SHORT loc_1_031A
        nop
loc_1_0310:
        or     eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    SHORT loc_1_0310
loc_1_031A:
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_0320:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        cmp    dword ptr [ecx + 0b8h], edx
        jbe    SHORT loc_1_0385
        lea    edi, [ecx + 98h]
        lea    esi, [ecx + 0bch]
        nop
loc_1_0340:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        not    eax
        and    eax, dword ptr [esi]
        mov    dword ptr [esi], eax
        jne    SHORT loc_1_0376
        mov    eax, dword ptr [ecx + 0b8h]
        mov    ebx, dword ptr [ecx + eax*4 + 50h]
        mov    dword ptr [esi - 68h], ebx
        mov    bx, word ptr [ecx + eax*2 + 96h]
        mov    word ptr [edi], bx
        dec    eax
        mov    eax, dword ptr [ecx + eax*4 + 0bch]
        mov    dword ptr [esi], eax
        dec    dword ptr [ecx + 0b8h]
loc_1_0376:
        add    edi, 2
        add    esi, 4
        inc    edx
        cmp    dword ptr [ecx + 0b8h], edx
        ja     SHORT loc_1_0340
loc_1_0385:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_041C
        mov    ebx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        nop
loc_1_03B0:
        push   ebx
        mov    eax, dword ptr [esi + 0b8h]
        push   eax
        lea    eax, [esi + 98h]
        push   eax
        lea    eax, [esi + 54h]
        push   eax
        call   sub_1_33CA
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_03E0
        mov    ecx, dword ptr [edi + 18h]
        or     dword ptr [esi + eax*4 + 0b8h], ecx
        jmp    SHORT loc_1_040E
        db  005h, 000h, 000h, 000h, 000h
loc_1_03E0:
        mov    eax, dword ptr [esi + 0b8h]
        cmp    eax, 10h
        jae    SHORT loc_1_0425
        lea    ecx, [esi + eax*4]
        mov    edx, dword ptr [ebx]
        mov    dword ptr [ecx + 54h], edx
        mov    dx, word ptr [ebx + 4]
        mov    word ptr [esi + eax*2 + 98h], dx
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ecx + 0bch], eax
        inc    dword ptr [esi + 0b8h]
loc_1_040E:
        add    ebx, 6
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 10h], eax
        ja     SHORT loc_1_03B0
loc_1_041C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0425:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0440:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   88h
        call   sub_1_3414
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0460
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_0460:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 8], 0
        mov    ecx, dword ptr [ebp + 10h]
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 10h], 1
        mov    dword ptr [eax + 14h], 1010103h
        mov    dword ptr [eax + 20h], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18h], edi
        mov    ecx, dword ptr [ebx + 0b8h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    dword ptr [eax + 1ch], ecx
        sub    ecx, ecx
        cmp    dword ptr [ebx + 0b8h], ecx
        jbe    SHORT loc_1_04D5
        mov    esi, ebx
        add    esi, 98h
        mov    edx, ebx
        add    edx, 54h
loc_1_04B5:
        mov    ebx, dword ptr [edx]
        mov    dword ptr [edi], ebx
        mov    bx, word ptr [esi]
        mov    word ptr [edi + 4], bx
        add    esi, 2
        add    edi, 6
        add    edx, 4
        mov    ebx, dword ptr [ebp + 8]
        inc    ecx
        cmp    dword ptr [ebx + 0b8h], ecx
        ja     SHORT loc_1_04B5
loc_1_04D5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_04E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 4ch]
        mov    edx, dword ptr [eax + 48h]
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [eax + 48h], ecx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0500:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 48h]
        or     edx, edx
        je     SHORT loc_1_0540
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, edx
        jne    SHORT loc_1_0520
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 48h], eax
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0520:
        mov    ecx, dword ptr [edx + 8]
        or     ecx, ecx
        je     SHORT loc_1_0540
loc_1_0527:
        cmp    eax, ecx
        je     SHORT loc_1_0537
        mov    edx, ecx
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    SHORT loc_1_0527
        pop    esi
        pop    ebp
        ret
loc_1_0537:
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        dec    dword ptr [esi + 4ch]
loc_1_0540:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0550:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 1
        mov    eax, dword ptr [esi + 108h]
loc_1_0565:
        test   eax, ecx
        jne    SHORT loc_1_0571
        add    ecx, ecx
        or     ecx, ecx
        jne    SHORT loc_1_0565
        jmp    SHORT loc_1_0580
loc_1_0571:
        mov    edx, ecx
        not    edx
        and    edx, eax
        mov    dword ptr [esi + 108h], edx
        mov    dword ptr [ebp - 4], ecx
loc_1_0580:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0590
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_0590:
        push   0
        push   2ch
        call   sub_1_3414
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_05B0
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [esi + 108h], eax
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_05B0:
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 10h], ecx
        mov    dword ptr [eax + 14h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 1ch], 0
        mov    dword ptr [eax + 20h], 0
        mov    dword ptr [eax + 28h], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_05F0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 18h]
        or     dword ptr [eax + 108h], edx
        push   0
        push   ecx
        call   sub_1_3456
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0610:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [edx], 0ffffffffh
        mov    eax, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 18h]
        lea    eax, [eax + edi*4]
        mov    esi, dword ptr [ebp + 20h]
loc_1_0633:
        or     edi, edi
        je     SHORT loc_1_0647
        sub    eax, 4
        dec    edi
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [esi + 114h], ecx
        jne    SHORT loc_1_0633
        mov    dword ptr [edx], edi
loc_1_0647:
        cmp    dword ptr [edx], -1
        jne    SHORT loc_1_0657
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebp
        ret    24h
loc_1_0657:
        cmp    dword ptr [esi + 24h], 2
        jne    SHORT loc_1_0668
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    24h
loc_1_0668:
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        push   esi
        call   sub_1_0550
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0684
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebp
        ret    24h
loc_1_0684:
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    dword ptr [ecx], eax
        push   esi
        call   sub_1_04E0
        add    esp, 8
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_1_06A0
        mov    dword ptr [esi + 24h], 1
loc_1_06A0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_06B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 1ch], 0
        je     SHORT loc_1_06D8
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, 103h
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [edi + 50h], 1
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_06D8:
        cmp    dword ptr [edi + 4ch], 1
        jne    SHORT loc_1_06E5
        mov    dword ptr [edi + 24h], 0
loc_1_06E5:
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   edi
        call   sub_1_0500
        add    esp, 8
        push   esi
        push   edi
        call   sub_1_05F0
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0710:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    eax, dword ptr [esi + 1ch]
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        cmp    dword ptr [esi + 50h], edi
        je     SHORT loc_1_078E
        cmp    eax, edi
        jne    SHORT loc_1_078E
        mov    ebx, dword ptr [esi + 48h]
        cmp    ebx, edi
        je     SHORT loc_1_0764
loc_1_0735:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebx + 0ch], 0
        jne    SHORT loc_1_075D
        cmp    dword ptr [esi + 4ch], 1
        jne    SHORT loc_1_074E
        mov    dword ptr [esi + 24h], 0
loc_1_074E:
        push   ebx
        push   esi
        call   sub_1_0500
        add    esp, 8
        mov    dword ptr [ebx + 8], edi
        mov    edi, ebx
loc_1_075D:
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, ebx
        jne    SHORT loc_1_0735
loc_1_0764:
        mov    dword ptr [esi + 50h], 0
        or     edi, edi
        je     SHORT loc_1_078E
        nop
loc_1_0770:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3568
        mov    ebx, dword ptr [edi + 8]
        push   edi
        push   esi
        call   sub_1_05F0
        add    esp, 8
        mov    edi, ebx
        or     ebx, ebx
        jne    SHORT loc_1_0770
loc_1_078E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 4]
        cmp    dword ptr [edi + 24h], 2
        jne    SHORT loc_1_07C0
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_07C0:
        cmp    dword ptr [esi + 0ch], 1
        je     SHORT loc_1_07D2
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_07D2:
        mov    dword ptr [esi + 0ch], 2
        push   0
        push   2ch
        call   sub_1_3414
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_0800
        mov    dword ptr [esi + 0ch], 1
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0800:
        mov    dword ptr [ebx + 14h], 1010eh
        mov    dword ptr [ebx + 10h], 1
        push   3
        push   ebx
        push   esi
        call   sub_1_1B00
        add    esp, 0ch
        cmp    eax, 103h
        jne    SHORT loc_1_082D
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_082D:
        push   0
        push   esi
        push   edi
        push   ebx
        call   sub_1_0850
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0850:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 114h], 1
        je     SHORT loc_1_0896
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_3456
        add    esp, 8
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0320
        add    esp, 8
        mov    dword ptr [esi + 0ch], 4
        push   0
        push   esi
        call   sub_1_1EB0
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_1_08D1
loc_1_0896:
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ch], 3
        mov    dword ptr [edi + 14h], 2010103h
        lea    eax, [edi + 28h]
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [eax], 0
        push   edi
        push   esi
        call   sub_1_1CB0
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_08D1
        push   0
        push   edi
        call   sub_1_3456
        add    esp, 8
loc_1_08D1:
        cmp    ebx, 103h
        jne    SHORT loc_1_08E3
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_08E3:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0900
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0900:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0ch], 5
        jne    SHORT loc_1_0914
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_0914:
        mov    dword ptr [eax + 0ch], 5
        cmp    dword ptr [eax + 14h], 0
        jle    SHORT loc_1_092A
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_092A:
        mov    esi, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06B0
        add    esp, 8
        mov    edi, eax
        cmp    edi, 103h
        je     SHORT loc_1_0951
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_0951
        push   edi
        push   esi
        call   sub_1_3568
loc_1_0951:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0960:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 5
        db  08Bh, 00Dh, 088h, 03Ah, 000h, 000h
        mov    ax, word ptr [eax + ecx + 14h]
        and    eax, 0ffffh
        sub    eax, 4
        db  023h, 005h, 07Ch, 037h, 000h, 000h
        db  08Bh, 00Dh, 07Ch, 03Ah, 000h, 000h
        or     dword ptr [eax + ecx], 75690000h
        db  03Bh, 005h, 074h, 037h, 000h, 000h
        jne    SHORT loc_1_09EF
        db  083h, 03Dh, 068h, 037h, 000h, 000h, 000h
        je     SHORT loc_1_09EF
        mov    edx, 7569h
        sub    esi, esi
loc_1_09A8:
        db  0A1h, 074h, 037h, 000h, 000h
        db  003h, 005h, 07Ch, 03Ah, 000h, 000h
        cmp    word ptr [eax + 2], dx
        jne    SHORT loc_1_09EF
        db  0FFh, 00Dh, 068h, 037h, 000h, 000h
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 074h, 037h, 000h, 000h
        add    ecx, eax
        add    ecx, 4
        db  023h, 00Dh, 07Ch, 037h, 000h, 000h
        db  089h, 00Dh, 074h, 037h, 000h, 000h
        add    eax, 4
        db  001h, 005h, 070h, 037h, 000h, 000h
        db  039h, 035h, 068h, 037h, 000h, 000h
        jne    SHORT loc_1_09A8
loc_1_09EF:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3
        and    eax, 0fffffffch
        mov    dword ptr [ebp - 18h], eax
        or     eax, 55490000h
        db  08Bh, 00Dh, 078h, 037h, 000h, 000h
        db  08Bh, 015h, 07Ch, 03Ah, 000h, 000h
        mov    dword ptr [ecx + edx], eax
        db  0A1h, 078h, 037h, 000h, 000h
        add    eax, 4
        db  023h, 005h, 07Ch, 037h, 000h, 000h
        db  0A3h, 078h, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        db  029h, 005h, 070h, 037h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        shl    eax, 5
        mov    dword ptr [ebp - 10h], eax
        db  0A1h, 078h, 037h, 000h, 000h
        db  08Bh, 00Dh, 088h, 03Ah, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + edx + 14h], ax
        lea    eax, [ebp - 0ch]
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3654
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 03Dh, 078h, 037h, 000h, 000h
        db  003h, 03Dh, 07Ch, 03Ah, 000h, 000h
        db  0A1h, 074h, 037h, 000h, 000h
        db  039h, 005h, 078h, 037h, 000h, 000h
        jl     SHORT loc_1_0AA2
        db  08Bh, 01Dh, 08Ch, 03Ah, 000h, 000h
        jmp    SHORT loc_1_0AA8
loc_1_0AA2:
        db  08Bh, 01Dh, 074h, 037h, 000h, 000h
loc_1_0AA8:
        db  02Bh, 01Dh, 078h, 037h, 000h, 000h
        cmp    dword ptr [ebp + 0ch], ebx
        ja     SHORT loc_1_0AD5
        mov    ebx, dword ptr [ebp + 0ch]
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        mov    word ptr [eax + edx + 0eh], 1
        db  08Bh, 00Dh, 088h, 03Ah, 000h, 000h
        mov    ax, bx
        mov    word ptr [ecx + edx + 12h], ax
        jmp    SHORT loc_1_0B02
loc_1_0AD5:
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        mov    word ptr [eax + edx + 0eh], 2
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    word ptr [edx + eax + 12h], bx
        mov    eax, dword ptr [ebp + 0ch]
        sub    ax, bx
        db  08Bh, 00Dh, 088h, 03Ah, 000h, 000h
        mov    word ptr [edx + ecx + 1ah], ax
        mov    dword ptr [ebp - 14h], ebx
loc_1_0B02:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_0B7A
loc_1_0B08:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0B2C
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3678
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_369C
loc_1_0B2C:
        cmp    ebx, dword ptr [ebp - 4]
        jl     SHORT loc_1_0B54
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_3399
        add    esp, 0ch
        sub    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_0B76
loc_1_0B54:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_3399
        add    esp, 0ch
        sub    esi, ebx
        add    dword ptr [ebp - 8], ebx
        sub    dword ptr [ebp - 4], ebx
        mov    ebx, dword ptr [ebp + 0ch]
        sub    ebx, dword ptr [ebp - 14h]
        db  08Bh, 03Dh, 07Ch, 03Ah, 000h, 000h
loc_1_0B76:
        or     esi, esi
        jne    SHORT loc_1_0B08
loc_1_0B7A:
        db  0A1h, 078h, 037h, 000h, 000h
        add    eax, dword ptr [ebp - 18h]
        db  023h, 005h, 07Ch, 037h, 000h, 000h
        db  0A3h, 078h, 037h, 000h, 000h
        db  0FFh, 005h, 068h, 037h, 000h, 000h
        db  0A1h, 074h, 037h, 000h, 000h
        db  039h, 005h, 078h, 037h, 000h, 000h
        jle    SHORT loc_1_0BE0
        db  0A1h, 08Ch, 03Ah, 000h, 000h
        db  02Bh, 005h, 078h, 037h, 000h, 000h
        cmp    eax, 104h
        jge    SHORT loc_1_0BE0
        lea    ecx, [eax - 4]
        or     ecx, 75690000h
        db  08Bh, 015h, 078h, 037h, 000h, 000h
        db  08Bh, 035h, 07Ch, 03Ah, 000h, 000h
        mov    dword ptr [edx + esi], ecx
        db  029h, 005h, 070h, 037h, 000h, 000h
        db  0C7h, 005h, 078h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 068h, 037h, 000h, 000h
loc_1_0BE0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        call   sub_1_0050
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_0E8F
        db  083h, 03Dh, 05Ch, 037h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0E83
        db  083h, 03Dh, 010h, 037h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0E83
        db  0C7h, 005h, 010h, 037h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 05Ch, 037h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0E60
loc_1_0C41:
        db  0A1h, 05Ch, 037h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 18h]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3654
        db  0A1h, 070h, 037h, 000h, 000h
        cmp    dword ptr [ebp - 18h], eax
        jge    NEAR PTR loc_1_0E60
        db  083h, 03Dh, 06Ch, 037h, 000h, 000h, 0FFh
        je     NEAR PTR loc_1_0E60
        mov    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [edi + 24h]
        add    edi, 24h
        mov    dword ptr [ebp - 0ch], eax
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi + 4]
        db  0A3h, 05Ch, 037h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_0C9C
        db  0C7h, 005h, 060h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0C9C:
        db  0A1h, 06Ch, 037h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    ebx, eax
        shl    ebx, 5
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    eax, dword ptr [ebx + eax]
        db  0A3h, 06Ch, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0A00
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [eax*2]
        mov    dword ptr [ebp - 14h], 1
        lea    ecx, [ecx + ecx*8]
        db  08Bh, 015h, 084h, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + edx + 2], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 0ch], 1
        jne    NEAR PTR loc_1_0DE6
        inc    dword ptr [esi + 126h]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_3222
        add    esp, 0ch
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 40h], 80h
        jne    NEAR PTR loc_1_0DEB
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    eax, dword ptr [ebx + eax + 14h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    ecx, dword ptr [esi + 11ch]
        and    eax, 0ffffh
        add    eax, ecx
        test   byte ptr [esi + 0fch], 20h
        jne    SHORT loc_1_0D9B
        cmp    dword ptr [esi + 114h], 1
        je     SHORT loc_1_0D80
        test   byte ptr [eax], 1
        jne    SHORT loc_1_0D9B
        lea    ecx, [esi + 120h]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_0D79
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
loc_1_0D79:
        je     SHORT loc_1_0D9B
        jmp    SHORT loc_1_0DEB
        db  08Dh, 049h, 000h
loc_1_0D80:
        test   byte ptr [eax], 80h
        jne    SHORT loc_1_0D9B
        lea    ecx, [esi + 120h]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_0DEB
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    SHORT loc_1_0DEB
loc_1_0D9B:
        db  0A1h, 088h, 03Ah, 000h, 000h
        lea    ecx, [ebx + eax]
        mov    ax, word ptr [ecx + 0eh]
        mov    word ptr [ebp - 28h], ax
        cmp    word ptr [ebp - 28h], 1
        mov    ax, word ptr [ecx + 12h]
        mov    word ptr [ebp - 26h], ax
        mov    eax, dword ptr [ecx + 14h]
        mov    dword ptr [ebp - 24h], eax
        jbe    SHORT loc_1_0DCE
        mov    ax, word ptr [ecx + 1ah]
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 1eh], eax
loc_1_0DCE:
        lea    eax, [ebp - 28h]
        push   eax
        push   esi
        call   sub_1_26E0
        add    esp, 8
        push   esi
        call   sub_1_2530
        add    esp, 4
        jmp    SHORT loc_1_0DEB
loc_1_0DE6:
        mov    edi, 40010007h
loc_1_0DEB:
        cmp    edi, 103h
        je     SHORT loc_1_0E3A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0960
        add    esp, 4
        db  0A1h, 06Ch, 037h, 000h, 000h
        db  08Bh, 00Dh, 088h, 03Ah, 000h, 000h
        mov    dword ptr [ebx + ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 06Ch, 037h, 000h, 000h
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_356E
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_2350
        add    esp, 8
loc_1_0E3A:
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_0E53
        call   sub_1_0050
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     SHORT loc_1_0E60
        mov    dword ptr [ebp - 14h], 0
loc_1_0E53:
        db  083h, 03Dh, 05Ch, 037h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0C41
loc_1_0E60:
        db  0C7h, 005h, 010h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_0E8F
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0E83:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_01B0
        add    esp, 4
loc_1_0E8F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0EA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + 4]
        cmp    dword ptr [ecx + 0ch], 5
        jne    SHORT loc_1_0EC3
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0EC3:
        call   sub_1_0050
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_1_0EE0
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_0EE0:
        db  083h, 03Dh, 010h, 037h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_10E0
        db  0C7h, 005h, 010h, 037h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 05Ch, 037h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_10D0
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        push   0
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_3654
        db  0A1h, 070h, 037h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], eax
        jge    NEAR PTR loc_1_10D3
        db  083h, 03Dh, 06Ch, 037h, 000h, 000h, 0FFh
        je     NEAR PTR loc_1_10D3
        db  0A1h, 06Ch, 037h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    ebx, eax
        shl    ebx, 5
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    eax, dword ptr [ebx + eax]
        db  0A3h, 06Ch, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_0A00
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [eax*2]
        lea    ecx, [ecx + ecx*8]
        db  0A1h, 084h, 037h, 000h, 000h
        mov    dword ptr [ecx + eax + 2], esi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 24h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 2ch], eax
        add    esi, 24h
        inc    dword ptr [edi + 126h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_3222
        add    esp, 0ch
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    esi, eax
        mov    eax, dword ptr [edi + 8]
        test   byte ptr [eax + 40h], 80h
        jne    NEAR PTR loc_1_1072
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    eax, dword ptr [ebx + eax + 14h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    ecx, dword ptr [edi + 11ch]
        and    eax, 0ffffh
        add    eax, ecx
        test   byte ptr [edi + 0fch], 20h
        jne    SHORT loc_1_101B
        cmp    dword ptr [edi + 114h], 1
        je     SHORT loc_1_1000
        test   byte ptr [eax], 1
        jne    SHORT loc_1_101B
        lea    ecx, [edi + 120h]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_0FFA
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
loc_1_0FFA:
        je     SHORT loc_1_101B
        jmp    SHORT loc_1_1072
        db  08Bh, 0FFh
loc_1_1000:
        test   byte ptr [eax], 80h
        jne    SHORT loc_1_101B
        lea    ecx, [edi + 120h]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_1072
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    SHORT loc_1_1072
loc_1_101B:
        db  0A1h, 088h, 03Ah, 000h, 000h
        lea    ecx, [ebx + eax]
        mov    ax, word ptr [ecx + 0eh]
        db  066h, 0A3h, 018h, 037h, 000h, 000h
        db  066h, 083h, 03Dh, 018h, 037h, 000h, 000h, 001h
        mov    ax, word ptr [ecx + 12h]
        db  066h, 0A3h, 01Ah, 037h, 000h, 000h
        mov    eax, dword ptr [ecx + 14h]
        db  0A3h, 01Ch, 037h, 000h, 000h
        jbe    SHORT loc_1_105B
        mov    ax, word ptr [ecx + 1ah]
        db  066h, 0A3h, 020h, 037h, 000h, 000h
        mov    eax, dword ptr [ecx + 1ch]
        db  0A3h, 022h, 037h, 000h, 000h
loc_1_105B:
        db  068h, 018h, 037h, 000h, 000h
        push   edi
        call   sub_1_26E0
        add    esp, 8
        push   edi
        call   sub_1_2530
        add    esp, 4
loc_1_1072:
        cmp    esi, 103h
        je     SHORT loc_1_10AA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0960
        add    esp, 4
        db  0A1h, 06Ch, 037h, 000h, 000h
        db  08Bh, 00Dh, 088h, 03Ah, 000h, 000h
        mov    dword ptr [ebx + ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 06Ch, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        jmp    SHORT loc_1_10B0
loc_1_10AA:
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [ecx + 14h]
loc_1_10B0:
        db  0C7h, 005h, 010h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_0BF0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_10D0:
        mov    esi, dword ptr [ebp + 0ch]
loc_1_10D3:
        db  0C7h, 005h, 010h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_10E3
        db  090h
loc_1_10E0:
        mov    esi, dword ptr [ebp + 0ch]
loc_1_10E3:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_01B0
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        inc    dword ptr [ecx + 14h]
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [esi + 24h], ecx
        lea    eax, [esi + 24h]
        db  083h, 03Dh, 060h, 037h, 000h, 000h, 000h
        je     SHORT loc_1_1120
        db  0A1h, 060h, 037h, 000h, 000h
        mov    dword ptr [eax + 28h], esi
        db  089h, 035h, 060h, 037h, 000h, 000h
        jmp    SHORT loc_1_112C
        db  005h, 000h, 000h, 000h, 000h
loc_1_1120:
        db  089h, 035h, 060h, 037h, 000h, 000h
        db  089h, 035h, 05Ch, 037h, 000h, 000h
loc_1_112C:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1140:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], esi
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp + 10h]
        add    eax, dword ptr [ecx + 118h]
        or     esi, esi
        mov    dword ptr [ebp + 10h], eax
        jne    SHORT loc_1_1186
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1186:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_3654
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_12B3
        mov    ecx, dword ptr [ebp - 18h]
        mov    edi, dword ptr [ecx + 28h]
        cmp    dword ptr [ecx + 3ch], 0
        jne    SHORT loc_1_11DB
        mov    eax, dword ptr [ecx + 38h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   ecx
        call   sub_1_32E2
        add    esp, 0ch
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    ecx, dword ptr [ebp - 18h]
        or     eax, eax
        mov    dword ptr [ecx + 3ch], 1
        jne    NEAR PTR loc_1_127B
loc_1_11DB:
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ecx + 34h]
        xor    ebx, ebx
        mov    bx, word ptr [eax]
        cmp    esi, ebx
        jae    SHORT loc_1_11EC
        mov    ebx, esi
loc_1_11EC:
        or     ebx, ebx
        jne    SHORT loc_1_1203
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1203:
        or     esi, esi
        je     SHORT loc_1_1279
loc_1_1207:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1231
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1284
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3678
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_369C
loc_1_1231:
        cmp    dword ptr [ebp - 4], ebx
        jbe    SHORT loc_1_1254
        or     ebx, ebx
        je     SHORT loc_1_1292
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3399
        add    esp, 0ch
        sub    esi, ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    ebx, ebx
        jmp    SHORT loc_1_1275
loc_1_1254:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3399
        add    esp, 0ch
        sub    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
loc_1_1275:
        or     esi, esi
        jne    SHORT loc_1_1207
loc_1_1279:
        sub    eax, eax
loc_1_127B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1284:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1292:
        mov    ecx, dword ptr [ebp - 18h]
        pop    edi
        mov    eax, dword ptr [ecx + 34h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    dword ptr [ecx], eax
        mov    eax, 0c0000001h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_12B3:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    ax, word ptr [edx]
        add    ecx, 4
        mov    dword ptr [ebp - 1ch], eax
loc_1_12C4:
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 1ch]
        jae    SHORT loc_1_12F7
        mov    eax, dword ptr [ecx]
        mov    edx, eax
        and    edx, 0ffff0fffh
        shr    edx, 0ch
        and    eax, 0ffffh
        lea    edi, [edx + eax]
        xor    ebx, ebx
        inc    dword ptr [ebp - 10h]
        mov    bx, word ptr [ecx - 2]
        add    ecx, 6
        cmp    dword ptr [ebp + 10h], ebx
        jbe    SHORT loc_1_1310
        sub    dword ptr [ebp + 10h], ebx
        jmp    SHORT loc_1_12C4
loc_1_12F7:
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_1_1310:
        add    edi, dword ptr [ebp + 10h]
        sub    ebx, dword ptr [ebp + 10h]
        cmp    esi, ebx
        jae    SHORT loc_1_131C
        mov    ebx, esi
loc_1_131C:
        or     esi, esi
        je     NEAR PTR loc_1_1279
        mov    eax, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 0ch]
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2 + 4]
        mov    dword ptr [ebp - 14h], eax
loc_1_1334:
        or     ebx, ebx
        jne    SHORT loc_1_1377
        mov    edx, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    ax, word ptr [edx]
        cmp    eax, dword ptr [ebp - 10h]
        jbe    NEAR PTR loc_1_13F8
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        lea    edi, [ecx + eax]
        mov    eax, dword ptr [ebp - 14h]
        xor    ebx, ebx
        add    dword ptr [ebp - 14h], 6
        mov    bx, word ptr [eax - 2]
        inc    dword ptr [ebp - 10h]
        cmp    esi, ebx
        jae    SHORT loc_1_1377
        mov    ebx, esi
loc_1_1377:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_13A5
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1410
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3678
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_369C
loc_1_13A5:
        cmp    dword ptr [ebp - 4], ebx
        jbe    SHORT loc_1_13C4
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3399
        add    esp, 0ch
        sub    esi, ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    ebx, ebx
        jmp    SHORT loc_1_13E5
loc_1_13C4:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3399
        add    esp, 0ch
        sub    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
loc_1_13E5:
        or     esi, esi
        jne    NEAR PTR loc_1_1334
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_13F8:
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        sub    dword ptr [ecx], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  005h, 000h, 000h, 000h, 000h
loc_1_1410:
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        sub    dword ptr [ecx], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1430:
        push   ebp
        mov    ebp, esp
        push   6
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3162
        add    esp, 14h
        push   eax
        call   sub_1_01E0
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1460:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        cmp    dword ptr [esi + 24h], 2
        jne    SHORT loc_1_1480
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1480:
        mov    dword ptr [esi + 24h], 2
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 48h]
        cmp    eax, ecx
        je     SHORT loc_1_149A
loc_1_1490:
        add    ecx, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_1_1490
loc_1_149A:
        or     ecx, ecx
        jg     NEAR PTR loc_1_1536
        mov    byte ptr [esi + 1ah], 1
        inc    dword ptr [esi + 1ch]
        mov    edi, dword ptr [esi + 48h]
        or     edi, edi
        je     SHORT loc_1_14D3
        mov    ebx, 1
loc_1_14B5:
        cmp    dword ptr [edi + 0ch], ebx
        jne    SHORT loc_1_14CF
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3592
        mov    edi, dword ptr [edi + 8]
loc_1_14CF:
        or     edi, edi
        jne    SHORT loc_1_14B5
loc_1_14D3:
        push   esi
        call   sub_1_0710
        add    esp, 4
        push   esi
        call   sub_1_1430
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_152D
        inc    dword ptr [esi + 1ch]
        mov    edi, dword ptr [esi + 48h]
        or     edi, edi
        je     SHORT loc_1_151D
loc_1_14F5:
        cmp    dword ptr [edi + 0ch], 1
        jne    SHORT loc_1_1519
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3598
        mov    edi, dword ptr [edi + 8]
loc_1_1519:
        or     edi, edi
        jne    SHORT loc_1_14F5
loc_1_151D:
        push   esi
        call   sub_1_0710
        add    esp, 4
        mov    dword ptr [esi + 24h], 1
loc_1_152D:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1536:
        mov    byte ptr [esi + 1ah], 0
        inc    dword ptr [esi + 1ch]
        mov    edi, dword ptr [esi + 48h]
        or     edi, edi
        je     SHORT loc_1_1570
        mov    ebx, 1
loc_1_1549:
        cmp    dword ptr [edi + 0ch], ebx
        jne    SHORT loc_1_156C
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3598
        mov    edi, dword ptr [edi + 8]
loc_1_156C:
        or     edi, edi
        jne    SHORT loc_1_1549
loc_1_1570:
        push   esi
        call   sub_1_0710
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1590:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        sub    ebx, ebx
        mov    word ptr [ebp - 2], 0
        sub    edx, edx
        mov    dword ptr [ebp - 10h], edx
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 2
        jne    SHORT loc_1_15C0
        sub    esi, esi
        mov    ecx, dword ptr [ebp + 8]
        jmp    SHORT loc_1_15C8
        db  08Dh, 064h, 024h, 000h
loc_1_15C0:
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, ecx
        mov    ecx, dword ptr [ecx + 4]
loc_1_15C8:
        mov    edi, dword ptr [ebp + 10h]
        cmp    edi, 20101h
        ja     SHORT loc_1_15F0
        je     NEAR PTR loc_1_1810
        sub    edi, 10101h
        cmp    edi, 10h
        ja     SHORT loc_1_1654
        shl    edi, 2
        db  0FFh, 0A7h, 088h, 019h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_15F0:
        cmp    edi, 1010104h
        ja     SHORT loc_1_1630
        je     NEAR PTR loc_1_1900
        cmp    edi, 20102h
        je     NEAR PTR loc_1_1820
        cmp    edi, 1010101h
        je     NEAR PTR loc_1_182B
        cmp    edi, 1010102h
        je     NEAR PTR loc_1_1840
        cmp    edi, 1010103h
        je     NEAR PTR loc_1_1851
        jmp    SHORT loc_1_1654
loc_1_1630:
        cmp    edi, 2010101h
        je     NEAR PTR loc_1_182B
        cmp    edi, 2010102h
        je     NEAR PTR loc_1_1840
        cmp    edi, 2010103h
        je     NEAR PTR loc_1_1933
loc_1_1654:
        mov    ebx, 0c00000bbh
        mov    esi, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_1944
loc_1_1661:
        db  0BEh, 08Ch, 037h, 000h, 000h
        mov    edx, 64h
        jmp    NEAR PTR loc_1_1944
loc_1_1670:
        mov    eax, dword ptr [ecx + 0ch]
        mov    eax, dword ptr [eax + 6]
        and    al, 7
        cmp    al, 7
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_168A
        mov    dword ptr [ebp - 10h], 4
loc_1_168A:
        lea    esi, [ebp - 10h]
        jmp    NEAR PTR loc_1_193F
loc_1_1692:
        mov    eax, dword ptr [ecx + 114h]
        mov    dword ptr [ebp - 14h], eax
        lea    esi, [ebp - 14h]
        jmp    NEAR PTR loc_1_193F
loc_1_16A3:
        mov    dword ptr [ebp - 8], 0d0h
        jmp    NEAR PTR loc_1_193C
        db  090h
loc_1_16B0:
        db  0A1h, 08Ch, 03Ah, 000h, 000h
        sub    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_16CF
        mov    eax, dword ptr [ebp - 8]
loc_1_16CF:
        cmp    dword ptr [ecx + 114h], 1
        je     SHORT loc_1_16E3
        sub    eax, dword ptr [ecx + 118h]
        jmp    NEAR PTR loc_1_1939
loc_1_16E3:
        sub    eax, 0eh
        jmp    NEAR PTR loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_16F0:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, 64h
        mov    eax, dword ptr [eax + 3ch]
        sub    edx, edx
        div    ecx
        jmp    NEAR PTR loc_1_1939
loc_1_1704:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 46h]
        jmp    NEAR PTR loc_1_1939
        db  090h
loc_1_1710:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 4ch]
        jmp    NEAR PTR loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_1720:
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 4ah]
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1939
loc_1_1731:
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 50h]
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1939
loc_1_1742:
        mov    dword ptr [ebp - 8], 0
        mov    edx, dword ptr [ecx + 8]
        add    edx, 52h
        lea    eax, [ebp - 8]
        mov    cx, word ptr [edx]
        mov    word ptr [eax], cx
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [eax + 2], cl
        mov    esi, eax
        jmp    NEAR PTR loc_1_193F
loc_1_1765:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 56h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        lea    esi, [ecx + eax]
        call   sub_1_34B7
        add    esi, eax
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edx, ecx
        jmp    NEAR PTR loc_1_1944
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_17A0:
        mov    dword ptr [ebp - 8], 0
        cmp    eax, 2
        jne    SHORT loc_1_17B7
        mov    eax, dword ptr [ecx + 0fch]
        jmp    NEAR PTR loc_1_1939
loc_1_17B7:
        mov    eax, dword ptr [esi + 1ch]
        jmp    NEAR PTR loc_1_1939
        db  090h
loc_1_17C0:
        mov    eax, dword ptr [ecx + 110h]
        jmp    NEAR PTR loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_17D0:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edx, 2
        jmp    NEAR PTR loc_1_1944
loc_1_17E3:
        db  0A1h, 08Ch, 03Ah, 000h, 000h
        sub    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 44h]
        cmp    word ptr [ebp - 8], ax
        jbe    NEAR PTR loc_1_193C
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1939
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1810:
        mov    eax, dword ptr [ecx + 126h]
        jmp    NEAR PTR loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_1820:
        mov    eax, dword ptr [ecx + 12ah]
        jmp    NEAR PTR loc_1_1939
loc_1_182B:
        mov    eax, dword ptr [ecx + 8]
        xor    edx, edx
        lea    esi, [eax + 14h]
        mov    dx, word ptr [eax + 12h]
        jmp    NEAR PTR loc_1_1944
        db  08Dh, 064h, 024h, 000h
loc_1_1840:
        mov    eax, dword ptr [ecx + 8]
        xor    edx, edx
        lea    esi, [eax + 24h]
        mov    dx, word ptr [eax + 12h]
        jmp    NEAR PTR loc_1_1944
loc_1_1851:
        sub    edi, edi
        cmp    eax, 2
        jne    SHORT loc_1_1861
        mov    dword ptr [ebp - 0ch], 0ffffffffh
        jmp    SHORT loc_1_1867
loc_1_1861:
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], esi
loc_1_1867:
        mov    eax, dword ptr [ecx + 0b8h]
        or     eax, eax
        je     SHORT loc_1_1885
        lea    edx, [ecx + 0bch]
loc_1_1877:
        mov    esi, dword ptr [ebp - 0ch]
        test   dword ptr [edx], esi
        je     SHORT loc_1_187F
        inc    edi
loc_1_187F:
        add    edx, 4
        dec    eax
        jne    SHORT loc_1_1877
loc_1_1885:
        lea    eax, [edi + edi*2]
        lea    esi, [eax*2]
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [eax]
        cmp    esi, edx
        jbe    SHORT loc_1_18AB
        mov    eax, dword ptr [ebp + 1ch]
        sub    esi, edx
        mov    dword ptr [eax], esi
        mov    eax, 0c0010014h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_18AB:
        sub    edx, esi
        mov    edi, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edx
        add    dword ptr [edi], esi
        sub    eax, eax
        cmp    dword ptr [ecx + 0b8h], eax
        jbe    SHORT loc_1_18F5
        lea    ebx, [ecx + 98h]
        mov    edi, dword ptr [ebp + 14h]
        lea    edx, [ecx + 0bch]
loc_1_18D0:
        mov    esi, dword ptr [ebp - 0ch]
        test   dword ptr [edx], esi
        je     SHORT loc_1_18E6
        mov    esi, dword ptr [edx - 68h]
        mov    dword ptr [edi], esi
        mov    si, word ptr [ebx]
        mov    word ptr [edi + 4], si
        add    edi, 6
loc_1_18E6:
        add    ebx, 2
        add    edx, 4
        inc    eax
        cmp    dword ptr [ecx + 0b8h], eax
        ja     SHORT loc_1_18D0
loc_1_18F5:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_1900:
        mov    eax, dword ptr [ecx + 8]
        mov    esi, dword ptr [eax + 38h]
        call   sub_1_34B7
        mov    ecx, esi
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        and    esi, 0ffffh
        mov    ax, word ptr [eax + esi]
        and    eax, 0ffffh
        cmp    eax, 10h
        jl     SHORT loc_1_1939
        mov    eax, 10h
        jmp    SHORT loc_1_1939
loc_1_1933:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 34h]
loc_1_1939:
        mov    dword ptr [ebp - 8], eax
loc_1_193C:
        lea    esi, [ebp - 8]
loc_1_193F:
        mov    edx, 4
loc_1_1944:
        or     ebx, ebx
        jne    SHORT loc_1_197E
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [eax]
        cmp    edx, ecx
        jbe    SHORT loc_1_1966
        mov    eax, dword ptr [ebp + 1ch]
        sub    edx, ecx
        mov    dword ptr [eax], edx
        mov    ebx, 0c0010014h
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1966:
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp + 20h]
        add    dword ptr [edi], edx
        sub    dword ptr [eax], edx
loc_1_197E:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_1988:
        popad
        push   ss
        add    byte ptr [eax], al
        jo     SHORT loc_1_19A4
        add    byte ptr [eax], al
        xchg   edx, eax
        push   ss
        add    byte ptr [eax], al
        xchg   edx, eax
        push   ss
        add    byte ptr [eax], al
        db  0A3h, 016h, 000h, 000h, 0B0h
        push   ss
        add    byte ptr [eax], al
        db  0F0h, 016h, 000h, 000h
loc_1_19A4:
        add    al, 17h
        add    byte ptr [eax], al
        adc    byte ptr [edi], dl
        add    byte ptr [eax], al
        and    byte ptr [edi], dl
        add    byte ptr [eax], al
        xor    dword ptr [edi], edx
        add    byte ptr [eax], al
        inc    edx
        pop    ss
        add    byte ptr [eax], al
        pop    ss
        add    byte ptr [eax], al
        db  0A0h, 017h, 000h, 000h, 0C0h
        pop    ss
        add    byte ptr [eax], al
        rcl    byte ptr [edi]
        add    byte ptr [eax], al
        db  0E3h, 017h
        add    byte ptr [eax], al
        lea    esp, [esp]
sub_1_19D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ecx + 20h]
        push   eax
        lea    eax, [ecx + 24h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ecx + 18h]
        push   eax
        mov    eax, dword ptr [ecx + 14h]
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1590
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 4]
        call   sub_1_0050
        mov    esi, eax
        lea    eax, [esi*2]
        mov    ecx, dword ptr [ebp + 0ch]
        lea    ebx, [eax + eax*8]
        db  0A1h, 084h, 037h, 000h, 000h
        mov    dword ptr [ebx + eax + 6], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 1
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        mov    dword ptr [ebx + ecx + 0eh], eax
        mov    ax, word ptr [ebp + 14h]
        push   eax
        push   0
        mov    ax, word ptr [ebp + 10h]
        push   eax
        push   esi
        push   edi
        call   sub_1_3162
        add    esp, 14h
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        cmp    edi, 103h
        je     SHORT loc_1_1A96
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1AA9
        push   esi
        call   sub_1_01B0
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1A96:
        db  0A1h, 084h, 037h, 000h, 000h
        mov    dword ptr [ebx + eax + 0eh], 0
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 14h]
loc_1_1AA9:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AC0:
        push   ebp
        mov    ebp, esp
        sub    ax, ax
        mov    ecx, dword ptr [ebp + 8]
        test   ecx, 0e007h
        je     SHORT loc_1_1AD5
        mov    ax, 1
loc_1_1AD5:
        test   cl, 8
        je     SHORT loc_1_1ADE
        or     ax, 2
loc_1_1ADE:
        test   cl, 20h
        je     SHORT loc_1_1AE7
        or     ax, 4
loc_1_1AE7:
        test   cl, 10h
        je     SHORT loc_1_1AF0
        or     ax, 8
loc_1_1AF0:
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 4]
        call   sub_1_0050
        mov    dword ptr [ebp - 4], eax
        add    eax, eax
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        lea    ebx, [eax + eax*8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + ecx + 6], eax
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0300
        add    esp, 4
        mov    dword ptr [edi + 0fch], eax
        push   eax
        call   sub_1_1AC0
        add    esp, 4
        mov    dword ptr [ebp - 8], 1
        lea    edx, [ebp - 8]
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        mov    dword ptr [ebx + ecx + 0eh], edx
        push   7
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        call   sub_1_3162
        add    esp, 14h
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        cmp    edi, 103h
        je     SHORT loc_1_1BA0
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1BB0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1BA0:
        db  0A1h, 084h, 037h, 000h, 000h
        mov    dword ptr [ebx + eax + 0eh], 0
        inc    dword ptr [esi + 14h]
loc_1_1BB0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BC0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    ecx, dword ptr [ecx + 48h]
        cmp    ecx, eax
        je     SHORT loc_1_1BDA
        nop
loc_1_1BD0:
        or     eax, dword ptr [ecx + 20h]
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    SHORT loc_1_1BD0
loc_1_1BDA:
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_1BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + 4]
        call   sub_1_0050
        mov    esi, eax
        lea    eax, [esi*2]
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        lea    ebx, [eax + eax*8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + ecx + 6], eax
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 20h], eax
        push   edi
        call   sub_1_1BC0
        add    esp, 4
        mov    dword ptr [edi + 100h], eax
        mov    eax, dword ptr [edi + 104h]
        db  08Bh, 00Dh, 030h, 037h, 000h, 000h
        or     eax, dword ptr [edi + 100h]
        mov    dword ptr [ecx], eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 4], 1
        db  0A1h, 084h, 037h, 000h, 000h
        mov    dword ptr [ebx + eax + 0eh], ecx
        push   0dh
        db  0A1h, 02Ch, 037h, 000h, 000h
        push   eax
        push   0
        push   esi
        push   edi
        call   sub_1_3162
        add    esp, 14h
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        cmp    edi, 103h
        je     SHORT loc_1_1C90
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1CA3
        push   esi
        call   sub_1_01B0
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_1C90:
        db  0A1h, 084h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + eax + 0eh], 0
        inc    dword ptr [ecx + 14h]
loc_1_1CA3:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_1CB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        db  080h, 03Dh, 038h, 037h, 000h, 000h, 000h
        jne    SHORT loc_1_1CF0
        db  0C6h, 005h, 038h, 037h, 000h, 000h, 001h
        push   ecx
        push   eax
        call   sub_1_1BE0
        add    esp, 8
        mov    esi, eax
        cmp    esi, 103h
        je     SHORT loc_1_1CE5
        call   sub_1_1F90
loc_1_1CE5:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1CF0:
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx], eax
        inc    dword ptr [eax + 14h]
        db  083h, 03Dh, 040h, 037h, 000h, 000h, 000h
        je     SHORT loc_1_1D20
        db  0A1h, 040h, 037h, 000h, 000h
        pop    esi
        mov    dword ptr [eax + 4], ecx
        db  089h, 00Dh, 040h, 037h, 000h, 000h
        mov    eax, 103h
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D20:
        db  089h, 00Dh, 040h, 037h, 000h, 000h
        db  089h, 00Dh, 03Ch, 037h, 000h, 000h
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D40:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [eax + 4]
        call   sub_1_0050
        mov    dword ptr [ebp - 10h], eax
        add    eax, eax
        mov    edi, dword ptr [ebp + 0ch]
        lea    ecx, [eax + eax*8]
        mov    dword ptr [ebp - 0ch], ecx
        db  0A1h, 084h, 037h, 000h, 000h
        mov    dword ptr [ecx + eax + 6], edi
loc_1_1D72:
        cmp    dword ptr [edi + 8], 0
        jne    SHORT loc_1_1DF0
        mov    eax, dword ptr [esi + 8]
        mov    ebx, dword ptr [eax + 38h]
        call   sub_1_34B7
        mov    ecx, ebx
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        and    ebx, 0ffffh
        add    eax, ebx
        cmp    word ptr [eax + 2], 0
        je     SHORT loc_1_1DE4
        add    eax, 4
        db  08Bh, 015h, 030h, 037h, 000h, 000h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 015h, 084h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [edx + ecx + 0eh], eax
        push   9
        db  0A1h, 02Ch, 037h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_3162
        add    esp, 14h
        jmp    SHORT loc_1_1E4D
loc_1_1DE4:
        mov    dword ptr [edi + 8], 1
        jmp    SHORT loc_1_1E59
        db  08Dh, 049h, 000h
loc_1_1DF0:
        mov    eax, dword ptr [edi + 20h]
        cmp    dword ptr [edi + 1ch], eax
        jbe    SHORT loc_1_1E63
        mov    edx, dword ptr [edi + 18h]
        add    edx, eax
        db  08Bh, 00Dh, 030h, 037h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ecx + 4], ax
        lea    eax, [ebp - 4]
        add    dword ptr [edi + 20h], 6
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 015h, 084h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [edx + ecx + 0eh], eax
        push   8
        db  0A1h, 02Ch, 037h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_3162
        add    esp, 14h
        and    eax, 0ffffh
        cmp    eax, 6
        jbe    SHORT loc_1_1E4D
        sub    eax, eax
loc_1_1E4D:
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
loc_1_1E59:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1D72
loc_1_1E63:
        cmp    dword ptr [ebp - 8], 103h
        je     SHORT loc_1_1E82
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1E82:
        db  0A1h, 084h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + ecx + 0eh], 0
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1EB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        mov    esi, dword ptr [edi + 4]
        je     SHORT loc_1_1EE8
        push   edi
        push   esi
        call   sub_1_0320
        add    esp, 8
        mov    eax, dword ptr [ebx + 18h]
        push   eax
        mov    ecx, 6
        mov    eax, dword ptr [ebx + 1ch]
        sub    edx, edx
        div    ecx
        push   eax
        push   edi
        push   esi
        call   sub_1_0390
        add    esp, 10h
loc_1_1EE8:
        push   ebx
        push   edi
        push   esi
        call   sub_1_0440
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_1F03
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_1F03:
        db  080h, 03Dh, 038h, 037h, 000h, 000h, 000h
        jne    SHORT loc_1_1F40
        db  0C6h, 005h, 038h, 037h, 000h, 000h, 001h
        push   esi
        push   edi
        call   sub_1_1D40
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 103h
        je     SHORT loc_1_1F40
        push   0
        push   esi
        call   sub_1_3456
        add    esp, 8
        call   sub_1_1F90
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_1F40:
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi], edi
        inc    dword ptr [edi + 14h]
        db  083h, 03Dh, 040h, 037h, 000h, 000h, 000h
        je     SHORT loc_1_1F70
        db  0A1h, 040h, 037h, 000h, 000h
        pop    edi
        mov    dword ptr [eax + 4], esi
        db  089h, 035h, 040h, 037h, 000h, 000h
        mov    eax, 103h
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_1F70:
        db  089h, 035h, 040h, 037h, 000h, 000h
        db  089h, 035h, 03Ch, 037h, 000h, 000h
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F90:
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 03Ch, 037h, 000h, 000h, 000h
        jne    SHORT loc_1_1FA7
        db  0C6h, 005h, 038h, 037h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1FA7:
        db  08Bh, 01Dh, 03Ch, 037h, 000h, 000h
        mov    esi, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [esi + 4]
        or     eax, eax
        db  0A3h, 03Ch, 037h, 000h, 000h
        jne    SHORT loc_1_1FC8
        db  0C7h, 005h, 040h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1FC8:
        cmp    dword ptr [edi + 24h], 1
        jne    SHORT loc_1_2035
        cmp    dword ptr [ebx + 10h], 1
        je     SHORT loc_1_1FE0
        mov    eax, 0c00000bbh
        jmp    SHORT loc_1_2043
        db  005h, 000h, 000h, 000h, 000h
loc_1_1FE0:
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1010103h
        je     SHORT loc_1_1FF8
        cmp    eax, 2010103h
        je     SHORT loc_1_2029
        mov    eax, 0c00000bbh
        jmp    SHORT loc_1_2043
loc_1_1FF8:
        push   ebx
        push   esi
        call   sub_1_1D40
        add    esp, 8
        cmp    eax, 103h
        jne    SHORT loc_1_204E
        db  08Bh, 00Dh, 03Ch, 037h, 000h, 000h
        mov    dword ptr [ebx + 4], ecx
        db  089h, 01Dh, 03Ch, 037h, 000h, 000h
        db  083h, 03Dh, 040h, 037h, 000h, 000h, 000h
        jne    SHORT loc_1_2043
        db  089h, 01Dh, 040h, 037h, 000h, 000h
        jmp    SHORT loc_1_2043
loc_1_2029:
        push   ebx
        push   esi
        call   sub_1_1BE0
        add    esp, 8
        jmp    SHORT loc_1_2043
loc_1_2035:
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0E8h, 090h, 0E2h, 0FFh, 0FFh
        add    esp, 4
loc_1_2043:
        cmp    eax, 103h
        je     NEAR PTR loc_1_20DA
loc_1_204E:
        mov    ecx, dword ptr [ebx + 14h]
        cmp    ecx, 1010103h
        je     SHORT loc_1_2063
        cmp    ecx, 2010103h
        je     SHORT loc_1_2082
        jmp    SHORT loc_1_20B3
loc_1_2063:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 1
        jne    SHORT loc_1_207B
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_3580
        jmp    SHORT loc_1_20B3
loc_1_207B:
        cmp    ecx, 4
        jne    SHORT loc_1_20B3
        jmp    SHORT loc_1_209C
loc_1_2082:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 1
        jne    SHORT loc_1_2097
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_3580
        jmp    SHORT loc_1_20B3
loc_1_2097:
        cmp    ecx, 3
        jne    SHORT loc_1_20B3
loc_1_209C:
        push   1
        push   esi
        push   edi
        call   sub_1_0900
        add    esp, 0ch
        push   0
        push   ebx
        call   sub_1_3456
        add    esp, 8
loc_1_20B3:
        db  083h, 03Dh, 03Ch, 037h, 000h, 000h, 000h
        jne    SHORT loc_1_20C3
        db  0C6h, 005h, 038h, 037h, 000h, 000h, 000h
loc_1_20C3:
        push   esi
        push   edi
        call   sub_1_2350
        add    esp, 8
        db  083h, 03Dh, 03Ch, 037h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1FA7
loc_1_20DA:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_20E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ecx + 4]
        mov    eax, dword ptr [edx + 18h]
        mov    ebx, dword ptr [edx + 14h]
        mov    edi, dword ptr [eax]
        sub    eax, eax
        cmp    ebx, 1010eh
        je     SHORT loc_1_2123
        cmp    ebx, 1010fh
        je     SHORT loc_1_2133
        cmp    ebx, 1010103h
        je     SHORT loc_1_2169
        cmp    ebx, 2010103h
        je     SHORT loc_1_2178
        mov    eax, 0c00000bbh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2123:
        push   edi
        push   edx
        push   ecx
        call   sub_1_1B00
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2133:
        cmp    edi, 100h
        jbe    SHORT loc_1_2145
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2145:
        cmp    dword ptr [esi + 110h], edi
        jae    SHORT loc_1_2182
        push   0eh
        push   edi
        push   edx
        push   ecx
        call   sub_1_1A10
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_2182
        mov    dword ptr [esi + 110h], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2169:
        push   edx
        push   ecx
        call   sub_1_1EB0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2178:
        push   edx
        push   ecx
        call   sub_1_1CB0
        add    esp, 8
loc_1_2182:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2190:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        je     SHORT loc_1_21C0
        cmp    ecx, 2
        je     SHORT loc_1_21C0
        cmp    ecx, 1
        jne    SHORT loc_1_21B7
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_20E0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_21B7:
        mov    eax, 10001h
        pop    ebp
        ret    8
loc_1_21C0:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19D0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_21D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 048h, 037h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_3550
        push   0
        db  0A1h, 028h, 037h, 000h, 000h
        push   eax
        call   sub_1_3544
        push   0
        db  0A1h, 044h, 037h, 000h, 000h
        push   eax
        call   sub_1_3456
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2210:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ecx, ecx
        db  08Bh, 035h, 054h, 037h, 000h, 000h
        cmp    esi, ecx
        je     SHORT loc_1_2230
        mov    eax, dword ptr [ebp + 8]
loc_1_2223:
        cmp    esi, eax
        je     SHORT loc_1_2230
        mov    ecx, esi
        mov    esi, dword ptr [esi + 44h]
        or     esi, esi
        jne    SHORT loc_1_2223
loc_1_2230:
        or     esi, esi
        je     SHORT loc_1_2272
        or     ecx, ecx
        mov    eax, dword ptr [esi + 44h]
        je     SHORT loc_1_2240
        mov    dword ptr [ecx + 44h], eax
        jmp    SHORT loc_1_2245
loc_1_2240:
        db  0A3h, 054h, 037h, 000h, 000h
loc_1_2245:
        mov    eax, dword ptr [esi]
        db  066h, 0C7h, 004h, 045h, 098h, 03Ah, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  0C7h, 004h, 085h, 0B8h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 40h]
        push   eax
        call   sub_1_355C
        push   0
        push   esi
        call   sub_1_3456
        add    esp, 8
loc_1_2272:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2280:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0ch], 5
        jne    SHORT loc_1_22B4
        mov    edi, dword ptr [eax + 10h]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_06B0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_1_2347
        push   0
        push   edi
        call   sub_1_3568
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_22B4:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 24h], 2
        jne    NEAR PTR loc_1_2347
        cmp    byte ptr [esi + 1ah], 0
        jne    NEAR PTR loc_1_2347
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 48h]
        cmp    eax, ecx
        je     SHORT loc_1_22DE
loc_1_22D4:
        add    ecx, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_1_22D4
loc_1_22DE:
        or     ecx, ecx
        jg     SHORT loc_1_2347
        push   esi
        call   sub_1_1430
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2300
        mov    byte ptr [esi + 1ah], 1
        mov    dword ptr [esi + 24h], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
loc_1_2300:
        inc    dword ptr [esi + 1ch]
        mov    edi, dword ptr [esi + 48h]
        or     edi, edi
        je     SHORT loc_1_2337
        mov    ebx, 1
        nop
loc_1_2310:
        cmp    dword ptr [edi + 0ch], ebx
        jne    SHORT loc_1_2330
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3598
loc_1_2330:
        mov    edi, dword ptr [edi + 8]
        or     edi, edi
        jne    SHORT loc_1_2310
loc_1_2337:
        push   esi
        call   sub_1_0710
        add    esp, 4
        mov    dword ptr [esi + 24h], 1
loc_1_2347:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_2350:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        dec    ecx
        mov    dword ptr [eax + 14h], ecx
        or     ecx, ecx
        jg     SHORT loc_1_236E
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2280
        add    esp, 8
loc_1_236E:
        pop    ebp
        ret
sub_1_2370:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        add    eax, eax
        sub    edi, edi
        lea    ecx, [eax + eax*8]
        db  0A1h, 084h, 037h, 000h, 000h
        mov    ebx, dword ptr [ecx + eax + 6]
        mov    dword ptr [ebp - 0ch], ecx
        mov    esi, dword ptr [ebx]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp + 0ch]
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        sub    ecx, 7
        cmp    ecx, 7
        ja     NEAR PTR loc_1_24C9
        db  0FFh, 024h, 08Dh, 008h, 025h, 000h, 000h
loc_1_23C8:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 1
        jne    SHORT loc_1_23E0
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_3580
        jmp    NEAR PTR loc_1_24C9
loc_1_23E0:
        cmp    ecx, 2
        jne    NEAR PTR loc_1_24C9
        push   1
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_0850
        add    esp, 10h
        jmp    NEAR PTR loc_1_24C9
        db  08Bh, 0FFh
loc_1_2400:
        or     eax, eax
        je     NEAR PTR loc_1_24C4
        db  08Bh, 015h, 040h, 037h, 000h, 000h
        db  08Bh, 00Dh, 03Ch, 037h, 000h, 000h
        mov    dword ptr [edx + 4], ebx
        db  039h, 01Dh, 03Ch, 037h, 000h, 000h
        jne    SHORT loc_1_2428
        mov    edx, dword ptr [ecx + 4]
        db  089h, 015h, 03Ch, 037h, 000h, 000h
loc_1_2428:
        cmp    ecx, ebx
        je     SHORT loc_1_2439
        lea    esp, [esp]
loc_1_2430:
        mov    edi, ecx
        mov    ecx, dword ptr [ecx + 4]
        cmp    ecx, ebx
        jne    SHORT loc_1_2430
loc_1_2439:
        db  08Bh, 015h, 040h, 037h, 000h, 000h
        mov    dword ptr [edx + 4], 0
        db  039h, 03Dh, 040h, 037h, 000h, 000h
        je     SHORT loc_1_246F
        db  039h, 00Dh, 040h, 037h, 000h, 000h
        jne    SHORT loc_1_2469
        db  089h, 03Dh, 040h, 037h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_246F
        mov    dword ptr [edi + 4], 0
        jmp    SHORT loc_1_246F
loc_1_2469:
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [edi + 4], ecx
loc_1_246F:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 1
        jne    SHORT loc_1_2487
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_3580
        jmp    SHORT loc_1_24B9
loc_1_2487:
        cmp    ecx, 4
        jne    SHORT loc_1_24C4
        jmp    SHORT loc_1_24AA
        db  08Bh, 0FFh
loc_1_2490:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 1
        jne    SHORT loc_1_24A5
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_3580
        jmp    SHORT loc_1_24C4
loc_1_24A5:
        cmp    ecx, 3
        jne    SHORT loc_1_24C4
loc_1_24AA:
        push   1
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0900
        add    esp, 0ch
loc_1_24B9:
        push   0
        push   ebx
        call   sub_1_3456
        add    esp, 8
loc_1_24C4:
        call   sub_1_1F90
loc_1_24C9:
        db  0A1h, 084h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + ecx + 0eh]
        or     eax, eax
        jne    SHORT loc_1_24DB
        jmp    SHORT loc_1_24E6
loc_1_24DB:
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_24F2
        mov    dword ptr [eax], 0
loc_1_24E6:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_01B0
        add    esp, 4
loc_1_24F2:
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2350
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 0C8h, 023h, 000h, 000h, 000h, 024h, 000h, 000h, 000h, 024h, 000h, 000h, 0C8h, 023h
        db  000h, 000h, 0C8h, 023h, 000h, 000h, 0C9h, 024h, 000h, 000h, 090h, 024h, 000h, 000h, 0C8h, 023h
        db  000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2530:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 1bh], 0
        je     SHORT loc_1_2598
        mov    byte ptr [esi + 1bh], 0
        inc    dword ptr [esi + 1ch]
        mov    edi, dword ptr [esi + 48h]
        or     edi, edi
        je     SHORT loc_1_258F
        lea    esp, [esp]
loc_1_2550:
        cmp    dword ptr [edi + 0ch], 1
        jne    SHORT loc_1_2588
        mov    eax, dword ptr [edi + 28h]
        test   al, 1
        je     SHORT loc_1_256C
        and    eax, 0fffffffeh
        mov    dword ptr [edi + 28h], eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_358C
loc_1_256C:
        cmp    dword ptr [edi + 0ch], 1
        jne    SHORT loc_1_2588
        mov    eax, dword ptr [edi + 28h]
        test   al, 2
        je     SHORT loc_1_2588
        and    eax, 0fffffffdh
        mov    dword ptr [edi + 28h], eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_358C
loc_1_2588:
        mov    edi, dword ptr [edi + 8]
        or     edi, edi
        jne    SHORT loc_1_2550
loc_1_258F:
        push   esi
        call   sub_1_0710
        add    esp, 4
loc_1_2598:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_25A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0c0000001h
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     SHORT loc_1_25D0
        cmp    eax, 3
        je     SHORT loc_1_2637
        cmp    eax, 5
        je     SHORT loc_1_25E5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_25D0:
        mov    edi, 40010006h
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [ebp + 8]
        jmp    SHORT loc_1_25F4
loc_1_25E5:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, 40010005h
        mov    dword ptr [ebx + 24h], 1
loc_1_25F4:
        inc    dword ptr [ebx + 1ch]
        mov    esi, dword ptr [ebx + 48h]
        or     esi, esi
        je     SHORT loc_1_262E
        mov    edi, edi
loc_1_2600:
        cmp    dword ptr [esi + 0ch], 1
        jne    SHORT loc_1_261F
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_3598
loc_1_261F:
        or     dword ptr [esi + 28h], 2
        mov    byte ptr [ebx + 1bh], 1
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        jne    SHORT loc_1_2600
loc_1_262E:
        push   ebx
        call   sub_1_0710
        add    esp, 4
loc_1_2637:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_2640:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ax, word ptr [ebp + 0ch]
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 8]
        and    edx, 0ffffh
        lea    eax, [edx*2]
        push   edx
        lea    eax, [eax + eax*8]
        db  08Bh, 00Dh, 084h, 037h, 000h, 000h
        mov    edi, dword ptr [eax + ecx + 2]
        mov    esi, dword ptr [edi + 24h]
        lea    ebx, [edi + 24h]
        call   sub_1_01B0
        add    esp, 4
        mov    eax, dword ptr [ebx + 8]
        push   eax
        call   sub_1_0960
        add    esp, 4
        db  08Bh, 00Dh, 06Ch, 037h, 000h, 000h
        mov    edx, dword ptr [ebx + 8]
        shl    edx, 5
        db  0A1h, 088h, 03Ah, 000h, 000h
        mov    dword ptr [edx + eax], ecx
        mov    eax, dword ptr [ebx + 8]
        db  0A3h, 06Ch, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_356E
        push   esi
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2350
        add    esp, 8
        call   sub_1_0BF0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_26E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 12ah]
        mov    ax, word ptr [ecx]
        or     ax, ax
        je     SHORT loc_1_2723
        mov    edx, ecx
        add    edx, 2
        and    eax, 0ffffh
loc_1_2715:
        xor    ecx, ecx
        mov    cx, word ptr [edx]
        add    edx, 6
        add    dword ptr [ebp - 4], ecx
        dec    eax
        jne    SHORT loc_1_2715
loc_1_2723:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    ecx, eax
        mov    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [esi + 114h]
        cmp    eax, 1
        jne    SHORT loc_1_2763
        mov    edx, 0eh
        test   byte ptr [ecx + 8], 80h
        je     SHORT loc_1_275D
        mov    dl, byte ptr [ecx + 0eh]
        and    edx, 1fh
        add    edx, 0eh
loc_1_275D:
        mov    dword ptr [esi + 118h], edx
loc_1_2763:
        mov    eax, dword ptr [esi + 118h]
        mov    dword ptr [ebp - 14h], eax
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    ax, word ptr [ecx + 2]
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, dword ptr [ebp - 14h]
        lea    edx, [esi + 120h]
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [esi + 11ch]
        add    edi, dword ptr [ebp - 10h]
        inc    dword ptr [esi + 1ch]
        lea    ecx, [edi + 6]
        mov    dword ptr [ebp - 0ch], 20h
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    SHORT loc_1_27C0
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_27C0
        mov    dword ptr [ebp - 0ch], 21h
        jmp    SHORT loc_1_2836
        db  090h
loc_1_27C0:
        db  0BAh, 0F8h, 03Ah, 000h, 000h
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    SHORT loc_1_27E0
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_27E0
        mov    dword ptr [ebp - 0ch], 28h
        jmp    SHORT loc_1_2836
        db  08Bh, 0FFh
loc_1_27E0:
        cmp    dword ptr [esi + 114h], 1
        je     SHORT loc_1_2815
        test   byte ptr [edi], 1
        je     SHORT loc_1_2836
        mov    dword ptr [ebp - 0ch], 24h
        push   edi
        mov    eax, dword ptr [esi + 0b8h]
        push   eax
        lea    eax, [esi + 98h]
        push   eax
        lea    eax, [esi + 54h]
        push   eax
        call   sub_1_33CA
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_2836
loc_1_2815:
        mov    al, byte ptr [edi]
        test   al, 80h
        je     SHORT loc_1_282D
        mov    dword ptr [ebp - 0ch], 1020h
        test   al, 40h
        je     SHORT loc_1_282D
        mov    dword ptr [ebp - 0ch], 3020h
loc_1_282D:
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_2836
        or     dword ptr [ebp - 0ch], 10h
loc_1_2836:
        mov    ebx, dword ptr [esi + 48h]
        or     ebx, ebx
        je     SHORT loc_1_28AF
loc_1_2840:
        cmp    dword ptr [ebx + 0ch], 1
        jne    SHORT loc_1_28A8
        mov    eax, dword ptr [ebp - 0ch]
        test   dword ptr [ebx + 1ch], eax
        jne    SHORT loc_1_287B
        mov    ecx, dword ptr [esi + 114h]
        cmp    ecx, 1
        je     SHORT loc_1_2873
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_286E
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + eax*4 + 0b8h]
        test   dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_287B
loc_1_286E:
        cmp    ecx, 1
        jne    SHORT loc_1_28A8
loc_1_2873:
        mov    eax, dword ptr [edi + 2]
        test   dword ptr [ebx + 20h], eax
        je     SHORT loc_1_28A8
loc_1_287B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_3586
        mov    byte ptr [esi + 1bh], 1
        or     dword ptr [ebx + 28h], 1
loc_1_28A8:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    SHORT loc_1_2840
loc_1_28AF:
        push   esi
        call   sub_1_0710
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_1_28C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        inc    dword ptr [esi + 12ah]
        mov    dword ptr [esi + 3ch], 0
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        jne    SHORT loc_1_28ED
        db  08Bh, 03Dh, 090h, 03Ah, 000h, 000h
loc_1_28ED:
        mov    eax, dword ptr [esi + 114h]
        cmp    eax, 1
        jne    SHORT loc_1_2915
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, 0eh
        test   byte ptr [ecx + 8], 80h
        je     SHORT loc_1_290F
        mov    dl, byte ptr [ecx + 0eh]
        and    edx, 1fh
        add    edx, 0eh
loc_1_290F:
        mov    dword ptr [esi + 118h], edx
loc_1_2915:
        mov    edx, dword ptr [esi + 118h]
        mov    dword ptr [ebp - 0ch], edx
        mov    eax, edx
        add    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, edx
        mov    dword ptr [ebp - 14h], eax
        sub    edi, edx
        mov    dword ptr [ebp - 18h], edi
        mov    dword ptr [esi + 3ch], 0
        mov    edi, dword ptr [esi + 11ch]
        lea    edx, [esi + 120h]
        add    edi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 1ch]
        lea    ecx, [edi + 6]
        mov    dword ptr [ebp - 8], 20h
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    SHORT loc_1_2970
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_2970
        mov    dword ptr [ebp - 8], 21h
        jmp    SHORT loc_1_29E6
        db  090h
loc_1_2970:
        db  0BAh, 0F8h, 03Ah, 000h, 000h
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    SHORT loc_1_2990
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_2990
        mov    dword ptr [ebp - 8], 28h
        jmp    SHORT loc_1_29E6
        db  08Bh, 0FFh
loc_1_2990:
        cmp    dword ptr [esi + 114h], 1
        je     SHORT loc_1_29C5
        test   byte ptr [edi], 1
        je     SHORT loc_1_29E6
        mov    dword ptr [ebp - 8], 24h
        push   edi
        mov    eax, dword ptr [esi + 0b8h]
        push   eax
        lea    eax, [esi + 98h]
        push   eax
        lea    eax, [esi + 54h]
        push   eax
        call   sub_1_33CA
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_29E6
loc_1_29C5:
        mov    al, byte ptr [edi]
        test   al, 80h
        je     SHORT loc_1_29DD
        mov    dword ptr [ebp - 8], 1020h
        test   al, 40h
        je     SHORT loc_1_29DD
        mov    dword ptr [ebp - 8], 3020h
loc_1_29DD:
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_29E6
        or     dword ptr [ebp - 8], 10h
loc_1_29E6:
        mov    ebx, dword ptr [esi + 48h]
        or     ebx, ebx
        je     SHORT loc_1_2A5D
loc_1_29F0:
        cmp    dword ptr [ebx + 0ch], 1
        jne    SHORT loc_1_2A56
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [ebx + 1ch], eax
        jne    SHORT loc_1_2A2B
        mov    ecx, dword ptr [esi + 114h]
        cmp    ecx, 1
        je     SHORT loc_1_2A23
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2A1E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + eax*4 + 0b8h]
        test   dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_2A2B
loc_1_2A1E:
        cmp    ecx, 1
        jne    SHORT loc_1_2A56
loc_1_2A23:
        mov    eax, dword ptr [edi + 2]
        test   dword ptr [ebx + 20h], eax
        je     SHORT loc_1_2A56
loc_1_2A2B:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_3586
        mov    byte ptr [esi + 1bh], 1
        or     dword ptr [ebx + 28h], 1
loc_1_2A56:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    SHORT loc_1_29F0
loc_1_2A5D:
        push   esi
        call   sub_1_0710
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_2A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_2E10
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    SHORT loc_1_2AA0
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_2AA0:
        push   0
        push   12eh
        call   sub_1_3414
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_2DF7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 24h], 0
        mov    byte ptr [esi + 1ah], 0
        mov    dword ptr [esi + 3ch], 0
        mov    byte ptr [esi + 1bh], 0
        mov    dword ptr [esi + 1ch], 0
        mov    dword ptr [esi + 110h], 40h
        mov    dword ptr [esi + 0b8h], 0
        mov    dword ptr [esi + 48h], 0
        mov    dword ptr [esi + 4ch], 0
        lea    edx, [eax*4]
        mov    dword ptr [esi + 50h], 0
        mov    dword ptr [esi + 0fch], 0
        mov    dword ptr [esi + 100h], 0
        mov    dword ptr [esi + 108h], 0ffffffffh
        mov    dword ptr [esi + 126h], 0
        mov    dword ptr [esi + 12ah], 0
        db  08Bh, 08Ah, 036h, 03Ah, 000h, 000h
        mov    eax, ecx
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        and    ecx, 0ffffh
        add    eax, ecx
        db  003h, 005h, 014h, 037h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        mov    cx, word ptr [eax + 20h]
        mov    eax, dword ptr [ebp - 4]
        db  066h, 089h, 00Ch, 045h, 098h, 03Ah, 000h, 000h
        db  089h, 0B2h, 0B8h, 03Ah, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 28h]
        mov    eax, ecx
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        and    ecx, 0ffffh
        add    eax, ecx
        db  003h, 005h, 014h, 037h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [eax + 38h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    ecx, eax
        db  003h, 00Dh, 014h, 037h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esi + 14h], ecx
        mov    eax, dword ptr [eax + 2ch]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    ecx, eax
        db  003h, 00Dh, 014h, 037h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 30h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    ecx, eax
        db  003h, 00Dh, 014h, 037h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esi + 10h], ecx
        mov    ax, word ptr [eax + 22h]
        mov    word ptr [esi + 18h], ax
        mov    eax, dword ptr [esi + 8]
        add    eax, 24h
        lea    edx, [esi + 120h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        db  0B9h, 0F0h, 037h, 000h, 000h
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [esi + 114h], 0
        mov    dword ptr [esi + 118h], 0eh
        mov    dword ptr [esi + 11ch], 0
        mov    eax, dword ptr [edx + 2]
        add    edx, 2
        cmp    eax, dword ptr [ecx]
        jne    SHORT loc_1_2C67
        mov    ax, word ptr [edx + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    SHORT loc_1_2C67
        mov    dword ptr [esi + 114h], 1
        mov    dword ptr [esi + 11ch], 2
        jmp    SHORT loc_1_2C98
loc_1_2C67:
        db  0B8h, 0F8h, 037h, 000h, 000h
        mov    ecx, dword ptr [edx]
        cmp    ecx, dword ptr [eax]
        jne    SHORT loc_1_2C98
        mov    cl, byte ptr [edx + 4]
        cmp    cl, byte ptr [eax + 4]
        jne    SHORT loc_1_2C98
        mov    dword ptr [esi + 114h], 2
        mov    dword ptr [esi + 118h], 0dh
        mov    dword ptr [esi + 11ch], 1
loc_1_2C98:
        mov    eax, dword ptr [esi + 114h]
        or     eax, eax
        jne    SHORT loc_1_2D09
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 38h]
        call   sub_1_34B7
        mov    ecx, edi
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        and    edi, 0ffffh
        lea    ecx, [eax + edi]
        sub    edx, edx
        lea    edi, [ecx + 4]
        cmp    word ptr [ecx + 2], dx
        jbe    SHORT loc_1_2D1A
        mov    edi, edi
loc_1_2CD0:
        mov    eax, dword ptr [esi + 0b8h]
        mov    ebx, dword ptr [edi]
        mov    dword ptr [esi + eax*4 + 54h], ebx
        mov    bx, word ptr [edi + 4]
        add    edi, 10h
        mov    word ptr [esi + eax*2 + 98h], bx
        mov    dword ptr [esi + eax*4 + 0bch], 80000000h
        inc    edx
        xor    eax, eax
        inc    dword ptr [esi + 0b8h]
        mov    ax, word ptr [ecx + 2]
        cmp    eax, edx
        jg     SHORT loc_1_2CD0
        jmp    SHORT loc_1_2D1A
loc_1_2D09:
        cmp    eax, 1
        jne    SHORT loc_1_2D1A
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 34h]
        mov    dword ptr [esi + 104h], eax
loc_1_2D1A:
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        add    eax, 0ch
        mov    ecx, eax
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0fffh
        xor    eax, edx
        sub    eax, edx
        sub    ecx, eax
        lea    eax, [ecx*2]
        cmp    eax, 1000h
        jge    SHORT loc_1_2D4E
        mov    eax, 1000h
loc_1_2D4E:
        push   4208h
        push   eax
        call   sub_1_340E
        add    esp, 8
        mov    dword ptr [esi + 2ch], eax
        or     eax, eax
        je     NEAR PTR loc_1_2DF7
        add    eax, 0ah
        mov    dword ptr [esi + 34h], eax
        add    eax, 2
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [esi + 2ch]
        mov    word ptr [eax], 1
        mov    eax, dword ptr [esi + 2ch]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 2ch]
        mov    ax, word ptr [eax + 44h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esi + 2ch]
        shl    eax, 0ch
        mov    dword ptr [esi + 30h], eax
        add    eax, 0ah
        mov    dword ptr [esi + 38h], eax
        mov    ecx, dword ptr [esi + 2ch]
        add    eax, 2
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        db  005h, 0B6h, 039h, 000h, 000h
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_2EC0
        add    esp, 8
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        db  0A1h, 048h, 037h, 000h, 000h
        push   eax
        lea    eax, [esi + 40h]
        push   eax
        call   sub_1_3556
        or     eax, eax
        jne    SHORT loc_1_2DF7
        db  0A1h, 054h, 037h, 000h, 000h
        mov    dword ptr [esi + 44h], eax
        db  089h, 035h, 054h, 037h, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2DF7:
        push   0
        push   esi
        call   sub_1_3456
        add    esp, 8
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_2E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        sub    eax, eax
        db  038h, 005h, 076h, 03Ah, 000h, 000h
        jbe    SHORT loc_1_2E99
        db  0C7h, 045h, 0FCh, 0B6h, 039h, 000h, 000h
loc_1_2E30:
        mov    ecx, dword ptr [ebp - 4]
loc_1_2E33:
        mov    bl, byte ptr [ecx]
        cmp    bl, 61h
        jb     SHORT loc_1_2E50
        cmp    bl, 7ah
        ja     SHORT loc_1_2E50
        mov    edi, ebx
        and    edi, 0ffh
        sub    edi, 20h
        jmp    SHORT loc_1_2E58
        db  08Dh, 064h, 024h, 000h
loc_1_2E50:
        mov    edi, ebx
        and    edi, 0ffh
loc_1_2E58:
        mov    dl, byte ptr [esi]
        cmp    dl, 61h
        jb     SHORT loc_1_2E70
        cmp    dl, 7ah
        ja     SHORT loc_1_2E70
        and    edx, 0ffh
        sub    edx, 20h
        jmp    SHORT loc_1_2E76
        db  090h
loc_1_2E70:
        and    edx, 0ffh
loc_1_2E76:
        cmp    edi, edx
        jne    SHORT loc_1_2E82
        or     bl, bl
        je     SHORT loc_1_2E82
        inc    ecx
        inc    esi
        jmp    SHORT loc_1_2E33
loc_1_2E82:
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [esi], cl
        je     SHORT loc_1_2E99
        add    dword ptr [ebp - 4], 10h
        inc    eax
        xor    ecx, ecx
        db  08Ah, 00Dh, 076h, 03Ah, 000h, 000h
        cmp    ecx, eax
        ja     SHORT loc_1_2E30
loc_1_2E99:
        xor    ecx, ecx
        db  08Ah, 00Dh, 076h, 03Ah, 000h, 000h
        cmp    ecx, eax
        ja     SHORT loc_1_2EAA
        mov    eax, 0ffffffffh
loc_1_2EAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2EC0:
        push   ebp
        mov    ebp, esp
        sub    dx, dx
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_2EC9:
        mov    ax, dx
        inc    dx
        and    eax, 0ffffh
        cmp    byte ptr [ecx + eax], 0
        jne    SHORT loc_1_2EC9
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 2], dx
        mov    word ptr [eax], dx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2EF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2E10
        add    esp, 4
        cmp    eax, -1
        jne    SHORT loc_1_2F08
        sub    al, al
        pop    ebp
        ret
loc_1_2F08:
        mov    al, 1
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
NDIS2SUP_Control:
        cmp    eax, 1ch
        jae    SHORT loc_1_2F1C
        db  0FFh, 024h, 085h, 01Eh, 02Fh, 000h, 000h
loc_1_2F1C:
        clc
        ret
        db  002h, 003h, 000h, 000h, 0AEh, 005h, 000h, 000h, 049h, 006h, 000h, 000h, 01Ch, 02Fh, 000h, 000h
        db  01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 09Ah, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h
        db  01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h
        db  01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h
        db  01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h
        db  01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h
        db  01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 01Ch, 02Fh, 000h, 000h, 0AEh, 005h, 000h, 000h
NDIS2SUP_Service_00:
        mov    eax, 30ah
        clc
        ret
NDIS2SUP_Service_01:
        jmp    NEAR PTR loc_1_2EF0
NDIS2SUP_SYS_CRITICAL_EXIT:
        pushfd
        cld
        cli
        mov    ecx, 6
        db  0BEh, 080h, 038h, 000h, 000h
        db  08Bh, 03Dh, 098h, 038h, 000h, 000h
        rep movsd
        popfd
        ret
sub_1_2FB1:
        push   edx
        movzx  edx, cx
        sub    ecx, edx
        shr    ecx, 0ch
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
        pop    edx
        ret
sub_1_2FC1:
        db  0BFh, 098h, 03Ah, 000h, 000h
        mov    ecx, 10h
        mov    ebx, ecx
        cld
        dec    ebx
        repne scasw
        jne    SHORT loc_1_2FEA
        sub    ebx, ecx
        mov    edi, ebx
        db  0B9h, 0B8h, 03Ah, 000h, 000h
        mov    ecx, dword ptr [ecx + edi*4]
        or     ecx, ecx
        je     SHORT loc_1_2FEA
        cmp    dword ptr [ecx + 24h], 0
        jne    SHORT loc_1_2FEC
loc_1_2FEA:
        xor    ecx, ecx
loc_1_2FEC:
        ret
loc_1_2FED:
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        call   sub_1_2FB1
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 6]
        push   ebx
        mov    bx, word ptr [ecx + 8]
        push   ebx
        mov    bx, word ptr [ecx + 0ah]
        push   ebx
        cld
        call   sub_1_2370
        add    esp, 0ch
        mov    eax, 0ch
        db  0CDh, 020h, 04Bh, 000h, 001h, 000h
        ret
loc_1_3023:
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        call   sub_1_2FB1
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 6]
        push   ebx
        mov    bx, word ptr [ecx + 8]
        push   ebx
        cld
        call   sub_1_2640
        add    esp, 8
        mov    eax, 0ah
        db  0CDh, 020h, 04Bh, 000h, 001h, 000h
        ret
loc_1_3054:
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        call   sub_1_2FB1
        mov    edi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [edi + 10h]
        push   ecx
        mov    cx, word ptr [edi + 0eh]
        push   ecx
        mov    ecx, dword ptr [edi + 0ah]
        call   sub_1_2FB1
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 12h]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     SHORT loc_1_3091
        cld
        call   sub_1_28C0
loc_1_3091:
        add    esp, 10h
        mov    eax, 10h
        db  0CDh, 020h, 04Bh, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 5
        ret
loc_1_30A6:
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        call   sub_1_2FB1
        xor    eax, eax
        mov    ax, word ptr [ecx + 6]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     SHORT loc_1_30CC
        cld
        call   sub_1_2530
loc_1_30CC:
        add    esp, 4
        mov    eax, 4
        db  0CDh, 020h, 04Bh, 000h, 001h, 000h
        ret
loc_1_30DB:
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        call   sub_1_2FB1
        mov    edi, ecx
        mov    ecx, dword ptr [edi + 0ah]
        call   sub_1_2FB1
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 12h]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     SHORT loc_1_310C
        cld
        call   sub_1_26E0
loc_1_310C:
        add    esp, 8
        mov    eax, 10h
        db  0CDh, 020h, 04Bh, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 5
        ret
loc_1_3121:
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        call   sub_1_2FB1
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 6]
        push   ebx
        mov    bx, word ptr [ecx + 0ch]
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [ecx + 0eh]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     SHORT loc_1_3153
        cld
        call   sub_1_25A0
loc_1_3153:
        add    esp, 0ch
        mov    eax, 0ch
        db  0CDh, 020h, 04Bh, 000h, 001h, 000h
        ret
sub_1_3162:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        and    word ptr [ebx + 2ch], 0fbffh
        db  066h, 0A1h, 09Eh, 038h, 000h, 000h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 14h]
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ecx, dword ptr [edx + 10h]
        mov    dx, word ptr [ecx + 4]
        mov    cx, word ptr [ecx + 6]
        and    edx, 0ffffh
        and    ecx, 0ffffh
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 1ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3222:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        db  0CDh, 020h, 086h, 001h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        and    word ptr [ebx + 2ch], 0fbffh
        db  066h, 0A1h, 09Eh, 038h, 000h, 000h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        rol    eax, 5
        db  003h, 005h, 088h, 03Ah, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ecx, dword ptr [edx + 10h]
        mov    dx, word ptr [ecx + 8]
        mov    cx, word ptr [ecx + 0ah]
        and    edx, 0ffffh
        and    ecx, 0ffffh
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 1ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_32E2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        and    word ptr [ebx + 2ch], 0fbffh
        mov    eax, dword ptr [ebp + 10h]
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 30h]
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        rol    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [edx + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ecx, dword ptr [edx + 10h]
        mov    dx, word ptr [ecx + 0ch]
        mov    cx, word ptr [ecx + 0eh]
        and    edx, 0ffffh
        and    ecx, 0ffffh
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 1ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3399:
        cld
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, 0ch
        jb     SHORT loc_1_33C5
        mov    eax, edi
        neg    eax
        and    eax, 3
        xchg   ecx, eax
        sub    eax, ecx
        rep movsb
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
loc_1_33C5:
        rep movsb
        pop    edi
        pop    esi
        ret
sub_1_33CA:
        push   ebx
        push   esi
        push   edi
        cld
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [edi + edx*4], eax
        xor    ecx, ecx
        dec    ecx
loc_1_33E2:
        repne scasd
        mov    ebx, ecx
        inc    ebx
        neg    ebx
        cmp    ebx, edx
        ja     SHORT loc_1_340A
        mov    edx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 1ch]
        mov    si, word ptr [esi + 4]
        cmp    si, word ptr [edx + ebx*2 - 2]
        mov    edx, dword ptr [esp + 18h]
        jne    SHORT loc_1_33E2
        mov    eax, ebx
loc_1_3406:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_340A:
        xor    eax, eax
        jmp    SHORT loc_1_3406
sub_1_340E:
        db  0CDh, 020h, 0A8h, 080h, 001h, 000h
sub_1_3414:
        mov    eax, dword ptr [esp + 4]
        add    eax, 4
        db  080h, 03Dh, 024h, 038h, 000h, 000h, 002h
        je     SHORT loc_1_343E
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_343D
        mov    dword ptr [eax], 0
        add    eax, 4
loc_1_343D:
        ret
loc_1_343E:
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_343D
        mov    dword ptr [eax], 1
        add    eax, 4
        ret
sub_1_3456:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_349C
        sub    dword ptr [esp + 4], 4
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx]
        and    eax, 0fffeh
        je     SHORT loc_1_3471
        jmp    SHORT loc_1_349C
loc_1_3471:
        db  080h, 03Dh, 024h, 038h, 000h, 000h, 002h
        je     SHORT loc_1_3480
        db  0CDh, 020h, 051h, 080h, 001h, 000h
loc_1_3480:
        mov    edx, dword ptr [esp + 4]
        push   esi
        push   ebx
        db  0BEh, 09Dh, 034h, 000h, 000h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ebx
        pop    esi
loc_1_349C:
        ret
loc_1_349D:
        push   0
        push   edx
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_34AF
        db  0CDh, 020h, 051h, 000h, 001h, 000h
loc_1_34AB:
        add    esp, 8
        ret
loc_1_34AF:
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        jmp    SHORT loc_1_34AB
sub_1_34B7:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 4]
        pop    ebx
        ret
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  06Eh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h
        db  024h, 008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh
        db  074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h
        db  00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh
        db  0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch
        db  08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_353E:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_3544:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_354A:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_3550:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_3556:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_355C:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_3568:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_356E:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_3580:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_3586:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_358C:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_3592:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_3598:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_3654:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_3678:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_369C:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
        db  0CDh, 020h, 005h, 080h, 028h, 000h
        db  0CDh, 020h, 006h, 080h, 028h, 000h
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
loc_1_3710:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3714:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3718:
        add    byte ptr [eax], al
loc_1_371A:
        add    byte ptr [eax], al
loc_1_371C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3720:
        add    byte ptr [eax], al
loc_1_3722:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3728:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_372C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3730:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3738:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_373C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3740:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3744:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3748:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_375C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3760:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3764:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_376C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3770:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3774:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3778:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_377C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_378C:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0e000101h
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [edx], eax
loc_1_37F0:
        cmp    byte ptr [eax], dh
        xor    ch, byte ptr [esi]
        xor    eax, 46000000h
        inc    esp
        inc    esp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    dl, byte ptr [ebx + 55h]
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_380C:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    dl, byte ptr [ebx + 55h]
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  08Eh, 02Fh, 000h, 000h, 095h, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 096h, 004h, 001h, 000h, 000h, 000h, 04Eh, 044h, 049h, 053h
        db  032h, 053h, 055h, 050h, 000h, 040h, 000h, 054h, 010h, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  018h, 038h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
_LTEXT ends
        end
