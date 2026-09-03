; dlc_lcode_01_07.asm -- DLC object 1 segment _LTEXT  (part 7/9)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_1F24C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        shr    eax, 4
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F282:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        shr    eax, 0ch
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        shr    eax, 4
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 3], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F2EC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 1ch
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 18h
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 14h
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 10h
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0ch
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 8
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 4
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 7], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F39B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    ecx, 2710h
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1F3F4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + edx], al
        inc    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_1F3FB
loc_1_1F3F4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 20h
loc_1_1F3FB:
        mov    ecx, 3e8h
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1F44D
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + edx], al
        inc    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_1F464
loc_1_1F44D:
        mov    eax, 3
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + ecx], 20h
loc_1_1F464:
        mov    ecx, 64h
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1F4B6
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + edx], al
        inc    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_1F4CD
loc_1_1F4B6:
        mov    eax, 2
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + ecx], 20h
loc_1_1F4CD:
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1F51F
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + edx], al
        inc    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_1F536
loc_1_1F51F:
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + ecx], 20h
loc_1_1F536:
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + edx], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F560:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jl     NEAR PTR loc_1_1F584
        mov    eax, ecx
loc_1_1F584:
        mov    word ptr [ebp + 8], ax
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    ecx, ecx
        inc    ecx
        mov    eax, 51h
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jb     NEAR PTR loc_1_1F5B0
        mov    eax, ecx
loc_1_1F5B0:
        mov    word ptr [ebp + 0ch], ax
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], 0
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1F5E3
loc_1_1F5CC:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
loc_1_1F5E3:
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1F733
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cdq
        idiv   ecx
        test   edx, edx
        jne    NEAR PTR loc_1_1F678
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1F678
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 0ah
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 20h
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1F685
loc_1_1F678:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 20h
        inc    dword ptr [ebp - 0ch]
        inc    word ptr [ebp - 4]
loc_1_1F685:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1F6B2
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        push   eax
        call   sub_1_1F24C
        add    esp, 8
        jmp    NEAR PTR loc_1_1F6FE
loc_1_1F6B2:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1F6DB
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax]
        push   eax
        call   sub_1_1F282
        add    esp, 8
        jmp    NEAR PTR loc_1_1F6FE
loc_1_1F6DB:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_1F6FE
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1F2EC
        add    esp, 8
loc_1_1F6FE:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        add    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        add    eax, eax
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        lea    eax, [ecx + eax*2]
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_1F5CC
loc_1_1F733:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1F75E
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 0ah
        inc    word ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
loc_1_1F75E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F763:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jl     NEAR PTR loc_1_1F787
        mov    eax, ecx
loc_1_1F787:
        mov    word ptr [ebp + 8], ax
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 8], 0
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_1F7A7
loc_1_1F7A3:
        inc    word ptr [ebp - 10h]
loc_1_1F7A7:
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1F89F
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0fh
        xor    eax, edx
        sub    eax, edx
        jne    NEAR PTR loc_1_1F843
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 0ah
        inc    word ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_1F804
loc_1_1F800:
        inc    word ptr [ebp - 14h]
loc_1_1F804:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 33h
        jge    NEAR PTR loc_1_1F829
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        db  0C6h, 080h, 000h, 000h, 000h, 000h, 020h
        jmp    NEAR PTR loc_1_1F800
loc_1_1F829:
        db  0B8h, 000h, 000h, 000h, 000h
        inc    eax
        mov    dword ptr [ebp - 18h], eax
        db  0B8h, 000h, 000h, 000h, 000h
        add    eax, 33h
        mov    dword ptr [ebp - 0ch], eax
        mov    word ptr [ebp - 8], 33h
loc_1_1F843:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        movsx  ax, byte ptr [ebp - 4]
        push   eax
        call   sub_1_1F24C
        add    esp, 8
        add    dword ptr [ebp - 18h], 3
        movsx  eax, byte ptr [ebp - 4]
        and    al, 7fh
        mov    byte ptr [ebp - 4], al
        movsx  eax, byte ptr [ebp - 4]
        cmp    eax, 20h
        jl     NEAR PTR loc_1_1F887
        movsx  eax, byte ptr [ebp - 4]
        cmp    eax, 7fh
        jl     NEAR PTR loc_1_1F88B
loc_1_1F887:
        mov    byte ptr [ebp - 4], 2eh
loc_1_1F88B:
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_1F7A3
loc_1_1F89F:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 33h
        je     NEAR PTR loc_1_1F8CB
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 0ah
        inc    word ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
loc_1_1F8CB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F8D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0f4240h
        jae    NEAR PTR loc_1_1F9BC
        mov    ecx, 186a0h
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
        mov    ecx, 2710h
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], eax
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], al
        mov    ecx, 3e8h
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], eax
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 2], al
        mov    ecx, 64h
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], eax
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 3], al
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], eax
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 4], al
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        div    ecx
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 5], al
        jmp    NEAR PTR loc_1_1F9E6
loc_1_1F9BC:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 02Ah
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 02Ah
        db  0C6h, 005h, 002h, 000h, 000h, 000h, 02Ah
        db  0C6h, 005h, 003h, 000h, 000h, 000h, 02Ah
        db  0C6h, 005h, 004h, 000h, 000h, 000h, 02Ah
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 02Ah
loc_1_1F9E6:
        db  0C6h, 005h, 006h, 000h, 000h, 000h, 020h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F9F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_1_1FA29
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, eax
        jne    NEAR PTR loc_1_1FA29
        jmp    NEAR PTR loc_1_201EA
loc_1_1FA29:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 40h
        je     NEAR PTR loc_1_1FAC8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_1_1FA54
        jmp    NEAR PTR loc_1_201EA
        db  0E9h, 074h, 000h, 000h, 000h
loc_1_1FA54:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 38h], ax
        db  066h, 0FFh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 38h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_1FAC8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        or     eax, 8
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1FAB5
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1FAB5
        call   sub_1_2025E
loc_1_1FAB5:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        jmp    NEAR PTR loc_1_201EA
loc_1_1FAC8:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        lea    eax, [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_1_1FB0D
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1FB0D
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_1FC0C
loc_1_1FB0D:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1FB12:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        db  02Bh, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 1ch]
        db  0A3h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_1F8D0
        add    esp, 8
        mov    word ptr [ebp - 28h], 0
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 10h], ax
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_1FB59
loc_1_1FB55:
        inc    word ptr [ebp - 28h]
loc_1_1FB59:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1FBA7
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_1FBA7
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 14h], ax
        shr    word ptr [ebp - 10h], 1
        jmp    NEAR PTR loc_1_1FB55
loc_1_1FBA7:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1FBD0
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        db  08Ah, 080h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0A2h, 007h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1FBF4
loc_1_1FBD0:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        jle    NEAR PTR loc_1_1FBED
        db  0C6h, 005h, 007h, 000h, 000h, 000h, 041h
        jmp    NEAR PTR loc_1_1FBF4
loc_1_1FBED:
        db  0C6h, 005h, 007h, 000h, 000h, 000h, 04Eh
loc_1_1FBF4:
        db  0C6h, 005h, 008h, 000h, 000h, 000h, 020h
        db  0B8h, 000h, 000h, 000h, 000h
        add    eax, 9
        mov    dword ptr [ebp - 2ch], eax
        mov    word ptr [ebp - 20h], 9
loc_1_1FC0C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        inc    dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_2019C
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 25h
        jne    NEAR PTR loc_1_20159
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_1_20159
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        inc    dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    dword ptr [ebp - 40h], eax
        jmp    NEAR PTR loc_1_200A6
loc_1_1FC58:
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 2
        cmp    eax, 52h
        jbe    NEAR PTR loc_1_1FC8A
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_1FC8A:
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        push   eax
        call   sub_1_1F24C
        add    esp, 8
        add    dword ptr [ebp - 2ch], 2
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 20h], ax
        jmp    NEAR PTR loc_1_20154
loc_1_1FCBA:
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 4
        cmp    eax, 52h
        jbe    NEAR PTR loc_1_1FCEC
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_1FCEC:
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        push   eax
        call   sub_1_1F282
        add    esp, 8
        add    dword ptr [ebp - 2ch], 4
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 4
        mov    word ptr [ebp - 20h], ax
        jmp    NEAR PTR loc_1_20154
loc_1_1FD1C:
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 8
        cmp    eax, 52h
        jbe    NEAR PTR loc_1_1FD4E
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_1FD4E:
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax - 4]
        push   eax
        call   sub_1_1F2EC
        add    esp, 8
        add    dword ptr [ebp - 2ch], 8
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 8
        mov    word ptr [ebp - 20h], ax
        jmp    NEAR PTR loc_1_20154
loc_1_1FD7D:
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 5
        cmp    eax, 52h
        jbe    NEAR PTR loc_1_1FDAF
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_1FDAF:
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        push   eax
        call   sub_1_1F39B
        add    esp, 8
        add    dword ptr [ebp - 2ch], 5
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, 5
        mov    word ptr [ebp - 20h], ax
        jmp    NEAR PTR loc_1_20154
loc_1_1FDDF:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 18h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1F763
        add    esp, 8
        jmp    NEAR PTR loc_1_20154
loc_1_1FE2E:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 18h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 30h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1F560
        add    esp, 10h
        jmp    NEAR PTR loc_1_20154
loc_1_1FE92:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 18h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 30h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   2
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1F560
        add    esp, 10h
        jmp    NEAR PTR loc_1_20154
loc_1_1FEF6:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 18h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 30h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1F560
        add    esp, 10h
        jmp    NEAR PTR loc_1_20154
loc_1_1FF5A:
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1FF67:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        inc    dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_1FFC2
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 2ch]
        inc    word ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        cmp    eax, 52h
        jne    NEAR PTR loc_1_1FFBD
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_1FFBD:
        jmp    NEAR PTR loc_1_1FF67
loc_1_1FFC2:
        jmp    NEAR PTR loc_1_20154
loc_1_1FFC7:
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax - 4]
        mov    word ptr [ebp - 24h], ax
        add    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1FFE3:
        mov    ax, word ptr [ebp - 24h]
        mov    word ptr [ebp - 3ch], ax
        dec    word ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 3ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_20047
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 2ch]
        inc    word ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        cmp    eax, 52h
        jne    NEAR PTR loc_1_20042
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_20042:
        jmp    NEAR PTR loc_1_1FFE3
loc_1_20047:
        jmp    NEAR PTR loc_1_20154
loc_1_2004C:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 2ch]
        inc    word ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        cmp    eax, 52h
        jne    NEAR PTR loc_1_2008A
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_2008A:
        jmp    NEAR PTR loc_1_20154
loc_1_2008F:
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_20154
        db  0E9h, 0AEh, 000h, 000h, 000h
loc_1_200A6:
        sub    dword ptr [ebp - 40h], 21h
        cmp    dword ptr [ebp - 40h], 56h
        ja     NEAR PTR loc_1_20154
        mov    eax, dword ptr [ebp - 40h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Ch, 00Ch, 00Ch, 001h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch
        db  00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch
        db  00Ch, 00Ch, 00Ch, 00Ch, 002h, 003h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 004h
        db  00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 005h, 00Ch, 00Ch, 00Ch, 006h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch
        db  00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 007h, 00Ch, 008h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 009h
        db  00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ah, 00Ch, 00Ch, 00Ch, 00Bh
loc_1_20154:
        jmp    NEAR PTR loc_1_20197
loc_1_20159:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 2ch]
        inc    word ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        cmp    eax, 52h
        jne    NEAR PTR loc_1_20197
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
loc_1_20197:
        jmp    NEAR PTR loc_1_1FC0C
loc_1_2019C:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_20AD7
        add    esp, 8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_1_201DC
        xor    al, al
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        cmp    ecx, 51h
        jb     NEAR PTR loc_1_201D6
        mov    ecx, 51h
loc_1_201D6:
        db  088h, 081h, 000h, 000h, 000h, 000h
loc_1_201DC:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_201EA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_201F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_2024A
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_10592
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_20245
        push   1
        push   48h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_10592
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
loc_1_20245:
        jmp    NEAR PTR loc_1_20251
loc_1_2024A:
        mov    dword ptr [ebp - 4], 0
loc_1_20251:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_20259
loc_1_20259:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2025E:
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
        je     NEAR PTR loc_1_20290
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_20329
loc_1_20290:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_1_202C0
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_00F1
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_20329
loc_1_202C0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_20322
        call   sub_1_01B2
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_20322
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2090F
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_00E5
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_20315
        jmp    NEAR PTR loc_1_2031D
loc_1_20315:
        call   sub_1_201F0
        mov    dword ptr [ebp - 4], eax
loc_1_2031D:
        jmp    NEAR PTR loc_1_20329
loc_1_20322:
        mov    dword ptr [ebp - 4], 0
loc_1_20329:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2033F
loc_1_2033F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_20344:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 2
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_20392
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_2090A
loc_1_20392:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_203CE
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 57h
        jmp    NEAR PTR loc_1_2090A
loc_1_203CE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_208C3
loc_1_203E1:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0fffdh
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_20902
loc_1_20407:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        je     NEAR PTR loc_1_20476
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_2090F
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        cmp    eax, 1
        jg     NEAR PTR loc_1_20465
        mov    eax, 1
loc_1_20465:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_20509
loc_1_20476:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_2049F
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_20D43
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20509
loc_1_2049F:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   15h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_00FB
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_2090F
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        cmp    eax, 1
        jg     NEAR PTR loc_1_204FD
        mov    eax, 1
loc_1_204FD:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 4], 0
loc_1_20509:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        jmp    NEAR PTR loc_1_20902
loc_1_2051C:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0fffeh
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        or     eax, 2
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 14h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 16h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        call   sub_1_2025E
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20902
loc_1_205B8:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 16h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 14h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 16h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        call   sub_1_2025E
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20902
loc_1_2062D:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 12h]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_2067A
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 12h]
        and    eax, 0ffffh
        test   ah, 40h
        je     NEAR PTR loc_1_2067A
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 12h]
        and    eax, 3fffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], ax
loc_1_2067A:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 12h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 14h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 16h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        call   sub_1_2025E
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20902
loc_1_206DF:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 14h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 14h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 16h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        call   sub_1_2025E
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20902
loc_1_20754:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 14h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 16h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   10h
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 18h
        push   eax
        call   sub_1_FA7C
        add    esp, 0ch
        push   10h
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 28h
        push   eax
        call   sub_1_FA7C
        add    esp, 0ch
        push   10h
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 38h
        push   eax
        call   sub_1_FA7C
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        call   sub_1_2025E
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20902
loc_1_207FE:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        or     eax, 2
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 14h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 16h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        call   sub_1_2025E
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_20902
loc_1_20877:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 1
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_20902
loc_1_2088C:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 57h
        jmp    NEAR PTR loc_1_20902
loc_1_208C3:
        sub    dword ptr [ebp - 14h], 0f0h
        cmp    dword ptr [ebp - 14h], 8
        ja     NEAR PTR loc_1_2088C
        mov    eax, dword ptr [ebp - 14h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_20902:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2090A
loc_1_2090A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2090F:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 10h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 12h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 14h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 16h], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    eax, 0fffeh
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jg     NEAR PTR loc_1_209A8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_209C6
loc_1_209A8:
        mov    eax, 2710h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 8], ax
loc_1_209C6:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_209E5
        mov    eax, ecx
loc_1_209E5:
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_20A13
        mov    eax, ecx
loc_1_20A13:
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_20A50
        mov    ax, word ptr [ebp - 8]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_20A69
loc_1_20A50:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        db  066h, 0A3h, 000h, 000h, 000h, 000h
loc_1_20A69:
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 10h], ax
        dec    word ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_20A9A
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_20A69
loc_1_20A9A:
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
loc_1_20AA1:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 14h], ax
        dec    word ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_20AD2
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_20AA1
loc_1_20AD2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_20AD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        cmp    eax, 270fh
        jl     NEAR PTR loc_1_20B08
        mov    eax, 270fh
loc_1_20B08:
        mov    word ptr [ebp + 0ch], ax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, 2710h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    eax, ecx
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_20B44
        mov    eax, ecx
loc_1_20B44:
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 10h], ax
        mov    dword ptr [ebp - 8], 0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jle    NEAR PTR loc_1_20BC0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jle    NEAR PTR loc_1_20BBB
        mov    dword ptr [ebp - 8], 1
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        or     eax, 1
        db  066h, 0A3h, 000h, 000h, 000h, 000h
loc_1_20BBB:
        jmp    NEAR PTR loc_1_20C1F
loc_1_20BC0:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_20C07
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_1_20C1F
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        cmp    eax, 2710h
        jne    NEAR PTR loc_1_20C1F
loc_1_20C07:
        mov    dword ptr [ebp - 8], 1
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        or     eax, 1
        db  066h, 0A3h, 000h, 000h, 000h, 000h
loc_1_20C1F:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_20C39
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 1
        je     NEAR PTR loc_1_20D2B
loc_1_20C39:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_20C58
        mov    ax, word ptr [ebp - 10h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_20C71
loc_1_20C58:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        db  066h, 0A3h, 000h, 000h, 000h, 000h
loc_1_20C71:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 2710h
        jne    NEAR PTR loc_1_20C8D
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_20C8D:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_20CC2
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        inc    eax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 2710h
        jne    NEAR PTR loc_1_20CC2
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_20CC2:
        mov    ax, word ptr [ebp - 0ch]
        mov    word ptr [ebp - 14h], ax
        dec    word ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_20CF3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 8]
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_20CC2
loc_1_20CF3:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
loc_1_20CFA:
        mov    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 18h], ax
        dec    word ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_20D2B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 8]
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_20CFA
loc_1_20D2B:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        call   sub_1_2025E
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_20D43:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_20D9F
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_10592
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_20D9F
        push   1
        push   48h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_10592
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
loc_1_20D9F:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_20DBF
        db  068h, 000h, 000h, 000h, 000h
        push   48h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_FF3F
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
loc_1_20DBF:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_20DEA
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_FF3F
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
loc_1_20DEA:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_20E06
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
loc_1_20E06:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_20E0E
loc_1_20E0E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_20E14:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        push   1
        push   64h
        call   sub_1_002E
        add    esp, 8
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_1_20E9E
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_28F22
        mov    eax, 0fffffffeh
        jmp    NEAR PTR loc_1_21511
loc_1_20E9E:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 14h
        mov    dword ptr [ebp - 3ch], eax
        mov    edi, dword ptr [ebp - 3ch]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, 7
        rep movsd
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax + 18h], 0
        je     NEAR PTR loc_1_20F66
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 40h
        mov    dword ptr [ebp - 10h], eax
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_20EEC
loc_1_20EE8:
        inc    word ptr [ebp - 20h]
loc_1_20EEC:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 16h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_20F2E
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ecx + 18h]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_20EE8
loc_1_20F2E:
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 16h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx + 0ah]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 34h]
        mov    word ptr [ecx + 38h], ax
        jmp    NEAR PTR loc_1_2141E
loc_1_20F66:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 40h], eax
        jmp    NEAR PTR loc_1_2131A
loc_1_20F74:
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 40h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2102F
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 5], al
        jmp    NEAR PTR loc_1_2104D
loc_1_2102F:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
loc_1_2104D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
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
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_210E5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    word ptr [eax + 0ch], 0d580h
        mov    eax, dword ptr [ebp - 24h]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 24h]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 11h
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        add    eax, 11h
        mov    ecx, dword ptr [ebp - 34h]
        mov    word ptr [ecx + 38h], ax
        jmp    NEAR PTR loc_1_21110
loc_1_210E5:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0eh
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp - 34h]
        mov    word ptr [ecx + 38h], ax
loc_1_21110:
        jmp    NEAR PTR loc_1_2133D
loc_1_21115:
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 40h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 18h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
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
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax + 6], 0
        je     NEAR PTR loc_1_2119B
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0eh
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_211BD
loc_1_2119B:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0eh
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp - 34h]
        mov    word ptr [ecx + 38h], ax
loc_1_211BD:
        jmp    NEAR PTR loc_1_2133D
loc_1_211C2:
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 40h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2128E
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 6], al
        jmp    NEAR PTR loc_1_212AD
loc_1_2128E:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 3], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 2]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 30h]
        mov    word ptr [ecx + 1], ax
loc_1_212AD:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 9], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 30h]
        mov    word ptr [ecx + 7], ax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 0dh
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        add    eax, 0dh
        mov    ecx, dword ptr [ebp - 34h]
        mov    word ptr [ecx + 38h], ax
        jmp    NEAR PTR loc_1_2133D
loc_1_212EE:
        push   24h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   12ch
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_21511
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_2131A:
        cmp    dword ptr [ebp - 40h], 0
        je     NEAR PTR loc_1_20F74
        cmp    dword ptr [ebp - 40h], 1
        je     NEAR PTR loc_1_21115
        cmp    dword ptr [ebp - 40h], 2
        je     NEAR PTR loc_1_211C2
        jmp    NEAR PTR loc_1_212EE
loc_1_2133D:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax + 6], 0
        je     NEAR PTR loc_1_2141E
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 6]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 28h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 6]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ecx + 6]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        mov    word ptr [ebp - 1ch], ax
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_2139C
loc_1_21398:
        inc    word ptr [ebp - 20h]
loc_1_2139C:
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        sub    eax, 2
        cdq
        sub    eax, edx
        sar    eax, 1
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_213EC
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ecx + 6]
        mov    ax, word ptr [ecx + eax*2 + 2]
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 28h]
        mov    word ptr [edx + ecx*2 + 2], ax
        jmp    NEAR PTR loc_1_21398
loc_1_213EC:
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 1ch]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, 0eh
        mov    ecx, dword ptr [ebp - 34h]
        mov    word ptr [ecx + 38h], ax
loc_1_2141E:
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 0ch
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [ebp - 10h], eax
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_2143F
loc_1_2143B:
        inc    word ptr [ebp - 20h]
loc_1_2143F:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_2147E
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_2143B
loc_1_2147E:
        mov    eax, dword ptr [ebp - 34h]
        mov    ax, word ptr [eax + 38h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 40h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 2ch], eax
        cmp    dword ptr [ebp - 2ch], 0
        jne    NEAR PTR loc_1_214EB
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_0034
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_28F22
        mov    eax, 0fffffffeh
        jmp    NEAR PTR loc_1_21511
loc_1_214EB:
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_28F5E
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_21511
loc_1_21511:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_21516:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
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
        mov    eax, dword ptr [ebp - 0ch]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_21581
        mov    eax, dword ptr [ebp - 0ch]
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
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_2161F
loc_1_21581:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_215BE
        mov    eax, dword ptr [ebp - 0ch]
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
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_2161F
loc_1_215BE:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_215FC
        mov    eax, dword ptr [ebp - 0ch]
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
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_2161F
loc_1_215FC:
        mov    eax, dword ptr [ebp - 0ch]
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
        mov    dword ptr [ebp - 18h], eax
loc_1_2161F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_2169B
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
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_2169B
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22108
        add    esp, 10h
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_2169B
        jmp    NEAR PTR loc_1_217D9
loc_1_2169B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 40h
        push   eax
        call   sub_1_19975
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_217D9
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_22F01
        add    esp, 4
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_216E2
        mov    eax, dword ptr [ebp - 10h]
        inc    dword ptr [eax + 78h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 2], 0cch
        mov    byte ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_217C3
loc_1_216E2:
        cmp    dword ptr [ebp + 10h], 0c001000ch
        je     NEAR PTR loc_1_216FC
        cmp    dword ptr [ebp + 10h], 0c001000dh
        jne    NEAR PTR loc_1_21762
loc_1_216FC:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
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
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 2], 0
        mov    byte ptr [ebp - 8], 0bh
        jmp    NEAR PTR loc_1_217C3
loc_1_21762:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
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
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 2], 0
        mov    byte ptr [ebp - 8], 7
loc_1_217C3:
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
loc_1_217D9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_217DE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 14h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_2181B
        jmp    NEAR PTR loc_1_21A16
loc_1_2181B:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_2187F
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_2187F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_2186C
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_2187A
loc_1_2186C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 8], eax
loc_1_2187A:
        jmp    NEAR PTR loc_1_21A16
loc_1_2187F:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_21932
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_21932
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 14h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 10h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 14h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 10h]
        mov    word ptr [ecx + 0ah], ax
        jmp    NEAR PTR loc_1_21A16
loc_1_21932:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_1_21992
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_21992
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0eh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 14h], 0
        jmp    NEAR PTR loc_1_21A16
loc_1_21992:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_1_219F9
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_219F9
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 14h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        add    dword ptr [ecx + 10h], eax
        jmp    NEAR PTR loc_1_21A16
loc_1_219F9:
        push   29h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   261h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_21A16:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_21A1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 0bh
        jne    NEAR PTR loc_1_21B8B
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_21A76
        mov    eax, dword ptr [ebp - 4]
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
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_21B14
loc_1_21A76:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_21AB3
        mov    eax, dword ptr [ebp - 4]
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
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_21B14
loc_1_21AB3:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_21AF1
        mov    eax, dword ptr [ebp - 4]
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
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_21B14
loc_1_21AF1:
        mov    eax, dword ptr [ebp - 4]
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
        mov    dword ptr [ebp - 14h], eax
loc_1_21B14:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 0b3h]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 16h], ax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0b4h
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 0ah], 4
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 90h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 0fh], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 14h], 0
        jmp    NEAR PTR loc_1_21EFF
loc_1_21B8B:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
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
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_1_21BEB
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_21C19
loc_1_21BEB:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        add    eax, dword ptr [ecx + 10h]
        add    eax, 0ch
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 18h], ax
        jmp    NEAR PTR loc_1_21C6B
loc_1_21C19:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_21C42
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_21C6B
loc_1_21C42:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_21F06
loc_1_21C6B:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        cmp    eax, 0eh
        jae    NEAR PTR loc_1_21CA5
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_21F06
loc_1_21CA5:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        add    eax, 10h
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_21DCA
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        cmp    eax, 0eh
        jle    NEAR PTR loc_1_21DCA
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jae    NEAR PTR loc_1_21D1C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_21F06
loc_1_21D1C:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 80h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 6]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 6]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_21DC5
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_21F06
loc_1_21DC5:
        jmp    NEAR PTR loc_1_21DDD
loc_1_21DCA:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 14h], 0eh
loc_1_21DDD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 0ah
        je     NEAR PTR loc_1_21EEA
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_21E0A
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 1], 40h
        jmp    NEAR PTR loc_1_21E1E
loc_1_21E0A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 2
        jne    NEAR PTR loc_1_21E1E
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 1], 10h
loc_1_21E1E:
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 0ah], 3
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_21EB9
loc_1_21E52:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 0eh], 0f3h
        jmp    NEAR PTR loc_1_21EE5
loc_1_21E5E:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 0eh], 3
        jmp    NEAR PTR loc_1_21EE5
loc_1_21E6A:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 0eh], 0bfh
        jmp    NEAR PTR loc_1_21EE5
loc_1_21E76:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 0eh], 0bfh
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 0dh], al
        jmp    NEAR PTR loc_1_21EE5
loc_1_21E95:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 0eh], 0afh
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 0dh], al
        jmp    NEAR PTR loc_1_21EE5
        db  0E9h, 02Ch, 000h, 000h, 000h
loc_1_21EB9:
        sub    dword ptr [ebp - 1ch], 0dh
        cmp    dword ptr [ebp - 1ch], 4
        ja     NEAR PTR loc_1_21EE5
        mov    eax, dword ptr [ebp - 1ch]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_21EE5:
        jmp    NEAR PTR loc_1_21EFF
loc_1_21EEA:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 0ah], 0
loc_1_21EFF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_21F06
loc_1_21F06:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_21F0B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 0c001000ch
        je     NEAR PTR loc_1_21F2E
        cmp    dword ptr [ebp + 10h], 0c001000dh
        jne    NEAR PTR loc_1_21F8C
loc_1_21F2E:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
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
        jmp    NEAR PTR loc_1_21FEF
loc_1_21F8C:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_21FEF
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
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
loc_1_21FEF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_2206D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 7
        jne    NEAR PTR loc_1_2206D
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_2206D
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_2206D
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22108
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_2206D
        jmp    NEAR PTR loc_1_22103
loc_1_2206D:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0ch
        push   eax
        call   sub_1_D1F8
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 6
        jne    NEAR PTR loc_1_220CE
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        push   1
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        jmp    NEAR PTR loc_1_22103
loc_1_220CE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_220F7
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_22103
loc_1_220F7:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_22103:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_22108:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_28F28
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_28F4C
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        cmp    eax, 0d580h
        jne    NEAR PTR loc_1_2229D
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 11h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0efh
        mov    byte ptr [ebp - 20h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 0e3h
        je     NEAR PTR loc_1_22198
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 0afh
        je     NEAR PTR loc_1_22198
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 6fh
        je     NEAR PTR loc_1_22198
        xor    eax, eax
        jmp    NEAR PTR loc_1_223F9
loc_1_22198:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_22283
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 0eh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 11h
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0eh
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 24h]
        sub    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        sub    dword ptr [eax + 4], 3
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 18h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23FDC
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 103h
        je     NEAR PTR loc_1_22274
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 24h]
        add    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [eax + 4], 3
        xor    eax, eax
        jmp    NEAR PTR loc_1_223F9
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_22274:
        mov    eax, 1
        jmp    NEAR PTR loc_1_223F9
        db  0E9h, 015h, 000h, 000h, 000h
loc_1_22283:
        mov    eax, dword ptr [ebp - 24h]
        add    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [eax + 4], 3
        xor    eax, eax
        jmp    NEAR PTR loc_1_223F9
        db  0E9h, 05Ch, 001h, 000h, 000h
loc_1_2229D:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0eh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0efh
        mov    byte ptr [ebp - 20h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 0e3h
        je     NEAR PTR loc_1_222EF
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 0afh
        je     NEAR PTR loc_1_222EF
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, 6fh
        je     NEAR PTR loc_1_222EF
        xor    eax, eax
        jmp    NEAR PTR loc_1_223F9
loc_1_222EF:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_223E4
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0eh
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 11h
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [eax + 0ch], 0d580h
        mov    eax, dword ptr [ebp - 24h]
        add    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [eax + 4], 3
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 18h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23FDC
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 103h
        je     NEAR PTR loc_1_223D5
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 24h]
        sub    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        sub    dword ptr [eax + 4], 3
        xor    eax, eax
        jmp    NEAR PTR loc_1_223F9
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_223D5:
        mov    eax, 1
        jmp    NEAR PTR loc_1_223F9
        db  0E9h, 015h, 000h, 000h, 000h
loc_1_223E4:
        mov    eax, dword ptr [ebp - 24h]
        sub    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        sub    dword ptr [eax + 4], 3
        xor    eax, eax
        jmp    NEAR PTR loc_1_223F9
loc_1_223F9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_223FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 8], 0ch
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        mov    word ptr [ecx + eax*8 + 5eh], 58h
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [ecx + eax*8 + 5eh]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        add    eax, ecx
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        sub    eax, ecx
        add    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    byte ptr [ebp - 1ch], 0
        jmp    NEAR PTR loc_1_22494
loc_1_22491:
        inc    byte ptr [ebp - 1ch]
loc_1_22494:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1ch]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_224C9
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 1]
        mov    al, byte ptr [eax + ecx]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_22491
loc_1_224C9:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0bh]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0dh]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        movsx  eax, word ptr [ecx + eax*8 + 5eh]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    cx, word ptr [ecx + 0ah]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [edx + 14h]
        mov    word ptr [edx + ecx*8 + 5eh], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DCCE
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_225AD
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   1
        push   1
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2274B
loc_1_225AD:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_225C0
loc_1_225B8:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
loc_1_225C0:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_2267E
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [ecx + eax*8 + 5eh]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 14h]
        add    eax, 0ch
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_2266C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_D1F8
        add    esp, 4
        push   1
        push   1
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2274B
loc_1_2266C:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_28F5E
        jmp    NEAR PTR loc_1_225B8
loc_1_2267E:
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0ch
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    byte ptr [eax + 4], 6
        mov    eax, dword ptr [ebp - 28h]
        mov    byte ptr [eax + 5], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 28h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 28h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 28h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 30h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23FDC
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 103h
        je     NEAR PTR loc_1_2271E
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 30h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21F0B
        add    esp, 0ch
loc_1_2271E:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_22735
        cmp    dword ptr [ebp - 0ch], 103h
        jne    NEAR PTR loc_1_22741
loc_1_22735:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2274B
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_22741:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2274B
loc_1_2274B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_22750:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0
        jne    NEAR PTR loc_1_227BA
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19506
        add    esp, 4
        mov    dword ptr [ebp + 18h], eax
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_2278D
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 18h
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_227BA
loc_1_2278D:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_22BF9
loc_1_227BA:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 1], 0
        je     NEAR PTR loc_1_22811
        mov    byte ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_227DC
loc_1_227D9:
        inc    byte ptr [ebp - 10h]
loc_1_227DC:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_22811
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 1]
        mov    al, byte ptr [eax + ecx]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_227D9
loc_1_22811:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 6], 0
        je     NEAR PTR loc_1_2287B
        mov    byte ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_22845
loc_1_22842:
        inc    byte ptr [ebp - 10h]
loc_1_22845:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_2287B
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 6]
        mov    al, byte ptr [eax + ecx]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_22842
loc_1_2287B:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_22965
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        add    dword ptr [ebp - 8], eax
        mov    eax, 28h
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    cl, byte ptr [ecx + 0ah]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        cmp    eax, dword ptr [ebp - 20h]
        jge    NEAR PTR loc_1_22901
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        mov    dword ptr [ebp - 20h], eax
loc_1_22901:
        mov    byte ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_2290D
loc_1_2290A:
        inc    byte ptr [ebp - 10h]
loc_1_2290D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 20h]
        jge    NEAR PTR loc_1_22939
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 10h]
        mov    al, byte ptr [eax + ecx]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_2290A
loc_1_22939:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    cl, byte ptr [ecx + 0ah]
        and    ecx, 0ffh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 18h], ax
loc_1_22965:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_229C1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0bh]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0dh]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_229C1
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0eh]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 3], al
loc_1_229C1:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DCCE
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_22A52
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 18h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_22A3C
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_22A48
loc_1_22A3C:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_22A48:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_22BF9
loc_1_22A52:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_22AE5
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_D1F8
        add    esp, 4
        mov    eax, dword ptr [ebp + 18h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_22ACF
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_22ADB
loc_1_22ACF:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_22ADB:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_22BF9
loc_1_22AE5:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28F5E
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 4], 7
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 5], 2
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 18h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp + 10h], eax
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_22B49
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 18h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_22B5C
loc_1_22B49:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_195C4
        add    esp, 8
loc_1_22B5C:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23FDC
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 103h
        je     NEAR PTR loc_1_22BCC
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_22BA2
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        jmp    NEAR PTR loc_1_22BB5
loc_1_22BA2:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_1962D
        add    esp, 8
loc_1_22BB5:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21F0B
        add    esp, 0ch
loc_1_22BCC:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_22BE3
        cmp    dword ptr [ebp - 0ch], 103h
        jne    NEAR PTR loc_1_22BEF
loc_1_22BE3:
        xor    eax, eax
        jmp    NEAR PTR loc_1_22BF9
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_22BEF:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_22BF9
loc_1_22BF9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_22BFE:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_22C35
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_22C35:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1845A
        add    esp, 8
        push   7530h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 18h
        push   eax
        call   sub_1_EACD
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_22D2E
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 18h
        push   eax
        call   sub_1_19726
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_22CAE
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_22CAE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 4
        je     NEAR PTR loc_1_22CCA
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 5
        jne    NEAR PTR loc_1_22CE2
loc_1_22CCA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22DAB
        add    esp, 8
        jmp    NEAR PTR loc_1_22D11
loc_1_22CE2:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 6
        je     NEAR PTR loc_1_22CFE
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 7
        jne    NEAR PTR loc_1_22D11
loc_1_22CFE:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22DAB
        add    esp, 8
loc_1_22D11:
        push   2eh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   6a3h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_22D2E:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        call   sub_1_19A9D
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_22DA6
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_22D6E
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 40h], ecx
        jne    NEAR PTR loc_1_22D81
loc_1_22D6E:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
loc_1_22D81:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_22F01
        add    esp, 4
        push   27h
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        jmp    NEAR PTR loc_1_22D2E
loc_1_22DA6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_22DAB:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 1
        jne    NEAR PTR loc_1_22DCA
        jmp    NEAR PTR loc_1_22EFC
loc_1_22DCA:
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_28F28
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_22E0D
        push   2
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   6e1h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_22EFC
loc_1_22E0D:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 1
        je     NEAR PTR loc_1_22E45
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_22E45:
        mov    word ptr [ebp - 14h], 1
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_22E71
loc_1_22E56:
        inc    word ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
loc_1_22E71:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_22EFC
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jg     NEAR PTR loc_1_22EFC
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_28F4C
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 1
        je     NEAR PTR loc_1_22EEA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 1ch
loc_1_22EEA:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_28F70
        jmp    NEAR PTR loc_1_22E56
loc_1_22EFC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_22F01:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 24h], eax
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_1_22F22
        jmp    NEAR PTR loc_1_23161
loc_1_22F22:
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 14h
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F28
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_22FA7
        push   2
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   740h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_23161
loc_1_22FA7:
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F64
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_28F46
        dec    dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 2ch]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_23024
        mov    eax, dword ptr [ebp - 2ch]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_23024
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F6A
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_28F4C
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_28F46
        push   0
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_10592
        add    esp, 0ch
        dec    dword ptr [ebp - 18h]
loc_1_23024:
        mov    eax, dword ptr [ebp - 2ch]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_23074
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F6A
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_28F4C
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_28F46
        push   0
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_10592
        add    esp, 0ch
        dec    dword ptr [ebp - 18h]
loc_1_23074:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 30h], eax
        dec    dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_1_23140
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F6A
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_28F4C
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_28F46
        mov    eax, dword ptr [ebp - 24h]
        cmp    dword ptr [eax + 34h], 0
        je     NEAR PTR loc_1_23129
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 34h], ecx
        jne    NEAR PTR loc_1_23124
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 34h], 0
        push   1
        push   0
        mov    eax, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_2311A
        push   25h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   79eh
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_23161
loc_1_2311A:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 8], 0
loc_1_23124:
        jmp    NEAR PTR loc_1_2313B
loc_1_23129:
        push   0
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_10592
        add    esp, 0ch
loc_1_2313B:
        jmp    NEAR PTR loc_1_23074
loc_1_23140:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F22
        push   0
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_0034
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
loc_1_23161:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_23166:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 1ch], 0
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_231A0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        dec    eax
        db  00Fh, 0AFh, 005h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 14h], eax
loc_1_231A0:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_231B3
loc_1_231AB:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_231B3:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_23436
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_232A0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 4], ecx
        ja     NEAR PTR loc_1_231F5
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 10h], eax
        ja     NEAR PTR loc_1_231F5
        mov    eax, dword ptr [ebp - 10h]
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     NEAR PTR loc_1_23224
loc_1_231F5:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], 0ffffffffh
        jmp    NEAR PTR loc_1_23436
loc_1_23224:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ecx + 4]
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        shl    eax, 3
        add    eax, dword ptr [ebp + 0ch]
        add    eax, 5ah
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffff00h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_2326C
        mov    eax, dword ptr [ebp - 28h]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_2329B
loc_1_2326C:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], 0ffffffffh
        jmp    NEAR PTR loc_1_23436
loc_1_2329B:
        jmp    NEAR PTR loc_1_23362
loc_1_232A0:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 18h], eax
        push   0ch
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_232EB
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], 0ffffffffh
        jmp    NEAR PTR loc_1_23436
loc_1_232EB:
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        mov    cx, word ptr [ecx + 0ah]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, 0ch
        mov    word ptr [ebp - 2ch], ax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_FF3F
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_23362
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], 0ffffffffh
        jmp    NEAR PTR loc_1_23436
loc_1_23362:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 0ch]
        add    eax, 0ch
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_233CC
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], 0fffffffeh
        jmp    NEAR PTR loc_1_23436
loc_1_233CC:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_28F5E
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 10h], eax
        jne    NEAR PTR loc_1_23411
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp + 1ch], 0
        je     NEAR PTR loc_1_233FF
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
loc_1_233FF:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 20h]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_1_23431
loc_1_23411:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 20h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 20h]
        mov    word ptr [ecx], ax
loc_1_23431:
        jmp    NEAR PTR loc_1_231AB
loc_1_23436:
        mov    eax, dword ptr [ebp - 1ch]
        jmp    NEAR PTR loc_1_2343E
loc_1_2343E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_23443:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    word ptr [ebp - 18h], 0
        mov    word ptr [ebp - 16h], 0
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21A1B
        add    esp, 0ch
        mov    dword ptr [ebp - 50h], eax
        cmp    dword ptr [ebp - 50h], 0
        je     NEAR PTR loc_1_234B1
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        jmp    NEAR PTR loc_1_23A12
loc_1_234B1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DCCE
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_234F8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 50h], 0fffffffeh
        mov    eax, dword ptr [ebp - 50h]
        jmp    NEAR PTR loc_1_23A12
loc_1_234F8:
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_20E14
        add    esp, 14h
        mov    dword ptr [ebp - 50h], eax
        cmp    dword ptr [ebp - 50h], 0
        je     NEAR PTR loc_1_23548
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        jmp    NEAR PTR loc_1_23A12
loc_1_23548:
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_217DE
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 5ch], eax
        mov    eax, dword ptr [ebp - 5ch]
        add    eax, 14h
        mov    dword ptr [ebp - 64h], eax
        mov    eax, dword ptr [ebp - 64h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 64h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 54h], eax
        mov    eax, dword ptr [ebp - 64h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ebp - 64h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp - 64h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 64h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 64h]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 64h]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 5ch]
        mov    word ptr [eax + 30h], 0
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_1_23641
        lea    eax, [ebp - 2ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 64h]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23166
        add    esp, 1ch
        mov    dword ptr [ebp - 50h], eax
        cmp    dword ptr [ebp - 50h], 0
        jne    NEAR PTR loc_1_23622
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 2ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 5ch]
        mov    word ptr [ecx + 30h], ax
        jmp    NEAR PTR loc_1_23641
loc_1_23622:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_23641:
        cmp    dword ptr [ebp - 50h], 0
        jne    NEAR PTR loc_1_236D1
        cmp    dword ptr [ebp - 54h], 0
        je     NEAR PTR loc_1_236D1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 5ch]
        add    eax, 34h
        push   eax
        mov    eax, dword ptr [ebp - 64h]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 54h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23166
        add    esp, 1ch
        mov    dword ptr [ebp - 50h], eax
        cmp    dword ptr [ebp - 50h], 0
        jne    NEAR PTR loc_1_236B2
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 2ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 5ch]
        mov    word ptr [ecx + 30h], ax
        jmp    NEAR PTR loc_1_236D1
loc_1_236B2:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_236D1:
        cmp    dword ptr [ebp - 50h], 0
        jne    NEAR PTR loc_1_237E5
        mov    ax, word ptr [ebp - 20h]
        mov    word ptr [ebp - 60h], ax
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_237E5
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 64h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_FF3F
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_237BF
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 58h], eax
        cmp    dword ptr [ebp - 58h], 0
        je     NEAR PTR loc_1_2377D
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_28F5E
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 64h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 60h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 5ch]
        mov    word ptr [ecx + 30h], ax
        jmp    NEAR PTR loc_1_237BA
loc_1_2377D:
        push   0
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_10592
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 50h], 0fffffffeh
loc_1_237BA:
        jmp    NEAR PTR loc_1_237E5
loc_1_237BF:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 50h], 0ffffffffh
loc_1_237E5:
        cmp    dword ptr [ebp - 50h], 0
        jne    NEAR PTR loc_1_23909
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_23909
        mov    ax, word ptr [ebp - 24h]
        mov    word ptr [ebp - 60h], ax
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_23909
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 64h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_FF3F
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_238E3
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 58h], eax
        cmp    dword ptr [ebp - 58h], 0
        je     NEAR PTR loc_1_238A1
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_28F5E
        mov    ax, word ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 64h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 60h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 5ch]
        mov    word ptr [ecx + 30h], ax
        jmp    NEAR PTR loc_1_238DE
loc_1_238A1:
        push   0
        mov    eax, dword ptr [ebp - 60h]
        and    eax, 0ffffh
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_10592
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 50h], 0fffffffeh
loc_1_238DE:
        jmp    NEAR PTR loc_1_23909
loc_1_238E3:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 50h], 0ffffffffh
loc_1_23909:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 68h], eax
        cmp    dword ptr [ebp - 68h], 0
        je     NEAR PTR loc_1_2392F
        mov    ax, word ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 68h]
        mov    word ptr [ecx + 6], ax
        mov    ax, word ptr [ebp - 16h]
        mov    ecx, dword ptr [ebp - 68h]
        mov    word ptr [ecx + 0ah], ax
loc_1_2392F:
        cmp    dword ptr [ebp - 50h], 0
        je     NEAR PTR loc_1_23945
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_22F01
        add    esp, 4
loc_1_23945:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_239E2
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 42h]
        add    eax, ecx
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 5ch]
        add    eax, 40h
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        cmp    eax, 0d580h
        jne    NEAR PTR loc_1_239B4
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 28h]
        mov    word ptr [ecx + 0eh], ax
        jmp    NEAR PTR loc_1_239E2
loc_1_239B4:
        mov    eax, dword ptr [ebp - 5ch]
        add    eax, 40h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 0ch], ax
loc_1_239E2:
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 64h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 54h]
        mov    ecx, dword ptr [ebp - 64h]
        mov    dword ptr [ecx + 8], eax
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 64h]
        mov    word ptr [ecx + 0ch], ax
        mov    ax, word ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 64h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 50h]
        jmp    NEAR PTR loc_1_23A12
loc_1_23A12:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_23A17:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F4B8
        add    esp, 8
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_23A60
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_23DB1
loc_1_23A60:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 10h
        push   eax
        push   1ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_24241
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_23AA6
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_23DB1
loc_1_23AA6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 0bh
        jne    NEAR PTR loc_1_23AC5
        mov    word ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_23ACB
loc_1_23AC5:
        mov    word ptr [ebp - 4], 3
loc_1_23ACB:
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F843
        add    esp, 14h
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_23B1A
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_23DB1
loc_1_23B1A:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_23B63
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 18h
loc_1_23B63:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 10h]
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
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_23C29
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23443
        add    esp, 0ch
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], -1
        jne    NEAR PTR loc_1_23C01
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_23DB1
        db  0E9h, 028h, 000h, 000h, 000h
loc_1_23C01:
        cmp    dword ptr [ebp - 1ch], -2
        jne    NEAR PTR loc_1_23C29
        push   65h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_23DB1
loc_1_23C29:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    byte ptr [eax + 5], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 0bh
        jne    NEAR PTR loc_1_23D2F
        mov    eax, dword ptr [ebp - 28h]
        mov    byte ptr [eax + 4], 5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14E84
        add    esp, 0ch
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_23C8A
        xor    eax, eax
        jmp    NEAR PTR loc_1_23DB1
        db  0E9h, 0A5h, 000h, 000h, 000h
loc_1_23C8A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 27h
        jne    NEAR PTR loc_1_23CE2
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_23CBD
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_23CBD:
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_23DB1
        db  0E9h, 04Dh, 000h, 000h, 000h
loc_1_23CE2:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_23DB1
loc_1_23D2F:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp - 28h]
        mov    byte ptr [eax + 4], 4
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 18h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23FDC
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 103h
        je     NEAR PTR loc_1_23DAA
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 18h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21516
        add    esp, 0ch
loc_1_23DAA:
        xor    eax, eax
        jmp    NEAR PTR loc_1_23DB1
loc_1_23DB1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_23DB6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 40h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_23DD5
loc_1_23DCD:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_23DD5:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_23E7C
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_23E30
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_23E2B
        push   20h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0ab1h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_23E2B:
        jmp    NEAR PTR loc_1_23E77
loc_1_23E30:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_23E60
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        or     al, 40h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 4], al
        jmp    NEAR PTR loc_1_23E7C
        db  0E9h, 017h, 000h, 000h, 000h
loc_1_23E60:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21516
        add    esp, 0ch
        jmp    NEAR PTR loc_1_23E7C
loc_1_23E77:
        jmp    NEAR PTR loc_1_23DCD
loc_1_23E7C:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_23EA3
        push   2ch
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0ac8h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_23EA3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_23EA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_23FAF
loc_1_23ED1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 6], 0
        jne    NEAR PTR loc_1_23EF7
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22DAB
        add    esp, 8
loc_1_23EF7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21516
        add    esp, 0ch
        jmp    NEAR PTR loc_1_23FD7
loc_1_23F16:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 6], 0
        jne    NEAR PTR loc_1_23F3C
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22DAB
        add    esp, 8
loc_1_23F3C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_23F69
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21516
        add    esp, 0ch
loc_1_23F69:
        jmp    NEAR PTR loc_1_23FD7
loc_1_23F6E:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 6], 0
        jne    NEAR PTR loc_1_23F8E
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22DAB
        add    esp, 8
loc_1_23F8E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21F0B
        add    esp, 0ch
        jmp    NEAR PTR loc_1_23FD7
        db  0E9h, 028h, 000h, 000h, 000h
loc_1_23FAF:
        sub    dword ptr [ebp - 8], 4
        cmp    dword ptr [ebp - 8], 3
        ja     NEAR PTR loc_1_23FD7
        mov    eax, dword ptr [ebp - 8]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_23FD7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_23FDC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28EA4
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_24079
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 1
        je     NEAR PTR loc_1_24045
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_24045:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22DAB
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_24106
        db  0E9h, 08Dh, 000h, 000h, 000h
loc_1_24079:
        cmp    dword ptr [ebp - 4], 103h
        jne    NEAR PTR loc_1_240BF
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 1
        je     NEAR PTR loc_1_240B0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_240B0:
        mov    eax, 103h
        jmp    NEAR PTR loc_1_24106
        db  0E9h, 047h, 000h, 000h, 000h
loc_1_240BF:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 4], al
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_24106
loc_1_24106:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_2410C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   18h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FF3F
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_2415C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2423C
loc_1_2415C:
        call   sub_1_19428
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_2419D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 7
        push   6
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   69h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_2423C
loc_1_2419D:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    byte ptr [ecx + 15h], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    byte ptr [ecx + 16h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_24214
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        shl    eax, 10h
        and    eax, 0ffff0000h
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 16h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3], 0
        jmp    NEAR PTR loc_1_24220
loc_1_24214:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0ch], 0
loc_1_24220:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        not    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 16h], ax
        xor    eax, eax
        jmp    NEAR PTR loc_1_2423C
loc_1_2423C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_24241:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FF3F
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_24284
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_242AE
        db  0E9h, 02Ah, 000h, 000h, 000h
loc_1_24284:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_242AE
loc_1_242AE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_242B3:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 16h]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 14h]
        not    eax
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_24370
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   8
        push   18h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   10h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0f8h
        call   sub_1_FAD7
        add    esp, 0ch
        xor    eax, eax
        jmp    NEAR PTR loc_1_24414
loc_1_24370:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   18h
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_10592
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_24398
        mov    dword ptr [ebp - 10h], 0
loc_1_24398:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_243DA
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_10592
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_243DA
        mov    dword ptr [ebp - 10h], 0
loc_1_243DA:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_24400
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
loc_1_24400:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1923E
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_24414
loc_1_24414:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_24419:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2443F
loc_1_24439:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
loc_1_2443F:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_24497
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_2446C
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_24473
loc_1_2446C:
        mov    dword ptr [ebp - 4], 0
loc_1_24473:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_24492
        xor    eax, eax
        jmp    NEAR PTR loc_1_2449F
loc_1_24492:
        jmp    NEAR PTR loc_1_24439
loc_1_24497:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_2449F
loc_1_2449F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_244A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 80h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 7ch], 0b0h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 76h], eax
        lea    eax, [ebp - 80h]
        mov    dword ptr [ebp - 72h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 7ah], eax
        push   30h
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_051D
        add    esp, 8
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 7ch]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_36F6
        add    esp, 8
        jmp    NEAR PTR loc_1_244F5
loc_1_244F5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_244FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_24512
loc_1_2450F:
        inc    dword ptr [ebp - 4]
loc_1_24512:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 4], eax
        jge    NEAR PTR loc_1_24573
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + eax*2 + 8], edx
        jne    NEAR PTR loc_1_2456E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*2 + 1eh]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_2456E
        jmp    NEAR PTR loc_1_24573
loc_1_2456E:
        jmp    NEAR PTR loc_1_2450F
loc_1_24573:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_1_24588
        xor    eax, eax
        jmp    NEAR PTR loc_1_245A2
loc_1_24588:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_245A2
loc_1_245A2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
