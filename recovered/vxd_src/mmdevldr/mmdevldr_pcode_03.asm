; mmdevldr_pcode_03.asm -- MMDEVLDR object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_1059
        add    esp, 14h
        leave
        ret
MMDEVLDR_Register_Device_Driver:
        push   ecx
        push   ebx
        push   eax
        call   sub_3_0799
        add    esp, 0ch
        clc
        ret
loc_3_0029:
        call   sub_3_058F
        xor    eax, eax
        clc
        ret
loc_3_0032:
        xor    eax, eax
        mov    ecx, 1000h
        push   eax
        push   eax
        push   ebx
        push   eax
        push   ecx
        call   sub_3_1059
        add    esp, 14h
        cmp    eax, 1
        ja     SHORT loc_3_0063
        push   3
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_3_0063:
        stc
        ret
sub_3_0065:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        lea    eax, [esp]
        push   eax
        push   8
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        add    esp, 8
        movzx  eax, ax
        pop    ebx
        leave
        ret
loc_3_0091:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_0821
        add    esp, 4
        leave
        ret
sub_3_00A1:
        push   ebx
        db  0CDh, 020h, 00Dh, 000h, 017h, 000h
        or     eax, eax
        je     SHORT loc_3_00AE
        jmp    SHORT loc_3_00B4
loc_3_00AE:
        clc
        jmp    NEAR PTR loc_3_0158
loc_3_00B4:
        db  0B8h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   92h
        push   20h
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        or     edx, eax
        je     SHORT loc_3_00F0
        jmp    SHORT loc_3_00F0
loc_3_00F0:
        db  0A3h, 000h, 000h, 000h, 000h
        shl    eax, 10h
        push   eax
        lea    eax, [esp]
        push   eax
        push   4
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        add    esp, 4
        movzx  eax, ax
        or     eax, eax
        je     SHORT loc_3_0141
        xor    eax, eax
        mov    ebx, 3
        push   eax
        push   ebx
        lea    eax, [esp]
        push   eax
        push   8
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        add    esp, 8
loc_3_0141:
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
loc_3_0158:
        pop    ebx
        ret
loc_3_015A:
        jecxz  loc_3_019D
        dec    ecx
        je     SHORT loc_3_01C0
        dec    ecx
        je     NEAR PTR loc_3_01F9
        dec    ecx
        je     NEAR PTR loc_3_0227
        dec    ecx
        je     NEAR PTR loc_3_0270
        dec    ecx
        je     NEAR PTR loc_3_02A9
        dec    ecx
        je     NEAR PTR loc_3_0246
        dec    ecx
        je     NEAR PTR loc_3_02CE
        dec    ecx
        je     NEAR PTR loc_3_0326
        dec    ecx
        je     NEAR PTR loc_3_035B
        mov    eax, 1
        ret
loc_3_019D:
        push   edi
        push   esi
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_3_01BB
        mov    edi, dword ptr [esi + 20h]
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], 100h
        or     edi, edi
        je     SHORT loc_3_01BB
        mov    dword ptr [edi], 4
loc_3_01BB:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_3_01C0:
        push   edi
        push   esi
        cmp    dword ptr [esi + 14h], 0ch
        jb     SHORT loc_3_01F4
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_3_01F4
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 10h]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], eax
        or     edi, edi
        je     SHORT loc_3_01F4
        mov    dword ptr [edi], 4
loc_3_01F4:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_3_01F9:
        push   edi
        push   esi
        cmp    dword ptr [esi + 14h], 0ch
        jb     SHORT loc_3_0222
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 10h]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     edi, edi
        je     SHORT loc_3_0222
        mov    dword ptr [edi], 0
loc_3_0222:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_3_0227:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        mov    edi, dword ptr [esi + 18h]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esi + 20h]
        or     edi, edi
        je     SHORT loc_3_0242
        mov    dword ptr [edi], 4
loc_3_0242:
        pop    edi
        xor    eax, eax
        ret
loc_3_0246:
        push   edi
        cmp    dword ptr [esi + 14h], 4
        jb     SHORT loc_3_026C
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_3_026C
        db  0CDh, 020h, 014h, 000h, 02Ah, 000h
        mov    edi, dword ptr [esi + 18h]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esi + 20h]
        or     edi, edi
        je     SHORT loc_3_0242
        mov    dword ptr [edi], 4
loc_3_026C:
        pop    edi
        xor    eax, eax
        ret
loc_3_0270:
        push   edi
        push   esi
        cmp    dword ptr [esi + 14h], 0ch
        jb     SHORT loc_3_02A4
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_3_02A4
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 10h]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], eax
        or     edi, edi
        je     SHORT loc_3_02A4
        mov    dword ptr [edi], 4
loc_3_02A4:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_3_02A9:
        push   edi
        push   esi
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_3_02C9
        mov    edi, dword ptr [esi + 20h]
        mov    esi, dword ptr [esi + 18h]
        db  08Dh, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi], eax
        or     edi, edi
        je     SHORT loc_3_02C9
        mov    dword ptr [edi], 4
loc_3_02C9:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_3_02CE:
        push   edi
        push   esi
        cmp    dword ptr [esi + 14h], 8
        jb     SHORT loc_3_0321
        cmp    dword ptr [esi + 1ch], 10h
        jb     SHORT loc_3_0321
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        or     eax, 2
        sub    esp, 4
        mov    ebx, esp
        push   eax
        push   ebx
        push   0fffffh
        push   0
        push   0fh
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ebx
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        or     edi, edi
        je     SHORT loc_3_0321
        mov    dword ptr [edi], 4
loc_3_0321:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_3_0326:
        push   edi
        push   esi
        cmp    dword ptr [esi + 14h], 4
        jb     SHORT loc_3_0356
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_3_0356
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 10h]
        push   0
        push   dword ptr [eax]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], eax
        or     edi, edi
        je     SHORT loc_3_0356
        mov    dword ptr [edi], 4
loc_3_0356:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_3_035B:
        push   edi
        push   esi
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_3_03D1
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [edi], 4
        mov    eax, dword ptr [esi + 10h]
        lea    ebx, [ebp - 4]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 002h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_3_03C7
        lea    eax, [ebp - 8]
        lea    ebx, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   0
        push   ecx
        push   ebx
        push   eax
        db  0CDh, 020h, 036h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    SHORT loc_3_03C7
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp - 0ch]
        test   eax, 8
        jne    SHORT loc_3_03B9
        cmp    ebx, 16h
        je     SHORT loc_3_03C0
        mov    eax, 2
        jmp    SHORT loc_3_03CC
loc_3_03B9:
        mov    eax, 1
        jmp    SHORT loc_3_03CC
loc_3_03C0:
        mov    eax, 3
        jmp    SHORT loc_3_03CC
loc_3_03C7:
        mov    eax, 0
loc_3_03CC:
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], eax
loc_3_03D1:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
sub_3_03D6:
        push   ebp
        mov    ebp, esp
        push   -1
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        leave
        ret
sub_3_03EC:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        leave
        ret
sub_3_03FD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        push   -1
        push   dword ptr [ebp + 0ch]
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        leave
        ret
mmdevldr_PM_API:
        movzx  ebx, word ptr [ebp + 14h]
        mov    eax, 0bh
        cmp    ebx, 7
        jae    NEAR PTR loc_3_04DD
        cmp    ebx, 3
        je     NEAR PTR loc_3_04EE
        cmp    ebx, 4
        je     NEAR PTR loc_3_0569
        cmp    ebx, 0
        je     SHORT loc_3_04BD
        cmp    ebx, 1
        je     SHORT loc_3_04BD
        cmp    ebx, 5
        je     NEAR PTR loc_3_050B
        cmp    ebx, 6
        je     NEAR PTR loc_3_054D
        cmp    ebx, 2
        jne    SHORT loc_3_04DD
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    esi, eax
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 3ch]
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    ecx, eax
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   ecx
        push   eax
        push   dword ptr [esi]
        call   sub_3_0869
        add    esp, 0ch
        pop    dword ptr [ebp + 3ch]
        pop    dword ptr [ebp + 14h]
        jmp    SHORT loc_3_04DD
loc_3_04BD:
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_3_04DB
        mov    eax, 0bh
        jmp    SHORT loc_3_04DD
loc_3_04DB:
        xor    eax, eax
loc_3_04DD:
        mov    dx, ax
        sub    ax, 1
        cmc
        lahf
        mov    byte ptr [ebp + 2ch], ah
        mov    word ptr [ebp + 1ch], dx
        ret
loc_3_04EE:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        jmp    SHORT loc_3_04DD
loc_3_050B:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        sub    esp, 4
        mov    ebx, esp
        push   dword ptr [eax]
        push   ebx
        push   dword ptr [eax + 4]
        push   0
        push   0fh
        push   0
        push   1
        push   dword ptr [eax + 8]
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ebx
        mov    ecx, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], edx
        xor    eax, eax
        jmp    SHORT loc_3_04DD
loc_3_054D:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   0
        push   dword ptr [eax]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_3_04DD
loc_3_0569:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    eax, dword ptr [eax]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        jmp    NEAR PTR loc_3_04DD
sub_3_0580:
        push   0ch
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0589:
        db  0A3h, 000h, 000h, 000h, 000h
        ret
sub_3_058F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        je     SHORT loc_3_05DE
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_05A4:
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_05C9
loc_3_05AA:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_05B7
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_05B7:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_05C3:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_05AA
loc_3_05C9:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_05D4:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_05DE:
        pop    esi
        ret
sub_3_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 208h
        push   esi
        lea    eax, [ebp - 208h]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   100h
        push   eax
        push   0
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0605:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_06DC
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 208h]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0625:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_3_0636
        mov    eax, 0dh
        jmp    NEAR PTR loc_3_06DC
loc_3_0636:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 108h]
        mov    dword ptr [ebp - 4], 100h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0659:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_06CA
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_3_06CA
        push   0
        lea    eax, [ebp - 108h]
        push   2
        push   eax
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0678:
        add    esp, 10h
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_3_06CF
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 108h]
        mov    dword ptr [ebp - 4], 100h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06A4:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_06CF
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_3_06CF
        push   0
        lea    eax, [ebp - 108h]
        push   0
        push   eax
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06C3:
        add    esp, 10h
        mov    edi, eax
        jmp    SHORT loc_3_06CF
loc_3_06CA:
        mov    edi, 0ch
loc_3_06CF:
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06D7:
        add    esp, 4
        mov    eax, edi
loc_3_06DC:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_06E2:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_06FB
        push   1
        push   dword ptr [esp + 8]
        call   sub_3_0065
        add    esp, 8
        jmp    SHORT loc_3_0713
loc_3_06FB:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0713
        call   sub_3_00A1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_3_0713:
        ret
sub_3_0714:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_072B
        push   0
        push   dword ptr [esp + 8]
        call   sub_3_0065
        add    esp, 8
loc_3_072B:
        ret
sub_3_072C:
        mov    eax, dword ptr [esp + 0ch]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, 1
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        je     SHORT loc_3_0792
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0746:
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_3_0792
        mov    esi, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 18h]
loc_3_0754:
        mov    ecx, dword ptr [ebp]
        mov    edi, 0
        test   ecx, ecx
        je     SHORT loc_3_0776
        push   dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 20h]
        push   dword ptr [ebp + 4]
        push   dword ptr [edi]
        push   ebx
        push   esi
        call   ecx
        add    esp, 14h
        mov    edi, eax
loc_3_0776:
        test   edi, edi
        je     SHORT loc_3_077F
        cmp    edi, 1
        jne    SHORT loc_3_0792
loc_3_077F:
        mov    eax, dword ptr [esp + 1ch]
        push   ebp
        push   dword ptr [eax + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_078C:
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_3_0754
loc_3_0792:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_0799:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        je     SHORT loc_3_07CF
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_07AF:
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_07CF
        mov    edi, dword ptr [esp + 0ch]
loc_3_07B9:
        cmp    dword ptr [esi], edi
        je     SHORT loc_3_07D7
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_07C9:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_07B9
loc_3_07CF:
        mov    eax, 0dh
loc_3_07D4:
        pop    edi
        pop    esi
        ret
loc_3_07D7:
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_3_07F4
        push   8
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_07E6:
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    SHORT loc_3_07F4
        mov    eax, 2
        jmp    SHORT loc_3_07D4
loc_3_07F4:
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_07FC:
        test   eax, eax
        jne    SHORT loc_3_0807
        mov    eax, 2
        jmp    SHORT loc_3_07D4
loc_3_0807:
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 14h]
        push   eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edx
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_081D:
        xor    eax, eax
        jmp    SHORT loc_3_07D4
sub_3_0821:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0838:
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_0866
        mov    edi, 2
loc_3_0843:
        cmp    dword ptr [esi + 4], edi
        jne    SHORT loc_3_0854
        push   1
        push   dword ptr [esi]
        call   sub_3_0065
        add    esp, 8
loc_3_0854:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0860:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_0843
loc_3_0866:
        pop    edi
        pop    esi
        ret
sub_3_0869:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   0
        push   dword ptr [ebp + 8]
        push   2
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0885:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_3_0893
        mov    eax, 0bh
        jmp    SHORT loc_3_08BF
loc_3_0893:
        xor    edi, edi
        mov    edx, dword ptr [ebp + 10h]
        cmp    dword ptr [edx], edi
        jbe    SHORT loc_3_08BB
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_3_08A2:
        cmp    edi, dword ptr [ebp - 4]
        jae    SHORT loc_3_08BB
        mov    eax, dword ptr [esi]
        inc    edi
        mov    ax, word ptr [eax + 6]
        add    esi, 4
        mov    word ptr [ecx], ax
        add    ecx, 2
        cmp    dword ptr [edx], edi
        ja     SHORT loc_3_08A2
loc_3_08BB:
        xor    eax, eax
        mov    dword ptr [edx], edi
loc_3_08BF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_08C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 218h
        push   ebx
        lea    eax, [ebp - 0ch]
        push   esi
        lea    ecx, [ebp - 218h]
        push   edi
        xor    esi, esi
        push   eax
        lea    edx, [ebp - 18h]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        lea    ecx, [ebp - 118h]
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 100h
        mov    dword ptr [ebp - 0ch], 100h
        push   eax
        push   ecx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0906:
        add    esp, 20h
        mov    ebx, dword ptr [ebp + 0ch]
        test   eax, eax
        jne    SHORT loc_3_0968
loc_3_0910:
        lea    ecx, [ebp - 218h]
        lea    eax, [ebp - 118h]
        push   dword ptr [ebp - 0ch]
        push   ecx
        push   dword ptr [ebp - 18h]
        push   0
        push   eax
        inc    esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_092D:
        add    esp, 18h
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 218h]
        lea    edx, [ebp - 18h]
        mov    dword ptr [ebp - 8], 100h
        mov    dword ptr [ebp - 0ch], 100h
        push   eax
        push   ecx
        push   edx
        lea    eax, [ebp - 8]
        push   0
        lea    ecx, [ebp - 118h]
        push   eax
        push   ecx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0961:
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_3_0910
loc_3_0968:
        xor    esi, esi
        push   100h
        mov    dword ptr [ebp - 8], 100h
        lea    eax, [ebp - 118h]
        push   eax
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0984:
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_0A04
loc_3_098B:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 118h]
        push   eax
        push   ecx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_099C:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_0A06
        lea    eax, [ebp - 14h]
        lea    ecx, [ebp - 118h]
        push   eax
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09B4:
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_0A0D
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        call   sub_3_08C5
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09D7:
        add    esp, 4
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09E2:
        add    esp, 4
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_0A1D
        inc    esi
        lea    ecx, [ebp - 118h]
        push   dword ptr [ebp - 8]
        push   ecx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09FD:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_3_098B
loc_3_0A04:
        xor    eax, eax
loc_3_0A06:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0A0D:
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A15:
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_3_0A06
loc_3_0A1D:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_3_0A06
_MMDEVLDR_SetDevicePresence:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        lea    eax, [ebp - 104h]
        push   1
        push   100h
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A44:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_0B23
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_3_03FD
        add    esp, 8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 104h]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A78:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_0B23
        push   2
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A9B:
        add    esp, 18h
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0AA6:
        add    esp, 4
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        je     SHORT loc_3_0B23
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_3_03FD
        add    esp, 8
        lea    eax, [ebp - 104h]
        push   esi
        push   eax
        call   sub_3_03FD
        add    esp, 8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 104h]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0AE9:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_0B23
        cmp    dword ptr [ebp + 10h], 0
        push   2
        je     SHORT loc_3_0AFF
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_3_0B04
loc_3_0AFF:
        db  068h, 000h, 000h, 000h, 000h
loc_3_0B04:
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B15:
        add    esp, 18h
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B20:
        add    esp, 4
loc_3_0B23:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0B28:
        push   ebp
        mov    ebp, esp
        sub    esp, 57ch
        push   ebx
        lea    eax, [ebp - 47ch]
        mov    dword ptr [ebp - 1ch], 0
        push   esi
        push   edi
        push   1
        push   100h
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B53:
        add    esp, 14h
        lea    ecx, [ebp - 47ch]
        lea    edx, [ebp - 37ch]
        push   ecx
        push   edx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 37ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 20h]
        lea    edx, [ebp - 37ch]
        push   ecx
        push   edx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B95:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_100A
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 2ch]
        mov    dword ptr [ebp - 0ch], 8
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0BC0:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_0C07
        cmp    byte ptr [ebp - 2ch], 31h
        jne    SHORT loc_3_0C07
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 1ch], 1
        mov    dword ptr [ebp - 0ch], 4
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0BF4:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_0C07
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 8], eax
        je     NEAR PTR loc_3_0FFF
loc_3_0C07:
        push   4
        lea    eax, [ebp + 8]
        push   eax
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0C1E:
        add    esp, 18h
        lea    ecx, [ebp - 3ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 47ch]
        push   ecx
        call   sub_3_03EC
        add    esp, 4
        cmp    byte ptr [ebp + eax - 47ch], 5ch
        je     SHORT loc_3_0C5A
        mov    ecx, 5ch
loc_3_0C50:
        dec    eax
        cmp    byte ptr [ebp + eax - 47ch], cl
        jne    SHORT loc_3_0C50
loc_3_0C5A:
        lea    eax, [ebp + eax - 47bh]
        lea    ecx, [ebp - 3ch]
        push   eax
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 3ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 57ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 7ch]
        mov    dword ptr [ebp - 0ch], 40h
        mov    dword ptr [ebp - 24h], 0
        push   40h
        push   ecx
        push   0
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0CB1:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_0FE4
loc_3_0CBC:
        lea    eax, [ebp - 14h]
        lea    ecx, [ebp - 7ch]
        mov    dword ptr [ebp - 18h], 1
        push   eax
        push   ecx
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0CD3:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_0FFF
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 2ch]
        mov    dword ptr [ebp - 0ch], 8
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0CFE:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_0D12
        cmp    byte ptr [ebp - 2ch], 31h
        jne    SHORT loc_3_0D12
        mov    dword ptr [ebp - 18h], 0
loc_3_0D12:
        xor    esi, esi
        push   100h
        mov    dword ptr [ebp - 0ch], 100h
        lea    eax, [ebp - 37ch]
        push   eax
        push   esi
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D30:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_0FB3
        mov    ebx, 5ch
loc_3_0D40:
        mov    edi, 1
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 37ch]
        push   ecx
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D58:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_1011
        lea    eax, [ebp - 3ch]
        lea    ecx, [ebp - 37ch]
        push   eax
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 57ch]
        lea    edx, [ebp - 27ch]
        push   ecx
        push   edx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 7ch]
        lea    edx, [ebp - 27ch]
        push   ecx
        push   edx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 27ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 37ch]
        lea    edx, [ebp - 27ch]
        push   ecx
        push   edx
        call   sub_3_03FD
        add    esp, 8
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_3_0E15
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 27ch]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0DE4:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_0E15
        push   4
        lea    eax, [ebp + 8]
        push   eax
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E02:
        add    esp, 18h
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E0D:
        add    esp, 4
        jmp    NEAR PTR loc_3_0F82
loc_3_0E15:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 2ch]
        mov    dword ptr [ebp - 0ch], 8
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E35:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_0E44
        cmp    byte ptr [ebp - 2ch], 31h
        jne    SHORT loc_3_0E44
        xor    edi, edi
loc_3_0E44:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_3_0F6E
        test   edi, edi
        je     NEAR PTR loc_3_0F6E
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 27ch]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E6B:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_101E
        push   0
        lea    eax, [ebp - 17ch]
        push   100h
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E8E:
        mov    dword ptr [ebp - 4], 0
        add    esp, 14h
        cmp    byte ptr [ebp - 17ch], bl
        je     SHORT loc_3_0EAF
loc_3_0EA0:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [ebp + eax - 17ch], bl
        jne    SHORT loc_3_0EA0
loc_3_0EAF:
        lea    eax, [ebp - 47ch]
        push   eax
        call   sub_3_03EC
        add    esp, 4
        inc    eax
        lea    ecx, [ebp - 47ch]
        push   eax
        push   ecx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0ED8:
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_3_1036
        lea    eax, [ebp - 47ch]
        push   eax
        call   sub_3_03EC
        add    esp, 4
        inc    eax
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        lea    edx, [ebp + ecx - 17bh]
        push   edx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F10:
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_3_1036
        push   4
        lea    eax, [ebp + 8]
        push   eax
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F32:
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_3_1036
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 10h]
        call   sub_3_08C5
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_1036
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F5B:
        add    esp, 4
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F66:
        add    esp, 4
        push   dword ptr [ebp - 8]
        jmp    SHORT loc_3_0F85
loc_3_0F6E:
        lea    eax, [ebp - 27ch]
        push   eax
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F7F:
        add    esp, 8
loc_3_0F82:
        push   dword ptr [ebp - 10h]
loc_3_0F85:
        inc    esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F8B:
        add    esp, 4
        lea    ecx, [ebp - 37ch]
        mov    dword ptr [ebp - 0ch], 100h
        push   dword ptr [ebp - 0ch]
        push   ecx
        push   esi
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0FA8:
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_3_0D40
loc_3_0FB3:
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0FBB:
        add    esp, 4
        lea    eax, [ebp - 7ch]
        mov    dword ptr [ebp - 0ch], 40h
        push   40h
        inc    dword ptr [ebp - 24h]
        push   eax
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0FD9:
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_3_0CBC
loc_3_0FE4:
        push   2
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0FFC:
        add    esp, 18h
loc_3_0FFF:
        push   dword ptr [ebp - 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1007:
        add    esp, 4
loc_3_100A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1011:
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1019:
        add    esp, 4
        jmp    SHORT loc_3_0FFF
loc_3_101E:
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1026:
        add    esp, 4
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1031:
        add    esp, 4
        jmp    SHORT loc_3_0FFF
loc_3_1036:
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_103E:
        add    esp, 4
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1049:
        add    esp, 4
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1054:
        add    esp, 4
        jmp    SHORT loc_3_0FFF
sub_3_1059:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        push   ebp
        jne    SHORT loc_3_1070
        mov    eax, 21h
        jmp    NEAR PTR loc_3_1216
loc_3_1070:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_107B:
        mov    esi, eax
        mov    ebp, dword ptr [esp + 1ch]
        test   esi, esi
        je     NEAR PTR loc_3_1146
loc_3_1089:
        cmp    dword ptr [esi], ebp
        je     SHORT loc_3_10A4
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1099:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_1089
        jmp    NEAR PTR loc_3_1146
loc_3_10A4:
        mov    edi, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 18h]
        cmp    edi, 4
        jne    SHORT loc_3_10B5
        test   ebx, ebx
        je     SHORT loc_3_10BA
loc_3_10B5:
        cmp    edi, 2
        jne    SHORT loc_3_10C3
loc_3_10BA:
        push   ebp
        call   sub_3_0714
        add    esp, 4
loc_3_10C3:
        push   dword ptr [esp + 24h]
        push   esi
        push   ebx
        push   edi
        call   sub_3_072C
        add    esp, 10h
        mov    ebx, eax
        cmp    edi, 4
        jne    SHORT loc_3_1105
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_10E6
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_10E6:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_10F2:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_10FE:
        mov    eax, ebx
        jmp    NEAR PTR loc_3_1216
loc_3_1105:
        cmp    edi, 2
        jne    SHORT loc_3_1118
        mov    dword ptr [esi + 4], 3
        mov    eax, ebx
        jmp    NEAR PTR loc_3_1216
loc_3_1118:
        cmp    edi, 1
        jne    SHORT loc_3_113F
        test   ebx, ebx
        je     SHORT loc_3_1126
        cmp    ebx, 1
        jne    SHORT loc_3_113F
loc_3_1126:
        mov    dword ptr [esi + 4], 2
        push   ebp
        call   sub_3_0B28
        add    esp, 4
        push   ebp
        call   sub_3_06E2
        add    esp, 4
loc_3_113F:
        mov    eax, ebx
        jmp    NEAR PTR loc_3_1216
loc_3_1146:
        mov    edi, dword ptr [esp + 14h]
        cmp    edi, 4
        je     NEAR PTR loc_3_1211
        cmp    edi, 0dh
        je     NEAR PTR loc_3_1211
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1167:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_1177
        mov    eax, 2
        jmp    NEAR PTR loc_3_1216
loc_3_1177:
        push   ebp
        mov    dword ptr [esi], ebp
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 4], 0
        call   sub_3_0B28
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_119C:
        push   esi
        call   sub_3_05E0
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_3_11D4
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_11B8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11B8:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11C4:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11D0:
        mov    eax, ebx
        jmp    SHORT loc_3_1216
loc_3_11D4:
        push   dword ptr [esp + 24h]
        push   esi
        push   dword ptr [esp + 20h]
        push   edi
        mov    dword ptr [esi + 4], 1
        call   sub_3_072C
        add    esp, 10h
        mov    ebx, eax
        cmp    edi, 1
        jne    SHORT loc_3_120D
        test   ebx, ebx
        je     SHORT loc_3_11FD
        cmp    ebx, 1
        jne    SHORT loc_3_120D
loc_3_11FD:
        mov    dword ptr [esi + 4], 2
        push   ebp
        call   sub_3_06E2
        add    esp, 4
loc_3_120D:
        mov    eax, ebx
        jmp    SHORT loc_3_1216
loc_3_1211:
        mov    eax, 1
loc_3_1216:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_121B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        push   esi
        push   esi
        mov    esi, dword ptr [ebp - 4]
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        mov    eax, edx
        pop    esi
        test   eax, eax
        jne    SHORT loc_3_123E
        xor    eax, eax
        jmp    SHORT loc_3_1258
loc_3_123E:
        movzx  ecx, word ptr [eax - 0dh]
        shl    ecx, 4
        test   ecx, ecx
        jne    SHORT loc_3_124D
        xor    eax, eax
        jmp    SHORT loc_3_1258
loc_3_124D:
        mov    edx, dword ptr [ebp + 8]
        test   edx, edx
        je     SHORT loc_3_1258
        add    ecx, eax
        mov    dword ptr [edx], ecx
loc_3_1258:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_125D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        push   edi
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        jecxz  loc_3_1291
        mov    edx, ecx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, edi
        xor    eax, eax
        repne scasb
        neg    ecx
        add    ecx, edx
        mov    edi, esi
        mov    esi, dword ptr [ebp + 0ch]
        repe cmpsb
        mov    al, byte ptr [esi - 1]
        xor    ecx, ecx
        cmp    al, byte ptr [edi - 1]
        ja     SHORT loc_3_128F
        je     SHORT loc_3_1291
        dec    ecx
        dec    ecx
loc_3_128F:
        not    ecx
loc_3_1291:
        mov    eax, ecx
        pop    esi
        pop    edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_3_1299:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   esi
        push   edi
        cld
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        rep movsb
        pop    edi
        pop    esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_3_12B2:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        cld
        jecxz  loc_3_12CE
loc_3_12C6:
        lodsb
        and    al, al
        je     SHORT loc_3_12CE
        stosb
        loop   loc_3_12C6
loc_3_12CE:
        mov    byte ptr [edi], 0
        pop    edi
        pop    esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_3_12D7:
        mov    edx, dword ptr [esp + 4]
        push   edi
        test   edx, edx
        jne    SHORT loc_3_12E4
        xor    eax, eax
        jmp    SHORT loc_3_1300
loc_3_12E4:
        cmp    byte ptr [edx], 0
        je     SHORT loc_3_12FD
loc_3_12E9:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    edx, ecx
        cmp    byte ptr [edx], 0
        jne    SHORT loc_3_12E9
loc_3_12FD:
        lea    eax, [edx + 1]
loc_3_1300:
        pop    edi
        ret
_MMDEVLDR_SetEnvironmentString:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   _MMDEVLDR_RemoveEnvironmentString
        mov    eax, dword ptr [esp + 10h]
        add    esp, 4
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_1323
        push   eax
        push   esi
        call   _MMDEVLDR_AddEnvironmentString
        add    esp, 8
loc_3_1323:
        pop    esi
        ret
_MMDEVLDR_GetEnvironmentString:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        call   sub_3_121B
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_3_1382
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esp + 18h]
        lea    ebp, [ecx - 1]
        mov    byte ptr [eax], 0
        cmp    byte ptr [ebx], 0
        je     SHORT loc_3_1382
loc_3_1359:
        push   ebp
        push   esi
        push   ebx
        call   sub_3_125D
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_136E
        cmp    byte ptr [ebx + ebp], 3dh
        je     SHORT loc_3_1389
loc_3_136E:
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_3_1359
loc_3_1382:
        xor    eax, eax
loc_3_1384:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_1389:
        lea    ebx, [ebp + ebx + 1]
        mov    eax, dword ptr [esp + 1ch]
        dec    eax
        push   eax
        push   ebx
        push   dword ptr [esp + 20h]
        call   sub_3_12B2
        add    esp, 0ch
        mov    eax, 1
        jmp    SHORT loc_3_1384
_MMDEVLDR_RemoveEnvironmentString:
        sub    esp, 8
        lea    eax, [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   eax
        call   sub_3_121B
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_3_1456
        mov    esi, eax
        mov    ebx, dword ptr [esp + 1ch]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esp + 10h]
        lea    ebp, [ecx - 1]
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_1456
loc_3_13E6:
        push   ebp
        push   ebx
        push   esi
        call   sub_3_125D
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_13FB
        cmp    byte ptr [esi + ebp], 3dh
        je     SHORT loc_3_1411
loc_3_13FB:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
        cmp    byte ptr [esi], 0
        jne    SHORT loc_3_13E6
        jmp    SHORT loc_3_1456
loc_3_1411:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx + esi]
        mov    ecx, dword ptr [esp + 14h]
        sub    ecx, eax
        push   ecx
        push   eax
        push   esi
        call   sub_3_1299
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        call   sub_3_12D7
        mov    edx, dword ptr [esp + 18h]
        add    esp, 4
        sub    edx, eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_3_1456:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
_MMDEVLDR_AddEnvironmentString:
        sub    esp, 4
        lea    eax, [esp]
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   eax
        call   sub_3_121B
        add    esp, 4
        mov    edx, eax
        test   edx, edx
        je     NEAR PTR loc_3_1579
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   edx
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        call   sub_3_12D7
        add    esp, 4
        mov    edx, eax
        test   edx, edx
        je     NEAR PTR loc_3_1579
        mov    ebp, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 1ch]
        sub    ebp, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ecx + esi + 1]
        cmp    ebp, ebx
        jae    SHORT loc_3_14E5
        add    esi, 3
        cmp    esi, ebp
        ja     NEAR PTR loc_3_1579
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, dword ptr [esp + 1ch]
        sub    ecx, ebx
        add    ecx, ebp
        mov    byte ptr [ecx + eax], 0
loc_3_14E5:
        dec    edx
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, edx
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, edx
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    byte ptr [edx + ecx], 0
loc_3_1579:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
_PTEXT ends
        end
