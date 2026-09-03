; dlc_pcode_06.asm -- DLC object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_6_001C
loc_6_001C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_6_0024:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_6_0040
loc_6_0040:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_6_0048:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_0059:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_006A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_007B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_6_0097
loc_6_0097:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_009C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_6_00B8
loc_6_00B8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_6_00C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_6_00DC
loc_6_00DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_6_00E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_010B
        mov    eax, 2
        jmp    NEAR PTR loc_6_02D1
loc_6_010B:
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0121
        mov    word ptr [ebp + 18h], 0a0h
loc_6_0121:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0137
        mov    word ptr [ebp + 14h], 100h
loc_6_0137:
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        cmp    eax, 50h
        jl     NEAR PTR loc_6_0162
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0fh
        xor    eax, edx
        sub    eax, edx
        je     NEAR PTR loc_6_016C
loc_6_0162:
        mov    eax, 18h
        jmp    NEAR PTR loc_6_02D1
loc_6_016C:
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        sar    eax, 4
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_6_019E
        mov    eax, 16h
        jmp    NEAR PTR loc_6_02D1
loc_6_019E:
        mov    ax, word ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 6], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_6_02CA
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0211
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 6], 0
        mov    eax, 16h
        jmp    NEAR PTR loc_6_02D1
loc_6_0211:
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 6], ax
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 8], ax
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_6_0252
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_6_0259
loc_6_0252:
        mov    dword ptr [ebp - 10h], 0
loc_6_0259:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0fff0h
        shl    eax, 0ch
        add    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_6_0282
loc_6_027F:
        dec    dword ptr [ebp - 4]
loc_6_0282:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_6_02CA
        mov    eax, dword ptr [ebp - 4]
        dec    eax
        je     NEAR PTR loc_6_02BC
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0fff0h
        shl    eax, 0ch
        add    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        add    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_6_02C5
loc_6_02BC:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax], 0
loc_6_02C5:
        jmp    NEAR PTR loc_6_027F
loc_6_02CA:
        xor    eax, eax
        jmp    NEAR PTR loc_6_02D1
loc_6_02D1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_0300
        push   0ah
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_02FD:
        add    esp, 0ch
loc_6_0300:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_0305:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 1ch], 0
        je     NEAR PTR loc_6_0340
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_6_0340
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp + 10h], ax
loc_6_0340:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0354
        inc    word ptr [ebp + 10h]
loc_6_0354:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_04CD
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        sub    eax, 0ch
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_6_04AE
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_6_04AE
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_6_03DE
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_6_03E5
loc_6_03DE:
        mov    dword ptr [ebp - 4], 0
loc_6_03E5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 6], ax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [ebp - 10h], ax
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
        dec    word ptr [ebp + 10h]
        jmp    NEAR PTR loc_6_042D
loc_6_0429:
        dec    word ptr [ebp + 10h]
loc_6_042D:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_048F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_6_046E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_6_0475
loc_6_046E:
        mov    dword ptr [ebp - 4], 0
loc_6_0475:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_6_048A
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
loc_6_048A:
        jmp    NEAR PTR loc_6_0429
loc_6_048F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_6_04BB
loc_6_04AE:
        mov    dword ptr [ebp - 0ch], 20h
        mov    word ptr [ebp - 10h], 0
loc_6_04BB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_6_04DA
loc_6_04CD:
        mov    dword ptr [ebp - 0ch], 40h
        mov    word ptr [ebp - 10h], 0
loc_6_04DA:
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 20h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_6_04EC
loc_6_04EC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_04F1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0518
        mov    eax, 40h
        jmp    NEAR PTR loc_6_0624
loc_6_0518:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_6_0549
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_6_0550
loc_6_0549:
        mov    dword ptr [ebp - 4], 0
loc_6_0550:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_6_05DD
loc_6_0569:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_6_05C6
        mov    eax, dword ptr [ebp + 0ch]
        inc    word ptr [eax + 6]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_6_05B1
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_6_05B8
loc_6_05B1:
        mov    dword ptr [ebp - 8], 0
loc_6_05B8:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        jmp    NEAR PTR loc_6_0569
loc_6_05C6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_6_05DD
loc_6_05DD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_6_061D
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 8], ax
loc_6_061D:
        xor    eax, eax
        jmp    NEAR PTR loc_6_0624
loc_6_0624:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_0629:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 10h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + 0eh]
        and    ecx, 0c0h
        cmp    ecx, 1
        sbb    ecx, ecx
        and    ecx, 26h
        add    ecx, 14h
        mov    edx, dword ptr [ebp + 14h]
        and    edx, 0ffffh
        add    ecx, edx
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        test   al, 20h
        je     NEAR PTR loc_6_06AF
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        test   al, 0c0h
        jne    NEAR PTR loc_6_06AA
        mov    word ptr [ebp - 0ch], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_6_06AA
loc_6_06AA:
        jmp    NEAR PTR loc_6_070D
loc_6_06AF:
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_6_06EC
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_6_070D
loc_6_06EC:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 0ch], 0
loc_6_070D:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_0798
        inc    word ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    ecx, 0ch
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_6_0776
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_6_0793
loc_6_0776:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 0ch], 0
loc_6_0793:
        jmp    NEAR PTR loc_6_070D
loc_6_0798:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 10h], eax
        jmp    NEAR PTR loc_6_07AD
loc_6_07A5:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 10h], eax
loc_6_07AD:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_6_08AB
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_6_0885
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        mov    word ptr [ebp - 4], 0
loc_6_07F5:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_0880
        inc    word ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    ecx, 0ch
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_6_085E
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_6_087B
loc_6_085E:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 0ch], 0
loc_6_087B:
        jmp    NEAR PTR loc_6_07F5
loc_6_0880:
        jmp    NEAR PTR loc_6_08A6
loc_6_0885:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 0ch], 0
loc_6_08A6:
        jmp    NEAR PTR loc_6_07A5
loc_6_08AB:
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx], ax
        mov    ax, word ptr [ebp - 10h]
        jmp    NEAR PTR loc_6_08BE
loc_6_08BE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_6_08C3:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_6_08F7
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_6_08FE
loc_6_08F7:
        mov    dword ptr [ebp - 1ch], 0
loc_6_08FE:
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        sub    eax, 14h
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [ebp - 4], ax
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0929:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 14h
        mov    dword ptr [ebp - 14h], eax
        test   byte ptr [ebp + 1ch], 1
        jne    NEAR PTR loc_6_098F
        push   26h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 18h
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0951:
        add    esp, 0ch
        add    dword ptr [ebp - 14h], 26h
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        sub    eax, 26h
        mov    word ptr [ebp - 18h], ax
        mov    word ptr [ebp - 2ch], 3ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 19h]
        and    ecx, 0ffh
        add    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_6_099B
loc_6_098F:
        mov    word ptr [ebp - 2ch], 14h
        mov    word ptr [ebp - 0ch], 0
loc_6_099B:
        test   byte ptr [ebp + 1ch], 2
        jne    NEAR PTR loc_6_09CC
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        add    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 18h], ax
        jmp    NEAR PTR loc_6_0A96
loc_6_09CC:
        mov    eax, dword ptr [ebp - 1ch]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 8], ax
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_6_0A34
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_6_0A3B
loc_6_0A34:
        mov    dword ptr [ebp - 1ch], 0
loc_6_0A3B:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_6_0A7B
        mov    word ptr [ebp - 2ch], 0ch
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    ecx, 0ch
        sub    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 1ch]
        add    eax, 0ch
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_6_0A96
loc_6_0A7B:
        test   byte ptr [ebp + 1ch], 4
        je     NEAR PTR loc_6_0A8F
        mov    eax, 21h
        jmp    NEAR PTR loc_6_0A91
loc_6_0A8F:
        xor    eax, eax
loc_6_0A91:
        jmp    NEAR PTR loc_6_0D80
loc_6_0A96:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 28h], ax
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0ah]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 20h], 0
loc_6_0AEA:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jg     NEAR PTR loc_6_0B44
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_6_0B80
loc_6_0B44:
        mov    ax, word ptr [ebp - 18h]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 24h], ax
        mov    word ptr [ebp - 18h], 0
loc_6_0B80:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_0BD6
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0BA6:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        add    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        add    dword ptr [ebp - 30h], eax
loc_6_0BD6:
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_6_0D1A
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0C3E
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 0ch], eax
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_6_0C39
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0ah]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 30h], eax
        jmp    NEAR PTR loc_6_0C3E
loc_6_0C39:
        jmp    NEAR PTR loc_6_0D65
loc_6_0C3E:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0D15
        mov    ax, word ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 8], ax
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_6_0CC3
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_6_0CCA
loc_6_0CC3:
        mov    dword ptr [ebp - 1ch], 0
loc_6_0CCA:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_6_0D10
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 1ch]
        add    eax, 0ch
        mov    dword ptr [ebp - 14h], eax
        mov    word ptr [ebp - 2ch], 0ch
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    ecx, 0ch
        sub    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_6_0D15
loc_6_0D10:
        jmp    NEAR PTR loc_6_0D65
loc_6_0D15:
        jmp    NEAR PTR loc_6_0D55
loc_6_0D1A:
        mov    ax, word ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 8], ax
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 0ah], ax
loc_6_0D55:
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_6_0AEA
loc_6_0D65:
        test   byte ptr [ebp + 1ch], 4
        je     NEAR PTR loc_6_0D79
        mov    eax, 21h
        jmp    NEAR PTR loc_6_0D7B
loc_6_0D79:
        xor    eax, eax
loc_6_0D7B:
        jmp    NEAR PTR loc_6_0D80
loc_6_0D80:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_PTEXT ends
        end
