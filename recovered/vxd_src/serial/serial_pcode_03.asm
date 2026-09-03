; serial_pcode_03.asm -- SERIAL object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        push   esi
        push   edi
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_3_0016
        test   byte ptr [esi + 0fch], 12h
        je     SHORT loc_3_0079
        jmp    SHORT loc_3_001F
loc_3_0016:
        test   byte ptr [esi + 0fch], 12h
        je     SHORT loc_3_0079
loc_3_001F:
        mov    eax, dword ptr [esi + 4ch]
        cmp    eax, dword ptr [esi + 0a0h]
        ja     SHORT loc_3_0079
        mov    edx, dword ptr [esi + 0c4h]
        mov    ah, byte ptr [esi + 0fdh]
        cli
        mov    cl, byte ptr [esi + 0fch]
        or     ah, ah
        je     SHORT loc_3_004F
        add    dl, 4
        in     al, dx
        or     al, ah
        jmp    SHORT loc_3_0049
loc_3_0049:
        jmp    SHORT loc_3_004B
loc_3_004B:
        out    dx, al
        and    cl, 0fdh
loc_3_004F:
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_3_0062
        test   cl, 10h
        je     SHORT loc_3_0072
        and    cl, 0efh
        jmp    SHORT loc_3_006A
loc_3_0062:
        test   cl, 10h
        je     SHORT loc_3_0072
        and    cl, 0efh
loc_3_006A:
        or     cl, 4
        db  0E8h, 028h, 009h, 0FDh, 0FFh
loc_3_0072:
        mov    byte ptr [esi + 0fch], cl
        sti
loc_3_0079:
        xor    eax, eax
        or     eax, dword ptr [esi + 34h]
        jne    SHORT loc_3_00C7
        or     eax, dword ptr [esi + 4ch]
        je     SHORT loc_3_00AD
        mov    edi, dword ptr [esi + 3ch]
        mov    ebx, dword ptr [esi + 50h]
        mov    al, byte ptr [edi + ebx]
        inc    ebx
        cmp    ebx, dword ptr [esi + 40h]
        jb     SHORT loc_3_0096
        xor    ebx, ebx
loc_3_0096:
        mov    dword ptr [esi + 50h], ebx
        dec    dword ptr [esi + 4ch]
        mov    ecx, dword ptr [esi + 4ch]
        jae    SHORT loc_3_00A8
        and    byte ptr [esi + 0cdh], 0feh
loc_3_00A8:
        or     esp, esp
        pop    edi
        pop    esi
        ret
loc_3_00AD:
        test   byte ptr [esi + 9ch], 1
        jne    SHORT loc_3_00C5
        mov    al, byte ptr [esi + 0b0h]
        test   byte ptr [esi + 0f8h], 20h
        jne    SHORT loc_3_00A8
loc_3_00C5:
        xor    eax, eax
loc_3_00C7:
        xor    ecx, ecx
        pop    edi
        pop    esi
        ret
sub_3_00CC:
        or     byte ptr [esi + 114h], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 34h], ecx
        mov    dword ptr [esi + 4ch], ecx
        call   0
        test   byte ptr [esi + 0fch], 80h
        jne    SHORT loc_3_010A
loc_3_00E9:
        mov    ecx, dword ptr [esi + 58h]
        jecxz  loc_3_010F
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    edi, eax
loc_3_00F6:
        cmp    dword ptr [esi + 58h], ecx
        jne    SHORT loc_3_00E9
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, edi
        cmp    eax, 0fah
        jb     SHORT loc_3_00F6
loc_3_010A:
        mov    ecx, 0fffffffeh
loc_3_010F:
        mov    edx, dword ptr [esi + 0c4h]
        call   sub_3_01B8
        mov    eax, ecx
        ret
        db  000h, 000h, 000h
sub_3_0120:
        push   edx
        mov    ecx, dword ptr [esi + 124h]
        test   ecx, ecx
        je     SHORT loc_3_014E
        mov    eax, dword ptr [esi + 128h]
        test   eax, eax
        je     SHORT loc_3_014B
        push   1
        push   esi
        db  068h, 044h, 009h, 000h, 000h
        push   eax
        push   1
        call   ecx
        add    esp, 14h
        mov    dword ptr [esi + 107h], eax
loc_3_014B:
        cmp    eax, 1
loc_3_014E:
        pop    edx
        ret
sub_3_0150:
        mov    ecx, dword ptr [esi + 107h]
        jecxz  loc_3_0169
        db  068h, 044h, 009h, 000h, 000h
        push   ecx
        push   3
        call   dword ptr [esi + 124h]
        add    esp, 0ch
loc_3_0169:
        ret
sub_3_016A:
        push   ecx
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_3_018F
        db  068h, 044h, 009h, 000h, 000h
        push   dword ptr [esi + 107h]
        push   2
        call   dword ptr [esi + 124h]
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_3_0191
        and    byte ptr [esi + 3bh], 0feh
loc_3_018F:
        test   esi, esi
loc_3_0191:
        pop    ecx
        ret
sub_3_0193:
        push   ecx
        push   eax
        sti
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ecx, eax
        add    ecx, 0c8h
loc_3_01A4:
        in     al, dx
        and    al, 60h
        cmp    al, 60h
        je     SHORT loc_3_01B5
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cmp    eax, ecx
        jb     SHORT loc_3_01A4
loc_3_01B5:
        pop    eax
        pop    ecx
        ret
sub_3_01B8:
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_3_01C5
        call   sub_3_016A
        je     SHORT loc_3_0221
loc_3_01C5:
        inc    edx
        mov    al, 1
        out    dx, al
        add    dl, 4
        jmp    SHORT loc_3_01CE
loc_3_01CE:
        call   sub_3_0193
        xor    al, al
        test   byte ptr [esi + 0f8h], 8
        je     SHORT loc_3_01E0
        mov    al, 87h
loc_3_01E0:
        sub    dl, 3
        out    dx, al
        jmp    SHORT loc_3_01E6
loc_3_01E6:
        db  0E8h, 06Ch, 007h, 0FDh, 0FFh
        add    dl, 2
        in     al, dx
        jmp    SHORT loc_3_01F1
loc_3_01F1:
        mov    ah, al
        or     al, 10h
        out    dx, al
        jmp    SHORT loc_3_01F8
loc_3_01F8:
        sub    dl, 4
        xor    al, al
        out    dx, al
        jmp    SHORT loc_3_0200
loc_3_0200:
        add    dl, 5
        call   sub_3_0193
        mov    al, ah
        dec    dl
        and    al, 3
        out    dx, al
        db  0E8h, 073h, 007h, 0FDh, 0FFh
        sub    dl, 3
        xor    al, al
        out    dx, al
        dec    dl
        db  0E8h, 036h, 007h, 0FDh, 0FFh
loc_3_0221:
        push   ecx
        push   ebx
        mov    edi, dword ptr [esi + 116h]
        dec    byte ptr [edi + 1]
        jne    SHORT loc_3_025B
        movzx  eax, byte ptr [esi + 115h]
        db  0CDh, 020h, 016h, 000h, 003h, 000h
        cmp    eax, 1
        ja     SHORT loc_3_0246
        cmp    byte ptr [edi + 2], 0
        jne    SHORT loc_3_024B
loc_3_0246:
        db  0E8h, 03Ch, 007h, 0FDh, 0FFh
loc_3_024B:
        xor    eax, eax
        xchg   dword ptr [esi + 10ch], eax
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        jmp    SHORT loc_3_0290
loc_3_025B:
        cli
        mov    eax, dword ptr [edi + 8]
        cmp    eax, esi
        jne    SHORT loc_3_026E
        mov    eax, dword ptr [eax + 130h]
        mov    dword ptr [edi + 8], eax
        jmp    SHORT loc_3_028A
loc_3_026E:
        cmp    dword ptr [eax + 130h], esi
        je     SHORT loc_3_027E
        mov    eax, dword ptr [eax + 130h]
        jmp    SHORT loc_3_026E
loc_3_027E:
        mov    ecx, dword ptr [esi + 130h]
        mov    dword ptr [eax + 130h], ecx
loc_3_028A:
        sti
        db  0E8h, 0F7h, 006h, 0FDh, 0FFh
loc_3_0290:
        pop    ebx
        call   sub_3_0150
        pop    ecx
        xor    eax, eax
        ret
sub_3_029A:
        push   edi
        mov    edx, dword ptr [eax + 11eh]
        xor    ecx, ecx
loc_3_02A3:
        mov    cl, byte ptr [edx]
        mov    ch, byte ptr [edi]
        or     ecx, ecx
        je     SHORT loc_3_02BF
        call   sub_3_02C1
        xchg   ch, cl
        call   sub_3_02C1
        inc    edx
        inc    edi
        cmp    cl, ch
        je     SHORT loc_3_02A3
        or     esi, esi
loc_3_02BF:
        pop    edi
        ret
sub_3_02C1:
        cmp    cl, 41h
        jb     SHORT loc_3_02CE
        cmp    cl, 5ah
        ja     SHORT loc_3_02CE
        or     cl, 20h
loc_3_02CE:
        ret
        db  000h
loc_3_02D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        db  08Bh, 035h, 058h, 011h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_3_02E5:
        je     SHORT loc_3_0312
        call   sub_3_029A
        je     SHORT loc_3_02F6
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jmp    SHORT loc_3_02E5
loc_3_02F6:
        mov    esi, eax
        cmp    dword ptr [esi + 64h], 534d5446h
        je     SHORT loc_3_031C
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_3_0120
        jae    SHORT loc_3_0330
        jb     SHORT loc_3_031C
loc_3_030D:
        call   sub_3_0150
loc_3_0312:
        mov    eax, 0fffffff6h
        jmp    NEAR PTR loc_3_0482
loc_3_031C:
        mov    eax, 0fffffffeh
        cmp    dword ptr [esi + 0c4h], -1
        jne    NEAR PTR loc_3_0482
        jmp    SHORT loc_3_0312
loc_3_0330:
        mov    edx, dword ptr [esi + 0c4h]
        add    dl, 2
        in     al, dx
        test   al, 30h
        jne    SHORT loc_3_030D
        mov    edx, dword ptr [esi + 116h]
        cmp    byte ptr [edx + 1], 0
        je     SHORT loc_3_0375
        cli
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 10ch], eax
        inc    byte ptr [edx + 1]
        xor    ecx, ecx
        mov    eax, dword ptr [edx + 8]
loc_3_035C:
        cmp    dword ptr [eax + 130h], ecx
        je     SHORT loc_3_036C
        mov    eax, dword ptr [eax + 130h]
        jmp    SHORT loc_3_035C
loc_3_036C:
        mov    dword ptr [eax + 130h], esi
        sti
        jmp    SHORT loc_3_03C1
loc_3_0375:
        movzx  eax, byte ptr [esi + 115h]
        db  0BFh, 0C8h, 011h, 000h, 000h
        mov    word ptr [edi], ax
        db  0C7h, 047h, 004h, 00Ch, 004h, 000h, 000h
        mov    word ptr [edi + 2], 6
        mov    dword ptr [edi + 1ch], edx
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     NEAR PTR loc_3_030D
        mov    dword ptr [edx + 8], esi
        mov    dword ptr [esi + 10ch], eax
        mov    dword ptr [edx + 4], eax
        inc    byte ptr [edx + 1]
        db  08Bh, 01Dh, 05Ch, 011h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        and    cl, 4
        mov    byte ptr [edx + 2], cl
loc_3_03C1:
        lea    edi, [esi + 4ch]
        xor    eax, eax
        mov    ecx, 6
        cld
        rep stosd
        mov    byte ptr [esi + 0fch], al
        mov    byte ptr [esi + 114h], al
        mov    byte ptr [esi + 3bh], al
        mov    dword ptr [esi + 0e4h], eax
        mov    dword ptr [esi + 0cch], eax
        dec    eax
        mov    dword ptr [esi + 0e0h], eax
        mov    dword ptr [esi + 0a0h], eax
        mov    dword ptr [esi + 0a4h], eax
        mov    dword ptr [esi + 9ch], 1
        mov    edx, dword ptr [esi + 0c4h]
        add    dl, 3
        in     al, dx
        mov    ah, al
        or     al, 80h
        out    dx, al
        sub    dl, 3
        in     al, dx
        movzx  ecx, al
        inc    dl
        in     al, dx
        mov    ch, al
        mov    al, ah
        add    dl, 2
        out    dx, al
        push   edx
        xor    edx, edx
        xor    eax, eax
        jecxz  loc_3_0435
        mov    eax, 1c200h
        div    ecx
loc_3_0435:
        pop    edx
        mov    dword ptr [esi + 98h], eax
        mov    byte ptr [esi + 0aah], 8
        dec    dl
        in     al, dx
        test   al, 80h
        je     SHORT loc_3_0451
        or     byte ptr [esi + 0f8h], 8
loc_3_0451:
        lea    edi, [esi + 88h]
        mov    dword ptr [esi + 12ch], edi
        lea    edi, [esi + 30h]
        mov    dword ptr [esi + 0f0h], edi
        lea    edi, [esi + 38h]
        mov    dword ptr [esi + 0f4h], edi
        and    byte ptr [esi + 0f8h], 1fh
        or     byte ptr [esi + 0f8h], 80h
        xor    eax, eax
        mov    dword ptr [esi + 34h], eax
loc_3_0482:
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], eax
        or     eax, eax
        jne    SHORT loc_3_04B1
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 11ah], ebx
        mov    dword ptr [esi + 0ch], eax
        mov    edx, dword ptr [esi + 0c4h]
        add    dl, 4
        in     al, dx
        or     al, 8
        out    dx, al
        db  0E8h, 0DEh, 004h, 0FDh, 0FFh
        and    dword ptr [esi + 34h], 0
        mov    eax, esi
        jmp    SHORT loc_3_04B3
loc_3_04B1:
        xor    eax, eax
loc_3_04B3:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_04B8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        xor    eax, eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ch], eax
        lea    esi, [esi + 94h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ch
        rep movsd
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_04DC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        call   sub_3_00CC
        mov    dword ptr [esi + 0ch], eax
        test   byte ptr [esi + 114h], 8
        je     SHORT loc_3_0503
        push   0
        push   dword ptr [esi + 3ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_0503:
        test   byte ptr [esi + 114h], 4
        je     SHORT loc_3_0519
        push   0
        push   dword ptr [esi + 44h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_0519:
        mov    ebx, esi
        or     byte ptr [esi + 114h], 20h
        db  0E8h, 00Fh, 00Bh, 0FDh, 0FFh
        call   sub_3_08F3
        or     al, 1
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_0533:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 0f4h], ecx
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        ret
loc_3_0548:
        push   esi
        push   edi
        db  0BEh, 01Ch, 000h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    eax, edi
        mov    ecx, 40h
        rep movsb
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [edx + 40h]
        mov    dword ptr [eax + 30h], ecx
        mov    ecx, dword ptr [edx + 48h]
        mov    dword ptr [eax + 2ch], ecx
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    edi
        pop    esi
        ret
loc_3_0575:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0ech], eax
        mov    ecx, dword ptr [ebp + 10h]
        jecxz  loc_3_058F
        mov    dword ptr [edx + 0f0h], ecx
loc_3_058F:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        leave
        ret
loc_3_0597:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        and    byte ptr [edx + 0cdh], 0bfh
        mov    dword ptr [edx + 110h], eax
        mov    dword ptr [edx + 0c8h], ecx
        jecxz  loc_3_05E1
        cli
        or     byte ptr [edx + 0cdh], 40h
        mov    ecx, dword ptr [edx + 0f0h]
        mov    eax, dword ptr [edx + 0ech]
        and    eax, dword ptr [ecx]
        sti
        or     eax, eax
        je     SHORT loc_3_05E1
        push   esi
        mov    esi, edx
        mov    eax, 4
        db  0E8h, 0ACh, 004h, 0FDh, 0FFh
        pop    esi
loc_3_05E1:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        ret
loc_3_05E8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, ecx
        inc    eax
        je     SHORT loc_3_0600
        cmp    ecx, dword ptr [esi + 40h]
        jb     SHORT loc_3_0600
        mov    ecx, dword ptr [esi + 40h]
loc_3_0600:
        mov    ebx, dword ptr [ebp + 10h]
        or     ebx, ebx
        jne    SHORT loc_3_060A
        or     ecx, 0ffffffffh
loc_3_060A:
        cli
        mov    dword ptr [esi + 0d0h], ebx
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 0d4h], edx
        mov    dword ptr [esi + 0e0h], ecx
        sti
        cmp    dword ptr [esi + 4ch], 0
        jne    SHORT loc_3_0633
        mov    esi, dword ptr [esi + 12ch]
        mov    dword ptr [esi], 0
loc_3_0633:
        db  0E8h, 0FEh, 009h, 0FDh, 0FFh
        or     al, 1
        pop    ebx
        pop    esi
        leave
        ret
loc_3_063E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        inc    edx
        je     SHORT loc_3_065E
        dec    edx
        test   byte ptr [esi + 114h], 10h
        je     SHORT loc_3_065E
        cmp    edx, dword ptr [esi + 48h]
        jbe    SHORT loc_3_065E
        mov    edx, dword ptr [esi + 48h]
loc_3_065E:
        mov    ebx, dword ptr [ebp + 10h]
        or     ebx, ebx
        jne    SHORT loc_3_0667
        xor    edx, edx
loc_3_0667:
        cli
        mov    dword ptr [esi + 0e4h], edx
        mov    dword ptr [esi + 0d8h], ebx
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 0dch], ebx
        cmp    dword ptr [esi + 58h], edx
        jae    SHORT loc_3_0689
        or     byte ptr [esi + 0cdh], 2
loc_3_0689:
        sti
        or     al, 1
        pop    ebx
        pop    esi
        leave
        ret
loc_3_0690:
        mov    edx, dword ptr [esp + 4]
        mov    edx, dword ptr [edx + 0c4h]
        add    dl, 6
        in     al, dx
        and    eax, 0f0h
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [edx], eax
        or     al, 1
        ret
loc_3_06AC:
        xor    eax, eax
        mov    edx, dword ptr [esp + 0ch]
        push   34h
        pop    ecx
        cmp    dword ptr [edx], ecx
        mov    dword ptr [edx], ecx
        jb     SHORT loc_3_06EF
        mov    edx, dword ptr [esp + 8]
        or     edx, edx
        je     SHORT loc_3_06EF
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [edx + 2ch], eax
        mov    byte ptr [edx + 30h], al
        mov    al, 34h
        mov    dword ptr [edx], eax
        mov    al, 1
        mov    dword ptr [edx + 24h], eax
        shl    eax, 8
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [esp + 4]
        lea    eax, [eax + 94h]
        lea    edx, [edx + 8]
        db  0CDh, 020h, 020h, 000h, 02Bh, 000h
loc_3_06EF:
        or     al, 1
        ret
loc_3_06F2:
        sub    esp, 30h
        xor    eax, eax
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [edx + 0ch], 0ffffffebh
        cmp    dword ptr [esp + 3ch], 34h
        jb     SHORT loc_3_0728
        lea    edx, [esp]
        mov    eax, dword ptr [esp + 38h]
        lea    eax, [eax + 8]
        db  0CDh, 020h, 01Fh, 000h, 02Bh, 000h
        push   -1
        push   edx
        push   dword ptr [esp + 3ch]
        db  0E8h, 041h, 004h, 0FDh, 0FFh
        add    esp, 0ch
loc_3_0728:
        add    esp, 30h
        ret
loc_3_072C:
        ret
        db  000h, 000h, 000h
loc_3_0730:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_3_0739
        ret
loc_3_0739:
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_3_08EF
        db  08Bh, 035h, 058h, 011h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_3_076D
        mov    ecx, dword ptr [esp + 1ch]
loc_3_0759:
        cmp    dword ptr [eax + 0c4h], ecx
        je     NEAR PTR loc_3_08EF
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_3_0759
loc_3_076D:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_3_08EF
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    edi, eax
        mov    ebx, eax
        xor    eax, eax
        mov    ecx, 4dh
        rep stosd
        mov    dword ptr [ebx], 10a0094h
        db  0C7h, 043h, 004h, 0FCh, 010h, 000h, 000h
        mov    dword ptr [ebx + 8], 17h
        mov    word ptr [ebx + 0adh], 1311h
        dec    dword ptr [ebx + 0e0h]
        mov    byte ptr [ebx + 122h], 80h
        mov    byte ptr [ebx + 123h], 10h
        db  08Dh, 035h, 069h, 000h, 000h, 000h
        mov    dword ptr [esi], 80001002h
        db  0C7h, 005h, 06Dh, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        push   0
        db  068h, 06Dh, 000h, 000h, 000h
        push   esi
        push   3
        db  068h, 060h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 2ch]
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_083A
        mov    cl, byte ptr [esi + 3]
        mov    byte ptr [ebx + 122h], cl
        cmp    byte ptr [esi + 2], 0
        je     SHORT loc_3_080B
        or     byte ptr [ebx + 0f8h], 1
loc_3_080B:
        cmp    byte ptr [esi], 1
        ja     SHORT loc_3_0822
        jb     SHORT loc_3_081B
        or     byte ptr [ebx + 0f8h], 4
        jmp    SHORT loc_3_0822
loc_3_081B:
        or     byte ptr [ebx + 0f8h], 2
loc_3_0822:
        cmp    byte ptr [esi + 1], 1
        jbe    SHORT loc_3_0833
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [ebx + 123h], cl
        jmp    SHORT loc_3_083A
loc_3_0833:
        or     byte ptr [ebx + 0f8h], 10h
loc_3_083A:
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 0c4h], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    byte ptr [ebx + 115h], cl
        db  08Bh, 035h, 05Ch, 000h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_3_0868
loc_3_085C:
        cmp    byte ptr [eax], cl
        je     SHORT loc_3_087E
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_3_085C
loc_3_0868:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    dword ptr [eax], ecx
        xor    ecx, ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 4], ecx
loc_3_087E:
        mov    dword ptr [ebx + 116h], eax
        mov    edi, dword ptr [esp + 24h]
        mov    esi, edi
        xor    eax, eax
        xor    ecx, ecx
        dec    ecx
        repne scasb
        neg    ecx
        dec    ecx
        push   ecx
        push   0
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    edi, eax
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_08EA
        mov    dword ptr [ebx + 11eh], eax
        rep movsb
        push   eax
        db  068h, 0D0h, 002h, 000h, 000h
        push   dword ptr [esp + 20h]
        db  0CDh, 020h, 019h, 000h, 02Bh, 000h
        add    esp, 0ch
        push   dword ptr [esp + 24h]
        db  0CDh, 020h, 021h, 000h, 02Bh, 000h
        pop    ecx
        mov    dword ptr [ebx + 124h], eax
        test   eax, eax
        je     SHORT loc_3_08EF
        push   dword ptr [esp + 24h]
        db  0CDh, 020h, 022h, 000h, 02Bh, 000h
        pop    ecx
        mov    dword ptr [ebx + 128h], eax
        jmp    SHORT loc_3_08EF
loc_3_08EA:
        call   sub_3_08F3
loc_3_08EF:
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_3_08F3:
        push   esi
        db  08Bh, 035h, 05Ch, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 116h]
        or     eax, eax
        je     SHORT loc_3_0916
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_3_0916
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_3_0916:
        cmp    dword ptr [ebx + 11eh], 0
        je     SHORT loc_3_092F
        push   0
        push   dword ptr [ebx + 11eh]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_092F:
        db  08Bh, 035h, 058h, 011h, 000h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        pop    esi
        ret
_PTEXT ends
        end
