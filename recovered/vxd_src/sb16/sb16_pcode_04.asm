; sb16_pcode_04.asm -- SB16 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  08Bh, 044h, 024h, 004h, 08Bh, 048h, 030h, 085h, 0C9h, 075h, 009h, 08Bh, 054h, 024h, 008h, 089h
        db  050h, 030h, 0EBh, 01Dh, 083h, 079h, 014h, 000h, 074h, 009h, 08Bh, 049h, 014h, 083h, 079h, 014h
        db  000h, 075h, 0F7h, 08Bh, 054h, 024h, 008h, 089h, 051h, 014h, 0C7h, 042h, 014h, 000h, 000h, 000h
        db  000h, 0C3h
sub_4_0032:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 30h]
        cmp    eax, edx
        jne    SHORT loc_4_0049
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ecx + 30h], eax
        jmp    SHORT loc_4_005F
loc_4_0049:
        cmp    dword ptr [edx + 14h], eax
        je     SHORT loc_4_0056
loc_4_004E:
        mov    edx, dword ptr [edx + 14h]
        cmp    dword ptr [edx + 14h], eax
        jne    SHORT loc_4_004E
loc_4_0056:
        mov    eax, dword ptr [edx + 14h]
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [edx + 14h], ecx
loc_4_005F:
        ret
sub_4_0060:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        jne    SHORT loc_4_0071
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 30h]
        jmp    SHORT loc_4_0074
loc_4_0071:
        mov    eax, dword ptr [eax + 14h]
loc_4_0074:
        ret
loc_4_0075:
        push   ebp
        mov    ecx, 10h
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], 0
        repe cmpsb
        mov    eax, dword ptr [ebp + 8]
        jne    SHORT loc_4_0099
        mov    dword ptr [edx], eax
loc_4_0099:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_00AF
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_00AF:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_4_00BF
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    SHORT loc_4_00C4
loc_4_00BF:
        mov    eax, 80004002h
loc_4_00C4:
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_4_00CA:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 1ch]
        inc    eax
        mov    dword ptr [ecx + 1ch], eax
        ret    4
loc_4_00D8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 1ch]
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_4_012A
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_4_0101
        mov    eax, dword ptr [esi + 88h]
        test   eax, eax
        je     SHORT loc_4_010A
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        jmp    SHORT loc_4_010A
loc_4_0101:
        push   esi
        call   sub_4_0AAE
        add    esp, 4
loc_4_010A:
        push   esi
        push   dword ptr [esi + 20h]
        call   sub_4_0032
        add    esp, 8
        mov    dword ptr [esi + 18h], 0deaddeadh
        push   0
        push   esi
        call   sub_4_2D4C
        add    esp, 8
        xor    eax, eax
loc_4_012A:
        pop    esi
        ret    4
loc_4_012E:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0ch
loc_4_0145:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_0158:
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_016B:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 84h], 0
        jne    SHORT loc_4_0185
        mov    eax, 80004001h
        jmp    SHORT loc_4_01AF
loc_4_0185:
        mov    ecx, dword ptr [eax + 88h]
        mov    eax, 88780096h
        test   ecx, ecx
        je     SHORT loc_4_01AF
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   dword ptr [eax + 0ch]
loc_4_01AF:
        pop    ebp
        ret    20h
loc_4_01B3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 84h], 0
        jne    SHORT loc_4_01CD
        mov    eax, 80004001h
        jmp    SHORT loc_4_01EE
loc_4_01CD:
        mov    ecx, dword ptr [eax + 88h]
        mov    eax, 88780096h
        test   ecx, ecx
        je     SHORT loc_4_01EE
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   dword ptr [eax + 10h]
loc_4_01EE:
        pop    ebp
        ret    14h
loc_4_01F2:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   dword ptr [ecx + 4]
        call   sub_4_0B72
        add    esp, 8
        ret    8
loc_4_0208:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 84h], 0
        je     SHORT loc_4_0235
        mov    eax, dword ptr [eax + 88h]
        test   eax, eax
        jne    SHORT loc_4_0229
        mov    eax, 88780096h
        jmp    SHORT loc_4_0242
loc_4_0229:
        push   dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 18h]
        jmp    SHORT loc_4_0242
loc_4_0235:
        push   dword ptr [esp + 8]
        push   eax
        call   sub_4_0B34
        add    esp, 8
loc_4_0242:
        ret    8
loc_4_0245:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 84h], 0
        jne    SHORT loc_4_025C
        mov    eax, 8878001eh
        jmp    SHORT loc_4_0277
loc_4_025C:
        mov    eax, dword ptr [eax + 88h]
        test   eax, eax
        jne    SHORT loc_4_026D
        mov    eax, 88780096h
        jmp    SHORT loc_4_0277
loc_4_026D:
        push   dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 1ch]
loc_4_0277:
        ret    8
loc_4_027A:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 84h], 0
        jne    SHORT loc_4_0291
        mov    eax, 80004001h
        jmp    SHORT loc_4_02AC
loc_4_0291:
        mov    eax, dword ptr [eax + 88h]
        test   eax, eax
        jne    SHORT loc_4_02A2
        mov    eax, 88780096h
        jmp    SHORT loc_4_02AC
loc_4_02A2:
        push   dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 20h]
loc_4_02AC:
        ret    8
loc_4_02AF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 84h], 0
        je     SHORT loc_4_02E4
        mov    ecx, dword ptr [eax + 88h]
        test   ecx, ecx
        jne    SHORT loc_4_02D6
        mov    eax, 88780096h
        jmp    SHORT loc_4_0304
loc_4_02D6:
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   dword ptr [eax + 24h]
        jmp    SHORT loc_4_0304
loc_4_02E4:
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        call   sub_4_0D57
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_0304
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    dword ptr [edx], ecx
loc_4_0304:
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_030A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 4]
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_4_0340
        mov    ecx, dword ptr [esi + 88h]
        test   ecx, ecx
        jne    SHORT loc_4_032F
        mov    eax, 88780096h
        jmp    SHORT loc_4_0361
loc_4_032F:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   dword ptr [eax + 28h]
        jmp    SHORT loc_4_0361
loc_4_0340:
        cmp    dword ptr [esi + 24h], 1
        jne    SHORT loc_4_034A
        xor    eax, eax
        jmp    SHORT loc_4_0361
loc_4_034A:
        push   dword ptr [ebp + 14h]
        push   esi
        call   sub_4_0C5C
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_0361
        mov    dword ptr [esi + 24h], 1
loc_4_0361:
        pop    esi
        pop    ebp
        ret    10h
loc_4_0366:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        cmp    dword ptr [esi + 84h], 0
        je     SHORT loc_4_0390
        mov    eax, dword ptr [esi + 88h]
        test   eax, eax
        jne    SHORT loc_4_0388
        mov    eax, 88780096h
        jmp    SHORT loc_4_03AC
loc_4_0388:
        push   eax
        mov    esi, dword ptr [eax]
        call   dword ptr [esi + 2ch]
        jmp    SHORT loc_4_03AC
loc_4_0390:
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_4_039A
        xor    eax, eax
        jmp    SHORT loc_4_03AC
loc_4_039A:
        push   esi
        call   sub_4_0D02
        add    esp, 4
        xor    ecx, ecx
        test   eax, eax
        jne    SHORT loc_4_03AC
        mov    dword ptr [esi + 24h], ecx
loc_4_03AC:
        pop    esi
        ret    4
loc_4_03B0:
        push   ebp
        mov    ecx, 10h
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], 0
        repe cmpsb
        mov    eax, dword ptr [ebp + 8]
        jne    SHORT loc_4_03D4
        mov    dword ptr [edx], eax
loc_4_03D4:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_03EA
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_03EA:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_4_03FA
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    SHORT loc_4_03FF
loc_4_03FA:
        mov    eax, 80004002h
loc_4_03FF:
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_4_0405:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 18h]
        inc    eax
        mov    dword ptr [ecx + 18h], eax
        ret    4
loc_4_0413:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 18h]
        dec    eax
        mov    dword ptr [ecx + 18h], eax
        jne    SHORT loc_4_0434
        mov    dword ptr [ecx + 14h], 0deaddeadh
        push   0
        push   ecx
        call   sub_4_2D4C
        add    esp, 8
        xor    eax, eax
loc_4_0434:
        ret    4
loc_4_0437:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0ch
loc_4_044E:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_0461:
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_0474:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 100h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 4]
        push   ebp
        call   sub_4_12FC
        mov    edi, dword ptr [esp + 118h]
        mov    ecx, dword ptr [ebx + 28h]
        movzx  edx, word ptr [ecx + 681h]
        lea    esi, [edi + 4]
        push   edx
        push   eax
        xor    ebp, ebp
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_4_2D5C
        add    esp, 10h
        mov    dword ptr [edi + 210h], ebp
        mov    eax, dword ptr [ebx + 2ch]
        test   eax, eax
        je     SHORT loc_4_04FD
        push   edi
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_4_0559
        mov    eax, dword ptr [ebx + 28h]
        movzx  ecx, word ptr [eax + 681h]
        lea    eax, [esp + 10h]
        push   ecx
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_4_2D5C
        lea    eax, [esp + 20h]
        add    esp, 10h
        push   100h
        push   eax
        push   esi
        call   sub_4_2D54
        add    esp, 0ch
loc_4_04FD:
        lea    esi, [edi + 104h]
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        or     byte ptr [edi], 0bh
        push   esi
        call   sub_4_2D54
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 20h]
        xor    ecx, ecx
        mov    dword ptr [edi + 204h], eax
        mov    eax, 0b3h
        mov    dword ptr [edi + 20ch], ecx
        mov    word ptr [edi + 208h], 32a5h
        cmp    byte ptr [esi], cl
        je     SHORT loc_4_0547
loc_4_053C:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_4_053C
loc_4_0547:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 20ah], dx
        xor    eax, eax
loc_4_0559:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 100h
        ret    8
loc_4_0566:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        xor    eax, eax
        inc    dword ptr [esi + 24h]
        mov    ecx, dword ptr [esi + 2ch]
        test   ecx, ecx
        je     SHORT loc_4_0587
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_4_0587
        dec    dword ptr [esi + 24h]
loc_4_0587:
        pop    esi
        ret    4
loc_4_058B:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 2ch]
        test   ecx, ecx
        je     SHORT loc_4_05A2
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 14h]
loc_4_05A2:
        test   eax, eax
        jne    SHORT loc_4_05A9
        dec    dword ptr [esi + 24h]
loc_4_05A9:
        pop    esi
        ret    4
loc_4_05AD:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   ecx
        mov    edi, dword ptr [eax + 4]
        push   edi
        call   sub_4_0921
        mov    esi, dword ptr [ebp + 0ch]
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0ch], 1
        mov    dword ptr [esi], ecx
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [esi + 14h], ecx
        mov    dword ptr [esi + 18h], ecx
        mov    dword ptr [esi + 1ch], ecx
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], ecx
        mov    dword ptr [esi + 28h], ecx
        mov    dword ptr [esi + 2ch], ecx
        mov    dword ptr [esi + 30h], ecx
        mov    dword ptr [esi + 34h], ecx
        mov    dword ptr [esi + 38h], ecx
        mov    dword ptr [esi + 3ch], ecx
        mov    dword ptr [esi + 40h], ecx
        mov    dword ptr [esi + 44h], ecx
        mov    dword ptr [esi + 48h], ecx
        mov    eax, dword ptr [edi + 2ch]
        test   eax, eax
        je     SHORT loc_4_0627
        push   esi
        mov    edi, dword ptr [eax]
        push   eax
        call   dword ptr [edi + 18h]
        test   eax, eax
        jne    SHORT loc_4_0629
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [esi + 0ch]
        or     dword ptr [esi], eax
loc_4_0627:
        xor    eax, eax
loc_4_0629:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0631:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 20h]
        push   201h
        push   8ch
        mov    dword ptr [ecx], 0
        call   sub_4_2D64
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_4_0669
        mov    eax, 8007000eh
        jmp    NEAR PTR loc_4_072E
loc_4_0669:
        xor    eax, eax
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp + 10h], 1
        mov    dword ptr [edi + 8], edi
        mov    dword ptr [edi + 0ch], edi
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 20h], esi
        mov    dword ptr [edi + 18h], 8c4233c0h
        jne    SHORT loc_4_06D0
        mov    ecx, dword ptr [esi + 2ch]
        test   ecx, ecx
        jne    SHORT loc_4_06A2
        mov    eax, 88780096h
        jmp    NEAR PTR loc_4_072E
loc_4_06A2:
        lea    eax, [edi + 88h]
        push   eax
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   dword ptr [eax + 1ch]
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_0705
        mov    dword ptr [edi + 84h], 1
        jmp    SHORT loc_4_0701
loc_4_06D0:
        push   edi
        call   sub_4_0944
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_0701
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_4_0B72
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_0705
        mov    ecx, dword ptr [edi + 34h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [edi + 38h]
        mov    dword ptr [ecx], edx
loc_4_0701:
        test   ebx, ebx
        je     SHORT loc_4_0714
loc_4_0705:
        push   0
        push   edi
        call   sub_4_2D4C
        add    esp, 8
        mov    eax, ebx
        jmp    SHORT loc_4_072E
loc_4_0714:
        lea    eax, [edi + 4]
        mov    ecx, dword ptr [ebp + 20h]
        push   edi
        push   esi
        mov    dword ptr [ecx], eax
        call   0
        add    esp, 8
        mov    eax, dword ptr [edi]
        push   edi
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_072E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
loc_4_0735:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   ebp
        mov    edi, dword ptr [eax + 4]
        mov    ebp, dword ptr [ecx + 4]
        xor    eax, eax
        mov    dword ptr [esi], eax
        cmp    dword ptr [edi + 2ch], eax
        jne    SHORT loc_4_075E
        mov    eax, 88780096h
        jmp    NEAR PTR loc_4_07F1
loc_4_075E:
        push   201h
        push   8ch
        call   sub_4_2D64
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_077D
        mov    eax, 8007000eh
        jmp    SHORT loc_4_07F1
loc_4_077D:
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        db  0C7h, 043h, 004h, 000h, 000h, 000h, 000h
        lea    eax, [ebx + 88h]
        mov    dword ptr [ebx + 8], ebx
        push   eax
        mov    dword ptr [ebx + 0ch], ebx
        mov    dword ptr [ebx + 24h], 0
        mov    dword ptr [ebx + 20h], edi
        mov    dword ptr [ebx + 18h], 8c4233c0h
        push   dword ptr [ebp + 88h]
        mov    ecx, dword ptr [edi + 2ch]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 20h]
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_4_07CB
        mov    dword ptr [ebx + 84h], 1
        test   ebp, ebp
        je     SHORT loc_4_07DA
loc_4_07CB:
        push   0
        push   ebx
        call   sub_4_2D4C
        add    esp, 8
        mov    eax, ebp
        jmp    SHORT loc_4_07F1
loc_4_07DA:
        lea    eax, [ebx + 4]
        push   ebx
        push   edi
        mov    dword ptr [esi], eax
        call   0
        add    esp, 8
        mov    eax, dword ptr [ebx]
        push   ebx
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_07F1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_4_07F8:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        push   edi
        xor    esi, esi
        mov    dword ptr [ebx], esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0808:
        test   eax, eax
        je     SHORT loc_4_082C
        mov    edi, dword ptr [esp + 10h]
loc_4_0810:
        mov    ecx, dword ptr [eax + 7a7h]
        test   ecx, ecx
        je     SHORT loc_4_081F
        cmp    dword ptr [ecx + 2ch], edi
        je     SHORT loc_4_0835
loc_4_081F:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0825:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_0810
loc_4_082C:
        mov    eax, 0ah
loc_4_0831:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_4_0835:
        mov    dword ptr [ebx], eax
        xor    eax, eax
        jmp    SHORT loc_4_0831
sub_4_083B:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 1ch
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0853:
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_08C2
loc_4_0859:
        push   0
        lea    eax, [ebp - 8]
        push   0
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 8], 0
        push   eax
        push   ecx
        push   dword ptr [ebx + 8]
        call   sub_4_2D6C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_08B3
        cmp    dword ptr [ebp - 4], 10h
        jne    SHORT loc_4_08B3
        lea    eax, [ebp - 1ch]
        lea    ecx, [ebp - 10h]
        push   eax
        lea    edx, [ebx + 0ch]
        push   ecx
        lea    esi, [ebp - 10h]
        push   edx
        call   sub_4_08D7
        add    esp, 0ch
        mov    ecx, 8
        mov    edi, dword ptr [ebp + 8]
        repe cmpsb
        jne    SHORT loc_4_08B3
        lea    esi, [ebp - 1ch]
        mov    ecx, 9
        mov    edi, dword ptr [ebp + 0ch]
        repe cmpsb
        je     SHORT loc_4_08CE
loc_4_08B3:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08B9:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_0859
loc_4_08C2:
        mov    eax, 0ah
loc_4_08C7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_08CE:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ebx
        xor    eax, eax
        jmp    SHORT loc_4_08C7
sub_4_08D7:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
loc_4_08DE:
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_4_08DE
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [edx], 5ch
        je     SHORT loc_4_08FE
loc_4_08EE:
        mov    cl, byte ptr [edx]
        cmp    cl, 5fh
        je     SHORT loc_4_08FE
        inc    edx
        mov    byte ptr [eax], cl
        inc    eax
        cmp    byte ptr [edx], 5ch
        jne    SHORT loc_4_08EE
loc_4_08FE:
        mov    byte ptr [eax], 0
loc_4_0901:
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_4_0901
        mov    eax, dword ptr [ebp + 10h]
        cmp    byte ptr [edx], 0
        je     SHORT loc_4_091C
loc_4_0911:
        mov    cl, byte ptr [edx]
        inc    edx
        mov    byte ptr [eax], cl
        inc    eax
        cmp    byte ptr [edx], 0
        jne    SHORT loc_4_0911
loc_4_091C:
        pop    ebp
        mov    byte ptr [eax], 0
        ret
sub_4_0921:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx], 7
        mov    edx, dword ptr [eax + 28h]
        test   byte ptr [edx + 0d4h], 20h
        je     SHORT loc_4_0941
        mov    dword ptr [ecx], 0fh
loc_4_0941:
        xor    eax, eax
        ret
sub_4_0944:
        sub    esp, 34h
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 40h]
        push   edi
        push   ebp
        mov    eax, dword ptr [ebx + 20h]
        mov    ebp, dword ptr [eax + 28h]
        test   byte ptr [ebp + 0f0h], 2
        je     SHORT loc_4_0971
        cmp    word ptr [ebp + 120h], 1
        jne    SHORT loc_4_0971
        mov    dword ptr [ebx + 3ch], 0
        jmp    SHORT loc_4_098B
loc_4_0971:
        test   byte ptr [ebp + 1dch], 2
        je     SHORT loc_4_098B
        cmp    word ptr [ebp + 20ch], 1
        jne    SHORT loc_4_098B
        mov    dword ptr [ebx + 3ch], 1
loc_4_098B:
        call   sub_4_2D74
        mov    dword ptr [esp + 10h], eax
        push   ebp
        mov    dword ptr [ebx + 40h], eax
        call   sub_4_0EE3
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_4_0A4F
        lea    edi, [esp + 30h]
        xor    eax, eax
        stosd
        stosd
        stosd
        stosw
        lea    esi, [ebp + 2c4h]
        lea    eax, [esp + 20h]
        push   eax
        push   esi
        push   8
        push   dword ptr [esp + 1ch]
        call   sub_4_1826
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_0A4F
        mov    dword ptr [ebx + 4ch], 1
        mov    dword ptr [esp + 16h], 30h
        mov    word ptr [esp + 14h], 0
        lea    edi, [esp + 14h]
        push   edi
        push   esi
        push   80h
        push   dword ptr [esp + 1ch]
        call   sub_4_1826
        mov    eax, dword ptr [esp + 2ah]
        add    esp, 10h
        mov    dword ptr [ebx + 50h], eax
        push   edi
        mov    dword ptr [esp + 1ah], 0ch
        push   esi
        push   80h
        push   dword ptr [esp + 1ch]
        call   sub_4_1826
        mov    dword ptr [esp + 26h], 30h
        add    esp, 10h
        push   edi
        push   esi
        push   100h
        push   dword ptr [esp + 1ch]
        call   sub_4_1826
        lea    eax, [esp + 30h]
        add    esp, 10h
        push   eax
        push   esi
        push   10h
        push   dword ptr [esp + 1ch]
        call   sub_4_1826
        add    esp, 10h
loc_4_0A4F:
        lea    esi, [ebp + 6cbh]
        lea    edi, [ebx + 6ch]
        mov    eax, 6
        mov    ecx, eax
        rep movsd
        lea    esi, [ebp + 6e3h]
        lea    edi, [ebx + 54h]
        mov    ecx, eax
        rep movsd
        cmp    dword ptr [ebx + 3ch], 1
        lea    eax, [ebx + 48h]
        jne    SHORT loc_4_0A7F
        mov    dword ptr [eax], 1
        jmp    SHORT loc_4_0A85
loc_4_0A7F:
        mov    dword ptr [eax], 0
loc_4_0A85:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_0A95
        mov    eax, dword ptr [ebx + 68h]
        mov    ecx, dword ptr [ebx + 5ch]
        mov    dword ptr [ebx + 34h], eax
        jmp    SHORT loc_4_0AA1
loc_4_0A95:
        mov    eax, dword ptr [ebx + 80h]
        mov    ecx, dword ptr [ebx + 74h]
        mov    dword ptr [ebx + 34h], eax
loc_4_0AA1:
        xor    eax, eax
        pop    ebp
        pop    edi
        mov    dword ptr [ebx + 38h], ecx
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
sub_4_0AAE:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 20h]
        mov    esi, dword ptr [eax + 28h]
        push   esi
        call   sub_4_0EE3
        add    esp, 4
        test   eax, eax
        je     SHORT loc_4_0B2D
        cmp    dword ptr [edi + 4ch], 0
        je     SHORT loc_4_0B2D
        lea    ebx, [ebp - 1ch]
        add    esi, 2c4h
        mov    dword ptr [edi + 4ch], 0
        push   ebx
        push   esi
        push   8
        push   dword ptr [edi + 40h]
        call   sub_4_1826
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_0B2D
        mov    eax, dword ptr [edi + 50h]
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    dword ptr [ebp - 6], eax
        mov    dword ptr [ebp - 0ah], 30h
        mov    word ptr [ebp - 0ch], 0
        push   esi
        push   100h
        push   dword ptr [edi + 40h]
        call   sub_4_1826
        add    esp, 10h
        push   ebx
        push   esi
        push   10h
        push   dword ptr [edi + 40h]
        call   sub_4_1826
        add    esp, 10h
loc_4_0B2D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_0B34:
        push   esi
        mov    eax, 0ech
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    edx, dword ptr [esi + 20h]
        mov    ecx, dword ptr [edx + 28h]
        mov    edx, dword ptr [esi + 3ch]
        imul   edx, eax
        add    ecx, edx
        add    ecx, eax
        push   ecx
        push   0ah
        push   dword ptr [esi + 40h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0B5D:
        add    esp, 10h
        cmp    eax, 1
        mov    eax, 0
        pop    esi
        adc    eax, -1
        and    eax, 80004005h
        ret
sub_4_0B72:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    eax, dword ptr [edx + 20h]
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 28h]
        mov    eax, 2
        test   dword ptr [ebx + 0f0h], eax
        je     SHORT loc_4_0BA0
        cmp    word ptr [ebx + 120h], 1
        jne    SHORT loc_4_0BA0
        xor    eax, eax
        jmp    SHORT loc_4_0BB8
loc_4_0BA0:
        test   byte ptr [ebx + 1dch], 2
        je     SHORT loc_4_0BB8
        cmp    word ptr [ebx + 20ch], 1
        jne    SHORT loc_4_0BB8
        mov    eax, 1
loc_4_0BB8:
        cmp    eax, 1
        sbb    ecx, ecx
        neg    ecx
        imul   ecx, ecx, 0ech
        lea    esi, [ecx + ebx]
        mov    ecx, dword ptr [ecx + ebx + 0f0h]
        test   cl, 1
        mov    esi, dword ptr [esi + 11ch]
        je     SHORT loc_4_0C01
        test   cl, 2
        je     SHORT loc_4_0C01
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 4], esi
        je     SHORT loc_4_0BEE
        mov    eax, 8878000ah
        jmp    SHORT loc_4_0C55
loc_4_0BEE:
        mov    ecx, dword ptr [ebp + 0ch]
        movzx  esi, word ptr [ecx + 0eh]
        cmp    esi, dword ptr [edx + 2ch]
        je     SHORT loc_4_0C01
        mov    eax, 8878000ah
        jmp    SHORT loc_4_0C55
loc_4_0C01:
        cmp    dword ptr [edx + 3ch], eax
        je     SHORT loc_4_0C39
        lea    edi, [ebp - 18h]
        lea    esi, [ebx + 6cbh]
        mov    ecx, 6
        rep movsd
        lea    edi, [ebx + 6cbh]
        lea    esi, [ebx + 6e3h]
        mov    ecx, 6
        rep movsd
        lea    esi, [ebp - 18h]
        lea    edi, [ebx + 6e3h]
        mov    ecx, 6
        rep movsd
loc_4_0C39:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 3ch], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 28h], eax
        movzx  ebx, word ptr [ecx + 0eh]
        mov    dword ptr [edx + 2ch], ebx
        movzx  eax, word ptr [ecx + 2]
        mov    dword ptr [edx + 30h], eax
        xor    eax, eax
loc_4_0C55:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_0C5C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    eax, dword ptr [ecx + 20h]
        mov    edx, 0ech
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 28h]
        mov    esi, dword ptr [ecx + 3ch]
        xor    edi, edi
        imul   esi, edx
        or     byte ptr [ebx + esi + 0f0h], 4
        mov    ax, word ptr [ecx + 30h]
        mov    esi, dword ptr [ecx + 2ch]
        mov    word ptr [ebp - 14h], si
        mov    ebx, dword ptr [ecx + 28h]
        mov    word ptr [ebp - 12h], ax
        mov    dword ptr [ebp - 18h], edi
        shr    esi, 3
        imul   si, ax
        mov    word ptr [ebp - 10h], bx
        mov    eax, dword ptr [ecx + 38h]
        mov    word ptr [ebp - 0eh], si
        mov    dword ptr [ebp - 6], eax
        mov    word ptr [ebp - 0ch], di
        mov    esi, eax
        mov    word ptr [ebp - 2], di
        lea    eax, [ebp - 18h]
        shr    esi, 1
        push   eax
        mov    ebx, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 0ah], esi
        mov    esi, dword ptr [ecx + 3ch]
        mov    eax, dword ptr [ebx + 28h]
        imul   esi, edx
        add    eax, esi
        add    eax, edx
        push   eax
        push   5
        push   dword ptr [ecx + 40h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CDC:
        add    esp, 10h
        cmp    eax, 8
        jne    SHORT loc_4_0CEB
        mov    eax, 8878000ah
        jmp    SHORT loc_4_0CFB
loc_4_0CEB:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 80004005h
loc_4_0CFB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_0D02:
        sub    esp, 4
        lea    eax, [esp]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   eax
        mov    edi, 0ech
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [esp + 0ch], 3
        mov    eax, dword ptr [ecx + 28h]
        mov    ecx, dword ptr [esi + 3ch]
        imul   ecx, edi
        add    eax, ecx
        add    eax, edi
        push   eax
        push   7
        push   dword ptr [esi + 40h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D38:
        add    esp, 10h
        mov    edx, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 3ch]
        mov    ecx, dword ptr [edx + 28h]
        imul   eax, edi
        pop    edi
        pop    esi
        and    byte ptr [ecx + eax + 0f0h], 0fbh
        xor    eax, eax
        add    esp, 4
        ret
sub_4_0D57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [esi + 3ch]
        mov    edx, dword ptr [eax + 28h]
        imul   ecx, ecx, 0ech
        lea    eax, [edx + ecx + 0ech]
        lea    edx, [ebp - 4]
        push   edx
        push   eax
        push   9
        push   dword ptr [esi + 40h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D86:
        add    esp, 10h
        test   eax, eax
        mov    eax, 80004005h
        jne    SHORT loc_4_0D9F
        mov    ecx, dword ptr [esi + 38h]
        mov    eax, dword ptr [ebp + 0ch]
        sub    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_4_0D9F:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_0DA4:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        jne    SHORT loc_4_0DCC
        push   19h
        push   0eh
        call   sub_4_2DA4
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0DCC
        xor    eax, eax
        jmp    SHORT loc_4_0E1D
loc_4_0DCC:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_4_2D90
        mov    esi, eax
        mov    eax, 0
        test   esi, esi
        je     SHORT loc_4_0E1D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   8
        mov    dword ptr [esi], eax
        lea    eax, [esi + 8]
        mov    dword ptr [esi + 4], ecx
        push   dword ptr [ebp + 10h]
        push   eax
        call   sub_4_2D54
        add    esp, 0ch
        lea    edx, [esi + 10h]
        push   9
        push   dword ptr [ebp + 14h]
        push   edx
        call   sub_4_2D54
        add    esp, 0ch
        push   esi
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_2D80
        mov    eax, esi
loc_4_0E1D:
        pop    esi
        pop    ebp
        ret
sub_4_0E20:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   edi
        test   eax, eax
        je     SHORT loc_4_0E65
        push   eax
        call   sub_4_2DC8
        test   eax, eax
        je     SHORT loc_4_0E65
loc_4_0E35:
        lea    esi, [eax + 8]
        mov    ecx, 8
        mov    edi, dword ptr [esp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_0E55
        lea    esi, [eax + 10h]
        mov    ecx, 9
        mov    edi, dword ptr [esp + 10h]
        repe cmpsb
        je     SHORT loc_4_0E67
loc_4_0E55:
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_2DB8
        test   eax, eax
        jne    SHORT loc_4_0E35
loc_4_0E65:
        xor    eax, eax
loc_4_0E67:
        pop    edi
        pop    esi
        ret
sub_4_0E6A:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        je     SHORT loc_4_0E9C
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_4_2DC8
        test   eax, eax
        je     SHORT loc_4_0E9C
        mov    esi, dword ptr [esp + 8]
loc_4_0E87:
        cmp    dword ptr [eax + 4], esi
        je     SHORT loc_4_0E9E
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_2DB8
        test   eax, eax
        jne    SHORT loc_4_0E87
loc_4_0E9C:
        xor    eax, eax
loc_4_0E9E:
        pop    esi
        ret
sub_4_0EA0:
        push   dword ptr [esp + 4]
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_2DF8
        push   dword ptr [esp + 4]
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_2DE8
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_2DC8
        test   eax, eax
        jne    SHORT loc_4_0EE2
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_4_2DD8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0EE2:
        ret
sub_4_0EE3:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4ch
        mov    ecx, dword ptr [eax + 7a7h]
        mov    eax, dword ptr [ecx + 2ch]
        test   eax, eax
        je     SHORT loc_4_0F16
        lea    ecx, [esp]
        push   ecx
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_4_0F16
        xor    eax, eax
        cmp    eax, dword ptr [esp + 40h]
        mov    eax, 1
        adc    eax, -1
        jmp    SHORT loc_4_0F18
loc_4_0F16:
        xor    eax, eax
loc_4_0F18:
        add    esp, 4ch
        ret
sub_4_0F1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   ebx
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F38:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_4_105E
        push   201h
        push   34h
        call   sub_4_2D64
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_4_105E
        lea    edi, [esi + 4]
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 0ch]
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 24h]
        push   ecx
        mov    dword ptr [esi + 8], esi
        mov    dword ptr [esi + 0ch], esi
        mov    dword ptr [esi + 20h], ebx
        mov    dword ptr [esi + 14h], 8c4233c0h
        call   sub_4_2E20
        test   eax, eax
        je     SHORT loc_4_0FA3
        push   0
        push   esi
        call   sub_4_2D4C
        add    esp, 8
        jmp    NEAR PTR loc_4_105E
loc_4_0FA3:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 1ch], edi
        mov    dword ptr [ebp - 20h], 0
        mov    dword ptr [esi + 1ch], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        db  0CDh, 020h, 001h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_4_0FEE
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_2E0C
        push   0
        push   esi
        call   sub_4_2D4C
        add    esp, 8
        jmp    SHORT loc_4_105E
loc_4_0FEE:
        mov    eax, dword ptr [ebp - 8]
        xor    edi, edi
        push   edi
        lea    ecx, [ebp - 14h]
        push   edi
        mov    dword ptr [eax + 7a7h], esi
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        mov    dword ptr [esi + 28h], eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], edi
        push   dword ptr [edx + 8]
        call   sub_4_2D6C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_105E
        cmp    dword ptr [ebp - 10h], 10h
        jne    SHORT loc_4_105E
        lea    eax, [ebp - 38h]
        lea    ecx, [ebp - 2ch]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        call   sub_4_08D7
        add    esp, 0ch
        lea    ecx, [ebp - 38h]
        lea    edx, [ebp - 2ch]
        push   ecx
        push   edx
        call   sub_4_0E20
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_105E
        mov    ecx, dword ptr [eax + 4]
        push   eax
        mov    dword ptr [esi + 2ch], ecx
        call   sub_4_0EA0
        add    esp, 4
loc_4_105E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1065:
        push   ebp
        mov    ebp, esp
        sub    esp, 324h
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1083:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_4_116B
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 7a7h]
        xor    eax, eax
        test   esi, esi
        je     NEAR PTR loc_4_116B
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [esi + 4]
        mov    dword ptr [ebp - 14h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        db  0CDh, 020h, 002h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [esi + 1ch]
        xor    edi, edi
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_2E0C
        mov    eax, dword ptr [esi + 2ch]
        test   eax, eax
        je     SHORT loc_4_1157
        lea    ecx, [ebp - 324h]
        mov    ebx, dword ptr [eax]
        push   ecx
        push   eax
        call   dword ptr [ebx + 0ch]
        test   eax, eax
        jne    SHORT loc_4_1157
        mov    ebx, dword ptr [ebp - 120h]
        push   edi
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    dword ptr [ebp - 10h], edi
        push   ebx
        call   sub_4_2D6C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_1157
        cmp    dword ptr [ebp - 0ch], 10h
        jne    SHORT loc_4_1157
        push   edi
        lea    eax, [ebp - 0f8h]
        push   0c8h
        push   eax
        push   ebx
        call   sub_4_2E44
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_1157
        lea    eax, [ebp - 30h]
        lea    ecx, [ebp - 24h]
        push   eax
        lea    edx, [ebp - 0f8h]
        push   ecx
        push   edx
        call   sub_4_08D7
        add    esp, 0ch
        lea    ecx, [ebp - 30h]
        lea    edx, [ebp - 24h]
        push   ecx
        push   edx
        push   dword ptr [esi + 2ch]
        push   ebx
        call   sub_4_0DA4
        add    esp, 10h
loc_4_1157:
        xor    edi, edi
        push   esi
        mov    dword ptr [esi + 1ch], edi
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 7a7h], edi
loc_4_116B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
VSB16_Service_01:
        push   ebp
        mov    ebp, esp
        sub    esp, 31ch
        push   esi
        lea    eax, [ebp - 31ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_4_1280
        mov    edi, dword ptr [ebp - 118h]
        xor    eax, eax
        push   eax
        lea    ecx, [ebp - 10h]
        push   eax
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ebp - 14h]
        push   eax
        push   ecx
        push   edi
        call   sub_4_2D6C
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_4_11C1
        mov    eax, 88780078h
        jmp    NEAR PTR loc_4_1280
loc_4_11C1:
        cmp    dword ptr [ebp - 10h], 10h
        push   0
        jne    SHORT loc_4_122F
        push   0c8h
        lea    eax, [ebp - 0f0h]
        push   eax
        push   edi
        call   sub_4_2E44
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_4_11EC
        mov    eax, 88780078h
        jmp    NEAR PTR loc_4_1280
loc_4_11EC:
        lea    eax, [ebp - 28h]
        lea    ecx, [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 0f0h]
        push   ecx
        push   edx
        call   sub_4_08D7
        add    esp, 0ch
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 28h]
        lea    eax, [ebp - 1ch]
        push   ecx
        push   edx
        push   eax
        call   sub_4_083B
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_1267
        lea    eax, [ebp - 28h]
        lea    ecx, [ebp - 1ch]
        push   eax
        push   ecx
        push   esi
        push   edi
        call   sub_4_0DA4
        add    esp, 10h
        jmp    SHORT loc_4_127E
loc_4_122F:
        push   edi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_4_2E4C
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_1247
        mov    eax, 88780078h
        jmp    SHORT loc_4_1280
loc_4_1247:
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [ebp - 0ch]
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1259:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_4_1267
        mov    eax, 88780078h
        jmp    SHORT loc_4_1280
loc_4_1267:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 7a7h]
        test   eax, eax
        jne    SHORT loc_4_127B
        mov    eax, 88780078h
        jmp    SHORT loc_4_1280
loc_4_127B:
        mov    dword ptr [eax + 2ch], esi
loc_4_127E:
        xor    eax, eax
loc_4_1280:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
VSB16_Service_02:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_07F8
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_12C2
        push   dword ptr [ebp + 8]
        call   sub_4_0E6A
        add    esp, 4
        test   eax, eax
        je     SHORT loc_4_12BB
        push   eax
        call   sub_4_0EA0
        add    esp, 4
        jmp    SHORT loc_4_12F2
loc_4_12BB:
        mov    eax, 88780078h
        jmp    SHORT loc_4_12F4
loc_4_12C2:
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   edi
        mov    esi, dword ptr [eax + 7a7h]
        push   esi
        mov    dword ptr [esi + 2ch], edi
        call   sub_4_0060
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_12F2
loc_4_12DE:
        push   eax
        mov    dword ptr [eax + 88h], edi
        push   esi
        call   sub_4_0060
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_12DE
loc_4_12F2:
        xor    eax, eax
loc_4_12F4:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
sub_4_12FC:
        db  00Fh, 0B7h, 005h, 016h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_4_130A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        test   al, 1
        je     SHORT loc_4_1376
        test   al, 2
        je     SHORT loc_4_1323
        mov    eax, 4
        jmp    SHORT loc_4_137B
loc_4_1323:
        or     al, 2
        mov    ecx, dword ptr [ebp + 0ch]
        push   2
        mov    dword ptr [esi], eax
        push   dword ptr [esi + 24h]
        mov    dword ptr [esi + 4], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1337:
        add    esp, 8
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1340:
        add    esp, 4
        mov    edi, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_4_136B
        test   edi, edi
        je     SHORT loc_4_1372
        push   0ffh
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_135B:
        add    esp, 8
        push   3fh
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1368:
        add    esp, 8
loc_4_136B:
        test   edi, edi
        je     SHORT loc_4_1372
        mov    dword ptr [edi + 2], esi
loc_4_1372:
        xor    eax, eax
        jmp    SHORT loc_4_137B
loc_4_1376:
        mov    eax, 3
loc_4_137B:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_137F:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        test   dl, 1
        je     SHORT loc_4_13B1
        test   dl, 2
        je     SHORT loc_4_13B1
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_4_139F
        mov    eax, 1
        jmp    SHORT loc_4_13B3
loc_4_139F:
        and    dl, 0fdh
        push   3
        push   dword ptr [eax + 24h]
        mov    dword ptr [eax], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13AE:
        add    esp, 8
loc_4_13B1:
        xor    eax, eax
loc_4_13B3:
        ret
sub_4_13B4:
        push   ebp
        movzx  eax, word ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        je     SHORT loc_4_13ED
        cmp    eax, 2
        je     SHORT loc_4_1402
        cmp    eax, 3
        je     SHORT loc_4_1413
        cmp    eax, 4
        je     SHORT loc_4_1424
        cmp    eax, 5
        je     SHORT loc_4_1432
        cmp    eax, 6
        je     SHORT loc_4_145B
        mov    eax, 0fh
        jmp    NEAR PTR loc_4_149D
loc_4_13ED:
        push   dword ptr [ebp + 14h]
        push   4
        push   dword ptr [esi + 24h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13FA:
        add    esp, 0ch
        jmp    NEAR PTR loc_4_149D
loc_4_1402:
        push   dword ptr [ebp + 14h]
        push   esi
        call   sub_4_152D
        add    esp, 8
        jmp    NEAR PTR loc_4_149D
loc_4_1413:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_130A
        add    esp, 0ch
        jmp    SHORT loc_4_149D
loc_4_1424:
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_137F
        add    esp, 8
        jmp    SHORT loc_4_149D
loc_4_1432:
        mov    edi, dword ptr [ebp + 14h]
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [edi]
        test   bl, bl
        je     SHORT loc_4_149B
loc_4_143F:
        movzx  eax, byte ptr [ebp - 1]
        mov    al, byte ptr [eax + edi + 1]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_144E:
        add    esp, 8
        inc    byte ptr [ebp - 1]
        cmp    bl, byte ptr [ebp - 1]
        ja     SHORT loc_4_143F
        jmp    SHORT loc_4_149B
loc_4_145B:
        mov    edi, dword ptr [ebp + 14h]
        lea    ecx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [edi + 4]
        push   eax
        shr    eax, 10h
        push   eax
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1472:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_4_1480
        mov    eax, 1
        jmp    SHORT loc_4_149D
loc_4_1480:
        xor    ebx, ebx
        cmp    dword ptr [edi], ebx
        jbe    SHORT loc_4_149B
loc_4_1486:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ebx]
        inc    ebx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1494:
        add    esp, 8
        cmp    dword ptr [edi], ebx
        ja     SHORT loc_4_1486
loc_4_149B:
        xor    eax, eax
loc_4_149D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_14A4:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_4_14B1:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 2ah], 2
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 24h], eax
        mov    word ptr [esi + 2ch], 0c9h
        xor    eax, eax
        lea    edx, [ebp - 2]
        mov    word ptr [esi + 50h], ax
        mov    dword ptr [esi + 56h], eax
        mov    word ptr [esi + 52h], ax
        push   edx
        mov    word ptr [esi + 4eh], 1
        mov    word ptr [esi + 54h], 0ffffh
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14F7:
        add    esp, 8
        lea    eax, [esi + 2eh]
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1513:
        add    esp, 10h
        xor    eax, eax
        or     byte ptr [esi], 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1520:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_4_152D:
        push   esi
        mov    ecx, 0ch
        mov    esi, dword ptr [esp + 8]
        push   edi
        add    esi, 28h
        mov    edi, dword ptr [esp + 10h]
        rep movsd
        movsw
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_4_1548:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        test   al, 1
        je     SHORT loc_4_15D5
        test   al, 2
        je     SHORT loc_4_1561
        mov    eax, 4
        jmp    SHORT loc_4_15DA
loc_4_1561:
        or     al, 2
        mov    ecx, dword ptr [ebp + 0ch]
        push   2
        mov    dword ptr [esi], eax
        push   dword ptr [esi + 54h]
        mov    dword ptr [esi + 4], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1575:
        add    esp, 8
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_157E:
        add    esp, 4
        mov    edi, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_4_15A9
        test   edi, edi
        je     SHORT loc_4_15D1
        push   0ffh
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1599:
        add    esp, 8
        push   3fh
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_15A6:
        add    esp, 8
loc_4_15A9:
        test   edi, edi
        je     SHORT loc_4_15D1
        mov    eax, dword ptr [edi + 0ah]
        mov    dword ptr [esi + 0ch], eax
        mov    ecx, dword ptr [edi + 6]
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [edi + 2], esi
        mov    dx, word ptr [esi + 3ah]
        mov    dword ptr [edi + 12h], 80h
        mov    word ptr [edi + 10h], 0
        mov    word ptr [edi + 0eh], dx
loc_4_15D1:
        xor    eax, eax
        jmp    SHORT loc_4_15DA
loc_4_15D5:
        mov    eax, 3
loc_4_15DA:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_15DE:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        test   dl, 1
        je     SHORT loc_4_1610
        test   dl, 2
        je     SHORT loc_4_1610
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_4_15FE
        mov    eax, 1
        jmp    SHORT loc_4_1612
loc_4_15FE:
        and    dl, 0fdh
        push   3
        push   dword ptr [eax + 54h]
        mov    dword ptr [eax], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_160D:
        add    esp, 8
loc_4_1610:
        xor    eax, eax
loc_4_1612:
        ret
sub_4_1613:
        push   ebp
        movzx  eax, word ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    edi, edi
        cmp    eax, 1
        je     SHORT loc_4_1651
        cmp    eax, 2
        je     SHORT loc_4_1668
        cmp    eax, 3
        je     SHORT loc_4_167B
        cmp    eax, 4
        je     SHORT loc_4_168E
        cmp    eax, 5
        je     SHORT loc_4_169E
        cmp    eax, 6
        je     NEAR PTR loc_4_16D4
        mov    edi, 0fh
        jmp    NEAR PTR loc_4_1702
loc_4_1651:
        push   dword ptr [ebp + 14h]
        push   2
        push   dword ptr [esi + 54h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_165E:
        add    esp, 0ch
        mov    edi, eax
        jmp    NEAR PTR loc_4_1702
loc_4_1668:
        push   dword ptr [ebp + 14h]
        push   esi
        call   sub_4_17BC
        add    esp, 8
        mov    edi, eax
        jmp    NEAR PTR loc_4_1702
loc_4_167B:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_1548
        add    esp, 0ch
        mov    edi, eax
        jmp    SHORT loc_4_1702
loc_4_168E:
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_15DE
        add    esp, 8
        mov    edi, eax
        jmp    SHORT loc_4_1702
loc_4_169E:
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16A7:
        add    esp, 4
        push   esi
        call   sub_4_17D7
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 0dch], eax
        push   1
        mov    word ptr [esi + 0d8h], ax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16CA:
        add    esp, 8
        mov    edi, eax
        push   dword ptr [ebp - 4]
        jmp    SHORT loc_4_16FA
loc_4_16D4:
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16DD:
        add    esp, 4
        mov    eax, dword ptr [esi]
        test   al, 8
        je     SHORT loc_4_16EC
        or     al, 10h
        mov    dword ptr [esi], eax
        jmp    SHORT loc_4_16F7
loc_4_16EC:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16F4:
        add    esp, 8
loc_4_16F7:
        push   dword ptr [ebp - 4]
loc_4_16FA:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16FF:
        add    esp, 4
loc_4_1702:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_170A:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_4_1717:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 2]
        mov    word ptr [esi + 16h], 2
        push   edx
        mov    dword ptr [esi + 8], ecx
        push   ecx
        mov    dword ptr [esi + 54h], eax
        mov    word ptr [esi + 18h], 0cah
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1744:
        add    esp, 8
        lea    eax, [esi + 1ah]
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1760:
        add    esp, 10h
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 8]
        lea    eax, [esi + 58h]
        push   ecx
        push   edx
        push   80h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1779:
        add    esp, 10h
        lea    ecx, [esi + 3ah]
        push   ecx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_178B:
        add    esp, 0ch
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_4_17AA
        xor    eax, eax
        or     byte ptr [esi], 1
        mov    word ptr [esi + 0d8h], ax
        mov    word ptr [esi + 0dah], ax
loc_4_17AA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_17AF:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_4_17BC:
        push   esi
        mov    ecx, 9
        mov    esi, dword ptr [esp + 8]
        push   edi
        add    esi, 14h
        mov    edi, dword ptr [esp + 10h]
        rep movsd
        movsw
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_4_17D7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        xor    di, di
        mov    esi, dword ptr [ebp + 8]
loc_4_17E5:
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [esi + 8]
        mov    dx, word ptr [ecx + 20h]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17F6:
        add    esp, 8
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_4_1816
        xor    di, di
        lea    eax, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [esi + 8]
        mov    dx, word ptr [ecx + 1eh]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1813:
        add    esp, 8
loc_4_1816:
        inc    di
        cmp    di, 3ch
        jb     SHORT loc_4_17E5
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1826:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        movzx  eax, word ptr [esp + 0ch]
        mov    ebp, esp
        cmp    eax, 8
        jg     SHORT loc_4_184A
        je     SHORT loc_4_1893
        cmp    eax, 1
        je     SHORT loc_4_186E
        cmp    eax, 2
        je     SHORT loc_4_1880
        cmp    eax, 4
        je     SHORT loc_4_188C
        jmp    SHORT loc_4_1867
loc_4_184A:
        cmp    eax, 10h
        je     SHORT loc_4_18A4
        cmp    eax, 20h
        je     SHORT loc_4_18AF
        cmp    eax, 40h
        je     SHORT loc_4_18BA
        cmp    eax, 80h
        je     SHORT loc_4_18C1
        cmp    eax, 100h
        je     SHORT loc_4_18CC
loc_4_1867:
        mov    eax, 0fh
        jmp    SHORT loc_4_18D5
loc_4_186E:
        push   dword ptr [ebp + 14h]
        push   8
        push   dword ptr [ecx + 24h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_187B:
        add    esp, 0ch
        jmp    SHORT loc_4_18D5
loc_4_1880:
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax], 7
        xor    eax, eax
        jmp    SHORT loc_4_18D5
loc_4_188C:
        mov    eax, 0ch
        jmp    SHORT loc_4_18D5
loc_4_1893:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_4_18D9
        add    esp, 0ch
        jmp    SHORT loc_4_18D5
loc_4_18A4:
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_4_191D
        jmp    SHORT loc_4_18D5
loc_4_18AF:
        push   dword ptr [ebp + 14h]
        push   ecx
        call   sub_4_2CEF
        jmp    SHORT loc_4_18D5
loc_4_18BA:
        mov    eax, 0ch
        jmp    SHORT loc_4_18D5
loc_4_18C1:
        push   dword ptr [ebp + 14h]
        push   ecx
        call   sub_4_1AF2
        jmp    SHORT loc_4_18D5
loc_4_18CC:
        push   dword ptr [ebp + 14h]
        push   ecx
        call   sub_4_2368
loc_4_18D5:
        mov    esp, ebp
        pop    ebp
        ret
sub_4_18D9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        test   al, 1
        je     SHORT loc_4_1916
        test   al, 2
        je     SHORT loc_4_18EF
        mov    eax, 4
        jmp    SHORT loc_4_191B
loc_4_18EF:
        or     al, 2
        mov    ecx, dword ptr [esp + 0ch]
        push   2
        mov    dword ptr [esi], eax
        push   dword ptr [esi + 24h]
        mov    dword ptr [esi + 4], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1904:
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_1912
        mov    dword ptr [eax + 2], esi
loc_4_1912:
        xor    eax, eax
        jmp    SHORT loc_4_191B
loc_4_1916:
        mov    eax, 3
loc_4_191B:
        pop    esi
        ret
sub_4_191D:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_4_1931
        mov    eax, 4
        jmp    SHORT loc_4_1943
loc_4_1931:
        push   3
        and    byte ptr [eax], 0fdh
        push   dword ptr [eax + 24h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_193E:
        add    esp, 8
        xor    eax, eax
loc_4_1943:
        ret
sub_4_1944:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_4_1951:
        mov    ecx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 24h], eax
        or     byte ptr [esi], 1
        mov    cx, word ptr [ecx + 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1971:
        and    ax, 0ffh
        add    esp, 4
        mov    word ptr [esi + 28h], ax
        xor    eax, eax
        pop    esi
        ret
sub_4_1980:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_4_198D:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 10h]
        test   byte ptr [esi + 2], 3
        je     SHORT loc_4_19AA
        push   esi
        push   dword ptr [esp + 10h]
        call   sub_4_19C4
        add    esp, 8
        mov    edi, eax
loc_4_19AA:
        test   byte ptr [esi + 2], 0ch
        je     SHORT loc_4_19BF
        push   esi
        push   dword ptr [esp + 10h]
        call   sub_4_1A16
        add    esp, 8
        or     edi, eax
loc_4_19BF:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_4_19C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   47h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19DE:
        add    esp, 0ch
        mov    ebx, eax
        mov    edi, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 1]
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 18h
        push   ecx
        push   46h
        mov    dword ptr [edi + 6], eax
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19FE:
        add    esp, 0ch
        or     ebx, eax
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1A16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   45h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A30:
        add    esp, 0ch
        mov    ebx, eax
        mov    edi, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 1]
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 18h
        push   ecx
        push   44h
        mov    dword ptr [edi + 6], eax
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A50:
        add    esp, 0ch
        or     ebx, eax
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1A68:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        lea    esi, [eax + 6]
        movzx  eax, word ptr [eax]
        mov    dword ptr [esi], ecx
        cmp    eax, 3
        je     SHORT loc_4_1A88
        cmp    eax, 4
        je     SHORT loc_4_1A8C
        jmp    SHORT loc_4_1A8E
loc_4_1A88:
        mov    cl, 3dh
        jmp    SHORT loc_4_1A8E
loc_4_1A8C:
        mov    cl, 3eh
loc_4_1A8E:
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A9E:
        add    esp, 0ch
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_4_1AB3
        mov    eax, dword ptr [esi]
        or     ah, 8
        mov    dword ptr [esi], eax
        or     ah, 4
        mov    dword ptr [esi], eax
loc_4_1AB3:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_4_1ABC
        or     byte ptr [esi], 40h
loc_4_1ABC:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_4_1AC5
        or     byte ptr [esi], 80h
loc_4_1AC5:
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_4_1ACF
        or     byte ptr [esi + 1], 1
loc_4_1ACF:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_4_1AD9
        or     byte ptr [esi + 1], 2
loc_4_1AD9:
        test   byte ptr [ebp - 1], 20h
        je     SHORT loc_4_1AE2
        or     byte ptr [esi], 10h
loc_4_1AE2:
        test   byte ptr [ebp - 1], 40h
        je     SHORT loc_4_1AEB
        or     byte ptr [esi], 20h
loc_4_1AEB:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1AF2:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        movzx  ecx, word ptr [eax]
        mov    ebp, esp
        test   ecx, ecx
        je     SHORT loc_4_1B25
        cmp    ecx, 1
        je     SHORT loc_4_1B30
        cmp    ecx, 2
        je     SHORT loc_4_1B3B
        cmp    ecx, 3
        jl     SHORT loc_4_1B1E
        cmp    ecx, 4
        jle    SHORT loc_4_1B46
        cmp    ecx, 5
        je     SHORT loc_4_1B51
        cmp    ecx, 6
        je     SHORT loc_4_1B5C
loc_4_1B1E:
        mov    eax, 0fh
        jmp    SHORT loc_4_1B65
loc_4_1B25:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1CBD
        jmp    SHORT loc_4_1B65
loc_4_1B30:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_198D
        jmp    SHORT loc_4_1B65
loc_4_1B3B:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1B69
        jmp    SHORT loc_4_1B65
loc_4_1B46:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1A68
        jmp    SHORT loc_4_1B65
loc_4_1B51:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1BCF
        jmp    SHORT loc_4_1B65
loc_4_1B5C:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1CA2
loc_4_1B65:
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1B69:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 1]
        mov    esi, dword ptr [ebp + 0ch]
        push   ecx
        add    esi, 6
        push   3ch
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi], 0
        push   dword ptr [edx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B8D:
        add    esp, 0ch
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_4_1BA2
        mov    eax, dword ptr [esi]
        or     ah, 8
        mov    dword ptr [esi], eax
        or     ah, 4
        mov    dword ptr [esi], eax
loc_4_1BA2:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_4_1BAB
        or     byte ptr [esi], 40h
loc_4_1BAB:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_4_1BB4
        or     byte ptr [esi], 80h
loc_4_1BB4:
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_4_1BBE
        or     byte ptr [esi + 1], 1
loc_4_1BBE:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_4_1BC8
        or     byte ptr [esi + 1], 2
loc_4_1BC8:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1BCF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        test   byte ptr [esi + 2], 3
        je     SHORT loc_4_1C28
        lea    eax, [ebp - 1]
        lea    ebx, [esi + 6]
        push   eax
        push   40h
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1BF9:
        add    esp, 0ch
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        lea    ecx, [ebp - 1]
        movzx  eax, al
        push   ecx
        mov    dword ptr [ebx], eax
        shl    eax, 10h
        push   3fh
        mov    dword ptr [ebx], eax
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C1A:
        add    esp, 0ch
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        movzx  ecx, al
        or     dword ptr [ebx], ecx
loc_4_1C28:
        test   byte ptr [esi + 2], 0ch
        je     SHORT loc_4_1C6E
        lea    eax, [ebp - 1]
        lea    ebx, [esi + 6]
        push   eax
        push   42h
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C3F:
        add    esp, 0ch
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        lea    ecx, [ebp - 1]
        movzx  eax, al
        push   ecx
        mov    dword ptr [ebx], eax
        shl    eax, 10h
        push   41h
        mov    dword ptr [ebx], eax
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C60:
        add    esp, 0ch
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        movzx  ecx, al
        or     dword ptr [ebx], ecx
loc_4_1C6E:
        test   byte ptr [esi + 2], 10h
        je     SHORT loc_4_1C99
        lea    eax, [ebp - 1]
        push   eax
        push   43h
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C82:
        mov    dword ptr [esi + 6], 0
        add    esp, 0ch
        test   byte ptr [ebp - 1], 1
        jne    SHORT loc_4_1C99
        mov    dword ptr [esi + 6], 1
loc_4_1C99:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1CA2:
        mov    eax, dword ptr [esp + 8]
        lea    ecx, [eax + 6]
        push   ecx
        push   dword ptr [eax + 2]
        mov    eax, dword ptr [esp + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CB9:
        add    esp, 0ch
        ret
sub_4_1CBD:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        lea    ebp, [edi + 2]
        test   byte ptr [ebp], 3
        je     SHORT loc_4_1CE2
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   esi
        call   sub_4_1D59
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_4_1CE6
loc_4_1CE2:
        mov    esi, dword ptr [esp + 14h]
loc_4_1CE6:
        test   byte ptr [ebp], 0ch
        je     SHORT loc_4_1CF8
        push   edi
        push   esi
        call   sub_4_1E3E
        add    esp, 8
        or     ebx, eax
loc_4_1CF8:
        test   byte ptr [ebp], 30h
        je     SHORT loc_4_1D0A
        push   edi
        push   esi
        call   sub_4_1F23
        add    esp, 8
        or     ebx, eax
loc_4_1D0A:
        test   byte ptr [ebp], 0c0h
        je     SHORT loc_4_1D1C
        push   edi
        push   esi
        call   sub_4_2008
        add    esp, 8
        or     ebx, eax
loc_4_1D1C:
        test   byte ptr [ebp + 1], 3
        je     SHORT loc_4_1D2E
        push   edi
        push   esi
        call   sub_4_20ED
        add    esp, 8
        or     ebx, eax
loc_4_1D2E:
        test   byte ptr [ebp + 1], 0ch
        je     SHORT loc_4_1D40
        push   edi
        push   esi
        call   sub_4_21D2
        add    esp, 8
        or     ebx, eax
loc_4_1D40:
        test   byte ptr [ebp + 1], 30h
        je     SHORT loc_4_1D52
        push   edi
        push   esi
        call   sub_4_2254
        add    esp, 8
        or     ebx, eax
loc_4_1D52:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_1D59:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 28h]
        cmp    ax, 4
        je     SHORT loc_4_1DD6
        cmp    ax, 5
        je     SHORT loc_4_1DD6
        cmp    ax, 7
        je     SHORT loc_4_1DD6
        cmp    ax, 8
        je     SHORT loc_4_1DD6
        lea    eax, [ebp - 1]
        push   eax
        push   31h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D8F:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   30h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1DC0:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_1E29
loc_4_1DD6:
        lea    eax, [ebp - 1]
        push   eax
        push   31h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1DE4:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   30h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E15:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
loc_4_1E29:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1E3E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 28h]
        cmp    ax, 4
        je     SHORT loc_4_1EBB
        cmp    ax, 5
        je     SHORT loc_4_1EBB
        cmp    ax, 7
        je     SHORT loc_4_1EBB
        cmp    ax, 8
        je     SHORT loc_4_1EBB
        lea    eax, [ebp - 1]
        push   eax
        push   33h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E74:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   32h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EA5:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_1F0E
loc_4_1EBB:
        lea    eax, [ebp - 1]
        push   eax
        push   33h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EC9:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   32h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EFA:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
loc_4_1F0E:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_1F23:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 28h]
        cmp    ax, 4
        je     SHORT loc_4_1FA0
        cmp    ax, 5
        je     SHORT loc_4_1FA0
        cmp    ax, 7
        je     SHORT loc_4_1FA0
        cmp    ax, 8
        je     SHORT loc_4_1FA0
        lea    eax, [ebp - 1]
        push   eax
        push   35h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F59:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   34h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F8A:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_1FF3
loc_4_1FA0:
        lea    eax, [ebp - 1]
        push   eax
        push   35h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FAE:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   34h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FDF:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
loc_4_1FF3:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_2008:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2085
        cmp    ax, 5
        je     SHORT loc_4_2085
        cmp    ax, 7
        je     SHORT loc_4_2085
        cmp    ax, 8
        je     SHORT loc_4_2085
        lea    eax, [ebp - 1]
        push   eax
        push   37h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_203E:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   36h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_206F:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_20D8
loc_4_2085:
        lea    eax, [ebp - 1]
        push   eax
        push   37h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2093:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   36h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_20C4:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
loc_4_20D8:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_20ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 28h]
        cmp    ax, 4
        je     SHORT loc_4_216A
        cmp    ax, 5
        je     SHORT loc_4_216A
        cmp    ax, 7
        je     SHORT loc_4_216A
        cmp    ax, 8
        je     SHORT loc_4_216A
        lea    eax, [ebp - 1]
        push   eax
        push   39h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2123:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   38h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2154:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_21BD
loc_4_216A:
        lea    eax, [ebp - 1]
        push   eax
        push   39h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2178:
        add    esp, 0ch
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0ch]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        movzx  eax, dl
        shl    eax, 18h
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   38h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_21A9:
        add    esp, 0ch
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        db  08Ah, 091h, 000h, 000h, 000h, 000h
loc_4_21BD:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_21D2:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 28h]
        mov    ebp, esp
        sub    esp, 4
        cmp    ax, 4
        je     SHORT loc_4_221A
        cmp    ax, 5
        je     SHORT loc_4_221A
        cmp    ax, 7
        je     SHORT loc_4_221A
        cmp    ax, 8
        je     SHORT loc_4_221A
        lea    eax, [ebp - 1]
        push   eax
        push   3ah
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2206:
        add    esp, 0ch
        mov    cl, byte ptr [ebp - 1]
        shr    cl, 3
        movzx  edx, cl
        db  08Ah, 08Ah, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_223A
loc_4_221A:
        lea    eax, [ebp - 1]
        push   eax
        push   3ah
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2228:
        add    esp, 0ch
        mov    cl, byte ptr [ebp - 1]
        shr    cl, 5
        movzx  edx, cl
        db  08Ah, 08Ah, 000h, 000h, 000h, 000h
loc_4_223A:
        mov    byte ptr [ebp - 1], cl
        movzx  ecx, cl
        mov    edx, ecx
        shl    edx, 10h
        or     edx, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        shl    edx, 8
        mov    dword ptr [ecx + 6], edx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_2254:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 1]
        push   eax
        push   3bh
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_226C:
        movzx  ecx, byte ptr [ebp - 1]
        add    esp, 0ch
        mov    edx, ecx
        shl    edx, 10h
        or     edx, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        shl    edx, 8
        mov    dword ptr [ecx + 6], edx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_2287:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 0ch]
        lea    ebx, [edi + 2]
        test   byte ptr [ebx], 2
        je     SHORT loc_4_22AA
        push   dword ptr [edi + 6]
        push   dword ptr [ebp + 8]
        call   sub_4_22F8
        add    esp, 8
        mov    esi, eax
loc_4_22AA:
        test   byte ptr [ebx], 1
        je     SHORT loc_4_22C3
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_2314
        add    esp, 8
        or     esi, eax
loc_4_22C3:
        test   byte ptr [ebx], 8
        je     SHORT loc_4_22D8
        push   dword ptr [edi + 6]
        push   dword ptr [ebp + 8]
        call   sub_4_2330
        add    esp, 8
        or     esi, eax
loc_4_22D8:
        test   byte ptr [ebx], 4
        je     SHORT loc_4_22F1
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_234C
        add    esp, 8
        or     esi, eax
loc_4_22F1:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_22F8:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   46h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2310:
        add    esp, 0ch
        ret
sub_4_2314:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   47h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_232C:
        add    esp, 0ch
        ret
sub_4_2330:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   44h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2348:
        add    esp, 0ch
        ret
sub_4_234C:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   45h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2364:
        add    esp, 0ch
        ret
sub_4_2368:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        movzx  ecx, word ptr [eax]
        mov    ebp, esp
        test   ecx, ecx
        je     SHORT loc_4_239B
        cmp    ecx, 1
        je     SHORT loc_4_23A6
        cmp    ecx, 2
        je     SHORT loc_4_23B1
        cmp    ecx, 3
        jl     SHORT loc_4_2394
        cmp    ecx, 4
        jle    SHORT loc_4_23BC
        cmp    ecx, 5
        je     SHORT loc_4_23C7
        cmp    ecx, 6
        je     SHORT loc_4_23D2
loc_4_2394:
        mov    eax, 0fh
        jmp    SHORT loc_4_23DB
loc_4_239B:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_26DA
        jmp    SHORT loc_4_23DB
loc_4_23A6:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_2287
        jmp    SHORT loc_4_23DB
loc_4_23B1:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_23DF
        jmp    SHORT loc_4_23DB
loc_4_23BC:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_24DA
        jmp    SHORT loc_4_23DB
loc_4_23C7:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_261E
        jmp    SHORT loc_4_23DB
loc_4_23D2:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_26C0
loc_4_23DB:
        mov    esp, ebp
        pop    ebp
        ret
sub_4_23DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   3ch
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_23FA:
        add    esp, 0ch
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 2]
        test   cl, 2
        je     SHORT loc_4_240D
        mov    esi, 0eh
loc_4_240D:
        test   cl, 1
        je     SHORT loc_4_2415
        or     esi, 0eh
loc_4_2415:
        test   cl, 8
        je     SHORT loc_4_241D
        or     esi, 0ch
loc_4_241D:
        test   cl, 4
        je     SHORT loc_4_2425
        or     esi, 0ch
loc_4_2425:
        test   cl, 20h
        je     SHORT loc_4_242D
        or     esi, 0ch
loc_4_242D:
        test   cl, 10h
        je     SHORT loc_4_2435
        or     esi, 0ch
loc_4_2435:
        test   ch, 20h
        je     SHORT loc_4_243D
        or     esi, 0ch
loc_4_243D:
        test   ch, 10h
        je     SHORT loc_4_2445
        or     esi, 0ch
loc_4_2445:
        test   ch, 4
        je     SHORT loc_4_244D
        or     esi, 0ch
loc_4_244D:
        mov    eax, 80h
        test   eax, ecx
        je     SHORT loc_4_2465
        test   dword ptr [edx + 6], eax
        je     SHORT loc_4_2461
        or     byte ptr [ebp - 1], 4
        jmp    SHORT loc_4_2465
loc_4_2461:
        and    byte ptr [ebp - 1], 0fbh
loc_4_2465:
        test   cl, 40h
        je     SHORT loc_4_247A
        test   byte ptr [edx + 6], 40h
        je     SHORT loc_4_2476
        or     byte ptr [ebp - 1], 2
        jmp    SHORT loc_4_247A
loc_4_2476:
        and    byte ptr [ebp - 1], 0fdh
loc_4_247A:
        mov    eax, 200h
        test   eax, ecx
        je     SHORT loc_4_2492
        test   dword ptr [edx + 6], eax
        je     SHORT loc_4_248E
        or     byte ptr [ebp - 1], 10h
        jmp    SHORT loc_4_2492
loc_4_248E:
        and    byte ptr [ebp - 1], 0efh
loc_4_2492:
        mov    eax, 100h
        test   eax, ecx
        je     SHORT loc_4_24AA
        test   dword ptr [edx + 6], eax
        je     SHORT loc_4_24A6
        or     byte ptr [ebp - 1], 8
        jmp    SHORT loc_4_24AA
loc_4_24A6:
        and    byte ptr [ebp - 1], 0f7h
loc_4_24AA:
        mov    eax, 800h
        test   eax, ecx
        je     SHORT loc_4_24C2
        test   dword ptr [edx + 6], eax
        je     SHORT loc_4_24BE
        or     byte ptr [ebp - 1], 1
        jmp    SHORT loc_4_24C2
loc_4_24BE:
        and    byte ptr [ebp - 1], 0feh
loc_4_24C2:
        push   dword ptr [ebp - 1]
        push   3ch
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24CF:
        add    esp, 0ch
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_24DA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [edi]
        cmp    eax, 3
        je     SHORT loc_4_24FF
        cmp    eax, 4
        je     SHORT loc_4_2503
        mov    eax, 1
        jmp    NEAR PTR loc_4_2617
loc_4_24FF:
        mov    bl, 3dh
        jmp    SHORT loc_4_2505
loc_4_2503:
        mov    bl, 3eh
loc_4_2505:
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ebx
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2515:
        add    esp, 0ch
        mov    ecx, dword ptr [edi + 2]
        test   cl, 10h
        je     SHORT loc_4_2530
        test   byte ptr [edi + 6], 10h
        je     SHORT loc_4_252C
        or     byte ptr [ebp - 1], 20h
        jmp    SHORT loc_4_2530
loc_4_252C:
        and    byte ptr [ebp - 1], 0dfh
loc_4_2530:
        test   cl, 20h
        je     SHORT loc_4_2545
        test   byte ptr [edi + 6], 20h
        je     SHORT loc_4_2541
        or     byte ptr [ebp - 1], 40h
        jmp    SHORT loc_4_2545
loc_4_2541:
        and    byte ptr [ebp - 1], 0bfh
loc_4_2545:
        mov    eax, 80h
        test   eax, ecx
        je     SHORT loc_4_255D
        test   dword ptr [edi + 6], eax
        je     SHORT loc_4_2559
        or     byte ptr [ebp - 1], 4
        jmp    SHORT loc_4_255D
loc_4_2559:
        and    byte ptr [ebp - 1], 0fbh
loc_4_255D:
        test   cl, 40h
        je     SHORT loc_4_2572
        test   byte ptr [edi + 6], 40h
        je     SHORT loc_4_256E
        or     byte ptr [ebp - 1], 2
        jmp    SHORT loc_4_2572
loc_4_256E:
        and    byte ptr [ebp - 1], 0fdh
loc_4_2572:
        mov    eax, 200h
        test   eax, ecx
        je     SHORT loc_4_258A
        test   dword ptr [edi + 6], eax
        je     SHORT loc_4_2586
        or     byte ptr [ebp - 1], 10h
        jmp    SHORT loc_4_258A
loc_4_2586:
        and    byte ptr [ebp - 1], 0efh
loc_4_258A:
        mov    eax, 100h
        test   eax, ecx
        je     SHORT loc_4_25A2
        test   dword ptr [edi + 6], eax
        je     SHORT loc_4_259E
        or     byte ptr [ebp - 1], 8
        jmp    SHORT loc_4_25A2
loc_4_259E:
        and    byte ptr [ebp - 1], 0f7h
loc_4_25A2:
        mov    eax, 800h
        test   eax, ecx
        je     SHORT loc_4_25BA
        test   dword ptr [edi + 6], eax
        je     SHORT loc_4_25B6
        or     byte ptr [ebp - 1], 1
        jmp    SHORT loc_4_25BA
loc_4_25B6:
        and    byte ptr [ebp - 1], 0feh
loc_4_25BA:
        mov    eax, 400h
        test   eax, ecx
        je     SHORT loc_4_25D2
        test   dword ptr [edi + 6], eax
        je     SHORT loc_4_25CE
        or     byte ptr [ebp - 1], 1
        jmp    SHORT loc_4_25D2
loc_4_25CE:
        and    byte ptr [ebp - 1], 0feh
loc_4_25D2:
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp - 1]
        push   ebx
        push   dword ptr [eax + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_25E1:
        add    esp, 0ch
        mov    eax, dword ptr [edi + 2]
        test   al, 2
        je     SHORT loc_4_25F0
        mov    esi, 0eh
loc_4_25F0:
        test   al, 1
        je     SHORT loc_4_25F7
        or     esi, 0eh
loc_4_25F7:
        test   al, 8
        je     SHORT loc_4_25FE
        or     esi, 0ch
loc_4_25FE:
        test   al, 4
        je     SHORT loc_4_2605
        or     esi, 0ch
loc_4_2605:
        test   ah, 20h
        je     SHORT loc_4_260D
        or     esi, 0ch
loc_4_260D:
        test   ah, 10h
        je     SHORT loc_4_2615
        or     esi, 0ch
loc_4_2615:
        mov    eax, esi
loc_4_2617:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_261E:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        lea    ebx, [edi + 2]
        test   byte ptr [ebx], 2
        je     SHORT loc_4_2647
        mov    eax, dword ptr [edi + 6]
        mov    esi, dword ptr [esp + 10h]
        shl    al, 6
        push   eax
        push   3fh
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2642:
        add    esp, 0ch
        jmp    SHORT loc_4_264B
loc_4_2647:
        mov    esi, dword ptr [esp + 10h]
loc_4_264B:
        test   byte ptr [ebx], 1
        je     SHORT loc_4_2667
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        shl    al, 6
        push   eax
        push   40h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2664:
        add    esp, 0ch
loc_4_2667:
        test   byte ptr [ebx], 4
        je     SHORT loc_4_2683
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        shl    al, 6
        push   eax
        push   42h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2680:
        add    esp, 0ch
loc_4_2683:
        test   byte ptr [ebx], 8
        je     SHORT loc_4_269C
        mov    eax, dword ptr [edi + 6]
        shl    al, 6
        push   eax
        push   41h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2699:
        add    esp, 0ch
loc_4_269C:
        test   byte ptr [ebx], 10h
        je     SHORT loc_4_26BA
        test   byte ptr [edi + 6], 1
        je     SHORT loc_4_26AB
        push   0
        jmp    SHORT loc_4_26AD
loc_4_26AB:
        push   1
loc_4_26AD:
        push   43h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_26B7:
        add    esp, 0ch
loc_4_26BA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_26C0:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 6]
        push   dword ptr [eax + 2]
        mov    eax, dword ptr [esp + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_26D6:
        add    esp, 0ch
        ret
sub_4_26DA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_26EE:
        add    esp, 4
        mov    edi, eax
        push   edi
        call   sub_4_0EE3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2705:
        add    esp, 4
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        test   byte ptr [edi + 2], 2
        je     SHORT loc_4_2722
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_28B7
        add    esp, 8
        mov    ebx, eax
loc_4_2722:
        test   byte ptr [edi + 2], 1
        je     SHORT loc_4_273A
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_290C
        add    esp, 8
        or     ebx, eax
loc_4_273A:
        test   byte ptr [edi + 2], 8
        je     SHORT loc_4_2768
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2961
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_2768
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_2768
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2A0B
        add    esp, 8
        or     ebx, eax
loc_4_2768:
        test   byte ptr [edi + 2], 4
        je     SHORT loc_4_279A
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_29B6
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_279A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_279A
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2A60
        add    esp, 8
        or     ebx, eax
loc_4_279A:
        test   byte ptr [edi + 2], 20h
        je     SHORT loc_4_27C8
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2A0B
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_27C8
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_27C8
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2961
        add    esp, 8
        or     ebx, eax
loc_4_27C8:
        test   byte ptr [edi + 2], 10h
        je     SHORT loc_4_27FE
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_2A60
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_27FE
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_27FE
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_29B6
        add    esp, 8
        or     ebx, eax
loc_4_27FE:
        test   byte ptr [edi + 2], 80h
        je     SHORT loc_4_2812
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2AB5
        add    esp, 8
        or     ebx, eax
loc_4_2812:
        test   byte ptr [edi + 2], 40h
        je     SHORT loc_4_282A
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_2B0A
        add    esp, 8
        or     ebx, eax
loc_4_282A:
        test   byte ptr [edi + 3], 2
        je     SHORT loc_4_283E
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2B5F
        add    esp, 8
        or     ebx, eax
loc_4_283E:
        test   byte ptr [edi + 3], 1
        je     SHORT loc_4_2856
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_2BB4
        add    esp, 8
        or     ebx, eax
loc_4_2856:
        test   byte ptr [edi + 3], 8
        je     SHORT loc_4_286A
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2C09
        add    esp, 8
        or     ebx, eax
loc_4_286A:
        test   byte ptr [edi + 3], 4
        je     SHORT loc_4_2882
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_2C5E
        add    esp, 8
        or     ebx, eax
loc_4_2882:
        test   byte ptr [edi + 3], 20h
        je     SHORT loc_4_2896
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2CB5
        add    esp, 8
        or     ebx, eax
loc_4_2896:
        test   byte ptr [edi + 3], 10h
        je     SHORT loc_4_28AE
        mov    eax, dword ptr [edi + 6]
        shr    eax, 10h
        push   eax
        push   esi
        call   sub_4_2CD1
        add    esp, 8
        or     ebx, eax
loc_4_28AE:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_28B7:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_28EB
        cmp    ax, 5
        je     SHORT loc_4_28EB
        cmp    ax, 7
        je     SHORT loc_4_28EB
        cmp    ax, 8
        je     SHORT loc_4_28EB
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_28FD
loc_4_28EB:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_28FD:
        push   eax
        push   30h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2908:
        add    esp, 0ch
        ret
sub_4_290C:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2940
        cmp    ax, 5
        je     SHORT loc_4_2940
        cmp    ax, 7
        je     SHORT loc_4_2940
        cmp    ax, 8
        je     SHORT loc_4_2940
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2952
loc_4_2940:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2952:
        push   eax
        push   31h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_295D:
        add    esp, 0ch
        ret
sub_4_2961:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2995
        cmp    ax, 5
        je     SHORT loc_4_2995
        cmp    ax, 7
        je     SHORT loc_4_2995
        cmp    ax, 8
        je     SHORT loc_4_2995
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_29A7
loc_4_2995:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_29A7:
        push   eax
        push   32h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_29B2:
        add    esp, 0ch
        ret
sub_4_29B6:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_29EA
        cmp    ax, 5
        je     SHORT loc_4_29EA
        cmp    ax, 7
        je     SHORT loc_4_29EA
        cmp    ax, 8
        je     SHORT loc_4_29EA
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_29FC
loc_4_29EA:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_29FC:
        push   eax
        push   33h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2A07:
        add    esp, 0ch
        ret
sub_4_2A0B:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2A3F
        cmp    ax, 5
        je     SHORT loc_4_2A3F
        cmp    ax, 7
        je     SHORT loc_4_2A3F
        cmp    ax, 8
        je     SHORT loc_4_2A3F
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2A51
loc_4_2A3F:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2A51:
        push   eax
        push   34h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2A5C:
        add    esp, 0ch
        ret
sub_4_2A60:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2A94
        cmp    ax, 5
        je     SHORT loc_4_2A94
        cmp    ax, 7
        je     SHORT loc_4_2A94
        cmp    ax, 8
        je     SHORT loc_4_2A94
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2AA6
loc_4_2A94:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2AA6:
        push   eax
        push   35h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2AB1:
        add    esp, 0ch
        ret
sub_4_2AB5:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2AE9
        cmp    ax, 5
        je     SHORT loc_4_2AE9
        cmp    ax, 7
        je     SHORT loc_4_2AE9
        cmp    ax, 8
        je     SHORT loc_4_2AE9
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2AFB
loc_4_2AE9:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2AFB:
        push   eax
        push   36h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B06:
        add    esp, 0ch
        ret
sub_4_2B0A:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2B3E
        cmp    ax, 5
        je     SHORT loc_4_2B3E
        cmp    ax, 7
        je     SHORT loc_4_2B3E
        cmp    ax, 8
        je     SHORT loc_4_2B3E
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2B50
loc_4_2B3E:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2B50:
        push   eax
        push   37h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B5B:
        add    esp, 0ch
        ret
sub_4_2B5F:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2B93
        cmp    ax, 5
        je     SHORT loc_4_2B93
        cmp    ax, 7
        je     SHORT loc_4_2B93
        cmp    ax, 8
        je     SHORT loc_4_2B93
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2BA5
loc_4_2B93:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2BA5:
        push   eax
        push   38h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2BB0:
        add    esp, 0ch
        ret
sub_4_2BB4:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2BE8
        cmp    ax, 5
        je     SHORT loc_4_2BE8
        cmp    ax, 7
        je     SHORT loc_4_2BE8
        cmp    ax, 8
        je     SHORT loc_4_2BE8
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2BFA
loc_4_2BE8:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2BFA:
        push   eax
        push   39h
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C05:
        add    esp, 0ch
        ret
sub_4_2C09:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2C3D
        cmp    ax, 5
        je     SHORT loc_4_2C3D
        cmp    ax, 7
        je     SHORT loc_4_2C3D
        cmp    ax, 8
        je     SHORT loc_4_2C3D
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2C4F
loc_4_2C3D:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2C4F:
        push   eax
        push   3ah
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C5A:
        add    esp, 0ch
        ret
sub_4_2C5E:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 28h]
        cmp    ax, 4
        je     SHORT loc_4_2C92
        cmp    ax, 5
        je     SHORT loc_4_2C92
        cmp    ax, 7
        je     SHORT loc_4_2C92
        cmp    ax, 8
        je     SHORT loc_4_2C92
        mov    ax, word ptr [esp + 8]
        shr    ax, 0bh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_2CA4
loc_4_2C92:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0dh
        movzx  edx, al
        db  08Ah, 082h, 000h, 000h, 000h, 000h
loc_4_2CA4:
        push   eax
        push   3ah
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2CAF:
        add    esp, 0ch
        or     al, 0ch
        ret
sub_4_2CB5:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   3bh
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2CCD:
        add    esp, 0ch
        ret
sub_4_2CD1:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   3bh
        push   dword ptr [ecx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2CE9:
        add    esp, 0ch
        or     al, 0ch
        ret
sub_4_2CEF:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 6], ax
        lea    ecx, [ebp - 2]
        push   ecx
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [esi + 2ch], 3
        mov    word ptr [esi + 2], 2
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [esi + 4], 199h
        push   dword ptr [edx + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2D24:
        add    esp, 8
        add    esi, 8
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2D40:
        add    esp, 10h
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
sub_4_2D4C:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_4_2D54:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_4_2D5C:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_4_2D64:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_4_2D6C:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_4_2D74:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh
sub_4_2D80:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_4_2D90:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        mov    esi, ecx
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_4_2DA4:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_4_2DB8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_4_2DC8:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_4_2DD8:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_4_2DE8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_4_2DF8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_4_2E0C:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_4_2E20:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_4_2E3E
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_4_2E3E:
        ret    10h
        db  0CCh, 0CCh, 0CCh
sub_4_2E44:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        int3
        int3
sub_4_2E4C:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
_PTEXT ends
        end
