; irlamp_lcode_01_01.asm -- IRLAMP object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

IRLAMPEX_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1ch
        jne    SHORT loc_1_0017
        call   sub_1_6206
        cmp    eax, 1
        ret
loc_1_0017:
        clc
        ret
sub_1_0019:
        db  0CDh, 020h, 021h, 080h, 0A2h, 004h
sub_1_001F:
        db  0CDh, 020h, 008h, 080h, 005h, 000h
        db  0CDh, 020h, 007h, 080h, 005h, 000h
loc_1_002B:
        mov    eax, dword ptr [esp + 4]
        db  039h, 005h, 094h, 0E0h, 000h, 000h
        jbe    SHORT loc_1_003E
        db  0FFh, 024h, 085h, 088h, 0E0h, 000h, 000h
loc_1_003E:
        mov    eax, 1
        ret
sub_1_0044:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_022C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        add    eax, 523h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        add    eax, 1a3h
        add    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 004h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 008h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 00Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0b8h], eax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_00FD
loc_1_00FA:
        inc    dword ptr [ebp - 8]
loc_1_00FD:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 8], eax
        jae    NEAR PTR loc_1_017F
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 68h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 68h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 64h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 5ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 60h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 60h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 5ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 104h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        push   eax
        call   sub_1_2011
        add    esp, 8
        jmp    NEAR PTR loc_1_00FA
loc_1_017F:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 54h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 58h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 58h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 54h], eax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_01B0
loc_1_01AD:
        inc    dword ptr [ebp - 8]
loc_1_01B0:
        cmp    dword ptr [ebp - 8], 8
        jae    NEAR PTR loc_1_01DB
        mov    eax, dword ptr [ebp - 4]
        add    eax, 35h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 54h
        push   eax
        call   sub_1_2011
        add    esp, 8
        add    dword ptr [ebp - 4], 50h
        jmp    NEAR PTR loc_1_01AD
loc_1_01DB:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0b4h], eax
        add    dword ptr [ebp - 4], 100h
        call   sub_1_001F
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_1_1078
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 50h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 50h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4ch], eax
loc_1_022C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0231:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_024E
        mov    eax, 1
        jmp    NEAR PTR loc_1_0287
loc_1_024E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_027D
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 70h], eax
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_0287
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_027D:
        mov    eax, 4
        jmp    NEAR PTR loc_1_0287
loc_1_0287:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_028C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_02B1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 6]
loc_1_02B1:
        xor    eax, eax
        jmp    NEAR PTR loc_1_02B8
loc_1_02B8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_02BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_02E8
        mov    eax, 4
        jmp    NEAR PTR loc_1_0395
loc_1_02E8:
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 1eh]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0305
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0395
loc_1_0305:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c6h], 0bh
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0bch], 1
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 16h]
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c2h], 1
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 1ah]
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_0366
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_036D
loc_1_0366:
        mov    dword ptr [ebp - 4], 0
loc_1_036D:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_037F
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0395
loc_1_037F:
        push   24eh
        push   7
        call   sub_1_12A4
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_0395
loc_1_0395:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_039A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_1301
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 22h]
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c2h], 0
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_03C7
loc_1_03C7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_03CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  081h, 078h, 06Ch, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_03F8
        mov    eax, 1
        jmp    NEAR PTR loc_1_04C4
loc_1_03F8:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0bch], 0
        je     NEAR PTR loc_1_0414
        mov    eax, 17h
        jmp    NEAR PTR loc_1_04C4
loc_1_0414:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 45h], 0ffh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 46h], 13h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 44h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c6h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 3dh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4fh], 7
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0bch], 1
        cmp    dword ptr [ebp + 8], 2580h
        je     NEAR PTR loc_1_0499
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2eh]
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 3eh], 1
        jmp    NEAR PTR loc_1_04A0
loc_1_0499:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 3eh], 0
loc_1_04A0:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_04BC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_1_04BC:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_04C4
loc_1_04C4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_04C9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0bch]
        jmp    NEAR PTR loc_1_04DF
loc_1_04DF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_04E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_056A
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 3e8h
        ja     NEAR PTR loc_1_0507
        mov    eax, 3e8h
loc_1_0507:
        mov    dword ptr [ebp + 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 38h], 0
        jne    NEAR PTR loc_1_0547
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 038h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 30h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 30h
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    NEAR PTR loc_1_0565
loc_1_0547:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_055C
        mov    eax, ecx
loc_1_055C:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 30h], eax
loc_1_0565:
        jmp    NEAR PTR loc_1_0587
loc_1_056A:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 30h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 38h], 0
loc_1_0587:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_058C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        db  081h, 078h, 06Ch, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_05EE
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0bch], 0
        jne    NEAR PTR loc_1_05DD
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 90h], 0ffffffffh
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        push   eax
        push   0
        call   sub_1_1DD1
        add    esp, 0ch
loc_1_05DD:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 30h
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_05EE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_05F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 78h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 94h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 98h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax]
        and    eax, 0feh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c8h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0efh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c9h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 1]
        and    eax, 10h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0cah], al
        call   sub_1_072C
        test   eax, eax
        je     NEAR PTR loc_1_070E
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_06B6
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_06E1
loc_1_06B6:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        test   al, 2
        jne    NEAR PTR loc_1_06DA
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_06E1
loc_1_06DA:
        mov    dword ptr [ebp - 4], 3
loc_1_06E1:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_06FF
        add    dword ptr [ebp - 4], 3
loc_1_06FF:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 6ch]
        add    esp, 4
loc_1_070E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0ah]
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_0727
loc_1_0727:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_072C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 3
        cmp    al, 3
        jne    NEAR PTR loc_1_0886
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        cmp    eax, 0feh
        jne    NEAR PTR loc_1_0886
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_0798
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0fh
        jne    NEAR PTR loc_1_07A2
loc_1_0798:
        mov    eax, 1
        jmp    NEAR PTR loc_1_098C
loc_1_07A2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 2fh
        je     NEAR PTR loc_1_07D6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0afh
        jne    NEAR PTR loc_1_07E9
loc_1_07D6:
        mov    dword ptr [ebp - 4], 0eh
        mov    dword ptr [ebp - 8], 3
        jmp    NEAR PTR loc_1_07F7
loc_1_07E9:
        mov    dword ptr [ebp - 8], 2
        mov    dword ptr [ebp - 4], 0ah
loc_1_07F7:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 98h], ecx
        jb     NEAR PTR loc_1_087F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0a0h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx + 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 9ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 9ch], -1
        je     NEAR PTR loc_1_0875
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 84h]
        cmp    dword ptr [eax + 9ch], ecx
        jne    NEAR PTR loc_1_087F
loc_1_0875:
        mov    eax, 1
        jmp    NEAR PTR loc_1_098C
loc_1_087F:
        xor    eax, eax
        jmp    NEAR PTR loc_1_098C
loc_1_0886:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 80h], 0
        je     NEAR PTR loc_1_0985
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 0c8h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_0985
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 63h
        jne    NEAR PTR loc_1_097B
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 98h], 0ah
        jb     NEAR PTR loc_1_0962
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    eax, dword ptr [eax + 2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0a0h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    eax, dword ptr [eax + 6]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 9ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 9ch], -1
        je     NEAR PTR loc_1_095D
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 84h]
        cmp    dword ptr [eax + 9ch], ecx
        je     NEAR PTR loc_1_095D
        xor    eax, eax
        jmp    NEAR PTR loc_1_098C
loc_1_095D:
        jmp    NEAR PTR loc_1_097B
loc_1_0962:
        db  0A1h, 000h, 000h, 000h, 000h
        db  081h, 078h, 06Ch, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_097B
        xor    eax, eax
        jmp    NEAR PTR loc_1_098C
loc_1_097B:
        mov    eax, 1
        jmp    NEAR PTR loc_1_098C
loc_1_0985:
        xor    eax, eax
        jmp    NEAR PTR loc_1_098C
loc_1_098C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0991:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 54h
        cmp    dword ptr [eax + 54h], ecx
        jne    NEAR PTR loc_1_09B8
        xor    eax, eax
        jmp    NEAR PTR loc_1_0A87
loc_1_09B8:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_09E1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cbh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_09E1
        xor    eax, eax
        jmp    NEAR PTR loc_1_0A87
loc_1_09E1:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 54h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 35h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 3eh], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 44h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c6h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 3dh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 4fh], 7
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 3fh], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 2], 1
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_0A70
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0cbh], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0b4h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 5], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 9], 0
        jmp    NEAR PTR loc_1_0A79
loc_1_0A70:
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 3], 0
loc_1_0A79:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_0A87
loc_1_0A87:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0A8C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_0ACD
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 0b4h]
        cmp    dword ptr [eax + 5], ecx
        jne    NEAR PTR loc_1_0ACD
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0cbh], 0
loc_1_0ACD:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 35h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 54h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0AF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 3fh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0B15
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
loc_1_0B15:
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 7ch], eax
        push   0bh
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 6ch]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0B33:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 4dh]
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0B58:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 34h], eax
        jmp    NEAR PTR loc_1_0F08
loc_1_0B6C:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 38h], eax
        jmp    NEAR PTR loc_1_0C24
loc_1_0B84:
        call   sub_1_1C01
        test   eax, eax
        jne    NEAR PTR loc_1_0B96
        call   sub_1_10EB
loc_1_0B96:
        jmp    NEAR PTR loc_1_0D3F
loc_1_0B9B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0BB8
        call   sub_1_28C1
loc_1_0BB8:
        jmp    NEAR PTR loc_1_0D3F
loc_1_0BBD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 78h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 41h]
        add    esp, 8
        jmp    NEAR PTR loc_1_0D3F
loc_1_0BDB:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0BF8
        call   sub_1_1143
loc_1_0BF8:
        jmp    NEAR PTR loc_1_0D3F
loc_1_0BFD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0C1A
        call   sub_1_10EB
loc_1_0C1A:
        jmp    NEAR PTR loc_1_0D3F
        db  0E9h, 01Bh, 001h, 000h, 000h
loc_1_0C24:
        sub    dword ptr [ebp - 38h], 3
        cmp    dword ptr [ebp - 38h], 0e0h
        ja     NEAR PTR loc_1_0BFD
        mov    eax, dword ptr [ebp - 38h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  001h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 002h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 003h
loc_1_0D3F:
        jmp    NEAR PTR loc_1_0F4D
loc_1_0D44:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0afh
        jne    NEAR PTR loc_1_0D7F
        push   1
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_1AB0
        add    esp, 8
        lea    eax, [ebp - 30h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 29h]
        add    esp, 4
loc_1_0D7F:
        jmp    NEAR PTR loc_1_0F4D
loc_1_0D84:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0bch]
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 12h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0bch], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0DD9
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0bch], 0
        jne    NEAR PTR loc_1_0DD9
        push   0
        push   18h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 49h]
        add    esp, 8
        jmp    NEAR PTR loc_1_0E07
loc_1_0DD9:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0E07
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0bch], 0
        je     NEAR PTR loc_1_0E07
        push   0
        push   17h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 49h]
        add    esp, 8
loc_1_0E07:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0bch], 0
        je     NEAR PTR loc_1_0E3A
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 16h]
        add    esp, 4
        push   24eh
        push   7
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_0E49
loc_1_0E3A:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ach], 0
loc_1_0E49:
        jmp    NEAR PTR loc_1_0F4D
loc_1_0E4E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 45h]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_0EF9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 46h]
        and    eax, 0ffh
        cmp    eax, 13h
        jne    NEAR PTR loc_1_0EF9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 3fh]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_0EF9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 3eh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_0EC4
        push   2580h
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2eh]
        add    esp, 4
loc_1_0EC4:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 16h]
        add    esp, 4
        push   24eh
        push   7
        call   sub_1_12A4
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 45h]
        add    esp, 8
loc_1_0EF9:
        jmp    NEAR PTR loc_1_0F4D
loc_1_0EFE:
        jmp    NEAR PTR loc_1_0F4D
        db  0E9h, 045h, 000h, 000h, 000h
loc_1_0F08:
        sub    dword ptr [ebp - 34h], 3
        cmp    dword ptr [ebp - 34h], 9
        ja     NEAR PTR loc_1_0F4D
        mov    eax, dword ptr [ebp - 34h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 001h, 002h, 005h, 005h, 005h
        db  003h, 004h
loc_1_0F4D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0F52:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0ch
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 6ch]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0F6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_0F9E
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0bch], 0
        mov    dword ptr [ebp - 4], 24eh
        jmp    NEAR PTR loc_1_0FAD
loc_1_0F9E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0bch], 1
loc_1_0FAD:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 16h]
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   7
        call   sub_1_12A4
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 38h], 0
        je     NEAR PTR loc_1_0FF4
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 30h
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_0FF4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0FF9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1016
loc_1_1013:
        dec    dword ptr [ebp - 4]
loc_1_1016:
        cmp    dword ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_1035
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_1013
loc_1_1035:
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_103D
loc_1_103D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_1044:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_105B
loc_1_1058:
        dec    dword ptr [ebp + 0ch]
loc_1_105B:
        cmp    dword ptr [ebp + 0ch], 0
        jbe    NEAR PTR loc_1_1073
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1058
loc_1_1073:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1078:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_1081:
        call   sub_1_10B1
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1081
        cmp    dword ptr [ebp - 4], -1
        je     NEAR PTR loc_1_1081
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 84h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10B1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [eax + eax*8]
        add    eax, 3619h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_10E6
loc_1_10E6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10EB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1115
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_113E
loc_1_1115:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 45h], 0feh
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 46h], 1fh
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_113E
loc_1_113E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1143:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_116D
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_129F
loc_1_116D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        cmp    eax, 0feh
        jne    NEAR PTR loc_1_11BD
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 45h], 0feh
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 47h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0a0h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4bh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 0ah
        jmp    NEAR PTR loc_1_11DB
loc_1_11BD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 45h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 2
loc_1_11DB:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 46h], 0f3h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 98h]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx + 44h]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jbe    NEAR PTR loc_1_128B
        cmp    dword ptr [ebp - 8], 100h
        ja     NEAR PTR loc_1_128B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cbh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_128B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0cbh], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0b4h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 5], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 9], ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    eax, dword ptr [ecx + 94h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, eax
        mov    ecx, dword ptr [ecx + 0b4h]
        call   sub_1_0FF9
loc_1_128B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_129F
loc_1_129F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 18h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_1526
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_12E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 6ch]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1301:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 37h
loc_1_1311:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 4ch
        cmp    dword ptr [eax + 4ch], ecx
        je     NEAR PTR loc_1_1378
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4ch
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 14h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_DDA0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 10h], ecx
        jae    NEAR PTR loc_1_1373
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
loc_1_1373:
        jmp    NEAR PTR loc_1_1311
loc_1_1378:
        cmp    dword ptr [ebp - 4], 37h
        jae    NEAR PTR loc_1_138E
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_138B:
        add    esp, 4
loc_1_138E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1393:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 14h
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_14E2
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        call   dword ptr [eax + 8]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_13DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 37h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [ebp - 8], eax
loc_1_13F7:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4ch
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_1436
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 14h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 10h], ecx
        jae    NEAR PTR loc_1_1429
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
loc_1_1429:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_13F7
loc_1_1436:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_143E
loc_1_143E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1443:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 37h
        cmp    dword ptr [ebp + 8], 6
        jae    NEAR PTR loc_1_1469
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_14D5
loc_1_1469:
        cmp    dword ptr [ebp + 8], 0ch
        jae    NEAR PTR loc_1_147F
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_14D5
loc_1_147F:
        cmp    dword ptr [ebp + 8], 1eh
        jae    NEAR PTR loc_1_1495
        mov    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_1_14D5
loc_1_1495:
        cmp    dword ptr [ebp + 8], 3ch
        ja     NEAR PTR loc_1_14AB
        mov    dword ptr [ebp - 4], 5
        jmp    NEAR PTR loc_1_14D5
loc_1_14AB:
        cmp    dword ptr [ebp + 8], 64h
        ja     NEAR PTR loc_1_14C1
        mov    dword ptr [ebp - 4], 0ah
        jmp    NEAR PTR loc_1_14D5
loc_1_14C1:
        cmp    dword ptr [ebp + 8], 0b4h
        ja     NEAR PTR loc_1_14D5
        mov    dword ptr [ebp - 4], 14h
loc_1_14D5:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_14DD
loc_1_14DD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_13DC
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jbe    NEAR PTR loc_1_1521
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1508:
        add    esp, 4
        cmp    dword ptr [ebp - 4], 37h
        jae    NEAR PTR loc_1_1521
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_151E:
        add    esp, 4
loc_1_1521:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1526:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1443
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 10h], eax
        call   sub_1_13DC
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 10h], ecx
        jae    NEAR PTR loc_1_158B
        cmp    dword ptr [ebp - 4], 37h
        jae    NEAR PTR loc_1_157C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1579:
        add    esp, 4
loc_1_157C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1588:
        add    esp, 4
loc_1_158B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_DD84
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_15D2
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4ch
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_1_15D2
loc_1_15D2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_15D7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_161E
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14h
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_DDA0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_14E2
        add    esp, 4
loc_1_161E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_1624:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        db  081h, 078h, 06Ch, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1651
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0bch], 0
        je     NEAR PTR loc_1_165B
loc_1_1651:
        mov    eax, 17h
        jmp    NEAR PTR loc_1_178D
loc_1_165B:
        cmp    dword ptr [ebp + 8], -1
        jne    NEAR PTR loc_1_16DA
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0ach], 1
        jbe    NEAR PTR loc_1_16BE
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ach], 0
        call   sub_1_DDB0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [ecx + 0b0h]
        cmp    eax, 24eh
        jae    NEAR PTR loc_1_16B9
        push   24eh
        call   sub_1_0F6A
        add    esp, 4
        mov    eax, 17h
        jmp    NEAR PTR loc_1_178D
loc_1_16B9:
        jmp    NEAR PTR loc_1_16DA
loc_1_16BE:
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0ach]
        call   sub_1_DDB0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0b0h], eax
loc_1_16DA:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 8ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c5h], 0
        mov    eax, dword ptr [ebp + 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 74h], eax
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 90h], eax
        mov    al, byte ptr [ebp + 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c7h], al
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 2], 0
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 16h]
        add    esp, 4
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        push   eax
        push   1
        call   sub_1_1DD1
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_176C
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_178D
loc_1_176C:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        inc    byte ptr [eax + 0c5h]
        mov    eax, 2
        jmp    NEAR PTR loc_1_178D
loc_1_178D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1792:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_1900
loc_1_17AD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c5h]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_17EE
        push   0
        call   sub_1_0F6A
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 74h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 25h]
        add    esp, 4
        jmp    NEAR PTR loc_1_17FA
loc_1_17EE:
        push   23h
        push   8
        call   sub_1_12A4
        add    esp, 8
loc_1_17FA:
        jmp    NEAR PTR loc_1_1940
loc_1_17FF:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0afh
        jne    NEAR PTR loc_1_188C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_188C
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 74h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    eax, dword ptr [ecx + 74h]
        add    eax, 4
        mov    dword ptr [ebp - 8], eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1AB0
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 74h]
        inc    word ptr [eax]
        call   sub_1_1B77
loc_1_188C:
        jmp    NEAR PTR loc_1_1940
loc_1_1891:
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2ah]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 1
        je     NEAR PTR loc_1_18C0
        cmp    dword ptr [ebp - 0ch], 2
        je     NEAR PTR loc_1_18C0
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 32h]
        test   eax, eax
        je     NEAR PTR loc_1_18EC
loc_1_18C0:
        cmp    dword ptr [ebp + 8], 8
        jne    NEAR PTR loc_1_18E0
        push   37h
        push   0ah
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_1940
        db  0E9h, 00Ch, 000h, 000h, 000h
loc_1_18E0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 74h]
        inc    word ptr [eax + 2]
loc_1_18EC:
        call   sub_1_1B77
        jmp    NEAR PTR loc_1_1940
loc_1_18F6:
        jmp    NEAR PTR loc_1_1940
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_1900:
        sub    dword ptr [ebp - 14h], 3
        cmp    dword ptr [ebp - 14h], 8
        ja     NEAR PTR loc_1_1940
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 001h, 004h, 002h, 003h
loc_1_1940:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1945:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 34h], eax
        jmp    NEAR PTR loc_1_1A92
loc_1_1959:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 2fh
        jne    NEAR PTR loc_1_1A5F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1A5F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 0ch]
        mov    byte ptr [ebp - 30h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 30h]
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_1A06
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 88h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 84h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   1
        lea    eax, [ebp - 2ch]
        push   eax
        call   sub_1_1AB0
        add    esp, 8
        lea    eax, [ebp - 2ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 29h]
        add    esp, 4
        push   4bh
        call   sub_1_0F6A
        add    esp, 4
        jmp    NEAR PTR loc_1_1A5F
loc_1_1A06:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c4h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1A5F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c5h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 30h]
        cmp    eax, ecx
        jg     NEAR PTR loc_1_1A5F
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 88h]
        push   eax
        push   0
        call   sub_1_1DD1
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c4h], 1
loc_1_1A5F:
        jmp    NEAR PTR loc_1_1AAB
loc_1_1A64:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 88h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 84h], eax
        push   24eh
        call   sub_1_0F6A
        add    esp, 4
        jmp    NEAR PTR loc_1_1AAB
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_1A92:
        cmp    dword ptr [ebp - 34h], 6
        je     NEAR PTR loc_1_1959
        cmp    dword ptr [ebp - 34h], 0ah
        je     NEAR PTR loc_1_1A64
        jmp    NEAR PTR loc_1_1AAB
loc_1_1AAB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 98h]
        sub    eax, 0eh
        cmp    eax, 20h
        jb     NEAR PTR loc_1_1AD5
        mov    eax, 20h
loc_1_1AD5:
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0a0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [eax + 94h]
        mov    ecx, dword ptr [ebp + 8]
        add    edx, 0eh
        add    ecx, 8
        call   sub_1_0FF9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 0dh]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 28h], al
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0afh
        jne    NEAR PTR loc_1_1B6B
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 9ch], -1
        jne    NEAR PTR loc_1_1B6B
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1], 1
        jmp    NEAR PTR loc_1_1B72
loc_1_1B6B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1], 0
loc_1_1B72:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1B77:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c5h]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ecx + 8ch]
        jne    NEAR PTR loc_1_1BBC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c5h], 0ffh
        mov    dword ptr [ebp - 4], 1
loc_1_1BBC:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        push   eax
        push   1
        call   sub_1_1DD1
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c5h]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_1BFC
        db  0A1h, 000h, 000h, 000h, 000h
        inc    byte ptr [eax + 0c5h]
loc_1_1BFC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1C01:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1C2D
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DCC
loc_1_1C2D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 0bh]
        mov    byte ptr [ebp - 10h], al
        mov    al, byte ptr [ebp - 10h]
        and    eax, 3
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 8ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 0ch]
        mov    byte ptr [ebp - 14h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ecx + 8ch]
        jb     NEAR PTR loc_1_1C89
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DCC
loc_1_1C89:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 8ch]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 14h]
        sub    eax, ecx
        mov    dword ptr [ebp - 4], eax
        call   sub_1_10B1
        shr    eax, 6
        sub    edx, edx
        div    dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        add    eax, edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c5h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   al, 4
        je     NEAR PTR loc_1_1D15
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c7h], 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        mov    dword ptr [ebp - 8], eax
        call   sub_1_1078
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 88h], eax
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 84h], eax
        jmp    NEAR PTR loc_1_1D38
loc_1_1D15:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 88h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c7h], 0
loc_1_1D38:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0a0h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 90h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c5h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 14h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1D95
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        push   eax
        push   0
        call   sub_1_1DD1
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c4h], 1
        jmp    NEAR PTR loc_1_1DA1
loc_1_1D95:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c4h], 0
loc_1_1DA1:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4 + 0ah]
        shl    eax, 5
        push   eax
        push   0ah
        call   sub_1_12A4
        add    esp, 8
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DCC
loc_1_1DCC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DD1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        push   1
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_1E02
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_1F6B
loc_1_1E02:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 5]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        or     al, 0feh
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 45h], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 1
        jne    NEAR PTR loc_1_1E32
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 46h], 3fh
        jmp    NEAR PTR loc_1_1E39
loc_1_1E32:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 46h], 0bfh
loc_1_1E39:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 44h], 2
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 90h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 5], eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 8ch], 1
        jne    NEAR PTR loc_1_1E7B
        mov    byte ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1EE2
loc_1_1E7B:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 8ch], 6
        ja     NEAR PTR loc_1_1EA5
        mov    byte ptr [ebp - 4], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8ch], 6
        jmp    NEAR PTR loc_1_1EE2
loc_1_1EA5:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 8ch], 8
        ja     NEAR PTR loc_1_1ECF
        mov    byte ptr [ebp - 4], 2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8ch], 8
        jmp    NEAR PTR loc_1_1EE2
loc_1_1ECF:
        mov    byte ptr [ebp - 4], 3
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8ch], 10h
loc_1_1EE2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c7h]
        and    eax, 0ffh
        or     al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 9], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c5h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0bh], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 9], 0ch
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_1F57
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 3], 2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        add    eax, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0bh], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        mov    al, byte ptr [eax + 24h]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0fh], ax
loc_1_1F57:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_1F6B
loc_1_1F6B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1FA3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1FD1
loc_1_1FD1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1FD6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2011:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2042:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_2053:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     NEAR PTR loc_1_2082
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_1_2075
        mov    eax, 1
        jmp    NEAR PTR loc_1_2089
loc_1_2075:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2053
loc_1_2082:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2089
loc_1_2089:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_208E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_1_20BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     NEAR PTR loc_1_20C6
loc_1_20BF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_211F
loc_1_20C6:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20DB
loc_1_20D3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_20DB:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     NEAR PTR loc_1_2115
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_1_2109
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], ecx
        je     NEAR PTR loc_1_2110
loc_1_2109:
        xor    eax, eax
        jmp    NEAR PTR loc_1_211F
loc_1_2110:
        jmp    NEAR PTR loc_1_20D3
loc_1_2115:
        mov    eax, 1
        jmp    NEAR PTR loc_1_211F
loc_1_211F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2124:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        db  081h, 078h, 06Ch, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_2151
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0bch], 0
        je     NEAR PTR loc_1_215B
loc_1_2151:
        mov    eax, 17h
        jmp    NEAR PTR loc_1_2277
loc_1_215B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ach], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0a8h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 64h
        cmp    dword ptr [eax + 64h], ecx
        jne    NEAR PTR loc_1_219A
        mov    eax, 14h
        jmp    NEAR PTR loc_1_2277
loc_1_219A:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 38h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 80h], eax
        call   sub_1_2F3B
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 58h], eax
        call   sub_1_3345
        mov    eax, dword ptr [ebp + 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 7ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_30CE
        add    esp, 4
        call   sub_1_3631
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_2246
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b7h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 38h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2277
loc_1_2246:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_2261
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_226D
loc_1_2261:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
loc_1_226D:
        mov    eax, 2
        jmp    NEAR PTR loc_1_2277
loc_1_2277:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_227C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        db  081h, 078h, 06Ch, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_22A1
        mov    eax, 1
        jmp    NEAR PTR loc_1_232B
loc_1_22A1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30CE
        add    esp, 4
        call   sub_1_320A
        push   0
        call   sub_1_2996
        add    esp, 4
        call   sub_1_36EC
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_2324
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b7h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 38h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        push   eax
        call   sub_1_2011
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 80h], 0
        push   24eh
        call   sub_1_0F6A
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_232B
loc_1_2324:
        xor    eax, eax
        jmp    NEAR PTR loc_1_232B
loc_1_232B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2330:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2605
loc_1_2344:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0a8h], 2
        jae    NEAR PTR loc_1_2360
        call   sub_1_341E
        jmp    NEAR PTR loc_1_236F
loc_1_2360:
        push   24eh
        push   11h
        call   sub_1_2846
        add    esp, 8
loc_1_236F:
        jmp    NEAR PTR loc_1_2650
loc_1_2374:
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2ah]
        cmp    eax, 1
        jne    NEAR PTR loc_1_2396
        push   30h
        push   0ah
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_2650
loc_1_2396:
        call   sub_1_3631
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0a8h]
        jmp    NEAR PTR loc_1_2650
loc_1_23AB:
        push   1f4h
        push   0dh
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_2650
loc_1_23BF:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 63h
        jne    NEAR PTR loc_1_2487
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0eh
        db  0BAh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax + 80h]
        add    ecx, 0c8h
        call   sub_1_0FF9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 0c8h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 98h]
        sub    eax, 0ah
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        add    eax, 0ah
        push   eax
        call   sub_1_3459
        add    esp, 0ch
        push   1
        call   sub_1_2996
        add    esp, 4
        call   sub_1_2ED1
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 10h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 31h]
        add    esp, 4
        jmp    NEAR PTR loc_1_24BD
loc_1_2487:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0fh
        jne    NEAR PTR loc_1_24BD
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        push   12h
        call   sub_1_2846
        add    esp, 8
loc_1_24BD:
        jmp    NEAR PTR loc_1_2650
loc_1_24C2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 83h
        jne    NEAR PTR loc_1_25C0
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 84h]
        cmp    dword ptr [eax + 0a0h], ecx
        jbe    NEAR PTR loc_1_25BB
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0a0h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 58h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0feh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0a6h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 0c8h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 98h]
        sub    eax, 0bh
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        add    eax, 0bh
        push   eax
        call   sub_1_3459
        add    esp, 0ch
        call   sub_1_320A
        push   0
        call   sub_1_2996
        add    esp, 4
        call   sub_1_36EC
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 31h]
        add    esp, 4
loc_1_25BB:
        jmp    NEAR PTR loc_1_25F6
loc_1_25C0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 43h
        jne    NEAR PTR loc_1_25F6
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        push   13h
        call   sub_1_2846
        add    esp, 8
loc_1_25F6:
        jmp    NEAR PTR loc_1_2650
loc_1_25FB:
        jmp    NEAR PTR loc_1_2650
        db  0E9h, 04Bh, 000h, 000h, 000h
loc_1_2605:
        sub    dword ptr [ebp - 4], 3
        cmp    dword ptr [ebp - 4], 0bh
        ja     NEAR PTR loc_1_2650
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h, 006h, 001h
        db  006h, 006h, 006h, 002h, 003h, 006h, 004h, 005h
loc_1_2650:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2655:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_270E
loc_1_2669:
        push   898h
        push   9
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_274E
loc_1_267D:
        push   24eh
        push   11h
        call   sub_1_2846
        add    esp, 8
        jmp    NEAR PTR loc_1_274E
loc_1_2691:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0afh
        jne    NEAR PTR loc_1_26F3
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 0a0h]
        cmp    dword ptr [eax + 58h], ecx
        jne    NEAR PTR loc_1_26F3
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        call   sub_1_3631
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_274E
loc_1_26F3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2330
        add    esp, 4
        jmp    NEAR PTR loc_1_274E
loc_1_2704:
        jmp    NEAR PTR loc_1_274E
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_270E:
        sub    dword ptr [ebp - 4], 3
        cmp    dword ptr [ebp - 4], 8
        ja     NEAR PTR loc_1_274E
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 001h, 004h, 004h, 002h, 004h, 003h
loc_1_274E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2753:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0bh
        jne    NEAR PTR loc_1_2781
        push   190h
        push   11h
        call   sub_1_12A4
        add    esp, 8
        push   1
        call   sub_1_39AA
        add    esp, 4
        jmp    NEAR PTR loc_1_279D
loc_1_2781:
        cmp    dword ptr [ebp + 8], 0fh
        jne    NEAR PTR loc_1_279D
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0a7h], 1
loc_1_279D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_27A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2804
loc_1_27B6:
        push   1
        push   0
        push   0
        push   0fh
        call   sub_1_5EA2
        add    esp, 10h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_281D
loc_1_27D7:
        call   sub_1_2ED1
        push   384h
        push   11h
        call   sub_1_12A4
        add    esp, 8
        push   0
        call   sub_1_39AA
        add    esp, 4
        jmp    NEAR PTR loc_1_281D
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_1_2804:
        cmp    dword ptr [ebp - 4], 0bh
        je     NEAR PTR loc_1_27D7
        cmp    dword ptr [ebp - 4], 0fh
        je     NEAR PTR loc_1_27B6
        jmp    NEAR PTR loc_1_281D
loc_1_281D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2822:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0bh
        jne    NEAR PTR loc_1_2841
        push   24eh
        push   12h
        call   sub_1_2846
        add    esp, 8
loc_1_2841:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2846:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_54C4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 35h]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b7h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 38h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        push   eax
        call   sub_1_2011
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0F6A
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 38h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 80h], eax
        call   sub_1_2F3B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0a0h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 58h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0feh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0a6h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 0c8h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 98h]
        sub    eax, 0bh
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        add    eax, 0bh
        push   eax
        call   sub_1_3459
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 2dh]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2996:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0a4h], 0
        jne    NEAR PTR loc_1_2A4E
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_2A09
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c8h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_29EA
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
loc_1_29EA:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0bah]
        and    eax, 0ffffh
        and    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_2A23
loc_1_2A09:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0bah]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
loc_1_2A23:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        mov    eax, 1
        mov    cl, byte ptr [ebp - 14h]
        shl    eax, cl
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 0c0h], ax
        jmp    NEAR PTR loc_1_2A6B
loc_1_2A4E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0c0h]
        and    eax, 0ffffh
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
loc_1_2A6B:
        mov    eax, dword ptr [ebp - 14h]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 8ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0cah]
        and    eax, 0fh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 2
        jb     NEAR PTR loc_1_2ABF
        mov    eax, 2
loc_1_2ABF:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    word ptr [ecx + 98h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0ceh]
        and    eax, 7fh
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_307B
        add    esp, 4
        inc    eax
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 1eh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_2B22
        mov    eax, ecx
loc_1_2B22:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 9ah], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c0h]
        and    eax, 7fh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        inc    eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 9bh], al
        cmp    dword ptr [ebp - 14h], 6
        jae    NEAR PTR loc_1_2C28
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0d0h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        and    ecx, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        cdq
        idiv   ecx
        and    eax, 0ffh
        inc    eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0ach], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c2h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        and    ecx, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        cdq
        idiv   ecx
        inc    eax
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_2C41
loc_1_2C28:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0ach], 1
        mov    dword ptr [ebp - 10h], 1
loc_1_2C41:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0d2h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        mov    al, byte ptr [ebp - 18h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0abh], al
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        shl    ecx, 4
        db  066h, 08Bh, 084h, 041h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        inc    eax
        mov    dword ptr [ebp - 24h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0cch]
        and    eax, 3fh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        mov    dword ptr [ebp - 20h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        cmp    eax, ecx
        jl     NEAR PTR loc_1_2CE8
        mov    eax, ecx
loc_1_2CE8:
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        shl    ecx, 4
        db  08Bh, 084h, 081h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 20h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 9ah
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0ach]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2FE8
        add    esp, 14h
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 84h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c4h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        shl    ecx, 4
        db  066h, 08Bh, 084h, 041h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        inc    eax
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 1
        lea    eax, [ebp - 20h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 9bh
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2FE8
        add    esp, 14h
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_2E1F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0d4h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_2E00
        xor    eax, eax
        db  066h, 0A1h, 00Ch, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
loc_1_2E00:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c6h]
        and    eax, 0ffffh
        and    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_2E39
loc_1_2E1F:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c6h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
loc_1_2E39:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 94h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_2E88
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 90h], 0ffffffffh
        jmp    NEAR PTR loc_1_2E9D
loc_1_2E88:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 90h], 0bb8h
loc_1_2E9D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 9bh]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0b0h], al
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_3931
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2ED1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 8ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2eh]
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 9ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 38h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 5ch
        push   eax
        call   sub_1_2011
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0a4h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2F3B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0d7h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        call   sub_1_1044
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 40h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 44h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 44h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 40h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 48h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 4ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 4ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 48h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b7h], 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2FE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
loc_1_3001:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 5
        add    eax, dword ptr [ebp + 0ch]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        imul   eax, ecx
        add    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 8], eax
        jl     NEAR PTR loc_1_3029
        jmp    NEAR PTR loc_1_3066
loc_1_3029:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 1
        jne    NEAR PTR loc_1_305E
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, dword ptr [ebp + 0ch]
        add    ecx, 5
        sub    eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 3
        jge    NEAR PTR loc_1_3059
        mov    dword ptr [ebp - 0ch], 3
loc_1_3059:
        jmp    NEAR PTR loc_1_3066
loc_1_305E:
        dec    byte ptr [ebp - 8]
        jmp    NEAR PTR loc_1_3001
loc_1_3066:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_307B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        and    dword ptr [ebp + 8], 0ffffh
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_309A
loc_1_3097:
        inc    dword ptr [ebp - 4]
loc_1_309A:
        cmp    dword ptr [ebp - 4], 0fh
        jae    NEAR PTR loc_1_30BC
        shl    dword ptr [ebp + 8], 1
        test   byte ptr [ebp + 0ah], 1
        je     NEAR PTR loc_1_30B7
        jmp    NEAR PTR loc_1_30BC
loc_1_30B7:
        jmp    NEAR PTR loc_1_3097
loc_1_30BC:
        mov    eax, 0fh
        sub    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_30C9
loc_1_30C9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_30CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 0bah
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        and    eax, dword ptr [ecx + 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 4], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 8], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0ch], 0ffh
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_3205
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0a4h], 0
        jbe    NEAR PTR loc_1_3182
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0c0h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_1_31A1
loc_1_3182:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        and    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
loc_1_31A1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 7
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 2]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0ffffh
        and    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        and    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ch], ax
loc_1_3205:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_320A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c8h]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cx, word ptr [ecx + 0bah]
        and    ecx, 0ffffh
        and    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    word ptr [ecx + 0bah], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0d4h]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cx, word ptr [ecx + 0c6h]
        and    ecx, 0ffffh
        and    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    word ptr [ecx + 0c6h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0bah]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_3308
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0a4h], 0
        jbe    NEAR PTR loc_1_32EF
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0c0h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    word ptr [ecx + 0bah], ax
        jmp    NEAR PTR loc_1_3308
loc_1_32EF:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    word ptr [ecx + 0bah], ax
loc_1_3308:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0c6h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_3340
        db  066h, 0A1h, 00Ch, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    word ptr [ecx + 0c6h], ax
loc_1_3340:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3345:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
loc_1_334E:
        call   sub_1_10B1
        mov    ecx, 0ffh
        sub    edx, edx
        div    ecx
        mov    eax, edx
        and    eax, 0feh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0a6h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3414
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 0
loc_1_33A5:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 5ch
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_3405
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 38h
        mov    dword ptr [ebp - 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ecx + 0a6h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_33F8
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_3405
loc_1_33F8:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33A5
loc_1_3405:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_3414
        jmp    NEAR PTR loc_1_3419
loc_1_3414:
        jmp    NEAR PTR loc_1_334E
loc_1_3419:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_341E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_10B1
        mov    ecx, 30h
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], edx
        cmp    dword ptr [ebp - 4], 10h
        jae    NEAR PTR loc_1_3446
        add    dword ptr [ebp - 4], 10h
loc_1_3446:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0eh
        call   sub_1_12A4
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3459:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
loc_1_3469:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 8], eax
        jae    NEAR PTR loc_1_3525
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 7fh
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 2
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 8], eax
        jbe    NEAR PTR loc_1_34A8
        jmp    NEAR PTR loc_1_3525
loc_1_34A8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        jg     NEAR PTR loc_1_3517
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 7
        je     NEAR PTR loc_1_3517
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        jne    NEAR PTR loc_1_34DB
        mov    byte ptr [ebp - 4], 6
        jmp    NEAR PTR loc_1_34DE
loc_1_34DB:
        dec    byte ptr [ebp - 4]
loc_1_34DE:
        cmp    dword ptr [ebp - 0ch], 1
        jne    NEAR PTR loc_1_3504
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffff00ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 10h]
        mov    word ptr [edx + ecx*2], ax
        jmp    NEAR PTR loc_1_3517
loc_1_3504:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 10h]
        mov    word ptr [edx + ecx*2], ax
loc_1_3517:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 2
        add    dword ptr [ebp + 8], eax
        jmp    NEAR PTR loc_1_3469
loc_1_3525:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_352A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 0bah
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_355C
loc_1_3559:
        inc    dword ptr [ebp - 0ch]
loc_1_355C:
        cmp    dword ptr [ebp - 0ch], 7
        jae    NEAR PTR loc_1_3624
        mov    eax, dword ptr [ebp - 0ch]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + eax*2]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_359B
        mov    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_35A2
loc_1_359B:
        mov    dword ptr [ebp - 8], 1
loc_1_35A2:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 1], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + eax*2]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_35DF
        mov    eax, dword ptr [ebp - 0ch]
        db  08Ah, 004h, 045h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 2], al
        jmp    NEAR PTR loc_1_3616
loc_1_35DF:
        cmp    dword ptr [ebp - 8], 2
        jne    NEAR PTR loc_1_3603
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + eax*2]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 2], ax
        jmp    NEAR PTR loc_1_3616
loc_1_3603:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [ecx + eax*2]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 2], al
loc_1_3616:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2
        add    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_3559
loc_1_3624:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_362C
loc_1_362C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3631:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   1
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_365B
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_36E7
loc_1_365B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 5]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 45h], 0ffh
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 46h], 93h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 44h], 2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 58h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp - 8]
        add    eax, 9
        push   eax
        call   sub_1_352A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 9], ax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_36E7
loc_1_36E7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_36EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   1
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_3716
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_37A2
loc_1_3716:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 5]
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 45h], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 46h], 73h
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 44h], 2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 58h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_1_352A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 3eh], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 9], ax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_37A2
loc_1_37A2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_37A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b7h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_37CE
        mov    eax, 1
        jmp    NEAR PTR loc_1_3877
loc_1_37CE:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_37F9
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 35h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 40h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 50h]
        jmp    NEAR PTR loc_1_3815
loc_1_37F9:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 35h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 48h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 54h]
loc_1_3815:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 3eh], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  081h, 078h, 074h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_386D
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b6h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_386D
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   13h
        call   sub_1_3A03
        add    esp, 4
loc_1_386D:
        mov    eax, 2
        jmp    NEAR PTR loc_1_3877
loc_1_3877:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_387C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b7h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_38A2
        mov    eax, 1
        jmp    NEAR PTR loc_1_392C
loc_1_38A2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 80h], ecx
        jne    NEAR PTR loc_1_3915
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  081h, 078h, 074h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_38EC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b6h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_38FE
loc_1_38EC:
        push   0fh
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 6ch]
        add    esp, 4
        jmp    NEAR PTR loc_1_3910
loc_1_38FE:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0a7h], 1
loc_1_3910:
        jmp    NEAR PTR loc_1_3922
loc_1_3915:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 80h], 0fh
loc_1_3922:
        mov    eax, 2
        jmp    NEAR PTR loc_1_392C
loc_1_392C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3931:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a0h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a2h], 5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a4h], 0ah
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 6ch], eax
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 70h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_39AA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c3h], al
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_39EC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_39FE
loc_1_39EC:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
loc_1_39FE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3A03:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        call   dword ptr [eax + 74h]
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 80h], 0
        je     NEAR PTR loc_1_3A53
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 80h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        call   dword ptr [eax + 74h]
        add    esp, 4
loc_1_3A53:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_3A58:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 8], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_3C22
loc_1_3A85:
        call   sub_1_4C16
        jmp    NEAR PTR loc_1_3C72
loc_1_3A8F:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3AAB
        mov    byte ptr [ebp - 8], 0
loc_1_3AAB:
        push   1
        push   0
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   1
        call   sub_1_5EA2
        add    esp, 10h
        jmp    NEAR PTR loc_1_3C72
loc_1_3AC2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 78h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 78h], 0
        jmp    NEAR PTR loc_1_3C72
loc_1_3AF3:
        call   sub_1_5FC8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3C72
loc_1_3B0F:
        call   sub_1_1143
        jmp    NEAR PTR loc_1_3C72
loc_1_3B19:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b6h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3B58
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 10h
        jmp    NEAR PTR loc_1_3B5D
loc_1_3B58:
        call   sub_1_44A0
loc_1_3B5D:
        jmp    NEAR PTR loc_1_3C72
loc_1_3B62:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aeh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3B85
        call   sub_1_4F55
loc_1_3B85:
        jmp    NEAR PTR loc_1_3C72
loc_1_3B8A:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        call   sub_1_4935
        jmp    NEAR PTR loc_1_3C72
loc_1_3BA5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 46h]
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 0efh
        cmp    al, 3
        jne    NEAR PTR loc_1_3C01
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 3fh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_3C01
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
loc_1_3C01:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   al, 10h
        je     NEAR PTR loc_1_3C13
        call   sub_1_44A0
loc_1_3C13:
        jmp    NEAR PTR loc_1_3C72
loc_1_3C18:
        jmp    NEAR PTR loc_1_3C72
        db  0E9h, 050h, 000h, 000h, 000h
loc_1_3C22:
        sub    dword ptr [ebp - 10h], 0bh
        cmp    dword ptr [ebp - 10h], 0dh
        ja     NEAR PTR loc_1_3C72
        mov    eax, dword ptr [ebp - 10h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3C72:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_3C77:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_3DFB
loc_1_3C8B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3CB4
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3CB4:
        call   sub_1_5B85
        jmp    NEAR PTR loc_1_3E52
loc_1_3CBE:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3CE7
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3CE7:
        call   sub_1_5CD9
        jmp    NEAR PTR loc_1_3E52
loc_1_3CF1:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3D1A
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3D1A:
        call   sub_1_58DF
        jmp    NEAR PTR loc_1_3E52
loc_1_3D24:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3D57
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   12h
        call   sub_1_458B
        add    esp, 4
loc_1_3D57:
        jmp    NEAR PTR loc_1_3E52
loc_1_3D5C:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0a7h], 1
        jmp    NEAR PTR loc_1_3E52
loc_1_3D73:
        push   16h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_3E52
loc_1_3D82:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 9eh]
        and    eax, 0ffffh
        cmp    eax, 4
        jl     NEAR PTR loc_1_3DB1
        push   1
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_3DE2
loc_1_3DB1:
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 10h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    word ptr [eax + 9eh]
        push   190h
        push   11h
        call   sub_1_12A4
        add    esp, 8
loc_1_3DE2:
        jmp    NEAR PTR loc_1_3E52
loc_1_3DE7:
        call   sub_1_5DE5
        jmp    NEAR PTR loc_1_3E52
loc_1_3DF1:
        jmp    NEAR PTR loc_1_3E52
        db  0E9h, 057h, 000h, 000h, 000h
loc_1_3DFB:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 10h
        ja     NEAR PTR loc_1_3E52
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 003h, 004h, 008h, 008h, 008h, 008h, 008h, 008h
        db  005h, 008h, 006h, 008h, 007h
loc_1_3E52:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_3E57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_3F4D
loc_1_3E6B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3E94
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3E94:
        call   sub_1_5B85
        jmp    NEAR PTR loc_1_3FA0
loc_1_3E9E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3EC7
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3EC7:
        call   sub_1_5CD9
        jmp    NEAR PTR loc_1_3FA0
loc_1_3ED1:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3EFA
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3EFA:
        call   sub_1_58DF
        jmp    NEAR PTR loc_1_3FA0
loc_1_3F04:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0a7h], 1
        jmp    NEAR PTR loc_1_3FA0
loc_1_3F1B:
        push   16h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_3FA0
loc_1_3F2A:
        push   1
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_3FA0
loc_1_3F39:
        call   sub_1_5DE5
        jmp    NEAR PTR loc_1_3FA0
loc_1_3F43:
        jmp    NEAR PTR loc_1_3FA0
        db  0E9h, 053h, 000h, 000h, 000h
loc_1_3F4D:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 10h
        ja     NEAR PTR loc_1_3FA0
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 001h, 002h, 003h, 007h, 007h, 007h, 007h, 007h, 007h, 004h, 007h, 005h, 007h
        db  006h
loc_1_3FA0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_3FA5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_40A9
loc_1_3FB9:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b2h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3FFE
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 98h]
        and    eax, 0ffffh
        push   eax
        push   0dh
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_4008
loc_1_3FFE:
        push   13h
        call   sub_1_5628
        add    esp, 4
loc_1_4008:
        jmp    NEAR PTR loc_1_40CC
loc_1_400D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 63h
        je     NEAR PTR loc_1_403F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 0fh
        jne    NEAR PTR loc_1_4049
loc_1_403F:
        push   13h
        call   sub_1_5628
        add    esp, 4
loc_1_4049:
        jmp    NEAR PTR loc_1_40CC
loc_1_404E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 9ch]
        and    eax, 0ffffh
        cmp    eax, 3
        jl     NEAR PTR loc_1_407D
        push   13h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_409F
loc_1_407D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    word ptr [eax + 9ch]
        push   1
        push   0
        push   1
        push   43h
        call   sub_1_5EA2
        add    esp, 10h
loc_1_409F:
        jmp    NEAR PTR loc_1_40CC
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_40A9:
        cmp    dword ptr [ebp - 4], 3
        je     NEAR PTR loc_1_400D
        cmp    dword ptr [ebp - 4], 0bh
        je     NEAR PTR loc_1_3FB9
        cmp    dword ptr [ebp - 4], 0dh
        je     NEAR PTR loc_1_404E
        jmp    NEAR PTR loc_1_40CC
loc_1_40CC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_40D1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_4221
loc_1_40E5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b2h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_412D
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 98h]
        and    eax, 0ffffh
        lea    eax, [eax + eax*2]
        push   eax
        push   0dh
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_4268
loc_1_412D:
        push   13h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_4268
loc_1_413C:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 43h
        jne    NEAR PTR loc_1_419A
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_419A
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        call   sub_1_5FC8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_4268
loc_1_419A:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_4203
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 9ch]
        and    eax, 0ffffh
        cmp    eax, 3
        jl     NEAR PTR loc_1_41E1
        push   13h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_4203
loc_1_41E1:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    word ptr [eax + 9ch]
        push   1
        push   0
        push   0
        push   43h
        call   sub_1_5EA2
        add    esp, 10h
loc_1_4203:
        jmp    NEAR PTR loc_1_4268
loc_1_4208:
        push   13h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_4268
loc_1_4217:
        jmp    NEAR PTR loc_1_4268
        db  0E9h, 047h, 000h, 000h, 000h
loc_1_4221:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0ch
        ja     NEAR PTR loc_1_4268
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 001h, 002h, 005h, 005h
        db  005h, 005h, 003h, 005h, 004h
loc_1_4268:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_426D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0bh
        jne    NEAR PTR loc_1_4287
        push   13h
        call   sub_1_5628
        add    esp, 4
loc_1_4287:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_428C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_42F1
loc_1_42A0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_42BD
        jmp    NEAR PTR loc_1_4334
loc_1_42BD:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_42CE:
        call   sub_1_4935
        jmp    NEAR PTR loc_1_4334
loc_1_42D8:
        push   16h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_4334
loc_1_42E7:
        jmp    NEAR PTR loc_1_4334
        db  0E9h, 043h, 000h, 000h, 000h
loc_1_42F1:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0ch
        ja     NEAR PTR loc_1_4334
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 001h, 002h, 004h, 004h, 004h, 004h, 004h, 004h
        db  003h
loc_1_4334:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_4339:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_4456
loc_1_434D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_43DE
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 43h
        jne    NEAR PTR loc_1_43D4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 78h]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 78h], 0
        call   sub_1_5FC8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_43DE
loc_1_43D4:
        push   16h
        call   sub_1_458B
        add    esp, 4
loc_1_43DE:
        jmp    NEAR PTR loc_1_449B
loc_1_43E3:
        push   16h
        call   sub_1_5628
        add    esp, 4
        jmp    NEAR PTR loc_1_449B
loc_1_43F2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0a7h], 1
        jmp    NEAR PTR loc_1_449B
loc_1_4409:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b3h], 0
        call   sub_1_4609
        call   sub_1_4782
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 98h]
        and    eax, 0ffffh
        push   eax
        push   0dh
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_449B
loc_1_444C:
        jmp    NEAR PTR loc_1_449B
        db  0E9h, 045h, 000h, 000h, 000h
loc_1_4456:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0eh
        ja     NEAR PTR loc_1_449B
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 001h, 001h, 004h, 004h, 004h, 004h, 004h, 004h
        db  002h, 004h, 003h
loc_1_449B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_44A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 98h]
        and    eax, 0ffffh
        push   eax
        push   0dh
        call   sub_1_12A4
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 80h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b3h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 9bh]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cl, byte ptr [ecx + 0a9h]
        and    ecx, 0ffh
        add    eax, ecx
        and    al, 7
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0b0h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b1h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_4574
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_4586
loc_1_4574:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
loc_1_4586:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_458B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 80h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a7h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_45E0
        cmp    dword ptr [ebp + 8], 0fh
        jne    NEAR PTR loc_1_45FA
loc_1_45E0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 80h], 0fh
        jmp    NEAR PTR loc_1_4604
loc_1_45FA:
        call   sub_1_4609
        call   sub_1_4782
loc_1_4604:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4609:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_471F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b8h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_4679
        call   sub_1_DDB0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 5ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b8h], 1
        jmp    NEAR PTR loc_1_471A
loc_1_4679:
        call   sub_1_DDB0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        sub    eax, dword ptr [ecx + 5ch]
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 90h], ecx
        ja     NEAR PTR loc_1_46F6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b2h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_46F6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   1bh
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 49h]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b2h], 1
loc_1_46F6:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 94h], ecx
        ja     NEAR PTR loc_1_471A
        push   1bh
        call   sub_1_5628
        add    esp, 4
loc_1_471A:
        jmp    NEAR PTR loc_1_477D
loc_1_471F:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b2h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_4759
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   19h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 49h]
        add    esp, 8
loc_1_4759:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b8h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b2h], 0
loc_1_477D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4782:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aeh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_48C0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b9h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_47F2
        call   sub_1_DDB0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 60h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b9h], 1
        jmp    NEAR PTR loc_1_48BB
loc_1_47F2:
        call   sub_1_DDB0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        sub    eax, dword ptr [ecx + 60h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 1770h
        jb     NEAR PTR loc_1_4862
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b4h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_4862
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   2eh
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 49h]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b4h], 1
loc_1_4862:
        cmp    dword ptr [ebp - 4], 9c40h
        jb     NEAR PTR loc_1_48BB
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_48BB
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   2fh
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 49h]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b5h], 1
loc_1_48BB:
        jmp    NEAR PTR loc_1_4930
loc_1_48C0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b4h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_48FA
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   30h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 49h]
        add    esp, 8
loc_1_48FA:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b9h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b4h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b5h], 0
loc_1_4930:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4935:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_4971
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_4987
loc_1_4971:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 4], 0
loc_1_4987:
        push   1
        push   0
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   43h
        call   sub_1_5EA2
        add    esp, 10h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [eax + 80h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 9ch], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_49C7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        shr    eax, 5
        and    al, 7
        mov    byte ptr [ebp - 4], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aah]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        je     NEAR PTR loc_1_4A2B
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4], 0
        je     NEAR PTR loc_1_4A5D
loc_1_4A2B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0fh
        cmp    al, 0dh
        jne    NEAR PTR loc_1_4A53
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4], 0
        je     NEAR PTR loc_1_4A5D
loc_1_4A53:
        mov    eax, 1
        jmp    NEAR PTR loc_1_4A64
loc_1_4A5D:
        xor    eax, eax
        jmp    NEAR PTR loc_1_4A64
loc_1_4A64:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4A69:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        shr    eax, 5
        and    al, 7
        mov    byte ptr [ebp - 2ch], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [ebp - 30h], eax
        mov    byte ptr [ebp - 4], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aah]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cl, byte ptr [ecx + 0a8h]
        and    ecx, 0ffh
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 28h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 2ch]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 28h]
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 34h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a8h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 34h]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_4B06
        int3
loc_1_4B06:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aah]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 2ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_4B3B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b3h], 1
loc_1_4B3B:
        jmp    NEAR PTR loc_1_4B43
loc_1_4B40:
        dec    byte ptr [ebp - 34h]
loc_1_4B43:
        xor    eax, eax
        mov    al, byte ptr [ebp - 34h]
        test   eax, eax
        jle    NEAR PTR loc_1_4BC8
        xor    eax, eax
        mov    al, byte ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ecx + eax*4]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    dword ptr [ebp + ecx*4 - 24h], eax
        inc    byte ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + eax*4], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 28h]
        inc    eax
        and    al, 7
        mov    byte ptr [ebp - 28h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a8h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_4BA0
        int3
loc_1_4BA0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    byte ptr [eax + 0a8h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b3h], 1
        jmp    NEAR PTR loc_1_4B40
loc_1_4BC8:
        mov    byte ptr [ebp - 34h], 0
        jmp    NEAR PTR loc_1_4BD4
loc_1_4BD1:
        inc    byte ptr [ebp - 34h]
loc_1_4BD4:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 34h]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_4C11
        xor    eax, eax
        mov    al, byte ptr [ebp - 34h]
        mov    eax, dword ptr [ebp + eax*4 - 24h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
        jmp    NEAR PTR loc_1_4BD1
loc_1_4C11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4C16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0adh], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aeh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_4CAE
        call   sub_1_4DDE
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 54h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        add    eax, dword ptr [ecx + 50h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cl, byte ptr [ecx + 9ah]
        and    ecx, 0ffh
        cmp    eax, ecx
        jb     NEAR PTR loc_1_4C93
        mov    eax, ecx
loc_1_4C93:
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_52BD
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_5046
        add    esp, 4
loc_1_4CAE:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0adh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_4DD9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_4DC9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0a0h]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cx, word ptr [ecx + 0a2h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_4D31
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 10h
        jmp    NEAR PTR loc_1_4DB2
loc_1_4D31:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b6h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 0a0h]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cx, word ptr [ecx + 0a4h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_4D99
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 6ch]
        push   eax
        push   13h
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_4DB2
loc_1_4D99:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 70h]
        push   eax
        push   13h
        call   sub_1_12A4
        add    esp, 8
loc_1_4DB2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    word ptr [eax + 0a0h]
        jmp    NEAR PTR loc_1_4DD9
loc_1_4DC9:
        push   1
        push   0
        push   0
        push   1
        call   sub_1_5EA2
        add    esp, 10h
loc_1_4DD9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4DDE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a8h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jbe    NEAR PTR loc_1_4F50
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aah]
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_1_4E26
loc_1_4E23:
        dec    dword ptr [ebp - 0ch]
loc_1_4E26:
        cmp    dword ptr [ebp - 0ch], 0
        jbe    NEAR PTR loc_1_4EAF
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        dec    eax
        and    al, 7
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 44h], al
        mov    eax, dword ptr [ebp - 4]
        add    eax, 35h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 40h
        push   eax
        call   sub_1_2011
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    dword ptr [eax + 50h]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + eax*4], 0
        jmp    NEAR PTR loc_1_4E23
loc_1_4EAF:
        mov    al, byte ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0aah], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a8h]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 64h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0d6h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_4F3E
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0d6h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a8h]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 68h], eax
loc_1_4F3E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0a8h], 0
loc_1_4F50:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4F55:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aah]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    cl, byte ptr [ecx + 0a8h]
        and    ecx, 0ffh
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a9h]
        and    eax, 0ffh
        shl    eax, 5
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        add    ecx, ecx
        or     eax, ecx
        or     al, 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 46h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0ach]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3dh], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0abh]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4fh], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a0h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    dword ptr [eax + 20h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    dword ptr [eax + 24h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5046:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aah]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_506B:
        cmp    dword ptr [ebp - 10h], 0
        jbe    NEAR PTR loc_1_529B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        add    ecx, 40h
        cmp    dword ptr [eax + 40h], ecx
        je     NEAR PTR loc_1_529B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 40h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 50h]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 35h
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 45h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_510B
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 45h]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 45h], al
loc_1_510B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a9h]
        and    eax, 0ffh
        shl    eax, 5
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0ch]
        add    ecx, ecx
        or     al, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 46h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    byte ptr [eax + 0a8h]
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edx + 80h]
        mov    dword ptr [edx + ecx*4], eax
        cmp    dword ptr [ebp - 10h], 1
        jne    NEAR PTR loc_1_5178
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 46h]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 46h], al
loc_1_5178:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0ach]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 3dh], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0adh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_51C9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0abh]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 4fh], al
        jmp    NEAR PTR loc_1_51D0
loc_1_51C9:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4fh], 7
loc_1_51D0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    byte ptr [eax + 0adh]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 44h], al
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a0h], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        inc    eax
        and    al, 7
        mov    byte ptr [ebp - 0ch], al
        dec    dword ptr [ebp - 10h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    dword ptr [eax + 20h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        cmp    dword ptr [eax + 64h], 0
        jbe    NEAR PTR loc_1_5265
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 64h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    dword ptr [eax + 24h]
loc_1_5265:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        cmp    dword ptr [eax + 68h], 0
        jbe    NEAR PTR loc_1_5296
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 68h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    dword ptr [eax + 28h]
loc_1_5296:
        jmp    NEAR PTR loc_1_506B
loc_1_529B:
        mov    al, byte ptr [ebp - 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0aah], al
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_52BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_52D2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        add    ecx, 48h
        cmp    dword ptr [eax + 48h], ecx
        je     NEAR PTR loc_1_54B7
        cmp    dword ptr [ebp - 10h], 0
        jbe    NEAR PTR loc_1_54B7
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 48h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 54h]
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 35h
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_534C
        mov    byte ptr [ebp - 4], 1
loc_1_534C:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        and    eax, 0ffh
        or     al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 45h], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 46h], 3
        cmp    dword ptr [ebp - 10h], 1
        jne    NEAR PTR loc_1_53AD
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b1h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_53AD
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 46h]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 46h], al
loc_1_53AD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0ach]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 3dh], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0adh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_53FE
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0abh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 4fh], al
        jmp    NEAR PTR loc_1_5405
loc_1_53FE:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 4fh], 7
loc_1_5405:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    byte ptr [eax + 0adh]
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 44h], al
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_5461
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
loc_1_5461:
        cmp    dword ptr [ebp - 10h], 1
        jne    NEAR PTR loc_1_549B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0b1h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_549B
        push   0
        push   0
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   1
        call   sub_1_5EA2
        add    esp, 10h
loc_1_549B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a0h], 0
        dec    dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_52D2
loc_1_54B7:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_54C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_54DC
loc_1_54D9:
        inc    dword ptr [ebp - 8]
loc_1_54DC:
        cmp    dword ptr [ebp - 8], 8
        jge    NEAR PTR loc_1_5531
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + ecx*4], 0
        je     NEAR PTR loc_1_552C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx*4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   15h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
loc_1_552C:
        jmp    NEAR PTR loc_1_54D9
loc_1_5531:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        add    ecx, 48h
        cmp    dword ptr [eax + 48h], ecx
        je     NEAR PTR loc_1_55AA
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 48h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 54h]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 35h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   13h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
        jmp    NEAR PTR loc_1_5531
loc_1_55AA:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        add    ecx, 40h
        cmp    dword ptr [eax + 40h], ecx
        je     NEAR PTR loc_1_5623
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 40h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 50h]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 35h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   13h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
        jmp    NEAR PTR loc_1_55AA
loc_1_5623:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5628:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 24eh
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 38h
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 0a4h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b7h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 38h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 64h
        push   eax
        call   sub_1_2011
        add    esp, 8
        call   sub_1_54C4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        cmp    dword ptr [eax + 78h], 0
        je     NEAR PTR loc_1_56CE
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 78h]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
loc_1_56CE:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0a4h], 0
        jne    NEAR PTR loc_1_5753
        push   2580h
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2eh]
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_570F
        mov    dword ptr [ebp - 4], 26ch
loc_1_570F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0F6A
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 35h]
        add    esp, 8
        jmp    NEAR PTR loc_1_5753
loc_1_5753:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5758:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_57B2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_579B
        call   sub_1_4935
        jmp    NEAR PTR loc_1_57AD
loc_1_579B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
loc_1_57AD:
        jmp    NEAR PTR loc_1_58DA
loc_1_57B2:
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 45h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 46h], 97h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 47h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a9h]
        and    eax, 0ffh
        shl    eax, 5
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        or     al, byte ptr [ecx + 0aah]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 48h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 94h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_584C
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 48h]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 48h], al
loc_1_584C:
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 49h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0ach]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3dh], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0abh]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4fh], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_58B6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_58DA
loc_1_58B6:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx + 78h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        db  0C7h, 040h, 074h, 000h, 000h, 000h, 000h
loc_1_58DA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_58DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 12h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        cmp    eax, 0feh
        jne    NEAR PTR loc_1_590F
        jmp    NEAR PTR loc_1_5B80
loc_1_590F:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_5A35
loc_1_5927:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_594B
        mov    dword ptr [ebp - 4], 0fh
        jmp    NEAR PTR loc_1_5952
loc_1_594B:
        mov    dword ptr [ebp - 4], 10h
loc_1_5952:
        jmp    NEAR PTR loc_1_5B5C
loc_1_5957:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a0h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0afh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_59B0
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 78h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 3dh]
        add    esp, 10h
loc_1_59B0:
        jmp    NEAR PTR loc_1_5B5C
loc_1_59B5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_59D9
        mov    dword ptr [ebp - 4], 17h
        jmp    NEAR PTR loc_1_59E0
loc_1_59D9:
        mov    dword ptr [ebp - 4], 18h
loc_1_59E0:
        jmp    NEAR PTR loc_1_5B5C
loc_1_59E5:
        mov    dword ptr [ebp - 4], 0fh
        jmp    NEAR PTR loc_1_5B5C
loc_1_59F1:
        mov    dword ptr [ebp - 4], 18h
        jmp    NEAR PTR loc_1_5B5C
loc_1_59FD:
        mov    dword ptr [ebp - 4], 0fh
loc_1_5A04:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_5A2B
        push   1
        call   sub_1_5758
        add    esp, 4
        jmp    NEAR PTR loc_1_5B80
loc_1_5A2B:
        jmp    NEAR PTR loc_1_5B5C
        db  0E9h, 027h, 001h, 000h, 000h
loc_1_5A35:
        sub    dword ptr [ebp - 8], 3
        cmp    dword ptr [ebp - 8], 0e0h
        ja     NEAR PTR loc_1_5A04
        mov    eax, dword ptr [ebp - 8]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 001h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  002h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  003h, 007h, 007h, 007h, 004h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 005h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  006h
loc_1_5B5C:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5B80
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_458B
        add    esp, 4
loc_1_5B80:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5B85:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        shr    eax, 1
        and    al, 7
        mov    byte ptr [ebp - 8], al
        mov    dword ptr [ebp - 4], 12h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 0a0h], 0
        call   sub_1_49C7
        test   eax, eax
        je     NEAR PTR loc_1_5BF0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5BEB
        call   sub_1_4A69
loc_1_5BEB:
        jmp    NEAR PTR loc_1_5BFF
loc_1_5BF0:
        push   8
        call   sub_1_5758
        add    esp, 4
        jmp    NEAR PTR loc_1_5CD4
loc_1_5BFF:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0afh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_5CB0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a9h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_5CA9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a9h]
        and    eax, 0ffh
        inc    eax
        and    al, 7
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 80h]
        mov    byte ptr [ecx + 0a9h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b1h], 1
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 78h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 3dh]
        add    esp, 10h
        jmp    NEAR PTR loc_1_5CB0
loc_1_5CA9:
        mov    dword ptr [ebp - 4], 15h
loc_1_5CB0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5CD4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_458B
        add    esp, 4
loc_1_5CD4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
