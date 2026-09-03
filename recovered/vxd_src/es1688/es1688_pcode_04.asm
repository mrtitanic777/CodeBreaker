; es1688_pcode_04.asm -- ES1688 object 4 segment _PTEXT
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
        je     SHORT loc_4_0027
        cmp    dword ptr [edi + 33h], 0
        jne    SHORT loc_4_002E
        cmp    dword ptr [edi + 3bh], 0
        jne    SHORT loc_4_002E
        cmp    dword ptr [edi + 43h], 0
        jne    SHORT loc_4_002E
loc_4_0027:
        mov    eax, 0
        jmp    SHORT loc_4_0033
loc_4_002E:
        mov    eax, 0ffffffffh
loc_4_0033:
        pop    edi
        pop    ebp
        ret
sub_4_0036:
        cmp    eax, 40h
        jle    SHORT loc_4_0040
        mov    eax, 40h
loc_4_0040:
        add    eax, 3
        shr    eax, 2
loc_4_0046:
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
        jne    SHORT loc_4_0088
        mov    eax, ecx
        shr    eax, 1
        jne    SHORT loc_4_0046
        xor    ecx, ecx
        stc
        ret
loc_4_0088:
        shl    ecx, 0ch
        clc
        ret
sub_4_008D:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_4_00C8
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_4_00C8
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 24h
        shr    ecx, 2
        rep stosd
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        clc
loc_4_00C3:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
loc_4_00C8:
        stc
        jmp    SHORT loc_4_00C3
sub_4_00CB:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     SHORT loc_4_0103
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_0103
loc_4_00E6:
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_4_00F5
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_0103
        jmp    SHORT loc_4_00E6
loc_4_00F5:
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_0103:
        popfd
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_4_0109:
        jmp    SHORT loc_4_0115
loc_4_010B:
        mov    esi, 0
        jmp    NEAR PTR loc_4_01CD
loc_4_0115:
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
        je     NEAR PTR loc_4_01C7
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_0150:
        push   ecx
        mov    ecx, 0ah
        db  0BEh, 000h, 000h, 000h, 000h
        cld
        rep movsb
        pop    ecx
        loop   loc_4_0150
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_016B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_019E
        add    edi, 6
        add    esi, 0ah
        loop   loc_4_016B
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_01CD
loc_4_019E:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_01B9
        add    edi, 2
loc_4_01AB:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_01AB
loc_4_01B9:
        push   0
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_01C7:
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
loc_4_01CD:
        popad
        add    esp, 0ch
        pop    ebp
        ret
sub_4_01D3:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_01DD:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_01DD
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret
sub_4_01FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_4_04EA
        movzx  edx, word ptr [ebp + 0ch]
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0224:
        add    esp, 8
        or     edi, edi
        jne    NEAR PTR loc_4_04EA
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_04EA
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        push   1
        push   86h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_04EA
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 53h], eax
        mov    word ptr [edi + 84h], 0
        mov    ax, word ptr [ebp + 28h]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [edi + 13h], ax
        mov    byte ptr [edi + 0fh], 16h
        mov    ax, word ptr [ebp + 18h]
        mov    byte ptr [edi + 10h], al
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [ebp + 2ch]
        mov    word ptr [edi + 0ah], ax
        mov    ax, word ptr [ebp + 30h]
        mov    byte ptr [edi + 0ch], al
        mov    ax, word ptr [ebp + 1ch]
        mov    byte ptr [edi + 0dh], al
        mov    ax, word ptr [ebp + 20h]
        mov    byte ptr [edi + 0eh], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02C5:
        mov    word ptr [edi + 15h], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02CE:
        jb     NEAR PTR loc_4_04EA
        push   edi
        movzx  ecx, byte ptr [edi + 0dh]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 01Ch, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jae    SHORT loc_4_0306
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02FE:
        add    esp, 4
        jmp    NEAR PTR loc_4_04EA
loc_4_0306:
        mov    dword ptr [edi + 2fh], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_0323
        or     word ptr [edi + 4], 8000h
loc_4_0323:
        movsx  edx, word ptr [ebp + 2ch]
        cmp    edx, -1
        je     SHORT loc_4_037B
        push   edi
        movzx  ecx, byte ptr [edi + 0ch]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 01Ch, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jae    SHORT loc_4_035E
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0356:
        add    esp, 4
        jmp    NEAR PTR loc_4_04EA
loc_4_035E:
        mov    dword ptr [edi + 2bh], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_037B
        or     word ptr [edi + 4], 4000h
loc_4_037B:
        db  0BEh, 000h, 000h, 000h, 000h
        cmp    word ptr [ebp + 14h], -1
        jne    SHORT loc_4_038C
        db  0BEh, 000h, 000h, 000h, 000h
loc_4_038C:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0ch]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0399:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0399
        mov    edx, dword ptr [esp]
        call   sub_4_0109
        pop    edi
        mov    dword ptr [edi + 57h], eax
        or     eax, eax
        jne    SHORT loc_4_03C4
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03BC:
        add    esp, 4
        jmp    NEAR PTR loc_4_04EA
loc_4_03C4:
        movsx  edx, word ptr [ebp + 2ch]
        cmp    edx, -1
        je     SHORT loc_4_040A
        mov    word ptr [edi + 0ah], dx
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_03DF:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_03DF
        mov    edx, dword ptr [esp]
        call   sub_4_0109
        pop    edi
        mov    dword ptr [edi + 5fh], eax
        or     eax, eax
        jne    SHORT loc_4_040A
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0402:
        add    esp, 4
        jmp    NEAR PTR loc_4_04EA
loc_4_040A:
        movsx  edx, word ptr [ebp + 14h]
        cmp    edx, -1
        je     SHORT loc_4_0454
        mov    word ptr [edi + 8], dx
        mov    dx, 388h
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0429:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0429
        mov    edx, dword ptr [esp]
        call   sub_4_0109
        pop    edi
        mov    dword ptr [edi + 5bh], eax
        or     eax, eax
        jne    SHORT loc_4_0454
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_044C:
        add    esp, 4
        jmp    NEAR PTR loc_4_04EA
loc_4_0454:
        mov    eax, dword ptr [ebp + 24h]
        call   sub_4_0036
        mov    dword ptr [edi + 17h], eax
        mov    dword ptr [edi + 1bh], ebx
        mov    dword ptr [edi + 23h], ecx
        mov    dword ptr [edi + 1fh], edx
        jae    SHORT loc_4_0479
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0474:
        add    esp, 4
        jmp    SHORT loc_4_04EA
loc_4_0479:
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
        mov    word ptr [edi + 27h], ax
        movzx  eax, word ptr [ebp + 20h]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        mov    dword ptr [edi + 4bh], eax
        jae    SHORT loc_4_04C1
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04BC:
        add    esp, 4
        jmp    SHORT loc_4_04EA
loc_4_04C1:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_04C8:
        call   sub_4_008D
        jb     SHORT loc_4_04E9
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_04C8
        pop    ebx
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_04E3:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_04E9:
        pop    ebx
loc_4_04EA:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_04F6
        call   sub_4_0500
loc_4_04F6:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_04E3
sub_4_0500:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     NEAR PTR loc_4_0610
        xor    esi, esi
        xchg   dword ptr [edi + 4fh], esi
        or     esi, esi
        je     SHORT loc_4_0523
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_0523:
        mov    eax, dword ptr [edi + 4bh]
        or     eax, eax
        je     SHORT loc_4_0530
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_0530:
        movzx  eax, word ptr [edi + 27h]
        or     eax, eax
        je     SHORT loc_4_0553
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [edi + 17h]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_0553:
        mov    eax, dword ptr [edi + 5bh]
        or     eax, eax
        je     SHORT loc_4_0582
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        mov    edx, 388h
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_056D:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_056D
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 5bh]
        call   sub_4_01D3
        pop    edi
loc_4_0582:
        mov    eax, dword ptr [edi + 5fh]
        or     eax, eax
        je     SHORT loc_4_05B0
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0ah]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_059B:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_059B
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 5fh]
        call   sub_4_01D3
        pop    edi
loc_4_05B0:
        mov    eax, dword ptr [edi + 57h]
        or     eax, eax
        je     SHORT loc_4_05EA
        db  0BEh, 000h, 000h, 000h, 000h
        cmp    word ptr [edi + 8], -1
        jne    SHORT loc_4_05C8
        db  0BEh, 000h, 000h, 000h, 000h
loc_4_05C8:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_05D5:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_05D5
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 57h]
        call   sub_4_01D3
        pop    edi
loc_4_05EA:
        mov    eax, dword ptr [edi + 2bh]
        or     eax, eax
        je     SHORT loc_4_05F7
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_05F7:
        mov    eax, dword ptr [edi + 2fh]
        or     eax, eax
        je     SHORT loc_4_0604
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_0604:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0610:
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
sub_4_062D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushad
        mov    dword ptr [esp + 1ch], 13h
        movzx  edx, word ptr [ebp + 0ch]
        call   sub_4_07E8
        jb     NEAR PTR loc_4_0701
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 8], dx
        movzx  edx, word ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_065C:
        test   dword ptr [ebp + 28h], 50h
        jne    SHORT loc_4_067C
        movsx  eax, word ptr [ebp + 2ch]
        movsx  ecx, word ptr [ebp + 10h]
        mov    bx, word ptr [ebp + 30h]
        mov    edi, dword ptr [ebp + 34h]
        mov    esi, dword ptr [ebp + 38h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_067C:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0681:
        movsx  edx, word ptr [ebp + 10h]
        cmp    edx, -1
        je     SHORT loc_4_0691
        call   sub_4_0865
        jb     SHORT loc_4_06FB
loc_4_0691:
        movsx  edx, word ptr [ebp + 2ch]
        cmp    edx, -1
        je     SHORT loc_4_06A1
        call   sub_4_08D0
        jb     SHORT loc_4_06FB
loc_4_06A1:
        test   dword ptr [ebp + 28h], 50h
        jne    SHORT loc_4_06C6
        movzx  edx, word ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 1ch]
        mov    bx, word ptr [ebp + 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06BA:
        mov    esi, dword ptr [ebp + 20h]
        mov    bx, word ptr [ebp + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06C6:
        push   dword ptr [ebp + 30h]
        push   dword ptr [ebp + 2ch]
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_01FC
        add    esp, 2ch
        or     eax, eax
        je     SHORT loc_4_06FB
        mov    dword ptr [esp + 1ch], 0
loc_4_06FB:
        popad
        add    esp, 8
        pop    ebp
        ret
loc_4_0701:
        mov    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_4_06FB
sub_4_070B:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        pushfd
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_0764
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_072A:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_0764
        mov    ebx, dword ptr [edi + 33h]
        or     ebx, ebx
        je     SHORT loc_4_0742
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0742:
        mov    ebx, dword ptr [edi + 43h]
        or     ebx, ebx
        je     SHORT loc_4_0753
        mov    eax, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0753:
        mov    ebx, dword ptr [edi + 3bh]
        or     ebx, ebx
        je     SHORT loc_4_0764
        mov    eax, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0764:
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_07B1
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_07B1
loc_4_077A:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 53h], ebx
        je     SHORT loc_4_078B
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_07B1
        jmp    SHORT loc_4_077A
loc_4_078B:
        popfd
        pushfd
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0794:
        call   sub_4_00CB
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0794
        pop    ebx
        mov    edi, eax
        call   sub_4_0500
        mov    eax, 0ffffffffh
loc_4_07B1:
        popfd
        pop    ebx
        pop    edi
        pop    ebp
        ret
loc_4_07B6:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07CD:
        add    esp, 14h
        leave
        clc
        ret
loc_4_07D3:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
        db  0CCh
sub_4_07E8:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        xor    al, al
        out    dx, al
        sub    edx, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07FE:
        jb     SHORT loc_4_0852
        cmp    al, 0aah
        jne    SHORT loc_4_0852
        mov    al, 0e1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_080B:
        jb     SHORT loc_4_0852
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0812:
        jb     SHORT loc_4_0852
        mov    ah, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_081B:
        jb     SHORT loc_4_0852
        movzx  eax, ax
        mov    dword ptr [esp + 4], eax
        mov    al, 0e7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_082B:
        jb     SHORT loc_4_0852
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0832:
        jb     SHORT loc_4_0852
        mov    ah, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_083B:
        jb     SHORT loc_4_0852
        cmp    ah, 68h
        jne    SHORT loc_4_0852
        mov    ah, al
        and    ah, 0fh
        cmp    ah, 8
        jb     SHORT loc_4_0852
        and    al, 0f0h
        cmp    al, 80h
        je     SHORT loc_4_085B
loc_4_0852:
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        stc
        jmp    SHORT loc_4_0862
loc_4_085B:
        movzx  eax, ah
        mov    dword ptr [esp], eax
        clc
loc_4_0862:
        pop    edx
        pop    eax
        ret
sub_4_0865:
        push   eax
        push   ebx
        push   ecx
        pushfd
        cli
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0876:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0882:
        mov    ebx, 2
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_088E:
        mov    ebx, 4
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_089A:
        mov    cx, 4000h
loc_4_089E:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_4_08A5
        loop   loc_4_089E
loc_4_08A5:
        push   eax
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08B2:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08BE:
        pop    eax
        popfd
        test   al, 40h
        je     SHORT loc_4_08CB
        test   al, 6
        jne    SHORT loc_4_08CB
        clc
        jmp    SHORT loc_4_08CC
loc_4_08CB:
        stc
loc_4_08CC:
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_4_08D0:
        enter  8, 0
        push   eax
        push   ecx
        push   edx
        mov    dword ptr [ebp - 8], 0
loc_4_08DE:
        mov    dword ptr [ebp - 4], 0
        mov    ecx, 40h
loc_4_08EA:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 80h
        jne    SHORT loc_4_08F4
        in     al, dx
        loop   loc_4_08EA
loc_4_08F4:
        mov    ecx, 1000h
loc_4_08F9:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 40h
        je     SHORT loc_4_0916
        loop   loc_4_08F9
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0940
        mov    dword ptr [ebp - 4], 0ffffffffh
        inc    edx
        mov    al, 0ffh
        out    dx, al
        dec    edx
        jmp    SHORT loc_4_08F4
loc_4_0916:
        mov    al, 0ffh
        inc    edx
        out    dx, al
        dec    edx
        mov    ecx, 1000h
loc_4_0920:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 80h
        je     SHORT loc_4_0938
        loop   loc_4_0920
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_0940
        mov    dword ptr [ebp - 8], 0ffffffffh
        jmp    SHORT loc_4_08DE
loc_4_0938:
        in     al, dx
        cmp    al, 0feh
        jne    SHORT loc_4_0940
        clc
        jmp    SHORT loc_4_0941
loc_4_0940:
        stc
loc_4_0941:
        pop    edx
        pop    ecx
        pop    eax
        leave
        ret
_PTEXT ends
        end
