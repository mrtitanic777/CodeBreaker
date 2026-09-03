; serwave_lcode_01.asm -- SERWAVE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  08Bh, 044h, 024h, 004h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 040h, 008h, 000h, 000h, 000h
        db  000h, 0C2h, 004h, 000h
loc_1_0014:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 8], 0
        push   2
        push   dword ptr [esp + 4]
        call   sub_1_0035
        add    esp, 4
        ret
sub_1_0035:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi], 0ffffffffh
        mov    dword ptr [esi + 4], eax
        push   dword ptr [esi + 8]
        call   sub_1_1744
        mov    dword ptr [esi + 8], 0
        push   56415753h
        call   sub_1_175C
        add    esp, 4
        pop    esi
        ret    8
sub_1_0067:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_171C
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 8], eax
        test   ah, 2
        jne    SHORT loc_1_0096
        int3
        mov    ecx, dword ptr [ebp - 8]
        or     ecx, 200h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2ch], ecx
loc_1_0096:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_00EA
        push   esi
        push   dword ptr [ebp + 0ch]
        db  068h, 014h, 000h, 000h, 000h
        call   sub_1_1728
        mov    dword ptr [esi + 8], eax
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_1_00BC
        mov    eax, 2
        jmp    SHORT loc_1_00ED
loc_1_00BC:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_00D5
loc_1_00C1:
        push   1
        push   56415753h
        call   sub_1_1754
        add    esp, 8
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_00C1
loc_1_00D5:
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_00EA
        push   dword ptr [esi + 8]
        call   sub_1_1744
        mov    dword ptr [esi + 8], 0
loc_1_00EA:
        mov    eax, dword ptr [esi + 4]
loc_1_00ED:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  000h, 000h
SERWAVE_Control:
        cmp    eax, 1bh
        je     SHORT loc_1_0109
        cmp    eax, 1ch
        je     SHORT loc_1_010B
        cmp    eax, 22h
        je     SHORT loc_1_0134
        clc
        ret
loc_1_0109:
        clc
        ret
loc_1_010B:
        clc
        ret
SERWAVE_PM_API:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    eax, 11h
        jg     SHORT loc_1_012C
        push   dword ptr [ebp + 4]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        db  0FFh, 014h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        ret
loc_1_012C:
        mov    dword ptr [ebp + 1ch], 0ffffffffh
        ret
loc_1_0134:
        cmp    edx, 0
        jne    SHORT loc_1_014C
        push   0
        db  068h, 0E7h, 003h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Fh, 000h, 033h, 000h
        add    esp, 0ch
        stc
        ret
loc_1_014C:
        mov    eax, ebx
        db  0BBh, 060h, 001h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
loc_1_0160:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 0E9h, 0FFh, 0FEh, 0FFh
        add    esp, 14h
        leave
        clc
        ret
        db  000h, 000h, 000h
sub_1_0180:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        movzx  edx, ax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        shr    eax, 10h
        push   edx
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        add    eax, edx
        pop    ebx
        leave
        ret    4
sub_1_01A9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ecx
        mov    eax, 1000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ecx, 1
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0ffffffffh
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    eax, esi
        pop    ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_01DB:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        pop    esi
        leave
        ret    4
sub_1_01ED:
        push   ebp
        mov    ebp, esp
        push   ecx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        and    dword ptr [eax + 2ch], 0fffffbffh
        mov    ax, word ptr [ebp + 0eh]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 0ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 16h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 14h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 1ah]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    cx, word ptr [ebp + 0ah]
        movzx  edx, word ptr [ebp + 8]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ecx
        leave
        ret    14h
sub_1_028A:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ecx
        cld
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [ebp + 0ch]
        repne scasb
        jne    SHORT loc_1_02A6
        dec    edi
        mov    eax, edi
        pop    ecx
        pop    edi
        leave
        ret    0ch
loc_1_02A6:
        xor    eax, eax
        pop    ecx
        pop    edi
        leave
        ret    0ch
        db  000h, 000h
loc_1_02B0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   1
        push   dword ptr [esi + 10h]
        call   sub_1_170C
        add    esp, 8
        mov    dword ptr [esi + 0fch], eax
        or     eax, eax
        jne    SHORT loc_1_02D3
        jmp    NEAR PTR loc_1_0421
loc_1_02D3:
        mov    dword ptr [esi + 80h], 1
        mov    dword ptr [esi + 84h], 0
        push   dword ptr [esp + 14h]
        call   sub_1_0180
        mov    dword ptr [esi + 88h], eax
        lea    edi, [esi + 78h]
        push   0
        push   edi
        push   0c0000002h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_0411
        cmp    dword ptr [esi + 7ch], 0
        jne    NEAR PTR loc_1_0411
        test   byte ptr [esi + 1eh], 8
        je     SHORT loc_1_0334
        push   0
        mov    ebx, dword ptr [esp + 1ch]
        push   0
        push   ebx
        push   esi
        db  0E8h, 0A4h, 0FDh, 0FEh, 0FFh
        jmp    SHORT loc_1_0338
loc_1_0334:
        mov    ebx, dword ptr [esp + 18h]
loc_1_0338:
        mov    dword ptr [esi + 80h], 1
        mov    dword ptr [esi + 84h], 0
        push   0
        push   edi
        push   0c0000003h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_03EB
        cmp    dword ptr [esi + 7ch], 0
        jne    SHORT loc_1_03EB
        test   byte ptr [esi + 1eh], 8
        jne    SHORT loc_1_037D
        push   0
        push   0
        push   ebx
        push   esi
        db  0E8h, 059h, 0FDh, 0FEh, 0FFh
loc_1_037D:
        mov    byte ptr [esi + 104h], 0
        mov    dword ptr [esi + 100h], 0
        mov    dword ptr [esi + 0f8h], 1
        lea    eax, [esi + 13dh]
        mov    dword ptr [esi + 12dh], eax
        mov    dword ptr [esi + 119h], 200h
        push   esi
        db  068h, 0CAh, 004h, 000h, 000h
        mov    eax, dword ptr [esi + 10h]
        shr    eax, 3
        push   eax
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_03CB
        jmp    SHORT loc_1_0411
loc_1_03CB:
        push   esi
        push   7d0h
        db  068h, 019h, 005h, 000h, 000h
        call   sub_1_1728
        mov    dword ptr [esi + 109h], eax
        push   esi
        call   sub_1_0632
        sub    eax, eax
        jmp    SHORT loc_1_0426
loc_1_03EB:
        mov    dword ptr [esi + 80h], 1
        mov    dword ptr [esi + 84h], 0
        push   0
        push   edi
        push   0c0000004h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
loc_1_0411:
        push   0
        push   dword ptr [esi + 0fch]
        call   sub_1_1714
        add    esp, 8
loc_1_0421:
        mov    eax, 2
loc_1_0426:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_042C:
        push   esi
        push   0
        push   0
        mov    esi, dword ptr [esp + 10h]
        push   -1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 10h
        push   dword ptr [esi + 109h]
        call   sub_1_1744
        mov    dword ptr [esi + 109h], 0
        push   dword ptr [esi + 14h]
        call   sub_1_01DB
        test   byte ptr [esi + 1eh], 10h
        je     SHORT loc_1_0477
        push   5fh
        lea    eax, [esi + 0b4h]
        push   eax
        push   dword ptr [esi]
        call   sub_1_16C4
        add    esp, 0ch
loc_1_0477:
        mov    dword ptr [esi + 80h], 1
        mov    dword ptr [esi + 84h], 0
        push   0
        lea    eax, [esi + 78h]
        push   eax
        push   0c0000004h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
        push   0
        push   dword ptr [esi + 0fch]
        call   sub_1_1714
        add    esp, 8
        mov    dword ptr [esi + 0f8h], 0
        mov    dword ptr [esi + 0fch], 0
        sub    eax, eax
        pop    esi
        ret    10h
loc_1_04CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_1_04EB
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        popfd
        jmp    SHORT loc_1_0514
loc_1_04EB:
        mov    dword ptr [esi + 18h], 0ffffffffh
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        popfd
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_050D
        push   esi
        db  068h, 0ADh, 005h, 000h, 000h
        call   sub_1_01A9
        mov    dword ptr [esi + 14h], eax
loc_1_050D:
        mov    dword ptr [esi + 18h], 0
loc_1_0514:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0519:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    dword ptr [ebp - 4], edx
        pushfd
        pop    dword ptr [ebp - 8]
        cli
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 18h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_055C
        mov    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        popfd
        push   dword ptr [ebp - 4]
        push   7d0h
        db  068h, 019h, 005h, 000h, 000h
        call   sub_1_1728
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 109h], eax
        jmp    SHORT loc_1_05A8
loc_1_055C:
        mov    dword ptr [eax], 0ffffffffh
        mov    dword ptr [ebp - 0ch], ecx
        push   dword ptr [ebp - 0ch]
        popfd
        mov    esi, dword ptr [ebp - 4]
        add    esi, 14h
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_0583
        push   dword ptr [ebp - 4]
        db  068h, 0ADh, 005h, 000h, 000h
        call   sub_1_01A9
        mov    dword ptr [esi], eax
loc_1_0583:
        push   dword ptr [ebp - 4]
        push   7d0h
        db  068h, 019h, 005h, 000h, 000h
        call   sub_1_1728
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 109h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
loc_1_05A8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_05AD:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 14h], 0
        push   dword ptr [esp]
        call   sub_1_0699
        push   dword ptr [esp]
        call   sub_1_0632
        add    esp, 4
        ret
loc_1_05D5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_1_0180
        mov    edi, eax
        mov    dword ptr [edi + 10h], esi
        push   dword ptr [edi]
        call   sub_1_0180
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 14h], 0
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edi + 0ch], 0
        cmp    dword ptr [eax + 0e4h], 0
        jne    SHORT loc_1_0613
        mov    dword ptr [eax + 0e4h], edi
        jmp    SHORT loc_1_061C
loc_1_0613:
        mov    ecx, dword ptr [eax + 0e8h]
        mov    dword ptr [ecx + 0ch], edi
loc_1_061C:
        mov    dword ptr [eax + 0e8h], edi
        push   eax
        call   sub_1_0632
        mov    eax, 1
        pop    edi
        pop    esi
        ret    10h
sub_1_0632:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0f8h], 0
        je     SHORT loc_1_0694
        cmp    dword ptr [esi + 0f4h], 0
        jne    SHORT loc_1_068E
        sub    edi, edi
loc_1_064C:
        mov    eax, dword ptr [esi + 0e4h]
        cmp    eax, edi
        je     SHORT loc_1_065F
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0e4h], ecx
loc_1_065F:
        mov    dword ptr [esi + 0f4h], eax
        cmp    eax, edi
        jne    SHORT loc_1_0680
        cmp    dword ptr [esi + 10dh], edi
        je     SHORT loc_1_068E
        lea    eax, [esi + 115h]
        mov    dword ptr [esi + 0f4h], eax
        mov    dword ptr [eax + 14h], edi
loc_1_0680:
        push   esi
        call   sub_1_0699
        cmp    dword ptr [esi + 0f4h], edi
        je     SHORT loc_1_064C
loc_1_068E:
        push   esi
        call   sub_1_092A
loc_1_0694:
        pop    edi
        pop    esi
        ret    4
sub_1_0699:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esi + 0f4h]
        or     ecx, ecx
        mov    dword ptr [esp + 10h], ecx
        je     NEAR PTR loc_1_0920
        cmp    dword ptr [esi + 139h], 0
        jne    NEAR PTR loc_1_0920
        mov    dword ptr [esi + 139h], 1
        mov    eax, dword ptr [ecx + 4]
        cmp    dword ptr [ecx + 14h], eax
        jae    NEAR PTR loc_1_0879
loc_1_06D9:
        mov    ecx, dword ptr [esp + 10h]
        movzx  eax, byte ptr [esi + 104h]
        mov    edx, dword ptr [ecx + 14h]
        mov    ebp, dword ptr [ecx + 18h]
        add    ebp, edx
        or     eax, eax
        je     SHORT loc_1_06FE
        cmp    eax, 4
        je     NEAR PTR loc_1_07EA
        jmp    NEAR PTR loc_1_0869
loc_1_06FE:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [ecx + 4]
        sub    ecx, edx
        cmp    ecx, eax
        jbe    SHORT loc_1_0710
        mov    ecx, eax
loc_1_0710:
        lea    eax, [esp + 14h]
        push   eax
        push   ecx
        push   dword ptr [esi + 0fch]
        push   dword ptr [esi]
        call   sub_1_097C
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_08A2
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_1_0916
        mov    edi, dword ptr [esi + 0fch]
        mov    eax, dword ptr [esp + 14h]
        add    eax, edi
        cmp    eax, edi
        jbe    NEAR PTR loc_1_0869
loc_1_074D:
        mov    al, byte ptr [edi]
        cmp    al, 10h
        jne    SHORT loc_1_07BE
        mov    eax, dword ptr [esi + 0fch]
        sub    eax, edi
        add    eax, dword ptr [esp + 14h]
        cmp    eax, 1
        je     SHORT loc_1_07E1
        mov    al, byte ptr [edi + 1]
        lea    ebx, [edi + 1]
        cmp    al, 10h
        je     SHORT loc_1_07B6
        cmp    al, 3
        jne    SHORT loc_1_0787
        mov    dword ptr [esi + 0f8h], 0
        push   0
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_0035
loc_1_0787:
        mov    dword ptr [esp + 1ch], 2
        mov    al, byte ptr [edi]
        mov    byte ptr [esp + 24h], al
        mov    al, byte ptr [ebx]
        mov    byte ptr [esp + 25h], al
        lea    eax, [esp + 18h]
        push   0
        add    edi, 2
        push   eax
        push   0c0000000h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
        jmp    SHORT loc_1_07CA
loc_1_07B6:
        mov    byte ptr [ebp], al
        lea    edi, [ebx + 1]
        jmp    SHORT loc_1_07C2
loc_1_07BE:
        mov    byte ptr [ebp], al
        inc    edi
loc_1_07C2:
        mov    ecx, dword ptr [esp + 10h]
        inc    ebp
        inc    dword ptr [ecx + 14h]
loc_1_07CA:
        mov    eax, dword ptr [esi + 0fch]
        add    eax, dword ptr [esp + 14h]
        cmp    eax, edi
        ja     NEAR PTR loc_1_074D
        jmp    NEAR PTR loc_1_0869
loc_1_07E1:
        mov    byte ptr [esi + 104h], 4
        jmp    SHORT loc_1_0869
loc_1_07EA:
        lea    eax, [esp + 14h]
        push   eax
        push   1
        push   ebp
        push   dword ptr [esi]
        call   sub_1_097C
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_089B
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_1_0916
        mov    al, byte ptr [ebp]
        cmp    al, 10h
        jne    SHORT loc_1_081F
        mov    ecx, dword ptr [esp + 10h]
        inc    dword ptr [ecx + 14h]
        jmp    SHORT loc_1_0862
loc_1_081F:
        cmp    al, 3
        jne    SHORT loc_1_0838
        mov    dword ptr [esi + 0f8h], 0
        push   0
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_0035
loc_1_0838:
        mov    dword ptr [esp + 2ch], 2
        mov    byte ptr [esp + 34h], 10h
        mov    al, byte ptr [ebp]
        mov    byte ptr [esp + 35h], al
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   0c0000000h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
loc_1_0862:
        mov    byte ptr [esi + 104h], 0
loc_1_0869:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 4]
        cmp    dword ptr [ecx + 14h], eax
        jb     NEAR PTR loc_1_06D9
loc_1_0879:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 14h]
        cmp    dword ptr [ecx + 4], eax
        jne    NEAR PTR loc_1_0916
        mov    ecx, dword ptr [esi + 0f4h]
        sub    ecx, esi
        cmp    ecx, 115h
        jne    SHORT loc_1_08E1
        jmp    SHORT loc_1_090C
loc_1_089B:
        mov    byte ptr [esi + 104h], 0
loc_1_08A2:
        mov    edx, dword ptr [esi + 0f4h]
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 115h
        jne    SHORT loc_1_08B5
        jmp    SHORT loc_1_090C
loc_1_08B5:
        mov    ecx, dword ptr [esp + 10h]
        lea    eax, [esi + 0ech]
        mov    dword ptr [ecx + 14h], 0
        mov    dword ptr [edx + 0ch], 0
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_08D6
        mov    dword ptr [eax], edx
        jmp    SHORT loc_1_08DC
loc_1_08D6:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0ch], edx
loc_1_08DC:
        mov    dword ptr [eax + 4], edx
        jmp    SHORT loc_1_090C
loc_1_08E1:
        add    dword ptr [esi + 100h], eax
        mov    eax, dword ptr [esi + 0f4h]
        lea    ecx, [esi + 0ech]
        mov    dword ptr [eax + 0ch], 0
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_0903
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_0909
loc_1_0903:
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0ch], eax
loc_1_0909:
        mov    dword ptr [ecx + 4], eax
loc_1_090C:
        mov    dword ptr [esi + 0f4h], 0
loc_1_0916:
        mov    dword ptr [esi + 139h], 0
loc_1_0920:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret    4
sub_1_092A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 135h], 1
        je     SHORT loc_1_0978
        mov    dword ptr [esi + 135h], 1
loc_1_0942:
        mov    eax, dword ptr [esi + 0ech]
        or     eax, eax
        je     SHORT loc_1_096E
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0ech], ecx
        or     eax, eax
        je     SHORT loc_1_096E
        push   dword ptr [eax + 14h]
        push   dword ptr [eax + 10h]
        push   2
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   sub_1_01ED
        jmp    SHORT loc_1_0942
loc_1_096E:
        mov    dword ptr [esi + 135h], 0
loc_1_0978:
        pop    esi
        ret    4
sub_1_097C:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 2ch]
        mov    edi, dword ptr [esp + 24h]
        mov    ebx, dword ptr [esp + 30h]
loc_1_098F:
        push   ebx
        push   esi
        sub    ebp, ebp
        push   dword ptr [esp + 30h]
        push   edi
        call   sub_1_16F4
        add    esp, 10h
        cmp    eax, ebp
        je     SHORT loc_1_09CE
        cmp    dword ptr [ebx], ebp
        jne    SHORT loc_1_09CA
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   edi
        call   sub_1_16D4
        add    esp, 0ch
        cmp    eax, ebp
        je     SHORT loc_1_09CE
        cmp    dword ptr [esp + 18h], ebp
        je     SHORT loc_1_09CA
        mov    ebp, 1
loc_1_09CA:
        or     ebp, ebp
        jne    SHORT loc_1_098F
loc_1_09CE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_09D6:
        push   esi
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_0A48
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esi + 0f4h]
        or     edx, edx
        je     NEAR PTR loc_1_0B19
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 115h
        jne    SHORT loc_1_0A14
        mov    dword ptr [esi + 0f4h], 0
        mov    dword ptr [esi + 10dh], 0
        jmp    NEAR PTR loc_1_0B19
loc_1_0A14:
        lea    eax, [esi + 0ech]
        mov    dword ptr [edx + 0ch], 0
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_0A2A
        mov    dword ptr [eax], edx
        jmp    SHORT loc_1_0A30
loc_1_0A2A:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0ch], edx
loc_1_0A30:
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [esi + 0f4h], 0
        push   esi
        call   sub_1_092A
        jmp    NEAR PTR loc_1_0B19
loc_1_0A48:
        mov    esi, dword ptr [esp + 8]
        push   1
        push   dword ptr [esi]
        call   sub_1_16E4
        add    esp, 8
        mov    ecx, dword ptr [esi + 0f4h]
        or     ecx, ecx
        je     SHORT loc_1_0AB4
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 115h
        jne    SHORT loc_1_0A83
        mov    dword ptr [esi + 0f4h], 0
        mov    dword ptr [esi + 10dh], 0
        jmp    SHORT loc_1_0AB4
loc_1_0A83:
        mov    dword ptr [ecx + 0ch], 0
        cmp    dword ptr [esi + 0ech], 0
        jne    SHORT loc_1_0A9B
        mov    dword ptr [esi + 0ech], ecx
        jmp    SHORT loc_1_0AA4
loc_1_0A9B:
        mov    eax, dword ptr [esi + 0f0h]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0AA4:
        mov    dword ptr [esi + 0f0h], ecx
        mov    dword ptr [esi + 0f4h], 0
loc_1_0AB4:
        mov    edx, dword ptr [esi + 0e4h]
        or     edx, edx
        je     SHORT loc_1_0B09
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 0e4h], eax
        or     edx, edx
        je     SHORT loc_1_0B09
        sub    ecx, ecx
loc_1_0ACD:
        mov    dword ptr [edx + 14h], ecx
        mov    dword ptr [edx + 0ch], ecx
        cmp    dword ptr [esi + 0ech], ecx
        jne    SHORT loc_1_0AE3
        mov    dword ptr [esi + 0ech], edx
        jmp    SHORT loc_1_0AEC
loc_1_0AE3:
        mov    eax, dword ptr [esi + 0f0h]
        mov    dword ptr [eax + 0ch], edx
loc_1_0AEC:
        mov    dword ptr [esi + 0f0h], edx
        mov    edx, dword ptr [esi + 0e4h]
        cmp    edx, ecx
        je     SHORT loc_1_0B09
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 0e4h], eax
        cmp    edx, ecx
        jne    SHORT loc_1_0ACD
loc_1_0B09:
        push   esi
        call   sub_1_092A
        mov    dword ptr [esi + 100h], 0
loc_1_0B19:
        sub    eax, eax
        pop    esi
        ret    10h
loc_1_0B1F:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0f4h]
        or     eax, eax
        je     SHORT loc_1_0B38
        mov    eax, dword ptr [eax + 14h]
        add    eax, dword ptr [ecx + 100h]
        jmp    SHORT loc_1_0B3E
loc_1_0B38:
        mov    eax, dword ptr [ecx + 100h]
loc_1_0B3E:
        ret    10h
loc_1_0B41:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   esi
        mov    dword ptr [esi + 10dh], 1
        db  068h, 0CAh, 004h, 000h, 000h
        push   1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 10h
        lea    edi, [esi + 20h]
        push   edi
        call   0
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi + 44h]
        lea    eax, [esi + 30h]
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 10h
        mov    dword ptr [esi + 100h], 0
        push   esi
        call   sub_1_0632
        push   2710h
        push   edi
        call   sub_1_0067
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0BB0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        cmp    dword ptr [edi + 110h], 0
        je     SHORT loc_1_0BDF
        mov    dword ptr [edi + 110h], 0
        push   dword ptr [edi + 12dh]
        call   sub_1_1744
        mov    dword ptr [edi + 12dh], 0
loc_1_0BDF:
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_0180
        mov    ebx, eax
        mov    dword ptr [ebx + 10h], esi
        push   dword ptr [ebx]
        call   sub_1_0180
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [ebx + 14h], 0
        mov    dword ptr [ebx + 0ch], 0
        cmp    dword ptr [edi + 0e4h], 0
        jne    SHORT loc_1_0C17
        mov    dword ptr [edi + 0e4h], ebx
        jmp    SHORT loc_1_0C20
loc_1_0C17:
        mov    eax, dword ptr [edi + 0e8h]
        mov    dword ptr [eax + 0ch], ebx
loc_1_0C20:
        mov    dword ptr [edi + 0e8h], ebx
        push   edi
        call   sub_1_0C37
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
sub_1_0C37:
        sub    esp, 0ch
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 119h], 0
        jne    NEAR PTR loc_1_0D1E
        cmp    dword ptr [esi + 0f4h], 0
        jne    NEAR PTR loc_1_0CF3
        sub    ebx, ebx
loc_1_0C5C:
        mov    eax, dword ptr [esi + 0e4h]
        cmp    eax, ebx
        je     SHORT loc_1_0C6F
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0e4h], ecx
loc_1_0C6F:
        mov    dword ptr [esi + 0f4h], eax
        cmp    eax, ebx
        je     SHORT loc_1_0CAD
        mov    eax, dword ptr [eax + 1ch]
        add    eax, dword ptr [esi + 125h]
        mov    dword ptr [esi + 125h], eax
        mov    ecx, dword ptr [esi + 121h]
        add    ecx, eax
        mov    eax, dword ptr [esi + 0f4h]
        push   esi
        mov    dword ptr [eax + 1ch], ecx
        mov    byte ptr [esi + 114h], bl
        call   sub_1_0D26
        cmp    dword ptr [esi + 0f4h], ebx
        je     SHORT loc_1_0C5C
loc_1_0CAD:
        cmp    dword ptr [esi + 0f4h], 0
        jne    SHORT loc_1_0CF3
        cmp    dword ptr [esi + 0ech], 0
        je     SHORT loc_1_0CF3
        cmp    dword ptr [esi + 12dh], 0
        jne    SHORT loc_1_0CF3
        push   esi
        push   32h
        db  068h, 010h, 010h, 000h, 000h
        call   sub_1_1728
        mov    dword ptr [esi + 12dh], eax
        or     eax, eax
        je     SHORT loc_1_0CEB
        mov    dword ptr [esi + 110h], 1
        jmp    SHORT loc_1_0CF3
loc_1_0CEB:
        push   1
        push   esi
        call   sub_1_0ED5
loc_1_0CF3:
        cmp    dword ptr [esi + 110h], 0
        je     SHORT loc_1_0D16
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi]
        call   sub_1_16CC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0D16
        cmp    dword ptr [esp + 10h], 0
        ja     SHORT loc_1_0D1E
loc_1_0D16:
        push   0
        push   esi
        call   sub_1_0ED5
loc_1_0D1E:
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    4
sub_1_0D26:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        mov    dword ptr [esp + 18h], 0
        mov    edi, dword ptr [esi + 0f4h]
        or     edi, edi
        je     NEAR PTR loc_1_0ECB
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [edi + 18h]
        mov    dword ptr [esp + 20h], eax
loc_1_0D51:
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esp + 1ch], eax
        mov    ebx, dword ptr [edi + 4]
        cmp    eax, ebx
        jb     SHORT loc_1_0D6C
        cmp    byte ptr [esi + 114h], 0
        je     NEAR PTR loc_1_0E69
loc_1_0D6C:
        mov    ebp, dword ptr [edi + 18h]
        add    ebp, dword ptr [esp + 1ch]
        movzx  eax, byte ptr [esi + 114h]
        or     eax, eax
        je     SHORT loc_1_0D91
        cmp    eax, 2
        je     NEAR PTR loc_1_0E29
        cmp    eax, 3
        je     SHORT loc_1_0DF7
        jmp    NEAR PTR loc_1_0E5E
loc_1_0D91:
        mov    byte ptr [esi + 114h], 2
        mov    eax, dword ptr [esp + 20h]
        sub    eax, ebp
        push   eax
        push   10h
        push   ebp
        call   sub_1_028A
        or     eax, eax
        je     SHORT loc_1_0E29
        sub    eax, ebp
        mov    byte ptr [esi + 114h], 1
        lea    ebx, [eax + 1]
        lea    eax, [esp + 14h]
        push   eax
        push   ebx
        push   ebp
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_0E93
        mov    eax, dword ptr [esp + 14h]
        add    dword ptr [edi + 14h], eax
        cmp    dword ptr [esp + 14h], ebx
        je     SHORT loc_1_0DEE
        mov    byte ptr [esi + 114h], 0
        mov    dword ptr [esp + 18h], 1
        jmp    SHORT loc_1_0E5E
loc_1_0DEE:
        mov    byte ptr [esi + 114h], 3
        jmp    SHORT loc_1_0E5E
loc_1_0DF7:
        mov    byte ptr [esp + 13h], 10h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 17h]
        push   1
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0E93
        cmp    dword ptr [esp + 14h], 1
        jne    SHORT loc_1_0E1F
        jmp    SHORT loc_1_0E57
loc_1_0E1F:
        mov    dword ptr [esp + 18h], 1
        jmp    SHORT loc_1_0E5E
loc_1_0E29:
        sub    ebx, dword ptr [esp + 1ch]
        lea    eax, [esp + 14h]
        push   eax
        push   ebx
        push   ebp
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0E93
        mov    eax, dword ptr [esp + 14h]
        add    dword ptr [edi + 14h], eax
        cmp    dword ptr [esp + 14h], ebx
        je     SHORT loc_1_0E57
        mov    dword ptr [esp + 18h], 1
loc_1_0E57:
        mov    byte ptr [esi + 114h], 0
loc_1_0E5E:
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_0D51
loc_1_0E69:
        mov    eax, dword ptr [edi + 14h]
        cmp    dword ptr [edi + 4], eax
        jne    SHORT loc_1_0ECB
        add    dword ptr [esi + 115h], eax
        mov    eax, dword ptr [esi + 0f4h]
        lea    ecx, [esi + 0ech]
        mov    dword ptr [eax + 0ch], 0
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_0EB8
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_0EBE
loc_1_0E93:
        mov    eax, dword ptr [edi + 14h]
        add    dword ptr [esi + 115h], eax
        mov    eax, dword ptr [esi + 0f4h]
        lea    ecx, [esi + 0ech]
        mov    dword ptr [eax + 0ch], 0
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_0EB8
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_0EBE
loc_1_0EB8:
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0ch], eax
loc_1_0EBE:
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi + 0f4h], 0
loc_1_0ECB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    4
sub_1_0ED5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 10ch], 1
        je     SHORT loc_1_0F53
        mov    dword ptr [esi + 10ch], 1
        mov    edi, dword ptr [ebp + 0ch]
loc_1_0EF6:
        or     edi, edi
        mov    ecx, dword ptr [esi + 0ech]
        je     SHORT loc_1_0F0F
        or     ecx, ecx
        je     SHORT loc_1_0F31
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 0ech], eax
        jmp    SHORT loc_1_0F31
loc_1_0F0F:
        or     ecx, ecx
        je     SHORT loc_1_0F31
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 1ch], eax
        jae    SHORT loc_1_0F2F
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 0ech], eax
        jmp    SHORT loc_1_0F31
loc_1_0F2F:
        sub    ecx, ecx
loc_1_0F31:
        or     ecx, ecx
        je     SHORT loc_1_0F49
        push   0
        push   dword ptr [ecx + 10h]
        push   1
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   sub_1_01ED
        jmp    SHORT loc_1_0EF6
loc_1_0F49:
        mov    dword ptr [esi + 10ch], 0
loc_1_0F53:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_0F5B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 24h]
loc_1_0F6E:
        push   ebx
        sub    ebp, ebp
        push   dword ptr [esp + 24h]
        push   edi
        push   esi
        call   sub_1_16EC
        add    esp, 10h
        cmp    eax, ebp
        je     SHORT loc_1_0FAA
        cmp    dword ptr [ebx], ebp
        jne    SHORT loc_1_0FA6
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        push   esi
        call   sub_1_16D4
        add    esp, 0ch
        cmp    eax, ebp
        je     SHORT loc_1_0FAA
        test   byte ptr [esp + 11h], 1
        jne    SHORT loc_1_0FA6
        mov    ebp, 1
loc_1_0FA6:
        or     ebp, ebp
        jne    SHORT loc_1_0F6E
loc_1_0FAA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0FB2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 0fch], 0
        je     SHORT loc_1_0FD6
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        popfd
        jmp    SHORT loc_1_100B
loc_1_0FD6:
        mov    dword ptr [esi + 0fch], 0ffffffffh
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        popfd
        cmp    dword ptr [esi + 0f8h], 0
        jne    SHORT loc_1_1001
        push   esi
        db  068h, 082h, 010h, 000h, 000h
        call   sub_1_01A9
        mov    dword ptr [esi + 0f8h], eax
loc_1_1001:
        mov    dword ptr [esi + 0fch], 0
loc_1_100B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1010:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    dword ptr [ebp - 4], edx
        pushfd
        pop    dword ptr [ebp - 8]
        cli
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0fch
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_1039
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        popfd
        jmp    SHORT loc_1_107D
loc_1_1039:
        mov    dword ptr [ecx], 0ffffffffh
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        popfd
        mov    esi, dword ptr [ebp - 4]
        add    esi, 0f8h
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_1063
        push   dword ptr [ebp - 4]
        db  068h, 082h, 010h, 000h, 000h
        call   sub_1_01A9
        mov    dword ptr [esi], eax
loc_1_1063:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 12dh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0fch], 0
loc_1_107D:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1082:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0f8h], 0
        push   dword ptr [esp]
        call   sub_1_0D26
        push   dword ptr [esp]
        call   sub_1_0C37
        add    esp, 4
        ret
sub_1_10AD:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0
        push   dword ptr [esi]
        call   sub_1_16E4
        add    esp, 8
        cmp    dword ptr [esi + 0f4h], 0
        jne    SHORT loc_1_10D0
        cmp    dword ptr [esi + 0ech], 0
        je     SHORT loc_1_10DA
loc_1_10D0:
        mov    dword ptr [esi + 131h], 1
loc_1_10DA:
        mov    eax, dword ptr [esi + 0f4h]
        or     eax, eax
        je     SHORT loc_1_1115
        mov    dword ptr [eax + 0ch], 0
        cmp    dword ptr [esi + 0ech], 0
        jne    SHORT loc_1_10FC
        mov    dword ptr [esi + 0ech], eax
        jmp    SHORT loc_1_1105
loc_1_10FC:
        mov    ecx, dword ptr [esi + 0f0h]
        mov    dword ptr [ecx + 0ch], eax
loc_1_1105:
        mov    dword ptr [esi + 0f0h], eax
        mov    dword ptr [esi + 0f4h], 0
loc_1_1115:
        mov    edx, dword ptr [esi + 0e4h]
        or     edx, edx
        je     SHORT loc_1_1167
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 0e4h], eax
        or     edx, edx
        je     SHORT loc_1_1167
        sub    ecx, ecx
loc_1_112E:
        mov    dword ptr [edx + 0ch], ecx
        cmp    dword ptr [esi + 0ech], ecx
        jne    SHORT loc_1_1141
        mov    dword ptr [esi + 0ech], edx
        jmp    SHORT loc_1_114A
loc_1_1141:
        mov    eax, dword ptr [esi + 0f0h]
        mov    dword ptr [eax + 0ch], edx
loc_1_114A:
        mov    dword ptr [esi + 0f0h], edx
        mov    edx, dword ptr [esi + 0e4h]
        cmp    edx, ecx
        je     SHORT loc_1_1167
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 0e4h], eax
        cmp    edx, ecx
        jne    SHORT loc_1_112E
loc_1_1167:
        push   1
        push   esi
        call   sub_1_0ED5
        mov    dword ptr [esi + 115h], 0
        mov    dword ptr [esi + 119h], 0
        sub    eax, eax
        pop    esi
        ret    10h
loc_1_1189:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        lea    edi, [esi + 78h]
        mov    dword ptr [esi + 0f4h], 0
        mov    dword ptr [esi + 80h], 0
        mov    dword ptr [esi + 84h], 0
        call   sub_1_0180
        mov    dword ptr [esi + 88h], eax
        push   0
        push   edi
        push   0c0000002h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_12F5
        cmp    dword ptr [esi + 7ch], 0
        jne    NEAR PTR loc_1_12F5
        test   byte ptr [esi + 1eh], 8
        je     SHORT loc_1_11FE
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        push   esi
        db  0E8h, 0DAh, 0EEh, 0FEh, 0FFh
        jmp    SHORT loc_1_1201
loc_1_11FE:
        mov    ebx, dword ptr [ebp + 10h]
loc_1_1201:
        mov    dword ptr [esi + 80h], 0
        mov    dword ptr [esi + 84h], 0
        push   0
        push   edi
        push   0c0000003h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_12CF
        cmp    dword ptr [esi + 7ch], 0
        jne    NEAR PTR loc_1_12CF
        test   byte ptr [esi + 1eh], 8
        jne    SHORT loc_1_124A
        push   0
        push   0
        push   ebx
        push   esi
        db  0E8h, 08Ch, 0EEh, 0FEh, 0FFh
loc_1_124A:
        mov    edi, dword ptr [esi]
        push   0
        push   edi
        call   sub_1_16E4
        add    esp, 8
        push   esi
        db  068h, 0B2h, 00Fh, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        shr    eax, 1
        push   eax
        push   edi
        call   sub_1_1704
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_1275
        jmp    NEAR PTR loc_1_12F5
loc_1_1275:
        push   esi
        db  068h, 075h, 014h, 000h, 000h
        push   2
        push   edi
        call   sub_1_16FC
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_129B
        push   0
        push   0
        push   -1
        push   edi
        call   sub_1_1704
        add    esp, 10h
        jmp    SHORT loc_1_12F5
loc_1_129B:
        mov    dword ptr [esi + 110h], 0
        mov    dword ptr [esi + 115h], 0
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 121h], eax
        mov    dword ptr [esi + 125h], 0
        sub    eax, eax
        jmp    SHORT loc_1_12FA
loc_1_12CF:
        mov    dword ptr [esi + 80h], 0
        mov    dword ptr [esi + 84h], 0
        push   0
        push   edi
        push   0c0000004h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
loc_1_12F5:
        mov    eax, 2
loc_1_12FA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1303:
        push   esi
        push   edi
        push   0
        push   0
        mov    esi, dword ptr [esp + 14h]
        push   0
        push   esi
        call   sub_1_10AD
        push   0
        push   0
        push   -1
        push   dword ptr [esi]
        call   sub_1_1704
        add    esp, 10h
        push   0
        push   0
        push   -1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 10h
        mov    edi, eax
        push   dword ptr [esi + 12dh]
        call   sub_1_1744
        mov    dword ptr [esi + 12dh], 0
        push   dword ptr [esi + 0f8h]
        call   sub_1_01DB
        mov    dword ptr [esi + 0f8h], 0
        push   dword ptr [esi + 100h]
        call   sub_1_01DB
        mov    dword ptr [esi + 100h], 0
        test   byte ptr [esi + 1eh], 10h
        je     SHORT loc_1_138F
        push   5fh
        lea    eax, [esi + 0b4h]
        push   eax
        push   dword ptr [esi]
        call   sub_1_16C4
        add    esp, 0ch
loc_1_138F:
        mov    dword ptr [esi + 80h], 0
        mov    dword ptr [esi + 84h], 0
        push   0
        lea    eax, [esi + 78h]
        push   eax
        push   0c0000004h
        push   dword ptr [esi]
        call   sub_1_16DC
        add    esp, 10h
        or     edi, edi
        mov    eax, 2
        je     SHORT loc_1_13C3
        sub    eax, eax
loc_1_13C3:
        pop    edi
        pop    esi
        ret    10h
loc_1_13C8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0f4h]
        or     ecx, ecx
        mov    eax, dword ptr [eax + 115h]
        je     SHORT loc_1_13DF
        add    eax, dword ptr [ecx + 14h]
loc_1_13DF:
        ret    10h
loc_1_13E2:
        sub    esp, 0ch
        lea    eax, [esp]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax]
        call   sub_1_16CC
        add    esp, 8
        or     eax, eax
        mov    eax, 0ffffffffh
        je     SHORT loc_1_1405
        mov    eax, dword ptr [esp + 8]
loc_1_1405:
        add    esp, 0ch
        ret    10h
loc_1_140B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    edx, dword ptr [ebp + 0ch]
        or     edx, edx
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_1_1438
        cmp    dword ptr [ecx + 119h], 0
        jne    SHORT loc_1_1438
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 129h], eax
        jmp    SHORT loc_1_145D
loc_1_1438:
        or     edx, edx
        jne    SHORT loc_1_145D
        cmp    dword ptr [ecx + 119h], 0
        je     SHORT loc_1_145D
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, dword ptr [ecx + 129h]
        add    dword ptr [ecx + 125h], eax
loc_1_145D:
        mov    dword ptr [ecx + 119h], edx
        or     edx, edx
        jne    SHORT loc_1_146D
        push   ecx
        call   sub_1_0C37
loc_1_146D:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1475:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 104h], 0
        je     SHORT loc_1_1499
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        popfd
        jmp    SHORT loc_1_14CE
loc_1_1499:
        mov    dword ptr [esi + 104h], 0ffffffffh
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        popfd
        cmp    dword ptr [esi + 100h], 0
        jne    SHORT loc_1_14C4
        push   esi
        db  068h, 0D3h, 014h, 000h, 000h
        call   sub_1_01A9
        mov    dword ptr [esi + 100h], eax
loc_1_14C4:
        mov    dword ptr [esi + 104h], 0
loc_1_14CE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_14D3:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 10h], edx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 100h], 0
        sub    ebx, ebx
        mov    esi, 2
loc_1_14F2:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 13h]
        push   1
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax]
        call   sub_1_097C
        add    esp, 10h
        cmp    eax, ebx
        je     NEAR PTR loc_1_160B
        cmp    dword ptr [esp + 14h], ebx
        je     NEAR PTR loc_1_160B
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 108h]
        add    edi, 108h
        cmp    eax, ebx
        je     SHORT loc_1_153D
        cmp    eax, 4
        je     NEAR PTR loc_1_15DA
        jmp    SHORT loc_1_14F2
loc_1_153D:
        cmp    byte ptr [esp + 0fh], 10h
        jne    SHORT loc_1_154C
        mov    dword ptr [edi], 4
        jmp    SHORT loc_1_14F2
loc_1_154C:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 11dh], ebx
        je     SHORT loc_1_14F2
        mov    cl, byte ptr [esp + 0fh]
        mov    eax, dword ptr [eax + 139h]
        mov    edx, dword ptr [esp + 10h]
        mov    byte ptr [eax + edx + 141h], cl
        mov    eax, dword ptr [esp + 10h]
        push   ebx
        inc    dword ptr [eax + 139h]
        mov    eax, dword ptr [esp + 14h]
        add    eax, 135h
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   0c0000001h
        push   dword ptr [eax]
        call   sub_1_16DC
        mov    eax, dword ptr [esp + 20h]
        add    esp, 10h
        mov    eax, dword ptr [eax + 13dh]
        cmp    eax, ebx
        je     SHORT loc_1_15AC
        cmp    eax, 3
        jne    NEAR PTR loc_1_14F2
loc_1_15AC:
        mov    eax, dword ptr [esp + 10h]
        push   ebx
        mov    ecx, dword ptr [eax + 139h]
        mov    byte ptr [eax + ecx + 141h], bl
        mov    eax, dword ptr [esp + 14h]
        add    eax, 20h
        push   eax
        call   sub_1_0035
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 11dh], ebx
        jmp    NEAR PTR loc_1_14F2
loc_1_15DA:
        mov    dword ptr [esp + 1ch], esi
        mov    byte ptr [esp + 24h], 10h
        mov    al, byte ptr [esp + 0fh]
        mov    byte ptr [esp + 25h], al
        lea    eax, [esp + 18h]
        push   ebx
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   0c0000000h
        push   dword ptr [eax]
        call   sub_1_16DC
        add    esp, 10h
        mov    dword ptr [edi], ebx
        jmp    NEAR PTR loc_1_14F2
loc_1_160B:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_1612:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    ebx, 2
        mov    edi, dword ptr [esi + 131h]
        mov    dword ptr [esi + 139h], 0
        mov    dword ptr [esi + 11dh], 1
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_10AD
        push   esi
        db  068h, 075h, 014h, 000h, 000h
        push   1
        push   dword ptr [esi]
        call   sub_1_16FC
        add    esp, 10h
        lea    ebp, [esi + 20h]
loc_1_165D:
        push   ebp
        call   0
        mov    byte ptr [esp + 13h], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 17h]
        push   1
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 10h
        or     edi, edi
        je     SHORT loc_1_1687
        mov    ecx, dword ptr [esi + 74h]
        jmp    SHORT loc_1_168A
loc_1_1687:
        mov    ecx, dword ptr [esi + 5ch]
loc_1_168A:
        or     edi, edi
        lea    eax, [esi + 60h]
        jne    SHORT loc_1_1694
        lea    eax, [esi + 48h]
loc_1_1694:
        lea    edx, [esp + 14h]
        push   edx
        push   ecx
        push   eax
        push   dword ptr [esi]
        call   sub_1_0F5B
        add    esp, 10h
        push   2710h
        push   ebp
        call   sub_1_0067
        dec    ebx
        or     eax, eax
        je     SHORT loc_1_16B9
        or     ebx, ebx
        jne    SHORT loc_1_165D
loc_1_16B9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    10h
        db  000h
sub_1_16C4:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_16CC:
        db  0CDh, 020h, 00Ah, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_16D4:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_16DC:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_16E4:
        db  0CDh, 020h, 00Fh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_16EC:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_16F4:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_16FC:
        db  0CDh, 020h, 017h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_1704:
        db  0CDh, 020h, 018h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_170C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_1714:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_171C:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_1_1728:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_1_1744:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_1_1754:
        db  0CDh, 020h, 0FAh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_175C:
        db  0CDh, 020h, 027h, 081h, 001h, 000h
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 053h, 045h, 052h, 057h
        db  041h, 056h, 045h, 020h, 000h, 000h, 000h, 080h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Dh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
