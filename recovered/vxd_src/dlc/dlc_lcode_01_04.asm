; dlc_lcode_01_04.asm -- DLC object 1 segment _LTEXT  (part 4/9)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_F797:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_F7E2
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A9CB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_F7FF
loc_1_F7E2:
        push   14h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0c4h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_F7FF:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_1_F828
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        cmp    eax, 0ch
        jne    NEAR PTR loc_1_F835
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 21h
        jne    NEAR PTR loc_1_F835
loc_1_F828:
        xor    ax, ax
        jmp    NEAR PTR loc_1_F83E
        db  0E9h, 009h, 000h, 000h, 000h
loc_1_F835:
        mov    ax, 2
        jmp    NEAR PTR loc_1_F83E
loc_1_F83E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_F843:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_F93F
loc_1_F85C:
        cmp    byte ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_F877
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 4
        jg     NEAR PTR loc_1_F896
loc_1_F877:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        sar    ecx, 8
        cmp    eax, ecx
        jge    NEAR PTR loc_1_F89D
loc_1_F896:
        mov    al, 40h
        jmp    NEAR PTR loc_1_FA77
loc_1_F89D:
        jmp    NEAR PTR loc_1_F962
loc_1_F8A2:
        cmp    byte ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_F8E7
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_F8E7
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        sar    ecx, 8
        cmp    eax, ecx
        jge    NEAR PTR loc_1_F8EE
loc_1_F8E7:
        mov    al, 40h
        jmp    NEAR PTR loc_1_FA77
loc_1_F8EE:
        jmp    NEAR PTR loc_1_F962
loc_1_F8F3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_F92E
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        sar    ecx, 8
        cmp    eax, ecx
        jge    NEAR PTR loc_1_F935
loc_1_F92E:
        mov    al, 40h
        jmp    NEAR PTR loc_1_FA77
loc_1_F935:
        jmp    NEAR PTR loc_1_F962
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_F93F:
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_F85C
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_F8A2
        cmp    dword ptr [ebp - 8], 3
        je     NEAR PTR loc_1_F8F3
        jmp    NEAR PTR loc_1_F962
loc_1_F962:
        cmp    byte ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_F992
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_FA21
loc_1_F992:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_F9C9
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffh
        add    eax, 3
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_FA21
loc_1_F9C9:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_FA01
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_FA21
loc_1_FA01:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffh
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 4], eax
loc_1_FA21:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_FA3C
        mov    al, 40h
        jmp    NEAR PTR loc_1_FA77
loc_1_FA3C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 8]
        mov    cl, byte ptr [ecx + 0bh]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_FA68
        mov    al, 40h
        jmp    NEAR PTR loc_1_FA77
loc_1_FA68:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        xor    al, al
        jmp    NEAR PTR loc_1_FA77
loc_1_FA77:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_FA7C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_1_FA8A
loc_1_FA87:
        dec    dword ptr [ebp + 10h]
loc_1_FA8A:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_1_FAB7
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_FAB7
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 0ch]
        inc    dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_FA87
loc_1_FAB7:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_FACA
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp + 8]
loc_1_FACA:
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_FAD2
loc_1_FAD2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_FAD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_FB11
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_FC41
loc_1_FB11:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, 1fbfh
        je     NEAR PTR loc_1_FB4D
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_FB4D:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, 1fbfh
        je     NEAR PTR loc_1_FB71
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_FB71:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, 1fbfh
        je     NEAR PTR loc_1_FB99
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_FB99:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, 1fbfh
        je     NEAR PTR loc_1_FBB9
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_FBB9:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_FBD1
loc_1_FBCD:
        inc    word ptr [ebp - 4]
loc_1_FBD1:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_FC2E
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_FC29
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_FC29
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_FC29
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0
loc_1_FC29:
        jmp    NEAR PTR loc_1_FBCD
loc_1_FC2E:
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_0106
        add    esp, 0ch
loc_1_FC41:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_FC46:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   90000000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_0137
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_FC6A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   90000000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_0137
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_FC8E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_FCA6
loc_1_FCA3:
        inc    dword ptr [ebp - 8]
loc_1_FCA6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        inc    eax
        cmp    eax, dword ptr [ebp - 8]
        jle    NEAR PTR loc_1_FD0E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, ecx
        shl    ecx, 3
        sub    ecx, edx
        shl    ecx, 2
        lea    ecx, [ecx + ecx*4]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_FD09
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_FD09
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_FD15
loc_1_FD09:
        jmp    NEAR PTR loc_1_FCA3
loc_1_FD0E:
        xor    eax, eax
        jmp    NEAR PTR loc_1_FD15
loc_1_FD15:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_FD1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_FDB3
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 7], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 6], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 5], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 3], al
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp + 14h], eax
loc_1_FDB3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_FDC9
loc_1_FDC1:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_FDC9:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_FE77
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_FE72
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_FE72
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 90h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 10h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_FE72
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 2]
        cmp    dword ptr [eax + 93h], ecx
        jne    NEAR PTR loc_1_FE59
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 91h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_FE59
        mov    dword ptr [ebp - 10h], 1
        jmp    NEAR PTR loc_1_FE60
loc_1_FE59:
        mov    dword ptr [ebp - 10h], 0
loc_1_FE60:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_FE72
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_FE7E
loc_1_FE72:
        jmp    NEAR PTR loc_1_FDC1
loc_1_FE77:
        xor    eax, eax
        jmp    NEAR PTR loc_1_FE7E
loc_1_FE7E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_FE83:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 8], 1
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 58h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_FEA7
loc_1_FEA0:
        inc    byte ptr [ebp - 8]
        add    dword ptr [ebp - 4], 58h
loc_1_FEA7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jg     NEAR PTR loc_1_FF14
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_FF0F
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 1
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 7], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_FF3A
loc_1_FF0F:
        jmp    NEAR PTR loc_1_FEA0
loc_1_FF14:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_FF3A
loc_1_FF3A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_FF3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_FF7F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_1007E
loc_1_FF7F:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0ch
        mov    dword ptr [ebp - 8], eax
        mov    eax, 1000h
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0fffh
        sub    eax, ecx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 14h], eax
        jae    NEAR PTR loc_1_FFBC
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 14h]
        dec    eax
        shr    eax, 0ch
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_FFC3
loc_1_FFBC:
        mov    dword ptr [ebp - 4], 1
loc_1_FFC3:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0046
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0ch], eax
        je     NEAR PTR loc_1_10019
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_1007E
loc_1_10019:
        push   0c0000000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_003A
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_10057
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fffh
        add    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_1007E
        db  0E9h, 027h, 000h, 000h, 000h
loc_1_10057:
        push   39h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2b2h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 8
        jmp    NEAR PTR loc_1_1007E
loc_1_1007E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10083:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 24h]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 24h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 24h]
        mov    word ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [eax + 20h], 0
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 24h]
        mov    word ptr [ecx + 24h], ax
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_100D4
loc_1_100D0:
        inc    word ptr [ebp - 4]
loc_1_100D4:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_10127
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_10112
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 24h]
        mov    byte ptr [ecx + edx + 26h], al
        jmp    NEAR PTR loc_1_10122
loc_1_10112:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 24h]
        mov    byte ptr [eax + ecx + 26h], 0
loc_1_10122:
        jmp    NEAR PTR loc_1_100D0
loc_1_10127:
        cmp    dword ptr [ebp + 1ch], 0
        je     NEAR PTR loc_1_1017E
        mov    eax, dword ptr [ebp + 24h]
        mov    word ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 24h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 24h]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_10179
        mov    eax, dword ptr [ebp + 24h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_24419
        add    esp, 4
loc_1_10179:
        jmp    NEAR PTR loc_1_101A4
loc_1_1017E:
        mov    eax, dword ptr [ebp + 24h]
        mov    word ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 24h]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [eax + 10h], 0
loc_1_101A4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_101A9:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_101CB
        jmp    NEAR PTR loc_1_103BB
loc_1_101CB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_101E7
loc_1_101E1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
loc_1_101E7:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_103BB
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_103B6
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        cmp    eax, 10h
        jne    NEAR PTR loc_1_102BB
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1024F
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   10h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A8BA
        add    esp, 10h
loc_1_1024F:
        lea    eax, [ebp - 14h]
        push   eax
        push   7
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B98
        add    esp, 10h
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_10282
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_10282:
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_103C0
        add    esp, 1ch
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_46AA
        add    esp, 8
        jmp    NEAR PTR loc_1_103B6
loc_1_102BB:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        cmp    eax, 40h
        jne    NEAR PTR loc_1_10394
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        cmp    eax, 7
        jne    NEAR PTR loc_1_10394
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_10317
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   40h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A8BA
        add    esp, 10h
loc_1_10317:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 38h
        push   eax
        call   sub_1_19726
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_1034E
        lea    eax, [ebp - 14h]
        push   eax
        push   62h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        jmp    NEAR PTR loc_1_10317
loc_1_1034E:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1036B
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_1036B:
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_103C0
        add    esp, 1ch
        jmp    NEAR PTR loc_1_103B6
loc_1_10394:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_103C0
        add    esp, 1ch
loc_1_103B6:
        jmp    NEAR PTR loc_1_101E1
loc_1_103BB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_103C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1048A
loc_1_103D6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_104AD
loc_1_103E4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_104AD
loc_1_103F2:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_1043C
loc_1_103FF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_10463
loc_1_1040D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_10463
loc_1_1041B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_10463
loc_1_10429:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_10463
        db  0E9h, 027h, 000h, 000h, 000h
loc_1_1043C:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 3
        ja     NEAR PTR loc_1_10463
        mov    eax, dword ptr [ebp - 14h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_10463:
        jmp    NEAR PTR loc_1_104AD
loc_1_10468:
        push   1
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   3e9h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_104AD
loc_1_1048A:
        cmp    dword ptr [ebp - 10h], 10h
        je     NEAR PTR loc_1_103F2
        cmp    dword ptr [ebp - 10h], 20h
        je     NEAR PTR loc_1_103E4
        cmp    dword ptr [ebp - 10h], 40h
        je     NEAR PTR loc_1_103D6
        jmp    NEAR PTR loc_1_10468
loc_1_104AD:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_10527
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_10527
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1A8BA
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_10527
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        call   sub_1_10083
        add    esp, 20h
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
loc_1_10527:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1052C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1053A:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        jae    NEAR PTR loc_1_10570
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0ch], eax
        ja     NEAR PTR loc_1_1056B
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 8], eax
        jbe    NEAR PTR loc_1_1056B
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_1058D
loc_1_1056B:
        jmp    NEAR PTR loc_1_10586
loc_1_10570:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0ch], eax
        ja     NEAR PTR loc_1_10586
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_1058D
loc_1_10586:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1058D
loc_1_1058D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10592:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_105CE
        push   38h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   44dh
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_1067E
loc_1_105CE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        shr    eax, 0ch
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, 1000h
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0fffh
        sub    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 10h], eax
        jae    NEAR PTR loc_1_1060F
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 10h]
        dec    eax
        shr    eax, 0ch
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_10616
loc_1_1060F:
        mov    dword ptr [ebp - 4], 1
loc_1_10616:
        push   40000000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0040
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_10657
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_1064B
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
loc_1_1064B:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1067E
        db  0E9h, 027h, 000h, 000h, 000h
loc_1_10657:
        push   3ah
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   477h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_1067E
loc_1_1067E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10683:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_10729
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 20h
        je     NEAR PTR loc_1_106FA
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 52h], 3
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_106F5
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0b4h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ch], 0d580h
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b3h], 11h
loc_1_106F5:
        jmp    NEAR PTR loc_1_10729
loc_1_106FA:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 52h], 2
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_10729
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0b4h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b3h], 0eh
loc_1_10729:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1072E:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_1076E
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_10814
loc_1_1076E:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0ch
        mov    dword ptr [ebp - 8], eax
        mov    eax, 1000h
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0fffh
        sub    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 10h], eax
        jae    NEAR PTR loc_1_107AB
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 10h]
        dec    eax
        shr    eax, 0ch
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_107B2
loc_1_107AB:
        mov    dword ptr [ebp - 4], 1
loc_1_107B2:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0046
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0ch], eax
        jne    NEAR PTR loc_1_107DF
        xor    eax, eax
        jmp    NEAR PTR loc_1_10814
        db  0E9h, 035h, 000h, 000h, 000h
loc_1_107DF:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_10814
loc_1_10814:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_1081C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 10h]
        and    eax, 0feh
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        je     NEAR PTR loc_1_10885
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        cmp    eax, ecx
        je     NEAR PTR loc_1_10885
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FC8E
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_10885
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 2
        jne    NEAR PTR loc_1_108BA
loc_1_10885:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 45h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10A1A
loc_1_108BA:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 60h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_10916
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 60h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 49h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10A1A
loc_1_10916:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 5
        cmp    al, 5
        jne    NEAR PTR loc_1_1094B
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx + 62h]
        and    ecx, 0ffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    dl, byte ptr [edx + 62h]
        and    edx, 8
        xor    ecx, edx
        and    ecx, 8
        cmp    eax, ecx
        je     NEAR PTR loc_1_10992
loc_1_1094B:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 6
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10A1A
loc_1_10992:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_10CCD
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_109BB
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 12h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10A1A
loc_1_109BB:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_10CCD
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_109F0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_11023
        add    esp, 4
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 12h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10A1A
loc_1_109F0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        inc    byte ptr [eax + 64h]
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_10A1A
loc_1_10A1A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10A1F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_10A34
loc_1_10A31:
        inc    byte ptr [ebp - 4]
loc_1_10A34:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_10A7D
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1081C
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_10A78
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10A84
loc_1_10A78:
        jmp    NEAR PTR loc_1_10A31
loc_1_10A7D:
        xor    eax, eax
        jmp    NEAR PTR loc_1_10A84
loc_1_10A84:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10A89:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_10AA8
loc_1_10AA0:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_10AA8:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_10B1D
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_10AC1
loc_1_10ABD:
        inc    word ptr [ebp - 14h]
loc_1_10AC1:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 14h
        jge    NEAR PTR loc_1_10B18
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + ecx + 4]
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_10B0E
        lea    eax, [ebp - 4]
        push   eax
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10B63
        add    esp, 10h
        jmp    NEAR PTR loc_1_10B13
loc_1_10B0E:
        jmp    NEAR PTR loc_1_10B18
loc_1_10B13:
        jmp    NEAR PTR loc_1_10ABD
loc_1_10B18:
        jmp    NEAR PTR loc_1_10AA0
loc_1_10B1D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_10B31
loc_1_10B2B:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
loc_1_10B31:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_10B54
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1923E
        add    esp, 4
        jmp    NEAR PTR loc_1_10B2B
loc_1_10B54:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 5ch], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10B63:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 10h]
        and    eax, 0feh
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        je     NEAR PTR loc_1_10BA1
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FC8E
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_10BD6
loc_1_10BA1:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 45h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10CC8
loc_1_10BD6:
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_10F62
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_10C2C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 45h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10CC8
loc_1_10C2C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_10F62
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_10C8B
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 45h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_10CC8
loc_1_10C8B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_10CA3
        mov    eax, dword ptr [ebp - 4]
        dec    byte ptr [eax + 64h]
loc_1_10CA3:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_11023
        add    esp, 4
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_10CC8
loc_1_10CC8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10CCD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_10D34
        call   sub_1_10E16
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 5ch], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        je     NEAR PTR loc_1_10D0E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        add    eax, 4
        jmp    NEAR PTR loc_1_10E11
        db  0E9h, 026h, 000h, 000h, 000h
loc_1_10D0E:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_10E11
loc_1_10D34:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_10D5F
loc_1_10D4B:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 0ch], 0
loc_1_10D5F:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_10DC3
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_10D78
loc_1_10D74:
        inc    word ptr [ebp - 0ch]
loc_1_10D78:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 14h
        jge    NEAR PTR loc_1_10DBE
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx + 4]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_10DB9
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [eax + ecx + 4]
        jmp    NEAR PTR loc_1_10E11
loc_1_10DB9:
        jmp    NEAR PTR loc_1_10D74
loc_1_10DBE:
        jmp    NEAR PTR loc_1_10D4B
loc_1_10DC3:
        call   sub_1_10E16
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_10DEB
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        add    eax, 4
        jmp    NEAR PTR loc_1_10E11
        db  0E9h, 026h, 000h, 000h, 000h
loc_1_10DEB:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_10E11
loc_1_10E11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10E16:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_19428
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_10E6F
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_10E49
loc_1_10E45:
        inc    word ptr [ebp - 8]
loc_1_10E49:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 14h
        jge    NEAR PTR loc_1_10E6F
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 4], 0
        jmp    NEAR PTR loc_1_10E45
loc_1_10E6F:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_10E77
loc_1_10E77:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10E7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_10EA2
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 5ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 0
loc_1_10EA2:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_10F47
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx]
        mov    al, byte ptr [eax + ecx + 4]
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_10F47
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FC8E
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_10F25
        mov    eax, dword ptr [ebp + 14h]
        inc    byte ptr [eax]
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 14h
        jne    NEAR PTR loc_1_10F18
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 0
loc_1_10F18:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_10F5D
        db  0E9h, 01Dh, 000h, 000h, 000h
loc_1_10F25:
        push   4
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   21ch
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_10F5D
loc_1_10F47:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_10F5D
loc_1_10F5D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10F62:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_10F8E
        mov    al, byte ptr [ebp + 0ch]
        and    eax, 0feh
        mov    byte ptr [ebp + 0ch], al
        jmp    NEAR PTR loc_1_10F98
loc_1_10F8E:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        or     al, 1
        mov    byte ptr [ebp + 0ch], al
loc_1_10F98:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_10FAE
loc_1_10FA6:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_10FAE:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_11017
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_10FC7
loc_1_10FC3:
        inc    word ptr [ebp - 8]
loc_1_10FC7:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 14h
        jge    NEAR PTR loc_1_11012
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 4]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1100D
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + ecx + 4]
        jmp    NEAR PTR loc_1_1101E
loc_1_1100D:
        jmp    NEAR PTR loc_1_10FC3
loc_1_11012:
        jmp    NEAR PTR loc_1_10FA6
loc_1_11017:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1101E
loc_1_1101E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_11023:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_11051
loc_1_11043:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    word ptr [ebp - 10h], 0
loc_1_11051:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_11171
        mov    word ptr [ebp - 10h], 0
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_11070
loc_1_1106C:
        inc    word ptr [ebp - 10h]
loc_1_11070:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 14h
        jge    NEAR PTR loc_1_1116C
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_110EA
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_110EA
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0ch], eax
        jne    NEAR PTR loc_1_110EA
        inc    word ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 14h
        jl     NEAR PTR loc_1_110E5
        mov    word ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_110E5:
        jmp    NEAR PTR loc_1_11167
loc_1_110EA:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_11167
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1112B
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0ch], eax
        je     NEAR PTR loc_1_11167
loc_1_1112B:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx + 4]
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 4], al
        inc    word ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 14h
        jl     NEAR PTR loc_1_11167
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_11167:
        jmp    NEAR PTR loc_1_1106C
loc_1_1116C:
        jmp    NEAR PTR loc_1_11043
loc_1_11171:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1118E
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_11194
loc_1_1118E:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
loc_1_11194:
        jmp    NEAR PTR loc_1_1119F
loc_1_11199:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1119F:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_111C2
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
        jmp    NEAR PTR loc_1_11199
loc_1_111C2:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_1121B
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 5ch], ecx
        jne    NEAR PTR loc_1_1121B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_11211
        push   1eh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2c3h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_11211:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 5ch], 0
loc_1_1121B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_11220:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_11235
loc_1_11232:
        inc    byte ptr [ebp - 4]
loc_1_11235:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_112CD
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_11296
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10B63
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_11291
        jmp    NEAR PTR loc_1_112CD
loc_1_11291:
        jmp    NEAR PTR loc_1_112C8
loc_1_11296:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1081C
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_112C8
        jmp    NEAR PTR loc_1_112CD
loc_1_112C8:
        jmp    NEAR PTR loc_1_11232
loc_1_112CD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_11023
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_112E1
loc_1_112E1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_112E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
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
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 3ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 44h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 48h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 50h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 54h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 58h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 5ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 60h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 64h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 68h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 69h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 70h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 71h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 72h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 73h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 74h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 75h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 76h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1148C
loc_1_11488:
        inc    word ptr [ebp - 4]
loc_1_1148C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jae    NEAR PTR loc_1_114B2
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 77h], 0
        jmp    NEAR PTR loc_1_11488
loc_1_114B2:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_114C1
loc_1_114BD:
        inc    word ptr [ebp - 4]
loc_1_114C1:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jae    NEAR PTR loc_1_114E7
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 7dh], 0
        jmp    NEAR PTR loc_1_114BD
loc_1_114E7:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 83h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 87h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 93h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 97h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_11541
loc_1_1153D:
        inc    word ptr [ebp - 4]
loc_1_11541:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 0bh
        jge    NEAR PTR loc_1_1156D
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + eax*4 + 98h], 0
        jmp    NEAR PTR loc_1_1153D
loc_1_1156D:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0c4h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 104h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 108h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10ch], 0deadceceh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_115A5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_11685
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_11685
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    word ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_115E4
loc_1_115E0:
        inc    word ptr [ebp - 4]
loc_1_115E4:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jg     NEAR PTR loc_1_1164F
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp + 8]
        je     NEAR PTR loc_1_1164A
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 14h]
        cmp    dword ptr [ecx + eax*8 + 14h], edx
        jne    NEAR PTR loc_1_1164A
        jmp    NEAR PTR loc_1_1164F
loc_1_1164A:
        jmp    NEAR PTR loc_1_115E0
loc_1_1164F:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jle    NEAR PTR loc_1_11677
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_18D9
        add    esp, 4
loc_1_11677:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_11685:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 9], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 3ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 44h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 48h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 50h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 54h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_117A3
loc_1_1179F:
        inc    word ptr [ebp - 4]
loc_1_117A3:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 14h
        jge    NEAR PTR loc_1_117C9
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 4], 0
        jmp    NEAR PTR loc_1_1179F
loc_1_117C9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_117CE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 6], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_117F5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_0023
        db  0A3h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1183F
loc_1_1183B:
        inc    word ptr [ebp - 4]
loc_1_1183F:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_11868
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  0C7h, 004h, 085h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1183B
loc_1_11868:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 010h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 028h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 064h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 010h, 027h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 00Fh, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 050h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_119D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_11A1A
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        jmp    NEAR PTR loc_1_11A21
loc_1_11A1A:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 1
loc_1_11A21:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 3ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 44h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 48h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 49h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 50h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 52h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 53h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 54h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 58h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 5ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 60h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 64h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 68h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 70h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 71h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 72h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 74h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 75h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 76h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 77h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 79h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 7eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 84h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 86h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 3], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 90h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_11C35
loc_1_11C31:
        inc    word ptr [ebp - 4]
loc_1_11C35:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_11C5E
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 91h], 0
        jmp    NEAR PTR loc_1_11C31
loc_1_11C5E:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 97h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_11C79
loc_1_11C75:
        inc    word ptr [ebp - 4]
loc_1_11C79:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 8
        jge    NEAR PTR loc_1_11CA4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*2 + 99h], 0
        jmp    NEAR PTR loc_1_11C75
loc_1_11CA4:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0a9h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0abh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0adh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0aeh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0afh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b1h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b2h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b3h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_11D09
loc_1_11D05:
        inc    word ptr [ebp - 4]
loc_1_11D09:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 20h
        jge    NEAR PTR loc_1_11D32
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0b4h], 0
        jmp    NEAR PTR loc_1_11D05
loc_1_11D32:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_11D37:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 3ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 44h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 48h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 49h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 50h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 52h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 53h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 54h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 58h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 5ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 60h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 62h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 63h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 66h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 68h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 70h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 74h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 7ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 84h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_11EFB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2], 1
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 50h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 104h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 108h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_11F50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_11FA3
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1237A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_11FA3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_11FA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_12191
loc_1_11FBF:
        xor    eax, eax
        mov    al, byte ptr [ebp + 20h]
        test   eax, eax
        je     NEAR PTR loc_1_12044
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    byte ptr [eax], 11h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [eax + 0ch], 0d580h
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 1ch]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 10h], 0
        jmp    NEAR PTR loc_1_120A7
loc_1_12044:
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    byte ptr [eax], 0eh
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 1ch]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ch], ax
loc_1_120A7:
        jmp    NEAR PTR loc_1_121B4
loc_1_120AC:
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    byte ptr [eax], 0eh
        xor    eax, eax
        mov    al, byte ptr [ebp + 18h]
        or     al, 10h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 1], 40h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0ah], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 8], ax
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_1211B
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 8], al
loc_1_1211B:
        jmp    NEAR PTR loc_1_121B4
loc_1_12120:
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    byte ptr [eax], 0dh
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax], 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 3], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 1], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 9], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 7], ax
        jmp    NEAR PTR loc_1_121B4
loc_1_1216A:
        push   24h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0ceh
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1227E
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_12191:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_11FBF
        cmp    dword ptr [ebp - 20h], 1
        je     NEAR PTR loc_1_120AC
        cmp    dword ptr [ebp - 20h], 2
        je     NEAR PTR loc_1_12120
        jmp    NEAR PTR loc_1_1216A
loc_1_121B4:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_1227E
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp + 28h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_12217
loc_1_12213:
        inc    word ptr [ebp - 14h]
loc_1_12217:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        sub    eax, 2
        cdq
        sub    eax, edx
        sar    eax, 1
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_12264
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    ax, word ptr [ecx + eax*2 + 2]
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 8]
        mov    word ptr [edx + ecx*2 + 2], ax
        jmp    NEAR PTR loc_1_12213
loc_1_12264:
        mov    eax, dword ptr [ebp + 28h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 28h]
        mov    byte ptr [ecx], al
loc_1_1227E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12283:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_12375
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 50h]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [eax + ecx + 3fh]
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        and    eax, 0ffh
        imul   eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_122EA
        mov    eax, ecx
loc_1_122EA:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 18h], ecx
        jb     NEAR PTR loc_1_12375
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0bfh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1232A
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_1232A:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_12361
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_12361:
        push   0
        push   25h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
loc_1_12375:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1237A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
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
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 74h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_124F4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1241F
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        add    eax, 100h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 6dh]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 6dh], ax
        jmp    NEAR PTR loc_1_1244E
loc_1_1241F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 6dh]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 6dh], ax
loc_1_1244E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4dh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 6dh]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_124F4
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 4dh]
        and    ecx, 0ffh
        mov    ax, word ptr [eax + 6dh]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 6ch]
        and    ecx, 0ffh
        lea    eax, [ecx + eax*2]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 4dh]
        and    ecx, 0ffh
        mov    ax, word ptr [eax + 6dh]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 6dh], dx
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 74h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_124EB
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 74h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 6ch], al
        jmp    NEAR PTR loc_1_124F4
loc_1_124EB:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 6ch], al
loc_1_124F4:
        jmp    NEAR PTR loc_1_1250D
loc_1_124F9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 70h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 70h], al
loc_1_1250D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_12587
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23DB6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        inc    word ptr [eax + 0a9h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0a9h]
        and    eax, 0ffffh
        cmp    eax, 8000h
        jne    NEAR PTR loc_1_1257C
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 40h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_1257C:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 78h]
        jmp    NEAR PTR loc_1_124F9
loc_1_12587:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1258C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_12688
loc_1_125A2:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        cmp    eax, 5
        jg     NEAR PTR loc_1_125CD
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_125E8
loc_1_125CD:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 72h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        sub    ecx, 5
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
loc_1_125E8:
        jmp    NEAR PTR loc_1_126AB
loc_1_125ED:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        cmp    eax, 5
        jg     NEAR PTR loc_1_12618
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 70h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_12633
loc_1_12618:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 73h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        sub    ecx, 5
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
loc_1_12633:
        jmp    NEAR PTR loc_1_126AB
loc_1_12638:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        cmp    eax, 5
        jg     NEAR PTR loc_1_12663
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 71h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1267E
loc_1_12663:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 74h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 14h]
        sub    ecx, 5
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
loc_1_1267E:
        jmp    NEAR PTR loc_1_126AB
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_12688:
        cmp    dword ptr [ebp - 4], 31h
        je     NEAR PTR loc_1_125A2
        cmp    dword ptr [ebp - 4], 32h
        je     NEAR PTR loc_1_125ED
        cmp    dword ptr [ebp - 4], 69h
        je     NEAR PTR loc_1_12638
        jmp    NEAR PTR loc_1_126AB
loc_1_126AB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_126B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_1_126E6
loc_1_126CB:
        inc    byte ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 6ah]
        and    ecx, 0ffh
        inc    eax
        cdq
        idiv   ecx
        mov    byte ptr [ebp - 8], dl
loc_1_126E6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_12790
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        add    eax, 4
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
        cmp    eax, 2
        je     NEAR PTR loc_1_12745
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_1278B
loc_1_12745:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_183B8
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_1278B
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_12783
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 6ah]
        and    ecx, 0ffh
        inc    eax
        cdq
        idiv   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], dx
loc_1_12783:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_127BD
loc_1_1278B:
        jmp    NEAR PTR loc_1_126CB
loc_1_12790:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_127B6
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 6ah]
        and    ecx, 0ffh
        inc    eax
        cdq
        idiv   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], dx
loc_1_127B6:
        xor    eax, eax
        jmp    NEAR PTR loc_1_127BD
loc_1_127BD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_127C2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_12856
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 4
        jne    NEAR PTR loc_1_12856
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_1280D
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 4
loc_1_1280D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_12851
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 14h], 0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp - 0ch]
        add    esp, 10h
loc_1_12851:
        jmp    NEAR PTR loc_1_128D0
loc_1_12856:
        push   0
        push   28h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_128D0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_128D0
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
        call   dword ptr [ebp - 0ch]
        add    esp, 10h
loc_1_128D0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_128D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 4
        jmp    NEAR PTR loc_1_128ED
loc_1_128E9:
        inc    word ptr [ebp - 4]
loc_1_128ED:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        add    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_12AC3
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_12953
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_12ABE
loc_1_12953:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 58h], 0
        je     NEAR PTR loc_1_129A0
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 58h]
        mov    edx, dword ptr [ebp + 0ch]
        cmp    ecx, edx
        jb     NEAR PTR loc_1_12975
        mov    ecx, edx
loc_1_12975:
        sub    eax, ecx
        neg    eax
        mov    ecx, dword ptr [ebp - 8]
        sub    dword ptr [ecx + 58h], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 58h], 0
        jne    NEAR PTR loc_1_129A0
        push   0
        push   2ah
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
loc_1_129A0:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 60h], 0
        je     NEAR PTR loc_1_129ED
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 60h]
        mov    edx, dword ptr [ebp + 0ch]
        cmp    ecx, edx
        jb     NEAR PTR loc_1_129C2
        mov    ecx, edx
loc_1_129C2:
        sub    eax, ecx
        neg    eax
        mov    ecx, dword ptr [ebp - 8]
        sub    dword ptr [ecx + 60h], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 60h], 0
        jne    NEAR PTR loc_1_129ED
        push   0
        push   2bh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
loc_1_129ED:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 68h], 0
        je     NEAR PTR loc_1_12A3A
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 68h]
        mov    edx, dword ptr [ebp + 0ch]
        cmp    ecx, edx
        jb     NEAR PTR loc_1_12A0F
        mov    ecx, edx
loc_1_12A0F:
        sub    eax, ecx
        neg    eax
        mov    ecx, dword ptr [ebp - 8]
        sub    dword ptr [ecx + 68h], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 68h], 0
        jne    NEAR PTR loc_1_12A3A
        push   0
        push   29h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
loc_1_12A3A:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 8
        je     NEAR PTR loc_1_12AAE
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 40h], 0
        jne    NEAR PTR loc_1_12AAE
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0f7h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 7dh], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_12A99
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_12A99:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_127C2
        add    esp, 8
        jmp    NEAR PTR loc_1_12ABE
loc_1_12AAE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12283
        add    esp, 8
loc_1_12ABE:
        jmp    NEAR PTR loc_1_128E9
loc_1_12AC3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12AC8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_12B16
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_12B16
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 8], al
        jmp    NEAR PTR loc_1_12B27
loc_1_12B16:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        xor    ax, ax
        jmp    NEAR PTR loc_1_12C42
loc_1_12B27:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    ah, 1fh
        mov    al, ah
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 4], -1
        jne    NEAR PTR loc_1_12B8B
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jne    NEAR PTR loc_1_12B8B
        mov    dword ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_12B92
loc_1_12B8B:
        mov    dword ptr [ebp - 0ch], 0
loc_1_12B92:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_12BBF
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_12BBF
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_12C42
loc_1_12BBF:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 0e000h
        shr    eax, 0dh
        and    eax, 0ffffh
        cmp    eax, 6
        jne    NEAR PTR loc_1_12C0A
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], 7082h
        jmp    NEAR PTR loc_1_12C39
loc_1_12C0A:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax]
        and    eax, 0ff1fh
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        xor    eax, 8000h
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx]
        mov    word ptr [ecx], ax
loc_1_12C39:
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_12C42
loc_1_12C42:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12C47:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_12C7F
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1237A
        add    esp, 8
loc_1_12C7F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 40h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_12C95
loc_1_12C8D:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_12C95:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_12CDB
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_12CC5
        jmp    NEAR PTR loc_1_12C8D
        db  0E9h, 011h, 000h, 000h, 000h
loc_1_12CC5:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 80h], eax
        jmp    NEAR PTR loc_1_12CDB
        db  0E9h, 0B2h, 0FFh, 0FFh, 0FFh
loc_1_12CDB:
        jmp    NEAR PTR loc_1_12CF4
loc_1_12CE0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 71h]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 71h], al
loc_1_12CF4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_12D56
        mov    eax, dword ptr [ebp + 0ch]
        inc    byte ptr [eax + 0aeh]
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0aeh]
        and    eax, 0ffh
        cmp    eax, 80h
        jne    NEAR PTR loc_1_12D51
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 40h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
loc_1_12D51:
        jmp    NEAR PTR loc_1_12CE0
loc_1_12D56:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 6ch], 2
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 6dh], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12D6B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        dec    byte ptr [eax + 75h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_12DBC
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 75h], al
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        push   0ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18494
        add    esp, 0ch
        jmp    NEAR PTR loc_1_12DCB
loc_1_12DBC:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        call   sub_1_12DD0
        add    esp, 4
loc_1_12DCB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12DD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12DE6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_12E0F
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    al, 1
        jmp    NEAR PTR loc_1_12E16
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_12E0F:
        xor    al, al
        jmp    NEAR PTR loc_1_12E16
loc_1_12E16:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12E1B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_12EC9
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1237A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_12E99
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_12EB7
loc_1_12E99:
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
loc_1_12EB7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_12EC9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12ECE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_12FC7
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 74h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_12F99
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 7eh]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 7eh], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 7eh]
        and    eax, 0ffffh
        cmp    eax, 1000h
        jl     NEAR PTR loc_1_12F99
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4bh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 74h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_12F90
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 74h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 74h], al
loc_1_12F90:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 7eh], 2
loc_1_12F99:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1237A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 72h], ax
loc_1_12FC7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_13009
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        jmp    NEAR PTR loc_1_13027
loc_1_13009:
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
loc_1_13027:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_130C7
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 4eh]
        and    ecx, 0ffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_130B7
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 2
        jle    NEAR PTR loc_1_13086
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        sub    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 74h], al
        jmp    NEAR PTR loc_1_130AE
loc_1_13086:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 74h]
        and    eax, 0ffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_130AE
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 74h]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 74h], al
loc_1_130AE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 7eh], 2
loc_1_130B7:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12C47
        add    esp, 8
loc_1_130C7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_130CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_131A7
loc_1_130E8:
        mov    word ptr [ebp - 8], 1ah
        jmp    NEAR PTR loc_1_13200
loc_1_130F3:
        mov    word ptr [ebp - 8], 16h
        jmp    NEAR PTR loc_1_13200
loc_1_130FE:
        mov    word ptr [ebp - 8], 12h
        jmp    NEAR PTR loc_1_13200
loc_1_13109:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_138FC
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_13131
        mov    word ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_13200
        db  0E9h, 03Ah, 000h, 000h, 000h
loc_1_13131:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1315A
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_13166
loc_1_1315A:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_13166:
        jmp    NEAR PTR loc_1_13464
loc_1_1316B:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_13200
loc_1_13176:
        mov    word ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_13200
loc_1_13181:
        mov    word ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_13200
loc_1_1318C:
        mov    word ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_13200
loc_1_13197:
        mov    word ptr [ebp - 8], 1eh
        jmp    NEAR PTR loc_1_13200
        db  0E9h, 059h, 000h, 000h, 000h
loc_1_131A7:
        sub    dword ptr [ebp - 14h], 0a0h
        cmp    dword ptr [ebp - 14h], 0eh
        ja     NEAR PTR loc_1_13197
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 008h, 001h, 008h, 002h, 008h, 003h, 008h, 004h, 008h, 005h, 008h
        db  006h, 008h, 007h
loc_1_13200:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_13250
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_13250
        push   32h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   63h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_13464
loc_1_13250:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_13280
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_13280
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_10683
        add    esp, 0ch
loc_1_13280:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0b1h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        cmp    eax, 0aah
        jl     NEAR PTR loc_1_133D1
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 6fh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_1338A
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 71h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_13346
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_1333B
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_13341
loc_1_1333B:
        mov    word ptr [ebp - 8], 1eh
loc_1_13341:
        jmp    NEAR PTR loc_1_1338A
loc_1_13346:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1338A
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1338A
        mov    word ptr [ebp - 8], 1eh
loc_1_1338A:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_133AC
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 8], ax
loc_1_133AC:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 3]
        and    eax, 1
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_133FE
loc_1_133D1:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_133FA
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        inc    eax
        mov    word ptr [ebp - 8], ax
        mov    byte ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_133FE
loc_1_133FA:
        mov    byte ptr [ebp - 4], 0
loc_1_133FE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 8bh], eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 8bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_13458
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_13464
loc_1_13458:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_13464:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_13469:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_134C0
        push   31h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0ech
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_138F7
loc_1_134C0:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_1350A
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_1350A
        push   30h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0f6h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_138F7
loc_1_1350A:
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 6fh], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 0b1h], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 76h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_13628
        mov    word ptr [ebp - 14h], 0ah
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_13623
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 71h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_135DF
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_135D4
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_135DA
loc_1_135D4:
        mov    word ptr [ebp - 14h], 1eh
loc_1_135DA:
        jmp    NEAR PTR loc_1_13623
loc_1_135DF:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 70h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_13623
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 6fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 71h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_13623
        mov    word ptr [ebp - 14h], 1eh
loc_1_13623:
        jmp    NEAR PTR loc_1_1362E
loc_1_13628:
        mov    word ptr [ebp - 14h], 0eh
loc_1_1362E:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 0ah
        jne    NEAR PTR loc_1_1368F
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_1368F
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_1368A
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_1368A
        push   40h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_146A5
        add    esp, 0ch
loc_1_1368A:
        jmp    NEAR PTR loc_1_13696
loc_1_1368F:
        mov    dword ptr [ebp - 18h], 0
loc_1_13696:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 0ah
        jne    NEAR PTR loc_1_13750
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        cmp    eax, 5
        jne    NEAR PTR loc_1_13750
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12E1B
        add    esp, 8
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 76h], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_13737
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_13737
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 5ch
        push   eax
        call   sub_1_12DE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 75h], al
        push   0dh
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18494
        add    esp, 0ch
        jmp    NEAR PTR loc_1_13747
loc_1_13737:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12D6B
        add    esp, 8
loc_1_13747:
        mov    byte ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_137C6
loc_1_13750:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_13772
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 14h], ax
loc_1_13772:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 1
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 8bh], eax
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 8bh], 0
loc_1_137C6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        jne    NEAR PTR loc_1_13867
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_13867
        mov    eax, dword ptr [ebp + 10h]
        inc    word ptr [eax + 0abh]
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 0abh]
        and    eax, 0ffffh
        cmp    eax, 8000h
        jne    NEAR PTR loc_1_1381D
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 40h
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 84h], ax
loc_1_1381D:
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 7ch]
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CFD9
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CDF3
        add    esp, 10h
        jmp    NEAR PTR loc_1_138F7
loc_1_13867:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        jne    NEAR PTR loc_1_138A5
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_138A5
        push   1fh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   18ah
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_138F7
        db  0E9h, 052h, 000h, 000h, 000h
loc_1_138A5:
        mov    eax, dword ptr [ebp + 10h]
        inc    byte ptr [eax + 0adh]
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0adh]
        and    eax, 0ffh
        cmp    eax, 80h
        jne    NEAR PTR loc_1_138E3
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 40h
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 84h], ax
loc_1_138E3:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CFD9
        add    esp, 0ch
loc_1_138F7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_138FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_1395D
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_1395D
        push   33h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   1bah
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_13E54
loc_1_1395D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0feh
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_1399D
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_139D2
loc_1_1399D:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_13E54
loc_1_139D2:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_13A12
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_13A12
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 8]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        add    eax, 0eh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_13A19
loc_1_13A12:
        mov    dword ptr [ebp - 8], 0
loc_1_13A19:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_13A36
loc_1_13A2E:
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 1ch], eax
loc_1_13A36:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_13BA6
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_13A69
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_13A64
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 14h], eax
loc_1_13A64:
        jmp    NEAR PTR loc_1_13BA1
loc_1_13A69:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 90h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_13BA1
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 93h]
        cmp    dword ptr [eax + 0ah], ecx
        jne    NEAR PTR loc_1_13AC9
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 1ch]
        mov    cx, word ptr [ecx + 91h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_13AC9
        mov    dword ptr [ebp - 24h], 1
        jmp    NEAR PTR loc_1_13AD0
loc_1_13AC9:
        mov    dword ptr [ebp - 24h], 0
loc_1_13AD0:
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_1_13BA1
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_13B9A
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ff1fh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        xor    eax, 8000h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 1ch]
        add    ecx, 97h
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 70h
        shr    eax, 4
        and    eax, 0ffffh
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 50h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_13B93
        mov    eax, ecx
loc_1_13B93:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 50h], ax
loc_1_13B9A:
        xor    eax, eax
        jmp    NEAR PTR loc_1_13E54
loc_1_13BA1:
        jmp    NEAR PTR loc_1_13A2E
loc_1_13BA6:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_13BFA
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 8fh], 0
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 6bh]
        mov    eax, dword ptr [ebp - 0ch]
        inc    byte ptr [eax + 65h]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_13BEC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_10683
        add    esp, 0ch
loc_1_13BEC:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_1_13C23
loc_1_13BFA:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_13E54
loc_1_13C23:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 48h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 48h], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 49h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 49h], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4ah]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4ah], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4bh]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4bh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4ch], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4dh]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4dh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4eh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 50h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 50h], ax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 93h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 91h], ax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 90h], al
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_13D3E
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ff1fh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        xor    eax, 8000h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 70h
        shr    eax, 4
        and    eax, 0ffffh
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 50h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_13D37
        mov    eax, ecx
loc_1_13D37:
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 50h], ax
loc_1_13D3E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_13DDB
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 0dh], al
loc_1_13DDB:
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0b3h
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0b4h
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        sub    eax, 3
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 7dh
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11FA8
        add    esp, 24h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14953
        add    esp, 8
        push   0
        push   22h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        xor    eax, eax
        jmp    NEAR PTR loc_1_13E54
loc_1_13E54:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_13E59:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_13E84
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_1_13E96
loc_1_13E84:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        add    eax, 1ah
        mov    dword ptr [ebp - 2ch], eax
loc_1_13E96:
        mov    byte ptr [ebp - 17h], 0
        mov    dword ptr [ebp - 16h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 28h], eax
        mov    byte ptr [ebp - 12h], 3
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0feh
        mov    byte ptr [ebp - 11h], al
        mov    byte ptr [ebp - 10h], 1
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0fh], al
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12AC8
        add    esp, 0ch
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_13F7E
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 0dh], al
loc_1_13F7E:
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 20h
        shr    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 7dh
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11FA8
        add    esp, 24h
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 1bh], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_140BC
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        add    ecx, 11h
        sub    eax, ecx
        mov    byte ptr [ebp - 0dh], al
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 3
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        cmp    eax, 8
        je     NEAR PTR loc_1_1404E
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        cmp    eax, 0ah
        jne    NEAR PTR loc_1_1409D
loc_1_1404E:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 81h
        je     NEAR PTR loc_1_1409D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1408C
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_14098
loc_1_1408C:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_14098:
        jmp    NEAR PTR loc_1_14109
loc_1_1409D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22750
        add    esp, 14h
        jmp    NEAR PTR loc_1_14109
loc_1_140BC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_140E5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_140F1
loc_1_140E5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_140F1:
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_223FE
        add    esp, 10h
loc_1_14109:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_14110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_1413A
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_14195
loc_1_1413A:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_1417C
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 1ah
        jne    NEAR PTR loc_1_1417C
        push   2
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp + 14h]
        add    esp, 10h
        jmp    NEAR PTR loc_1_142B7
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_1417C:
        push   4ah
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp + 14h]
        add    esp, 10h
        jmp    NEAR PTR loc_1_142B7
loc_1_14195:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 40h], 0
        je     NEAR PTR loc_1_14279
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_14233
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 40h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_14233
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_141F3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_141F3:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22BFE
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14573
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 4
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp + 14h]
        add    esp, 10h
        jmp    NEAR PTR loc_1_142B7
loc_1_14233:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        or     al, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_14274
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_14274:
        jmp    NEAR PTR loc_1_142B7
loc_1_14279:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_142A7
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_142A7:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_127C2
        add    esp, 8
loc_1_142B7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_142BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_142E9
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_14344
loc_1_142E9:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_1432B
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 1bh
        jne    NEAR PTR loc_1_1432B
        push   2
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp + 14h]
        add    esp, 10h
        jmp    NEAR PTR loc_1_1456E
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_1432B:
        push   4ah
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp + 14h]
        add    esp, 10h
        jmp    NEAR PTR loc_1_1456E
loc_1_14344:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 8fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_144AC
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_143FC
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 91h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 92h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0bh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 93h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0ah], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 94h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 9], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 95h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 96h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 7], al
        jmp    NEAR PTR loc_1_14416
loc_1_143FC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 93h]
        mov    dword ptr [ebp - 0ah], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 91h]
        mov    word ptr [ebp - 0ch], ax
loc_1_14416:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_14440
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_14440
        mov    byte ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_14444
loc_1_14440:
        mov    byte ptr [ebp - 4], 0
loc_1_14444:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 97h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1446B
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 97h
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_14472
loc_1_1446B:
        mov    dword ptr [ebp - 1ch], 0
loc_1_14472:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0b3h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0b4h
        push   eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4fh]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 7dh
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11FA8
        add    esp, 24h
loc_1_144AC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0fbh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        push   0
        push   22h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        push   0
        push   27h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        je     NEAR PTR loc_1_14531
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 14h], 0
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp - 18h]
        add    esp, 10h
loc_1_14531:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1456E
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 8fh]
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
        add    esp, 10h
loc_1_1456E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14573:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   23h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_14645
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_14645
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_14645
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_145E8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_145E8:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_14623
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 53h]
        and    eax, 0ffh
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_14623:
        push   13h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18494
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
loc_1_14645:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_12DE6
        add    esp, 4
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
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_146A0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_146A0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_146A5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        cmp    eax, 80h
        jne    NEAR PTR loc_1_14716
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_1470C
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_14701
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_14701:
        mov    word ptr [ebp - 4], 25h
        jmp    NEAR PTR loc_1_14711
loc_1_1470C:
        jmp    NEAR PTR loc_1_14863
loc_1_14711:
        jmp    NEAR PTR loc_1_1484D
loc_1_14716:
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        cmp    eax, 0c0h
        jne    NEAR PTR loc_1_14780
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_14776
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0bfh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1476B
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_1476B:
        mov    word ptr [ebp - 4], 25h
        jmp    NEAR PTR loc_1_1477B
loc_1_14776:
        jmp    NEAR PTR loc_1_14863
loc_1_1477B:
        jmp    NEAR PTR loc_1_1484D
loc_1_14780:
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        cmp    eax, 40h
        jne    NEAR PTR loc_1_14806
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_147AB
        jmp    NEAR PTR loc_1_14863
        db  0E9h, 056h, 000h, 000h, 000h
loc_1_147AB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        or     al, 40h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        or     eax, 1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 84h], ax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_147FB
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_147FB:
        mov    word ptr [ebp - 4], 24h
        jmp    NEAR PTR loc_1_1484D
loc_1_14806:
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   eax, eax
        jne    NEAR PTR loc_1_1484D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_14847
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_14847:
        mov    word ptr [ebp - 4], 24h
loc_1_1484D:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14FC4
        add    esp, 10h
loc_1_14863:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14868:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4bh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 74h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_148D0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4bh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 74h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 74h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_148D0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 74h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 6ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 7eh], 2
loc_1_148D0:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_148FA
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0b4h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
loc_1_148FA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 48h]
        push   eax
        push   31h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1258C
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 49h]
        push   eax
        push   32h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1258C
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ah]
        push   eax
        push   69h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1258C
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_14953:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 8fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1497B
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 1
        jmp    NEAR PTR loc_1_14995
loc_1_1497B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 53h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        or     al, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
loc_1_14995:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7dh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4eh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4bh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 74h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 74h]
        and    eax, 0ffh
        cmp    eax, 10h
        jl     NEAR PTR loc_1_149D9
        mov    eax, 10h
loc_1_149D9:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 6ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 48h]
        push   eax
        push   31h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1258C
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 49h]
        push   eax
        push   32h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 5ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1258C
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4ah]
        push   eax
        push   69h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1258C
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        push   eax
        call   sub_1_12DD0
        add    esp, 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_14A7F
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 8fh]
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
        add    esp, 10h
loc_1_14A7F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
