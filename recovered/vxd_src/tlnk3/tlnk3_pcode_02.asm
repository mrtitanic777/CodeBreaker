; tlnk3_pcode_02.asm -- TLNK3 object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 18h], 6
        mov    word ptr [ebp - 16h], 6
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 10h], 0
        mov    word ptr [ebp - 0eh], 2
        mov    word ptr [ebp - 0ch], 0e000h
        mov    word ptr [ebp - 0ah], 0e001h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp + 8], eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0055:
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 1ch], 3
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 022h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 026h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 036h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 02Ah, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 02Eh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 032h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 03Eh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 03Ah, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 042h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 046h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 1ch]
        add    edx, 4ah
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   36h
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 1ch
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00FB:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_011C
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0112:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_2_0123
loc_2_011C:
        xor    eax, eax
        jmp    NEAR PTR loc_2_0123
loc_2_0123:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_2_012A:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_0138
loc_2_0138:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_054D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_014A:
        lea    edi, [ebp - 30h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 26h], eax
        mov    dword ptr [ebp - 1ah], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 55h]
        mov    dword ptr [ebp - 16h], eax
        mov    dword ptr [ebp - 12h], 10h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0bdh
        mov    dword ptr [ebp - 0eh], eax
        db  083h, 03Dh, 052h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_01AF
        mov    dword ptr [ebp - 1ah], 1
        mov    dword ptr [ebp - 16h], 110h
        mov    dword ptr [ebp - 12h], 4
        db  0B8h, 000h, 000h, 000h, 000h
        add    eax, 52h
        mov    dword ptr [ebp - 0eh], eax
loc_2_01AF:
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c1h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0c5h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_01D7:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_01EC
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_0546
loc_2_01EC:
        xor    eax, eax
        mov    al, byte ptr [ebp + 18h]
        test   eax, eax
        je     NEAR PTR loc_2_022B
        push   0
        push   0aa00000ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_020F:
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0221:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_2_0546
loc_2_022B:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_023A
loc_2_0236:
        inc    word ptr [ebp - 8]
loc_2_023A:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_2_0274
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bdh]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 79h], eax
        jmp    NEAR PTR loc_2_0236
loc_2_0274:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    NEAR PTR loc_2_02AC
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_028B:
        test   eax, eax
        jne    NEAR PTR loc_2_02A2
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_02AC
loc_2_02A2:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_02AC:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    NEAR PTR loc_2_031E
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 55h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 55h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0bdh], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bdh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0302:
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_0319
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_2_0546
loc_2_0319:
        jmp    NEAR PTR loc_2_032E
loc_2_031E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_032E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1540
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_035D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0353:
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_2_0546
loc_2_035D:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 36h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_0380
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e18h], 0c0h
loc_2_0380:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 55h]
        inc    edx
        xor    al, al
        out    dx, al
        push   0c350h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0394:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 55h]
        add    edx, 2
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2fh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2fh]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03EC:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_0512
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_05DD
        test   eax, eax
        je     NEAR PTR loc_2_044B
        push   1
        push   0
        push   2
        push   0aa000004h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0421:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_042D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_043C:
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_2_0546
        db  0E9h, 0C2h, 000h, 000h, 000h
loc_2_044B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0c9h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 75h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 49h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_1D50
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_04CC
        push   2
        push   0
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_04A2:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_04AE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_04BD:
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_2_0546
        db  0E9h, 041h, 000h, 000h, 000h
loc_2_04CC:
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 019h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 19h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 21h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 25h], eax
        push   7530h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0506:
        xor    eax, eax
        jmp    NEAR PTR loc_2_0546
        db  0E9h, 034h, 000h, 000h, 000h
loc_2_0512:
        push   3
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_052C:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_053E:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_0546
loc_2_0546:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_2_054D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 61h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 61h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 5dh], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 65h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 69h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 65h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6dh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 71h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 6dh], eax
        jmp    NEAR PTR loc_2_05A0
loc_2_05A0:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 129h], 100h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 11fh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b2h], 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_2_05DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 32h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0E97
loc_2_05F6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_0625
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0625
        push   186a0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_061D:
        dec    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_05F6
loc_2_0625:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_0647
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_2_0B11
        db  0E9h, 0CAh, 004h, 000h, 000h
loc_2_0647:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e02h]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e03h]
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        shl    eax, 8
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        add    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    ax, word ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0f9h], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_2_072D
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 30h
        jne    NEAR PTR loc_2_06F8
        push   7
        push   4
        push   32h
        push   0aa00000ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_06E6:
        add    esp, 14h
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_2_0B11
        db  0E9h, 030h, 000h, 000h, 000h
loc_2_06F8:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        push   7
        push   4
        push   3
        push   0aa000003h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_071B:
        add    esp, 18h
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_2_0B11
        db  0E9h, 011h, 000h, 000h, 000h
loc_2_072D:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b2h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2ah], 1
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 1
        je     NEAR PTR loc_2_0763
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 33h], 1
        jmp    NEAR PTR loc_2_076A
loc_2_0763:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 33h], 0
loc_2_076A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 3bh], 2000h
        jbe    NEAR PTR loc_2_09DB
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 11fh]
        mov    byte ptr [ebp - 24h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3bh]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 3bh], 10000h
        jne    NEAR PTR loc_2_07AB
        sub    dword ptr [ebp - 20h], 2830h
        jmp    NEAR PTR loc_2_07B2
loc_2_07AB:
        sub    dword ptr [ebp - 20h], 630h
loc_2_07B2:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 33h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_07DB
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 41h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 121h], ax
        jmp    NEAR PTR loc_2_07EC
loc_2_07DB:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3fh]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 121h], ax
loc_2_07EC:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 20h]
        jbe    NEAR PTR loc_2_082F
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 129h]
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, edx
        div    ecx
        shr    eax, 1
        and    eax, 0ffffh
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 127h], ax
        jmp    NEAR PTR loc_2_0861
loc_2_082F:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 129h]
        and    ecx, 0ffffh
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        and    eax, 0ffffh
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 127h], ax
loc_2_0861:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 127h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 129h]
        and    edx, 0ffffh
        imul   ecx, edx
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 20h], eax
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 121h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jae    NEAR PTR loc_2_0935
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, edx
        div    ecx
        cmp    eax, 1000h
        jae    NEAR PTR loc_2_08D9
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 121h], 800h
        jmp    NEAR PTR loc_2_0935
loc_2_08D9:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, edx
        div    ecx
        cmp    eax, 2000h
        jae    NEAR PTR loc_2_0901
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 121h], 1000h
        jmp    NEAR PTR loc_2_0935
loc_2_0901:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, edx
        div    ecx
        cmp    eax, 4000h
        jae    NEAR PTR loc_2_0929
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 121h], 2000h
        jmp    NEAR PTR loc_2_0935
loc_2_0929:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 121h], 4000h
loc_2_0935:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 123h]
        jbe    NEAR PTR loc_2_0966
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 123h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 121h], ax
loc_2_0966:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        sub    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 123h], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 121h]
        and    ecx, 0ffffh
        xor    edx, edx
        mov    dl, byte ptr [ebp - 24h]
        imul   ecx, edx
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 129h]
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, edx
        div    ecx
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 127h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 127h], ax
        jmp    NEAR PTR loc_2_0A4B
loc_2_09DB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 121h], 800h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 11fh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 123h]
        jbe    NEAR PTR loc_2_0A24
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 123h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 121h], ax
loc_2_0A24:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        sub    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 123h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 127h], 0fh
loc_2_0A4B:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 14h], eax
        push   6
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4fh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0A8E:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 49h]
        test   eax, eax
        jne    NEAR PTR loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 4ah]
        test   eax, eax
        jne    NEAR PTR loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 4bh]
        test   eax, eax
        jne    NEAR PTR loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 4ch]
        test   eax, eax
        jne    NEAR PTR loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 4dh]
        test   eax, eax
        jne    NEAR PTR loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 4eh]
        test   eax, eax
        jne    NEAR PTR loc_2_0AFA
        push   6
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 49h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0AFA:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e0fh], 0ffh
        xor    eax, eax
        jmp    NEAR PTR loc_2_0B11
loc_2_0B11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_2_0B18:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_2_0B40
loc_2_0B3D:
        inc    dword ptr [ebp - 0ch]
loc_2_0B40:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 18h], eax
        jbe    NEAR PTR loc_2_0B66
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + eax*4], 1
        jne    NEAR PTR loc_2_0B61
        jmp    NEAR PTR loc_2_0B66
loc_2_0B61:
        jmp    NEAR PTR loc_2_0B3D
loc_2_0B66:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 18h], eax
        jne    NEAR PTR loc_2_0B7C
        mov    eax, 0c0010019h
        jmp    NEAR PTR loc_2_0E90
loc_2_0B7C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 59h]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   96h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0BA6:
        test   eax, eax
        jne    NEAR PTR loc_2_0E12
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
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        call   sub_2_1E40
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_0C5B
        push   4
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0C3C:
        add    esp, 14h
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0C4F:
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    NEAR PTR loc_2_0E0D
loc_2_0C5B:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0d0h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1a8h], 0d0h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 5dh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 61h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 61h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 61h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1b3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_0E0D
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 1
        mov    dword ptr [ebp - 10h], 103h
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1b4h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_0E0D
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1b4h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 19fh], 3
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0f9h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 14h], eax
        mov    edi, dword ptr [ebp - 14h]
        xor    eax, eax
        mov    ecx, 0fh
        rep stosd
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0ah
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 49h
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 127h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ecx, dword ptr [ebp - 8]
        mov    ah, byte ptr [ecx + 127h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 129h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ecx, dword ptr [ebp - 8]
        mov    ah, byte ptr [ecx + 129h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 1ah], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 121h]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 11fh]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 1eh], al
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 20h
loc_2_0E0D:
        jmp    NEAR PTR loc_2_0E39
loc_2_0E12:
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0E2C:
        add    esp, 14h
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_2_0E90
loc_2_0E39:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_2_0E88
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_0E7A
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 20h]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_2_0E88
loc_2_0E7A:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0E83:
        jmp    NEAR PTR loc_2_0E88
loc_2_0E88:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_2_0E90
loc_2_0E90:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    24h
sub_2_0E97:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e08h], 0c0h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e0fh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_0EF5
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 43h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0f1h]
        mov    byte ptr [ecx + 1e00h], al
loc_2_0EF5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_2_0EFC:
        push   ebp
        mov    ebp, esp
        sub    esp, 1cch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 28h], 8
        mov    word ptr [ebp - 26h], 8
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 50h], 7
        mov    word ptr [ebp - 4eh], 7
        db  0C7h, 045h, 0B4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 14h], 0bh
        mov    word ptr [ebp - 12h], 0bh
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 0ch], 0eh
        mov    word ptr [ebp - 0ah], 0eh
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], 0
        mov    byte ptr [ebp - 20h], 1
        mov    byte ptr [ebp - 30h], 0
        mov    byte ptr [ebp - 44h], 0
        mov    byte ptr [ebp - 40h], 0
        mov    dword ptr [ebp - 5ch], 0a20h
        mov    dword ptr [ebp - 38h], 0
        mov    dword ptr [ebp - 2ch], 2
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   3bch
        lea    eax, [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F96:
        test   eax, eax
        je     NEAR PTR loc_2_0FA8
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_2_149B
loc_2_0FA8:
        mov    edi, dword ptr [ebp - 18h]
        xor    eax, eax
        mov    ecx, 0efh
        rep stosd
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 123h], 4628h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 19fh], 2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 0c1h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FEE:
        cmp    dword ptr [ebp - 3ch], 0
        je     NEAR PTR loc_2_1012
        push   0
        push   3bch
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1008:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_2_149B
loc_2_1012:
        push   1
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1029:
        cmp    dword ptr [ebp - 3ch], 0
        jne    NEAR PTR loc_2_104E
        mov    eax, dword ptr [ebp - 34h]
        cmp    dword ptr [eax + 4], 3
        jne    NEAR PTR loc_2_104E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        mov    byte ptr [ebp - 44h], 1
loc_2_104E:
        push   0
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1065:
        cmp    dword ptr [ebp - 3ch], 0
        jne    NEAR PTR loc_2_1091
        mov    eax, dword ptr [ebp - 34h]
        cmp    dword ptr [eax + 4], 1
        jne    NEAR PTR loc_2_1082
        db  0FEh, 005h, 000h, 000h, 000h, 000h
loc_2_1082:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_109B
loc_2_1091:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
loc_2_109B:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    NEAR PTR loc_2_1109
        push   0
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_10BF:
        cmp    dword ptr [ebp - 3ch], 0
        jne    NEAR PTR loc_2_10E3
        mov    eax, dword ptr [ebp - 34h]
        cmp    dword ptr [eax + 4], 1
        jne    NEAR PTR loc_2_10DF
        mov    byte ptr [ebp - 20h], 1
        jmp    NEAR PTR loc_2_10E3
loc_2_10DF:
        mov    byte ptr [ebp - 20h], 0
loc_2_10E3:
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        test   eax, eax
        je     NEAR PTR loc_2_10FF
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 55h], 0a20h
        jmp    NEAR PTR loc_2_1109
loc_2_10FF:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 55h], 0a24h
loc_2_1109:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    NEAR PTR loc_2_12F4
        lea    eax, [ebp - 1b8h]
        push   eax
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_112E:
        cmp    dword ptr [ebp - 3ch], 0
        je     NEAR PTR loc_2_114B
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1141:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_2_149B
loc_2_114B:
        lea    eax, [ebp - 0b4h]
        mov    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [ebp - 38h]
        shl    eax, 0ch
        add    eax, 0c80h
        mov    word ptr [ebp - 6ch], ax
        mov    byte ptr [ebp - 78h], 0
loc_2_1167:
        xor    eax, eax
        mov    al, byte ptr [ebp - 78h]
        test   eax, eax
        jne    NEAR PTR loc_2_120D
        mov    eax, dword ptr [ebp - 70h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 74h], al
        inc    dword ptr [ebp - 70h]
        mov    eax, dword ptr [ebp - 70h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 1bch], ax
        add    dword ptr [ebp - 70h], 2
        xor    eax, eax
        mov    al, byte ptr [ebp - 74h]
        test   al, 80h
        jne    NEAR PTR loc_2_11A1
        mov    byte ptr [ebp - 78h], 1
loc_2_11A1:
        mov    eax, dword ptr [ebp - 70h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 68h], al
        inc    dword ptr [ebp - 70h]
        xor    eax, eax
        mov    al, byte ptr [ebp - 74h]
        test   al, 40h
        je     NEAR PTR loc_2_11C9
        mov    eax, dword ptr [ebp - 70h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 60h], al
        inc    dword ptr [ebp - 70h]
        jmp    NEAR PTR loc_2_11CD
loc_2_11C9:
        mov    byte ptr [ebp - 60h], 0
loc_2_11CD:
        mov    eax, dword ptr [ebp - 1bch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 6ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_2_11EE
        jmp    NEAR PTR loc_2_1167
loc_2_11EE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 60h]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 64h]
        and    eax, ecx
        mov    byte ptr [ebp - 64h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 68h]
        or     al, byte ptr [ebp - 64h]
        mov    byte ptr [ebp - 64h], al
        jmp    NEAR PTR loc_2_1167
loc_2_120D:
        mov    al, byte ptr [ebp - 64h]
        and    eax, 6
        shr    eax, 1
        mov    dword ptr [ebp - 1c8h], eax
        jmp    NEAR PTR loc_2_1286
loc_2_1221:
        mov    dword ptr [ebp - 2ch], 2
        jmp    NEAR PTR loc_2_12C0
loc_2_122D:
        mov    dword ptr [ebp - 2ch], 3
        jmp    NEAR PTR loc_2_12C0
loc_2_1239:
        mov    dword ptr [ebp - 2ch], 5
        jmp    NEAR PTR loc_2_12C0
loc_2_1245:
        mov    dword ptr [ebp - 2ch], 6
        jmp    NEAR PTR loc_2_12C0
loc_2_1251:
        mov    dword ptr [ebp - 2ch], 7
        jmp    NEAR PTR loc_2_12C0
loc_2_125D:
        mov    dword ptr [ebp - 2ch], 0ah
        jmp    NEAR PTR loc_2_12C0
loc_2_1269:
        mov    dword ptr [ebp - 2ch], 0bh
        jmp    NEAR PTR loc_2_12C0
loc_2_1275:
        mov    dword ptr [ebp - 2ch], 0fh
        jmp    NEAR PTR loc_2_12C0
        db  0E9h, 03Ah, 000h, 000h, 000h
loc_2_1286:
        cmp    dword ptr [ebp - 1c8h], 7
        ja     NEAR PTR loc_2_12C0
        mov    eax, dword ptr [ebp - 1c8h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_12C0:
        mov    al, byte ptr [ebp - 64h]
        and    eax, 6
        shr    eax, 1
        mov    byte ptr [ebp - 40h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 40h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 2fh], eax
        mov    eax, dword ptr [ebp - 38h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 6ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 55h], eax
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
loc_2_12F4:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 003h
        jne    NEAR PTR loc_2_137C
        lea    eax, [ebp - 58h]
        push   eax
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1316:
        cmp    dword ptr [ebp - 3ch], 0
        je     NEAR PTR loc_2_1329
        mov    byte ptr [ebp - 30h], 1
        jmp    NEAR PTR loc_2_1454
loc_2_1329:
        mov    al, byte ptr [ebp - 55h]
        and    eax, 1
        mov    dword ptr [ebp - 1cch], eax
        jmp    NEAR PTR loc_2_135D
loc_2_133A:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 55h], 0a20h
        jmp    NEAR PTR loc_2_137C
loc_2_1349:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 55h], 0a24h
        jmp    NEAR PTR loc_2_137C
        db  0E9h, 01Fh, 000h, 000h, 000h
loc_2_135D:
        cmp    dword ptr [ebp - 1cch], 0
        je     NEAR PTR loc_2_133A
        cmp    dword ptr [ebp - 1cch], 1
        je     NEAR PTR loc_2_1349
        jmp    NEAR PTR loc_2_137C
loc_2_137C:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1393:
        cmp    dword ptr [ebp - 3ch], 0
        jne    NEAR PTR loc_2_13AC
        mov    eax, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 123h], eax
loc_2_13AC:
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_13C1:
        cmp    dword ptr [ebp - 4], 6
        jne    NEAR PTR loc_2_1454
        cmp    dword ptr [ebp - 3ch], 0
        jne    NEAR PTR loc_2_1454
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 49h
        mov    dword ptr [ebp - 1c0h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 1c4h], eax
        mov    eax, dword ptr [ebp - 1c4h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 1c0h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 1c4h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 1c0h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 1c4h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 1c0h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 1c4h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 1c0h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 1c4h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 1c0h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 1c4h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 1c0h]
        mov    byte ptr [ecx + 5], al
loc_2_1454:
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_145D:
        mov    al, byte ptr [ebp - 30h]
        push   eax
        mov    al, byte ptr [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_2_012A
        mov    dword ptr [ebp - 3ch], eax
        cmp    dword ptr [ebp - 3ch], 0
        je     NEAR PTR loc_2_1493
        push   0
        push   3bch
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1493:
        mov    eax, dword ptr [ebp - 3ch]
        jmp    NEAR PTR loc_2_149B
loc_2_149B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_2_14A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 1dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14C1:
        push   2625a0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14CB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14D4:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        call   sub_2_1E20
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14F2:
        push   0
        push   3bch
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1502:
        jmp    NEAR PTR loc_2_1507
loc_2_1507:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_2_150E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1527:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1534:
        jmp    NEAR PTR loc_2_1539
loc_2_1539:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_2_1540:
        push   ebp
        mov    ebp, esp
        sub    esp, 7ch
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_156B
        db  0A1h, 000h, 000h, 000h, 000h
        shl    eax, 0ch
        add    eax, 800h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 55h], eax
loc_2_156B:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_2_15B7
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 55h]
        in     al, dx
        mov    byte ptr [ebp - 48h], al
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 55h]
        add    edx, 2
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 1
        shl    eax, 13h
        mov    cl, byte ptr [ebp - 48h]
        and    ecx, 0fch
        shl    ecx, 0bh
        or     eax, ecx
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_2_15DC
loc_2_15B7:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0fe00h
        not    eax
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0fc00h
        shl    eax, 4
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
loc_2_15DC:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0fe00h
        not    eax
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0fc00h
        shl    eax, 4
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 4], eax
        push   2000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0f1h
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1626:
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_2_1650
        push   0
        push   0aa000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1646:
        add    esp, 0ch
        xor    al, al
        jmp    NEAR PTR loc_2_1D3F
loc_2_1650:
        mov    dword ptr [ebp - 50h], 0
        jmp    NEAR PTR loc_2_165F
loc_2_165C:
        inc    dword ptr [ebp - 50h]
loc_2_165F:
        cmp    dword ptr [ebp - 50h], 3
        jae    NEAR PTR loc_2_16CC
        mov    eax, dword ptr [ebp - 50h]
        mov    dword ptr [ebp + eax*4 - 5ch], 0
        mov    dword ptr [ebp - 60h], 0
        jmp    NEAR PTR loc_2_1684
loc_2_1680:
        add    dword ptr [ebp - 60h], 2
loc_2_1684:
        cmp    dword ptr [ebp - 60h], 10h
        jae    NEAR PTR loc_2_16C7
        mov    eax, dword ptr [ebp - 50h]
        shl    eax, 4
        add    eax, dword ptr [ebp - 60h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0f1h]
        mov    al, byte ptr [eax + ecx + 1f30h]
        mov    byte ptr [ebp - 4ch], al
        mov    eax, dword ptr [ebp - 50h]
        mov    eax, dword ptr [ebp + eax*4 - 5ch]
        shl    eax, 4
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4ch]
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 50h]
        mov    dword ptr [ebp + ecx*4 - 5ch], eax
        jmp    NEAR PTR loc_2_1680
loc_2_16C7:
        jmp    NEAR PTR loc_2_165C
loc_2_16CC:
        mov    eax, dword ptr [ebp - 5ch]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_1705
        mov    eax, dword ptr [ebp - 58h]
        db  039h, 005h, 004h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_1705
        mov    eax, dword ptr [ebp - 54h]
        db  039h, 005h, 008h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_1705
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 34h], 1
        jmp    NEAR PTR loc_2_1744
loc_2_1705:
        mov    eax, dword ptr [ebp - 5ch]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_173D
        mov    eax, dword ptr [ebp - 58h]
        db  039h, 005h, 004h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_173D
        db  0A1h, 008h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 54h], eax
        jne    NEAR PTR loc_2_173D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 34h], 0
        jmp    NEAR PTR loc_2_1744
loc_2_173D:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 34h], 1
loc_2_1744:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_1774
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e01h]
        mov    byte ptr [ebp - 3ch], al
        mov    al, byte ptr [ebp - 3ch]
        and    eax, 0ch
        shr    eax, 2
        mov    byte ptr [ebp - 3ch], al
        jmp    NEAR PTR loc_2_1783
loc_2_1774:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0ch
        shr    eax, 2
        mov    byte ptr [ebp - 3ch], al
loc_2_1783:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0ch
        shr    eax, 2
        mov    byte ptr [ebp - 24h], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_1998
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_17D0
        mov    al, byte ptr [ebp - 48h]
        and    eax, 0fch
        shr    eax, 1
        or     eax, 80h
        add    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 43h], al
        jmp    NEAR PTR loc_2_17F0
loc_2_17D0:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0fc00h
        shr    eax, 1
        or     eax, 8000h
        mov    word ptr [ebp - 34h], ax
        mov    al, byte ptr [ebp - 34h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 43h], al
loc_2_17F0:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0fc00h
        shr    eax, 1
        or     eax, 8000h
        mov    word ptr [ebp - 34h], ax
        mov    eax, dword ptr [ebp - 34h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 43h], ah
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_18A6
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        mov    dword ptr [ebp - 64h], eax
        jmp    NEAR PTR loc_2_187D
loc_2_182F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        test   eax, eax
        jne    NEAR PTR loc_2_1848
        mov    dword ptr [ebp - 38h], 2000h
        jmp    NEAR PTR loc_2_184F
loc_2_1848:
        mov    dword ptr [ebp - 38h], 4000h
loc_2_184F:
        mov    byte ptr [ebp - 28h], 4
        jmp    NEAR PTR loc_2_18A1
loc_2_1858:
        mov    dword ptr [ebp - 38h], 8000h
        mov    byte ptr [ebp - 28h], 8
        jmp    NEAR PTR loc_2_18A1
loc_2_1868:
        mov    dword ptr [ebp - 38h], 10000h
        mov    byte ptr [ebp - 28h], 10h
        jmp    NEAR PTR loc_2_18A1
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_187D:
        cmp    dword ptr [ebp - 64h], 3
        ja     NEAR PTR loc_2_18A1
        mov    eax, dword ptr [ebp - 64h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_18A1:
        jmp    NEAR PTR loc_2_1925
loc_2_18A6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        mov    dword ptr [ebp - 68h], eax
        jmp    NEAR PTR loc_2_1901
loc_2_18B3:
        mov    dword ptr [ebp - 38h], 10000h
        mov    byte ptr [ebp - 28h], 10h
        jmp    NEAR PTR loc_2_1925
loc_2_18C3:
        mov    dword ptr [ebp - 38h], 8000h
        mov    byte ptr [ebp - 28h], 8
        jmp    NEAR PTR loc_2_1925
loc_2_18D3:
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        test   eax, eax
        jne    NEAR PTR loc_2_18EC
        mov    dword ptr [ebp - 38h], 2000h
        jmp    NEAR PTR loc_2_18F3
loc_2_18EC:
        mov    dword ptr [ebp - 38h], 4000h
loc_2_18F3:
        mov    byte ptr [ebp - 28h], 4
        jmp    NEAR PTR loc_2_1925
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_1901:
        cmp    dword ptr [ebp - 68h], 3
        ja     NEAR PTR loc_2_1925
        mov    eax, dword ptr [ebp - 68h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_1925:
        mov    eax, dword ptr [ebp - 38h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 3bh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 43h]
        and    eax, 0ffh
        shl    eax, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0f5h
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1969:
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_2_1993
        push   0
        push   0aa000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1989:
        add    esp, 0ch
        xor    al, al
        jmp    NEAR PTR loc_2_1D3F
loc_2_1993:
        jmp    NEAR PTR loc_2_1A69
loc_2_1998:
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        mov    dword ptr [ebp - 6ch], eax
        jmp    NEAR PTR loc_2_19DA
loc_2_19A5:
        mov    dword ptr [ebp - 2ch], 2000h
        jmp    NEAR PTR loc_2_19FE
loc_2_19B1:
        mov    dword ptr [ebp - 2ch], 4000h
        jmp    NEAR PTR loc_2_19FE
loc_2_19BD:
        mov    dword ptr [ebp - 2ch], 8000h
        jmp    NEAR PTR loc_2_19FE
loc_2_19C9:
        mov    dword ptr [ebp - 2ch], 10000h
        jmp    NEAR PTR loc_2_19FE
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_19DA:
        cmp    dword ptr [ebp - 6ch], 3
        ja     NEAR PTR loc_2_19FE
        mov    eax, dword ptr [ebp - 6ch]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_19FE:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0feh
        shl    eax, 0ch
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0f5h
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A36:
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_2_1A60
        push   0
        push   0aa000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A56:
        add    esp, 0ch
        xor    al, al
        jmp    NEAR PTR loc_2_1D3F
loc_2_1A60:
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 3bh], eax
loc_2_1A69:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2fh]
        and    eax, 0ffh
        mov    dword ptr [ebp - 40h], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_1B29
        mov    al, byte ptr [ebp - 48h]
        and    eax, 3
        mov    dword ptr [ebp - 70h], eax
        jmp    NEAR PTR loc_2_1B05
loc_2_1A92:
        mov    dword ptr [ebp - 44h], 2
        jmp    NEAR PTR loc_2_1B29
loc_2_1A9E:
        mov    dword ptr [ebp - 44h], 3
        jmp    NEAR PTR loc_2_1B29
loc_2_1AAA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_1AC9
        mov    dword ptr [ebp - 44h], 6
        jmp    NEAR PTR loc_2_1AD0
loc_2_1AC9:
        mov    dword ptr [ebp - 44h], 0ah
loc_2_1AD0:
        jmp    NEAR PTR loc_2_1B29
loc_2_1AD5:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_1AF4
        mov    dword ptr [ebp - 44h], 7
        jmp    NEAR PTR loc_2_1AFB
loc_2_1AF4:
        mov    dword ptr [ebp - 44h], 0bh
loc_2_1AFB:
        jmp    NEAR PTR loc_2_1B29
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_1B05:
        cmp    dword ptr [ebp - 70h], 3
        ja     NEAR PTR loc_2_1B29
        mov    eax, dword ptr [ebp - 70h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_1B29:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 35h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1fa6h]
        mov    byte ptr [ebp - 18h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        mov    dword ptr [ebp - 74h], eax
        jmp    NEAR PTR loc_2_1BF6
loc_2_1B4F:
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 37h], eax
        jmp    NEAR PTR loc_2_1C26
loc_2_1B5D:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 37h], 2000h
        jmp    NEAR PTR loc_2_1C26
loc_2_1B6C:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 37h], 4000h
        jmp    NEAR PTR loc_2_1C26
loc_2_1B7B:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 37h], 8000h
        jmp    NEAR PTR loc_2_1C26
loc_2_1B8A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 37h], 10000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 35h], 1
        jmp    NEAR PTR loc_2_1C26
loc_2_1BA0:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 37h], 10000h
        jmp    NEAR PTR loc_2_1C26
loc_2_1BAF:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        push   eax
        push   6
        push   2
        push   3
        push   0aa00000ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1BCF:
        add    esp, 18h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_1BEA
        jmp    NEAR PTR loc_2_1BEA
loc_2_1BEA:
        xor    al, al
        jmp    NEAR PTR loc_2_1D3F
        db  0E9h, 030h, 000h, 000h, 000h
loc_2_1BF6:
        sub    dword ptr [ebp - 74h], 0ah
        cmp    dword ptr [ebp - 74h], 5
        ja     NEAR PTR loc_2_1BAF
        mov    eax, dword ptr [ebp - 74h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_1C26:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1fa8h]
        mov    byte ptr [ebp - 1ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 36h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1faah]
        mov    byte ptr [ebp - 18h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        mov    dword ptr [ebp - 78h], eax
        jmp    NEAR PTR loc_2_1C8D
loc_2_1C5E:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3fh], 800h
        jmp    NEAR PTR loc_2_1CA6
loc_2_1C6C:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3fh], 1000h
        jmp    NEAR PTR loc_2_1CA6
loc_2_1C7A:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3fh], 1170h
        jmp    NEAR PTR loc_2_1CA6
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1C8D:
        cmp    dword ptr [ebp - 78h], 0dh
        je     NEAR PTR loc_2_1C7A
        cmp    dword ptr [ebp - 78h], 0eh
        je     NEAR PTR loc_2_1C6C
        jmp    NEAR PTR loc_2_1C5E
loc_2_1CA6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1fach]
        mov    byte ptr [ebp - 18h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        mov    dword ptr [ebp - 7ch], eax
        jmp    NEAR PTR loc_2_1D10
loc_2_1CC5:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 41h], 800h
        jmp    NEAR PTR loc_2_1D38
loc_2_1CD3:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 41h], 1000h
        jmp    NEAR PTR loc_2_1D38
loc_2_1CE1:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 41h], 2000h
        jmp    NEAR PTR loc_2_1D38
loc_2_1CEF:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 41h], 4000h
        jmp    NEAR PTR loc_2_1D38
loc_2_1CFD:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 41h], 4628h
        jmp    NEAR PTR loc_2_1D38
        db  0E9h, 028h, 000h, 000h, 000h
loc_2_1D10:
        sub    dword ptr [ebp - 7ch], 0bh
        cmp    dword ptr [ebp - 7ch], 3
        ja     NEAR PTR loc_2_1CC5
        mov    eax, dword ptr [ebp - 7ch]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_1D38:
        mov    al, 1
        jmp    NEAR PTR loc_2_1D3F
loc_2_1D3F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_1D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        db  0A1h, 01Ch, 024h, 000h, 000h
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   32h
        push   eax
        db  0E8h, 00Ch, 004h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_1D80
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  090h
loc_2_1D80:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 2eh], 0ffffffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 18h], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [ecx + 19h], al
        add    ecx, 18h
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1eh], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 22h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 26h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 2ah], eax
        mov    al, 1
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E20:
        push   ebp
        mov    ebp, esp
        push   0
        push   32h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 04Ch, 003h, 0FEh, 0FFh
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2eh], 0
        jne    SHORT loc_2_1E59
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_1E59:
        db  0A1h, 01Ch, 024h, 000h, 000h
        push   eax
        push   0
        push   1fh
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 00Ah, 003h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_1E79
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_1E79:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 2eh]
        mov    edi, edi
loc_2_1E80:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_1E93
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_1E80
        jmp    SHORT loc_2_1EA2
loc_2_1E93:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 2eh], eax
loc_2_1EA2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_1EB8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_1EB8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 1ch], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1eh], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_2_1F20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2240
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_1F49
        cmp    ebx, 103h
        jne    SHORT loc_2_1F61
loc_2_1F49:
        push   0
        db  068h, 018h, 024h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_2010
        or     eax, eax
        je     SHORT loc_2_1F61
        mov    ebx, eax
loc_2_1F61:
        or     ebx, ebx
        je     SHORT loc_2_1F6D
        cmp    ebx, 103h
        jne    SHORT loc_2_1F98
loc_2_1F6D:
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_2_1F98
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [edi + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_2_1F98
        push   0
        db  068h, 018h, 024h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_20B0
        or     eax, eax
        je     SHORT loc_2_1F98
        mov    ebx, eax
loc_2_1F98:
        or     ebx, ebx
        je     SHORT loc_2_1FA4
        cmp    ebx, 103h
        jne    SHORT loc_2_2005
loc_2_1FA4:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_2_2000
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_1FBA
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_1FBA:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_1FC9
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_1FCF
loc_2_1FC9:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_2_1FCF:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_2_1FDE
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 0FAh, 000h, 0FEh, 0FFh
loc_2_1FDE:
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        db  0E8h, 087h, 001h, 0FEh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_2000:
        mov    ebx, 0c001000eh
loc_2_2005:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_2010:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        xor    edx, edx
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        mov    dl, byte ptr [eax]
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        xor    edx, edx
        mov    dl, byte ptr [eax + 1]
        shl    edx, 10h
        or     ecx, edx
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     ecx, eax
        mov    eax, ecx
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [edi + 4], 0
        or     eax, eax
        je     SHORT loc_2_2070
loc_2_2063:
        mov    ecx, dword ptr [eax + 0ch]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_2063
loc_2_2070:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     SHORT loc_2_20A4
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 1eh]
        or     eax, eax
        je     SHORT loc_2_20A6
        cmp    eax, 103h
        je     SHORT loc_2_20A6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0ch], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_20A4:
        sub    eax, eax
loc_2_20A6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
sub_2_20B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 14h]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        mov    al, byte ptr [ecx + 2]
        shl    edx, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 1]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        or     edx, eax
        mov    dword ptr [ebp - 4], edx
        db  0A0h, 018h, 024h, 000h, 000h
        cmp    byte ptr [ecx], al
        jne    SHORT loc_2_2158
        db  0A0h, 019h, 024h, 000h, 000h
        cmp    byte ptr [ecx + 1], al
        jne    SHORT loc_2_2158
        db  0A0h, 01Ah, 024h, 000h, 000h
        cmp    byte ptr [ecx + 2], al
        jne    SHORT loc_2_2158
        db  0A0h, 01Bh, 024h, 000h, 000h
        cmp    byte ptr [ecx + 3], al
        jne    SHORT loc_2_2158
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_2139
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [ebx + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        je     NEAR PTR loc_2_21D3
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2139:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_214D
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_214D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2158:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_2_2190
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_2174
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2174:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_21D3
        mov    byte ptr [ebx + 1dh], 1
        inc    dword ptr [esi + 0ch]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_2_2190:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 1
        jbe    SHORT loc_2_21A6
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_21A6:
        mov    ebx, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_21C0
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_2_21C0
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
loc_2_21C0:
        cmp    eax, 1
        jne    SHORT loc_2_21D3
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_21D3
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [ebx + 1dh], 0
loc_2_21D3:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   edi
        call   dword ptr [esi + 22h]
        or     eax, eax
        je     SHORT loc_2_2206
        cmp    eax, 103h
        je     SHORT loc_2_2206
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0ch], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2206:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_2220
        mov    byte ptr [ebx + 1dh], 0
        mov    dword ptr [esi + 0ch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2220:
        mov    byte ptr [ebx + 1dh], 1
        mov    dword ptr [esi + 0ch], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2240:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        mov    dword ptr [esi + 10h], 0
        je     SHORT loc_2_227D
        lea    esp, [esp]
loc_2_2270:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_2270
loc_2_227D:
        mov    ecx, dword ptr [esi + 10h]
        cmp    ebx, ecx
        je     SHORT loc_2_22B1
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 26h]
        or     eax, eax
        je     SHORT loc_2_22B3
        cmp    eax, 103h
        je     SHORT loc_2_22B3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 10h], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_22B1:
        sub    eax, eax
loc_2_22B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
_PTEXT ends
        end
