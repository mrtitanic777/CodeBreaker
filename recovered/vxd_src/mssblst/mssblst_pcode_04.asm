; mssblst_pcode_04.asm -- MSSBLST object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_0021
        cmp    dword ptr [edi + 28h], 0
        jne    SHORT loc_4_0028
        cmp    dword ptr [edi + 30h], 0
        jne    SHORT loc_4_0028
loc_4_0021:
        mov    eax, 0
        jmp    SHORT loc_4_002D
loc_4_0028:
        mov    eax, 0ffffffffh
loc_4_002D:
        pop    edi
        pop    ebp
        ret
sub_4_0030:
        cmp    eax, 40h
        jle    SHORT loc_4_003A
        mov    eax, 40h
loc_4_003A:
        add    eax, 3
        shr    eax, 2
loc_4_0040:
        mov    ebx, eax
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        mov    ecx, ebx
        push   ecx
        sub    esp, 4
        mov    ebx, esp
        push   0eh
        push   ebx
        push   0fffh
        push   0
        push   eax
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ebx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_4_0082
        mov    eax, ecx
        shr    eax, 1
        jne    SHORT loc_4_0040
        xor    ecx, ecx
        stc
        ret
loc_4_0082:
        shl    ecx, 0ch
        clc
        ret
sub_4_0087:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_4_00C2
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_4_00C2
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 8
        shr    ecx, 2
        rep stosd
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        clc
loc_4_00BD:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
loc_4_00C2:
        stc
        jmp    SHORT loc_4_00BD
sub_4_00C5:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     SHORT loc_4_00FD
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_00FD
loc_4_00E0:
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_4_00EF
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_00FD
        jmp    SHORT loc_4_00E0
loc_4_00EF:
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_00FD:
        popfd
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_4_0103:
        jmp    SHORT loc_4_010F
loc_4_0105:
        mov    esi, 0
        jmp    NEAR PTR loc_4_01C8
loc_4_010F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        pushad
        mov    dword ptr [ebp - 0ch], edx
        movzx  ecx, word ptr [edi]
        mov    eax, ecx
        shl    eax, 2
        add    eax, ecx
        shl    eax, 1
        mov    dword ptr [ebp - 4], eax
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_01C2
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_014A:
        push   ecx
        mov    ecx, 0ah
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        cld
        rep movsb
        pop    ecx
        loop   loc_4_014A
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_0166:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_0199
        add    edi, 6
        add    esi, 0ah
        loop   loc_4_0166
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_01C8
loc_4_0199:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_01B4
        add    edi, 2
loc_4_01A6:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_01A6
loc_4_01B4:
        push   0
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_01C2:
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
loc_4_01C8:
        popad
        add    esp, 0ch
        pop    ebp
        ret
sub_4_01CE:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_01D8:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_01D8
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret
sub_4_01F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_4_041E
        movzx  edx, word ptr [ebp + 0ch]
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_021F:
        add    esp, 8
        or     edi, edi
        jne    NEAR PTR loc_4_041E
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_041E
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        push   1
        push   52h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_041E
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 40h], eax
        mov    ax, word ptr [ebp + 24h]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [edi + 0ch], ax
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [ebp + 18h]
        mov    byte ptr [edi + 0ah], al
        mov    ax, word ptr [ebp + 1ch]
        mov    byte ptr [edi + 0bh], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_029A:
        mov    word ptr [edi + 0eh], ax
        push   edi
        movzx  ecx, byte ptr [edi + 0ah]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jae    SHORT loc_4_02CA
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02C2:
        add    esp, 4
        jmp    NEAR PTR loc_4_041E
loc_4_02CA:
        mov    dword ptr [edi + 24h], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_02E7
        or     word ptr [edi + 4], 8000h
loc_4_02E7:
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0ch]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_02F9:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_02F9
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 44h], eax
        or     eax, eax
        jne    SHORT loc_4_0324
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_031C:
        add    esp, 4
        jmp    NEAR PTR loc_4_041E
loc_4_0324:
        movsx  edx, word ptr [ebp + 14h]
        cmp    edx, -1
        je     SHORT loc_4_0370
        mov    word ptr [edi + 8], dx
        or     word ptr [edi + 4], 100h
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0345:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0345
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 48h], eax
        or     eax, eax
        jne    SHORT loc_4_0370
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0368:
        add    esp, 4
        jmp    NEAR PTR loc_4_041E
loc_4_0370:
        mov    eax, dword ptr [ebp + 20h]
        call   sub_4_0030
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], ebx
        mov    dword ptr [edi + 1ch], ecx
        mov    dword ptr [edi + 18h], edx
        jae    SHORT loc_4_0398
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0390:
        add    esp, 4
        jmp    NEAR PTR loc_4_041E
loc_4_0398:
        push   0
        push   0
        push   92h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        mov    word ptr [edi + 20h], ax
        movzx  eax, word ptr [ebp + 1ch]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        mov    dword ptr [edi + 38h], eax
        jae    SHORT loc_4_03E0
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03DB:
        add    esp, 4
        jmp    SHORT loc_4_041E
loc_4_03E0:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_03E7:
        call   sub_4_0087
        jb     SHORT loc_4_041D
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_03E7
        pop    ebx
        mov    dword ptr [esp + 1ch], 0ffffffffh
        mov    eax, dword ptr [edi + 24h]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_4_0417:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_041D:
        pop    ebx
loc_4_041E:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_042A
        call   sub_4_0434
loc_4_042A:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_0417
sub_4_0434:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     NEAR PTR loc_4_04FC
        xor    esi, esi
        xchg   dword ptr [edi + 3ch], esi
        or     esi, esi
        je     SHORT loc_4_0457
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_0457:
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_4_0464
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_0464:
        movzx  eax, word ptr [edi + 20h]
        or     eax, eax
        je     SHORT loc_4_0487
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [edi + 10h]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_0487:
        mov    eax, dword ptr [edi + 48h]
        or     eax, eax
        je     SHORT loc_4_04B5
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 8]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04A0:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04A0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 48h]
        call   sub_4_01CE
        pop    edi
loc_4_04B5:
        mov    eax, dword ptr [edi + 44h]
        or     eax, eax
        je     SHORT loc_4_04E3
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04CE:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04CE
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 44h]
        call   sub_4_01CE
        pop    edi
loc_4_04E3:
        mov    eax, dword ptr [edi + 24h]
        or     eax, eax
        je     SHORT loc_4_04F0
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_04F0:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_04FC:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        popad
        add    esp, 4
        pop    ebp
        ret
sub_4_0519:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [esp + 1ch], 13h
        movzx  edx, word ptr [ebp + 0ch]
        call   sub_4_0680
        jb     SHORT loc_4_05A7
        test   ax, 8000h
        je     SHORT loc_4_0544
        or     dword ptr [ebp + 20h], 400h
        and    ax, 7fffh
loc_4_0544:
        mov    word ptr [ebp - 4], ax
        test   dword ptr [ebp + 20h], 10h
        jne    SHORT loc_4_0575
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_071B
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_05A1
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_08B5
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_05A1
loc_4_0575:
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_01F7
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_4_05A1
        mov    dword ptr [esp + 1ch], 0
loc_4_05A1:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_05A7:
        mov    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_4_05A1
sub_4_05B1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_05FA
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05D1:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_05FA
        mov    ebx, dword ptr [edi + 28h]
        or     ebx, ebx
        je     SHORT loc_4_05E9
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05E9:
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_4_05FA
        mov    eax, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05FA:
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_0647
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_0647
loc_4_0610:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 40h], ebx
        je     SHORT loc_4_0621
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_0647
        jmp    SHORT loc_4_0610
loc_4_0621:
        popfd
        pushfd
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_062A:
        call   sub_4_00C5
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_062A
        pop    ebx
        mov    edi, eax
        call   sub_4_0434
        mov    eax, 0ffffffffh
loc_4_0647:
        popfd
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_064D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0664:
        add    esp, 14h
        leave
        clc
        ret
loc_4_066A:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
        db  0CCh, 0CCh
sub_4_0680:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, al
        out    dx, al
        sub    edx, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0695:
        jb     SHORT loc_4_070E
        cmp    al, 0aah
        jne    SHORT loc_4_070E
        mov    al, 0e1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06A2:
        jb     SHORT loc_4_070E
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06A9:
        jb     SHORT loc_4_070E
        mov    ah, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06B2:
        jb     SHORT loc_4_070E
        movzx  eax, ax
        mov    dword ptr [esp + 4], eax
        mov    al, 0e1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06C2:
        jb     SHORT loc_4_070E
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06C9:
        jb     SHORT loc_4_070E
        mov    ah, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06D2:
        jb     SHORT loc_4_070E
        movzx  eax, ax
        cmp    dword ptr [esp + 4], eax
        je     SHORT loc_4_06E5
        or     dword ptr [esp + 4], 8000h
loc_4_06E5:
        cmp    ax, 100h
        jb     SHORT loc_4_070E
        cmp    ax, 400h
        jae    SHORT loc_4_070E
        mov    al, 0e0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06F8:
        jb     SHORT loc_4_070E
        mov    al, 0aah
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0701:
        jb     SHORT loc_4_070E
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0708:
        jb     SHORT loc_4_070E
        cmp    al, 55h
        je     SHORT loc_4_0717
loc_4_070E:
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        stc
        jmp    SHORT loc_4_0718
loc_4_0717:
        clc
loc_4_0718:
        pop    edx
        pop    eax
        ret
sub_4_071B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        movzx  eax, word ptr [ebp + 0ch]
        xor    ecx, ecx
        bts    ecx, eax
        pushfd
        cli
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        push   eax
        or     eax, ecx
        xchg   ah, al
        out    0a1h, al
        xchg   ah, al
        out    21h, al
        mov    al, 0ah
        out    0a0h, al
        jmp    SHORT loc_4_0747
loc_4_0747:
        out    20h, al
        jmp    SHORT loc_4_074B
loc_4_074B:
        add    edx, 0eh
        in     al, dx
        sub    edx, 0eh
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0779
        add    edx, 0ch
        mov    al, 0f2h
        out    dx, al
        sub    edx, 0ch
        push   ecx
        mov    ecx, 1f4h
loc_4_076B:
        in     al, dx
        loop   loc_4_076B
        pop    ecx
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0785
loc_4_0779:
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0780:
        add    esp, 8
        xor    eax, eax
loc_4_0785:
        add    edx, 0eh
        in     al, dx
        sub    edx, 0eh
        mov    edx, eax
        pop    eax
        xchg   ah, al
        out    0a1h, al
        xchg   ah, al
        out    21h, al
        mov    eax, edx
        popfd
        pop    edx
        pop    ecx
        pop    ebx
        pop    ebp
        ret
sub_4_079F:
        push   ebp
        mov    ebp, esp
        movzx  edx, word ptr [ebp + 8]
        pushfd
        mov    eax, 0ffffffffh
        pushad
        cli
        movzx  ebx, word ptr [ebp + 0ch]
        and    ebx, 3
        mov    eax, ebx
        or     al, 4
        out    0ah, al
        xor    al, al
        out    0ch, al
        mov    eax, ebx
        test   dword ptr [ebp + 14h], 1
        jne    SHORT loc_4_07CE
        add    al, 48h
        jmp    SHORT loc_4_07D0
loc_4_07CE:
        add    al, 44h
loc_4_07D0:
        out    0bh, al
        mov    edx, ebx
        shl    edx, 1
        add    edx, 0
        mov    eax, dword ptr [ebp + 10h]
        out    dx, al
        shr    eax, 8
        out    dx, al
        movzx  ebx, word ptr [ebp + 0ch]
        db  00Fh, 0B6h, 093h, 000h, 000h, 000h, 000h
        shr    eax, 8
        out    dx, al
        and    ebx, 3
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    al, 0fh
        out    dx, al
        xor    al, al
        out    dx, al
        mov    eax, ebx
        out    0ah, al
        movzx  edx, word ptr [ebp + 8]
        add    edx, 4
        mov    al, 4
        out    dx, al
        inc    dx
        xor    al, al
        out    dx, al
        sub    dx, 5
        mov    al, 0d3h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_081E:
        mov    al, 40h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0825:
        mov    al, 2dh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_082C:
        movzx  edx, word ptr [ebp + 8]
        mov    al, 14h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0837:
        mov    al, 0fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_083E:
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0845:
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    ecx, 0ffffh
loc_4_0851:
        xor    al, al
        out    0ch, al
        jmp    SHORT loc_4_0857
loc_4_0857:
        in     al, dx
        mov    ah, al
        jmp    SHORT loc_4_085C
loc_4_085C:
        in     al, dx
        cmp    ax, -1
        je     SHORT loc_4_0890
        loop   loc_4_0851
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_086C:
        add    esp, 8
        mov    dword ptr [esp + 1ch], 0
        movzx  edx, word ptr [ebp + 8]
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, al
        out    dx, al
        sub    edx, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_088E:
        jmp    SHORT loc_4_08B1
loc_4_0890:
        movzx  edx, word ptr [ebp + 8]
        mov    al, 0d0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_089B:
        mov    eax, ebx
        and    eax, 3
        or     eax, 4
        out    0ah, al
        mov    ecx, 64h
loc_4_08AA:
        in     al, dx
        loop   loc_4_08AA
        add    edx, 0eh
        in     al, dx
loc_4_08B1:
        popad
        popfd
        pop    ebp
        ret
sub_4_08B5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushad
        lea    ebx, [ebp - 4]
        push   0eh
        push   ebx
        push   0fffh
        push   0
        push   0
        push   0
        push   1
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_4_0903
        mov    edi, edx
        mov    ecx, 400h
        mov    eax, 80808080h
        cld
        rep stosd
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_079F
        add    esp, 10h
loc_4_0903:
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [ebp - 8]
        or     eax, eax
        je     SHORT loc_4_091A
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_091A:
        popad
        add    esp, 8
        pop    ebp
        ret
_PTEXT ends
        end
