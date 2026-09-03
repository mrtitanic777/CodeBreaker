; t20n3_pcode_05_01.asm -- T20N3 object 5 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 48h]
        mov    dword ptr [ebp - 10h], eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0022:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0035:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_004C
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_011F
loc_5_004C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 006h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 00Ah, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 01Ah, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 00Eh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 012h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 016h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 022h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 01Eh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 026h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 02Ah, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        add    edx, 2eh
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   36h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00F9:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0118
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_010E:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_5_011F
loc_5_0118:
        xor    eax, eax
        jmp    NEAR PTR loc_5_011F
loc_5_011F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_0126:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 34h], 0
        jmp    NEAR PTR loc_5_0138
loc_5_0138:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0EC4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0FAB
        lea    edi, [ebp - 30h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 26h], 1
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
loc_5_01BE:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_5_01D3
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_0454
loc_5_01D3:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_5_020C
        push   0
        push   0aa00000ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_01F3:
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0202:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_5_0454
loc_5_020C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_12AC
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_5_0240
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0236:
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_5_0454
loc_5_0240:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4848
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0260
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_0454
loc_5_0260:
        push   1
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
loc_5_029A:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_5_02DB
        push   3
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02BB:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02D3:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_0454
loc_5_02DB:
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
loc_5_0331:
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0399
        push   2
        push   0
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0355:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ch], 1
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0371:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_037A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_038F:
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_0454
loc_5_0399:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_12FC
        and    ax, 0ffh
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_5_0426
        mov    byte ptr [ebp + 14h], 1
        push   1
        push   0
        push   2
        push   0aa000003h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03D6:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ch], 1
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03F2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03FE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0407:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_041C:
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_5_0454
loc_5_0426:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 248h], 3ch
        push   3e8h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_044D:
        xor    eax, eax
        jmp    NEAR PTR loc_5_0454
loc_5_0454:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
loc_5_045B:
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
        jmp    NEAR PTR loc_5_0483
loc_5_0480:
        inc    dword ptr [ebp - 0ch]
loc_5_0483:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 0ch], eax
        jae    NEAR PTR loc_5_04A9
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + eax*4], 1
        jne    NEAR PTR loc_5_04A4
        jmp    NEAR PTR loc_5_04A9
loc_5_04A4:
        jmp    NEAR PTR loc_5_0480
loc_5_04A9:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 0ch], eax
        jne    NEAR PTR loc_5_04BF
        mov    eax, 0c0010019h
        jmp    NEAR PTR loc_5_065A
loc_5_04BF:
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
loc_5_04E9:
        test   eax, eax
        jne    NEAR PTR loc_5_05E2
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
loc_5_0555:
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0598
        push   4
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0579:
        add    esp, 14h
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_058C:
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    NEAR PTR loc_5_05DD
loc_5_0598:
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
loc_5_05DD:
        jmp    NEAR PTR loc_5_0606
loc_5_05E2:
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05F9:
        add    esp, 14h
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_065A
loc_5_0606:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_0652
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0644
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_0652
loc_5_0644:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_0652
loc_5_0652:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_5_065A
loc_5_065A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    24h
loc_5_0661:
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
        jne    NEAR PTR loc_5_091E
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
loc_5_06B4:
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_07A6
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_5_0740
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
        jne    NEAR PTR loc_5_072B
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_3579
loc_5_072B:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_073B:
        jmp    NEAR PTR loc_5_07A1
loc_5_0740:
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
loc_5_07A1:
        jmp    NEAR PTR loc_5_0919
loc_5_07A6:
        cmp    dword ptr [ebp - 0ch], 103h
        jne    NEAR PTR loc_5_081B
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_27AF
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
        jmp    NEAR PTR loc_5_0919
loc_5_081B:
        cmp    dword ptr [ebp - 0ch], 0c001000eh
        jne    NEAR PTR loc_5_088D
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
        jmp    NEAR PTR loc_5_0919
loc_5_088D:
        cmp    dword ptr [ebp - 0ch], 0c001000dh
        jne    NEAR PTR loc_5_08FF
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
        jmp    NEAR PTR loc_5_0919
loc_5_08FF:
        push   1
        push   0ah
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0916:
        add    esp, 14h
loc_5_0919:
        jmp    NEAR PTR loc_5_0925
loc_5_091E:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_0925:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_0971
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0963
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 28h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_0971
loc_5_0963:
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_0971
loc_5_0971:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_0979
loc_5_0979:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0980:
        push   ebp
        mov    ebp, esp
        sub    esp, 78h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 70h], 0ah
        mov    word ptr [ebp - 6eh], 0ah
        db  0C7h, 045h, 094h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 1ch], 0ah
        mov    word ptr [ebp - 1ah], 0ah
        db  0C7h, 045h, 0E8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 54h], 7
        mov    word ptr [ebp - 52h], 7
        db  0C7h, 045h, 0B0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 68h], 0bh
        mov    word ptr [ebp - 66h], 0bh
        db  0C7h, 045h, 09Ch, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4ch], 0ah
        mov    word ptr [ebp - 4ah], 0ah
        db  0C7h, 045h, 0B8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 3ch], 6
        mov    word ptr [ebp - 3ah], 6
        db  0C7h, 045h, 0C8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 60h], 4
        mov    word ptr [ebp - 5eh], 4
        db  0C7h, 045h, 0A4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 24h], 4
        mov    word ptr [ebp - 22h], 4
        db  0C7h, 045h, 0E0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 44h], 4
        mov    word ptr [ebp - 42h], 4
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 14h], 4
        mov    word ptr [ebp - 12h], 4
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 34h], 0
        mov    word ptr [ebp - 78h], 0bh
        mov    word ptr [ebp - 76h], 0bh
        db  0C7h, 045h, 08Ch, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   2deh
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A7B:
        test   eax, eax
        je     NEAR PTR loc_5_0A8D
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_5_0E13
loc_5_0A8D:
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
        lea    eax, [ebp - 58h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0AD1:
        cmp    dword ptr [ebp - 34h], 0
        je     NEAR PTR loc_5_0AF5
        push   0
        push   2deh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0AEB:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_5_0E13
loc_5_0AF5:
        push   1
        lea    eax, [ebp - 70h]
        push   eax
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B0C:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0B2A
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1c6h], eax
        jmp    NEAR PTR loc_5_0B2E
loc_5_0B2A:
        mov    byte ptr [ebp - 0ch], 1
loc_5_0B2E:
        push   0
        lea    eax, [ebp - 68h]
        push   eax
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B45:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0B63
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1ceh], eax
        jmp    NEAR PTR loc_5_0B67
loc_5_0B63:
        mov    byte ptr [ebp - 0ch], 1
loc_5_0B67:
        push   0
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B7E:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0B9C
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1bah], eax
        jmp    NEAR PTR loc_5_0BA0
loc_5_0B9C:
        mov    byte ptr [ebp - 0ch], 1
loc_5_0BA0:
        push   0
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BB7:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0BF2
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [eax + 4], 1
        jne    NEAR PTR loc_5_0BE0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1cah], 1
        jmp    NEAR PTR loc_5_0BED
loc_5_0BE0:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1cah], 0
loc_5_0BED:
        jmp    NEAR PTR loc_5_0BF6
loc_5_0BF2:
        mov    byte ptr [ebp - 0ch], 1
loc_5_0BF6:
        push   2
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C0D:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0C82
        push   1
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C29:
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0C48
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1c2h], 1
        jmp    NEAR PTR loc_5_0C7D
loc_5_0C48:
        push   1
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C5A:
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0C79
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1c2h], 0
        jmp    NEAR PTR loc_5_0C7D
loc_5_0C79:
        mov    byte ptr [ebp - 0ch], 1
loc_5_0C7D:
        jmp    NEAR PTR loc_5_0C86
loc_5_0C82:
        mov    byte ptr [ebp - 0ch], 1
loc_5_0C86:
        push   2
        lea    eax, [ebp - 54h]
        push   eax
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C9D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1d2h], 0
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0D1A
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CC6:
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0CE5
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1d2h], 0
        jmp    NEAR PTR loc_5_0D1A
loc_5_0CE5:
        push   1
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CF7:
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0D16
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1d2h], 1
        jmp    NEAR PTR loc_5_0D1A
loc_5_0D16:
        mov    byte ptr [ebp - 0ch], 1
loc_5_0D1A:
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D2F:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0D80
        cmp    dword ptr [ebp - 4], 6
        jbe    NEAR PTR loc_5_0D4A
        mov    dword ptr [ebp - 4], 6
loc_5_0D4A:
        mov    dword ptr [ebp - 30h], 0
        jmp    NEAR PTR loc_5_0D59
loc_5_0D56:
        inc    dword ptr [ebp - 30h]
loc_5_0D59:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 30h], eax
        jae    NEAR PTR loc_5_0D80
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 30h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 1edh], al
        jmp    NEAR PTR loc_5_0D56
loc_5_0D80:
        push   0
        lea    eax, [ebp - 78h]
        push   eax
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D97:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_0DB0
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1f3h], eax
loc_5_0DB0:
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DB9:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1f3h], 1194h
        jbe    NEAR PTR loc_5_0DD9
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1f3h], 1194h
loc_5_0DD9:
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_0126
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [ebp - 34h], 0
        je     NEAR PTR loc_5_0E0B
        push   0
        push   2deh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E0B:
        mov    eax, dword ptr [ebp - 34h]
        jmp    NEAR PTR loc_5_0E13
loc_5_0E13:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_5_0E1A:
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
loc_5_0E46:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E4F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E5B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E70:
        push   0
        push   2deh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E80:
        jmp    NEAR PTR loc_5_0E85
loc_5_0E85:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0E8C:
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
loc_5_0EAB:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EB8:
        jmp    NEAR PTR loc_5_0EBD
loc_5_0EBD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_0EC4:
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
sub_5_0FAB:
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
        jmp    NEAR PTR loc_5_0FEE
loc_5_0FEB:
        inc    dword ptr [ebp - 4]
loc_5_0FEE:
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        cmp    eax, dword ptr [ebp - 4]
        jbe    NEAR PTR loc_5_100F
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_100A:
        jmp    NEAR PTR loc_5_0FEB
loc_5_100F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_1016:
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
        je     NEAR PTR loc_5_105A
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_5_10B1
loc_5_105A:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1079
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_10B1
loc_5_1079:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_10AA
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_121E
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_10B1
loc_5_10AA:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_10B1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_10FD
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_10EF
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_10FD
loc_5_10EF:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_10FD
loc_5_10FD:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_1105
loc_5_1105:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_110C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
loc_5_1115:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 62h], 0
        je     NEAR PTR loc_5_1171
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
loc_5_1166:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_1115
loc_5_1171:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_1181
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_1181:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0FAB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7ch], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4ah], 0
        je     NEAR PTR loc_5_1217
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ah]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 36h]
        mov    dword ptr [ebp - 4], eax
loc_5_11C0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_5_1203
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11EA:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11F6:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_11C0
loc_5_1203:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1217:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_121E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_1271
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 36h]
        mov    dword ptr [ebp - 4], eax
loc_5_123A:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_5_1271
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1264:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_123A
loc_5_1271:
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
        je     NEAR PTR loc_5_12A2
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
loc_5_12A2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  000h, 000h, 000h
sub_5_12AC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_166C
        test   ax, ax
        je     SHORT loc_5_12C7
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_12C7:
        push   esi
        call   sub_5_153C
        test   ax, ax
        je     SHORT loc_5_12DB
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_12DB:
        push   esi
        call   sub_5_171C
        cmp    ax, 1
        pop    esi
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_12FC:
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
        call   sub_5_192C
        test   ax, ax
        je     SHORT loc_5_1340
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_1340:
        push   esi
        call   sub_5_193C
        test   ax, ax
        je     SHORT loc_5_1354
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_1354:
        push   esi
        call   sub_5_136C
        cmp    ax, 1
        pop    esi
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_136C:
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
loc_5_139F:
        mov    dl, byte ptr [ebp + eax - 14h]
        mov    ecx, dword ptr [esi + 10h]
        inc    eax
        cmp    eax, 12h
        mov    byte ptr [ecx + eax - 1], dl
        jb     SHORT loc_5_139F
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
        call   sub_5_18DC
        push   2
        mov    eax, dword ptr [esi + 8]
        add    eax, 4
        push   eax
        call   sub_5_18DC
        push   2
        mov    eax, dword ptr [esi + 8]
        add    eax, 6
        push   eax
        call   sub_5_18DC
        push   20h
        mov    ecx, dword ptr [esi + 8]
        push   ecx
        call   sub_5_1D8C
        mov    dword ptr [esi + 126h], 3
        push   esi
        call   sub_5_1A5C
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
        call   sub_5_151C
loc_5_14C3:
        mov    eax, dword ptr [esi + 126h]
        cmp    eax, 1
        je     SHORT loc_5_14F5
        cmp    eax, 5
        je     SHORT loc_5_1500
        cmp    eax, 4
        je     SHORT loc_5_150C
        push   0ah
        inc    edi
        push   esi
        call   sub_5_4FD0
        cmp    edi, 1194h
        jb     SHORT loc_5_14C3
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_14F5:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1500:
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_150C:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_5_151C:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 13ah]
        add    ecx, 6
        push   ecx
        push   eax
        call   sub_5_502B
        pop    ebp
        ret    8
        db  090h
sub_5_153C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 136h], 0
        mov    di, 100h
        je     SHORT loc_5_1555
        mov    di, 140h
loc_5_1555:
        cmp    dword ptr [esi + 13eh], 0
        je     SHORT loc_5_1563
        or     di, 80h
loc_5_1563:
        cmp    dword ptr [esi + 146h], 0
        je     SHORT loc_5_1571
        or     di, 200h
loc_5_1571:
        mov    eax, dword ptr [esi + 12eh]
        sub    eax, 3
        cmp    eax, 0ch
        ja     SHORT loc_5_1586
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_1586:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_1590:
        or     di, 1
        jmp    SHORT loc_5_15D0
loc_5_1596:
        or     di, 2
        jmp    SHORT loc_5_15D0
loc_5_159C:
        or     di, 3
        jmp    SHORT loc_5_15D0
loc_5_15A2:
        or     di, 4
        jmp    SHORT loc_5_15D0
loc_5_15A8:
        or     di, 5
        jmp    SHORT loc_5_15D0
loc_5_15AE:
        or     di, 6
        jmp    SHORT loc_5_15D0
loc_5_15B4:
        or     di, 7
        jmp    SHORT loc_5_15D0
loc_5_15BA:
        or     di, 8
        jmp    SHORT loc_5_15D0
loc_5_15C0:
        or     di, 9
        jmp    SHORT loc_5_15D0
loc_5_15C6:
        or     di, 0ah
        jmp    SHORT loc_5_15D0
loc_5_15CC:
        or     di, 0bh
loc_5_15D0:
        mov    eax, dword ptr [esi + 142h]
        cmp    eax, 5
        je     SHORT loc_5_15EF
        cmp    eax, 6
        je     SHORT loc_5_15F5
        cmp    eax, 7
        je     SHORT loc_5_15FB
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_15EF:
        or     di, 10h
        jmp    SHORT loc_5_15FF
loc_5_15F5:
        or     di, 20h
        jmp    SHORT loc_5_15FF
loc_5_15FB:
        or     di, 30h
loc_5_15FF:
        push   edi
        mov    eax, dword ptr [esi + 13ah]
        and    di, 0feffh
        add    eax, 10h
        push   eax
        push   esi
        call   sub_5_502B
        push   edi
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 10h
        push   eax
        push   esi
        call   sub_5_502B
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_166C:
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
        call   sub_5_502B
        push   0
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 4
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_502B
loc_5_169A:
        mov    eax, dword ptr [esi + 13ah]
        inc    edi
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_5087
        mov    byte ptr [esi + edi + 15ah], ah
        cmp    edi, 6
        jb     SHORT loc_5_169A
        cmp    byte ptr [esi + 161h], 0
        jne    SHORT loc_5_1709
        cmp    byte ptr [esi + 162h], 0
        jne    SHORT loc_5_1709
        cmp    byte ptr [esi + 163h], 0
        jne    SHORT loc_5_1709
        cmp    byte ptr [esi + 164h], 0
        jne    SHORT loc_5_1709
        cmp    byte ptr [esi + 165h], 0
        jne    SHORT loc_5_1709
        cmp    byte ptr [esi + 166h], 0
        jne    SHORT loc_5_1709
        xor    cx, cx
loc_5_16F0:
        movsx  eax, cx
        inc    cx
        add    eax, esi
        cmp    cx, 6
        mov    dl, byte ptr [eax + 15bh]
        mov    byte ptr [eax + 161h], dl
        jl     SHORT loc_5_16F0
loc_5_1709:
        mov    al, byte ptr [esi + 15dh]
        pop    edi
        sub    al, 0b8h
        pop    esi
        pop    ebp
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        ret    4
sub_5_171C:
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
        call   sub_5_5087
        and    ax, 107ch
        db  0BFh, 000h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_502B
loc_5_1752:
        mov    eax, edi
        add    edi, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_5_502B
        mov    ax, word ptr [edi]
        mov    ecx, edi
        add    edi, 2
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    edi, 2
        add    eax, 4
        push   eax
        push   esi
        call   sub_5_502B
        mov    ax, word ptr [edi - 2]
        test   ax, ax
        je     SHORT loc_5_17B8
        jbe    SHORT loc_5_1752
        mov    ebx, eax
        and    ebx, 0ffffh
loc_5_179A:
        mov    eax, edi
        add    edi, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_502B
        dec    ebx
        jne    SHORT loc_5_179A
        jmp    SHORT loc_5_1752
loc_5_17B8:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_5087
        and    ax, 0ffbfh
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 8
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_502B
        mov    word ptr [ebp - 2], 1
loc_5_17E5:
        xor    ebx, ebx
loc_5_17E7:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        and    al, 0f0h
        cmp    al, 40h
        je     SHORT loc_5_1810
        push   0ah
        inc    ebx
        push   esi
        call   sub_5_4FD0
        cmp    ebx, 12ch
        jb     SHORT loc_5_17E7
        jmp    SHORT loc_5_1816
loc_5_1810:
        mov    word ptr [ebp - 2], 0
loc_5_1816:
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_5_1838
        push   0ff00h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        inc    edi
        push   eax
        push   esi
        call   sub_5_502B
        cmp    edi, 3
        jb     SHORT loc_5_17E5
loc_5_1838:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_184C:
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
        call   sub_5_502B
        mov    word ptr [ebp - 2], 1
loc_5_1875:
        xor    ebx, ebx
loc_5_1877:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        and    al, 0f0h
        cmp    al, 40h
        je     SHORT loc_5_18A0
        push   0ah
        inc    ebx
        push   esi
        call   sub_5_4FD0
        cmp    ebx, 12ch
        jb     SHORT loc_5_1877
        jmp    SHORT loc_5_18A6
loc_5_18A0:
        mov    word ptr [ebp - 2], 0
loc_5_18A6:
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_5_18C8
        push   0ff00h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        inc    edi
        push   eax
        push   esi
        call   sub_5_502B
        cmp    edi, 3
        jb     SHORT loc_5_1875
loc_5_18C8:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_18DC:
        push   ebp
        mov    al, byte ptr [esp + 0ch]
        mov    ebp, esp
        cmp    al, 1
        je     SHORT loc_5_1919
        cmp    al, 2
        jne    SHORT loc_5_18FC
        mov    edx, dword ptr [ebp + 8]
        pop    ebp
        mov    al, byte ptr [edx + 1]
        mov    cl, byte ptr [edx]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx], al
        ret    8
loc_5_18FC:
        cmp    al, 4
        jne    SHORT loc_5_1919
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [edx]
        mov    cl, byte ptr [edx + 3]
        mov    byte ptr [edx], cl
        mov    byte ptr [edx + 3], al
        mov    al, byte ptr [edx + 1]
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx + 2], al
loc_5_1919:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_192C:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_193C:
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
        call   sub_5_1D8C
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0eh
        push   eax
        call   sub_5_18DC
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 12h
        push   eax
        call   sub_5_18DC
        push   1
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_5_502B
        push   0a00h
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 4
        push   eax
        push   esi
        call   sub_5_502B
        mov    ebx, dword ptr [esi + 0ch]
        mov    edi, 0bh
loc_5_19F8:
        mov    eax, ebx
        add    ebx, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_502B
        dec    edi
        jne    SHORT loc_5_19F8
        push   9080h
        push   esi
        call   sub_5_151C
loc_5_1A1F:
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        test   al, 70h
        je     SHORT loc_5_1A50
        test   al, 10h
        je     SHORT loc_5_1A1F
        mov    ax, 1
loc_5_1A3B:
        mov    ecx, dword ptr [esi]
        cmp    word ptr [ecx], 0
        jne    SHORT loc_5_1A49
        cmp    word ptr [ecx + 2], 0e2c1h
loc_5_1A49:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_1A50:
        xor    ax, ax
        jmp    SHORT loc_5_1A3B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1A5C:
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
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1A9C:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 157h], 0
        lea    edi, [esi + 18h]
        mov    edx, edi
loc_5_1AB6:
        cmp    ecx, 3
        jae    SHORT loc_5_1AD5
        mov    eax, dword ptr [edx + 8ch]
        mov    ebx, dword ptr [edx]
        shr    eax, 10h
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [edx + 8ch]
        mov    ebx, dword ptr [edx]
        mov    word ptr [ebx + 2], ax
loc_5_1AD5:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    word ptr [eax + 4], 0
        inc    ecx
        cmp    ecx, 4
        jb     SHORT loc_5_1AB6
        mov    eax, dword ptr [esi + 0a0h]
        mov    ecx, dword ptr [esi + 24h]
        shr    eax, 10h
        mov    ebx, 4
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0a0h]
        mov    ecx, dword ptr [esi + 24h]
        mov    word ptr [ecx + 2], ax
loc_5_1B07:
        mov    eax, dword ptr [edi]
        push   1ah
        push   eax
        add    edi, 4
        call   sub_5_1D8C
        dec    ebx
        jne    SHORT loc_5_1B07
        lea    ecx, [esi + 118h]
        mov    eax, dword ptr [esi + 0a0h]
        mov    word ptr [ecx], 4
        mov    edx, eax
        mov    word ptr [esi + 11ch], ax
        push   6
        shr    edx, 10h
        push   ecx
        mov    word ptr [esi + 11ah], dx
        call   sub_5_1D8C
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_5_1B4C:
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
loc_5_1B66:
        cmp    ecx, 9
        jae    SHORT loc_5_1B85
        mov    eax, dword ptr [edx + 8ch]
        mov    ebx, dword ptr [edx]
        shr    eax, 10h
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [edx + 8ch]
        mov    ebx, dword ptr [edx]
        mov    word ptr [ebx + 2], ax
loc_5_1B85:
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
        jb     NEAR PTR loc_5_1B66
        mov    eax, dword ptr [esi + 0b0h]
        mov    ecx, dword ptr [esi + 4ch]
        shr    eax, 10h
        mov    ebx, 0ah
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0b0h]
        mov    ecx, dword ptr [esi + 4ch]
        mov    word ptr [ecx + 2], ax
loc_5_1C36:
        mov    eax, dword ptr [edi]
        push   1ah
        push   eax
        add    edi, 4
        call   sub_5_1D8C
        dec    ebx
        jne    SHORT loc_5_1C36
        lea    ecx, [esi + 118h]
        mov    eax, dword ptr [esi + 0b0h]
        mov    word ptr [ecx], 6
        mov    edx, eax
        mov    word ptr [esi + 11ch], ax
        push   6
        shr    edx, 10h
        push   ecx
        mov    word ptr [esi + 11ah], dx
        call   sub_5_1D8C
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_1C7C:
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
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1CBC:
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
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1CFC:
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
        call   sub_5_1D8C
        mov    cx, word ptr [esi + 11ah]
        mov    edx, dword ptr [esi + 8]
        mov    word ptr [edx], cx
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1D4C:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    ecx, [edx + 118h]
        mov    eax, dword ptr [edx + 9ch]
        mov    word ptr [ecx], 0ah
        mov    esi, eax
        mov    word ptr [edx + 11ch], ax
        push   6
        shr    esi, 10h
        push   ecx
        mov    word ptr [edx + 11ah], si
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1D8C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        test   edi, 0fffffffeh
        jbe    SHORT loc_5_1DB2
        shr    edi, 1
        mov    esi, dword ptr [ebp + 8]
loc_5_1DA2:
        mov    eax, esi
        push   2
        add    esi, 2
        push   eax
        call   sub_5_18DC
        dec    edi
        jne    SHORT loc_5_1DA2
loc_5_1DB2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_5_1DBC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 13ah]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        test   al, 80h
        jne    SHORT loc_5_1DDF
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_5_1DDF:
        and    ax, 0fh
        mov    word ptr [esi + 185h], ax
        pop    esi
        mov    ax, 1
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1DFC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0
        push   eax
        call   sub_5_151C
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E1C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0a080h
        push   eax
        call   sub_5_151C
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E3C:
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
        jb     SHORT loc_5_1E6F
        mov    dword ptr [ecx + 153h], 0
loc_5_1E6F:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_1E7C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 153h]
        mov    eax, dword ptr [esi + eax*4 + 28h]
        test   byte ptr [eax + 4], 80h
        je     SHORT loc_5_1E9C
        mov    ax, 0ffffh
        pop    esi
        pop    ebp
        ret    4
loc_5_1E9C:
        push   2
        add    eax, 6
        push   eax
        call   sub_5_18DC
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
sub_5_1EDC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   8280h
        push   eax
        call   sub_5_151C
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1EFC:
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
sub_5_1F2C:
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
sub_5_1F5C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [esp + 10h]
        mov    ebp, esp
        test   dx, dx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 18bh]
        je     SHORT loc_5_1F86
        mov    di, dx
        mov    ecx, dword ptr [ebp + 0ch]
loc_5_1F7C:
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [esi], bl
        inc    esi
        dec    di
        jne    SHORT loc_5_1F7C
loc_5_1F86:
        add    word ptr [eax + 18fh], dx
        pop    edi
        mov    dword ptr [eax + 18bh], esi
        pop    esi
        xor    ax, ax
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1FAC:
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
        call   sub_5_18DC
        push   2
        mov    ecx, dword ptr [esi + 157h]
        mov    eax, dword ptr [esi + ecx*4 + 18h]
        add    eax, 8
        push   eax
        call   sub_5_18DC
        push   2
        mov    ecx, dword ptr [esi + 157h]
        mov    eax, dword ptr [esi + ecx*4 + 18h]
        add    eax, 0ah
        push   eax
        call   sub_5_18DC
        push   2
        mov    ecx, dword ptr [esi + 157h]
        mov    eax, dword ptr [esi + ecx*4 + 18h]
        add    eax, 0ch
        push   eax
        call   sub_5_18DC
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_20CC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   8180h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 157h]
        mov    ecx, dword ptr [esi + eax*4 + 18h]
        mov    word ptr [ecx + 4], 0b8h
        call   sub_5_151C
        mov    eax, dword ptr [esi + 157h]
        inc    eax
        mov    dword ptr [esi + 157h], eax
        cmp    eax, 4
        jb     SHORT loc_5_210A
        mov    dword ptr [esi + 157h], 0
loc_5_210A:
        xor    ax, ax
        inc    dword ptr [esi + 16bh]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_5_211C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0ah
        lea    eax, [ebp - 4]
        push   eax
        call   sub_5_5131
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
        jne    SHORT loc_5_2184
        mov    dword ptr [ecx + 122h], eax
        mov    dword ptr [ecx + 11eh], eax
        xor    ax, ax
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_2184:
        mov    edx, dword ptr [ecx + 11eh]
        mov    dword ptr [edx + 6], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 11eh], eax
        mov    esp, ebp
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_21AC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 14dh], 1
        jne    SHORT loc_5_21C7
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_21C7:
        mov    word ptr [esi + 14dh], 1
        mov    ecx, dword ptr [esi]
        cmp    word ptr [ecx], 0
        je     SHORT loc_5_21EA
        xor    ax, ax
        pop    edi
        mov    word ptr [esi + 14dh], 0
        pop    esi
        pop    ebp
        ret    4
loc_5_21EA:
        mov    edi, dword ptr [esi + 122h]
        test   edi, edi
        jne    SHORT loc_5_2206
        xor    ax, ax
        pop    edi
        mov    word ptr [esi + 14dh], 0
        pop    esi
        pop    ebp
        ret    4
loc_5_2206:
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
        call   sub_5_151C
        push   0ah
        push   edi
        call   sub_5_515B
        mov    word ptr [esi + 14dh], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_5_224C:
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
        ja     NEAR PTR loc_5_2414
        xor    ebx, ebx
        db  08Ah, 098h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 09Dh, 000h, 000h, 000h, 000h
loc_5_22A7:
        mov    dword ptr [esi + 12ah], 2
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_22BB:
        test   cl, 8
        je     SHORT loc_5_22DE
        mov    dword ptr [esi + 126h], 4
        mov    dword ptr [esi + 12ah], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_22DE:
        test   cl, 4
        je     SHORT loc_5_2301
        mov    dword ptr [esi + 126h], 4
        mov    dword ptr [esi + 12ah], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2301:
        test   cl, 1
        je     NEAR PTR loc_5_2414
        mov    dword ptr [esi + 126h], 4
        mov    dword ptr [esi + 12ah], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2328:
        mov    eax, edx
        and    eax, 0ffffh
        cmp    eax, 400h
        jg     NEAR PTR loc_5_2414
        je     NEAR PTR loc_5_23E2
        cmp    eax, 300h
        jne    NEAR PTR loc_5_2414
        cmp    cx, 80h
        jne    SHORT loc_5_23A4
        mov    dword ptr [esi + 126h], 1
        mov    eax, dword ptr [esi + 17bh]
        push   eax
        push   esi
        call   sub_5_1C7C
        push   esi
        call   sub_5_211C
        mov    eax, dword ptr [esi + 17fh]
        push   eax
        push   esi
        call   sub_5_1CBC
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_1A9C
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_1B4C
        push   esi
        call   sub_5_211C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_23A4:
        cmp    dword ptr [esi + 126h], 3
        mov    dword ptr [esi + 126h], 4
        push   esi
        jne    SHORT loc_5_23CD
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_23CD:
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_23E2:
        mov    di, 8
        push   esi
        call   sub_5_1A9C
        push   esi
        call   sub_5_211C
        mov    ax, di
        inc    dword ptr [esi + 173h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2402:
        mov    di, 1
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2410:
        mov    di, 2
loc_5_2414:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 005h, 001h, 005h
        db  005h, 005h, 002h, 005h, 003h, 005h, 004h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_244C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 12ah]
        cmp    eax, 2
        jne    SHORT loc_5_2498
        mov    dword ptr [esi + 12ah], 1
        push   esi
        call   sub_5_184C
        push   esi
        call   sub_5_193C
        mov    dword ptr [esi + 126h], 4
        push   esi
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_21AC
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_5_2498:
        cmp    eax, 3
        jne    SHORT loc_5_24B9
        mov    dword ptr [esi + 12ah], 1
        push   esi
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_21AC
loc_5_24B9:
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_24CC:
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
loc_5_24F8:
        cmp    dword ptr [ebp - 8], 0eh
        jb     NEAR PTR loc_5_2514
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1f3h], ecx
        jae    NEAR PTR loc_5_251E
loc_5_2514:
        mov    eax, 0c001000fh
        jmp    NEAR PTR loc_5_266E
loc_5_251E:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_25C2
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_25B6
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
loc_5_2569:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_5_2585
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 1f3h], ecx
        jae    NEAR PTR loc_5_2591
loc_5_2585:
        mov    dword ptr [ebp - 0ch], 0c000009ah
        jmp    NEAR PTR loc_5_25AB
loc_5_2591:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_2675
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_27AF
loc_5_25AB:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_25BD
loc_5_25B6:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_25BD:
        jmp    NEAR PTR loc_5_261A
loc_5_25C2:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_25E1
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_5_261A
loc_5_25E1:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_2600
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_261A
loc_5_2600:
        push   2
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2617:
        add    esp, 14h
loc_5_261A:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_2666
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_2658
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 18h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_2666
loc_5_2658:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_2666
loc_5_2666:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_266E
loc_5_266E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_2675:
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
        jne    NEAR PTR loc_5_26BD
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 62h], eax
        jmp    NEAR PTR loc_5_26C9
loc_5_26BD:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 66h]
        mov    dword ptr [ecx + 24h], eax
loc_5_26C9:
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
sub_5_26E8:
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
        call   sub_5_1F2C
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2719:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_5_2762
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2734:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1F5C
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ebp - 18h], eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_275D:
        jmp    NEAR PTR loc_5_2719
loc_5_2762:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1FAC
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_20CC
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
loc_5_27A2:
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_27AF:
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
        jne    NEAR PTR loc_5_27D4
        jmp    NEAR PTR loc_5_292F
loc_5_27D4:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 1
        jne    NEAR PTR loc_5_289C
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_27FD
loc_5_27FA:
        inc    dword ptr [ebp - 4]
loc_5_27FD:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_2897
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1EFC
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        cmp    eax, -1
        jne    NEAR PTR loc_5_2835
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
        jmp    NEAR PTR loc_5_292F
loc_5_2835:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 62h], 0
        je     NEAR PTR loc_5_2883
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
        call   sub_5_26E8
        jmp    NEAR PTR loc_5_2892
loc_5_2883:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
        jmp    NEAR PTR loc_5_292F
loc_5_2892:
        jmp    NEAR PTR loc_5_27FA
loc_5_2897:
        jmp    NEAR PTR loc_5_2925
loc_5_289C:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_28AB
loc_5_28A8:
        inc    dword ptr [ebp - 4]
loc_5_28AB:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_2925
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 62h], 0
        je     NEAR PTR loc_5_2911
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
loc_5_2906:
        mov    eax, dword ptr [ebp - 1ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_2920
loc_5_2911:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
        jmp    NEAR PTR loc_5_292F
loc_5_2920:
        jmp    NEAR PTR loc_5_28A8
loc_5_2925:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 255h], 0
loc_5_292F:
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
        jne    NEAR PTR loc_5_2960
        jmp    NEAR PTR loc_5_2A75
loc_5_2960:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2975:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_5_2984
        jmp    NEAR PTR loc_5_2A75
loc_5_2984:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2995:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 10h], eax
        jbe    NEAR PTR loc_5_2A6D
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_5_29DD
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_29B8:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_5_29C7
        jmp    NEAR PTR loc_5_2A6D
loc_5_29C7:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_29D8:
        jmp    NEAR PTR loc_5_2995
loc_5_29DD:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_2A25
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_2A0E
        xor    eax, eax
        sub    eax, dword ptr [ebp - 10h]
        neg    eax
        sub    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_5_2995
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2A0E:
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 8], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0ch]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp + 0ch], 0
loc_5_2A25:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2A38
        mov    eax, ecx
loc_5_2A38:
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2A4C:
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
        jmp    NEAR PTR loc_5_2995
loc_5_2A6D:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_5_2A75:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_5_2A7C:
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
        jne    NEAR PTR loc_5_2AA4
        jmp    NEAR PTR loc_5_2BC5
loc_5_2AA4:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2AB9:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_2AC8
        jmp    NEAR PTR loc_5_2BC5
loc_5_2AC8:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2AD9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
loc_5_2ADF:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_2BBD
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_2B27
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2B02:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_5_2B11
        jmp    NEAR PTR loc_5_2BBD
loc_5_2B11:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2B22:
        jmp    NEAR PTR loc_5_2ADF
loc_5_2B27:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_5_2B6F
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 14h], eax
        jbe    NEAR PTR loc_5_2B58
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0ch]
        neg    eax
        sub    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_2ADF
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2B58:
        mov    eax, dword ptr [ebp + 14h]
        add    dword ptr [ebp - 14h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 14h]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp + 14h], 0
loc_5_2B6F:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2B8E
        mov    eax, ecx
loc_5_2B8E:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2BA2:
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 10h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 1ch]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_2ADF
loc_5_2BBD:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_5_2BC5:
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
        jne    NEAR PTR loc_5_2BF4
        jmp    NEAR PTR loc_5_2DE5
loc_5_2BF4:
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C09:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_2C18
        jmp    NEAR PTR loc_5_2DE5
loc_5_2C18:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C29:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C3E:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_5_2C4D
        jmp    NEAR PTR loc_5_2DE5
loc_5_2C4D:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C5E:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 10h], eax
        jbe    NEAR PTR loc_5_2DDD
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_5_2CA6
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C81:
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_5_2C90
        jmp    NEAR PTR loc_5_2DDD
loc_5_2C90:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2CA1:
        jmp    NEAR PTR loc_5_2C5E
loc_5_2CA6:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_5_2CE2
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2CBD:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_2CCC
        jmp    NEAR PTR loc_5_2DDD
loc_5_2CCC:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2CDD:
        jmp    NEAR PTR loc_5_2C5E
loc_5_2CE2:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_2D2A
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_2D13
        xor    eax, eax
        sub    eax, dword ptr [ebp - 10h]
        neg    eax
        sub    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_5_2C5E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2D13:
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 1ch], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0ch]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp + 0ch], 0
loc_5_2D2A:
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_5_2D72
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 18h], eax
        jbe    NEAR PTR loc_5_2D5B
        xor    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        sub    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_2C5E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2D5B:
        mov    eax, dword ptr [ebp + 18h]
        add    dword ptr [ebp - 18h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 18h]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp + 18h], 0
loc_5_2D72:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2D8B
        mov    eax, ecx
loc_5_2D8B:
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 28h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_2D9E
        mov    eax, ecx
loc_5_2D9E:
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2DB2:
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
        jmp    NEAR PTR loc_5_2C5E
loc_5_2DDD:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
loc_5_2DE5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
loc_5_2DEC:
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
        call   sub_5_1DBC
        mov    word ptr [ebp - 8], ax
        movsx  ecx, word ptr [ebp - 8]
        test   ecx, ecx
        jne    NEAR PTR loc_5_2E20
        xor    al, al
        jmp    NEAR PTR loc_5_2E35
loc_5_2E20:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_1DFC
        mov    al, 1
        jmp    NEAR PTR loc_5_2E35
loc_5_2E35:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_2E3C:
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
        call   sub_5_224C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_2E9F
loc_5_2E73:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_3284
        jmp    NEAR PTR loc_5_2EE9
loc_5_2E81:
        jmp    NEAR PTR loc_5_2EE9
loc_5_2E86:
        jmp    NEAR PTR loc_5_2EE9
loc_5_2E8B:
        jmp    NEAR PTR loc_5_2EE9
loc_5_2E90:
        jmp    NEAR PTR loc_5_2EE9
loc_5_2E95:
        jmp    NEAR PTR loc_5_2EE9
        db  0E9h, 04Ah, 000h, 000h, 000h
loc_5_2E9F:
        dec    dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 18h], 0fh
        ja     NEAR PTR loc_5_2EE9
        mov    eax, dword ptr [ebp - 18h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 002h, 005h, 005h, 005h, 003h
        db  005h, 005h, 005h, 005h, 005h, 005h, 005h, 004h
loc_5_2EE9:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_27AF
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8ch
        push   eax
        call   sub_5_21AC
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_2F4C
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_2F3E
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_2F4C
loc_5_2F3E:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_2F4C
loc_5_2F4C:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1E1C
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_2F61:
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
        je     NEAR PTR loc_5_2FA3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 46h], 0
        jne    NEAR PTR loc_5_2FA3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 46h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_110C
loc_5_2FA3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_3150
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 3eh
        cmp    dword ptr [eax + 3eh], ecx
        je     NEAR PTR loc_5_3150
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
loc_5_2FEC:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_30BD
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_5_3065
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
        jne    NEAR PTR loc_5_3042
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3579
loc_5_3042:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3050:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3060:
        jmp    NEAR PTR loc_5_30B8
loc_5_3065:
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
loc_5_30B8:
        jmp    NEAR PTR loc_5_3150
loc_5_30BD:
        cmp    dword ptr [ebp - 8], 103h
        jne    NEAR PTR loc_5_3132
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_27AF
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
        jmp    NEAR PTR loc_5_3150
loc_5_3132:
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
loc_5_314D:
        add    esp, 18h
loc_5_3150:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_31D2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2eh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_5_31D2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1eh]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_3193
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3579
loc_5_3193:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_31A1:
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
loc_5_31D2:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 2ah]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_31DF:
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
        jne    NEAR PTR loc_5_3207
        jmp    NEAR PTR loc_5_327D
loc_5_3207:
        push   3e8h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 23ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_321A:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 248h]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 248h], 1
        jae    NEAR PTR loc_5_325C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 248h], 0ah
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_1D4C
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_211C
loc_5_325C:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_244C
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ch
        push   eax
        call   sub_5_21AC
        jmp    NEAR PTR loc_5_327D
loc_5_327D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_3284:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_32A0
loc_5_329D:
        inc    dword ptr [ebp - 8]
loc_5_32A0:
        cmp    dword ptr [ebp - 8], 0ah
        jae    NEAR PTR loc_5_33A6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1E7C
        movsx  eax, ax
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], -1
        jne    NEAR PTR loc_5_32CD
        jmp    NEAR PTR loc_5_33A6
loc_5_32CD:
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
        je     NEAR PTR loc_5_3314
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_5_331B
loc_5_3314:
        mov    dword ptr [ebp - 14h], 0eh
loc_5_331B:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 18h], eax
        jae    NEAR PTR loc_5_335D
        cmp    dword ptr [ebp - 18h], 6
        jb     NEAR PTR loc_5_3358
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
loc_5_3358:
        jmp    NEAR PTR loc_5_3393
loc_5_335D:
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
loc_5_3393:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1E3C
        jmp    NEAR PTR loc_5_329D
loc_5_33A6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 1
        jne    NEAR PTR loc_5_33CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_33C0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1EDC
loc_5_33CE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h
loc_5_33D8:
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
        je     NEAR PTR loc_5_3418
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_364F
        mov    dword ptr [ebp - 4], eax
loc_5_3418:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_3420
loc_5_3420:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_5_3427:
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
        je     NEAR PTR loc_5_3467
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_36FA
        mov    dword ptr [ebp - 4], eax
loc_5_3467:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_346F
loc_5_346F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_5_3476:
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
        call   sub_5_364F
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_376A
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_34C3
loc_5_34C3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_5_34CA:
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
sub_5_34DD:
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
loc_5_3513:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_3524
loc_5_3524:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_352B:
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
loc_5_3561:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_3572
loc_5_3572:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_3579:
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
loc_5_3592:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_5_35CC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_5_35BF
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
loc_5_35BF:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_3592
loc_5_35CC:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_35DD
        mov    dword ptr [ebp - 8], 0f0h
loc_5_35DD:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1eh], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_35ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 14h], 8056h
        je     NEAR PTR loc_5_3614
        mov    eax, 0c00000bbh
        jmp    NEAR PTR loc_5_3648
loc_5_3614:
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
loc_5_3637:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_3648
loc_5_3648:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_364F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 7fffffffh
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 73h], 20h
        je     NEAR PTR loc_5_3677
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_3699
loc_5_3677:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 73h], 40h
        je     NEAR PTR loc_5_3692
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 22h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_3699
loc_5_3692:
        mov    dword ptr [ebp - 4], 0
loc_5_3699:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 207h], ecx
        jne    NEAR PTR loc_5_36B2
        xor    eax, eax
        jmp    NEAR PTR loc_5_36F3
loc_5_36B2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 207h], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1C7C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_211C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_21AC
        xor    eax, eax
        jmp    NEAR PTR loc_5_36F3
loc_5_36F3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_36FA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 20bh]
        cmp    dword ptr [eax + 26h], ecx
        jne    NEAR PTR loc_5_371C
        xor    eax, eax
        jmp    NEAR PTR loc_5_3763
loc_5_371C:
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
        call   sub_5_1CBC
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_211C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_21AC
        xor    eax, eax
        jmp    NEAR PTR loc_5_3763
loc_5_3763:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_376A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 72h], 20h
        je     NEAR PTR loc_5_3795
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, 2
        mov    word ptr [ebp - 4], ax
loc_5_3795:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_5_37BC
        xor    eax, eax
        jmp    NEAR PTR loc_5_37FF
loc_5_37BC:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 20fh], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_1CFC
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_211C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        call   sub_5_21AC
        xor    eax, eax
        jmp    NEAR PTR loc_5_37FF
loc_5_37FF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h
loc_5_3808:
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
        jne    NEAR PTR loc_5_3853
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_3F34
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_390F
loc_5_3853:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 1
        jne    NEAR PTR loc_5_3908
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_38AB
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_389F
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_3FAE
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_38A6
loc_5_389F:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_5_38A6:
        jmp    NEAR PTR loc_5_3903
loc_5_38AB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_38CA
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_5_3903
loc_5_38CA:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_38E9
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_3903
loc_5_38E9:
        push   3
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3900:
        add    esp, 14h
loc_5_3903:
        jmp    NEAR PTR loc_5_390F
loc_5_3908:
        mov    dword ptr [ebp - 0ch], 10001h
loc_5_390F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_395B
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_394D
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_395B
loc_5_394D:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_395B
loc_5_395B:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_3963
loc_5_3963:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_396A:
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
        jmp    NEAR PTR loc_5_3E3C
loc_5_39A0:
        mov    dword ptr [ebp - 10h], 6
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_39B9:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_5_39D9
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 5ch
        jmp    NEAR PTR loc_5_39E7
loc_5_39D9:
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 0a0h
loc_5_39E7:
        jmp    NEAR PTR loc_5_3EE2
loc_5_39EC:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3A0B
        mov    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_5_3A50
loc_5_3A0B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3A2A
        mov    dword ptr [ebp - 8], 1
        jmp    NEAR PTR loc_5_3A50
loc_5_3A2A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3A49
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_5_3A50
loc_5_3A49:
        mov    dword ptr [ebp - 8], 0
loc_5_3A50:
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3A62:
        lea    eax, [ebp - 20h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3A74:
        mov    dword ptr [ebp - 10h], 0f0h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3A8D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp + 10h], 10106h
        jne    NEAR PTR loc_5_3AAA
        sub    dword ptr [ebp - 10h], 0eh
loc_5_3AAA:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3ABC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1cah], 1
        jne    NEAR PTR loc_5_3AD8
        mov    dword ptr [ebp - 10h], 9c40h
        jmp    NEAR PTR loc_5_3ADF
loc_5_3AD8:
        mov    dword ptr [ebp - 10h], 27100h
loc_5_3ADF:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3AF1:
        mov    dword ptr [ebp - 10h], 4650h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3B0A:
        mov    dword ptr [ebp - 10h], 0afc8h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3B23:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3B41:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3B5F:
        push   3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1e7h
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3B73:
        mov    byte ptr [ebp - 9], 0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3B89:
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 2fh
        jmp    NEAR PTR loc_5_3EE2
loc_5_3B9C:
        mov    word ptr [ebp - 28h], 300h
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_5_3EE2
loc_5_3BB4:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_5_3BCF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 72h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_3BDB
loc_5_3BCF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 10h], eax
loc_5_3BDB:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3BED:
        mov    dword ptr [ebp - 10h], 0
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3C06:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_5_3C21
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_3C7D
loc_5_3C21:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 36h]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 10h], 0
loc_5_3C31:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 36h
        cmp    eax, dword ptr [ebp - 2ch]
        je     NEAR PTR loc_5_3C7D
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_5_3C70
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0f0h
        jne    NEAR PTR loc_5_3C70
        jmp    NEAR PTR loc_5_3C7D
loc_5_3C70:
        mov    eax, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_5_3C31
loc_5_3C7D:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_5_3EE2
loc_5_3C8F:
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
        jmp    NEAR PTR loc_5_3EE2
loc_5_3CF8:
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
        jmp    NEAR PTR loc_5_3EE2
loc_5_3D61:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_5_3D7F
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_3D8B
loc_5_3D7F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 207h]
        mov    dword ptr [ebp - 10h], eax
loc_5_3D8B:
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
        jmp    NEAR PTR loc_5_3EE2
loc_5_3DD5:
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
        jmp    NEAR PTR loc_5_3EE2
loc_5_3E2B:
        mov    dword ptr [ebp - 24h], 0c00000bbh
        jmp    NEAR PTR loc_5_3EE2
        db  0E9h, 0A6h, 000h, 000h, 000h
loc_5_3E3C:
        cmp    dword ptr [ebp - 44h], 2010101h
        ja     NEAR PTR loc_5_3EB6
        je     NEAR PTR loc_5_3C8F
        sub    dword ptr [ebp - 44h], 10101h
        cmp    dword ptr [ebp - 44h], 12h
        ja     NEAR PTR loc_5_3E2B
        mov    eax, dword ptr [ebp - 44h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_3EB6:
        cmp    dword ptr [ebp - 44h], 2010102h
        je     NEAR PTR loc_5_3CF8
        cmp    dword ptr [ebp - 44h], 2010103h
        je     NEAR PTR loc_5_3D61
        cmp    dword ptr [ebp - 44h], 2010104h
        je     NEAR PTR loc_5_3DD5
        jmp    NEAR PTR loc_5_3E2B
loc_5_3EE2:
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_5_3F25
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 1ch], eax
        jae    NEAR PTR loc_5_3F0C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 24h], 0c0010014h
        jmp    NEAR PTR loc_5_3F25
loc_5_3F0C:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3F1D:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], eax
loc_5_3F25:
        mov    eax, dword ptr [ebp - 24h]
        jmp    NEAR PTR loc_5_3F2D
loc_5_3F2D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    20h
sub_5_3F34:
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
        call   sub_5_396A
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_5_3FA7
loc_5_3FA7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_3FAE:
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
        je     NEAR PTR loc_5_4013
        mov    dword ptr [ebp - 18h], 0c0010014h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_5_41B8
loc_5_4013:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_5_4142
loc_5_401E:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_34DD
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_4193
loc_5_403B:
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_404A:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_35ED
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_4193
loc_5_4067:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_352B
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_4193
        db  0E9h, 00Ah, 001h, 000h, 000h
loc_5_4089:
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4098:
        cmp    dword ptr [ebp - 1ch], 0f0h
        ja     NEAR PTR loc_5_4111
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 1eh], ecx
        jae    NEAR PTR loc_5_40CB
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1eh], eax
        jmp    NEAR PTR loc_5_410C
loc_5_40CB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1eh]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_4103
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_5_4103
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3579
        jmp    NEAR PTR loc_5_410C
loc_5_4103:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
loc_5_410C:
        jmp    NEAR PTR loc_5_4118
loc_5_4111:
        mov    dword ptr [ebp - 18h], 0c0010014h
loc_5_4118:
        jmp    NEAR PTR loc_5_4193
loc_5_411D:
        mov    dword ptr [ebp - 18h], 0c0010017h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        jmp    NEAR PTR loc_5_4193
        db  0E9h, 051h, 000h, 000h, 000h
loc_5_4142:
        cmp    dword ptr [ebp - 24h], 2010103h
        ja     NEAR PTR loc_5_4181
        je     NEAR PTR loc_5_401E
        cmp    dword ptr [ebp - 24h], 1010eh
        je     NEAR PTR loc_5_403B
        cmp    dword ptr [ebp - 24h], 1010fh
        je     NEAR PTR loc_5_4089
        cmp    dword ptr [ebp - 24h], 10112h
        je     NEAR PTR loc_5_4193
        jmp    NEAR PTR loc_5_411D
loc_5_4181:
        cmp    dword ptr [ebp - 24h], 2010104h
        je     NEAR PTR loc_5_4067
        jmp    NEAR PTR loc_5_411D
loc_5_4193:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_5_41B0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
loc_5_41B0:
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_5_41B8
loc_5_41B8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_41BF:
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
        call   sub_5_396A
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0c00000bbh
        jne    NEAR PTR loc_5_4562
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_5_4458
loc_5_4246:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f7h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_4257:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1fbh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_4268:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ffh]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_4279:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 203h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_428A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_42A3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_42BB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_42D4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 1
        jne    NEAR PTR loc_5_42F0
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_42F7
loc_5_42F0:
        mov    dword ptr [ebp - 0ch], 0c0010007h
loc_5_42F7:
        jmp    NEAR PTR loc_5_4524
loc_5_42FC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b6h], 1
        jne    NEAR PTR loc_5_4318
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_434B
loc_5_4318:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b6h], 2
        jne    NEAR PTR loc_5_4334
        mov    dword ptr [ebp - 0ch], 8000h
        jmp    NEAR PTR loc_5_434B
loc_5_4334:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b6h], 3
        jne    NEAR PTR loc_5_434B
        mov    dword ptr [ebp - 0ch], 4000h
loc_5_434B:
        jmp    NEAR PTR loc_5_4524
loc_5_4350:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 1
        jne    NEAR PTR loc_5_436C
        mov    dword ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_5_43C7
loc_5_436C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 3
        jne    NEAR PTR loc_5_4388
        mov    dword ptr [ebp - 0ch], 3
        jmp    NEAR PTR loc_5_43C7
loc_5_4388:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 4
        jne    NEAR PTR loc_5_43A4
        mov    dword ptr [ebp - 0ch], 3
        jmp    NEAR PTR loc_5_43C7
loc_5_43A4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1b2h], 5
        jne    NEAR PTR loc_5_43C0
        mov    dword ptr [ebp - 0ch], 5
        jmp    NEAR PTR loc_5_43C7
loc_5_43C0:
        mov    dword ptr [ebp - 0ch], 2
loc_5_43C7:
        jmp    NEAR PTR loc_5_4524
loc_5_43CC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_43E5:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4524
loc_5_43F1:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4524
loc_5_43FD:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_4416:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4524
loc_5_4422:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a0h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_4524
loc_5_443B:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_4524
loc_5_4447:
        mov    dword ptr [ebp - 18h], 0c0010017h
        jmp    NEAR PTR loc_5_4524
        db  0E9h, 0CCh, 000h, 000h, 000h
loc_5_4458:
        cmp    dword ptr [ebp - 20h], 2010105h
        ja     NEAR PTR loc_5_449A
        je     NEAR PTR loc_5_42D4
        sub    dword ptr [ebp - 20h], 20101h
        cmp    dword ptr [ebp - 20h], 4
        ja     NEAR PTR loc_5_4447
        mov    eax, dword ptr [ebp - 20h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_5_449A:
        cmp    dword ptr [ebp - 20h], 2020101h
        ja     NEAR PTR loc_5_44CC
        je     NEAR PTR loc_5_42A3
        cmp    dword ptr [ebp - 20h], 2010106h
        je     NEAR PTR loc_5_42FC
        cmp    dword ptr [ebp - 20h], 2010107h
        je     NEAR PTR loc_5_4350
        jmp    NEAR PTR loc_5_4447
loc_5_44CC:
        cmp    dword ptr [ebp - 20h], 2020201h
        ja     NEAR PTR loc_5_44F1
        je     NEAR PTR loc_5_43CC
        cmp    dword ptr [ebp - 20h], 2020102h
        je     NEAR PTR loc_5_42BB
        jmp    NEAR PTR loc_5_4447
loc_5_44F1:
        sub    dword ptr [ebp - 20h], 2020202h
        cmp    dword ptr [ebp - 20h], 5
        ja     NEAR PTR loc_5_4447
        mov    eax, dword ptr [ebp - 20h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_4524:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_5_4562
        cmp    dword ptr [ebp - 8], 4
        jb     NEAR PTR loc_5_4550
        push   4
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4547:
        add    dword ptr [ebp - 14h], 4
        jmp    NEAR PTR loc_5_4562
loc_5_4550:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], 0c0010014h
loc_5_4562:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_5_457C
loc_5_457C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_5_4583:
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
        jmp    NEAR PTR loc_5_45CF
        db  0E9h, 0E0h, 000h, 000h, 000h
loc_5_45B2:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_4692
loc_5_45BE:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    NEAR PTR loc_5_4692
        db  0E9h, 0C3h, 000h, 000h, 000h
loc_5_45CF:
        cmp    dword ptr [ebp - 10h], 10113h
        ja     NEAR PTR loc_5_4601
        je     NEAR PTR loc_5_4692
        cmp    dword ptr [ebp - 10h], 10101h
        jb     NEAR PTR loc_5_45BE
        cmp    dword ptr [ebp - 10h], 10111h
        jbe    NEAR PTR loc_5_4692
        jmp    NEAR PTR loc_5_45BE
loc_5_4601:
        cmp    dword ptr [ebp - 10h], 2010103h
        ja     NEAR PTR loc_5_463A
        cmp    dword ptr [ebp - 10h], 2010101h
        jae    NEAR PTR loc_5_4692
        cmp    dword ptr [ebp - 10h], 20101h
        jb     NEAR PTR loc_5_45BE
        cmp    dword ptr [ebp - 10h], 20105h
        jbe    NEAR PTR loc_5_45B2
        jmp    NEAR PTR loc_5_45BE
loc_5_463A:
        cmp    dword ptr [ebp - 10h], 2020102h
        ja     NEAR PTR loc_5_4673
        cmp    dword ptr [ebp - 10h], 2020101h
        jae    NEAR PTR loc_5_45B2
        cmp    dword ptr [ebp - 10h], 2010106h
        jb     NEAR PTR loc_5_45BE
        cmp    dword ptr [ebp - 10h], 2010107h
        jbe    NEAR PTR loc_5_45B2
        jmp    NEAR PTR loc_5_45BE
loc_5_4673:
        cmp    dword ptr [ebp - 10h], 2020201h
        jb     NEAR PTR loc_5_45BE
        cmp    dword ptr [ebp - 10h], 2020207h
        jbe    NEAR PTR loc_5_45B2
        jmp    NEAR PTR loc_5_45BE
loc_5_4692:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_46AC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_41BF
        mov    dword ptr [ebp - 8], eax
loc_5_46AC:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_46F8
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_46EA
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_46F8
loc_5_46EA:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_46F8
loc_5_46F8:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_4700
loc_5_4700:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  000h
loc_5_4708:
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
        jne    NEAR PTR loc_5_4793
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_4787
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
        call   sub_5_2A7C
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_478E
loc_5_4787:
        mov    dword ptr [ebp - 8], 0c001000ch
loc_5_478E:
        jmp    NEAR PTR loc_5_47EB
loc_5_4793:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_47B2
        mov    dword ptr [ebp - 8], 0c001000dh
        jmp    NEAR PTR loc_5_47EB
loc_5_47B2:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_47D1
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_5_47EB
loc_5_47D1:
        push   1
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 56h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_47E8:
        add    esp, 14h
loc_5_47EB:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 2ah]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 2ah], 0
        jne    NEAR PTR loc_5_4837
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_4829
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 2eh
        cmp    dword ptr [eax + 2eh], ecx
        je     NEAR PTR loc_5_4837
loc_5_4829:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_2F61
        jmp    NEAR PTR loc_5_4837
loc_5_4837:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_483F
loc_5_483F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
        db  000h, 000h
sub_5_4848:
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
loc_5_4876:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 8ch], 0
        jne    NEAR PTR loc_5_488D
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_488D:
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
loc_5_48D9:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 90h], 0
        jne    NEAR PTR loc_5_48F0
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_48F0:
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
loc_5_493A:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 94h], 0
        jne    NEAR PTR loc_5_4951
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_4951:
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
loc_5_4997:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 98h], 0
        jne    NEAR PTR loc_5_49AE
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_49AE:
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
loc_5_4A13:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 9ch], 0
        jne    NEAR PTR loc_5_4A2A
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_4A2A:
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
loc_5_4A88:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0a0h], 0
        jne    NEAR PTR loc_5_4A9F
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_4A9F:
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
        jmp    NEAR PTR loc_5_4AE3
loc_5_4AE0:
        inc    dword ptr [ebp - 4]
loc_5_4AE3:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4B6D
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
loc_5_4B1B:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0a4h], 0
        jne    NEAR PTR loc_5_4B36
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_4B36:
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
        jmp    NEAR PTR loc_5_4AE0
loc_5_4B6D:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4B7C
loc_5_4B79:
        inc    dword ptr [ebp - 4]
loc_5_4B7C:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4C06
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
loc_5_4BB4:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0b4h], 0
        jne    NEAR PTR loc_5_4BCF
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_4BCF:
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
        jmp    NEAR PTR loc_5_4B79
loc_5_4C06:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4C15
loc_5_4C12:
        inc    dword ptr [ebp - 4]
loc_5_4C15:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4CA0
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
loc_5_4C50:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0dch], 0
        jne    NEAR PTR loc_5_4C6B
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_4C6B:
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
        jmp    NEAR PTR loc_5_4C12
loc_5_4CA0:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4CAF
loc_5_4CAC:
        inc    dword ptr [ebp - 4]
loc_5_4CAF:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4D3A
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
loc_5_4CEA:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 104h], 0
        jne    NEAR PTR loc_5_4D05
        xor    al, al
        jmp    NEAR PTR loc_5_4D41
loc_5_4D05:
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
        jmp    NEAR PTR loc_5_4CAC
loc_5_4D3A:
        mov    al, 1
        jmp    NEAR PTR loc_5_4D41
loc_5_4D41:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_4D48:
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
loc_5_4D78:
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
loc_5_4D9F:
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
loc_5_4DC3:
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
loc_5_4DE7:
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
loc_5_4E0B:
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
loc_5_4E2F:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4E3E
loc_5_4E3B:
        inc    dword ptr [ebp - 4]
loc_5_4E3E:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4E79
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
loc_5_4E74:
        jmp    NEAR PTR loc_5_4E3B
loc_5_4E79:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4E88
loc_5_4E85:
        inc    dword ptr [ebp - 4]
loc_5_4E88:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4EC3
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
loc_5_4EBE:
        jmp    NEAR PTR loc_5_4E85
loc_5_4EC3:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4ED2
loc_5_4ECF:
        inc    dword ptr [ebp - 4]
loc_5_4ED2:
        cmp    dword ptr [ebp - 4], 0ah
        jae    NEAR PTR loc_5_4F10
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
loc_5_4F0B:
        jmp    NEAR PTR loc_5_4ECF
loc_5_4F10:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_4F1F
loc_5_4F1C:
        inc    dword ptr [ebp - 4]
loc_5_4F1F:
        cmp    dword ptr [ebp - 4], 4
        jae    NEAR PTR loc_5_4F5D
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
loc_5_4F58:
        jmp    NEAR PTR loc_5_4F1C
loc_5_4F5D:
        mov    al, 1
        jmp    NEAR PTR loc_5_4F64
loc_5_4F64:
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
        jmp    NEAR PTR loc_5_4F94
loc_5_4F94:
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
        jmp    NEAR PTR loc_5_4FB4
loc_5_4FB1:
        inc    dword ptr [ebp - 4]
loc_5_4FB4:
        cmp    dword ptr [ebp - 4], 28h
        jae    NEAR PTR loc_5_4FC3
        jmp    NEAR PTR loc_5_4FB1
loc_5_4FC3:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_4FCB
loc_5_4FCB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_5_4FD0:
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
loc_5_4FF1:
        xor    ax, ax
        jmp    NEAR PTR loc_5_4FF9
loc_5_4FF9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        xor    ax, ax
        jmp    NEAR PTR loc_5_5024
loc_5_5024:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_502B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        out    dx, ax
        xor    ax, ax
        jmp    NEAR PTR loc_5_5055
loc_5_5055:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        jmp    NEAR PTR loc_5_5080
loc_5_5080:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
_PTEXT ends
        end
