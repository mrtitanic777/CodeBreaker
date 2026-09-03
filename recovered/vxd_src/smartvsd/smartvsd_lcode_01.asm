; smartvsd_lcode_01.asm -- SMARTVSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

SMARTVSD_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1ch
        jne    SHORT loc_1_0017
        call   sub_1_0851
        cmp    eax, 1
        ret
loc_1_0017:
        cmp    eax, 23h
        jne    SHORT loc_1_0029
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_0150
        cmp    eax, 1
        ret
loc_1_0029:
        clc
        ret
        db  000h
loc_1_002C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  esi, word ptr [ebx]
        cmp    esi, 3
        jne    SHORT loc_1_0049
        push   ebx
        db  0E8h, 0ECh, 0FFh, 0FEh, 0FFh
        add    esp, 4
        jmp    SHORT loc_1_005B
loc_1_0049:
        cmp    esi, 2
        jne    SHORT loc_1_0055
        db  0E8h, 0ADh, 0FFh, 0FEh, 0FFh
        jmp    SHORT loc_1_005B
loc_1_0055:
        mov    word ptr [ebx + 2], 0ffffh
loc_1_005B:
        ret
sub_1_005C:
        db  0CDh, 020h, 067h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0064:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   edx
        push   ebx
        push   ecx
        sub    esp, 10h
        mov    esi, esp
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 4]
        add    ecx, 64h
        mov    dword ptr [esi + 6], ecx
        add    ecx, 68h
        mov    word ptr [esi + 4], cx
        mov    byte ptr [esi + 0eh], 2
        mov    word ptr [esi], 2
        push   esi
        db  0FFh, 015h, 078h, 011h, 000h, 000h
        mov    esi, dword ptr [esi + 0ah]
        add    esp, 14h
        or     esi, esi
        je     SHORT loc_1_011C
        add    esi, dword ptr [edi + 4]
        movzx  eax, word ptr [ebp + 14h]
        mov    dword ptr [esi + 7ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 80h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 90h], eax
        mov    ax, 2bh
        mov    word ptr [esi + 68h], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 6ch], eax
        or     dword ptr [esi + 6ch], 501h
        xor    eax, eax
        mov    dword ptr [esi + 64h], eax
        mov    dword ptr [esi + 78h], eax
        mov    dword ptr [esi + 0a8h], eax
        mov    al, byte ptr [edi + 15h]
        mov    byte ptr [esi + 0b1h], al
        lea    ebx, [edi + 0c3h]
        push   esi
        lea    esi, [esi + 64h]
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    esi
        movzx  eax, word ptr [esi + 6ah]
        sub    esi, dword ptr [edi + 4]
        sub    esp, 8
        mov    edi, esp
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi], 4
        push   edi
        db  0FFh, 015h, 078h, 011h, 000h, 000h
        add    esp, 0ch
loc_1_0115:
        pop    ecx
        pop    ebx
        pop    edx
        pop    edi
        pop    esi
        leave
        ret
loc_1_011C:
        mov    ax, 1dh
        jmp    SHORT loc_1_0115
sub_1_0122:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ecx
        cld
        movzx  ecx, word ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        shr    ecx, 2
        rep movsd
        movzx  ecx, byte ptr [ebp + 10h]
        and    cx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 8]
        pop    ecx
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_0150:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_01E0
loc_1_016B:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0220
loc_1_0177:
        call   sub_1_0866
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0220
loc_1_0184:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_022F
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0220
loc_1_019D:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_02BA
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0220
loc_1_01B6:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_04F9
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0220
loc_1_01CF:
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_0220
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_01E0:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_016B
        cmp    dword ptr [ebp - 8], 74080h
        je     NEAR PTR loc_1_0184
        cmp    dword ptr [ebp - 8], 7c084h
        je     NEAR PTR loc_1_019D
        cmp    dword ptr [ebp - 8], 7c088h
        je     NEAR PTR loc_1_01B6
        cmp    dword ptr [ebp - 8], -1
        je     NEAR PTR loc_1_0177
        jmp    NEAR PTR loc_1_01CF
loc_1_0220:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0228
loc_1_0228:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_022F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_08C0
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0258
        mov    eax, 1
        jmp    NEAR PTR loc_1_02B3
loc_1_0258:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 1ch], 18h
        jne    NEAR PTR loc_1_0277
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 4], eax
loc_1_0277:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 7
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0
        db  0A0h, 0ACh, 011h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [eax], 18h
        sub    eax, eax
        jmp    NEAR PTR loc_1_02B3
loc_1_02B3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_02BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0AC1
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0305
        mov    eax, 1
        jmp    NEAR PTR loc_1_04F2
loc_1_0305:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        add    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 3], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 0ch]
        push   eax
        call   sub_1_087B
        add    esp, 4
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 0
        jne    NEAR PTR loc_1_0383
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    NEAR PTR loc_1_04F2
loc_1_0383:
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_06FA
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_03C6
        mov    al, byte ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    NEAR PTR loc_1_04F2
loc_1_03C6:
        mov    eax, dword ptr [ebp - 24h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_03DF
        mov    dword ptr [ebp - 8], 400000h
        mov    word ptr [ebp - 14h], 1
loc_1_03DF:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 1ch], 18h
        jne    NEAR PTR loc_1_0409
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 0dah
        jne    NEAR PTR loc_1_0409
        mov    dword ptr [ebp - 4], 0ffffffffh
loc_1_0409:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ax, word ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_1_0064
        add    esp, 14h
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0457
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 1ch], eax
        push   8
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 10h
        push   eax
        call   sub_1_0122
        add    esp, 0ch
loc_1_0457:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_0487
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 200h
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 20h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_0493
loc_1_0487:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [eax], 0ch
loc_1_0493:
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_04B8
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 1], al
loc_1_04B8:
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_07C0
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_04EB
        mov    eax, 1
        jmp    NEAR PTR loc_1_04F2
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_04EB:
        sub    eax, eax
        jmp    NEAR PTR loc_1_04F2
loc_1_04F2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_04F9:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0CD7
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_053D
        mov    eax, 1
        jmp    NEAR PTR loc_1_06F3
loc_1_053D:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 0ch]
        push   eax
        call   sub_1_087B
        add    esp, 4
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_0598
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    NEAR PTR loc_1_06F3
loc_1_0598:
        lea    eax, [ebp - 14h]
        push   eax
        push   20h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_06FA
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_05D4
        mov    al, byte ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    NEAR PTR loc_1_06F3
loc_1_05D4:
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_0630
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 10h
        push   eax
        call   sub_1_06FA
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_0623
        mov    al, byte ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    NEAR PTR loc_1_06F3
loc_1_0623:
        mov    dword ptr [ebp - 4], 200000h
        mov    word ptr [ebp - 10h], 1
loc_1_0630:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ax, word ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_0064
        add    esp, 14h
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_068F
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_07C0
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 200h
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 20h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_069B
loc_1_068F:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [eax], 0ch
loc_1_069B:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_06C0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
loc_1_06C0:
        push   20h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_07C0
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_06EC
        mov    eax, 1
        jmp    NEAR PTR loc_1_06F3
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_06EC:
        sub    eax, eax
        jmp    NEAR PTR loc_1_06F3
loc_1_06F3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_06FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_0723
        db  068h, 050h, 012h, 000h, 000h
        call   sub_1_1230
        add    esp, 4
        mov    ax, 4
        jmp    NEAR PTR loc_1_07B9
loc_1_0723:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0ch
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fffh
        add    eax, dword ptr [ebp + 0ch]
        dec    eax
        shr    eax, 0ch
        inc    eax
        mov    dword ptr [ebp - 8], eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_005C
        add    esp, 0ch
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_0763
        mov    ax, 4
        jmp    NEAR PTR loc_1_07B9
loc_1_0763:
        push   0c0000000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1168
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_07A3
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0fffh
        add    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], ecx
        sub    ax, ax
        jmp    NEAR PTR loc_1_07B9
        db  0E9h, 016h, 000h, 000h, 000h
loc_1_07A3:
        db  068h, 07Ch, 012h, 000h, 000h
        call   sub_1_1230
        add    esp, 4
        mov    ax, 7
        jmp    NEAR PTR loc_1_07B9
loc_1_07B9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_07C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_07E9
        db  068h, 0A4h, 012h, 000h, 000h
        call   sub_1_1230
        add    esp, 4
        mov    ax, 4
        jmp    NEAR PTR loc_1_084A
loc_1_07E9:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0ch
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fffh
        add    eax, dword ptr [ebp + 0ch]
        dec    eax
        shr    eax, 0ch
        inc    eax
        mov    dword ptr [ebp - 4], eax
        push   40000000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1170
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_0834
        sub    ax, ax
        jmp    NEAR PTR loc_1_084A
        db  0E9h, 016h, 000h, 000h, 000h
loc_1_0834:
        db  068h, 0D0h, 012h, 000h, 000h
        call   sub_1_1230
        add    esp, 4
        mov    ax, 57h
        jmp    NEAR PTR loc_1_084A
loc_1_084A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_0851:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        jmp    NEAR PTR loc_1_0861
loc_1_0861:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0866:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        jmp    NEAR PTR loc_1_0876
loc_1_0876:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_087B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 8]
        sar    ecx, 1
        lea    ecx, [ecx + ecx*8]
        db  08Bh, 084h, 081h, 001h, 012h, 000h, 000h
        jmp    NEAR PTR loc_1_08B2
loc_1_08B2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_08C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_08F3
        mov    al, 4
        jmp    NEAR PTR loc_1_0ABA
loc_1_08F3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 18h
        jne    NEAR PTR loc_1_0907
        mov    dword ptr [ebp - 4], 0ffffffffh
loc_1_0907:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 10h
        je     NEAR PTR loc_1_0925
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0925
        mov    al, 4
        jmp    NEAR PTR loc_1_0ABA
loc_1_0925:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 1], 0
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_093B
loc_1_0937:
        inc    word ptr [ebp - 0ch]
loc_1_093B:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 2
        jae    NEAR PTR loc_1_0977
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     NEAR PTR loc_1_0972
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0ABA
loc_1_0972:
        jmp    NEAR PTR loc_1_0937
loc_1_0977:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0986
loc_1_0982:
        inc    word ptr [ebp - 0ch]
loc_1_0986:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 2
        jae    NEAR PTR loc_1_09C8
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + ecx + 2]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_09C3
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0ABA
loc_1_09C3:
        jmp    NEAR PTR loc_1_0982
loc_1_09C8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_09E4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 14h], eax
loc_1_09E4:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_09FE
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0ABA
loc_1_09FE:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 18h
        je     NEAR PTR loc_1_0A15
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0A2C
loc_1_0A15:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 18h
        je     NEAR PTR loc_1_0A3C
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0A3C
loc_1_0A2C:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0ABA
loc_1_0A3C:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0A4B
loc_1_0A47:
        inc    word ptr [ebp - 0ch]
loc_1_0A4B:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 4
        jae    NEAR PTR loc_1_0A87
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [ecx + eax*4 + 8], 0
        je     NEAR PTR loc_1_0A82
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0ABA
loc_1_0A82:
        jmp    NEAR PTR loc_1_0A47
loc_1_0A87:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0AAA
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0ABA
loc_1_0AAA:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0ABA
loc_1_0ABA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_0AC1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0AE4
        mov    al, 4
        jmp    NEAR PTR loc_1_0CD0
loc_1_0AE4:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 10h
        je     NEAR PTR loc_1_0B0E
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 18h
        je     NEAR PTR loc_1_0B0E
        mov    al, 4
        jmp    NEAR PTR loc_1_0CD0
loc_1_0B0E:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0B21
loc_1_0B1E:
        inc    byte ptr [ebp - 0ch]
loc_1_0B21:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 2
        jae    NEAR PTR loc_1_0B57
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     NEAR PTR loc_1_0B52
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0B52:
        jmp    NEAR PTR loc_1_0B1E
loc_1_0B57:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0B63
loc_1_0B60:
        inc    byte ptr [ebp - 0ch]
loc_1_0B63:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 2
        jae    NEAR PTR loc_1_0B9F
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + ecx + 2]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0B9A
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0B9A:
        jmp    NEAR PTR loc_1_0B60
loc_1_0B9F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_0BC2
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0BC2:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        add    eax, 20h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 14h]
        je     NEAR PTR loc_1_0BE6
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0BE6:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0BF2
loc_1_0BEF:
        inc    byte ptr [ebp - 0ch]
loc_1_0BF2:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 4
        jae    NEAR PTR loc_1_0C28
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4 + 10h], 0
        je     NEAR PTR loc_1_0C23
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0C23:
        jmp    NEAR PTR loc_1_0BEF
loc_1_0C28:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0C34
loc_1_0C31:
        inc    byte ptr [ebp - 0ch]
loc_1_0C34:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 3
        jae    NEAR PTR loc_1_0C70
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx + 0dh]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0C6B
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0C6B:
        jmp    NEAR PTR loc_1_0C31
loc_1_0C70:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0C93
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0C93:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_0CBE
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 200h
        je     NEAR PTR loc_1_0CBE
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0CD0
loc_1_0CBE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0F3F
        jmp    NEAR PTR loc_1_0CD0
loc_1_0CD0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_0CD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0CFA
        mov    al, 4
        jmp    NEAR PTR loc_1_0F38
loc_1_0CFA:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 10h
        jae    NEAR PTR loc_1_0D0E
        mov    al, 4
        jmp    NEAR PTR loc_1_0F38
loc_1_0D0E:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_0D41
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0D41:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 20h
        je     NEAR PTR loc_1_0D5E
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0D5E:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0D6A
loc_1_0D67:
        inc    byte ptr [ebp - 0ch]
loc_1_0D6A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 2
        jae    NEAR PTR loc_1_0DA0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     NEAR PTR loc_1_0D9B
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0D9B:
        jmp    NEAR PTR loc_1_0D67
loc_1_0DA0:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0DAC
loc_1_0DA9:
        inc    byte ptr [ebp - 0ch]
loc_1_0DAC:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 2
        jae    NEAR PTR loc_1_0DE8
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + ecx + 2]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0DE3
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0DE3:
        jmp    NEAR PTR loc_1_0DA9
loc_1_0DE8:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0DF4
loc_1_0DF1:
        inc    byte ptr [ebp - 0ch]
loc_1_0DF4:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 4
        jae    NEAR PTR loc_1_0E2A
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4 + 10h], 0
        je     NEAR PTR loc_1_0E25
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0E25:
        jmp    NEAR PTR loc_1_0DF1
loc_1_0E2A:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0E36
loc_1_0E33:
        inc    byte ptr [ebp - 0ch]
loc_1_0E36:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 3
        jae    NEAR PTR loc_1_0E72
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx + 0dh]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0E6D
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0E6D:
        jmp    NEAR PTR loc_1_0E33
loc_1_0E72:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0E95
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0E95:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_0EC0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 200h
        je     NEAR PTR loc_1_0EC0
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0EC0:
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0ECC
loc_1_0EC9:
        inc    byte ptr [ebp - 0ch]
loc_1_0ECC:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 2
        jae    NEAR PTR loc_1_0F02
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     NEAR PTR loc_1_0EFD
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0EFD:
        jmp    NEAR PTR loc_1_0EC9
loc_1_0F02:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        add    eax, 10h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 1ch]
        je     NEAR PTR loc_1_0F26
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_0F38
loc_1_0F26:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0F3F
        jmp    NEAR PTR loc_1_0F38
loc_1_0F38:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_0F3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        sar    eax, 1
        and    eax, 0ffffh
        cmp    eax, 1
        jle    NEAR PTR loc_1_0F73
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_0F73:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 3
        mov    byte ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_0F85
loc_1_0F82:
        inc    byte ptr [ebp - 8]
loc_1_0F85:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 3
        jae    NEAR PTR loc_1_0FC7
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        db  08Ah, 080h, 0FCh, 012h, 000h, 000h
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 0ah]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_0FC2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        jmp    NEAR PTR loc_1_0FC7
loc_1_0FC2:
        jmp    NEAR PTR loc_1_0F82
loc_1_0FC7:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_0FE3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_0FE3:
        mov    eax, 1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 0ch]
        shl    eax, cl
        xor    ecx, ecx
        db  08Ah, 00Dh, 0ACh, 011h, 000h, 000h
        test   ecx, eax
        jne    NEAR PTR loc_1_1010
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0ah
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_1010:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 0b0h
        jne    NEAR PTR loc_1_1026
loc_1_1026:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        db  08Ah, 080h, 000h, 013h, 000h, 000h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        je     NEAR PTR loc_1_107D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 0d0h
        jl     NEAR PTR loc_1_106D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 0efh
        jle    NEAR PTR loc_1_107D
loc_1_106D:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_107D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 0d7h
        jne    NEAR PTR loc_1_10AA
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1], 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_10AA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     NEAR PTR loc_1_10DB
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        cmp    eax, 4fh
        je     NEAR PTR loc_1_10DB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_10DB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 10h
        je     NEAR PTR loc_1_110E
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        cmp    eax, 0c2h
        je     NEAR PTR loc_1_110E
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_110E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 20h
        je     NEAR PTR loc_1_1150
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 1
        shl    eax, 4
        or     eax, 0a0h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 9]
        and    ecx, 0f0h
        cmp    eax, ecx
        je     NEAR PTR loc_1_1150
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_1150:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_1160
loc_1_1160:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  000h
sub_1_1168:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_1170:
        db  0CDh, 020h, 064h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_1178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 053h, 04Dh, 041h, 052h
        db  054h, 056h, 053h, 044h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1230:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pushad
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        popad
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1250:
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        dec    esp
        outsd
        arpl   word ptr [ebx + 42h], bp
        db  075h, 066h
        jb     NEAR PTR loc_1_128D
        sub    dword ptr [eax], esp
        dec    esi
        push   ebp
        dec    esp
        dec    esp
        and    byte ptr [edx + 69h], dl
        outsb
        xor    esp, dword ptr [bx + si]
        jo     SHORT loc_1_12E3
        imul   ebp, dword ptr [esi + 74h], 0d0a7265h
        db  000h
loc_1_127C:
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        dec    esp
        outsd
        arpl   word ptr [ebx + 42h], bp
        jne    SHORT loc_1_12F3
loc_1_128D:
        db  066h, 065h, 072h, 028h
        sub    dword ptr [eax], esp
        inc    ebx
        popad
        outsb
        daa
        db  074h, 020h
        insb
        outsd
        arpl   word ptr [ebx + 21h], bp
        or     cl, byte ptr [0]
loc_1_12A4:
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebp + 6eh], dl
        dec    esp
        outsd
        arpl   word ptr [ebx + 42h], bp
        db  075h, 066h
        db  066h, 065h, 072h, 028h
        sub    dword ptr [eax], esp
        dec    esi
        push   ebp
        dec    esp
        dec    esp
        and    byte ptr [eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 0a207265h
        or     eax, 53000000h
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx + 42h], bp
        db  075h, 066h
loc_1_12E3:
        db  066h, 065h, 072h, 028h
        sub    dword ptr [eax], esp
        inc    ebx
        popad
        outsb
        daa
        db  074h, 020h
        push   ebp
        outsb
        dec    esp
        outsd
loc_1_12F3:
        arpl   word ptr [ebx + 21h], bp
        or     cl, byte ptr [0]
loc_1_12FC:
        mov    al, 0ech
        db  0A1h, 000h
loc_1_1300:
        cmp    dword ptr [eax], esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
