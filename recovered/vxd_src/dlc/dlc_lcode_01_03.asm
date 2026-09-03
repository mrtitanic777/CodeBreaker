; dlc_lcode_01_03.asm -- DLC object 1 segment _LTEXT  (part 3/9)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_9FEA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_A00E
loc_1_A00E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A013:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F4B8
        add    esp, 8
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_A05C
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_A472
loc_1_A05C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 44h], eax
        push   0ch
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_A09C
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_A472
loc_1_A09C:
        lea    eax, [ebp - 4ch]
        push   eax
        push   3
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F843
        add    esp, 14h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_A0E9
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_A472
loc_1_A0E9:
        mov    eax, dword ptr [ebp - 44h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_A131
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_A472
loc_1_A131:
        lea    eax, [ebp - 40h]
        mov    dword ptr [ebp - 48h], eax
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_A156
        mov    eax, dword ptr [ebp - 44h]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_A203
loc_1_A156:
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 78h]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 7ch]
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 84h]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 18h]
        mov    word ptr [ebp - 30h], ax
        mov    eax, dword ptr [ebp - 44h]
        mov    word ptr [eax + 8], 14h
        mov    eax, dword ptr [ebp - 44h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_A1FE
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 7ch], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 84h], 0
loc_1_A1FE:
        jmp    NEAR PTR loc_1_A3AB
loc_1_A203:
        mov    eax, dword ptr [ebp - 4ch]
        mov    ax, word ptr [eax + 0a9h]
        mov    word ptr [ebp - 40h], ax
        mov    eax, dword ptr [ebp - 4ch]
        mov    ax, word ptr [eax + 0abh]
        mov    word ptr [ebp - 3eh], ax
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 0adh]
        mov    byte ptr [ebp - 3ch], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 0aeh]
        mov    byte ptr [ebp - 3bh], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    ax, word ptr [eax + 0afh]
        mov    word ptr [ebp - 3ah], ax
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 0b1h]
        mov    byte ptr [ebp - 38h], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 0b2h]
        mov    byte ptr [ebp - 37h], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 36h], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 35h], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_A2A2
        xor    eax, eax
        mov    al, byte ptr [ebp - 35h]
        or     al, 40h
        mov    byte ptr [ebp - 35h], al
loc_1_A2A2:
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_A2BF
        xor    eax, eax
        mov    al, byte ptr [ebp - 35h]
        or     al, 20h
        mov    byte ptr [ebp - 35h], al
loc_1_A2BF:
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 71h]
        and    eax, 0ffh
        sar    eax, 1
        mov    byte ptr [ebp - 34h], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        sar    eax, 1
        mov    byte ptr [ebp - 33h], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        sar    eax, 1
        mov    byte ptr [ebp - 32h], al
        mov    eax, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + 0b3h]
        mov    byte ptr [ebp - 31h], al
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_A30D
loc_1_A309:
        inc    word ptr [ebp - 10h]
loc_1_A30D:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 20h
        jge    NEAR PTR loc_1_A342
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4ch]
        mov    al, byte ptr [eax + ecx + 0b4h]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        mov    byte ptr [ebp + ecx - 30h], al
        jmp    NEAR PTR loc_1_A309
loc_1_A342:
        mov    eax, dword ptr [ebp - 44h]
        mov    word ptr [eax + 8], 30h
        mov    eax, dword ptr [ebp - 44h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_A3AB
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 8
        jle    NEAR PTR loc_1_A3AB
        mov    eax, dword ptr [ebp - 4ch]
        mov    word ptr [eax + 0a9h], 0
        mov    eax, dword ptr [ebp - 4ch]
        mov    word ptr [eax + 0abh], 0
        mov    eax, dword ptr [ebp - 4ch]
        mov    byte ptr [eax + 0adh], 0
        mov    eax, dword ptr [ebp - 4ch]
        mov    byte ptr [eax + 0aeh], 0
        mov    eax, dword ptr [ebp - 4ch]
        mov    word ptr [eax + 0afh], 0
loc_1_A3AB:
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_A3BA
loc_1_A3B6:
        inc    word ptr [ebp - 10h]
loc_1_A3BA:
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 44h]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_A3DD
        mov    eax, ecx
loc_1_A3DD:
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_A410
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 48h]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_A3B6
loc_1_A410:
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 44h]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_A454
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_A472
        db  0E9h, 01Eh, 000h, 000h, 000h
loc_1_A454:
        push   15h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_A472
loc_1_A472:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A477:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_A4AD
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_A4AD
        jmp    NEAR PTR loc_1_A93A
loc_1_A4AD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   al, 0eh
        je     NEAR PTR loc_1_A63B
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0fff1h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   al, 8
        je     NEAR PTR loc_1_A544
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    byte ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_A598
loc_1_A544:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   al, 2
        je     NEAR PTR loc_1_A55D
        mov    byte ptr [ebp - 4], 4dh
        jmp    NEAR PTR loc_1_A598
loc_1_A55D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14573
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_A594
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 84h], 0
loc_1_A594:
        mov    byte ptr [ebp - 4], 0
loc_1_A598:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_A5E3
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 14h], 0
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp - 1ch]
        add    esp, 10h
        jmp    NEAR PTR loc_1_A62F
loc_1_A5E3:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   al, 8
        jne    NEAR PTR loc_1_A603
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   al, 2
        je     NEAR PTR loc_1_A62F
loc_1_A603:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_A62F:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0fff1h
        mov    word ptr [ebp - 14h], ax
loc_1_A63B:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A65F
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_A664
loc_1_A65F:
        jmp    NEAR PTR loc_1_A93A
loc_1_A664:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   ah, 4
        je     NEAR PTR loc_1_A69F
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 8fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_A69F
        mov    word ptr [ebp - 14h], 800h
        mov    ax, word ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_A69F:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, 0ffffffbfh
        je     NEAR PTR loc_1_A6E9
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_A6E9
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_A6E9:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   ah, 20h
        je     NEAR PTR loc_1_A730
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8bh]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 18h]
        add    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 86h
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], al
loc_1_A730:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   ah, 0f8h
        je     NEAR PTR loc_1_A7A5
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_A75E
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_A75E:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_A795
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_A795:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22BFE
        add    esp, 8
loc_1_A7A5:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   ah, 30h
        je     NEAR PTR loc_1_A81B
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 86h
        push   eax
        push   5
        push   5
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 1ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_A81B:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 68h], 0
        je     NEAR PTR loc_1_A93A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffdfh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_A93A
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 20h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_A8BE
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   8
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 20h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_A8BE
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   8
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        add    eax, 40h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_A93A
loc_1_A8BE:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_A8DB
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_A8DB:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_A912
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_A912:
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1A1CB
        add    esp, 0ch
        push   0
        push   0
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
loc_1_A93A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A93F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 4
        mov    byte ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_A95B
loc_1_A957:
        inc    word ptr [ebp - 8]
loc_1_A95B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        add    eax, 4
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_AA51
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_AA51
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_AA4C
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_119D8
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        sub    eax, 3
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 52h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 52h], al
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        inc    byte ptr [eax + 63h]
        mov    eax, dword ptr [ebp + 8]
        dec    byte ptr [eax + 6ch]
        inc    byte ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_AA4C
        jmp    NEAR PTR loc_1_AA51
loc_1_AA4C:
        jmp    NEAR PTR loc_1_A957
loc_1_AA51:
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_1_AA59
loc_1_AA59:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_AA60:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_AA9D
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_AA93
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_AA93:
        mov    eax, 1
        jmp    NEAR PTR loc_1_AE11
loc_1_AA9D:
        call   sub_1_117F5
        push   1
        push   1000h
        call   sub_1_019A
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_AACA
        xor    eax, eax
        jmp    NEAR PTR loc_1_AE11
loc_1_AACA:
        push   1
        push   1000h
        call   sub_1_019A
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_AB11
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_AE11
loc_1_AB11:
        push   0
        call   sub_1_00BB
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_AB50
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_AE11
loc_1_AB50:
        call   sub_1_0159
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_AB88
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_AB83
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_AB83:
        jmp    NEAR PTR loc_1_ABAC
loc_1_AB88:
        push   2bh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   68h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_AE11
loc_1_ABAC:
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_F323
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_ABCB
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_AE11
loc_1_ABCB:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_1_ABF1
        xor    eax, eax
        db  066h, 0A1h, 004h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_ABF1:
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_F323
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_AC10
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_AE11
loc_1_AC10:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_1_AC36
        xor    eax, eax
        db  066h, 0A1h, 004h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_AC36:
        lea    eax, [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_E002
        add    esp, 8
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_48BC
        add    esp, 8
        cmp    eax, 1
        je     NEAR PTR loc_1_AC88
        push   9
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   82h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_AE11
loc_1_AC88:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    eax, [eax + eax*2]
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_1_18D76
        cmp    eax, 1
        je     NEAR PTR loc_1_ACCA
        push   3
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   8eh
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_AE11
loc_1_ACCA:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_E060
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_AD05
        push   0ah
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   96h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_AE11
loc_1_AD05:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_ADFD
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 40h]
        mov    ecx, 0ah
        rep movsd
        movsw
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 40h]
        push   eax
        push   80000002h
        call   sub_1_068C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_ADAA
        lea    eax, [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_068C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_AD69
        mov    dword ptr [ebp - 44h], 0
        jmp    NEAR PTR loc_1_AD8D
loc_1_AD69:
        mov    dword ptr [ebp - 44h], 1
        lea    eax, [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0693
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_AD8D
loc_1_AD8D:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0699
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0699
        add    esp, 4
        jmp    NEAR PTR loc_1_ADAA
loc_1_ADAA:
        call   sub_1_04E7
        test   eax, eax
        je     NEAR PTR loc_1_ADFD
        cmp    dword ptr [ebp - 44h], 0
        jne    NEAR PTR loc_1_ADFD
        push   3e8h
        push   0
        push   19h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_0106
        add    esp, 10h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_1_ADFD
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_ADFD:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_AE11
loc_1_AE11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_AE18:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_AE38
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_B026
loc_1_AE38:
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_AE70
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 8
        je     NEAR PTR loc_1_AE64
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_AE64:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_B01E
loc_1_AE70:
        cmp    dword ptr [ebp + 8], 1
        jne    NEAR PTR loc_1_AE96
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_B02D
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_B01E
loc_1_AE96:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_AEAF
        mov    dword ptr [ebp - 0ch], 1fh
        jmp    NEAR PTR loc_1_B01E
loc_1_AEAF:
        cmp    dword ptr [ebp + 8], -1
        jne    NEAR PTR loc_1_AFF4
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 2ch], ecx
        jne    NEAR PTR loc_1_AEE0
        call   sub_1_201F0
loc_1_AEE0:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 8
        je     NEAR PTR loc_1_AF13
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_AF13:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_AF22
loc_1_AF1E:
        inc    word ptr [ebp - 8]
loc_1_AF22:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_AFE8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_AFE3
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_AFE3
        push   0
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_AF8B
loc_1_AF83:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_AF8B:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_AFD4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_AFCF
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 10h]
        cmp    dword ptr [eax + 2ch], ecx
        jne    NEAR PTR loc_1_AFCF
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_46AA
        add    esp, 8
        jmp    NEAR PTR loc_1_AFD4
loc_1_AFCF:
        jmp    NEAR PTR loc_1_AF83
loc_1_AFD4:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_AFE3:
        jmp    NEAR PTR loc_1_AF1E
loc_1_AFE8:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_B01E
loc_1_AFF4:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], 32h
loc_1_B01E:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_B026
loc_1_B026:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_B02D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_B072
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_B1D1
loc_1_B072:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 14h], 4ah
        jae    NEAR PTR loc_1_B0BC
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_B1D1
loc_1_B0BC:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 0b0h
        jne    NEAR PTR loc_1_B0F7
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_B0F7
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_36F6
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_B1C9
loc_1_B0F7:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 0b1h
        jne    NEAR PTR loc_1_B136
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_B136
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_3A74
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_B1C9
loc_1_B136:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 0f0h
        jl     NEAR PTR loc_1_B17E
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 0f8h
        jg     NEAR PTR loc_1_B17E
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_20344
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_B1C9
loc_1_B17E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B197
        mov    dword ptr [ebp - 8], 1fh
        jmp    NEAR PTR loc_1_B1C9
loc_1_B197:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], 75h
loc_1_B1C9:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_B1D1
loc_1_B1D1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B1D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_B1E4:
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B1EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_B21C
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B21C
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B223
loc_1_B21C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_B3D8
loc_1_B223:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B254
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_B254:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        call   sub_1_EDEA
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_B278
loc_1_B274:
        inc    word ptr [ebp - 8]
loc_1_B278:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_B345
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_B340
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_B2E4
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_B2E4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_B3E0
        add    esp, 4
loc_1_B2E4:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_B2FA
loc_1_B2F2:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_B2FA:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_B319
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_46AA
        add    esp, 8
        jmp    NEAR PTR loc_1_B2F2
loc_1_B319:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_18F4B
        add    esp, 4
loc_1_B340:
        jmp    NEAR PTR loc_1_B274
loc_1_B345:
        call   sub_1_19331
        call   sub_1_2025E
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B371
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_00CA
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B371:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B396
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A0
        add    esp, 4
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B396:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A0
        add    esp, 4
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B3CE
        call   sub_1_0512
loc_1_B3CE:
        mov    eax, 1
        jmp    NEAR PTR loc_1_B3D8
loc_1_B3D8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_B3E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 104h], 103h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28E9E
        cmp    dword ptr [ebp - 4], 103h
        je     NEAR PTR loc_1_B43C
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 104h], eax
        jmp    NEAR PTR loc_1_B455
loc_1_B43C:
        push   0bb8h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 104h
        push   eax
        call   sub_1_EB38
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
loc_1_B455:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_B4C3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 0
        jne    NEAR PTR loc_1_B4C3
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B49C
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_B49C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11EFB
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_B51F
        db  0E9h, 05Ch, 000h, 000h, 000h
loc_1_B4C3:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B4FA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 104h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_B4FA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11EFB
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_B51F
loc_1_B51F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B524:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_B53C
loc_1_B538:
        inc    word ptr [ebp - 4]
loc_1_B53C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_B55A
        mov    eax, ecx
loc_1_B55A:
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_B590
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_B538
loc_1_B590:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B5B3:
        push   ebp
        mov    ebp, esp
        sub    esp, 64h
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B5D7
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
        jmp    NEAR PTR loc_1_B8A8
loc_1_B5D7:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B5F8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_B5F8:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B63D
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        dec    eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_B63D:
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_B712
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_01C9
        add    esp, 0ch
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_B6EA
        push   1
        push   50h
        lea    eax, [ebp - 64h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_01CF
        add    esp, 14h
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_B6BD
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B6B8
        lea    eax, [ebp - 64h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_B6B8:
        jmp    NEAR PTR loc_1_B6E5
loc_1_B6BD:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B6E5:
        jmp    NEAR PTR loc_1_B712
loc_1_B6EA:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B712:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_48BC
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 1
        je     NEAR PTR loc_1_B755
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   106h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
        jmp    NEAR PTR loc_1_B8A8
loc_1_B755:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_18850
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_B79E
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
        jmp    NEAR PTR loc_1_B8A8
loc_1_B79E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 58h], eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_4BE6
        add    esp, 4
        push   10h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0cch
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0c4h
        push   eax
        call   sub_1_B524
        add    esp, 10h
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 0c4h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_B813
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0c4h
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_DD35
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_B851
loc_1_B813:
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 0c4h], 0
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_DD35
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_1_B851:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_B89F
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_B86F
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 75h], 0
loc_1_B86F:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_E8E1
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_B891
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_1_B89A
loc_1_B891:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_1_B89A:
        jmp    NEAR PTR loc_1_B8A8
loc_1_B89F:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_1_B8A8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_B8AF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B8C7
        jmp    NEAR PTR loc_1_B960
loc_1_B8C7:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B8F8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_B8F8:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_B954
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B954:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 104h], eax
loc_1_B960:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_B967:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B982
        jmp    NEAR PTR loc_1_BA63
loc_1_B982:
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_B9C5
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_B9B9
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_B9B9:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_BA22
loc_1_B9C5:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 0c0000001h
loc_1_BA22:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 104h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 58h], 0
        je     NEAR PTR loc_1_BA63
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 58h]
        push   eax
        call   sub_1_28EC8
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_BA63
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 58h], 0
loc_1_BA63:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_1_BA6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 64h
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_BA94
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_BA9E
loc_1_BA94:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BA9E:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_BABC
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_BABC
        cmp    dword ptr [ebp + 14h], 0dh
        jae    NEAR PTR loc_1_BAE3
loc_1_BABC:
        push   1ah
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   231h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BAE3:
        cmp    dword ptr [ebp + 1ch], 3
        jae    NEAR PTR loc_1_BB1F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BB1F:
        mov    byte ptr [ebp - 13h], 2
        mov    dword ptr [ebp - 60h], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_BC5C
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 0c0h
        jne    NEAR PTR loc_1_BB62
        mov    word ptr [ebp - 3ch], 1
        mov    dword ptr [ebp - 28h], 0
        jmp    NEAR PTR loc_1_BBF2
loc_1_BB62:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0c0h
        cmp    al, 40h
        jne    NEAR PTR loc_1_BBE8
        mov    word ptr [ebp - 3ch], 2
        cmp    dword ptr [ebp + 14h], 0eh
        jbe    NEAR PTR loc_1_BBDC
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp - 28h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        mov    word ptr [ebp - 58h], ax
        mov    eax, dword ptr [ebp - 58h]
        and    eax, 0ffffh
        cmp    eax, 12h
        jle    NEAR PTR loc_1_BBD7
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BBD7:
        jmp    NEAR PTR loc_1_BBE3
loc_1_BBDC:
        mov    dword ptr [ebp - 28h], 0
loc_1_BBE3:
        jmp    NEAR PTR loc_1_BBF2
loc_1_BBE8:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BBF2:
        mov    byte ptr [ebp - 36h], 10h
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 35h], al
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 12h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 32h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebp - 34h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ah]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 2eh], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 34h]
        test   al, 80h
        je     NEAR PTR loc_1_BC51
        mov    word ptr [ebp - 38h], 1
        jmp    NEAR PTR loc_1_BC57
loc_1_BC51:
        mov    word ptr [ebp - 38h], 0
loc_1_BC57:
        jmp    NEAR PTR loc_1_BEEE
loc_1_BC5C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_BDEF
        cmp    dword ptr [ebp + 14h], 0eh
        je     NEAR PTR loc_1_BC9A
        push   1bh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2a2h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BC9A:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 5ch], eax
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 5ch]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        mov    word ptr [ebp - 4ch], ax
        mov    ax, word ptr [ebp - 4ch]
        mov    word ptr [ebp - 3ah], ax
        xor    eax, eax
        mov    ax, word ptr [ebp - 3ah]
        cmp    eax, 5dch
        jle    NEAR PTR loc_1_BCF6
        mov    eax, dword ptr [ebp - 4ch]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_BD76
loc_1_BCF6:
        xor    eax, eax
        mov    ax, word ptr [ebp - 3ah]
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_BD48
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        mov    word ptr [ebp - 4ch], ax
        add    dword ptr [ebp + 18h], 3
        sub    dword ptr [ebp + 1ch], 3
        xor    eax, eax
        mov    al, byte ptr [ebp - 13h]
        or     al, 20h
        mov    byte ptr [ebp - 13h], al
        mov    dword ptr [ebp - 60h], 3
loc_1_BD48:
        mov    word ptr [ebp - 3ch], 2
        mov    eax, dword ptr [ebp - 4ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp + 20h], eax
        mov    eax, dword ptr [ebp - 4ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_BD6E
        mov    eax, ecx
loc_1_BD6E:
        mov    dword ptr [ebp + 1ch], eax
        jmp    NEAR PTR loc_1_BD7C
loc_1_BD76:
        mov    word ptr [ebp - 3ch], 3
loc_1_BD7C:
        mov    byte ptr [ebp - 36h], 10h
        mov    byte ptr [ebp - 35h], 40h
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 28h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 12h], eax
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 2]
        mov    dword ptr [ebp - 32h], eax
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 34h], ax
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 2eh], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 34h]
        xor    ecx, ecx
        db  08Ah, 00Dh, 080h, 000h, 000h, 000h
        test   ecx, eax
        je     NEAR PTR loc_1_BDE4
        mov    word ptr [ebp - 38h], 1
        jmp    NEAR PTR loc_1_BDEA
loc_1_BDE4:
        mov    word ptr [ebp - 38h], 0
loc_1_BDEA:
        jmp    NEAR PTR loc_1_BEEE
loc_1_BDEF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 2
        jne    NEAR PTR loc_1_BEEE
        cmp    dword ptr [ebp + 14h], 0dh
        je     NEAR PTR loc_1_BE2D
        push   1bh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   300h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BE2D:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 48h], eax
        mov    eax, dword ptr [ebp - 48h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 30h
        jne    NEAR PTR loc_1_BE54
        mov    word ptr [ebp - 3ch], 1
        mov    byte ptr [ebp - 35h], 0
        jmp    NEAR PTR loc_1_BE7D
loc_1_BE54:
        mov    eax, dword ptr [ebp - 48h]
        mov    al, byte ptr [eax]
        and    eax, 30h
        cmp    al, 10h
        jne    NEAR PTR loc_1_BE73
        mov    word ptr [ebp - 3ch], 2
        mov    byte ptr [ebp - 35h], 40h
        jmp    NEAR PTR loc_1_BE7D
loc_1_BE73:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_BE7D:
        mov    dword ptr [ebp - 28h], 0
        mov    byte ptr [ebp - 36h], 10h
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 12h], eax
        mov    eax, dword ptr [ebp - 48h]
        mov    eax, dword ptr [eax + 3]
        mov    dword ptr [ebp - 32h], eax
        mov    eax, dword ptr [ebp - 48h]
        mov    ax, word ptr [eax + 1]
        mov    word ptr [ebp - 34h], ax
        mov    eax, dword ptr [ebp - 48h]
        mov    eax, dword ptr [eax + 9]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 48h]
        mov    ax, word ptr [eax + 7]
        mov    word ptr [ebp - 2eh], ax
        mov    eax, dword ptr [ebp - 48h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        xor    ecx, ecx
        db  08Ah, 00Dh, 080h, 000h, 000h, 000h
        test   ecx, eax
        je     NEAR PTR loc_1_BEE8
        mov    word ptr [ebp - 38h], 1
        jmp    NEAR PTR loc_1_BEEE
loc_1_BEE8:
        mov    word ptr [ebp - 38h], 0
loc_1_BEEE:
        lea    eax, [ebp - 64h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 50h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E73A
        add    esp, 14h
        mov    dword ptr [ebp - 44h], eax
        cmp    dword ptr [ebp - 44h], 10003h
        jne    NEAR PTR loc_1_BF22
        mov    eax, dword ptr [ebp - 44h]
        jmp    NEAR PTR loc_1_C238
loc_1_BF22:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_BF44
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_BF44:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_BF7D
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_BF7D:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_C1EE
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp - 50h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 50h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 40h], eax
        cmp    dword ptr [ebp - 40h], 0
        je     NEAR PTR loc_1_BFC5
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 40h]
        add    eax, 18h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_C023
loc_1_BFC5:
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C010
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 30h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_C023
loc_1_C010:
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_195C4
        add    esp, 8
loc_1_C023:
        lea    eax, [ebp - 54h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 64h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        add    eax, dword ptr [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        call   sub_1_28EAA
        cmp    dword ptr [ebp - 44h], 103h
        je     NEAR PTR loc_1_C1C9
        cmp    dword ptr [ebp - 40h], 0
        je     NEAR PTR loc_1_C079
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 40h]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        jmp    NEAR PTR loc_1_C0D7
loc_1_C079:
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C0C4
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 30h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        jmp    NEAR PTR loc_1_C0D7
loc_1_C0C4:
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_1962D
        add    esp, 8
loc_1_C0D7:
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 4]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp - 50h]
        mov    byte ptr [ecx + 4], al
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_D1F8
        add    esp, 4
        cmp    dword ptr [ebp - 44h], 0
        jne    NEAR PTR loc_1_C108
        mov    eax, dword ptr [ebp - 64h]
        cmp    dword ptr [ebp - 54h], eax
        je     NEAR PTR loc_1_C1C4
loc_1_C108:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 54h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C185
        push   1
        push   1
        mov    eax, dword ptr [ebp - 50h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
loc_1_C185:
        mov    eax, dword ptr [ebp - 50h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_C1AE
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_C1BA
loc_1_C1AE:
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_C1BA:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_C238
loc_1_C1C4:
        jmp    NEAR PTR loc_1_C1EE
loc_1_C1C9:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_C1E7
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_C1E7:
        xor    eax, eax
        jmp    NEAR PTR loc_1_C238
loc_1_C1EE:
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_C223
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_C223:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F2C4
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_C238
loc_1_C238:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    1ch
loc_1_C23F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_C266
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_C26B
loc_1_C266:
        jmp    NEAR PTR loc_1_C29B
loc_1_C26B:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_C28D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_C28D:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F2C4
        add    esp, 8
loc_1_C29B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_C2A2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C2BA
        jmp    NEAR PTR loc_1_C355
loc_1_C2BA:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C2DF
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_C2DF:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_C34C
        cmp    dword ptr [ebp + 10h], 0c00000bbh
        je     NEAR PTR loc_1_C34C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_C34C:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 28h], eax
loc_1_C355:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_1_C35C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C374
        jmp    NEAR PTR loc_1_C41A
loc_1_C374:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C3A5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_C3A5:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_C40E
        cmp    dword ptr [ebp + 0ch], 80010003h
        je     NEAR PTR loc_1_C40E
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_C40E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 104h], eax
loc_1_C41A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_C421:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C43C
        jmp    NEAR PTR loc_1_C75A
loc_1_C43C:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_C46D
        push   34h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4bah
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_C46D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 1ch], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C4A3
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_C4A3:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_C74C
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 38h], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_C50B
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_C50B:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 5
        je     NEAR PTR loc_1_C533
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 4
        jne    NEAR PTR loc_1_C66A
loc_1_C533:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 5
        jne    NEAR PTR loc_1_C62B
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_C588
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_C626
loc_1_C588:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_C5C5
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffh
        add    eax, 3
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_C626
loc_1_C5C5:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_C603
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_C626
loc_1_C603:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffh
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 10h], eax
loc_1_C626:
        jmp    NEAR PTR loc_1_C64F
loc_1_C62B:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 10h], eax
loc_1_C64F:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_C704
loc_1_C66A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 6
        jne    NEAR PTR loc_1_C6B2
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 30h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_C704
loc_1_C6B2:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 7
        jne    NEAR PTR loc_1_C6EE
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_C6EE
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_C704
loc_1_C6EE:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
loc_1_C704:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_C72F
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_C74C
loc_1_C72F:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C74C
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_C74C:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F2C4
        add    esp, 8
loc_1_C75A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_1_C761:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C77C
        jmp    NEAR PTR loc_1_C872
loc_1_C77C:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_C7C3
        cmp    dword ptr [ebp + 0ch], 40010006h
        jne    NEAR PTR loc_1_C7C3
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C7BE
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_C7BE:
        jmp    NEAR PTR loc_1_C7E8
loc_1_C7C3:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C7E8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_C7E8:
        call   sub_1_19428
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_C853
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_C831
        cmp    dword ptr [ebp + 0ch], 40010006h
        jne    NEAR PTR loc_1_C831
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_1_C83B
loc_1_C831:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
loc_1_C83B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_C872
loc_1_C853:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_C872:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
loc_1_C879:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C891
        jmp    NEAR PTR loc_1_C8C0
loc_1_C891:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C8B2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_C8B2:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F2C4
        add    esp, 8
loc_1_C8C0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_C8C7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C8E2
        jmp    NEAR PTR loc_1_CDAC
loc_1_C8E2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_CD50
loc_1_C8F0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        je     NEAR PTR loc_1_C90A
        jmp    NEAR PTR loc_1_CDA0
loc_1_C90A:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B3E0
        add    esp, 4
        db  068h, 000h, 000h, 000h, 000h
        push   8000h
        push   2
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
        jmp    NEAR PTR loc_1_CDA0
loc_1_C997:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C9B4
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_C9B4:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_C9F5
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_C9F5:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2], 4
        jmp    NEAR PTR loc_1_CDA0
loc_1_CA03:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2], 2
        db  068h, 000h, 000h, 000h, 000h
        push   8000h
        push   2
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
        jmp    NEAR PTR loc_1_CDA0
loc_1_CA8D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 2
        je     NEAR PTR loc_1_CAA7
        jmp    NEAR PTR loc_1_CDA0
loc_1_CAA7:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2], 4
        jmp    NEAR PTR loc_1_CDA0
loc_1_CB17:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        je     NEAR PTR loc_1_CB31
        jmp    NEAR PTR loc_1_CDA0
loc_1_CB31:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        and    dword ptr [ebp - 4], 3ffh
        shr    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0fffffc00h
        or     dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 8], 800h
        je     NEAR PTR loc_1_CB85
        cmp    dword ptr [ebp - 8], 400h
        je     NEAR PTR loc_1_CB85
        cmp    dword ptr [ebp - 8], 200h
        jne    NEAR PTR loc_1_CC0B
loc_1_CB85:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B3E0
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   1
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
        jmp    NEAR PTR loc_1_CC7C
loc_1_CC0B:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_CC28
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_CC28:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_CC66
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_CC66:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        push   20h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
loc_1_CC7C:
        jmp    NEAR PTR loc_1_CDA0
loc_1_CC81:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_CC9E
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_CC9E:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_CCDF
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_CCDF:
        jmp    NEAR PTR loc_1_CDA0
loc_1_CCE4:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_CDA0
        db  0E9h, 050h, 000h, 000h, 000h
loc_1_CD50:
        cmp    dword ptr [ebp - 0ch], 40010003h
        ja     NEAR PTR loc_1_CD75
        je     NEAR PTR loc_1_C997
        cmp    dword ptr [ebp - 0ch], 10004h
        je     NEAR PTR loc_1_CC81
        jmp    NEAR PTR loc_1_CCE4
loc_1_CD75:
        sub    dword ptr [ebp - 0ch], 40010004h
        cmp    dword ptr [ebp - 0ch], 3
        ja     NEAR PTR loc_1_CCE4
        mov    eax, dword ptr [ebp - 0ch]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_CDA0:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_CDAC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_CDB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_CDCC
        jmp    NEAR PTR loc_1_D000
loc_1_CDCC:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_CDFD
        push   34h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   6bah
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_CDFD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_CE34
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_CE98
loc_1_CE34:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_CE82
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 30h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_CE98
loc_1_CE82:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
loc_1_CE98:
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_D000
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 4], al
        lea    eax, [ebp + 0ch]
        push   eax
        call   sub_1_D1F8
        add    esp, 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_CEDF
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_CEDF:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_CF09
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_CF09:
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_CF5B
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_CF48
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_CF48:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F2C4
        add    esp, 8
        jmp    NEAR PTR loc_1_D000
loc_1_CF5B:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_CFCB
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        push   1
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
loc_1_CFCB:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_CFF4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_D000
loc_1_CFF4:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_D000:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
loc_1_D007:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_D02B
        jmp    NEAR PTR loc_1_D151
loc_1_D02B:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D04C
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_D04C:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_D05B
loc_1_D057:
        inc    word ptr [ebp - 4]
loc_1_D05B:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_D098
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_D093
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        db  039h, 00Ch, 085h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_D093
        jmp    NEAR PTR loc_1_D098
loc_1_D093:
        jmp    NEAR PTR loc_1_D057
loc_1_D098:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 10h
        jne    NEAR PTR loc_1_D0D1
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_D151
loc_1_D0D1:
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_D110
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_D110
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_B3E0
        add    esp, 4
loc_1_D110:
        db  068h, 000h, 000h, 000h, 000h
        push   8000h
        push   2
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_18F4B
        add    esp, 4
loc_1_D151:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_1_D158:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D17E
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_D17E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_D1F3
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28E92
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D1D0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D1C1
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_D1C1:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_D1F3
loc_1_D1D0:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_D1F3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D1F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_28F28
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_D225
loc_1_D222:
        inc    dword ptr [ebp - 8]
loc_1_D225:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jge    NEAR PTR loc_1_D25F
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_28F6A
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_D253
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F46
loc_1_D253:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_D222
loc_1_D25F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_28F22
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D278:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 108h
        push   eax
        push   10111h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D30A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 108h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_D2D0
        mov    eax, ecx
loc_1_D2D0:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 8], ax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D305
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_D305:
        jmp    NEAR PTR loc_1_D35A
loc_1_D30A:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_D941
loc_1_D35A:
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 108h
        push   eax
        push   10107h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D3DC
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 2710h
        mov    eax, dword ptr [eax + 108h]
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 97h], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D3D7
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 2710h
        mov    eax, dword ptr [eax + 108h]
        sub    edx, edx
        div    ecx
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_D3D7:
        jmp    NEAR PTR loc_1_D42C
loc_1_D3DC:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_D941
loc_1_D42C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_D718
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 83h
        push   eax
        push   2010103h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D4D0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D4CB
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 83h]
        shl    eax, 18h
        and    eax, 0ff000000h
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 83h]
        shr    ecx, 18h
        mov    al, cl
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 83h]
        shl    ecx, 8
        and    ecx, 0ff0000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 83h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_D4CB:
        jmp    NEAR PTR loc_1_D520
loc_1_D4D0:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_D941
loc_1_D520:
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 87h
        push   eax
        push   2010104h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D5BD
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D5B8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 87h]
        shl    eax, 8
        and    eax, 0ff0000h
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 87h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 87h]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 87h]
        shr    ecx, 18h
        and    ecx, 0ffh
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_D5B8:
        jmp    NEAR PTR loc_1_D60D
loc_1_D5BD:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_D941
loc_1_D60D:
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 93h
        push   eax
        push   2010106h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D666
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D661
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 93h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_D661:
        jmp    NEAR PTR loc_1_D705
loc_1_D666:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D683
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_D683:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D6A8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_D6A8:
        cmp    dword ptr [ebp - 4], 0c00000bbh
        je     NEAR PTR loc_1_D705
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_D941
loc_1_D705:
        mov    dword ptr [ebp - 10h], 2010101h
        mov    dword ptr [ebp - 8], 2010102h
        jmp    NEAR PTR loc_1_D753
loc_1_D718:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_D738
        mov    dword ptr [ebp - 10h], 1010101h
        mov    dword ptr [ebp - 8], 1010102h
        jmp    NEAR PTR loc_1_D753
loc_1_D738:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 2
        jne    NEAR PTR loc_1_D753
        mov    dword ptr [ebp - 10h], 3010101h
        mov    dword ptr [ebp - 8], 3010102h
loc_1_D753:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 77h
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D7AA
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D7A5
        mov    eax, dword ptr [ebp + 8]
        add    eax, 77h
        push   eax
        push   10h
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_D7A5:
        jmp    NEAR PTR loc_1_D7FA
loc_1_D7AA:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_D941
loc_1_D7FA:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 7dh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D851
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D84C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 7dh
        push   eax
        push   10h
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_D84C:
        jmp    NEAR PTR loc_1_D8A1
loc_1_D851:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_D941
loc_1_D8A1:
        push   4
        lea    eax, [ebp - 0ch]
        push   eax
        push   1010fh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_D8F4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_D8EF
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_D8EF:
        jmp    NEAR PTR loc_1_D93A
loc_1_D8F4:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_D93A:
        xor    eax, eax
        jmp    NEAR PTR loc_1_D941
loc_1_D941:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D946:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_D968
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_D995
loc_1_D968:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_D981
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_D995
loc_1_D981:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 2
        jne    NEAR PTR loc_1_D995
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
loc_1_D995:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_D9A4
loc_1_D9A1:
        inc    dword ptr [ebp - 0ch]
loc_1_D9A4:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 0bh
        jb     NEAR PTR loc_1_D9B5
        mov    eax, 0bh
loc_1_D9B5:
        cmp    eax, dword ptr [ebp - 0ch]
        jbe    NEAR PTR loc_1_DB24
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ecx + eax*4], 0
        je     NEAR PTR loc_1_DB1F
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 108h
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ecx + eax*4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DBBC
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_DA4C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        sub    eax, dword ptr [edx + ecx*4 + 98h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0ffh
        ja     NEAR PTR loc_1_DA28
        mov    eax, 0ffh
loc_1_DA28:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 98h], eax
        jmp    NEAR PTR loc_1_DB1F
loc_1_DA4C:
        cmp    dword ptr [ebp - 8], 0c00000bbh
        je     NEAR PTR loc_1_DA66
        cmp    dword ptr [ebp - 8], 0c0010017h
        jne    NEAR PTR loc_1_DAC1
loc_1_DA66:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_DA83
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_DA83:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_DAB2
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ecx + eax*4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_DAB2:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + ecx], 0
        jmp    NEAR PTR loc_1_DB1F
loc_1_DAC1:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ecx + eax*4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_DB2B
loc_1_DB1F:
        jmp    NEAR PTR loc_1_D9A1
loc_1_DB24:
        xor    eax, eax
        jmp    NEAR PTR loc_1_DB2B
loc_1_DB2B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DB30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 60h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28F3A
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_DB6C
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_DBB7
        db  0E9h, 04Bh, 000h, 000h, 000h
loc_1_DB6C:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   8
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   937h
        call   sub_1_FAD7
        add    esp, 0ch
        xor    eax, eax
        jmp    NEAR PTR loc_1_DBB7
loc_1_DBB7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DBBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ech], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0f0h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0f4h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0f8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 104h], 103h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0dch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28EB6
        cmp    dword ptr [ebp - 4], 103h
        je     NEAR PTR loc_1_DC51
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_DCC9
loc_1_DC51:
        push   0bb8h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 104h
        push   eax
        call   sub_1_EB38
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_DC91
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 104h]
        jmp    NEAR PTR loc_1_DCC9
        db  0E9h, 038h, 000h, 000h, 000h
loc_1_DC91:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_DCC9
loc_1_DCC9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DCCE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 64h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_28F16
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_DD02
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_DD30
        db  0E9h, 02Eh, 000h, 000h, 000h
loc_1_DD02:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_DD30
loc_1_DD30:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DD35:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_DD7A
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_DFFD
loc_1_DD7A:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 104h], 103h
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   10h
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 54h
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28E98
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_DDD8
loc_1_DDD4:
        inc    word ptr [ebp - 10h]
loc_1_DDD8:
        cmp    dword ptr [ebp - 4], 103h
        je     NEAR PTR loc_1_DDFD
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 104h], eax
        jmp    NEAR PTR loc_1_DE16
loc_1_DDFD:
        push   0bb8h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 104h
        push   eax
        call   sub_1_EB38
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
loc_1_DE16:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_DF1C
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 0
        jne    NEAR PTR loc_1_DF1C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 5ch], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_DE89
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 5ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_DE89:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D278
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_DED6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2], 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 83h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8bh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 87h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8fh], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_DFFD
        db  0E9h, 041h, 000h, 000h, 000h
loc_1_DED6:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B3E0
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18F4B
        add    esp, 4
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_DFFD
        db  0E9h, 0DCh, 000h, 000h, 000h
loc_1_DF1C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 103h
        jne    NEAR PTR loc_1_DF73
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_DF69
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_DF69:
        jmp    NEAR PTR loc_1_DDD4
        db  0E9h, 085h, 000h, 000h, 000h
loc_1_DF73:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        dec    eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 104h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18F4B
        add    esp, 4
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_DFFD
        db  0E9h, 0D7h, 0FDh, 0FFh, 0FFh
loc_1_DFFD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E002:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_E01A
loc_1_E016:
        inc    word ptr [ebp - 4]
loc_1_E01A:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + ecx]
        test   eax, eax
        je     NEAR PTR loc_1_E036
        jmp    NEAR PTR loc_1_E016
loc_1_E036:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        inc    eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E060:
        push   ebp
        mov    ebp, esp
        sub    esp, 5ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_E078
loc_1_E074:
        inc    word ptr [ebp - 18h]
loc_1_E078:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        db  00Fh, 0BEh, 080h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_1_E0AF
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        mov    byte ptr [ebp + ecx - 10h], al
        jmp    NEAR PTR loc_1_E074
loc_1_E0AF:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    byte ptr [ebp + eax - 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 5ch], ah
        mov    al, byte ptr [ebp + 8]
        mov    byte ptr [ebp - 5bh], al
        mov    dword ptr [ebp - 5ah], 0
        db  0C7h, 045h, 0AAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0AEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp - 18h]
        mov    word ptr [ebp - 22h], ax
        mov    ax, word ptr [ebp - 18h]
        mov    word ptr [ebp - 20h], ax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 1eh], eax
        push   42h
        lea    eax, [ebp - 5ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_28E8C
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_E187
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_1_E17B
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_E17B:
        xor    eax, eax
        jmp    NEAR PTR loc_1_E1B4
        db  0E9h, 02Dh, 000h, 000h, 000h
loc_1_E187:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_E1B4
loc_1_E1B4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E1B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 104h], 103h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28EB0
        cmp    dword ptr [ebp - 4], 103h
        je     NEAR PTR loc_1_E215
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 104h], eax
        jmp    NEAR PTR loc_1_E22E
loc_1_E215:
        push   7530h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 104h
        push   eax
        call   sub_1_EB38
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
loc_1_E22E:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_E2FB
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 0
        je     NEAR PTR loc_1_E26E
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 80010003h
        je     NEAR PTR loc_1_E26E
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 80010001h
        jne    NEAR PTR loc_1_E2FB
loc_1_E26E:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 80010001h
        je     NEAR PTR loc_1_E2B3
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_E2AE
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_E2AE:
        jmp    NEAR PTR loc_1_E2E0
loc_1_E2B3:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_E2E0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_E2E0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_E354
        db  0E9h, 059h, 000h, 000h, 000h
loc_1_E2FB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2], 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_E33B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 104h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_E33B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_E354
loc_1_E354:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E359:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 108h], eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 108h
        push   eax
        push   2010103h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E9BB
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_E412
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_E406
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        shl    eax, 8
        and    eax, 0ff0000h
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 108h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 108h]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 108h]
        shr    ecx, 18h
        and    ecx, 0ffh
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_E406:
        xor    eax, eax
        jmp    NEAR PTR loc_1_E43B
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_E412:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_E43B
loc_1_E43B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E440:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 108h], eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 108h
        push   eax
        push   2010104h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E9BB
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_E4F9
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_E4ED
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        shl    eax, 8
        and    eax, 0ff0000h
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 108h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 108h]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 108h]
        shr    ecx, 18h
        and    ecx, 0ffh
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_E4ED:
        xor    eax, eax
        jmp    NEAR PTR loc_1_E522
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_E4F9:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_E522
loc_1_E522:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E527:
        push   ebp
        mov    ebp, esp
        sub    esp, 0d8h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0d8h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_E553
loc_1_E54B:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_E553:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_E56E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 50h]
        or     dword ptr [ebp - 0d8h], eax
        jmp    NEAR PTR loc_1_E54B
loc_1_E56E:
        mov    word ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_E58F
loc_1_E587:
        inc    word ptr [ebp - 14h]
        shl    dword ptr [ebp - 8], 1
loc_1_E58F:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 20h
        jge    NEAR PTR loc_1_E6DE
        cmp    dword ptr [ebp - 10h], 20h
        jae    NEAR PTR loc_1_E6DE
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [ebp - 0d8h], eax
        je     NEAR PTR loc_1_E6D9
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    byte ptr [ebp + eax*2 - 0d4h], 0c0h
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    byte ptr [ebp + eax*2 - 0d3h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    dword ptr [ebp + ecx*2 - 0d2h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_E6D6
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d4h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d4h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d3h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d3h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d2h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d2h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d1h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d1h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d0h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d0h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0cfh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0cfh], al
loc_1_E6D6:
        inc    dword ptr [ebp - 10h]
loc_1_E6D9:
        jmp    NEAR PTR loc_1_E587
loc_1_E6DE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_E6F4
loc_1_E6EC:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_E6F4:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_E83D
        cmp    dword ptr [ebp - 10h], 20h
        jae    NEAR PTR loc_1_E83D
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_E838
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    byte ptr [ebp + eax*2 - 0d4h], 0c0h
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    byte ptr [ebp + eax*2 - 0d3h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 54h]
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    dword ptr [ebp + ecx*2 - 0d2h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_E835
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d4h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d4h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d3h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d3h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d2h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d2h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d1h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d1h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0d0h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0d0h], al
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [eax + eax*2]
        mov    al, byte ptr [ebp + eax*2 - 0cfh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        mov    byte ptr [ebp + ecx*2 - 0cfh], al
loc_1_E835:
        inc    dword ptr [ebp - 10h]
loc_1_E838:
        jmp    NEAR PTR loc_1_E6EC
loc_1_E83D:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0d4h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        sbb    eax, eax
        and    eax, 0fe000000h
        add    eax, 3010103h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E9BB
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_E8B3
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_E8A7
        lea    eax, [ebp - 0d4h]
        push   eax
        push   6
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 18h
loc_1_E8A7:
        xor    eax, eax
        jmp    NEAR PTR loc_1_E8DC
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_E8B3:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_E8DC
loc_1_E8DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E8E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 108h], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_E923
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 108h], 4000h
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 108h], 1000h
        jmp    NEAR PTR loc_1_E92D
loc_1_E923:
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 108h], 2
loc_1_E92D:
        push   4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 108h
        push   eax
        push   1010eh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E9BB
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_E986
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_E981
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_E981:
        jmp    NEAR PTR loc_1_E9AF
loc_1_E986:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_E9B6
loc_1_E9AF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_E9B6
loc_1_E9B6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_E9BB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ech], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0f0h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0f4h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0f8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 104h], 103h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0dch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28EB6
        cmp    dword ptr [ebp - 4], 103h
        je     NEAR PTR loc_1_EA50
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_EAC8
loc_1_EA50:
        push   0bb8h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 104h
        push   eax
        call   sub_1_EB38
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_EA90
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 104h]
        jmp    NEAR PTR loc_1_EAC8
        db  0E9h, 038h, 000h, 000h, 000h
loc_1_EA90:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_EAC8
loc_1_EAC8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_EACD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EAE2:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
loc_1_EAEE:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_EB2B
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1052C
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_EB2B
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EB1C:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EB21:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EB26:
        jmp    NEAR PTR loc_1_EAEE
loc_1_EB2B:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_EB33
loc_1_EB33:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_EB38:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EB4D:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
loc_1_EB59:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 103h
        jne    NEAR PTR loc_1_EB99
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1052C
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_EB99
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EB8A:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EB8F:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_EB94:
        jmp    NEAR PTR loc_1_EB59
loc_1_EB99:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_EBA1
loc_1_EBA1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
loc_1_EBA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 14h], edx
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_EBC9
        jmp    NEAR PTR loc_1_EDAA
loc_1_EBC9:
        mov    dword ptr [ebp - 4], 1
        cmp    dword ptr [ebp - 0ch], 0
        jle    NEAR PTR loc_1_EC08
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        add    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 8]
        sub    edx, edx
        div    dword ptr [ecx + 4]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 8]
        sub    edx, edx
        div    dword ptr [ecx + 4]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 8], edx
loc_1_EC08:
        db  081h, 07Dh, 0ECh, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_EC8D
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_EC24
loc_1_EC20:
        inc    word ptr [ebp - 8]
loc_1_EC24:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_EC88
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_EC83
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_EC83
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_EC83
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        add    dword ptr [ecx + 4ch], eax
loc_1_EC83:
        jmp    NEAR PTR loc_1_EC20
loc_1_EC88:
        jmp    NEAR PTR loc_1_ED31
loc_1_EC8D:
        db  081h, 07Dh, 0ECh, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_ED12
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_ECA9
loc_1_ECA5:
        inc    word ptr [ebp - 8]
loc_1_ECA9:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_ED0D
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_ED08
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_ED08
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_ED08
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        add    dword ptr [ecx + 50h], eax
loc_1_ED08:
        jmp    NEAR PTR loc_1_ECA5
loc_1_ED0D:
        jmp    NEAR PTR loc_1_ED31
loc_1_ED12:
        push   28h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   74h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_EDAA
loc_1_ED31:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_F323
        add    esp, 4
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_ED55
loc_1_ED51:
        inc    word ptr [ebp - 8]
loc_1_ED55:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_EDAA
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_EDA5
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_EDA5
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_F2C4
        add    esp, 8
loc_1_EDA5:
        jmp    NEAR PTR loc_1_ED51
loc_1_EDAA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_EDAF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], edx
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_EDCD
        jmp    NEAR PTR loc_1_EDE5
loc_1_EDCD:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_F0C8
        add    esp, 8
loc_1_EDE5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_EDEA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_EE12
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_017F
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_EE12:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_EE34
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_017F
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_EE34:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_EE39:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_F088
loc_1_EE53:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C8C7
        add    esp, 8
        jmp    NEAR PTR loc_1_F0C3
loc_1_EE68:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_EE9E
        push   2fh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   113h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_F0C3
loc_1_EE9E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1D4FA
        add    esp, 8
        jmp    NEAR PTR loc_1_F0C3
loc_1_EEB3:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_EEE9
        push   34h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   122h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_F0C3
loc_1_EEE9:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23EA8
        add    esp, 8
        jmp    NEAR PTR loc_1_F0C3
loc_1_EEFE:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_EF34
        push   1dh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   12eh
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_F0C3
loc_1_EF34:
        push   40h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_146A5
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_EF72
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_EF7E
loc_1_EF72:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_EF7E:
        jmp    NEAR PTR loc_1_F0C3
loc_1_EF83:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_EFB9
        push   0fh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   142h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_F0C3
loc_1_EFB9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_EFFC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        push   21h
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 84h]
loc_1_EFFC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_F025
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_F031
loc_1_F025:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_F031:
        jmp    NEAR PTR loc_1_F0C3
loc_1_F036:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   26h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   15ah
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_F0C3
        db  0E9h, 03Bh, 000h, 000h, 000h
loc_1_F088:
        dec    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 8
        ja     NEAR PTR loc_1_F036
        mov    eax, dword ptr [ebp - 0ch]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_F0C3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_F0C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        je     NEAR PTR loc_1_F0EB
        jmp    NEAR PTR loc_1_F2BF
loc_1_F0EB:
        call   sub_1_01B2
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_F119
        push   2dh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   186h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_F119:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_F150
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        jmp    NEAR PTR loc_1_F2BF
loc_1_F150:
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0ah], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
loc_1_F17A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_F1BE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_F1BE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_128D5
        add    esp, 8
        xor    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 4ch], eax
loc_1_F1BE:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_F20E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 50h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_F20E
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4C9C
        add    esp, 8
        xor    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 50h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_132A
        add    esp, 4
loc_1_F20E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_19726
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_F23F
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_EE39
        add    esp, 8
        jmp    NEAR PTR loc_1_F20E
loc_1_F23F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_F27E
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_126B0
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_F27E
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1863A
        add    esp, 8
loc_1_F27E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   2
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F387
        add    esp, 10h
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_F2BA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        jmp    NEAR PTR loc_1_F2BF
loc_1_F2BA:
        jmp    NEAR PTR loc_1_F17A
loc_1_F2BF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_F2C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_F2DF
        jmp    NEAR PTR loc_1_F31E
loc_1_F2DF:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_F2F5
        mov    dword ptr [ebp - 4], 8
        jmp    NEAR PTR loc_1_F2FC
loc_1_F2F5:
        mov    dword ptr [ebp - 4], 0
loc_1_F2FC:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        call   sub_1_0221
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 10h], eax
loc_1_F31E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_F323:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_0166
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_F35B
        xor    eax, eax
        jmp    NEAR PTR loc_1_F382
        db  0E9h, 027h, 000h, 000h, 000h
loc_1_F35B:
        push   0bh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   24dh
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_F382
loc_1_F382:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_F387:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_F448
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_F3F1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4ch], 0
        jne    NEAR PTR loc_1_F423
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 50h], 0
        jne    NEAR PTR loc_1_F423
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_126B0
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_F423
loc_1_F3F1:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 3ch], 0
        jne    NEAR PTR loc_1_F423
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_F448
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_F448
loc_1_F423:
        mov    word ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_F443
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0ah], 0
loc_1_F443:
        jmp    NEAR PTR loc_1_F49A
loc_1_F448:
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_F46F
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0ah], ax
        jmp    NEAR PTR loc_1_F49A
loc_1_F46F:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 2
        je     NEAR PTR loc_1_F491
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_F49A
loc_1_F491:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0ah], 0
loc_1_F49A:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_F4B1
loc_1_F4B1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_F4B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_F4F2
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 20h
        jne    NEAR PTR loc_1_F4F2
        xor    al, al
        jmp    NEAR PTR loc_1_F792
loc_1_F4F2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_F523
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 21h
        jne    NEAR PTR loc_1_F523
        mov    al, 0ch
        jmp    NEAR PTR loc_1_F792
loc_1_F523:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_F56C
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_F56C
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jle    NEAR PTR loc_1_F5C4
loc_1_F56C:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_F5BD
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_F5BD
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 14h], 0
loc_1_F5BD:
        mov    al, 52h
        jmp    NEAR PTR loc_1_F792
loc_1_F5C4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_F663
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_F663
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 21h
        jne    NEAR PTR loc_1_F62E
        mov    al, 0ch
        jmp    NEAR PTR loc_1_F792
        db  0E9h, 035h, 000h, 000h, 000h
loc_1_F62E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_F65C
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_F65C
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 14h], 0
loc_1_F65C:
        mov    al, 4
        jmp    NEAR PTR loc_1_F792
loc_1_F663:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 17h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_F689
        mov    al, 68h
        jmp    NEAR PTR loc_1_F792
loc_1_F689:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_F71C
loc_1_F69C:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_F70D
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_F706
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_F706
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 14h], 0
loc_1_F706:
        mov    al, 19h
        jmp    NEAR PTR loc_1_F792
loc_1_F70D:
        jmp    NEAR PTR loc_1_F78B
loc_1_F712:
        jmp    NEAR PTR loc_1_F78B
        db  0E9h, 06Fh, 000h, 000h, 000h
loc_1_F71C:
        sub    dword ptr [ebp - 8], 0ah
        cmp    dword ptr [ebp - 8], 2bh
        ja     NEAR PTR loc_1_F78B
        mov    eax, dword ptr [ebp - 8]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 007h, 001h, 001h, 001h, 001h, 001h, 007h, 007h, 002h, 002h, 002h, 002h, 007h, 003h
        db  003h, 003h, 003h, 003h, 003h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 004h, 004h, 004h, 004h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 005h, 005h, 007h, 006h, 006h
loc_1_F78B:
        xor    al, al
        jmp    NEAR PTR loc_1_F792
loc_1_F792:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
