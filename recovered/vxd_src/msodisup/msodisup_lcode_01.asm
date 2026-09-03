; msodisup_lcode_01.asm -- MSODISUP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        or     ax, ax
        jne    SHORT loc_1_0010
        sub    eax, eax
        pop    ebp
        ret
loc_1_0010:
        and    ax, 7fffh
        cmp    ax, 0bh
        jbe    SHORT loc_1_0020
        sub    eax, eax
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_0020:
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 0F4h, 038h, 000h, 000h
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_0030:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 18h]
        dec    eax
        mov    dword ptr [ecx + 18h], eax
        jne    NEAR PTR loc_1_015A
        cmp    dword ptr [ecx + 14h], 5
        jne    NEAR PTR loc_1_00D9
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx]
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_1_0070
        mov    dword ptr [ecx + 14h], 0
        mov    dword ptr [esi + 34h], 1
        mov    eax, 103h
        jmp    SHORT loc_1_00C4
loc_1_0070:
        cmp    dword ptr [esi + 30h], 1
        jne    SHORT loc_1_007D
        mov    dword ptr [esi + 20h], 0
loc_1_007D:
        mov    edx, dword ptr [esi + 2ch]
        or     edx, edx
        je     SHORT loc_1_00AF
        cmp    edx, ecx
        jne    SHORT loc_1_0090
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esi + 2ch], eax
        jmp    SHORT loc_1_00AF
loc_1_0090:
        mov    eax, dword ptr [edx + 10h]
        or     eax, eax
        je     SHORT loc_1_00AF
loc_1_0097:
        cmp    eax, ecx
        je     SHORT loc_1_00A6
        mov    edx, eax
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_0097
        jmp    SHORT loc_1_00AF
loc_1_00A6:
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [edx + 10h], eax
        dec    dword ptr [esi + 30h]
loc_1_00AF:
        mov    eax, dword ptr [ecx + 1ch]
        or     dword ptr [esi + 0ech], eax
        push   0
        push   28h
        push   ecx
        call   sub_1_36FC
        sub    eax, eax
loc_1_00C4:
        or     eax, eax
        jne    NEAR PTR loc_1_015A
        push   0
        push   edi
        call   sub_1_3634
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_00D9:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 20h], 2
        jne    SHORT loc_1_015A
        cmp    dword ptr [esi + 38h], 0
        jne    SHORT loc_1_015A
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 2ch]
        cmp    eax, ecx
        je     SHORT loc_1_00FB
loc_1_00F1:
        add    ecx, dword ptr [eax + 18h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_00F1
loc_1_00FB:
        or     ecx, ecx
        jne    SHORT loc_1_015A
        push   esi
        call   sub_1_323D
        add    esp, 4
        mov    dword ptr [esi + 38h], 1
        mov    dword ptr [esi + 20h], 1
        inc    dword ptr [esi + 18h]
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_1_014A
        mov    ebx, 1
loc_1_0125:
        cmp    dword ptr [edi + 14h], ebx
        jne    SHORT loc_1_0143
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
loc_1_0143:
        mov    edi, dword ptr [edi + 10h]
        or     edi, edi
        jne    SHORT loc_1_0125
loc_1_014A:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        mov    dword ptr [esi + 20h], 1
loc_1_015A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_1_0160:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 6
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    ax, word ptr [eax + ecx + 32h]
        and    eax, 0ffffh
        sub    eax, 4
        db  023h, 005h, 06Ch, 038h, 000h, 000h
        db  08Bh, 00Dh, 074h, 038h, 000h, 000h
        mov    word ptr [eax + ecx + 2], 7569h
        db  039h, 005h, 064h, 038h, 000h, 000h
        jne    SHORT loc_1_01EF
        db  083h, 03Dh, 0D8h, 038h, 000h, 000h, 000h
        je     SHORT loc_1_01EF
        mov    edx, 7569h
        sub    esi, esi
loc_1_01A8:
        db  0A1h, 074h, 038h, 000h, 000h
        db  003h, 005h, 064h, 038h, 000h, 000h
        cmp    word ptr [eax + 2], dx
        jne    SHORT loc_1_01EF
        db  0FFh, 00Dh, 0D8h, 038h, 000h, 000h
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 064h, 038h, 000h, 000h
        add    ecx, eax
        add    ecx, 4
        db  023h, 00Dh, 06Ch, 038h, 000h, 000h
        db  089h, 00Dh, 064h, 038h, 000h, 000h
        add    eax, 4
        db  001h, 005h, 060h, 038h, 000h, 000h
        db  039h, 035h, 0D8h, 038h, 000h, 000h
        jne    SHORT loc_1_01A8
loc_1_01EF:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0200:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 6
        db  08Bh, 00Dh, 094h, 038h, 000h, 000h
        mov    ax, word ptr [eax + ecx + 2ah]
        and    eax, 0ffffh
        sub    eax, 4
        db  023h, 005h, 088h, 038h, 000h, 000h
        db  08Bh, 00Dh, 090h, 038h, 000h, 000h
        mov    word ptr [eax + ecx + 2], 7569h
        db  039h, 005h, 080h, 038h, 000h, 000h
        jne    SHORT loc_1_028F
        db  083h, 03Dh, 0DCh, 038h, 000h, 000h, 000h
        je     SHORT loc_1_028F
        mov    edx, 7569h
        sub    esi, esi
loc_1_0248:
        db  0A1h, 080h, 038h, 000h, 000h
        db  003h, 005h, 090h, 038h, 000h, 000h
        cmp    word ptr [eax + 2], dx
        jne    SHORT loc_1_028F
        db  0FFh, 00Dh, 0DCh, 038h, 000h, 000h
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 080h, 038h, 000h, 000h
        add    ecx, eax
        add    ecx, 4
        db  023h, 00Dh, 088h, 038h, 000h, 000h
        db  089h, 00Dh, 080h, 038h, 000h, 000h
        add    eax, 4
        db  001h, 005h, 07Ch, 038h, 000h, 000h
        db  039h, 035h, 0DCh, 038h, 000h, 000h
        jne    SHORT loc_1_0248
loc_1_028F:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_02A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3
        and    eax, 0fffffffch
        mov    dword ptr [ebp - 18h], eax
        or     eax, 55490000h
        db  08Bh, 00Dh, 074h, 038h, 000h, 000h
        db  08Bh, 015h, 068h, 038h, 000h, 000h
        mov    dword ptr [ecx + edx], eax
        db  0A1h, 068h, 038h, 000h, 000h
        add    eax, 4
        db  023h, 005h, 06Ch, 038h, 000h, 000h
        db  0A3h, 068h, 038h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        db  029h, 005h, 060h, 038h, 000h, 000h
        mov    esi, dword ptr [ebp + 10h]
        shl    esi, 6
        db  0A1h, 068h, 038h, 000h, 000h
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    word ptr [esi + ecx + 32h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 10h]
        push   0
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3720
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 03Dh, 074h, 038h, 000h, 000h
        db  003h, 03Dh, 068h, 038h, 000h, 000h
        db  0A1h, 064h, 038h, 000h, 000h
        db  039h, 005h, 068h, 038h, 000h, 000h
        jb     SHORT loc_1_0340
        db  08Bh, 01Dh, 070h, 038h, 000h, 000h
        jmp    SHORT loc_1_0346
        db  090h
loc_1_0340:
        db  08Bh, 01Dh, 064h, 038h, 000h, 000h
loc_1_0346:
        db  02Bh, 01Dh, 068h, 038h, 000h, 000h
        mov    cx, word ptr [ebp + 0ch]
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 2eh], cx
        cmp    dword ptr [ebp + 0ch], ebx
        ja     SHORT loc_1_0380
        mov    ebx, dword ptr [ebp + 0ch]
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 30h], 1
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 36h], cx
        jmp    SHORT loc_1_03A6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0380:
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 30h], 2
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 36h], bx
        sub    cx, bx
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 3ch], cx
        mov    dword ptr [ebp - 14h], ebx
loc_1_03A6:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_03F0
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_3744
        mov    eax, dword ptr [ebp - 1ch]
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ecx + esi + 21h], al
        mov    eax, dword ptr [ebp - 1ch]
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ecx + esi + 19h], al
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 12h], 0
        jmp    SHORT loc_1_03FC
        db  005h, 000h, 000h, 000h, 000h
loc_1_03F0:
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 12h], 0ffffh
loc_1_03FC:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_047D
loc_1_0402:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0426
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_3744
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_3768
loc_1_0426:
        cmp    ebx, dword ptr [ebp - 4]
        jl     SHORT loc_1_0454
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        call   sub_1_3508
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_0477
loc_1_0454:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        call   sub_1_3508
        add    esp, 0ch
        sub    dword ptr [ebp - 8], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
        mov    ebx, dword ptr [ebp + 0ch]
        sub    ebx, dword ptr [ebp - 14h]
        db  08Bh, 03Dh, 074h, 038h, 000h, 000h
loc_1_0477:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_0402
loc_1_047D:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_04A3
        db  0A1h, 078h, 038h, 000h, 000h
        sub    word ptr [esi + eax + 36h], 3
        db  0A1h, 078h, 038h, 000h, 000h
        add    dword ptr [esi + eax + 32h], 3
        db  0A1h, 078h, 038h, 000h, 000h
        sub    word ptr [esi + eax + 2eh], 3
loc_1_04A3:
        mov    eax, dword ptr [ebp - 18h]
        db  003h, 005h, 068h, 038h, 000h, 000h
        db  023h, 005h, 06Ch, 038h, 000h, 000h
        db  0A3h, 068h, 038h, 000h, 000h
        db  0FFh, 005h, 0D8h, 038h, 000h, 000h
        db  0A1h, 064h, 038h, 000h, 000h
        db  039h, 005h, 068h, 038h, 000h, 000h
        jbe    SHORT loc_1_0508
        db  0A1h, 070h, 038h, 000h, 000h
        db  02Bh, 005h, 068h, 038h, 000h, 000h
        cmp    eax, 34h
        jge    SHORT loc_1_0508
        lea    edx, [eax - 4]
        or     edx, 75690000h
        db  08Bh, 035h, 074h, 038h, 000h, 000h
        db  08Bh, 00Dh, 068h, 038h, 000h, 000h
        mov    dword ptr [esi + ecx], edx
        db  029h, 005h, 060h, 038h, 000h, 000h
        db  0C7h, 005h, 068h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 0D8h, 038h, 000h, 000h
loc_1_0508:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_1_0510:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 035h, 084h, 038h, 000h, 000h
        lea    ecx, [edx + 3]
        and    ecx, 0fffffffch
        mov    eax, ecx
        or     eax, 55490000h
        db  08Bh, 03Dh, 090h, 038h, 000h, 000h
        mov    dword ptr [esi + edi], eax
        db  0A1h, 084h, 038h, 000h, 000h
        add    eax, 4
        db  023h, 005h, 088h, 038h, 000h, 000h
        db  0A3h, 084h, 038h, 000h, 000h
        lea    eax, [ecx + 4]
        db  029h, 005h, 07Ch, 038h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        shl    esi, 6
        db  0A1h, 084h, 038h, 000h, 000h
        db  08Bh, 03Dh, 094h, 038h, 000h, 000h
        mov    word ptr [esi + edi + 32h], ax
        db  0A1h, 080h, 038h, 000h, 000h
        db  039h, 005h, 084h, 038h, 000h, 000h
        jb     SHORT loc_1_0580
        db  08Bh, 03Dh, 08Ch, 038h, 000h, 000h
        jmp    SHORT loc_1_0586
        db  005h, 000h, 000h, 000h, 000h
loc_1_0580:
        db  08Bh, 03Dh, 080h, 038h, 000h, 000h
loc_1_0586:
        db  02Bh, 03Dh, 084h, 038h, 000h, 000h
        cmp    edx, edi
        db  0A1h, 094h, 038h, 000h, 000h
        jg     SHORT loc_1_05A8
        mov    word ptr [esi + eax + 30h], 1
        db  0A1h, 094h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 36h], dx
        jmp    SHORT loc_1_05C6
loc_1_05A8:
        mov    word ptr [esi + eax + 30h], 2
        db  0A1h, 094h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 36h], di
        sub    dx, di
        db  0A1h, 094h, 038h, 000h, 000h
        mov    word ptr [esi + eax + 3ch], dx
loc_1_05C6:
        db  0A1h, 084h, 038h, 000h, 000h
        add    eax, ecx
        db  023h, 005h, 088h, 038h, 000h, 000h
        db  0A3h, 084h, 038h, 000h, 000h
        db  0FFh, 005h, 0DCh, 038h, 000h, 000h
        db  0A1h, 080h, 038h, 000h, 000h
        db  039h, 005h, 084h, 038h, 000h, 000h
        jbe    SHORT loc_1_062B
        db  0A1h, 08Ch, 038h, 000h, 000h
        db  02Bh, 005h, 084h, 038h, 000h, 000h
        cmp    eax, 104h
        jge    SHORT loc_1_062B
        lea    ecx, [eax - 4]
        or     ecx, 75690000h
        db  08Bh, 015h, 084h, 038h, 000h, 000h
        db  08Bh, 035h, 090h, 038h, 000h, 000h
        mov    dword ptr [edx + esi], ecx
        db  029h, 005h, 07Ch, 038h, 000h, 000h
        db  0C7h, 005h, 084h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 0DCh, 038h, 000h, 000h
loc_1_062B:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h
sub_1_0630:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 14h], 0
        db  08Bh, 00Dh, 0E0h, 038h, 000h, 000h
        movsx  eax, word ptr [eax + 10h]
        mov    esi, dword ptr [ecx + eax*4]
        db  0FFh, 005h, 024h, 039h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        sub    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 20h], eax
        mov    edi, dword ptr [esi + 0f4h]
        cmp    edi, 1
        jne    SHORT loc_1_06A6
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, 0eh
        test   byte ptr [eax + 8], 80h
        je     SHORT loc_1_0691
        mov    dl, byte ptr [eax + 0eh]
        and    edx, 1fh
        add    edx, 0eh
loc_1_0691:
        cmp    edi, 7
        mov    dword ptr [esi + 0f8h], 3
        je     SHORT loc_1_06A6
        mov    dword ptr [esi + 0f8h], edx
loc_1_06A6:
        cmp    dword ptr [esi + 106h], 0
        jne    SHORT loc_1_0712
        mov    eax, dword ptr [esi + 0f4h]
        or     eax, eax
        jne    SHORT loc_1_06E1
        mov    eax, dword ptr [ebp + 0ch]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0ch]
        shl    ecx, 8
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 5dch
        jle    SHORT loc_1_071C
        mov    eax, dword ptr [esi + 8]
        movsx  eax, word ptr [eax + 28h]
        jmp    SHORT loc_1_0719
loc_1_06E1:
        cmp    eax, 7
        jne    SHORT loc_1_0709
        mov    al, byte ptr [ecx - 3]
        or     al, al
        jne    SHORT loc_1_06F6
        mov    dword ptr [ebp - 8], 200h
        jmp    SHORT loc_1_071C
loc_1_06F6:
        and    eax, 0ffh
        dec    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        add    eax, 2
        shl    eax, 9
        jmp    SHORT loc_1_0719
loc_1_0709:
        mov    eax, dword ptr [esi + 8]
        movsx  eax, word ptr [eax + 28h]
        jmp    SHORT loc_1_0719
loc_1_0712:
        mov    eax, dword ptr [ebp + 10h]
        movsx  eax, word ptr [eax + 12h]
loc_1_0719:
        mov    dword ptr [ebp - 8], eax
loc_1_071C:
        mov    ecx, dword ptr [ebp - 20h]
        sub    ecx, dword ptr [esi + 0f8h]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        add    eax, ecx
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [esi + 0f4h], 7
        je     SHORT loc_1_0742
        mov    eax, dword ptr [ebp - 8]
        add    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 8], eax
loc_1_0742:
        mov    eax, dword ptr [ebp - 8]
        db  03Bh, 005h, 07Ch, 038h, 000h, 000h
        jge    NEAR PTR loc_1_0A51
        db  083h, 03Dh, 0ECh, 038h, 000h, 000h, 0FFh
        je     NEAR PTR loc_1_0A51
        inc    dword ptr [esi + 18h]
        mov    edx, dword ptr [esi + 0fch]
        add    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], edx
        mov    ecx, edx
        add    ecx, 6
        mov    edi, 20h
        cmp    dword ptr [esi + 0f4h], 7
        jne    SHORT loc_1_07A6
        mov    al, byte ptr [edx]
        or     al, al
        jne    SHORT loc_1_0790
        mov    edi, 28h
        jmp    NEAR PTR loc_1_0837
loc_1_0790:
        cmp    byte ptr [esi + 105h], al
        jne    NEAR PTR loc_1_0837
        mov    edi, 21h
        jmp    NEAR PTR loc_1_0837
loc_1_07A6:
        mov    eax, dword ptr [ebp - 10h]
        lea    edx, [esi + 100h]
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    SHORT loc_1_07C6
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_07C6
        mov    edi, 21h
        jmp    SHORT loc_1_0837
loc_1_07C6:
        mov    eax, dword ptr [ebp - 10h]
        db  0BAh, 0B4h, 039h, 000h, 000h
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    SHORT loc_1_07E5
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_07E5
        mov    edi, 28h
        jmp    SHORT loc_1_0837
loc_1_07E5:
        cmp    dword ptr [esi + 0f4h], 1
        mov    edx, dword ptr [ebp - 10h]
        je     SHORT loc_1_081B
        test   byte ptr [edx], 1
        je     SHORT loc_1_0837
        mov    edi, 24h
        push   edx
        mov    eax, dword ptr [esi + 0a0h]
        push   eax
        lea    eax, [esi + 80h]
        push   eax
        lea    eax, [esi + 3ch]
        push   eax
        call   sub_1_3539
        add    esp, 10h
        mov    dword ptr [ebp - 14h], eax
        jmp    SHORT loc_1_0837
loc_1_081B:
        mov    al, byte ptr [edx]
        test   al, 80h
        je     SHORT loc_1_082F
        mov    edi, 1020h
        test   al, 40h
        je     SHORT loc_1_082F
        mov    edi, 3020h
loc_1_082F:
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_0837
        or     edi, 10h
loc_1_0837:
        mov    ebx, dword ptr [esi + 2ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_08F4
loc_1_0842:
        cmp    dword ptr [ebx + 14h], 1
        jne    NEAR PTR loc_1_08E9
        test   dword ptr [ebx + 20h], edi
        jne    SHORT loc_1_0886
        mov    ecx, dword ptr [esi + 0f4h]
        cmp    ecx, 1
        je     SHORT loc_1_087B
        cmp    ecx, 7
        je     SHORT loc_1_0876
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_0876
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [esi + eax*4 + 0a0h]
        test   dword ptr [ebx + 1ch], eax
        jne    SHORT loc_1_0886
loc_1_0876:
        cmp    ecx, 1
        jne    SHORT loc_1_08E9
loc_1_087B:
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebx + 24h]
        test   dword ptr [edx + 2], eax
        je     SHORT loc_1_08E9
loc_1_0886:
        mov    eax, dword ptr [ebp + 10h]
        movsx  eax, word ptr [eax + 8]
        cmp    eax, dword ptr [ebp - 0ch]
        jb     SHORT loc_1_08E0
        db  083h, 03Dh, 0F0h, 038h, 000h, 000h, 000h
        je     SHORT loc_1_08B0
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_08B0
        mov    dword ptr [ebp - 1ch], 1
        jmp    SHORT loc_1_08E0
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_08B0:
        mov    ecx, dword ptr [esi + 0f8h]
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, ecx
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_1_3652
        push   ebx
        call   sub_1_2D1B
        add    esp, 4
        jmp    SHORT loc_1_08E9
        db  090h
loc_1_08E0:
        mov    eax, dword ptr [ebp - 4]
        or     eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ebp - 4], eax
loc_1_08E9:
        mov    ebx, dword ptr [ebx + 10h]
        or     ebx, ebx
        jne    NEAR PTR loc_1_0842
loc_1_08F4:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0A42
        db  08Bh, 01Dh, 0ECh, 038h, 000h, 000h
        mov    edi, ebx
        shl    edi, 6
        db  08Bh, 00Dh, 094h, 038h, 000h, 000h
        add    ecx, edi
        db  0FFh, 005h, 0F0h, 038h, 000h, 000h
        mov    eax, dword ptr [ecx + 4]
        db  0A3h, 0ECh, 038h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0510
        db  0A1h, 094h, 038h, 000h, 000h
        add    eax, edi
        add    esp, 8
        cmp    dword ptr [ebp - 1ch], 0
        mov    cx, word ptr [eax + 32h]
        mov    word ptr [eax + 2ah], cx
        mov    eax, dword ptr [ebp + 10h]
        mov    cx, word ptr [eax + 10h]
        db  0A1h, 094h, 038h, 000h, 000h
        mov    word ptr [edi + eax + 1ah], cx
        je     SHORT loc_1_0970
        cmp    dword ptr [ebp - 0ch], 80h
        jbe    SHORT loc_1_09A0
        mov    dword ptr [ebp - 1ch], 0
loc_1_0970:
        db  0A1h, 094h, 038h, 000h, 000h
        xor    ecx, ecx
        mov    cx, word ptr [edi + eax + 32h]
        db  003h, 00Dh, 090h, 038h, 000h, 000h
        cmp    dword ptr [esi + 0f4h], 7
        je     SHORT loc_1_09F0
        mov    edx, dword ptr [ebp - 20h]
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        call   sub_1_3508
        add    esp, 0ch
        jmp    SHORT loc_1_0A09
        db  08Bh, 0FFh
loc_1_09A0:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0A1h, 094h, 038h, 000h, 000h
        shl    bx, 8
        mov    ax, word ptr [edi + eax + 32h]
        and    eax, 0ffffh
        db  003h, 005h, 090h, 038h, 000h, 000h
        push   eax
        call   sub_1_3508
        add    esp, 0ch
        add    bx, 51h
        db  0A1h, 094h, 038h, 000h, 000h
        mov    ax, word ptr [edi + eax + 32h]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 090h, 038h, 000h, 000h
        mov    word ptr [eax + ecx - 2], bx
        jmp    SHORT loc_1_0A09
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_09F0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 20h], 3
        mov    ax, word ptr [eax]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0fh]
        mov    byte ptr [ecx + 2], al
loc_1_0A09:
        db  0A1h, 094h, 038h, 000h, 000h
        mov    edx, dword ptr [ebp - 20h]
        add    dword ptr [edi + eax + 32h], edx
        db  0A1h, 094h, 038h, 000h, 000h
        sub    word ptr [edi + eax + 36h], dx
        db  0FFh, 00Dh, 024h, 039h, 000h, 000h
        cmp    dword ptr [ebp - 1ch], 0
        jne    SHORT loc_1_0A39
        db  0A1h, 094h, 038h, 000h, 000h
        add    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0A39:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0A42:
        db  0FFh, 00Dh, 024h, 039h, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0A51:
        db  0FFh, 00Dh, 024h, 039h, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_0A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    esp, [esp]
loc_1_0A70:
        db  08Bh, 00Dh, 0E0h, 038h, 000h, 000h
        movsx  eax, word ptr [esi + 1ah]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [esi]
        movsx  ecx, word ptr [esi + 2ah]
        db  0A1h, 090h, 038h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        movsx  eax, word ptr [esi + 3eh]
        mov    dword ptr [ebp - 14h], eax
        db  02Bh, 00Dh, 080h, 038h, 000h, 000h
        cmp    ecx, 4
        jne    NEAR PTR loc_1_0C28
        cmp    word ptr [esi + 0ch], 0
        je     SHORT loc_1_0AD8
        push   eax
        call   sub_1_0200
        add    esp, 4
        db  08Bh, 00Dh, 0ECh, 038h, 000h, 000h
        mov    edx, dword ptr [ebp - 14h]
        shl    edx, 6
        db  0A1h, 094h, 038h, 000h, 000h
        mov    dword ptr [edx + eax + 4], ecx
        mov    eax, dword ptr [ebp - 14h]
        db  0A3h, 0ECh, 038h, 000h, 000h
        jmp    NEAR PTR loc_1_0BF6
loc_1_0AD8:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0f4h]
        cmp    eax, 1
        jne    SHORT loc_1_0B17
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, 0eh
        test   byte ptr [edx + 8], 80h
        je     SHORT loc_1_0AFD
        mov    cl, byte ptr [edx + 0eh]
        and    ecx, 1fh
        add    ecx, 0eh
loc_1_0AFD:
        cmp    eax, 7
        mov    eax, dword ptr [ebp - 8]
        jne    SHORT loc_1_0B11
        mov    dword ptr [eax + 0f8h], 3
        jmp    SHORT loc_1_0B17
loc_1_0B11:
        mov    dword ptr [eax + 0f8h], ecx
loc_1_0B17:
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    eax, dword ptr [eax + 0f8h]
        mov    dword ptr [ebp - 0ch], eax
        movsx  eax, word ptr [esi + 2ah]
        mov    cx, word ptr [esi + 32h]
        sub    ecx, eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 1ch], eax
        movsx  eax, word ptr [esi + 36h]
        sub    ecx, dword ptr [ebp - 0ch]
        add    ecx, eax
        mov    dword ptr [ebp - 10h], ecx
        mov    dword ptr [ebp - 18h], ecx
        cmp    word ptr [esi + 30h], 2
        jne    SHORT loc_1_0B57
        movsx  eax, word ptr [esi + 3ch]
        add    eax, ecx
        mov    dword ptr [ebp - 18h], eax
loc_1_0B57:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 18h]
        mov    ebx, dword ptr [eax + 2ch]
        or     ebx, ebx
        je     SHORT loc_1_0B97
loc_1_0B64:
        cmp    dword ptr [ebx + 14h], 1
        jne    SHORT loc_1_0B90
        test   dword ptr [ebx + 1ch], edi
        je     SHORT loc_1_0B90
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   edx
        push   esi
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_3652
loc_1_0B90:
        mov    ebx, dword ptr [ebx + 10h]
        or     ebx, ebx
        jne    SHORT loc_1_0B64
loc_1_0B97:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0200
        add    esp, 4
        db  0A1h, 0ECh, 038h, 000h, 000h
        mov    ecx, dword ptr [ebp - 14h]
        shl    ecx, 6
        db  08Bh, 015h, 094h, 038h, 000h, 000h
        mov    dword ptr [ecx + edx + 4], eax
        mov    eax, dword ptr [ebp - 14h]
        db  0A3h, 0ECh, 038h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 2ch]
        or     esi, esi
        je     SHORT loc_1_0BEA
        mov    ebx, 1
        nop
loc_1_0BD0:
        cmp    dword ptr [esi + 14h], ebx
        jne    SHORT loc_1_0BE3
        test   dword ptr [esi + 1ch], edi
        je     SHORT loc_1_0BE3
        push   esi
        call   sub_1_2D1B
        add    esp, 4
loc_1_0BE3:
        mov    esi, dword ptr [esi + 10h]
        or     esi, esi
        jne    SHORT loc_1_0BD0
loc_1_0BEA:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0EF0
        add    esp, 4
loc_1_0BF6:
        db  0FFh, 00Dh, 0F0h, 038h, 000h, 000h
        db  083h, 03Dh, 0DCh, 038h, 000h, 000h, 000h
        je     SHORT loc_1_0C3A
        db  0A1h, 080h, 038h, 000h, 000h
        db  003h, 005h, 090h, 038h, 000h, 000h
        cmp    byte ptr [eax + 2], 51h
        jne    SHORT loc_1_0C3A
        movzx  esi, byte ptr [eax + 3]
        shl    esi, 6
        db  003h, 035h, 094h, 038h, 000h, 000h
        jmp    NEAR PTR loc_1_0A70
loc_1_0C28:
        mov    eax, dword ptr [ebp - 14h]
        shl    ax, 8
        mov    edx, dword ptr [ebp - 4]
        add    ax, 51h
        mov    word ptr [edx - 2], ax
loc_1_0C3A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 0E0h, 038h, 000h, 000h
        movsx  eax, word ptr [edx + 1ah]
        mov    edi, dword ptr [ecx + eax*4]
        movsx  eax, word ptr [edx + 3eh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edx]
        push   0
        mov    esi, dword ptr [eax + 24h]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_363A
        mov    ecx, dword ptr [edi + 0f4h]
        cmp    ecx, 7
        mov    ebx, dword ptr [ebp - 4]
        jne    SHORT loc_1_0CE2
        shl    ebx, 6
        db  0A1h, 078h, 038h, 000h, 000h
        mov    word ptr [ebx + eax + 12h], 0ffffh
        db  0A1h, 078h, 038h, 000h, 000h
        sub    dword ptr [ebx + eax + 32h], 3
        db  0A1h, 078h, 038h, 000h, 000h
        add    word ptr [ebx + eax + 36h], 3
        db  0A1h, 078h, 038h, 000h, 000h
        add    word ptr [ebx + eax + 2eh], 3
        db  0A1h, 078h, 038h, 000h, 000h
        lea    ecx, [ebx + eax]
        mov    al, byte ptr [ecx + 21h]
        or     al, al
        je     SHORT loc_1_0CDF
        xor    edx, edx
        mov    dl, byte ptr [edi + 105h]
        movsx  eax, al
        cmp    edx, eax
        jne    NEAR PTR loc_1_0D5D
loc_1_0CDF:
        push   ecx
        jmp    SHORT loc_1_0D54
loc_1_0CE2:
        shl    ebx, 6
        db  0A1h, 078h, 038h, 000h, 000h
        mov    eax, dword ptr [ebx + eax + 32h]
        mov    edx, eax
        and    edx, 0ffff0fffh
        shr    edx, 0ch
        add    edx, dword ptr [edi + 0fch]
        and    eax, 0ffffh
        add    eax, edx
        test   byte ptr [edi + 0e4h], 20h
        jne    SHORT loc_1_0D4C
        cmp    ecx, 1
        je     SHORT loc_1_0D31
        test   byte ptr [eax], 1
        jne    SHORT loc_1_0D4C
        lea    ecx, [edi + 100h]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_0D2D
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
loc_1_0D2D:
        je     SHORT loc_1_0D4C
        jmp    SHORT loc_1_0D5D
loc_1_0D31:
        test   byte ptr [eax], 80h
        jne    SHORT loc_1_0D4C
        lea    ecx, [edi + 100h]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_1_0D5D
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    SHORT loc_1_0D5D
loc_1_0D4C:
        db  0A1h, 078h, 038h, 000h, 000h
        add    eax, ebx
        push   eax
loc_1_0D54:
        push   edi
        call   sub_1_1350
        add    esp, 8
loc_1_0D5D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0160
        add    esp, 4
        db  0A1h, 0E8h, 038h, 000h, 000h
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    dword ptr [ebx + ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 0E8h, 038h, 000h, 000h
        push   esi
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_1_0030
        add    esp, 8
        call   sub_1_11C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
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
loc_1_0DC6:
        or     edi, edi
        je     SHORT loc_1_0DDA
        sub    eax, 4
        dec    edi
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [esi + 0f4h], ecx
        jne    SHORT loc_1_0DC6
        mov    dword ptr [edx], edi
loc_1_0DDA:
        cmp    dword ptr [edx], -1
        jne    SHORT loc_1_0DF0
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h
loc_1_0DF0:
        cmp    dword ptr [esi + 20h], 2
        jne    SHORT loc_1_0E03
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0E03:
        mov    ecx, 1
        mov    eax, dword ptr [esi + 0ech]
        mov    edi, edi
loc_1_0E10:
        test   ecx, eax
        jne    SHORT loc_1_0E1C
        add    ecx, ecx
        or     ecx, ecx
        jne    SHORT loc_1_0E10
        jmp    SHORT loc_1_0E2B
loc_1_0E1C:
        mov    edx, ecx
        not    edx
        and    edx, eax
        mov    dword ptr [esi + 0ech], edx
        mov    dword ptr [ebp - 8], ecx
loc_1_0E2B:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_0E35
        sub    ecx, ecx
        jmp    SHORT loc_1_0EAB
loc_1_0E35:
        push   -1
        push   0
        push   28h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_36F6
        or     eax, eax
        je     SHORT loc_1_0E55
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [esi + 0ech], eax
        sub    ecx, ecx
        jmp    SHORT loc_1_0EAB
loc_1_0E55:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], 0
        mov    ecx, dword ptr [ebp - 4]
loc_1_0EAB:
        or     ecx, ecx
        jne    SHORT loc_1_0EC0
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h
loc_1_0EC0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        inc    dword ptr [esi + 30h]
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [ecx + 10h], eax
        mov    dword ptr [esi + 2ch], ecx
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_0EDE
        mov    dword ptr [esi + 20h], 1
loc_1_0EDE:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0EF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    eax, dword ptr [esi + 18h]
        dec    eax
        mov    dword ptr [esi + 18h], eax
        cmp    dword ptr [esi + 34h], edi
        je     NEAR PTR loc_1_0F9E
        cmp    eax, edi
        jne    NEAR PTR loc_1_0F9E
        mov    ecx, dword ptr [esi + 2ch]
        cmp    ecx, edi
        je     SHORT loc_1_0F6D
loc_1_0F1A:
        mov    edx, dword ptr [ecx + 10h]
        cmp    dword ptr [ecx + 14h], 0
        jne    SHORT loc_1_0F67
        cmp    dword ptr [esi + 30h], 1
        jne    SHORT loc_1_0F30
        mov    dword ptr [esi + 20h], 0
loc_1_0F30:
        mov    eax, dword ptr [esi + 2ch]
        or     eax, eax
        je     SHORT loc_1_0F62
        cmp    ecx, eax
        jne    SHORT loc_1_0F43
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esi + 2ch], eax
        jmp    SHORT loc_1_0F62
loc_1_0F43:
        mov    ebx, dword ptr [eax + 10h]
        or     ebx, ebx
        je     SHORT loc_1_0F62
loc_1_0F4A:
        cmp    ebx, ecx
        je     SHORT loc_1_0F59
        mov    eax, ebx
        mov    ebx, dword ptr [ebx + 10h]
        or     ebx, ebx
        jne    SHORT loc_1_0F4A
        jmp    SHORT loc_1_0F62
loc_1_0F59:
        mov    ebx, dword ptr [ebx + 10h]
        mov    dword ptr [eax + 10h], ebx
        dec    dword ptr [esi + 30h]
loc_1_0F62:
        mov    dword ptr [ecx + 10h], edi
        mov    edi, ecx
loc_1_0F67:
        mov    ecx, edx
        or     edx, edx
        jne    SHORT loc_1_0F1A
loc_1_0F6D:
        mov    dword ptr [esi + 34h], 0
        or     edi, edi
        je     SHORT loc_1_0F9E
loc_1_0F78:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3634
        mov    ebx, dword ptr [edi + 10h]
        mov    eax, dword ptr [edi + 1ch]
        or     dword ptr [esi + 0ech], eax
        push   0
        push   28h
        push   edi
        call   sub_1_36FC
        mov    edi, ebx
        or     ebx, ebx
        jne    SHORT loc_1_0F78
loc_1_0F9E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0ch]
        cmp    dword ptr [edi + 20h], 2
        jne    SHORT loc_1_0FD1
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FD1:
        cmp    dword ptr [esi + 14h], 1
        je     SHORT loc_1_0FE4
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FE4:
        mov    dword ptr [esi + 14h], 2
        push   -1
        push   0
        push   2ch
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_36F6
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1014
        mov    dword ptr [esi + 14h], 1
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1014:
        mov    dword ptr [esi + 20h], 0
        mov    eax, dword ptr [esi + 0ch]
        sub    ecx, ecx
        mov    eax, dword ptr [eax + 2ch]
        cmp    eax, ecx
        je     SHORT loc_1_1031
loc_1_1027:
        or     ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_1027
loc_1_1031:
        mov    dword ptr [edi + 0e4h], ecx
        or     ecx, ecx
        je     SHORT loc_1_1046
        mov    eax, dword ptr [edi + 4]
        or     cl, 1
        mov    byte ptr [eax + 3], cl
        jmp    SHORT loc_1_104D
loc_1_1046:
        mov    eax, dword ptr [edi + 4]
        mov    byte ptr [eax + 3], 0
loc_1_104D:
        push   0
        push   esi
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1070
        add    esp, 10h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1070:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0f4h], 1
        je     SHORT loc_1_10A8
        push   0
        push   2ch
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_36FC
        mov    esi, dword ptr [ebp + 10h]
        push   0
        mov    dword ptr [esi + 14h], 4
        push   esi
        call   sub_1_1F50
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_1_10E2
loc_1_10A8:
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 14h], 3
        mov    dword ptr [edi + 14h], 2010103h
        lea    eax, [edi + 28h]
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [eax], 0
        push   edi
        push   esi
        call   sub_1_2000
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_10E2
        push   0
        push   2ch
        push   edi
        call   sub_1_36FC
loc_1_10E2:
        cmp    ebx, 103h
        jne    SHORT loc_1_10F4
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_10F4:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1110
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 14h], 5
        cmp    dword ptr [edx + 18h], 0
        je     SHORT loc_1_1130
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_1130:
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx]
        cmp    dword ptr [ecx + 18h], 0
        je     SHORT loc_1_1150
        mov    dword ptr [edx + 14h], 0
        mov    dword ptr [ecx + 34h], 1
        mov    edi, 103h
        jmp    SHORT loc_1_11A4
loc_1_1150:
        cmp    dword ptr [ecx + 30h], 1
        jne    SHORT loc_1_115D
        mov    dword ptr [ecx + 20h], 0
loc_1_115D:
        mov    edi, dword ptr [ecx + 2ch]
        or     edi, edi
        je     SHORT loc_1_118F
        cmp    edi, edx
        jne    SHORT loc_1_1170
        mov    eax, dword ptr [edx + 10h]
        mov    dword ptr [ecx + 2ch], eax
        jmp    SHORT loc_1_118F
loc_1_1170:
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_1_118F
loc_1_1177:
        cmp    eax, edx
        je     SHORT loc_1_1186
        mov    edi, eax
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_1177
        jmp    SHORT loc_1_118F
loc_1_1186:
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [edi + 10h], eax
        dec    dword ptr [ecx + 30h]
loc_1_118F:
        mov    eax, dword ptr [edx + 1ch]
        or     dword ptr [ecx + 0ech], eax
        push   0
        push   28h
        sub    edi, edi
        push   edx
        call   sub_1_36FC
loc_1_11A4:
        cmp    edi, 103h
        je     SHORT loc_1_11B9
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_11B9
        push   edi
        push   esi
        call   sub_1_3634
loc_1_11B9:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h
sub_1_11C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 028h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 0D0h, 038h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1343
        db  083h, 03Dh, 0A0h, 038h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1343
        db  0C7h, 005h, 0A0h, 038h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 0D0h, 038h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1339
loc_1_1204:
        db  08Bh, 00Dh, 0D0h, 038h, 000h, 000h
        mov    dword ptr [ebp - 4], ecx
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_3720
        mov    eax, dword ptr [ebp - 10h]
        db  039h, 005h, 060h, 038h, 000h, 000h
        jle    NEAR PTR loc_1_1339
        db  083h, 03Dh, 0E8h, 038h, 000h, 000h, 0FFh
        je     NEAR PTR loc_1_1339
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [edi + 24h]
        add    edi, 24h
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 4]
        db  0A3h, 0D0h, 038h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_125E
        db  0C7h, 005h, 0D4h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_125E:
        db  0A1h, 0E8h, 038h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, eax
        shl    ebx, 6
        db  0A1h, 078h, 038h, 000h, 000h
        mov    eax, dword ptr [ebx + eax + 4]
        db  0A3h, 0E8h, 038h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0f4h]
        sub    eax, 7
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_02A0
        add    esp, 10h
        mov    dword ptr [edi], esi
        db  0A1h, 078h, 038h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + ebx], ecx
        cmp    dword ptr [esi + 14h], 1
        jne    SHORT loc_1_12E2
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        mov    word ptr [ecx + ebx + 1ah], ax
        db  0A1h, 078h, 038h, 000h, 000h
        mov    edi, 103h
        mov    eax, dword ptr [eax + ebx + 26h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2E2C
        add    esp, 8
        jmp    SHORT loc_1_12E7
loc_1_12E2:
        mov    edi, 40010007h
loc_1_12E7:
        cmp    edi, 103h
        je     SHORT loc_1_132C
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0160
        add    esp, 4
        db  0A1h, 0E8h, 038h, 000h, 000h
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    dword ptr [ebx + ecx + 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 0E8h, 038h, 000h, 000h
        push   edi
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_363A
        push   esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0030
        add    esp, 8
loc_1_132C:
        db  083h, 03Dh, 0D0h, 038h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1204
loc_1_1339:
        db  0C7h, 005h, 0A0h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1343:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1350:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    word ptr [eax + 2ah], 0ffffh
        mov    eax, dword ptr [eax + 32h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    ecx, eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0f4h]
        cmp    eax, 1
        jne    SHORT loc_1_13B8
        mov    edx, 0eh
        test   byte ptr [ecx + 8], 80h
        je     SHORT loc_1_13A3
        mov    dl, byte ptr [ecx + 0eh]
        and    edx, 1fh
        add    edx, 0eh
loc_1_13A3:
        cmp    eax, 7
        mov    dword ptr [esi + 0f8h], 3
        je     SHORT loc_1_13B8
        mov    dword ptr [esi + 0f8h], edx
loc_1_13B8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [esi + 0fch]
        movsx  eax, word ptr [eax + 2eh]
        sub    eax, dword ptr [esi + 0f8h]
        mov    edi, 20h
        mov    dword ptr [ebp - 10h], eax
        add    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], edx
        mov    ecx, edx
        add    ecx, 6
        cmp    dword ptr [esi + 0f4h], 7
        jne    SHORT loc_1_1410
        mov    al, byte ptr [edx]
        or     al, al
        jne    SHORT loc_1_13F7
        mov    edi, 28h
        jmp    NEAR PTR loc_1_14A2
loc_1_13F7:
        cmp    byte ptr [esi + 105h], al
        jne    NEAR PTR loc_1_14A2
        mov    edi, 21h
        jmp    NEAR PTR loc_1_14A2
        db  08Dh, 049h, 000h
loc_1_1410:
        mov    eax, dword ptr [ebp - 8]
        lea    edx, [esi + 100h]
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    SHORT loc_1_1430
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_1430
        mov    edi, 21h
        jmp    SHORT loc_1_14A2
loc_1_1430:
        mov    eax, dword ptr [ebp - 8]
        db  0BAh, 0B4h, 039h, 000h, 000h
        mov    ebx, dword ptr [eax]
        cmp    ebx, dword ptr [edx]
        jne    SHORT loc_1_1450
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_1450
        mov    edi, 28h
        jmp    SHORT loc_1_14A2
        db  090h
loc_1_1450:
        cmp    dword ptr [esi + 0f4h], 1
        mov    edx, dword ptr [ebp - 8]
        je     SHORT loc_1_1486
        test   byte ptr [edx], 1
        je     SHORT loc_1_14A2
        mov    edi, 24h
        push   edx
        mov    eax, dword ptr [esi + 0a0h]
        push   eax
        lea    eax, [esi + 80h]
        push   eax
        lea    eax, [esi + 3ch]
        push   eax
        call   sub_1_3539
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        jmp    SHORT loc_1_14A2
loc_1_1486:
        mov    al, byte ptr [edx]
        test   al, 80h
        je     SHORT loc_1_149A
        mov    edi, 1020h
        test   al, 40h
        je     SHORT loc_1_149A
        mov    edi, 3020h
loc_1_149A:
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_14A2
        or     edi, 10h
loc_1_14A2:
        inc    dword ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 2ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_152F
loc_1_14B0:
        cmp    dword ptr [ebx + 14h], 1
        jne    SHORT loc_1_1528
        test   dword ptr [ebx + 20h], edi
        jne    SHORT loc_1_14F0
        mov    ecx, dword ptr [esi + 0f4h]
        cmp    ecx, 1
        je     SHORT loc_1_14E5
        cmp    ecx, 7
        je     SHORT loc_1_14E0
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_14E0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + eax*4 + 0a0h]
        test   dword ptr [ebx + 1ch], eax
        jne    SHORT loc_1_14F0
loc_1_14E0:
        cmp    ecx, 1
        jne    SHORT loc_1_1528
loc_1_14E5:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebx + 24h]
        test   dword ptr [edx + 2], eax
        je     SHORT loc_1_1528
loc_1_14F0:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 0f8h]
        movsx  eax, word ptr [eax + 36h]
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_3652
        push   ebx
        call   sub_1_2D1B
        add    esp, 4
loc_1_1528:
        mov    ebx, dword ptr [ebx + 10h]
        or     ebx, ebx
        jne    SHORT loc_1_14B0
loc_1_152F:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_1540:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [esi + 14h], 5
        jne    SHORT loc_1_1566
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1566:
        db  083h, 03Dh, 0A0h, 038h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1670
        db  083h, 03Dh, 024h, 039h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1670
        call   sub_1_2D57
        or     eax, eax
        jne    NEAR PTR loc_1_1670
        db  0C7h, 005h, 0A0h, 038h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 0D0h, 038h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1660
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        call   sub_1_3720
        db  0A1h, 060h, 038h, 000h, 000h
        cmp    dword ptr [ebp - 8], eax
        jge    NEAR PTR loc_1_1663
        db  083h, 03Dh, 0E8h, 038h, 000h, 000h, 0FFh
        je     NEAR PTR loc_1_1663
        db  08Bh, 00Dh, 0E8h, 038h, 000h, 000h
        mov    ebx, ecx
        shl    ebx, 6
        db  0A1h, 078h, 038h, 000h, 000h
        mov    eax, dword ptr [ebx + eax + 4]
        db  0A3h, 0E8h, 038h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f4h]
        sub    eax, 7
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_02A0
        add    esp, 10h
        mov    dword ptr [edi + 24h], esi
        db  0A1h, 078h, 038h, 000h, 000h
        mov    dword ptr [ebx + eax], edi
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [esi + 18h]
        mov    eax, dword ptr [eax + 10h]
        db  08Bh, 00Dh, 078h, 038h, 000h, 000h
        mov    word ptr [ebx + ecx + 1ah], ax
        db  0A1h, 078h, 038h, 000h, 000h
        mov    eax, dword ptr [ebx + eax + 26h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2E2C
        add    esp, 8
        db  0C7h, 005h, 0A0h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_11C0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_1660:
        mov    edi, dword ptr [ebp + 0ch]
loc_1_1663:
        db  0C7h, 005h, 0A0h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_1673
        db  090h
loc_1_1670:
        mov    edi, dword ptr [ebp + 0ch]
loc_1_1673:
        inc    dword ptr [esi + 18h]
        mov    dword ptr [edi + 28h], 0
        mov    dword ptr [edi + 24h], esi
        lea    eax, [edi + 24h]
        db  083h, 03Dh, 0D4h, 038h, 000h, 000h, 000h
        je     SHORT loc_1_16A0
        db  0A1h, 0D4h, 038h, 000h, 000h
        mov    dword ptr [eax + 28h], edi
        db  089h, 03Dh, 0D4h, 038h, 000h, 000h
        jmp    SHORT loc_1_16AC
        db  08Dh, 064h, 024h, 000h
loc_1_16A0:
        db  089h, 03Dh, 0D4h, 038h, 000h, 000h
        db  089h, 03Dh, 0D0h, 038h, 000h, 000h
loc_1_16AC:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_16C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_16F4
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_16F4:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 2ah]
        cmp    ax, 0ffffh
        je     NEAR PTR loc_1_1793
        add    ecx, 32h
        mov    dword ptr [ebp - 18h], ecx
        movsx  esi, ax
        xor    edx, edx
        mov    dx, word ptr [ecx]
        mov    eax, dword ptr [ebp - 14h]
        sub    edx, esi
        mov    eax, dword ptr [eax + 0f4h]
        cmp    eax, 1
        jne    SHORT loc_1_1766
        db  003h, 035h, 090h, 038h, 000h, 000h
        mov    ecx, 0eh
        cmp    eax, 1
        jne    SHORT loc_1_174A
        test   byte ptr [esi + 8], 80h
        je     SHORT loc_1_174A
        mov    cl, byte ptr [esi + 0eh]
        and    ecx, 1fh
        add    ecx, 0eh
loc_1_174A:
        cmp    eax, 7
        mov    eax, dword ptr [ebp - 14h]
        jne    SHORT loc_1_1760
        mov    dword ptr [eax + 0f8h], 3
        jmp    SHORT loc_1_1766
        db  08Bh, 0FFh
loc_1_1760:
        mov    dword ptr [eax + 0f8h], ecx
loc_1_1766:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0f8h]
        sub    ax, dx
        add    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 36h
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], ecx
        sub    dx, word ptr [eax + 0f8h]
        add    word ptr [ecx], dx
        jmp    NEAR PTR loc_1_181D
loc_1_1793:
        mov    eax, dword ptr [ebp - 14h]
        mov    edx, dword ptr [eax + 0f4h]
        cmp    edx, 1
        jne    SHORT loc_1_17EC
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 32h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    eax, ecx
        mov    ecx, 0eh
        cmp    edx, 1
        jne    SHORT loc_1_17D2
        test   byte ptr [eax + 8], 80h
        je     SHORT loc_1_17D2
        mov    cl, byte ptr [eax + 0eh]
        and    ecx, 1fh
        add    ecx, 0eh
loc_1_17D2:
        cmp    edx, 7
        mov    eax, dword ptr [ebp - 14h]
        jne    SHORT loc_1_17E6
        mov    dword ptr [eax + 0f8h], 3
        jmp    SHORT loc_1_17EC
loc_1_17E6:
        mov    dword ptr [eax + 0f8h], ecx
loc_1_17EC:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 32h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0f8h
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 18h], ecx
        mov    eax, dword ptr [eax + 0f8h]
        add    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 36h
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], ecx
        mov    eax, dword ptr [eax + 0f8h]
        sub    word ptr [ecx], ax
loc_1_181D:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_195D
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_3720
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 18h]
        movsx  ecx, word ptr [ecx + 30h]
        mov    esi, dword ptr [ebp + 10h]
loc_1_1847:
        cmp    dword ptr [ebp - 8], ecx
        jae    SHORT loc_1_1872
        mov    eax, dword ptr [edx]
        mov    edi, eax
        and    edi, 0ffff0fffh
        shr    edi, 0ch
        and    eax, 0ffffh
        add    edi, eax
        movsx  ebx, word ptr [edx + 4]
        add    edx, 6
        inc    dword ptr [ebp - 8]
        cmp    ebx, esi
        jae    SHORT loc_1_1880
        sub    esi, ebx
        jmp    SHORT loc_1_1847
loc_1_1872:
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_1_1998
loc_1_1880:
        add    edi, esi
        sub    ebx, esi
        cmp    ebx, dword ptr [ebp + 14h]
        jbe    SHORT loc_1_188C
        mov    ebx, dword ptr [ebp + 14h]
loc_1_188C:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_195D
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [eax + eax*2]
        lea    esi, [ecx + eax*2 + 32h]
loc_1_18A3:
        or     ebx, ebx
        jne    SHORT loc_1_18DE
        mov    ecx, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [ecx + 30h]
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_1990
        mov    eax, dword ptr [esi]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        lea    edi, [ecx + eax]
        movsx  ebx, word ptr [esi + 4]
        add    esi, 6
        inc    dword ptr [ebp - 8]
        cmp    ebx, dword ptr [ebp + 14h]
        jbe    SHORT loc_1_18DE
        mov    ebx, dword ptr [ebp + 14h]
loc_1_18DE:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_190C
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_19D0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3744
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3768
loc_1_190C:
        cmp    ebx, dword ptr [ebp - 4]
        jae    SHORT loc_1_192C
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_3508
        add    esp, 0ch
        sub    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    ebx, ebx
        jmp    SHORT loc_1_1953
loc_1_192C:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_3508
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 14h]
        sub    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp + 14h], ecx
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
loc_1_1953:
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_18A3
loc_1_195D:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    word ptr [ecx + 2ah], -1
        jne    SHORT loc_1_1984
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0f8h]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 0f8h]
        add    word ptr [ecx], ax
loc_1_1984:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  090h
loc_1_1990:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 14h]
        sub    dword ptr [eax], ecx
loc_1_1998:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    word ptr [ecx + 2ah], -1
        jne    SHORT loc_1_19BF
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0f8h]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 0f8h]
        add    word ptr [ecx], ax
loc_1_19BF:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
loc_1_19D0:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 14h]
        sub    dword ptr [eax], ecx
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0ch]
        cmp    dword ptr [esi + 20h], 2
        jne    SHORT loc_1_1A10
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1A10:
        mov    dword ptr [esi + 20h], 2
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 2ch]
        cmp    eax, ecx
        je     SHORT loc_1_1A2A
loc_1_1A20:
        add    ecx, dword ptr [eax + 18h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_1A20
loc_1_1A2A:
        or     ecx, ecx
        jne    NEAR PTR loc_1_1AC0
        mov    dword ptr [esi + 38h], 1
        push   esi
        call   sub_1_323D
        add    esp, 4
        inc    dword ptr [esi + 18h]
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_1_1A76
        mov    ebx, 1
loc_1_1A51:
        cmp    dword ptr [edi + 14h], ebx
        jne    SHORT loc_1_1A72
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
        mov    edi, dword ptr [edi + 10h]
loc_1_1A72:
        or     edi, edi
        jne    SHORT loc_1_1A51
loc_1_1A76:
        mov    dword ptr [esi + 20h], 1
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_1_1AAE
        mov    ebx, 1
loc_1_1A89:
        cmp    dword ptr [edi + 14h], ebx
        jne    SHORT loc_1_1AAA
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
        mov    edi, dword ptr [edi + 10h]
loc_1_1AAA:
        or     edi, edi
        jne    SHORT loc_1_1A89
loc_1_1AAE:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1AC0:
        mov    dword ptr [esi + 38h], 0
        inc    dword ptr [esi + 18h]
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_1_1AFB
        mov    ebx, 1
loc_1_1AD6:
        cmp    dword ptr [edi + 14h], ebx
        jne    SHORT loc_1_1AF7
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_365E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3664
        mov    edi, dword ptr [edi + 10h]
loc_1_1AF7:
        or     edi, edi
        jne    SHORT loc_1_1AD6
loc_1_1AFB:
        push   esi
        call   sub_1_0EF0
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_1B10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        cmp    eax, 2
        jne    SHORT loc_1_1B25
        mov    eax, 0c001000dh
        pop    ebp
        ret
loc_1_1B25:
        or     eax, eax
        jne    SHORT loc_1_1B30
        mov    eax, 0c0000001h
        pop    ebp
        ret
loc_1_1B30:
        sub    eax, eax
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B40:
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 0B0h, 038h, 000h, 000h, 000h
        jne    SHORT loc_1_1B60
        db  0C7h, 005h, 0ACh, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1B60:
        db  08Bh, 01Dh, 0B0h, 038h, 000h, 000h
        mov    esi, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [esi + 0ch]
        or     eax, eax
        db  0A3h, 0B0h, 038h, 000h, 000h
        jne    SHORT loc_1_1B81
        db  0C7h, 005h, 0B4h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1B81:
        cmp    dword ptr [edi + 20h], 1
        jne    SHORT loc_1_1C03
        cmp    dword ptr [ebx + 10h], 1
        je     SHORT loc_1_1B94
        mov    eax, 0c00000bbh
        jmp    SHORT loc_1_1C11
loc_1_1B94:
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1010103h
        je     SHORT loc_1_1BAC
        cmp    eax, 2010103h
        je     SHORT loc_1_1BB8
        mov    eax, 0c00000bbh
        jmp    SHORT loc_1_1C11
loc_1_1BAC:
        push   ebx
        push   esi
        call   sub_1_1EA0
        add    esp, 8
        jmp    SHORT loc_1_1C11
loc_1_1BB8:
        mov    eax, dword ptr [ebx + 18h]
        sub    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [edi + 2ch]
        cmp    eax, ecx
        je     SHORT loc_1_1BD3
loc_1_1BC9:
        or     ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_1BC9
loc_1_1BD3:
        mov    dword ptr [edi + 0e8h], ecx
        db  0A1h, 05Ch, 038h, 000h, 000h
        push   edi
        mov    word ptr [eax], 0c0h
        db  08Bh, 00Dh, 05Ch, 038h, 000h, 000h
        mov    eax, dword ptr [edi + 0e8h]
        mov    dword ptr [ecx + 2], eax
        call   sub_1_2F77
        add    esp, 4
        push   eax
        call   0
        jmp    SHORT loc_1_1C0E
loc_1_1C03:
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_1B10
loc_1_1C0E:
        add    esp, 4
loc_1_1C11:
        mov    ecx, dword ptr [ebx + 14h]
        cmp    ecx, 1010103h
        je     SHORT loc_1_1C26
        cmp    ecx, 2010103h
        je     SHORT loc_1_1C47
        jmp    SHORT loc_1_1C76
loc_1_1C26:
        mov    ecx, dword ptr [esi + 14h]
        cmp    ecx, 1
        jne    SHORT loc_1_1C40
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_364C
        jmp    SHORT loc_1_1C76
        db  08Dh, 049h, 000h
loc_1_1C40:
        cmp    ecx, 4
        jne    SHORT loc_1_1C76
        jmp    SHORT loc_1_1C60
loc_1_1C47:
        mov    ecx, dword ptr [esi + 14h]
        cmp    ecx, 1
        jne    SHORT loc_1_1C5B
        push   eax
        push   ebx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_364C
        jmp    SHORT loc_1_1C76
loc_1_1C5B:
        cmp    ecx, 3
        jne    SHORT loc_1_1C76
loc_1_1C60:
        push   1
        push   esi
        push   edi
        call   sub_1_1110
        add    esp, 0ch
        push   0
        push   2ch
        push   ebx
        call   sub_1_36FC
loc_1_1C76:
        db  083h, 03Dh, 0B0h, 038h, 000h, 000h, 000h
        jne    SHORT loc_1_1C89
        db  0C7h, 005h, 0ACh, 038h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1C89:
        push   esi
        push   edi
        call   sub_1_0030
        add    esp, 8
        db  083h, 03Dh, 0B0h, 038h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1B60
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 14h], 0
        push   -1
        push   0
        push   0e8h
        push   eax
        call   sub_1_36F6
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_1CE7
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1CE7:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 18h], ecx
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 8], 0
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 10h], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 14h], 1010103h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [eax + 20h], 0
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        add    esi, 28h
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18h], esi
        cmp    dword ptr [edi + 0a0h], 0
        jbe    SHORT loc_1_1DAA
        lea    ebx, [edi + 80h]
        lea    edx, [edi + 0a4h]
        lea    esp, [esp]
loc_1_1D50:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 1ch]
        not    eax
        and    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        jne    SHORT loc_1_1D96
        mov    eax, dword ptr [edx - 68h]
        mov    dword ptr [esi], eax
        inc    word ptr [ebp - 2]
        mov    ax, word ptr [ebx]
        mov    word ptr [esi + 4], ax
        add    esi, 6
        mov    eax, dword ptr [edi + 0a0h]
        mov    ecx, dword ptr [edi + eax*4 + 38h]
        mov    dword ptr [edx - 68h], ecx
        mov    cx, word ptr [edi + eax*2 + 7eh]
        mov    word ptr [ebx], cx
        dec    eax
        mov    eax, dword ptr [edi + eax*4 + 0a4h]
        mov    dword ptr [edx], eax
        dec    dword ptr [edi + 0a0h]
loc_1_1D96:
        add    ebx, 2
        add    edx, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [edi + 0a0h], eax
        ja     SHORT loc_1_1D50
loc_1_1DAA:
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [ebp + 10h], 0
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ecx + 0ah], ax
        je     NEAR PTR loc_1_1E67
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, 6
        mov    eax, dword ptr [eax + 1ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [eax + 18h]
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_1E67
loc_1_1DE8:
        push   ebx
        mov    eax, dword ptr [edi + 0a0h]
        push   eax
        lea    eax, [edi + 80h]
        push   eax
        lea    eax, [edi + 3ch]
        push   eax
        call   sub_1_3539
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1E16
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 1ch]
        or     dword ptr [edi + eax*4 + 0a0h], ecx
        jmp    SHORT loc_1_1E59
loc_1_1E16:
        mov    eax, dword ptr [edi + 0a0h]
        cmp    eax, 10h
        jae    SHORT loc_1_1E85
        lea    edx, [edi + eax*4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [edx + 3ch], ecx
        lea    eax, [edi + eax*2 + 80h]
        mov    cx, word ptr [ebx + 4]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 1ch]
        add    esi, 6
        mov    dword ptr [edx + 0a4h], ecx
        inc    dword ptr [edi + 0a0h]
        mov    ecx, dword ptr [edx + 3ch]
        mov    dword ptr [esi - 6], ecx
        mov    ax, word ptr [eax]
        mov    word ptr [esi - 2], ax
loc_1_1E59:
        add    ebx, 6
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 14h], eax
        ja     SHORT loc_1_1DE8
loc_1_1E67:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        add    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1E85:
        push   0
        push   0e8h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_36FC
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_1EA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [eax + 0ch]
        mov    edi, edi
loc_1_1EB0:
        cmp    word ptr [esi + 8], 0
        jne    SHORT loc_1_1F10
        mov    eax, dword ptr [esi + 18h]
        add    eax, dword ptr [esi + 20h]
        db  08Bh, 015h, 05Ch, 038h, 000h, 000h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    ax, word ptr [esi + 0ah]
        or     ax, ax
        je     SHORT loc_1_1F05
        dec    ax
        mov    word ptr [esi + 0ah], ax
        mov    ecx, dword ptr [esi + 18h]
        add    ecx, dword ptr [esi + 20h]
        db  08Bh, 015h, 05Ch, 038h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [edx + 4], ax
        add    dword ptr [esi + 20h], 6
        push   edi
        call   sub_1_30D4
        add    esp, 4
        jmp    SHORT loc_1_1EB0
loc_1_1F05:
        mov    word ptr [esi + 8], 1
        jmp    SHORT loc_1_1EB0
        db  08Dh, 049h, 000h
loc_1_1F10:
        mov    eax, dword ptr [esi + 20h]
        cmp    dword ptr [esi + 1ch], eax
        jbe    SHORT loc_1_1F41
        mov    ecx, dword ptr [esi + 18h]
        add    ecx, eax
        db  08Bh, 015h, 05Ch, 038h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [edx + 4], ax
        add    dword ptr [esi + 20h], 6
        push   edi
        call   sub_1_2F77
        add    esp, 4
        jmp    NEAR PTR loc_1_1EB0
loc_1_1F41:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_1_1CB0
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_1F78
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_1F78:
        db  083h, 03Dh, 0ACh, 038h, 000h, 000h, 000h
        jne    SHORT loc_1_1FB0
        db  0C7h, 005h, 0ACh, 038h, 000h, 000h, 001h, 000h, 000h, 000h
        push   edi
        push   esi
        call   sub_1_1EA0
        add    esp, 8
        mov    esi, eax
        push   0
        push   2ch
        push   edi
        call   sub_1_36FC
        call   sub_1_1B40
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1FB0:
        mov    dword ptr [edi + 4], 0
        mov    dword ptr [edi], esi
        inc    dword ptr [esi + 18h]
        db  083h, 03Dh, 0B4h, 038h, 000h, 000h, 000h
        je     SHORT loc_1_1FE0
        db  0A1h, 0B4h, 038h, 000h, 000h
        mov    dword ptr [eax + 4], edi
        db  089h, 03Dh, 0B4h, 038h, 000h, 000h
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1FE0:
        db  089h, 03Dh, 0B4h, 038h, 000h, 000h
        db  089h, 03Dh, 0B0h, 038h, 000h, 000h
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2000:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [edx], esi
        db  083h, 03Dh, 0ACh, 038h, 000h, 000h, 000h
        jne    SHORT loc_1_2080
        db  0C7h, 005h, 0ACh, 038h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 18h]
        sub    edx, edx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [ecx + 2ch]
        cmp    eax, edx
        je     SHORT loc_1_203D
loc_1_2033:
        or     edx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_2033
loc_1_203D:
        mov    dword ptr [ecx + 0e8h], edx
        db  0A1h, 05Ch, 038h, 000h, 000h
        push   ecx
        mov    word ptr [eax], 0c0h
        db  08Bh, 015h, 05Ch, 038h, 000h, 000h
        mov    eax, dword ptr [ecx + 0e8h]
        mov    dword ptr [edx + 2], eax
        call   sub_1_2F77
        add    esp, 4
        push   eax
        call   0
        add    esp, 4
        mov    esi, eax
        call   sub_1_1B40
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2080:
        mov    dword ptr [edx + 4], 0
        mov    dword ptr [edx], esi
        inc    dword ptr [esi + 18h]
        db  083h, 03Dh, 0B4h, 038h, 000h, 000h, 000h
        je     SHORT loc_1_20B0
        db  0A1h, 0B4h, 038h, 000h, 000h
        pop    esi
        mov    dword ptr [eax + 4], edx
        db  089h, 015h, 0B4h, 038h, 000h, 000h
        mov    eax, 103h
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_20B0:
        db  089h, 015h, 0B4h, 038h, 000h, 000h
        db  089h, 015h, 0B0h, 038h, 000h, 000h
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_20D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [edi + 18h]
        mov    ebx, dword ptr [edi + 14h]
        mov    ecx, dword ptr [eax]
        sub    eax, eax
        cmp    ebx, 1010eh
        je     SHORT loc_1_2120
        cmp    ebx, 1010fh
        je     SHORT loc_1_2162
        cmp    ebx, 1010103h
        je     NEAR PTR loc_1_2189
        cmp    ebx, 2010103h
        je     NEAR PTR loc_1_2198
        mov    eax, 0c00000bbh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_2120:
        mov    dword ptr [esi + 20h], ecx
        mov    eax, dword ptr [esi + 0ch]
        sub    ecx, ecx
        mov    eax, dword ptr [eax + 2ch]
        cmp    eax, ecx
        je     SHORT loc_1_213A
        nop
loc_1_2130:
        or     ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_2130
loc_1_213A:
        mov    dword ptr [edx + 0e4h], ecx
        or     ecx, ecx
        je     SHORT loc_1_2154
        mov    eax, dword ptr [edx + 4]
        or     cl, 1
        mov    byte ptr [eax + 3], cl
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2154:
        mov    eax, dword ptr [edx + 4]
        pop    edi
        mov    byte ptr [eax + 3], 0
        sub    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2162:
        cmp    ecx, 0d0h
        jbe    SHORT loc_1_2174
        mov    eax, 0c0010015h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2174:
        cmp    dword ptr [edx + 0f0h], ecx
        jae    SHORT loc_1_21A2
        mov    dword ptr [edx + 0f0h], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2189:
        push   edi
        push   esi
        call   sub_1_1F50
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2198:
        push   edi
        push   esi
        call   sub_1_2000
        add    esp, 8
loc_1_21A2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_21B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        sub    edx, edx
        mov    word ptr [ebp - 2], 0
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 14h], edx
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 2
        jne    SHORT loc_1_21E1
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        jmp    SHORT loc_1_21E9
loc_1_21E1:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        mov    ecx, dword ptr [ecx + 0ch]
loc_1_21E9:
        mov    esi, dword ptr [ebp + 10h]
        cmp    esi, 20101h
        ja     SHORT loc_1_2212
        je     NEAR PTR loc_1_24A0
        sub    esi, 10101h
        cmp    esi, 10h
        ja     NEAR PTR loc_1_22B2
        shl    esi, 2
        db  0FFh, 0A6h, 094h, 026h, 000h, 000h
loc_1_2212:
        cmp    esi, 1010102h
        ja     SHORT loc_1_2264
        cmp    esi, 1010101h
        jae    NEAR PTR loc_1_2550
        cmp    esi, 20102h
        je     NEAR PTR loc_1_24C0
        cmp    esi, 20103h
        je     NEAR PTR loc_1_24E5
        cmp    esi, 20104h
        je     NEAR PTR loc_1_2501
        cmp    esi, 20105h
        je     NEAR PTR loc_1_2523
        cmp    esi, 2020dh
        je     NEAR PTR loc_1_2539
        jmp    SHORT loc_1_22B2
loc_1_2264:
        cmp    esi, 2010102h
        ja     SHORT loc_1_2292
        cmp    esi, 2010101h
        jae    NEAR PTR loc_1_2550
        cmp    esi, 1010103h
        je     NEAR PTR loc_1_2560
        cmp    esi, 1010104h
        je     NEAR PTR loc_1_2610
        jmp    SHORT loc_1_22B2
loc_1_2292:
        cmp    esi, 2010103h
        je     NEAR PTR loc_1_2621
        cmp    esi, 6010101h
        jb     SHORT loc_1_22B2
        cmp    esi, 6010102h
        jbe    NEAR PTR loc_1_2640
loc_1_22B2:
        mov    dword ptr [ebp - 10h], 10001h
        mov    esi, dword ptr [ebp - 1ch]
        jmp    NEAR PTR loc_1_264B
loc_1_22C1:
        db  0BEh, 02Ch, 039h, 000h, 000h
        mov    edx, 7ch
        jmp    NEAR PTR loc_1_264B
loc_1_22D0:
        mov    dword ptr [ebp - 14h], 0
        lea    esi, [ebp - 14h]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_22E4:
        mov    eax, dword ptr [ecx + 0f4h]
        mov    dword ptr [ebp - 18h], eax
        lea    esi, [ebp - 18h]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2300:
        mov    dword ptr [ebp - 8], 0d0h
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_2314:
        db  0A1h, 070h, 038h, 000h, 000h
        sub    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 8]
        movsx  edx, word ptr [eax + 28h]
        and    ecx, 0ffffh
        cmp    ecx, edx
        jle    SHORT loc_1_2340
        movsx  eax, word ptr [eax + 2ah]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_2349
        db  08Dh, 064h, 024h, 000h
loc_1_2340:
        movsx  eax, word ptr [eax + 2ch]
        sub    eax, edx
        add    dword ptr [ebp - 8], eax
loc_1_2349:
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_2356:
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 46h]
        test   ah, 80h
        je     SHORT loc_1_2374
        and    ax, 7fffh
        mov    cx, 0ah
        cwd
        idiv   cx
        movsx  eax, ax
        jmp    SHORT loc_1_2386
loc_1_2374:
        movsx  eax, ax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 4
loc_1_2386:
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_2396:
        db  0A1h, 070h, 038h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
        db  005h, 000h, 000h, 000h, 000h
loc_1_23B0:
        db  0A1h, 08Ch, 038h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_23C5:
        mov    dword ptr [ebp - 8], 4
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_23E0:
        mov    dword ptr [ebp - 8], 4
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_23F4:
        mov    dword ptr [ebp - 8], 2a4f5741h
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_2408:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 2eh]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        lea    esi, [ecx + eax]
        call   sub_1_3583
        lea    esi, [eax + esi + 1]
        xor    edx, edx
        mov    dl, byte ptr [esi - 1]
        jmp    NEAR PTR loc_1_264B
loc_1_2434:
        cmp    ebx, 2
        jne    SHORT loc_1_2441
        mov    eax, dword ptr [ecx + 0e4h]
        jmp    SHORT loc_1_2444
loc_1_2441:
        mov    eax, dword ptr [eax + 20h]
loc_1_2444:
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_2454:
        mov    eax, dword ptr [ecx + 0f0h]
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2470:
        mov    word ptr [ebp - 2], 300h
        lea    esi, [ebp - 2]
        mov    edx, 2
        jmp    NEAR PTR loc_1_264B
loc_1_2483:
        mov    eax, dword ptr [ecx + 8]
        lea    esi, [ebp - 2]
        mov    ax, word ptr [eax + 28h]
        mov    word ptr [ebp - 2], ax
        mov    edx, 2
        jmp    NEAR PTR loc_1_264B
        db  005h, 000h, 000h, 000h, 000h
loc_1_24A0:
        mov    eax, dword ptr [ecx + 0ch]
        lea    esi, [ebp - 8]
        mov    ecx, dword ptr [eax + 8]
        sub    ecx, dword ptr [eax + 14h]
        sub    ecx, dword ptr [eax + 28h]
        sub    ecx, dword ptr [eax + 18h]
        mov    edx, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_264B
        db  090h
loc_1_24C0:
        mov    eax, dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        sub    ecx, dword ptr [eax + 24h]
        sub    ecx, dword ptr [eax + 20h]
        lea    esi, [ebp - 8]
        sub    ecx, dword ptr [eax + 34h]
        sub    ecx, dword ptr [eax + 38h]
        sub    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_24E5:
        mov    eax, dword ptr [ecx + 0ch]
        lea    esi, [ebp - 8]
        mov    ecx, dword ptr [eax + 14h]
        add    ecx, dword ptr [eax + 28h]
        add    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_2501:
        mov    eax, dword ptr [ecx + 0ch]
        lea    esi, [ebp - 8]
        mov    ecx, dword ptr [eax + 24h]
        add    ecx, dword ptr [eax + 2ch]
        add    ecx, dword ptr [eax + 20h]
        add    ecx, dword ptr [eax + 34h]
        mov    edx, 4
        add    ecx, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_264B
loc_1_2523:
        mov    eax, dword ptr [ecx + 0ch]
        lea    esi, [ebp - 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
loc_1_2539:
        mov    eax, dword ptr [ecx + 0ch]
        lea    esi, [ebp - 8]
        mov    eax, dword ptr [eax + 34h]
        mov    dword ptr [ebp - 8], eax
        mov    edx, 4
        jmp    NEAR PTR loc_1_264B
        db  090h
loc_1_2550:
        mov    esi, dword ptr [ecx + 8]
        add    esi, 1ch
        mov    edx, 6
        jmp    NEAR PTR loc_1_264B
loc_1_2560:
        sub    edi, edi
        cmp    ebx, 2
        jne    SHORT loc_1_2570
        mov    dword ptr [ebp - 0ch], 0ffffffffh
        jmp    SHORT loc_1_2576
loc_1_2570:
        mov    esi, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 0ch], esi
loc_1_2576:
        mov    edx, dword ptr [ecx + 0a0h]
        or     edx, edx
        je     SHORT loc_1_2594
        lea    eax, [ecx + 0a4h]
loc_1_2586:
        mov    esi, dword ptr [ebp - 0ch]
        test   dword ptr [eax], esi
        je     SHORT loc_1_258E
        inc    edi
loc_1_258E:
        add    eax, 4
        dec    edx
        jne    SHORT loc_1_2586
loc_1_2594:
        lea    eax, [edi + edi*2]
        lea    edi, [eax*2]
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [eax]
        cmp    edi, edx
        jbe    SHORT loc_1_25BA
        mov    eax, dword ptr [ebp + 1ch]
        sub    edi, edx
        mov    dword ptr [eax], edi
        mov    eax, 0c0010014h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_25BA:
        sub    edx, edi
        mov    esi, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edx
        add    dword ptr [esi], edi
        sub    eax, eax
        cmp    dword ptr [ecx + 0a0h], eax
        jbe    SHORT loc_1_2605
        lea    ebx, [ecx + 80h]
        mov    edi, dword ptr [ebp + 14h]
        lea    edx, [ecx + 0a4h]
        lea    esp, [esp]
loc_1_25E0:
        mov    esi, dword ptr [ebp - 0ch]
        test   dword ptr [edx], esi
        je     SHORT loc_1_25F6
        mov    esi, dword ptr [edx - 68h]
        mov    dword ptr [edi], esi
        mov    si, word ptr [ebx]
        mov    word ptr [edi + 4], si
        add    edi, 6
loc_1_25F6:
        add    ebx, 2
        add    edx, 4
        inc    eax
        cmp    dword ptr [ecx + 0a0h], eax
        ja     SHORT loc_1_25E0
loc_1_2605:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_2610:
        mov    dword ptr [ebp - 8], 10h
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    SHORT loc_1_264B
loc_1_2621:
        cmp    ebx, 2
        jne    SHORT loc_1_2630
        mov    eax, dword ptr [ecx + 0e8h]
        jmp    SHORT loc_1_2633
        db  08Bh, 0FFh
loc_1_2630:
        mov    eax, dword ptr [eax + 24h]
loc_1_2633:
        mov    dword ptr [ebp - 8], eax
        lea    esi, [ebp - 8]
        mov    edx, 4
        jmp    SHORT loc_1_264B
loc_1_2640:
        mov    esi, dword ptr [ecx + 8]
        add    esi, 21h
        mov    edx, 1
loc_1_264B:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_268A
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [eax]
        cmp    ecx, edx
        jae    SHORT loc_1_2672
        mov    eax, dword ptr [ebp + 1ch]
        sub    edx, ecx
        mov    dword ptr [eax], edx
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2672:
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [ebp + 20h]
        add    dword ptr [esi], edx
        sub    dword ptr [eax], edx
loc_1_268A:
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2694:
        shl    dword ptr [edx], 0
        db  000h, 0D0h
        and    al, byte ptr [eax]
        db  000h, 0E4h
        and    al, byte ptr [eax]
        db  000h, 0E4h
        and    al, byte ptr [eax]
        db  000h, 000h
        and    eax, dword ptr [eax]
        db  000h, 014h, 023h
        add    byte ptr [eax], al
        push   esi
        and    eax, dword ptr [eax]
        db  000h, 096h, 023h, 000h, 000h, 0B0h
        and    eax, dword ptr [eax]
        db  000h, 0C5h
        and    eax, dword ptr [eax]
        db  000h, 0E0h
        and    eax, dword ptr [eax]
        db  000h, 0F4h
        and    eax, dword ptr [eax]
        db  000h, 008h
        and    al, 0
        db  000h, 034h, 024h
        add    byte ptr [eax], al
        push   esp
        and    al, 0
        db  000h, 070h, 024h
        add    byte ptr [eax], al
        and    dword ptr [eax + eax], 0
        lea    esp, [esp]
        lea    esp, [esp]
sub_1_26E0:
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
        call   sub_1_21B0
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2720:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        je     SHORT loc_1_2750
        cmp    ecx, 2
        je     SHORT loc_1_2750
        cmp    ecx, 1
        jne    SHORT loc_1_2747
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_20D0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2747:
        mov    eax, 10001h
        pop    ebp
        ret    8
loc_1_2750:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_26E0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2760:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 0BCh, 038h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_361C
        push   0
        db  0A1h, 0A4h, 038h, 000h, 000h
        push   eax
        call   sub_1_3610
        db  0A1h, 0E4h, 038h, 000h, 000h
        push   0
        movsx  eax, word ptr [eax]
        shl    eax, 4
        db  08Bh, 00Dh, 0E4h, 038h, 000h, 000h
        movsx  eax, byte ptr [eax + ecx + 3]
        lea    eax, [eax*4 + 4]
        push   eax
        db  0A1h, 0E0h, 038h, 000h, 000h
        push   eax
        call   sub_1_36FC
        push   0
        push   3fh
        db  0A1h, 0B8h, 038h, 000h, 000h
        push   eax
        call   sub_1_36FC
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27D0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ecx, ecx
        db  08Bh, 035h, 0C4h, 038h, 000h, 000h
        cmp    esi, ecx
        je     SHORT loc_1_27F0
        mov    eax, dword ptr [ebp + 8]
loc_1_27E3:
        cmp    esi, eax
        je     SHORT loc_1_27F0
        mov    ecx, esi
        mov    esi, dword ptr [esi + 28h]
        or     esi, esi
        jne    SHORT loc_1_27E3
loc_1_27F0:
        or     esi, esi
        je     SHORT loc_1_2818
        or     ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        je     SHORT loc_1_2800
        mov    dword ptr [ecx + 28h], eax
        jmp    SHORT loc_1_2805
loc_1_2800:
        db  0A3h, 0C4h, 038h, 000h, 000h
loc_1_2805:
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_3628
        push   0
        push   0
        push   esi
        call   sub_1_36FC
loc_1_2818:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2820:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_2B30
        add    esp, 4
        mov    esi, eax
        cmp    esi, -1
        jne    SHORT loc_1_2846
        mov    eax, 0c0010006h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2846:
        push   -1
        push   0
        push   10ah
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_36F6
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2B20
        shl    esi, 4
        db  0A1h, 0E4h, 038h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + eax + 6]
        mov    dword ptr [ecx], eax
        db  0A1h, 0E4h, 038h, 000h, 000h
        mov    eax, dword ptr [esi + eax + 0ah]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        db  0A1h, 0E4h, 038h, 000h, 000h
        mov    eax, dword ptr [esi + eax + 0eh]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        db  003h, 035h, 0E4h, 038h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        add    esi, 2
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        movsx  eax, byte ptr [eax]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        movsx  eax, byte ptr [eax + 1]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax + 10h]
        cmp    dword ptr [eax + 14h], esi
        jb     SHORT loc_1_28FB
        lea    edx, [esi*4]
loc_1_28E3:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 0E0h, 038h, 000h, 000h
        mov    dword ptr [edx + ecx], eax
        add    edx, 4
        mov    eax, dword ptr [ebp - 4]
        inc    esi
        cmp    dword ptr [eax + 14h], esi
        jae    SHORT loc_1_28E3
loc_1_28FB:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        movsx  eax, word ptr [eax + 44h]
        mov    dword ptr [ecx + 0f0h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0a0h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0e4h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0e8h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ech], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        test   byte ptr [eax + 22h], 80h
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_1_29A0
        mov    dword ptr [eax + 106h], 1
        jmp    SHORT loc_1_29AA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_29A0:
        mov    dword ptr [eax + 106h], 0
loc_1_29AA:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 1ch
        add    edx, 100h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f4h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f8h], 0eh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0fch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 3ch]
        cmp    ax, 4
        je     SHORT loc_1_2A70
        cmp    ax, 0bh
        je     SHORT loc_1_2A70
        cmp    ax, 14h
        je     SHORT loc_1_2A40
        cmp    ax, 17h
        je     SHORT loc_1_2A40
        cmp    ax, 0eh
        jne    SHORT loc_1_2A8A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f4h], 7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f8h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0fch], 1
        jmp    SHORT loc_1_2A8A
loc_1_2A40:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f4h], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f8h], 0dh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0fch], 1
        jmp    SHORT loc_1_2A8A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2A70:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f4h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0fch], 2
loc_1_2A8A:
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 0fch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 32h]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        inc    eax
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_2BF0
        add    esp, 8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_2B10
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 0BCh, 038h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        push   eax
        call   sub_1_3622
        or     eax, eax
        jne    SHORT loc_1_2B10
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 0C4h, 038h, 000h, 000h
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 0C4h, 038h, 000h, 000h
        push   80h
        push   eax
        call   sub_1_3383
        add    esp, 8
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2B10:
        push   0
        push   10ah
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_36FC
loc_1_2B20:
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_2B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    edi, edi
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], 0
loc_1_2B48:
        db  0A1h, 0E4h, 038h, 000h, 000h
        mov    eax, dword ptr [edi + eax + 0ah]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    eax, dword ptr [eax + 32h]
        push   eax
        call   sub_1_2C80
        add    esp, 4
        inc    eax
loc_1_2B67:
        mov    cl, byte ptr [eax]
        cmp    cl, 61h
        jb     SHORT loc_1_2B7C
        cmp    cl, 7ah
        ja     SHORT loc_1_2B7C
        xor    ebx, ebx
        mov    bl, cl
        sub    ebx, 20h
        jmp    SHORT loc_1_2B80
loc_1_2B7C:
        xor    ebx, ebx
        mov    bl, cl
loc_1_2B80:
        mov    dl, byte ptr [esi]
        cmp    dl, 61h
        jb     SHORT loc_1_2B97
        cmp    dl, 7ah
        ja     SHORT loc_1_2B97
        and    edx, 0ffh
        sub    edx, 20h
        jmp    SHORT loc_1_2B9D
loc_1_2B97:
        and    edx, 0ffh
loc_1_2B9D:
        cmp    ebx, edx
        jne    SHORT loc_1_2BA9
        or     cl, cl
        je     SHORT loc_1_2BA9
        inc    eax
        inc    esi
        jmp    SHORT loc_1_2B67
loc_1_2BA9:
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi], al
        je     SHORT loc_1_2BC2
        db  0A1h, 0E4h, 038h, 000h, 000h
        inc    dword ptr [ebp - 4]
        movsx  eax, word ptr [eax]
        add    edi, 10h
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_1_2B48
loc_1_2BC2:
        db  0A1h, 0E4h, 038h, 000h, 000h
        movsx  eax, word ptr [eax]
        cmp    eax, dword ptr [ebp - 4]
        jb     SHORT loc_1_2BD9
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2BD9:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BF0:
        push   ebp
        mov    ebp, esp
        sub    dx, dx
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_2BF9:
        mov    ax, dx
        inc    dx
        and    eax, 0ffffh
        cmp    byte ptr [ecx + eax], 0
        jne    SHORT loc_1_2BF9
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 2], dx
        mov    word ptr [eax], dx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2C20:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2B30
        add    esp, 4
        cmp    eax, -1
        jne    SHORT loc_1_2C38
        sub    al, al
        pop    ebp
        ret
loc_1_2C38:
        mov    al, 1
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
MSODISUP_Control:
        cmp    eax, 7
        jae    SHORT loc_1_2C4C
        db  0FFh, 024h, 085h, 04Eh, 02Ch, 000h, 000h
loc_1_2C4C:
        clc
        ret
        db  082h, 002h, 000h, 000h, 089h, 002h, 000h, 000h, 01Eh, 003h, 000h, 000h, 04Ch, 02Ch, 000h, 000h
        db  04Ch, 02Ch, 000h, 000h, 04Ch, 02Ch, 000h, 000h, 0D1h, 034h, 000h, 000h
MSODISUP_Service_00:
        mov    eax, 30ah
        clc
        ret
MSODISUP_Service_01:
        jmp    NEAR PTR loc_1_2C20
sub_1_2C76:
        mov    edx, eax
        shl    edx, 0ch
        shr    dx, 0ch
        ret
sub_1_2C80:
        call   sub_1_3583
        mov    ecx, dword ptr [esp + 4]
        movzx  edx, cx
        sub    ecx, edx
        shr    ecx, 0ch
        add    ecx, edx
        add    eax, ecx
        ret
sub_1_2C96:
        push   edx
        movzx  edx, cx
        sub    ecx, edx
        shr    ecx, 0ch
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
        pop    edx
        ret
loc_1_2CA6:
        mov    cx, word ptr [ebp + 3ch]
        rol    ecx, 10h
        mov    cx, word ptr [ebp]
        call   sub_1_2C96
        push   ecx
        mov    eax, ecx
        mov    ecx, dword ptr [eax]
        call   sub_1_2C96
        push   ecx
        mov    ecx, dword ptr [eax + 4]
        call   sub_1_2C96
        push   ecx
        cld
        call   sub_1_0630
        add    esp, 0ch
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_2CF8
        mov    eax, dword ptr [eax + 26h]
        mov    word ptr [ebp + 4], ax
        ror    eax, 10h
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 1ch], 0
        or     word ptr [ebp + 2ch], 40h
        ret
loc_1_2CF8:
        mov    word ptr [ebp + 1ch], 8001h
        and    word ptr [ebp + 2ch], 0ffbfh
        ret
loc_1_2D05:
        push   edx
        call   sub_1_11C0
        pop    edx
        push   dword ptr [edx]
        xor    eax, eax
        mov    dword ptr [edx + 4], eax
        cld
        db  0CDh, 020h, 03Eh, 000h, 028h, 000h
        ret
sub_1_2D1B:
        xor    ecx, ecx
        mov    edx, dword ptr [esp + 4]
        inc    ecx
        xchg   dword ptr [edx + 4], ecx
        jecxz  loc_1_2D28
        ret
loc_1_2D28:
        push   esi
        db  0BEh, 005h, 02Dh, 000h, 000h
        db  0CDh, 020h, 019h, 000h, 001h, 000h
        pop    esi
        ret
loc_1_2D36:
        movzx  ecx, word ptr [ebp + 38h]
        rol    ecx, 4
        movzx  eax, word ptr [ebp + 4]
        add    ecx, eax
        sub    ecx, 4
        push   ecx
        cld
        call   sub_1_0A60
        add    esp, 4
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
sub_1_2D57:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        test   word ptr [ebx + 2ch], 200h
        je     SHORT loc_1_2D6D
        pop    ebx
        ret
loc_1_2D6D:
        inc    eax
        db  039h, 005h, 028h, 039h, 000h, 000h
        je     SHORT loc_1_2D88
        db  0A3h, 028h, 039h, 000h, 000h
        push   esi
        db  0BEh, 0C0h, 011h, 000h, 000h
        db  0CDh, 020h, 019h, 000h, 001h, 000h
        pop    esi
loc_1_2D88:
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2D96:
        db  08Bh, 00Dh, 08Ah, 02Dh, 000h, 000h
        jecxz  loc_1_2DBC
        mov    eax, dword ptr [ecx + 4]
        db  0A3h, 08Ah, 02Dh, 000h, 000h
        cmp    eax, 0
        jne    SHORT loc_1_2DB0
        db  0A3h, 08Eh, 02Dh, 000h, 000h
loc_1_2DB0:
        push   ecx
        cld
        call   sub_1_0C50
        add    esp, 4
        jmp    SHORT loc_1_2D96
loc_1_2DBC:
        db  0C7h, 005h, 092h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h
        ret
loc_1_2DC7:
        xor    edx, edx
        mov    dx, word ptr [ebp + 38h]
        rol    edx, 4
        movzx  eax, word ptr [ebp + 4]
        add    edx, eax
        sub    edx, 4
        db  083h, 03Dh, 0A0h, 038h, 000h, 000h, 000h
        jne    SHORT loc_1_2E13
        mov    ecx, edx
        db  087h, 00Dh, 08Eh, 02Dh, 000h, 000h
        jecxz  loc_1_2E24
        mov    dword ptr [ecx + 4], edx
loc_1_2DEF:
        mov    dword ptr [edx + 4], 0
        mov    eax, 1
        db  087h, 005h, 092h, 02Dh, 000h, 000h
        cmp    eax, 0
        jne    SHORT loc_1_2E1D
        db  0BEh, 096h, 02Dh, 000h, 000h
        db  0CDh, 020h, 019h, 000h, 001h, 000h
        jmp    SHORT loc_1_2E1D
loc_1_2E13:
        push   edx
        cld
        call   sub_1_0C50
        add    esp, 4
loc_1_2E1D:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
loc_1_2E24:
        db  089h, 015h, 08Ah, 02Dh, 000h, 000h
        jmp    SHORT loc_1_2DEF
sub_1_2E2C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        jne    SHORT loc_1_2E60
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    SHORT loc_1_2ED0
loc_1_2E60:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 096h, 02Eh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        test   dword ptr [edx], 2
        je     SHORT loc_1_2E8F
        push   25h
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
loc_1_2E8F:
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2E96:
        test   dword ptr [edx], 1
        jne    SHORT loc_1_2ED0
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        je     SHORT loc_1_2EC1
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 096h, 02Eh, 000h, 000h
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
loc_1_2EC1:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_2ED0:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        and    word ptr [ebx + 2ch], 0fbffh
        db  0A1h, 098h, 038h, 000h, 000h
        mov    word ptr [ebx + 30h], ax
        shr    eax, 10h
        mov    word ptr [ebx + 34h], ax
        mov    edi, edx
        mov    edx, dword ptr [edi + 10h]
        mov    edx, dword ptr [edx + 10h]
        mov    word ptr [ebx + 10h], 0ch
        mov    ecx, dword ptr [edi + 14h]
        mov    word ptr [ebx + 4], cx
        ror    ecx, 10h
        mov    word ptr [ebx + 38h], cx
        db  00Fh, 0B7h, 015h, 04Ch, 038h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 04Eh, 038h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    edx, edi
        btr    dword ptr [edx], 0
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        test   dword ptr [edx], 2
        je     NEAR PTR loc_1_2E8F
        btr    dword ptr [edx], 1
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
sub_1_2F77:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        jne    SHORT loc_1_2FAB
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    SHORT loc_1_3024
loc_1_2FAB:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 0EAh, 02Fh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        test   dword ptr [edx], 2
        je     SHORT loc_1_2FDA
        push   25h
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
loc_1_2FDA:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2FEA:
        test   dword ptr [edx], 1
        jne    SHORT loc_1_3024
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        je     SHORT loc_1_3015
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 0EAh, 02Fh, 000h, 000h
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
loc_1_3015:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_3024:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 2ch], 0fbffh
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [edi + 10h]
loc_1_3050:
        mov    word ptr [eax + 1ch], si
        mov    word ptr [eax + 10h], 2
        db  08Bh, 015h, 058h, 038h, 000h, 000h
        mov    word ptr [eax + 4], dx
        ror    edx, 10h
        mov    word ptr [eax + 38h], dx
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        inc    esi
        cmp    dword ptr [edi + 14h], esi
        jns    SHORT loc_1_3050
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 1ch]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        btr    dword ptr [edx], 0
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        test   dword ptr [edx], 2
        je     NEAR PTR loc_1_2FDA
        btr    dword ptr [edx], 1
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
sub_1_30D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        jne    SHORT loc_1_3108
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    SHORT loc_1_3181
loc_1_3108:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 047h, 031h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        test   dword ptr [edx], 2
        je     SHORT loc_1_3137
        push   25h
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
loc_1_3137:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_3147:
        test   dword ptr [edx], 1
        jne    SHORT loc_1_3181
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        je     SHORT loc_1_3172
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 047h, 031h, 000h, 000h
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
loc_1_3172:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_3181:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 2ch], 0fbffh
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [edi + 10h]
loc_1_31AD:
        mov    word ptr [eax + 1ch], si
        mov    word ptr [eax + 10h], 3
        db  08Bh, 015h, 058h, 038h, 000h, 000h
        mov    word ptr [eax + 4], dx
        ror    edx, 10h
        mov    word ptr [eax + 38h], dx
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        inc    esi
        cmp    dword ptr [edi + 14h], esi
        jns    SHORT loc_1_31AD
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 1ch]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        btr    dword ptr [edx], 0
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        test   dword ptr [edx], 2
        je     NEAR PTR loc_1_3137
        btr    dword ptr [edx], 1
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
        db  0CDh, 020h, 01Ah, 080h, 001h, 000h, 0CDh, 020h, 01Bh, 080h, 001h, 000h
sub_1_323D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        jne    SHORT loc_1_3271
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    SHORT loc_1_32EA
loc_1_3271:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 0B0h, 032h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        test   dword ptr [edx], 2
        je     SHORT loc_1_32A0
        push   25h
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
loc_1_32A0:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_32B0:
        test   dword ptr [edx], 1
        jne    SHORT loc_1_32EA
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        je     SHORT loc_1_32DB
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 0B0h, 032h, 000h, 000h
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
loc_1_32DB:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_32EA:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 2ch], 0fbffh
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [edi + 10h]
        mov    word ptr [eax + 1ch], si
        mov    word ptr [eax + 10h], 6
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 1ch]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        btr    dword ptr [edx], 0
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        test   dword ptr [edx], 2
        je     NEAR PTR loc_1_32A0
        btr    dword ptr [edx], 1
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
sub_1_3383:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        mov    dword ptr [edx], 0
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        jne    SHORT loc_1_33B7
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    SHORT loc_1_3430
loc_1_33B7:
        bts    dword ptr [edx], 1
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 0F6h, 033h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        test   dword ptr [edx], 2
        je     SHORT loc_1_33E6
        push   25h
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
loc_1_33E6:
        xor    eax, eax
        lea    edx, [ebp - 8]
        mov    ax, word ptr [edx + 4]
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_33F6:
        test   dword ptr [edx], 1
        jne    SHORT loc_1_3430
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        cmp    eax, edi
        je     SHORT loc_1_3421
        mov    ebx, edi
        mov    eax, 10h
        mov    ecx, 101h
        db  0BEh, 0F6h, 033h, 000h, 000h
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
loc_1_3421:
        bts    dword ptr [edx], 0
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_3430:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        and    word ptr [eax + 2ch], 0fbffh
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [edi + 10h]
        mov    word ptr [eax + 1ch], si
        mov    word ptr [eax + 10h], 9
        mov    cx, word ptr [ebx + 14h]
        mov    word ptr [eax + 18h], cx
        movzx  edx, word ptr [edi]
        movzx  ecx, word ptr [edi + 2]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cld
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 1ch]
        mov    word ptr [edx + 4], ax
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        btr    dword ptr [edx], 0
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        test   dword ptr [edx], 2
        je     NEAR PTR loc_1_33E6
        btr    dword ptr [edx], 1
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
MSODISUP_SYS_CRITICAL_EXIT:
        pushfd
        cli
        db  0A1h, 038h, 038h, 000h, 000h
        mov    ecx, dword ptr [eax + 28h]
        call   sub_1_2C96
        mov    eax, ecx
        movzx  ecx, word ptr [ecx]
        inc    ecx
        add    eax, 2
loc_1_34E9:
        mov    byte ptr [eax + 3], 0
        mov    byte ptr [eax + 2], 0
        add    eax, 10h
        loop   loc_1_34E9
        db  0A1h, 03Ch, 038h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        db  0A1h, 040h, 038h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        popfd
        ret
sub_1_3508:
        cld
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, 0ch
        jb     SHORT loc_1_3534
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
loc_1_3534:
        rep movsb
        pop    edi
        pop    esi
        ret
sub_1_3539:
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
loc_1_3551:
        repne scasd
        mov    ebx, ecx
        inc    ebx
        neg    ebx
        cmp    ebx, edx
        ja     SHORT loc_1_3579
        mov    edx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 1ch]
        mov    si, word ptr [esi + 4]
        cmp    si, word ptr [edx + ebx*2 - 2]
        mov    edx, dword ptr [esp + 18h]
        jne    SHORT loc_1_3551
        mov    eax, ebx
loc_1_3575:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3579:
        xor    eax, eax
        jmp    SHORT loc_1_3575
        db  0CDh, 020h, 0A8h, 080h, 001h, 000h
sub_1_3583:
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
sub_1_360A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_3610:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_3616:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_361C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_3622:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_3628:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_3634:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_363A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_364C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_3652:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_365E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_3664:
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
sub_1_36F6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_36FC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_3720:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_3744:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_3768:
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
        db  06Ah, 02Ch, 000h, 000h, 071h, 02Ch, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 097h, 004h, 001h, 000h, 000h, 000h, 04Dh, 053h, 04Fh, 044h
        db  049h, 053h, 055h, 050h, 000h, 040h, 000h, 054h, 040h, 02Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0DCh, 037h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  09Ah, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h
        db  001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 0BBh, 000h, 000h, 0C0h
        db  001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 001h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_392C:
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
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 1000201h
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
        add    dword ptr [ecx], eax
        add    dword ptr [esi], eax
        add    al, byte ptr [ecx]
        add    dword ptr [esi], eax
loc_1_39A8:
        dec    ebp
        push   ebx
        dec    edi
        inc    esp
        dec    ecx
        push   ebx
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
_LTEXT ends
        end
