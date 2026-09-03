; t30n3_pcode_05_01.asm -- T30N3 object 5 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 50h]
        mov    dword ptr [ebp - 18h], eax
        mov    word ptr [ebp - 10h], 0
        mov    word ptr [ebp - 0eh], 1
        mov    word ptr [ebp - 0ch], 50h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp + 8], eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_003A:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_004D:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0064
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_0137
loc_5_0064:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 006h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 00Ah, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 01Ah, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 00Eh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 012h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 016h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 022h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 01Eh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 026h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 02Ah, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 18h]
        add    edx, 2eh
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   36h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0111:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0130
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0126:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_5_0137
loc_5_0130:
        xor    eax, eax
        jmp    NEAR PTR loc_5_0137
loc_5_0137:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_013E:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 34h], 0
        jmp    NEAR PTR loc_5_0150
loc_5_0150:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0CA3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0D8A
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_2168
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_5_0184
        mov    byte ptr [ebp + 14h], 1
loc_5_0184:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_22B8
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_5_01A6
        mov    byte ptr [ebp + 14h], 1
loc_5_01A6:
        lea    edi, [ebp - 30h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 26h], 3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ceh]
        mov    dword ptr [ebp - 30h], eax
        mov    byte ptr [ebp - 2ch], 1
        mov    byte ptr [ebp - 2bh], 0
        mov    dword ptr [ebp - 2ah], 0
        mov    dword ptr [ebp - 22h], 0
        mov    dword ptr [ebp - 1ah], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1c6h]
        mov    dword ptr [ebp - 16h], eax
        mov    dword ptr [ebp - 12h], 4
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 52h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 56h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_021A:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_5_022F
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_04B0
loc_5_022F:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_5_0268
        push   0
        push   0aa00000ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_024F:
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_025E:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_5_04B0
loc_5_0268:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1088
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_5_029C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0292:
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_5_04B0
loc_5_029C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_48A4
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_02BC
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_04B0
loc_5_02BC:
        push   2
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1bah]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1bah]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02F6:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_5_0337
        push   3
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0317:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4DA4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_032F:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_04B0
loc_5_0337:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 23ch], 0
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 040h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 244h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1edh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_038D:
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_03F5
        push   2
        push   0
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03B1:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ch], 1
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03CD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03D6:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4DA4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03EB:
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_04B0
loc_5_03F5:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_10D8
        and    ax, 0ffh
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_5_0482
        mov    byte ptr [ebp + 14h], 1
        push   1
        push   0
        push   2
        push   0aa000003h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0432:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ch], 1
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_044E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_045A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0463:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4DA4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0478:
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_5_04B0
loc_5_0482:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 248h], 3ch
        push   3e8h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04A9:
        xor    eax, eax
        jmp    NEAR PTR loc_5_04B0
loc_5_04B0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
loc_5_04B7:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_04DF
loc_5_04DC:
        inc    dword ptr [ebp - 0ch]
loc_5_04DF:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 0ch], eax
        jae    NEAR PTR loc_5_0505
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + eax*4], 1
        jne    NEAR PTR loc_5_0500
        jmp    NEAR PTR loc_5_0505
loc_5_0500:
        jmp    NEAR PTR loc_5_04DC
loc_5_0505:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 0ch], eax
        jne    NEAR PTR loc_5_051B
        mov    eax, 0c0010019h
        jmp    NEAR PTR loc_5_06B6
loc_5_051B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 2ah]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   96h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0545:
        test   eax, eax
        jne    NEAR PTR loc_5_063E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05B1:
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_05F4
        push   4
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05D5:
        add    esp, 14h
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05E8:
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    NEAR PTR loc_5_0639
loc_5_05F4:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0f0h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1eh], 0f0h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 36h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 3ah]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 3ah]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 3ah], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
loc_5_0639:
        jmp    NEAR PTR loc_5_0662
loc_5_063E:
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0655:
        add    esp, 14h
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_06B6
loc_5_0662:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_06AE
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_06A0
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_06AE
loc_5_06A0:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_06AE
loc_5_06AE:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_5_06B6
loc_5_06B6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    24h
loc_5_06BD:
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
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_097A
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0710:
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_0802
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_5_079C
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
        mov    ecx, dword ptr [ecx + 1eh]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_0787
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_35D5
loc_5_0787:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0797:
        jmp    NEAR PTR loc_5_07FD
loc_5_079C:
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
        add    eax, 2eh
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 32h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 32h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 32h], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 14h], 2
        mov    dword ptr [ebp - 0ch], 103h
loc_5_07FD:
        jmp    NEAR PTR loc_5_0975
loc_5_0802:
        cmp    dword ptr [ebp - 0ch], 103h
        jne    NEAR PTR loc_5_0877
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_280B
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
        add    eax, 2eh
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 32h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 32h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 32h], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 14h], 2
        jmp    NEAR PTR loc_5_0975
loc_5_0877:
        cmp    dword ptr [ebp - 0ch], 0c001000eh
        jne    NEAR PTR loc_5_08E9
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
        add    eax, 2eh
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 32h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 32h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 32h], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_0975
loc_5_08E9:
        cmp    dword ptr [ebp - 0ch], 0c001000dh
        jne    NEAR PTR loc_5_095B
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
        add    eax, 3eh
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 42h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 42h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 42h], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], 103h
        jmp    NEAR PTR loc_5_0975
loc_5_095B:
        push   1
        push   0ah
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0972:
        add    esp, 14h
loc_5_0975:
        jmp    NEAR PTR loc_5_0981
loc_5_097A:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_0981:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_09CD
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_09BF
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 28h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_09CD
loc_5_09BF:
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_09CD
loc_5_09CD:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_09D5
loc_5_09D5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_09DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 20h], 0
        mov    word ptr [ebp - 34h], 0bh
        mov    word ptr [ebp - 32h], 0bh
        db  0C7h, 045h, 0D0h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   2deh
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A19:
        test   eax, eax
        je     NEAR PTR loc_5_0A2B
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_0BF2
loc_5_0A2B:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0b7h
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 21dh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 52h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1f3h], 1194h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A6F:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_5_0A93
        push   0
        push   2deh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A89:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_5_0BF2
loc_5_0A93:
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0AA8:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_5_0AF9
        cmp    dword ptr [ebp - 4], 6
        jbe    NEAR PTR loc_5_0AC3
        mov    dword ptr [ebp - 4], 6
loc_5_0AC3:
        mov    dword ptr [ebp - 1ch], 0
        jmp    NEAR PTR loc_5_0AD2
loc_5_0ACF:
        inc    dword ptr [ebp - 1ch]
loc_5_0AD2:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 1ch], eax
        jae    NEAR PTR loc_5_0AF9
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 1edh], al
        jmp    NEAR PTR loc_5_0ACF
loc_5_0AF9:
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B10:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_5_0B29
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1f3h], eax
loc_5_0B29:
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B3E:
        mov    al, byte ptr [ebp - 2ah]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1dbh], al
        mov    al, byte ptr [ebp - 29h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1dch], al
        mov    al, byte ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1ddh], al
        mov    al, byte ptr [ebp - 27h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1deh], al
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        dec    eax
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 1d7h], ax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_5_0B8F
        mov    byte ptr [ebp - 0ch], 1
loc_5_0B8F:
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B98:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1f3h], 1194h
        jbe    NEAR PTR loc_5_0BB8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1f3h], 1194h
loc_5_0BB8:
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_013E
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_5_0BEA
        push   0
        push   2deh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BEA:
        mov    eax, dword ptr [ebp - 20h]
        jmp    NEAR PTR loc_5_0BF2
loc_5_0BF2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_5_0BF9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 1
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C25:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C2E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C3A:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_4DA4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C4F:
        push   0
        push   2deh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C5F:
        jmp    NEAR PTR loc_5_0C64
loc_5_0C64:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0C6B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C8A:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C97:
        jmp    NEAR PTR loc_5_0C9C
loc_5_0C9C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_0CA3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 3ah], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 36h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2eh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 32h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 32h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2eh], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3eh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 42h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 42h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 3eh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 248h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 46h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 84h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 88h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 72h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 22h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 26h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_0D8A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 62h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 66h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 5eh], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_0DCD
loc_5_0DCA:
        inc    dword ptr [ebp - 4]
loc_5_0DCD:
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        cmp    eax, dword ptr [ebp - 4]
        jbe    NEAR PTR loc_5_0DEE
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DE9:
        jmp    NEAR PTR loc_5_0DCA
loc_5_0DEE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0DF5:
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
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0E39
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_5_0E90
loc_5_0E39:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0E58
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_0E90
loc_5_0E58:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0E89
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_0FFD
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_0E90
loc_5_0E89:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_0E90:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_0EDC
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0ECE
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_0EDC
loc_5_0ECE:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_0EDC
loc_5_0EDC:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_0EE4
loc_5_0EE4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_0EEB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
loc_5_0EF4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 62h], 0
        je     NEAR PTR loc_5_0F50
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 62h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 62h], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 80h]
        push   0c0000001h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F45:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_0EF4
loc_5_0F50:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0F60
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_0F60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0D8A
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7ch], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4ah], 0
        je     NEAR PTR loc_5_0FF6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ah]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 36h]
        mov    dword ptr [ebp - 4], eax
loc_5_0F9F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_5_0FE2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FC9:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FD5:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_0F9F
loc_5_0FE2:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FF6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_0FFD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_1050
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 36h]
        mov    dword ptr [ebp - 4], eax
loc_5_1019:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_5_1050
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1043:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_1019
loc_5_1050:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7dh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7ch], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4ah], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 46h], 0
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_1081
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
loc_5_1081:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_1088:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_24C8
        test   ax, ax
        je     SHORT loc_5_10A3
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_10A3:
        push   esi
        call   sub_5_1308
        test   ax, ax
        je     SHORT loc_5_10B7
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_10B7:
        push   esi
        call   sub_5_13B8
        cmp    ax, 1
        pop    esi
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_10D8:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 14dh], ax
        mov    dword ptr [esi + 122h], eax
        push   esi
        mov    dword ptr [esi + 11eh], eax
        mov    dword ptr [esi + 12ah], 1
        mov    dword ptr [esi + 126h], 2
        call   sub_5_15C8
        test   ax, ax
        je     SHORT loc_5_111C
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_111C:
        push   esi
        call   sub_5_1138
        cmp    ax, 1
        pop    esi
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1138:
        push   ebp
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 14h
        mov    eax, dword ptr [edx]
        push   esi
        push   edi
        lea    ecx, [ebp - 14h]
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [edx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dx, word ptr [edx + 10h]
        mov    dword ptr [ecx + 4], esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], edi
        mov    dword ptr [ecx + 0ch], eax
        xor    eax, eax
        mov    word ptr [ecx + 10h], dx
loc_5_116B:
        mov    dl, byte ptr [ebp + eax - 14h]
        mov    ecx, dword ptr [esi + 10h]
        inc    eax
        cmp    eax, 12h
        mov    byte ptr [ecx + eax - 1], dl
        jb     SHORT loc_5_116B
        mov    eax, dword ptr [esi + 8]
        xor    edi, edi
        mov    word ptr [eax], di
        mov    eax, dword ptr [esi + 8]
        mov    dx, word ptr [esi + 161h]
        mov    word ptr [eax + 2], dx
        mov    eax, dword ptr [esi + 8]
        mov    dx, word ptr [esi + 163h]
        mov    word ptr [eax + 4], dx
        mov    eax, dword ptr [esi + 8]
        mov    dx, word ptr [esi + 165h]
        mov    word ptr [eax + 6], dx
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 8], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0ah], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0ch], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0eh], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 10h], 0eh
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 12h], 1ah
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 14h], 3f8h
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 16h], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 18h], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 1ah], 40ch
        mov    eax, dword ptr [esi + 98h]
        mov    ecx, dword ptr [esi + 8]
        shr    eax, 10h
        push   2
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [esi + 98h]
        mov    ecx, dword ptr [esi + 8]
        mov    word ptr [ecx + 1eh], ax
        mov    eax, dword ptr [esi + 8]
        add    eax, 2
        push   eax
        call   sub_5_1578
        push   2
        mov    eax, dword ptr [esi + 8]
        add    eax, 4
        push   eax
        call   sub_5_1578
        push   2
        mov    eax, dword ptr [esi + 8]
        add    eax, 6
        push   eax
        call   sub_5_1578
        push   20h
        mov    ecx, dword ptr [esi + 8]
        push   ecx
        call   sub_5_1A18
        mov    dword ptr [esi + 126h], 3
        push   esi
        call   sub_5_16E8
        mov    dx, word ptr [esi + 118h]
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx], dx
        mov    ecx, dword ptr [esi]
        mov    dx, word ptr [esi + 11ah]
        push   9080h
        mov    word ptr [ecx + 2], dx
        push   esi
        mov    ax, word ptr [esi + 11ch]
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx + 4], ax
        call   sub_5_12E8
loc_5_128F:
        mov    eax, dword ptr [esi + 126h]
        cmp    eax, 1
        je     SHORT loc_5_12C1
        cmp    eax, 5
        je     SHORT loc_5_12CC
        cmp    eax, 4
        je     SHORT loc_5_12D8
        push   0ah
        inc    edi
        push   esi
        call   sub_5_502C
        cmp    edi, 1194h
        jb     SHORT loc_5_128F
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_12C1:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_12CC:
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_12D8:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_5_12E8:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 13ah]
        add    ecx, 6
        push   ecx
        push   eax
        call   sub_5_5087
        pop    ebp
        ret    8
        db  090h
sub_5_1308:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_5_5087
        push   0
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 4
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_5087
loc_5_1336:
        mov    eax, dword ptr [esi + 13ah]
        inc    edi
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_50E3
        mov    byte ptr [esi + edi + 15ah], ah
        cmp    edi, 6
        jb     SHORT loc_5_1336
        cmp    byte ptr [esi + 161h], 0
        jne    SHORT loc_5_13A5
        cmp    byte ptr [esi + 162h], 0
        jne    SHORT loc_5_13A5
        cmp    byte ptr [esi + 163h], 0
        jne    SHORT loc_5_13A5
        cmp    byte ptr [esi + 164h], 0
        jne    SHORT loc_5_13A5
        cmp    byte ptr [esi + 165h], 0
        jne    SHORT loc_5_13A5
        cmp    byte ptr [esi + 166h], 0
        jne    SHORT loc_5_13A5
        xor    cx, cx
loc_5_138C:
        movsx  eax, cx
        inc    cx
        add    eax, esi
        cmp    cx, 6
        mov    dl, byte ptr [eax + 15bh]
        mov    byte ptr [eax + 161h], dl
        jl     SHORT loc_5_138C
loc_5_13A5:
        mov    al, byte ptr [esi + 15dh]
        pop    edi
        sub    al, 0b8h
        pop    esi
        pop    ebp
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        ret    4
sub_5_13B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_50E3
        and    ax, 107ch
        db  0BFh, 000h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_5087
loc_5_13EE:
        mov    eax, edi
        add    edi, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_5_5087
        mov    ax, word ptr [edi]
        mov    ecx, edi
        add    edi, 2
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    edi, 2
        add    eax, 4
        push   eax
        push   esi
        call   sub_5_5087
        mov    ax, word ptr [edi - 2]
        test   ax, ax
        je     SHORT loc_5_1454
        jbe    SHORT loc_5_13EE
        mov    ebx, eax
        and    ebx, 0ffffh
loc_5_1436:
        mov    eax, edi
        add    edi, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_5087
        dec    ebx
        jne    SHORT loc_5_1436
        jmp    SHORT loc_5_13EE
loc_5_1454:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_50E3
        and    ax, 0ffbfh
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 8
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_5087
        mov    word ptr [ebp - 2], 1
loc_5_1481:
        xor    ebx, ebx
loc_5_1483:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_50E3
        and    al, 0f0h
        cmp    al, 40h
        je     SHORT loc_5_14AC
        push   0ah
        inc    ebx
        push   esi
        call   sub_5_502C
        cmp    ebx, 12ch
        jb     SHORT loc_5_1483
        jmp    SHORT loc_5_14B2
loc_5_14AC:
        mov    word ptr [ebp - 2], 0
loc_5_14B2:
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_5_14D4
        push   0ff00h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        inc    edi
        push   eax
        push   esi
        call   sub_5_5087
        cmp    edi, 3
        jb     SHORT loc_5_1481
loc_5_14D4:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_14E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0ff00h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_5087
        mov    word ptr [ebp - 2], 1
loc_5_1511:
        xor    ebx, ebx
loc_5_1513:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_50E3
        and    al, 0f0h
        cmp    al, 40h
        je     SHORT loc_5_153C
        push   0ah
        inc    ebx
        push   esi
        call   sub_5_502C
        cmp    ebx, 12ch
        jb     SHORT loc_5_1513
        jmp    SHORT loc_5_1542
loc_5_153C:
        mov    word ptr [ebp - 2], 0
loc_5_1542:
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_5_1564
        push   0ff00h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        inc    edi
        push   eax
        push   esi
        call   sub_5_5087
        cmp    edi, 3
        jb     SHORT loc_5_1511
loc_5_1564:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1578:
        push   ebp
        mov    al, byte ptr [esp + 0ch]
        mov    ebp, esp
        cmp    al, 1
        je     SHORT loc_5_15B5
        cmp    al, 2
        jne    SHORT loc_5_1598
        mov    edx, dword ptr [ebp + 8]
        pop    ebp
        mov    al, byte ptr [edx + 1]
        mov    cl, byte ptr [edx]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx], al
        ret    8
loc_5_1598:
        cmp    al, 4
        jne    SHORT loc_5_15B5
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [edx]
        mov    cl, byte ptr [edx + 3]
        mov    byte ptr [edx], cl
        mov    byte ptr [edx + 3], al
        mov    al, byte ptr [edx + 1]
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx + 2], al
loc_5_15B5:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_15C8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ecx, ecx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [eax], 9fh
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [eax + 3], cl
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [eax + 4], cl
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [eax + 6], cl
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [eax + 7], cl
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [eax + 8], cx
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [eax + 0ah], cx
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [eax + 0ch], 505h
        mov    edx, dword ptr [esi + 88h]
        mov    eax, dword ptr [esi + 0ch]
        push   16h
        mov    dword ptr [eax + 0eh], edx
        mov    ebx, dword ptr [esi + 8ch]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 12h], ebx
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_5_1A18
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0eh
        push   eax
        call   sub_5_1578
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 12h
        push   eax
        call   sub_5_1578
        push   1
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_5_5087
        push   0a00h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 4
        push   eax
        push   esi
        call   sub_5_5087
        mov    ebx, dword ptr [esi + 0ch]
        mov    edi, 0bh
loc_5_1684:
        mov    eax, ebx
        add    ebx, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_5087
        dec    edi
        jne    SHORT loc_5_1684
        push   9080h
        push   esi
        call   sub_5_12E8
loc_5_16AB:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_50E3
        test   al, 70h
        je     SHORT loc_5_16DC
        test   al, 10h
        je     SHORT loc_5_16AB
        mov    ax, 1
loc_5_16C7:
        mov    ecx, dword ptr [esi]
        cmp    word ptr [ecx], 0
        jne    SHORT loc_5_16D5
        cmp    word ptr [ecx + 2], 0e2c1h
loc_5_16D5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_16DC:
        xor    ax, ax
        jmp    SHORT loc_5_16C7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_16E8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    edx, [ecx + 118h]
        mov    eax, dword ptr [ecx + 90h]
        mov    word ptr [edx], 3
        mov    esi, eax
        mov    word ptr [ecx + 11ch], ax
        push   6
        shr    esi, 10h
        push   edx
        mov    word ptr [ecx + 11ah], si
        call   sub_5_1A18
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1728:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 157h], 0
        lea    edi, [esi + 18h]
        mov    ecx, edi
loc_5_1742:
        cmp    edx, 3
        jae    SHORT loc_5_1761
        mov    eax, dword ptr [ecx + 8ch]
        mov    ebx, dword ptr [ecx]
        shr    eax, 10h
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [ecx + 8ch]
        mov    ebx, dword ptr [ecx]
        mov    word ptr [ebx + 2], ax
loc_5_1761:
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        mov    word ptr [eax + 4], 0
        inc    edx
        cmp    edx, 4
        jb     SHORT loc_5_1742
        mov    eax, dword ptr [esi + 0a0h]
        mov    ecx, dword ptr [esi + 24h]
        shr    eax, 10h
        mov    ebx, 4
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0a0h]
        mov    ecx, dword ptr [esi + 24h]
        mov    word ptr [ecx + 2], ax
loc_5_1793:
        mov    eax, dword ptr [edi]
        push   1ah
        push   eax
        add    edi, 4
        call   sub_5_1A18
        dec    ebx
        jne    SHORT loc_5_1793
        lea    ecx, [esi + 118h]
        mov    eax, dword ptr [esi + 0a0h]
        mov    word ptr [ecx], 4
        mov    edx, eax
        mov    word ptr [esi + 11ch], ax
        push   6
        shr    edx, 10h
        push   ecx
        mov    word ptr [esi + 11ah], dx
        call   sub_5_1A18
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_5_17D8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 153h], 0
        lea    edi, [esi + 28h]
        mov    edx, edi
loc_5_17F2:
        cmp    ecx, 9
        jae    SHORT loc_5_1811
        mov    eax, dword ptr [edx + 8ch]
        mov    ebx, dword ptr [edx]
        shr    eax, 10h
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [edx + 8ch]
        mov    ebx, dword ptr [edx]
        mov    word ptr [ebx + 2], ax
loc_5_1811:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    word ptr [eax + 4], 8800h
        mov    eax, dword ptr [edx - 4]
        inc    ecx
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [edx - 4]
        mov    word ptr [eax + 8], 1194h
        mov    eax, dword ptr [edx + 0ach]
        mov    ebx, dword ptr [edx - 4]
        shr    eax, 10h
        mov    word ptr [ebx + 0ah], ax
        mov    eax, dword ptr [edx + 0ach]
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 0ch], ax
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 0eh], 0
        mov    eax, dword ptr [edx + 0ach]
        mov    ebx, dword ptr [edx - 4]
        shr    eax, 10h
        mov    word ptr [ebx + 10h], ax
        mov    eax, dword ptr [edx + 0ach]
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 12h], ax
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 14h], 0
        mov    eax, dword ptr [edx + 0ach]
        mov    ebx, dword ptr [edx - 4]
        shr    eax, 10h
        cmp    ecx, 0ah
        mov    word ptr [ebx + 16h], ax
        mov    eax, dword ptr [edx + 0ach]
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 18h], ax
        jb     NEAR PTR loc_5_17F2
        mov    eax, dword ptr [esi + 0b0h]
        mov    ecx, dword ptr [esi + 4ch]
        shr    eax, 10h
        mov    ebx, 0ah
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0b0h]
        mov    ecx, dword ptr [esi + 4ch]
        mov    word ptr [ecx + 2], ax
loc_5_18C2:
        mov    eax, dword ptr [edi]
        push   1ah
        push   eax
        add    edi, 4
        call   sub_5_1A18
        dec    ebx
        jne    SHORT loc_5_18C2
        lea    ecx, [esi + 118h]
        mov    eax, dword ptr [esi + 0b0h]
        mov    word ptr [ecx], 6
        mov    edx, eax
        mov    word ptr [esi + 11ch], ax
        push   6
        shr    edx, 10h
        push   ecx
        mov    word ptr [esi + 11ah], dx
        call   sub_5_1A18
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_1908:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    edx, [eax + 118h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [edx], 9
        mov    esi, ecx
        mov    word ptr [eax + 11ch], cx
        push   6
        shr    esi, 10h
        push   edx
        mov    word ptr [eax + 11ah], si
        call   sub_5_1A18
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1948:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    edx, [eax + 118h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [edx], 8
        mov    esi, ecx
        mov    word ptr [eax + 11ch], cx
        push   6
        shr    esi, 10h
        push   edx
        mov    word ptr [eax + 11ah], si
        call   sub_5_1A18
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1988:
        push   ebp
        mov    ax, word ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   6
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 11ah], ax
        lea    ecx, [esi + 118h]
        mov    word ptr [ecx], 0dh
        push   ecx
        mov    word ptr [esi + 11ch], 0
        call   sub_5_1A18
        mov    cx, word ptr [esi + 11ah]
        mov    edx, dword ptr [esi + 8]
        mov    word ptr [edx], cx
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_19D8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    edx, [ecx + 118h]
        mov    eax, dword ptr [ecx + 9ch]
        mov    word ptr [edx], 0ah
        mov    esi, eax
        mov    word ptr [ecx + 11ch], ax
        push   6
        shr    esi, 10h
        push   edx
        mov    word ptr [ecx + 11ah], si
        call   sub_5_1A18
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1A18:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, 0fffffffeh
        jbe    SHORT loc_5_1A3E
        shr    esi, 1
        mov    edi, dword ptr [ebp + 8]
loc_5_1A2E:
        mov    eax, edi
        push   2
        add    edi, 2
        push   eax
        call   sub_5_1578
        dec    esi
        jne    SHORT loc_5_1A2E
loc_5_1A3E:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_5_1A48:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_50E3
        test   al, 80h
        jne    SHORT loc_5_1A6B
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_5_1A6B:
        and    ax, 0fh
        mov    word ptr [esi + 185h], ax
        pop    esi
        mov    ax, 1
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1A88:
        push   ebp
        mov    ebp, esp
        push   esi
        push   4
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 13h
        push   eax
        push   esi
        call   sub_5_505C
        push   0
        push   esi
        call   sub_5_12E8
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1AB8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0a080h
        push   eax
        call   sub_5_12E8
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1AD8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 153h]
        mov    edx, dword ptr [ecx + eax*4 + 28h]
        mov    word ptr [edx + 4], 88h
        mov    eax, dword ptr [ecx + 153h]
        inc    eax
        mov    dword ptr [ecx + 153h], eax
        cmp    eax, 0ah
        jb     SHORT loc_5_1B0B
        mov    dword ptr [ecx + 153h], 0
loc_5_1B0B:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_1B18:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 153h]
        mov    eax, dword ptr [esi + eax*4 + 28h]
        test   byte ptr [eax + 4], 80h
        je     SHORT loc_5_1B38
        mov    ax, 0ffffh
        pop    esi
        pop    ebp
        ret    4
loc_5_1B38:
        push   2
        add    eax, 6
        push   eax
        call   sub_5_1578
        mov    ecx, dword ptr [esi + 153h]
        mov    edx, dword ptr [esi + ecx*4 + 28h]
        lea    ecx, [esi + ecx*4]
        mov    dx, word ptr [edx + 6]
        mov    ecx, dword ptr [ecx + 50h]
        mov    ax, dx
        mov    dword ptr [esi + 187h], ecx
        inc    dword ptr [esi + 16fh]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1B78:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   8280h
        push   eax
        call   sub_5_12E8
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1B98:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 157h]
        mov    edx, dword ptr [eax + ecx*4 + 18h]
        pop    ebp
        mov    ax, word ptr [edx + 4]
        and    ax, 80h
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 2
        dec    eax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1BC8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 157h]
        pop    ebp
        mov    edx, dword ptr [ecx + eax*4 + 78h]
        xor    ax, ax
        mov    dword ptr [ecx + 18bh], edx
        mov    word ptr [ecx + 18fh], 0
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1BF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [esp + 10h]
        mov    ebp, esp
        test   dx, dx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 18bh]
        je     SHORT loc_5_1C22
        mov    di, dx
        mov    ecx, dword ptr [ebp + 0ch]
loc_5_1C18:
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [esi], bl
        inc    esi
        dec    di
        jne    SHORT loc_5_1C18
loc_5_1C22:
        add    word ptr [eax + 18fh], dx
        pop    edi
        mov    dword ptr [eax + 18bh], esi
        pop    esi
        xor    ax, ax
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1C48:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 18fh]
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 6], cx
        mov    cx, word ptr [esi + 18fh]
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 8], cx
        mov    eax, dword ptr [esi + 157h]
        mov    ecx, dword ptr [esi + eax*4 + 100h]
        lea    eax, [esi + eax*4]
        shr    ecx, 10h
        mov    eax, dword ptr [eax + 18h]
        mov    word ptr [eax + 0ah], cx
        mov    edx, dword ptr [esi + 157h]
        mov    ecx, dword ptr [esi + edx*4 + 100h]
        lea    eax, [esi + edx*4]
        mov    edx, dword ptr [esi + edx*4 + 18h]
        mov    word ptr [edx + 0ch], cx
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 0eh], cx
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 10h], cx
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 12h], cx
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 14h], cx
        push   2
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 16h], cx
        mov    eax, dword ptr [esi + 157h]
        mov    edx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [edx + 18h], cx
        mov    eax, dword ptr [esi + 157h]
        mov    eax, dword ptr [esi + eax*4 + 18h]
        add    eax, 6
        push   eax
        call   sub_5_1578
        push   2
        mov    ecx, dword ptr [esi + 157h]
        mov    eax, dword ptr [esi + ecx*4 + 18h]
        add    eax, 8
        push   eax
        call   sub_5_1578
        push   2
        mov    ecx, dword ptr [esi + 157h]
        mov    eax, dword ptr [esi + ecx*4 + 18h]
        add    eax, 0ah
        push   eax
        call   sub_5_1578
        push   2
        mov    ecx, dword ptr [esi + 157h]
        mov    eax, dword ptr [esi + ecx*4 + 18h]
        add    eax, 0ch
        push   eax
        call   sub_5_1578
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1D68:
        push   ebp
        mov    ebp, esp
        push   esi
        push   8180h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 157h]
        mov    ecx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [ecx + 4], 0b8h
        call   sub_5_12E8
        mov    eax, dword ptr [esi + 157h]
        inc    eax
        mov    dword ptr [esi + 157h], eax
        cmp    eax, 4
        jb     SHORT loc_5_1DA6
        mov    dword ptr [esi + 157h], 0
loc_5_1DA6:
        xor    ax, ax
        inc    dword ptr [esi + 16bh]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_5_1DB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0ah
        lea    eax, [ebp - 4]
        push   eax
        call   sub_5_518D
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 118h]
        mov    word ptr [edx], ax
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 11ah]
        mov    word ptr [edx + 2], ax
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 11ch]
        mov    word ptr [edx + 4], ax
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 6], 0
        cmp    dword ptr [ecx + 122h], 0
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_5_1E20
        mov    dword ptr [ecx + 122h], eax
        mov    dword ptr [ecx + 11eh], eax
        xor    ax, ax
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1E20:
        mov    edx, dword ptr [ecx + 11eh]
        mov    dword ptr [edx + 6], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 11eh], eax
        mov    esp, ebp
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E48:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 14dh], 1
        jne    SHORT loc_5_1E63
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_1E63:
        mov    word ptr [esi + 14dh], 1
        mov    ecx, dword ptr [esi]
        cmp    word ptr [ecx], 0
        je     SHORT loc_5_1E86
        xor    ax, ax
        pop    edi
        mov    word ptr [esi + 14dh], 0
        pop    esi
        pop    ebp
        ret    4
loc_5_1E86:
        mov    edi, dword ptr [esi + 122h]
        test   edi, edi
        jne    SHORT loc_5_1EA2
        xor    ax, ax
        pop    edi
        mov    word ptr [esi + 14dh], 0
        pop    esi
        pop    ebp
        ret    4
loc_5_1EA2:
        mov    eax, dword ptr [edi + 6]
        mov    dword ptr [esi + 122h], eax
        mov    dx, word ptr [edi]
        mov    word ptr [ecx], dx
        mov    ecx, dword ptr [esi]
        mov    ax, word ptr [edi + 2]
        push   9080h
        mov    word ptr [ecx + 2], ax
        push   esi
        mov    dx, word ptr [edi + 4]
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx + 4], dx
        call   sub_5_12E8
        push   0ah
        push   edi
        call   sub_5_51B7
        mov    word ptr [esi + 14dh], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_5_1EE8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dx, word ptr [eax]
        mov    word ptr [esi + 110h], dx
        mov    cx, word ptr [eax + 2]
        mov    word ptr [esi + 112h], cx
        mov    bx, word ptr [eax + 4]
        mov    word ptr [esi + 114h], bx
        mov    ax, word ptr [eax + 6]
        mov    word ptr [esi + 116h], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 185h]
        cmp    eax, 0ch
        ja     NEAR PTR loc_5_20B0
        xor    ebx, ebx
        db  08Ah, 098h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 09Dh, 000h, 000h, 000h, 000h
loc_5_1F43:
        mov    dword ptr [esi + 12ah], 2
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_1F57:
        test   cl, 8
        je     SHORT loc_5_1F7A
        mov    dword ptr [esi + 126h], 4
        mov    dword ptr [esi + 12ah], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_1F7A:
        test   cl, 4
        je     SHORT loc_5_1F9D
        mov    dword ptr [esi + 126h], 4
        mov    dword ptr [esi + 12ah], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_1F9D:
        test   cl, 1
        je     NEAR PTR loc_5_20B0
        mov    dword ptr [esi + 126h], 4
        mov    dword ptr [esi + 12ah], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_1FC4:
        mov    eax, edx
        and    eax, 0ffffh
        cmp    eax, 400h
        jg     NEAR PTR loc_5_20B0
        je     NEAR PTR loc_5_207E
        cmp    eax, 300h
        jne    NEAR PTR loc_5_20B0
        cmp    cx, 80h
        jne    SHORT loc_5_2040
        mov    dword ptr [esi + 126h], 1
        mov    eax, dword ptr [esi + 17bh]
        push   eax
        push   esi
        call   sub_5_1908
        push   esi
        call   sub_5_1DB8
        mov    eax, dword ptr [esi + 17fh]
        push   eax
        push   esi
        call   sub_5_1948
        push   esi
        call   sub_5_1DB8
        push   esi
        call   sub_5_1728
        push   esi
        call   sub_5_1DB8
        push   esi
        call   sub_5_17D8
        push   esi
        call   sub_5_1DB8
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2040:
        cmp    dword ptr [esi + 126h], 3
        mov    dword ptr [esi + 126h], 4
        push   esi
        jne    SHORT loc_5_2069
        call   sub_5_16E8
        push   esi
        call   sub_5_1DB8
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2069:
        call   sub_5_16E8
        push   esi
        call   sub_5_1DB8
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_207E:
        mov    di, 8
        push   esi
        call   sub_5_1728
        push   esi
        call   sub_5_1DB8
        mov    ax, di
        inc    dword ptr [esi + 173h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_209E:
        mov    di, 1
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_20AC:
        mov    di, 2
loc_5_20B0:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 005h, 001h, 005h
        db  005h, 005h, 002h, 005h, 003h, 005h, 004h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_20E8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 12ah]
        cmp    eax, 2
        jne    SHORT loc_5_2134
        mov    dword ptr [esi + 12ah], 1
        push   esi
        call   sub_5_14E8
        push   esi
        call   sub_5_15C8
        mov    dword ptr [esi + 126h], 4
        push   esi
        call   sub_5_16E8
        push   esi
        call   sub_5_1DB8
        push   esi
        call   sub_5_1E48
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_5_2134:
        cmp    eax, 3
        jne    SHORT loc_5_2155
        mov    dword ptr [esi + 12ah], 1
        push   esi
        call   sub_5_16E8
        push   esi
        call   sub_5_1DB8
        push   esi
        call   sub_5_1E48
loc_5_2155:
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2168:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 150h]
        and    ecx, 0fh
        cmp    ecx, 7
        ja     SHORT loc_5_2184
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_5_2184:
        mov    ax, 1
        pop    ebp
        ret    4
loc_5_218C:
        mov    dword ptr [eax + 13ah], 0a00h
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_219D:
        mov    dword ptr [eax + 13ah], 1a00h
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_21AE:
        mov    dword ptr [eax + 13ah], 2a00h
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_21BF:
        mov    dword ptr [eax + 13ah], 3a00h
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_21D0:
        mov    dword ptr [eax + 13ah], 0a20h
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_21E1:
        mov    dword ptr [eax + 13ah], 1a20h
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_21F2:
        mov    dword ptr [eax + 13ah], 2a20h
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_2203:
        mov    dword ptr [eax + 13ah], 3a20h
        xor    ax, ax
        pop    ebp
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Dh, 064h, 024h, 000h
sub_5_2238:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 151h]
        and    ecx, 3
        cmp    ecx, 3
        ja     SHORT loc_5_2254
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_5_2254:
        mov    ax, 1
        pop    ebp
        ret    4
loc_5_225C:
        mov    dword ptr [eax + 12eh], 3
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_226D:
        mov    dword ptr [eax + 12eh], 7
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_227E:
        mov    dword ptr [eax + 12eh], 9
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_228F:
        mov    dword ptr [eax + 12eh], 0ch
        xor    ax, ax
        pop    ebp
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_22B8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 14fh]
        and    cl, 1eh
        shr    cl, 1
        pop    ebp
        and    ecx, 0ffh
        mov    dword ptr [eax + 142h], ecx
        mov    eax, 0eh
        cmp    eax, ecx
        mov    eax, 1
        adc    eax, -1
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_22F8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 150h]
        shr    cl, 7
        and    ecx, 0ffh
        test   ecx, ecx
        je     SHORT loc_5_231F
        cmp    ecx, 1
        je     SHORT loc_5_2330
        mov    ax, 1
        pop    ebp
        ret    4
loc_5_231F:
        mov    dword ptr [eax + 136h], 0
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_2330:
        mov    dword ptr [eax + 136h], 1
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2348:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 150h]
        and    cl, 40h
        shr    cl, 6
        and    ecx, 0ffh
        test   ecx, ecx
        je     SHORT loc_5_2372
        cmp    ecx, 1
        je     SHORT loc_5_2383
        mov    ax, 1
        pop    ebp
        ret    4
loc_5_2372:
        mov    dword ptr [eax + 13eh], 0
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_2383:
        mov    dword ptr [eax + 13eh], 1
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    bl, byte ptr [esi + 152h]
        and    bl, 0f8h
loc_5_23AC:
        lea    eax, [ebx + edi]
        inc    edi
        push   eax
        push   105h
        push   esi
        call   sub_5_505C
        push   104h
        push   esi
        call   sub_5_50B8
        mov    byte ptr [edi + esi + 15ah], al
        cmp    edi, 6
        mov    byte ptr [edi + esi + 160h], al
        jb     SHORT loc_5_23AC
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 14bh]
        add    al, 8
        push   eax
        push   96h
        push   esi
        call   sub_5_505C
        push   101h
        push   esi
        call   sub_5_50B8
        test   al, al
        jne    SHORT loc_5_2439
        push   100h
        push   esi
        call   sub_5_50B8
        cmp    al, 50h
        jne    SHORT loc_5_2439
        mov    ax, word ptr [esi + 14bh]
        add    al, 8
        mov    byte ptr [esi + 14ah], al
        pop    esi
        xor    ax, ax
        pop    ebp
        ret    4
loc_5_2439:
        push   0
        push   96h
        push   esi
        call   sub_5_505C
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2458:
        push   ebp
        mov    ebp, esp
        push   esi
        push   102h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_50B8
        or     al, 1
        push   eax
        push   102h
        push   esi
        call   sub_5_505C
        push   90h
        push   105h
        push   esi
        call   sub_5_505C
        push   30h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 10h
        push   eax
        push   esi
        call   sub_5_505C
        push   11h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 11h
        push   eax
        push   esi
        call   sub_5_505C
        push   4
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 12h
        push   eax
        push   esi
        call   sub_5_505C
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_5_24C8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_2168
        test   ax, ax
        jne    SHORT loc_5_251E
        push   esi
        call   sub_5_2238
        test   ax, ax
        jne    SHORT loc_5_251E
        push   esi
        call   sub_5_22B8
        test   ax, ax
        jne    SHORT loc_5_251E
        push   esi
        call   sub_5_22F8
        test   ax, ax
        jne    SHORT loc_5_251E
        push   esi
        call   sub_5_2348
        test   ax, ax
        jne    SHORT loc_5_251E
        push   esi
        call   sub_5_2458
        cmp    ax, 1
        mov    cx, ax
        mov    eax, 0
        adc    eax, -1
        and    ax, cx
loc_5_251E:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_5_2528:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 103h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2554:
        cmp    dword ptr [ebp - 8], 0eh
        jb     NEAR PTR loc_5_2570
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1f3h], ecx
        jae    NEAR PTR loc_5_257A
loc_5_2570:
        mov    eax, 0c001000fh
        jmp    NEAR PTR loc_5_26CA
loc_5_257A:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_261E
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_2612
        mov    eax, dword ptr [ebp - 10h]
        inc    dword ptr [eax + 14h]
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25C5:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_5_25E1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 1f3h], ecx
        jae    NEAR PTR loc_5_25ED
loc_5_25E1:
        mov    dword ptr [ebp - 0ch], 0c000009ah
        jmp    NEAR PTR loc_5_2607
loc_5_25ED:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_26D1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_280B
loc_5_2607:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_2619
loc_5_2612:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_2619:
        jmp    NEAR PTR loc_5_2676
loc_5_261E:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_263D
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_5_2676
loc_5_263D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_265C
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_2676
loc_5_265C:
        push   2
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2673:
        add    esp, 14h
loc_5_2676:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_26C2
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_26B4
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 18h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_26C2
loc_5_26B4:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_26C2
loc_5_26C2:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_26CA
loc_5_26CA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_26D1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 80h]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 62h], 0
        jne    NEAR PTR loc_5_2719
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 62h], eax
        jmp    NEAR PTR loc_5_2725
loc_5_2719:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 66h]
        mov    dword ptr [ecx + 24h], eax
loc_5_2725:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 66h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_2744:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1BC8
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2775:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_5_27BE
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2790:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1BF8
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ebp - 18h], eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_27B9:
        jmp    NEAR PTR loc_5_2775
loc_5_27BE:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1C48
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1D68
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 14h], eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_27FE:
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_280B:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 255h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_5_2830
        jmp    NEAR PTR loc_5_298B
loc_5_2830:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 1
        jne    NEAR PTR loc_5_28F8
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_2859
loc_5_2856:
        inc    dword ptr [ebp - 4]
loc_5_2859:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_28F3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1B98
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        cmp    eax, -1
        jne    NEAR PTR loc_5_2891
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
        jmp    NEAR PTR loc_5_298B
loc_5_2891:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 62h], 0
        je     NEAR PTR loc_5_28DF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 62h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 24h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 62h], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 80h]
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 5eh], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2744
        jmp    NEAR PTR loc_5_28EE
loc_5_28DF:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
        jmp    NEAR PTR loc_5_298B
loc_5_28EE:
        jmp    NEAR PTR loc_5_2856
loc_5_28F3:
        jmp    NEAR PTR loc_5_2981
loc_5_28F8:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_2907
loc_5_2904:
        inc    dword ptr [ebp - 4]
loc_5_2907:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_2981
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 62h], 0
        je     NEAR PTR loc_5_296D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 62h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 24h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 62h], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 80h]
        push   0c0000001h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2962:
        mov    eax, dword ptr [ebp - 1ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_297C
loc_5_296D:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
        jmp    NEAR PTR loc_5_298B
loc_5_297C:
        jmp    NEAR PTR loc_5_2904
loc_5_2981:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
loc_5_298B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_29BC
        jmp    NEAR PTR loc_5_2AD1
loc_5_29BC:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_29D1:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_5_29E0
        jmp    NEAR PTR loc_5_2AD1
loc_5_29E0:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_29F1:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 10h], eax
        jbe    NEAR PTR loc_5_2AC9
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_5_2A39
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2A14:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_5_2A23
        jmp    NEAR PTR loc_5_2AC9
loc_5_2A23:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2A34:
        jmp    NEAR PTR loc_5_29F1
loc_5_2A39:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_2A81
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_2A6A
        xor    eax, eax
        sub    eax, dword ptr [ebp - 10h]
        neg    eax
        sub    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_5_29F1
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2A6A:
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 8], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0ch]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp + 0ch], 0
loc_5_2A81:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2A94
        mov    eax, ecx
loc_5_2A94:
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2AA8:
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 18h]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_29F1
loc_5_2AC9:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_5_2AD1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_5_2AD8:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_5_2B00
        jmp    NEAR PTR loc_5_2C21
loc_5_2B00:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2B15:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_2B24
        jmp    NEAR PTR loc_5_2C21
loc_5_2B24:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2B35:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
loc_5_2B3B:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_2C19
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_2B83
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2B5E:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_5_2B6D
        jmp    NEAR PTR loc_5_2C19
loc_5_2B6D:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2B7E:
        jmp    NEAR PTR loc_5_2B3B
loc_5_2B83:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_5_2BCB
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 14h], eax
        jbe    NEAR PTR loc_5_2BB4
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0ch]
        neg    eax
        sub    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_2B3B
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2BB4:
        mov    eax, dword ptr [ebp + 14h]
        add    dword ptr [ebp - 14h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 14h]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp + 14h], 0
loc_5_2BCB:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2BEA
        mov    eax, ecx
loc_5_2BEA:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2BFE:
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 10h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 1ch]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_2B3B
loc_5_2C19:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_5_2C21:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_2C50
        jmp    NEAR PTR loc_5_2E41
loc_5_2C50:
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C65:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_2C74
        jmp    NEAR PTR loc_5_2E41
loc_5_2C74:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C85:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C9A:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_5_2CA9
        jmp    NEAR PTR loc_5_2E41
loc_5_2CA9:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2CBA:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 10h], eax
        jbe    NEAR PTR loc_5_2E39
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_5_2D02
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2CDD:
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_5_2CEC
        jmp    NEAR PTR loc_5_2E39
loc_5_2CEC:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2CFD:
        jmp    NEAR PTR loc_5_2CBA
loc_5_2D02:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_5_2D3E
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2D19:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_2D28
        jmp    NEAR PTR loc_5_2E39
loc_5_2D28:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2D39:
        jmp    NEAR PTR loc_5_2CBA
loc_5_2D3E:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_2D86
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_2D6F
        xor    eax, eax
        sub    eax, dword ptr [ebp - 10h]
        neg    eax
        sub    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_5_2CBA
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2D6F:
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 1ch], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0ch]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp + 0ch], 0
loc_5_2D86:
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_5_2DCE
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 18h], eax
        jbe    NEAR PTR loc_5_2DB7
        xor    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        sub    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_2CBA
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2DB7:
        mov    eax, dword ptr [ebp + 18h]
        add    dword ptr [ebp - 18h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 18h]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp + 18h], 0
loc_5_2DCE:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2DE7
        mov    eax, ecx
loc_5_2DE7:
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 28h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2DFA
        mov    eax, ecx
loc_5_2DFA:
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2E0E:
        mov    eax, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 14h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 28h]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 28h]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_2CBA
loc_5_2E39:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
loc_5_2E41:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
loc_5_2E48:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_1A48
        mov    word ptr [ebp - 8], ax
        movsx  ecx, word ptr [ebp - 8]
        test   ecx, ecx
        jne    NEAR PTR loc_5_2E7C
        xor    al, al
        jmp    NEAR PTR loc_5_2E91
loc_5_2E7C:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_1A88
        mov    al, 1
        jmp    NEAR PTR loc_5_2E91
loc_5_2E91:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_2E98:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1EE8
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_2EFB
loc_5_2ECF:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_32E0
        jmp    NEAR PTR loc_5_2F45
loc_5_2EDD:
        jmp    NEAR PTR loc_5_2F45
loc_5_2EE2:
        jmp    NEAR PTR loc_5_2F45
loc_5_2EE7:
        jmp    NEAR PTR loc_5_2F45
loc_5_2EEC:
        jmp    NEAR PTR loc_5_2F45
loc_5_2EF1:
        jmp    NEAR PTR loc_5_2F45
        db  0E9h, 04Ah, 000h, 000h, 000h
loc_5_2EFB:
        dec    dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 18h], 0fh
        ja     NEAR PTR loc_5_2F45
        mov    eax, dword ptr [ebp - 18h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 002h, 005h, 005h, 005h, 003h
        db  005h, 005h, 005h, 005h, 005h, 005h, 005h, 004h
loc_5_2F45:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_280B
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1E48
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_2FA8
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_2F9A
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_2FA8
loc_5_2F9A:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_2FA8
loc_5_2FA8:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1AB8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_2FBD:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_2FFF
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 46h], 0
        jne    NEAR PTR loc_5_2FFF
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 46h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0EEB
loc_5_2FFF:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_31AC
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 3eh
        cmp    dword ptr [eax + 3eh], ecx
        je     NEAR PTR loc_5_31AC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3eh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3048:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_3119
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_5_30C1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1eh]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_309E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_35D5
loc_5_309E:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_30AC:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_30BC:
        jmp    NEAR PTR loc_5_3114
loc_5_30C1:
        mov    eax, dword ptr [ebp - 4]
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
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2eh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 32h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 32h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 32h], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 14h], 2
loc_5_3114:
        jmp    NEAR PTR loc_5_31AC
loc_5_3119:
        cmp    dword ptr [ebp - 8], 103h
        jne    NEAR PTR loc_5_318E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_280B
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 4]
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
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2eh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 32h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 32h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 32h], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 14h], 2
        jmp    NEAR PTR loc_5_31AC
loc_5_318E:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0ah
        push   7
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_31A9:
        add    esp, 18h
loc_5_31AC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_322E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2eh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_5_322E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1eh]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_31EF
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_35D5
loc_5_31EF:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_31FD:
        mov    eax, dword ptr [ebp - 4]
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
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_322E:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 2ah]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_323B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1ch]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_5_3263
        jmp    NEAR PTR loc_5_32D9
loc_5_3263:
        push   3e8h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3276:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 248h]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 248h], 1
        jae    NEAR PTR loc_5_32B8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 248h], 0ah
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_19D8
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_1DB8
loc_5_32B8:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_20E8
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_1E48
        jmp    NEAR PTR loc_5_32D9
loc_5_32D9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_32E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_32FC
loc_5_32F9:
        inc    dword ptr [ebp - 8]
loc_5_32FC:
        cmp    dword ptr [ebp - 8], 0ah
        jae    NEAR PTR loc_5_3402
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1B18
        movsx  eax, ax
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], -1
        jne    NEAR PTR loc_5_3329
        jmp    NEAR PTR loc_5_3402
loc_5_3329:
        mov    byte ptr [ebp - 0ch], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 213h]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 14h], 0eh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1eh]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_5_3370
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_5_3377
loc_5_3370:
        mov    dword ptr [ebp - 14h], 0eh
loc_5_3377:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 18h], eax
        jae    NEAR PTR loc_5_33B9
        cmp    dword ptr [ebp - 18h], 6
        jb     NEAR PTR loc_5_33B4
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_33B4:
        jmp    NEAR PTR loc_5_33EF
loc_5_33B9:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        sub    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_33EF:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1AD8
        jmp    NEAR PTR loc_5_32F9
loc_5_3402:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 1
        jne    NEAR PTR loc_5_342A
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_341C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1B78
loc_5_342A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h
loc_5_3434:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        je     NEAR PTR loc_5_3474
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_36AB
        mov    dword ptr [ebp - 4], eax
loc_5_3474:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_347C
loc_5_347C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_5_3483:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 26h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        je     NEAR PTR loc_5_34C3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_3756
        mov    dword ptr [ebp - 4], eax
loc_5_34C3:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_34CB
loc_5_34CB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_5_34D2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 72h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 76h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 72h], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_36AB
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_37C6
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_351F
loc_5_351F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_5_3526:
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
sub_5_3539:
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
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_356F:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_3580
loc_5_3580:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_3587:
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
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_35BD:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_35CE
loc_5_35CE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_35D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 36h]
        mov    dword ptr [ebp - 4], eax
loc_5_35EE:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_5_3628
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_5_361B
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
loc_5_361B:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_35EE
loc_5_3628:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_3639
        mov    dword ptr [ebp - 8], 0f0h
loc_5_3639:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1eh], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_3649:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 14h], 8056h
        je     NEAR PTR loc_5_3670
        mov    eax, 0c00000bbh
        jmp    NEAR PTR loc_5_36A4
loc_5_3670:
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
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3693:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_36A4
loc_5_36A4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_36AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 7fffffffh
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 73h], 20h
        je     NEAR PTR loc_5_36D3
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_36F5
loc_5_36D3:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 73h], 40h
        je     NEAR PTR loc_5_36EE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 22h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_36F5
loc_5_36EE:
        mov    dword ptr [ebp - 4], 0
loc_5_36F5:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 207h], ecx
        jne    NEAR PTR loc_5_370E
        xor    eax, eax
        jmp    NEAR PTR loc_5_374F
loc_5_370E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 207h], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1908
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1DB8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1E48
        xor    eax, eax
        jmp    NEAR PTR loc_5_374F
loc_5_374F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_3756:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 20bh]
        cmp    dword ptr [eax + 26h], ecx
        jne    NEAR PTR loc_5_3778
        xor    eax, eax
        jmp    NEAR PTR loc_5_37BF
loc_5_3778:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 26h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20bh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 26h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1948
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1DB8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1E48
        xor    eax, eax
        jmp    NEAR PTR loc_5_37BF
loc_5_37BF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_37C6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 72h], 20h
        je     NEAR PTR loc_5_37F1
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, 2
        mov    word ptr [ebp - 4], ax
loc_5_37F1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_5_3818
        xor    eax, eax
        jmp    NEAR PTR loc_5_385B
loc_5_3818:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 20fh], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1988
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1DB8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1E48
        xor    eax, eax
        jmp    NEAR PTR loc_5_385B
loc_5_385B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h
loc_5_3864:
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
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_5_38AF
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_3F90
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_396B
loc_5_38AF:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 1
        jne    NEAR PTR loc_5_3964
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_3907
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_38FB
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_400A
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_3902
loc_5_38FB:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_3902:
        jmp    NEAR PTR loc_5_395F
loc_5_3907:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3926
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_5_395F
loc_5_3926:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3945
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_395F
loc_5_3945:
        push   3
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_395C:
        add    esp, 14h
loc_5_395F:
        jmp    NEAR PTR loc_5_396B
loc_5_3964:
        mov    dword ptr [ebp - 0ch], 10001h
loc_5_396B:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_39B7
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_39A9
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_39B7
loc_5_39A9:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_39B7
loc_5_39B7:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_39BF
loc_5_39BF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_39C6:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 20h], 1
        mov    dword ptr [ebp - 10h], 0
        mov    word ptr [ebp - 28h], 0
        mov    dword ptr [ebp - 24h], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 44h], eax
        jmp    NEAR PTR loc_5_3E98
loc_5_39FC:
        mov    dword ptr [ebp - 10h], 6
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3A15:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_5_3A35
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 5ch
        jmp    NEAR PTR loc_5_3A43
loc_5_3A35:
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 0a0h
loc_5_3A43:
        jmp    NEAR PTR loc_5_3F3E
loc_5_3A48:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3A67
        mov    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_5_3AAC
loc_5_3A67:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3A86
        mov    dword ptr [ebp - 8], 1
        jmp    NEAR PTR loc_5_3AAC
loc_5_3A86:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3AA5
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_5_3AAC
loc_5_3AA5:
        mov    dword ptr [ebp - 8], 0
loc_5_3AAC:
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3ABE:
        lea    eax, [ebp - 20h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3AD0:
        mov    dword ptr [ebp - 10h], 0f0h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3AE9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp + 10h], 10106h
        jne    NEAR PTR loc_5_3B06
        sub    dword ptr [ebp - 10h], 0eh
loc_5_3B06:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3B18:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1cah], 1
        jne    NEAR PTR loc_5_3B34
        mov    dword ptr [ebp - 10h], 9c40h
        jmp    NEAR PTR loc_5_3B3B
loc_5_3B34:
        mov    dword ptr [ebp - 10h], 27100h
loc_5_3B3B:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3B4D:
        mov    dword ptr [ebp - 10h], 4650h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3B66:
        mov    dword ptr [ebp - 10h], 0afc8h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3B7F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3B9D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3BBB:
        push   3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1e7h
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3BCF:
        mov    byte ptr [ebp - 9], 0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3BE5:
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 2fh
        jmp    NEAR PTR loc_5_3F3E
loc_5_3BF8:
        mov    word ptr [ebp - 28h], 300h
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_5_3F3E
loc_5_3C10:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_5_3C2B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 72h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_3C37
loc_5_3C2B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 10h], eax
loc_5_3C37:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3C49:
        mov    dword ptr [ebp - 10h], 0
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3C62:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_5_3C7D
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_3CD9
loc_5_3C7D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 36h]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 10h], 0
loc_5_3C8D:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 2ch]
        je     NEAR PTR loc_5_3CD9
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_5_3CCC
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0f0h
        jne    NEAR PTR loc_5_3CCC
        jmp    NEAR PTR loc_5_3CD9
loc_5_3CCC:
        mov    eax, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_5_3C8D
loc_5_3CD9:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3CEB:
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1e7h
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 6
        jmp    NEAR PTR loc_5_3F3E
loc_5_3D54:
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1edh
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 6
        jmp    NEAR PTR loc_5_3F3E
loc_5_3DBD:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_5_3DDB
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_3DE7
loc_5_3DDB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 207h]
        mov    dword ptr [ebp - 10h], eax
loc_5_3DE7:
        mov    eax, dword ptr [ebp - 10h]
        shr    eax, 18h
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 10h]
        shl    ecx, 8
        and    ecx, 0ff0000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3E31:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20bh]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        shr    eax, 18h
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 10h]
        shl    ecx, 8
        and    ecx, 0ff0000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3F3E
loc_5_3E87:
        mov    dword ptr [ebp - 24h], 0c00000bbh
        jmp    NEAR PTR loc_5_3F3E
        db  0E9h, 0A6h, 000h, 000h, 000h
loc_5_3E98:
        cmp    dword ptr [ebp - 44h], 2010101h
        ja     NEAR PTR loc_5_3F12
        je     NEAR PTR loc_5_3CEB
        sub    dword ptr [ebp - 44h], 10101h
        cmp    dword ptr [ebp - 44h], 12h
        ja     NEAR PTR loc_5_3E87
        mov    eax, dword ptr [ebp - 44h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_3F12:
        cmp    dword ptr [ebp - 44h], 2010102h
        je     NEAR PTR loc_5_3D54
        cmp    dword ptr [ebp - 44h], 2010103h
        je     NEAR PTR loc_5_3DBD
        cmp    dword ptr [ebp - 44h], 2010104h
        je     NEAR PTR loc_5_3E31
        jmp    NEAR PTR loc_5_3E87
loc_5_3F3E:
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_5_3F81
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 1ch], eax
        jae    NEAR PTR loc_5_3F68
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 24h], 0c0010014h
        jmp    NEAR PTR loc_5_3F81
loc_5_3F68:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3F79:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], eax
loc_5_3F81:
        mov    eax, dword ptr [ebp - 24h]
        jmp    NEAR PTR loc_5_3F89
loc_5_3F89:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    20h
sub_5_3F90:
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
        call   sub_5_39C6
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_5_4003
loc_5_4003:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_400A:
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
        je     NEAR PTR loc_5_406F
        mov    dword ptr [ebp - 18h], 0c0010014h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_5_4214
loc_5_406F:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_5_419E
loc_5_407A:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3539
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_41EF
loc_5_4097:
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_40A6:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3649
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_41EF
loc_5_40C3:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3587
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_41EF
        db  0E9h, 00Ah, 001h, 000h, 000h
loc_5_40E5:
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_40F4:
        cmp    dword ptr [ebp - 1ch], 0f0h
        ja     NEAR PTR loc_5_416D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 1eh], ecx
        jae    NEAR PTR loc_5_4127
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1eh], eax
        jmp    NEAR PTR loc_5_4168
loc_5_4127:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1eh]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_415F
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_5_415F
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_35D5
        jmp    NEAR PTR loc_5_4168
loc_5_415F:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
loc_5_4168:
        jmp    NEAR PTR loc_5_4174
loc_5_416D:
        mov    dword ptr [ebp - 18h], 0c0010014h
loc_5_4174:
        jmp    NEAR PTR loc_5_41EF
loc_5_4179:
        mov    dword ptr [ebp - 18h], 0c0010017h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        jmp    NEAR PTR loc_5_41EF
        db  0E9h, 051h, 000h, 000h, 000h
loc_5_419E:
        cmp    dword ptr [ebp - 24h], 2010103h
        ja     NEAR PTR loc_5_41DD
        je     NEAR PTR loc_5_407A
        cmp    dword ptr [ebp - 24h], 1010eh
        je     NEAR PTR loc_5_4097
        cmp    dword ptr [ebp - 24h], 1010fh
        je     NEAR PTR loc_5_40E5
        cmp    dword ptr [ebp - 24h], 10112h
        je     NEAR PTR loc_5_41EF
        jmp    NEAR PTR loc_5_4179
loc_5_41DD:
        cmp    dword ptr [ebp - 24h], 2010104h
        je     NEAR PTR loc_5_40C3
        jmp    NEAR PTR loc_5_4179
loc_5_41EF:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_5_420C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
loc_5_420C:
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_5_4214
loc_5_4214:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_421B:
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
        mov    dword ptr [ebp - 0ch], 0
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
        call   sub_5_39C6
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0c00000bbh
        jne    NEAR PTR loc_5_45BE
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_5_44B4
loc_5_42A2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f7h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_42B3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1fbh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_42C4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ffh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_42D5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 203h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_42E6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_42FF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_4317:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_4330:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 1
        jne    NEAR PTR loc_5_434C
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4353
loc_5_434C:
        mov    dword ptr [ebp - 0ch], 0c0010007h
loc_5_4353:
        jmp    NEAR PTR loc_5_4580
loc_5_4358:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b6h], 1
        jne    NEAR PTR loc_5_4374
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_43A7
loc_5_4374:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b6h], 2
        jne    NEAR PTR loc_5_4390
        mov    dword ptr [ebp - 0ch], 8000h
        jmp    NEAR PTR loc_5_43A7
loc_5_4390:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b6h], 3
        jne    NEAR PTR loc_5_43A7
        mov    dword ptr [ebp - 0ch], 4000h
loc_5_43A7:
        jmp    NEAR PTR loc_5_4580
loc_5_43AC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 1
        jne    NEAR PTR loc_5_43C8
        mov    dword ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_5_4423
loc_5_43C8:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 3
        jne    NEAR PTR loc_5_43E4
        mov    dword ptr [ebp - 0ch], 3
        jmp    NEAR PTR loc_5_4423
loc_5_43E4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 4
        jne    NEAR PTR loc_5_4400
        mov    dword ptr [ebp - 0ch], 3
        jmp    NEAR PTR loc_5_4423
loc_5_4400:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 5
        jne    NEAR PTR loc_5_441C
        mov    dword ptr [ebp - 0ch], 5
        jmp    NEAR PTR loc_5_4423
loc_5_441C:
        mov    dword ptr [ebp - 0ch], 2
loc_5_4423:
        jmp    NEAR PTR loc_5_4580
loc_5_4428:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_4441:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4580
loc_5_444D:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4580
loc_5_4459:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_4472:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4580
loc_5_447E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4580
loc_5_4497:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4580
loc_5_44A3:
        mov    dword ptr [ebp - 18h], 0c0010017h
        jmp    NEAR PTR loc_5_4580
        db  0E9h, 0CCh, 000h, 000h, 000h
loc_5_44B4:
        cmp    dword ptr [ebp - 20h], 2010105h
        ja     NEAR PTR loc_5_44F6
        je     NEAR PTR loc_5_4330
        sub    dword ptr [ebp - 20h], 20101h
        cmp    dword ptr [ebp - 20h], 4
        ja     NEAR PTR loc_5_44A3
        mov    eax, dword ptr [ebp - 20h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_5_44F6:
        cmp    dword ptr [ebp - 20h], 2020101h
        ja     NEAR PTR loc_5_4528
        je     NEAR PTR loc_5_42FF
        cmp    dword ptr [ebp - 20h], 2010106h
        je     NEAR PTR loc_5_4358
        cmp    dword ptr [ebp - 20h], 2010107h
        je     NEAR PTR loc_5_43AC
        jmp    NEAR PTR loc_5_44A3
loc_5_4528:
        cmp    dword ptr [ebp - 20h], 2020201h
        ja     NEAR PTR loc_5_454D
        je     NEAR PTR loc_5_4428
        cmp    dword ptr [ebp - 20h], 2020102h
        je     NEAR PTR loc_5_4317
        jmp    NEAR PTR loc_5_44A3
loc_5_454D:
        sub    dword ptr [ebp - 20h], 2020202h
        cmp    dword ptr [ebp - 20h], 5
        ja     NEAR PTR loc_5_44A3
        mov    eax, dword ptr [ebp - 20h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_4580:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_5_45BE
        cmp    dword ptr [ebp - 8], 4
        jb     NEAR PTR loc_5_45AC
        push   4
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_45A3:
        add    dword ptr [ebp - 14h], 4
        jmp    NEAR PTR loc_5_45BE
loc_5_45AC:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], 0c0010014h
loc_5_45BE:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_5_45D8
loc_5_45D8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_5_45DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_462B
        db  0E9h, 0E0h, 000h, 000h, 000h
loc_5_460E:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_46EE
loc_5_461A:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    NEAR PTR loc_5_46EE
        db  0E9h, 0C3h, 000h, 000h, 000h
loc_5_462B:
        cmp    dword ptr [ebp - 10h], 10113h
        ja     NEAR PTR loc_5_465D
        je     NEAR PTR loc_5_46EE
        cmp    dword ptr [ebp - 10h], 10101h
        jb     NEAR PTR loc_5_461A
        cmp    dword ptr [ebp - 10h], 10111h
        jbe    NEAR PTR loc_5_46EE
        jmp    NEAR PTR loc_5_461A
loc_5_465D:
        cmp    dword ptr [ebp - 10h], 2010103h
        ja     NEAR PTR loc_5_4696
        cmp    dword ptr [ebp - 10h], 2010101h
        jae    NEAR PTR loc_5_46EE
        cmp    dword ptr [ebp - 10h], 20101h
        jb     NEAR PTR loc_5_461A
        cmp    dword ptr [ebp - 10h], 20105h
        jbe    NEAR PTR loc_5_460E
        jmp    NEAR PTR loc_5_461A
loc_5_4696:
        cmp    dword ptr [ebp - 10h], 2020102h
        ja     NEAR PTR loc_5_46CF
        cmp    dword ptr [ebp - 10h], 2020101h
        jae    NEAR PTR loc_5_460E
        cmp    dword ptr [ebp - 10h], 2010106h
        jb     NEAR PTR loc_5_461A
        cmp    dword ptr [ebp - 10h], 2010107h
        jbe    NEAR PTR loc_5_460E
        jmp    NEAR PTR loc_5_461A
loc_5_46CF:
        cmp    dword ptr [ebp - 10h], 2020201h
        jb     NEAR PTR loc_5_461A
        cmp    dword ptr [ebp - 10h], 2020207h
        jbe    NEAR PTR loc_5_460E
        jmp    NEAR PTR loc_5_461A
loc_5_46EE:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_4708
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_421B
        mov    dword ptr [ebp - 8], eax
loc_5_4708:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_4754
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_4746
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_4754
loc_5_4746:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_4754
loc_5_4754:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_475C
loc_5_475C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  000h
loc_5_4764:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_47EF
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_47E3
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_5_2AD8
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_47EA
loc_5_47E3:
        mov    dword ptr [ebp - 8], 0c001000ch
loc_5_47EA:
        jmp    NEAR PTR loc_5_4847
loc_5_47EF:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_480E
        mov    dword ptr [ebp - 8], 0c001000dh
        jmp    NEAR PTR loc_5_4847
loc_5_480E:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_482D
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_5_4847
loc_5_482D:
        push   1
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4844:
        add    esp, 14h
loc_5_4847:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_4893
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_4885
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_4893
loc_5_4885:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_2FBD
        jmp    NEAR PTR loc_5_4893
loc_5_4893:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_489B
loc_5_489B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
        db  000h, 000h
sub_5_48A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 256h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        push   0
        push   106h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_48D2:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 8ch], 0
        jne    NEAR PTR loc_5_48E9
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_48E9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8ch]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 256h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 114h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 25ah
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 90h
        push   eax
        push   0
        push   108h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4935:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 90h], 0
        jne    NEAR PTR loc_5_494C
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_494C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 90h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 25ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 118h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 25eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 94h
        push   eax
        push   0
        push   20h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4996:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 94h], 0
        jne    NEAR PTR loc_5_49AD
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_49AD:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 94h]
        xor    eax, eax
        mov    ecx, 8
        rep stosd
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 25eh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 11ch], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 262h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 98h
        push   eax
        push   0
        push   16h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_49F3:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 98h], 0
        jne    NEAR PTR loc_5_4A0A
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_4A0A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 98h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [eax + 10h], 0
        mov    word ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 262h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 120h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 266h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 9ch
        push   eax
        push   0
        push   12h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4A6F:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 9ch], 0
        jne    NEAR PTR loc_5_4A86
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_4A86:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 9ch]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    word ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 266h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 124h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 26ah
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0a0h
        push   eax
        push   0
        push   0eh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4AE4:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0a0h], 0
        jne    NEAR PTR loc_5_4AFB
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_4AFB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 26ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 128h], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4B3F
loc_5_4B3C:
        inc    dword ptr [ebp - 4]
loc_5_4B3F:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4BC9
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 26eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0a4h
        push   eax
        push   0
        push   1ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4B77:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0a4h], 0
        jne    NEAR PTR loc_5_4B92
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_4B92:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 0a4h]
        xor    eax, eax
        mov    ecx, 6
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 26eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 12ch], eax
        jmp    NEAR PTR loc_5_4B3C
loc_5_4BC9:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4BD8
loc_5_4BD5:
        inc    dword ptr [ebp - 4]
loc_5_4BD8:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4C62
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 27eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0b4h
        push   eax
        push   0
        push   1ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4C10:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0b4h], 0
        jne    NEAR PTR loc_5_4C2B
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_4C2B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 0b4h]
        xor    eax, eax
        mov    ecx, 6
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 27eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 13ch], eax
        jmp    NEAR PTR loc_5_4BD5
loc_5_4C62:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4C71
loc_5_4C6E:
        inc    dword ptr [ebp - 4]
loc_5_4C71:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4CFC
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 2a6h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0dch
        push   eax
        push   0
        push   1194h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4CAC:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0dch], 0
        jne    NEAR PTR loc_5_4CC7
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_4CC7:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 0dch]
        xor    eax, eax
        mov    ecx, 465h
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 2a6h]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 164h], eax
        jmp    NEAR PTR loc_5_4C6E
loc_5_4CFC:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4D0B
loc_5_4D08:
        inc    dword ptr [ebp - 4]
loc_5_4D0B:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4D96
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 2ceh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 104h
        push   eax
        push   0
        push   1194h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4D46:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 104h], 0
        jne    NEAR PTR loc_5_4D61
        xor    al, al
        jmp    NEAR PTR loc_5_4D9D
loc_5_4D61:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 104h]
        xor    eax, eax
        mov    ecx, 465h
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 2ceh]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 18ch], eax
        jmp    NEAR PTR loc_5_4D08
loc_5_4D96:
        mov    al, 1
        jmp    NEAR PTR loc_5_4D9D
loc_5_4D9D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_4DA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 256h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8ch]
        push   eax
        push   0
        push   106h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4DD4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 25ah]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 90h]
        push   eax
        push   0
        push   108h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4DFB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 25eh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 94h]
        push   eax
        push   0
        push   20h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4E1F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 262h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 98h]
        push   eax
        push   0
        push   16h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4E43:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 266h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        push   0
        push   12h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4E67:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 26ah]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        push   eax
        push   0
        push   0eh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4E8B:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4E9A
loc_5_4E97:
        inc    dword ptr [ebp - 4]
loc_5_4E9A:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4ED5
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 26eh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0a4h]
        push   eax
        push   0
        push   1ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4ED0:
        jmp    NEAR PTR loc_5_4E97
loc_5_4ED5:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4EE4
loc_5_4EE1:
        inc    dword ptr [ebp - 4]
loc_5_4EE4:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4F1F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 27eh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0b4h]
        push   eax
        push   0
        push   1ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4F1A:
        jmp    NEAR PTR loc_5_4EE1
loc_5_4F1F:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4F2E
loc_5_4F2B:
        inc    dword ptr [ebp - 4]
loc_5_4F2E:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4F6C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 2a6h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0dch]
        push   eax
        push   0
        push   1194h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4F67:
        jmp    NEAR PTR loc_5_4F2B
loc_5_4F6C:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4F7B
loc_5_4F78:
        inc    dword ptr [ebp - 4]
loc_5_4F7B:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4FB9
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 2ceh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 104h]
        push   eax
        push   0
        push   1194h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4FB4:
        jmp    NEAR PTR loc_5_4F78
loc_5_4FB9:
        mov    al, 1
        jmp    NEAR PTR loc_5_4FC0
loc_5_4FC0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        add    dword ptr [ebp - 4], 0ffh
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffff00h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_4FF0
loc_5_4FF0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_5010
loc_5_500D:
        inc    dword ptr [ebp - 4]
loc_5_5010:
        cmp    dword ptr [ebp - 4], 28h
        jae    NEAR PTR loc_5_501F
        jmp    NEAR PTR loc_5_500D
loc_5_501F:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_5027
loc_5_5027:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_5_502C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 3
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_504D:
        xor    ax, ax
        jmp    NEAR PTR loc_5_5055
loc_5_5055:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
_PTEXT ends
        end
