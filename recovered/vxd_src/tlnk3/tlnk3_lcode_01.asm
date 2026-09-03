; tlnk3_lcode_01.asm -- TLNK3 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 12bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 12fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 133h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 137h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 112h], 0
        mov    edi, dword ptr [ebp + 8]
        add    edi, 1bch
        xor    eax, eax
        mov    ecx, 80h
        rep stosd
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0cdh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d1h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 44h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 46h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 111h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 116h], 0ffffh
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 118h], 0ffffh
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 11eh], 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_00C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 4], eax
loc_1_00E1:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_011B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_010E
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
loc_1_010E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_00E1
loc_1_011B:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_012C
        mov    dword ptr [ebp - 8], 0d0h
loc_1_012C:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1a8h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_013F:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0425
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0195:
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_028A
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_1_0224
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 1a8h]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_1_020F
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_00C8
loc_1_020F:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_217C
        jmp    NEAR PTR loc_1_0285
loc_1_0224:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 65h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 69h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 69h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 14h], 2
        mov    dword ptr [ebp - 0ch], 103h
loc_1_0285:
        jmp    NEAR PTR loc_1_0420
loc_1_028A:
        cmp    dword ptr [ebp - 0ch], 103h
        jne    NEAR PTR loc_1_031F
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_02C0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_02C0:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 65h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 69h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 69h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 14h], 2
        jmp    NEAR PTR loc_1_0420
loc_1_031F:
        cmp    dword ptr [ebp - 0ch], 0c001000eh
        jne    NEAR PTR loc_1_0391
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    dword ptr [ebp - 0ch], 103h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 65h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 69h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 69h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_0420
loc_1_0391:
        cmp    dword ptr [ebp - 0ch], 0c001000dh
        jne    NEAR PTR loc_1_0403
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 6dh
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 71h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 71h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 71h], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], 103h
        jmp    NEAR PTR loc_1_0420
loc_1_0403:
        push   1
        push   0ah
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        call   sub_1_2248
        add    esp, 14h
loc_1_0420:
        jmp    NEAR PTR loc_1_042C
loc_1_0425:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_1_042C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_1_047B
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_046D
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 28h]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_1_047B
loc_1_046D:
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0476:
        jmp    NEAR PTR loc_1_047B
loc_1_047B:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_0483
loc_1_0483:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_048A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_1_04D5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0BB7
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_059D
loc_1_04D5:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 1
        jne    NEAR PTR loc_1_0596
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b5h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0530
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0524
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0C31
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_052B
loc_1_0524:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_1_052B:
        jmp    NEAR PTR loc_1_0591
loc_1_0530:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0552
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_1_0591
loc_1_0552:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0574
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_1_0591
loc_1_0574:
        push   3
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        call   sub_1_2248
        add    esp, 14h
loc_1_0591:
        jmp    NEAR PTR loc_1_059D
loc_1_0596:
        mov    dword ptr [ebp - 0ch], 10001h
loc_1_059D:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_1_05EC
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_05DE
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_1_05EC
loc_1_05DE:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_05E7:
        jmp    NEAR PTR loc_1_05EC
loc_1_05EC:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_05F4
loc_1_05F4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_05FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 1ch], 1
        mov    dword ptr [ebp - 20h], 0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 4
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 40h], eax
        jmp    NEAR PTR loc_1_0ABF
loc_1_0631:
        mov    dword ptr [ebp - 0ch], 6
        jmp    NEAR PTR loc_1_0B65
loc_1_063D:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_1_065D
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 5ch
        jmp    NEAR PTR loc_1_066B
loc_1_065D:
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 84h
loc_1_066B:
        jmp    NEAR PTR loc_1_0B65
loc_1_0670:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0692
        mov    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_06F3
loc_1_0692:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1b2h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_06BE
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1b4h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_06CA
loc_1_06BE:
        mov    dword ptr [ebp - 8], 1
        jmp    NEAR PTR loc_1_06F3
loc_1_06CA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1b5h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_06EC
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_06F3
loc_1_06EC:
        mov    dword ptr [ebp - 8], 0
loc_1_06F3:
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_1_0B65
loc_1_0705:
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_1_0B65
loc_1_0717:
        mov    dword ptr [ebp - 0ch], 0d0h
        jmp    NEAR PTR loc_1_0B65
loc_1_0723:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 123h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 10h], 10106h
        jne    NEAR PTR loc_1_0740
        sub    dword ptr [ebp - 0ch], 0eh
loc_1_0740:
        jmp    NEAR PTR loc_1_0B65
loc_1_0745:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 33h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0764
        mov    dword ptr [ebp - 0ch], 27100h
        jmp    NEAR PTR loc_1_076B
loc_1_0764:
        mov    dword ptr [ebp - 0ch], 9c40h
loc_1_076B:
        jmp    NEAR PTR loc_1_0B65
loc_1_0770:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 121h]
        and    ecx, 0ffffh
        imul   eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0B65
loc_1_079A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 127h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 129h]
        and    ecx, 0ffffh
        imul   eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0B65
loc_1_07C4:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0B65
loc_1_07DB:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 129h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0B65
loc_1_07F2:
        push   3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4fh
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_225C
        and    dword ptr [ebp - 0ch], 0ffffff00h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0822
        or     dword ptr [ebp - 0ch], 1
loc_1_0822:
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_1_0B65
loc_1_0834:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_085A
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 2ch
        jmp    NEAR PTR loc_1_0868
loc_1_085A:
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 29h
loc_1_0868:
        jmp    NEAR PTR loc_1_0B65
loc_1_086D:
        mov    word ptr [ebp - 24h], 300h
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_0B65
loc_1_0885:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_1_08A3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d9h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_08AF
loc_1_08A3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
loc_1_08AF:
        jmp    NEAR PTR loc_1_0B65
loc_1_08B4:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_1_08CF
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_092B
loc_1_08CF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 0ch], 0
loc_1_08DF:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 28h]
        je     NEAR PTR loc_1_092B
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_091E
        mov    eax, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0d0h
        jne    NEAR PTR loc_1_091E
        jmp    NEAR PTR loc_1_092B
loc_1_091E:
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_08DF
loc_1_092B:
        jmp    NEAR PTR loc_1_0B65
loc_1_0930:
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4fh
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 6
        jmp    NEAR PTR loc_1_0B65
loc_1_0997:
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 49h
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 6
        jmp    NEAR PTR loc_1_0B65
loc_1_09FE:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_1_0A1C
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0A28
loc_1_0A1C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e5h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_0A28:
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 8
        and    eax, 0ff0000h
        mov    ecx, dword ptr [ebp - 0ch]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        shr    ecx, 18h
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0B65
loc_1_0A65:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0edh]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 8
        and    eax, 0ff0000h
        mov    ecx, dword ptr [ebp - 0ch]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        shr    ecx, 18h
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0B65
loc_1_0AAE:
        mov    dword ptr [ebp - 20h], 0c00000bbh
        jmp    NEAR PTR loc_1_0B65
        db  0E9h, 0A6h, 000h, 000h, 000h
loc_1_0ABF:
        cmp    dword ptr [ebp - 40h], 2010101h
        ja     NEAR PTR loc_1_0B39
        je     NEAR PTR loc_1_0930
        sub    dword ptr [ebp - 40h], 10101h
        cmp    dword ptr [ebp - 40h], 12h
        ja     NEAR PTR loc_1_0AAE
        mov    eax, dword ptr [ebp - 40h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0B39:
        cmp    dword ptr [ebp - 40h], 2010102h
        je     NEAR PTR loc_1_0997
        cmp    dword ptr [ebp - 40h], 2010103h
        je     NEAR PTR loc_1_09FE
        cmp    dword ptr [ebp - 40h], 2010104h
        je     NEAR PTR loc_1_0A65
        jmp    NEAR PTR loc_1_0AAE
loc_1_0B65:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_0BA8
        mov    eax, dword ptr [ebp + 1ch]
        cmp    dword ptr [ebp - 4], eax
        jbe    NEAR PTR loc_1_0B8F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 20h], 0c0010014h
        jmp    NEAR PTR loc_1_0BA8
loc_1_0B8F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_225C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], eax
loc_1_0BA8:
        mov    eax, dword ptr [ebp - 20h]
        jmp    NEAR PTR loc_1_0BB0
loc_1_0BB0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    20h
sub_1_0BB7:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 10h], 0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05FB
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_0C2A
loc_1_0C2A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_0C31:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 4
        je     NEAR PTR loc_1_0C96
        mov    dword ptr [ebp - 18h], 0c0010014h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_0E4F
loc_1_0C96:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_0DD9
loc_1_0CA1:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0EF1
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_0E2A
loc_1_0CBE:
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_225C
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0E56
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_0E2A
loc_1_0CEA:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0F78
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_0E2A
loc_1_0D07:
        mov    dword ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_0E2A
loc_1_0D13:
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_225C
        add    dword ptr [ebp - 1ch], 20h
        cmp    dword ptr [ebp - 1ch], 0d0h
        ja     NEAR PTR loc_1_0DA8
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 1a8h], ecx
        jae    NEAR PTR loc_1_0D5F
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1a8h], eax
        jmp    NEAR PTR loc_1_0DA3
loc_1_0D5F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1a8h]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_1_0D9A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_0D9A
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_00C8
        jmp    NEAR PTR loc_1_0DA3
loc_1_0D9A:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
loc_1_0DA3:
        jmp    NEAR PTR loc_1_0DAF
loc_1_0DA8:
        mov    dword ptr [ebp - 18h], 0c0010014h
loc_1_0DAF:
        jmp    NEAR PTR loc_1_0E2A
loc_1_0DB4:
        mov    dword ptr [ebp - 18h], 0c0010017h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        jmp    NEAR PTR loc_1_0E2A
        db  0E9h, 051h, 000h, 000h, 000h
loc_1_0DD9:
        cmp    dword ptr [ebp - 24h], 2010103h
        ja     NEAR PTR loc_1_0E18
        je     NEAR PTR loc_1_0CA1
        cmp    dword ptr [ebp - 24h], 1010eh
        je     NEAR PTR loc_1_0CBE
        cmp    dword ptr [ebp - 24h], 1010fh
        je     NEAR PTR loc_1_0D13
        cmp    dword ptr [ebp - 24h], 10112h
        je     NEAR PTR loc_1_0D07
        jmp    NEAR PTR loc_1_0DB4
loc_1_0E18:
        cmp    dword ptr [ebp - 24h], 2010104h
        je     NEAR PTR loc_1_0CEA
        jmp    NEAR PTR loc_1_0DB4
loc_1_0E2A:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_0E47
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
loc_1_0E47:
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_0E4F
loc_1_0E4F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_0E56:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 14h], 8076h
        je     NEAR PTR loc_1_0E7D
        mov    eax, 0c00000bbh
        jmp    NEAR PTR loc_1_0EEA
loc_1_0E7D:
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0EA3:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        cmp    dword ptr [ebp - 4], 103h
        jne    NEAR PTR loc_1_0EE2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0EE2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0EE2:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0EEA
loc_1_0EEA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_0EF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0F2A:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        cmp    dword ptr [ebp - 4], 103h
        jne    NEAR PTR loc_1_0F69
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0F69
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0F69:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0F71
loc_1_0F71:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_0F78:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0FB1:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        cmp    dword ptr [ebp - 4], 103h
        jne    NEAR PTR loc_1_0FF0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0FF0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0FF0:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0FF8
loc_1_0FF8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_0FFF:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 4], 0ch
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05FB
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0c00000bbh
        jne    NEAR PTR loc_1_1209
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_1145
loc_1_107F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 13bh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_1090:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 13fh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_10A1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 143h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_10B2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 147h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_10C3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 163h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_10D4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14bh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_10E5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15fh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_10F6:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1b6h]
        and    eax, 0ffffh
        or     eax, 0c0011000h
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_1112:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 19bh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_1123:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 19fh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11CB
loc_1_1134:
        mov    dword ptr [ebp - 18h], 0c0010017h
        jmp    NEAR PTR loc_1_11CB
        db  0E9h, 086h, 000h, 000h, 000h
loc_1_1145:
        cmp    dword ptr [ebp - 20h], 2010105h
        ja     NEAR PTR loc_1_1187
        je     NEAR PTR loc_1_10F6
        sub    dword ptr [ebp - 20h], 20101h
        cmp    dword ptr [ebp - 20h], 4
        ja     NEAR PTR loc_1_1134
        mov    eax, dword ptr [ebp - 20h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1187:
        cmp    dword ptr [ebp - 20h], 2020101h
        ja     NEAR PTR loc_1_11B9
        je     NEAR PTR loc_1_10D4
        cmp    dword ptr [ebp - 20h], 2010106h
        je     NEAR PTR loc_1_1112
        cmp    dword ptr [ebp - 20h], 2010107h
        je     NEAR PTR loc_1_1123
        jmp    NEAR PTR loc_1_1134
loc_1_11B9:
        cmp    dword ptr [ebp - 20h], 2020102h
        je     NEAR PTR loc_1_10E5
        jmp    NEAR PTR loc_1_1134
loc_1_11CB:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_1209
        cmp    dword ptr [ebp - 8], 4
        jb     NEAR PTR loc_1_11F7
        push   4
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_225C
        add    dword ptr [ebp - 14h], 4
        jmp    NEAR PTR loc_1_1209
loc_1_11F7:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], 0c0010014h
loc_1_1209:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_1223
loc_1_1223:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_122A:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_1276
        db  0E9h, 0C0h, 000h, 000h, 000h
loc_1_1259:
        mov    dword ptr [ebp - 8], 103h
        jmp    NEAR PTR loc_1_1319
loc_1_1265:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    NEAR PTR loc_1_1319
        db  0E9h, 0A3h, 000h, 000h, 000h
loc_1_1276:
        cmp    dword ptr [ebp - 14h], 10111h
        ja     NEAR PTR loc_1_12AF
        cmp    dword ptr [ebp - 14h], 1010dh
        jae    NEAR PTR loc_1_1319
        cmp    dword ptr [ebp - 14h], 10101h
        jb     NEAR PTR loc_1_1265
        cmp    dword ptr [ebp - 14h], 1010bh
        jbe    NEAR PTR loc_1_1319
        jmp    NEAR PTR loc_1_1265
loc_1_12AF:
        cmp    dword ptr [ebp - 14h], 20105h
        ja     NEAR PTR loc_1_12DB
        cmp    dword ptr [ebp - 14h], 20101h
        jae    NEAR PTR loc_1_1259
        cmp    dword ptr [ebp - 14h], 10113h
        je     NEAR PTR loc_1_1319
        jmp    NEAR PTR loc_1_1265
loc_1_12DB:
        cmp    dword ptr [ebp - 14h], 2020102h
        ja     NEAR PTR loc_1_1265
        cmp    dword ptr [ebp - 14h], 2020101h
        jae    NEAR PTR loc_1_1259
        cmp    dword ptr [ebp - 14h], 2010101h
        jb     NEAR PTR loc_1_1265
        cmp    dword ptr [ebp - 14h], 2010107h
        jbe    NEAR PTR loc_1_1319
        jmp    NEAR PTR loc_1_1265
        db  0E9h, 04Ch, 0FFh, 0FFh, 0FFh
loc_1_1319:
        cmp    dword ptr [ebp - 8], 103h
        jne    NEAR PTR loc_1_13AE
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_1_136F
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0d1h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0d1h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0cdh], eax
        jmp    NEAR PTR loc_1_137D
loc_1_136F:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0d1h]
        mov    dword ptr [ecx], eax
loc_1_137D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_13A6
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_13A6:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_141F
loc_1_13AE:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_13C8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0FFF
        mov    dword ptr [ebp - 8], eax
loc_1_13C8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_1_1417
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1409
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_1_1417
loc_1_1409:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1412:
        jmp    NEAR PTR loc_1_1417
loc_1_1417:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_141F
loc_1_141F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_1426:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 103h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_146D
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_1_14C7
loc_1_146D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_148F
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_1_14C7
loc_1_148F:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_14C0
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1AB1
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_14C7
loc_1_14C0:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_1_14C7:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_1_1516
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1508
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_1_1516
loc_1_1508:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1511:
        jmp    NEAR PTR loc_1_1516
loc_1_1516:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_151E
loc_1_151E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1525:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_155A
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 1eh
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 10h], 4
loc_1_155A:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_157C
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    NEAR PTR loc_1_1628
loc_1_157C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        jne    NEAR PTR loc_1_1588
loc_1_1588:
        test   byte ptr [ebp + 0ch], 30h
        je     NEAR PTR loc_1_159E
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_1628
loc_1_159E:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 10h], 4
        jne    NEAR PTR loc_1_15C8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_15D1
loc_1_15C8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
loc_1_15D1:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_1_160D
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0d1h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0d1h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0cdh], eax
        jmp    NEAR PTR loc_1_161B
loc_1_160D:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0d1h]
        mov    dword ptr [ecx], eax
loc_1_161B:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], 103h
loc_1_1628:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1630
loc_1_1630:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_1637:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_166C
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 46h
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 10h], 9
loc_1_166C:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_168E
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    NEAR PTR loc_1_16FD
loc_1_168E:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_1_16E2
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0d1h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0d1h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0cdh], eax
        jmp    NEAR PTR loc_1_16F0
loc_1_16E2:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0d1h]
        mov    dword ptr [ecx], eax
loc_1_16F0:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], 103h
loc_1_16FD:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1705
loc_1_1705:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_170C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_1741
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 6eh
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 10h], 0ah
loc_1_1741:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1763
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    NEAR PTR loc_1_17D2
loc_1_1763:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_1_17B7
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0d1h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0d1h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0cdh], eax
        jmp    NEAR PTR loc_1_17C5
loc_1_17B7:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0d1h]
        mov    dword ptr [ecx], eax
loc_1_17C5:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], 103h
loc_1_17D2:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_17DA
loc_1_17DA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_17E1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_17F4:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_216A
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 55h]
        inc    edx
        xor    al, al
        out    dx, al
        push   0c350h
        call   sub_1_2170
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 55h]
        add    edx, 2
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 36h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1858
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e18h], 0c0h
loc_1_1858:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1880
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 43h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0f1h]
        mov    byte ptr [ecx + 1e00h], al
loc_1_1880:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_216A
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e08h], 0c0h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 12bh]
        mov    dword ptr [ebp - 0ch], eax
loc_1_18B4:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1917
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_18E5
loc_1_18E5:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 143h]
        push   0c001000ch
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_20BA
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_18B4
loc_1_1917:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 112h], 0
        je     NEAR PTR loc_1_1973
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 112h]
        add    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_194C
loc_1_194C:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 143h]
        push   0c001000ch
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_20BA
loc_1_1973:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1982
loc_1_197F:
        inc    dword ptr [ebp - 4]
loc_1_1982:
        cmp    dword ptr [ebp - 4], 80h
        jae    NEAR PTR loc_1_19F4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 1bch]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_19EF
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_19C8
loc_1_19C8:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 143h]
        push   0c001000ch
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_20BA
loc_1_19EF:
        jmp    NEAR PTR loc_1_197F
loc_1_19F4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_19FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ach], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1adh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1A3E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 4], eax
loc_1_1A47:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_1A8A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_20DE
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_20E4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1A47
loc_1_1A8A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        dec    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_20C6
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_1AB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 4], eax
loc_1_1AC3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_1AFA
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_20DE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1AC3
loc_1_1AFA:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1a3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1aeh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 0
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_1B37
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
loc_1_1B37:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  000h, 000h
loc_1_1B40:
        db  0A0h, 008h, 023h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_1B65
        db  0FEh, 005h, 008h, 023h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_1B67
        push   0
        push   0
        db  0E8h, 0A0h, 0E4h, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_1B67
loc_1_1B65:
        clc
        ret
loc_1_1B67:
        stc
        ret
IBMTOK_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_1B78
        db  0FFh, 024h, 085h, 07Ah, 01Bh, 000h, 000h
loc_1_1B78:
        clc
        ret
        db  040h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h
        db  078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h
        db  078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h
        db  078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h
        db  078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h
        db  078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h
        db  078h, 01Bh, 000h, 000h, 078h, 01Bh, 000h, 000h, 040h, 01Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_1BF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C20
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 8
        cmp    dword ptr [ebp + 14h], 0eh
        jb     NEAR PTR loc_1_1D40
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     NEAR PTR loc_1_1CE4
        mov    al, byte ptr [ecx]
        cmp    al, 0ffh
        jne    SHORT loc_1_1C62
        cmp    byte ptr [ecx + 1], 0ffh
        je     SHORT loc_1_1C6C
loc_1_1C62:
        cmp    al, 0c0h
        jne    SHORT loc_1_1C88
        cmp    byte ptr [ecx + 1], 0
        jne    SHORT loc_1_1C88
loc_1_1C6C:
        cmp    byte ptr [ecx + 2], 0ffh
        jne    SHORT loc_1_1C88
        cmp    byte ptr [ecx + 3], 0ffh
        jne    SHORT loc_1_1C88
        cmp    byte ptr [ecx + 4], 0ffh
        jne    SHORT loc_1_1C88
        cmp    byte ptr [ecx + 5], 0ffh
        mov    byte ptr [ebp - 2], 1
        je     SHORT loc_1_1C8C
loc_1_1C88:
        mov    byte ptr [ebp - 2], 0
loc_1_1C8C:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_1CA0
        mov    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_1D4B
        db  08Bh, 0FFh
loc_1_1CA0:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 1000h
        jne    SHORT loc_1_1CBC
        mov    dword ptr [ebp - 8], 4000h
loc_1_1CBC:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0ch], edx
        jmp    SHORT loc_1_1D4B
loc_1_1CE4:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 10h], 30h
        je     SHORT loc_1_1D30
        mov    edx, dword ptr [esi + 1ah]
        lea    eax, [esi + 18h]
        cmp    dword ptr [ecx + 2], edx
        jne    SHORT loc_1_1D07
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_1D0E
loc_1_1D07:
        mov    dword ptr [ebp - 10h], 1
loc_1_1D0E:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_1D20
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_1D4E
        db  08Dh, 049h, 000h
loc_1_1D20:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_1D4E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D30:
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_1D4E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D40:
        mov    dword ptr [ebp - 8], 20h
        mov    byte ptr [ebp - 1], 0
loc_1_1D4B:
        mov    esi, dword ptr [ebp + 8]
loc_1_1D4E:
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     NEAR PTR loc_1_1E31
loc_1_1D59:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    SHORT loc_1_1D9D
        test   ah, 40h
        je     SHORT loc_1_1D72
        mov    eax, dword ptr [edi + 0ch]
        test   dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_1D9D
loc_1_1D72:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_1D7D
        test   ch, 20h
        jne    SHORT loc_1_1D9D
loc_1_1D7D:
        test   byte ptr [ebp - 7], 10h
        je     SHORT loc_1_1D89
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_1D9D
loc_1_1D89:
        test   cl, 10h
        je     SHORT loc_1_1D94
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_1D9D
loc_1_1D94:
        test   cl, 20h
        je     NEAR PTR loc_1_1E26
loc_1_1D9D:
        inc    dword ptr [edi + 18h]
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_20D2
        mov    byte ptr [edi + 1eh], 1
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1E26
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_1DDF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_1DDF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1DF0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1DF6
        db  08Bh, 0FFh
loc_1_1DF0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_1DF6:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_1E05
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_20D8
loc_1_1E05:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_217C
        mov    edi, ebx
        jmp    SHORT loc_1_1E29
loc_1_1E26:
        mov    edi, dword ptr [edi + 10h]
loc_1_1E29:
        or     edi, edi
        jne    NEAR PTR loc_1_1D59
loc_1_1E31:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1E40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E50
        pop    ebp
        ret    4
sub_1_1E50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     SHORT loc_1_1ECE
loc_1_1E60:
        inc    dword ptr [edi + 18h]
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_1E76
        mov    byte ptr [edi + 1eh], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_20D8
loc_1_1E76:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1EC7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_1E8C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_1E8C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1EA0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1EA6
        db  005h, 000h, 000h, 000h, 000h
loc_1_1EA0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_1EA6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_217C
        mov    edi, ebx
        jmp    SHORT loc_1_1ECA
loc_1_1EC7:
        mov    edi, dword ptr [edi + 10h]
loc_1_1ECA:
        or     edi, edi
        jne    SHORT loc_1_1E60
loc_1_1ECE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_1F32
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_1F32
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_1F32
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1F32:
        mov    esi, dword ptr [eax + 10h]
        or     esi, esi
        jne    SHORT loc_1_1F42
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1F42:
        test   esi, 20h
        je     SHORT loc_1_1F53
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1F53:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     SHORT loc_1_1F70
        test   esi, 10h
        je     SHORT loc_1_1F70
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_1F70:
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        and    cl, 80h
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     SHORT loc_1_1FFA
        mov    cl, byte ptr [eax]
        cmp    cl, 0ffh
        jne    SHORT loc_1_1F8C
        cmp    byte ptr [eax + 1], 0ffh
        je     SHORT loc_1_1F97
loc_1_1F8C:
        cmp    cl, 0c0h
        jne    SHORT loc_1_1FB3
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_1_1FB3
loc_1_1F97:
        cmp    byte ptr [eax + 2], 0ffh
        jne    SHORT loc_1_1FB3
        cmp    byte ptr [eax + 3], 0ffh
        jne    SHORT loc_1_1FB3
        cmp    byte ptr [eax + 4], 0ffh
        jne    SHORT loc_1_1FB3
        cmp    byte ptr [eax + 5], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_1_1FB7
loc_1_1FB3:
        mov    byte ptr [ebp - 1], 0
loc_1_1FB7:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1FE0
        test   esi, 8
        je     SHORT loc_1_1FD0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_1FD0:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1FE0:
        test   esi, 6000h
        je     SHORT loc_1_1FF1
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1FF1:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1FFA:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 056h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_208A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_2090:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_2096:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_209C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_20A2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_20A8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_20AE:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_20B4:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_20BA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_20C6:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_20CC:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_20D2:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_20D8:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_20DE:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_20E4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
sub_1_20EA:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_20F0:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
sub_1_20F6:
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_20FC:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_2152:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_2158:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_215E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_2164:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_216A:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_2170:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_2176:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_217C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_21A0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_21C4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_21E8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_21F4:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_21FA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_2200:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
sub_1_2242:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_2248:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_225C:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 049h, 042h, 04Dh, 054h
        db  04Fh, 04Bh, 020h, 020h, 000h, 040h, 000h, 054h, 069h, 01Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2324:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2328:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_232C:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_2330:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
loc_1_2334:
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
loc_1_2338:
        xor    al, 58h
        xor    esi, dword ptr [esi]
loc_1_233C:
        and    byte ptr [eax], bh
        xor    dword ptr [6b6e6c54h], esi
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_2348:
        push   esp
        insb
        outsb
        imul   esi, dword ptr [ebx], 0
        add    byte ptr [eax], al
loc_1_2350:
        dec    ecx
        bound  ebp, qword ptr [ebp + 20h]
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        db  077h, 06Fh
        jb     SHORT loc_1_23D1
        and    byte ptr [ebx + 61h], al
        jb     SHORT loc_1_23CF
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_1_2390
        push   eax
        inc    ebx
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [edx + 75h], ah
        jae    SHORT loc_1_23A9
        add    byte ptr [ecx + 62h], cl
        insd
        and    byte ptr [edi + ebp*2 + 6bh], dl
        outsb
        and    byte ptr [edx + 69h], dl
        outsb
        and    byte ptr [bp + 65h], cl
        db  074h, 077h
        outsd
loc_1_2390:
        jb     SHORT loc_1_23FD
        and    byte ptr [ebx + 61h], al
        jb     SHORT loc_1_23FB
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_1_23BC
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 75h], ah
        jae    SHORT loc_1_23D2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
loc_1_23A9:
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_2408
        jns    SHORT loc_1_2426
        add    byte ptr gs:[ecx + 4fh], cl
        inc    edx
        inc    ecx
loc_1_23BC:
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_23C0:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_23CF:
        inc    ebp
        push   ebx
loc_1_23D1:
        push   ebx
loc_1_23D2:
        add    byte ptr [eax], al
loc_1_23D4:
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 41h], cl
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
loc_1_23FB:
        pop    edx
        inc    ebp
loc_1_23FD:
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [esi], al
        add    byte ptr [ebx], al
        add    byte ptr [edx], al
        db  000h
loc_1_2408:
        pop    es
        add    byte ptr [edi], cl
        add    byte ptr [esi], al
        add    byte ptr [ebx], cl
        add    byte ptr [ebx], al
        add    byte ptr [edx], cl
        add    byte ptr [edx], al
        add    byte ptr [0], al
loc_1_241B:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2426:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2434:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2438:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2448:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_245C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_246C:
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
loc_1_24BE:
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
loc_1_250C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
