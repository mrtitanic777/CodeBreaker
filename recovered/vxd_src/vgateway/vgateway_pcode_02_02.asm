; vgateway_pcode_02_02.asm -- VGATEWAY object 2 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_4AE7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 210h
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_4B13
        jmp    NEAR PTR loc_2_4B5F
loc_2_4B13:
        jmp    NEAR PTR loc_2_4B18
loc_2_4B18:
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 0B8h, 048h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 20ch]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_85AB
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_4B5F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4B64:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 210h]
        mov    dword ptr [ebp - 4], eax
loc_2_4B79:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 210h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_4BF6
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 36h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_2_4BE9
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_4BE9
        jmp    NEAR PTR loc_2_4BBA
loc_2_4BBA:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_4BF1
loc_2_4BE9:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_2_4BF1:
        jmp    NEAR PTR loc_2_4B79
loc_2_4BF6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4BFB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_2_4C04:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 210h
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_4C3C
        jmp    NEAR PTR loc_2_4C27
loc_2_4C27:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_4C04
loc_2_4C3C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4C41:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A42
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_4C73
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4BFB
        add    esp, 4
loc_2_4C73:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4C78:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_4CD2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_4CD2
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 6]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_4CD2
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
loc_2_4CD2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4CD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 190h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_4D5D
loc_2_4D06:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 190h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_2_4D58
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 0d8h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_2_4D4B
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_4D58
loc_2_4D4B:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_4D06
loc_2_4D58:
        jmp    NEAR PTR loc_2_4DD1
loc_2_4D5D:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 190h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_2_4DD1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 0dah]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_2_4DC4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ecx + 0dbh]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_2_4DC4
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_4DD1
loc_2_4DC4:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_4D5D
loc_2_4DD1:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_4DE3
        sub    ax, ax
        jmp    NEAR PTR loc_2_4E41
loc_2_4DE3:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_2_4E1E
loc_2_4DFE:
        mov    ax, 1
        jmp    NEAR PTR loc_2_4E41
loc_2_4E07:
        mov    ax, 2
        jmp    NEAR PTR loc_2_4E41
loc_2_4E10:
        mov    ax, 3
        jmp    NEAR PTR loc_2_4E41
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_4E1E:
        cmp    dword ptr [ebp - 10h], 1
        jl     NEAR PTR loc_2_4E10
        cmp    dword ptr [ebp - 10h], 2
        jle    NEAR PTR loc_2_4DFE
        cmp    dword ptr [ebp - 10h], 3
        je     NEAR PTR loc_2_4E07
        jmp    NEAR PTR loc_2_4E10
loc_2_4E41:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4E46:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        cmp    eax, 1
        jne    NEAR PTR loc_2_4E6E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2e0h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_4E75
loc_2_4E6E:
        mov    dword ptr [ebp - 0ch], 0ffffh
loc_2_4E75:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 78h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f4h]
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 2e4h]
        jbe    NEAR PTR loc_2_4EA0
        sub    eax, eax
        jmp    NEAR PTR loc_2_4F2F
loc_2_4EA0:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        db  0E8h, 055h, 0B1h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_4EC2
        sub    eax, eax
        jmp    NEAR PTR loc_2_4F2F
loc_2_4EC2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 1f4h], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        db  068h, 080h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2ch
        push   eax
        db  0E8h, 05Dh, 0B6h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 14h], eax
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 34h
        push   eax
        db  0E8h, 058h, 0B6h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        add    eax, 74h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 3ch], ax
        jmp    NEAR PTR loc_2_4F27
loc_2_4F27:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_4F2F
loc_2_4F2F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4F34:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ecx + 14h]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 1f4h], eax
        jmp    NEAR PTR loc_2_4F55
loc_2_4F55:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 0D6h, 0B0h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_4F96
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1f4h], 0
        jne    NEAR PTR loc_2_4F96
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_4F96:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4F9B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 10h], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 11h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 36h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 36h], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4FD6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        cmp    eax, 40h
        jl     NEAR PTR loc_2_5025
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        cmp    eax, 1
        jne    NEAR PTR loc_2_500E
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_501C
loc_2_500E:
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
loc_2_501C:
        mov    ax, 1
        jmp    NEAR PTR loc_2_5090
loc_2_5025:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_506D
loc_2_5032:
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        mov    ax, 1
        jmp    NEAR PTR loc_2_5090
loc_2_5049:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        mov    ax, 1
        jmp    NEAR PTR loc_2_5090
loc_2_5060:
        sub    ax, ax
        jmp    NEAR PTR loc_2_5090
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_506D:
        cmp    dword ptr [ebp - 4], 36h
        je     NEAR PTR loc_2_5049
        cmp    dword ptr [ebp - 4], 38h
        je     NEAR PTR loc_2_5049
        cmp    dword ptr [ebp - 4], 3ch
        je     NEAR PTR loc_2_5032
        jmp    NEAR PTR loc_2_5060
loc_2_5090:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_5095:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_50C2
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_5132
loc_2_50C2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 8], eax
loc_2_50D7:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_2_5126
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp - 10h]
        add    eax, 28h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_5119
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_5121
loc_2_5119:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_5132
loc_2_5121:
        jmp    NEAR PTR loc_2_50D7
loc_2_5126:
        jmp    NEAR PTR loc_2_512B
loc_2_512B:
        sub    eax, eax
        jmp    NEAR PTR loc_2_5132
loc_2_5132:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_5140:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 180h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 190h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1e4h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1e0h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1f0h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_51AA:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_51B8
loc_2_51B8:
        mov    eax, dword ptr [ebp + 10h]
        movsx  eax, byte ptr [eax]
        or     eax, eax
        jne    NEAR PTR loc_2_51ED
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_2_51E8
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 12h
        mov    dword ptr [ebp - 0ch], eax
loc_2_51E8:
        jmp    NEAR PTR loc_2_51F3
loc_2_51ED:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
loc_2_51F3:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6089
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_5219
        jmp    NEAR PTR loc_2_5455
loc_2_5219:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_2_523B
        jmp    NEAR PTR loc_2_5455
loc_2_523B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_322C
        add    esp, 8
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_5262
        jmp    NEAR PTR loc_2_525D
loc_2_525D:
        jmp    NEAR PTR loc_2_5455
loc_2_5262:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5DE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_5280
        jmp    NEAR PTR loc_2_5455
loc_2_5280:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5EE5
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_52B3
        jmp    NEAR PTR loc_2_529E
loc_2_529E:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5EBF
        add    esp, 8
        jmp    NEAR PTR loc_2_5455
loc_2_52B3:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 190h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ch], 1
        push   10h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1dh
        push   eax
        db  0E8h, 069h, 0B2h, 0FDh, 0FFh
        add    esp, 0ch
        push   10h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2dh
        push   eax
        db  0E8h, 054h, 0B2h, 0FDh, 0FFh
        add    esp, 0ch
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0dh
        push   eax
        db  0E8h, 03Fh, 0B2h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 44h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E13
        add    esp, 8
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_2_5340
loc_2_533C:
        inc    word ptr [ebp - 10h]
loc_2_5340:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_5369
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 48h], 0
        jmp    NEAR PTR loc_2_533C
loc_2_5369:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 58h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0d9h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0d8h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0dch], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0dbh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f4h], 0
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_2_53BC
loc_2_53B8:
        inc    word ptr [ebp - 10h]
loc_2_53BC:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_5449
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6007
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 58h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1ch], 1
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1dh], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 02Ch, 05Ah, 054h, 000h, 000h
        jmp    NEAR PTR loc_2_5423
loc_2_5423:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0dh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1dh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8232
        add    esp, 10h
        jmp    NEAR PTR loc_2_53B8
loc_2_5449:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
loc_2_5455:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_545A:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_2_548F
loc_2_548F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 40h
        jl     NEAR PTR loc_2_54B5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + ecx + 48h], 1
        jmp    NEAR PTR loc_2_5570
loc_2_54B5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_2_5561
loc_2_54C8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0dah], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0dbh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0dch], 3
loc_2_54F0:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 58h
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_2_5548
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1e4h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 14h]
        db  0C7h, 040h, 02Ch, 06Dh, 057h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_34D2
        add    esp, 8
        jmp    NEAR PTR loc_2_5543
loc_2_5543:
        jmp    NEAR PTR loc_2_54F0
loc_2_5548:
        jmp    NEAR PTR loc_2_554D
loc_2_554D:
        jmp    NEAR PTR loc_2_5570
        db  0E9h, 000h, 000h, 000h, 000h, 0E9h, 014h, 000h, 000h, 000h, 0E9h, 00Fh, 000h, 000h, 000h
loc_2_5561:
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_2_54C8
        jmp    NEAR PTR loc_2_5570
loc_2_5570:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6031
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 58h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_55A1
        jmp    NEAR PTR loc_2_5768
loc_2_55A1:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 44h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_55CD
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5768
loc_2_55CD:
        mov    word ptr [ebp - 1ch], 0
        jmp    NEAR PTR loc_2_55DC
loc_2_55D8:
        inc    word ptr [ebp - 1ch]
loc_2_55DC:
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_5635
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx + 48h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_5630
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_5768
loc_2_5630:
        jmp    NEAR PTR loc_2_55D8
loc_2_5635:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0dch]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_2_5661
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5768
loc_2_5661:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0ch], 2
        lea    eax, [ebp - 20h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0dh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_043C
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_2_56D9
        db  0E9h, 060h, 000h, 000h, 000h
loc_2_569C:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5768
loc_2_56B1:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_5768
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_56D9:
        cmp    dword ptr [ebp - 28h], 0
        je     NEAR PTR loc_2_56FC
        cmp    dword ptr [ebp - 28h], 0dh
        jl     NEAR PTR loc_2_56B1
        cmp    dword ptr [ebp - 28h], 0eh
        jle    NEAR PTR loc_2_569C
        jmp    NEAR PTR loc_2_56B1
loc_2_56FC:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 64h
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 020h, 0ACh, 058h, 000h, 000h
        jmp    NEAR PTR loc_2_5724
loc_2_5724:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 2dh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0dh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 10h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0d9h], 2
        db  068h, 080h, 058h, 000h, 000h
        push   1eh
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0ddh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 10h
loc_2_5768:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_576D:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 34h]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 7fh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_2_5846
loc_2_579F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_2_57DC
        db  0E9h, 000h, 000h, 000h, 000h
loc_2_57B1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0ch
        jmp    NEAR PTR loc_2_587B
        db  0E9h, 000h, 000h, 000h, 000h, 0E9h, 014h, 000h, 000h, 000h, 0E9h, 00Fh, 000h, 000h, 000h
loc_2_57DC:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_2_57B1
        jmp    NEAR PTR loc_2_57EB
loc_2_57EB:
        jmp    NEAR PTR loc_2_585F
        db  0E9h, 000h, 000h, 000h, 000h
loc_2_57F5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 21h
        je     NEAR PTR loc_2_5811
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 22h
        jne    NEAR PTR loc_2_583C
loc_2_5811:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 10h
loc_2_583C:
        jmp    NEAR PTR loc_2_585F
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_5846:
        cmp    dword ptr [ebp - 10h], 10h
        je     NEAR PTR loc_2_579F
        cmp    dword ptr [ebp - 10h], 12h
        je     NEAR PTR loc_2_57F5
        jmp    NEAR PTR loc_2_585F
loc_2_585F:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6031
        add    esp, 8
loc_2_587B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_5880:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_588E
loc_2_588E:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_34D2
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_58AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ddh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3E42
        add    esp, 8
        jmp    NEAR PTR loc_2_58D8
loc_2_58D8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        cmp    eax, 40h
        jl     NEAR PTR loc_2_5914
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_5A54
        db  0E9h, 05Ah, 000h, 000h, 000h
loc_2_5914:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_595F
loc_2_5927:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0d8h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0d9h], 3
        jmp    NEAR PTR loc_2_596E
loc_2_5945:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5A54
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_595F:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_5927
        jmp    NEAR PTR loc_2_5945
loc_2_596E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 44h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_599A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5A54
loc_2_599A:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0dch]
        and    eax, 0ffh
        cmp    eax, 5
        jne    NEAR PTR loc_2_59C6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5A54
loc_2_59C6:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3FE8
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0dah]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 44h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4088
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_5A1D
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5A54
loc_2_5A1D:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0f4h], 0
        je     NEAR PTR loc_2_5A54
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f4h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4211
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f4h], 0
loc_2_5A54:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_5A59:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_5AF4
loc_2_5A75:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0dch], 5
        jmp    NEAR PTR loc_2_5B17
loc_2_5A84:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_5ABC
loc_2_5A94:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0dch], 5
        jmp    NEAR PTR loc_2_5AD5
loc_2_5AA3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0d9h], 5
        jmp    NEAR PTR loc_2_5AD5
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_5ABC:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_5AA3
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_2_5A94
        jmp    NEAR PTR loc_2_5AD5
loc_2_5AD5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_5B17
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_5AF4:
        cmp    dword ptr [ebp - 4], 1
        jl     NEAR PTR loc_2_5B17
        cmp    dword ptr [ebp - 4], 2
        jle    NEAR PTR loc_2_5A75
        cmp    dword ptr [ebp - 4], 3
        je     NEAR PTR loc_2_5A84
        jmp    NEAR PTR loc_2_5B17
loc_2_5B17:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_5B1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 64h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0ch], 4
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0f4h], 0
        je     NEAR PTR loc_2_5B6C
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0f4h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4211
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0f4h], 0
loc_2_5B6C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0dch]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_5BCB
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0dah]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 030h, 05Ch, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0dbh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0ch
        jmp    NEAR PTR loc_2_5C2B
loc_2_5BCB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0d9h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_5C1B
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 003h, 05Dh, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0d8h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0ch
        jmp    NEAR PTR loc_2_5C2B
loc_2_5C1B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5D81
        add    esp, 8
loc_2_5C2B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_5C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0ch], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 4], al
        jmp    NEAR PTR loc_2_5C57
loc_2_5C57:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 21h
        je     NEAR PTR loc_2_5C73
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 22h
        jne    NEAR PTR loc_2_5C9E
loc_2_5C73:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 10h
loc_2_5C9E:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0d9h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_5CEE
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 020h, 003h, 05Dh, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0d8h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8417
        add    esp, 0ch
        jmp    NEAR PTR loc_2_5CFE
loc_2_5CEE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5D81
        add    esp, 8
loc_2_5CFE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_5D03:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0ch], 6
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 21h
        je     NEAR PTR loc_2_5D41
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 22h
        jne    NEAR PTR loc_2_5D6C
loc_2_5D41:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 10h
loc_2_5D6C:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5D81
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_5D81:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 44h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 40h], 0
        je     NEAR PTR loc_2_5DB5
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 40h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0575
        add    esp, 0ch
loc_2_5DB5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5EBF
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E25
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_5DE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 18ch], 0
        je     NEAR PTR loc_2_5E30
        push   0f8h
        push   0
        db  0E8h, 0EEh, 0A1h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_5E30
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 18ch]
        jmp    NEAR PTR loc_2_5E37
loc_2_5E30:
        sub    eax, eax
        jmp    NEAR PTR loc_2_5EBA
loc_2_5E37:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 58h
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ddh
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ebh], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 64h
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        db  068h, 08Ch, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 90h
        push   eax
        db  0E8h, 0B0h, 0A6h, 0FDh, 0FFh
        add    esp, 0ch
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 98h
        push   eax
        db  0E8h, 0B2h, 0A6h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_5EBA
loc_2_5EBA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_5EBF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 18ch]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 05Ah, 0A1h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_5EE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_2_5F04
loc_2_5EF9:
        inc    word ptr [ebp - 0ch]
        add    dword ptr [ebp - 4], 80h
loc_2_5F04:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_5FFA
        push   80h
        push   0
        db  0E8h, 0DCh, 0A0h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_5F93
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_5F43
loc_2_5F3F:
        inc    word ptr [ebp - 8]
loc_2_5F43:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_2_5F85
        mov    eax, dword ptr [ebp + 8]
        add    eax, 180h
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6031
        add    esp, 8
        jmp    NEAR PTR loc_2_5F3F
loc_2_5F85:
        mov    ax, 1
        jmp    NEAR PTR loc_2_6002
        db  0E9h, 062h, 000h, 000h, 000h
loc_2_5F93:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        db  068h, 098h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 38h
        push   eax
        db  0E8h, 089h, 0A5h, 0FDh, 0FFh
        add    esp, 0ch
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 40h
        push   eax
        db  0E8h, 08Dh, 0A5h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 180h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 1f0h]
        jmp    NEAR PTR loc_2_5EF9
loc_2_5FFA:
        sub    ax, ax
        jmp    NEAR PTR loc_2_6002
loc_2_6002:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6007:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 180h
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_602C
loc_2_602C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6031:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 0F1h, 09Fh, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 1f0h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_6084
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1f0h], 0
        jne    NEAR PTR loc_2_6084
        push   7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_6084:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6089:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 190h]
        mov    dword ptr [ebp - 4], eax
loc_2_609E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 190h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_611A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 1dh
        push   eax
        db  0E8h, 064h, 0A4h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_2_610D
        push   10h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0dh
        push   eax
        db  0E8h, 047h, 0A4h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_2_610D
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx + 0ch]
        and    ecx, 0ffh
        cmp    ecx, 3
        je     NEAR PTR loc_2_610D
        mov    ax, 1
        jmp    NEAR PTR loc_2_6122
loc_2_610D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_609E
loc_2_611A:
        sub    ax, ax
        jmp    NEAR PTR loc_2_6122
loc_2_6122:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6127:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 4], eax
loc_2_613C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_61A4
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 12h
        push   eax
        db  0E8h, 0CCh, 0A3h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_2_6177
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_619F
loc_2_6177:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_2_6197
        sub    eax, eax
        jmp    NEAR PTR loc_2_61AB
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_6197:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_61AB
loc_2_619F:
        jmp    NEAR PTR loc_2_613C
loc_2_61A4:
        sub    eax, eax
        jmp    NEAR PTR loc_2_61AB
loc_2_61AB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_61B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 010h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 014h, 00Ah, 000h, 000h
        call   sub_2_7A70
        add    esp, 4
        db  068h, 044h, 00Ah, 000h, 000h
        call   sub_2_7A70
        add    esp, 4
        db  0C7h, 005h, 058h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h
        mov    ax, 1
        jmp    NEAR PTR loc_2_61ED
loc_2_61ED:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_61F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6850
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_6220
        sub    ax, ax
        jmp    NEAR PTR loc_2_63A0
loc_2_6220:
        mov    ax, word ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_697F
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_624B
        mov    ax, 1
        jmp    NEAR PTR loc_2_63A0
loc_2_624B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_68BF
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_633C
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_6A82
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_6296
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        mov    ax, 1
        jmp    NEAR PTR loc_2_63A0
loc_2_6296:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2ah]
        push   eax
        db  0A1h, 054h, 00Ah, 000h, 000h
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_2_6B3C
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_62E1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_6B13
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        mov    ax, 1
        jmp    NEAR PTR loc_2_63A0
loc_2_62E1:
        db  0A1h, 054h, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 2eh], ax
        mov    byte ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_2_62F9
loc_2_62F6:
        inc    byte ptr [ebp - 18h]
loc_2_62F9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_633C
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        db  0C7h, 040h, 011h, 05Dh, 070h, 000h, 000h
        mov    al, byte ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_2_6D87
        add    esp, 0ch
        jmp    NEAR PTR loc_2_62F6
loc_2_633C:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_6372
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 21h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6479
        add    esp, 4
        jmp    NEAR PTR loc_2_6379
loc_2_6372:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 0
loc_2_6379:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 3bh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0ch], ecx
        sub    ax, ax
        jmp    NEAR PTR loc_2_63A0
loc_2_63A0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_63A5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_2_6407
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3bh
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_6407
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_6D38
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_6B13
        add    esp, 4
loc_2_6407:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_642E
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 25h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3E42
        add    esp, 8
loc_2_642E:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4fh
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_645F
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_642E
loc_2_645F:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        jmp    NEAR PTR loc_2_6474
loc_2_6474:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6479:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 174h]
        mov    dword ptr [ebp - 20h], eax
loc_2_648E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 174h
        cmp    eax, dword ptr [ebp - 20h]
        je     NEAR PTR loc_2_65E2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_64B8
        jmp    NEAR PTR loc_2_65E2
loc_2_64B8:
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_65DD
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 23h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_6554
        push   10h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        db  0E8h, 042h, 0A0h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 21h]
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 21h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_653A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
loc_2_653A:
        mov    ax, word ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0ch
        jmp    NEAR PTR loc_2_65DD
loc_2_6554:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 21h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_65CF
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0ch
        mov    byte ptr [ebp - 1ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        or     eax, eax
        je     NEAR PTR loc_2_65A4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        jmp    NEAR PTR loc_2_65CA
loc_2_65A4:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 3
        db  068h, 013h, 068h, 000h, 000h
        push   4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 25h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 10h
        jmp    NEAR PTR loc_2_65CA
loc_2_65CA:
        jmp    NEAR PTR loc_2_65DD
loc_2_65CF:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_65DD:
        jmp    NEAR PTR loc_2_648E
loc_2_65E2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_65E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 20h], 0
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 28h], 0
        mov    dword ptr [ebp - 14h], 0
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 174h]
        mov    dword ptr [ebp - 24h], eax
loc_2_6624:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 174h
        cmp    eax, dword ptr [ebp - 24h]
        je     NEAR PTR loc_2_6743
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 24h], eax
loc_2_6643:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4fh
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_2_673E
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_668A
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 248h], 0
        jne    NEAR PTR loc_2_668A
        mov    dword ptr [ebp - 20h], 1
loc_2_668A:
        db  083h, 03Dh, 024h, 00Ah, 000h, 000h, 000h
        je     NEAR PTR loc_2_66C1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 26ch
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_66BC
        mov    dword ptr [ebp - 18h], 1
loc_2_66BC:
        jmp    NEAR PTR loc_2_66C8
loc_2_66C1:
        mov    dword ptr [ebp - 18h], 1
loc_2_66C8:
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_2_66EA
        mov    dword ptr [ebp - 28h], 1
loc_2_66EA:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_2_6729
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_6729
        cmp    dword ptr [ebp - 28h], 0
        je     NEAR PTR loc_2_6729
        mov    dword ptr [ebp - 14h], 1
        jmp    NEAR PTR loc_2_6714
loc_2_6714:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1420
        add    esp, 8
        jmp    NEAR PTR loc_2_6739
loc_2_6729:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
loc_2_6739:
        jmp    NEAR PTR loc_2_6643
loc_2_673E:
        jmp    NEAR PTR loc_2_6624
loc_2_6743:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_2_675B
        db  0A1h, 024h, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 248h], eax
loc_2_675B:
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_2_6789
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_675B
loc_2_6789:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_678E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_2_6797:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 174h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_67D6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 174h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_63A5
        add    esp, 8
        jmp    NEAR PTR loc_2_6797
loc_2_67D6:
        db  068h, 044h, 00Ah, 000h, 000h
        call   sub_2_7B96
        add    esp, 4
        mov    ecx, eax
        and    ecx, 0ffffh
        or     ecx, ecx
        jne    NEAR PTR loc_2_67FF
        mov    al, 1
        jmp    NEAR PTR loc_2_680E
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_67FF:
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 010h, 00Ah, 000h, 000h
        sub    al, al
        jmp    NEAR PTR loc_2_680E
loc_2_680E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_6813:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_681E
loc_2_681E:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6A59
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_2_6850:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 174h]
        mov    dword ptr [ebp - 8], eax
loc_2_6865:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 174h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_2_68B3
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        db  0E8h, 09Dh, 09Ch, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_2_68A6
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_68AE
loc_2_68A6:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_68BA
loc_2_68AE:
        jmp    NEAR PTR loc_2_6865
loc_2_68B3:
        sub    eax, eax
        jmp    NEAR PTR loc_2_68BA
loc_2_68BA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_68BF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 014h, 00Ah, 000h, 000h
        mov    dword ptr [ebp - 8], eax
loc_2_68D0:
        db  081h, 07Dh, 0F8h, 014h, 00Ah, 000h, 000h
        je     NEAR PTR loc_2_691A
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1ah
        push   eax
        db  0E8h, 036h, 09Ch, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_2_690D
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_6915
loc_2_690D:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_6921
loc_2_6915:
        jmp    NEAR PTR loc_2_68D0
loc_2_691A:
        sub    eax, eax
        jmp    NEAR PTR loc_2_6921
loc_2_6921:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6926:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 014h, 00Ah, 000h, 000h
        mov    dword ptr [ebp - 8], eax
loc_2_6937:
        db  081h, 07Dh, 0F8h, 014h, 00Ah, 000h, 000h
        je     NEAR PTR loc_2_6973
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2ah], ecx
        je     NEAR PTR loc_2_6966
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_696E
loc_2_6966:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_697A
loc_2_696E:
        jmp    NEAR PTR loc_2_6937
loc_2_6973:
        sub    eax, eax
        jmp    NEAR PTR loc_2_697A
loc_2_697A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_697F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   5fh
        push   0
        db  0E8h, 06Fh, 096h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_69A8
        sub    eax, eax
        jmp    NEAR PTR loc_2_6A54
loc_2_69A8:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        db  0E8h, 07Ah, 09Bh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 0
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 23h], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 25h
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 33h], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 3bh
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 47h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4bh], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4fh
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 5bh], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 174h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_6A54
loc_2_6A54:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6A59:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 0BDh, 095h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6A82:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   50h
        push   0
        db  0E8h, 06Ch, 095h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_6AAB
        sub    eax, eax
        jmp    NEAR PTR loc_2_6B0E
loc_2_6AAB:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 18h], 0
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1ah
        push   eax
        db  0E8h, 069h, 09Ah, 0FDh, 0FFh
        add    esp, 0ch
        db  0A1h, 058h, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2ah], eax
        db  0FFh, 005h, 058h, 00Ah, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 014h, 00Ah, 000h, 000h
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_6B0E
loc_2_6B0E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6B13:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 003h, 095h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6B3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_6B60
loc_2_6B5D:
        inc    dword ptr [ebp - 4]
loc_2_6B60:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_2_6C04
        push   59h
        push   0
        db  0E8h, 08Bh, 094h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_6BC7
loc_2_6B85:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_6BBE
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_6C11
        add    esp, 4
        jmp    NEAR PTR loc_2_6B85
loc_2_6BBE:
        mov    ax, 1
        jmp    NEAR PTR loc_2_6C0C
loc_2_6BC7:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0dh], eax
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 19h
        push   eax
        db  0E8h, 075h, 099h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_6B5D
loc_2_6C04:
        sub    ax, ax
        jmp    NEAR PTR loc_2_6C0C
loc_2_6C0C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6C11:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 011h, 094h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6C2E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_6C52
loc_2_6C4F:
        inc    dword ptr [ebp - 4]
loc_2_6C52:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_2_6D0E
        push   606h
        push   0
        db  0E8h, 096h, 093h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_6CBC
loc_2_6C7A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_6CB3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
        jmp    NEAR PTR loc_2_6C7A
loc_2_6CB3:
        mov    ax, 1
        jmp    NEAR PTR loc_2_6D16
loc_2_6CBC:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0dh], eax
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 19h
        push   eax
        db  0E8h, 080h, 098h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        add    eax, 59h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1dh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 21h], 5adh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_6C4F
loc_2_6D0E:
        sub    ax, ax
        jmp    NEAR PTR loc_2_6D16
loc_2_6D16:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6D1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 007h, 093h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6D38:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_6D46
loc_2_6D46:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_6D55
loc_2_6D51:
        inc    word ptr [ebp - 4]
loc_2_6D55:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_6D82
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ah
        push   eax
        call   sub_2_6EDF
        add    esp, 8
        jmp    NEAR PTR loc_2_6D51
loc_2_6D82:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6D87:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 19h], 0b6h
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 33h
        push   eax
        db  0E8h, 0A1h, 097h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 15h], 9
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 49h], al
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 044h, 00Ah, 000h, 000h
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 045h, 058h, 004h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 19h
        push   eax
        db  0E8h, 01Dh, 098h, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     NEAR PTR loc_2_6E26
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1ah], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 4ah], al
        push   0
        call   sub_2_6F40
        add    esp, 4
loc_2_6E26:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6E2B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 19h], 0a1h
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1ch], al
        mov    eax, dword ptr [ebp + 8]
        add    eax, 59h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1dh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 21h], 5adh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 15h], 9
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4ah], 0ffh
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 49h], al
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 044h, 00Ah, 000h, 000h
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 045h, 058h, 004h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 19h
        push   eax
        db  0E8h, 069h, 097h, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     NEAR PTR loc_2_6EDA
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1ah], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 4ah], al
        push   0
        call   sub_2_6F40
        add    esp, 4
loc_2_6EDA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_6EDF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 06Bh, 096h, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 031h
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0B8h, 090h, 007h, 000h, 000h
        add    eax, 1ah
        push   eax
        db  0E8h, 036h, 096h, 0FDh, 0FFh
        add    esp, 0ch
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        db  0A2h, 0C0h, 007h, 000h, 000h
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 010h, 097h, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h
sub_2_6F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        db  0A1h, 044h, 00Ah, 000h, 000h
        mov    dword ptr [ebp - 8], eax
loc_2_6F5D:
        db  081h, 07Dh, 0F8h, 044h, 00Ah, 000h, 000h
        je     NEAR PTR loc_2_6FC2
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4ah]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_2_6FB5
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_6FBD
loc_2_6FB5:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_2_6FBD:
        jmp    NEAR PTR loc_2_6F5D
loc_2_6FC2:
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_6FED
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        call   dword ptr [eax + 11h]
        add    esp, 4
        jmp    NEAR PTR loc_2_6FC2
loc_2_6FED:
        db  083h, 03Dh, 010h, 00Ah, 000h, 000h, 000h
        je     NEAR PTR loc_2_7051
        db  068h, 044h, 00Ah, 000h, 000h
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_7051
        push   8
        db  0A1h, 010h, 00Ah, 000h, 000h
        push   eax
        call   sub_2_0ECA
        add    esp, 8
        db  0A1h, 010h, 00Ah, 000h, 000h
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_7047
        db  0A1h, 010h, 00Ah, 000h, 000h
        push   eax
        db  0E8h, 0F0h, 08Fh, 0FDh, 0FFh
        add    esp, 4
loc_2_7047:
        db  0C7h, 005h, 010h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_7051:
        sub    eax, eax
        jmp    NEAR PTR loc_2_7058
loc_2_7058:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_705D:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dh]
        push   eax
        call   sub_2_6926
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_2_7093
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6C11
        add    esp, 4
        jmp    NEAR PTR loc_2_72D3
loc_2_7093:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ch]
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_2_70A1
loc_2_70A1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1ah]
        and    eax, 0ffh
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_2_70FE
loc_2_70B4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1ch]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    byte ptr [edx + ecx*2 + 30h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + eax*2 + 31h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 19h
        push   eax
        db  0E8h, 093h, 095h, 0FDh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_2_7117
loc_2_70E7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + eax*2 + 31h], 1
        jmp    NEAR PTR loc_2_7117
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_70FE:
        cmp    dword ptr [ebp - 2ch], 0
        je     NEAR PTR loc_2_70B4
        cmp    dword ptr [ebp - 2ch], 0dh
        je     NEAR PTR loc_2_70B4
        jmp    NEAR PTR loc_2_70E7
loc_2_7117:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6C11
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        dec    word ptr [eax + 2eh]
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 2eh]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_7143
        jmp    NEAR PTR loc_2_72D3
loc_2_7143:
        mov    word ptr [ebp - 18h], 0
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_2_7158
loc_2_7154:
        inc    word ptr [ebp - 18h]
loc_2_7158:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_7198
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ecx + eax*2 + 31h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_2_7193
        mov    word ptr [ebp - 14h], 1
        jmp    NEAR PTR loc_2_7198
loc_2_7193:
        jmp    NEAR PTR loc_2_7154
loc_2_7198:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 2ah]
        push   eax
        db  0A1h, 054h, 00Ah, 000h, 000h
        db  00Fh, 0AFh, 005h, 050h, 00Ah, 000h, 000h
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_2_6C2E
        add    esp, 0ch
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_71DC
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_722F
loc_2_71DC:
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_7206
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
        jmp    NEAR PTR loc_2_71DC
loc_2_7206:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_6D38
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_74E7
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_6B13
        add    esp, 4
        jmp    NEAR PTR loc_2_72D3
loc_2_722F:
        mov    word ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_2_723E
loc_2_723A:
        inc    word ptr [ebp - 18h]
loc_2_723E:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_72B9
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_7261
loc_2_725D:
        inc    word ptr [ebp - 4]
loc_2_7261:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 050h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_72B4
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 011h, 0D8h, 072h, 000h, 000h
        mov    ax, word ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ecx + eax*2 + 30h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_2_6E2B
        add    esp, 0ch
        jmp    NEAR PTR loc_2_725D
loc_2_72B4:
        jmp    NEAR PTR loc_2_723A
loc_2_72B9:
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_7486
        add    esp, 4
        jmp    NEAR PTR loc_2_72D3
loc_2_72D3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_72D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dh]
        push   eax
        call   sub_2_6926
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_730E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
        jmp    NEAR PTR loc_2_7481
loc_2_730E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1ah]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_2_736A
        jmp    NEAR PTR loc_2_7326
loc_2_7326:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1ah]
        and    eax, 0ffh
        cmp    eax, 6
        jne    NEAR PTR loc_2_7359
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6E2B
        add    esp, 0ch
        jmp    NEAR PTR loc_2_7365
loc_2_7359:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6D1B
        add    esp, 4
loc_2_7365:
        jmp    NEAR PTR loc_2_7481
loc_2_736A:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
loc_2_7373:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_2_7462
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        cmp    dword ptr [eax + 248h], 0
        jne    NEAR PTR loc_2_745D
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 21h]
        push   eax
        push   2
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_2_17B6
        add    esp, 10h
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_745D
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 34h], 0a0h
        push   10h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 23h
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 4eh
        push   eax
        db  0E8h, 05Eh, 091h, 0FDh, 0FFh
        add    esp, 0ch
        push   10h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 3eh
        push   eax
        db  0E8h, 043h, 091h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 21h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 21h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1dh]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 38h]
        push   eax
        db  0E8h, 012h, 091h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_2_65E7
        add    esp, 4
loc_2_745D:
        jmp    NEAR PTR loc_2_7373
loc_2_7462:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6E2B
        add    esp, 0ch
        jmp    NEAR PTR loc_2_7481
loc_2_7481:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7486:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
loc_2_7498:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0ch
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_2_74E2
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 21h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_2_6479
        add    esp, 4
        jmp    NEAR PTR loc_2_7498
loc_2_74E2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_74E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_2_74F0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0ch
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_7543
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 21h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_2_6479
        add    esp, 4
        jmp    NEAR PTR loc_2_74F0
loc_2_7543:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_7550:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 228h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7567:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_7575
loc_2_7575:
        call   sub_2_7BC5
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_759A
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_75FC
loc_2_759A:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 228h]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 001h, 076h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 34h], 0f6h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 3ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 37h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 36h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
loc_2_75FC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_7601:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_7615
loc_2_7615:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 228h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_7655
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 228h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_7650
        mov    word ptr [ebp - 4], 1
loc_2_7650:
        jmp    NEAR PTR loc_2_7760
loc_2_7655:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_7747
loc_2_7668:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7567
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 37h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_2_76F4
loc_2_7687:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2A74
        add    esp, 8
        jmp    NEAR PTR loc_2_7717
loc_2_769F:
        push   2
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_61F2
        add    esp, 0ch
        jmp    NEAR PTR loc_2_7717
loc_2_76B9:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax + 3ch], 0
        je     NEAR PTR loc_2_76E5
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_51AA
        add    esp, 0ch
        jmp    NEAR PTR loc_2_76E5
loc_2_76E5:
        jmp    NEAR PTR loc_2_7717
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_76F4:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_769F
        cmp    dword ptr [ebp - 10h], 1
        je     NEAR PTR loc_2_7687
        cmp    dword ptr [ebp - 10h], 0ah
        je     NEAR PTR loc_2_76B9
        jmp    NEAR PTR loc_2_7717
loc_2_7717:
        jmp    NEAR PTR loc_2_7760
loc_2_771C:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_7760
loc_2_772F:
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_7760
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_7747:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_7668
        cmp    dword ptr [ebp - 0ch], 1
        je     NEAR PTR loc_2_771C
        jmp    NEAR PTR loc_2_772F
loc_2_7760:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_778A
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_778A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_778F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 228h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_77B8
        sub    al, al
        jmp    NEAR PTR loc_2_77BF
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_77B8:
        mov    al, 1
        jmp    NEAR PTR loc_2_77BF
loc_2_77BF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_77C4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 22ah], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_77DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_77E9
loc_2_77E9:
        call   sub_2_7BC5
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_780E
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_78B4
loc_2_780E:
        push   5adh
        push   0
        db  0E8h, 0E6h, 087h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_7849
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_78B4
loc_2_7849:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 22ah]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 0B9h, 078h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 34h], 0f4h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 3ch], 5adh
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 37h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 36h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
loc_2_78B4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_78B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_78CD
loc_2_78CD:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 22ah]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_790D
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22ah]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_7908
        mov    word ptr [ebp - 4], 1
loc_2_7908:
        jmp    NEAR PTR loc_2_79FA
loc_2_790D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_79C5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_77DB
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_79C0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_796C
        jmp    NEAR PTR loc_2_79C0
loc_2_796C:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_2_797B
loc_2_7977:
        inc    word ptr [ebp - 0ch]
loc_2_797B:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_79C0
        mov    ax, word ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8620
        add    esp, 10h
        jmp    NEAR PTR loc_2_7977
loc_2_79C0:
        jmp    NEAR PTR loc_2_79FA
loc_2_79C5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_79EC
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_79FA
loc_2_79EC:
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
loc_2_79FA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 38h]
        push   eax
        db  0E8h, 02Eh, 086h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_7A36
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_7A36:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7A3B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22ah]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_7A64
        sub    al, al
        jmp    NEAR PTR loc_2_7A6B
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_7A64:
        mov    al, 1
        jmp    NEAR PTR loc_2_7A6B
loc_2_7A6B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7A70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7A95:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7AAA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7AE6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7B16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_2_7B34
        sub    eax, eax
        jmp    NEAR PTR loc_2_7B52
loc_2_7B34:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_7B52
loc_2_7B52:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7B57:
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
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7B96:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_2_7BB7
        sub    ax, ax
        jmp    NEAR PTR loc_2_7BC0
        db  0E9h, 009h, 000h, 000h, 000h
loc_2_7BB7:
        mov    ax, 1
        jmp    NEAR PTR loc_2_7BC0
loc_2_7BC0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7BC5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   74h
        push   0
        db  0E8h, 029h, 084h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_7BEE
        sub    eax, eax
        jmp    NEAR PTR loc_2_7C2B
loc_2_7BEE:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        db  068h, 0A4h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2ch
        push   eax
        db  0E8h, 03Dh, 089h, 0FDh, 0FFh
        add    esp, 0ch
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 34h
        push   eax
        db  0E8h, 041h, 089h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_7C2B
loc_2_7C2B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 0F2h, 083h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7C4D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0dh], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7C6B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 4], eax
loc_2_7C80:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_7CC4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_2_7CB7
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_7CCB
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_7CB7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_7C80
loc_2_7CC4:
        sub    eax, eax
        jmp    NEAR PTR loc_2_7CCB
loc_2_7CCB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7CD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 4], eax
loc_2_7CE5:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_7D25
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax + 21h]
        or     eax, eax
        jne    NEAR PTR loc_2_7D18
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_7D2C
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_7D18:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_7CE5
loc_2_7D25:
        sub    eax, eax
        jmp    NEAR PTR loc_2_7D2C
loc_2_7D2C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7D31:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_7D5A
        sub    eax, eax
        jmp    NEAR PTR loc_2_7DCE
loc_2_7D5A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 4], eax
loc_2_7D66:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_7DC7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   0fh
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 12h
        push   eax
        db  0E8h, 099h, 087h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_2_7DBA
        mov    eax, dword ptr [ebp - 0ch]
        movsx  eax, byte ptr [eax + 21h]
        cmp    eax, 20h
        jne    NEAR PTR loc_2_7DBA
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_2_7DCE
        db  0E9h, 008h, 000h, 000h, 000h
loc_2_7DBA:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_7D66
loc_2_7DC7:
        sub    eax, eax
        jmp    NEAR PTR loc_2_7DCE
loc_2_7DCE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7DD3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_7E03
        sub    eax, eax
        jmp    NEAR PTR loc_2_7E11
        db  0E9h, 00Eh, 000h, 000h, 000h
loc_2_7E03:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        jmp    NEAR PTR loc_2_7E11
loc_2_7E11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_7E20:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        db  068h, 0B0h, 008h, 000h, 000h
        push   80000002h
        db  0E8h, 094h, 089h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_2_7E95
        mov    dword ptr [ebp - 0ch], 4
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 0A7h, 089h, 0FDh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_2_7E7E
        cmp    dword ptr [ebp - 4], 3
        je     NEAR PTR loc_2_7E84
loc_2_7E7E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
loc_2_7E84:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 066h, 089h, 0FDh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_2_7E9B
loc_2_7E95:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
loc_2_7E9B:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    eax, ecx
        ja     NEAR PTR loc_2_7EAB
        mov    eax, ecx
loc_2_7EAB:
        mov    ecx, dword ptr [ebp + 14h]
        cmp    eax, ecx
        jb     NEAR PTR loc_2_7EB8
        mov    eax, ecx
loc_2_7EB8:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_7EC3
loc_2_7EC3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_7EC8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0ffh
        push   1
        push   0ffh
        db  068h, 0FCh, 008h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2d8h], eax
        push   0ffh
        push   1
        push   0ffh
        db  068h, 008h, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2dch], eax
        push   10000h
        push   400h
        push   1194h
        db  068h, 014h, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2e0h], eax
        push   800h
        push   80h
        push   200h
        db  068h, 020h, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        shl    eax, 0ah
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2e4h], eax
        push   2
        push   0
        push   0
        db  068h, 02Ch, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2e8h], eax
        push   1
        push   0
        push   0
        db  068h, 03Ch, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2f0h], eax
        push   0e10h
        push   0ah
        push   3ch
        db  068h, 04Ch, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2f4h], eax
        push   1
        push   0
        push   1
        db  068h, 05Ch, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2f8h], eax
        push   0ffh
        push   10h
        push   20h
        db  068h, 080h, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2fch], eax
        push   20h
        push   1
        push   3
        db  068h, 094h, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 300h], eax
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_2_8038
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0ea60h
        jb     NEAR PTR loc_2_802A
        mov    eax, 0ea60h
loc_2_802A:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2ech], eax
        jmp    NEAR PTR loc_2_8057
loc_2_8038:
        push   0ea60h
        push   0
        push   0
        db  068h, 0ACh, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2ech], eax
loc_2_8057:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_805C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   -1
        push   0
        push   5
        db  068h, 0BCh, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        db  0A3h, 024h, 00Ah, 000h, 000h
        db  083h, 03Dh, 024h, 00Ah, 000h, 000h, 000h
        je     NEAR PTR loc_2_80A0
        db  0C7h, 005h, 020h, 00Ah, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 050h, 00Ah, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_80D3
loc_2_80A0:
        push   0ffh
        push   1
        push   0ah
        db  068h, 0D4h, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        db  0A3h, 020h, 00Ah, 000h, 000h
        push   20h
        push   1
        push   3
        db  068h, 0F0h, 009h, 000h, 000h
        call   sub_2_7E20
        add    esp, 10h
        db  0A3h, 050h, 00Ah, 000h, 000h
loc_2_80D3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_80E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 0b0h
        push   10h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        db  0E8h, 04Bh, 084h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8138:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 012h, 084h, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 075h
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0B8h, 090h, 007h, 000h, 000h
        add    eax, 1ah
        push   eax
        db  0E8h, 0DDh, 083h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        db  0A2h, 0C0h, 007h, 000h, 000h
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 08Ch, 084h, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        db  08Ah, 00Dh, 093h, 007h, 000h, 000h
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [edx], cl
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_2_81A1
loc_2_81A1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_81A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 0A4h, 083h, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 031h
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0B8h, 090h, 007h, 000h, 000h
        add    eax, 1ah
        push   eax
        db  0E8h, 06Fh, 083h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_8207
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        db  0A2h, 0C0h, 007h, 000h, 000h
        jmp    NEAR PTR loc_2_8215
loc_2_8207:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        db  0A2h, 0C0h, 007h, 000h, 000h
loc_2_8215:
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 0F5h, 083h, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_2_822D
loc_2_822D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8232:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 90h
        push   10h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        db  0E8h, 0F9h, 082h, 0FDh, 0FFh
        add    esp, 0ch
        push   10h
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        db  0E8h, 0E4h, 082h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_82B1
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5eh], 5ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5fh], 5ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        jmp    NEAR PTR loc_2_82D8
loc_2_82B1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5eh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5fh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
loc_2_82D8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_82ED:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 91h
        push   10h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        db  0E8h, 03Eh, 082h, 0FDh, 0FFh
        add    esp, 0ch
        push   10h
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        db  0E8h, 029h, 082h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_836C
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5eh], 5ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5fh], 5ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        jmp    NEAR PTR loc_2_8393
loc_2_836C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5eh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5fh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
loc_2_8393:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_83A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 0A5h, 081h, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 012h
        mov    al, byte ptr [ebp + 14h]
        db  0A2h, 092h, 007h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_83F7
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        db  0A2h, 0C0h, 007h, 000h, 000h
        jmp    NEAR PTR loc_2_8405
loc_2_83F7:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        db  0A2h, 0C0h, 007h, 000h, 000h
loc_2_8405:
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 005h, 082h, 0FDh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8417:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 34h
        push   eax
        db  0E8h, 034h, 081h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 92h
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 36h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_847A
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        jmp    NEAR PTR loc_2_8493
loc_2_847A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
loc_2_8493:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_84A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 96h
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 37h], 0ffh
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_84EA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 96h
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 37h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8536:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 95h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_857D
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        jmp    NEAR PTR loc_2_8596
loc_2_857D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
loc_2_8596:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_85AB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 94h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_85F2
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        jmp    NEAR PTR loc_2_860B
loc_2_85F2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
loc_2_860B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 02Ah, 07Fh, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 020h
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 3ch]
        db  066h, 0A3h, 098h, 007h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 38h]
        db  0A3h, 094h, 007h, 000h, 000h
        push   10h
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 4eh
        push   eax
        db  0B8h, 090h, 007h, 000h, 000h
        add    eax, 0ah
        push   eax
        db  0E8h, 0DAh, 07Eh, 0FDh, 0FFh
        add    esp, 0ch
        mov    al, byte ptr [ebp + 0ch]
        db  0A2h, 093h, 007h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        db  0A2h, 0C0h, 007h, 000h, 000h
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 07Fh, 07Fh, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        jmp    NEAR PTR loc_2_86A0
loc_2_86A0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_86A5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 10h], 1
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 0a1h
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 3ch], 5adh
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 37h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 5
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8704:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 049h, 07Eh, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 022h
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 3ch]
        db  066h, 0A3h, 098h, 007h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 38h]
        db  0A3h, 094h, 007h, 000h, 000h
        mov    al, byte ptr [ebp + 0ch]
        db  0A2h, 093h, 007h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        db  0A2h, 0C0h, 007h, 000h, 000h
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 0B8h, 07Eh, 0FDh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8764:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_87A4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 02Ch, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
        jmp    NEAR PTR loc_2_87BD
loc_2_87A4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 64h], al
loc_2_87BD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 0a3h
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 3ch], 5adh
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 37h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_87EB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   3ch
        push   0
        db  0E8h, 003h, 078h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_8814
        sub    eax, eax
        jmp    NEAR PTR loc_2_88A6
loc_2_8814:
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 03Fh, 07Dh, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 033h
        db  066h, 0C7h, 005h, 098h, 007h, 000h, 000h, 03Ch, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 094h, 007h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        db  0A2h, 0C0h, 007h, 000h, 000h
        db  0C6h, 005h, 09Ah, 007h, 000h, 000h, 02Ah
        db  0C6h, 005h, 0AAh, 007h, 000h, 000h, 02Ah
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 0B7h, 07Dh, 0FDh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0CBh, 077h, 0FDh, 0FFh
        add    esp, 4
        db  00Fh, 0BEh, 005h, 0AAh, 007h, 000h, 000h
        cmp    eax, 2ah
        jne    NEAR PTR loc_2_8883
        sub    eax, eax
        jmp    NEAR PTR loc_2_88A6
loc_2_8883:
        db  0F6h, 005h, 0AEh, 007h, 000h, 000h, 002h
        je     NEAR PTR loc_2_889F
        mov    eax, 1
        jmp    NEAR PTR loc_2_88A6
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_889F:
        sub    eax, eax
        jmp    NEAR PTR loc_2_88A6
loc_2_88A6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_88AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   3ch
        push   0
        db  0E8h, 043h, 077h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_88D7
        mov    eax, 1
        jmp    NEAR PTR loc_2_896C
loc_2_88D7:
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 07Ch, 07Ch, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 033h
        db  066h, 0C7h, 005h, 098h, 007h, 000h, 000h, 03Ch, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 094h, 007h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        db  0A2h, 0C0h, 007h, 000h, 000h
        db  0C6h, 005h, 09Ah, 007h, 000h, 000h, 02Ah
        db  0C6h, 005h, 0AAh, 007h, 000h, 000h, 02Ah
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 0F4h, 07Ch, 0FDh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 008h, 077h, 0FDh, 0FFh
        add    esp, 4
        db  00Fh, 0BEh, 005h, 0AAh, 007h, 000h, 000h
        cmp    eax, 2ah
        jne    NEAR PTR loc_2_8949
        mov    eax, 1
        jmp    NEAR PTR loc_2_896C
loc_2_8949:
        db  0F6h, 005h, 0AEh, 007h, 000h, 000h, 001h
        je     NEAR PTR loc_2_8965
        mov    eax, 1
        jmp    NEAR PTR loc_2_896C
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_8965:
        sub    eax, eax
        jmp    NEAR PTR loc_2_896C
loc_2_896C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_8980:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_898E
loc_2_898E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_2_89A9
        sub    ax, ax
        jmp    NEAR PTR loc_2_8A6A
loc_2_89A9:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2220
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0420
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 174h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5140
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3F70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1090
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3530
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_32A0
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0C20
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7550
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_77C4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_22FC
        add    esp, 4
        mov    ax, 1
        jmp    NEAR PTR loc_2_8A6A
loc_2_8A6A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8A6F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_8A7A
loc_2_8A7A:
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2b4h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_01DD
        add    esp, 4
        mov    ax, 1
        jmp    NEAR PTR loc_2_8A9B
loc_2_8A9B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_8AA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_8AAB
loc_2_8AAB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02AB
        add    esp, 4
        mov    ax, 1
        jmp    NEAR PTR loc_2_8AC0
loc_2_8AC0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_PTEXT ends
        end
