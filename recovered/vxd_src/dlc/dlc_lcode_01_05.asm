; dlc_lcode_01_05.asm -- DLC object 1 segment _LTEXT  (part 5/9)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_14A84:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 1ch], 0
        je     NEAR PTR loc_1_14AB4
        push   0ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        call   dword ptr [eax + 1ch]
        add    esp, 10h
loc_1_14AB4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_14AE1
loc_1_14ADB:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_1_14AE1:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_14B77
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_14B23
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14CA1
        add    esp, 10h
        jmp    NEAR PTR loc_1_14B72
loc_1_14B23:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_14B72
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_14B72
        push   0ah
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 14h]
        add    esp, 10h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 014h, 000h, 000h, 000h, 000h
loc_1_14B72:
        jmp    NEAR PTR loc_1_14ADB
loc_1_14B77:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8], 1
        push   0feh
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14B9C
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14B9C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        cmp    eax, 0feh
        jne    NEAR PTR loc_1_14BC0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_14BC9
loc_1_14BC0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
loc_1_14BC9:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp + 0ch], eax
        jmp    NEAR PTR loc_1_14BDF
loc_1_14BD7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 0ch], eax
loc_1_14BDF:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_14C1B
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_14C11
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_14C16
loc_1_14C11:
        jmp    NEAR PTR loc_1_14C9C
loc_1_14C16:
        jmp    NEAR PTR loc_1_14BD7
loc_1_14C1B:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_14C9C
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 3
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_14C62
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_14C62:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp - 0ch]
        add    esp, 10h
loc_1_14C9C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14CA1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 3
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_14CD7
loc_1_14CD1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
loc_1_14CD7:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_14D5B
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_14D56
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_14D38
        push   0ah
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        call   dword ptr [eax + 14h]
        add    esp, 10h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 14h], 0
loc_1_14D38:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], 3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14110
        add    esp, 10h
loc_1_14D56:
        jmp    NEAR PTR loc_1_14CD1
loc_1_14D5B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14D7D
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14D7D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
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
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp + 0ch], eax
        jmp    NEAR PTR loc_1_14DC1
loc_1_14DB9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 0ch], eax
loc_1_14DC1:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_14DFD
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_14DF3
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_14DF8
loc_1_14DF3:
        jmp    NEAR PTR loc_1_14E7F
loc_1_14DF8:
        jmp    NEAR PTR loc_1_14DB9
loc_1_14DFD:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_14E7F
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_14E45
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_14E45:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 14h], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp - 0ch]
        add    esp, 10h
loc_1_14E7F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14E84:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_14EB3
        mov    al, 27h
        jmp    NEAR PTR loc_1_14FBD
loc_1_14EB3:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        add    eax, 4
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 0b3h]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_14F77
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0b3h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_127C2
        add    esp, 8
        mov    al, 28h
        jmp    NEAR PTR loc_1_14FBD
loc_1_14F77:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 80h], 0
        jne    NEAR PTR loc_1_14FA6
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 80h], eax
loc_1_14FA6:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1863A
        add    esp, 8
        xor    al, al
        jmp    NEAR PTR loc_1_14FBD
loc_1_14FBD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_14FC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 14h], 0
        mov    byte ptr [ebp - 10h], 0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 10h
        je     NEAR PTR loc_1_15019
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_15019:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 20h
        je     NEAR PTR loc_1_15094
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 1dh
        jne    NEAR PTR loc_1_15094
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        cmp    eax, 8
        je     NEAR PTR loc_1_15065
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        cmp    eax, 9
        jne    NEAR PTR loc_1_15094
loc_1_15065:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0dfh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 79h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_15094:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        add    ecx, ecx
        db  066h, 08Bh, 084h, 0C1h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        test   byte ptr [ebp - 0bh], 80h
        je     NEAR PTR loc_1_15814
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 7fffh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_15796
loc_1_150F8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_15110
        jmp    NEAR PTR loc_1_15133
loc_1_15110:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1512C
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_15133
loc_1_1512C:
        mov    dword ptr [ebp - 4], 0
loc_1_15133:
        jmp    NEAR PTR loc_1_15801
loc_1_15138:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_15150
        jmp    NEAR PTR loc_1_15157
loc_1_15150:
        mov    dword ptr [ebp - 4], 0
loc_1_15157:
        jmp    NEAR PTR loc_1_15801
loc_1_1515C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_15174
        jmp    NEAR PTR loc_1_1517B
loc_1_15174:
        mov    dword ptr [ebp - 4], 0
loc_1_1517B:
        jmp    NEAR PTR loc_1_15801
loc_1_15180:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_151A8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_151AD
loc_1_151A8:
        jmp    NEAR PTR loc_1_151D0
loc_1_151AD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_151C9
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_151D0
loc_1_151C9:
        mov    dword ptr [ebp - 4], 0
loc_1_151D0:
        jmp    NEAR PTR loc_1_15801
loc_1_151D5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_151ED
        jmp    NEAR PTR loc_1_1520F
loc_1_151ED:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_15208
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1520F
loc_1_15208:
        mov    dword ptr [ebp - 4], 0
loc_1_1520F:
        jmp    NEAR PTR loc_1_15801
loc_1_15214:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_15240
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_15240
        jmp    NEAR PTR loc_1_152D6
loc_1_15240:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1526F
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1526F
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_152D6
loc_1_1526F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_1529F
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_1529F
        add    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_152D6
loc_1_1529F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_152CF
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_152CF
        add    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_1_152D6
loc_1_152CF:
        mov    dword ptr [ebp - 4], 0
loc_1_152D6:
        jmp    NEAR PTR loc_1_15801
loc_1_152DB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_15303
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_15308
loc_1_15303:
        jmp    NEAR PTR loc_1_1532B
loc_1_15308:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_15324
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1532B
loc_1_15324:
        mov    dword ptr [ebp - 4], 0
loc_1_1532B:
        jmp    NEAR PTR loc_1_15801
loc_1_15330:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_15358
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_15370
loc_1_15358:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_15370
        jmp    NEAR PTR loc_1_153D7
loc_1_15370:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_15398
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_153B3
loc_1_15398:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_153B3
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_153D7
loc_1_153B3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_153D0
        add    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_153D7
loc_1_153D0:
        mov    dword ptr [ebp - 4], 0
loc_1_153D7:
        jmp    NEAR PTR loc_1_15801
loc_1_153DC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_1541B
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1541B
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_1541B
        jmp    NEAR PTR loc_1_15464
loc_1_1541B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_1545D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1545D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_1545D
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_15464
loc_1_1545D:
        mov    dword ptr [ebp - 4], 0
loc_1_15464:
        jmp    NEAR PTR loc_1_15801
loc_1_15469:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_15481
        jmp    NEAR PTR loc_1_154A3
loc_1_15481:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1549C
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_154A3
loc_1_1549C:
        mov    dword ptr [ebp - 4], 0
loc_1_154A3:
        jmp    NEAR PTR loc_1_15801
loc_1_154A8:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 72h]
        test   eax, eax
        jg     NEAR PTR loc_1_154BC
        jmp    NEAR PTR loc_1_154DA
loc_1_154BC:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 72h]
        test   eax, eax
        jle    NEAR PTR loc_1_154D3
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_154DA
loc_1_154D3:
        mov    dword ptr [ebp - 4], 0
loc_1_154DA:
        jmp    NEAR PTR loc_1_15801
loc_1_154DF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_15503
        jmp    NEAR PTR loc_1_15531
loc_1_15503:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1552A
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_15531
loc_1_1552A:
        mov    dword ptr [ebp - 4], 0
loc_1_15531:
        jmp    NEAR PTR loc_1_15801
loc_1_15536:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 72h]
        test   eax, eax
        jle    NEAR PTR loc_1_1554A
        jmp    NEAR PTR loc_1_1556C
loc_1_1554A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        test   eax, eax
        jg     NEAR PTR loc_1_15565
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1556C
loc_1_15565:
        mov    dword ptr [ebp - 4], 0
loc_1_1556C:
        jmp    NEAR PTR loc_1_15801
loc_1_15571:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_15589
        jmp    NEAR PTR loc_1_155AB
loc_1_15589:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_155A4
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_155AB
loc_1_155A4:
        mov    dword ptr [ebp - 4], 0
loc_1_155AB:
        jmp    NEAR PTR loc_1_15801
loc_1_155B0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 8
        je     NEAR PTR loc_1_155C9
        jmp    NEAR PTR loc_1_155EC
loc_1_155C9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_155E5
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_155EC
loc_1_155E5:
        mov    dword ptr [ebp - 4], 0
loc_1_155EC:
        jmp    NEAR PTR loc_1_15801
loc_1_155F1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_15615
        jmp    NEAR PTR loc_1_15643
loc_1_15615:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1563C
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_15643
loc_1_1563C:
        mov    dword ptr [ebp - 4], 0
loc_1_15643:
        jmp    NEAR PTR loc_1_15801
loc_1_15648:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_15660
        jmp    NEAR PTR loc_1_15683
loc_1_15660:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        and    eax, 0ffh
        cmp    eax, 40h
        jne    NEAR PTR loc_1_1567C
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_15683
loc_1_1567C:
        mov    dword ptr [ebp - 4], 0
loc_1_15683:
        jmp    NEAR PTR loc_1_15801
loc_1_15688:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_156AC
        jmp    NEAR PTR loc_1_156B3
loc_1_156AC:
        mov    dword ptr [ebp - 4], 0
loc_1_156B3:
        jmp    NEAR PTR loc_1_15801
loc_1_156B8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        cmp    eax, 30h
        jne    NEAR PTR loc_1_156D1
        jmp    NEAR PTR loc_1_156F4
loc_1_156D1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        cmp    eax, 10h
        jne    NEAR PTR loc_1_156ED
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_156F4
loc_1_156ED:
        mov    dword ptr [ebp - 4], 0
loc_1_156F4:
        jmp    NEAR PTR loc_1_15801
loc_1_156F9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_1571D
        jmp    NEAR PTR loc_1_15767
loc_1_1571D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_15744
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_15767
loc_1_15744:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_15760
        add    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_15767
loc_1_15760:
        mov    dword ptr [ebp - 4], 0
loc_1_15767:
        jmp    NEAR PTR loc_1_15801
loc_1_1576C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        and    eax, 0ffh
        cmp    eax, 40h
        jne    NEAR PTR loc_1_15785
        jmp    NEAR PTR loc_1_1578C
loc_1_15785:
        mov    dword ptr [ebp - 4], 0
loc_1_1578C:
        jmp    NEAR PTR loc_1_15801
        db  0E9h, 06Bh, 000h, 000h, 000h
loc_1_15796:
        dec    dword ptr [ebp - 1ch]
        cmp    dword ptr [ebp - 1ch], 14h
        ja     NEAR PTR loc_1_15801
        mov    eax, dword ptr [ebp - 1ch]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_15801:
        mov    eax, dword ptr [ebp - 4]
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
loc_1_15814:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_17D0F
loc_1_1581F:
        mov    byte ptr [ebp - 10h], 0c2h
        jmp    NEAR PTR loc_1_181B6
loc_1_15828:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 79h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 79h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7bh], al
loc_1_15847:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
loc_1_1584E:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_181B6
loc_1_15862:
        mov    byte ptr [ebp - 10h], 41h
        jmp    NEAR PTR loc_1_181B6
loc_1_1586B:
        mov    byte ptr [ebp - 10h], 0c4h
        jmp    NEAR PTR loc_1_181B6
loc_1_15874:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0
        jmp    NEAR PTR loc_1_181B6
loc_1_15880:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        jmp    NEAR PTR loc_1_181B6
loc_1_1588C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
        jmp    NEAR PTR loc_1_181B6
loc_1_15898:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 1
        mov    byte ptr [ebp - 14h], 1fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 2
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_158C8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
loc_1_158D4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_181B6
loc_1_158EB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 77h]
        and    eax, 0ffh
        or     al, 20h
        mov    byte ptr [ebp - 14h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 2
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 78h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 70h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_158D4
loc_1_15954:
        mov    byte ptr [ebp - 14h], 14h
loc_1_15958:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_15974:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_181B6
loc_1_15988:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 77h]
        and    eax, 0ffh
        or     al, 14h
        mov    byte ptr [ebp - 14h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        jmp    NEAR PTR loc_1_15958
loc_1_159A4:
        jmp    NEAR PTR loc_1_181B6
loc_1_159A9:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     al, 14h
        mov    byte ptr [ebp - 14h], al
        jmp    NEAR PTR loc_1_181B6
loc_1_159B8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 400h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_159D6:
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 77h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 2
        jmp    NEAR PTR loc_1_15974
loc_1_159EB:
        mov    byte ptr [ebp - 14h], 15h
        jmp    NEAR PTR loc_1_181B6
loc_1_159F4:
        mov    byte ptr [ebp - 14h], 14h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_15A3F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        jmp    NEAR PTR loc_1_15847
loc_1_15A4B:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 3
        mov    byte ptr [ebp - 14h], 13h
loc_1_15A6C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
loc_1_15A78:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_181B6
loc_1_15A8C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_15AAA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 8
        mov    byte ptr [ebp - 14h], 0bh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
loc_1_15ABC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15A6C
loc_1_15ADD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_15B02:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        mov    byte ptr [ebp - 14h], 0fh
        jmp    NEAR PTR loc_1_15ABC
loc_1_15B12:
        mov    byte ptr [ebp - 14h], 1fh
loc_1_15B16:
        mov    eax, dword ptr [ebp + 0ch]
        dec    byte ptr [eax + 7ch]
        jmp    NEAR PTR loc_1_15A78
loc_1_15B21:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15A3F
loc_1_15B60:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 78h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 70h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_15AAA
loc_1_15BB5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 78h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 70h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_15B02
loc_1_15C11:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_15C68:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_159A9
loc_1_15C7C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_15CCC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        jmp    NEAR PTR loc_1_15974
loc_1_15CD8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 3
loc_1_15CDF:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     al, 20h
        mov    byte ptr [ebp - 14h], al
        jmp    NEAR PTR loc_1_181B6
loc_1_15CEE:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_15CDF
loc_1_15D02:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 78h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 70h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 8
        mov    byte ptr [ebp - 14h], 0bh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
loc_1_15D3F:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        jmp    NEAR PTR loc_1_158C8
loc_1_15D82:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 78h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 70h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        mov    byte ptr [ebp - 14h], 0fh
        jmp    NEAR PTR loc_1_15D3F
loc_1_15DC4:
        mov    byte ptr [ebp - 14h], 8
loc_1_15DC8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
loc_1_15DCF:
        mov    byte ptr [ebp - 10h], 0c1h
loc_1_15DD3:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
loc_1_15DE3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15CCC
loc_1_15E04:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12D6B
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
loc_1_15E3C:
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
loc_1_15E54:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 5
        jmp    NEAR PTR loc_1_15DD3
loc_1_15E60:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_15DC8
loc_1_15E69:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_15E3C
loc_1_15E72:
        mov    byte ptr [ebp - 14h], 4
loc_1_15E76:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        jmp    NEAR PTR loc_1_15DCF
loc_1_15E82:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    byte ptr [ebp - 10h], 0c1h
        jmp    NEAR PTR loc_1_181B6
loc_1_15ED4:
        mov    byte ptr [ebp - 14h], 5
        jmp    NEAR PTR loc_1_15E76
loc_1_15EDD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 30h
loc_1_15EEB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_15DE3
loc_1_15F02:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 200h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        jmp    NEAR PTR loc_1_15EEB
loc_1_15F33:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_15EDD
loc_1_15F3C:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_15F02
loc_1_15F45:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        jmp    NEAR PTR loc_1_15DD3
loc_1_15F51:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_15F45
loc_1_15F5A:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_15E54
loc_1_15F63:
        mov    byte ptr [ebp - 14h], 13h
        jmp    NEAR PTR loc_1_15B16
loc_1_15F6C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15847
loc_1_15F8D:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_15F6C
loc_1_15FA1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_15C68
loc_1_15FD8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 1
        mov    byte ptr [ebp - 14h], 1fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 2
loc_1_15FEA:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_15A6C
loc_1_15FFE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 3
        mov    byte ptr [ebp - 14h], 13h
        jmp    NEAR PTR loc_1_15FEA
loc_1_1600E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_1584E
loc_1_16031:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_1604F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        jmp    NEAR PTR loc_1_15CEE
loc_1_1605B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_16079:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        jmp    NEAR PTR loc_1_15974
loc_1_16085:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
loc_1_16091:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 14h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15974
loc_1_160BB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0bh
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 800h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_159D6
loc_1_160E5:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     al, 18h
        mov    byte ptr [ebp - 14h], al
loc_1_160EF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 1000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15974
loc_1_16112:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        mov    byte ptr [ebp - 14h], 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_15880
loc_1_1613D:
        mov    byte ptr [ebp - 14h], 18h
loc_1_16141:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8ah], 1
loc_1_1614B:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_1615A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_160EF
loc_1_16175:
        mov    byte ptr [ebp - 14h], 19h
        jmp    NEAR PTR loc_1_16141
loc_1_1617E:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 72h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
loc_1_161A0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
loc_1_161A7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
loc_1_161B3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        jmp    NEAR PTR loc_1_181B6
loc_1_161C4:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_161E2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 3
        mov    byte ptr [ebp - 14h], 13h
loc_1_161ED:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_161B3
loc_1_16201:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        mov    byte ptr [ebp - 14h], 0fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        jmp    NEAR PTR loc_1_161ED
loc_1_16248:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_161E2
loc_1_1627A:
        mov    byte ptr [ebp - 14h], 0ch
loc_1_1627E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        jmp    NEAR PTR loc_1_181B6
loc_1_1628F:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_1629E:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_16031
loc_1_162B2:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_162C1:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_1605B
loc_1_162D5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 14h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_161ED
loc_1_1631D:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_1632C:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_160BB
loc_1_16340:
        mov    byte ptr [ebp - 14h], 18h
        jmp    NEAR PTR loc_1_1614B
loc_1_16349:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12D6B
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
loc_1_16381:
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
loc_1_16399:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_181B6
loc_1_163AE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_163C9:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_16381
loc_1_163D2:
        mov    byte ptr [ebp - 14h], 4
loc_1_163D6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    byte ptr [ebp - 10h], 0c1h
loc_1_163ED:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_16399
loc_1_16401:
        mov    byte ptr [ebp - 14h], 5
        jmp    NEAR PTR loc_1_163D6
loc_1_1640A:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
loc_1_1642A:
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 72h]
        jmp    NEAR PTR loc_1_181B6
loc_1_16436:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_16451:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_1640A
loc_1_1645A:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_163ED
loc_1_16463:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_1647C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 200h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_164A1:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        jmp    NEAR PTR loc_1_16399
loc_1_164B6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    byte ptr [ebp - 14h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_1647C
loc_1_164F3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        jmp    NEAR PTR loc_1_1645A
loc_1_16504:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
loc_1_16539:
        mov    byte ptr [ebp - 14h], 0fh
        jmp    NEAR PTR loc_1_161A0
loc_1_16542:
        mov    byte ptr [ebp - 14h], 18h
loc_1_16546:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8ah], 1
        jmp    NEAR PTR loc_1_1615A
loc_1_16555:
        mov    byte ptr [ebp - 14h], 19h
        jmp    NEAR PTR loc_1_16546
loc_1_1655E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
loc_1_16571:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        jmp    NEAR PTR loc_1_1642A
loc_1_1659D:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_165AC:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_165BB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 3
        mov    byte ptr [ebp - 14h], 13h
        jmp    NEAR PTR loc_1_161B3
loc_1_165CB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
loc_1_165D2:
        mov    byte ptr [ebp - 14h], 0bh
loc_1_165D6:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
loc_1_165E6:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_161A7
loc_1_165FA:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_16609:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_165BB
loc_1_1661D:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_1662C:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_1663B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 14h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_161B3
loc_1_16665:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8ah], 8
loc_1_1666F:
        mov    byte ptr [ebp - 14h], 18h
        jmp    NEAR PTR loc_1_1615A
loc_1_16678:
        mov    byte ptr [ebp - 14h], 8
loc_1_1667C:
        mov    byte ptr [ebp - 10h], 0c1h
        jmp    NEAR PTR loc_1_16399
loc_1_16685:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_1667C
loc_1_1668E:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_1640A
loc_1_16697:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0dh
loc_1_1669E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 30h
loc_1_166A5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_164A1
loc_1_166BC:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_16697
loc_1_166C5:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_16399
loc_1_166CE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0eh
        mov    byte ptr [ebp - 14h], 8
        jmp    NEAR PTR loc_1_15880
loc_1_166DE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8ah], 1
        jmp    NEAR PTR loc_1_1666F
loc_1_166ED:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8ah], 1
        mov    byte ptr [ebp - 14h], 19h
        jmp    NEAR PTR loc_1_1615A
loc_1_16700:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        jmp    NEAR PTR loc_1_16571
loc_1_1670C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        mov    byte ptr [ebp - 14h], 0fh
        jmp    NEAR PTR loc_1_165D6
loc_1_1671C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 5
        jmp    NEAR PTR loc_1_16349
loc_1_16728:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 5
        jmp    NEAR PTR loc_1_163C9
loc_1_16734:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_1667C
loc_1_1673D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 200h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        jmp    NEAR PTR loc_1_166A5
loc_1_1676E:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_1673D
loc_1_16777:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_16399
loc_1_16780:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
loc_1_1678E:
        mov    byte ptr [ebp - 14h], 8
loc_1_16792:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_1627E
loc_1_167A6:
        mov    byte ptr [ebp - 14h], 18h
loc_1_167AA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8ah], 1
loc_1_167B4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 1000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_167D9:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_167E8:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_1584E
loc_1_167FC:
        mov    byte ptr [ebp - 14h], 19h
        jmp    NEAR PTR loc_1_167AA
loc_1_16805:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 40h
        jmp    NEAR PTR loc_1_181B6
loc_1_16811:
        mov    eax, dword ptr [ebp + 0ch]
        dec    byte ptr [eax + 7ch]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
loc_1_16827:
        mov    byte ptr [ebp - 14h], 0fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_16792
loc_1_1684B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_16869:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        jmp    NEAR PTR loc_1_167E8
loc_1_16875:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_16884:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_15CDF
loc_1_168CC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_16869
loc_1_168FE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_1661D
loc_1_16919:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0bh
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 800h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 2
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 77h], al
        jmp    NEAR PTR loc_1_167D9
loc_1_16953:
        mov    byte ptr [ebp - 14h], 18h
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        jmp    NEAR PTR loc_1_167B4
loc_1_16963:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12D6B
        add    esp, 8
        mov    byte ptr [ebp - 10h], 0
loc_1_1698B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        jmp    NEAR PTR loc_1_158D4
loc_1_169A0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12D6B
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
loc_1_169D8:
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        jmp    NEAR PTR loc_1_181B6
loc_1_169F5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_16A17:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_16A32:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_169D8
loc_1_16A3B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        jmp    NEAR PTR loc_1_169A0
loc_1_16A67:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    byte ptr [ebp - 10h], 0c1h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_165AC
loc_1_16AA5:
        mov    byte ptr [ebp - 14h], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_16AD2:
        mov    byte ptr [ebp - 10h], 0c1h
        jmp    NEAR PTR loc_1_16792
loc_1_16ADB:
        mov    byte ptr [ebp - 14h], 4
loc_1_16ADF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        jmp    NEAR PTR loc_1_16AD2
loc_1_16AEB:
        mov    byte ptr [ebp - 14h], 5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_16ADF
loc_1_16B16:
        mov    byte ptr [ebp - 14h], 5
        jmp    NEAR PTR loc_1_16ADF
loc_1_16B1F:
        mov    byte ptr [ebp - 14h], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        jmp    NEAR PTR loc_1_16AD2
loc_1_16B4F:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    byte ptr [ebp - 10h], 0c1h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_165BB
loc_1_16B8D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        jmp    NEAR PTR loc_1_158D4
loc_1_16BA2:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        mov    byte ptr [ebp - 14h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_16792
loc_1_16BCD:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_16BDC:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_181B6
loc_1_16BE5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 5
loc_1_16BEC:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
loc_1_16BFC:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        jmp    NEAR PTR loc_1_181B6
loc_1_16C11:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_16C30:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        jmp    NEAR PTR loc_1_165BB
loc_1_16C3C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        jmp    NEAR PTR loc_1_1698B
loc_1_16C48:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        jmp    NEAR PTR loc_1_158D4
loc_1_16C54:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
loc_1_16C64:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
loc_1_16C6B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_16C7D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        jmp    NEAR PTR loc_1_16BCD
loc_1_16C8E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ch
loc_1_16C95:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 200h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        jmp    NEAR PTR loc_1_158D4
loc_1_16CC8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        jmp    NEAR PTR loc_1_16C6B
loc_1_16CDD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 11h
loc_1_16CE4:
        mov    byte ptr [ebp - 14h], 0ch
        jmp    NEAR PTR loc_1_1588C
loc_1_16CED:
        mov    byte ptr [ebp - 14h], 18h
loc_1_16CF1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 8ah], 1
loc_1_16CFB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 1000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_16D20:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_1584E
loc_1_16D34:
        mov    byte ptr [ebp - 14h], 19h
        jmp    NEAR PTR loc_1_16CF1
loc_1_16D3D:
        mov    byte ptr [ebp - 14h], 0bh
loc_1_16D41:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        jmp    NEAR PTR loc_1_15B16
loc_1_16D62:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15847
loc_1_16D85:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        jmp    NEAR PTR loc_1_16D20
loc_1_16DAF:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_1662C
loc_1_16DC3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0bh
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 800h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 2
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 77h], al
        jmp    NEAR PTR loc_1_16D20
loc_1_16DFD:
        mov    byte ptr [ebp - 14h], 18h
        jmp    NEAR PTR loc_1_16CFB
loc_1_16E06:
        mov    byte ptr [ebp - 14h], 8
        mov    byte ptr [ebp - 10h], 0c1h
        jmp    NEAR PTR loc_1_1698B
loc_1_16E13:
        mov    byte ptr [ebp - 14h], 8
loc_1_16E17:
        mov    byte ptr [ebp - 10h], 0c1h
        jmp    NEAR PTR loc_1_181B6
loc_1_16E20:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_16E42:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_16E17
loc_1_16E4B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        jmp    NEAR PTR loc_1_16E13
loc_1_16E77:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_16EA5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 3
        mov    byte ptr [ebp - 14h], 13h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        jmp    NEAR PTR loc_1_16E17
loc_1_16EC8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_16E17
loc_1_16EEF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
loc_1_16EF6:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        jmp    NEAR PTR loc_1_16E17
loc_1_16F1B:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_16B8D
loc_1_16F24:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_181B6
loc_1_16F2D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        jmp    NEAR PTR loc_1_16BEC
loc_1_16F39:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_16C30
loc_1_16F6C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 30h
        jmp    NEAR PTR loc_1_1698B
loc_1_16F78:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 30h
        jmp    NEAR PTR loc_1_158D4
loc_1_16F84:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
loc_1_16F94:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 30h
loc_1_16F9B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_16F24
loc_1_16FB2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0dh
loc_1_16FB9:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        jmp    NEAR PTR loc_1_16F78
loc_1_16FCE:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        jmp    NEAR PTR loc_1_16F9B
loc_1_16FE3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 10h
        jmp    NEAR PTR loc_1_15880
loc_1_16FEF:
        mov    byte ptr [ebp - 14h], 0fh
        jmp    NEAR PTR loc_1_16D41
loc_1_16FF8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_169A0
loc_1_17026:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        jmp    NEAR PTR loc_1_169A0
loc_1_17032:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_16A32
loc_1_17060:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        jmp    NEAR PTR loc_1_16A32
loc_1_1706C:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_16EA5
loc_1_17090:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_16EC8
loc_1_17099:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_16E17
loc_1_170A2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        jmp    NEAR PTR loc_1_16EF6
loc_1_170AE:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_16B8D
loc_1_170B7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        jmp    NEAR PTR loc_1_16BEC
loc_1_170C3:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
loc_1_170D3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
loc_1_170DA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_16BDC
loc_1_170F1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0fh
        jmp    NEAR PTR loc_1_16C95
loc_1_170FD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        jmp    NEAR PTR loc_1_170DA
loc_1_17112:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 77h]
        and    eax, 0ffh
        or     al, 14h
        mov    byte ptr [ebp - 14h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_1713E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        jmp    NEAR PTR loc_1_16079
loc_1_1714A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        jmp    NEAR PTR loc_1_16031
loc_1_17156:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 4000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_1713E
loc_1_17179:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        jmp    NEAR PTR loc_1_1663B
loc_1_17194:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 800h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_15974
loc_1_171B7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 30h
        jmp    NEAR PTR loc_1_1678E
loc_1_171CA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        jmp    NEAR PTR loc_1_16827
loc_1_171E2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_1720E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_169A0
loc_1_17223:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
loc_1_17236:
        mov    byte ptr [ebp - 14h], 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_16E17
loc_1_1725E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
loc_1_17280:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
loc_1_17290:
        mov    byte ptr [ebp - 10h], 0c1h
        jmp    NEAR PTR loc_1_16F24
loc_1_17299:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_17236
loc_1_172CA:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 72h]
loc_1_172F1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 5
loc_1_172F8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
loc_1_172FF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_16BFC
loc_1_17322:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_172F1
loc_1_17337:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    byte ptr [ebp - 14h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_172CA
loc_1_1735B:
        mov    byte ptr [ebp - 14h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_17322
loc_1_17373:
        push   0fh
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18494
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_16C7D
loc_1_173D5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        jmp    NEAR PTR loc_1_1735B
loc_1_173E6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
loc_1_173ED:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
        mov    byte ptr [ebp - 14h], 0fh
loc_1_17407:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_16399
loc_1_1742A:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        jmp    NEAR PTR loc_1_16539
loc_1_17454:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
loc_1_1745B:
        mov    byte ptr [ebp - 14h], 0bh
        jmp    NEAR PTR loc_1_165E6
loc_1_17464:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 72h]
loc_1_1748B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        jmp    NEAR PTR loc_1_172FF
loc_1_1749E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 72h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        jmp    NEAR PTR loc_1_16F24
loc_1_17506:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
loc_1_1750D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        push   0bh
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18494
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_1753E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        jmp    NEAR PTR loc_1_16F24
loc_1_1754A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_1753E
loc_1_17594:
        mov    byte ptr [ebp - 14h], 0bh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_175AE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        jmp    NEAR PTR loc_1_17407
loc_1_175BA:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_1748B
loc_1_175CF:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
        jmp    NEAR PTR loc_1_1670C
loc_1_175EA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 10h
        jmp    NEAR PTR loc_1_16571
loc_1_175F6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 10h
        jmp    NEAR PTR loc_1_165D2
loc_1_17602:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_16091
loc_1_17616:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_16E13
loc_1_17632:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 6
        jmp    NEAR PTR loc_1_17280
loc_1_1763E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_181B6
loc_1_17663:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_1763E
loc_1_1766C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 13h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 200h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_1669E
loc_1_17696:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 13h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 200h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_16F94
loc_1_176E0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 13h
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 30h
loc_1_176EE:
        mov    byte ptr [ebp - 14h], 8
        jmp    NEAR PTR loc_1_181B6
loc_1_176F7:
        mov    byte ptr [ebp - 14h], 0fh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        jmp    NEAR PTR loc_1_165E6
loc_1_17707:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ch
        jmp    NEAR PTR loc_1_1720E
loc_1_17713:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ch
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_16BDC
loc_1_17747:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
loc_1_17755:
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 72h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18478
        add    esp, 8
        jmp    NEAR PTR loc_1_1763E
loc_1_1778F:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_17747
loc_1_17798:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        push   0fh
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18494
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_16BDC
loc_1_177FA:
        mov    byte ptr [ebp - 14h], 0dh
loc_1_177FE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 7
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_172F8
loc_1_1781A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        jmp    NEAR PTR loc_1_173ED
loc_1_17826:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 12h
        mov    byte ptr [ebp - 14h], 0ch
loc_1_17831:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        jmp    NEAR PTR loc_1_181B6
loc_1_1783D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 12h
        jmp    NEAR PTR loc_1_16CE4
loc_1_17849:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_17872:
        mov    byte ptr [ebp - 10h], 0c1h
        jmp    NEAR PTR loc_1_176EE
loc_1_1787B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
        jmp    NEAR PTR loc_1_17872
loc_1_17887:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_17290
loc_1_178B5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
        jmp    NEAR PTR loc_1_17290
loc_1_178C1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0eh
        jmp    NEAR PTR loc_1_16EF6
loc_1_178CD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
        jmp    NEAR PTR loc_1_181B6
loc_1_178E2:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_178CD
loc_1_178EB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0eh
        jmp    NEAR PTR loc_1_16BEC
loc_1_17903:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 13h
        jmp    NEAR PTR loc_1_16FB9
loc_1_1790F:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 3
        mov    byte ptr [ebp - 14h], 13h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        jmp    NEAR PTR loc_1_17831
loc_1_17960:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F50
        add    esp, 8
        jmp    NEAR PTR loc_1_181B6
loc_1_17975:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0eh
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        jmp    NEAR PTR loc_1_172FF
loc_1_17991:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 9
loc_1_17998:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        jmp    NEAR PTR loc_1_176EE
loc_1_179A4:
        mov    byte ptr [ebp - 14h], 4
loc_1_179A8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        mov    byte ptr [ebp - 10h], 0c1h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
        jmp    NEAR PTR loc_1_17960
loc_1_179CA:
        mov    byte ptr [ebp - 14h], 4
loc_1_179CE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        jmp    NEAR PTR loc_1_16E17
loc_1_179DA:
        mov    byte ptr [ebp - 14h], 5
        jmp    NEAR PTR loc_1_179A8
loc_1_179E3:
        mov    byte ptr [ebp - 14h], 5
        jmp    NEAR PTR loc_1_179CE
loc_1_179EC:
        mov    byte ptr [ebp - 14h], 4
        jmp    NEAR PTR loc_1_170A2
loc_1_179F5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
loc_1_17A01:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 8
loc_1_17A27:
        mov    byte ptr [ebp - 14h], 0fh
        jmp    NEAR PTR loc_1_181B6
loc_1_17A30:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_17831
loc_1_17A39:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        jmp    NEAR PTR loc_1_17A01
loc_1_17A4A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 10h
        jmp    NEAR PTR loc_1_17998
loc_1_17A56:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_179A8
loc_1_17A5F:
        mov    byte ptr [ebp - 14h], 0dh
        jmp    NEAR PTR loc_1_179CE
loc_1_17A68:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 72h]
loc_1_17A9A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0ah
        jmp    NEAR PTR loc_1_17A27
loc_1_17AA6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_17AB8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12ECE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        jmp    NEAR PTR loc_1_17A9A
loc_1_17AE8:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 20h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 78h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        jmp    NEAR PTR loc_1_17A9A
loc_1_17B2A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 10h
        jmp    NEAR PTR loc_1_1745B
loc_1_17B36:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_17872
loc_1_17B70:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0dh
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_17280
loc_1_17B9A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0eh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        jmp    NEAR PTR loc_1_17755
loc_1_17BAD:
        mov    byte ptr [ebp - 14h], 9
        jmp    NEAR PTR loc_1_17B9A
loc_1_17BB6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0eh
        jmp    NEAR PTR loc_1_16399
loc_1_17BC2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 13h
loc_1_17BC9:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        jmp    NEAR PTR loc_1_16F24
loc_1_17BDE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_1750D
loc_1_17C08:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0eh
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_17BC9
loc_1_17C39:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 10h
        mov    byte ptr [ebp - 14h], 0bh
        jmp    NEAR PTR loc_1_175AE
loc_1_17C49:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 79h], 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_17BB6
loc_1_17C73:
        mov    eax, dword ptr [ebp + 0ch]
        dec    byte ptr [eax + 7ch]
        jmp    NEAR PTR loc_1_1584E
loc_1_17C7E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_16609
loc_1_17CA1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_1604F
loc_1_17CC4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_16079
loc_1_17CE7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 2000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        jmp    NEAR PTR loc_1_181B6
        db  0E9h, 0A7h, 004h, 000h, 000h
loc_1_17D0F:
        cmp    dword ptr [ebp - 20h], 123h
        ja     NEAR PTR loc_1_181B6
        mov    eax, dword ptr [ebp - 20h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_181B6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        test   eax, eax
        je     NEAR PTR loc_1_1826A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        test   ah, 10h
        je     NEAR PTR loc_1_18256
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 86h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8bh]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 2]
        and    eax, 3
        cmp    al, 3
        jne    NEAR PTR loc_1_1821D
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        jmp    NEAR PTR loc_1_18229
loc_1_1821D:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
loc_1_18229:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1]
        and    eax, 1
        mov    ecx, dword ptr [ebp - 8]
        or     al, byte ptr [ecx + 3]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 3], al
loc_1_18256:
        mov    al, byte ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18494
        add    esp, 0ch
loc_1_1826A:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 10h
        je     NEAR PTR loc_1_182B1
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 18h
loc_1_182B1:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 10h
        je     NEAR PTR loc_1_182D7
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_182D7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1839B
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        jne    NEAR PTR loc_1_18310
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A477
        add    esp, 8
        jmp    NEAR PTR loc_1_1839B
loc_1_18310:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 10h
        je     NEAR PTR loc_1_1838F
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_1838F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 84h], 0
loc_1_1839B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1863A
        add    esp, 8
        mov    al, byte ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_183B3
loc_1_183B3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_183B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1844E
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_1844E
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        add    eax, ecx
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_1844E
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_1843F
        xor    eax, eax
        jmp    NEAR PTR loc_1_18455
loc_1_1843F:
        mov    eax, 1
        jmp    NEAR PTR loc_1_18455
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1844E:
        xor    eax, eax
        jmp    NEAR PTR loc_1_18455
loc_1_18455:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1845A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_18478:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0efh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_18494:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        sar    eax, 2
        mov    byte ptr [ebp - 20h], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1852E
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_184FE
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0b4h
        mov    dword ptr [ebp - 1ch], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 4
        jge    NEAR PTR loc_1_184F0
        mov    eax, dword ptr [ebp - 1ch]
        mov    word ptr [eax + 0eh], 400h
        jmp    NEAR PTR loc_1_184F9
loc_1_184F0:
        mov    eax, dword ptr [ebp - 1ch]
        mov    word ptr [eax + 0eh], 300h
loc_1_184F9:
        jmp    NEAR PTR loc_1_1852E
loc_1_184FE:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0b4h
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 4
        jge    NEAR PTR loc_1_18525
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0ch], 400h
        jmp    NEAR PTR loc_1_1852E
loc_1_18525:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0ch], 300h
loc_1_1852E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0b3h]
        mov    byte ptr [ebp - 18h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0b4h
        mov    dword ptr [ebp - 17h], eax
        mov    byte ptr [ebp - 13h], 0
        mov    dword ptr [ebp - 12h], 0
        mov    byte ptr [ebp - 0eh], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 90h]
        mov    byte ptr [ebp - 0dh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0bh], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   al, 2
        jne    NEAR PTR loc_1_1858E
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        or     al, 1
        mov    byte ptr [ebp - 0ch], al
loc_1_1858E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 4
        jge    NEAR PTR loc_1_185BB
        mov    byte ptr [ebp - 0eh], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        mov    cl, byte ptr [ebp + 10h]
        and    ecx, 1
        or     al, cl
        mov    byte ptr [ebp - 0ah], al
        jmp    NEAR PTR loc_1_185D6
loc_1_185BB:
        mov    byte ptr [ebp - 0eh], 3
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   al, 1
        je     NEAR PTR loc_1_185D6
        xor    eax, eax
        mov    al, byte ptr [ebp - 0bh]
        or     al, 10h
        mov    byte ptr [ebp - 0bh], al
loc_1_185D6:
        mov    al, byte ptr [ebp + 10h]
        and    eax, 0fch
        cmp    al, 18h
        jne    NEAR PTR loc_1_185FA
        mov    byte ptr [ebp - 9], 5
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 86h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_18605
loc_1_185FA:
        mov    byte ptr [ebp - 9], 0
        mov    dword ptr [ebp - 8], 0
loc_1_18605:
        mov    al, byte ptr [ebp - 0bh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 0b2h], al
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22750
        add    esp, 14h
        xor    eax, eax
        jmp    NEAR PTR loc_1_18635
loc_1_18635:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1863A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1884A
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_1884A
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        add    eax, ecx
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_1884A
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_186BF
        jmp    NEAR PTR loc_1_1884A
loc_1_186BF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 76h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        add    eax, ecx
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_187C7
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 78h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1879B
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 72h]
        test   eax, eax
        jne    NEAR PTR loc_1_1879B
        push   0
        push   2ah
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        jmp    NEAR PTR loc_1_1884A
loc_1_1879B:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 3], al
        push   0
        push   26h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        jmp    NEAR PTR loc_1_187EF
loc_1_187C7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 58h], 0
        jne    NEAR PTR loc_1_187EF
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_187EF:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 18h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23FDC
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 103h
        je     NEAR PTR loc_1_1883C
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
loc_1_1883C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 80h], eax
loc_1_1884A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_18850:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jg     NEAR PTR loc_1_18880
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        db  083h, 03Ch, 085h, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_188AF
loc_1_18880:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_189A7
loc_1_188AF:
        push   -1
        push   0
        push   110h
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_28EFE
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_188F8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_189A7
loc_1_188F8:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_112E8
        add    esp, 4
        mov    ax, word ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 1
        push   1
        push   1000h
        call   sub_1_019A
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0ch], 0
        jne    NEAR PTR loc_1_1895F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_189A7
loc_1_1895F:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1899F
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1899F:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_189A7
loc_1_189A7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_189AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_189D3
        mov    eax, 1
        jmp    NEAR PTR loc_1_18D71
loc_1_189D3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        inc    eax
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        add    eax, 4
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp - 1ch]
        and    ecx, 0ffffh
        mov    edx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [edx + ecx*4]
        lea    ecx, [edx + ecx*4]
        lea    eax, [eax + ecx*4]
        mov    dword ptr [ebp - 4], eax
        push   -1
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_28EFE
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_18A6C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_18D71
loc_1_18A6C:
        mov    word ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_18A88
loc_1_18A7D:
        inc    word ptr [ebp - 18h]
        add    dword ptr [ebp - 0ch], 8ch
loc_1_18A88:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_18AB2
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_11D37
        add    esp, 4
        jmp    NEAR PTR loc_1_18A7D
loc_1_18AB2:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    word ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_18AD7
loc_1_18ACC:
        inc    word ptr [ebp - 18h]
        add    dword ptr [ebp - 28h], 0d4h
loc_1_18AD7:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 1ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_18B03
        push   1
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_1_119D8
        add    esp, 8
        jmp    NEAR PTR loc_1_18ACC
loc_1_18B03:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 32h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 32h]
        and    eax, 0ffffh
        shl    eax, 0ch
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 32h]
        and    eax, 0ffffh
        shl    eax, 0ch
        shr    eax, 6
        mov    word ptr [ebp - 14h], ax
        push   81h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 32h]
        and    eax, 0ffffh
        push   eax
        call   sub_1_018E
        add    esp, 20h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24h], 0
        jne    NEAR PTR loc_1_18BB5
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 32h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_18D71
loc_1_18BB5:
        mov    word ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_18BD1
loc_1_18BC9:
        inc    word ptr [ebp - 18h]
        add    dword ptr [ebp - 24h], 40h
loc_1_18BD1:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_18BFF
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_18BC9
loc_1_18BFF:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 60h
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_28F2E
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_18C5F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 60h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_18D71
loc_1_18C5F:
        push   4
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 64h
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_28F0A
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_18CC1
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 64h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_18D71
loc_1_18CC1:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 4], 1
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_18D00
        mov    eax, dword ptr [ebp - 4]
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
loc_1_18D00:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_18D38
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_18D38:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_18D67
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_18D67:
        mov    eax, 1
        jmp    NEAR PTR loc_1_18D71
loc_1_18D71:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_18D76:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        inc    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], 0c00h
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 0ch]
        dec    eax
        shr    eax, 0ch
        inc    eax
        mov    dword ptr [ebp - 8], eax
        push   81h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_018E
        add    esp, 20h
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_18E09
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_18F46
loc_1_18E09:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [ebp - 0ch]
        mov    ecx, 18h
        and    eax, 0ffffh
        sub    edx, edx
        div    ecx
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_18E49
loc_1_18E45:
        inc    word ptr [ebp - 14h]
loc_1_18E49:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_18EA9
        push   0
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_115A5
        add    esp, 8
        mov    al, byte ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        mov    edx, ecx
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [edx + ecx*2]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [edx + ecx*8 + 6], al
        jmp    NEAR PTR loc_1_18E45
loc_1_18EA9:
        mov    word ptr [ebp - 14h], 0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        inc    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_18ED9
loc_1_18ED1:
        inc    word ptr [ebp - 14h]
        add    dword ptr [ebp - 10h], 18h
loc_1_18ED9:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jge    NEAR PTR loc_1_18F03
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1923E
        add    esp, 4
        jmp    NEAR PTR loc_1_18ED1
loc_1_18F03:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 4
        je     NEAR PTR loc_1_18F3C
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        inc    eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_18F3C:
        mov    eax, 1
        jmp    NEAR PTR loc_1_18F46
loc_1_18F46:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_18F4B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_18F77
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0212
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0
loc_1_18F77:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_18F93
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A0
        add    esp, 4
loc_1_18F93:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  0C7h, 004h, 085h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   110h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_28F04
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_18FEA
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_18FEA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_18FEF:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   0bb8h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_EACD
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_19038
        push   2eh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   1a3h
        call   sub_1_FAD7
        add    esp, 0ch
        xor    eax, eax
        jmp    NEAR PTR loc_1_19239
loc_1_19038:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_190BF
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        inc    eax
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        add    eax, 4
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    edx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [edx + ecx*4]
        lea    ecx, [edx + ecx*4]
        lea    eax, [eax + ecx*4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_28F04
loc_1_190BF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24h], 0
        je     NEAR PTR loc_1_191B3
        push   2000h
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 32h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   sub_1_01BD
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_19122
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_19239
loc_1_19122:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   sub_1_0194
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_1918D
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_19158
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_19158:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 32h], 0
        jmp    NEAR PTR loc_1_191B3
loc_1_1918D:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_19239
loc_1_191B3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 60h], 0
        je     NEAR PTR loc_1_191D6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 60h]
        push   eax
        call   sub_1_28F34
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 60h], 0
loc_1_191D6:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 64h], 0
        je     NEAR PTR loc_1_191F9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 64h]
        push   eax
        call   sub_1_28F10
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 64h], 0
loc_1_191F9:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 4], 0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1922F
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1922F:
        mov    eax, 1
        jmp    NEAR PTR loc_1_19239
loc_1_19239:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1923E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1928C
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 004h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1929C
loc_1_1928C:
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 004h, 000h, 000h, 000h
loc_1_1929C:
        db  066h, 0FFh, 005h, 008h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_192B6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2ch], 0
        je     NEAR PTR loc_1_19305
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2ch], eax
        jmp    NEAR PTR loc_1_19317
loc_1_19305:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2ch], eax
loc_1_19317:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 30h]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19331:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_19351
        mov    eax, 1
        jmp    NEAR PTR loc_1_19423
loc_1_19351:
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 0ch
        mov    dword ptr [ebp - 4], eax
        push   2000h
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01BD
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_193A9
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_19423
loc_1_193A9:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_0194
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_193F7
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_193DE
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_193DE:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_19423
        db  0E9h, 02Ch, 000h, 000h, 000h
loc_1_193F7:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_19423
loc_1_19423:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19428:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_194CC
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_19474
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_19474:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_19491
loc_1_1948D:
        inc    word ptr [ebp - 8]
loc_1_19491:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 6
        jae    NEAR PTR loc_1_194B9
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + eax*4], 0
        jmp    NEAR PTR loc_1_1948D
loc_1_194B9:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 5], 1
        db  066h, 0FFh, 00Dh, 008h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_194F9
loc_1_194CC:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_194F9:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_19501
loc_1_19501:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19506:
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
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_19588
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 28h], 0
        jne    NEAR PTR loc_1_19554
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2ch], 0
loc_1_19554:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 5], 2
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 30h]
        jmp    NEAR PTR loc_1_195B5
loc_1_19588:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_195B5:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_195BD
loc_1_195BD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_195C4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_19609
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_1961A
loc_1_19609:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
loc_1_1961A:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1962D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_19659
loc_1_19651:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_19659:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1967B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_1_19676
        jmp    NEAR PTR loc_1_1967B
loc_1_19676:
        jmp    NEAR PTR loc_1_19651
loc_1_1967B:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_196DD
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_196C8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_1_196BF
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        jmp    NEAR PTR loc_1_196C8
loc_1_196BF:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
loc_1_196C8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1970B
loc_1_196DD:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], 0fffffffdh
loc_1_1970B:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_19721
loc_1_19721:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19726:
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
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1977A
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_19771
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
loc_1_19771:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
loc_1_1977A:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_19790
loc_1_19790:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19795:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_197DC
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_197ED
loc_1_197DC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
loc_1_197ED:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19800:
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
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_19856
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1984C
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
loc_1_1984C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
loc_1_19856:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1986C
loc_1_1986C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19871:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_198B6
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_198C7
loc_1_198B6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
loc_1_198C7:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_198DA:
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
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_19929
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1991C
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
loc_1_1991C:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_19951
loc_1_19929:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_19949
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
loc_1_19949:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_19951:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_19970
loc_1_19970:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19975:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_199B7
loc_1_199A9:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_199B7:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_199F2
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_199ED
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_198DA
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_199F2
loc_1_199ED:
        jmp    NEAR PTR loc_1_199A9
loc_1_199F2:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_19A08
loc_1_19A08:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19A0D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_19A48
loc_1_19A3A:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_19A48:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_19A82
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_19A7D
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_198DA
        add    esp, 0ch
        jmp    NEAR PTR loc_1_19A82
loc_1_19A7D:
        jmp    NEAR PTR loc_1_19A3A
loc_1_19A82:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_19A98
loc_1_19A98:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19A9D:
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
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_19AF1
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_19AE8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
loc_1_19AE8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
loc_1_19AF1:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_19B07
loc_1_19B07:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19B0C:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F4B8
        add    esp, 8
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_19B55
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19B55:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 10h
        push   eax
        push   2ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_24241
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_19B9B
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19B9B:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_19BCC
        push   5
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19BCC:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_19BFD
        push   6
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19BFD:
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
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_19F70
loc_1_19C32:
        lea    eax, [ebp - 10h]
        push   eax
        push   2
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F843
        add    esp, 14h
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_19C7F
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19C7F:
        mov    eax, dword ptr [ebp - 14h]
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
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 0fh
        je     NEAR PTR loc_1_19CF5
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_19FD8
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_19CF5
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
        db  0E9h, 018h, 000h, 000h, 000h
loc_1_19CF5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 20h
        push   eax
        call   sub_1_19871
        add    esp, 8
        jmp    NEAR PTR loc_1_19F93
loc_1_19D0D:
        lea    eax, [ebp - 0ch]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F843
        add    esp, 14h
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_19D5A
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19D5A:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 0fh
        je     NEAR PTR loc_1_19DA8
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1A07D
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_19DA8
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
        db  0E9h, 018h, 000h, 000h, 000h
loc_1_19DA8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 20h
        push   eax
        call   sub_1_19871
        add    esp, 8
        jmp    NEAR PTR loc_1_19F93
loc_1_19DC0:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 0fh
        je     NEAR PTR loc_1_19F05
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_19E1F
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 48h
        push   eax
        call   sub_1_1A677
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_19E1F
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19E1F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_19E35
loc_1_19E2D:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_19E35:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_19EED
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1A07D
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_19E75
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19E75:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_19E8B
loc_1_19E83:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_19E8B:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_19EE8
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_19EE3
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_19FD8
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_19EE3
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
loc_1_19EE3:
        jmp    NEAR PTR loc_1_19E83
loc_1_19EE8:
        jmp    NEAR PTR loc_1_19E2D
loc_1_19EED:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
        jmp    NEAR PTR loc_1_19F18
loc_1_19F05:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_19F18:
        jmp    NEAR PTR loc_1_19F93
loc_1_19F1D:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   6
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_19FD3
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_19F70:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_19C32
        cmp    dword ptr [ebp - 18h], 1
        je     NEAR PTR loc_1_19D0D
        cmp    dword ptr [ebp - 18h], 2
        je     NEAR PTR loc_1_19DC0
        jmp    NEAR PTR loc_1_19F1D
loc_1_19F93:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_19FCC
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_19FCC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_19FD3
loc_1_19FD3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19FD8:
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
        call   sub_1_1A171
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_1A004
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A078
loc_1_1A004:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A2F2
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_1A02A
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A078
loc_1_1A02A:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1A4B7
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_1A04C
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A078
loc_1_1A04C:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 28h
        push   eax
        call   sub_1_1A677
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_1A071
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A078
loc_1_1A071:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A078
loc_1_1A078:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A07D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 8
        je     NEAR PTR loc_1_1A0F8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1A0AF
loc_1_1A0A7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_1A0AF:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1A0F8
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1A0F3
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A171
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_1A0F3
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A16C
loc_1_1A0F3:
        jmp    NEAR PTR loc_1_1A0A7
loc_1_1A0F8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A2F2
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_1A11E
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A16C
loc_1_1A11E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A4B7
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_1A140
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A16C
loc_1_1A140:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 28h
        push   eax
        call   sub_1_1A677
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_1A165
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A16C
loc_1_1A165:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A16C
loc_1_1A16C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
