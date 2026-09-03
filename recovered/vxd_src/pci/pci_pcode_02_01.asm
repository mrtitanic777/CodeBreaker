; pci_pcode_02_01.asm -- PCI object 2 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0A1h, 000h, 000h, 000h, 000h, 08Bh, 048h, 010h, 085h, 0C9h, 074h, 010h, 06Ah, 000h, 051h, 0E8h
        db  0ECh, 078h, 000h, 000h, 083h, 0C4h, 008h, 0A1h, 000h, 000h, 000h, 000h, 08Bh, 040h, 014h, 085h
        db  0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0E8h, 0D5h, 078h, 000h, 000h, 083h, 0C4h, 008h, 0C3h
sub_2_002F:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_2_0040
        cmp    eax, 2
        je     SHORT loc_2_004A
        jmp    SHORT loc_2_005A
loc_2_0040:
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [ecx + 8]
        jmp    SHORT loc_2_0054
loc_2_004A:
        movzx  eax, word ptr [ecx + 4]
        mov    dword ptr [edx], eax
        movzx  edx, word ptr [ecx + 6]
loc_2_0054:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], edx
loc_2_005A:
        ret    4
sub_2_005D:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_2_006E
        cmp    eax, 2
        je     SHORT loc_2_0073
        jmp    SHORT loc_2_0077
loc_2_006E:
        mov    eax, dword ptr [ecx + 14h]
        jmp    SHORT loc_2_0077
loc_2_0073:
        movzx  eax, word ptr [ecx + 0eh]
loc_2_0077:
        ret
sub_2_0078:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_2_0089
        cmp    eax, 2
        je     SHORT loc_2_008E
        jmp    SHORT loc_2_0097
loc_2_0089:
        mov    eax, dword ptr [ecx + 10h]
        jmp    SHORT loc_2_0097
loc_2_008E:
        movzx  eax, word ptr [ecx + 0ch]
        sub    eax, 10000h
loc_2_0097:
        ret
sub_2_0098:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
loc_2_009E:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_2_00E5
        push   0
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_00E5
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_2_00DE
        mov    eax, dword ptr [esi]
        mov    ax, word ptr [eax + 0ch]
        and    ax, 4
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_009E
loc_2_00DE:
        mov    eax, 1
        jmp    SHORT loc_2_00FA
loc_2_00E5:
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        test   eax, eax
        je     SHORT loc_2_00F8
        mov    ecx, dword ptr [eax + 0ch]
        test   eax, eax
        mov    dword ptr [esi], ecx
        jne    SHORT loc_2_009E
loc_2_00F8:
        xor    eax, eax
loc_2_00FA:
        pop    edi
        pop    esi
        ret
sub_2_00FD:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    eax, dword ptr [ecx]
        mov    edi, ecx
        xor    esi, esi
        cmp    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebp - 0ch], esi
        mov    dword ptr [ebp - 8], esi
        jne    SHORT loc_2_0122
        mov    dword ptr [ebp - 10h], 0ffffffffh
        jmp    SHORT loc_2_012C
loc_2_0122:
        mov    ecx, dword ptr [edi]
        call   sub_2_005D
        mov    dword ptr [ebp - 10h], eax
loc_2_012C:
        test   ebx, ebx
        je     NEAR PTR loc_2_01C8
loc_2_0134:
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 4], eax
loc_2_013A:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_01BE
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        lea    edx, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_01BE
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_2_0180
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    ax, 4
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_013A
loc_2_0180:
        mov    eax, dword ptr [edi]
        cmp    dword ptr [ebp - 4], eax
        jne    SHORT loc_2_0190
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_2_013A
loc_2_0190:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_005D
        cmp    eax, dword ptr [ebp - 10h]
        ja     SHORT loc_2_013A
        jae    SHORT loc_2_01AC
        cmp    dword ptr [ebp - 0ch], eax
        jae    SHORT loc_2_013A
        mov    esi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        jmp    SHORT loc_2_013A
loc_2_01AC:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_013A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
        mov    eax, 1
        jmp    SHORT loc_2_01D0
loc_2_01BE:
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    NEAR PTR loc_2_0134
loc_2_01C8:
        mov    dword ptr [edi], esi
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
loc_2_01D0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_01D7:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_2_01E8
        cmp    eax, 2
        je     SHORT loc_2_01F4
        jmp    SHORT loc_2_0200
loc_2_01E8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], eax
        jmp    SHORT loc_2_0200
loc_2_01F4:
        mov    word ptr [ecx + 4], dx
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [ecx + 6], ax
loc_2_0200:
        ret    4
sub_2_0203:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, ecx
        test   esi, esi
        je     NEAR PTR loc_2_02A0
loc_2_0214:
        push   2
        lea    ecx, [ebp - 4]
        push   dword ptr [esi + 8]
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0296
loc_2_0229:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        lea    edx, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0296
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_2_0269
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    ax, 4
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0229
loc_2_0269:
        lea    eax, [ebp - 0ch]
        lea    edx, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_002F
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   dword ptr [eax + 14h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        call   sub_2_7910
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_0229
        jmp    SHORT loc_2_02A2
loc_2_0296:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    NEAR PTR loc_2_0214
loc_2_02A0:
        xor    eax, eax
loc_2_02A2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_02A7:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        mov    esi, ecx
        push   6
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_2_03DF
        or     byte ptr [ecx + 1ch], 4
        push   0
        push   dword ptr [ecx + 14h]
        push   -1
        push   0
        call   sub_2_7910
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_0409
        mov    eax, dword ptr [ebp - 14h]
        xor    bl, bl
        mov    dword ptr [ebp - 4], eax
loc_2_02F8:
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0353
        inc    bl
        db  03Ah, 01Dh, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_02F8
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 10h], eax
        je     SHORT loc_2_0367
        mov    ecx, dword ptr [ebp - 10h]
        and    ch, 7fh
        cmp    ecx, eax
        jne    SHORT loc_2_035D
        db  0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        push   1
        mov    dword ptr [eax + 4], ecx
        push   dword ptr [eax + 14h]
        push   -1
        push   ecx
        mov    dword ptr [eax + 0ch], ecx
        call   sub_2_7928
        add    esp, 10h
        jmp    NEAR PTR loc_2_0409
loc_2_0353:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_0409
loc_2_035D:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_0409
loc_2_0367:
        lea    eax, [ebp - 8]
        lea    edx, [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_002F
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [ebp - 0ch]
        inc    eax
        mov    dword ptr [esi + 4], eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_0078
        not    eax
        inc    eax
        mov    dword ptr [esi + 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_03D8
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        push   dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 0ch]
        dec    eax
        push   eax
        push   0
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_0409
        cmp    dword ptr [ebp - 8], -1
        je     SHORT loc_2_0409
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        push   dword ptr [eax + 14h]
        push   -1
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        push   eax
        call   sub_2_7928
        add    esp, 10h
        jmp    SHORT loc_2_0409
loc_2_03D8:
        mov    eax, 13h
        jmp    SHORT loc_2_0409
loc_2_03DF:
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 8]
        push   0
        mov    dword ptr [ecx + 4], eax
        push   dword ptr [ecx + 14h]
        push   dword ptr [ecx + 10h]
        and    byte ptr [ecx + 1ch], 0fbh
        mov    dword ptr [ecx + 0ch], edx
        call   sub_2_7920
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0409
        mov    ecx, esi
        call   sub_2_0203
loc_2_0409:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_040F:
        sub    esp, 0cch
        lea    eax, [esp]
        push   0
        push   0c9h
        push   eax
        push   ecx
        call   sub_2_7930
        lea    eax, [esp + 10h]
        add    esp, 10h
        db  0B9h, 000h, 000h, 000h, 000h
loc_2_0433:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_2_0453
        or     dl, dl
        je     SHORT loc_2_044F
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_2_0453
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_2_0433
loc_2_044F:
        xor    eax, eax
        jmp    SHORT loc_2_0458
loc_2_0453:
        sbb    eax, eax
        sbb    eax, -1
loc_2_0458:
        test   eax, eax
        mov    eax, 1
        je     SHORT loc_2_0463
        xor    eax, eax
loc_2_0463:
        add    esp, 0cch
        ret
sub_2_046A:
        xor    eax, eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    edx, 1
        je     SHORT loc_2_047E
        cmp    edx, 2
        je     SHORT loc_2_04A4
        jmp    SHORT loc_2_04D1
loc_2_047E:
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx + 8]
        cmp    edx, 0a0000h
        jne    SHORT loc_2_0494
        cmp    ecx, 0affffh
        je     SHORT loc_2_04CC
loc_2_0494:
        cmp    edx, 0b0000h
        jne    SHORT loc_2_04D1
        cmp    ecx, 0bffffh
        jmp    SHORT loc_2_04CA
loc_2_04A4:
        movzx  edx, word ptr [ecx + 4]
        movzx  ecx, word ptr [ecx + 6]
        cmp    edx, 3b0h
        jne    SHORT loc_2_04BC
        cmp    ecx, 3bbh
        je     SHORT loc_2_04CC
loc_2_04BC:
        cmp    edx, 3c0h
        jne    SHORT loc_2_04D1
        cmp    ecx, 3dfh
loc_2_04CA:
        jne    SHORT loc_2_04D1
loc_2_04CC:
        mov    eax, 1
loc_2_04D1:
        ret
sub_2_04D2:
        push   esi
        xor    eax, eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_0525
        movzx  esi, word ptr [ecx + 4]
        movzx  edx, word ptr [ecx + 6]
        cmp    esi, 1f0h
        jne    SHORT loc_2_04F6
        cmp    edx, 1f7h
        je     SHORT loc_2_0520
loc_2_04F6:
        mov    ecx, 3f6h
        cmp    esi, ecx
        jne    SHORT loc_2_0503
        cmp    edx, ecx
        je     SHORT loc_2_0520
loc_2_0503:
        cmp    esi, 170h
        jne    SHORT loc_2_0513
        cmp    edx, 177h
        je     SHORT loc_2_0520
loc_2_0513:
        mov    ecx, 376h
        cmp    esi, ecx
        jne    SHORT loc_2_0525
        cmp    edx, ecx
        jne    SHORT loc_2_0525
loc_2_0520:
        mov    eax, 1
loc_2_0525:
        pop    esi
        ret
sub_2_0527:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        call   sub_2_02A7
        test   eax, eax
        jne    NEAR PTR loc_2_061D
        mov    eax, dword ptr [esi + 0ch]
        lea    ecx, [ebp - 4]
        mov    edx, esi
        mov    dword ptr [ebp - 4], eax
        call   sub_2_00FD
        test   eax, eax
        je     NEAR PTR loc_2_061B
loc_2_0557:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_046A
        test   eax, eax
        jne    NEAR PTR loc_2_0609
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_04D2
        test   eax, eax
        jne    NEAR PTR loc_2_0609
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_005D
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], 0ffffffffh
        mov    ebx, 100h
        call   sub_2_0078
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
loc_2_059E:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   dword ptr [ecx + 14h]
        push   -1
        inc    eax
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0ch]
        push   edi
        push   eax
        push   ecx
        call   sub_2_7938
        add    esp, 1ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_2_061D
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_05E3
        test   byte ptr [ecx + 1ch], 2
        je     SHORT loc_2_05E3
        cmp    edi, ebx
        ja     SHORT loc_2_05E3
        mov    eax, dword ptr [ebp - 8]
        and    eax, 3ffh
        cmp    eax, ebx
        jae    SHORT loc_2_059E
loc_2_05E3:
        mov    eax, dword ptr [ebp - 8]
        push   1
        push   dword ptr [ecx + 14h]
        lea    edi, [eax + edi - 1]
        push   edi
        push   eax
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_061D
        push   edi
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_01D7
loc_2_0609:
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_00FD
        test   eax, eax
        jne    NEAR PTR loc_2_0557
loc_2_061B:
        xor    eax, eax
loc_2_061D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0624:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    dword ptr [ebp - 8], ecx
        mov    esi, edx
        call   sub_2_02A7
        test   eax, eax
        jne    NEAR PTR loc_2_06BE
        mov    eax, dword ptr [ebp - 8]
        lea    edx, [ebp - 8]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        call   sub_2_0098
        test   eax, eax
        je     SHORT loc_2_06BC
loc_2_0655:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_046A
        test   eax, eax
        jne    SHORT loc_2_06AD
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_04D2
        test   eax, eax
        jne    SHORT loc_2_06AD
        lea    eax, [ebp - 10h]
        lea    edx, [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_002F
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    esi, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   dword ptr [ecx + 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_06BE
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ebp - 10h]
        call   sub_2_01D7
loc_2_06AD:
        lea    edx, [ebp - 8]
        lea    ecx, [ebp - 4]
        call   sub_2_0098
        test   eax, eax
        jne    SHORT loc_2_0655
loc_2_06BC:
        xor    eax, eax
loc_2_06BE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_06C3:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx + 0ch]
        push   0
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [edx + 10h]
        push   ecx
        mov    dword ptr [edx + 10h], eax
        push   -1
        mov    dword ptr [edx + 14h], ecx
        push   0
        call   sub_2_7910
        add    esp, 10h
        xor    eax, eax
        ret
sub_2_06F2:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 0ch
        test   byte ptr [eax + 3], 2
        push   esi
        je     SHORT loc_2_072D
        mov    esi, ecx
        push   0
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_7948
        add    esp, 0ch
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7948
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0732
loc_2_072D:
        xor    esi, esi
        mov    dword ptr [ebp - 4], ecx
loc_2_0732:
        movzx  eax, byte ptr [eax + 1]
        cmp    eax, 0fdh
        je     SHORT loc_2_0746
        cmp    eax, 0feh
        je     SHORT loc_2_077A
        jmp    SHORT loc_2_07A1
loc_2_0746:
        xor    eax, eax
        lea    ecx, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 0ch], eax
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   ecx
        push   dword ptr [ebp - 4]
        call   sub_2_7940
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_07A1
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_2_07A6
loc_2_077A:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_040F
        test   eax, eax
        jne    SHORT loc_2_07A1
        mov    ecx, esi
        call   sub_2_046A
        test   eax, eax
        jne    SHORT loc_2_07A1
        mov    ecx, esi
        call   sub_2_04D2
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_07A6
loc_2_07A1:
        mov    eax, 13h
loc_2_07A6:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_07AB:
        push   ebp
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   edx, edx
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_0878
        db  00Fh, 0B6h, 005h, 000h, 000h, 000h, 000h
        imul   eax, eax, 2ch
        lea    edx, [eax + edx + 4]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 20h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 1ch]
        and    eax, 1
        cmp    dword ptr [edx + 10h], 0
        db  0A3h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_07F8
        xor    eax, eax
        jmp    NEAR PTR loc_2_087D
loc_2_07F8:
        db  089h, 015h, 000h, 000h, 000h, 000h
        cmp    ecx, 7
        ja     SHORT loc_2_080A
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_2_080A:
        mov    eax, 1
        jmp    SHORT loc_2_087D
loc_2_0811:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        call   sub_2_0527
        jmp    SHORT loc_2_087D
loc_2_081D:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        call   sub_2_0624
        jmp    SHORT loc_2_087D
loc_2_082B:
        call   sub_2_06C3
        jmp    SHORT loc_2_087D
loc_2_0832:
        push   0
        db  089h, 015h, 000h, 000h, 000h, 000h
        push   dword ptr [edx + 14h]
        push   -1
        push   0
        call   sub_2_7910
        add    esp, 10h
        xor    eax, eax
        jmp    SHORT loc_2_087D
loc_2_084D:
        call   0
        db  0A1h, 000h, 000h, 000h, 000h
        dec    byte ptr [eax]
        xor    eax, eax
        jmp    SHORT loc_2_087D
loc_2_085D:
        mov    edx, 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        call   sub_2_0624
        jmp    SHORT loc_2_087D
loc_2_086E:
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_2_06F2
        jmp    SHORT loc_2_087D
loc_2_0878:
        mov    eax, 13h
loc_2_087D:
        pop    ebp
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_08A1:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        db  0A3h, 000h, 000h, 000h, 000h
        sub    esp, 4
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, 1
        cmp    al, bl
        je     SHORT loc_2_08FD
loc_2_08C2:
        test   esi, esi
        je     SHORT loc_2_08CB
        cmp    esi, 1
        jne    SHORT loc_2_08FD
loc_2_08CB:
        movzx  ecx, bl
        db  0A1h, 000h, 000h, 000h, 000h
        imul   ecx, ecx, 2ch
        cmp    dword ptr [ecx + eax + 24h], 2
        jne    SHORT loc_2_08F6
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        call   sub_2_07AB
        mov    esi, eax
loc_2_08F6:
        inc    bl
        cmp    byte ptr [ebp - 1], bl
        ja     SHORT loc_2_08C2
loc_2_08FD:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_0912
        push   0
        push   eax
        call   sub_2_7950
        add    esp, 8
loc_2_0912:
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_091A:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        db  0A3h, 000h, 000h, 000h, 000h
        sub    esp, 4
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, 1
        cmp    al, bl
        je     SHORT loc_2_0976
loc_2_093B:
        test   esi, esi
        je     SHORT loc_2_0944
        cmp    esi, 1
        jne    SHORT loc_2_0976
loc_2_0944:
        movzx  ecx, bl
        db  0A1h, 000h, 000h, 000h, 000h
        imul   ecx, ecx, 2ch
        cmp    dword ptr [ecx + eax + 24h], 1
        jne    SHORT loc_2_096F
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        call   sub_2_07AB
        mov    esi, eax
loc_2_096F:
        inc    bl
        cmp    byte ptr [ebp - 1], bl
        ja     SHORT loc_2_093B
loc_2_0976:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_098B
        push   0
        push   eax
        call   sub_2_7950
        add    esp, 8
loc_2_098B:
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0993:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        mov    esi, ecx
        push   0
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_09B3
        xor    eax, eax
        jmp    SHORT loc_2_09D9
loc_2_09B3:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_09D7
        mov    esi, dword ptr [ebp - 4]
        push   0
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_09B3
        xor    eax, eax
        jmp    SHORT loc_2_09D9
loc_2_09D7:
        mov    eax, esi
loc_2_09D9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_09DE:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 2], cl
        mov    bl, 0ffh
        push   0
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0A3D
loc_2_0A02:
        push   0
        lea    eax, [ebp - 3]
        push   eax
        lea    edx, [ebp - 1]
        push   0
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        mov    al, byte ptr [ebp - 2]
        cmp    al, byte ptr [ebp - 1]
        jae    SHORT loc_2_0A28
        dec    byte ptr [ebp - 1]
        cmp    bl, byte ptr [ebp - 1]
        jbe    SHORT loc_2_0A28
        mov    bl, byte ptr [ebp - 1]
loc_2_0A28:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0A02
loc_2_0A3D:
        mov    al, bl
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0A44:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        lea    edx, [ebp - 3]
        mov    esi, ecx
        push   0
        push   eax
        xor    bl, bl
        push   edx
        lea    edx, [ebp - 2]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_2_0A88
        mov    bl, 1
        push   0
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0A88
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0A44
        lea    ebx, [eax + 1]
loc_2_0A88:
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        and    byte ptr [ebp - 1], 0f7h
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 3]
        mov    ecx, esi
        call   sub_2_426A
        push   0
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0ABA
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0A44
        add    bl, al
loc_2_0ABA:
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0AC2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        call   sub_2_0993
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        jne    SHORT loc_2_0ADD
        xor    eax, eax
        jmp    NEAR PTR loc_2_0B6E
loc_2_0ADD:
        xor    esi, esi
        lea    edx, [ebp - 1]
        push   esi
        mov    ecx, dword ptr [ebp - 0ch]
        push   esi
        push   esi
        call   sub_2_4212
        mov    bl, byte ptr [ebp - 1]
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0B61
loc_2_0B07:
        lea    eax, [ebp - 3]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 5]
        push   ecx
        push   edx
        lea    edx, [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_4212
        mov    cl, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 1], cl
        jne    SHORT loc_2_0B4C
        test   byte ptr [ebp - 2], 4
        je     SHORT loc_2_0B4C
        test   byte ptr [ebp - 2], 8
        jne    SHORT loc_2_0B74
        mov    al, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp - 4]
        or     al, 8
        mov    ecx, dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 3]
        push   eax
        push   dword ptr [ebp - 5]
        call   sub_2_426A
        add    bl, byte ptr [ebp - 3]
loc_2_0B4C:
        push   0
        lea    ecx, [ebp - 0ch]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0B07
loc_2_0B61:
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_09DE
        cmp    al, bl
        sbb    eax, eax
        inc    eax
loc_2_0B6E:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0B74:
        mov    eax, 1
        jmp    SHORT loc_2_0B6E
sub_2_0B7B:
        push   esi
        test   cl, cl
        je     SHORT loc_2_0B9B
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_0B9B
loc_2_0B8F:
        cmp    byte ptr [esi], cl
        jne    SHORT loc_2_0B95
        mov    byte ptr [esi], dl
loc_2_0B95:
        add    esi, 10h
        dec    eax
        jne    SHORT loc_2_0B8F
loc_2_0B9B:
        pop    esi
        ret
sub_2_0B9D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        test   edx, edx
        push   esi
        mov    esi, ecx
        jne    SHORT loc_2_0BC7
        push   0
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0BC7
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0B9D
loc_2_0BC7:
        push   0
        lea    eax, [ebp - 9]
        push   eax
        lea    ecx, [ebp - 1]
        push   ecx
        lea    edx, [ebp - 5]
        mov    ecx, esi
        call   sub_2_4212
        test   byte ptr [ebp - 9], 4
        je     SHORT loc_2_0C09
        push   0
        lea    eax, [ebp - 0ch]
        push   esi
        push   eax
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0BFE
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_0B9D
loc_2_0BFE:
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 5]
        call   sub_2_5868
loc_2_0C09:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0C0E:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    bl, dl
        mov    edi, ecx
        xor    esi, esi
loc_2_0C1D:
        lea    eax, [ebp - 3]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 1]
        push   ecx
        push   edx
        lea    edx, [ebp - 4]
        mov    ecx, edi
        call   sub_2_4212
        test   byte ptr [ebp - 2], 11h
        je     NEAR PTR loc_2_0D5C
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_2_0C4B
        cmp    bl, byte ptr [ebp - 4]
        jne    NEAR PTR loc_2_0CFF
loc_2_0C4B:
        test   byte ptr [ebp - 2], 10h
        jne    NEAR PTR loc_2_0CFF
        mov    al, byte ptr [ebp - 2]
        mov    edx, ebx
        and    al, 0f7h
        mov    ecx, edi
        push   dword ptr [ebp - 3]
        push   eax
        push   dword ptr [ebp - 1]
        call   sub_2_426A
        mov    word ptr [ebp - 0ah], si
        mov    cl, byte ptr [ebp - 1]
        push   esi
        lea    edx, [ebp - 0ch]
        push   edx
        mov    byte ptr [ebp - 0ch], bl
        push   4
        mov    byte ptr [ebp - 0bh], cl
        push   4
        push   edi
        call   sub_2_7978
        add    esp, 14h
        test   byte ptr [ebp - 2], 4
        je     SHORT loc_2_0CFF
        push   ebx
        mov    edx, dword ptr [ebp - 1]
        push   18h
        mov    ecx, ebx
        call   sub_2_58EF
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        push   dword ptr [ebp + 8]
        push   19h
        call   sub_2_58EF
        mov    al, byte ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 1]
        add    al, byte ptr [ebp - 3]
        mov    ecx, ebx
        dec    al
        push   eax
        push   1ah
        call   sub_2_58EF
        push   2
        lea    ecx, [ebp - 8]
        push   edi
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0CFF
        push   esi
        lea    ecx, [ebp - 8]
        push   esi
        push   43h
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0CFF
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx]
        call   sub_2_0B7B
        mov    ecx, dword ptr [ebp - 8]
        mov    dl, byte ptr [ebp + 8]
        mov    byte ptr [ecx], dl
loc_2_0CFF:
        push   esi
        lea    eax, [ebp - 14h]
        push   edi
        push   eax
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0D3C
        push   esi
        lea    eax, [ebp - 18h]
        push   edi
        push   esi
        push   eax
        call   sub_2_7970
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_0D3C
        test   byte ptr [ebp - 16h], 1
        jne    SHORT loc_2_0D3C
        push   esi
        mov    al, byte ptr [ebp + 8]
        inc    al
        mov    edx, dword ptr [ebp + 8]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        call   sub_2_0C0E
loc_2_0D3C:
        push   esi
        lea    eax, [ebp - 10h]
        push   edi
        push   eax
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0D5C
        mov    al, byte ptr [ebp - 3]
        mov    edi, dword ptr [ebp - 10h]
        add    byte ptr [ebp + 8], al
        jmp    NEAR PTR loc_2_0C1D
loc_2_0D5C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_0D65:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   0
        mov    esi, ecx
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_0E55
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0A44
        test   esi, esi
        je     SHORT loc_2_0DAC
loc_2_0D96:
        mov    ecx, dword ptr [esi + 8]
        call   sub_2_0AC2
        test   eax, eax
        je     NEAR PTR loc_2_0E5C
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_2_0D96
loc_2_0DAC:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0DFD
        xor    esi, esi
loc_2_0DC6:
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        xor    edx, edx
        push   esi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_2_0DE9
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0B9D
loc_2_0DE9:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0DC6
loc_2_0DFD:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0E55
        xor    esi, esi
loc_2_0E17:
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        lea    edx, [ebp - 2]
        push   esi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_2_0E41
        push   1
        mov    al, byte ptr [ebp - 2]
        inc    al
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0C0E
loc_2_0E41:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0E17
loc_2_0E55:
        xor    eax, eax
loc_2_0E57:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0E5C:
        mov    eax, 13h
        jmp    SHORT loc_2_0E57
sub_2_0E63:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 1]
        push   eax
        xor    edx, edx
        push   ebx
        call   sub_2_4212
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_2_0EC1
        push   2
        lea    eax, [ebp - 8]
        push   edi
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0EC1
        push   ebx
        lea    ecx, [ebp - 8]
        push   ebx
        push   43h
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0EC1
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        test   al, al
        je     SHORT loc_2_0EC1
        mov    byte ptr [esi], al
        mov    eax, 1
        jmp    SHORT loc_2_0EC3
loc_2_0EC1:
        xor    eax, eax
loc_2_0EC3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0ECA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    edx, [ebp - 1]
        push   esi
        xor    bl, bl
        mov    esi, ecx
        call   sub_2_0E63
        test   eax, eax
        je     SHORT loc_2_0F1E
        mov    bl, byte ptr [ebp - 1]
        push   0
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0F1E
loc_2_0EF8:
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0ECA
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        jbe    SHORT loc_2_0F09
        mov    bl, al
loc_2_0F09:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0EF8
loc_2_0F1E:
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0F26:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        mov    bl, dl
        mov    esi, ecx
        push   eax
        lea    edx, [ebp - 1]
        push   eax
        push   eax
        call   sub_2_4212
        cmp    byte ptr [ebp - 1], bl
        jae    NEAR PTR loc_2_108A
        push   0
        lea    eax, [ebp - 10h]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_108A
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_108A
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_09DE
        cmp    al, bl
        jb     NEAR PTR loc_2_108A
        mov    byte ptr [ebp - 2], 0
loc_2_0F8C:
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_2_0FB5
        lea    edx, [ebp - 1]
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0E63
        test   eax, eax
        je     SHORT loc_2_0FB5
        cmp    byte ptr [ebp - 1], bl
        ja     SHORT loc_2_0FB5
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0ECA
        cmp    al, byte ptr [ebp - 2]
        jbe    SHORT loc_2_0FB5
        mov    byte ptr [ebp - 2], al
loc_2_0FB5:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0F8C
        cmp    byte ptr [ebp - 2], bl
        jae    NEAR PTR loc_2_108A
        push   0
        mov    dword ptr [ebp - 0ch], esi
        push   esi
        lea    esi, [ebp - 0ch]
        push   esi
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_1083
        xor    esi, esi
loc_2_0FEF:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], eax
        je     NEAR PTR loc_2_1083
        push   esi
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_108A
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_108A
        lea    edx, [ebp - 2]
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_0E63
        test   eax, eax
        je     SHORT loc_2_108A
loc_2_1034:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0ch], eax
        je     SHORT loc_2_1056
        lea    edx, [ebp - 1]
        mov    ecx, eax
        call   sub_2_0E63
        test   eax, eax
        je     SHORT loc_2_1056
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        ja     SHORT loc_2_1056
        cmp    al, bl
        jbe    SHORT loc_2_108A
loc_2_1056:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_1034
        push   esi
        lea    ecx, [ebp - 0ch]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_0FEF
loc_2_1083:
        mov    eax, 1
        jmp    SHORT loc_2_108C
loc_2_108A:
        xor    eax, eax
loc_2_108C:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1092:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, ecx
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_2_10DA
loc_2_10A2:
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_2_10DA
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   43h
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_10D4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edi + 8]
        mov    dl, byte ptr [eax]
        call   sub_2_0F26
        test   eax, eax
        je     SHORT loc_2_10DA
loc_2_10D4:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_2_10A2
loc_2_10DA:
        test   edi, edi
        jne    SHORT loc_2_10E2
        xor    eax, eax
        jmp    SHORT loc_2_1138
loc_2_10E2:
        mov    ecx, esi
        call   sub_2_0D65
        test   eax, eax
        jne    SHORT loc_2_1133
        test   esi, esi
        je     SHORT loc_2_112F
loc_2_10F1:
        push   2
        lea    ecx, [ebp - 4]
        push   dword ptr [esi + 8]
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_1129
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   43h
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_1129
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0ch]
        mov    cl, byte ptr [eax]
        mov    byte ptr [edx], cl
loc_2_1129:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_2_10F1
loc_2_112F:
        xor    eax, eax
        jmp    SHORT loc_2_1138
loc_2_1133:
        mov    eax, 13h
loc_2_1138:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_113E:
        push   ebp
        mov    eax, dword ptr [ecx + 0ch]
        mov    ebp, esp
        sub    esp, 3ch
        test   eax, eax
        push   ebx
        push   esi
        mov    dword ptr [ebp - 20h], eax
        push   edi
        je     NEAR PTR loc_2_1590
        mov    esi, dword ptr [ecx + 8]
        push   0
        push   0
        lea    ecx, [ebp - 20h]
        push   43h
        push   dword ptr [ebp - 20h]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_117B
        mov    eax, 1
        jmp    NEAR PTR loc_2_1592
loc_2_117B:
        xor    ebx, ebx
        push   2
        push   esi
        lea    eax, [ebp - 34h]
        push   eax
        mov    byte ptr [ebp - 3], bl
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_11D2
        push   ebx
        lea    ecx, [ebp - 34h]
        push   ebx
        push   43h
        push   dword ptr [ebp - 34h]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_11D2
        mov    eax, dword ptr [ebp - 34h]
        mov    cl, byte ptr [eax]
        mov    byte ptr [ebp - 3], cl
        mov    ecx, esi
        mov    edx, dword ptr [ebp - 3]
        call   sub_2_0F26
        test   eax, eax
        je     SHORT loc_2_11D2
        mov    eax, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ebp - 3]
        mov    byte ptr [eax], cl
        mov    eax, 1
        jmp    NEAR PTR loc_2_1592
loc_2_11D2:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_2_1590
        xor    edi, edi
        lea    edx, [ebp - 1]
        push   edi
        mov    ecx, esi
        push   edi
        push   edi
        call   sub_2_4212
        movzx  ecx, byte ptr [ebp - 1]
        push   1
        push   dword ptr [ebp - 0ch]
        push   ecx
        push   edi
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_1214
        push   edi
        jmp    NEAR PTR loc_2_1585
loc_2_1214:
        push   1
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 0ch]
        push   100h
        call   sub_2_09DE
        movzx  eax, al
        inc    eax
        push   eax
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_1583
        xor    edi, edi
        xor    ebx, ebx
        mov    dword ptr [ebp - 1ch], esi
loc_2_1242:
        push   ebx
        lea    ecx, [ebp - 1ch]
        push   dword ptr [ebp - 1ch]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        db  0A1h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_1307
        cmp    eax, dword ptr [ebp - 1ch]
        je     NEAR PTR loc_2_1307
        push   ebx
        lea    ecx, [ebp - 14h]
        push   dword ptr [ebp - 1ch]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_1583
        push   ebx
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_1583
        lea    edx, [ebp - 7]
        mov    ecx, dword ptr [ebp - 1ch]
        call   sub_2_0E63
        test   eax, eax
        je     NEAR PTR loc_2_1583
loc_2_12AB:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 1ch], eax
        je     SHORT loc_2_12EB
        lea    edx, [ebp - 4]
        mov    ecx, eax
        call   sub_2_0E63
        test   eax, eax
        je     SHORT loc_2_12EB
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 7], al
        ja     SHORT loc_2_12EB
        mov    edi, 1
        push   ebx
        push   dword ptr [ebp - 0ch]
        push   0ffh
        movzx  ecx, al
        push   ecx
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_1583
loc_2_12EB:
        push   ebx
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_12AB
        test   edi, edi
        je     NEAR PTR loc_2_1242
loc_2_1307:
        push   0
        lea    eax, [ebp - 14h]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_1583
        push   0
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_1583
loc_2_1337:
        cmp    dword ptr [ebp - 10h], esi
        je     SHORT loc_2_1371
        lea    edx, [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        call   sub_2_0E63
        test   eax, eax
        je     SHORT loc_2_1371
        mov    ecx, dword ptr [ebp - 10h]
        call   sub_2_0ECA
        movzx  eax, al
        movzx  edx, byte ptr [ebp - 4]
        push   1
        push   dword ptr [ebp - 0ch]
        push   eax
        push   edx
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_1583
loc_2_1371:
        push   0
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_1337
        push   0
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_2_1583
        xor    edi, edi
        push   edi
        push   0ffh
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 0ch]
        call   sub_2_7990
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_13C8
        push   edi
        push   dword ptr [ebp - 28h]
        call   sub_2_7900
        add    esp, 8
        push   edi
        jmp    NEAR PTR loc_2_1585
loc_2_13C8:
        push   0
        lea    eax, [ebp - 38h]
        push   eax
        lea    ecx, [ebp - 24h]
        push   ecx
        lea    edx, [ebp - 18h]
        push   edx
        push   dword ptr [ebp - 28h]
        call   sub_2_7988
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_1576
        mov    ebx, dword ptr [ebp - 18h]
        mov    edi, dword ptr [ebp - 24h]
        push   0
        lea    ecx, [ebp - 24h]
        push   ecx
        lea    eax, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 38h]
        push   edx
        mov    dword ptr [ebp - 3ch], ebx
        call   sub_2_7980
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_143E
loc_2_140C:
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 24h]
        sub    eax, dword ptr [ebp - 3ch]
        sub    ecx, dword ptr [ebp - 18h]
        cmp    eax, ecx
        jae    SHORT loc_2_1424
        mov    eax, dword ptr [ebp - 18h]
        mov    edi, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 3ch], eax
loc_2_1424:
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        lea    ecx, [ebp - 18h]
        push   ecx
        lea    edx, [ebp - 38h]
        push   edx
        call   sub_2_7980
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_140C
loc_2_143E:
        mov    eax, dword ptr [ebp - 20h]
        push   0
        push   esi
        lea    ecx, [ebp - 2ch]
        push   ecx
        mov    byte ptr [eax], bl
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_14B4
        xor    edi, edi
loc_2_1458:
        lea    eax, [ebp - 6]
        lea    ecx, [ebp - 5]
        push   eax
        lea    edx, [ebp - 2]
        push   ecx
        push   edx
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 2ch]
        call   sub_2_4212
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 5]
        push   dword ptr [ebp - 2]
        call   sub_2_426A
        mov    word ptr [ebp - 2eh], di
        mov    cl, byte ptr [ebp - 2]
        push   edi
        lea    edx, [ebp - 30h]
        push   edx
        mov    byte ptr [ebp - 30h], bl
        push   4
        mov    byte ptr [ebp - 2fh], cl
        push   4
        push   esi
        call   sub_2_7978
        add    esp, 14h
        lea    eax, [ebp - 2ch]
        push   edi
        push   dword ptr [ebp - 2ch]
        push   eax
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_1458
loc_2_14B4:
        push   0
        lea    eax, [ebp - 2]
        push   0
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, esi
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 1]
        push   18h
        call   sub_2_58EF
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        push   19h
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58EF
        mov    ecx, esi
        call   sub_2_0ECA
        cmp    al, bl
        ja     SHORT loc_2_14F1
        mov    al, bl
loc_2_14F1:
        push   eax
        mov    edx, dword ptr [ebp - 2]
        push   1ah
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58EF
        push   0
        mov    dword ptr [ebp - 14h], esi
        push   esi
        lea    esi, [ebp - 14h]
        push   esi
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_1565
        xor    esi, esi
loc_2_1517:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 14h]
        je     SHORT loc_2_1565
        push   esi
        lea    eax, [ebp - 2]
        push   esi
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        call   sub_2_4212
        push   1ah
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        cmp    al, bl
        jae    SHORT loc_2_1565
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        push   1ah
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58EF
        push   esi
        lea    ecx, [ebp - 14h]
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_1517
loc_2_1565:
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 3]
        call   sub_2_0B7B
        mov    eax, 1
        jmp    SHORT loc_2_1592
loc_2_1576:
        push   0
        push   dword ptr [ebp - 28h]
        call   sub_2_7900
        add    esp, 8
loc_2_1583:
        push   0
loc_2_1585:
        push   dword ptr [ebp - 0ch]
        call   sub_2_7900
        add    esp, 8
loc_2_1590:
        xor    eax, eax
loc_2_1592:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1599:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, ecx
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_2_15BA
loc_2_15A9:
        mov    ecx, edi
        call   sub_2_113E
        test   eax, eax
        je     SHORT loc_2_15BA
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_2_15A9
loc_2_15BA:
        test   edi, edi
        jne    SHORT loc_2_15C2
        xor    eax, eax
        jmp    SHORT loc_2_1624
loc_2_15C2:
        mov    ecx, esi
        call   sub_2_0D65
        test   eax, eax
        jne    SHORT loc_2_161F
        test   esi, esi
        je     SHORT loc_2_161B
        xor    edi, edi
loc_2_15D3:
        push   edi
        lea    eax, [ebp - 2]
        push   edi
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [esi + 8]
        call   sub_2_4212
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], eax
        cmp    eax, edi
        je     SHORT loc_2_161B
        push   edi
        lea    ecx, [ebp - 8]
        push   edi
        push   43h
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_1615
        push   19h
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
loc_2_1615:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_2_15D3
loc_2_161B:
        xor    eax, eax
        jmp    SHORT loc_2_1624
loc_2_161F:
        mov    eax, 13h
loc_2_1624:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_162A:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_2_1643
        cmp    eax, 1
        je     SHORT loc_2_1650
        cmp    eax, 6
        je     SHORT loc_2_1650
        mov    eax, 1
        jmp    SHORT loc_2_165B
loc_2_1643:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        call   sub_2_1599
        jmp    SHORT loc_2_165B
loc_2_1650:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        jmp    NEAR PTR loc_2_1092
loc_2_165B:
        ret
sub_2_165C:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        db  0B8h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1684
        mov    edx, eax
        mov    ecx, dword ptr [esp + 4]
loc_2_1670:
        cmp    dword ptr [eax], ecx
        jne    SHORT loc_2_167A
        cmp    byte ptr [eax + 0ch], 0
        je     SHORT loc_2_1687
loc_2_167A:
        add    edx, 0eh
        mov    eax, edx
        cmp    dword ptr [edx], -1
        jne    SHORT loc_2_1670
loc_2_1684:
        xor    eax, eax
loc_2_1686:
        ret
loc_2_1687:
        cmp    byte ptr [esp + 8], 0
        je     SHORT loc_2_1686
        mov    byte ptr [eax + 0ch], 1
        jmp    SHORT loc_2_1686
sub_2_1694:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        push   0
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_16DA
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   3000h
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_17B4
loc_2_16DA:
        xor    ebx, ebx
        push   ebx
        push   dword ptr [ebp + 8]
        push   edi
        push   esi
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_2_79A0
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_17B4
        db  00Fh, 0B6h, 005h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        jl     SHORT loc_2_174F
        cmp    eax, 2
        jg     SHORT loc_2_174F
        push   ebx
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79B0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_1737
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   0f000h
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_17B4
loc_2_1737:
        push   0
        push   dword ptr [ebp + 8]
        push   edi
        push   esi
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_17B4
loc_2_174F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_17B4
        push   3
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_178A
        push   3
        db  0A1h, 000h, 000h, 000h, 000h
        push   1
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_17B4
loc_2_178A:
        push   0
        mov    eax, 0
        push   dword ptr [ebp + 8]
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        adc    eax, -1
        and    eax, 8000h
        or     eax, esi
        push   eax
        push   dword ptr [ebp - 4]
        push   0
        call   sub_2_79A0
        add    esp, 18h
loc_2_17B4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_17BD:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 4], dx
        cmp    ecx, 1
        adc    eax, -1
        push   8
        and    ax, 3
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 2], 0
        movzx  cx, byte ptr [ebp + 8]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
        mov    ecx, 4
        call   sub_2_1694
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_17FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        mov    word ptr [ebp - 18h], cx
        push   esi
        mov    word ptr [ebp - 1ch], 1
        mov    word ptr [ebp - 1ah], 0ch
        mov    byte ptr [ebp - 12h], 0ffh
        mov    si, dx
        xor    edx, edx
        mov    word ptr [ebp - 14h], dx
        mov    byte ptr [ebp - 11h], dl
        mov    dx, word ptr [ebp + 8]
        lea    eax, [esi + ecx]
        dec    ax
        mov    word ptr [ebp - 16h], ax
        cmp    byte ptr [ebp + 0ch], 0ffh
        mov    word ptr [ebp - 10h], dx
        mov    word ptr [ebp - 0eh], si
        jne    SHORT loc_2_184A
        mov    word ptr [ebp - 0ch], cx
        mov    word ptr [ebp - 0ah], ax
        jmp    SHORT loc_2_1856
loc_2_184A:
        mov    word ptr [ebp - 0ch], 0
        mov    word ptr [ebp - 0ah], 0ffffh
loc_2_1856:
        xor    eax, eax
        push   1ch
        mov    word ptr [ebp - 8], ax
        lea    edx, [ebp - 1ch]
        cmp    si, 1
        mov    byte ptr [ebp - 5], al
        mov    byte ptr [ebp - 6], 0ffh
        movzx  ecx, byte ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], ecx
        sbb    ecx, ecx
        and    ecx, 8000h
        mov    cl, 2
        call   sub_2_1694
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_1889:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        mov    word ptr [ebp - 28h], 1
        push   esi
        mov    dword ptr [ebp - 24h], ecx
        lea    eax, [ecx + edx]
        mov    esi, edx
        dec    eax
        mov    ecx, 0
        cmp    dword ptr [ebp + 10h], 1
        mov    word ptr [ebp - 26h], 14h
        adc    ecx, -1
        mov    dword ptr [ebp - 20h], eax
        and    cx, 4
        xor    edx, edx
        mov    word ptr [ebp - 1ah], dx
        or     cl, 1
        mov    word ptr [ebp - 1ch], cx
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 8], cx
        mov    dword ptr [ebp - 18h], eax
        mov    word ptr [ebp - 6], dx
        mov    eax, dword ptr [ebp + 14h]
        push   28h
        mov    dword ptr [ebp - 14h], esi
        lea    edx, [ebp - 28h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 18h]
        cmp    esi, 1
        movzx  ecx, byte ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], ecx
        sbb    ecx, ecx
        and    ecx, 8000h
        mov    cl, 1
        call   sub_2_1694
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
sub_2_1907:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        jne    SHORT loc_2_1924
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_1A27
loc_2_1924:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_193A
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_1A27
loc_2_193A:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, edx
        call   sub_2_32C0
        test   eax, eax
        jne    NEAR PTR loc_2_1A09
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        test   cl, cl
        je     SHORT loc_2_1984
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_197A
        mov    dx, 1
        push   ecx
        shl    dx, cl
        mov    ecx, 1
        call   sub_2_17BD
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
loc_2_197A:
        call   sub_2_33D9
        jmp    NEAR PTR loc_2_1A27
loc_2_1984:
        xor    ebx, ebx
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_19F7
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_19F7
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 8]
        je     SHORT loc_2_19F7
        push   ebx
        lea    eax, [ebp - 2]
        push   ebx
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_406B
        cmp    al, 2
        jne    SHORT loc_2_19F7
        push   3ch
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        cmp    al, bl
        mov    cl, al
        je     SHORT loc_2_19F7
        cmp    cl, 0fh
        ja     SHORT loc_2_19F7
        mov    dx, 1
        push   ecx
        shl    dx, cl
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_1A1D
loc_2_19F7:
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79B8
        add    esp, 8
        jmp    SHORT loc_2_1A27
loc_2_1A09:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1A27
        mov    dx, word ptr [eax + 2]
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
loc_2_1A1D:
        mov    ecx, 1
        call   sub_2_17BD
loc_2_1A27:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1A2C:
        push   ebx
        push   esi
        xor    bl, bl
        db  038h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1A8B
loc_2_1A38:
        movzx  ecx, bl
        db  08Bh, 014h, 08Dh, 000h, 000h, 000h, 000h
        lea    esi, [ecx*4]
        test   edx, edx
        je     SHORT loc_2_1A81
        db  083h, 0BEh, 000h, 000h, 000h, 000h, 001h
        sbb    eax, eax
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        and    eax, 0fff00000h
        add    eax, 0fffffh
        push   eax
        push   0
        db  0FFh, 0B6h, 000h, 000h, 000h, 000h
        push   ecx
        lea    eax, [edx - 1]
        db  08Bh, 08Eh, 000h, 000h, 000h, 000h
        not    eax
        push   eax
        call   sub_2_1889
loc_2_1A81:
        inc    bl
        db  038h, 01Dh, 000h, 000h, 000h, 000h
        ja     SHORT loc_2_1A38
loc_2_1A8B:
        pop    esi
        pop    ebx
        ret
sub_2_1A8E:
        push   ebx
        xor    bl, bl
        db  038h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1AD0
loc_2_1A99:
        movzx  eax, bl
        db  066h, 08Bh, 014h, 045h, 000h, 000h, 000h, 000h
        test   dx, dx
        je     SHORT loc_2_1AC6
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        push   ecx
        mov    cx, dx
        dec    cx
        not    cx
        push   ecx
        db  066h, 08Bh, 00Ch, 045h, 000h, 000h, 000h, 000h
        call   sub_2_17FE
loc_2_1AC6:
        inc    bl
        db  038h, 01Dh, 000h, 000h, 000h, 000h
        ja     SHORT loc_2_1A99
loc_2_1AD0:
        pop    ebx
        ret
loc_2_1AD2:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1AFC
        push   -1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   400000h
        xor    ecx, ecx
        push   0
        lea    eax, [edx - 1]
        not    eax
        push   0fch
        push   eax
        call   sub_2_1889
loc_2_1AFC:
        ret
sub_2_1AFD:
        push   esi
        mov    esi, 3
loc_2_1B03:
        push   0
        xor    edx, edx
        mov    ecx, 8000h
        call   sub_2_1694
        dec    esi
        jne    SHORT loc_2_1B03
        pop    esi
        ret
sub_2_1B16:
        push   ebx
        push   esi
        mov    si, dx
        mov    bl, cl
        push   ebx
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58E1
        and    ax, 0fffch
        jne    SHORT loc_2_1B3C
        call   sub_2_1AFD
        jmp    SHORT loc_2_1BAE
loc_2_1B3C:
        push   ebx
        mov    edx, 8
        push   0ffffh
        mov    ecx, eax
        call   sub_2_17FE
        mov    al, bl
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        add    al, 4
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_58E1
        mov    ecx, eax
        and    cx, 0fffch
        movzx  eax, cx
        and    al, 0f8h
        cmp    eax, 370h
        je     SHORT loc_2_1B7E
        cmp    eax, 3f0h
        jne    SHORT loc_2_1B8B
loc_2_1B7E:
        and    cx, 0fff8h
        add    cx, 6
        mov    si, 1
loc_2_1B8B:
        cmp    si, 1
        mov    eax, 0ffh
        je     SHORT loc_2_1B9C
        movzx  eax, bl
        add    eax, 4
loc_2_1B9C:
        push   eax
        mov    edx, esi
        push   0ffffh
        call   sub_2_17FE
        call   sub_2_1907
loc_2_1BAE:
        pop    esi
        pop    ebx
        ret
sub_2_1BB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        je     NEAR PTR loc_2_1C5C
        xor    cl, cl
        db  038h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1C06
loc_2_1BD3:
        movzx  eax, cl
        shl    eax, 2
        db  08Dh, 0B8h, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        je     SHORT loc_2_1BFC
        db  08Bh, 080h, 000h, 000h, 000h, 000h
        cmp    esi, eax
        jb     SHORT loc_2_1BFC
        add    eax, ebx
        dec    eax
        cmp    eax, edx
        jb     SHORT loc_2_1BFC
        mov    dword ptr [edi], 0
loc_2_1BFC:
        inc    cl
        db  038h, 00Dh, 000h, 000h, 000h, 000h
        ja     SHORT loc_2_1BD3
loc_2_1C06:
        mov    ecx, 1
        mov    dword ptr [ebp - 24h], esi
        mov    word ptr [ebp - 28h], cx
        mov    dword ptr [ebp - 20h], edx
        mov    word ptr [ebp - 26h], 14h
        mov    dword ptr [ebp - 18h], 0ffffffffh
        mov    ebx, edx
        cmp    dword ptr [ebp + 8], ecx
        sbb    eax, eax
        xor    edi, edi
        add    ax, cx
        sub    ebx, esi
        mov    word ptr [ebp - 1ch], ax
        add    ebx, ecx
        mov    word ptr [ebp - 1ah], di
        mov    dword ptr [ebp - 14h], ebx
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 10h], esi
        mov    word ptr [ebp - 6], di
        push   28h
        mov    dword ptr [ebp - 4], 0ffh
        mov    dword ptr [ebp - 0ch], edx
        lea    edx, [ebp - 28h]
        call   sub_2_1694
loc_2_1C5C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1C65:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        je     NEAR PTR loc_2_1D21
        xor    bl, bl
        db  038h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1CC3
loc_2_1C85:
        movzx  eax, bl
        add    eax, eax
        db  08Dh, 0B8h, 000h, 000h, 000h, 000h
        mov    si, word ptr [edi]
        test   si, si
        je     SHORT loc_2_1CB9
        db  066h, 08Bh, 080h, 000h, 000h, 000h, 000h
        cmp    cx, ax
        jb     SHORT loc_2_1CB9
        movzx  esi, si
        movzx  eax, ax
        add    esi, eax
        movzx  eax, dx
        dec    esi
        cmp    esi, eax
        jl     SHORT loc_2_1CB9
        mov    word ptr [edi], 0
loc_2_1CB9:
        inc    bl
        db  03Ah, 01Dh, 000h, 000h, 000h, 000h
        jb     SHORT loc_2_1C85
loc_2_1CC3:
        mov    word ptr [ebp - 18h], cx
        xor    eax, eax
        mov    byte ptr [ebp - 12h], 4
        mov    byte ptr [ebp - 11h], 3
        mov    word ptr [ebp - 1ch], 1
        mov    word ptr [ebp - 1ah], 0ch
        mov    word ptr [ebp - 10h], 0ffffh
        mov    si, dx
        sub    si, cx
        inc    si
        mov    word ptr [ebp - 16h], dx
        mov    word ptr [ebp - 14h], ax
        mov    byte ptr [ebp - 6], 4
        mov    byte ptr [ebp - 5], 3
        mov    dword ptr [ebp - 4], 0ffh
        mov    word ptr [ebp - 0eh], si
        push   1ch
        mov    word ptr [ebp - 0ch], cx
        mov    ecx, 2
        mov    word ptr [ebp - 0ah], dx
        lea    edx, [ebp - 1ch]
        mov    word ptr [ebp - 8], ax
        call   sub_2_1694
loc_2_1D21:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1D28:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        movzx  eax, cl
        push   ebx
        push   esi
        push   edi
        mov    bl, 10h
        lea    esi, [eax*4 + 10h]
        cmp    esi, 10h
        jle    NEAR PTR loc_2_1E89
loc_2_1D5E:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        push   ebx
        push   ecx
        push   edx
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        call   sub_2_4F1B
        test   eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_1E6F
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_1DDA
        mov    ecx, dword ptr [ebp - 4]
        not    cx
        or     cl, 3
        inc    cx
        movzx  eax, cx
        lea    edx, [eax - 1]
        test   eax, edx
        je     SHORT loc_2_1DAA
        mov    cx, 4
loc_2_1DAA:
        db  00Fh, 0B6h, 005h, 000h, 000h, 000h, 000h
        db  066h, 089h, 00Ch, 045h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        and    dx, 0fffch
        db  088h, 098h, 000h, 000h, 000h, 000h
        db  066h, 089h, 014h, 045h, 000h, 000h, 000h, 000h
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1E6F
loc_2_1DDA:
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_2_1DF9
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 040h
        jne    SHORT loc_2_1DF9
        movzx  ecx, al
        db  0C7h, 004h, 08Dh, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_2_1E07
loc_2_1DF9:
        movzx  ecx, al
        db  0C7h, 004h, 08Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_1E07:
        movzx  edx, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 7
        sub    ecx, 2
        cmp    ecx, 1
        sbb    ecx, ecx
        neg    ecx
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_2_1E2C
        or     dword ptr [ebp - 4], 0fff00000h
loc_2_1E2C:
        and    byte ptr [ebp - 4], 0f0h
        mov    ecx, dword ptr [ebp - 4]
        not    ecx
        mov    dword ptr [ebp - 4], ecx
        inc    dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        dec    ecx
        test   dword ptr [ebp - 4], ecx
        je     SHORT loc_2_1E4B
        mov    dword ptr [ebp - 4], 10h
loc_2_1E4B:
        mov    ecx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        and    cl, 0f0h
        inc    al
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 095h, 000h, 000h, 000h, 000h
        db  088h, 09Ah, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_1E6F:
        mov    eax, dword ptr [ebp - 8]
        and    al, 7
        cmp    al, 4
        jne    SHORT loc_2_1E7B
        add    bl, 4
loc_2_1E7B:
        add    bl, 4
        movzx  eax, bl
        cmp    eax, esi
        jl     NEAR PTR loc_2_1D5E
loc_2_1E89:
        push   3dh
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_2_58C5
        cmp    al, 1
        sbb    ecx, ecx
        inc    ecx
        cmp    al, 4
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_1EC3
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79B8
        add    esp, 8
        jmp    NEAR PTR loc_2_1F5A
loc_2_1EC3:
        push   3ch
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58C5
        cmp    al, 0fh
        jbe    SHORT loc_2_1EE0
        cmp    al, 15h
        ja     SHORT loc_2_1EE0
        sub    al, 6
loc_2_1EE0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1EF2
        cmp    al, 0fh
        jbe    SHORT loc_2_1EF9
loc_2_1EF2:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
loc_2_1EF9:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_430E
        test   eax, eax
        je     SHORT loc_2_1F2D
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_165C
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1F2D
        mov    ecx, dword ptr [eax + 4]
        push   0
        mov    eax, dword ptr [eax + 8]
        lea    edx, [eax + ecx - 1]
        call   sub_2_1BB1
loc_2_1F2D:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_1F5A
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_4FCA
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_1F5A
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_1F5A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1F61:
        call   sub_2_1907
        call   sub_2_1A2C
        call   sub_2_1A8E
        jmp    NEAR PTR loc_2_1AD2
sub_2_1F75:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        push   ebx
        mov    byte ptr [ebp - 1], dl
        mov    bl, cl
        jne    SHORT loc_2_1FCF
        push   18h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        push   19h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        mov    byte ptr [ebp - 8], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 8], al
        jae    SHORT loc_2_1FCF
        push   19h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        push   1ah
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        mov    byte ptr [ebp - 8], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 8], al
        mov    eax, 1
        jbe    SHORT loc_2_1FD1
loc_2_1FCF:
        xor    eax, eax
loc_2_1FD1:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1FD6:
        sub    esp, 4
        mov    al, dl
        mov    dword ptr [esp], 1
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, ecx
        db  00Fh, 0B6h, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, 1
        je     SHORT loc_2_1FFC
        cmp    ecx, 2
        je     SHORT loc_2_202B
        jmp    SHORT loc_2_2049
loc_2_1FFC:
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, eax
        call   sub_2_5256
        test   eax, eax
        je     SHORT loc_2_201B
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, ebp
        mov    ecx, 22h
        rep movsd
        jmp    SHORT loc_2_2049
loc_2_201B:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, ebp
        mov    ecx, 22h
        rep movsd
        jmp    SHORT loc_2_2049
loc_2_202B:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, ebp
        mov    ecx, 22h
        rep movsd
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 079h, 011h, 003h, 006h
        jne    SHORT loc_2_2049
        add    byte ptr [ebp + 28h], 2
loc_2_2049:
        xor    bl, bl
        cmp    byte ptr [ebp], bl
        jbe    SHORT loc_2_208A
        xor    esi, esi
loc_2_2052:
        movzx  eax, bl
        imul   eax, eax, 2ch
        push   esi
        lea    ecx, [eax + ebp + 14h]
        lea    edi, [eax + ebp + 4]
        push   ecx
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_207F
        push   esi
        add    edi, 14h
        push   edi
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_2083
loc_2_207F:
        mov    dword ptr [esp + 10h], esi
loc_2_2083:
        inc    bl
        cmp    byte ptr [ebp], bl
        ja     SHORT loc_2_2052
loc_2_208A:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_2_20D9
        xor    bl, bl
        cmp    byte ptr [ebp], bl
        je     SHORT loc_2_20CE
loc_2_2098:
        movzx  eax, bl
        imul   eax, eax, 2ch
        lea    edi, [eax + ebp + 4]
        mov    eax, dword ptr [eax + ebp + 14h]
        test   eax, eax
        je     SHORT loc_2_20B5
        push   0
        push   eax
        call   sub_2_7900
        add    esp, 8
loc_2_20B5:
        mov    eax, dword ptr [edi + 14h]
        test   eax, eax
        je     SHORT loc_2_20C7
        push   0
        push   eax
        call   sub_2_7900
        add    esp, 8
loc_2_20C7:
        inc    bl
        cmp    bl, byte ptr [ebp]
        jb     SHORT loc_2_2098
loc_2_20CE:
        push   0
        push   ebp
        call   sub_2_7950
        add    esp, 8
loc_2_20D9:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_2_20E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        je     SHORT loc_2_2114
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_1F75
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_2114:
        push   19h
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_2_58C5
        push   1ah
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], al
        mov    ecx, ebx
        call   sub_2_58C5
        db  03Ah, 005h, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_214F
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_214F:
        push   1
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        push   esi
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_4451
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_248E
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_2_218A
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_5868
loc_2_218A:
        push   0
        lea    eax, [ebp - 1ch]
        push   esi
        push   eax
        call   sub_2_79D0
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 1ch]
        db  02Bh, 005h, 000h, 000h, 000h, 000h
        cmp    eax, 10h
        ja     NEAR PTR loc_2_248E
        push   201h
        push   88h
        call   sub_2_79C8
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_2_21CD
        mov    edx, 1
        jmp    NEAR PTR loc_2_2483
loc_2_21CD:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        push   eax
        mov    ecx, esi
        call   sub_2_1FD6
        test   eax, eax
        jne    SHORT loc_2_21EE
        mov    edx, 1
        jmp    NEAR PTR loc_2_2483
loc_2_21EE:
        mov    al, byte ptr [esi]
        test   al, al
        je     SHORT loc_2_21FE
        movzx  ecx, al
        lea    edi, [ebp - 28h]
        xor    eax, eax
        rep stosd
loc_2_21FE:
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_42DD
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_2259
        xor    ebx, ebx
        mov    al, byte ptr [esi + 3]
        test   al, 1
        je     SHORT loc_2_221E
        mov    ebx, 8
loc_2_221E:
        test   al, 2
        je     SHORT loc_2_2225
        or     bl, 10h
loc_2_2225:
        mov    edi, esi
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0
        call   sub_2_79C0
        add    esp, 18h
        db  0A1h, 000h, 000h, 000h, 000h
        push   ebx
        push   eax
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0
        call   sub_2_79C0
        add    esp, 18h
loc_2_2259:
        xor    ebx, ebx
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_230C
        mov    dword ptr [ebp - 14h], ebx
        cmp    byte ptr [esi], bl
        jbe    NEAR PTR loc_2_2349
        lea    ecx, [ebp - 2ch]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 8], ecx
loc_2_227B:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        push   ecx
        push   edi
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_4DD7
        test   byte ptr [edi + 1ch], 1
        je     SHORT loc_2_22C4
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx], eax
        jne    SHORT loc_2_22C4
        push   ebx
        db  0A0h, 000h, 000h, 000h, 000h
        push   -1
        mov    edx, eax
        push   edi
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [ebp - 0ch], ebx
        call   sub_2_4E43
loc_2_22C4:
        cmp    dword ptr [ebp - 10h], ebx
        je     SHORT loc_2_22F4
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_2_22F4
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 0ch], eax
        jb     SHORT loc_2_22F4
        sub    dword ptr [ebp - 0ch], eax
        mov    eax, 1
        mov    cl, byte ptr [edi + 28h]
        shl    eax, cl
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], edx
loc_2_22F4:
        add    dword ptr [ebp - 8], 4
        movzx  eax, byte ptr [esi]
        add    edi, 2ch
        inc    dword ptr [ebp - 14h]
        cmp    eax, dword ptr [ebp - 14h]
        ja     NEAR PTR loc_2_227B
        jmp    SHORT loc_2_2349
loc_2_230C:
        mov    al, byte ptr [esi]
        test   al, al
        je     SHORT loc_2_2334
        movzx  edi, al
        lea    ebx, [esi + 4]
loc_2_2318:
        mov    eax, dword ptr [ebx + 10h]
        test   eax, eax
        je     SHORT loc_2_232E
        push   0
        push   eax
        push   -1
        push   0
        call   sub_2_7928
        add    esp, 10h
loc_2_232E:
        add    ebx, 2ch
        dec    edi
        jne    SHORT loc_2_2318
loc_2_2334:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_2_5D8F
        add    esp, 8
loc_2_2349:
        push   4
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_2_58D3
        mov    word ptr [ebp - 6], ax
        mov    al, byte ptr [esi]
        test   al, al
        je     NEAR PTR loc_2_240B
        lea    ebx, [ebp - 28h]
        lea    edi, [esi + 4]
        movzx  eax, al
        mov    dword ptr [ebp - 14h], eax
loc_2_2379:
        mov    eax, dword ptr [edi + 20h]
        cmp    eax, 1
        je     SHORT loc_2_2388
        cmp    eax, 2
        je     SHORT loc_2_23C8
        jmp    SHORT loc_2_23FC
loc_2_2388:
        mov    ecx, dword ptr [ebx]
        test   ecx, ecx
        je     SHORT loc_2_23A0
        test   byte ptr [ebp - 6], 2
        je     SHORT loc_2_23A0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_23AA
loc_2_23A0:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_23AA:
        push   -1
        mov    eax, dword ptr [edi + 1ch]
        push   0
        and    eax, 1
        push   eax
        mov    dl, byte ptr [esi + 1]
        push   edx
        mov    eax, dword ptr [edi + 18h]
        not    eax
        push   eax
        mov    edx, dword ptr [edi]
        call   sub_2_1889
        jmp    SHORT loc_2_23FC
loc_2_23C8:
        mov    ecx, dword ptr [ebx]
        test   ecx, ecx
        je     SHORT loc_2_23E0
        test   byte ptr [ebp - 6], 1
        je     SHORT loc_2_23E0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_23EA
loc_2_23E0:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_23EA:
        mov    al, byte ptr [esi + 1]
        mov    edx, dword ptr [edi]
        push   eax
        mov    eax, dword ptr [edi + 18h]
        not    ax
        push   eax
        call   sub_2_17FE
loc_2_23FC:
        add    ebx, 4
        add    edi, 2ch
        dec    dword ptr [ebp - 14h]
        jne    NEAR PTR loc_2_2379
loc_2_240B:
        xor    ebx, ebx
        push   1
        lea    edx, [ebp - 1]
        mov    ecx, 43h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        call   sub_2_1694
        mov    cl, byte ptr [esi + 2]
        call   sub_2_1D28
        call   sub_2_1F61
        cmp    byte ptr [esi], bl
        je     SHORT loc_2_2474
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 18h]
        push   ebx
        push   ecx
        call   sub_2_7970
        add    esp, 10h
        test   byte ptr [ebp - 17h], 4
        jne    SHORT loc_2_2474
        cmp    byte ptr [esi], bl
        jbe    SHORT loc_2_2469
        lea    eax, [esi + 4]
        mov    ecx, 0ffffffffh
loc_2_2459:
        inc    ebx
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 8], ecx
        add    eax, 2ch
        movzx  edx, byte ptr [esi]
        cmp    edx, ebx
        ja     SHORT loc_2_2459
loc_2_2469:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_7403
loc_2_2474:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_2_248E
        db  08Bh, 015h, 000h, 000h, 000h, 000h
loc_2_2483:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_7005
loc_2_248E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2495:
        db  066h, 081h, 03Dh, 000h, 000h, 000h, 000h, 005h, 006h
        jne    SHORT loc_2_24D4
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_24B3
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_24B3:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 086h, 080h, 021h, 012h
        jne    SHORT loc_2_24D4
        push   0
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        push   50h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58EF
loc_2_24D4:
        ret
sub_2_24D5:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   esi
        cmp    ax, 1
        je     SHORT loc_2_24F4
        cmp    ah, 3
        je     SHORT loc_2_24F4
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 002h
        je     NEAR PTR loc_2_25D4
loc_2_24F4:
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_250C
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_250C
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_250C:
        push   4
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58D3
        and    ax, 3
        sub    ax, 3
        cmp    ax, 1
        sbb    esi, esi
        neg    esi
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    ax, 1
        je     SHORT loc_2_254C
        cmp    ax, 300h
        je     SHORT loc_2_254C
        cmp    ax, 301h
        mov    ecx, 0
        jne    SHORT loc_2_2551
loc_2_254C:
        mov    ecx, 1
loc_2_2551:
        db  0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     SHORT loc_2_2563
        mov    ecx, eax
        and    ecx, 8000h
loc_2_2563:
        test   ecx, ecx
        je     SHORT loc_2_25BB
        test   esi, esi
        je     SHORT loc_2_25AD
        push   1
        mov    edx, 0affffh
        mov    ecx, 0a0000h
        call   sub_2_1BB1
        push   1
        mov    edx, 0bffffh
        mov    ecx, 0b0000h
        call   sub_2_1BB1
        mov    edx, 3bbh
        mov    ecx, 3b0h
        call   sub_2_1C65
        mov    edx, 3dfh
        mov    ecx, 3c0h
        call   sub_2_1C65
        jmp    SHORT loc_2_25BB
loc_2_25AD:
        test   al, 2
        jne    SHORT loc_2_25BB
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_25BB:
        test   esi, esi
        jne    SHORT loc_2_25D4
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_2_5D8F
        add    esp, 8
loc_2_25D4:
        pop    esi
        ret
sub_2_25D6:
        push   ebx
        push   esi
        mov    esi, 0ffffffffh
        mov    bl, 0c0h
loc_2_25DF:
        push   0
        mov    edx, ebx
        xor    ecx, ecx
        call   sub_2_58E1
        cmp    eax, 84c48086h
        je     SHORT loc_2_25FB
        add    bl, 8
        cmp    bl, 0e0h
        jb     SHORT loc_2_25DF
        jmp    SHORT loc_2_2609
loc_2_25FB:
        push   8
        mov    edx, ebx
        xor    ecx, ecx
        call   sub_2_58C5
        movzx  esi, al
loc_2_2609:
        mov    eax, 2
        cmp    eax, esi
        pop    esi
        sbb    eax, eax
        pop    ebx
        inc    eax
        ret
sub_2_2616:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 4], 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        db  066h, 081h, 03Dh, 000h, 000h, 000h, 000h, 001h, 001h
        je     SHORT loc_2_2642
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 001h
        jne    SHORT loc_2_2642
        xor    eax, eax
        jmp    NEAR PTR loc_2_2AC3
loc_2_2642:
        push   9
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58C5
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], al
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 095h, 010h, 040h, 006h
        mov    edi, ebx
        je     SHORT loc_2_2678
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 095h, 010h, 046h, 006h
        jne    SHORT loc_2_26C0
loc_2_2678:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_57DE
        mov    ebx, eax
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_5819
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    edi, eax
        jne    SHORT loc_2_26C0
        test   ebx, ebx
        jne    SHORT loc_2_26B6
        test   edi, edi
        je     NEAR PTR loc_2_2AAE
loc_2_26B6:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_26C0:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 086h, 080h, 022h, 012h
        jne    SHORT loc_2_2724
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], 8
        mov    esi, 1
        je     SHORT loc_2_2724
        push   0feh
        db  0A0h, 000h, 000h, 000h, 000h
        push   4ch
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_58EF
        push   3
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        push   4dh
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58EF
        push   0
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        push   50h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58EF
loc_2_2724:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 086h, 080h, 030h, 012h
        jne    SHORT loc_2_275C
        call   sub_2_25D6
        test   eax, eax
        je     SHORT loc_2_275C
        push   0
        lea    ecx, [ebp - 5]
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 5], 1
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   eax
        call   sub_2_79E0
        add    esp, 1ch
loc_2_275C:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 095h, 010h, 040h, 006h
        je     SHORT loc_2_27B5
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 095h, 010h, 046h, 006h
        je     SHORT loc_2_27B5
        push   18h
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0f8h
        cmp    eax, 170h
        jne    SHORT loc_2_27B5
        push   1ch
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0f0h
        cmp    eax, 370h
        jne    SHORT loc_2_27B5
        mov    esi, 1
loc_2_27B5:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 045h, 010h, 021h, 0C6h
        jne    SHORT loc_2_2801
        push   9
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_58C5
        mov    byte ptr [ebp - 5], 1
        test   al, 80h
        jne    SHORT loc_2_27E2
        mov    byte ptr [ebp - 5], 0
loc_2_27E2:
        push   0
        lea    eax, [ebp - 5]
        push   1
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79E0
        add    esp, 1ch
loc_2_2801:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0ch], 4
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79D8
        add    esp, 1ch
        test   byte ptr [ebp - 1], 0ch
        jne    SHORT loc_2_2845
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_2845
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_2_2883
loc_2_2845:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 6401095h
        je     SHORT loc_2_287A
        cmp    eax, 6461095h
        je     SHORT loc_2_287A
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_2C70
        mov    ebx, eax
        and    eax, 2
        shr    eax, 1
        and    ebx, 1
        mov    esi, eax
        jmp    SHORT loc_2_2883
loc_2_287A:
        test   edi, edi
        je     SHORT loc_2_2883
        mov    esi, 1
loc_2_2883:
        test   esi, esi
        jne    SHORT loc_2_2889
        xor    edi, edi
loc_2_2889:
        test   ebx, ebx
        je     SHORT loc_2_28D3
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_2_28D3
        push   10h
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0f8h
        cmp    eax, 1f0h
        jne    SHORT loc_2_28D3
        push   14h
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0f8h
        cmp    eax, 3f0h
        jne    SHORT loc_2_28D3
        and    byte ptr [ebp - 1], 0feh
loc_2_28D3:
        test   edi, edi
        je     SHORT loc_2_291B
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_2_291B
        push   18h
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0f8h
        cmp    eax, 170h
        jne    SHORT loc_2_291B
        push   1ch
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58E1
        and    al, 0f8h
        cmp    eax, 370h
        jne    SHORT loc_2_291B
        and    byte ptr [ebp - 1], 0fbh
loc_2_291B:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 5]
        mov    dword ptr [ebp - 0ch], 1
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79D8
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_295B
        xor    ebx, ebx
        mov    al, byte ptr [ebp - 5]
        mov    bl, byte ptr [ebp - 5]
        and    al, 2
        and    ebx, 1
        cmp    al, 1
        sbb    edi, edi
        inc    edi
        mov    esi, edi
loc_2_295B:
        test   ebx, ebx
        je     SHORT loc_2_299F
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_2_2974
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, 10h
        call   sub_2_1B16
        jmp    SHORT loc_2_29A4
loc_2_2974:
        mov    edx, 1f7h
        mov    ecx, 1f0h
        call   sub_2_1C65
        mov    ecx, 3f6h
        mov    edx, ecx
        call   sub_2_1C65
        push   0eh
        mov    edx, 4000h
        xor    ecx, ecx
        call   sub_2_17BD
        jmp    SHORT loc_2_29A4
loc_2_299F:
        call   sub_2_1AFD
loc_2_29A4:
        test   esi, esi
        je     SHORT loc_2_29EC
        test   edi, edi
        je     SHORT loc_2_29EC
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_2_29C1
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, 18h
        call   sub_2_1B16
        jmp    SHORT loc_2_29F1
loc_2_29C1:
        mov    edx, 177h
        mov    ecx, 170h
        call   sub_2_1C65
        mov    ecx, 376h
        mov    edx, ecx
        call   sub_2_1C65
        push   0fh
        mov    edx, 8000h
        xor    ecx, ecx
        call   sub_2_17BD
        jmp    SHORT loc_2_29F1
loc_2_29EC:
        call   sub_2_1AFD
loc_2_29F1:
        test   byte ptr [ebp - 1], 80h
        je     NEAR PTR loc_2_2AA6
        push   20h
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58E1
        mov    byte ptr [ebp - 1], 0
        mov    esi, eax
        and    si, 0fffch
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 020h
        je     SHORT loc_2_2A3D
loc_2_2A22:
        mov    al, byte ptr [ebp - 1]
        db  03Ah, 005h, 000h, 000h, 000h, 000h
        jae    SHORT loc_2_2A3D
        inc    byte ptr [ebp - 1]
        movzx  eax, byte ptr [ebp - 1]
        db  080h, 0B8h, 000h, 000h, 000h, 000h, 020h
        jne    SHORT loc_2_2A22
loc_2_2A3D:
        test   si, si
        je     SHORT loc_2_2AA6
        test   ebx, ebx
        je     SHORT loc_2_2A5E
        push   0ffh
        mov    edx, 8
        push   0ffffh
        mov    ecx, esi
        call   sub_2_17FE
        jmp    SHORT loc_2_2A6C
loc_2_2A5E:
        push   0
        xor    edx, edx
        mov    ecx, 8000h
        call   sub_2_1694
loc_2_2A6C:
        test   edi, edi
        je     SHORT loc_2_2A98
        mov    cx, si
        push   0ffh
        movzx  eax, byte ptr [ebp - 1]
        add    cx, 8
        push   0ffffh
        db  066h, 08Bh, 014h, 045h, 000h, 000h, 000h, 000h
        sub    dx, 8
        call   sub_2_17FE
        jmp    SHORT loc_2_2AA6
loc_2_2A98:
        push   0
        xor    edx, edx
        mov    ecx, 8000h
        call   sub_2_1694
loc_2_2AA6:
        test   ebx, ebx
        jne    SHORT loc_2_2ABE
        test   edi, edi
        jne    SHORT loc_2_2ABE
loc_2_2AAE:
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79B8
        add    esp, 8
loc_2_2ABE:
        mov    eax, 1
loc_2_2AC3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_2ACA:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   ebx
        push   0
        mov    bl, byte ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        mov    ecx, dword ptr [ebp + 14h]
        push   1
        mov    edx, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 0ch]
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_2_40BC
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 080h
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_2B1E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_2B1E:
        push   0ah
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_58D3
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_406B
        db  0A2h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    ah, 6
        jne    SHORT loc_2_2B70
        movzx  eax, ax
        cmp    eax, 604h
        je     SHORT loc_2_2BC1
        cmp    eax, 605h
        je     SHORT loc_2_2BDB
        cmp    eax, 607h
        je     SHORT loc_2_2BEE
loc_2_2B70:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_2C6D
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 086h, 080h, 084h, 004h
        jne    NEAR PTR loc_2_2C1F
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 008h
        jne    NEAR PTR loc_2_2C1F
        push   41h
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_2_58C5
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 010h
        je     SHORT loc_2_2C01
        test   al, 1
        jne    SHORT loc_2_2C1F
        or     al, 1
        jmp    SHORT loc_2_2C07
loc_2_2BC1:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    NEAR PTR loc_2_2C6D
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_20E7
        jmp    NEAR PTR loc_2_2C6D
loc_2_2BDB:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_2B70
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_20E7
        jmp    SHORT loc_2_2C6D
loc_2_2BEE:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_2C6D
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_20E7
        jmp    SHORT loc_2_2C6D
loc_2_2C01:
        test   al, 1
        je     SHORT loc_2_2C1F
        and    al, 0feh
loc_2_2C07:
        push   eax
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        push   41h
        mov    edx, ecx
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_2_58EF
loc_2_2C1F:
        push   0
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, ecx
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        mov    ecx, ebx
        call   sub_2_4451
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_2C6D
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 010h
        jne    SHORT loc_2_2C6D
        mov    ecx, 6
        call   sub_2_1D28
        call   sub_2_2495
        call   sub_2_24D5
        call   sub_2_2616
        test   eax, eax
        jne    SHORT loc_2_2C6D
        call   sub_2_1F61
loc_2_2C6D:
        pop    ebx
        pop    ebp
        ret
sub_2_2C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a4h
        push   ebx
        mov    byte ptr [ebp - 4], cl
        mov    byte ptr [ebp - 9fh], dl
        push   esi
        push   edi
        mov    bl, dl
        lea    eax, [ebp - 14h]
        mov    byte ptr [ebp - 0a0h], cl
        db  0C7h, 085h, 066h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79F0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_2CBC
        mov    eax, 3
        jmp    NEAR PTR loc_2_2D6E
loc_2_2CBC:
        push   0
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 10h]
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        push   dword ptr [ebp - 14h]
        call   sub_2_79F0
        add    esp, 0ch
        mov    esi, eax
        push   dword ptr [ebp - 14h]
        call   sub_2_79E8
        add    esp, 4
        test   esi, esi
        je     SHORT loc_2_2D05
        mov    eax, 3
        jmp    SHORT loc_2_2D6E
loc_2_2D05:
        lea    edx, [ebp - 0a4h]
        mov    ecx, dword ptr [ebp - 10h]
        call   sub_2_3F18
        push   dword ptr [ebp - 10h]
        mov    esi, eax
        call   sub_2_79E8
        add    esp, 4
        cmp    esi, 20h
        je     SHORT loc_2_2D2C
        mov    eax, 3
        jmp    SHORT loc_2_2D6E
loc_2_2D2C:
        lea    ecx, [ebp - 0a4h]
        call   sub_2_3F7D
        cmp    eax, 20h
        je     SHORT loc_2_2D43
        mov    eax, 3
        jmp    SHORT loc_2_2D6E
loc_2_2D43:
        mov    edi, 3
        lea    eax, [ebp - 7]
        push   eax
        mov    dword ptr [ebp - 7], edi
        db  0FFh, 015h, 014h, 000h, 000h, 000h
        add    esp, 4
        mov    esi, eax
        lea    ecx, [ebp - 0a4h]
        call   sub_2_402B
        mov    eax, edi
        test   esi, esi
        jne    SHORT loc_2_2D6E
        mov    eax, dword ptr [ebp - 7]
loc_2_2D6E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2D75:
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx], eax
        test   eax, eax
        jne    SHORT loc_2_2D84
        xor    eax, eax
        jmp    SHORT loc_2_2D9B
loc_2_2D84:
        push   0
        push   0
        push   4
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_2D9B:
        ret
loc_2_2D9C:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [edx + 1]
        test   bl, bl
        jne    SHORT loc_2_2DC1
        mov    eax, 13h
        jmp    NEAR PTR loc_2_2EB7
loc_2_2DC1:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_2_2DF5
        cmp    eax, 1
        je     SHORT loc_2_2E18
        cmp    eax, 4
        je     SHORT loc_2_2E41
        cmp    eax, 5
        je     NEAR PTR loc_2_2E88
        cmp    eax, 6
        je     SHORT loc_2_2E18
        cmp    eax, 7
        je     NEAR PTR loc_2_2E9E
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_2EB4
loc_2_2DF5:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 14h]
        call   sub_2_2D75
        test   eax, eax
        je     NEAR PTR loc_2_2EB4
        mov    ecx, dword ptr [ebp - 8]
        movzx  ax, bl
        mov    word ptr [ecx + 2], ax
        jmp    NEAR PTR loc_2_2EB4
loc_2_2E18:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 14h]
        call   sub_2_2D75
        test   eax, eax
        je     NEAR PTR loc_2_2EB4
        mov    ecx, dword ptr [ebp - 8]
        movzx  ax, bl
        cmp    word ptr [ecx + 2], ax
        je     SHORT loc_2_2EB4
        mov    dword ptr [ebp - 4], 13h
        jmp    SHORT loc_2_2EB4
loc_2_2E41:
        mov    si, 1
        mov    eax, dword ptr [ebp + 14h]
        mov    cl, bl
        db  0BFh, 004h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], 18h
        shl    si, cl
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 089h, 035h, 000h, 000h, 000h, 000h
        db  066h, 089h, 035h, 002h, 000h, 000h, 000h
        stosd
        stosd
        stosd
        stosd
        movzx  eax, bl
        mov    cl, byte ptr [edx + 8]
        db  088h, 088h, 004h, 000h, 000h, 000h
        db  066h, 089h, 035h, 014h, 000h, 000h, 000h
        jmp    SHORT loc_2_2EB4
loc_2_2E88:
        mov    eax, dword ptr [edx + 8]
        test   eax, eax
        je     SHORT loc_2_2EB4
        dec    eax
        mov    dword ptr [edx + 8], eax
        jne    SHORT loc_2_2EB4
        mov    ecx, edx
        call   sub_2_36EA
        jmp    SHORT loc_2_2EB4
loc_2_2E9E:
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax]
        and    ax, 2
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 13h
        mov    dword ptr [ebp - 4], eax
loc_2_2EB4:
        mov    eax, dword ptr [ebp - 4]
loc_2_2EB7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2EBE:
        push   esi
        mov    eax, 0
        push   edi
        mov    esi, ecx
        cmp    dword ptr [esp + 0ch], 1
        adc    eax, -1
        mov    edi, edx
        and    eax, 18h
        or     al, 2
        push   eax
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   4
        push   0
        call   sub_2_79C0
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_2EF1
        inc    dword ptr [edi + 8]
loc_2_2EF1:
        push   0
        push   esi
        push   dword ptr [edi + 4]
        call   sub_2_7A00
        add    esp, 0ch
        pop    edi
        pop    esi
        ret    4
sub_2_2F04:
        push   ebx
        xor    eax, eax
        push   esi
        db  039h, 005h, 000h, 000h, 000h, 000h
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_2F37
loc_2_2F17:
        cmp    byte ptr [edi], cl
        jne    SHORT loc_2_2F2B
        mov    bl, byte ptr [edi + 1]
        xor    bl, dl
        movzx  esi, bl
        test   esi, 0fffffff8h
        je     SHORT loc_2_2F37
loc_2_2F2B:
        inc    eax
        add    edi, 10h
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jb     SHORT loc_2_2F17
loc_2_2F37:
        db  02Bh, 005h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_2F4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        mov    byte ptr [ebp - 3], dl
        push   esi
        mov    dword ptr [ebp - 8], ecx
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 1ch]
        push   3dh
        movsd
        movsd
        movsd
        movsd
        movsb
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 3]
        call   sub_2_58C5
        mov    bl, al
        test   bl, bl
        jne    SHORT loc_2_2F84
        xor    eax, eax
        jmp    NEAR PTR loc_2_30D8
loc_2_2F84:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_2F94
        xor    eax, eax
        jmp    NEAR PTR loc_2_30D8
loc_2_2F94:
        dec    bl
        call   sub_2_44FB
        xor    edi, edi
loc_2_2F9D:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 3]
        call   sub_2_2F04
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_2_30B2
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        db  0A1h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_30B2
        cmp    dword ptr [ebp - 8], eax
        je     NEAR PTR loc_2_30B2
        push   0
        lea    eax, [ebp - 1]
        push   0
        lea    edx, [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_4075
        test   eax, 20000h
        jne    NEAR PTR loc_2_309E
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        movzx  eax, byte ptr [ebp + 8]
        push   eax
        push   0
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 1ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 10h
        lea    ecx, [ebp - 1ch]
        call   sub_2_4521
        test   eax, eax
        je     SHORT loc_2_3051
        lea    ecx, [ebx*8]
        shr    eax, cl
        and    eax, 0ffh
        dec    eax
        cmp    eax, 3
        jbe    SHORT loc_2_304D
        mov    edi, 1
        jmp    SHORT loc_2_309E
loc_2_304D:
        mov    bl, al
        jmp    SHORT loc_2_309E
loc_2_3051:
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_406B
        movzx  eax, al
        cmp    eax, 1
        je     SHORT loc_2_3070
        cmp    eax, 2
        je     SHORT loc_2_308E
        mov    edi, 1
        jmp    SHORT loc_2_309E
loc_2_3070:
        mov    al, byte ptr [ebp + 8]
        shr    al, 3
        movzx  ecx, al
        movzx  eax, bl
        add    eax, ecx
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 3
        xor    eax, edx
        sub    eax, edx
        mov    bl, al
        jmp    SHORT loc_2_309E
loc_2_308E:
        push   3dh
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        lea    ebx, [eax - 1]
loc_2_309E:
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp + 8], cl
        test   edi, edi
        je     NEAR PTR loc_2_2F9D
loc_2_30B2:
        call   sub_2_4592
        test   edi, edi
        je     SHORT loc_2_30BD
        xor    esi, esi
loc_2_30BD:
        test   esi, esi
        je     SHORT loc_2_30D6
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dl, byte ptr [ebp - 3]
        mov    byte ptr [eax], bl
        mov    ebx, dword ptr [ebp + 14h]
        mov    al, byte ptr [ebp + 8]
        mov    byte ptr [ecx], dl
        mov    byte ptr [ebx], al
loc_2_30D6:
        mov    eax, esi
loc_2_30D8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_2_30E1:
        push   esi
        xor    eax, eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    eax, edx
        je     SHORT loc_2_3102
loc_2_30F4:
        cmp    byte ptr [esi], cl
        je     SHORT loc_2_3100
        inc    eax
        add    esi, 0ch
        cmp    edx, eax
        ja     SHORT loc_2_30F4
loc_2_3100:
        cmp    eax, edx
loc_2_3102:
        sbb    eax, eax
        and    eax, esi
        pop    esi
        ret
sub_2_3108:
        push   ebp
        mov    ebp, esp
        sub    esp, 0dch
        mov    word ptr [ebp - 10h], 3
        push   ebx
        xor    eax, eax
        mov    word ptr [ebp - 0eh], ax
        push   esi
        mov    word ptr [ebp - 0ah], ax
        push   edi
        mov    esi, ecx
        lea    edx, [ebp - 0dch]
        movzx  ecx, byte ptr [esi]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_2_79F8
        add    esp, 0ch
        lea    edx, [ebp - 0dch]
        lea    eax, [ebp - 4]
        push   1
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        call   sub_2_7A18
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_3164
        xor    eax, eax
        jmp    NEAR PTR loc_2_32B9
loc_2_3164:
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   edi
        xor    edx, edx
        push   10h
        mov    ecx, eax
        push   edi
        mov    dword ptr [esi + 4], eax
        call   sub_2_426A
        push   edi
        lea    eax, [ebp - 8]
        push   2000h
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_3199
        xor    eax, eax
        jmp    NEAR PTR loc_2_32B9
loc_2_3199:
        mov    di, word ptr [esi + 2]
        push   0
        mov    ax, di
        push   8
        db  066h, 023h, 005h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 10h]
        mov    word ptr [ebp - 0ch], ax
        push   ecx
        push   4
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_2_31CD
        xor    eax, eax
        jmp    NEAR PTR loc_2_32B9
loc_2_31CD:
        push   0
        lea    ecx, [ebp - 8]
        push   3000h
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_31EE
        xor    eax, eax
        jmp    NEAR PTR loc_2_32B9
loc_2_31EE:
        mov    word ptr [ebp - 0ch], di
        push   0
        push   8
        lea    eax, [ebp - 10h]
        push   eax
        push   4
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_2_3214
        xor    eax, eax
        jmp    NEAR PTR loc_2_32B9
loc_2_3214:
        xor    ebx, ebx
        lea    eax, [ebp - 14h]
        push   eax
        mov    dword ptr [ebp - 14h], ebx
        db  0FFh, 015h, 00Ch, 000h, 000h, 000h
        add    esp, 4
        mov    al, byte ptr [esi]
        lea    ecx, [ebp - 15h]
        push   eax
        push   ecx
        db  0FFh, 015h, 004h, 000h, 000h, 000h
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_3292
        cmp    byte ptr [ebp - 15h], bl
        je     SHORT loc_2_3292
        mov    eax, 1
        mov    cl, byte ptr [ebp - 15h]
        shl    eax, cl
        test   dword ptr [ebp - 14h], eax
        je     SHORT loc_2_3292
        push   3
        lea    ecx, [ebp - 8]
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_3269
        xor    eax, eax
        jmp    SHORT loc_2_32B9
loc_2_3269:
        push   0
        lea    ecx, [ebp - 10h]
        movzx  ax, byte ptr [ebp - 15h]
        mov    word ptr [ebp - 0eh], ax
        push   8
        push   ecx
        push   4
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 18h
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_2_32B9
loc_2_3292:
        push   0
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_7A10
        add    esp, 0ch
        push   7
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_2_7A08
        add    esp, 10h
        mov    eax, 1
loc_2_32B9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_32C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    ecx, edx
        mov    edi, edx
        push   0
        lea    eax, [ebp - 3]
        push   0
        lea    edx, [ebp - 2]
        push   eax
        call   sub_2_4212
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 5]
        push   eax
        lea    edx, [ebp - 1]
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, edi
        push   dword ptr [ebp - 3]
        call   sub_2_2F4E
        test   eax, eax
        je     SHORT loc_2_3348
        movzx  ecx, byte ptr [ebp - 1]
        imul   ecx, ecx, 3
        mov    cl, byte ptr [ecx + eax + 2]
        call   sub_2_30E1
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_2_3348
        cmp    dword ptr [ebx + 4], 0
        jne    SHORT loc_2_3332
        mov    ecx, ebx
        call   sub_2_3108
        test   eax, eax
        jne    SHORT loc_2_3332
        push   2
        push   esi
        call   sub_2_79B8
        add    esp, 8
        mov    eax, ebx
        jmp    SHORT loc_2_334A
loc_2_3332:
        sub    edi, esi
        mov    edx, ebx
        mov    ecx, esi
        cmp    edi, 1
        sbb    eax, eax
        inc    eax
        push   eax
        call   sub_2_2EBE
        mov    eax, ebx
        jmp    SHORT loc_2_334A
loc_2_3348:
        xor    eax, eax
loc_2_334A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_3351:
        push   ebp
        mov    eax, 1
        mov    ebp, esp
        sub    esp, 0d8h
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        cmp    ecx, eax
        mov    esi, dword ptr [ebp + 14h]
        je     SHORT loc_2_3377
        cmp    ecx, 2
        je     SHORT loc_2_33C0
        cmp    ecx, 4
        je     SHORT loc_2_33C0
        jmp    SHORT loc_2_33D3
loc_2_3377:
        push   0
        lea    ecx, [ebp - 0d8h]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_2_7A20
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_3396
        mov    eax, 13h
        jmp    SHORT loc_2_33D3
loc_2_3396:
        cmp    word ptr [ebp - 2ah], 0
        jne    SHORT loc_2_33A4
        mov    eax, 13h
        jmp    SHORT loc_2_33D3
loc_2_33A4:
        mov    bl, byte ptr [ebp - 28h]
        mov    dl, byte ptr [esi]
        mov    ecx, ebx
        call   sub_2_4CEB
        test   eax, eax
        jne    SHORT loc_2_33BB
        mov    eax, 13h
        jmp    SHORT loc_2_33D3
loc_2_33BB:
        mov    byte ptr [esi + 1], bl
        jmp    SHORT loc_2_33D1
loc_2_33C0:
        xor    ebx, ebx
        mov    dl, byte ptr [esi]
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ebx
        mov    byte ptr [esi + 1], bl
        call   sub_2_4CEB
loc_2_33D1:
        xor    eax, eax
loc_2_33D3:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_33D9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0d4h
        mov    word ptr [ebp - 0ch], 3
        push   ebx
        push   esi
        push   edi
        mov    bl, cl
        movzx  si, bl
        movzx  eax, si
        xor    edi, edi
        push   eax
        mov    word ptr [ebp - 0ah], di
        db  068h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 6], di
        lea    ecx, [ebp - 0d4h]
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    edx, [ebp - 0d4h]
        lea    eax, [ebp - 4]
        push   1
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        call   sub_2_7A18
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_3442
        cmp    eax, 10h
        je     NEAR PTR loc_2_34ED
        xor    eax, eax
        jmp    NEAR PTR loc_2_34F2
loc_2_3442:
        xor    edi, edi
        xor    edx, edx
        push   edi
        mov    ecx, dword ptr [ebp - 4]
        push   10h
        push   edi
        call   sub_2_426A
        push   edi
        lea    ecx, [ebp - 10h]
        push   3000h
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_3472
        xor    eax, eax
        jmp    NEAR PTR loc_2_34F2
loc_2_3472:
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        mov    word ptr [ebp - 8], ax
        push   0
        push   8
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   4
        push   dword ptr [ebp - 10h]
        push   0
        call   sub_2_79A0
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_2_349E
        xor    eax, eax
        jmp    SHORT loc_2_34F2
loc_2_349E:
        push   3
        lea    ecx, [ebp - 10h]
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_34B9
        xor    eax, eax
        jmp    SHORT loc_2_34F2
loc_2_34B9:
        mov    word ptr [ebp - 0ah], si
        push   0
        push   8
        lea    eax, [ebp - 0ch]
        push   eax
        push   4
        push   dword ptr [ebp - 10h]
        push   0
        call   sub_2_79A0
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_2_34DC
        xor    eax, eax
        jmp    SHORT loc_2_34F2
loc_2_34DC:
        push   7
        push   0
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_7A08
        add    esp, 10h
loc_2_34ED:
        mov    eax, 1
loc_2_34F2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_34F9:
        push   ebp
        mov    ebp, esp
        sub    esp, 114h
        xor    eax, eax
        mov    ecx, 3fh
        mov    byte ptr [ebp - 114h], 0
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 113h]
        rep stosd
        stosw
        stosb
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_368A
        xor    edx, edx
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  039h, 015h, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_357B
        db  0A1h, 000h, 000h, 000h, 000h
loc_2_3545:
        cmp    byte ptr [edi], 0
        jne    SHORT loc_2_3575
        lea    esi, [edi + 2]
        mov    ecx, 4
loc_2_3552:
        mov    bl, byte ptr [esi]
        test   bl, bl
        je     SHORT loc_2_356F
        movzx  ebx, bl
        lea    ebx, [ebp + ebx - 114h]
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_2_356D
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_2_356D:
        inc    byte ptr [ebx]
loc_2_356F:
        add    esi, 3
        dec    ecx
        jne    SHORT loc_2_3552
loc_2_3575:
        add    edi, 10h
        dec    eax
        jne    SHORT loc_2_3545
loc_2_357B:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_368A
        push   201h
        db  0A1h, 000h, 000h, 000h, 000h
        imul   eax, eax, 0ch
        push   eax
        call   sub_2_79C8
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_2_35B3
        call   sub_2_3D54
        xor    eax, eax
        jmp    NEAR PTR loc_2_368F
loc_2_35B3:
        xor    edx, edx
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_35BA:
        cmp    byte ptr [ebp + edx - 114h], 0
        je     SHORT loc_2_3623
        mov    si, 0ffffh
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax], dl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_3612
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], ecx
loc_2_35E2:
        cmp    byte ptr [edi], 0
        jne    SHORT loc_2_360A
        mov    dword ptr [ebp - 10h], 4
        lea    ecx, [edi + 3]
loc_2_35F1:
        movzx  ebx, byte ptr [ecx - 1]
        cmp    ebx, edx
        jne    SHORT loc_2_3602
        cmp    word ptr [ecx], 0
        je     SHORT loc_2_3602
        and    si, word ptr [ecx]
loc_2_3602:
        add    ecx, 3
        dec    dword ptr [ebp - 10h]
        jne    SHORT loc_2_35F1
loc_2_360A:
        add    edi, 10h
        dec    dword ptr [ebp - 0ch]
        jne    SHORT loc_2_35E2
loc_2_3612:
        cmp    si, 0ffffh
        jne    SHORT loc_2_361C
        xor    si, si
loc_2_361C:
        mov    word ptr [eax + 2], si
        add    eax, 0ch
loc_2_3623:
        inc    edx
        cmp    edx, 100h
        jb     SHORT loc_2_35BA
        xor    edi, edi
        lea    eax, [ebp - 8]
        push   eax
        xor    esi, esi
        mov    dword ptr [ebp - 8], edi
        db  0FFh, 015h, 00Ch, 000h, 000h, 000h
        add    esp, 4
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_368A
loc_2_364E:
        mov    al, byte ptr [ebx]
        lea    ecx, [ebp - 1]
        push   eax
        push   ecx
        db  0FFh, 015h, 004h, 000h, 000h, 000h
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_367E
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_2_367E
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        je     SHORT loc_2_367E
        mov    ecx, ebx
        call   sub_2_3108
loc_2_367E:
        inc    esi
        add    ebx, 0ch
        db  039h, 035h, 000h, 000h, 000h, 000h
        ja     SHORT loc_2_364E
loc_2_368A:
        mov    eax, 1
loc_2_368F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  056h, 057h, 033h, 0F6h, 08Bh, 03Dh, 000h, 000h, 000h, 000h, 039h, 035h, 000h, 000h, 000h, 000h
        db  074h, 01Eh, 08Bh, 047h, 004h, 085h, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0E8h, 071h, 043h, 000h
        db  000h, 083h, 0C4h, 008h, 046h, 083h, 0C7h, 00Ch, 03Bh, 035h, 000h, 000h, 000h, 000h, 072h, 0E2h
        db  05Fh, 05Eh, 0C3h
sub_2_36C9:
        push   esi
        xor    eax, eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    edx, eax
        je     SHORT loc_2_36E8
loc_2_36DC:
        cmp    byte ptr [esi + 1], cl
        jne    SHORT loc_2_36E2
        inc    eax
loc_2_36E2:
        add    esi, 0ch
        dec    edx
        jne    SHORT loc_2_36DC
loc_2_36E8:
        pop    esi
        ret
sub_2_36EA:
        sub    esp, 4
        push   ebx
        push   0
        push   dword ptr [ecx + 4]
        mov    bl, byte ptr [ecx + 1]
        call   sub_2_7A30
        add    esp, 8
        mov    ecx, ebx
        call   sub_2_36C9
        test   eax, eax
        jne    SHORT loc_2_3733
        lea    eax, [esp + 4]
        push   eax
        db  0FFh, 015h, 00Ch, 000h, 000h, 000h
        add    esp, 4
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        not    eax
        and    dword ptr [esp + 4], eax
        push   dword ptr [esp + 4]
        db  0FFh, 015h, 008h, 000h, 000h, 000h
        add    esp, 4
loc_2_3733:
        pop    ebx
        add    esp, 4
        ret
        db  056h, 057h, 033h, 0FFh, 08Bh, 035h, 000h, 000h, 000h, 000h, 039h, 03Dh, 000h, 000h, 000h, 000h
        db  074h, 01Fh, 083h, 07Eh, 004h, 000h, 074h, 00Dh, 083h, 07Eh, 008h, 000h, 075h, 007h, 08Bh, 0CEh
        db  0E8h, 08Dh, 0FFh, 0FFh, 0FFh, 047h, 083h, 0C6h, 00Ch, 03Bh, 03Dh, 000h, 000h, 000h, 000h, 072h
        db  0E1h, 05Fh, 05Eh, 0C3h, 0E8h, 025h, 0FFh, 0FFh, 0FFh, 0E9h, 0C2h, 0FFh, 0FFh, 0FFh
sub_2_3776:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        xor    eax, eax
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 10h], si
        lea    edi, [ebp - 0eh]
        mov    dword ptr [ebp - 8], ecx
        lea    ecx, [ebp - 28h]
        stosd
        stosw
        lea    edi, [ebp - 10h]
        mov    dword ptr [ebp - 24h], esi
        mov    dword ptr [ebp - 28h], 0b10eh
        mov    dword ptr [ebp - 14h], edi
        call   sub_2_4745
        cmp    word ptr [ebp - 10h], si
        jne    SHORT loc_2_37B8
        xor    eax, eax
        jmp    NEAR PTR loc_2_3843
loc_2_37B8:
        mov    ax, word ptr [ebp - 10h]
        push   201h
        shr    ax, 4
        movzx  ebx, ax
        movzx  eax, word ptr [ebp - 10h]
        add    eax, 20h
        push   eax
        call   sub_2_79C8
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_2_37E2
        xor    eax, eax
        jmp    SHORT loc_2_3843
loc_2_37E2:
        lea    eax, [esi + 20h]
        mov    dword ptr [ebp - 0eh], eax
        mov    word ptr [ebp - 0ah], ds
        lea    ecx, [ebp - 28h]
        mov    dword ptr [ebp - 14h], edi
        mov    dword ptr [ebp - 28h], 0b10eh
        mov    dword ptr [ebp - 24h], 0
        call   sub_2_4745
        test   eax, eax
        jne    SHORT loc_2_3818
        push   0
        push   esi
        call   sub_2_7950
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_2_3843
loc_2_3818:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 24h]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], ebx
        mov    word ptr [eax], dx
        mov    ax, word ptr [ebp - 10h]
        add    ax, 20h
        mov    word ptr [esi + 6], ax
        mov    eax, 1
        mov    edx, dword ptr [ebp - 24h]
        mov    word ptr [esi + 0ah], dx
loc_2_3843:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_384C:
        push   ebx
        cmp    dword ptr [ecx], 52495024h
        push   esi
        jne    SHORT loc_2_3895
        cmp    word ptr [ecx + 4], 100h
        jne    SHORT loc_2_3895
        mov    ax, word ptr [ecx + 6]
        test   al, 0fh
        jne    SHORT loc_2_3895
        cmp    ax, 20h
        jbe    SHORT loc_2_3895
        xor    bl, bl
        xor    esi, esi
        test   ax, ax
        je     SHORT loc_2_3880
        movzx  eax, ax
loc_2_3878:
        add    bl, byte ptr [esi + ecx]
        inc    esi
        cmp    esi, eax
        jb     SHORT loc_2_3878
loc_2_3880:
        test   bl, bl
        je     SHORT loc_2_388E
        cmp    byte ptr [ecx + 1fh], 0
        jne    SHORT loc_2_3895
        test   edx, edx
        je     SHORT loc_2_3895
loc_2_388E:
        mov    eax, 1
        jmp    SHORT loc_2_3897
loc_2_3895:
        xor    eax, eax
loc_2_3897:
        pop    esi
        pop    ebx
        ret
sub_2_389A:
        sub    esp, 8
        mov    dword ptr [esp + 4], edx
        push   ebx
        mov    dword ptr [esp + 4], ecx
        push   esi
        push   edi
        push   ebp
        push   0
        push   10000h
        push   0f0000h
        call   sub_2_7A38
        add    esp, 0ch
        mov    ebx, eax
        lea    esi, [eax + 10000h]
        cmp    eax, esi
        jae    SHORT loc_2_38EF
loc_2_38C9:
        cmp    dword ptr [ebx], 52495024h
        jne    SHORT loc_2_38E8
        movzx  eax, word ptr [ebx + 6]
        add    eax, ebx
        cmp    eax, esi
        ja     SHORT loc_2_38E8
        xor    edx, edx
        mov    ecx, ebx
        call   sub_2_384C
        test   eax, eax
        jne    SHORT loc_2_38FB
loc_2_38E8:
        add    ebx, 10h
        cmp    ebx, esi
        jb     SHORT loc_2_38C9
loc_2_38EF:
        xor    eax, eax
loc_2_38F1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    10h
loc_2_38FB:
        movzx  ebp, word ptr [ebx + 6]
        push   201h
        push   ebp
        call   sub_2_79C8
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_38EF
        mov    edi, eax
        mov    esi, ebx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 10h]
        mov    dx, word ptr [ebx + 0ah]
        mov    dword ptr [ecx], eax
        movzx  ecx, word ptr [ebx + 6]
        sub    ecx, 20h
        mov    eax, dword ptr [esp + 14h]
        shr    ecx, 4
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    word ptr [ecx], dx
        mov    eax, dword ptr [esp + 20h]
        mov    dl, byte ptr [ebx + 8]
        mov    cl, byte ptr [ebx + 9]
        mov    ebx, dword ptr [ebx + 0ch]
        mov    byte ptr [eax], dl
        mov    edx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 28h]
        mov    byte ptr [edx], cl
        mov    dword ptr [eax], ebx
        mov    eax, 1
        jmp    SHORT loc_2_38F1
sub_2_3966:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        mov    ebx, edx
        push   edi
        push   eax
        mov    edi, ecx
        db  068h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79F0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_3995
        xor    eax, eax
        jmp    NEAR PTR loc_2_3A82
loc_2_3995:
        xor    eax, eax
        lea    ecx, [ebp - 8]
        push   ecx
        mov    dword ptr [ebp - 8], eax
        push   eax
        push   eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_2_7A40
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_2_39C7
        push   dword ptr [ebp - 4]
        call   sub_2_79E8
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_2_3A82
loc_2_39C7:
        push   201h
        push   dword ptr [ebp - 8]
        call   sub_2_79C8
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_2_39EF
        push   dword ptr [ebp - 4]
        call   sub_2_79E8
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_2_3A82
loc_2_39EF:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_2_7A40
        add    esp, 18h
        test   eax, eax
        push   dword ptr [ebp - 4]
        je     SHORT loc_2_3A26
        call   sub_2_79E8
        add    esp, 4
        push   0
        push   esi
        call   sub_2_7950
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_2_3A82
loc_2_3A26:
        call   sub_2_79E8
        add    esp, 4
        mov    edx, 1
        mov    ecx, esi
        call   sub_2_384C
        test   eax, eax
        jne    SHORT loc_2_3A4D
        push   0
        push   esi
        call   sub_2_7950
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_2_3A82
loc_2_3A4D:
        mov    cx, word ptr [esi + 0ah]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx], cx
        mov    dword ptr [edi], esi
        movzx  eax, word ptr [esi + 6]
        sub    eax, 20h
        mov    cl, byte ptr [esi + 9]
        shr    eax, 4
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], eax
        mov    bl, byte ptr [esi + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [edx], cl
        mov    byte ptr [eax], bl
        mov    ebx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], ebx
        mov    eax, 1
loc_2_3A82:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_2_3A8B:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ecx], eax
        mov    dword ptr [edx], eax
        mov    edi, edx
        mov    esi, ecx
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ebx], ax
        mov    edx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 10h]
        push   edx
        push   eax
        mov    byte ptr [ecx], 0ffh
        push   ecx
        mov    byte ptr [eax], 0ffh
        push   ebx
        mov    ecx, esi
        mov    dword ptr [edx], 0ffffffffh
        mov    edx, edi
        call   sub_2_3966
        test   eax, eax
        je     SHORT loc_2_3ACE
        mov    eax, 20h
        jmp    SHORT loc_2_3B0A
loc_2_3ACE:
        mov    edx, edi
        mov    ecx, esi
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_389A
        test   eax, eax
        je     SHORT loc_2_3AEC
        mov    eax, 21h
        jmp    SHORT loc_2_3B0A
loc_2_3AEC:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 020h
        je     SHORT loc_2_3B08
        push   ebx
        mov    edx, edi
        mov    ecx, esi
        call   sub_2_3776
        test   eax, eax
        mov    eax, 22h
        jne    SHORT loc_2_3B0A
loc_2_3B08:
        xor    eax, eax
loc_2_3B0A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_2_3B11:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        jne    SHORT loc_2_3B70
        push   0
        mov    edx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79F0
        add    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_3B70
        mov    al, byte ptr [ebp + 0ch]
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_3B70:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_3B75:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        db  0A3h, 000h, 000h, 000h, 000h
        sub    esp, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        mov    bl, dl
        mov    esi, ecx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79F0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_3BB3
        mov    eax, 1
        jmp    NEAR PTR loc_2_3D1F
loc_2_3BB3:
        db  068h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79F0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_3BF6
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_3F18
        mov    esi, eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79E8
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_3D14
loc_2_3BF6:
        cmp    bl, 0ffh
        je     NEAR PTR loc_2_3CD2
        cmp    byte ptr [ebp + 8], 0ffh
        je     SHORT loc_2_3C6A
        push   0
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        cmp    eax, -1
        je     SHORT loc_2_3C6A
        push   eax
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79F0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_3C6A
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_3F18
        mov    esi, eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79E8
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_3D14
loc_2_3C6A:
        cmp    bl, 0ffh
        je     SHORT loc_2_3CD2
        cmp    byte ptr [ebp + 8], 0ffh
        je     SHORT loc_2_3CD2
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, -1
        je     SHORT loc_2_3CD2
        test   eax, eax
        je     SHORT loc_2_3CD2
        push   eax
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_79F0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_3CD2
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_3F18
        mov    esi, eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79E8
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_3D14
loc_2_3CD2:
        db  068h, 000h, 000h, 000h, 000h
        xor    edx, edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_65F2
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79E8
        add    esp, 4
        mov    eax, 2
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_3D1F
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_3F18
        mov    esi, eax
        db  0A1h, 000h, 000h, 000h, 000h
loc_2_3D14:
        push   eax
        call   sub_2_79E8
        add    esp, 4
        mov    eax, esi
loc_2_3D1F:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_3D27:
        push   esi
        mov    esi, edx
        mov    edx, ecx
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_2_4564
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, esi
        call   sub_2_4564
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        call   sub_2_4564
        pop    esi
        ret    4
sub_2_3D54:
        push   esi
        push   edi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_3D7B
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_7950
        add    esp, 8
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_2_3D7B:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_3D8E
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_2_402B
loc_2_3D8E:
        db  0BFh, 000h, 000h, 000h, 000h
        xor    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
        pop    edi
        pop    esi
        ret
sub_2_3D9D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_2_3D54
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_3DBE
        mov    edx, 0ffffffffh
        push   edx
        mov    ecx, edx
        jmp    NEAR PTR loc_2_3F0C
loc_2_3DBE:
        lea    eax, [esp + 0ch]
        db  0BAh, 000h, 000h, 000h, 000h
        push   eax
        db  0B9h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_3A8B
        cmp    eax, 20h
        mov    esi, eax
        jae    SHORT loc_2_3DF4
        push   -1
        mov    edx, 0ffffffffh
        jmp    NEAR PTR loc_2_3F0A
loc_2_3DF4:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        add    eax, 20h
        db  0B9h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 0ch]
        push   edx
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_2_3B75
        cmp    eax, 20h
        mov    edi, eax
        jae    SHORT loc_2_3E2F
        call   sub_2_3D54
        push   -1
        jmp    NEAR PTR loc_2_3F08
loc_2_3E2F:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        db  0A2h, 004h, 000h, 000h, 000h
        db  088h, 00Dh, 005h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        db  0C7h, 005h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax], 0ffh
        jne    SHORT loc_2_3E64
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax], cl
loc_2_3E64:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 9
        cmp    byte ptr [eax], 0ffh
        jne    SHORT loc_2_3E79
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax], cl
loc_2_3E79:
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_2_3F7D
        cmp    eax, 20h
        mov    ebx, eax
        jae    SHORT loc_2_3E92
        call   sub_2_3D54
        push   ebx
        jmp    SHORT loc_2_3F08
loc_2_3E92:
        db  0A1h, 014h, 000h, 000h, 000h
        db  08Bh, 00Dh, 01Ch, 000h, 000h, 000h
        db  08Bh, 015h, 024h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 02Ch, 000h, 000h, 000h
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        db  089h, 015h, 008h, 000h, 000h, 000h
        db  0A3h, 00Ch, 000h, 000h, 000h
        db  089h, 00Dh, 010h, 000h, 000h, 000h
        xor    eax, eax
        cmp    esi, 22h
        jne    SHORT loc_2_3ED6
        mov    eax, 1
loc_2_3ED6:
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0FFh, 015h, 010h, 000h, 000h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_3EF3
        call   sub_2_3D54
        push   3
        jmp    SHORT loc_2_3F08
loc_2_3EF3:
        call   sub_2_34F9
        test   eax, eax
        jne    SHORT loc_2_3F06
        mov    ecx, 0ffffffffh
        push   ecx
        mov    edx, ecx
        jmp    SHORT loc_2_3F0C
loc_2_3F06:
        push   20h
loc_2_3F08:
        mov    edx, edi
loc_2_3F0A:
        mov    ecx, esi
loc_2_3F0C:
        call   sub_2_3D27
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_2_3F18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, ecx
        lea    ecx, [edx + 0eh]
        mov    edi, edx
        mov    dword ptr [ebp - 4], 80h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_7A40
        add    esp, 18h
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_2_3F77
        lea    eax, [ebp - 4]
        add    edi, 6
        mov    dword ptr [ebp - 4], 4
        push   eax
        push   edi
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_7A40
        add    esp, 18h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 20h
loc_2_3F77:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_3F7D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    dword ptr [ecx], 0
        push   edi
        mov    esi, ecx
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 4]
        lea    ecx, [esi + 0eh]
        push   ecx
        push   edx
        call   sub_2_7A58
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_3FAB
        xor    eax, eax
        jmp    SHORT loc_2_4024
loc_2_3FAB:
        mov    edi, dword ptr [esi + 0ah]
        cmp    dword ptr [edi], 0
        je     SHORT loc_2_3FD4
loc_2_3FB3:
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [edi]
        push   dword ptr [ebp - 4]
        call   sub_2_7A50
        add    esp, 0ch
        mov    dword ptr [edi + 4], eax
        test   eax, eax
        je     SHORT loc_2_4003
        add    edi, 8
        cmp    dword ptr [edi], 0
        jne    SHORT loc_2_3FB3
loc_2_3FD4:
        mov    eax, dword ptr [esi + 0ah]
        mov    cl, byte ptr [esi + 5]
        push   ecx
        mov    dl, byte ptr [esi + 4]
        push   edx
        push   dword ptr [esi + 6]
        call   dword ptr [eax + 4]
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_401A
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_2_7A48
        add    esp, 8
        mov    eax, 2
        jmp    SHORT loc_2_4024
loc_2_4003:
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_2_7A48
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_2_4024
loc_2_401A:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi], eax
        mov    eax, 20h
loc_2_4024:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_402B:
        push   esi
        push   edi
        mov    esi, dword ptr [ecx]
        mov    edi, ecx
        test   esi, esi
        je     SHORT loc_2_4068
        mov    eax, dword ptr [edi + 0ah]
        call   dword ptr [eax + 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_7A48
        add    esp, 8
        mov    eax, dword ptr [edi + 0ah]
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_405D
        xor    ecx, ecx
loc_2_4053:
        mov    dword ptr [eax + 4], ecx
        add    eax, 8
        cmp    dword ptr [eax], ecx
        jne    SHORT loc_2_4053
loc_2_405D:
        xor    eax, eax
        mov    ecx, 23h
        rep stosd
        stosw
loc_2_4068:
        pop    edi
        pop    esi
        ret
sub_2_406B:
        push   0eh
        call   sub_2_58C5
        and    al, 7fh
        ret
sub_2_4075:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    byte ptr [ebp - 4], dl
        push   esi
        mov    bl, cl
        call   sub_2_44FB
        push   0
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, ebx
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 0ch]
        call   sub_2_4521
        mov    esi, eax
        call   sub_2_4592
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_40BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   edi
        mov    edx, dword ptr [ebp - 1]
        push   4
        mov    ecx, ebx
        call   sub_2_58D3
        mov    word ptr [ebp - 4], ax
        mov    eax, 3
        movzx  ecx, word ptr [ebp - 4]
        and    eax, ecx
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 8], eax
        call   sub_2_4075
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_41F2
        test   eax, 10000h
        je     NEAR PTR loc_2_41F2
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_406B
        cmp    al, 2
        jne    NEAR PTR loc_2_41F2
        mov    dword ptr [ebp - 8], 0
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_2_4195
        push   10h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        test   eax, eax
        je     SHORT loc_2_4146
        mov    eax, 1
        jmp    NEAR PTR loc_2_41F5
loc_2_4146:
        push   1ch
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        mov    esi, eax
        push   20h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        push   24h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        mov    dword ptr [ebp - 0ch], eax
        call   sub_2_58E1
        mov    edi, eax
        push   28h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        test   esi, esi
        je     SHORT loc_2_4186
        cmp    dword ptr [ebp - 0ch], esi
        jae    SHORT loc_2_418E
loc_2_4186:
        test   edi, edi
        je     SHORT loc_2_4195
        cmp    eax, edi
        jb     SHORT loc_2_4195
loc_2_418E:
        mov    eax, 1
        jmp    SHORT loc_2_41F5
loc_2_4195:
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_41F2
        push   2ch
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58D3
        push   30h
        mov    edx, dword ptr [ebp - 1]
        movzx  esi, ax
        mov    ecx, ebx
        call   sub_2_58D3
        push   34h
        mov    edx, dword ptr [ebp - 1]
        movzx  eax, ax
        mov    ecx, ebx
        mov    dword ptr [ebp - 0ch], eax
        call   sub_2_58D3
        push   38h
        mov    edx, dword ptr [ebp - 1]
        movzx  edi, ax
        mov    ecx, ebx
        call   sub_2_58D3
        movzx  eax, ax
        test   esi, esi
        je     SHORT loc_2_41E3
        cmp    dword ptr [ebp - 0ch], esi
        jae    SHORT loc_2_41EB
loc_2_41E3:
        test   edi, edi
        je     SHORT loc_2_41F2
        cmp    eax, edi
        jb     SHORT loc_2_41F2
loc_2_41EB:
        mov    eax, 1
        jmp    SHORT loc_2_41F5
loc_2_41F2:
        mov    eax, dword ptr [ebp - 8]
loc_2_41F5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_41FE:
        cmp    cx, 0ffffh
        je     SHORT loc_2_420F
        test   cx, cx
        mov    eax, 1
        jne    SHORT loc_2_4211
loc_2_420F:
        xor    eax, eax
loc_2_4211:
        ret
sub_2_4212:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0
        mov    esi, edx
        push   eax
        push   ecx
        lea    edx, [ebp - 4]
        push   edx
        call   sub_2_7A60
        add    esp, 10h
        test   esi, esi
        je     SHORT loc_2_4239
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [esi], ah
loc_2_4239:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_2_4245
        mov    cl, byte ptr [ebp - 4]
        mov    byte ptr [eax], cl
loc_2_4245:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_2_4254
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 10h
        mov    byte ptr [eax], cl
loc_2_4254:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_2_4263
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 10h
        mov    byte ptr [eax], ch
loc_2_4263:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_2_426A:
        movzx  eax, byte ptr [esp + 8]
        push   esi
        push   0
        movzx  esi, byte ptr [esp + 14h]
        shl    esi, 8
        add    esi, eax
        movzx  eax, dl
        shl    esi, 8
        add    esi, eax
        movzx  eax, byte ptr [esp + 0ch]
        shl    esi, 8
        add    esi, eax
        push   esi
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_7A68
        add    esp, 10h
        pop    esi
        ret    0ch
sub_2_42A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    edx, [ebp - 1]
        mov    esi, ecx
        push   0
        push   edx
        push   0
        xor    edx, edx
        call   sub_2_4212
        test   byte ptr [ebp - 1], 4
        mov    eax, 0
        je     SHORT loc_2_42D8
        push   0
        lea    eax, [ebp - 4]
        push   esi
        push   esi
        push   eax
        call   sub_2_7A60
        add    esp, 10h
        mov    eax, dword ptr [ebp - 4]
loc_2_42D8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_42DD:
        push   0
        push   edx
        push   ecx
        push   ecx
        call   sub_2_7A68
        add    esp, 10h
        ret
sub_2_42EB:
        db  066h, 081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0B8h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_430A
loc_2_42FB:
        cmp    word ptr [eax], cx
        je     SHORT loc_2_430A
        add    eax, 6
        cmp    word ptr [eax], 0ffffh
        jne    SHORT loc_2_42FB
loc_2_430A:
        mov    eax, dword ptr [eax + 2]
        ret
sub_2_430E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0
        lea    eax, [ebp - 4]
        push   ecx
        mov    dword ptr [ebp - 4], ecx
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        db  02Bh, 005h, 000h, 000h, 000h, 000h
        mov    esp, ebp
        cmp    eax, 1
        pop    ebp
        sbb    eax, eax
        neg    eax
        ret
sub_2_433A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        xor    ebx, ebx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        mov    edx, eax
        cmp    byte ptr [ebp - 1], bl
        je     NEAR PTR loc_2_440C
        cmp    dword ptr [ebp - 8], ebx
        je     NEAR PTR loc_2_440C
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 8], eax
        je     NEAR PTR loc_2_440C
        db  0BEh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    edi, esi
        mov    ecx, 0eh
        rep stosd
        stosb
        mov    eax, edx
        shr    eax, 10h
        movzx  edx, dx
        movzx  ecx, ax
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        xor    esi, esi
        call   sub_2_79F8
        add    esp, 10h
loc_2_43A7:
        db  08Dh, 08Bh, 016h, 000h, 000h, 000h
        add    ebx, 2
        movzx  eax, byte ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 8], ecx
        je     SHORT loc_2_43F1
        push   esi
        lea    eax, [ebp + 8]
        push   esi
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        jmp    SHORT loc_2_43A7
loc_2_43F1:
        movzx  eax, byte ptr [ebp - 1]
        push   eax
        db  081h, 0C3h, 016h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        call   sub_2_79F8
        add    esp, 0ch
        jmp    SHORT loc_2_4443
loc_2_440C:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        and    eax, 7
        push   eax
        mov    al, byte ptr [ebp + 8]
        shr    al, 3
        movzx  ebx, byte ptr [ebp - 1]
        movzx  ecx, al
        push   ecx
        mov    eax, edx
        shr    eax, 10h
        push   ebx
        movzx  ecx, ax
        movzx  edx, dx
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_79F8
        add    esp, 1ch
loc_2_4443:
        db  0B8h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_4451:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, ebx
        mov    ecx, dword ptr [ebp + 8]
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 1]
        call   sub_2_433A
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7A18
        add    esp, 10h
        cmp    eax, 10h
        jne    SHORT loc_2_44AB
        test   esi, esi
        je     SHORT loc_2_4492
        xor    eax, eax
        jmp    SHORT loc_2_44F3
loc_2_4492:
        push   0
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_44B3
        xor    eax, eax
        jmp    SHORT loc_2_44F3
loc_2_44AB:
        test   eax, eax
        je     SHORT loc_2_44B3
        xor    eax, eax
        jmp    SHORT loc_2_44F3
loc_2_44B3:
        mov    al, 0
        cmp    esi, 1
        adc    al, 0ffh
        xor    esi, esi
        and    al, 4
        push   esi
        or     al, 1
        mov    edx, ebx
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   dword ptr [ebp - 1]
        call   sub_2_426A
        mov    word ptr [ebp - 0ah], si
        mov    cl, byte ptr [ebp - 1]
        push   esi
        mov    byte ptr [ebp - 0ch], bl
        lea    ebx, [ebp - 0ch]
        mov    byte ptr [ebp - 0bh], cl
        push   ebx
        push   4
        push   4
        push   dword ptr [ebp - 8]
        call   sub_2_7978
        add    esp, 14h
        mov    eax, dword ptr [ebp - 8]
loc_2_44F3:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_44FB:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_451A
        db  068h, 000h, 000h, 000h, 000h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   2
        call   sub_2_7A70
        add    esp, 10h
loc_2_451A:
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        ret
sub_2_4521:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        sub    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_4560
        xor    edx, edx
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 4], edx
        mov    dword ptr [ebp - 8], 4
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   edx
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_7A40
        add    esp, 18h
        mov    eax, dword ptr [ebp - 4]
loc_2_4560:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_4564:
        sub    esp, 4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esp], edx
        je     SHORT loc_2_458E
        lea    eax, [esp]
        push   4
        push   eax
        push   3
        push   0
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_7A78
        add    esp, 18h
loc_2_458E:
        add    esp, 4
        ret
_PTEXT ends
        end
