; pppmac_lcode_01_02.asm -- PPPMAC object 1 segment _LTEXT  (part 2/6)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 2
        je     NEAR PTR loc_1_5116
        mov    edi, dword ptr [esi + 61h]
        sub    ebx, ebx
        lea    ebx, [ebx]
loc_1_4EE0:
        mov    dword ptr [ebp - 18h], 103h
        mov    eax, dword ptr [esi + 71h]
        sub    eax, dword ptr [esi + 75h]
        add    eax, dword ptr [esi + 1ch]
        cmp    eax, ebx
        mov    dword ptr [ebp - 14h], eax
        jne    SHORT loc_1_4F07
        mov    byte ptr [esi + 79h], 1
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 71h]
        mov    dword ptr [esi + 75h], eax
loc_1_4F07:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_39B0
        cmp    dword ptr [ebp - 10h], ebx
        je     SHORT loc_1_4F41
        test   byte ptr [ebp - 10h], 1
        je     SHORT loc_1_4F29
        inc    dword ptr [esi + 0c4h]
loc_1_4F29:
        test   byte ptr [ebp - 10h], 2
        je     SHORT loc_1_4F35
        inc    dword ptr [esi + 0bch]
loc_1_4F35:
        test   byte ptr [ebp - 10h], 8
        je     SHORT loc_1_4F41
        inc    dword ptr [esi + 0c0h]
loc_1_4F41:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        add    eax, dword ptr [esi + 75h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A70
        cmp    dword ptr [ebp - 0ch], ebx
        je     NEAR PTR loc_1_5101
        mov    dword ptr [ebp - 18h], ebx
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 0a0h], eax
        mov    ecx, dword ptr [esi + 75h]
        mov    dl, bl
        mov    eax, dword ptr [edi + 1ch]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        sub    ecx, dword ptr [esi + 71h]
        mov    dword ptr [ebp - 8], ecx
loc_1_4F81:
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        cmp    cl, 7eh
        jne    NEAR PTR loc_1_5075
        mov    byte ptr [esi + 7ah], 1
        mov    byte ptr [esi + 79h], bl
        cmp    dword ptr [ebp - 8], ebx
        je     NEAR PTR loc_1_5065
        mov    ax, 6
        cmp    byte ptr [esi + 7ch], bl
        jne    SHORT loc_1_4FAC
        mov    ax, 4
loc_1_4FAC:
        cmp    byte ptr [esi + 7dh], bl
        je     SHORT loc_1_4FB3
        dec    ax
loc_1_4FB3:
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_4FCB
        mov    dword ptr [edi + 24h], ebx
        inc    dword ptr [esi + 0c8h]
        jmp    NEAR PTR loc_1_5056
loc_1_4FCB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        add    eax, dword ptr [esi + 71h]
        push   eax
        push   0ffffh
        call   sub_1_5E30
        cmp    ax, 0f0b8h
        je     SHORT loc_1_4FF1
        mov    dword ptr [edi + 24h], ebx
        inc    dword ptr [esi + 0b0h]
        jmp    SHORT loc_1_5056
loc_1_4FF1:
        mov    eax, dword ptr [esi + 71h]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 2
        mov    dword ptr [edi + 24h], eax
        cmp    byte ptr [esi + 7ch], bl
        je     SHORT loc_1_500D
        add    dword ptr [edi + 20h], 2
        sub    dword ptr [edi + 24h], 2
loc_1_500D:
        cmp    byte ptr [esi + 7dh], bl
        mov    eax, dword ptr [edi + 1ch]
        mov    ecx, dword ptr [edi + 20h]
        je     SHORT loc_1_5030
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffff00ffh
        inc    dword ptr [edi + 20h]
        shl    ax, 8
        dec    dword ptr [edi + 24h]
        mov    word ptr [edi + 28h], ax
        jmp    SHORT loc_1_5040
loc_1_5030:
        mov    ax, word ptr [eax + ecx]
        add    dword ptr [edi + 20h], 2
        mov    word ptr [edi + 28h], ax
        sub    dword ptr [edi + 24h], 2
loc_1_5040:
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_1_5050
        inc    dword ptr [esi + 0ach]
        jmp    SHORT loc_1_5056
        db  08Dh, 049h, 000h
loc_1_5050:
        inc    dword ptr [esi + 0a8h]
loc_1_5056:
        push   esi
        call   sub_1_4950
        mov    eax, dword ptr [esi + 71h]
        mov    dword ptr [esi + 75h], eax
        add    eax, dword ptr [edi + 1ch]
loc_1_5065:
        inc    dword ptr [ebp - 4]
        mov    dl, 1
        mov    dword ptr [ebp - 8], ebx
        mov    byte ptr [esi + 7ch], bl
        mov    byte ptr [esi + 7dh], bl
        jmp    SHORT loc_1_50EF
loc_1_5075:
        cmp    byte ptr [esi + 79h], bl
        je     SHORT loc_1_5080
        inc    dword ptr [ebp - 4]
        jmp    SHORT loc_1_50EF
        db  090h
loc_1_5080:
        cmp    cl, 7dh
        jne    SHORT loc_1_5090
        mov    byte ptr [esi + 7bh], 1
        inc    dword ptr [ebp - 4]
        mov    dl, 1
        jmp    SHORT loc_1_50EF
loc_1_5090:
        cmp    byte ptr [esi + 7bh], bl
        je     SHORT loc_1_509E
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [esi + 7bh], bl
        xor    byte ptr [ecx], 20h
loc_1_509E:
        cmp    dword ptr [ebp - 8], ebx
        jne    SHORT loc_1_50C0
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        cmp    cl, 0ffh
        je     SHORT loc_1_50B9
        test   cl, 1
        je     SHORT loc_1_50D7
        cmp    cl, 0c1h
        je     SHORT loc_1_50D7
        jmp    SHORT loc_1_50D3
loc_1_50B9:
        mov    byte ptr [esi + 7ch], 1
        jmp    SHORT loc_1_50D7
        db  090h
loc_1_50C0:
        cmp    dword ptr [ebp - 8], 2
        jne    SHORT loc_1_50D7
        cmp    byte ptr [esi + 7ch], bl
        je     SHORT loc_1_50D7
        mov    ecx, dword ptr [ebp - 4]
        test   byte ptr [ecx], 1
        je     SHORT loc_1_50D7
loc_1_50D3:
        mov    byte ptr [esi + 7dh], 1
loc_1_50D7:
        cmp    dl, bl
        je     SHORT loc_1_50E8
        mov    ecx, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        mov    byte ptr [eax], cl
        inc    eax
        jmp    SHORT loc_1_50EC
loc_1_50E8:
        inc    eax
        mov    dword ptr [ebp - 4], eax
loc_1_50EC:
        inc    dword ptr [ebp - 8]
loc_1_50EF:
        dec    dword ptr [ebp - 0ch]
        jne    NEAR PTR loc_1_4F81
        mov    eax, dword ptr [esi + 71h]
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 75h], eax
loc_1_5101:
        cmp    dword ptr [ebp - 18h], ebx
        je     NEAR PTR loc_1_4EE0
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 65h], eax
        push   esi
        call   sub_1_46D0
loc_1_5116:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_5120:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    ax, ax
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_5130:
        movzx  edi, byte ptr [edx]
        add    edi, edi
        mov    cx, ax
        shr    cx, 7
        and    ecx, 0ffffh
        xor    edi, ecx
        and    edi, 1feh
        shl    ax, 8
        db  066h, 08Bh, 08Fh, 0F4h, 084h, 001h, 000h
        xor    cx, ax
        mov    ax, cx
        inc    edx
        dec    esi
        jne    SHORT loc_1_5130
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5170:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 61h]
        mov    edx, dword ptr [eax + 20h]
        mov    dword ptr [ecx + 0ch], edx
        mov    dword ptr [ecx + 10h], eax
        mov    dword ptr [eax + 65h], 1
        mov    dword ptr [eax + 6dh], 0
        mov    byte ptr [eax + 79h], 0
        push   eax
        call   sub_1_52F0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_51A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 2
        mov    edx, dword ptr [eax + 1ch]
        mov    ebx, dword ptr [eax + 20h]
        add    ebx, edx
        mov    ecx, dword ptr [eax + 24h]
        sub    ecx, 0eh
        mov    eax, dword ptr [eax + 10h]
        cmp    byte ptr [ebx], 3
        jne    SHORT loc_1_51CA
        mov    byte ptr [ebp - 1], 1
loc_1_51CA:
        add    ebx, 0ah
        mov    eax, dword ptr [eax + 10h]
        and    eax, 10h
        je     SHORT loc_1_51D9
        or     byte ptr [ebp - 1], 20h
loc_1_51D9:
        lea    edi, [ebx + ecx + 4]
        cmp    byte ptr [ebx + 2], 6
        jb     SHORT loc_1_51ED
        or     byte ptr [ebp - 1], 80h
        sub    ebx, 2
        add    ecx, 2
loc_1_51ED:
        or     eax, eax
        je     NEAR PTR loc_1_52A5
        mov    eax, edi
        sub    eax, ebx
        add    eax, 3
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebx + eax - 1]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   ecx
        add    ebx, 4
        mov    esi, dword ptr [eax + 0c7h]
        lea    esi, [edx + esi - 1]
        mov    dword ptr [ebp - 14h], esi
        mov    eax, dword ptr [eax + 0ffh]
        mov    dword ptr [ebp - 10h], eax
        mov    byte ptr [edi], 3
        push   ebx
        call   sub_1_5120
        mov    byte ptr [edi + 1], al
        mov    byte ptr [edi + 2], ah
        sub    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        or     eax, eax
        je     SHORT loc_1_5279
loc_1_5243:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax]
        cmp    cl, 20h
        jae    SHORT loc_1_525C
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [ebp - 10h], eax
        jne    SHORT loc_1_5265
loc_1_525C:
        cmp    cl, 7dh
        je     SHORT loc_1_5265
        mov    byte ptr [esi], cl
        jmp    SHORT loc_1_526E
loc_1_5265:
        xor    cl, 20h
        mov    byte ptr [esi], cl
        dec    esi
        mov    byte ptr [esi], 7dh
loc_1_526E:
        dec    esi
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        or     eax, eax
        jne    SHORT loc_1_5243
loc_1_5279:
        mov    ecx, dword ptr [ebp - 14h]
        sub    ecx, esi
        mov    eax, ecx
        or     al, 80h
        mov    byte ptr [esi], al
        dec    esi
        mov    eax, ecx
        shr    eax, 7
        or     al, 80h
        mov    byte ptr [esi], al
        dec    esi
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi], al
        dec    esi
        mov    ebx, esi
        mov    byte ptr [esi], 36h
        mov    eax, dword ptr [ebp + 8]
        add    ecx, 4
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_1_52D4
loc_1_52A5:
        mov    byte ptr [ebx], 16h
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 2], ch
        mov    byte ptr [ebx + 3], cl
        lea    edx, [ebx + 1]
        mov    byte ptr [edi], 3
        mov    esi, edi
        sub    esi, ebx
        push   esi
        push   edx
        call   sub_1_5120
        mov    byte ptr [edi + 1], al
        mov    byte ptr [edi + 2], ah
        mov    eax, dword ptr [ebp + 8]
        add    esi, 3
        mov    dword ptr [eax + 24h], esi
loc_1_52D4:
        sub    edi, ebx
        mov    eax, dword ptr [ebp + 8]
        add    edi, 3
        mov    dword ptr [eax + 24h], edi
        sub    ebx, dword ptr [eax + 1ch]
        mov    dword ptr [eax + 20h], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_52F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 20h], 103h
        mov    eax, dword ptr [esi + 61h]
        mov    dword ptr [ebp - 18h], eax
loc_1_5309:
        mov    edi, dword ptr [ebp - 20h]
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 65h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [esi + 6dh]
        add    ecx, dword ptr [eax + 1ch]
        add    ecx, 0ah
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A70
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 65h], eax
        mov    dword ptr [ebp - 20h], 102h
        ja     SHORT loc_1_5340
        mov    dword ptr [ebp - 20h], 0
loc_1_5340:
        mov    eax, dword ptr [ebp - 10h]
        add    dword ptr [esi + 0a0h], eax
        mov    eax, dword ptr [esi]
        cmp    eax, 3
        je     NEAR PTR loc_1_5530
        cmp    eax, 2
        je     NEAR PTR loc_1_5530
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_39B0
        test   byte ptr [ebp - 8], 1
        je     SHORT loc_1_537A
        inc    dword ptr [esi + 0c4h]
loc_1_537A:
        test   byte ptr [ebp - 8], 2
        je     SHORT loc_1_5386
        inc    dword ptr [esi + 0bch]
loc_1_5386:
        test   byte ptr [ebp - 8], 8
        je     SHORT loc_1_5392
        inc    dword ptr [esi + 0c0h]
loc_1_5392:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_5445
        mov    dword ptr [ebp - 8], 1
        mov    edi, 1
        mov    ebx, 2
loc_1_53B0:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 65h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [esi + 6dh]
        add    ecx, dword ptr [eax + 1ch]
        add    ecx, 0ah
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A70
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_39B0
        test   dword ptr [ebp - 8], edi
        je     SHORT loc_1_53EA
        inc    dword ptr [esi + 0c4h]
loc_1_53EA:
        test   dword ptr [ebp - 8], ebx
        je     SHORT loc_1_53F5
        inc    dword ptr [esi + 0bch]
loc_1_53F5:
        test   byte ptr [ebp - 8], 8
        je     SHORT loc_1_5401
        inc    dword ptr [esi + 0c0h]
loc_1_5401:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_53B0
        mov    dword ptr [ebp - 20h], 0
        inc    dword ptr [esi + 0b4h]
        mov    eax, dword ptr [ebp - 10h]
        add    dword ptr [esi + 0a0h], eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 20h]
        mov    byte ptr [esi + 79h], 1
        mov    eax, dword ptr [eax + 0c3h]
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A70
        jmp    NEAR PTR loc_1_5512
loc_1_5445:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 65h], eax
        jbe    SHORT loc_1_5481
        cmp    edi, 103h
        je     SHORT loc_1_5463
        add    dword ptr [esi + 6dh], eax
        mov    eax, dword ptr [ebp - 10h]
        sub    dword ptr [esi + 65h], eax
        jmp    NEAR PTR loc_1_5520
loc_1_5463:
        cmp    byte ptr [esi + 79h], 0
        je     SHORT loc_1_5470
        jmp    NEAR PTR loc_1_5512
        db  08Bh, 0FFh
loc_1_5470:
        mov    eax, dword ptr [ebp - 10h]
        add    dword ptr [esi + 6dh], eax
        mov    eax, dword ptr [ebp - 10h]
        sub    dword ptr [esi + 65h], eax
        jmp    NEAR PTR loc_1_5520
loc_1_5481:
        mov    eax, dword ptr [ebp - 18h]
        mov    edi, dword ptr [eax + 1ch]
        mov    al, byte ptr [edi + 0ah]
        add    edi, 0ah
        cmp    al, 36h
        jne    NEAR PTR loc_1_56B0
        test   byte ptr [esi + 14h], 10h
        je     NEAR PTR loc_1_56B0
        mov    ebx, dword ptr [esi + 6dh]
        or     ebx, ebx
        jne    SHORT loc_1_54B5
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 6dh], eax
        mov    dword ptr [esi + 65h], 5
        jmp    SHORT loc_1_5520
loc_1_54B5:
        mov    al, byte ptr [edi + 2]
        and    eax, 7fh
        shl    eax, 7
        mov    cl, byte ptr [edi + 3]
        and    ecx, 7fh
        lea    edx, [eax + ecx]
        mov    eax, dword ptr [esi + 20h]
        cmp    dword ptr [eax + 0c7h], edx
        jae    SHORT loc_1_5539
loc_1_54D2:
        mov    byte ptr [edi], 0
loc_1_54D5:
        cmp    byte ptr [esi + 79h], 0
        jne    SHORT loc_1_5512
        inc    dword ptr [esi + 0b8h]
        mov    byte ptr [esi + 79h], 1
        cmp    byte ptr [edi], 1
        je     SHORT loc_1_54F0
        push   esi
        call   sub_1_4600
loc_1_54F0:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 0c3h]
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A70
        mov    byte ptr [edi], 1
loc_1_5512:
        mov    dword ptr [esi + 6dh], 0
        mov    dword ptr [esi + 65h], 1
loc_1_5520:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_5309
        push   esi
        call   sub_1_46D0
loc_1_5530:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_5539:
        cmp    ebx, 6
        jae    SHORT loc_1_5550
        mov    eax, dword ptr [ebp - 10h]
        add    eax, ebx
        mov    dword ptr [esi + 6dh], eax
        sub    edx, eax
        add    edx, 4
        mov    dword ptr [esi + 65h], edx
        jmp    SHORT loc_1_5520
loc_1_5550:
        mov    dword ptr [esi + 6dh], 0
        lea    eax, [edi + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, eax
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esi + 65h], 1
        mov    eax, dword ptr [eax + 0ffh]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, edx
        dec    edx
        or     eax, eax
        je     SHORT loc_1_55BA
loc_1_5579:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax]
        cmp    cl, 7dh
        jne    SHORT loc_1_5599
        or     edx, edx
        je     SHORT loc_1_55BA
        mov    eax, dword ptr [ebp - 0ch]
        dec    edx
        mov    al, byte ptr [eax]
        xor    al, 20h
        mov    byte ptr [ebx], al
        inc    dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_55B2
loc_1_5599:
        cmp    cl, 1fh
        ja     SHORT loc_1_55B0
        mov    dword ptr [ebp - 14h], 1
        shl    dword ptr [ebp - 14h], cl
        mov    eax, dword ptr [ebp - 14h]
        test   dword ptr [ebp - 1ch], eax
        jne    SHORT loc_1_55B3
loc_1_55B0:
        mov    byte ptr [ebx], cl
loc_1_55B2:
        inc    ebx
loc_1_55B3:
        mov    eax, edx
        dec    edx
        or     eax, eax
        jne    SHORT loc_1_5579
loc_1_55BA:
        sub    ebx, edi
        sub    ebx, 7
        lea    ecx, [edi + ebx + 4]
        cmp    byte ptr [ecx], 3
        je     SHORT loc_1_55D0
        jmp    NEAR PTR loc_1_54D2
        db  08Dh, 049h, 000h
loc_1_55D0:
        mov    al, byte ptr [ecx + 2]
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    dl, byte ptr [ecx + 1]
        and    edx, 0ffff00ffh
        add    ax, dx
        mov    word ptr [ebp - 4], ax
        sub    ecx, edi
        sub    ecx, 4
        push   ecx
        lea    eax, [edi + 4]
        push   eax
        call   sub_1_5120
        cmp    ax, word ptr [ebp - 4]
        je     SHORT loc_1_5612
        push   esi
        call   sub_1_4600
        inc    dword ptr [esi + 0b0h]
        jmp    NEAR PTR loc_1_5520
loc_1_5612:
        mov    byte ptr [edi + 2], bh
        mov    byte ptr [edi + 3], bl
loc_1_5618:
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [ebp - 1], al
        sub    edi, 0ah
        test   al, 80h
        je     SHORT loc_1_562B
        add    edi, 2
        sub    ebx, 2
loc_1_562B:
        test   byte ptr [ebp - 1], 40h
        je     SHORT loc_1_563F
        mov    eax, dword ptr [esi + 14h]
        or     eax, dword ptr [esi + 10h]
        test   al, 1
        je     NEAR PTR loc_1_5520
loc_1_563F:
        push   6
        lea    eax, [esi + 90h]
        push   eax
        push   edi
        call   sub_1_083C
        push   6
        lea    eax, [esi + 96h]
        push   eax
        lea    eax, [edi + 6]
        push   eax
        call   sub_1_083C
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_5673
        push   6
        db  068h, 018h, 087h, 001h, 000h
        push   edi
        call   sub_1_083C
loc_1_5673:
        cmp    ebx, 2
        jbe    NEAR PTR loc_1_5520
        inc    dword ptr [esi + 0a8h]
        lea    eax, [edi + 0eh]
        push   ebx
        push   ebx
        push   eax
        push   0eh
        push   edi
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_1_0300
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_1_04E0
        jmp    NEAR PTR loc_1_5520
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_56B0:
        cmp    al, 16h
        jne    NEAR PTR loc_1_54D5
        cmp    dword ptr [esi + 6dh], 0
        jne    SHORT loc_1_56D0
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 6dh], eax
        mov    dword ptr [esi + 65h], 5
        jmp    NEAR PTR loc_1_5520
loc_1_56D0:
        xor    eax, eax
        mov    al, byte ptr [edi + 2]
        shl    eax, 8
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 3]
        lea    ebx, [eax + ecx]
        lea    eax, [esi + 20h]
        mov    eax, dword ptr [esi + 20h]
        cmp    dword ptr [eax + 0c7h], ebx
        jae    SHORT loc_1_56F3
        jmp    NEAR PTR loc_1_54D2
loc_1_56F3:
        mov    byte ptr [esi + 79h], 0
        mov    eax, dword ptr [esi + 6dh]
        cmp    eax, 6
        jae    SHORT loc_1_5712
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 6dh], eax
        sub    ebx, eax
        add    ebx, 7
        mov    dword ptr [esi + 65h], ebx
        jmp    NEAR PTR loc_1_5520
loc_1_5712:
        mov    dword ptr [esi + 6dh], 0
        lea    ecx, [ebx + edi + 4]
        mov    dword ptr [esi + 65h], 1
        cmp    byte ptr [ecx], 3
        je     SHORT loc_1_5740
        push   esi
        call   sub_1_4600
        mov    byte ptr [esi + 79h], 1
        inc    dword ptr [esi + 0b8h]
        jmp    NEAR PTR loc_1_5520
        db  08Bh, 0FFh
loc_1_5740:
        mov    al, byte ptr [ecx + 2]
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    dl, byte ptr [ecx + 1]
        and    edx, 0ffff00ffh
        add    ax, dx
        mov    word ptr [ebp - 4], ax
        sub    ecx, edi
        push   ecx
        lea    eax, [edi + 1]
        push   eax
        call   sub_1_5120
        cmp    ax, word ptr [ebp - 4]
        je     NEAR PTR loc_1_5618
        push   esi
        call   sub_1_4600
        inc    dword ptr [esi + 0b0h]
        jmp    NEAR PTR loc_1_5520
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5790:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        cmp    byte ptr [esi + 112h], 0
        je     SHORT loc_1_57ED
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_D6C0
        push   10h
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_D4F0
        push   1
        push   1
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_E830
        db  068h, 050h, 0D8h, 000h, 000h
        db  068h, 030h, 0DEh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   2100h
        push   esi
        call   sub_1_1620
        add    esp, 14h
loc_1_57ED:
        mov    dword ptr [ebp - 28h], 6
        mov    eax, dword ptr [esi + 20h]
        add    eax, 8
        lea    edx, [ebp - 24h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    ecx, 1ch
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [esi + 187h], 0
        je     NEAR PTR loc_1_58E5
        test   byte ptr [esi + 18bh], 2
        je     NEAR PTR loc_1_58E5
        mov    eax, dword ptr [esi + 193h]
        or     eax, eax
        je     SHORT loc_1_5845
        mov    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 10h], 4
        mov    ecx, 20h
loc_1_5845:
        mov    eax, dword ptr [esi + 197h]
        or     eax, eax
        je     SHORT loc_1_585D
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp + edx - 0ch], eax
        add    dword ptr [ebp - 10h], 4
        add    ecx, 4
loc_1_585D:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_5890
        mov    dword ptr [ebp - 14h], 6
        push   ecx
        lea    eax, [ebp - 28h]
        mov    edi, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        push   2
        call   sub_1_0287
        add    esp, 10h
        mov    ecx, 1ch
        mov    dword ptr [ebp - 10h], 0
        jmp    SHORT loc_1_5893
        db  08Bh, 0FFh
loc_1_5890:
        mov    edi, dword ptr [ebp + 0ch]
loc_1_5893:
        mov    eax, dword ptr [esi + 19bh]
        or     eax, eax
        je     SHORT loc_1_58AB
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp + edx - 0ch], eax
        add    dword ptr [ebp - 10h], 4
        add    ecx, 4
loc_1_58AB:
        mov    eax, dword ptr [esi + 19fh]
        or     eax, eax
        je     SHORT loc_1_58C3
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp + edx - 0ch], eax
        add    dword ptr [ebp - 10h], 4
        add    ecx, 4
loc_1_58C3:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_58E8
        mov    dword ptr [ebp - 14h], 2ch
        push   ecx
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        push   2
        call   sub_1_0287
        add    esp, 10h
        jmp    SHORT loc_1_58E8
loc_1_58E5:
        mov    edi, dword ptr [ebp + 0ch]
loc_1_58E8:
        mov    ecx, dword ptr [edi + 10h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        push   edx
        push   esi
        call   sub_1_F110
        add    esp, 8
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_5920:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   2100h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        mov    eax, dword ptr [eax + 4eh]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_D7D0
        push   0
        push   0
        push   0
        push   2100h
        push   esi
        call   sub_1_1620
        add    esp, 14h
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 28h], 6
        mov    eax, dword ptr [esi + 20h]
        add    eax, 8
        lea    edx, [ebp - 24h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 14h], 6
        push   1ch
        push   eax
        push   -1
        push   2
        call   sub_1_0287
        add    esp, 10h
        mov    dword ptr [ebp - 14h], 2ch
        push   1ch
        lea    eax, [ebp - 28h]
        push   eax
        push   -1
        push   2
        call   sub_1_0287
        add    esp, 10h
        push   esi
        call   sub_1_F290
        add    esp, 4
        mov    dword ptr [esi + 187h], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_59D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    ebx, [edi + 16fh]
        mov    word ptr [ebx], 0
        db  0E8h, 01Bh, 0C0h, 0FDh, 0FFh
        or     eax, eax
        jne    SHORT loc_1_5A00
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5A00:
        mov    dword ptr [edi + 16bh], eax
        lea    esi, [edi + 17bh]
        lea    ecx, [edi + 17fh]
        push   ecx
        push   esi
        lea    ecx, [edi + 177h]
        push   ecx
        lea    ecx, [edi + 173h]
        push   ecx
        push   ebx
        mov    ecx, dword ptr [edi + 20h]
        add    ecx, 8
        push   ecx
        push   eax
        call   sub_1_EA00
        add    esp, 1ch
        cmp    eax, 802h
        jne    SHORT loc_1_5A41
        mov    word ptr [ebx], 0
        jmp    SHORT loc_1_5A49
loc_1_5A41:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 183h], eax
loc_1_5A49:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5A60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 61h]
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [esi + 75h], 8eh
        mov    dword ptr [esi + 71h], 8eh
        mov    byte ptr [esi + 7bh], 0
        mov    byte ptr [esi + 7ah], 0
        mov    byte ptr [esi + 79h], 0
        mov    dword ptr [esi + 18h], 3eeh
        push   0c0h
        push   esi
        call   sub_1_46A0
        mov    dword ptr [esi + 6dh], 0
        push   esi
        call   sub_1_5C60
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + 24h]
        mov    edi, ecx
        mov    esi, dword ptr [edx + 1ch]
        mov    eax, dword ptr [edx + 20h]
        add    eax, esi
        and    edi, 0ffffh
        mov    ebx, dword ptr [edx + 0ch]
        lea    edi, [eax + edi - 1]
        mov    ebx, dword ptr [ebx + 0c7h]
        lea    esi, [esi + ebx - 1]
        mov    dword ptr [ebp - 4], esi
        mov    bx, word ptr [edx + 28h]
        cmp    bx, 2f00h
        jne    SHORT loc_1_5B04
        or     byte ptr [eax], 70h
        jmp    SHORT loc_1_5B0E
loc_1_5B04:
        cmp    bx, 2d00h
        jne    SHORT loc_1_5B0E
        or     byte ptr [eax], 80h
loc_1_5B0E:
        mov    byte ptr [esi], 0c0h
        dec    esi
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     SHORT loc_1_5B4E
loc_1_5B1C:
        mov    al, byte ptr [edi]
        dec    edi
        xor    ebx, ebx
        mov    bl, al
        cmp    ebx, 0c0h
        je     SHORT loc_1_5B37
        cmp    ebx, 0dbh
        je     SHORT loc_1_5B3C
        mov    byte ptr [esi], al
        jmp    SHORT loc_1_5B43
loc_1_5B37:
        mov    byte ptr [esi], 0dch
        jmp    SHORT loc_1_5B3F
loc_1_5B3C:
        mov    byte ptr [esi], 0ddh
loc_1_5B3F:
        dec    esi
        mov    byte ptr [esi], 0dbh
loc_1_5B43:
        dec    esi
        mov    ax, cx
        dec    cx
        or     ax, ax
        jne    SHORT loc_1_5B1C
loc_1_5B4E:
        mov    byte ptr [esi], 0c0h
        mov    eax, dword ptr [ebp - 4]
        sub    eax, esi
        sub    esi, dword ptr [edx + 1ch]
        inc    ax
        and    eax, 0ffffh
        mov    dword ptr [edx + 24h], eax
        mov    dword ptr [edx + 20h], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_5B70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   2100h
        mov    edi, dword ptr [esi + 61h]
        mov    eax, dword ptr [esi + 20h]
        mov    bx, word ptr [edi + 24h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        or     bx, bx
        jne    SHORT loc_1_5BB2
        mov    ecx, dword ptr [eax + 52h]
        or     ecx, ecx
        je     NEAR PTR loc_1_5C4E
        push   edi
        mov    eax, dword ptr [eax + 4eh]
        push   eax
        call   ecx
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_5BB2:
        mov    edx, dword ptr [edi + 1ch]
        add    edx, dword ptr [edi + 20h]
        mov    cl, byte ptr [edx]
        test   cl, 80h
        je     SHORT loc_1_5BC7
        mov    word ptr [edi + 28h], 2d00h
        jmp    SHORT loc_1_5BDD
loc_1_5BC7:
        cmp    cl, 70h
        jb     SHORT loc_1_5BD7
        mov    word ptr [edi + 28h], 2f00h
        and    byte ptr [edx], 0cfh
        jmp    SHORT loc_1_5BDD
loc_1_5BD7:
        mov    word ptr [edi + 28h], 2100h
loc_1_5BDD:
        mov    ecx, dword ptr [eax + 52h]
        or     ecx, ecx
        je     SHORT loc_1_5BEE
        push   edi
        mov    eax, dword ptr [eax + 4eh]
        push   eax
        call   ecx
        add    esp, 8
loc_1_5BEE:
        mov    bx, word ptr [edi + 24h]
        mov    ecx, dword ptr [edi + 1ch]
        mov    eax, dword ptr [edi + 20h]
        lea    edi, [ecx + eax - 0eh]
        push   6
        lea    eax, [esi + 90h]
        push   eax
        push   edi
        call   sub_1_083C
        push   6
        lea    eax, [esi + 96h]
        push   eax
        lea    eax, [edi + 6]
        push   eax
        call   sub_1_083C
        mov    word ptr [edi + 0ch], 8
        mov    ecx, ebx
        and    ecx, 0ffffh
        lea    eax, [edi + 0eh]
        push   ecx
        push   ecx
        push   eax
        push   0eh
        push   edi
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_1_0300
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_1_04E0
loc_1_5C4E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5C60:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        cmp    eax, 3
        je     NEAR PTR loc_1_5E1D
        cmp    eax, 2
        je     NEAR PTR loc_1_5E1D
        mov    edx, dword ptr [esi + 61h]
        mov    dword ptr [ebp - 0ch], edx
        sub    ebx, ebx
loc_1_5C88:
        mov    dword ptr [ebp - 1ch], 103h
        mov    eax, dword ptr [esi + 71h]
        sub    eax, dword ptr [esi + 75h]
        add    eax, dword ptr [esi + 1ch]
        cmp    eax, ebx
        mov    dword ptr [ebp - 14h], eax
        jne    SHORT loc_1_5CAF
        mov    byte ptr [esi + 79h], 1
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 71h]
        mov    dword ptr [esi + 75h], eax
loc_1_5CAF:
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_39B0
        cmp    dword ptr [ebp - 18h], ebx
        je     SHORT loc_1_5CF9
        test   byte ptr [ebp - 18h], 1
        je     SHORT loc_1_5CD1
        inc    dword ptr [esi + 0c4h]
loc_1_5CD1:
        test   byte ptr [ebp - 18h], 2
        je     SHORT loc_1_5CDD
        inc    dword ptr [esi + 0bch]
loc_1_5CDD:
        test   byte ptr [ebp - 18h], 8
        je     SHORT loc_1_5CE9
        inc    dword ptr [esi + 0c0h]
loc_1_5CE9:
        mov    byte ptr [esi + 79h], 1
        mov    eax, dword ptr [esi + 71h]
        mov    dword ptr [esi + 75h], eax
        mov    eax, dword ptr [esi + 65h]
        mov    dword ptr [ebp - 14h], eax
loc_1_5CF9:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [edx + 1ch]
        add    eax, dword ptr [esi + 75h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A70
        cmp    dword ptr [ebp - 10h], ebx
        je     NEAR PTR loc_1_5E08
        mov    dword ptr [ebp - 1ch], ebx
        mov    eax, dword ptr [ebp - 10h]
        add    dword ptr [esi + 0a0h], eax
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [esi + 75h]
        mov    edx, dword ptr [edx + 1ch]
        add    edx, ecx
        mov    dword ptr [ebp - 8], edx
        mov    edi, edx
        sub    ecx, dword ptr [esi + 71h]
        mov    byte ptr [ebp - 1], bl
        nop
loc_1_5D40:
        mov    al, byte ptr [edi]
        cmp    al, 0c0h
        jne    SHORT loc_1_5D92
        cmp    byte ptr [esi + 79h], bl
        je     SHORT loc_1_5D5C
        mov    edx, dword ptr [ebp - 0ch]
        push   esi
        mov    dword ptr [edx + 24h], ebx
        call   sub_1_5B70
        mov    byte ptr [esi + 79h], bl
        jmp    SHORT loc_1_5D89
loc_1_5D5C:
        cmp    ecx, ebx
        je     SHORT loc_1_5D89
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 71h]
        mov    dword ptr [edx + 20h], eax
        mov    dword ptr [edx + 24h], ecx
        inc    dword ptr [esi + 0a8h]
        push   esi
        call   sub_1_5B70
        mov    eax, dword ptr [esi + 71h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 75h], eax
        mov    edx, dword ptr [edx + 1ch]
        add    edx, eax
        mov    dword ptr [ebp - 8], edx
loc_1_5D89:
        inc    edi
        mov    byte ptr [ebp - 1], 1
        mov    ecx, ebx
        jmp    SHORT loc_1_5DF7
loc_1_5D92:
        cmp    byte ptr [esi + 79h], bl
        je     SHORT loc_1_5D9A
        inc    edi
        jmp    SHORT loc_1_5DF7
loc_1_5D9A:
        cmp    al, 0dbh
        jne    SHORT loc_1_5DA9
        mov    byte ptr [esi + 7bh], 1
        inc    edi
        mov    byte ptr [ebp - 1], 1
        jmp    SHORT loc_1_5DF7
loc_1_5DA9:
        cmp    byte ptr [esi + 7bh], bl
        je     SHORT loc_1_5DDA
        mov    byte ptr [esi + 7bh], bl
        xor    eax, eax
        mov    al, byte ptr [edi]
        cmp    eax, 0dch
        je     SHORT loc_1_5DD2
        cmp    eax, 0ddh
        je     SHORT loc_1_5DD7
        mov    byte ptr [esi + 79h], 1
        mov    eax, dword ptr [esi + 71h]
        mov    dword ptr [esi + 75h], eax
        mov    ecx, ebx
        inc    edi
        jmp    SHORT loc_1_5DF7
loc_1_5DD2:
        mov    byte ptr [edi], 0c0h
        jmp    SHORT loc_1_5DDA
loc_1_5DD7:
        mov    byte ptr [edi], 0dbh
loc_1_5DDA:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_5DF0
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [edi]
        mov    byte ptr [edx], al
        inc    edi
        inc    dword ptr [ebp - 8]
        jmp    SHORT loc_1_5DF6
        db  08Dh, 064h, 024h, 000h
loc_1_5DF0:
        inc    dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 8]
loc_1_5DF6:
        inc    ecx
loc_1_5DF7:
        dec    dword ptr [ebp - 10h]
        jne    NEAR PTR loc_1_5D40
        mov    eax, dword ptr [esi + 71h]
        add    eax, ecx
        mov    dword ptr [esi + 75h], eax
loc_1_5E08:
        cmp    dword ptr [ebp - 1ch], ebx
        je     NEAR PTR loc_1_5C88
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 65h], eax
        push   esi
        call   sub_1_46D0
loc_1_5E1D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5E30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    dx, word ptr [ebp + 10h]
        mov    ax, dx
        dec    dx
        or     ax, ax
        mov    ax, word ptr [ebp + 8]
        je     SHORT loc_1_5E81
        mov    esi, dword ptr [ebp + 0ch]
loc_1_5E4A:
        xor    ecx, ecx
        mov    cl, byte ptr [esi]
        mov    edi, eax
        and    edi, 0ffffh
        xor    ecx, edi
        and    ecx, 0ffh
        db  066h, 08Bh, 00Ch, 04Dh, 020h, 087h, 001h, 000h
        shr    ax, 8
        xor    cx, ax
        mov    ax, cx
        inc    esi
        mov    cx, dx
        dec    dx
        or     cx, cx
        jne    SHORT loc_1_5E4A
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_5E81:
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5E90:
        push   esi
        db  08Bh, 035h, 020h, 089h, 001h, 000h
        db  0C7h, 005h, 020h, 089h, 001h, 000h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_5EB7
loc_1_5EA5:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   0
        push   54h
        push   eax
        call   sub_1_075C
        or     esi, esi
        jne    SHORT loc_1_5EA5
loc_1_5EB7:
        sub    eax, eax
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_5EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 024h, 089h, 001h, 000h
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   28h
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_5EF6
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_5EF6:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 28h
        mov    eax, dword ptr [edx + 0feh]
        mov    ecx, dword ptr [ebp - 8]
        or     esi, esi
        mov    ebx, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0ch], edx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edx + 0cch], eax
        je     NEAR PTR loc_1_63D9
loc_1_5F35:
        cmp    word ptr [esi + 0ch], 4
        jne    NEAR PTR loc_1_5FC5
        cmp    word ptr [esi + 10h], 0
        je     SHORT loc_1_5FC5
        mov    ax, word ptr [esi + 0eh]
        and    ax, 0ff7fh
        mov    edx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edx + 20h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_5FC5
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_63CE
        mov    ecx, dword ptr [esi + 18h]
        or     ecx, ecx
        jne    SHORT loc_1_5F86
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 016h, 001h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_1_5F86:
        mov    ax, word ptr [esi + 0eh]
        push   ecx
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 022h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 077h, 0BCh, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_63CE
loc_1_5FC5:
        mov    cx, word ptr [esi + 0eh]
        cmp    cx, 0fd80h
        jne    SHORT loc_1_6013
        test   bl, 1
        jne    SHORT loc_1_6013
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_63CE
        xor    eax, eax
        db  066h, 0A1h, 01Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 01Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 029h, 0BCh, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_63CE
loc_1_6013:
        cmp    cx, 23c0h
        jne    SHORT loc_1_6060
        test   bl, 2
        je     SHORT loc_1_6060
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_63CE
        xor    eax, eax
        db  066h, 0A1h, 01Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 01Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0DFh, 0BBh, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_63CE
        db  08Dh, 049h, 000h
loc_1_6060:
        cmp    cx, 23c2h
        jne    SHORT loc_1_60B9
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0feh]
        cmp    dword ptr [eax + 4], 2
        jne    SHORT loc_1_60B9
        test   bl, 4
        je     SHORT loc_1_60B9
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_63CE
        xor    eax, eax
        db  066h, 0A1h, 020h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 01Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 083h, 0BBh, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_63CE
loc_1_60B9:
        cmp    cx, 29c0h
        jne    SHORT loc_1_60D3
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0feh]
        cmp    dword ptr [eax + 4], 2
        je     NEAR PTR loc_1_63CE
loc_1_60D3:
        cmp    cx, 3f80h
        jne    SHORT loc_1_6140
        test   bh, 2
        jne    SHORT loc_1_6140
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_63CE
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        jne    SHORT loc_1_6101
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_6101:
        push   eax
        mov    dx, cx
        and    edx, 0ffh
        shr    cx, 8
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ffffh
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 024h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0FEh, 0BAh, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_63CE
        db  08Bh, 0FFh
loc_1_6140:
        cmp    cx, 2b80h
        jne    SHORT loc_1_61B0
        test   bh, 4
        jne    SHORT loc_1_61B0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_63CE
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        jne    SHORT loc_1_616E
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_616E:
        push   eax
        mov    dx, cx
        and    edx, 0ffh
        shr    cx, 8
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ffffh
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 024h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 091h, 0BAh, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_63CE
        db  005h, 000h, 000h, 000h, 000h
loc_1_61B0:
        cmp    cx, 2180h
        jne    SHORT loc_1_6220
        test   bh, 8
        jne    SHORT loc_1_6220
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_63CE
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        jne    SHORT loc_1_61DE
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_61DE:
        push   eax
        mov    dx, cx
        and    edx, 0ffh
        shr    cx, 8
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ffffh
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 024h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 021h, 0BAh, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_63CE
        db  005h, 000h, 000h, 000h, 000h
loc_1_6220:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6276
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        jne    SHORT loc_1_623E
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_623E:
        push   eax
        mov    dx, cx
        and    edx, 0ffh
        shr    cx, 8
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ffffh
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 026h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0C1h, 0B9h, 0FDh, 0FFh
        add    esp, 10h
loc_1_6276:
        mov    eax, dword ptr [esi + 12h]
        add    eax, 4bh
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_643C
        mov    edx, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 27h], 0bb8h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 23h], 0bb8h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 29h], 2
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2bh], 0ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2dh], 0ah
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 3fh], 800h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 16h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 01Ah, 060h, 066h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 1eh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 22h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        je     NEAR PTR loc_1_63C5
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   eax
        add    esp, 4
        cmp    eax, 800h
        je     SHORT loc_1_63C5
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6396
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        jne    SHORT loc_1_6371
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_6371:
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0eh]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 028h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0A1h, 0B8h, 0FDh, 0FFh
        add    esp, 10h
loc_1_6396:
        mov    eax, dword ptr [esi + 24h]
        or     eax, eax
        je     SHORT loc_1_63A6
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   eax
        add    esp, 4
loc_1_63A6:
        cmp    word ptr [esi + 0ch], 4
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jne    NEAR PTR loc_1_6437
        call   sub_1_075C
        jmp    SHORT loc_1_63CE
loc_1_63C5:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
loc_1_63CE:
        mov    esi, dword ptr [esi + 4]
        or     esi, esi
        jne    NEAR PTR loc_1_5F35
loc_1_63D9:
        mov    ecx, dword ptr [ebp - 8]
        db  0A1h, 028h, 089h, 001h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        db  0A3h, 028h, 089h, 001h, 000h
        mov    dword ptr [edx + 0d0h], 801h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax + 11h], 2
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_6437:
        call   sub_1_075C
loc_1_643C:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_6480
        sub    esi, esi
        add    eax, 0
loc_1_6450:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    eax, esi
        je     SHORT loc_1_6466
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   eax
        add    esp, 4
loc_1_6466:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        push   esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx + 0ch]
        push   eax
        push   ecx
        call   sub_1_075C
        cmp    dword ptr [ebp - 4], esi
        jne    SHORT loc_1_6450
loc_1_6480:
        mov    eax, dword ptr [ebp - 8]
        push   0
        mov    eax, dword ptr [eax + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0c0000001h
        mov    dword ptr [edx + 0cch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_64B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0cch]
        or     esi, esi
        jne    SHORT loc_1_64D0
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_64D0:
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 11h], 0
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6500:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cch]
        mov    esi, dword ptr [eax + 4]
        push   esi
        mov    byte ptr [esi + 11h], 1
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_6530:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cch]
        mov    esi, dword ptr [eax + 4]
        push   esi
        mov    byte ptr [esi + 11h], 3
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
sub_1_6560:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0cch]
        or     esi, esi
        jne    SHORT loc_1_6584
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_6584:
        mov    ebx, dword ptr [esi + 4]
        cmp    byte ptr [ebx + 22h], 0
        je     SHORT loc_1_659E
        mov    byte ptr [ebx + 22h], 0
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebx + 16h]
        push   eax
        call   sub_1_0738
loc_1_659E:
        or     ebx, ebx
        je     SHORT loc_1_65BE
loc_1_65A2:
        mov    byte ptr [ebx + 11h], 3
        push   ebx
        call   PPPMAC_Service_07
        add    esp, 4
        push   ebx
        call   PPPMAC_Service_08
        add    esp, 4
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_65A2
loc_1_65BE:
        mov    ebx, dword ptr [esi + 4]
        or     ebx, ebx
        je     SHORT loc_1_65DB
loc_1_65C5:
        mov    eax, dword ptr [ebx + 8]
        mov    eax, dword ptr [eax + 24h]
        or     eax, eax
        je     SHORT loc_1_65D5
        push   ebx
        call   eax
        add    esp, 4
loc_1_65D5:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_65C5
loc_1_65DB:
        mov    ebx, dword ptr [esi + 4]
        or     ebx, ebx
        je     SHORT loc_1_65FD
        mov    eax, 800h
loc_1_65E7:
        cmp    dword ptr [ebx + 3fh], eax
        jne    SHORT loc_1_65F4
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_65E7
        jmp    SHORT loc_1_65F9
loc_1_65F4:
        call   sub_1_021A
loc_1_65F9:
        or     ebx, ebx
        jne    SHORT loc_1_65DB
loc_1_65FD:
        mov    dword ptr [edi + 0cch], 0
        db  0B9h, 028h, 089h, 001h, 000h
        db  083h, 03Dh, 028h, 089h, 001h, 000h, 000h
        je     SHORT loc_1_6622
loc_1_6615:
        mov    eax, dword ptr [ecx]
        cmp    eax, esi
        je     SHORT loc_1_6622
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_6615
loc_1_6622:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        je     SHORT loc_1_6644
loc_1_6630:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
        or     edi, edi
        jne    SHORT loc_1_6630
loc_1_6644:
        push   0
        mov    eax, dword ptr [esi + 24h]
        push   eax
        push   esi
        call   sub_1_075C
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_6660:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_6692
        mov    byte ptr [esi + 22h], 0
        cmp    word ptr [esi + 25h], 0
        mov    byte ptr [esi + 11h], 4
        ja     SHORT loc_1_6680
        mov    byte ptr [esi + 11h], 5
loc_1_6680:
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_6692:
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_66A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], cl
        mov    ebx, dword ptr [eax + 0cch]
        mov    esi, dword ptr [ebx + 8]
        or     esi, esi
        je     SHORT loc_1_66D4
        mov    eax, dword ptr [esi + 8]
        mov    di, word ptr [ebp + 0ch]
        cmp    word ptr [eax + 0eh], di
        je     NEAR PTR loc_1_6762
        jmp    SHORT loc_1_66D8
loc_1_66D4:
        mov    di, word ptr [ebp + 0ch]
loc_1_66D8:
        mov    eax, dword ptr [ebx + 4]
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_66F4
loc_1_66E1:
        mov    ecx, dword ptr [esi + 8]
        cmp    word ptr [ecx + 0eh], di
        je     SHORT loc_1_66F0
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_66E1
loc_1_66F0:
        or     esi, esi
        jne    SHORT loc_1_6762
loc_1_66F4:
        cmp    byte ptr [eax + 10h], 9
        jne    NEAR PTR loc_1_68B9
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_673F
        mov    cx, di
        and    ecx, 0ffh
        shl    ecx, 8
        mov    ax, di
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 02Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0F8h, 0B4h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_673F:
        mov    cx, word ptr [ebp + 14h]
        sub    dword ptr [ebp + 10h], 2
        mov    eax, dword ptr [ebp + 10h]
        add    cx, 2
        mov    word ptr [eax], di
        push   ecx
        push   eax
        push   ebx
        call   sub_1_7530
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_6762:
        mov    al, byte ptr [esi + 10h]
        or     al, al
        je     NEAR PTR loc_1_68B9
        cmp    al, 1
        je     NEAR PTR loc_1_68B9
        cmp    dword ptr [ebx + 4], esi
        jne    SHORT loc_1_6798
        cmp    byte ptr [ebp - 1], 8
        jne    SHORT loc_1_6798
        mov    cx, word ptr [ebp + 14h]
        push   ecx
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   ebx
        call   sub_1_75C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_6798:
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 16h], 0
        je     SHORT loc_1_67D7
        mov    cl, byte ptr [ebp - 1]
        cmp    byte ptr [eax + 17h], cl
        jne    SHORT loc_1_67D7
        mov    eax, dword ptr [esi + 4]
        push   3e8h
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_68B9
        mov    dword ptr [esi + 3bh], eax
        mov    ecx, dword ptr [eax + 1ch]
        add    ecx, dword ptr [eax + 20h]
        mov    dword ptr [esi + 35h], ecx
        mov    word ptr [esi + 39h], 3e8h
loc_1_67D7:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 2fh], eax
        mov    cx, word ptr [ebp + 14h]
        mov    word ptr [esi + 33h], cx
        mov    eax, dword ptr [esi + 8]
        push   esi
        call   dword ptr [eax + 40h]
        add    esp, 4
        mov    al, byte ptr [esi + 11h]
        cmp    al, 10h
        je     NEAR PTR loc_1_688B
        cmp    al, 0ch
        jne    SHORT loc_1_6844
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6844
        mov    cx, di
        and    ecx, 0ffh
        shr    di, 8
        shl    ecx, 8
        mov    eax, edi
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 03Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0F3h, 0B3h, 0FDh, 0FFh
        add    esp, 10h
loc_1_6844:
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_6879
        mov    al, byte ptr [esi + 11h]
        mov    cl, byte ptr [esi + 10h]
        cmp    cl, 6
        je     SHORT loc_1_6860
        cmp    cl, 8
        je     SHORT loc_1_6860
        cmp    al, 0bh
        jne    SHORT loc_1_6879
        jmp    SHORT loc_1_6868
loc_1_6860:
        cmp    al, 8
        je     SHORT loc_1_6868
        cmp    al, 9
        jne    SHORT loc_1_6879
loc_1_6868:
        lea    eax, [ebp - 2]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0738
        mov    byte ptr [esi + 22h], 0
loc_1_6879:
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_688B:
        mov    eax, dword ptr [esi + 3bh]
        or     eax, eax
        je     SHORT loc_1_689F
        push   eax
        call   sub_1_0080
        mov    dword ptr [esi + 3bh], 0
loc_1_689F:
        mov    dword ptr [esi + 2fh], 0
        mov    word ptr [esi + 33h], 0
        mov    dword ptr [esi + 35h], 0
        mov    word ptr [esi + 39h], 0
loc_1_68B9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_68D0:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_68E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    esi, dword ptr [edi + 4]
        db  039h, 01Dh, 018h, 009h, 000h, 000h
        je     SHORT loc_1_693C
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 18h]
        cmp    eax, ebx
        jne    SHORT loc_1_690E
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_690E:
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 00Ch, 001h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0EEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0FBh, 0B2h, 0FDh, 0FFh
        add    esp, 10h
loc_1_693C:
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 28h]
        or     eax, eax
        je     SHORT loc_1_694C
        push   edi
        call   eax
        add    esp, 4
loc_1_694C:
        cmp    dword ptr [esi + 8], edi
        jne    NEAR PTR loc_1_6CDD
        mov    ecx, dword ptr [edi + 8]
        mov    ax, word ptr [ecx + 0ch]
        cmp    ax, 4
        jne    NEAR PTR loc_1_6A61
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_6996
loc_1_6970:
        mov    cl, byte ptr [eax + 10h]
        cmp    cl, 1
        je     SHORT loc_1_6988
        cmp    cl, 5
        jbe    SHORT loc_1_6982
        cmp    cl, 9
        jb     SHORT loc_1_6988
loc_1_6982:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_6970
loc_1_6988:
        or     eax, eax
        je     SHORT loc_1_6996
        mov    dword ptr [esi + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_6996:
        mov    ebx, 804h
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_69D8
        xor    eax, eax
        db  066h, 0A1h, 02Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 076h, 0B2h, 0FDh, 0FFh
        add    esp, 8
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_69D8
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 065h, 0B2h, 0FDh, 0FFh
        add    esp, 4
loc_1_69D8:
        mov    dword ptr [esi + 8], 0
        mov    ecx, dword ptr [esi + 4]
        or     ecx, ecx
        je     SHORT loc_1_6A17
        mov    eax, 4
        add    eax, 0
loc_1_69F0:
        mov    edx, dword ptr [ecx + 8]
        cmp    word ptr [edx + 0ch], ax
        jne    SHORT loc_1_6A0A
        cmp    word ptr [edx + 10h], 0
        je     SHORT loc_1_6A0A
        cmp    byte ptr [ecx + 10h], 9
        je     SHORT loc_1_6A12
        cmp    ecx, edi
        je     SHORT loc_1_6A12
loc_1_6A0A:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_69F0
        jmp    SHORT loc_1_6A17
loc_1_6A12:
        mov    ebx, 800h
loc_1_6A17:
        cmp    ebx, 800h
        je     SHORT loc_1_6A45
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6A45
        xor    eax, eax
        db  066h, 0A1h, 03Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0F2h, 0B1h, 0FDh, 0FFh
        add    esp, 8
loc_1_6A45:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0d0h], ebx
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_6A61:
        cmp    word ptr [ecx + 0eh], 21c0h
        jne    NEAR PTR loc_1_6B70
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ecx + 0feh]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_6AF2
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        jne    SHORT loc_1_6AA0
        mov    dword ptr [ecx + 0d0h], 803h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_6AA0:
        cmp    dword ptr [esi + 18h], eax
        jne    SHORT loc_1_6AAC
        mov    dword ptr [esi + 18h], 0
loc_1_6AAC:
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esi + 8], eax
        mov    byte ptr [eax + 11h], 2
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 11h], 0
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        mov    ebx, 1
loc_1_6AF2:
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_1_6B47
        mov    byte ptr [eax + 11h], 2
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        mov    byte ptr [eax + 11h], 0
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        or     ebx, ebx
        jne    NEAR PTR loc_1_6CDD
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esi + 8], eax
        mov    ebx, 1
loc_1_6B47:
        or     ebx, ebx
        jne    NEAR PTR loc_1_6CDD
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax + 8]
        cmp    word ptr [eax + 0ch], 2
        jae    SHORT loc_1_6BB5
        mov    ecx, 2
loc_1_6B60:
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax + 8]
        cmp    word ptr [eax + 0ch], cx
        jb     SHORT loc_1_6B60
        jmp    SHORT loc_1_6BB5
        db  090h
loc_1_6B70:
        cmp    ax, 1
        jne    SHORT loc_1_6BB5
        cmp    dword ptr [esi + 14h], edi
        jne    SHORT loc_1_6B92
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_1_6B92
        cmp    byte ptr [eax + 10h], 9
        je     SHORT loc_1_6B92
        mov    dword ptr [esi + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_6B92:
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_6BB2
        mov    eax, 1
        lea    esp, [esp]
loc_1_6BA0:
        mov    ecx, dword ptr [edi]
        mov    edx, dword ptr [ecx + 8]
        cmp    word ptr [edx + 0ch], ax
        jne    SHORT loc_1_6BB2
        mov    edi, ecx
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_6BA0
loc_1_6BB2:
        mov    dword ptr [esi + 8], edi
loc_1_6BB5:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_6C01
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6BE1
        xor    eax, eax
        db  066h, 0A1h, 030h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 056h, 0B0h, 0FDh, 0FFh
        add    esp, 8
loc_1_6BE1:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0d0h], 804h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_6C01:
        mov    eax, dword ptr [edi + 8]
        cmp    word ptr [eax + 0ch], 2
        jne    SHORT loc_1_6C49
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_1_6C47
        mov    byte ptr [edi + 11h], 2
        push   edi
        call   PPPMAC_Service_07
        add    esp, 4
        push   edi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    byte ptr [edi + 11h], 0
        push   edi
        call   PPPMAC_Service_07
        add    esp, 4
        push   edi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    dword ptr [esi + 8], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_6C47:
        mov    edi, dword ptr [edi]
loc_1_6C49:
        or     edi, edi
        jne    SHORT loc_1_6C93
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6C73
        xor    eax, eax
        db  066h, 0A1h, 030h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0C4h, 0AFh, 0FDh, 0FFh
        add    esp, 8
loc_1_6C73:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0d0h], 800h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_6C93:
        mov    dword ptr [esi + 8], edi
        mov    esi, edi
        or     edi, edi
        je     SHORT loc_1_6CBC
        mov    ebx, 2
loc_1_6CA1:
        mov    byte ptr [edi + 11h], bl
        push   edi
        call   PPPMAC_Service_07
        add    esp, 4
        push   edi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_6CA1
loc_1_6CBC:
        or     esi, esi
        je     SHORT loc_1_6CDD
        sub    ebx, ebx
loc_1_6CC2:
        mov    byte ptr [esi + 11h], bl
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    esi, dword ptr [esi]
        cmp    esi, ebx
        jne    SHORT loc_1_6CC2
loc_1_6CDD:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_6CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_1_6D54
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        jne    SHORT loc_1_6D26
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_6D26:
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 00Eh, 001h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0EEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E3h, 0AEh, 0FDh, 0FFh
        add    esp, 10h
loc_1_6D54:
        cmp    byte ptr [edi + 22h], 0
        je     SHORT loc_1_6D6B
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [edi + 16h]
        push   eax
        call   sub_1_0738
        mov    byte ptr [edi + 22h], 0
loc_1_6D6B:
        cmp    dword ptr [ebx + 4], edi
        jne    SHORT loc_1_6D98
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     SHORT loc_1_6D92
loc_1_6D76:
        mov    byte ptr [esi + 11h], 1
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_6D76
loc_1_6D92:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebx + 8], eax
loc_1_6D98:
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 2ch]
        or     eax, eax
        je     SHORT loc_1_6DA8
        push   edi
        call   eax
        add    esp, 4
loc_1_6DA8:
        mov    eax, dword ptr [edi + 8]
        cmp    word ptr [eax + 0ch], 4
        jne    NEAR PTR loc_1_6E78
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     NEAR PTR loc_1_6E78
        cmp    edi, eax
        jne    NEAR PTR loc_1_6E78
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_6DF8
loc_1_6DD0:
        mov    cl, byte ptr [eax + 10h]
        cmp    cl, 1
        je     SHORT loc_1_6DE8
        cmp    cl, 5
        jbe    SHORT loc_1_6DE2
        cmp    cl, 9
        jb     SHORT loc_1_6DE8
loc_1_6DE2:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_6DD0
loc_1_6DE8:
        or     eax, eax
        je     SHORT loc_1_6DF8
        mov    dword ptr [ebx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_6DF8:
        mov    esi, 804h
        mov    dword ptr [ebx + 8], 0
        mov    ecx, dword ptr [ebx + 4]
        or     ecx, ecx
        je     SHORT loc_1_6E38
        mov    edx, 4
        sub    ebx, ebx
loc_1_6E12:
        mov    eax, dword ptr [ecx + 8]
        cmp    word ptr [eax + 0ch], dx
        jne    SHORT loc_1_6E2B
        cmp    word ptr [eax + 10h], bx
        je     SHORT loc_1_6E2B
        cmp    byte ptr [ecx + 10h], 9
        jne    SHORT loc_1_6E2B
        cmp    edi, ecx
        jne    SHORT loc_1_6E33
loc_1_6E2B:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, ebx
        jne    SHORT loc_1_6E12
        jmp    SHORT loc_1_6E38
loc_1_6E33:
        mov    esi, 800h
loc_1_6E38:
        cmp    esi, 800h
        je     SHORT loc_1_6E66
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6E66
        xor    eax, eax
        db  066h, 0A1h, 03Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D1h, 0ADh, 0FDh, 0FFh
        add    esp, 8
loc_1_6E66:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    dword ptr [eax + 0d0h], esi
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_6E78:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_6E90:
        push   ebp
        mov    ebp, esp
        push   esi
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_1_6EE6
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        jne    SHORT loc_1_6EB8
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_6EB8:
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 010h, 001h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0EEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 051h, 0ADh, 0FDh, 0FFh
        add    esp, 10h
loc_1_6EE6:
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 30h]
        or     eax, eax
        je     SHORT loc_1_6EF6
        push   esi
        call   eax
        add    esp, 4
loc_1_6EF6:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_6F00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        mov    ebx, dword ptr [esi + 4]
        mov    edi, dword ptr [ebx + 0ch]
        je     SHORT loc_1_6F61
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        jne    SHORT loc_1_6F33
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_6F33:
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 012h, 001h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0EEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D6h, 0ACh, 0FDh, 0FFh
        add    esp, 10h
loc_1_6F61:
        lea    eax, [esi + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 34h]
        or     eax, eax
        je     SHORT loc_1_6F76
        push   esi
        call   eax
        add    esp, 4
loc_1_6F76:
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 4], esi
        jne    SHORT loc_1_6FA5
        mov    eax, dword ptr [edi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_6F92
        cmp    eax, 801h
        jne    SHORT loc_1_6F9C
loc_1_6F92:
        mov    dword ptr [edi + 0d0h], 802h
loc_1_6F9C:
        push   edi
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_6FA5:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 0ch]
        cmp    ax, 1
        jne    SHORT loc_1_7016
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_6FDA
        xor    eax, eax
        db  066h, 0A1h, 034h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 05Dh, 0ACh, 0FDh, 0FFh
        add    esp, 8
loc_1_6FDA:
        cmp    byte ptr [esi + 11h], 5
        mov    dword ptr [edi + 0d0h], 817h
        je     SHORT loc_1_6FF4
        mov    dword ptr [edi + 0d0h], 803h
loc_1_6FF4:
        mov    esi, dword ptr [ebx + 4]
        push   esi
        mov    byte ptr [esi + 11h], 3
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_7016:
        cmp    ax, 2
        jne    SHORT loc_1_7082
        mov    eax, dword ptr [edi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_7030
        cmp    eax, 801h
        jne    SHORT loc_1_7060
loc_1_7030:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_7056
        xor    eax, eax
        db  066h, 0A1h, 036h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E1h, 0ABh, 0FDh, 0FFh
        add    esp, 8
loc_1_7056:
        mov    dword ptr [edi + 0d0h], 805h
loc_1_7060:
        mov    esi, dword ptr [ebx + 4]
        push   esi
        mov    byte ptr [esi + 11h], 3
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_7082:
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_7099
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0738
        mov    byte ptr [esi + 22h], 0
loc_1_7099:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        cmp    word ptr [eax + 0ch], 4
        jne    NEAR PTR loc_1_7192
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     NEAR PTR loc_1_7192
        cmp    esi, eax
        jne    NEAR PTR loc_1_7192
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_70F0
loc_1_70C2:
        mov    cl, byte ptr [eax + 10h]
        cmp    cl, 1
        je     SHORT loc_1_70DA
        cmp    cl, 5
        jbe    SHORT loc_1_70D4
        cmp    cl, 9
        jb     SHORT loc_1_70DA
loc_1_70D4:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_70C2
loc_1_70DA:
        or     eax, eax
        je     SHORT loc_1_70F0
        mov    dword ptr [ebx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_70F0:
        mov    esi, 804h
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_711B
        xor    eax, eax
        db  066h, 0A1h, 038h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 01Ch, 0ABh, 0FDh, 0FFh
        add    esp, 8
loc_1_711B:
        mov    dword ptr [ebx + 8], 0
        mov    eax, dword ptr [ebx + 4]
        or     eax, eax
        je     SHORT loc_1_7155
        mov    edx, 4
        sub    ebx, ebx
loc_1_7130:
        mov    ecx, dword ptr [eax + 8]
        cmp    word ptr [ecx + 0ch], dx
        jne    SHORT loc_1_7145
        cmp    word ptr [ecx + 10h], bx
        je     SHORT loc_1_7145
        cmp    byte ptr [eax + 10h], 9
        je     SHORT loc_1_7150
loc_1_7145:
        mov    eax, dword ptr [eax]
        cmp    eax, ebx
        jne    SHORT loc_1_7130
        jmp    SHORT loc_1_7155
        db  08Dh, 049h, 000h
loc_1_7150:
        mov    esi, 800h
loc_1_7155:
        cmp    esi, 800h
        je     SHORT loc_1_7183
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_7183
        xor    eax, eax
        db  066h, 0A1h, 03Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B4h, 0AAh, 0FDh, 0FFh
        add    esp, 8
loc_1_7183:
        mov    dword ptr [edi + 0d0h], esi
        push   edi
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_7192:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_71A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 11h]
        cmp    al, 3
        je     SHORT loc_1_71B7
        cmp    al, 0eh
        je     SHORT loc_1_71B7
        mov    ax, word ptr [ecx + 2bh]
        jmp    SHORT loc_1_71BB
loc_1_71B7:
        mov    ax, word ptr [ecx + 29h]
loc_1_71BB:
        mov    word ptr [ecx + 25h], ax
        mov    ax, word ptr [ecx + 27h]
        mov    word ptr [ecx + 23h], ax
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_71D0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    word ptr [eax + 25h], 0
        mov    byte ptr [eax + 22h], 1
        mov    cx, word ptr [eax + 23h]
        push   ecx
        add    eax, 16h
        push   eax
        call   sub_1_0732
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7200:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 0ch]
        sub    al, al
        mov    ecx, dword ptr [ecx + 38h]
        or     ecx, ecx
        je     SHORT loc_1_7227
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   ecx
        add    esp, 8
loc_1_7227:
        or     al, al
        je     NEAR PTR loc_1_72C5
        xor    eax, eax
        add    word ptr [ebp - 2], 4
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   edi
        call   PPPMAC_Service_02
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_7268
        mov    byte ptr [esi + 22h], 1
        push   1f4h
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0732
        dec    word ptr [esi + 25h]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_7268:
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [edi + 28h], ax
        mov    eax, dword ptr [edi + 1ch]
        add    eax, dword ptr [edi + 20h]
        mov    byte ptr [eax], 1
        mov    cl, byte ptr [esi + 14h]
        inc    cl
        mov    byte ptr [esi + 14h], cl
        mov    byte ptr [esi + 15h], cl
        mov    byte ptr [eax + 1], cl
        mov    cx, word ptr [ebp - 2]
        mov    byte ptr [eax + 2], ch
        add    eax, 4
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax - 1], cl
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   esi
        call   dword ptr [eax + 3ch]
        add    esp, 8
        push   1
        push   edi
        call   sub_1_4360
        mov    byte ptr [esi + 22h], 1
        xor    eax, eax
        mov    ax, word ptr [esi + 23h]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0732
        dec    word ptr [esi + 25h]
loc_1_72C5:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_72D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 3bh]
        cmp    word ptr [esi + 39h], 0
        jne    SHORT loc_1_72F7
        or     ecx, ecx
        je     SHORT loc_1_731A
        push   ecx
        call   sub_1_0080
        mov    dword ptr [esi + 3bh], 0
        pop    esi
        pop    ebp
        ret    4
loc_1_72F7:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 3bh], 0
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx + 28h], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 39h]
        push   1
        mov    dword ptr [ecx + 24h], eax
        push   ecx
        call   sub_1_4360
loc_1_731A:
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_1_7320:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   4
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_7357
        mov    byte ptr [esi + 22h], 1
        push   1f4h
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0732
        dec    word ptr [esi + 25h]
        pop    esi
        pop    ebp
        ret    4
loc_1_7357:
        mov    ecx, dword ptr [esi + 8]
        mov    cx, word ptr [ecx + 0eh]
        mov    word ptr [eax + 28h], cx
        mov    ecx, dword ptr [eax + 1ch]
        add    ecx, dword ptr [eax + 20h]
        mov    byte ptr [ecx], 5
        mov    dl, byte ptr [esi + 14h]
        inc    dl
        mov    byte ptr [esi + 14h], dl
        mov    byte ptr [esi + 15h], dl
        mov    byte ptr [ecx + 1], dl
        mov    byte ptr [ecx + 2], 0
        mov    byte ptr [ecx + 3], 4
        push   1
        push   eax
        call   sub_1_4360
        mov    byte ptr [esi + 22h], 1
        xor    eax, eax
        mov    ax, word ptr [esi + 23h]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0732
        dec    word ptr [esi + 25h]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_73B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   4
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_73F9
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [eax + 1ch]
        add    edx, dword ptr [eax + 20h]
        mov    cx, word ptr [ecx + 0eh]
        mov    word ptr [eax + 28h], cx
        mov    byte ptr [edx], 6
        mov    ecx, dword ptr [esi + 2fh]
        push   1
        mov    cl, byte ptr [ecx + 1]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx + 2], 0
        mov    byte ptr [edx + 3], 4
        push   eax
        call   sub_1_4360
loc_1_73F9:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_7400:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 2fh]
        mov    dword ptr [ebp - 8], ecx
        mov    ax, word ptr [edi + 33h]
        mov    ebx, eax
        and    ebx, 0ffffh
        mov    word ptr [ebp - 2], ax
        lea    eax, [ebx + 4]
        push   eax
        mov    eax, dword ptr [edx + 0ch]
        push   eax
        call   PPPMAC_Service_02
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_7484
        mov    eax, dword ptr [edi + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [esi + 28h], ax
        mov    eax, dword ptr [esi + 1ch]
        add    eax, dword ptr [esi + 20h]
        mov    byte ptr [eax], 7
        mov    cl, byte ptr [edi + 14h]
        inc    cl
        mov    byte ptr [edi + 14h], cl
        mov    byte ptr [eax + 1], cl
        lea    ecx, [ebx + 4]
        shr    ecx, 8
        mov    dx, word ptr [ebp - 2]
        add    dx, 4
        mov    ch, dl
        mov    word ptr [eax + 2], cx
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        add    eax, 4
        push   eax
        call   sub_1_083C
        push   1
        push   esi
        call   sub_1_4360
loc_1_7484:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_7490:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 2fh]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx + 0ch]
        push   eax
        call   PPPMAC_Service_02
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_7519
        mov    eax, dword ptr [esi + 8]
        mov    ebx, dword ptr [edi + 1ch]
        add    ebx, dword ptr [edi + 20h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [edi + 28h], ax
        mov    byte ptr [ebx], 0ah
        mov    eax, dword ptr [ebp - 4]
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        push   eax
        lea    eax, [ebx + 1]
        push   eax
        call   sub_1_083C
        mov    ecx, dword ptr [esi + 7ah]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebx + 4], edx
        push   1
        push   edi
        call   sub_1_4360
loc_1_7519:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7530:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    bx, word ptr [ebp + 10h]
        cmp    bx, 5c8h
        mov    ecx, dword ptr [edi + 0ch]
        jbe    SHORT loc_1_754E
        mov    bx, 5c8h
loc_1_754E:
        lea    eax, [ebx + 4]
        mov    word ptr [ebp - 2], ax
        and    eax, 0ffffh
        push   eax
        push   ecx
        call   PPPMAC_Service_02
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_75AF
        mov    ecx, dword ptr [esi + 1ch]
        add    ecx, dword ptr [esi + 20h]
        mov    word ptr [esi + 28h], 21c0h
        mov    byte ptr [ecx], 8
        mov    eax, dword ptr [edi + 4]
        inc    byte ptr [eax + 14h]
        mov    eax, dword ptr [edi + 4]
        mov    al, byte ptr [eax + 14h]
        mov    byte ptr [ecx + 1], al
        mov    ah, byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    word ptr [ecx + 2], ax
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        add    ecx, 4
        push   ecx
        call   sub_1_083C
        push   1
        push   esi
        call   sub_1_4360
loc_1_75AF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_75C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dx, word ptr [eax + 4]
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        or     esi, esi
        je     SHORT loc_1_75DE
        mov    ecx, dword ptr [esi + 8]
        cmp    word ptr [ecx + 0eh], dx
        je     SHORT loc_1_75F8
loc_1_75DE:
        mov    esi, dword ptr [eax + 4]
        or     esi, esi
        je     SHORT loc_1_7653
loc_1_75E5:
        mov    eax, dword ptr [esi + 8]
        cmp    word ptr [eax + 0eh], dx
        je     SHORT loc_1_75F4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_75E5
loc_1_75F4:
        or     esi, esi
        je     SHORT loc_1_7653
loc_1_75F8:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_763D
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 02Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0FAh, 0A5h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_763D:
        mov    byte ptr [esi + 11h], 0eh
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_7653:
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_7660:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        cmp    edi, ebx
        je     SHORT loc_1_7689
        mov    esi, dword ptr [edi + 0cch]
        mov    eax, dword ptr [esi + 4]
        cmp    eax, ebx
        je     SHORT loc_1_769C
        nop
loc_1_7680:
        inc    ebx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_7680
        jmp    SHORT loc_1_769C
loc_1_7689:
        db  0A1h, 020h, 089h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_7699
loc_1_7692:
        inc    ebx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_7692
loc_1_7699:
        mov    esi, dword ptr [ebp - 4]
loc_1_769C:
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebx*8 + 1ch]
        cmp    dword ptr [ecx + 8], eax
        jae    SHORT loc_1_76C0
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], 80bh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_76C0:
        lea    edx, [ecx + 1ch]
        mov    word ptr [ecx + 10h], bx
        or     edi, edi
        je     NEAR PTR loc_1_7760
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_1_76E3
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx + 12h], ax
        jmp    SHORT loc_1_76E9
loc_1_76E3:
        mov    word ptr [ecx + 12h], 0
loc_1_76E9:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_7700
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx + 14h], ax
        jmp    SHORT loc_1_7706
        db  08Dh, 049h, 000h
loc_1_7700:
        mov    word ptr [ecx + 14h], 0
loc_1_7706:
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_1_7720
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx + 16h], ax
        jmp    SHORT loc_1_7726
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_7720:
        mov    word ptr [ecx + 16h], 0
loc_1_7726:
        mov    esi, dword ptr [esi + 4]
        or     esi, esi
        je     SHORT loc_1_778C
loc_1_7730:
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [edx], ax
        mov    al, byte ptr [esi + 10h]
        mov    byte ptr [edx + 4], al
        add    edx, 8
        mov    al, byte ptr [esi + 11h]
        mov    byte ptr [edx - 2], al
        mov    al, byte ptr [esi + 12h]
        mov    byte ptr [edx - 1], al
        mov    al, byte ptr [esi + 13h]
        mov    byte ptr [edx - 3], al
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_7730
        jmp    SHORT loc_1_778C
        db  08Dh, 049h, 000h
loc_1_7760:
        db  0A1h, 020h, 089h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_778C
        sub    ebx, ebx
        add    eax, 0
loc_1_7770:
        mov    si, word ptr [eax + 0eh]
        mov    word ptr [edx], si
        mov    byte ptr [edx + 4], bl
        mov    byte ptr [edx + 6], bl
        mov    byte ptr [edx + 7], bl
        mov    byte ptr [edx + 5], bl
        add    edx, 8
        mov    eax, dword ptr [eax]
        cmp    eax, ebx
        jne    SHORT loc_1_7770
loc_1_778C:
        mov    dword ptr [ecx + 0ch], 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_77A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        jne    SHORT loc_1_77C0
        mov    edx, dword ptr [ebp + 0ch]
        pop    esi
        mov    dword ptr [edx + 0ch], 80ch
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_77C0:
        mov    eax, dword ptr [eax + 0cch]
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 4]
        or     esi, esi
        je     SHORT loc_1_77E3
        mov    ax, word ptr [edx + 10h]
loc_1_77D4:
        mov    ecx, dword ptr [esi + 8]
        cmp    word ptr [ecx + 0eh], ax
        je     SHORT loc_1_77E3
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_77D4
loc_1_77E3:
        or     esi, esi
        jne    SHORT loc_1_77F3
        mov    dword ptr [edx + 0ch], 80dh
        pop    esi
        pop    ebp
        ret    8
loc_1_77F3:
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 44h]
        or     eax, eax
        je     SHORT loc_1_7810
        push   edx
        push   esi
        call   eax
        add    esp, 8
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7810:
        mov    dword ptr [edx + 0ch], 80eh
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
PPPMAC_Service_01:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        or     ebx, ebx
        jne    SHORT loc_1_7840
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_7840:
        cmp    dword ptr [ebx], 4ch
        je     SHORT loc_1_7851
        mov    eax, 80fh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_7851:
        mov    esi, 54h
        mov    edi, dword ptr [ebx + 10h]
        or     edi, edi
        je     SHORT loc_1_786B
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx + 54h]
loc_1_786B:
        mov    edi, dword ptr [ebx + 14h]
        or     edi, edi
        je     SHORT loc_1_787F
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
loc_1_787F:
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_78A3
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_78A3:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, 12h
        mov    dword ptr [eax + 8], esi
        lea    esi, [ebx + 4]
        mov    edi, dword ptr [ebp - 4]
        add    edi, 0ch
        rep movsd
        sub    edx, edx
        cmp    dword ptr [ebx + 10h], edx
        je     SHORT loc_1_7912
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 54h
        mov    dword ptr [ecx + 18h], eax
        mov    edi, dword ptr [ebx + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ecx - 1]
        mov    edi, dword ptr [eax + 18h]
        mov    esi, dword ptr [ebx + 10h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        mov    byte ptr [edx + eax], 0
        inc    edx
loc_1_7912:
        cmp    dword ptr [ebx + 14h], 0
        je     SHORT loc_1_795C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 54h
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        add    dword ptr [eax + 1ch], edx
        mov    edi, dword ptr [ebx + 14h]
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ecx - 1]
        mov    edi, dword ptr [eax + 1ch]
        mov    esi, dword ptr [ebx + 14h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        mov    byte ptr [edx + eax], 0
loc_1_795C:
        db  0B9h, 020h, 089h, 001h, 000h
        db  083h, 03Dh, 020h, 089h, 001h, 000h, 000h
        je     SHORT loc_1_7980
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
loc_1_7971:
        mov    edx, dword ptr [ecx]
        cmp    word ptr [edx + 0ch], ax
        ja     SHORT loc_1_7980
        mov    ecx, edx
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_7971
loc_1_7980:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_79A4
        mov    edx, dword ptr [ebp - 4]
        db  0A1h, 024h, 089h, 001h, 000h
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 024h, 089h, 001h, 000h
        jmp    SHORT loc_1_79B7
loc_1_79A4:
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 4], eax
loc_1_79B7:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_79D0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0cch]
        or     ecx, ecx
        jne    SHORT loc_1_79E9
        mov    eax, 0c0000001h
        pop    ebp
        ret    8
loc_1_79E9:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_7A02
        mov    eax, dword ptr [ecx + 4]
        cmp    byte ptr [eax + 10h], 6
        jb     SHORT loc_1_7A02
        push   edx
        call   sub_1_6530
        pop    ebp
        ret    8
loc_1_7A02:
        push   edx
        call   sub_1_6500
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
PPPMAC_Service_02:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   0
        or     eax, eax
        je     SHORT loc_1_7A30
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_7A30:
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [edx + 8]
        cmp    dword ptr [eax + 18h], ecx
        jae    SHORT loc_1_7A50
        push   eax
        call   sub_1_0080
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_7A50:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2bh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
PPPMAC_Service_03:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0080
        pop    ebp
        ret
        db  08Bh, 0FFh
PPPMAC_Service_04:
        push   ebp
        mov    ebp, esp
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_4360
        pop    ebp
        ret
PPPMAC_Service_05:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [ebp + 0ch]
        mov    edx, dword ptr [esi + 20h]
        mov    eax, edx
        mov    ecx, dword ptr [edx + 12h]
        sub    eax, ecx
        cmp    eax, -12h
        je     SHORT loc_1_7AD1
        nop
loc_1_7AC0:
        cmp    word ptr [ecx + 47h], di
        je     SHORT loc_1_7AD1
        mov    ecx, dword ptr [ecx]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, -12h
        jne    SHORT loc_1_7AC0
loc_1_7AD1:
        sub    edx, ecx
        cmp    edx, -12h
        je     NEAR PTR loc_1_7B6C
        mov    ebx, 7dh
        mov    eax, dword ptr [esi + 8]
        sub    edx, edx
        div    ebx
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [esi + 0ch]
        sub    edx, edx
        mov    dword ptr [ebp - 14h], eax
        mov    word ptr [ebp - 10h], 2
        nop
loc_1_7B00:
        lea    ebx, [esi + edx]
        inc    edx
        mov    al, byte ptr [ebx + 90h]
        mov    byte ptr [ebp + edx - 9], al
        mov    al, byte ptr [ebx + 96h]
        mov    byte ptr [ebp + edx - 0fh], al
        cmp    edx, 6
        jl     SHORT loc_1_7B00
        mov    word ptr [ebp - 2], 0
        cmp    dword ptr [esi + 0ech], 3
        je     SHORT loc_1_7B3D
        cmp    di, 3f00h
        jne    SHORT loc_1_7B3D
        cmp    dword ptr [ecx + 3dh], 1
        jne    SHORT loc_1_7B3D
        add    dword ptr [ebp - 18h], 0eh
loc_1_7B3D:
        cmp    di, 2b00h
        jne    SHORT loc_1_7B54
        cmp    dword ptr [ebp - 18h], 488h
        jbe    SHORT loc_1_7B54
        mov    dword ptr [ebp - 18h], 488h
loc_1_7B54:
        mov    byte ptr [ecx + 49h], 1
        push   1ch
        lea    eax, [ebp - 1ch]
        push   eax
        push   40010008h
        mov    eax, dword ptr [ecx + 10h]
        push   eax
        call   sub_1_06BE
loc_1_7B6C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
PPPMAC_Service_06:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [edx + 20h]
        mov    eax, edi
        mov    esi, dword ptr [edi + 12h]
        sub    eax, esi
        cmp    eax, -12h
        je     SHORT loc_1_7BB1
        mov    ax, word ptr [ebp + 0ch]
        mov    edi, edi
loc_1_7BA0:
        cmp    word ptr [esi + 47h], ax
        je     SHORT loc_1_7BB1
        mov    esi, dword ptr [esi]
        mov    ecx, edi
        sub    ecx, esi
        cmp    ecx, -12h
        jne    SHORT loc_1_7BA0
loc_1_7BB1:
        sub    edi, esi
        cmp    edi, -12h
        je     SHORT loc_1_7BE9
        cmp    byte ptr [esi + 49h], 0
        je     SHORT loc_1_7BE9
        sub    eax, eax
loc_1_7BC0:
        mov    cl, byte ptr [edx + eax + 96h]
        mov    byte ptr [ebp + eax - 0ch], cl
        inc    eax
        cmp    eax, 6
        jl     SHORT loc_1_7BC0
        push   6
        lea    eax, [ebp - 0ch]
        push   eax
        push   40010009h
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_06BE
        mov    byte ptr [esi + 49h], 0
loc_1_7BE9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
PPPMAC_Service_07:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 11h]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 10h]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Dh, 004h, 085h, 064h, 089h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        sub    edi, edi
loc_1_7C18:
        mov    eax, dword ptr [ebp - 4]
        mov    bl, byte ptr [edi + eax]
        cmp    bl, 0ffh
        je     SHORT loc_1_7C3C
        mov    byte ptr [esi + 12h], bl
        push   esi
        xor    eax, eax
        mov    al, bl
        db  0FFh, 014h, 085h, 02Ch, 089h, 001h, 000h
        or     bl, bl
        je     SHORT loc_1_7C3C
        inc    edi
        cmp    edi, 3
        jl     SHORT loc_1_7C18
loc_1_7C3C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
PPPMAC_Service_08:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 11h]
        mov    cl, byte ptr [esi + 10h]
        xor    edx, edx
        mov    dl, cl
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*2]
        db  08Dh, 004h, 085h, 064h, 089h, 001h, 000h
        mov    byte ptr [esi + 13h], cl
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [esi + 10h], al
        cmp    al, 4
        jb     SHORT loc_1_7C84
        cmp    al, 8
        jbe    SHORT loc_1_7C9B
loc_1_7C84:
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_7C9B
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0738
        mov    byte ptr [esi + 22h], 0
loc_1_7C9B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_7CA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    di, di
        db  08Bh, 035h, 020h, 089h, 001h, 000h
        or     esi, esi
        je     SHORT loc_1_7CE9
        mov    ebx, dword ptr [ebp + 8]
loc_1_7CB6:
        mov    ecx, dword ptr [esi + 1ch]
        or     ecx, ecx
        je     SHORT loc_1_7CD1
        xor    eax, eax
        mov    ax, word ptr [ebx]
        push   eax
        mov    eax, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_1_16E0
        or     eax, eax
        je     SHORT loc_1_7CE0
loc_1_7CD1:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_7CB6
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
loc_1_7CE0:
        mov    di, word ptr [esi + 0eh]
        and    di, 0ff7fh
loc_1_7CE9:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7D00:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ax, ax
        db  08Bh, 015h, 020h, 089h, 001h, 000h
        or     edx, edx
        je     SHORT loc_1_7D38
        mov    cx, word ptr [ebp + 8]
loc_1_7D15:
        mov    si, word ptr [edx + 10h]
        or     si, si
        je     SHORT loc_1_7D23
        cmp    cx, si
        je     SHORT loc_1_7D30
loc_1_7D23:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_7D15
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_7D30:
        mov    ax, word ptr [edx + 0eh]
        and    ax, 0ff7fh
loc_1_7D38:
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_7D40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        call   sub_1_01F7
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7D60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 0
        mov    word ptr [ebp - 46h], 21c0h
        mov    word ptr [ebp - 44h], 0
        mov    dword ptr [ebp - 42h], 6fh
        mov    byte ptr [ebp - 3eh], 1
        mov    byte ptr [ebp - 3dh], 1
        mov    dword ptr [ebp - 3ch], 0
        mov    dword ptr [ebp - 38h], 0
        db  0C7h, 045h, 0CCh, 090h, 07Eh, 000h, 000h
        mov    dword ptr [ebp - 30h], 0
        db  0C7h, 045h, 0D4h, 050h, 07Fh, 000h, 000h
        db  0C7h, 045h, 0D8h, 000h, 080h, 000h, 000h
        db  0C7h, 045h, 0DCh, 0A0h, 080h, 000h, 000h
        db  0C7h, 045h, 0E0h, 050h, 080h, 000h, 000h
        db  0C7h, 045h, 0E4h, 010h, 081h, 000h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 0C0h, 081h, 000h, 000h
        db  0C7h, 045h, 0ECh, 060h, 083h, 000h, 000h
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        db  066h, 0A1h, 0FAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_7E20:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax], 5dch
        mov    dword ptr [eax + 2], 0ffffffffh
        mov    word ptr [eax + 6], 0
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 0ch], 0
        mov    dword ptr [eax + 0eh], 0
        mov    dword ptr [eax + 12h], 0
        mov    byte ptr [eax + 16h], 0
        mov    byte ptr [eax + 17h], 0
        mov    byte ptr [eax + 18h], 0
        mov    byte ptr [eax + 19h], 0
        mov    byte ptr [eax + 1ah], 0
        mov    byte ptr [eax + 1bh], 0
        mov    byte ptr [eax + 1ch], 0
        mov    byte ptr [eax + 1dh], 0
        mov    byte ptr [eax + 1eh], 0
        mov    byte ptr [eax + 1fh], 0
        mov    byte ptr [eax + 20h], 0
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7E90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 4bh]
        push   esi
        call   sub_1_7E20
        lea    eax, [esi + 42h]
        push   eax
        call   sub_1_7E20
        mov    dword ptr [esi + 2], 0a0000h
        mov    byte ptr [esi + 1ah], 1
        call   sub_1_7D40
        mov    dword ptr [esi + 0eh], eax
        or     eax, eax
        jne    SHORT loc_1_7EC8
        mov    dword ptr [esi + 0eh], 0deadbeefh
loc_1_7EC8:
        mov    byte ptr [esi + 1dh], 1
        mov    byte ptr [esi + 16h], 1
        mov    byte ptr [esi + 1eh], 1
        mov    byte ptr [esi + 17h], 1
        mov    byte ptr [esi + 1fh], 1
        mov    eax, dword ptr [edi + 4]
        mov    eax, dword ptr [eax + 1ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_7F02
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ecx + 8]
        cmp    word ptr [eax + 0ch], 1
        jne    SHORT loc_1_7F45
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [esi + 6], ax
        mov    byte ptr [esi + 1bh], 1
        mov    dword ptr [esi + 8], ecx
loc_1_7F02:
        mov    eax, dword ptr [edi + 4]
        mov    eax, dword ptr [eax + 1ch]
        test   byte ptr [eax + 0ch], 8
        je     SHORT loc_1_7F3C
        mov    byte ptr [esi + 18h], 1
        mov    byte ptr [esi + 20h], 1
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_7F3C
        xor    eax, eax
        db  066h, 0A1h, 03Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0FBh, 09Ch, 0FDh, 0FFh
        add    esp, 8
loc_1_7F3C:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_7F45:
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_7F50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 4]
        lea    esi, [edx + 4bh]
        mov    edi, dword ptr [eax + 0ch]
        lea    ecx, [esi + 21h]
        xor    eax, eax
        mov    ax, word ptr [esi + 42h]
        add    esi, 42h
        mov    dword ptr [edi + 18h], eax
        mov    ax, word ptr [ecx]
        mov    word ptr [edi + 88h], ax
        mov    al, byte ptr [esi + 16h]
        mov    byte ptr [edi + 8ah], al
        mov    al, byte ptr [esi + 17h]
        mov    byte ptr [edi + 8bh], al
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [edi + 8ch], eax
        cmp    byte ptr [ecx + 1bh], 0
        je     SHORT loc_1_7FA6
        mov    edi, dword ptr [edx + 4]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edi + 14h], eax
        jmp    SHORT loc_1_7FB0
loc_1_7FA6:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax + 14h], 0
loc_1_7FB0:
        cmp    byte ptr [esi + 1bh], 0
        je     SHORT loc_1_7FD0
        mov    edi, dword ptr [edx + 4]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax + 10h], 1
        jmp    SHORT loc_1_7FDA
        db  005h, 000h, 000h, 000h, 000h
loc_1_7FD0:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax + 18h], 0
loc_1_7FDA:
        cmp    byte ptr [ecx + 20h], 0
        jne    SHORT loc_1_7FE6
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_7FF0
loc_1_7FE6:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax + 20h], 1
loc_1_7FF0:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8000:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [eax + 18h], 5dch
        mov    word ptr [eax + 88h], 5dch
        mov    byte ptr [eax + 8ah], 0
        mov    byte ptr [eax + 8bh], 0
        mov    dword ptr [eax + 8ch], 0ffffffffh
        mov    dword ptr [ecx + 20h], 0
        mov    dword ptr [ecx + 14h], 0
        mov    dword ptr [ecx + 18h], 0
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_8050:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    eax, dword ptr [edx + 4]
        mov    cl, byte ptr [edx + 10h]
        mov    eax, dword ptr [eax + 0ch]
        cmp    ecx, 4
        jl     SHORT loc_1_8091
        cmp    ecx, 5
        jle    SHORT loc_1_8072
        cmp    ecx, 6
        je     SHORT loc_1_8080
        pop    ebp
        ret
loc_1_8072:
        push   eax
        call   sub_1_16C0
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_8080:
        cmp    word ptr [edx + 25h], 0
        jne    SHORT loc_1_8091
        mov    dword ptr [eax + 0d0h], 810h
loc_1_8091:
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_80A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 4]
        mov    eax, dword ptr [edx + 0ch]
        add    ebx, 4bh
        mov    dword ptr [eax + 18h], 5dch
        mov    word ptr [eax + 88h], 5dch
        mov    byte ptr [eax + 8ah], 0
        mov    byte ptr [eax + 8bh], 0
        lea    edi, [ebx + 21h]
        mov    dword ptr [eax + 8ch], 0ffffffffh
        mov    esi, ebx
        mov    ecx, 8
        rep movsd
        movsb
        mov    dword ptr [edx + 20h], 0
        cmp    byte ptr [ebx + 41h], 0
        je     SHORT loc_1_8102
        mov    dword ptr [ebx + 63h], 1
        mov    dword ptr [ebx + 67h], 0
loc_1_8102:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8110:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        lea    edi, [ecx + 6ch]
        add    ecx, 4bh
        sub    si, si
        cmp    dword ptr [ecx + 63h], 0
        je     SHORT loc_1_8144
        cmp    dword ptr [ecx + 6bh], 0
        jne    SHORT loc_1_8144
        mov    eax, dword ptr [ecx + 67h]
        inc    eax
        mov    dword ptr [ecx + 67h], eax
        cmp    eax, 4
        jbe    SHORT loc_1_8144
        mov    dword ptr [ecx + 63h], 0
        mov    byte ptr [ecx + 41h], 0
loc_1_8144:
        cmp    byte ptr [edi + 19h], 0
        je     SHORT loc_1_814E
        mov    si, 4
loc_1_814E:
        cmp    byte ptr [edi + 1ah], 0
        je     SHORT loc_1_8158
        add    si, 6
loc_1_8158:
        cmp    byte ptr [edi + 1bh], 0
        je     SHORT loc_1_8178
        mov    ecx, dword ptr [edi + 8]
        add    si, 4
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 48h]
        or     eax, eax
        je     SHORT loc_1_8178
        push   ecx
        call   eax
        add    esp, 4
        add    si, ax
loc_1_8178:
        cmp    byte ptr [edi + 1ch], 0
        je     SHORT loc_1_8182
        add    si, 4
loc_1_8182:
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_1_818C
        add    si, 6
loc_1_818C:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_8196
        add    si, 2
loc_1_8196:
        cmp    byte ptr [edi + 1fh], 0
        je     SHORT loc_1_81A0
        add    si, 2
loc_1_81A0:
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_1_81AA
        add    si, 3
loc_1_81AA:
        mov    eax, dword ptr [ebp + 0ch]
        pop    edi
        mov    word ptr [eax], si
        mov    al, 1
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_81C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 6ch
        cmp    byte ptr [esi + 19h], 0
        je     SHORT loc_1_81F4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 1
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 4
        inc    dword ptr [ebp + 0ch]
        mov    ax, word ptr [esi]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax], cx
        add    dword ptr [ebp + 0ch], 2
loc_1_81F4:
        cmp    byte ptr [esi + 1ah], 0
        je     SHORT loc_1_823B
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 6
        inc    dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 2]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], edx
        add    dword ptr [ebp + 0ch], 4
loc_1_823B:
        cmp    byte ptr [esi + 1bh], 0
        je     SHORT loc_1_829A
        mov    ecx, dword ptr [esi + 8]
        sub    edi, edi
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 48h]
        cmp    eax, edi
        je     SHORT loc_1_8258
        push   ecx
        call   eax
        add    esp, 4
        mov    edi, eax
loc_1_8258:
        mov    eax, dword ptr [ebp + 0ch]
        lea    ecx, [edi + 4]
        mov    byte ptr [eax], 3
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp + 0ch], 2
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 8]
        mov    eax, dword ptr [eax + 4ch]
        or     eax, eax
        je     SHORT loc_1_829A
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   edx
        call   eax
        add    esp, 8
        add    dword ptr [ebp + 0ch], edi
loc_1_829A:
        cmp    byte ptr [esi + 1ch], 0
        je     SHORT loc_1_82C0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 4
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 4
        inc    dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [esi + 0ch]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp + 0ch], 2
loc_1_82C0:
        cmp    byte ptr [esi + 1dh], 0
        je     SHORT loc_1_8307
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 5
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 6
        inc    dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 0eh]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], edx
        add    dword ptr [ebp + 0ch], 4
loc_1_8307:
        cmp    byte ptr [esi + 1eh], 0
        je     SHORT loc_1_831F
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 7
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp + 0ch]
loc_1_831F:
        cmp    byte ptr [esi + 1fh], 0
        je     SHORT loc_1_8337
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 8
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp + 0ch]
loc_1_8337:
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_8358
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0dh
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 3
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 6
        inc    dword ptr [ebp + 0ch]
loc_1_8358:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_8360:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 10h
        mov    eax, dword ptr [esi + 4]
        lea    edx, [esi + 4bh]
        mov    ecx, dword ptr [esi + 2fh]
        xor    ebx, ebx
        mov    bl, byte ptr [ecx]
        mov    edi, dword ptr [eax + 0ch]
        mov    eax, ebx
        cmp    eax, 2
        jl     SHORT loc_1_839D
        cmp    eax, 4
        jle    SHORT loc_1_8391
        cmp    eax, 6
        jne    SHORT loc_1_839D
loc_1_8391:
        mov    al, byte ptr [ecx + 1]
        cmp    byte ptr [esi + 15h], al
        jne    NEAR PTR loc_1_85E5
loc_1_839D:
        lea    eax, [ebx - 1]
        cmp    eax, 0ah
        ja     SHORT loc_1_83B0
        db  0FFh, 024h, 085h, 0F4h, 085h, 000h, 000h
        db  08Dh, 064h, 024h, 000h
loc_1_83B0:
        mov    byte ptr [ebp - 1], 0ch
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_85E5
        push   ebx
        xor    eax, eax
        db  066h, 0A1h, 060h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 058h, 098h, 0FDh, 0FFh
        add    esp, 0ch
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_83EC:
        lea    eax, [esi + 39h]
        push   eax
        mov    eax, dword ptr [esi + 35h]
        push   eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        push   esi
        call   sub_1_8620
        or     al, al
        je     SHORT loc_1_8415
        mov    byte ptr [ebp - 1], 6
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8415:
        mov    byte ptr [ebp - 1], 7
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8426:
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        push   esi
        call   sub_1_8F20
        or     al, al
        je     NEAR PTR loc_1_85E5
        mov    byte ptr [ebp - 1], 8
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_844B:
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        push   esi
        call   sub_1_91B0
        or     al, al
        je     NEAR PTR loc_1_85E5
        mov    byte ptr [ebp - 1], 9
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8470:
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        push   esi
        call   sub_1_9740
        or     al, al
        je     NEAR PTR loc_1_85E5
        mov    byte ptr [ebp - 1], 9
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8495:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_84BB
        xor    eax, eax
        db  066h, 0A1h, 062h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 07Ch, 097h, 0FDh, 0FFh
        add    esp, 8
loc_1_84BB:
        mov    eax, dword ptr [edi + 0feh]
        cmp    dword ptr [eax + 4], 2
        je     SHORT loc_1_8513
        cmp    dword ptr [edi + 0e4h], 0
        je     SHORT loc_1_8508
        mov    eax, dword ptr [edi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_84E4
        cmp    eax, 801h
        jne    SHORT loc_1_84EE
loc_1_84E4:
        mov    dword ptr [edi + 0d0h], 802h
loc_1_84EE:
        push   edi
        call   PPPMAC_Service_09
        add    esp, 4
        mov    byte ptr [ebp - 1], 0ah
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8508:
        push   1
        push   edi
        call   sub_1_1690
        add    esp, 8
loc_1_8513:
        mov    byte ptr [ebp - 1], 0ah
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8524:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_854A
        xor    eax, eax
        db  066h, 0A1h, 064h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0EDh, 096h, 0FDh, 0FFh
        add    esp, 8
loc_1_854A:
        mov    byte ptr [ebp - 1], 0bh
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_8560:
        cmp    dword ptr [edx + 63h], 0
        je     SHORT loc_1_8582
        mov    dword ptr [edx + 63h], 0
        mov    byte ptr [edx + 41h], 0
        mov    byte ptr [ebp - 1], 10h
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8582:
        mov    al, byte ptr [ecx + 4]
        cmp    al, 1
        jb     SHORT loc_1_85D0
        cmp    al, 0bh
        ja     SHORT loc_1_85D0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_85B9
        and    eax, 0ffh
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 040h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 07Eh, 096h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_85B9:
        mov    byte ptr [ebp - 1], 0eh
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_85D0:
        mov    byte ptr [ebp - 1], 0dh
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_85E1:
        mov    byte ptr [ebp - 1], 0fh
loc_1_85E5:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 0ECh, 083h, 000h, 000h, 026h, 084h, 000h, 000h, 04Bh, 084h, 000h, 000h, 070h, 084h
        db  000h, 000h, 095h, 084h, 000h, 000h, 024h, 085h, 000h, 000h, 060h, 085h, 000h, 000h, 0E5h, 085h
        db  000h, 000h, 0E1h, 085h, 000h, 000h, 0E5h, 085h, 000h, 000h, 0E5h, 085h, 000h, 000h
sub_1_8620:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 8dh]
        lea    ecx, [eax + 6ch]
        add    eax, 4bh
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 18h], ecx
        lea    edi, [ebx + 4]
        mov    ax, word ptr [ebp + 10h]
        sub    ax, 4
        mov    word ptr [ebp - 4], ax
        mov    byte ptr [ebp - 1], 2
        mov    dword ptr [ebp - 1ch], 1
        push   esi
        call   sub_1_7E20
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 0ch], edx
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [edx + 1], al
        add    dword ptr [ebp - 0ch], 4
        cmp    word ptr [ebp - 4], 0
        je     NEAR PTR loc_1_8724
loc_1_8679:
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 1]
        inc    edi
        inc    edi
        mov    cl, al
        and    ecx, 0ffff00ffh
        mov    word ptr [ebp - 14h], cx
        mov    ebx, ecx
        and    ebx, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    ecx, ebx
        jb     NEAR PTR loc_1_8ED6
        xor    edx, edx
        mov    dl, byte ptr [ebp - 2]
        lea    ecx, [edx - 1]
        cmp    ecx, 0ch
        ja     SHORT loc_1_86C0
        db  0FFh, 024h, 08Dh, 0ECh, 08Eh, 000h, 000h
        db  08Dh, 064h, 024h, 000h
loc_1_86C0:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_86E7
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 05Ch, 000h, 000h, 000h
loc_1_86D2:
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 050h, 095h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_86E7:
        cmp    byte ptr [ebp - 1], 4
        je     SHORT loc_1_86FA
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [ebp - 1], 4
loc_1_86FA:
        sub    edi, 2
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_083C
        add    dword ptr [ebp - 0ch], ebx
        add    edi, ebx
loc_1_870D:
        mov    ax, word ptr [ebp - 4]
        sub    ax, word ptr [ebp - 14h]
        mov    word ptr [ebp - 4], ax
loc_1_8719:
        cmp    word ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_8679
loc_1_8724:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_8730
        push   esi
        call   sub_1_7E20
loc_1_8730:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp + 18h]
        sub    ecx, dword ptr [ebp + 14h]
        mov    word ptr [eax], cx
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 0ch], edx
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx], cl
        mov    ax, word ptr [eax]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        mov    word ptr [eax + 2], cx
        mov    al, byte ptr [ebp - 1]
        sub    al, 2
        cmp    al, 1
        sbb    al, al
        neg    al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8770:
        cmp    al, 4
        jne    NEAR PTR loc_1_86E7
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_8784
        lea    edi, [ebx + edi - 2]
        jmp    SHORT loc_1_870D
loc_1_8784:
        mov    ax, word ptr [edi]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        mov    byte ptr [esi + 19h], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 4
        inc    dword ptr [ebp - 0ch]
        mov    cl, byte ptr [edi - 1]
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        sub    word ptr [ebp - 4], 4
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [edi - 1]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 042h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 044h, 094h, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_8719
loc_1_87F8:
        cmp    al, 6
        jne    NEAR PTR loc_1_86E7
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_8810
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
        db  090h
loc_1_8810:
        mov    ecx, dword ptr [edi]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esi + 2], edx
        mov    byte ptr [esi + 1ah], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 6
        inc    dword ptr [ebp - 0ch]
        mov    cl, byte ptr [edi - 1]
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        mov    cl, byte ptr [edi - 1]
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        mov    cl, byte ptr [edi - 1]
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        sub    word ptr [ebp - 4], 6
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [edi - 1]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        mov    eax, dword ptr [esi + 2]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 044h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 084h, 093h, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_8719
loc_1_88B8:
        cmp    al, 4
        jb     NEAR PTR loc_1_86E7
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx + 1ch]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_8900
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_86E7
        xor    eax, eax
        db  066h, 0A1h, 046h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 03Fh, 093h, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_86E7
        db  08Dh, 049h, 000h
loc_1_8900:
        mov    cx, word ptr [edi]
        mov    word ptr [ebp - 6], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        mov    dword ptr [ebp - 10h], ecx
        je     NEAR PTR loc_1_8B50
        mov    ecx, dword ptr [ecx + 8]
        cmp    word ptr [ecx + 0ch], 1
        jne    NEAR PTR loc_1_8B50
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_8A45
        nop
loc_1_8930:
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ecx + 8]
        cmp    word ptr [edx + 0ch], 1
        jne    SHORT loc_1_8953
        mov    cx, word ptr [ebp - 6]
        cmp    word ptr [edx + 0eh], cx
        je     SHORT loc_1_8953
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ebp - 10h], ecx
        or     ecx, ecx
        jne    SHORT loc_1_8930
loc_1_8953:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_8A45
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 8]
        cmp    word ptr [ecx + 0ch], 1
        jne    NEAR PTR loc_1_8A45
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_8980
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
        db  08Dh, 049h, 000h
loc_1_8980:
        cmp    al, 4
        jbe    SHORT loc_1_89A2
        mov    edx, dword ptr [ecx + 50h]
        or     edx, edx
        je     NEAR PTR loc_1_8A45
        lea    eax, [ebx - 2]
        push   eax
        push   edi
        push   1
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   edx
        add    esp, 10h
        mov    dword ptr [ebp - 1ch], eax
loc_1_89A2:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_8A45
        mov    cx, word ptr [ebp - 6]
        mov    word ptr [esi + 6], cx
        mov    byte ptr [esi + 1bh], 1
        mov    ecx, dword ptr [ebp - 10h]
        sub    edi, 2
        mov    dword ptr [esi + 8], ecx
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        add    edi, ebx
        push   eax
        call   sub_1_083C
        mov    ax, word ptr [ebp - 4]
        add    dword ptr [ebp - 0ch], ebx
        sub    ax, word ptr [ebp - 14h]
        mov    word ptr [ebp - 4], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [eax + 18h]
        or     ecx, ecx
        jne    SHORT loc_1_8A06
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 016h, 001h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_1_8A06:
        mov    ax, word ptr [esi + 6]
        push   ecx
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 04Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0F7h, 091h, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_8719
loc_1_8A45:
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_8A54
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
loc_1_8A54:
        cmp    byte ptr [ebp - 1], 3
        je     SHORT loc_1_8A67
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [ebp - 1], 3
loc_1_8A67:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 48h]
        or     ecx, ecx
        je     SHORT loc_1_8A8C
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   ecx
        add    esp, 4
        mov    word ptr [ebp - 8], ax
loc_1_8A8C:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        add    al, 4
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 10h]
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 0ch], 2
        cmp    word ptr [ebp - 8], 0
        je     SHORT loc_1_8ADC
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [eax + 8]
        call   dword ptr [eax + 4ch]
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    dword ptr [ebp - 0ch], eax
loc_1_8ADC:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_8B42
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 18h]
        or     ecx, ecx
        jne    SHORT loc_1_8B01
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 016h, 001h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_1_8B01:
        mov    ax, word ptr [eax + 0eh]
        push   ecx
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 04Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0F5h, 090h, 0FDh, 0FFh
        add    esp, 14h
loc_1_8B42:
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
        db  005h, 000h, 000h, 000h, 000h
loc_1_8B50:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_86E7
        mov    cx, word ptr [ebp - 6]
        and    ecx, 0ffh
        shl    ecx, 8
        mov    ax, word ptr [ebp - 6]
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 048h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_86D2
loc_1_8B87:
        cmp    al, 6
        jne    NEAR PTR loc_1_86E7
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_8BA0
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
        db  08Bh, 0FFh
loc_1_8BA0:
        mov    ecx, dword ptr [edi]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esi + 0eh], edx
        mov    byte ptr [esi + 1dh], 1
        mov    eax, edx
        or     eax, eax
        je     SHORT loc_1_8BE0
        mov    ecx, dword ptr [ebp - 18h]
        cmp    byte ptr [ecx + 1dh], 0
        je     SHORT loc_1_8C23
        cmp    dword ptr [ecx + 0eh], eax
        jne    SHORT loc_1_8C23
loc_1_8BE0:
        cmp    byte ptr [ebp - 1], 3
        je     SHORT loc_1_8BF3
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [ebp - 1], 3
loc_1_8BF3:
        call   sub_1_7D40
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 0eh], eax
        cmp    byte ptr [ecx + 1dh], 0
        je     SHORT loc_1_8C09
        cmp    dword ptr [ecx + 0eh], eax
        je     SHORT loc_1_8BF3
loc_1_8C09:
        cmp    dword ptr [esi + 0eh], 0
        jne    SHORT loc_1_8C16
        mov    dword ptr [esi + 0eh], 0feedc0deh
loc_1_8C16:
        mov    byte ptr [esi + 1dh], 0
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [esi + 0eh]
        mov    dword ptr [ecx + 12h], eax
loc_1_8C23:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 6
        inc    dword ptr [ebp - 0ch]
        mov    edx, dword ptr [esi + 0eh]
        mov    ecx, edx
        and    ecx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    eax, dword ptr [ebp - 0ch]
        add    edi, 4
        mov    dword ptr [eax], ecx
        add    dword ptr [ebp - 0ch], 4
        sub    word ptr [ebp - 4], 6
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_8CA7
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        mov    eax, dword ptr [esi + 0eh]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 04Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 095h, 08Fh, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_8719
loc_1_8CA7:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        xor    eax, eax
        db  066h, 0A1h, 050h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 066h, 08Fh, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_8719
loc_1_8CD6:
        cmp    al, 2
        jne    NEAR PTR loc_1_86E7
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_8CF0
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
        db  08Dh, 049h, 000h
loc_1_8CF0:
        mov    byte ptr [esi + 16h], 1
        mov    byte ptr [esi + 1eh], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        sub    word ptr [ebp - 4], 2
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 0ch]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        xor    eax, eax
        db  066h, 0A1h, 052h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0FCh, 08Eh, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_8719
loc_1_8D40:
        cmp    al, 2
        jne    NEAR PTR loc_1_86E7
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_8D57
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
loc_1_8D57:
        mov    byte ptr [esi + 17h], 1
        mov    byte ptr [esi + 1fh], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        sub    word ptr [ebp - 4], 2
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 0ch]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        xor    eax, eax
        db  066h, 0A1h, 054h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 095h, 08Eh, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_8719
loc_1_8DA7:
        cmp    al, 3
        jne    NEAR PTR loc_1_86E7
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        mov    eax, dword ptr [eax + 1ch]
        test   byte ptr [eax + 0ch], 8
        jne    SHORT loc_1_8DF0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_86E7
        xor    eax, eax
        db  066h, 0A1h, 056h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 04Fh, 08Eh, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_86E7
        db  08Dh, 049h, 000h
loc_1_8DF0:
        cmp    byte ptr [edi], 6
        jne    SHORT loc_1_8E60
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_8E04
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
loc_1_8E04:
        mov    byte ptr [esi + 18h], 1
        mov    byte ptr [esi + 20h], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        sub    word ptr [ebp - 4], 3
        mov    byte ptr [eax], 3
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [edi - 1]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8719
        xor    eax, eax
        db  066h, 0A1h, 058h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0DCh, 08Dh, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_8719
loc_1_8E60:
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_8E70
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
        db  090h
loc_1_8E70:
        cmp    byte ptr [ebp - 1], 3
        je     SHORT loc_1_8E83
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [ebp - 1], 3
loc_1_8E83:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 3
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 6
        inc    dword ptr [ebp - 0ch]
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_8ECD
        push   6
        xor    eax, eax
        mov    al, byte ptr [edi]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 05Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 06Ah, 08Dh, 0FDh, 0FFh
        add    esp, 10h
loc_1_8ECD:
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_870D
loc_1_8ED6:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h, 070h, 087h, 000h, 000h, 0F8h, 087h, 000h, 000h, 0B8h, 088h, 000h, 000h, 0C0h
        db  086h, 000h, 000h, 087h, 08Bh, 000h, 000h, 0C0h, 086h, 000h, 000h, 0D6h, 08Ch, 000h, 000h, 040h
        db  08Dh, 000h, 000h, 0C0h, 086h, 000h, 000h, 0C0h, 086h, 000h, 000h, 0C0h, 086h, 000h, 000h, 0C0h
        db  086h, 000h, 000h, 0A7h, 08Dh, 000h, 000h
sub_1_8F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    dword ptr [ebp - 4], eax
        mov    ebx, eax
        add    ebx, 21h
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 4
        mov    di, word ptr [ebp + 10h]
        sub    di, 4
        cmp    byte ptr [ebx + 19h], 0
        je     SHORT loc_1_8F9B
        cmp    di, 4
        jb     NEAR PTR loc_1_9043
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 1
        jne    NEAR PTR loc_1_9043
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 4
        jne    NEAR PTR loc_1_9043
        mov    ax, word ptr [ebx]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        cmp    ecx, eax
        jne    NEAR PTR loc_1_9043
        add    esi, 2
        sub    di, 4
loc_1_8F9B:
        cmp    byte ptr [ebx + 1ah], 0
        je     SHORT loc_1_8FF5
        cmp    di, 6
        jb     NEAR PTR loc_1_9050
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 2
        jne    NEAR PTR loc_1_9050
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 6
        jne    NEAR PTR loc_1_9050
        mov    eax, dword ptr [ebx + 2]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        cmp    ecx, dword ptr [esi]
        jne    SHORT loc_1_9050
        add    esi, 4
        sub    di, 6
loc_1_8FF5:
        cmp    byte ptr [ebx + 1bh], 0
        je     NEAR PTR loc_1_908D
        cmp    di, 4
        jb     NEAR PTR loc_1_9166
        cmp    byte ptr [esi], 3
        jne    NEAR PTR loc_1_9166
        mov    edx, dword ptr [ebx + 8]
        mov    ecx, dword ptr [edx + 8]
        mov    eax, dword ptr [ecx + 50h]
        or     eax, eax
        je     SHORT loc_1_9060
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 1]
        sub    ecx, 2
        push   ecx
        lea    ecx, [esi + 2]
        push   ecx
        push   2
        push   edx
        call   eax
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_907B
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_9043:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_9050:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_9060:
        lea    eax, [esi + 1]
        cmp    byte ptr [esi + 1], 4
        jne    NEAR PTR loc_1_915B
        mov    ax, word ptr [ecx + 0eh]
        cmp    word ptr [esi + 2], ax
        jne    NEAR PTR loc_1_915B
loc_1_907B:
        mov    al, byte ptr [esi + 1]
        and    eax, 0ffff00ffh
        sub    di, ax
        and    eax, 0ffffh
        add    esi, eax
loc_1_908D:
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_1_90EB
        cmp    di, 6
        jb     NEAR PTR loc_1_9171
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 5
        jne    NEAR PTR loc_1_9171
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 6
        jne    NEAR PTR loc_1_9171
        mov    eax, dword ptr [ebx + 0eh]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        cmp    ecx, dword ptr [esi]
        jne    NEAR PTR loc_1_9171
        add    esi, 4
        sub    di, 6
loc_1_90EB:
        cmp    byte ptr [ebx + 1eh], 0
        je     SHORT loc_1_910F
        cmp    di, 2
        jb     NEAR PTR loc_1_917C
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 7
        jne    SHORT loc_1_917C
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_917C
        sub    di, 2
loc_1_910F:
        cmp    byte ptr [ebx + 1fh], 0
        je     SHORT loc_1_912F
        cmp    di, 2
        jb     SHORT loc_1_9187
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 8
        jne    SHORT loc_1_9187
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_9187
        sub    di, 2
loc_1_912F:
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_9192
        cmp    di, 3
        jb     SHORT loc_1_9150
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 0dh
        jne    SHORT loc_1_9150
        mov    eax, esi
        inc    esi
        cmp    byte ptr [eax], 3
        jne    SHORT loc_1_9150
        cmp    byte ptr [esi], 6
        je     SHORT loc_1_9192
loc_1_9150:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_915B:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_9166:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_9171:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_917C:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_9187:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_9192:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 6bh], 1
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_91B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    dword ptr [ebp - 14h], eax
        mov    esi, eax
        add    esi, 21h
        mov    dword ptr [ebp - 10h], 0
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 4
        mov    bx, word ptr [ebp + 10h]
        sub    bx, 4
        je     NEAR PTR loc_1_96B1
loc_1_91E2:
        xor    eax, eax
        mov    al, byte ptr [edi]
        dec    eax
        cmp    eax, 0ch
        ja     NEAR PTR loc_1_96D0
        db  0FFh, 024h, 085h, 008h, 097h, 000h, 000h
loc_1_91F7:
        cmp    byte ptr [edi + 1], 4
        jne    NEAR PTR loc_1_96D0
        mov    cx, word ptr [edi + 2]
        mov    ah, cl
        mov    al, ch
        cmp    byte ptr [esi + 19h], 0
        je     SHORT loc_1_9220
        mov    cx, word ptr [esi]
        cmp    cx, ax
        je     SHORT loc_1_9224
        jbe    SHORT loc_1_9224
        mov    word ptr [esi], ax
        jmp    SHORT loc_1_9224
        db  08Bh, 0FFh
loc_1_9220:
        mov    byte ptr [esi + 19h], 1
loc_1_9224:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9250
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 076h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E7h, 089h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_9250:
        add    edi, 4
        sub    bx, 4
        jmp    NEAR PTR loc_1_96A8
        db  08Dh, 064h, 024h, 000h
loc_1_9260:
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_96D0
        mov    eax, dword ptr [edi + 2]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        cmp    byte ptr [esi + 1ah], 0
        je     SHORT loc_1_92D0
        mov    eax, dword ptr [esi + 2]
        cmp    ecx, eax
        je     SHORT loc_1_92D4
        mov    edx, ecx
        or     edx, eax
        mov    dword ptr [esi + 2], edx
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_92D4
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 078h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 06Bh, 089h, 0FDh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_1_92D4
        db  08Bh, 0FFh
loc_1_92D0:
        mov    byte ptr [esi + 1ah], 1
loc_1_92D4:
        add    edi, 6
        sub    bx, 6
        jmp    NEAR PTR loc_1_96A8
loc_1_92E0:
        lea    ecx, [edi + 1]
        mov    dword ptr [ebp - 8], ecx
        cmp    byte ptr [ecx], 4
        jb     NEAR PTR loc_1_96D0
        lea    ecx, [edi + 2]
        mov    dword ptr [ebp - 0ch], ecx
        cmp    byte ptr [esi + 1bh], 0
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 2], ax
        je     NEAR PTR loc_1_9468
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9365
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        jne    SHORT loc_1_932A
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_932A:
        push   eax
        mov    cx, word ptr [ebp - 2]
        and    ecx, 0ffh
        shl    ecx, 8
        mov    ax, word ptr [ebp - 2]
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 07Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D2h, 088h, 0FDh, 0FFh
        add    esp, 10h
loc_1_9365:
        mov    edx, dword ptr [esi + 8]
        mov    ecx, dword ptr [edx + 8]
        mov    eax, dword ptr [ecx + 50h]
        or     eax, eax
        je     SHORT loc_1_9392
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        sub    ecx, 2
        push   ecx
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        push   3
        push   edx
        call   eax
        add    esp, 10h
        mov    dword ptr [ebp - 10h], eax
        jmp    SHORT loc_1_93A0
loc_1_9392:
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ecx + 0eh], ax
        je     NEAR PTR loc_1_9468
loc_1_93A0:
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_9468
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     NEAR PTR loc_1_9431
        mov    ecx, dword ptr [eax + 8]
        cmp    word ptr [ecx + 0ch], 1
        jne    SHORT loc_1_9431
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [eax + 8]
        mov    cx, word ptr [eax + 0eh]
        mov    word ptr [esi + 6], cx
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_9468
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        jne    SHORT loc_1_93F7
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_93F7:
        push   eax
        mov    dx, cx
        and    edx, 0ffh
        shr    cx, 8
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ffffh
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 08Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 008h, 088h, 0FDh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_1_9468
loc_1_9431:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9457
        xor    eax, eax
        db  066h, 0A1h, 05Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E0h, 087h, 0FDh, 0FFh
        add    esp, 8
loc_1_9457:
        mov    byte ptr [esi + 1bh], 0
        mov    word ptr [esi + 6], 0
        mov    dword ptr [esi + 8], 0
loc_1_9468:
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx]
        and    eax, 0ffff00ffh
        sub    bx, ax
        and    eax, 0ffffh
        add    edi, eax
        jmp    NEAR PTR loc_1_96A8
loc_1_9481:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_94A7
        xor    eax, eax
        db  066h, 0A1h, 07Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 090h, 087h, 0FDh, 0FFh
        add    esp, 8
loc_1_94A7:
        mov    al, byte ptr [edi + 1]
        and    eax, 0ffff00ffh
        sub    bx, ax
        and    eax, 0ffffh
        add    edi, eax
        jmp    NEAR PTR loc_1_96A8
        db  08Bh, 0FFh
loc_1_94C0:
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_96D0
        mov    eax, dword ptr [edi + 2]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        cmp    dword ptr [esi + 12h], ecx
        jne    SHORT loc_1_9532
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9520
        xor    eax, eax
        db  066h, 0A1h, 080h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 01Bh, 087h, 0FDh, 0FFh
        add    esp, 8
        lea    esp, [esp]
loc_1_9520:
        call   sub_1_7D40
        mov    dword ptr [esi + 0eh], eax
        mov    eax, dword ptr [esi + 12h]
        cmp    dword ptr [esi + 0eh], eax
        je     SHORT loc_1_9520
        jmp    SHORT loc_1_9538
loc_1_9532:
        lea    eax, [esi + 0eh]
        mov    dword ptr [esi + 0eh], ecx
loc_1_9538:
        mov    byte ptr [esi + 1dh], 1
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9566
        mov    eax, dword ptr [esi + 0eh]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 07Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D1h, 086h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_9566:
        add    edi, 6
        sub    bx, 6
        jmp    NEAR PTR loc_1_96A8
loc_1_9572:
        cmp    byte ptr [edi + 1], 2
        jne    NEAR PTR loc_1_96D0
        cmp    byte ptr [esi + 1eh], 0
        je     SHORT loc_1_95B2
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_95A8
        xor    eax, eax
        db  066h, 0A1h, 082h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 08Fh, 086h, 0FDh, 0FFh
        add    esp, 8
loc_1_95A8:
        mov    byte ptr [esi + 1eh], 0
        mov    byte ptr [esi + 16h], 0
        jmp    SHORT loc_1_95E0
loc_1_95B2:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_95D8
        xor    eax, eax
        db  066h, 0A1h, 084h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 05Fh, 086h, 0FDh, 0FFh
        add    esp, 8
loc_1_95D8:
        mov    byte ptr [esi + 1eh], 1
        mov    byte ptr [esi + 16h], 1
loc_1_95E0:
        add    edi, 2
        sub    bx, 2
        jmp    NEAR PTR loc_1_96A8
        db  08Dh, 064h, 024h, 000h
loc_1_95F0:
        cmp    byte ptr [edi + 1], 2
        jne    NEAR PTR loc_1_96D0
        cmp    byte ptr [esi + 1fh], 0
        je     SHORT loc_1_9630
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9626
        xor    eax, eax
        db  066h, 0A1h, 088h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 011h, 086h, 0FDh, 0FFh
        add    esp, 8
loc_1_9626:
        mov    byte ptr [esi + 1fh], 0
        mov    byte ptr [esi + 17h], 0
        jmp    SHORT loc_1_965E
loc_1_9630:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9656
        xor    eax, eax
        db  066h, 0A1h, 086h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E1h, 085h, 0FDh, 0FFh
        add    esp, 8
loc_1_9656:
        mov    byte ptr [esi + 1fh], 1
        mov    byte ptr [esi + 17h], 1
loc_1_965E:
        add    edi, 2
        sub    bx, 2
        jmp    SHORT loc_1_96A8
loc_1_9667:
        cmp    byte ptr [edi + 1], 3
        jne    SHORT loc_1_96D0
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_96A1
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9699
        xor    eax, eax
        db  066h, 0A1h, 08Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 09Eh, 085h, 0FDh, 0FFh
        add    esp, 8
loc_1_9699:
        mov    byte ptr [esi + 20h], 0
        mov    byte ptr [esi + 18h], 0
loc_1_96A1:
        add    edi, 3
        sub    bx, 3
loc_1_96A8:
        or     bx, bx
        jne    NEAR PTR loc_1_91E2
loc_1_96B1:
        or     bx, bx
        jne    SHORT loc_1_96D0
        mov    eax, dword ptr [ebp - 14h]
        pop    edi
        mov    dword ptr [eax + 6bh], 1
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_96D0:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_96FB
        xor    eax, eax
        mov    al, byte ptr [edi]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 074h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 03Ch, 085h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_96FB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 0F7h, 091h, 000h, 000h, 060h, 092h, 000h, 000h, 0E0h, 092h, 000h, 000h, 081h, 094h
        db  000h, 000h, 0C0h, 094h, 000h, 000h, 0D0h, 096h, 000h, 000h, 072h, 095h, 000h, 000h, 0F0h, 095h
        db  000h, 000h, 0D0h, 096h, 000h, 000h, 0D0h, 096h, 000h, 000h, 0D0h, 096h, 000h, 000h, 0D0h, 096h
        db  000h, 000h, 067h, 096h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_1_9740:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    dword ptr [ebp - 4], eax
        mov    ebx, eax
        add    ebx, 21h
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 4
        mov    di, word ptr [ebp + 10h]
        sub    di, 4
        cmp    byte ptr [ebx + 19h], 0
        je     SHORT loc_1_979B
        cmp    byte ptr [esi], 1
        jne    SHORT loc_1_979B
        cmp    byte ptr [esi + 1], 4
        jne    NEAR PTR loc_1_9A33
        mov    cx, word ptr [esi + 2]
        mov    ah, cl
        mov    al, ch
        cmp    word ptr [ebx], ax
        jne    NEAR PTR loc_1_9A33
        mov    byte ptr [ebx + 19h], 0
        mov    word ptr [ebx], 5dch
        add    esi, 4
        sub    di, 4
loc_1_979B:
        cmp    byte ptr [ebx + 1ah], 0
        je     SHORT loc_1_9818
        cmp    byte ptr [esi], 2
        jne    SHORT loc_1_9818
        cmp    byte ptr [esi + 1], 6
        jne    NEAR PTR loc_1_9A33
        mov    eax, dword ptr [esi + 2]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        cmp    dword ptr [ebx + 2], ecx
        jne    NEAR PTR loc_1_9A33
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9806
        xor    eax, eax
        db  066h, 0A1h, 072h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 031h, 084h, 0FDh, 0FFh
        add    esp, 8
loc_1_9806:
        mov    byte ptr [ebx + 1ah], 0
        mov    dword ptr [ebx + 2], 0ffffffffh
        add    esi, 6
        sub    di, 6
loc_1_9818:
        cmp    byte ptr [ebx + 1bh], 0
        je     NEAR PTR loc_1_98C2
        cmp    byte ptr [esi], 3
        jne    NEAR PTR loc_1_98C2
        cmp    byte ptr [esi + 1], 4
        jb     NEAR PTR loc_1_9A33
        mov    eax, dword ptr [ebx + 8]
        mov    cx, word ptr [esi + 2]
        mov    eax, dword ptr [eax + 8]
        cmp    word ptr [eax + 0eh], cx
        jne    NEAR PTR loc_1_9A33
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_989F
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        jne    SHORT loc_1_9867
        xor    eax, eax
        db  066h, 0A1h, 016h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_1_9867:
        push   eax
        mov    dx, cx
        and    edx, 0ffh
        shr    cx, 8
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ffffh
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 068h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 098h, 083h, 0FDh, 0FFh
        add    esp, 10h
loc_1_989F:
        mov    byte ptr [ebx + 1bh], 0
        mov    word ptr [ebx + 6], 0
        mov    dword ptr [ebx + 8], 0
        mov    al, byte ptr [esi + 1]
        and    eax, 0ffff00ffh
        sub    di, ax
        and    eax, 0ffffh
        add    esi, eax
loc_1_98C2:
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_1_993F
        cmp    byte ptr [esi], 5
        jne    SHORT loc_1_993F
        cmp    byte ptr [esi + 1], 6
        jne    NEAR PTR loc_1_9A33
        mov    eax, dword ptr [esi + 2]
        mov    edx, eax
        and    edx, 0ff00h
        mov    ecx, eax
        shl    ecx, 10h
        or     edx, ecx
        shl    edx, 8
        mov    ecx, eax
        and    ecx, 0ff0000h
        shr    eax, 10h
        or     ecx, eax
        shr    ecx, 8
        or     edx, ecx
        cmp    dword ptr [ebx + 0eh], edx
        jne    NEAR PTR loc_1_9A33
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_992D
        xor    eax, eax
        db  066h, 0A1h, 070h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 00Ah, 083h, 0FDh, 0FFh
        add    esp, 8
loc_1_992D:
        mov    byte ptr [ebx + 1dh], 0
        mov    dword ptr [ebx + 0eh], 0
        add    esi, 6
        sub    di, 6
loc_1_993F:
        cmp    byte ptr [ebx + 1eh], 0
        je     SHORT loc_1_9989
        cmp    byte ptr [esi], 7
        jne    SHORT loc_1_9989
        cmp    byte ptr [esi + 1], 2
        jne    NEAR PTR loc_1_9A33
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_997A
        xor    eax, eax
        db  066h, 0A1h, 06Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0BDh, 082h, 0FDh, 0FFh
        add    esp, 8
loc_1_997A:
        mov    byte ptr [ebx + 1eh], 0
        mov    byte ptr [ebx + 16h], 0
        add    esi, 2
        sub    di, 2
loc_1_9989:
        cmp    byte ptr [ebx + 1fh], 0
        je     SHORT loc_1_99D3
        cmp    byte ptr [esi], 8
        jne    SHORT loc_1_99D3
        cmp    byte ptr [esi + 1], 2
        jne    NEAR PTR loc_1_9A33
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_99C4
        xor    eax, eax
        db  066h, 0A1h, 06Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 073h, 082h, 0FDh, 0FFh
        add    esp, 8
loc_1_99C4:
        mov    byte ptr [ebx + 1fh], 0
        mov    byte ptr [ebx + 17h], 0
        add    esi, 2
        sub    di, 2
loc_1_99D3:
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_9A19
        cmp    byte ptr [esi], 0dh
        jne    SHORT loc_1_9A19
        cmp    byte ptr [esi + 1], 3
        jne    SHORT loc_1_9A33
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9A0A
        xor    eax, eax
        db  066h, 0A1h, 06Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 02Dh, 082h, 0FDh, 0FFh
        add    esp, 8
loc_1_9A0A:
        mov    byte ptr [ebx + 20h], 0
        mov    byte ptr [ebx + 18h], 0
        add    esi, 3
        sub    di, 3
loc_1_9A19:
        or     di, di
        jne    SHORT loc_1_9A33
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 6bh], 1
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_9A33:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_9A5E
        xor    eax, eax
        mov    al, byte ptr [esi]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 066h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D9h, 081h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_9A5E:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
