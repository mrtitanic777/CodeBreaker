; vcd_pcode_04.asm -- VCD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0E8h, 059h, 000h, 0FCh, 0FFh
        mov    dl, 10h
        jmp    SHORT loc_4_0017
        db  0B2h, 00Ch, 0EBh, 00Ah, 0B2h, 008h, 0EBh, 006h, 0B2h, 004h, 0EBh, 002h, 0B2h, 000h
loc_4_0017:
        db  0E8h, 0C2h, 001h, 0FCh, 0FFh
sub_4_001C:
        mov    edx, dword ptr [esi + 18h]
        add    edx, 3
        in     al, dx
        mov    byte ptr [esi + 37h], al
        or     al, 80h
        jmp    SHORT loc_4_002A
loc_4_002A:
        out    dx, al
        add    edx, -3
        jmp    SHORT loc_4_0030
loc_4_0030:
        in     al, dx
        mov    byte ptr [esi + 34h], al
        inc    edx
        jmp    SHORT loc_4_0037
loc_4_0037:
        in     al, dx
        mov    byte ptr [esi + 35h], al
        add    edx, 2
        mov    al, byte ptr [esi + 37h]
        jmp    SHORT loc_4_0043
loc_4_0043:
        out    dx, al
        add    edx, -2
        jmp    SHORT loc_4_0049
loc_4_0049:
        in     al, dx
        mov    byte ptr [esi + 36h], al
        add    edx, 3
        jmp    SHORT loc_4_0052
loc_4_0052:
        in     al, dx
        mov    byte ptr [esi + 38h], al
        inc    edx
        jmp    SHORT loc_4_0059
loc_4_0059:
        in     al, dx
        or     al, 60h
        mov    byte ptr [esi + 39h], al
        inc    edx
        jmp    SHORT loc_4_0062
loc_4_0062:
        in     al, dx
        mov    byte ptr [esi + 3ah], al
        mov    byte ptr [esi + 3bh], 1
        ret
VCD_Virtualize_Port_Dynamic:
        pushad
        add    eax, 90h
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_4_013E
        mov    esi, eax
        add    esi, 1ch
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + edx*4 - 4], esi
        mov    byte ptr [esi + 4], dl
        mov    edx, dword ptr [esp + 4]
        mov    byte ptr [esi + 5], dl
        shr    edx, 10h
        mov    dword ptr [esi + 18h], edx
        push   esi
        mov    edi, eax
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 7
        cld
        rep movsd
        pop    esi
        db  0BFh, 0DEh, 001h, 000h, 000h
        sub    edi, esi
        mov    dword ptr [esi - 4], edi
        db  0BFh, 059h, 000h, 000h, 000h
        sub    edi, eax
        mov    dword ptr [esi - 1bh], edi
        mov    eax, 18h
        add    eax, dword ptr [esp + 18h]
        push   0
        push   eax
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_013E
        mov    dword ptr [esi], eax
        call   sub_4_001C
        mov    edx, dword ptr [esp + 4]
        shr    edx, 10h
        push   esi
        lea    esi, [esi - 1ch]
        push   ecx
        mov    ecx, 7
loc_4_0101:
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_010C
        inc    edx
        loop   loc_4_0101
loc_4_010C:
        pop    ecx
        pop    esi
        jb     SHORT loc_4_013E
        test   byte ptr [esp + 10h], 1
        jne    SHORT loc_4_011B
        or     byte ptr [esi + 6], 2
loc_4_011B:
        mov    dword ptr [esi + 0ch], 0fffffffeh
        xor    eax, eax
        mov    dword ptr [esi + 8], eax
        call   sub_4_0540
        mov    dword ptr [esp + 4], esi
        popad
        mov    dword ptr [esi + 3ch], edi
        test   edi, edi
        je     SHORT loc_4_013C
        or     byte ptr [esi + 6], 80h
loc_4_013C:
        clc
        ret
loc_4_013E:
        popad
        stc
        ret
VCD_Unvirtualize_Port_Dynamic:
        pushad
        test   byte ptr [esi + 6], 8
        je     SHORT loc_4_0151
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_0151:
        mov    edx, dword ptr [esi + 18h]
        mov    ecx, 7
loc_4_0159:
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        inc    edx
        loop   loc_4_0159
        push   0
        push   dword ptr [esi]
        db  0CDh, 020h, 015h, 001h, 001h, 000h
        add    esp, 8
        sub    esi, 1ch
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        popad
        ret
VCD_Get_Version:
        mov    eax, 400h
        clc
        ret
VCD_Set_Port_Global:
        push   eax
        db  0F6h, 005h, 008h, 000h, 000h, 000h, 001h
        je     SHORT loc_4_01E5
        db  03Bh, 005h, 034h, 000h, 000h, 000h
        ja     SHORT loc_4_01E5
        sub    eax, 1
        jb     SHORT loc_4_01E6
        shl    eax, 2
        db  003h, 005h, 0DCh, 003h, 000h, 000h
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_01E5
        or     edx, edx
        jne    SHORT loc_4_01C1
        or     byte ptr [eax + 6], 1
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        jmp    SHORT loc_4_01D1
loc_4_01C1:
        and    byte ptr [eax + 6], 0feh
        push   ebx
        mov    ebx, dword ptr [eax + 1ch]
        or     ebx, ebx
        je     SHORT loc_4_01E3
        inc    ebx
        je     SHORT loc_4_01E3
        dec    ebx
loc_4_01D1:
        push   esi
        mov    esi, eax
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        pop    ebx
        call   sub_4_08AD
        pop    esi
loc_4_01E3:
        pop    ebx
        stc
loc_4_01E5:
        cmc
loc_4_01E6:
        pop    eax
        ret
VCD_Set_Port_Global_Special:
        db  080h, 00Dh, 023h, 000h, 000h, 000h, 001h
        db  0CDh, 020h, 001h, 080h, 00Eh, 000h
VCD_Get_Focus:
        push   eax
        db  03Bh, 005h, 034h, 000h, 000h, 000h
        ja     SHORT loc_4_0218
        sub    eax, 1
        jb     SHORT loc_4_0219
        db  08Bh, 01Dh, 0DCh, 003h, 000h, 000h
        mov    ebx, dword ptr [ebx + eax*4]
        or     ebx, ebx
        je     SHORT loc_4_0218
        mov    ebx, dword ptr [ebx + 1ch]
        inc    ebx
        je     SHORT loc_4_0217
        dec    ebx
loc_4_0217:
        stc
loc_4_0218:
        cmc
loc_4_0219:
        pop    eax
        ret
VCD_Acquire_Port:
        pushad
        test   cl, 2
        je     SHORT loc_4_0226
        call   sub_4_02E7
loc_4_0226:
        sub    eax, 1
        jb     NEAR PTR loc_4_02BC
        db  03Bh, 005h, 034h, 000h, 000h, 000h
        jae    NEAR PTR loc_4_02C1
        shl    eax, 2
        db  003h, 005h, 0DCh, 003h, 000h, 000h
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     SHORT loc_4_02C1
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_4_02C1
        mov    ecx, dword ptr [esi + 1ch]
        jecxz  loc_4_02A8
        inc    ecx
        je     SHORT loc_4_02BC
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_4_02A8
        jl     SHORT loc_4_026E
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [esi + 24h]
        cmp    eax, dword ptr [esi + 0ch]
        ja     SHORT loc_4_02A8
loc_4_026E:
        db  0BBh, 027h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebx + 4], eax
        mov    ebx, ecx
        db  068h, 027h, 000h, 000h, 000h
        push   0c0003h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        mov    eax, 0c0001014h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        cmp    al, 6
        jne    SHORT loc_4_02BC
loc_4_02A8:
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        dec    ebx
        mov    dword ptr [esi + 1ch], ebx
        pop    ebx
        mov    dword ptr [esp + 1ch], esi
        popad
        clc
        ret
loc_4_02BC:
        xor    eax, eax
        popad
        stc
        ret
loc_4_02C1:
        test   esp, esp
        popad
        stc
        ret
VCD_Free_Port:
        cmp    dword ptr [eax + 1ch], -1
        jne    SHORT loc_4_02D3
        mov    dword ptr [eax + 1ch], 0
loc_4_02D3:
        pushad
        mov    edx, dword ptr [eax + 18h]
        mov    ecx, 7
loc_4_02DC:
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        inc    edx
        loop   loc_4_02DC
        popad
        ret
sub_4_02E7:
        push   ebx
        push   ecx
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
        xor    ecx, ecx
loc_4_02F1:
        db  03Bh, 00Dh, 034h, 000h, 000h, 000h
        jae    SHORT loc_4_030A
        mov    ebx, dword ptr [edx]
        or     ebx, ebx
        je     SHORT loc_4_0304
        cmp    dword ptr [ebx + 18h], eax
        je     SHORT loc_4_030A
loc_4_0304:
        inc    ecx
        add    edx, 4
        jmp    SHORT loc_4_02F1
loc_4_030A:
        mov    eax, ecx
        inc    eax
        pop    ecx
        pop    ebx
        ret
VCD_Find_COM_Index:
        push   edx
        call   sub_4_02E7
        pop    edx
        db  039h, 005h, 034h, 000h, 000h, 000h
        ret
VCD_Acquire_Port_Windows_Style:
        pushad
        db  00Fh, 0BAh, 035h, 023h, 000h, 000h, 000h, 001h
        jb     SHORT loc_4_0330
        db  080h, 00Dh, 023h, 000h, 000h, 000h, 001h
loc_4_0330:
        test   cl, 2
        je     SHORT loc_4_033A
        call   sub_4_02E7
loc_4_033A:
        sub    eax, 1
        jb     NEAR PTR loc_4_043C
        db  03Bh, 005h, 034h, 000h, 000h, 000h
        jae    NEAR PTR loc_4_043C
        db  08Bh, 035h, 0DCh, 003h, 000h, 000h
        mov    esi, dword ptr [esi + eax*4]
        or     esi, esi
        je     NEAR PTR loc_4_043C
        cmp    dword ptr [esi + 18h], 0
        je     NEAR PTR loc_4_043C
        cmp    byte ptr [esi + 5], 0ffh
        je     NEAR PTR loc_4_043C
        cmp    al, 2
        jae    SHORT loc_4_038D
        test   byte ptr [esi + 6], 1
        je     SHORT loc_4_038D
        cmp    word ptr [eax*2 + 400h], 0
        je     NEAR PTR loc_4_043C
loc_4_038D:
        mov    ecx, dword ptr [esi + 1ch]
        jecxz  loc_4_03CA
        inc    ecx
        je     NEAR PTR loc_4_043E
        dec    ecx
        test   byte ptr [esp + 18h], 1
        je     SHORT loc_4_03A9
        add    ecx, dword ptr [esi]
        or     byte ptr [ecx + 6], 1
        jmp    SHORT loc_4_03CA
loc_4_03A9:
        cmp    dword ptr [esi + 1ch], ebx
        je     SHORT loc_4_03CA
        test   byte ptr [esi + 6], 1
        jne    SHORT loc_4_03CA
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_4_03CA
        jl     SHORT loc_4_0438
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [esi + 24h]
        cmp    eax, dword ptr [esi + 0ch]
        jb     SHORT loc_4_0438
loc_4_03CA:
        cmp    ebx, dword ptr [esi + 1ch]
        jne    SHORT loc_4_03D8
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        pop    ebx
loc_4_03D8:
        mov    edi, ebx
        add    edi, dword ptr [esi]
        mov    byte ptr [edi + 2], 0
        or     byte ptr [edi + 6], 40h
        test   byte ptr [esp + 18h], 4
        je     SHORT loc_4_03F1
        xor    word ptr [edi + 6], 140h
loc_4_03F1:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp]
        test   byte ptr [esp + 18h], 8
        jne    SHORT loc_4_041B
        cmp    dword ptr [esi + 54h], 0
        je     SHORT loc_4_0411
        pushad
        push   0
        push   dword ptr [esi + 58h]
        call   dword ptr [esi + 54h]
        pop    ecx
        pop    ecx
        popad
loc_4_0411:
        mov    dword ptr [esi + 28h], eax
        add    eax, ecx
        mov    dword ptr [esi + 2ch], eax
        jmp    SHORT loc_4_0421
loc_4_041B:
        mov    dword ptr [esi + 54h], eax
        mov    dword ptr [esi + 58h], ecx
loc_4_0421:
        mov    ebx, dword ptr [esp + 10h]
        call   sub_4_08AD
        mov    dword ptr [esp + 1ch], esi
        popad
        db  080h, 025h, 023h, 000h, 000h, 000h, 0FEh
        clc
        ret
loc_4_0438:
        xor    eax, eax
        jmp    SHORT loc_4_043E
loc_4_043C:
        or     ebx, ebx
loc_4_043E:
        popad
        pushfd
        db  080h, 025h, 023h, 000h, 000h, 000h, 0FEh
        popfd
        stc
        ret
sub_4_044A:
        push   edi
        push   ecx
        db  08Bh, 03Dh, 0DCh, 003h, 000h, 000h
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        test   esp, esp
        jecxz  loc_4_045F
        cld
        repne scasd
loc_4_045F:
        pop    ecx
        pop    edi
        ret
VCD_Free_Port_Windows_Style:
        pushad
        call   sub_4_044A
        jne    SHORT loc_4_048A
        mov    esi, eax
        mov    edi, ebx
        add    edi, dword ptr [esi]
        test   word ptr [edi + 6], 140h
        je     SHORT loc_4_048A
        and    word ptr [edi + 6], 0feafh
        cmp    dword ptr [esi + 1ch], ebx
        jne    SHORT loc_4_048A
        xor    ebx, ebx
        call   sub_4_08AD
loc_4_048A:
        popad
        ret
VCD_Steal_Port_Windows_Style:
        mov    ecx, esp
        mov    eax, esi
        call   sub_4_044A
        jne    SHORT loc_4_049C
        call   sub_4_0A85
loc_4_049C:
        xor    eax, eax
        or     ecx, ecx
        jne    SHORT loc_4_04A3
        inc    eax
loc_4_04A3:
        ret
loc_4_04A4:
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        mov    word ptr [ebp + 1ch], ax
        ret
loc_4_04AF:
        pushad
        cmp    edx, 0eh
        jne    SHORT loc_4_050D
        db  03Bh, 035h, 034h, 000h, 000h, 000h
        ja     SHORT loc_4_050D
        sub    esi, 1
        jb     SHORT loc_4_050D
        shl    esi, 2
        db  003h, 035h, 0DCh, 003h, 000h, 000h
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     SHORT loc_4_050D
        mov    edx, ebx
        test   byte ptr [esi + 6], 1
        je     SHORT loc_4_0506
        mov    ebx, dword ptr [esi + 1ch]
        or     ebx, ebx
        je     SHORT loc_4_0506
        inc    ebx
        je     SHORT loc_4_050D
        dec    ebx
        test   byte ptr [esi + 6], 8
        je     SHORT loc_4_0506
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 20h
        je     SHORT loc_4_0506
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        mov    ebx, edx
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_4_0506:
        mov    ebx, edx
        call   sub_4_08AD
loc_4_050D:
        clc
        popad
        ret
loc_4_0510:
        db  08Bh, 03Dh, 0DCh, 003h, 000h, 000h
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        jecxz  loc_4_053E
loc_4_051E:
        push   ecx
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     SHORT loc_4_0538
        cmp    dword ptr [esi + 1ch], ebx
        jne    SHORT loc_4_0538
        test   byte ptr [esi + 6], 8
        je     SHORT loc_4_0538
        mov    eax, dword ptr [esi + 20h]
        call   sub_4_068D
loc_4_0538:
        add    edi, 4
        pop    ecx
        loop   loc_4_051E
loc_4_053E:
        clc
        ret
sub_4_0540:
        push   eax
        db  0A1h, 014h, 004h, 000h, 000h
        mov    dword ptr [esi + 40h], eax
        db  0C7h, 046h, 044h, 0A5h, 005h, 000h, 000h
        db  0A1h, 018h, 004h, 000h, 000h
        mov    dword ptr [esi + 48h], eax
        db  0C7h, 046h, 04Ch, 023h, 003h, 000h, 000h
        db  0C7h, 046h, 050h, 0A5h, 005h, 000h, 000h
        pop    eax
        test   byte ptr [esi + 6], 80h
        je     SHORT loc_4_05A5
        test   word ptr [esi + 6], 141h
        jne    SHORT loc_4_05A5
        pushad
        mov    ecx, 5
        mov    edi, dword ptr [esi + 3ch]
        lea    edi, [edi + 4]
        lea    ebx, [esi + 40h]
loc_4_0584:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_4_058C
        mov    dword ptr [ebx], eax
loc_4_058C:
        add    edi, 4
        add    ebx, 4
        loop   loc_4_0584
        mov    edi, dword ptr [esi + 3ch]
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_4_05A4
        db  0C7h, 046h, 048h, 0A5h, 005h, 000h, 000h
loc_4_05A4:
        popad
loc_4_05A5:
        ret
sub_4_05A6:
        test   byte ptr [esi + 6], 80h
        je     SHORT loc_4_05BE
        test   byte ptr [esi + 7], 1
        jne    SHORT loc_4_05BE
        pushad
        mov    eax, 0
        mov    edi, dword ptr [esi + 3ch]
        call   dword ptr [edi]
        popad
loc_4_05BE:
        ret
sub_4_05BF:
        pushad
        mov    edx, dword ptr [esi + 18h]
        mov    ecx, 7
loc_4_05C8:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    edx
        loop   loc_4_05C8
        popad
        ret
sub_4_05D3:
        pushad
        mov    edx, dword ptr [esi + 18h]
        mov    ecx, 7
        db  08Bh, 02Dh, 010h, 004h, 000h, 000h
        test   word ptr [esi + 6], 140h
        je     SHORT loc_4_05ED
        or     ebp, 0ffffffffh
loc_4_05ED:
        shr    ebp, 1
        jae    SHORT loc_4_05F7
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
loc_4_05F7:
        inc    edx
        loop   loc_4_05ED
        popad
        ret
sub_4_05FC:
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        jecxz  loc_4_0618
        db  08Bh, 035h, 0DCh, 003h, 000h, 000h
loc_4_060A:
        push   esi
        mov    esi, dword ptr [esi]
        call   sub_4_061A
        pop    esi
        add    esi, 4
        loop   loc_4_060A
loc_4_0618:
        clc
        ret
sub_4_061A:
        test   esi, esi
        je     SHORT loc_4_0665
        call   sub_4_0666
        mov    edi, dword ptr [esi]
        lea    edi, [ebx + edi]
        test   byte ptr [esi + 6], 1
        je     SHORT loc_4_065C
        push   ebx
        mov    ebx, dword ptr [esi + 1ch]
        inc    ebx
        je     SHORT loc_4_064F
        dec    ebx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_064F
        push   edi
        lea    edi, [esi + 34h]
        push   eax
        push   edx
        call   sub_4_0767
        pop    edx
        pop    eax
        pop    edi
        pop    ebx
        jmp    SHORT loc_4_065C
loc_4_064F:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        add    ebx, dword ptr [esi]
        lea    esi, [ebx]
        pop    ebx
        jmp    SHORT loc_4_065F
loc_4_065C:
        lea    esi, [esi + 34h]
loc_4_065F:
        cld
        movsd
        movsb
        mov    byte ptr [edi], 0
loc_4_0665:
        ret
sub_4_0666:
        test   byte ptr [esi + 7], 10h
        je     SHORT loc_4_068C
        push   eax
        movzx  eax, byte ptr [esi + 4]
        cmp    al, 4
        ja     SHORT loc_4_068B
        mov    edi, dword ptr [ebx + 4]
        lea    edi, [edi + eax*2 + 3feh]
        cmp    word ptr [edi], 0
        jne    SHORT loc_4_068B
        mov    eax, dword ptr [esi + 18h]
        mov    word ptr [edi], ax
loc_4_068B:
        pop    eax
loc_4_068C:
        ret
sub_4_068D:
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 80h
        je     SHORT loc_4_06A9
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0E8h, 089h, 0FCh, 0FBh, 0FFh
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_4_06A9:
        ret
sub_4_06AA:
        pushad
        test   byte ptr [esi + 6], 80h
        je     SHORT loc_4_06C0
        test   byte ptr [esi + 6], 1
        jne    SHORT loc_4_06C0
        test   word ptr [ecx + ebx + 6], 140h
        je     SHORT loc_4_06F0
loc_4_06C0:
        call   sub_4_05D3
        test   byte ptr [ecx + ebx + 6], 40h
        je     SHORT loc_4_06F0
        test   byte ptr [ecx + ebx + 6], 10h
        jne    SHORT loc_4_06F0
        push   ecx
        mov    ecx, dword ptr [esi + 54h]
        jecxz  loc_4_06E5
        push   1
        push   dword ptr [esi + 58h]
        call   ecx
        pop    ecx
        pop    ecx
        pop    ecx
        jmp    SHORT loc_4_06F0
loc_4_06E5:
        pop    ecx
        mov    eax, dword ptr [esi + 2ch]
        or     eax, eax
        je     SHORT loc_4_06F0
        and    byte ptr [eax], 0feh
loc_4_06F0:
        test   byte ptr [esi + 6], 8
        je     SHORT loc_4_06FF
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
loc_4_06FF:
        mov    dword ptr [esi + 1ch], ebx
        add    ecx, ebx
        mov    edx, dword ptr [esi + 18h]
        add    edx, 3
        mov    al, 80h
        out    dx, al
        jmp    SHORT loc_4_070F
loc_4_070F:
        add    edx, -3
        mov    al, byte ptr [ecx]
        out    dx, al
        jmp    SHORT loc_4_0717
loc_4_0717:
        inc    edx
        mov    al, byte ptr [ecx + 1]
        out    dx, al
        jmp    SHORT loc_4_071E
loc_4_071E:
        add    edx, 2
        mov    al, byte ptr [ecx + 3]
        out    dx, al
        jmp    SHORT loc_4_0727
loc_4_0727:
        inc    edx
        mov    al, byte ptr [ecx + 4]
        out    dx, al
        jmp    SHORT loc_4_072E
loc_4_072E:
        movzx  eax, word ptr [ecx + 6]
        and    eax, 150h
        or     word ptr [esi + 6], ax
        add    edx, -3
        mov    al, byte ptr [ecx + 2]
        out    dx, al
        jmp    SHORT loc_4_0744
loc_4_0744:
        call   sub_4_074B
        popad
        ret
sub_4_074B:
        test   byte ptr [esi + 6], 8
        je     SHORT loc_4_0766
        mov    eax, dword ptr [esi + 20h]
        cmp    dword ptr [esi + 54h], 0
        je     SHORT loc_4_0760
        db  0CDh, 020h, 009h, 080h, 003h, 000h
loc_4_0760:
        db  0CDh, 020h, 00Ah, 080h, 003h, 000h
loc_4_0766:
        ret
sub_4_0767:
        mov    edx, dword ptr [esi + 18h]
        add    edx, 3
        in     al, dx
        mov    byte ptr [edi + 3], al
        or     al, 80h
        out    dx, al
        jmp    SHORT loc_4_0776
loc_4_0776:
        add    dl, 0fdh
        in     al, dx
        mov    byte ptr [edi], al
        jmp    SHORT loc_4_077E
loc_4_077E:
        inc    edx
        in     al, dx
        jmp    SHORT loc_4_0782
loc_4_0782:
        mov    byte ptr [edi + 1], al
        add    dl, 2
        mov    al, byte ptr [edi + 3]
        out    dx, al
        sub    dl, 2
        in     al, dx
        jmp    SHORT loc_4_0792
loc_4_0792:
        mov    byte ptr [edi + 2], al
        add    dl, 3
        in     al, dx
        mov    byte ptr [edi + 4], al
        ret
sub_4_079D:
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_4_07F8
        cmp    dword ptr [esi + 1ch], -1
        je     SHORT loc_4_07F8
        pushad
        mov    ebx, dword ptr [esi + 1ch]
        mov    edi, ebx
        add    edi, ecx
        test   byte ptr [esi + 6], 40h
        je     SHORT loc_4_07D7
        test   byte ptr [esi + 6], 10h
        jne    SHORT loc_4_07D7
        mov    ecx, dword ptr [esi + 54h]
        jecxz  loc_4_07CD
        push   0
        push   dword ptr [esi + 58h]
        call   ecx
        pop    ecx
        pop    ecx
        jmp    SHORT loc_4_07D7
loc_4_07CD:
        mov    eax, dword ptr [esi + 2ch]
        or     eax, eax
        je     SHORT loc_4_07D7
        or     byte ptr [eax], 1
loc_4_07D7:
        call   sub_4_05BF
        call   sub_4_0767
        mov    edx, dword ptr [esi + 18h]
        inc    edx
        xor    al, al
        out    dx, al
        jmp    SHORT loc_4_07EA
loc_4_07EA:
        inc    edx
        out    dx, al
        xor    ebx, ebx
        xchg   dword ptr [esi + 1ch], ebx
        and    word ptr [esi + 6], 0feafh
        popad
loc_4_07F8:
        ret
sub_4_07F9:
        test   byte ptr [esi + 6], 4
        jne    NEAR PTR loc_4_089B
        push   ecx
        sub    esp, 20h
        mov    edi, esp
        movzx  eax, byte ptr [esi + 5]
        mov    word ptr [edi], ax
        xor    ecx, ecx
        test   byte ptr [esi + 6], 2
        jne    SHORT loc_4_081B
        or     cl, 2
loc_4_081B:
        mov    word ptr [edi + 2], cx
        lea    eax, [esi - 7]
        lea    edi, [edi + 4]
        cld
        mov    ecx, 5
loc_4_082B:
        stosd
        sub    eax, 4
        loop   loc_4_082B
        mov    edi, esp
        mov    dword ptr [edi + 18h], 1f4h
        xor    eax, eax
        mov    ecx, dword ptr [esi + 3ch]
        jecxz  loc_4_0853
        cmp    dword ptr [ecx + 8], eax
        jne    SHORT loc_4_084B
        mov    dword ptr [edi + 8], eax
loc_4_084B:
        cmp    dword ptr [ecx + 14h], eax
        jne    SHORT loc_4_0853
        mov    dword ptr [edi + 14h], eax
loc_4_0853:
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_4_0893
        mov    dword ptr [esi + 20h], eax
        or     byte ptr [esi + 6], 8
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 8
        je     SHORT loc_4_0873
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_4_0873:
        test   byte ptr [esi + 6], 2
        jne    SHORT loc_4_0882
        test   cl, 4
        jne    SHORT loc_4_0882
        or     byte ptr [esi + 7], 4
loc_4_0882:
        movzx  eax, byte ptr [esi + 5]
        db  00Fh, 0ABh, 005h, 024h, 004h, 000h, 000h
        jb     SHORT loc_4_0893
        or     byte ptr [esi + 7], 2
loc_4_0893:
        or     byte ptr [esi + 6], 4
        add    esp, 20h
        pop    ecx
loc_4_089B:
        ret
loc_4_089C:
        mov    esi, edx
        xor    ebx, ebx
        xchg   dword ptr [esi + 1ch], ebx
        call   sub_4_08AD
        and    byte ptr [esi + 6], 0dfh
        ret
sub_4_08AD:
        pushad
        test   esi, esi
        je     NEAR PTR loc_4_099D
        mov    ecx, dword ptr [esi]
        cmp    ebx, dword ptr [esi + 1ch]
        je     NEAR PTR loc_4_098E
        push   dword ptr [esi + 1ch]
        call   sub_4_079D
        or     ebx, ebx
        jne    NEAR PTR loc_4_096F
        mov    edx, dword ptr [esi + 18h]
        test   byte ptr [esp + 5], 2
        jne    SHORT loc_4_08EC
        inc    edx
        xor    al, al
        out    dx, al
        jmp    SHORT loc_4_08E1
loc_4_08E1:
        add    dl, 3
        in     al, dx
        jmp    SHORT loc_4_08E7
loc_4_08E7:
        and    al, 3
        out    dx, al
        jmp    SHORT loc_4_0967
loc_4_08EC:
        add    dl, 4
        in     al, dx
        test   al, 8
        je     SHORT loc_4_0967
        dec    ebx
        mov    dword ptr [esi + 1ch], ebx
        inc    ebx
        test   byte ptr [esi + 6], 8
        je     SHORT loc_4_0908
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
loc_4_0908:
        db  0A1h, 014h, 004h, 000h, 000h
        mov    dword ptr [esi + 40h], eax
        mov    al, 1
        sub    dl, 3
        out    dx, al
        add    dl, 4
        jmp    SHORT loc_4_091B
loc_4_091B:
        in     al, dx
        jmp    SHORT loc_4_091E
loc_4_091E:
        and    al, 60h
        cmp    al, 60h
        jne    SHORT loc_4_091B
        dec    edx
        in     al, dx
        jmp    SHORT loc_4_0928
loc_4_0928:
        mov    ah, al
        or     al, 10h
        out    dx, al
        jmp    SHORT loc_4_092F
loc_4_092F:
        sub    dl, 4
        xor    al, al
        out    dx, al
        jmp    SHORT loc_4_0937
loc_4_0937:
        add    dl, 5
loc_4_093A:
        in     al, dx
        jmp    SHORT loc_4_093D
loc_4_093D:
        and    al, 60h
        cmp    al, 60h
        jne    SHORT loc_4_093A
        dec    edx
        mov    al, ah
        and    al, 3
        out    dx, al
        test   byte ptr [esi + 6], 8
        je     SHORT loc_4_0958
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_4_0958:
        sti
        sub    dl, 3
        xor    eax, eax
        out    dx, al
        mov    dword ptr [esi + 1ch], eax
        call   sub_4_074B
loc_4_0967:
        pop    edx
        call   sub_4_05A6
        jmp    SHORT loc_4_099D
loc_4_096F:
        db  00Fh, 0BAh, 035h, 023h, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_097E
        call   sub_4_07F9
loc_4_097E:
        call   sub_4_06AA
        call   sub_4_0540
        pop    edx
        call   sub_4_05A6
loc_4_098E:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
        db  0FFh, 015h, 0D0h, 003h, 000h, 000h
loc_4_099D:
        popad
        ret
loc_4_099F:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 7], 4
        jne    SHORT loc_4_09B9
        push   0
        push   8
        push   dword ptr [eax + 64h]
        db  0CDh, 020h, 064h, 000h, 033h, 000h
        add    esp, 0ch
loc_4_09B9:
        ret
sub_4_09BA:
        enter  0ch, 0
        push   eax
        push   ebx
        push   ecx
        push   edx
        xor    ebx, ebx
        mov    dword ptr [ebp - 0ch], 4
        lea    ecx, [ebp - 4]
        push   ecx
        db  068h, 038h, 000h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    ebx, eax
        jb     SHORT loc_4_0A1B
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   eax
        push   0
        push   0
        db  068h, 064h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        cmp    ebx, eax
        jb     SHORT loc_4_0A0B
        cmp    dword ptr [ebp - 8], 1
loc_4_0A0B:
        pushfd
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        or     eax, eax
        popfd
loc_4_0A1B:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        leave
        ret
loc_4_0A21:
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        jecxz  loc_4_0A3D
        db  08Bh, 035h, 0DCh, 003h, 000h, 000h
loc_4_0A2F:
        push   esi
        mov    esi, dword ptr [esi]
        call   sub_4_0A3F
        pop    esi
        add    esi, 4
        loop   loc_4_0A2F
loc_4_0A3D:
        clc
        ret
sub_4_0A3F:
        test   esi, esi
        je     SHORT loc_4_0A64
        cmp    ebx, dword ptr [esi + 1ch]
        jne    SHORT loc_4_0A64
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        cmp    dword ptr [esi + 54h], 0
        je     SHORT loc_4_0A65
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ecx
        call   sub_4_0A85
        pop    ecx
        pop    ebx
loc_4_0A64:
        ret
loc_4_0A65:
        pushad
        call   sub_4_09BA
        jb     SHORT loc_4_0A82
        and    byte ptr [esi + 7], 0fbh
        push   0
        push   esi
        db  068h, 09Fh, 009h, 000h, 000h
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
loc_4_0A82:
        popad
        pop    ebx
        ret
sub_4_0A85:
        pushad
        cmp    dword ptr [esi + 1ch], ebx
        je     SHORT loc_4_0ABE
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_4_0AB9
        cmp    dword ptr [esi + 1ch], -1
        je     SHORT loc_4_0B13
        test   byte ptr [esi + 6], 1
        jne    SHORT loc_4_0B0C
        cmp    dword ptr [esi + 0ch], -2
        je     SHORT loc_4_0B0C
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_4_0AB9
        jl     SHORT loc_4_0AC2
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [esi + 24h]
        cmp    eax, dword ptr [esi + 0ch]
        jb     SHORT loc_4_0AC2
loc_4_0AB9:
        call   sub_4_08AD
loc_4_0ABE:
        popad
        xor    ecx, ecx
        ret
loc_4_0AC2:
        mov    eax, dword ptr [esi]
        add    eax, ebx
        bts    word ptr [eax + 6], 0
        jb     SHORT loc_4_0B0C
        mov    eax, dword ptr [esi + 1ch]
        push   esi
        lea    esi, [esi + 10h]
        db  0CDh, 020h, 001h, 000h, 017h, 000h
        pop    esi
        jae    SHORT loc_4_0AE0
        mov    ebx, eax
loc_4_0AE0:
        cmp    ebx, eax
        je     SHORT loc_4_0AF6
        movzx  ecx, byte ptr [esi + 4]
        mov    edx, dword ptr [esi]
        or     byte ptr [eax + edx + 6], 1
        and    byte ptr [ebx + edx + 6], 0feh
        jmp    SHORT loc_4_0AB9
loc_4_0AF6:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
        db  0FFh, 015h, 0D0h, 003h, 000h, 000h
        popad
        mov    ecx, 1
        ret
loc_4_0B0C:
        popad
        mov    ecx, 2
        ret
loc_4_0B13:
        popad
        mov    ecx, 3
        ret
loc_4_0B1A:
        movzx  edx, word ptr [ebp + 14h]
        db  03Bh, 015h, 034h, 000h, 000h, 000h
        jae    SHORT loc_4_0B41
        db  08Bh, 035h, 0DCh, 003h, 000h, 000h
        mov    esi, dword ptr [esi + edx*4]
        test   esi, esi
        je     SHORT loc_4_0B41
        cmp    ebx, dword ptr [esi + 1ch]
        jne    SHORT loc_4_0B41
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
loc_4_0B41:
        stc
        ret
loc_4_0B43:
        db  0B8h, 04Ah, 00Bh, 000h, 000h
        clc
        ret
loc_4_0B4A:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 5
        ja     SHORT loc_4_0B5A
        db  0FFh, 024h, 085h, 07Ah, 000h, 000h, 000h
loc_4_0B5A:
        ret
loc_4_0B5B:
        push   esi
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        jecxz  loc_4_0B7A
        mov    eax, dword ptr [esp + 14h]
loc_4_0B6E:
        mov    esi, dword ptr [edx]
        cmp    eax, dword ptr [esi + 18h]
        je     SHORT loc_4_0B7C
        lea    edx, [edx + 4]
        loop   loc_4_0B6E
loc_4_0B7A:
        xor    esi, esi
loc_4_0B7C:
        mov    eax, esi
        pop    esi
        ret
sub_4_0B80:
        db  0BFh, 011h, 000h, 000h, 000h
        db  0B8h, 00Dh, 000h, 000h, 000h
        mov    dword ptr [eax], 9
        push   0
        push   eax
        push   edi
        push   1
        db  068h, 01Ah, 000h, 000h, 000h
        push   0
        push   ecx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_0BE5
        mov    eax, dword ptr [edi]
        sub    eax, 304d4f43h
        jb     SHORT loc_4_0BE5
        test   eax, 0ffffffh
        jne    SHORT loc_4_0BE5
        rol    eax, 8
        cmp    al, 9
        ja     SHORT loc_4_0BE5
        cmp    al, 1
        jb     SHORT loc_4_0BE5
        movzx  ecx, byte ptr [edi + 4]
        test   ecx, ecx
        je     SHORT loc_4_0BE5
        cmp    byte ptr [edi + 5], 0
        jne    SHORT loc_4_0BE5
        imul   eax, eax, 0ah
        sub    cl, 30h
        jb     SHORT loc_4_0BE5
        cmp    cl, 9
        ja     SHORT loc_4_0BE5
        add    eax, ecx
        xor    ecx, ecx
loc_4_0BE5:
        ret
sub_4_0BE6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        je     SHORT loc_4_0C41
        test   byte ptr [esi + 7], 9
        jne    SHORT loc_4_0C41
        mov    ecx, dword ptr [esi + 54h]
        jecxz  loc_4_0C27
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_4_0C41
        push   0
        push   dword ptr [esi + 58h]
        call   ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_0C43
        cmp    dword ptr [esi + 54h], 0
        je     SHORT loc_4_0C27
        mov    eax, dword ptr [esi + 54h]
        mov    edx, dword ptr [esi + 58h]
        mov    dword ptr [esi + 5ch], eax
        mov    dword ptr [esi + 60h], edx
        or     byte ptr [esi + 7], 8
loc_4_0C27:
        mov    eax, dword ptr [esi + 18h]
        push   0ah
        pop    ecx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 14h]
        db  0CDh, 020h, 006h, 000h, 00Eh, 000h
        jae    SHORT loc_4_0C43
loc_4_0C41:
        xor    eax, eax
loc_4_0C43:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_4_0C48:
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 14h]
        call   sub_4_044A
        jne    SHORT loc_4_0CAC
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_4_0CAC
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 5ch], eax
        jne    SHORT loc_4_0C91
        cmp    ebx, dword ptr [esi + 1ch]
        jne    SHORT loc_4_0C82
        push   0
        push   dword ptr [esi + 58h]
        call   dword ptr [esi + 54h]
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_0CB2
        mov    eax, dword ptr [esp + 18h]
loc_4_0C82:
        xchg   dword ptr [esi + 54h], eax
        xchg   dword ptr [esi + 5ch], eax
        mov    eax, dword ptr [esi + 58h]
        xchg   dword ptr [esi + 60h], eax
        mov    dword ptr [esi + 58h], eax
loc_4_0C91:
        cmp    ebx, dword ptr [esi + 1ch]
        je     SHORT loc_4_0CB0
        db  080h, 00Dh, 023h, 000h, 000h, 000h, 001h
        db  0CDh, 020h, 008h, 000h, 00Eh, 000h
        db  080h, 025h, 023h, 000h, 000h, 000h, 0FEh
        jmp    SHORT loc_4_0CB2
loc_4_0CAC:
        xor    eax, eax
        jmp    SHORT loc_4_0CB2
loc_4_0CB0:
        or     al, 1
loc_4_0CB2:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_4_0CB6:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 10h]
        call   sub_4_044A
        jne    SHORT loc_4_0D0C
        mov    esi, eax
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 5ch], eax
        je     SHORT loc_4_0D08
        mov    eax, esi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 007h, 000h, 00Eh, 000h
        mov    dword ptr [esi + 54h], 0
        btr    word ptr [esi + 6], 0bh
        jae    SHORT loc_4_0D0C
        push   0
        push   dword ptr [esi + 60h]
        push   dword ptr [esi + 5ch]
        push   esi
        push   1
        call   sub_4_0BE6
        add    esp, 14h
        push   1
        push   dword ptr [esi + 58h]
        call   dword ptr [esi + 54h]
        pop    ecx
        pop    ecx
loc_4_0D08:
        and    byte ptr [esi + 7], 0f7h
loc_4_0D0C:
        pop    esi
        pop    ebx
        ret
        db  000h
_PTEXT ends
        end
