; dlc_lcode_01_08.asm -- DLC object 1 segment _LTEXT  (part 8/9)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_245A7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_245BF
loc_1_245BC:
        inc    dword ptr [ebp - 4]
loc_1_245BF:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 4], eax
        jge    NEAR PTR loc_1_245F5
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + eax*2 + 8], 0
        jne    NEAR PTR loc_1_245F0
        jmp    NEAR PTR loc_1_245F5
loc_1_245F0:
        jmp    NEAR PTR loc_1_245BC
loc_1_245F5:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_1_2460A
        xor    eax, eax
        jmp    NEAR PTR loc_1_24661
loc_1_2460A:
        push   9eh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_051D
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + eax*2], 42434f56h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_24661
loc_1_24661:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_24666:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 8], al
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_24692
        xor    eax, eax
        jmp    NEAR PTR loc_1_2470B
loc_1_24692:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_246A1
loc_1_2469E:
        inc    dword ptr [ebp - 0ch]
loc_1_246A1:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_246D0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_246C7
        jmp    NEAR PTR loc_1_246D0
loc_1_246C7:
        add    dword ptr [ebp - 4], 1dh
        jmp    NEAR PTR loc_1_2469E
loc_1_246D0:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_24704
        push   1dh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_051D
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 42435053h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 17h], 0ffh
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2470B
loc_1_24704:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2470B
loc_1_2470B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_24710:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_24725
        jmp    NEAR PTR loc_1_24738
loc_1_24725:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 17h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 16h], 0
        jmp    NEAR PTR loc_1_24738
loc_1_24738:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2473D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_24767
loc_1_24764:
        inc    dword ptr [ebp - 0ch]
loc_1_24767:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_247AE
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_247A5
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_247A5
        jmp    NEAR PTR loc_1_247AE
loc_1_247A5:
        add    dword ptr [ebp - 4], 1dh
        jmp    NEAR PTR loc_1_24764
loc_1_247AE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0ch]
        jg     NEAR PTR loc_1_247C3
        xor    eax, eax
        jmp    NEAR PTR loc_1_247CB
loc_1_247C3:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_247CB
loc_1_247CB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_247D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_247FE
loc_1_247F7:
        inc    dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 8], 8
loc_1_247FE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_24833
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_24833
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_2482E
        jmp    NEAR PTR loc_1_24833
loc_1_2482E:
        jmp    NEAR PTR loc_1_247F7
loc_1_24833:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jg     NEAR PTR loc_1_24848
        xor    eax, eax
        jmp    NEAR PTR loc_1_2485E
loc_1_24848:
        push   8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_051D
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2485E
loc_1_2485E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_24863:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_2488D
loc_1_2488A:
        inc    dword ptr [ebp - 0ch]
loc_1_2488D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_248D9
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_248D0
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_248D0
        jmp    NEAR PTR loc_1_248D9
loc_1_248D0:
        add    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_2488A
loc_1_248D9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jg     NEAR PTR loc_1_248EE
        xor    eax, eax
        jmp    NEAR PTR loc_1_248F6
loc_1_248EE:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_248F6
loc_1_248F6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_248FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        cmp    eax, 3
        jge    NEAR PTR loc_1_24929
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 50h]
        jmp    NEAR PTR loc_1_24A6E
loc_1_24929:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_249C7
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 4], al
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_24957
loc_1_24954:
        inc    dword ptr [ebp - 0ch]
loc_1_24957:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_249A3
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2499A
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_2499A
        jmp    NEAR PTR loc_1_249A3
loc_1_2499A:
        add    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_24954
loc_1_249A3:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_249C7
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_249C7
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        jmp    NEAR PTR loc_1_24A6E
loc_1_249C7:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 18h], al
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    byte ptr [ebp - 14h], ah
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        test   eax, eax
        jne    NEAR PTR loc_1_249F8
        xor    eax, eax
        jmp    NEAR PTR loc_1_24A6E
loc_1_249F8:
        mov    dword ptr [ebp - 1ch], 0
        jmp    NEAR PTR loc_1_24A07
loc_1_24A04:
        inc    dword ptr [ebp - 1ch]
loc_1_24A07:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        cmp    eax, dword ptr [ebp - 1ch]
        jle    NEAR PTR loc_1_24A4E
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_24A45
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 14h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_24A45
        jmp    NEAR PTR loc_1_24A4E
loc_1_24A45:
        add    dword ptr [ebp - 10h], 1dh
        jmp    NEAR PTR loc_1_24A04
loc_1_24A4E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        cmp    eax, dword ptr [ebp - 1ch]
        jle    NEAR PTR loc_1_24A67
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        jmp    NEAR PTR loc_1_24A6E
loc_1_24A67:
        xor    eax, eax
        jmp    NEAR PTR loc_1_24A6E
loc_1_24A6E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_24A73:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 8], al
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_24A9D
loc_1_24A9A:
        inc    dword ptr [ebp - 0ch]
loc_1_24A9D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_24AE4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_24ADB
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_24ADB
        jmp    NEAR PTR loc_1_24AE4
loc_1_24ADB:
        add    dword ptr [ebp - 4], 1dh
        jmp    NEAR PTR loc_1_24A9A
loc_1_24AE4:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_24B11
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_24B11
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        jmp    NEAR PTR loc_1_24B30
loc_1_24B11:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_24B25
        xor    eax, eax
        jmp    NEAR PTR loc_1_24B30
loc_1_24B25:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14h
        jmp    NEAR PTR loc_1_24B30
loc_1_24B30:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_24B35:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_24B49
loc_1_24B49:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_24B4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_24B91
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_24B98
loc_1_24B91:
        mov    dword ptr [ebp - 8], 0
loc_1_24B98:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_24BC8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_24C3A
loc_1_24BC8:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 30h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 34h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_24C3A
loc_1_24C3A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_24C3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_24C78
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_24CD9
loc_1_24C78:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_24CCB
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 38h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 40h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_24CD9
loc_1_24CCB:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 6
        xor    eax, eax
        jmp    NEAR PTR loc_1_24CD9
loc_1_24CD9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_24CDE:
        push   ebp
        mov    ebp, esp
        sub    esp, 60h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_24D3C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_24D43
loc_1_24D3C:
        mov    dword ptr [ebp - 1ch], 0
loc_1_24D43:
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_24D7A
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_24D81
loc_1_24D7A:
        mov    dword ptr [ebp - 4], 0
loc_1_24D81:
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_1_24DCF
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        test   ah, 2
        je     NEAR PTR loc_1_24DC1
        push   18h
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 74h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        jmp    NEAR PTR loc_1_24DC8
loc_1_24DC1:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 3
loc_1_24DC8:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_24DCF:
        call   sub_1_245A7
        mov    dword ptr [ebp - 30h], eax
        cmp    dword ptr [ebp - 30h], 0
        jne    NEAR PTR loc_1_24DEF
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 61h
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_24DEF:
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        call   sub_1_28BA4
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_1_24E22
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 2], 12h
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 8], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_24E22:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 34h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 30h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 1eh], al
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 48h
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 74h
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 80h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 3ch]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 38h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_24EE4
        mov    eax, dword ptr [ebp - 38h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 38h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_24EEB
loc_1_24EE4:
        mov    dword ptr [ebp - 1ch], 0
loc_1_24EEB:
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_24F28
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 8]
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 60h], eax
        jmp    NEAR PTR loc_1_24F2F
loc_1_24F28:
        mov    dword ptr [ebp - 60h], 0
loc_1_24F2F:
        mov    eax, dword ptr [ebp - 60h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 38h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 20h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        test   eax, eax
        jne    NEAR PTR loc_1_24F69
        mov    byte ptr [ebp - 20h], 2
loc_1_24F69:
        mov    al, byte ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        jne    NEAR PTR loc_1_24F8C
        mov    byte ptr [ebp - 10h], 6
loc_1_24F8C:
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 6], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        lea    eax, [ecx + eax*4]
        push   eax
        call   sub_1_05BE
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_24FD8
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        shl    eax, 3
        push   eax
        call   sub_1_05BE
        add    esp, 4
        mov    dword ptr [ebp - 24h], eax
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_1_25012
loc_1_24FD8:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_24FEE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_05F2
        add    esp, 4
loc_1_24FEE:
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 2], 12h
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_25012:
        mov    dword ptr [ebp - 28h], 0
        jmp    NEAR PTR loc_1_25021
loc_1_2501E:
        inc    dword ptr [ebp - 28h]
loc_1_25021:
        xor    eax, eax
        mov    al, byte ptr [ebp - 20h]
        cmp    eax, dword ptr [ebp - 28h]
        jle    NEAR PTR loc_1_25060
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        lea    eax, [ecx + eax*4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax + ecx], 42435053h
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        lea    eax, [ecx + eax*4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx + 17h], 0
        jmp    NEAR PTR loc_1_2501E
loc_1_25060:
        mov    dword ptr [ebp - 28h], 0
        jmp    NEAR PTR loc_1_2506F
loc_1_2506C:
        inc    dword ptr [ebp - 28h]
loc_1_2506F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 28h]
        jle    NEAR PTR loc_1_2508D
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 24h]
        mov    byte ptr [ecx + eax*8 + 6], 0
        jmp    NEAR PTR loc_1_2506C
loc_1_2508D:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 48h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 4ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        push   10h
        lea    eax, [ebp - 44h]
        push   eax
        call   sub_1_0603
        add    esp, 0ch
        mov    dword ptr [ebp - 40h], eax
        cmp    dword ptr [ebp - 44h], 0
        jne    NEAR PTR loc_1_250F3
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 2], 12h
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_250F3:
        mov    eax, dword ptr [ebp - 40h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 44h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_2513F
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_25146
loc_1_2513F:
        mov    dword ptr [ebp - 0ch], 0
loc_1_25146:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, 2
        lea    eax, [eax + eax*4]
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 5ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2520F
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 50h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 14h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_251B2:
        add    esp, 14h
        mov    dword ptr [ebp - 48h], eax
        cmp    dword ptr [ebp - 48h], 0
        je     NEAR PTR loc_1_2520F
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   sub_1_0629
        add    esp, 4
        mov    al, byte ptr [ebp - 48h]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 2], al
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_2520F:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_2526A
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_25271
loc_1_2526A:
        mov    dword ptr [ebp - 4], 0
loc_1_25271:
        push   10h
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_051D
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 16h]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    word ptr [ecx + 16h], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_25303
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   sub_1_0629
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 8], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_25303:
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 4], al
        push   18h
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        push   18h
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 74h
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        mov    edi, dword ptr [ebp - 34h]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 23h
        rep movsd
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 38h
        mov    dword ptr [ebp - 54h], eax
        mov    eax, dword ptr [ebp - 54h]
        mov    ecx, dword ptr [ebp - 38h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 1], 2dh
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 3], 90h
        mov    eax, dword ptr [ebp - 54h]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 54h]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [ebp - 54h]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 54h]
        mov    dword ptr [eax + 0ch], 1
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 38h
        mov    dword ptr [ebp - 58h], eax
        mov    eax, dword ptr [ebp - 58h]
        mov    ecx, dword ptr [ebp - 38h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 1], 30h
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 58h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [eax + 8], 10000h
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [eax + 0ch], 4000h
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_2540A
        mov    eax, dword ptr [ebp - 58h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 20h], eax
        jmp    NEAR PTR loc_1_25469
loc_1_2540A:
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   sub_1_0629
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 48h]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 4ch]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 34h]
        mov    byte ptr [eax + 1], 4
        mov    eax, dword ptr [ebp - 34h]
        mov    byte ptr [eax + 3], 91h
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 46h
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_25469:
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 38h
        mov    dword ptr [ebp - 5ch], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 1], 31h
        mov    eax, dword ptr [ebp - 5ch]
        mov    ecx, dword ptr [ebp - 38h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 5ch]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 5ch]
        mov    byte ptr [eax + 2], 2
        mov    eax, dword ptr [ebp - 38h]
        mov    byte ptr [eax + 3], 91h
        mov    eax, dword ptr [ebp - 5ch]
        mov    byte ptr [eax + 3], 7fh
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_2552C
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   sub_1_0629
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 48h]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 4ch]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 34h]
        mov    byte ptr [eax + 1], 4
        mov    eax, dword ptr [ebp - 34h]
        mov    byte ptr [eax + 3], 90h
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 46h
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_2552C:
        mov    eax, dword ptr [ebp - 34h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 44h], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_2553C
loc_1_2553C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25541:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2557A
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_255D3
loc_1_2557A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 5eh
        jne    NEAR PTR loc_1_255C9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 5eh
loc_1_255C9:
        mov    eax, 1
        jmp    NEAR PTR loc_1_255D3
loc_1_255D3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_255D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_255E1:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_2560B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp + 8], eax
        jmp    NEAR PTR loc_1_255E1
loc_1_2560B:
        jmp    NEAR PTR loc_1_25610
loc_1_25610:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_25615:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 24h], 0
        je     NEAR PTR loc_1_25652
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   sub_1_0629
        add    esp, 4
loc_1_25652:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 48h]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4ch]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_256F0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
loc_1_25692:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_256C7
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_256B9
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 0bh
loc_1_256B9:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_25692
loc_1_256C7:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_256E6
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_255D8
        add    esp, 4
loc_1_256E6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
loc_1_256F0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_256F7
loc_1_256F7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_256FC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 1
        xor    eax, eax
        jmp    NEAR PTR loc_1_25710
loc_1_25710:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25715:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2574E
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_25776
loc_1_2574E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_25776
loc_1_25776:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2577B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_257B4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_257DC
loc_1_257B4:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_257DC
loc_1_257DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_257E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_2581A
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_25916
loc_1_2581A:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_2587E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_25885
loc_1_2587E:
        mov    dword ptr [ebp - 4], 0
loc_1_25885:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_258DF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_258E9
loc_1_258DF:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4], 0
loc_1_258E9:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 8], ax
        xor    eax, eax
        jmp    NEAR PTR loc_1_25916
loc_1_25916:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2591B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_25924:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_25979
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_2595F
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
        jmp    NEAR PTR loc_1_25966
loc_1_2595F:
        mov    dword ptr [ebp - 4], 0
loc_1_25966:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp + 0ch], eax
        jmp    NEAR PTR loc_1_25924
loc_1_25979:
        xor    eax, eax
        jmp    NEAR PTR loc_1_25980
loc_1_25980:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_25985:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        and    dword ptr [ebp + 8], 0fffffh
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffff0h
        mov    dword ptr [ebp - 4], eax
        shl    dword ptr [ebp - 4], 0ch
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_259B5
loc_1_259B5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_259BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_259C3:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_259ED
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_25985
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_259C3
loc_1_259ED:
        xor    eax, eax
        jmp    NEAR PTR loc_1_259F4
loc_1_259F4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_25A4E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_25A55
loc_1_25A4E:
        mov    dword ptr [ebp - 8], 0
loc_1_25A55:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_25A7D
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_259BA
        add    esp, 4
loc_1_25A7D:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_25AFD
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_259BA
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        push   eax
        call   sub_1_24A73
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_25AFD
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_25AFD
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_25AFA:
        add    esp, 10h
loc_1_25AFD:
        xor    eax, eax
        jmp    NEAR PTR loc_1_25B04
loc_1_25B04:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_25B09:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_25B42
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_25D9D
loc_1_25B42:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 40h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_25BAA
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_25BB1
loc_1_25BAA:
        mov    dword ptr [ebp - 8], 0
loc_1_25BB1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_25C1B
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_25C25
loc_1_25C1B:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
loc_1_25C25:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_25C62
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_25C6C
loc_1_25C62:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
loc_1_25C6C:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_25CC5
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        jmp    NEAR PTR loc_1_25CCF
loc_1_25CC5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
loc_1_25CCF:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_25D0C
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 14h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_25D16
loc_1_25D0C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
loc_1_25D16:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_25D43
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_2591B
        add    esp, 8
loc_1_25D43:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_25D69
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_2591B
        add    esp, 8
loc_1_25D69:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 5eh
        jne    NEAR PTR loc_1_25D93
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 5eh
loc_1_25D93:
        mov    eax, 1
        jmp    NEAR PTR loc_1_25D9D
loc_1_25D9D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25DA2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25B09
        add    esp, 4
        jmp    NEAR PTR loc_1_25DB9
loc_1_25DB9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25DBE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25B09
        add    esp, 4
        jmp    NEAR PTR loc_1_25DD5
loc_1_25DD5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25DDA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25B09
        add    esp, 4
        jmp    NEAR PTR loc_1_25DF1
loc_1_25DF1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25DF6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25B09
        add    esp, 4
        jmp    NEAR PTR loc_1_25E0D
loc_1_25E0D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25E12:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25B09
        add    esp, 4
        jmp    NEAR PTR loc_1_25E29
loc_1_25E29:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25E2E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25B09
        add    esp, 4
        jmp    NEAR PTR loc_1_25E45
loc_1_25E45:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_25E4A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25B09
        add    esp, 4
        jmp    NEAR PTR loc_1_25E61
loc_1_25E61:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_25E66:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_25E89
        xor    eax, eax
        jmp    NEAR PTR loc_1_26074
loc_1_25E89:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 18h], al
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 14h], al
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    byte ptr [ebp - 0ch], ah
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_25F59
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_1_2473D
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 17h], 0
        mov    dword ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_25F06
loc_1_25EFF:
        inc    dword ptr [ebp - 20h]
        add    dword ptr [ebp - 1ch], 8
loc_1_25F06:
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, dword ptr [ebp - 20h]
        jle    NEAR PTR loc_1_25F54
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_25F4F
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_25F4F
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 6], 0
loc_1_25F4F:
        jmp    NEAR PTR loc_1_25EFF
loc_1_25F54:
        jmp    NEAR PTR loc_1_25FD9
loc_1_25F59:
        mov    dword ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_25F6C
loc_1_25F65:
        inc    dword ptr [ebp - 20h]
        add    dword ptr [ebp - 8], 1dh
loc_1_25F6C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        cmp    eax, dword ptr [ebp - 20h]
        jle    NEAR PTR loc_1_25F99
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_25F94
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 17h], 0
loc_1_25F94:
        jmp    NEAR PTR loc_1_25F65
loc_1_25F99:
        mov    dword ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_25FAC
loc_1_25FA5:
        inc    dword ptr [ebp - 20h]
        add    dword ptr [ebp - 1ch], 8
loc_1_25FAC:
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, dword ptr [ebp - 20h]
        jle    NEAR PTR loc_1_25FD9
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_25FD4
        mov    dword ptr [ebp - 1ch], 0
loc_1_25FD4:
        jmp    NEAR PTR loc_1_25FA5
loc_1_25FD9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 24h], eax
loc_1_25FE8:
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_1_26032
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_26024
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 24h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 0bh
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 4], eax
loc_1_26024:
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_25FE8
loc_1_26032:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 24h], eax
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_1_2606D
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_255D8
        add    esp, 4
loc_1_2606D:
        xor    eax, eax
        jmp    NEAR PTR loc_1_26074
loc_1_26074:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26079:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_260B2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_2610B
loc_1_260B2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 5eh
        jne    NEAR PTR loc_1_26101
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 5eh
loc_1_26101:
        mov    eax, 1
        jmp    NEAR PTR loc_1_2610B
loc_1_2610B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26110:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_26158
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_262E8
loc_1_26158:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_24666
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_26188
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 2], 46h
        xor    eax, eax
        jmp    NEAR PTR loc_1_262E8
loc_1_26188:
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_261C2
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_261C9
loc_1_261C2:
        mov    dword ptr [ebp - 10h], 0
loc_1_261C9:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2622A
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 1eh]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 20h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2620A:
        add    esp, 14h
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_2622A
        mov    al, byte ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 2], al
        xor    eax, eax
        jmp    NEAR PTR loc_1_262E8
loc_1_2622A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 10h]
        mov    ecx, 9
        rep movsd
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 13h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_262D1
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 16h], ah
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_262E1
loc_1_262D1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_24710
        add    esp, 8
loc_1_262E1:
        xor    eax, eax
        jmp    NEAR PTR loc_1_262E8
loc_1_262E8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_262ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 4], al
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_26317
loc_1_26314:
        inc    dword ptr [ebp - 0ch]
loc_1_26317:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jle    NEAR PTR loc_1_26363
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2635E
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_2635E
        jmp    NEAR PTR loc_1_26363
loc_1_2635E:
        jmp    NEAR PTR loc_1_26314
loc_1_26363:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0ch]
        jg     NEAR PTR loc_1_2637E
        xor    ax, ax
        jmp    NEAR PTR loc_1_2638A
        db  0E9h, 00Ch, 000h, 000h, 000h
loc_1_2637E:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        jmp    NEAR PTR loc_1_2638A
loc_1_2638A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2638F:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_263C8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26557
loc_1_263C8:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2473D
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2641D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 43h
        xor    eax, eax
        jmp    NEAR PTR loc_1_26557
loc_1_2641D:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_262ED
        add    esp, 8
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2646A
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 47h
        xor    eax, eax
        jmp    NEAR PTR loc_1_26557
loc_1_2646A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_26550
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], eax
loc_1_264A4:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_264FB
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        cmp    eax, 28h
        jne    NEAR PTR loc_1_264ED
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 0ah
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [eax + 4], 0
        jmp    NEAR PTR loc_1_264FB
loc_1_264ED:
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_264A4
loc_1_264FB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_2651A
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_255D8
        add    esp, 4
loc_1_2651A:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_26540
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 5ch], 0
loc_1_26540:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_24710
        add    esp, 8
loc_1_26550:
        xor    eax, eax
        jmp    NEAR PTR loc_1_26557
loc_1_26557:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2655C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_26595
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26744
loc_1_26595:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_26601
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_26608
loc_1_26601:
        mov    dword ptr [ebp - 8], 0
loc_1_26608:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_26645
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx + 3]
        and    ecx, 0ffh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_26653
loc_1_26645:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        add    dword ptr [ebp - 0ch], eax
loc_1_26653:
        cmp    dword ptr [ebp - 0ch], 0ffh
        jg     NEAR PTR loc_1_2666A
        cmp    dword ptr [ebp - 0ch], 1
        jge    NEAR PTR loc_1_26678
loc_1_2666A:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 58h
        xor    eax, eax
        jmp    NEAR PTR loc_1_26744
loc_1_26678:
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 3
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 4ch
        push   eax
        call   sub_1_05CF
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_26736
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 6], al
        push   6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_266D8
loc_1_266D5:
        inc    dword ptr [ebp - 10h]
loc_1_266D8:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 10h], eax
        jge    NEAR PTR loc_1_26731
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + ecx*8 + 6]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2672C
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + ecx*8], 0
        je     NEAR PTR loc_1_2672C
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [eax + ecx*8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx + 4ch]
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + edx*8]
        mov    dword ptr [ecx + 4ch], eax
loc_1_2672C:
        jmp    NEAR PTR loc_1_266D5
loc_1_26731:
        jmp    NEAR PTR loc_1_2673D
loc_1_26736:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 57h
loc_1_2673D:
        xor    eax, eax
        jmp    NEAR PTR loc_1_26744
loc_1_26744:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26749:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_26782
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_268B1
loc_1_26782:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_247D0
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_267B2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 46h
        xor    eax, eax
        jmp    NEAR PTR loc_1_268B1
loc_1_267B2:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_267EF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_267F6
loc_1_267EF:
        mov    dword ptr [ebp - 8], 0
loc_1_267F6:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, 5
        rep movsd
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_26852
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        jmp    NEAR PTR loc_1_2685C
loc_1_26852:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
loc_1_2685C:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_268AA
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 6], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 2], ax
loc_1_268AA:
        xor    eax, eax
        jmp    NEAR PTR loc_1_268B1
loc_1_268B1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_268B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_268F1
        xor    eax, eax
        jmp    NEAR PTR loc_1_269BB
loc_1_268F1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_24863
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_26919
        xor    eax, eax
        jmp    NEAR PTR loc_1_269BB
loc_1_26919:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
loc_1_26928:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_26972
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_26964
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 24h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 0bh
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 4], eax
loc_1_26964:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_26928
loc_1_26972:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_269AD
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_255D8
        add    esp, 4
loc_1_269AD:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 6], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_269BB
loc_1_269BB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_269C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_269F9
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26A74
loc_1_269F9:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_24863
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_26A2A
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 7], 1
loc_1_26A2A:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 5eh
        jne    NEAR PTR loc_1_26A6A
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 5eh
loc_1_26A6A:
        mov    eax, 1
        jmp    NEAR PTR loc_1_26A74
loc_1_26A74:
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
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26A92
loc_1_26A92:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26A97:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_26AD0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26BC9
loc_1_26AD0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_26B13
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_26B1A
loc_1_26B13:
        mov    dword ptr [ebp - 0ch], 0
loc_1_26B1A:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_26B8B
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_26B95
loc_1_26B8B:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4], 0
loc_1_26B95:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 5eh
        jne    NEAR PTR loc_1_26BBF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 5eh
loc_1_26BBF:
        mov    eax, 1
        jmp    NEAR PTR loc_1_26BC9
loc_1_26BC9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26BCE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_26C07
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26CD6
loc_1_26C07:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_26C56
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_26C5D
loc_1_26C56:
        mov    dword ptr [ebp - 0ch], 0
loc_1_26C5D:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 0ch]
        mov    ecx, 6
        rep movsd
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_26CB9
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_26CC3
loc_1_26CB9:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
loc_1_26CC3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26CD6
loc_1_26CD6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26CDB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_26D14
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26D49
loc_1_26D14:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_26D49
loc_1_26D49:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26D4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_26D87
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26E5F
loc_1_26D87:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_26DD6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_26DDD
loc_1_26DD6:
        mov    dword ptr [ebp - 4], 0
loc_1_26DDD:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_26E42
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_26E4C
loc_1_26E42:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4], 0
loc_1_26E4C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26E5F
loc_1_26E5F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26E64:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_26EAD
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_26EB4
loc_1_26EAD:
        mov    dword ptr [ebp - 4], 0
loc_1_26EB4:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_26ED0
loc_1_26ED0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_26ED5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_26F21
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_26F28
loc_1_26F21:
        mov    dword ptr [ebp - 8], 0
loc_1_26F28:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_26F71
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
loc_1_26F71:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_26FA4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 0ch
        jne    NEAR PTR loc_1_2707F
loc_1_26FA4:
        push   36h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_051D
        add    esp, 8
        push   6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        push   6
        mov    eax, dword ptr [ebp - 4]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 6
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        push   4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        push   4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 10h
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 15h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 15h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 16h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 16h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 17h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 17h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 18h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 19h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 19h], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 2ch], 40006ch
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, 6
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 28h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 24h], eax
        jmp    NEAR PTR loc_1_2707F
loc_1_2707F:
        xor    eax, eax
        jmp    NEAR PTR loc_1_27086
loc_1_27086:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2708B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_270C4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_2712C
loc_1_270C4:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 64h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 64h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 5eh
        jne    NEAR PTR loc_1_27122
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 5eh
loc_1_27122:
        mov    eax, 1
        jmp    NEAR PTR loc_1_2712C
loc_1_2712C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_27131:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 64h], 0
        jne    NEAR PTR loc_1_2714E
        xor    eax, eax
        jmp    NEAR PTR loc_1_271D1
loc_1_2714E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 64h]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 20h], ecx
        jne    NEAR PTR loc_1_27180
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 64h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 64h]
        mov    eax, dword ptr [eax + 38h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 64h], eax
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_271D1
loc_1_27180:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 64h]
        mov    dword ptr [ebp - 4], eax
loc_1_27189:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_271CA
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 20h], ecx
        jne    NEAR PTR loc_1_271BF
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 38h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_271D1
loc_1_271BF:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_27189
loc_1_271CA:
        xor    eax, eax
        jmp    NEAR PTR loc_1_271D1
loc_1_271D1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_271D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_2720F
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_27295
loc_1_2720F:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_27131
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_27287
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_27282
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 2], 0ah
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
loc_1_27282:
        jmp    NEAR PTR loc_1_2728E
loc_1_27287:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 11h
loc_1_2728E:
        xor    eax, eax
        jmp    NEAR PTR loc_1_27295
loc_1_27295:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2729A:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_272D3
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_273A5
loc_1_272D3:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_27316
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2731D
loc_1_27316:
        mov    dword ptr [ebp - 8], 0
loc_1_2731D:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    byte ptr [ebp - 10h], ah
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_24A73
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_27360
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 43h
        xor    eax, eax
        jmp    NEAR PTR loc_1_273A5
loc_1_27360:
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2738F:
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        xor    eax, eax
        jmp    NEAR PTR loc_1_273A5
loc_1_273A5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_273AA:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_273E3
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_27550
loc_1_273E3:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_2742F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_27436
loc_1_2742F:
        mov    dword ptr [ebp - 10h], 0
loc_1_27436:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        mov    dword ptr [ebp - 18h], eax
        push   14h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_051D
        add    esp, 8
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        push   0dh
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 3
        jge    NEAR PTR loc_1_274DE
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [edx + ecx*4 + 50h], eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        shl    eax, 2
        add    eax, dword ptr [ebp - 1ch]
        add    eax, 50h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4ch], eax
        jmp    NEAR PTR loc_1_27546
loc_1_274DE:
        cmp    byte ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_27511
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_24863
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4ch], eax
        jmp    NEAR PTR loc_1_27546
loc_1_27511:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_2473D
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4ch], eax
loc_1_27546:
        mov    eax, 1
        jmp    NEAR PTR loc_1_27550
loc_1_27550:
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
        and    dword ptr [ebp + 8], 0fffffh
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fffffff0h
        shl    eax, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0fh
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_27582
loc_1_27582:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], edx
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        call   sub_1_0300
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 38h], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0312
        add    esp, 4
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        call   sub_1_0349
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        jmp    NEAR PTR loc_1_27638
loc_1_27638:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2763D:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 8], eax
        call   sub_1_0300
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 38h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0312
        add    esp, 4
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        call   sub_1_0349
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        jmp    NEAR PTR loc_1_276EE
loc_1_276EE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_276F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        call   sub_1_0300
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 68h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 38h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 64h]
        push   eax
        call   sub_1_0312
        add    esp, 4
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        call   sub_1_0349
        jmp    NEAR PTR loc_1_27796
loc_1_27796:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2779B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_0300
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 70h], 0
        je     NEAR PTR loc_1_277E9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 70h]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 70h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_277F0
loc_1_277E9:
        mov    dword ptr [ebp - 4], 0
loc_1_277F0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 70h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 70h]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 38h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 56h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 6ch]
        push   eax
        call   sub_1_0312
        add    esp, 4
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        call   sub_1_0349
        jmp    NEAR PTR loc_1_2786B
loc_1_2786B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_27870:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 28h], ecx
        je     NEAR PTR loc_1_278A7
        xor    eax, eax
        jmp    NEAR PTR loc_1_27A30
loc_1_278A7:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_278D6
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27A30
loc_1_278D6:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 70h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_2473D
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_2791C
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27A30
loc_1_2791C:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_2793C
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27A30
        db  0E9h, 009h, 000h, 000h, 000h
loc_1_2793C:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 6ch], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_27979
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_27980
loc_1_27979:
        mov    dword ptr [ebp - 4], 0
loc_1_27980:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_24863
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   ah, 4
        je     NEAR PTR loc_1_279DC
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_247D0
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_279C5
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27A30
loc_1_279C5:
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 6], 1
        mov    ax, word ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 4], ax
        jmp    NEAR PTR loc_1_27A08
loc_1_279DC:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_27A08
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_27A08
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27A30
loc_1_27A08:
        push   14h
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0558
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2779B
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_27A30
loc_1_27A30:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_0300
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 1ch], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        push   eax
        call   sub_1_2473D
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_27AC4
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 18h], 0
        jne    NEAR PTR loc_1_27AC9
loc_1_27AC4:
        jmp    NEAR PTR loc_1_27AE8
loc_1_27AC9:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        call   sub_1_0312
        add    esp, 4
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        call   sub_1_0349
        jmp    NEAR PTR loc_1_27AE8
loc_1_27AE8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_27AED:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 20h], ax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        push   eax
        call   sub_1_248FB
        add    esp, 8
        mov    dword ptr [ebp - 30h], eax
        cmp    dword ptr [ebp - 30h], 0
        jne    NEAR PTR loc_1_27B3D
        xor    eax, eax
        jmp    NEAR PTR loc_1_27F03
loc_1_27B3D:
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 24h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        push   eax
        call   sub_1_24A73
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
        call   sub_1_28BA4
        mov    dword ptr [ebp - 3ch], eax
        cmp    dword ptr [ebp - 3ch], 0
        jne    NEAR PTR loc_1_27BC5
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_27BBE
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27F03
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_27BBE:
        xor    eax, eax
        jmp    NEAR PTR loc_1_27F03
loc_1_27BC5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_27C0E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_27C15
loc_1_27C0E:
        mov    dword ptr [ebp - 0ch], 0
loc_1_27C15:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_27C32:
        add    esp, 14h
        mov    word ptr [ebp - 38h], ax
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_27C58:
        add    esp, 1ch
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 38h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_27D82
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_27D23
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_27CB2
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27F03
loc_1_27CB2:
        mov    edi, dword ptr [ebp - 3ch]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 23h
        rep movsd
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    byte ptr [eax + 1], 1dh
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp - 3ch]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_27F03
        db  0E9h, 05Fh, 000h, 000h, 000h
loc_1_27D23:
        mov    edi, dword ptr [ebp - 3ch]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 23h
        rep movsd
        mov    eax, dword ptr [ebp - 3ch]
        mov    byte ptr [eax + 1], 27h
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 3ch]
        add    eax, 38h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_27F03
loc_1_27D82:
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0c0h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    ecx, dword ptr [ebp - 24h]
        mov    cl, byte ptr [ecx + 0eh]
        and    ecx, 20h
        shr    ecx, 4
        or     eax, ecx
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_27DC5:
        add    esp, 18h
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 64h], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 68h], eax
        cmp    dword ptr [ebp - 2ch], 0
        je     NEAR PTR loc_1_27E0B
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp - 2ch]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 44h], eax
        jmp    NEAR PTR loc_1_27E12
loc_1_27E0B:
        mov    dword ptr [ebp - 44h], 0
loc_1_27E12:
        mov    ax, word ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 44h]
        mov    word ptr [ecx + 10h], ax
        mov    edi, dword ptr [ebp - 3ch]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 23h
        rep movsd
        mov    eax, dword ptr [ebp - 3ch]
        mov    byte ptr [eax + 1], 27h
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 3ch]
        add    eax, 38h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_27EE4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    byte ptr [eax + 1], 1dh
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp - 3ch]
        mov    word ptr [eax + 0eh], 80h
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp - 3ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_276F3
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_27F03
loc_1_27EE4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_276F3
        add    esp, 4
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_27F03
loc_1_27F03:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_27F08:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_27F9F
loc_1_27F2D:
        mov    word ptr [ebp - 4], 8000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_27FC6
loc_1_27F44:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 24h]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_27FC6
loc_1_27F60:
        mov    word ptr [ebp - 4], 8000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_27FC6
loc_1_27F77:
        mov    word ptr [ebp - 4], 8000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_27FC6
loc_1_27F8E:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_27FC6
        db  0E9h, 027h, 000h, 000h, 000h
loc_1_27F9F:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 3
        ja     NEAR PTR loc_1_27F8E
        mov    eax, dword ptr [ebp - 14h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_27FC6:
        call   sub_1_0300
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 18h], ax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_2800F
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0312
        add    esp, 4
loc_1_2800F:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        call   sub_1_0349
        jmp    NEAR PTR loc_1_2801F
loc_1_2801F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_28024:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
loc_1_28030:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 68h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_28327
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 68h]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 68h], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 68h], 0
        jne    NEAR PTR loc_1_28069
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 6ch], 0
loc_1_28069:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_1_28087
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        jmp    NEAR PTR loc_1_28030
loc_1_28087:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_28294
loc_1_280A3:
        jmp    NEAR PTR loc_1_28316
loc_1_280A8:
        jmp    NEAR PTR loc_1_28316
loc_1_280AD:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_27F08
        add    esp, 4
        jmp    NEAR PTR loc_1_28316
loc_1_280BE:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_27870
        add    esp, 4
        jmp    NEAR PTR loc_1_28316
loc_1_280CF:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_27AED
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_28120
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 68h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 28h]
        mov    dword ptr [ecx + 68h], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 6ch], 0
        jne    NEAR PTR loc_1_28111
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 6ch], eax
loc_1_28111:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 70h], 0
        jmp    NEAR PTR loc_1_28336
loc_1_28120:
        jmp    NEAR PTR loc_1_28316
loc_1_28125:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_2814F
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2763D
        add    esp, 4
        jmp    NEAR PTR loc_1_2816A
loc_1_2814F:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 24h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
loc_1_2816A:
        jmp    NEAR PTR loc_1_28316
loc_1_2816F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_281E0
loc_1_2818B:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 20h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_27131
        add    esp, 8
        jmp    NEAR PTR loc_1_28236
loc_1_281A3:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_25E66
        add    esp, 4
        jmp    NEAR PTR loc_1_28236
loc_1_281B4:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_268B6
        add    esp, 4
        jmp    NEAR PTR loc_1_28236
loc_1_281C5:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_25615
        add    esp, 4
        jmp    NEAR PTR loc_1_28236
loc_1_281D6:
        jmp    NEAR PTR loc_1_28236
        db  0E9h, 056h, 000h, 000h, 000h
loc_1_281E0:
        sub    dword ptr [ebp - 18h], 4
        cmp    dword ptr [ebp - 18h], 1eh
        ja     NEAR PTR loc_1_28236
        mov    eax, dword ptr [ebp - 18h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 001h, 004h, 004h, 004h, 004h, 004h, 002h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 003h
loc_1_28236:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_2826A
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 10h
        jne    NEAR PTR loc_1_2826A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2763D
        add    esp, 4
        jmp    NEAR PTR loc_1_28285
loc_1_2826A:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 24h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
loc_1_28285:
        jmp    NEAR PTR loc_1_28316
loc_1_2828A:
        jmp    NEAR PTR loc_1_28316
        db  0E9h, 082h, 000h, 000h, 000h
loc_1_28294:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 3fh
        ja     NEAR PTR loc_1_28316
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 007h, 002h, 007h, 007h, 007h, 003h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 004h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 005h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 006h
loc_1_28316:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        jmp    NEAR PTR loc_1_28030
loc_1_28327:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 70h], 0
        jmp    NEAR PTR loc_1_28336
loc_1_28336:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2833B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 68h], 0
        jne    NEAR PTR loc_1_2837B
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 68h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 6ch], eax
        jmp    NEAR PTR loc_1_28390
loc_1_2837B:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 6ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 6ch], eax
loc_1_28390:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 70h], 0
        jne    NEAR PTR loc_1_283BB
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0407
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 70h], eax
loc_1_283BB:
        jmp    NEAR PTR loc_1_283C0
loc_1_283C0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_283C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2833B
        add    esp, 4
        call   sub_1_28BA4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_28405
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [eax + 44h], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_28447
        db  0E9h, 00Ch, 000h, 000h, 000h
loc_1_28405:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 28h]
        mov    dword ptr [ecx + 44h], eax
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 23h
        rep movsd
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 38h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_28447
loc_1_28447:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2844C:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_28498
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, 0ffff0fffh
        shr    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_2849F
loc_1_28498:
        mov    dword ptr [ebp - 14h], 0
loc_1_2849F:
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_284D9
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_285D3
loc_1_284D9:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_28546
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_2473D
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_28538
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 43h
        xor    eax, eax
        jmp    NEAR PTR loc_1_285D3
loc_1_28538:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_2854F
loc_1_28546:
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 14h
        mov    dword ptr [ebp - 0ch], eax
loc_1_2854F:
        lea    eax, [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2856D:
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [eax + 68h], 0
        je     NEAR PTR loc_1_285AB
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [eax + 70h], 0
        jne    NEAR PTR loc_1_285AB
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0407
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 70h], eax
loc_1_285AB:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 8], 0
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        xor    eax, eax
        jmp    NEAR PTR loc_1_285D3
loc_1_285D3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_285D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_28611
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_28677
loc_1_28611:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_28670
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_28670
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
loc_1_28670:
        xor    eax, eax
        jmp    NEAR PTR loc_1_28677
loc_1_28677:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2867C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 1
        xor    eax, eax
        jmp    NEAR PTR loc_1_28690
loc_1_28690:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_28695:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 1
        xor    eax, eax
        jmp    NEAR PTR loc_1_286A9
loc_1_286A9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_286AE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        call   sub_1_244FA
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_286E5
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 4
        jmp    NEAR PTR loc_1_2876B
loc_1_286E5:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_28732
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_28739
loc_1_28732:
        mov    dword ptr [ebp - 4], 0
loc_1_28739:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2876B
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 34h], eax
        jmp    NEAR PTR loc_1_2876B
loc_1_2876B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_28779
loc_1_28779:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2877E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_287A8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_283C5
        add    esp, 4
        jmp    NEAR PTR loc_1_287D3
loc_1_287A8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 44h], ecx
        jne    NEAR PTR loc_1_287C7
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [eax + 44h], 0
loc_1_287C7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
loc_1_287D3:
        jmp    NEAR PTR loc_1_287D8
loc_1_287D8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_287DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 10h
        shl    eax, 4
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 2
        jl     NEAR PTR loc_1_28828
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 2], 1dh
        jmp    NEAR PTR loc_1_2895A
loc_1_28828:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 2eh
        jbe    NEAR PTR loc_1_28842
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 2], 1
        jmp    NEAR PTR loc_1_2895A
loc_1_28842:
        call   sub_1_28BA4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_28860
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 2], 12h
        jmp    NEAR PTR loc_1_2895A
loc_1_28860:
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 2], 0ffh
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 24h], eax
        call   sub_1_05B3
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 34h], eax
        call   sub_1_05AC
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 30h], eax
        call   sub_1_05A1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 2ch], eax
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_288D1
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 24h], eax
        jmp    NEAR PTR loc_1_288DB
loc_1_288D1:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 24h], 0
loc_1_288DB:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 3], 80h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        db  0FFh, 014h, 085h, 000h, 000h, 000h, 000h
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_28955
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        jmp    NEAR PTR loc_1_2895A
loc_1_28955:
        jmp    NEAR PTR loc_1_2898E
loc_1_2895A:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_28989
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_28989
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0430
        add    esp, 4
loc_1_28989:
        jmp    NEAR PTR loc_1_2898E
loc_1_2898E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28993:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   8c4h
        call   sub_1_05BE
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_28A13
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_289E7
loc_1_289DD:
        inc    dword ptr [ebp - 8]
        add    dword ptr [ebp - 4], 8ch
loc_1_289E7:
        cmp    dword ptr [ebp - 8], 10h
        jge    NEAR PTR loc_1_28A0C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 5a424243h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
        jmp    NEAR PTR loc_1_289DD
loc_1_28A0C:
        db  083h, 005h, 000h, 000h, 000h, 000h, 010h
loc_1_28A13:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_28A25
loc_1_28A25:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28A2A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  00Fh, 0BFh, 005h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        add    eax, eax
        push   eax
        call   sub_1_05BE
        add    esp, 4
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_28A6B
        mov    eax, 12h
        jmp    NEAR PTR loc_1_28AFC
loc_1_28A6B:
        call   sub_1_28993
        test   eax, eax
        jne    NEAR PTR loc_1_28A90
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_05F2
        add    esp, 4
        mov    eax, 12h
        jmp    NEAR PTR loc_1_28AFC
loc_1_28A90:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_28A9F
loc_1_28A9C:
        inc    dword ptr [ebp - 4]
loc_1_28A9F:
        db  00Fh, 0BFh, 005h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 4]
        jle    NEAR PTR loc_1_28AE9
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + eax*2 + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + eax*2], 42434f56h
        jmp    NEAR PTR loc_1_28A9C
loc_1_28AE9:
        db  00Fh, 0BFh, 005h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_28AFC
loc_1_28AFC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28B01:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_05F2
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_28B2D
loc_1_28B25:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
loc_1_28B2D:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_28B52
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_05F2
        add    esp, 4
        jmp    NEAR PTR loc_1_28B25
loc_1_28B52:
        jmp    NEAR PTR loc_1_28B57
loc_1_28B57:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28B5C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 18h], 42435246h
        jne    NEAR PTR loc_1_28B77
        jmp    NEAR PTR loc_1_28B9F
loc_1_28B77:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 18h], 42435246h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_28B9F
loc_1_28B9F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28BA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_28BC1
        xor    eax, eax
        jmp    NEAR PTR loc_1_28C23
loc_1_28BC1:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 1ch]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 004h
        jg     NEAR PTR loc_1_28C00
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_28C00
        call   sub_1_064B
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_28C00:
        push   8ch
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_051D
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 5a424243h
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_28C23
loc_1_28C23:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28C28:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_28C4C
        jmp    NEAR PTR loc_1_28C9F
loc_1_28C4C:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_28C5B
loc_1_28C58:
        inc    dword ptr [ebp - 4]
loc_1_28C5B:
        cmp    dword ptr [ebp - 4], 0ffffffh
        jge    NEAR PTR loc_1_28C9A
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_28C83
        jmp    NEAR PTR loc_1_28C9F
loc_1_28C83:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_28C58
loc_1_28C9A:
        jmp    NEAR PTR loc_1_28C9F
loc_1_28C9F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_28CA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_28CBC
loc_1_28CB9:
        inc    dword ptr [ebp - 8]
loc_1_28CBC:
        mov    eax, dword ptr [ebp - 8]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jle    NEAR PTR loc_1_28E80
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + eax*2 + 8], edx
        jne    NEAR PTR loc_1_28E7B
        call   sub_1_28BA4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_28D1D
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + eax*2 + 8], 0
        jmp    NEAR PTR loc_1_28E85
loc_1_28D1D:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*2 + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*2 + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*2 + 1eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 3], 90h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        shl    eax, 4
        sub    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*2 + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 14h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28C28
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_28E4A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_25E66
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_244A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28C28
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_28E4A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_25615
        add    esp, 4
loc_1_28E4A:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_28E6F
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [eax + 8], 0
        jmp    NEAR PTR loc_1_28E7B
loc_1_28E6F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28B5C
        add    esp, 4
loc_1_28E7B:
        jmp    NEAR PTR loc_1_28CB9
loc_1_28E80:
        jmp    NEAR PTR loc_1_28E85
loc_1_28E85:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_28E8C:
        db  0CDh, 020h, 029h, 080h, 028h, 000h
sub_1_28E92:
        db  0CDh, 020h, 02Ah, 080h, 028h, 000h
sub_1_28E98:
        db  0CDh, 020h, 02Bh, 080h, 028h, 000h
sub_1_28E9E:
        db  0CDh, 020h, 02Ch, 080h, 028h, 000h
sub_1_28EA4:
        db  0CDh, 020h, 02Dh, 080h, 028h, 000h
sub_1_28EAA:
        db  0CDh, 020h, 02Eh, 080h, 028h, 000h
sub_1_28EB0:
        db  0CDh, 020h, 02Fh, 080h, 028h, 000h
sub_1_28EB6:
        db  0CDh, 020h, 030h, 080h, 028h, 000h
        db  0CDh, 020h, 050h, 080h, 028h, 000h
sub_1_28EC2:
        db  0CDh, 020h, 051h, 080h, 028h, 000h
sub_1_28EC8:
        db  0CDh, 020h, 052h, 080h, 028h, 000h
        db  0CDh, 020h, 053h, 080h, 028h, 000h
        db  0CDh, 020h, 000h, 080h, 028h, 000h
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_28EFE:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_28F04:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
sub_1_28F0A:
        db  0CDh, 020h, 016h, 080h, 028h, 000h
sub_1_28F10:
        db  0CDh, 020h, 017h, 080h, 028h, 000h
sub_1_28F16:
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
sub_1_28F22:
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_28F28:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_28F2E:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_28F34:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_28F3A:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_28F46:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_28F4C:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
sub_1_28F5E:
        db  0CDh, 020h, 024h, 080h, 028h, 000h
sub_1_28F64:
        db  0CDh, 020h, 025h, 080h, 028h, 000h
sub_1_28F6A:
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_28F70:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_28F82:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_28F88:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 044h, 04Ch, 043h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 084h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2903C:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
        insw
        db  067h, 072h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [edx + 55h], al
        inc    esi
        cmp    ah, byte ptr [eax]
        inc    ebx
        popad
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   eax, dword ptr [esp + ebp*2 + 63h], 42h
        jne    SHORT loc_1_290C9
        push   eax
        outsd
        outsd
        insb
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   eax, dword ptr [ebx + 6eh], 74h
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_2907C:
        inc    edx
        push   ebp
        inc    esi
        cmp    ah, byte ptr [eax]
        inc    ebx
        push   446b6365h
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        push   ebx
        imul   edi, dword ptr [edx + 65h], 6c447020h
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_1_29106
        imul   ebp, dword ptr [ebp + 65h], 2064253dh
        popad
        db  076h, 067h
        dec    esp
        popad
        db  073h, 074h
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 2064253dh
        popad
loc_1_290C9:
        db  076h, 067h
        push   ebx
        db  074h, 061h
        db  072h, 074h
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 0a64253dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_290DC:
        inc    edx
        push   ebp
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_1_2912A
        insb
        je     SHORT loc_1_2914B
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 2064253dh
        insb
        popad
        db  073h, 074h
        push   edx
        insb
        popad
        je     SHORT loc_1_29165
        jbe    SHORT loc_1_29163
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 2064253dh
loc_1_29106:
        arpl   word ptr [ebp + 72h], si
        push   edx
        insb
        popad
        db  074h, 069h
        db  076h, 065h
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 0a64253dh
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebp + 6ch], ah
        je     SHORT loc_1_29188
        inc    ebx
        outsd
        insd
loc_1_2912A:
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ecx + 76h], ah
        inc    ebx
        outsd
        insd
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        db  072h, 043h
        outsd
        insd
        insd
loc_1_2914B:
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        inc    esi
        cmp    ah, byte ptr [eax]
        db  070h, 044h
        insb
        arpl   word ptr [edx + 75h], ax
loc_1_29163:
        push   ax
loc_1_29165:
        outsd
        outsd
        insb
        cmp    eax, 63206c25h
        db  075h, 072h
        inc    ecx
        db  076h, 061h
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        db  072h, 04Ch
        outsd
        arpl   word ptr [ebx + 65h], bp
        inc    ebx
        outsb
        db  074h, 03Dh
loc_1_29188:
        and    eax, 42000a64h
        push   ebp
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        popad
        db  076h, 067h
        inc    ebx
        outsd
        insd
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        db  072h, 043h
        outsd
        insd
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        or     al, byte ptr [eax]
loc_1_291B4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        jb     SHORT loc_1_29221
        popad
        je     SHORT loc_1_29224
        inc    esp
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        and    byte ptr [edx + 75h], ah
        and    byte ptr [edi + ebp*2 + 6fh], dh
        and    byte ptr [ebx + 6dh], dh
        popad
        insb
        insb
        and    byte ptr [eax + 4ch], dh
        arpl   word ptr [edx + eax*2 + 75h], bp
        cmp    ax, 6c25h
        or     al, byte ptr [eax]
loc_1_291E4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [edx + 65h], dh
        db  071h, 042h
        jne    SHORT loc_1_29256
        dec    esp
        outsb
        cmp    eax, 75206c25h
        jae    SHORT loc_1_2925F
        popad
        bound  ebp, qword ptr [ebp + 4ch]
        outsb
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_29208:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        jb     SHORT loc_1_29275
        popad
        db  074h, 065h
        inc    esp
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        and    byte ptr [edi + 50h], bl
        popad
loc_1_29221:
        inc    ecx
loc_1_29224:
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 70202d20h
        popad
        inc    ebx
        outsb
        db  074h, 03Dh
        and    eax, 0a64h
        add    byte ptr [eax], al
loc_1_29244:
        inc    edx
        push   ebp
        inc    esi
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [edx + 75h], ah
        jb     NEAR PTR loc_1_29274
        jo     SHORT loc_1_292C5
loc_1_29256:
        outsd
        insb
        and    byte ptr [ebx + 72h], ah
        popad
        je     SHORT loc_1_292C4
loc_1_2925F:
        and    byte ptr fs:[eax + 44h], dh
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        db  000h
loc_1_29274:
        inc    edx
loc_1_29275:
        push   ebp
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebx + 6ch], ch
        inc    edx
        popad
        jae    SHORT loc_1_292E6
        inc    ecx
        jb     NEAR PTR loc_1_292C3
        and    eax, 6572206ch
        jno    SHORT loc_1_292CF
        jne    SHORT loc_1_292F5
        dec    esp
        outsb
        cmp    eax, 75206425h
        jae    SHORT loc_1_292FE
        popad
        bound  ebp, qword ptr [ebp + 4ch]
        outsb
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [edi + ebp*2 + 74h], dh
        popad
        insb
        inc    ebx
        outsb
        db  074h, 03Dh
        and    eax, 696d2064h
        outsb
        push   esp
        push   68736572h
loc_1_292C3:
        outsd
loc_1_292C4:
        insb
loc_1_292C5:
        cmp    eax, 0a6425h
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_292CF:
        cmp    ah, byte ptr [eax]
        inc    esp
        arpl   word ptr gs:[edx + 65h], si
        popad
        db  073h, 065h
        inc    esp
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        and    byte ptr [ebx + 75h], ah
loc_1_292E6:
        jb     SHORT loc_1_29329
        jbe    SHORT loc_1_2934B
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
loc_1_292F5:
        jb     SHORT loc_1_29343
        outsd
        arpl   word ptr [ebx + 65h], bp
        inc    ebx
        outsb
loc_1_292FE:
        je     SHORT loc_1_2933D
        and    eax, 2e000a64h
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
        insw
        jb     NEAR PTR loc_1_29343
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
        insw
        db  067h, 072h, 02Eh
loc_1_29329:
        arpl   word ptr [eax], ax
        add    byte ptr [edx + 55h], al
        inc    esi
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [edx + 75h], ah
        and    byte ptr [eax + 6fh], dh
        outsd
        insb
loc_1_2933D:
        and    byte ptr [ebp + 63h], ah
        jb     SHORT loc_1_293A8
loc_1_29343:
        popad
        db  073h, 065h
        and    byte ptr fs:[eax + 44h], dh
        insb
loc_1_2934B:
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        cmp    eax, 73206c25h
        inc    ebx
        outsb
        db  074h, 03Dh
        and    eax, 0a64h
        add    byte ptr [edx + 55h], al
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebx + 75h], ah
        db  072h, 041h
        db  076h, 061h
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebp + 69h], ch
        outsb
        inc    ecx
        db  076h, 061h
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebp + 61h], ch
        js     SHORT loc_1_293CD
        jbe    SHORT loc_1_293EF
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2939C:
        inc    edx
        push   ebp
        inc    esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ecx + 76h], ah
        inc    ebx
        outsd
        insd
loc_1_293A8:
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        db  072h, 043h
        outsd
        insd
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        db  072h, 04Ch
        outsd
        arpl   word ptr [ebx + 65h], bp
        inc    ebx
        outsb
loc_1_293CD:
        je     SHORT loc_1_2940C
        and    eax, 0a64h
loc_1_293D4:
        inc    edx
        push   ebp
        inc    esi
        cmp    ah, byte ptr [eax]
        db  073h, 065h
        inc    esi
        db  072h, 065h
        push   ecx
        inc    ebx
        outsb
        db  074h, 073h
        cmp    ah, byte ptr [eax]
        and    eax, 57h
loc_1_293EC:
        pop    esp
loc_1_293EF:
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
        insw
        jb     NEAR PTR loc_1_2942B
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
loc_1_2940C:
        insw
        jb     NEAR PTR loc_1_2943F
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        jae    SHORT loc_1_2948E
        arpl   word ptr [edx + 75h], bx
        insw
        db  067h, 072h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
        pop    esp
loc_1_2942B:
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
        insw
        db  067h, 072h, 02Eh
        arpl   word ptr [eax], ax
        db  000h
loc_1_2943C:
        inc    edx
        push   ebp
        inc    esi
loc_1_2943F:
        cmp    ah, byte ptr [eax]
        inc    esp
        jae    NEAR PTR loc_1_294B9
        jb     SHORT loc_1_294B6
        db  079h, 044h
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        and    byte ptr [edx + 75h], ah
        and    byte ptr [eax + 6fh], dh
        outsd
        insb
        and    byte ptr [ebp + 73h], ah
        db  074h, 072h
        outsd
        jns    SHORT loc_1_294C9
        and    byte ptr fs:[eax + 44h], dh
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_29478:
        pop    esp
        pop    esp
        jae    SHORT loc_1_294F2
        arpl   word ptr [edx + 75h], bx
        insw
        db  067h, 072h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [ebp + 52h], al
loc_1_2948E:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebp
        outsb
        db  071h, 075h
        db  065h, 075h, 065h
        inc    esp
        insb
        arpl   word ptr [ebx + 65h], dx
        and    byte ptr [bp + di + 75h], ah
        jb     SHORT loc_1_294E4
        jbe    SHORT loc_1_29506
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_1_294FE
        outsd
        arpl   word ptr [ebx + 65h], bp
loc_1_294B6:
        inc    ebx
        outsb
loc_1_294B9:
        db  074h, 03Dh
        and    eax, 0a64h
loc_1_294C0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jae    SHORT loc_1_2952E
loc_1_294C9:
        dec    ecx
        outsb
        js     NEAR PTR loc_1_2950D
        and    eax, 69732064h
        db  07Ah, 065h
        dec    ecx
        outsb
        js     NEAR PTR loc_1_2951A
        and    eax, 0a64h
        add    byte ptr [eax], al
loc_1_294E4:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
        insw
loc_1_294F2:
        jb     NEAR PTR loc_1_29523
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
loc_1_294FE:
        db  073h, 072h
        arpl   word ptr [edx + 75h], bx
        insw
loc_1_29506:
        db  067h, 072h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
loc_1_2950D:
        pop    esp
        pop    esp
        jae    SHORT loc_1_29586
        arpl   word ptr [edx + 75h], bx
        insw
loc_1_2951A:
        db  067h, 072h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_29523:
        cmp    ah, byte ptr [eax]
        inc    esi
        db  072h, 065h
        inc    esp
        insb
        arpl   word ptr [edx + 75h], ax
loc_1_2952E:
        jae    NEAR PTR loc_1_29559
        sub    dword ptr [eax], esp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [ebx + 75h], ah
        db  072h, 041h
        db  064h, 064h, 072h, 03Dh
        and    eax, 0a6ch
loc_1_29544:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        jb     SHORT loc_1_295B1
        inc    esp
        insb
        arpl   word ptr [edx + 75h], ax
        db  066h, 073h, 028h
        sub    dword ptr [eax], esp
        jae    SHORT loc_1_295BE
loc_1_29559:
        insd
        outsb
        db  074h, 020h
        outsb
        outsd
        db  074h, 020h
        imul   ebp, dword ptr [esi + 75h], 63206573h
        jne    SHORT loc_1_295DE
        inc    ecx
        db  064h, 064h, 072h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_29578:
        pop    esp
        pop    esp
        jae    SHORT loc_1_295F2
        arpl   word ptr [edx + 75h], bx
        insw
loc_1_29586:
        db  067h, 072h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 044h
        insb
        arpl   word ptr [edx + 75h], ax
        db  066h, 073h, 028h
        sub    dword ptr [eax], esp
        bound  esi, qword ptr [ebp + 66h]
        push   ebx
        imul   edi, dword ptr [edx + 65h], 2064253dh
        imul   esi, dword ptr [ebx + 20h], 6c206f74h
        popad
loc_1_295B1:
        db  072h, 067h
        or     al, byte ptr gs:[eax]
        add    byte ptr [eax], al
loc_1_295B8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
loc_1_295BE:
        je     NEAR PTR loc_1_29605
        insb
        arpl   word ptr [edx + 75h], ax
        jae    NEAR PTR loc_1_295F0
        sub    dword ptr [eax], esp
        bound  esi, qword ptr [ebp + 66h]
        inc    ebx
        outsb
        je     SHORT loc_1_2960E
        and    eax, 73692064h
        and    byte ptr [edi + ebp*2 + 20h], dh
        insb
        popad
        db  072h, 067h
loc_1_295DE:
        or     al, byte ptr gs:[eax]
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        jae    SHORT loc_1_2965E
        arpl   word ptr [edx + 75h], bx
loc_1_295F0:
        insw
loc_1_295F2:
        jb     NEAR PTR loc_1_29623
        arpl   word ptr [eax], ax
        db  000h
loc_1_295F8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        arpl   word ptr [edx + 65h], si
        popad
        db  073h, 065h
loc_1_29605:
        inc    esp
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
loc_1_2960E:
        insb
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  070h, 061h
        or     al, byte ptr gs:[bx + si]
        add    byte ptr [eax], al
loc_1_29623:
        add    byte ptr [edx + 55h], al
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [edx + 75h], ah
        and    byte ptr [eax + 6fh], dh
        outsd
        insb
        and    byte ptr [ecx + 6eh], ch
        arpl   word ptr [edx + 65h], si
        popad
        db  073h, 065h
        and    byte ptr fs:[eax + 44h], dh
        insb
        arpl   word ptr [edx + 75h], ax
        push   ax
        outsd
        outsd
        insb
        cmp    eax, 73206c25h
        inc    ebx
        outsb
        db  074h, 03Dh
        and    eax, 0a64h
        add    byte ptr [edx + 55h], al
loc_1_2965E:
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebx + 75h], ah
        db  072h, 041h
        db  076h, 061h
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebp + 69h], ch
        outsb
        inc    ecx
        db  076h, 061h
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebp + 61h], ch
        js     SHORT loc_1_296C5
        db  076h, 061h
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_29694:
        inc    edx
        push   ebp
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [ecx + 76h], ah
        inc    ebx
        outsd
        insd
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_1_296F1
        outsd
        insd
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [ebx + 75h], ah
        db  072h, 04Ch
        outsd
        arpl   word ptr [ebx + 65h], bp
        inc    ebx
        outsb
loc_1_296C5:
        db  074h, 03Dh
        and    eax, 0a64h
loc_1_296CC:
        inc    edx
        push   ebp
        inc    edi
        cmp    ah, byte ptr [eax]
        db  073h, 065h
        inc    esi
        jb     SHORT loc_1_2973C
        push   ecx
        inc    ebx
        outsb
        db  074h, 073h
        cmp    ah, byte ptr [eax]
        and    eax, 57h
loc_1_296E4:
        inc    ebx
        inc    edx
        xor    bh, byte ptr [edx]
        and    byte ptr [ebx + 43h], al
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 49h]
        insd
loc_1_296F1:
        insd
        push   edx
        popad
        and    byte ptr fs:[ebx + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        popad
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_2970C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 49h]
        insd
        insd
        push   edx
        popad
        and    byte ptr fs:[esi + 6fh], ch
        and    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        and    byte ptr [ebx + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        inc    ebp
        inc    ecx
        inc    esp
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        db  000h
loc_1_2973C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 49h]
        insd
        insd
        push   edx
        popad
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_1_29775
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        and    byte ptr [ebx + 6dh], ah
        and    byte ptr fs:[ebx + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        inc    ebp
        inc    ecx
        inc    esp
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_29770:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29775:
        inc    ebx
        inc    ebx
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 49h]
        insd
        insd
        push   edx
        popad
        and    byte ptr fs:[ecx + 64h], ah
        jo     SHORT loc_1_297A9
        outsb
        outsd
        je     SHORT loc_1_297AD
        db  073h, 061h
        insd
        and    byte ptr gs:[ebx + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        inc    ebp
        inc    ecx
        inc    esp
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_297A4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_297A9:
        inc    ebx
        inc    ebx
        inc    edx
        pop    edi
loc_1_297AD:
        xor    bl, byte ptr [edi + 49h]
        insd
        insd
        push   edx
        popad
        and    byte ptr fs:[edi + 69h], dh
        db  074h, 068h
        and    byte ptr [ebx + 6dh], ah
        jo     SHORT loc_1_2982E
        db  074h, 046h
        insb
        popad
        and    byte ptr [bp + di + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        inc    ebp
        inc    ecx
        inc    esp
        cmp    eax, 0a6c25h
loc_1_297D8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 49h]
        insd
        insd
        push   edx
        popad
        and    byte ptr fs:[edi + 69h], dh
        db  074h, 068h
        and    byte ptr [edx + 65h], dh
        popad
        inc    esi
        insb
        popad
        and    byte ptr [bp + di + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        inc    ebp
        inc    ecx
        inc    esp
        cmp    eax, 0a6c25h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 49h]
        insd
        insd
        push   edx
        popad
        and    byte ptr fs:[edx + 61h], al
        and    byte ptr fs:[edx + 45h], dl
        inc    ecx
        inc    esp
        and    byte ptr [eax + 61h], dh
        jb     SHORT loc_1_2989B
loc_1_2982E:
        and    byte ptr [ebx + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        inc    ebp
        inc    ecx
        inc    esp
        cmp    eax, 0a6c25h
        db  000h
loc_1_29840:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 43h], ax
        inc    edx
        xor    al, byte ptr [esi + 75h]
        outsb
        arpl   word ptr [eax], sp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [ebx + 68h], ah
        popad
        imul   ebp, dword ptr [esi + 20h], 20727470h
        db  070h, 043h
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_29868:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 52h]
        db  065h, 071h, 075h
        jae    NEAR PTR loc_1_298EE
        and    byte ptr [edx + 61h], al
        and    byte ptr fs:[ebx + 43h], al
        inc    edx
        and    byte ptr [ecx + 64h], ah
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        outsb
        jne    SHORT loc_1_298FB
        bound  esp, qword ptr [ebp + 72h]
        and    dword ptr [edx], ecx
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
loc_1_2989B:
        inc    edx
        pop    edi
        xor    bl, byte ptr [edi + 52h]
        jno    NEAR PTR loc_1_29918
        jae    NEAR PTR loc_1_2991A
        and    byte ptr [ebx + 43h], al
        inc    edx
        and    byte ptr [ecx + 64h], ah
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_298D3
        outsb
        outsd
        je     SHORT loc_1_298D7
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    dword ptr fs:[edx], ecx
        add    byte ptr [eax], al
loc_1_298C4:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 43h], ax
        inc    edx
        xor    al, byte ptr [esi + 75h]
        outsb
loc_1_298D3:
        arpl   word ptr [eax], bp
        sub    dword ptr [eax], esp
loc_1_298D7:
        popad
        db  070h, 070h
        insb
        push   eax
        dec    ecx
        inc    esp
        cmp    eax, 0a6c25h
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 43h], ax
loc_1_298EE:
        inc    edx
        xor    al, byte ptr [esi + 75h]
        outsb
        arpl   word ptr [eax], bp
        sub    dword ptr [eax], esp
        db  070h, 043h
        inc    ebx
        inc    edx
loc_1_298FB:
        cmp    eax, 70206c25h
        dec    esi
        db  065h, 078h, 074h
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
        db  070h, 042h
        popad
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_29918:
        inc    ebx
        inc    edx
loc_1_2991A:
        xor    bh, byte ptr [edx]
        and    byte ptr [esp + ebp*2 + 63h], al
        inc    ebx
        inc    ebx
        inc    edx
        xor    al, byte ptr [esi + 75h]
        outsb
        arpl   word ptr [eax], bp
        sub    dword ptr [eax], esp
        db  070h, 043h
        inc    ebx
        inc    edx
        cmp    eax, 63206c25h
        insd
        cmp    eax, 61206225h
        jo     SHORT loc_1_299AD
        insb
        dec    ecx
        inc    esp
        cmp    eax, 43206225h
        inc    ebx
        inc    edx
        pop    edi
        db  065h, 076h, 065h
        outsb
        je     SHORT loc_1_2998B
        and    eax, 0a6ch
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 43h], ax
        inc    edx
        xor    al, byte ptr [esi + 75h]
        outsb
        arpl   word ptr [eax], sp
        popad
        popad
        jo     SHORT loc_1_299DE
        and    byte ptr [esi + 75h], ch
        insd
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_1_29993
        db  073h, 061h
        insd
        and    byte ptr gs:[eax + 43h], dh
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
loc_1_2998B:
        arpl   word ptr [ebx + 43h], ax
        inc    edx
        xor    al, byte ptr [esi + 75h]
        outsb
loc_1_29993:
        arpl   word ptr [eax], sp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [ebx + 68h], ah
        popad
        imul   ebp, dword ptr [esi + 20h], 20727470h
        db  070h, 043h
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_299AC:
        inc    esp
loc_1_299AD:
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebp + 78h], ax
        imul   esi, dword ptr [esi + eax*2 + 75h], 2928636eh
        and    byte ptr [eax + 44h], ch
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 3dh], sp
        and    eax, 6174206ch
        push   eax
        db  072h, 06Fh
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_1_29A12
        and    eax, 7061206ch
        jo     SHORT loc_1_29A48
        push   eax
        dec    ecx
loc_1_299DE:
        inc    esp
        cmp    eax, 0a6c25h
loc_1_299E4:
        pop    esp
        pop    esp
        jae    SHORT loc_1_29A5E
        arpl   word ptr [ebx + 6ch], bx
        popad
        outsb
        jne    SHORT loc_1_29A65
        arpl   word ptr cs:[eax], ax
loc_1_299F8:
        pop    esp
        pop    esp
        jae    SHORT loc_1_29A72
        arpl   word ptr [ebx + 6ch], bx
        popad
        outsb
        jne    SHORT loc_1_29A79
        arpl   word ptr cs:[eax], ax
loc_1_29A0C:
        pop    esp
        pop    esp
loc_1_29A12:
        db  073h, 072h
        arpl   word ptr [ebx + 6ch], bx
        popad
        outsb
        db  075h, 070h
        arpl   word ptr cs:[eax], ax
loc_1_29A20:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [ebx + 6ch], bx
        popad
        outsb
        jne    SHORT loc_1_29AA1
        arpl   word ptr cs:[eax], ax
loc_1_29A34:
        pop    esp
        pop    esp
        jae    SHORT loc_1_29AAE
        arpl   word ptr [ebx + 6ch], bx
        popad
        outsb
        db  075h, 070h
        arpl   word ptr cs:[eax], ax
loc_1_29A48:
        pop    esp
        pop    esp
        jae    SHORT loc_1_29AC2
        arpl   word ptr [ebx + 6ch], bx
        popad
        outsb
        jne    SHORT loc_1_29AC9
        arpl   word ptr cs:[eax], ax
loc_1_29A5C:
        inc    esp
        inc    edx
loc_1_29A5E:
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        db  065h, 073h, 074h
loc_1_29A65:
        db  072h, 06Fh
        jns    SHORT loc_1_29AAA
        db  070h, 070h
        insb
        and    byte ptr [ebx + 6ch], ah
        popad
        outsb
loc_1_29A72:
        db  075h, 070h
        and    byte ptr [eax + 41h], dh
        dec    edi
        inc    ebx
loc_1_29A79:
        inc    edx
        cmp    eax, 61206c25h
        jo     SHORT loc_1_29AF1
        insb
        dec    ecx
        inc    esp
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        db  065h, 073h, 074h
        db  072h, 06Fh
        jns    SHORT loc_1_29ADA
        db  070h, 070h
        insb
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
loc_1_29AA1:
        db  074h, 020h
        jae    SHORT loc_1_29B0A
        je     SHORT loc_1_29AC7
        inc    esi
        jne    SHORT loc_1_29B18
loc_1_29AAA:
        arpl   word ptr [ecx + ebp*2 + 6fh], si
loc_1_29AAE:
        outsb
        popad
        insb
        and    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
        jae    SHORT loc_1_29B2C
        or     al, byte ptr [eax]
        db  000h
loc_1_29ABC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_29AC2:
        db  065h, 073h, 074h
        jb     SHORT loc_1_29B36
loc_1_29AC7:
        jns    SHORT loc_1_29B0A
loc_1_29AC9:
        jo     SHORT loc_1_29B3B
        insb
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        db  073h, 065h
        je     SHORT loc_1_29AF7
        dec    ebp
        jne    SHORT loc_1_29B46
loc_1_29ADA:
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_29B01
        inc    ecx
        jb     NEAR PTR loc_1_29B4B
        jae    SHORT loc_1_29B5B
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_29AEC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29AF1:
        inc    esp
        db  065h, 073h, 074h
        db  072h, 06Fh
loc_1_29AF7:
        db  079h, 041h
        db  070h, 070h
        insb
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
loc_1_29B01:
        je     SHORT loc_1_29B23
        db  073h, 065h
        je     SHORT loc_1_29B27
        inc    edi
        jb     SHORT loc_1_29B79
loc_1_29B0A:
        db  075h, 070h
        and    byte ptr [ecx + 64h], al
        jb     NEAR PTR loc_1_29B77
        jae    SHORT loc_1_29B87
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_29B18:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        db  065h, 073h, 074h
        db  072h, 06Fh
loc_1_29B23:
        db  079h, 041h
        jo     SHORT loc_1_29B97
loc_1_29B27:
        insb
        and    byte ptr [ebx + 61h], ah
        outsb
loc_1_29B2C:
        daa
        db  074h, 020h
        db  073h, 065h
        db  074h, 020h
        dec    ebp
        jne    SHORT loc_1_29BA2
loc_1_29B36:
        je     SHORT loc_1_29BA1
        arpl   word ptr [ecx + 73h], sp
loc_1_29B3B:
        db  074h, 020h
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        or     al, byte ptr [eax]
loc_1_29B46:
        add    byte ptr [eax], al
loc_1_29B48:
        inc    ebp
        push   edx
        push   edx
loc_1_29B4B:
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 043h
        push   ax
        popad
        db  072h, 06Dh
        db  073h, 028h
        sub    dword ptr [eax], esp
loc_1_29B5B:
        dec    edi
        db  070h, 065h
        outsb
        push   eax
        db  072h, 06Fh
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        inc    ebx
        and    byte ptr [bp + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 45000a21h
        push   edx
        push   edx
loc_1_29B77:
        cmp    ah, byte ptr [eax]
loc_1_29B79:
        and    byte ptr [eax], ah
        db  070h, 043h
        push   bx
        db  074h, 072h
        sub    eax, 6675423eh
loc_1_29B87:
        db  066h, 065h, 072h, 03Dh
        and    eax, 646e2053h
        imul   esi, dword ptr [ebx + 53h], 75746174h
loc_1_29B97:
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_29BA0:
        inc    esp
loc_1_29BA1:
        inc    edx
loc_1_29BA2:
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 043h
        push   ax
        popad
        db  072h, 06Dh
        db  073h, 028h
        sub    dword ptr [eax], esp
        push   edx
        popad
        inc    ebx
        and    byte ptr [bp + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 44000a21h
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_29C10
        push   bx
        db  074h, 072h
        sub    eax, 6675423eh
        db  066h, 065h, 072h, 03Dh
        and    eax, 656b2053h
        jns    SHORT loc_1_29C35
        db  074h, 072h
        cmp    eax, 6e207325h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 0a6ch
loc_1_29BF8:
        inc    ebx
        inc    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 043h
        push   si
        popad
        insb
        jne    SHORT loc_1_29C6D
        sub    byte ptr [ecx], ch
        and    byte ptr [203d2073h], ah
loc_1_29C10:
        and    eax, 43000a62h
        inc    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 043h
        push   si
        popad
        insb
        db  075h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [203d2073h], ah
        and    eax, 43000a77h
        inc    esi
        inc    edi
        cmp    ah, byte ptr [eax]
loc_1_29C35:
        push   ebx
        je     NEAR PTR loc_1_29C7C
        push   si
        popad
        insb
        jne    SHORT loc_1_29CA5
        sub    byte ptr [ecx], ch
        and    byte ptr [203d2073h], ah
        and    eax, 43000a6ch
        inc    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 043h
        push   si
        popad
        insb
        db  075h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [203d2073h], ah
        and    eax, 45000a42h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29C6D:
        inc    esp
        imul   esi, dword ptr [edx + 43h], 65736f6ch
        inc    ecx
        db  064h, 070h, 020h
        arpl   word ptr [ecx + 6eh], sp
loc_1_29C7C:
        daa
        db  074h, 020h
        jae    SHORT loc_1_29CE6
        db  074h, 020h
        inc    esi
        jne    SHORT loc_1_29CF4
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        popad
        insb
        and    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
        jae    SHORT loc_1_29D08
        or     al, byte ptr [eax]
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 43h], 65736f6ch
loc_1_29CA5:
        inc    ecx
        jo     NEAR PTR loc_1_29CC9
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        db  073h, 065h
        db  074h, 020h
        dec    ebp
        jne    SHORT loc_1_29D22
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_29CDD
        inc    ecx
        jb     NEAR PTR loc_1_29D27
        db  073h, 073h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_29CC8:
        inc    ebp
loc_1_29CC9:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 43h], 65736f6ch
        inc    ecx
        jo     NEAR PTR loc_1_29CF9
        arpl   word ptr [ecx + 6eh], sp
        daa
loc_1_29CDD:
        db  074h, 020h
        jae    SHORT loc_1_29D46
        db  074h, 020h
        inc    edi
        db  072h, 06Fh
loc_1_29CE6:
        db  075h, 070h
        and    byte ptr [ecx + 64h], al
        jb     NEAR PTR loc_1_29D53
        db  073h, 073h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_29CF4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29CF9:
        inc    esp
        imul   esi, dword ptr [edx + 43h], 65736f6ch
        inc    ecx
        jo     NEAR PTR loc_1_29D25
        arpl   word ptr [ecx + 6eh], sp
loc_1_29D08:
        daa
        db  074h, 020h
        db  073h, 065h
        db  074h, 020h
        dec    ebp
        db  075h, 06Ch
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_29D39
        inc    ecx
        jb     NEAR PTR loc_1_29D83
        jae    SHORT loc_1_29D93
        or     al, byte ptr [eax]
loc_1_29D22:
        add    byte ptr [eax], al
loc_1_29D24:
        inc    ebp
loc_1_29D25:
        push   edx
        push   edx
loc_1_29D27:
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 43h], 65736f6ch
        inc    esp
        imul   esi, dword ptr [edx + 65h], 61207463h
loc_1_29D39:
        jo     SHORT loc_1_29DAB
        insb
        and    byte ptr [edi + ebp*2 + 65h], ah
        db  073h, 06Eh
        daa
        je     SHORT loc_1_29D65
        outsd
loc_1_29D46:
        db  077h, 06Eh
        and    byte ptr [esi + 55h], cl
        dec    esp
        dec    esp
        and    byte ptr [ebx + 41h], dl
        push   eax
        or     al, byte ptr [eax]
loc_1_29D53:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_29D9E
        dec    edi
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
loc_1_29D65:
        push   ebx
        inc    ecx
        push   eax
        sub    eax, 4f41703eh
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_29D74:
        push   ebx
        inc    ebp
        push   ebx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 65h], 53207463h
        db  074h, 061h
loc_1_29D83:
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ebx + 6ch], al
        outsd
        jae    SHORT loc_1_29DF2
        or     al, byte ptr fs:[eax]
loc_1_29D90:
        inc    esp
        inc    edx
        inc    edi
loc_1_29D93:
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 43h], 65736f6ch
        inc    esp
loc_1_29D9E:
        imul   esi, dword ptr [edx + 65h], 4e207463h
        push   ebp
        dec    esp
        dec    esp
        and    byte ptr [ebx + 41h], dl
loc_1_29DAB:
        push   eax
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        outsd
        db  070h, 065h
        outsb
        or     al, byte ptr [eax]
        db  000h
loc_1_29DB8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 49h], 6974696eh
        popad
        insb
        imul   edi, dword ptr [edx + 65h], 61646120h
        je     SHORT loc_1_29E35
        jb     SHORT loc_1_29DF2
        jb     SHORT loc_1_29E39
        db  073h, 065h
        je     SHORT loc_1_29DE2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_29DDC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_29DE2:
        imul   esi, dword ptr [edx + 49h], 6974696eh
        popad
        insb
        imul   edi, dword ptr [edx + 65h], 6e616320h
loc_1_29DF2:
        daa
        db  074h, 020h
        jb     SHORT loc_1_29E5C
        jae    SHORT loc_1_29E5E
        db  074h, 020h
        popad
        popad
        db  070h, 074h
        db  065h, 072h, 00Ah
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 4fh], 416e6570h
        db  064h, 070h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_29E3A
        db  067h, 065h, 074h, 020h
        arpl   word ptr [ebp + 72h], si
        jb     SHORT loc_1_29E88
        outsb
        db  074h, 020h
        popad
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        imul   ebp, dword ptr [esi + 66h], 45000a6fh
loc_1_29E35:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29E39:
        inc    esp
loc_1_29E3A:
        imul   esi, dword ptr [edx + 4fh], 446e6570h
        imul   esi, dword ptr [edx + 65h], 69207463h
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 45h], 72656874h
        outsb
        je     NEAR PTR loc_1_29E78
        db  070h, 061h
        jb     SHORT loc_1_29EC9
loc_1_29E5C:
        db  073h, 00Ah
loc_1_29E5E:
        add    byte ptr [eax], al
loc_1_29E60:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  065h, 074h, 068h
        db  065h, 072h, 06Eh
        db  065h, 074h, 054h
        jns    SHORT loc_1_29EE2
        cmp    eax, 70207725h
loc_1_29E78:
        db  072h, 06Fh
        je     SHORT loc_1_29EEB
        arpl   word ptr [edi + 6ch], bp
        push   esp
        jns    SHORT loc_1_29EF2
        dec    edi
        jae    NEAR PTR loc_1_29EED
loc_1_29E88:
        je     SHORT loc_1_29EC7
        and    eax, 0a64h
        add    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        db  074h, 061h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ecx + 6ch], ah
        db  072h, 065h
        popad
        jns    NEAR PTR loc_1_29EC7
        outsd
        jo     SHORT loc_1_29F0F
        outsb
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + 69h], bx
        jb     SHORT loc_1_29F21
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [ebx + 45h], dl
        push   ebx
loc_1_29EC7:
        cmp    ah, byte ptr [eax]
loc_1_29EC9:
        inc    esp
        imul   esi, dword ptr [edx + 65h], 53207463h
        db  074h, 061h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edi + 70h], cl
        outsb
        and    byte ptr fs:[eax + 41h], dh
        inc    ebx
loc_1_29EE2:
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_29F39
loc_1_29EEB:
        jne    SHORT loc_1_29F5A
loc_1_29EED:
        cmp    eax, 70206425h
loc_1_29EF2:
        inc    ecx
        dec    edi
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        db  000h
loc_1_29EFC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 52h], 4c646165h
        outsd
        and    byte ptr [bp + di + 61h], ah
        outsb
loc_1_29F0F:
        daa
        je     SHORT loc_1_29F32
        db  067h, 065h, 074h, 020h
        popad
        popad
        jo     SHORT loc_1_29F8F
        db  065h, 072h, 020h
        db  073h, 074h
        popad
loc_1_29F21:
        db  074h, 069h
        jae    SHORT loc_1_29F99
        imul   esp, dword ptr [ebx + 73h], 0ah
loc_1_29F2C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_29F32:
        imul   esi, dword ptr [edx + 53h], 75467465h
loc_1_29F39:
        outsb
        arpl   word ptr [ecx + 64h], ax
        jb     NEAR PTR loc_1_29F60
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_29F66
        db  073h, 065h
        db  074h, 020h
        inc    esi
        jne    SHORT loc_1_29FBB
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        popad
        insb
        and    byte ptr [ecx + 64h], al
        jb     NEAR PTR loc_1_29FBF
loc_1_29F5A:
        db  073h, 073h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_29F60:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_29F66:
        imul   esi, dword ptr [edx + 53h], 75467465h
        outsb
        arpl   word ptr [ecx + 64h], ax
        db  064h, 072h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_29F9A
        jae    SHORT loc_1_29FE1
        db  074h, 020h
        dec    ebp
        db  075h, 06Ch
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        db  074h, 020h
        inc    ecx
        jb     NEAR PTR loc_1_29FF2
        jae    SHORT loc_1_2A002
loc_1_29F8F:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29F99:
        inc    esp
loc_1_29F9A:
        imul   esi, dword ptr [edx + 53h], 72477465h
        outsd
        jne    SHORT loc_1_2A014
        inc    ecx
        jb     NEAR PTR loc_1_29FC9
        jb     NEAR PTR loc_1_2A01B
        jne    SHORT loc_1_2A01E
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        jae    SHORT loc_1_2A029
        and    byte ptr [ecx + 6ch], ah
        db  072h, 065h
loc_1_29FBB:
        popad
        jns    NEAR PTR loc_1_29FDF
loc_1_29FBF:
        jae    SHORT loc_1_2A026
        db  074h, 00Ah
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29FC9:
        inc    esp
        imul   esi, dword ptr [edx + 53h], 72477465h
        outsd
        db  075h, 070h
        inc    ecx
        jb     NEAR PTR loc_1_29FF9
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
loc_1_29FDF:
        db  073h, 065h
loc_1_29FE1:
        db  074h, 020h
        db  067h, 072h, 06Fh
        jne    SHORT loc_1_2A058
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        db  073h, 073h
        or     al, byte ptr [eax]
loc_1_29FF2:
        add    byte ptr [eax], al
loc_1_29FF4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_29FF9:
        inc    esp
        imul   esi, dword ptr [edx + 53h], 72477465h
        outsd
loc_1_2A002:
        db  075h, 070h
        inc    ecx
        jb     NEAR PTR loc_1_2A029
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        db  073h, 065h
        db  074h, 020h
        dec    ebp
loc_1_2A014:
        jne    SHORT loc_1_2A082
        je     SHORT loc_1_2A081
        arpl   word ptr [ecx + 73h], sp
loc_1_2A01B:
        je     SHORT loc_1_2A03D
        inc    ecx
loc_1_2A01E:
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        or     al, byte ptr [eax]
loc_1_2A026:
        add    byte ptr [eax], al
loc_1_2A028:
        inc    ebp
loc_1_2A029:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 53h], 754d7465h
        insb
        je     SHORT loc_1_2A0A1
        arpl   word ptr [ecx + 73h], sp
        db  074h, 041h
loc_1_2A03D:
        db  064h, 064h, 072h, 020h
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 61646120h
        db  070h, 074h
        jb     NEAR PTR loc_1_2A05E
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A058:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2A05E:
        imul   esi, dword ptr [edx + 53h], 72477465h
        outsd
        db  075h, 070h
        inc    ecx
        jb     NEAR PTR loc_1_2A08D
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        db  073h, 065h
        db  074h, 020h
        dec    ebp
        jne    SHORT loc_1_2A0E6
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_2A0A1
loc_1_2A081:
        inc    ecx
loc_1_2A082:
        jb     NEAR PTR loc_1_2A0EB
        jae    SHORT loc_1_2A0FB
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2A08C:
        inc    ebp
loc_1_2A08D:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [edx + 53h], 75746174h
        db  073h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_2A0C1
loc_1_2A0A1:
        db  067h, 065h, 074h, 020h
        arpl   word ptr [ebp + 72h], si
        db  072h, 065h
        outsb
        je     SHORT loc_1_2A0CD
        popad
        popad
        jo     SHORT loc_1_2A126
        db  065h, 072h, 020h
        imul   ebp, dword ptr [esi + 66h], 0a6fh
loc_1_2A0BC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2A0C1:
        inc    ebx
        push   646e416bh
        push   ebx
        je     NEAR PTR loc_1_2A11E
        inc    ecx
        push   eax
loc_1_2A0CD:
        push   eax
        popad
        db  072h, 06Dh
        and    byte ptr [eax + 61h], dh
        db  072h, 06Dh
        cmp    eax, 6d206225h
        popad
        js     SHORT loc_1_2A11B
        and    eax, 0a62h
        add    byte ptr [ebp + 52h], al
loc_1_2A0E6:
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_1_2A0EB:
        and    eax, 0a73h
loc_1_2A0F0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        inc    ebx
        insb
        outsd
loc_1_2A0FB:
        db  073h, 065h
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [ecx + ebp*2 + 6eh], cl
        imul   edi, dword ptr [6f206c25h], 70h
        outsb
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [ebx + 41h], dl
        push   eax
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_2A11B:
        add    byte ptr [ebp + 52h], al
loc_1_2A11E:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        inc    ebx
        insb
loc_1_2A126:
        outsd
        db  073h, 065h
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [edi + 72h], ah
        outsd
        jne    SHORT loc_1_2A1A2
        and    byte ptr [ebp + 65h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        db  073h, 020h
        outsd
        outsb
        and    byte ptr [ebx + 41h], dl
        push   eax
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_2A148:
        push   ebx
        inc    ebp
        push   ebx
        cmp    ah, byte ptr [eax]
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [ebx + 6ch], al
        outsd
        db  073h, 065h
        and    byte ptr fs:[61747320h], ch
        dec    ecx
        inc    esp
        cmp    eax, 0a7725h
loc_1_2A164:
        push   ebx
        inc    ebp
        push   ebx
        cmp    ah, byte ptr [eax]
        push   ebx
        je     SHORT loc_1_2A1CD
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ebx + 6ch], al
        outsd
        db  073h, 065h
        and    byte ptr fs:[61747320h], ch
        dec    ecx
        inc    esp
        cmp    eax, 72207725h
        je     NEAR PTR loc_1_2A1CA
        outsd
        cmp    eax, 0a6225h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        inc    ebx
        insb
        outsd
        db  073h, 065h
        push   ebx
        db  074h, 061h
        inc    ebx
        insd
loc_1_2A1A2:
        jo     SHORT loc_1_2A210
        db  074h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        arpl   word ptr [edi + ebp*2 + 73h], bp
        and    byte ptr gs:[ebx + 74h], dh
        popad
        dec    ecx
        inc    esp
        cmp    eax, 72207725h
        db  065h, 074h, 043h
        outsd
        cmp    eax, 0a6225h
        add    byte ptr [ebp + 52h], al
loc_1_2A1CA:
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2A1CD:
        inc    esp
        dec    esp
        inc    ebx
        inc    ebx
        outsd
        outsb
        push   ebx
        db  074h, 061h
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[ebx + 6fh], dl
        db  075h, 072h
        arpl   word ptr [ebp + 20h], sp
        push   edx
        outsd
        jne    SHORT loc_1_2A25A
        and    byte ptr gs:[edx + 74h], dh
        inc    ebx
        je     SHORT loc_1_2A25A
        cmp    eax, 0a7725h
        add    byte ptr [ebx + 45h], dl
        push   ebx
        cmp    ah, byte ptr [eax]
        push   ebx
        je     SHORT loc_1_2A25D
        je     SHORT loc_1_2A267
        outsd
        outsb
        and    byte ptr [ebx + 6fh], ah
        outsb
        outsb
        arpl   word ptr gs:[ebp + 64h], si
        and    byte ptr [61747320h], ch
loc_1_2A210:
        dec    ecx
        inc    esp
        cmp    eax, 72207725h
        je     NEAR PTR loc_1_2A25D
        outsd
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2A224:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebx
        db  074h, 061h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[ebp + 64h], si
        and    byte ptr [61747320h], ch
        dec    ecx
        inc    esp
        cmp    eax, 72207725h
        db  065h, 074h, 043h
        outsd
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2A258:
        push   ebx
        inc    ebp
loc_1_2A25A:
        push   ebx
        cmp    ah, byte ptr [eax]
loc_1_2A25D:
        inc    esi
        insb
        outsd
        db  077h, 020h
        inc    ebx
        outsd
        outsb
        db  074h, 072h
loc_1_2A267:
        outsd
        insb
        and    byte ptr [61747320h], ch
        dec    ecx
        inc    esp
        cmp    eax, 43207725h
        inc    ebx
        inc    edx
        pop    edi
        insb
        outsd
        db  077h, 043h
        db  074h, 06Ch
        cmp    eax, 52207725h
        je     NEAR PTR loc_1_2A2CB
        outsd
        cmp    eax, 0a6225h
loc_1_2A290:
        db  074h, 069h
        insd
        db  065h, 072h, 054h
        xor    dword ptr [eax], eax
loc_1_2A298:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 20732520h
        db  070h, 061h
        db  072h, 06Dh
        cmp    eax, 6d206225h
        popad
        db  078h, 03Dh
        and    eax, 0a62h
        db  000h
loc_1_2A2BC:
        db  074h, 069h
        insd
        jb     NEAR PTR loc_1_2A316
        xor    al, byte ptr [eax]
loc_1_2A2C4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
loc_1_2A2CB:
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 20732520h
        db  070h, 061h
        jb     SHORT loc_1_2A347
        cmp    eax, 6d206225h
        popad
        js     SHORT loc_1_2A31F
        and    eax, 0a62h
        add    byte ptr [ecx + ebp*2 + 6dh], dh
        jb     NEAR PTR loc_1_2A342
        imul   eax, dword ptr [eax], 3a525245h
        and    byte ptr [esp + ecx*2 + 43h], al
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 20732520h
        db  070h, 061h
        jb     SHORT loc_1_2A373
        cmp    eax, 6d206225h
        popad
        js     SHORT loc_1_2A34B
        and    eax, 0a62h
        add    byte ptr [ebp + 61h], ch
loc_1_2A316:
        db  078h, 04Fh
        jne    SHORT loc_1_2A38E
        add    byte ptr [eax], al
loc_1_2A31C:
        inc    ebp
        push   edx
        push   edx
loc_1_2A31F:
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 20732520h
        db  070h, 061h
        db  072h, 06Dh
        cmp    eax, 6d206225h
        popad
        db  078h, 03Dh
        and    eax, 0a62h
        add    byte ptr [ebp + 61h], ch
loc_1_2A342:
        db  078h, 049h
        outsb
        add    byte ptr [eax], al
loc_1_2A347:
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2A34B:
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 20732520h
        db  070h, 061h
        jb     SHORT loc_1_2A3CB
        cmp    eax, 6d206225h
        popad
        js     SHORT loc_1_2A3A3
        and    eax, 0a62h
        add    byte ptr [ebp + 61h], ch
        db  078h, 04Fh
        db  075h, 074h
        dec    ecx
loc_1_2A373:
        outsb
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2A378:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 20732520h
        db  070h, 061h
        db  072h, 06Dh
loc_1_2A38E:
        cmp    eax, 6d206225h
        popad
        js     SHORT loc_1_2A3D3
        and    eax, 0a62h
        add    byte ptr [ebp + 61h], ch
        db  078h, 052h
        db  065h, 074h, 072h
loc_1_2A3A3:
        jns    SHORT loc_1_2A3E8
        outsb
        je     SHORT loc_1_2A3A8
loc_1_2A3A8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 20732520h
        jo     SHORT loc_1_2A41D
        jb     SHORT loc_1_2A42B
        cmp    eax, 6d206225h
        popad
        js     SHORT loc_1_2A403
        and    eax, 0a62h
loc_1_2A3CB:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
loc_1_2A3D3:
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 766e6920h
        popad
        insb
        imul   esp, dword ptr [eax + 61h], 73656363h
loc_1_2A3E8:
        jae    SHORT loc_1_2A40A
        jo     SHORT loc_1_2A45E
        imul   ebp, dword ptr [edi + 72h], 20797469h
        popad
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        push   eax
        jb     SHORT loc_1_2A465
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2A403:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2A40A:
        dec    esp
        inc    ebx
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 6f726720h
        db  075h, 070h
        dec    ebp
        insd
        inc    ebx
        outsb
loc_1_2A41D:
        db  074h, 020h
        and    byte ptr ds:[edi + 73h], ah
        popad
        jo     SHORT loc_1_2A473
        insd
        bound  esp, qword ptr [ebp + 72h]
loc_1_2A42B:
        dec    ebp
        popad
        js     SHORT loc_1_2A439
        db  000h
loc_1_2A430:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    ebp
loc_1_2A439:
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 50415320h
        and    byte ptr [edi + 72h], ah
        jo     SHORT loc_1_2A494
        insd
        inc    ebx
        outsb
        db  074h, 03Dh
        and    eax, 68632064h
        popad
        outsb
        or     al, byte ptr fs:[bx + si]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
loc_1_2A45E:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    ebp
loc_1_2A465:
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 6f726720h
        db  075h, 070h
        and    byte ptr [ebx + 41h], dl
loc_1_2A473:
        push   eax
        and    byte ptr [eax + 72h], dh
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        and    byte ptr [edx + 65h], dh
        db  074h, 043h
        outsd
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
loc_1_2A494:
        dec    ebp
        outsd
        imul   esp, dword ptr fs:[esi + 79h], 61747320h
        dec    ecx
        inc    esp
        cmp    eax, 63207725h
        popad
        outsb
        daa
        je     SHORT loc_1_2A4CA
        bound  esp, qword ptr [ebp + 20h]
        popad
        and    byte ptr [edi + 72h], ah
        outsd
        jne    SHORT loc_1_2A524
        and    byte ptr [ebp + 65h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 45h], dl
        push   ebx
        cmp    ah, byte ptr [eax]
        push   ebx
        db  074h, 061h
        db  074h, 069h
loc_1_2A4CA:
        outsd
        outsb
        and    byte ptr [eax + 61h], dh
        jb     SHORT loc_1_2A532
        insd
        db  065h, 074h, 065h
        jb     SHORT loc_1_2A54A
        and    byte ptr [ebp + 6fh], ch
        imul   esp, dword ptr fs:[esi + 69h], 2d206465h
        and    byte ptr [ebx + 74h], dh
        popad
        dec    ecx
        inc    esp
        cmp    eax, 0a7725h
        add    byte ptr [eax], al
        db  000h
loc_1_2A4F0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    edi
        jo     SHORT loc_1_2A560
        outsb
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [ecx + 76h], ah
        popad
        imul   ebp, dword ptr [ecx + 62h], 5320656ch
        inc    ecx
        push   eax
        jae    SHORT loc_1_2A51C
        add    byte ptr [eax], al
loc_1_2A514:
        je     SHORT loc_1_2A57F
        insd
        db  065h, 072h, 054h
        xor    dword ptr [eax], eax
loc_1_2A51C:
        je     SHORT loc_1_2A587
        insd
        jb     NEAR PTR loc_1_2A576
        xor    al, byte ptr [eax]
loc_1_2A524:
        je     SHORT loc_1_2A58F
        insd
        jb     NEAR PTR loc_1_2A57E
        imul   eax, dword ptr [eax], 4f78616dh
        jne    SHORT loc_1_2A5A6
loc_1_2A532:
        add    byte ptr [eax], al
loc_1_2A534:
        insd
        popad
        db  078h, 049h
        outsb
        add    byte ptr [eax], al
        db  000h
loc_1_2A53C:
        insd
        popad
        js     SHORT loc_1_2A58F
        jne    SHORT loc_1_2A5B6
        dec    ecx
        outsb
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2A548:
        insd
        popad
loc_1_2A54A:
        js     SHORT loc_1_2A59E
        db  065h, 074h, 072h
        jns    SHORT loc_1_2A594
        outsb
        je     SHORT loc_1_2A554
loc_1_2A554:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
loc_1_2A560:
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        db  065h, 072h, 054h
        xor    ah, byte ptr [eax]
        and    byte ptr ds:[ecx + ebp*2 + 6dh], dh
        jb     NEAR PTR loc_1_2A5C8
        xor    dword ptr [edx], ecx
loc_1_2A576:
        add    byte ptr [eax], al
loc_1_2A578:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2A57E:
        insb
loc_1_2A57F:
        arpl   word ptr [edi + 70h], cx
        outsb
        push   ebx
        inc    ecx
        push   eax
loc_1_2A587:
        and    byte ptr [ebp + 61h], ch
        js     SHORT loc_1_2A5D3
        jb     SHORT loc_1_2A5FE
        dec    ebp
loc_1_2A58F:
        insd
        bound  esp, qword ptr [ebp + 72h]
loc_1_2A594:
        jae    SHORT loc_1_2A5B6
        and    byte ptr ds:[ebp + 61h], ch
        js     SHORT loc_1_2A5E3
        jb     SHORT loc_1_2A60E
loc_1_2A59E:
        dec    ebp
        insd
        bound  esp, qword ptr [ebp + 72h]
        db  073h, 00Ah
loc_1_2A5A6:
        add    byte ptr [eax], al
loc_1_2A5A8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
        push   ebx
        inc    ecx
loc_1_2A5B6:
        push   eax
        and    byte ptr [ebp + 61h], ch
        js     SHORT loc_1_2A605
        imul   sp, word ptr [ebp + 6ch], 3d64h
        and    eax, 6f742064h
        outsd
loc_1_2A5C8:
        and    byte ptr [ecx + 72h], ch
        and    byte ptr gs:[di + 61h], ch
        js     SHORT loc_1_2A625
loc_1_2A5D3:
        arpl   word ptr [esi + 46h], si
        jb     SHORT loc_1_2A645
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2A5E3:
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [ebx + 61h], dh
        jo     SHORT loc_1_2A64A
        popad
        insb
        jne    SHORT loc_1_2A65D
        cmp    eax, 61206225h
        insb
loc_1_2A5FE:
        db  072h, 065h
        popad
        jns    NEAR PTR loc_1_2A624
        outsd
loc_1_2A605:
        db  070h, 065h
        outsb
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + 6eh], ch
loc_1_2A60E:
        db  076h, 061h
        insb
        imul   esp, dword ptr [edx + ecx], 45000000h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
loc_1_2A624:
        push   ebx
loc_1_2A625:
        inc    ecx
        push   eax
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 61h], 73656363h
        jae    SHORT loc_1_2A687
        jb     SHORT loc_1_2A6A2
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2A640:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2A645:
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
loc_1_2A64A:
        outsb
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 6fh], 6f697470h
loc_1_2A65D:
        outsb
        db  073h, 050h
        jb     SHORT loc_1_2A6CB
        cmp    eax, 0a6225h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [edi + ebp*2 + 6fh], dh
        and    byte ptr [ebp + 61h], ch
        outsb
        db  079h, 020h
        jae    SHORT loc_1_2A6F7
        popad
        db  074h, 069h
        outsd
loc_1_2A687:
        outsb
        db  073h, 020h
        insd
        popad
        db  078h, 053h
        db  074h, 061h
        inc    ebx
        outsb
        je     SHORT loc_1_2A6D1
        and    eax, 45000a64h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
loc_1_2A6A2:
        outsb
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [edi + ebp*2 + 6fh], dh
        and    byte ptr [ebp + 61h], ch
        outsb
        jns    SHORT loc_1_2A6D1
        jae    SHORT loc_1_2A727
        popad
        je     SHORT loc_1_2A71F
        outsd
        outsb
        jae    SHORT loc_1_2A6DA
        jae    SHORT loc_1_2A730
        popad
        inc    ebx
        outsb
        db  074h, 073h
        cmp    eax, 252f6425h
        or     al, byte ptr fs:[eax]
        add    byte ptr [eax], al
loc_1_2A6CB:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2A6D1:
        inc    esp
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
        push   ebx
        inc    ecx
loc_1_2A6DA:
        push   eax
        and    byte ptr [edi + ebp*2 + 6fh], dh
        and    byte ptr [ebp + 61h], ch
        outsb
        jns    SHORT loc_1_2A705
        jb     NEAR PTR loc_1_2A757
        db  075h, 070h
        and    byte ptr [ebx + 41h], dl
        push   eax
        db  073h, 020h
        jb     NEAR PTR loc_1_2A763
        dec    ebp
        insd
        inc    ebx
loc_1_2A6F7:
        outsb
        je     SHORT loc_1_2A737
        and    eax, 0a64h
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
loc_1_2A705:
        inc    esp
        dec    esp
        inc    ebx
        dec    edi
        db  070h, 065h
        outsb
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [ebx + 41h], dl
        push   eax
        and    byte ptr [edi + 70h], ch
        outsb
        imul   ebp, dword ptr [esi + 67h], 74697720h
loc_1_2A71F:
        push   70726720h
        dec    ebp
        insd
loc_1_2A727:
        inc    ebx
        outsb
        je     SHORT loc_1_2A768
        and    eax, 0a64h
loc_1_2A730:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
loc_1_2A737:
        inc    ebx
        dec    edi
        db  070h, 065h
        outsb
        push   ebx
        inc    ecx
        push   eax
        and    byte ptr [edi + 72h], ah
        outsd
        jne    SHORT loc_1_2A7B5
        and    byte ptr [ebx + 41h], dl
        push   eax
        and    byte ptr [eax + 72h], dh
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        and    byte ptr [edx + 65h], dh
        db  074h, 043h
        outsd
loc_1_2A757:
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2A760:
        push   ebx
        inc    ebp
        push   ebx
loc_1_2A763:
        cmp    ah, byte ptr [eax]
        push   ebx
        inc    ecx
        push   eax
loc_1_2A768:
        and    byte ptr [edi + 70h], cl
        outsb
        and    byte ptr fs:[61747320h], ch
        dec    ecx
        inc    esp
        cmp    eax, 73207725h
        popad
        db  070h, 056h
        popad
        insb
        jne    SHORT loc_1_2A7E8
        cmp    eax, 0a6225h
loc_1_2A788:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    edi
        jo     SHORT loc_1_2A7F8
        outsb
        push   ebx
        je     SHORT loc_1_2A7F8
        and    byte ptr [edx + 53h], dh
        inc    ecx
        push   eax
        push   esi
        popad
        insb
        jne    SHORT loc_1_2A806
        cmp    eax, 61206225h
        insb
        jb     SHORT loc_1_2A80E
        popad
        db  064h, 079h, 020h
        outsd
        jo     SHORT loc_1_2A815
        outsb
        or     al, byte ptr [eax]
        db  000h
loc_1_2A7B4:
        inc    ebp
loc_1_2A7B5:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    edi
        jo     SHORT loc_1_2A824
        outsb
        push   ebx
        je     SHORT loc_1_2A824
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [ecx + 76h], ah
        popad
        imul   ebp, dword ptr [ecx + 62h], 4c20656ch
        imul   ebp, dword ptr [esi + 6bh], 74000a73h
        imul   ebp, dword ptr [ebp + 65h], 315472h
loc_1_2A7E0:
        je     SHORT loc_1_2A84B
        insd
        jb     NEAR PTR loc_1_2A83A
        xor    al, byte ptr [eax]
loc_1_2A7E8:
        db  074h, 069h
        insd
        jb     NEAR PTR loc_1_2A842
        imul   eax, dword ptr [eax], 4f78616dh
        jne    SHORT loc_1_2A86A
        add    byte ptr [eax], al
loc_1_2A7F8:
        insd
        popad
        js     SHORT loc_1_2A845
        outsb
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], ch
        db  078h, 04Fh
        db  075h, 074h
loc_1_2A806:
        dec    ecx
        outsb
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2A80C:
        insd
        popad
loc_1_2A80E:
        js     SHORT loc_1_2A862
        db  065h, 074h, 072h
        db  079h, 043h
loc_1_2A815:
        outsb
        je     SHORT loc_1_2A818
loc_1_2A818:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    edi
        jo     SHORT loc_1_2A888
        outsb
loc_1_2A824:
        push   ebx
        je     SHORT loc_1_2A888
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        db  065h, 072h, 054h
        xor    ah, byte ptr [eax]
        and    byte ptr ds:[ecx + ebp*2 + 6dh], dh
        db  065h, 072h, 054h
        xor    dword ptr [edx], ecx
loc_1_2A83A:
        add    byte ptr [eax], al
loc_1_2A83C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2A842:
        dec    esp
        inc    ebx
        dec    edi
loc_1_2A845:
        db  070h, 065h
        outsb
        push   ebx
        db  074h, 061h
loc_1_2A84B:
        and    byte ptr [ecx + 6eh], ch
        jbe    SHORT loc_1_2A8B1
        insb
        imul   esp, dword ptr [eax + 72h], 56504153h
        popad
        insb
        jne    SHORT loc_1_2A8C2
        cmp    eax, 0a6225h
loc_1_2A862:
        add    byte ptr [eax], al
loc_1_2A864:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2A86A:
        dec    esp
        inc    ebx
        dec    edi
        db  070h, 065h
        outsb
        push   ebx
        db  074h, 061h
        and    byte ptr [ebp + 61h], ch
        db  078h, 049h
        imul   sp, word ptr [ebp + 6ch], 3d64h
        and    eax, 6f742064h
        outsd
        and    byte ptr [ecx + 72h], ch
loc_1_2A888:
        and    byte ptr gs:[di + 61h], ch
        js     SHORT loc_1_2A8E1
        arpl   word ptr [esi + 46h], si
        db  072h, 06Dh
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    edi
        jo     SHORT loc_1_2A90C
        outsb
        push   ebx
        je     SHORT loc_1_2A90C
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
loc_1_2A8B1:
        imul   esp, dword ptr [eax + 61h], 73656363h
        db  073h, 050h
        db  072h, 069h
        cmp    eax, 0a6225h
loc_1_2A8C2:
        add    byte ptr [eax], al
loc_1_2A8C4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        dec    edi
        jo     SHORT loc_1_2A934
        outsb
        push   ebx
        je     SHORT loc_1_2A934
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 64h], 41747365h
loc_1_2A8E1:
        jb     NEAR PTR loc_1_2A922
        and    eax, 0a42h
        add    byte ptr [eax], al
loc_1_2A8EC:
        push   ebx
        inc    ebp
        push   ebx
        cmp    ah, byte ptr [eax]
        push   ebx
        db  074h, 061h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edi + 70h], cl
        outsb
        and    byte ptr fs:[61747320h], ch
        dec    ecx
        inc    esp
        cmp    eax, 52207725h
loc_1_2A90C:
        push   ebx
        inc    ecx
        push   eax
        cmp    eax, 52206225h
        dec    esi
        outsd
        cmp    eax, 53004225h
        inc    ebp
        push   ebx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2A922:
        dec    esp
        inc    ebx
        and    byte ptr [edx + 65h], dl
        jae    SHORT loc_1_2A98E
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 70202d20h
        inc    ecx
        dec    edi
loc_1_2A934:
        inc    ebx
        inc    edx
        cmp    eax, 73206c25h
        db  074h, 061h
        dec    ecx
        inc    esp
        cmp    eax, 0a7725h
loc_1_2A944:
        push   ebx
        inc    ebp
        push   ebx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [edx + 65h], dl
        db  073h, 065h
        je     SHORT loc_1_2A9C7
        imul   ebp, dword ptr [esi + 67h], 70202d20h
        push   ebx
        inc    ecx
        push   eax
        cmp    eax, 73206c25h
        db  074h, 061h
        dec    ecx
        inc    esp
        cmp    eax, 0a7725h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        push   edx
        db  065h, 073h, 065h
        db  074h, 041h
        dec    edi
        inc    ebx
        inc    edx
        inc    ebx
        insd
        db  070h, 06Ch
        je     SHORT loc_1_2A9A3
        jo     SHORT loc_1_2A9C6
        dec    edi
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
        inc    ebx
loc_1_2A98E:
        inc    ebx
        inc    edx
        cmp    eax, 72206c25h
        db  065h, 074h, 043h
        outsd
        cmp    eax, 0a6225h
loc_1_2A9A0:
        push   ebx
        inc    ebp
        push   ebx
loc_1_2A9A3:
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [edx + 65h], dl
        db  073h, 065h
        je     SHORT loc_1_2A9CF
        arpl   word ptr [edi + 6dh], bp
        db  070h, 06Ch
        db  065h, 074h, 065h
        and    byte ptr [4f417020h], ch
        inc    ebx
        inc    edx
        cmp    eax, 73206c25h
        je     SHORT loc_1_2AA27
loc_1_2A9C6:
        dec    ecx
loc_1_2A9C7:
        inc    esp
        cmp    eax, 0a7725h
        add    byte ptr [eax], al
loc_1_2A9CF:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        push   edx
        jae    NEAR PTR loc_1_2AA41
        je     SHORT loc_1_2AA31
        inc    ecx
        push   eax
        inc    ebx
        insd
        db  070h, 06Ch
        je     SHORT loc_1_2AA06
        jo     SHORT loc_1_2AA3B
        inc    ecx
        push   eax
        cmp    eax, 70206c25h
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 72206c25h
        db  065h, 074h, 043h
        outsd
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2AA04:
        push   ebx
        inc    ebp
loc_1_2AA06:
        push   ebx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [edx + 65h], dl
        jae    SHORT loc_1_2AA76
        je     SHORT loc_1_2AA33
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_1_2AA84
        db  065h, 074h, 065h
        and    byte ptr [41537020h], ch
        push   eax
        cmp    eax, 73206c25h
loc_1_2AA27:
        db  074h, 061h
        dec    ecx
        inc    esp
        cmp    eax, 0a7725h
loc_1_2AA30:
        inc    ebp
loc_1_2AA31:
        push   edx
        push   edx
loc_1_2AA33:
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        push   ebx
        db  074h, 073h
loc_1_2AA3B:
        inc    ebx
        push   65676e61h
loc_1_2AA41:
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 65h], 746e6576h
        and    byte ptr [eax + 4ch], dh
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        insb
        arpl   word ptr [ebp + 76h], ax
        outsb
        je     SHORT loc_1_2AA9F
        and    eax, 0a77h
        add    byte ptr [ebx + 45h], dl
        push   ebx
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [ebx + 74h], dl
        popad
        je     SHORT loc_1_2AAEB
loc_1_2AA76:
        jae    SHORT loc_1_2AA98
        db  070h, 04Ch
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  073h, 074h
        popad
loc_1_2AA84:
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        insb
        arpl   word ptr [ebp + 76h], ax
        outsb
        je     SHORT loc_1_2AAD0
        and    eax, 0a77h
loc_1_2AA98:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
loc_1_2AA9F:
        inc    ebx
        push   ebx
        db  074h, 073h
        inc    ebx
        push   65676e61h
        and    byte ptr [ecx + 62h], al
        outsd
        jb     SHORT loc_1_2AB23
        imul   ebp, dword ptr [esi + 67h], 462d4920h
        db  072h, 061h
        insd
        db  065h, 073h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2AAC0:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 04Ch
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
loc_1_2AAD0:
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        insb
        arpl   word ptr [ebp + 76h], ax
        outsb
        db  074h, 03Dh
        and    eax, 0a77h
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2AAEB:
        cmp    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        push   ebx
        db  074h, 073h
        inc    ebx
        push   65676e61h
        and    byte ptr [esi + 72h], al
        popad
        insd
        and    byte ptr gs:[edx + 65h], dl
        push   65h
        arpl   word ptr [edx + ecx], si
loc_1_2AB08:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 04Ch
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        jae    SHORT loc_1_2AB8E
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        insb
loc_1_2AB23:
        arpl   word ptr [ebp + 76h], ax
        outsb
        db  074h, 03Dh
        and    eax, 72462077h
        insd
        jb     SHORT loc_1_2AB76
        popad
        je     SHORT loc_1_2AB96
        cmp    eax, 0a4225h
        add    byte ptr [eax], al
loc_1_2AB3C:
        push   ebx
        inc    ebp
        push   ebx
        cmp    ah, byte ptr [eax]
        push   eax
        outsd
        jae    SHORT loc_1_2ABB9
        imul   ebp, dword ptr [esi + 67h], 42434320h
        xor    ah, byte ptr [eax]
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [ebx + 74h], dl
        popad
        je     SHORT loc_1_2ABCC
        jae    SHORT loc_1_2AB63
        add    byte ptr [eax], al
        add    byte ptr [ebx + 45h], dl
        push   ebx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_1_2AB63:
        db  070h, 04Ch
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
loc_1_2AB76:
        insb
        arpl   word ptr [ebp + 76h], ax
        outsb
        je     SHORT loc_1_2ABBB
        and    eax, 0a77h
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ecx + 6eh], cx
loc_1_2AB8E:
        imul   esi, dword ptr [eax + ebp + 29h], 726c6120h
loc_1_2AB96:
        popad
        jns    NEAR PTR loc_1_2ABBB
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 45000a64h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ebx + 72h], ah
loc_1_2ABB9:
        popad
loc_1_2ABBB:
        db  074h, 065h
        and    byte ptr [esp + ecx*2 + 43h], al
        and    byte ptr [ebp + 75h], ch
        je     SHORT loc_1_2AC2B
        db  078h, 021h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2ABCC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ebx + 72h], ah
        popad
        je     SHORT loc_1_2AC46
        and    byte ptr [edx + esi*2 + 61h], dh
        arpl   word ptr [ebp + 20h], sp
        db  073h, 065h
        insd
        popad
        jo     SHORT loc_1_2AC56
        outsd
        jb     SHORT loc_1_2AC56
        and    dword ptr [edx], ecx
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ecx + 6eh], cx
        imul   esi, dword ptr [eax + ebp + 29h], 69646e20h
        jae    SHORT loc_1_2AC5E
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 0a6c253dh
        add    byte ptr [eax], al
loc_1_2AC14:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2AC8E
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ecx + 6eh], bp
        imul   esi, dword ptr [esi + ebp + 63h], 494e4900h
loc_1_2AC2B:
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        db  065h, 072h, 020h
        db  073h, 074h
        popad
        jb     SHORT loc_1_2ACB0
        and    byte ptr fs:[ecx + 6eh], ch
        db  074h, 065h
        db  072h, 076h
        popad
loc_1_2AC46:
        insb
        and    byte ptr [20642520h], bh
        insd
        imul   ebp, dword ptr [esp + ebp*2 + 69h], 6365732dh
loc_1_2AC56:
        jae    SHORT loc_1_2AC62
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2AC5C:
        dec    ecx
        dec    esi
loc_1_2AC5E:
        dec    ecx
        cmp    ah, byte ptr [eax]
        inc    ebx
loc_1_2AC62:
        inc    ebx
        inc    edx
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        db  065h, 072h, 020h
        jae    SHORT loc_1_2ACE1
        popad
        jb     SHORT loc_1_2ACE4
        and    byte ptr fs:[ecx + 6eh], ch
        je     SHORT loc_1_2ACDC
        db  072h, 076h
        popad
        insb
        and    byte ptr [20642520h], bh
        insd
        imul   ebp, dword ptr [esp + ebp*2 + 69h], 6365732dh
        db  073h, 00Ah
        add    byte ptr [eax], al
loc_1_2AC8E:
        add    byte ptr [eax], al
loc_1_2AC90:
        inc    esp
        dec    esp
        inc    ebx
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        jae    SHORT loc_1_2AD0E
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ecx + 6eh], bp
        imul   esi, dword ptr [esi + ebp + 63h], 5c2e2e00h
        pop    esp
        db  073h, 072h
loc_1_2ACB0:
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ecx + 6eh], bp
        db  069h, 074h, 02Eh, 063h, 000h
loc_1_2ACBC:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ecx + 6eh], bp
        imul   esi, dword ptr [esi + ebp + 63h], 73795300h
        je     SHORT loc_1_2AD3A
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_1_2AD41
loc_1_2ACDC:
        outsb
        db  074h, 043h
        outsd
        outsb
loc_1_2ACE1:
        db  074h, 072h
        outsd
loc_1_2ACE4:
        insb
        push   ebx
        je     NEAR PTR loc_1_2AD45
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        inc    esp
        insb
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2ACFC:
        inc    esi
        imul   esi, dword ptr [edx + 73h], 6d695474h
        add    byte ptr gs:[eax], al
        add    byte ptr [esi + 69h], al
        db  072h, 073h
        db  074h, 054h
loc_1_2AD0E:
        imul   ebp, dword ptr [ebp + 65h], 49000000h
        dec    esi
        dec    ecx
        cmp    cl, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    cl, byte ptr [edx]
        xor    dword ptr [esi], esi
        sub    eax, 20746962h
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [ebx + 75h], dh
        jo     SHORT loc_1_2AD9C
        outsd
        jb     SHORT loc_1_2ADA3
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebx + 65h], dh
        db  074h, 020h
        bound  esi, qword ptr [ebp + 74h]
loc_1_2AD3A:
        and    byte ptr [esp + ecx*2 + 43h], al
        dec    eax
        dec    esp
        push   eax
loc_1_2AD41:
        inc    ebp
        pop    eax
        inc    ebp
loc_1_2AD45:
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 62h], ah
        db  073h, 065h
        outsb
        je     SHORT loc_1_2AD7E
        or     ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2AD58:
        dec    ecx
        dec    edi
        inc    ebx
        cmp    ah, byte ptr [eax]
        inc    esp
        jbe    NEAR PTR loc_1_2ADCA
        arpl   word ptr [ebp + 49h], sp
        dec    edi
        inc    ebx
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        and    byte ptr [65724320h], ch
        popad
        db  074h, 065h
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 75716572h
loc_1_2AD7E:
        jae    NEAR PTR loc_1_2ADF5
        and    byte ptr [eax + 44h], ch
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 3dh], sp
        and    eax, 0a6ch
        add    byte ptr [ecx + 4fh], cl
        inc    ebx
        cmp    ah, byte ptr [eax]
        inc    esp
        jbe    NEAR PTR loc_1_2AE02
        arpl   word ptr [ebp + 49h], sp
loc_1_2AD9C:
        dec    edi
        inc    ebx
        outsd
        outsb
        je     SHORT loc_1_2AE14
        outsd
loc_1_2ADA3:
        insb
        and    byte ptr [6f6c4320h], ch
        jae    SHORT loc_1_2AE11
        dec    eax
        popad
        outsb
        insb
        and    byte ptr gs:[ecx + 67h], dh
        push   eax
        db  072h, 06Fh
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_1_2ADFB
        and    eax, 0a6ch
        db  000h
loc_1_2ADC4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2ADCA:
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 49h], sp
        dec    edi
        inc    ebx
        outsd
        outsb
        je     SHORT loc_1_2AE48
        outsd
        insb
        and    byte ptr [736e7520h], ch
        db  075h, 070h
        db  070h, 06Fh
        jb     SHORT loc_1_2AE58
        and    byte ptr fs:[ecx + ebp*2 + 6fh], ah
        arpl   word ptr [edx + 65h], dx
        jno    SHORT loc_1_2AE2C
        and    eax, 0a6ch
loc_1_2ADF4:
        inc    ebp
loc_1_2ADF5:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsd
loc_1_2ADFB:
        arpl   word ptr [esp + ebp*2 + 52h], si
        jno    NEAR PTR loc_1_2AE2A
loc_1_2AE02:
        sub    dword ptr [eax], esp
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [eax + esi*2 + 76h], ch
        dec    ecx
        outsb
loc_1_2AE11:
        inc    edx
        jne    SHORT loc_1_2AE7A
loc_1_2AE14:
        db  066h, 065h, 072h, 020h
        db  070h, 061h
        db  072h, 061h
        insd
        db  065h, 074h, 065h
        jb     SHORT loc_1_2AE2C
        add    byte ptr [eax], al
loc_1_2AE24:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        insb
loc_1_2AE2A:
        jo     SHORT loc_1_2AEA2
loc_1_2AE2C:
        dec    ecx
        outsb
        inc    edx
        jne    SHORT loc_1_2AE97
        jb     NEAR PTR loc_1_2AE72
        and    eax, 6263206ch
        dec    ecx
        outsb
        inc    edx
        db  075h, 066h
        db  066h, 065h, 072h, 03Dh
        and    eax, 0a64h
loc_1_2AE48:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsd
        arpl   word ptr [esp + ebp*2 + 52h], si
        db  065h, 071h, 028h
        sub    dword ptr [eax], esp
loc_1_2AE58:
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [ecx + 6fh], ch
        arpl   word ptr [esp + ebp*2 + 52h], si
        db  065h, 071h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_2AE70:
        inc    esp
        inc    edx
loc_1_2AE72:
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        db  079h, 06Eh
        popad
        insd
loc_1_2AE7A:
        imul   esp, dword ptr [ebx + 20h], 74697845h
        or     al, byte ptr [eax]
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        inc    ebx
        insb
        outsd
        db  073h, 065h
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 61207064h
loc_1_2AE97:
        popad
        jo     SHORT loc_1_2AF0F
        jb     NEAR PTR loc_1_2AEBE
        arpl   word ptr [edi + ebp*2 + 73h], bp
loc_1_2AEA2:
        and    byte ptr fs:[eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2AEFF
        jne    SHORT loc_1_2AF20
        cmp    eax, 0a6425h
loc_1_2AEB8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    ebx
loc_1_2AEBE:
        insb
        outsd
        jae    SHORT loc_1_2AF27
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 63207064h
        popad
        outsb
        daa
        db  074h, 020h
        arpl   word ptr [edi + ebp*2 + 73h], bp
        and    byte ptr gs:[eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2AF30
        jne    SHORT loc_1_2AF51
        cmp    eax, 73206425h
        db  074h, 061h
        je     SHORT loc_1_2AF62
        db  073h, 03Dh
        and    eax, 0a6ch
loc_1_2AEF4:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 69h], ax
        outsb
loc_1_2AEFF:
        inc    ecx
        popad
        jo     SHORT loc_1_2AF79
        jb     NEAR PTR loc_1_2AF30
        sub    dword ptr [eax], esp
        push   70617257h
loc_1_2AF0F:
        db  070h, 065h
        db  072h, 043h
        outsd
        outsb
        je     SHORT loc_1_2AF7C
        js     SHORT loc_1_2AF8D
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_2AF20:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        db  070h, 041h
loc_1_2AF27:
        popad
        jo     SHORT loc_1_2AF9F
        jb     NEAR PTR loc_1_2AF7C
        popad
        insd
loc_1_2AF30:
        cmp    eax, 53205325h
        jo     SHORT loc_1_2AF9D
        arpl   word ptr [ecx + 66h], bp
        imul   esp, dword ptr [ebx + 31h], 0a53253dh
        add    byte ptr [eax], al
loc_1_2AF44:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 69h], ax
        outsb
        inc    ecx
loc_1_2AF51:
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_2AF80
        sub    dword ptr [eax], esp
        popad
        popad
        db  070h, 074h
        and    byte ptr [ebx + 65h], ch
loc_1_2AF62:
        jns    SHORT loc_1_2AFA1
        and    eax, 45000a73h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 69h], ax
        outsb
        inc    ecx
        popad
        db  070h, 074h
loc_1_2AF79:
        jb     NEAR PTR loc_1_2AF9C
loc_1_2AF7C:
        arpl   word ptr [ecx + 6eh], sp
        daa
loc_1_2AF80:
        je     SHORT loc_1_2AFA2
        je     NEAR PTR loc_1_2AFA6
        arpl   word ptr [esi + 67h], sp
        and    byte ptr cs:[ebx + 65h], ch
loc_1_2AF8D:
        db  079h, 020h
        sub    eax, 526d6320h
        inc    ebx
        cmp    eax, 78383025h
        or     al, byte ptr [eax]
loc_1_2AF9C:
        inc    ebp
loc_1_2AF9D:
        push   edx
        push   edx
loc_1_2AF9F:
        cmp    ah, byte ptr [eax]
loc_1_2AFA1:
        inc    esp
loc_1_2AFA2:
        insb
        arpl   word ptr [edx + 69h], ax
loc_1_2AFA6:
        outsb
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        inc    ebx
        dec    ebp
        pop    edi
        inc    edi
        je     NEAR PTR loc_1_2B016
        push   eax
        popad
        jb     SHORT loc_1_2B020
        outsb
        db  074h, 028h
        sub    dword ptr [eax], esp
        popaw
        imul   ebp, dword ptr [ebp + 64h], 63202d20h
        insd
        push   edx
        inc    ebx
        cmp    eax, 78383025h
        or     al, byte ptr [eax]
loc_1_2AFD4:
        inc    ebx
        popad
        outsb
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_2AFFE
        imul   ebp, dword ptr [esi + 73h], 636e6174h
        and    byte ptr gs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2AFF8:
        pop    esp
        pop    esp
loc_1_2AFFE:
        jae    SHORT loc_1_2B072
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [esi + 64h], bp
        imul   esi, dword ptr [ebx + 2eh], 52450063h
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 69h], ax
loc_1_2B016:
        outsb
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 028h
loc_1_2B020:
        sub    dword ptr [eax], esp
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[ecx + 43h], al
        inc    edx
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2B038:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 69h], ax
        outsb
        inc    ecx
        popad
        jo     SHORT loc_1_2B0BD
        db  065h, 072h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_2B072
        arpl   word ptr [edi + 70h], bp
        db  079h, 020h
        popad
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_2B07F
        outsb
        popad
        insd
        or     al, byte ptr gs:[eax]
        add    byte ptr [eax], al
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 6ch], ax
loc_1_2B072:
        outsd
        db  073h, 065h
        inc    ecx
        popad
        jo     SHORT loc_1_2B0EE
        jb     NEAR PTR loc_1_2B0C0
        outsd
        insd
loc_1_2B07F:
        db  070h, 06Ch
        je     NEAR PTR loc_1_2B0E9
        sub    byte ptr [ecx], ch
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        jne    SHORT loc_1_2B102
        cmp    eax, 73206425h
        je     SHORT loc_1_2B0FD
        db  074h, 075h
        jae    SHORT loc_1_2B0DD
        and    eax, 45000a6ch
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 6ch], ax
        outsd
        db  073h, 065h
        inc    ecx
        popad
        jo     SHORT loc_1_2B12A
        jb     NEAR PTR loc_1_2B0FC
        outsd
        insd
        db  070h, 06Ch
loc_1_2B0BD:
        je     NEAR PTR loc_1_2B125
loc_1_2B0C0:
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 6ch], ah
        outsd
        db  073h, 065h
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 0ah
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_2B11E
loc_1_2B0DD:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2B135
        db  075h, 06Dh
loc_1_2B0E9:
        cmp    eax, 6e206425h
loc_1_2B0EE:
        imul   esi, dword ptr fs:[ebx + 53h], 73756174h
        cmp    eax, 0a6c25h
        db  000h
loc_1_2B0FC:
        inc    esp
loc_1_2B0FD:
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    edi
loc_1_2B102:
        jo     SHORT loc_1_2B169
        outsb
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 043h
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        and    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr fs:[eax + 41h], dh
loc_1_2B11E:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
loc_1_2B125:
        db  064h, 070h, 04Eh
        db  075h, 06Dh
loc_1_2B12A:
        cmp    eax, 0a6425h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2B135:
        dec    edi
        db  070h, 065h
        outsb
        inc    ecx
        popad
        jo     SHORT loc_1_2B1B2
        jb     NEAR PTR loc_1_2B184
        outsd
        insd
        jo     SHORT loc_1_2B1B1
        db  065h, 074h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr [ecx + 64h], ah
        popad
        jo     SHORT loc_1_2B1C9
        db  065h, 072h, 020h
        popaw
        imul   ebp, dword ptr [ebp + 64h], 5245000ah
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 041h
loc_1_2B169:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 6e206425h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        db  073h, 03Dh
loc_1_2B184:
        and    eax, 706f206ch
        outsb
        inc    ebp
        db  072h, 072h
        outsd
        jb     SHORT loc_1_2B1CE
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_2B198:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [esi + 64h], bp
        imul   esi, dword ptr [ebx + 2eh], 52450063h
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2B1B1:
        inc    esp
loc_1_2B1B2:
        insb
        arpl   word ptr [edx + 65h], dx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[edi + ebp*2 + 6fh], ch
        imul   esp, dword ptr [ecx + 68h], 65h
loc_1_2B1C9:
        popad
        inc    edx
        jne    SHORT loc_1_2B234
loc_1_2B1CE:
        dec    esp
        outsb
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_2B1D8:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2B252
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [esi + 64h], bp
        imul   esi, dword ptr [ebx + 2eh], 2e2e0063h
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [esi + 64h], bp
        imul   esi, dword ptr [ebx + 2eh], 43520063h
        push   esi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [edx + 43h], dl
        push   esi
        and    byte ptr [eax], ah
        and    byte ptr [eax + 64h], ch
        db  072h, 042h
        jne    SHORT loc_1_2B28E
        dec    esp
        outsb
        cmp    eax, 70206425h
        popad
        arpl   word ptr [ebx + 65h], bp
loc_1_2B234:
        db  074h, 04Ch
        outsb
        cmp    eax, 6c206425h
        outsd
        outsd
        imul   esp, dword ptr [ecx + 68h], 65h
        popad
        inc    edx
        jne    SHORT loc_1_2B2AE
        dec    esp
        outsb
        cmp    eax, 0a6425h
loc_1_2B250:
        inc    ebp
        push   edx
loc_1_2B252:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_2B2C4
        sub    byte ptr [ecx], ch
        and    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 54h], 736e6172h
        jb     NEAR PTR loc_1_2B2B3
        popad
        je     SHORT loc_1_2B2D3
        and    byte ptr [esi + 61h], ah
        db  069h, 06Ch, 065h, 064h, 00Ah, 000h, 000h
loc_1_2B27C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_2B2C4
        inc    ebx
        inc    edx
        cmp    eax, 73206c25h
        je     SHORT loc_1_2B2ED
        db  074h, 075h
loc_1_2B28E:
        db  073h, 03Dh
        and    eax, 7962206ch
        db  074h, 065h
        jae    SHORT loc_1_2B2F1
        jb     NEAR PTR loc_1_2B2DA
        and    eax, 0a64h
        add    byte ptr [eax], al
loc_1_2B2A4:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
loc_1_2B2AE:
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_2B318
loc_1_2B2B3:
        sub    byte ptr [ecx], ch
        and    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 54h], 736e6172h
        jb     NEAR PTR loc_1_2B307
        popad
loc_1_2B2C4:
        je     SHORT loc_1_2B327
        and    byte ptr [eax + 65h], dh
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 4352000ah
        push   esi
loc_1_2B2D3:
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
loc_1_2B2DA:
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [esi + 72h], ah
        popad
        insd
        and    byte ptr gs:[ecx + 75h], dh
        db  065h, 075h, 065h
loc_1_2B2ED:
        and    byte ptr fs:[eax + 41h], dh
loc_1_2B2F1:
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_2B2F8:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_2B36C
loc_1_2B307:
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        je     NEAR PTR loc_1_2B374
        sub    byte ptr [ecx], ch
        and    byte ptr [ebp + 6eh], ah
        db  074h, 072h
        jns    SHORT loc_1_2B338
loc_1_2B318:
        jo     SHORT loc_1_2B35B
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        add    byte ptr [edx + eax*2 + 47h], al
loc_1_2B327:
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
        db  071h, 075h
        db  065h, 073h, 074h
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
loc_1_2B338:
        je     NEAR PTR loc_1_2B3A0
        sub    byte ptr [ecx], ch
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        jae    SHORT loc_1_2B38E
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_2B358:
        inc    ebp
        push   edx
        push   edx
loc_1_2B35B:
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
        jno    SHORT loc_1_2B3D9
        db  065h, 073h, 074h
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
loc_1_2B36C:
        je     NEAR PTR loc_1_2B3D4
        sub    byte ptr [ecx], ch
        and    byte ptr [edx + 65h], dh
loc_1_2B374:
        db  071h, 075h
        db  065h, 073h, 074h
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 0ah
loc_1_2B384:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 041h
        inc    ebx
loc_1_2B38E:
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2B3E5
        db  075h, 06Dh
        cmp    eax, 70206425h
        inc    esp
        insb
loc_1_2B3A0:
        arpl   word ptr [esi + 64h], cx
        imul   esi, dword ptr [ebx + 52h], 253d7165h
        insb
        and    byte ptr [ebx + 74h], dh
        popad
        db  075h, 073h
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_2B3B8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
        db  073h, 065h
        db  074h, 043h
        outsd
        insd
        jo     SHORT loc_1_2B436
        je     NEAR PTR loc_1_2B432
        sub    byte ptr [ecx], ch
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
loc_1_2B3D4:
        cmp    eax, 61206c25h
loc_1_2B3D9:
        db  064h, 070h, 04Eh
        jne    SHORT loc_1_2B44B
        cmp    eax, 73206425h
        db  074h, 061h
loc_1_2B3E5:
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_2B3F0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + 65h], dx
        jae    SHORT loc_1_2B461
        db  074h, 043h
        outsd
        insd
        jo     SHORT loc_1_2B46E
        db  065h, 074h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [edx + 65h], dh
        db  073h, 065h
        db  074h, 020h
        popaw
        imul   ebp, dword ptr [ebp + 64h], 0ah
loc_1_2B418:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2B479
        jne    SHORT loc_1_2B49A
        cmp    eax, 73206425h
loc_1_2B432:
        je     SHORT loc_1_2B495
        jne    SHORT loc_1_2B4A9
loc_1_2B436:
        cmp    eax, 0a6c25h
        db  000h
loc_1_2B43C:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2B4B6
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [esi + 64h], bp
loc_1_2B44B:
        imul   esi, dword ptr [ebx + 2eh], 42440063h
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 65h], dx
        outsb
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
loc_1_2B461:
        je     NEAR PTR loc_1_2B4C9
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
loc_1_2B46E:
        push   eax
        imul   esi, dword ptr [ebp + edi + 25h], 6ch
        and    byte ptr [ebp + 76h], ah
        outsb
loc_1_2B479:
        db  074h, 03Dh
        and    eax, 646e206ch
        imul   esi, dword ptr [ebx + 53h], 75746174h
        jae    SHORT loc_1_2B4C6
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_2B490:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2B495:
        inc    esp
        insb
        arpl   word ptr [ebx + 65h], dx
loc_1_2B49A:
        outsb
        inc    ebx
        outsd
        insd
        jo     SHORT loc_1_2B50D
        je     NEAR PTR loc_1_2B509
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 65h], dh
loc_1_2B4A9:
        outsb
        and    byte ptr fs:[esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 5245000ah
loc_1_2B4B6:
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
        push   eax
        popad
loc_1_2B4C6:
        arpl   word ptr [ebx + 65h], bp
loc_1_2B4C9:
        je     SHORT loc_1_2B508
        and    eax, 646e206ch
        imul   esi, dword ptr [ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_2B4E0:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 65h], dx
        outsb
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        and    byte ptr [ebp + 76h], ah
        outsb
        je     SHORT loc_1_2B51B
        outsb
        outsd
        db  074h, 020h
        outsw
        db  075h, 06Eh
        or     al, byte ptr fs:[eax]
        add    byte ptr [eax], al
loc_1_2B508:
        inc    esp
loc_1_2B509:
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
loc_1_2B50D:
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        je     SHORT loc_1_2B58A
        jae    SHORT loc_1_2B53F
        sub    dword ptr [eax], esp
        db  070h, 041h
loc_1_2B51B:
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        imul   esi, dword ptr fs:[ebx + 47h], 74536e65h
        popad
        je     SHORT loc_1_2B5A2
        db  073h, 03Dh
        and    eax, 7562206ch
        cmp    ax, 6c25h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2B53C:
        inc    esp
        inc    edx
        inc    edi
loc_1_2B53F:
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        db  074h, 075h
        jae    SHORT loc_1_2B573
        sub    dword ptr [eax], esp
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        imul   esi, dword ptr fs:[ebx + 47h], 74536e65h
        popad
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 0a6ch
loc_1_2B568:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
loc_1_2B573:
        db  074h, 075h
        jae    SHORT loc_1_2B59F
        sub    dword ptr [eax], esp
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_2B59F
        db  067h, 065h, 074h, 020h
        inc    ecx
        push   ebp
        pop    eax
        pop    edi
        inc    ebx
        inc    ebx
        inc    edx
loc_1_2B58A:
        or     al, byte ptr [eax]
loc_1_2B58C:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        je     SHORT loc_1_2B60E
        jae    SHORT loc_1_2B5DE
        outsd
        insd
        jo     SHORT loc_1_2B60B
loc_1_2B59F:
        db  065h, 074h, 065h
loc_1_2B5A2:
        sub    byte ptr [ecx], ch
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_2B5B0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        je     SHORT loc_1_2B632
        jae    SHORT loc_1_2B604
        jbe    SHORT loc_1_2B626
        outsb
        je     SHORT loc_1_2B5E4
        inc    ebx
        insb
        outsd
        db  073h, 065h
        and    byte ptr fs:[eax], ch
        popad
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        arpl   word ptr [eax + 65h], bp
        arpl   word ptr [ebx + 29h], bp
        or     al, byte ptr [eax]
loc_1_2B5DC:
        inc    esp
        inc    edx
loc_1_2B5DE:
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
loc_1_2B5E4:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 73206425h
        je     SHORT loc_1_2B658
        db  074h, 075h
        jae    SHORT loc_1_2B638
        and    eax, 6461206ch
        db  070h, 053h
        je     SHORT loc_1_2B665
loc_1_2B604:
        je     SHORT loc_1_2B66B
        cmp    eax, 0a7725h
loc_1_2B60B:
        add    byte ptr [ebx + 4ch], al
loc_1_2B60E:
        dec    edi
        push   ebx
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
loc_1_2B614:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        je     SHORT loc_1_2B696
        db  073h, 045h
        db  076h, 065h
        outsb
loc_1_2B626:
        db  074h, 020h
        dec    edi
        outsb
        insb
        imul   ebp, dword ptr [esi + 65h], 4244000ah
loc_1_2B632:
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
loc_1_2B638:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 73206425h
        je     SHORT loc_1_2B6AC
        je     SHORT loc_1_2B6C2
        jae    SHORT loc_1_2B68C
        and    eax, 6461206ch
        db  070h, 053h
        je     SHORT loc_1_2B6B9
loc_1_2B658:
        je     SHORT loc_1_2B6BF
        cmp    eax, 0a7725h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2B665:
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
loc_1_2B66B:
        db  074h, 075h
        db  073h, 045h
        jbe    SHORT loc_1_2B6D6
        outsb
        db  074h, 020h
        push   edx
        db  065h, 073h, 065h
        db  074h, 020h
        push   ebx
        db  074h, 061h
        db  072h, 074h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
loc_1_2B68C:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2B6E4
loc_1_2B696:
        db  075h, 06Dh
        cmp    eax, 73206425h
        je     SHORT loc_1_2B700
        je     SHORT loc_1_2B716
        db  073h, 03Dh
        and    eax, 6461206ch
        db  070h, 053h
        db  074h, 061h
loc_1_2B6AC:
        je     SHORT loc_1_2B713
        cmp    eax, 0a7725h
        add    byte ptr [edx + 45h], dl
        push   ebx
        inc    ebp
        push   esp
loc_1_2B6B9:
        add    byte ptr [eax], al
        db  000h
loc_1_2B6BC:
        inc    ebp
        push   edx
        push   edx
loc_1_2B6BF:
        cmp    ah, byte ptr [eax]
        inc    esp
loc_1_2B6C2:
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        db  074h, 075h
        db  073h, 045h
        db  076h, 065h
        outsb
        je     SHORT loc_1_2B6F0
        push   edx
        db  065h, 073h, 065h
        db  074h, 020h
loc_1_2B6D6:
        inc    ebp
        outsb
        or     al, byte ptr fs:[eax]
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
loc_1_2B6E4:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
loc_1_2B6F0:
        cmp    eax, 73206425h
        je     SHORT loc_1_2B758
        je     SHORT loc_1_2B76E
        db  073h, 03Dh
        and    eax, 6461206ch
loc_1_2B700:
        db  070h, 053h
        je     SHORT loc_1_2B765
        db  074h, 065h
        cmp    eax, 0a7725h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
loc_1_2B713:
        arpl   word ptr [ebx + 74h], dx
loc_1_2B716:
        popad
        db  074h, 075h
        db  073h, 045h
        jbe    SHORT loc_1_2B782
        outsb
        je     SHORT loc_1_2B740
        inc    ebx
        db  072h, 069h
        db  074h, 069h
        arpl   word ptr [ecx + 6ch], sp
        and    byte ptr [edx + 69h], dl
        outsb
        and    byte ptr [bp + di + 74h], dh
        popad
        db  074h, 075h
        db  073h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
loc_1_2B740:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 72206425h
        imul   ebp, dword ptr [esi + 67h], 61747320h
loc_1_2B758:
        db  074h, 075h
        jae    SHORT loc_1_2B799
        and    eax, 6461206ch
        db  070h, 053h
        db  074h, 061h
loc_1_2B765:
        db  074h, 065h
        cmp    eax, 0a7725h
loc_1_2B76C:
        inc    esp
        inc    edx
loc_1_2B76E:
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        db  074h, 075h
        jae    SHORT loc_1_2B7C0
        jbe    SHORT loc_1_2B7E2
        outsb
        db  074h, 020h
        outsb
        outsd
loc_1_2B782:
        outsb
        sub    eax, 74697243h
        imul   esp, dword ptr [ebx + 61h], 6952206ch
        outsb
        and    byte ptr [bp + di + 74h], dh
        popad
        db  074h, 075h
        db  073h, 00Ah
loc_1_2B799:
        add    byte ptr [eax], al
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 72206425h
        imul   ebp, dword ptr [esi + 67h], 61747320h
        db  074h, 075h
        db  073h, 03Dh
loc_1_2B7C0:
        and    eax, 6461206ch
        jo     SHORT loc_1_2B81A
        je     SHORT loc_1_2B82A
        db  074h, 065h
        cmp    eax, 0a7725h
loc_1_2B7D0:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
        popad
        je     SHORT loc_1_2B852
        db  073h, 045h
        db  076h, 065h
        outsb
loc_1_2B7E2:
        je     SHORT loc_1_2B804
        dec    ebp
        inc    ecx
        pop    ecx
        pop    edi
        inc    ebx
        dec    esp
        dec    edi
        push   ebx
        inc    ebp
        or     al, byte ptr [eax]
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2B850
        db  075h, 06Dh
loc_1_2B804:
        cmp    eax, 73206425h
        je     SHORT loc_1_2B86C
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 6461206ch
        db  070h, 053h
        je     SHORT loc_1_2B879
        db  074h, 065h
loc_1_2B81A:
        cmp    eax, 0a7725h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebx + 74h], dx
loc_1_2B82A:
        popad
        je     SHORT loc_1_2B8A2
        db  073h, 045h
        db  076h, 065h
        outsb
        db  074h, 020h
        db  075h, 06Eh
        jb     SHORT loc_1_2B89D
        arpl   word ptr [edi + 67h], bp
        outsb
        imul   edi, dword ptr [edx + 65h], 76652064h
        outsb
        je     SHORT loc_1_2B851
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
loc_1_2B850:
        inc    ebx
loc_1_2B851:
        inc    edx
loc_1_2B852:
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_2B8A8
        db  075h, 06Dh
        cmp    eax, 73206425h
        db  074h, 061h
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 6461206ch
loc_1_2B86C:
        jo     SHORT loc_1_2B8C1
        db  074h, 061h
        je     SHORT loc_1_2B8D7
        cmp    eax, 0a7725h
        add    byte ptr [esi], ch
loc_1_2B879:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2B8F2
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [esi + 64h], bp
        imul   esi, dword ptr [ebx + 2eh], 43520063h
        push   esi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + esi*2 + 61h], dx
        outsb
        jae    SHORT loc_1_2B900
        db  065h, 072h, 044h
loc_1_2B89D:
        popad
        db  074h, 061h
        inc    ebx
        outsd
loc_1_2B8A2:
        insd
        jo     SHORT loc_1_2B911
        db  065h, 074h, 065h
loc_1_2B8A8:
        sub    byte ptr [ecx], ch
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_2B8B4:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        bound  edi, qword ptr [ecx + 74h]
        jae    NEAR PTR loc_1_2B919
loc_1_2B8C1:
        jb     NEAR PTR loc_1_2B902
        and    eax, 5270206ch
        arpl   word ptr [esi + 45h], si
        jbe    SHORT loc_1_2B934
        outsb
        je     SHORT loc_1_2B90F
        and    eax, 646e206ch
loc_1_2B8D7:
        imul   esi, dword ptr [ebx + 53h], 75746174h
        jae    SHORT loc_1_2B91D
        and    eax, 52000a6ch
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [edx + esi*2 + 61h], dx
        outsb
        db  073h, 066h
loc_1_2B8F2:
        jb     NEAR PTR loc_1_2B939
        popad
        je     SHORT loc_1_2B959
        inc    ebx
        outsd
        insd
        jo     SHORT loc_1_2B969
        db  065h, 074h, 065h
loc_1_2B900:
        sub    byte ptr [ecx], ch
loc_1_2B902:
        and    byte ptr [esi + 72h], ah
        popad
        insd
        and    byte ptr gs:[ecx + 27h], dh
        and    byte ptr fs:[eax + 41h], dh
loc_1_2B90F:
        inc    ebx
        inc    edx
loc_1_2B911:
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_2B918:
        inc    ebp
loc_1_2B919:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2B91D:
        inc    esp
        insb
        arpl   word ptr [edx + esi*2 + 61h], dx
        outsb
        db  073h, 066h
        jb     NEAR PTR loc_1_2B96D
        popad
        je     SHORT loc_1_2B98D
        inc    ebx
        outsd
        insd
        jo     SHORT loc_1_2B99D
        je     NEAR PTR loc_1_2B999
loc_1_2B934:
        sub    byte ptr [ecx], ch
        and    byte ptr [esi + 61h], ah
loc_1_2B939:
        imul   ebp, dword ptr [ebp + 64h], 43417020h
        inc    edx
        cmp    eax, 0a6c25h
        db  000h
loc_1_2B948:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebp + 6eh], dx
        bound  ebp, qword ptr [ecx + 6eh]
        inc    ecx
        popad
loc_1_2B959:
        jo     SHORT loc_1_2B9CF
        jb     NEAR PTR loc_1_2B986
        sub    dword ptr [eax], esp
        jo     SHORT loc_1_2B9A3
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_2B969:
        add    byte ptr [eax], al
        db  000h
loc_1_2B96C:
        inc    ebp
loc_1_2B96D:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebp + 6eh], dx
        bound  ebp, qword ptr [ecx + 6eh]
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_2B9AA
        sub    dword ptr [eax], esp
        jo     SHORT loc_1_2B9C7
loc_1_2B986:
        inc    ebx
        inc    edx
        cmp    eax, 69206c25h
loc_1_2B98D:
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [edx + ecx], 55000000h
loc_1_2B999:
        dec    esi
        inc    edx
        dec    ecx
        dec    esi
loc_1_2B99D:
        inc    esp
        add    byte ptr [eax], al
loc_1_2B9A0:
        inc    esp
        inc    edx
        inc    edi
loc_1_2B9A3:
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebp + 6eh], dx
loc_1_2B9AA:
        insb
        outsd
        popad
        sub    byte ptr fs:[ecx], ch
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2B9B4:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    esp
        insb
        arpl   word ptr [ebp + 6eh], dx
        insb
        outsd
        popad
        sub    byte ptr fs:[ecx], ch
        and    byte ptr [esi + 44h], cl
loc_1_2B9C7:
        dec    ecx
        push   ebx
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
loc_1_2B9CF:
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [ebp + 72h], ah
        imul   esi, dword ptr gs:[bp + di + 74h], 64657265h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esi
        imul   esi, dword ptr fs:[ebx + 44h], 67657265h
        imul   esi, dword ptr [ebx + 74h], 72507265h
        outsd
        je     SHORT loc_1_2BA6B
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 6e202d20h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        jae    SHORT loc_1_2BA51
        and    eax, 44000a6ch
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_2BA60
        inc    ebx
        inc    edx
        cmp    eax, 6d206c25h
        popad
        db  078h, 052h
        arpl   word ptr [esi + 46h], si
        jb     SHORT loc_1_2BA9B
        cmp    eax, 0a6425h
        db  000h
loc_1_2BA34:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 041h
        jo     NEAR PTR loc_1_2BA89
        outsb
        outsw
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_2BA70
        insd
loc_1_2BA51:
        popad
        js     SHORT loc_1_2BA74
        db  070h, 061h
        arpl   word ptr [ebx + 65h], bp
        db  074h, 020h
        insb
        outsb
        or     al, byte ptr [eax]
loc_1_2BA60:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 041h
        inc    ebx
        inc    edx
loc_1_2BA6B:
        cmp    eax, 6e206c25h
loc_1_2BA70:
        jae    NEAR PTR loc_1_2BADC
        push   ebx
loc_1_2BA74:
        je     SHORT loc_1_2BAD7
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        db  070h, 041h
        inc    ebx
        inc    edx
loc_1_2BA89:
        cmp    eax, 6c206c25h
        imul   ebp, dword ptr [esi + 6bh], 65657053h
        cmp    eax, 0a6425h
loc_1_2BA9B:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_2BAE6
        jo     NEAR PTR loc_1_2BAF1
        outsb
        outsw
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
        arpl   word ptr [ebp + 72h], si
        db  072h, 065h
        outsb
        je     SHORT loc_1_2BAE0
        popad
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_2BB16
        inc    ebx
        inc    edx
loc_1_2BAD7:
        cmp    eax, 6e206c25h
loc_1_2BADC:
        db  064h, 073h, 069h
        push   ebx
loc_1_2BAE0:
        je     SHORT loc_1_2BB43
        db  074h, 075h
        db  073h, 03Dh
loc_1_2BAE6:
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
loc_1_2BAF1:
        jo     SHORT loc_1_2BB34
        inc    ebx
        inc    edx
        cmp    eax, 63206c25h
        jne    SHORT loc_1_2BB6E
        jb     SHORT loc_1_2BB63
        outsb
        je     SHORT loc_1_2BB21
        jne    NEAR PTR loc_1_2BB72
        arpl   word ptr [ecx + 64h], ax
        jb     NEAR PTR loc_1_2BB47
        and    eax, 0a6ch
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
loc_1_2BB16:
        je     NEAR PTR loc_1_2BB5A
        jo     NEAR PTR loc_1_2BB65
        outsb
        outsw
        sub    byte ptr [ecx], ch
loc_1_2BB21:
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
        inc    esi
        db  075h, 06Eh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
loc_1_2BB34:
        popad
        insb
        and    byte ptr [ecx + 64h], ah
        jb     NEAR PTR loc_1_2BBA1
        db  073h, 073h
        or     al, byte ptr [eax]
loc_1_2BB40:
        inc    ebp
        push   edx
        push   edx
loc_1_2BB43:
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_1_2BB47:
        jo     SHORT loc_1_2BB8A
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        db  064h, 073h, 069h
        push   ebx
        je     SHORT loc_1_2BBB7
        db  074h, 075h
        db  073h, 03Dh
loc_1_2BB5A:
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
loc_1_2BB63:
        cmp    ah, byte ptr [eax]
loc_1_2BB65:
        jo     SHORT loc_1_2BBA8
        inc    ebx
        inc    edx
        cmp    eax, 63206c25h
loc_1_2BB6E:
        jne    SHORT loc_1_2BBE2
        jb     SHORT loc_1_2BBD7
loc_1_2BB72:
        outsb
        je     SHORT loc_1_2BB95
        db  067h, 072h, 06Fh
        jne    SHORT loc_1_2BBEA
        inc    ecx
        db  064h, 064h, 072h, 03Dh
        and    eax, 0a6ch
loc_1_2BB84:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
loc_1_2BB8A:
        db  065h, 074h, 041h
        jo     NEAR PTR loc_1_2BBD9
        outsb
        outsw
        sub    byte ptr [ecx], ch
loc_1_2BB95:
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_2BBC0
        inc    edi
loc_1_2BBA1:
        db  072h, 06Fh
        db  075h, 070h
        and    byte ptr [ecx + 64h], ah
loc_1_2BBA8:
        jb     NEAR PTR loc_1_2BC10
        jae    SHORT loc_1_2BC20
        or     al, byte ptr [eax]
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_1_2BBB7:
        jo     SHORT loc_1_2BBFA
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
loc_1_2BBC0:
        db  064h, 073h, 069h
        push   ebx
        je     SHORT loc_1_2BC27
        db  074h, 075h
        jae    SHORT loc_1_2BC07
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        db  070h, 041h
loc_1_2BBD7:
        inc    ebx
        inc    edx
loc_1_2BBD9:
        cmp    eax, 63206c25h
        db  075h, 072h
        jb     SHORT loc_1_2BC47
loc_1_2BBE2:
        outsb
        je     SHORT loc_1_2BC05
        outsb
        je     NEAR PTR loc_1_2BC60
        outsd
loc_1_2BBEA:
        db  072h, 06Bh
        push   ebx
        je     SHORT loc_1_2BC50
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 0a6ch
loc_1_2BBF8:
        inc    esp
        inc    edx
loc_1_2BBFA:
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 041h
        jo     NEAR PTR loc_1_2BC4D
        outsb
loc_1_2BC05:
        outsw
loc_1_2BC07:
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
loc_1_2BC10:
        je     NEAR PTR loc_1_2BC34
        db  072h, 069h
        outsb
        and    byte ptr [bp + di + 74h], dh
        popad
        je     SHORT loc_1_2BC93
        db  073h, 00Ah
loc_1_2BC20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BC24:
        inc    esp
        inc    edx
        inc    edi
loc_1_2BC27:
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_2BC6E
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
loc_1_2BC34:
        db  064h, 073h, 069h
        push   ebx
        db  074h, 061h
        db  074h, 075h
        jae    SHORT loc_1_2BC7B
        and    eax, 0a6ch
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2BC47:
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 041h
loc_1_2BC4D:
        jo     NEAR PTR loc_1_2BC99
loc_1_2BC50:
        outsb
        outsw
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_2BC80
loc_1_2BC60:
        db  072h, 069h
        outsb
        and    byte ptr [bp + di + 74h], dh
        popad
        je     SHORT loc_1_2BCDF
        db  073h, 00Ah
        add    byte ptr [eax], al
loc_1_2BC6E:
        add    byte ptr [eax], al
loc_1_2BC70:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_2BCBA
        inc    ebx
        inc    edx
loc_1_2BC7B:
        cmp    eax, 6e206c25h
loc_1_2BC80:
        db  064h, 073h, 069h
        push   ebx
        je     SHORT loc_1_2BCE7
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
loc_1_2BC93:
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_2BCD8
        inc    ebx
        inc    edx
loc_1_2BC99:
        cmp    eax, 70206c25h
        db  065h, 072h, 06Dh
        popad
        outsb
        outsb
        db  074h, 020h
        outsb
        outsd
        inc    ecx
        jb     NEAR PTR loc_1_2BCED
        and    eax, 45000042h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
loc_1_2BCBA:
        je     NEAR PTR loc_1_2BCFE
        jo     NEAR PTR loc_1_2BD09
        outsb
        outsw
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
        jo     SHORT loc_1_2BD37
        db  072h, 06Dh
        popad
        outsb
        outsb
loc_1_2BCD8:
        je     SHORT loc_1_2BCFA
        popad
        jb     NEAR PTR loc_1_2BD44
loc_1_2BCDF:
        db  073h, 073h
        or     al, byte ptr [eax]
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2BCE7:
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_2BD2E
loc_1_2BCED:
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        db  064h, 073h, 069h
        push   ebx
        je     SHORT loc_1_2BD5B
loc_1_2BCFA:
        db  074h, 075h
        db  073h, 03Dh
loc_1_2BCFE:
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
loc_1_2BD09:
        jo     SHORT loc_1_2BD4C
        inc    ebx
        inc    edx
        cmp    eax, 63206c25h
        jne    SHORT loc_1_2BD86
        jb     SHORT loc_1_2BD7B
        outsb
        je     SHORT loc_1_2BD39
        outsb
        outsd
        inc    ecx
        jb     NEAR PTR loc_1_2BD5F
        and    byte ptr [eax], ah
        and    eax, 45000042h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
loc_1_2BD2E:
        je     NEAR PTR loc_1_2BD72
        jo     NEAR PTR loc_1_2BD7D
        outsb
        outsw
loc_1_2BD37:
        sub    byte ptr [ecx], ch
loc_1_2BD39:
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
loc_1_2BD44:
        arpl   word ptr [ebp + 72h], si
        db  072h, 065h
        outsb
        je     SHORT loc_1_2BD6C
loc_1_2BD4C:
        popad
        jb     NEAR PTR loc_1_2BDB6
        jae    SHORT loc_1_2BDC6
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2BD5B:
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_1_2BD5F:
        jo     SHORT loc_1_2BDA2
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        db  064h, 073h, 069h
        push   ebx
loc_1_2BD6C:
        je     SHORT loc_1_2BDCF
        db  074h, 075h
        jae    SHORT loc_1_2BDAF
loc_1_2BD72:
        and    eax, 0a6ch
        db  000h
loc_1_2BD78:
        inc    esp
        inc    edx
        inc    edi
loc_1_2BD7B:
        cmp    ah, byte ptr [eax]
loc_1_2BD7D:
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 63206c25h
loc_1_2BD86:
        jne    SHORT loc_1_2BDFA
        jb     SHORT loc_1_2BDEF
        outsb
        je     SHORT loc_1_2BDAD
        insb
        outsd
        outsd
        imul   esp, dword ptr [ecx + 68h], 65h
        popad
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0a64253dh
        add    byte ptr [eax], al
loc_1_2BDA0:
        inc    ebp
        push   edx
loc_1_2BDA2:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 041h
        jo     NEAR PTR loc_1_2BDF5
        outsb
loc_1_2BDAD:
        outsw
loc_1_2BDAF:
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
loc_1_2BDB6:
        db  074h, 020h
        je     NEAR PTR loc_1_2BDDC
        insb
        outsd
        outsd
        imul   esp, dword ptr [ecx + 68h], 65h
        popad
        push   ebx
loc_1_2BDC6:
        imul   edi, dword ptr [edx + 65h], 4500000ah
        push   edx
        push   edx
loc_1_2BDCF:
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jo     SHORT loc_1_2BE16
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
loc_1_2BDDC:
        db  064h, 073h, 069h
        push   ebx
        je     SHORT loc_1_2BE43
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
loc_1_2BDEF:
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 041h
loc_1_2BDF5:
        db  064h, 070h, 053h
        je     SHORT loc_1_2BE5B
loc_1_2BDFA:
        db  074h, 073h
        and    byte ptr [ebx + 74h], dh
        popad
        db  074h, 069h
        jae    SHORT loc_1_2BE78
        imul   esp, dword ptr [ebx + 20h], 20746f6eh
        db  073h, 075h
        jo     SHORT loc_1_2BE7F
        outsd
        jb     SHORT loc_1_2BE86
        or     al, byte ptr fs:[eax]
loc_1_2BE16:
        add    byte ptr [eax], al
loc_1_2BE18:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_2BE60
        inc    ebx
        inc    edx
        cmp    eax, 73206c25h
        db  074h, 061h
        je     SHORT loc_1_2BE93
        jae    SHORT loc_1_2BEA0
        imul   esp, dword ptr [ebx + 20h], 44494f28h
        sub    dword ptr [69206c25h], edi
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_2BE40:
        inc    ebp
        push   edx
        push   edx
loc_1_2BE43:
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_2BE8A
        db  064h, 070h, 053h
        je     SHORT loc_1_2BEAF
        je     SHORT loc_1_2BEC3
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
loc_1_2BE5B:
        db  073h, 074h
        popad
        db  074h, 069h
loc_1_2BE60:
        jae    SHORT loc_1_2BED6
        imul   esp, dword ptr [ebx + 20h], 42434170h
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_2BE70:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [esi + 64h], ch
loc_1_2BE78:
        imul   esi, dword ptr [ebx + 53h], 75746174h
loc_1_2BE7F:
        db  073h, 03Dh
        and    eax, 7320206ch
loc_1_2BE86:
        db  074h, 061h
        je     SHORT loc_1_2BEF3
loc_1_2BE8A:
        db  073h, 074h
        imul   esp, dword ptr [ebx + 20h], 44494f28h
loc_1_2BE93:
        sub    dword ptr [69206c25h], edi
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_2BEA0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_2BEEB
        db  075h, 066h
        inc    esp
        db  065h, 073h, 063h
loc_1_2BEAF:
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_2BED6
        db  067h, 065h, 074h, 020h
        bound  esi, qword ptr [ebp + 66h]
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [eax], sp
loc_1_2BEC3:
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        jae    SHORT loc_1_2BF13
loc_1_2BED6:
        and    eax, 0a6ch
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        jae    SHORT loc_1_2BF56
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [esi + 64h], bp
loc_1_2BEEB:
        imul   esi, dword ptr [ebx + 2eh], 52450063h
        push   edx
loc_1_2BEF3:
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 04Eh
        imul   esi, dword ptr fs:[ebx + 52h], 65757165h
        jae    SHORT loc_1_2BF77
        sub    byte ptr [ecx], ch
        and    byte ptr [edi + 61h], dh
        imul   esi, dword ptr [eax + 74h], 64656d69h
        and    byte ptr [edi + 75h], ch
loc_1_2BF13:
        db  074h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 050h
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
        arpl   word ptr [eax], sp
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_2BF4E
        db  067h, 065h, 074h, 020h
        db  070h, 06Bh
        je     SHORT loc_1_2BF56
        db  064h, 065h, 073h, 063h
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 6e206c25h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        db  073h, 03Dh
loc_1_2BF4E:
        and    eax, 0a6ch
        add    byte ptr [ebp + 52h], al
loc_1_2BF56:
        push   edx
        cmp    ah, byte ptr [eax]
        dec    edi
        db  070h, 065h
        outsb
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 69207064h
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 61h], 74706164h
        db  065h, 072h, 020h
        outsb
        popad
loc_1_2BF77:
        insd
        or     al, byte ptr gs:[eax]
        db  000h
loc_1_2BF7C:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    edi
        jo     SHORT loc_1_2BFE9
        outsb
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 6f207064h
        db  070h, 065h
        outsb
        and    byte ptr [edi + 4bh], cl
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 6d206425h
        imul   esi, dword ptr fs:[ebp + 6dh], 0a64253dh
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        dec    edi
        db  070h, 065h
        outsb
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 63207064h
        popad
        outsb
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
        popad
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        imul   ebp, dword ptr [esi + 66h], 0a6fh
        add    byte ptr [eax], al
loc_1_2BFE0:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    edi
        jo     SHORT loc_1_2C04D
        outsb
loc_1_2BFE9:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 74207064h
        imul   ebp, dword ptr [ebp + 65h], 2074756fh
        imul   edi, dword ptr [70206425h], 70616441h
        db  074h, 065h
        db  072h, 04Eh
        popad
        insd
        cmp    eax, 0a5325h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        dec    edi
        db  070h, 065h
        outsb
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 63207064h
        popad
        outsb
        daa
        db  074h, 020h
        outsd
        jo     SHORT loc_1_2C08F
        outsb
        and    byte ptr [ecx + 64h], ah
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        and    eax, 45000a53h
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        jae    SHORT loc_1_2C0B5
        popad
        db  074h, 075h
        db  073h, 03Dh
        and    eax, 706f206ch
        outsb
loc_1_2C04D:
        inc    ebp
        db  072h, 072h
        outsd
        db  072h, 03Dh
        and    eax, 0a6ch
loc_1_2C058:
        inc    esp
        dec    esp
        inc    ebx
        db  000h
loc_1_2C05C:
        inc    esp
        dec    esp
        inc    ebx
        db  000h
loc_1_2C060:
        dec    ecx
        dec    esi
        dec    ecx
        cmp    ah, byte ptr [eax]
        push   edx
        imul   esi, dword ptr gs:[bp + di + 74h], 644e7265h
        imul   esi, dword ptr [ebx + 44h], 2928636ch
        and    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        and    byte ptr [eax + 72h], dh
        outsd
        je     SHORT loc_1_2C0F0
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edx + 65h], dh
        imul   esi, dword ptr [bp + di + 74h], 64657265h
loc_1_2C08F:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   edx
        imul   esi, dword ptr gs:[bp + di + 74h], 644e7265h
        imul   esi, dword ptr [ebx + 44h], 2928636ch
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 6e202d20h
loc_1_2C0B5:
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 0a6ch
loc_1_2C0C4:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        push   edx
        jae    NEAR PTR loc_1_2C132
        db  074h, 04Eh
        imul   esi, dword ptr fs:[ebx + 41h], 61207064h
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_2C0FE
        db  072h, 065h
        db  073h, 065h
        db  074h, 020h
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
loc_1_2C0F0:
        jne    SHORT loc_1_2C15F
        cmp    eax, 0a6425h
        db  000h
loc_1_2C0F8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        push   edx
loc_1_2C0FE:
        db  065h, 073h, 065h
        db  074h, 04Eh
        imul   esi, dword ptr fs:[ebx + 41h], 6e207064h
        outsd
        db  074h, 020h
        db  072h, 065h
        db  073h, 065h
        db  074h, 061h
        bound  ebp, qword ptr [ebp + 20h]
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 0a6425h
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        push   edx
loc_1_2C132:
        jae    NEAR PTR loc_1_2C19A
        je     SHORT loc_1_2C185
        imul   esi, dword ptr fs:[ebx + 41h], 63207064h
        popad
        outsb
        daa
        db  074h, 020h
        db  072h, 065h
        jae    SHORT loc_1_2C1AD
        je     SHORT loc_1_2C16A
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 73206425h
        db  074h, 061h
loc_1_2C15F:
        je     SHORT loc_1_2C1D6
        jae    SHORT loc_1_2C1A0
        and    eax, 0a6ch
loc_1_2C168:
        inc    esp
        inc    edx
loc_1_2C16A:
        inc    edi
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_2C1B0
        inc    ebx
        inc    edx
        cmp    eax, 46206c25h
        db  075h, 06Eh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        popad
        outsb
        insb
        and    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
loc_1_2C185:
        db  073h, 073h
        and    byte ptr [ebx + 65h], dh
        db  074h, 020h
        sub    byte ptr [0a296ch], ah
        add    byte ptr [eax], al
loc_1_2C194:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
loc_1_2C19A:
        db  065h, 074h, 041h
        db  064h, 070h, 046h
loc_1_2C1A0:
        db  075h, 06Eh
        arpl   word ptr [ecx + 64h], ax
        jb     NEAR PTR loc_1_2C1D0
        sub    dword ptr [eax], esp
        arpl   word ptr [ecx + 6eh], sp
loc_1_2C1AD:
        daa
        je     SHORT loc_1_2C1D0
loc_1_2C1B0:
        db  073h, 065h
        db  074h, 020h
        inc    esi
        jne    SHORT loc_1_2C225
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        popad
        outsb
        insb
        and    byte ptr [ecx + 64h], al
        jb     NEAR PTR loc_1_2C229
        db  073h, 073h
        or     al, byte ptr [eax]
loc_1_2C1C8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        db  070h, 041h
        inc    ebx
loc_1_2C1D0:
        inc    edx
        cmp    eax, 47206c25h
loc_1_2C1D6:
        jb     SHORT loc_1_2C247
        db  075h, 070h
        and    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [ebx + 65h], dh
        je     SHORT loc_1_2C207
        sub    byte ptr [0a296ch], ah
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 041h
        jo     NEAR PTR loc_1_2C243
        jb     SHORT loc_1_2C26D
        db  075h, 070h
        inc    ecx
        db  064h, 064h, 072h, 028h
        sub    dword ptr [eax], esp
loc_1_2C207:
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        db  073h, 065h
        db  074h, 020h
        inc    edi
        db  072h, 06Fh
        db  075h, 070h
        and    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
        jae    SHORT loc_1_2C291
        or     al, byte ptr [eax]
loc_1_2C220:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
loc_1_2C225:
        jo     SHORT loc_1_2C268
        inc    ebx
        inc    edx
loc_1_2C229:
        cmp    eax, 4d206c25h
        db  075h, 06Ch
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        db  074h, 020h
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [ebx + 65h], dh
        db  074h, 020h
loc_1_2C243:
        insd
        arpl   word ptr [ecx + 4ch], sp
loc_1_2C247:
        imul   esi, dword ptr [ebx + 74h], 657a6953h
        cmp    eax, 250a6425h
        inc    edx
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2C258:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        je     NEAR PTR loc_1_2C2A2
        db  064h, 070h, 04Dh
        jne    SHORT loc_1_2C2D2
        db  074h, 069h
loc_1_2C268:
        arpl   word ptr [ecx + 73h], sp
        db  074h, 041h
loc_1_2C26D:
        jb     NEAR PTR loc_1_2C299
        sub    dword ptr [eax], esp
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_2C299
        jae    SHORT loc_1_2C2E0
        db  074h, 020h
        dec    ebp
        db  075h, 06Ch
        je     SHORT loc_1_2C2EB
        arpl   word ptr [ecx + 73h], sp
        db  074h, 020h
        inc    ecx
        jb     NEAR PTR loc_1_2C2F1
        db  073h, 073h
        or     al, byte ptr [eax]
loc_1_2C290:
        inc    esp
loc_1_2C291:
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        db  070h, 041h
        inc    ebx
        inc    edx
loc_1_2C299:
        cmp    eax, 70206c25h
        popad
        arpl   word ptr [ebx + 65h], bp
loc_1_2C2A2:
        je     SHORT loc_1_2C2C4
        imul   bp, word ptr [esp + esi*2 + 65h], 2072h
        db  073h, 065h
        je     SHORT loc_1_2C2CF
        sub    byte ptr [0a296ch], ah
        add    byte ptr [eax], al
        db  000h
loc_1_2C2B8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 041h
        db  064h, 070h, 050h
loc_1_2C2C4:
        imul   esi, dword ptr [esi + eax*2 + 69h], 6ch
        je     SHORT loc_1_2C330
        db  072h, 028h
        sub    dword ptr [eax], esp
loc_1_2C2CF:
        arpl   word ptr [ecx + 6eh], sp
loc_1_2C2D2:
        daa
        db  074h, 020h
        db  073h, 065h
        je     SHORT loc_1_2C2F9
        db  070h, 061h
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_2C300
loc_1_2C2E0:
        imul   bp, word ptr [esp + esi*2 + 65h], 0a72h
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2C2EB:
        cmp    ah, byte ptr [eax]
        push   ebx
        db  065h, 074h, 04Eh
loc_1_2C2F1:
        imul   esi, dword ptr fs:[ebx + 52h], 65757165h
loc_1_2C2F9:
        db  073h, 074h
        sub    byte ptr [ecx], ch
        and    byte ptr [edi + 61h], dh
loc_1_2C300:
        imul   esi, dword ptr [eax + 74h], 64656d69h
        and    byte ptr [edi + 75h], ch
        db  074h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        jae    SHORT loc_1_2C38A
        arpl   word ptr [ebp + 76h], bx
        outsb
        db  074h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2C324:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [ebp + 76h], bx
loc_1_2C330:
        outsb
        db  074h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2C338:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2C3B2
        arpl   word ptr [ebp + 76h], bx
        outsb
        je     SHORT loc_1_2C376
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2C34C:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2C3C6
        arpl   word ptr [ebp + 76h], bx
        outsb
        je     SHORT loc_1_2C38A
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2C360:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2C3DA
        arpl   word ptr [ebp + 76h], bx
        outsb
        db  074h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2C374:
        inc    ebp
        push   edx
loc_1_2C376:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 68637461h
        dec    esi
        imul   esi, dword ptr fs:[ebx + 45h], 746e6576h
loc_1_2C38A:
        sub    byte ptr [ecx], ch
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[ebp + 56h], al
        inc    ebp
        dec    esi
        push   esp
        pop    edi
        db  074h, 079h
        db  070h, 065h
        cmp    eax, 0a6225h
loc_1_2C3A0:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2C41A
        arpl   word ptr [ebp + 76h], bx
        outsb
        je     SHORT loc_1_2C3DE
        arpl   word ptr [eax], ax
loc_1_2C3B2:
        add    byte ptr [eax], al
loc_1_2C3B4:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [ebp + 76h], bx
        outsb
        je     SHORT loc_1_2C3F2
        arpl   word ptr [eax], ax
loc_1_2C3C6:
        add    byte ptr [eax], al
loc_1_2C3C8:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [ebp + 76h], bx
        outsb
        je     SHORT loc_1_2C406
        arpl   word ptr [eax], ax
loc_1_2C3DA:
        add    byte ptr [eax], al
loc_1_2C3DC:
        inc    ebp
        push   edx
loc_1_2C3DE:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        push   4243436bh
        sub    byte ptr [ecx], ch
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[ecx + 70h], ah
        db  070h, 06Ch
loc_1_2C3F2:
        dec    ecx
        inc    esp
        and    byte ptr [ebx + 43h], al
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
loc_1_2C406:
        push   4243436bh
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [edx + 75h], al
        and    byte ptr [eax + 6fh], dl
        outsd
        insb
        and    byte ptr [eax + 41h], dh
loc_1_2C41A:
        dec    edi
        inc    ebx
        inc    edx
        cmp    eax, 43206c25h
        inc    ebx
        inc    edx
        and    byte ptr [ebx + 6dh], ah
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [esi + ebp + 63h], 76724400h
        db  072h, 046h
        popad
        je     SHORT loc_1_2C4AD
        insb
        inc    ebp
        js     SHORT loc_1_2C4B9
        db  074h, 028h
        sub    dword ptr [eax], esp
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ah, byte ptr [eax]
        push   edx
        arpl   word ptr gs:[ebp + 72h], si
        jae    SHORT loc_1_2C4CA
        jbe    SHORT loc_1_2C4C8
        and    byte ptr [ebp + 6eh], ah
        db  074h, 072h
        db  079h, 021h
        and    byte ptr [edx], ch
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        db  025h, 021h, 00Ah, 000h
loc_1_2C478:
        and    eax, 0a732521h
        add    byte ptr [eax], al
        add    byte ptr [20746121h], ah
        insb
        imul   ebp, dword ptr [esi + 65h], 20642520h
        imul   ebp, dword ptr [esi + 20h], 656c6966h
        and    byte ptr [0a73h], ah
        add    byte ptr [eax], al
loc_1_2C49C:
        and    eax, 2a2a2a21h
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
loc_1_2C4AD:
        sub    ah, byte ptr [eax]
        inc    esi
        inc    ecx
        push   esp
        inc    ecx
        dec    esp
        and    byte ptr [edx + edx*2 + 49h], al
        push   esi
loc_1_2C4B9:
        inc    ebp
        push   edx
        and    byte ptr [ebp + 58h], al
        dec    ecx
        push   esp
        and    byte ptr [edx], ch
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
loc_1_2C4C8:
        sub    ch, byte ptr [edx]
loc_1_2C4CA:
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        or     al, byte ptr [eax]
loc_1_2C4D4:
        xor    esi, dword ptr [edx]
        sub    eax, 20746962h
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [esi + 78h], dl
        inc    esp
        add    byte ptr [eax], al
loc_1_2C4E4:
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [esi + 78h], dl
        inc    esp
        and    byte ptr [esi + 61h], al
        db  074h, 061h
        insb
        and    byte ptr [ebp + 72h], al
        jb     SHORT loc_1_2C565
        jb     SHORT loc_1_2C4F8
loc_1_2C4F8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_2C542
        dec    edi
        inc    ebx
        inc    edx
        and    byte ptr [206f6e20h], ch
        jb     NEAR PTR loc_1_2C572
        and    byte ptr gs:[ecx + 4fh], al
        inc    ebx
        inc    edx
        daa
        db  073h, 00Ah
        add    byte ptr [eax], al
loc_1_2C518:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esp
        outsd
        arpl   word ptr [ebx + 41h], bp
        outsb
        pop    eax
        insb
        popad
        je     SHORT loc_1_2C549
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [eax + 74h], dh
        db  072h, 020h
        db  070h, 057h
        imul   ebp, dword ptr [esi + 33h], 6c253d32h
        or     al, byte ptr [eax]
loc_1_2C53C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esp
loc_1_2C542:
        outsd
        arpl   word ptr [ebx + 41h], bp
        outsb
        pop    eax
loc_1_2C549:
        insb
        popad
        je     SHORT loc_1_2C56D
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [eax + 74h], dh
        db  072h, 020h
        db  070h, 057h
        imul   ebp, dword ptr [esi + 33h], 6c253d32h
        and    byte ptr [eax + 61h], dh
        inc    ebx
        outsb
loc_1_2C565:
        je     SHORT loc_1_2C5A4
        and    eax, 6b6f2064h
        inc    ebx
loc_1_2C56D:
        outsb
        db  074h, 03Dh
        db  025h, 064h
loc_1_2C572:
        or     al, byte ptr [eax]
loc_1_2C574:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2C5EE
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [esi + ebp + 63h], 5c2e2e00h
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [esi + ebp + 63h], 5c2e2e00h
        pop    esp
        db  073h, 072h
loc_1_2C5A4:
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [esi + ebp + 63h], 5c2e2e00h
        pop    esp
        jae    SHORT loc_1_2C62A
        arpl   word ptr [esp + 6ch], bx
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [esi + ebp + 63h], 52524500h
        cmp    ah, byte ptr [eax]
        push   esi
        popad
        insb
        imul   esp, dword ptr [ecx + 74h], 72745065h
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[eax + 74h], dh
        db  072h, 020h
        jo     SHORT loc_1_2C636
        imul   ebp, dword ptr [esi + 33h], 6c253d32h
        or     al, byte ptr [eax]
loc_1_2C5E8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   esi
loc_1_2C5EE:
        popad
        insb
        imul   esp, dword ptr [ecx + 74h], 72745065h
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[eax + 74h], dh
        jb     SHORT loc_1_2C621
        db  070h, 057h
        imul   ebp, dword ptr [esi + 33h], 6c253d32h
        and    byte ptr [eax + 61h], dh
        inc    ebx
        outsb
        je     SHORT loc_1_2C650
        and    eax, 6b6f2064h
        inc    ebx
        outsb
        je     SHORT loc_1_2C659
        and    eax, 0a64h
loc_1_2C621:
        add    byte ptr [eax], al
        db  000h
loc_1_2C624:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2C628:
        add    byte ptr [eax], al
loc_1_2C62A:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2C636:
        add    byte ptr [eax], al
loc_1_2C638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C650:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_2C659:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_2C678:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C67C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C680:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C684:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C688:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C68C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C69C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C6E0:
        add    byte ptr [eax - 5fdf3fc0h], al
        pushad
        db  0E0h, 010h
        nop
        push   eax
        sal    byte ptr [eax], 1
        mov    al, 70h
        lock or byte ptr [eax - 57d737b8h], cl
        push   589818e8h
        fdivr  dword ptr [eax]
        mov    eax, 8404f878h
        inc    esp
        les    esp, ptr [esp]
        in     al, 14h
        xchg   esp, eax
        push   esp
        aam    34h
        mov    ah, 74h
        hlt
        or     al, 8ch
        dec    esp
        int3
        sub    al, 0ach
        insb
        in     al, dx
        sbb    al, 9ch
        pop    esp
        fdivr  qword ptr [esp + edi*4]
        db  07Ch, 0FCh
        add    al, byte ptr [edx - 5ddd3dbeh]
        db  062h, 0E2h, 012h, 092h, 052h, 0D2h, 032h, 0B2h, 072h, 0F2h, 00Ah, 08Ah, 04Ah, 0CAh, 02Ah, 0AAh
        db  06Ah, 0EAh, 01Ah, 09Ah, 05Ah, 0DAh, 03Ah, 0BAh, 07Ah, 0FAh, 006h, 086h, 046h, 0C6h, 026h, 0A6h
        db  066h, 0E6h, 016h, 096h, 056h, 0D6h, 036h, 0B6h, 076h, 0F6h, 00Eh, 08Eh, 04Eh, 0CEh, 02Eh, 0AEh
        db  06Eh, 0EEh, 01Eh, 09Eh, 05Eh, 0DEh, 03Eh, 0BEh, 07Eh, 0FEh
loc_1_2C760:
        add    dword ptr [ecx - 5ede3ebfh], eax
        popad
        db  0E1h, 011h
        xchg   ecx, eax
        push   ecx
        sal    dword ptr [ecx], 1
        mov    cl, 71h
        int1
        or     dword ptr [ecx - 56d636b7h], ecx
        imul   ebp, ecx, 0d9599919h
        cmp    dword ptr [ecx - 7afa0687h], edi
        inc    ebp
        lds    esp, ptr [15e565a5h]
        xchg   ebp, eax
        push   ebp
        aad    35h
        mov    ch, 75h
        cmc
        or     eax, 2dcd4d8dh
        lodsd
        insd
        in     eax, dx
        sbb    eax, 3ddd5d9dh
        mov    ebp, 8303fd7dh
        inc    ebx
        ret
        db  023h, 0A3h, 063h, 0E3h, 013h, 093h, 053h, 0D3h, 033h, 0B3h, 073h, 0F3h, 00Bh, 08Bh, 04Bh, 0CBh
        db  02Bh, 0ABh, 06Bh, 0EBh, 01Bh, 09Bh, 05Bh, 0DBh, 03Bh, 0BBh, 07Bh, 0FBh, 007h, 087h, 047h, 0C7h
        db  027h, 0A7h, 067h, 0E7h, 017h, 097h, 057h, 0D7h, 037h, 0B7h, 077h, 0F7h, 00Fh, 08Fh, 04Fh, 0CFh
        db  02Fh, 0AFh, 06Fh, 0EFh, 01Fh, 09Fh, 05Fh, 0DFh, 03Fh, 0BFh, 07Fh, 0FFh, 000h, 001h, 000h, 000h
loc_1_2C7E4:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2C7E8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C810:
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C81C:
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_1_2C820:
        pop    eax
        add    byte ptr [eax], al
        db  000h
loc_1_2C824:
        mov    word ptr [eax], es
        add    byte ptr [eax], al
loc_1_2C828:
        aam    0
        add    byte ptr [eax], al
loc_1_2C82C:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C830:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C834:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        daa
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C840:
        pop    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    bh, bh
        inc    dword ptr [eax]
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_2EF80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EF84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EF88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EF8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EF90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EF94:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EF98:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EF9C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EFA0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EFA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        push   ebx
        db  075h, 070h
        jo     SHORT loc_1_2F022
        jb     SHORT loc_1_2F029
        and    byte ptr [ebx + 43h], al
        inc    edx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [edx + esi*2 + 61h], dl
        arpl   word ptr [ebp + 20h], sp
        insd
        popad
        jae    SHORT loc_1_2F051
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 002h, 000h, 04Dh, 061h, 078h
        db  020h, 041h, 064h, 061h, 070h, 074h, 065h, 072h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_2F022:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        db  000h
loc_1_2F029:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [ebp + 61h], cl
        db  078h, 020h
        push   ebp
        db  073h, 065h
        db  072h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F051:
        add    byte ptr [eax], al
        add    byte ptr [1000000h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        and    byte ptr [eax + 6bh], dl
        je     SHORT loc_1_2F08F
        inc    esp
        db  065h, 073h, 063h
        db  072h, 069h
        jo     SHORT loc_1_2F0EB
        outsd
        jb     SHORT loc_1_2F0ED
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F08F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F0C5:
        add    byte ptr [eax], al
loc_1_2F0C7:
        add    byte ptr [eax], al
loc_1_2F0C9:
        add    byte ptr [eax], al
loc_1_2F0CB:
        add    byte ptr [eax], al
loc_1_2F0CD:
        add    byte ptr [eax], al
loc_1_2F0CF:
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [ebx + 43h], al
        inc    edx
        and    byte ptr [ecx + 64h], ah
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_2F102
        outsb
        jne    SHORT loc_1_2F152
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F0EB:
        add    byte ptr [eax], al
loc_1_2F0ED:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        db  000h, 004h, 000h
        add    byte ptr [eax], al
loc_1_2F102:
        add    dword ptr [eax], eax
        dec    ebp
        popad
        db  078h, 020h
        push   ebx
        inc    ecx
        push   eax
        jae    SHORT loc_1_2F10D
loc_1_2F10D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [2000000h], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        db  000h, 005h, 000h, 000h, 000h, 001h
        add    byte ptr [ebp + 61h], cl
        js     SHORT loc_1_2F156
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        jae    SHORT loc_1_2F13C
loc_1_2F13C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
loc_1_2F152:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2F156:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 31542072h
        and    byte ptr [eax], ch
        xor    dword ptr [ecx], ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 32542072h
        and    byte ptr [eax], ch
        xor    dword ptr [ecx], ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        db  000h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx + ebp*2 + 6dh], dl
        db  065h, 072h, 020h
        push   esp
        imul   esp, dword ptr [eax], 293128h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [0ff000000h], al
        add    byte ptr [eax], al
        db  000h, 00Bh
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx + ebp*2 + 6dh], dl
        jb     NEAR PTR loc_1_2F210
        push   esp
        xor    dword ptr [eax], esp
        sub    byte ptr [edx], dh
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
loc_1_2F210:
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 32542072h
        and    byte ptr [eax], ch
        xor    ch, byte ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     eax, 1000000h
        add    byte ptr [ecx + ebp*2 + 6dh], dl
        db  065h, 072h, 020h
        push   esp
        imul   esp, dword ptr [eax], 293228h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [0ff000000h], al
        add    byte ptr [eax], al
        db  000h, 002h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [ebp + 61h], cl
        db  078h, 020h
        db  066h, 072h, 061h
        insd
        and    byte ptr gs:[ebx + 69h], dh
        db  07Ah, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  070h, 011h
        add    byte ptr [eax], al
        pushad
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        inc    esi
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ebp + 61h], cl
        db  078h, 020h
        jb     NEAR PTR loc_1_2F319
        and    byte ptr [ebx + 41h], dl
        push   eax
        jae    SHORT loc_1_2F2AF
loc_1_2F2AF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ebp + 61h], cl
        js     SHORT loc_1_2F2F4
        db  067h, 072h, 070h
        and    byte ptr [ebp + 65h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        jae    SHORT loc_1_2F2E0
loc_1_2F2E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F2F4:
        cmp    al, 0
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ebx + 77h], dl
        popad
        jo     SHORT loc_1_2F323
        popad
        db  064h, 064h, 072h, 020h
        bound  ebp, qword ptr [ecx + 74h]
        jae    SHORT loc_1_2F30D
loc_1_2F30D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F319:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
loc_1_2F323:
        add    byte ptr [eax], al
        db  000h, 00Fh
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ebp + 74h], al
        push   656e7265h
        db  074h, 020h
        inc    esp
        dec    ecx
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F388:
        add    al, 2
        mov    esi, 78080405h
        adc    eax, edx
        pop    ds
        db  08Fh, 02Ch, 088h, 045h, 004h, 002h
loc_1_2F398:
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        pop    es
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [4000201h]
        add    al, byte ptr [edx]
        add    al, byte ptr [6020202h]
        add    al, byte ptr [edx]
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    al, byte ptr [edx]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    dword ptr [201h], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_2F428:
        add    byte ptr [eax + 10h], 20h
        add    al, 1
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [eax], ecx
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_2F440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax + 10100088h], 8
        sbb    byte ptr [eax + 188c84h], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     dword ptr [870f4301h], eax
        outsd
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F468:
        add    byte ptr [eax], al
loc_1_2F46A:
        add    byte ptr [eax], al
loc_1_2F46C:
        add    byte ptr [eax], al
loc_1_2F46E:
        add    byte ptr [eax], al
loc_1_2F470:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F4C0:
        dec    esi
        outsd
        jbe    SHORT loc_1_2F4E4
        and    byte ptr [ecx], bh
        and    byte ptr [ecx], dh
        cmp    dword ptr [ecx], edi
        xor    eax, 0
        db  000h
loc_1_2F4D0:
        xor    dword ptr [eax], esi
        cmp    dh, byte ptr [esp + esi]
        cmp    dh, byte ptr [37h]
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        outsb
        outsd
        outsb
loc_1_2F4E4:
        sub    eax, 73697865h
        db  074h, 065h
        outsb
        db  074h, 020h
        jb     NEAR PTR loc_1_2F560
        db  075h, 070h
        and    byte ptr [ebx + 41h], dl
        push   eax
        and    byte ptr [edi + 72h], ch
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[edi + 72h], ah
        db  070h, 053h
        inc    ecx
        push   eax
        push   esi
        popad
        insb
        jne    SHORT loc_1_2F56F
        cmp    eax, 0a6225h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        jb     NEAR PTR loc_1_2F587
        db  075h, 070h
        and    byte ptr [ebx + 41h], dl
        push   eax
        and    byte ptr [ebp + 61h], ch
        js     SHORT loc_1_2F543
        insd
        insd
        bound  esp, qword ptr [ebp + 72h]
        db  073h, 020h
        insd
        popad
        db  078h, 047h
        db  072h, 070h
        dec    ebp
        insd
        bound  esp, qword ptr [ebp + 72h]
        db  073h, 03Dh
        and    eax, 0a64h
        add    byte ptr [eax], al
loc_1_2F540:
        inc    ebp
        push   edx
        push   edx
loc_1_2F543:
        cmp    ah, byte ptr [eax]
        jb     NEAR PTR loc_1_2F5B7
        db  075h, 070h
        and    byte ptr [ebp + 65h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [ebx + 41h], dl
        push   eax
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[edi + 70h], ch
        je     SHORT loc_1_2F5C7
        outsd
        outsb
loc_1_2F560:
        jae    SHORT loc_1_2F59F
        and    eax, 62252f62h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_2F56F:
        cmp    ah, byte ptr [eax]
        outsb
        outsd
        outsb
        sub    eax, 73697865h
        db  074h, 065h
        outsb
        db  074h, 020h
        popad
        db  073h, 073h
        outsd
        arpl   word ptr [ecx + 61h], bp
        je     SHORT loc_1_2F5EC
loc_1_2F587:
        and    byte ptr fs:[ebx + 41h], dl
        push   eax
        and    byte ptr [ebp + 6ch], ah
        push   ebx
        inc    ecx
        push   eax
        push   esi
        popad
        insb
        jne    SHORT loc_1_2F5FD
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2F59F:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_2F5FA
        inc    ecx
        push   eax
        cmp    eax, 6e206c25h
        outsd
        and    byte ptr [ebp + 6ch], ah
        push   ebx
        inc    ecx
        push   eax
        push   esi
loc_1_2F5B7:
        popad
        insb
        db  075h, 065h
        cmp    eax, 65206225h
        insb
        insd
        outsb
        je     SHORT loc_1_2F5D1
loc_1_2F5C7:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_2F610
        db  073h, 073h
loc_1_2F5D1:
        outsd
        arpl   word ptr [ebx + 41h], dx
        push   eax
        cmp    eax, 6e206c25h
        outsd
        and    byte ptr [ecx + 73h], ah
        db  073h, 06Fh
        arpl   word ptr [ecx + 61h], bp
        db  074h, 065h
        and    byte ptr fs:[ebx + 61h], dh
        db  070h, 056h
loc_1_2F5EC:
        popad
        insb
        jne    SHORT loc_1_2F655
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
loc_1_2F5FA:
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_2F5FD:
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_2F627
        db  067h, 072h, 06Fh
        db  075h, 070h
        and    byte ptr [ebx + 41h], dl
        push   eax
loc_1_2F610:
        and    byte ptr [ebx + 74h], dh
        db  072h, 075h
        arpl   word ptr [ebp + esi*2 + 72h], si
        or     al, byte ptr gs:[eax]
loc_1_2F61C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
loc_1_2F627:
        db  067h, 065h, 074h, 020h
        db  067h, 072h, 06Fh
        db  075h, 070h
        and    byte ptr [ebx + 41h], dl
        push   eax
        and    byte ptr [ebx + 74h], dh
        db  072h, 075h
        arpl   word ptr [ebp + esi*2 + 72h], si
        or     al, byte ptr gs:[eax]
loc_1_2F640:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2F6BA
        arpl   word ptr [edi + 72h], bx
        jo     SHORT loc_1_2F6C1
        popad
        db  070h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
loc_1_2F655:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2F6CE
        arpl   word ptr [edi + 72h], bx
        db  070h, 073h
        popad
        db  070h, 02Eh
        arpl   word ptr [eax], ax
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + ebp*2 + 6ch], bx
        arpl   word ptr [edx], si
        insd
        imul   esi, dword ptr [ebx + 63h], 632eh
        add    byte ptr [eax], al
loc_1_2F680:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    ebx
        push   646e416bh
        push   edx
        db  065h, 073h, 065h
        je     SHORT loc_1_2F6DD
        arpl   word ptr [edx + eax*2 + 75h], bp
        db  073h, 079h
        and    byte ptr [edx + 65h], dh
        db  073h, 065h
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 636f4c20h
        popad
        insb
        and    byte ptr [edx + 75h], al
        db  073h, 079h
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F6B8:
        inc    esp
        inc    edx
loc_1_2F6BA:
        inc    edi
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        db  070h, 04Ch
loc_1_2F6C1:
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        arpl   word ptr [ebp + 72h], si
        inc    ecx
        jbe    SHORT loc_1_2F72F
loc_1_2F6CE:
        imul   ebp, dword ptr [ebx + eax*2 + 6eh], 64253d74h
        and    byte ptr [edx + 65h], dh
        jae    SHORT loc_1_2F740
        je     SHORT loc_1_2F720
loc_1_2F6DD:
        outsb
        db  074h, 03Dh
        and    eax, 44000a64h
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 54h]
        imul   ebp, dword ptr [ebp + 65h], 746e4972h
        and    byte ptr [eax + 4ch], dh
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        jae    SHORT loc_1_2F776
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        imul   esi, dword ptr [ebx + 63h], 656e6e6fh
        arpl   word ptr [ecx + ebp*2 + 6eh], si
        or     al, byte ptr [bx + si]
loc_1_2F718:
        pop    esp
        pop    esp
        db  073h, 072h
loc_1_2F720:
        arpl   word ptr [esp + ebp*2 + 6ch], bx
        arpl   word ptr [edx], si
        jb     SHORT loc_1_2F78B
        db  076h, 02Eh
        arpl   word ptr [eax], ax
loc_1_2F72C:
        pop    esp
loc_1_2F72F:
        pop    esp
        jae    SHORT loc_1_2F7A6
        arpl   word ptr [esp + ebp*2 + 6ch], bx
        arpl   word ptr [edx], si
        db  072h, 063h
        db  076h, 02Eh
        arpl   word ptr [eax], ax
loc_1_2F740:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + ebp*2 + 6ch], bx
        arpl   word ptr [edx], si
        db  072h, 063h
        db  076h, 02Eh
        arpl   word ptr [eax], ax
loc_1_2F754:
        pop    esp
        pop    esp
        jae    SHORT loc_1_2F7CE
        arpl   word ptr [esp + ebp*2 + 6ch], bx
        arpl   word ptr [edx], si
        db  072h, 063h
        db  076h, 02Eh
        arpl   word ptr [eax], ax
loc_1_2F768:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + ebp*2 + 6ch], bx
        arpl   word ptr [edx], si
loc_1_2F776:
        jb     SHORT loc_1_2F7DB
        jbe    SHORT loc_1_2F7A8
        arpl   word ptr [eax], ax
loc_1_2F77C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 52h]
        arpl   word ptr [esi + 53h], si
        inc    ecx
loc_1_2F78B:
        inc    edx
        dec    ebp
        inc    ebp
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[ebx + 72h], dh
        arpl   word ptr [ebx + 41h], dx
        push   eax
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_2F7A0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esp
loc_1_2F7A6:
        dec    esp
        inc    ebx
loc_1_2F7A8:
        xor    bl, byte ptr [edi + 52h]
        arpl   word ptr [esi + 53h], si
        inc    ecx
        inc    edx
        dec    ebp
        inc    ebp
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [ecx + 76h], ah
        popad
        imul   ebp, dword ptr [ecx + 62h], 4c20656ch
        imul   ebp, dword ptr [esi + 6bh], 0a73h
loc_1_2F7C8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
loc_1_2F7CE:
        dec    esp
        inc    ebx
        xor    ah, byte ptr [eax]
        jo     SHORT loc_1_2F820
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
loc_1_2F7DB:
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 63207725h
        arpl   word ptr [edx + 31h], sp
        and    byte ptr [ecx + ebp*2 + 73h], ah
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6eh], si
        or     al, byte ptr [bx + si]
loc_1_2F7F8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    ah, byte ptr [eax]
        jo     SHORT loc_1_2F850
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        jae    SHORT loc_1_2F881
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        imul   esi, dword ptr [ebx + 63h], 656e6e6fh
        arpl   word ptr [eax + 70h], si
loc_1_2F820:
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 4244000ah
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    ah, byte ptr [eax]
        jo     SHORT loc_1_2F880
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        imul   esi, dword ptr [ebx + 63h], 656e6e6fh
        arpl   word ptr [ecx + ebp*2 + 6eh], si
loc_1_2F850:
        or     al, byte ptr [bx + si]
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    ah, byte ptr [eax]
        db  070h, 04Ch
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        jae    SHORT loc_1_2F8DD
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 63206225h
        outsd
loc_1_2F880:
        outsb
loc_1_2F881:
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6eh], si
        or     al, byte ptr [bx + si]
        add    byte ptr [eax], al
loc_1_2F88C:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 44h]
        popad
        arpl   word ptr [ecx + ebp*2 + 76h], si
        popad
        je     SHORT loc_1_2F905
        push   ebx
        db  074h, 061h
        and    byte ptr [eax + 4ch], dh
        imul   ebp, dword ptr [esi + 6bh], 0a6c253dh
        add    byte ptr [eax], al
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 44h]
        popad
        arpl   word ptr [ecx + ebp*2 + 76h], si
        popad
        je     SHORT loc_1_2F929
        push   ebx
        je     SHORT loc_1_2F928
        and    byte ptr [edx + 65h], dh
        db  074h, 043h
        outsd
        cmp    eax, 53206225h
        db  074h, 061h
        je     SHORT loc_1_2F93D
        cmp    eax, 73206225h
loc_1_2F8DD:
        je     SHORT loc_1_2F940
        je     SHORT loc_1_2F956
        jae    SHORT loc_1_2F920
        and    eax, 0a62h
loc_1_2F8E8:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    ah, byte ptr [eax]
        jo     SHORT loc_1_2F940
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
loc_1_2F905:
        popad
        arpl   word ptr [ecx + ebp*2 + 76h], si
        popad
        je     SHORT loc_1_2F973
        or     al, byte ptr fs:[eax]
        add    byte ptr [eax], al
        add    byte ptr [esp + ecx*2 + 43h], cl
        xor    bl, byte ptr [edi + 46h]
        insb
        outsd
        ja     SHORT loc_1_2F961
        je     SHORT loc_1_2F98C
loc_1_2F920:
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 694c7020h
loc_1_2F928:
        outsb
loc_1_2F929:
        imul   edi, dword ptr [72206c25h], 65h
        db  073h, 065h
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 636f4c20h
        popad
        insb
loc_1_2F93D:
        and    byte ptr [edx + 75h], al
loc_1_2F940:
        db  073h, 079h
        and    byte ptr [ebp + 73h], dl
        jb     NEAR PTR loc_1_2F952
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F94C:
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 46h]
loc_1_2F952:
        insb
        outsd
        ja     SHORT loc_1_2F999
loc_1_2F956:
        je     SHORT loc_1_2F9C4
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 694c7020h
        outsb
loc_1_2F961:
        imul   edi, dword ptr [72206c25h], 65h
        db  073h, 065h
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 636f4c20h
loc_1_2F973:
        popad
        insb
        and    byte ptr [edx + 75h], al
        jae    SHORT loc_1_2F9F3
        and    byte ptr [edx + 75h], al
        jb     NEAR PTR loc_1_2F98C
        add    byte ptr [eax], al
loc_1_2F984:
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 46h]
        insb
        outsd
loc_1_2F98C:
        ja     SHORT loc_1_2F9D1
        je     SHORT loc_1_2F9FC
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 694c7020h
        outsb
loc_1_2F999:
        imul   edi, dword ptr [73206c25h], 65h
        je     SHORT loc_1_2FA16
        imul   ebp, dword ptr [esi + 67h], 636f4c20h
        popad
        insb
        and    byte ptr [edx + 75h], al
        jae    SHORT loc_1_2FA29
        and    byte ptr [edx + 75h], al
        jb     NEAR PTR loc_1_2F9C2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F9BC:
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 46h]
loc_1_2F9C2:
        insb
        outsd
loc_1_2F9C4:
        db  077h, 043h
        db  074h, 06Ch
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 694c7020h
        outsb
loc_1_2F9D1:
        imul   edi, dword ptr [73206c25h], 65h
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 636f4c20h
        popad
        insb
        and    byte ptr [edx + 75h], al
        jae    SHORT loc_1_2FA61
        and    byte ptr [ebp + 73h], dl
        jb     NEAR PTR loc_1_2F9F8
        add    byte ptr [eax], al
loc_1_2F9F0:
        inc    esp
        inc    edx
        inc    edi
loc_1_2F9F3:
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
loc_1_2F9F8:
        xor    ah, byte ptr [eax]
        db  070h, 04Ch
loc_1_2F9FC:
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 64207725h
        insb
        arpl   word ptr [edi + 70h], cx
        outsb
        inc    ebx
        inc    ebx
        inc    edx
loc_1_2FA16:
        cmp    eax, 6f206225h
        jo     SHORT loc_1_2FA82
        outsb
        or     al, byte ptr fs:[eax]
        add    byte ptr [eax], al
loc_1_2FA24:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
loc_1_2FA29:
        dec    esp
        dec    esp
        inc    ebx
        xor    ah, byte ptr [eax]
        db  070h, 041h
        dec    edi
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     SHORT loc_1_2FAAA
        insb
        dec    ecx
        inc    esp
        cmp    eax, 72206225h
        jae    NEAR PTR loc_1_2FAAA
        je     SHORT loc_1_2FA67
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_1_2FAB8
        db  065h, 074h, 065h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    ah, byte ptr [eax]
        jo     SHORT loc_1_2FAB3
        inc    ecx
loc_1_2FA61:
        push   eax
        cmp    eax, 73206c25h
loc_1_2FA67:
        je     SHORT loc_1_2FACA
        dec    ecx
        inc    esp
        cmp    eax, 72207725h
        jae    NEAR PTR loc_1_2FAD8
        je     SHORT loc_1_2FA95
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_1_2FAE6
        je     NEAR PTR loc_1_2FAE2
        or     al, byte ptr [eax]
        add    byte ptr [ebp + 52h], al
loc_1_2FA82:
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esp
        dec    esp
        inc    ebx
        xor    bl, byte ptr [edi + 58h]
        insd
        imul   esi, dword ptr [ecx + ecx*2 + 66h], 62206d72h
        popad
loc_1_2FA95:
        and    byte ptr fs:[ecx + 46h], ch
        imul   esp, dword ptr [ebp + 6ch], 6e654c64h
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
loc_1_2FAAA:
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        insd
        popad
        db  078h, 052h
loc_1_2FAB3:
        arpl   word ptr [esi + 46h], si
        db  072h, 06Dh
loc_1_2FAB8:
        cmp    eax, 6d206425h
        popad
        arpl   word ptr [eax + 64h], cx
        db  072h, 04Ch
        outsb
        cmp    eax, 0a6425h
loc_1_2FACA:
        add    byte ptr [eax], al
loc_1_2FACC:
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
loc_1_2FAD8:
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
loc_1_2FAE2:
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
loc_1_2FAE6:
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        or     eax, 3000d00h
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], cl
        or     byte ptr [ecx], 80h
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        sldt   word ptr [ebx]
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edi], cl
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edi], cl
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edi], cl
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edi], cl
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edx], al
        add    byte ptr [eax + eax], al
        add    eax, 600h
        add    byte ptr [ecx], al
        add    byte ptr [eax + eax*4], 0bh
        add    byte ptr [edi], al
        add    byte ptr [edx], 0
        sbb    byte ptr [eax], al
        sbb    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        sbb    dword ptr [eax], eax
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        push   ss
        adc    byte ptr [esi], 80h
        add    byte ptr [eax], al
        sbb    dword ptr [eax + 801dh], eax
        sbb    eax, 23802080h
        add    byte ptr [eax], 0
        and    eax, dword ptr [eax - 7fd07fdah]
        add    byte ptr [eax], al
        das
        xor    byte ptr [edx], 80h
        sub    dword ptr [eax - 7fd70000h], eax
        sub    al, 80h
        das
        add    byte ptr [eax], 0
        das
        xor    byte ptr [edx], 80h
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    eax, dword ptr [eax]
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    eax, 600h
        add    byte ptr [edx], al
        add    byte ptr [ebx], cl
        or     byte ptr [esi], 80h
        adc    dword ptr [eax + 2f0002h], eax
        das
        add    byte ptr [eax], dh
        add    byte ptr [eax], dh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], dh
        add    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [600h], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [35000280h], dh
        add    byte ptr [36003600h], dh
        add    byte ptr [edi], dh
        add    byte ptr [edi], dh
        add    byte ptr [eax], bh
        add    byte ptr [eax], bh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], bh
        add    byte ptr [ecx], bh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], bh
        add    byte ptr [ecx], bh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], bh
        add    byte ptr [ecx], bh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], bh
        add    byte ptr [ecx], bh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], bh
        add    byte ptr [ecx], bh
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [600h], al
        add    byte ptr [edx], dh
        add    byte ptr [ebx], dh
        add    byte ptr [eax + eax], dh
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        inc    edx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [ebx], al
        inc    esp
        add    byte ptr [eax + eax + 45h], al
        add    byte ptr [ebp], al
        inc    esi
        add    byte ptr [esi], al
        inc    edi
        add    byte ptr [edi], al
        inc    edi
        add    byte ptr [eax], cl
        dec    ecx
        add    byte ptr [ecx], cl
        dec    ecx
        add    byte ptr [edx], cl
        cmp    eax, dword ptr [eax - 7fc47fc5h]
        or     byte ptr ds:[edi], 4fh
        add    byte ptr [edi], cl
        push   eax
        add    byte ptr [eax + eax + 4ch], cl
        add    byte ptr [eax + eax + 51h], cl
        add    byte ptr [ebx], bh
        add    byte ptr [ebx], bh
        add    byte ptr [ebx], bh
        add    byte ptr [eax + eax], bh
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        cmp    al, byte ptr [eax]
        add    al, byte ptr [eax]
        cmp    eax, 3e000200h
        add    byte ptr [edi], bh
        add    byte ptr [eax], bh
        add    byte ptr [ecx], 59h
        add    byte ptr [ecx], bl
        pop    edx
        add    byte ptr [edx], bl
        pop    ebx
        add    byte ptr [ebx], bl
        pop    esp
        add    byte ptr [eax + eax + 5dh], bl
        add    byte ptr [ebp], bl
        pop    esi
        add    byte ptr [esi], bl
        pop    esi
        add    byte ptr [edi], bl
        pop    esi
        add    byte ptr [esi], bl
        pop    esi
        add    byte ptr [edi], bl
        cmp    eax, dword ptr [eax - 7fc47fc5h]
        inc    esp
        and    byte ptr [ecx], 61h
        add    byte ptr [ecx], ah
        bound  eax, qword ptr [eax]
        dec    esp
        add    byte ptr [eax + eax + 4ch], cl
        add    byte ptr [ebx], ah
        push   ebx
        add    byte ptr [ebx], dl
        push   ebx
        add    byte ptr [eax + eax + 2], dl
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], dl
        push   ebp
        add    byte ptr [edx], al
        add    byte ptr [esi], dl
        push   edi
        add    byte ptr [ecx - 80h], al
        add    al, byte ptr [eax]
        pop    ecx
        add    byte ptr [ecx], bl
        pop    edx
        add    byte ptr [edx], bl
        pop    ebx
        add    byte ptr [ebx], bl
        pop    esp
        add    byte ptr [eax + eax + 6ah], bl
        add    byte ptr [edx], ch
        imul   eax, dword ptr [eax], 6bh
        add    byte ptr [ebx], ch
        insb
        add    byte ptr [ebp], ch
        insd
        add    byte ptr [ebp], ch
        outsb
        add    byte ptr [ebx], bh
        cmp    byte ptr [ebx], 80h
        cmp    eax, dword ptr [eax + 70804ah]
        jo     SHORT loc_1_2FD64
loc_1_2FD64:
        jo     SHORT loc_1_2FD66
loc_1_2FD66:
        jno    SHORT loc_1_2FD68
loc_1_2FD68:
        dec    esp
        add    byte ptr [eax + eax + 4ch], cl
        add    byte ptr [edx], dh
        add    byte ptr gs:[ebp], ah
        add    byte ptr gs:[esi], ah
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    byte ptr fs:[edx], al
        add    byte ptr [edi], ah
        add    al, byte ptr [eax]
        push   esi
        add    byte ptr [eax], ch
        inc    edi
        add    byte ptr [edx], 0
        jns    SHORT loc_1_2FD8E
loc_1_2FD8E:
        jns    SHORT loc_1_2FD90
loc_1_2FD90:
        jp     SHORT loc_1_2FD92
loc_1_2FD92:
        jp     SHORT loc_1_2FD94
loc_1_2FD94:
        jnp    SHORT loc_1_2FD96
loc_1_2FD96:
        jnp    SHORT loc_1_2FD98
loc_1_2FD98:
        jl     SHORT loc_1_2FD9A
loc_1_2FD9A:
        jl     SHORT loc_1_2FD9C
loc_1_2FD9C:
        push   ebx
        adc    byte ptr [ebx - 80h], 56h
        sbb    byte ptr [eax + eax*4 + 56h], 80h
        pop    ecx
        sbb    byte ptr [edi - 80h], 65h
        sbb    byte ptr [edi - 80h], 62h
        sub    byte ptr [eax - 80h], 6dh
        sub    byte ptr [eax - 80h], 6ah
        xor    byte ptr [eax - 80h], 76h
        xor    byte ptr [eax - 80h], 73h
        cmp    byte ptr [ecx - 80h], 6dh
        cmp    byte ptr [ecx - 80h], 7ch
        xor    byte ptr [eax + eax + 74h], 0
        je     SHORT loc_1_2FDCE
loc_1_2FDCE:
        jne    SHORT loc_1_2FDD0
loc_1_2FDD0:
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        jae    SHORT loc_1_2FDD6
loc_1_2FDD6:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        dec    ebp
        add    byte ptr [edx], 0
        push   eax
        add    byte ptr [ecx], 9ch
        add    byte ptr [eax + eax + 9d009dh], bl
        sahf
        add    byte ptr [esi - 60ff6100h], bl
        add    byte ptr [edx - 7a7f7d80h], al
        or     byte ptr [ebx - 777f7a80h], 80h
        mov    es, word ptr [eax - 7f717f6fh]
        mov    byte ptr [eax - 7f687f98h], al
        push   9a809480h
        and    byte ptr [eax - 627f6580h], 80h
        jns    SHORT loc_1_2FD9A
        xchg   edi, eax
        cmp    byte ptr [ecx - 80h], 0a3h
        sbb    byte ptr [eax - 67ff6800h], 0
        cdq
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [edi + 2000000h], dl
        add    byte ptr [ebp - 80h], cl
        add    al, byte ptr [eax]
        db  07Fh, 080h
        add    al, byte ptr [eax]
        pushfd
        add    byte ptr [eax + eax + 9d009dh], bl
        sahf
        add    byte ptr [esi - 60ff6100h], bl
        add    byte ptr [eax - 56ff6000h], ah
        sub    byte ptr [edi - 537f5680h], 80h
        mov    es, word ptr [eax - 7f717f4bh]
        mov    dl, 80h
        push   6880bb80h
        cmp    byte ptr [eax - 3e7f8f80h], 80h
        db  070h, 080h
        mov    esi, 0bb80c480h
        add    ah, 80h
        mov    byte ptr [eax + 980098h], 98h
        add    byte ptr [ecx + 2000200h], bl
        add    byte ptr [eax + eax + 2], dh
        add    al, byte ptr [eax]
        dec    ebp
        add    byte ptr [edx], 0
        cmpsb
        add    byte ptr [edx], 0
        mov    byte ptr [eax], 0c6h
        add    bh, al
        add    bh, al
        add    al, cl
        add    al, cl
        add    cl, cl
        add    cl, cl
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [600h], al
        add    byte ptr [eax], cl
        add    ch, al
        add    byte ptr [eax + eax], dh
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        inc    edx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [ebx], al
        inc    esp
        add    byte ptr [eax + eax + 45h], al
        add    byte ptr [ebp], al
        inc    esi
        add    byte ptr [esi], al
        inc    edi
        add    ah, cl
        add    byte ptr [edi], al
        dec    eax
        add    ch, cl
        add    bh, cl
        add    ch, cl
        add    dh, cl
        add    ah, cl
        or     ah, 80h
        int3
        or     bh, 80h
        dec    esp
        add    byte ptr [eax + eax + 4ch], cl
        add    byte ptr [ecx], dl
        aad    80h
        aad    80h
        aad    80h
        rol    byte ptr [eax + 3b003bh], cl
        cmp    eax, dword ptr [eax]
        cmp    al, 0
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        retf   200h
        db  000h, 000h, 000h, 002h, 000h, 03Eh, 000h, 0CBh, 000h, 0C9h, 080h, 041h, 000h, 059h, 000h, 059h
        db  000h, 05Ah, 000h, 05Ah, 000h, 05Bh, 000h, 05Bh, 000h, 05Ch, 000h, 05Ch, 000h, 06Ah, 000h, 06Ah
        db  000h, 05Eh, 000h, 05Eh, 000h, 05Eh, 000h, 05Fh, 000h, 05Eh, 000h, 05Eh, 000h, 05Eh, 000h, 05Fh
        db  000h, 0D8h, 080h, 0D8h, 080h, 0D8h, 080h, 0DBh, 080h, 04Ch, 000h, 04Ch, 000h, 04Ch, 000h, 063h
        db  000h, 0E1h, 080h, 0E1h, 080h, 0E1h, 080h, 0DEh, 080h, 053h, 000h, 053h, 000h, 053h, 000h, 054h
        db  000h, 002h, 000h, 002h, 000h, 002h, 000h, 0D7h, 000h, 000h, 000h, 002h, 000h, 056h, 000h, 0D8h
        db  000h, 0D8h, 000h, 002h, 000h, 059h, 000h, 059h, 000h, 05Ah, 000h, 05Ah, 000h, 0E2h, 000h, 0E2h
        db  000h, 05Ch, 000h, 05Ch, 000h, 06Ah, 000h, 06Ah, 000h, 0E3h, 000h, 0E3h, 000h, 0E3h, 000h, 0E4h
        db  000h, 06Dh, 000h, 06Dh, 000h, 06Dh, 000h, 05Fh, 000h, 0E5h, 000h, 0E5h, 000h, 0E5h, 000h, 0E6h
        db  000h, 0E7h, 000h, 0E7h, 000h, 0E7h, 000h, 0E8h, 000h, 04Ch, 000h, 04Ch, 000h, 04Ch, 000h, 063h
        db  000h, 053h, 000h, 053h, 000h, 053h, 000h, 054h, 000h, 002h, 000h, 002h, 000h, 002h, 000h, 0DFh
        db  000h, 0E0h, 000h, 002h, 000h, 056h, 000h, 0E1h, 000h, 0E4h, 080h, 002h, 000h, 059h, 000h, 059h
        db  000h, 05Ah, 000h, 05Ah, 000h, 05Bh, 000h, 05Bh, 000h, 05Ch, 000h, 05Ch, 000h, 06Ah, 000h, 06Ah
        db  000h, 0EBh, 000h, 0EBh, 000h, 0EBh, 000h, 0ECh, 000h, 06Dh, 000h, 06Dh, 000h, 06Dh, 000h, 06Eh
        db  000h, 0EAh, 080h, 0EAh, 080h, 0EAh, 080h, 0EDh, 080h, 04Ch, 000h, 04Ch, 000h, 04Ch, 000h, 072h
        db  000h, 0F3h, 080h, 0F3h, 080h, 0F3h, 080h, 0F0h, 080h, 053h, 000h, 053h, 000h, 053h, 000h, 054h
        db  000h, 002h, 000h, 002h, 000h, 0E9h, 000h, 002h, 000h, 000h, 000h, 002h, 000h, 056h, 000h, 0EAh
        db  000h, 0E7h, 080h, 002h, 000h, 09Ch, 000h, 09Ch, 000h, 09Dh, 000h, 09Dh, 000h, 09Eh, 000h, 09Eh
        db  000h, 09Fh, 000h, 09Fh, 000h, 0A0h, 000h, 0A0h, 000h, 0F9h, 080h, 08Bh, 080h, 0F9h, 080h, 0FCh
        db  080h, 08Eh, 080h, 0FFh, 080h, 08Eh, 080h, 088h, 080h, 002h, 081h, 008h, 081h, 002h, 081h, 004h
        db  081h, 09Ah, 080h, 00Ah, 081h, 09Ah, 080h, 09Dh, 080h, 00Dh, 081h, 010h, 081h, 00Dh, 081h, 0A3h
        db  080h, 098h, 000h, 098h, 000h, 098h, 000h, 099h, 000h, 002h, 000h, 002h, 000h, 002h, 000h, 0F6h
        db  080h, 000h, 000h, 002h, 000h, 04Dh, 080h, 002h, 000h, 07Fh, 080h, 002h, 000h, 09Ch, 000h, 09Ch
        db  000h, 09Dh, 000h, 09Dh, 000h, 09Eh, 000h, 09Eh, 000h, 09Fh, 000h, 09Fh, 000h, 082h, 080h, 082h
        db  080h, 0A9h, 080h, 013h, 081h, 0A9h, 080h, 0ACh, 080h, 016h, 081h, 01Ch, 081h, 016h, 081h, 019h
        db  081h, 068h, 080h, 01Fh, 081h, 068h, 080h, 0B8h, 080h, 022h, 081h, 028h, 081h, 022h, 081h, 025h
        db  081h, 079h, 080h, 028h, 081h, 079h, 080h, 0C6h, 080h, 098h, 000h, 098h, 000h, 098h, 000h, 099h
        db  000h, 002h, 000h, 002h, 000h, 001h, 001h, 002h, 000h, 000h, 000h, 002h, 000h, 04Dh, 080h, 002h
        db  000h, 0A6h, 080h, 002h, 000h, 09Ch, 000h, 09Ch, 000h, 09Dh, 000h, 09Dh, 000h, 09Eh, 000h, 09Eh
        db  000h, 09Fh, 000h, 09Fh, 000h, 0A0h, 000h, 0A0h, 000h, 0A9h, 080h, 0AFh, 080h, 0A9h, 080h, 0ACh
        db  080h, 02Bh, 081h, 0B5h, 080h, 02Bh, 081h, 02Eh, 081h, 068h, 080h, 031h, 081h, 068h, 080h, 0B8h
        db  080h, 022h, 081h, 034h, 081h, 022h, 081h, 0BEh, 080h, 079h, 080h, 037h, 081h, 079h, 080h, 0C6h
        db  080h, 098h, 000h, 098h, 000h, 098h, 000h, 099h, 000h, 002h, 000h, 002h, 000h, 00Bh, 001h, 002h
        db  000h, 000h, 000h, 002h, 000h, 04Dh, 080h, 002h, 000h, 0A6h, 080h, 002h, 000h, 059h, 000h, 059h
        db  000h, 05Ah, 000h, 05Ah, 000h, 0E2h, 000h, 0E2h, 000h, 05Ch, 000h, 05Ch, 000h, 06Ah, 000h, 06Ah
        db  000h, 015h, 001h, 015h, 001h, 015h, 001h, 016h, 001h, 06Dh, 000h, 06Dh, 000h, 06Dh, 000h, 05Fh
        db  000h, 017h, 001h, 017h, 001h, 017h, 001h, 018h, 001h, 019h, 001h, 019h, 001h, 019h, 001h, 01Ah
        db  001h, 040h, 081h, 040h, 081h, 040h, 081h, 03Dh, 081h, 053h, 000h, 053h, 000h, 053h, 000h, 054h
        db  000h, 002h, 000h, 002h, 000h, 002h, 000h, 013h, 001h, 000h, 000h, 002h, 000h, 056h, 000h, 014h
        db  001h, 03Ah, 081h, 002h, 000h, 021h, 001h, 021h, 001h, 022h, 001h, 022h, 001h, 023h, 001h, 023h
        db  001h, 038h, 000h, 038h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h
        db  000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h
        db  000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h
        db  000h, 003h, 000h, 003h, 000h, 003h, 000h, 003h, 000h, 002h, 000h, 002h, 000h, 005h, 000h, 006h
        db  000h, 000h, 000h, 032h, 000h, 033h, 000h, 043h, 081h, 002h, 000h, 002h, 000h, 000h, 000h, 001h
        db  000h, 007h, 000h, 008h, 000h, 001h, 000h, 009h, 000h, 00Ah, 000h, 002h, 000h, 00Ch, 000h, 003h
        db  000h, 00Eh, 000h, 004h, 000h, 010h, 000h, 011h, 000h, 005h, 000h, 012h, 000h, 013h, 000h, 006h
        db  000h, 014h, 000h, 015h, 000h, 016h, 000h, 017h, 000h, 007h, 000h, 01Ah, 000h, 01Bh, 000h, 008h
        db  000h, 01Ch, 000h, 01Dh, 000h, 003h, 000h, 009h, 000h, 01Eh, 000h, 01Fh, 000h, 009h, 000h, 020h
        db  000h, 021h, 000h, 009h, 000h, 01Eh, 000h, 022h, 000h, 009h, 000h, 023h, 000h, 024h, 000h, 009h
        db  000h, 025h, 000h, 026h, 000h, 009h, 000h, 027h, 000h, 028h, 000h, 009h, 000h, 029h, 000h, 02Ah
        db  000h, 009h, 000h, 02Bh, 000h, 02Ch, 000h, 00Ah, 000h, 02Dh, 000h, 02Eh, 000h, 00Bh, 000h, 040h
        db  000h, 03Fh, 000h, 00Ch, 000h, 04Bh, 000h, 04Ch, 000h, 00Ch, 000h, 04Dh, 000h, 04Eh, 000h, 00Bh
        db  000h, 058h, 000h, 057h, 000h, 00Ch, 000h, 060h, 000h, 04Eh, 000h, 00Dh, 000h, 068h, 000h, 069h
        db  000h, 00Ch, 000h, 06Fh, 000h, 04Eh, 000h, 00Eh, 000h, 002h, 000h, 076h, 000h, 00Ah, 000h, 077h
        db  000h, 078h, 000h, 00Fh, 000h, 07Dh, 000h, 003h, 000h, 010h, 000h, 07Eh, 000h, 07Fh, 000h, 010h
        db  000h, 080h, 000h, 081h, 000h, 011h, 000h, 082h, 000h, 083h, 000h, 010h, 000h, 084h, 000h, 085h
        db  000h, 010h, 000h, 086h, 000h, 087h, 000h, 011h, 000h, 088h, 000h, 089h, 000h, 012h, 000h, 08Ah
        db  000h, 010h, 000h, 08Bh, 000h, 08Ch, 000h, 011h, 000h, 08Dh, 000h, 08Eh, 000h, 010h, 000h, 08Fh
        db  000h, 090h, 000h, 010h, 000h, 091h, 000h, 092h, 000h, 011h, 000h, 093h, 000h, 08Eh, 000h, 010h
        db  000h, 094h, 000h, 00Ch, 000h, 010h, 000h, 095h, 000h, 096h, 000h, 00Ah, 000h, 09Ah, 000h, 09Bh
        db  000h, 00Fh, 000h, 0A0h, 000h, 003h, 000h, 010h, 000h, 0A1h, 000h, 0A2h, 000h, 010h, 000h, 0A3h
        db  000h, 0A4h, 000h, 011h, 000h, 0A5h, 000h, 0A6h, 000h, 010h, 000h, 0A7h, 000h, 0A8h, 000h, 011h
        db  000h, 0A9h, 000h, 0A6h, 000h, 010h, 000h, 0AAh, 000h, 0ABh, 000h, 011h, 000h, 0ACh, 000h, 0ADh
        db  000h, 010h, 000h, 0AEh, 000h, 0AFh, 000h, 010h, 000h, 0B0h, 000h, 0B1h, 000h, 011h, 000h, 0B2h
        db  000h, 0ADh, 000h, 010h, 000h, 0B3h, 000h, 0ABh, 000h, 00Ah, 000h, 0B5h, 000h, 09Bh, 000h, 010h
        db  000h, 0B6h, 000h, 0B7h, 000h, 010h, 000h, 0B8h, 000h, 0B9h, 000h, 011h, 000h, 082h, 000h, 0BAh
        db  000h, 010h, 000h, 0BBh, 000h, 0BCh, 000h, 011h, 000h, 0BDh, 000h, 0BAh, 000h, 010h, 000h, 0BEh
        db  000h, 0BFh, 000h, 011h, 000h, 0C0h, 000h, 08Eh, 000h, 010h, 000h, 0C1h, 000h, 0C2h, 000h, 011h
        db  000h, 0C3h, 000h, 08Eh, 000h, 012h, 000h, 094h, 000h, 010h, 000h, 0C4h, 000h, 0BFh, 000h, 00Bh
        db  000h, 058h, 000h, 0CBh, 000h, 00Ch, 000h, 0D0h, 000h, 0D1h, 000h, 00Ch, 000h, 0D2h, 000h, 0D3h
        db  000h, 00Ch, 000h, 0D4h, 000h, 0D5h, 000h, 00Ch, 000h, 0D6h, 000h, 0D1h, 000h, 00Ch, 000h, 0D9h
        db  000h, 04Ch, 000h, 00Ch, 000h, 0DAh, 000h, 04Eh, 000h, 00Ch, 000h, 0DBh, 000h, 0DCh, 000h, 00Ch
        db  000h, 0DDh, 000h, 0DEh, 000h, 00Bh, 000h, 058h, 000h, 0E1h, 000h, 00Bh, 000h, 058h, 000h, 0EAh
        db  000h, 00Ch, 000h, 0EDh, 000h, 04Ch, 000h, 00Ch, 000h, 0EEh, 000h, 04Eh, 000h, 00Ch, 000h, 0EFh
        db  000h, 0F0h, 000h, 00Ch, 000h, 0F1h, 000h, 0F2h, 000h, 013h, 000h, 0F3h, 000h, 0F4h, 000h, 010h
        db  000h, 0F5h, 000h, 0F6h, 000h, 010h, 000h, 0F7h, 000h, 0F8h, 000h, 011h, 000h, 0F9h, 000h, 0A6h
        db  000h, 012h, 000h, 0FAh, 000h, 014h, 000h, 0FBh, 000h, 0ABh, 000h, 0FCh, 000h, 015h, 000h, 0ADh
        db  000h, 011h, 000h, 0FDh, 000h, 0FEh, 000h, 010h, 000h, 0FFh, 000h, 00Ch, 000h, 011h, 000h, 000h
        db  001h, 0ADh, 000h, 011h, 000h, 082h, 000h, 0A6h, 000h, 010h, 000h, 002h, 001h, 003h, 001h, 010h
        db  000h, 004h, 001h, 005h, 001h, 011h, 000h, 006h, 001h, 0A6h, 000h, 011h, 000h, 007h, 001h, 08Eh
        db  000h, 010h, 000h, 08Fh, 000h, 008h, 001h, 010h, 000h, 0C1h, 000h, 009h, 001h, 011h, 000h, 00Ah
        db  001h, 08Eh, 000h, 010h, 000h, 00Ch, 001h, 00Dh, 001h, 010h, 000h, 00Eh, 001h, 00Fh, 001h, 011h
        db  000h, 010h, 001h, 08Eh, 000h, 011h, 000h, 011h, 001h, 08Eh, 000h, 011h, 000h, 012h, 001h, 08Eh
        db  000h, 00Bh, 000h, 069h, 000h, 014h, 001h, 00Ch, 000h, 01Bh, 001h, 01Ch, 001h, 00Ch, 000h, 01Dh
        db  001h, 01Eh, 001h, 00Ah, 000h, 01Fh, 001h, 020h, 001h
loc_1_30490:
        inc    esi
        push   ebx
        dec    ebp
        cmp    ah, byte ptr [eax]
        inc    ebp
        outsb
        je     SHORT loc_1_3050B
        jns    SHORT loc_1_304BB
        sub    eax, 694c7020h
        outsb
        imul   edi, dword ptr [73206c25h], 74h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 53207725h
        db  074h, 061h
        db  074h, 065h
        cmp    eax, 75206225h
        jae    SHORT loc_1_30504
loc_1_304BB:
        outsb
        jo     SHORT loc_1_30533
        je     SHORT loc_1_304FD
        db  025h, 077h, 00Ah, 000h
loc_1_304C4:
        inc    esi
        push   ebx
        dec    ebp
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_30517
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        jae    SHORT loc_1_30548
        popad
        dec    ecx
        inc    esp
        cmp    eax, 53207725h
        db  074h, 061h
        db  074h, 065h
        cmp    eax, 75206225h
        arpl   word ptr [eax + 53h], bp
        outsb
        dec    ecx
        cmp    eax, 53206225h
        db  074h, 061h
        je     SHORT loc_1_3056B
        db  073h, 03Dh
        db  025h, 062h, 00Ah, 000h
loc_1_304FC:
        inc    esi
loc_1_304FD:
        push   ebx
        dec    ebp
        cmp    ah, byte ptr [eax]
        inc    ebp
        db  078h, 069h
loc_1_30504:
        db  074h, 020h
        sub    eax, 41737520h
loc_1_3050B:
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        cmp    eax, 75207725h
        jae    SHORT loc_1_30558
loc_1_30517:
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        dec    ecx
        outsb
        db  064h, 065h, 078h, 03Dh
        and    eax, 0a77h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        push   ebx
        dec    ebp
        and    byte ptr [edx + 61h], ah
loc_1_30533:
        and    byte ptr fs:[ebx + 74h], dl
        popad
        je     SHORT loc_1_305AF
        db  073h, 03Dh
        and    eax, 4c702062h
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
loc_1_30548:
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 53207725h
        je     SHORT loc_1_305B5
        db  074h, 075h
        jae    SHORT loc_1_3059B
loc_1_30558:
        outsd
        cmp    eax, 0a7725h
loc_1_30560:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        push   ebx
        dec    ebp
        and    byte ptr [ebx + 74h], dl
loc_1_3056B:
        popad
        db  074h, 065h
        cmp    eax, 75206225h
        arpl   word ptr [eax + 53h], bp
        outsb
        dec    ecx
        cmp    eax, 75206225h
        db  073h, 049h
        outsb
        db  070h, 075h
        je     SHORT loc_1_305C4
        and    eax, 0a77h
loc_1_3058C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        inc    ecx
        inc    ebx
loc_1_3059B:
        inc    edx
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[ecx + 64h], ah
        jo     SHORT loc_1_305F3
        jne    SHORT loc_1_30614
        cmp    eax, 0a6425h
loc_1_305AC:
        inc    ebp
        push   edx
        push   edx
loc_1_305AF:
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
loc_1_305B5:
        arpl   word ptr [ecx + 74h], sp
        inc    ecx
        inc    ebx
        inc    edx
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_305E3
        popad
loc_1_305C4:
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[ebp + 65h], ch
        insd
        outsd
        jb     SHORT loc_1_3064B
        and    byte ptr [esi + 64h], ch
        imul   esi, dword ptr [ebx + 53h], 75746174h
        jae    SHORT loc_1_3061B
        and    eax, 0a6ch
loc_1_305E3:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        inc    ecx
        inc    ebx
loc_1_305F3:
        inc    edx
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_3061B
        arpl   word ptr [edx + 65h], si
        popad
        db  074h, 065h
        and    byte ptr [eax + 72h], dh
        outsd
        arpl   word ptr [ebp + 76h], ax
        outsb
        db  074h, 020h
        insd
        db  075h, 074h
        db  065h, 078h, 021h
        or     al, byte ptr [eax]
loc_1_30614:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        db  070h, 041h
loc_1_3061B:
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[ecx + 64h], ah
        jo     SHORT loc_1_3067D
        db  075h, 06Dh
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_30638:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        inc    ecx
        db  064h, 070h, 053h
        db  074h, 075h
loc_1_3064B:
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_30674
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[ebp + 65h], ch
        insd
        and    byte ptr cs:[esi + 64h], ch
        imul   esi, dword ptr [ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_30674:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
loc_1_3067D:
        arpl   word ptr [ecx + 74h], sp
        inc    ecx
        db  064h, 070h, 053h
        je     SHORT loc_1_306FC
        and    byte ptr [edi + 50h], bl
        popad
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 70202d20h
        popad
        inc    ebx
        outsb
        db  074h, 03Dh
        and    eax, 0a64h
        add    byte ptr [eax], al
loc_1_306B0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        inc    ecx
        db  064h, 070h, 053h
        je     SHORT loc_1_30738
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[edx + 75h], ah
        and    byte ptr [eax + 6fh], dh
        outsd
        insb
        and    byte ptr [esi + 64h], ch
        imul   esi, dword ptr [ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_306F0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
loc_1_306FC:
        inc    ecx
        db  064h, 070h, 053h
        je     SHORT loc_1_30778
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[eax + 6bh], dh
        je     SHORT loc_1_30739
        jo     SHORT loc_1_3078A
        outsd
        insb
        and    byte ptr [esi + 64h], ch
        imul   esi, dword ptr [ebx + 53h], 75746174h
        jae    SHORT loc_1_30766
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_30730:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
loc_1_30738:
        outsd
loc_1_30739:
        arpl   word ptr [ecx + 74h], sp
        inc    ecx
        db  064h, 070h, 053h
        db  074h, 075h
        and    byte ptr [eax], ch
        jo     SHORT loc_1_3078A
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        jne    SHORT loc_1_307C2
        cmp    eax, 62206425h
        jns    SHORT loc_1_307D0
        jae    NEAR PTR loc_1_3079C
        and    eax, 61202964h
        insb
        insb
loc_1_30766:
        outsd
        arpl   word ptr [ecx + 74h], sp
        cmp    cl, byte ptr fs:[edx]
        add    byte ptr [eax], al
loc_1_30770:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebx
        inc    ecx
        push   eax
loc_1_30778:
        jae    SHORT loc_1_307B7
        and    eax, 694c2064h
        outsb
        imul   esi, dword ptr [ebx + 3dh], 25h
        and    byte ptr fs:[ebx + 4dh], dl
        dec    esp
        pop    edi
loc_1_3078A:
        inc    esi
        push   edx
        dec    ebp
        push   ebx
        cmp    eax, 0a6425h
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        dec    esi
        jne    SHORT loc_1_30809
loc_1_3079C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 42h], 65446675h
        db  073h, 063h
        cmp    eax, 4e206425h
        jne    SHORT loc_1_3081B
        dec    esi
        imul   esi, dword ptr fs:[ebx + 50h], 6544746bh
loc_1_307B7:
        jae    SHORT loc_1_3081C
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_307C0:
        inc    ebp
        push   edx
loc_1_307C2:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        push   ebp
        jae    SHORT loc_1_30835
loc_1_307D0:
        jb     SHORT loc_1_3081F
        insd
        and    byte ptr [edi + 50h], bl
        popad
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 70202d20h
        popad
        inc    ebx
        outsb
        je     SHORT loc_1_30831
        and    eax, 49000a64h
        dec    esi
        dec    ecx
        cmp    ah, byte ptr [eax]
        and    eax, 4f412064h
        inc    ebx
        inc    edx
        daa
        db  073h, 020h
        popad
        outsb
loc_1_30809:
        and    byte ptr fs:[55412064h], ah
        pop    eax
        pop    edi
        inc    ebx
        inc    ebx
        inc    edx
        daa
        jae    SHORT loc_1_30838
        popad
        insb
        insb
loc_1_3081B:
        outsd
loc_1_3081C:
        arpl   word ptr [ecx + 74h], sp
loc_1_3081F:
        and    byte ptr fs:[ecx + 6eh], ch
        and    byte ptr [61702064h], ah
        jae    NEAR PTR loc_1_30838
        add    byte ptr [eax], al
loc_1_30830:
        inc    esp
loc_1_30831:
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
loc_1_30835:
        jo     SHORT loc_1_30878
        inc    ebx
loc_1_30838:
        inc    edx
        cmp    eax, 66206c25h
        db  072h, 065h
        and    byte ptr fs:[ecx + 64h], ah
        jo     SHORT loc_1_30895
        jne    SHORT loc_1_308B6
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_30850:
        pop    esp
        pop    esp
        jae    SHORT loc_1_308CA
        arpl   word ptr [ebp + ebp*2 + 65h], bx
        insd
        insd
        jb     NEAR PTR loc_1_3088F
        arpl   word ptr [eax], ax
        db  000h
loc_1_30864:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        db  072h, 065h
        inc    ecx
        jo     NEAR PTR loc_1_308C4
        je     SHORT loc_1_308E8
        and    byte ptr [edi + 50h], bl
loc_1_30878:
        popad
        push   ebp
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx + 20h], bp
        popaw
        imul   ebp, dword ptr [ebp + 64h], 0ah
loc_1_3088C:
        inc    esp
        inc    edx
        inc    edi
loc_1_3088F:
        cmp    ah, byte ptr [eax]
        push   ebx
        dec    ebp
        inc    ecx
        dec    esp
loc_1_30895:
        dec    esp
        pop    edi
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        and    byte ptr [edx + 75h], ah
        db  066h, 066h, 065h, 072h, 073h
        and    byte ptr [esi + 72h], ah
        or     al, byte ptr fs:[eax]
loc_1_308AC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        db  072h, 065h
        inc    ecx
loc_1_308B6:
        db  064h, 070h, 053h
        db  074h, 075h
        and    byte ptr [edi + 50h], bl
        popad
        inc    esi
loc_1_308C4:
        db  072h, 065h
        and    byte ptr gs:[esi + 61h], ah
loc_1_308CA:
        imul   ebp, dword ptr [ebp + 64h], 4244000ah
        inc    edi
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_30918
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_3092F
        jne    SHORT loc_1_30950
        cmp    eax, 4c206425h
loc_1_308E8:
        imul   ebp, dword ptr [esi + 6bh], 5041532fh
        daa
        db  073h, 02Fh
        push   ebx
        dec    ebp
        inc    ecx
        dec    esp
        dec    esp
        pop    edi
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        daa
        jae    SHORT loc_1_30920
        insd
        insd
        outsd
        jb     SHORT loc_1_3097F
        and    byte ptr [esi + 72h], ah
        or     al, byte ptr fs:[eax]
        add    byte ptr [eax], al
loc_1_30910:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        db  072h, 065h
loc_1_30918:
        push   ebp
        db  073h, 065h
        db  072h, 04Dh
        insd
loc_1_30920:
        and    byte ptr [edi + 50h], bl
        popad
        push   ebp
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx + 20h], bp
        popaw
loc_1_3092F:
        imul   ebp, dword ptr [ebp + 64h], 70202d20h
        push   ebp
        db  073h, 065h
        db  072h, 053h
        je     SHORT loc_1_309B0
        jne    SHORT loc_1_309A3
        je     SHORT loc_1_3097F
        and    eax, 0a6ch
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        inc    ecx
        dec    edi
        inc    ebx
loc_1_30950:
        inc    edx
        daa
        jae    SHORT loc_1_30974
        popad
        outsb
        and    byte ptr fs:[ecx + 55h], al
        pop    eax
        pop    edi
        inc    ebx
        inc    ebx
        inc    edx
        daa
        db  073h, 020h
        jb     NEAR PTR loc_1_309CA
        or     al, byte ptr fs:[eax]
        add    byte ptr [eax], al
        db  000h
loc_1_3096C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        db  072h, 065h
loc_1_30974:
        push   ebp
        jae    SHORT loc_1_309DD
        jb     SHORT loc_1_309C7
        insd
        and    byte ptr [edi + 50h], bl
loc_1_3097F:
        popad
        inc    esi
        jb     SHORT loc_1_309EA
        and    byte ptr gs:[esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 70202d20h
        push   ebp
        jae    SHORT loc_1_309F9
        jb     SHORT loc_1_309E9
        je     SHORT loc_1_30A0A
        db  075h, 063h
        db  074h, 03Dh
        and    eax, 45000a6ch
        push   edx
        push   edx
loc_1_309A3:
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_309EA
        db  075h, 078h
        inc    ebx
        inc    ebx
        inc    edx
        sub    byte ptr [ecx], ch
loc_1_309B0:
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_309DB
        popad
        outsb
        and    byte ptr [ecx + 55h], al
        pop    eax
        pop    edi
        inc    ebx
        inc    ebx
        inc    edx
        or     al, byte ptr [eax]
loc_1_309C7:
        add    byte ptr [ebp + 52h], al
loc_1_309CA:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_30A24
        insd
        insb
        inc    esi
        db  072h, 06Dh
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
loc_1_309DB:
        outsb
        daa
loc_1_309DD:
        db  074h, 020h
        db  067h, 065h, 074h, 020h
        popad
        and    byte ptr [ebx + 4dh], dl
        inc    ecx
        dec    esp
loc_1_309E9:
        dec    esp
loc_1_309EA:
        pop    edi
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_309F4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_309F9:
        inc    ebp
        db  078h, 074h
        jb     SHORT loc_1_30A5F
        arpl   word ptr [ecx + edx*2 + 28h], si
        sub    dword ptr [eax], esp
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_30A2A
loc_1_30A0A:
        imul   bp, word ptr [esi + 64h], 2520h
        insb
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [0a216ch], ah
        add    byte ptr [eax], al
loc_1_30A1C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edx
        popad
loc_1_30A24:
        sub    byte ptr fs:[ecx], ch
        and    byte ptr [edx + 61h], ah
loc_1_30A2A:
        and    byte ptr fs:[edi + 70h], ch
        je     SHORT loc_1_30A99
        outsd
        outsb
        and    byte ptr [ecx + 6eh], ch
        imul   esp, dword ptr fs:[ebx + 61h], 20726f74h
        sub    eax, 0a622520h
        add    byte ptr [eax], al
loc_1_30A44:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        and    byte ptr [ebx + 43h], al
        inc    edx
        cmp    eax, 4f206c25h
        db  070h, 074h
        cmp    eax, 45206225h
        db  076h, 065h
loc_1_30A5F:
        outsb
        je     SHORT loc_1_30AB5
        db  065h, 074h, 03Dh
        and    eax, 0a62h
        add    byte ptr [eax], al
loc_1_30A6C:
        inc    esp
        dec    edi
        dec    esi
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        xor    dl, byte ptr [eax + 6fh]
        jae    SHORT loc_1_30AED
        and    byte ptr [ebx + 43h], al
        inc    edx
        cmp    eax, 63206c25h
        insd
        cmp    eax, 61206225h
        jo     SHORT loc_1_30AFB
        insb
        cmp    eax, 72206225h
        arpl   word ptr [70206225h], di
        jb     SHORT loc_1_30B08
loc_1_30A99:
        arpl   word ptr [ebp + 76h], ax
        outsb
        db  074h, 073h
        cmp    eax, 0a7725h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        xor    dl, byte ptr [eax + 6fh]
        jae    SHORT loc_1_30B29
loc_1_30AB5:
        and    byte ptr [ebx + 43h], al
        inc    edx
        cmp    eax, 63206c25h
        insd
        cmp    eax, 61206225h
        db  070h, 070h
        insb
        cmp    eax, 72206225h
        arpl   word ptr [70206225h], di
        db  072h, 06Fh
        arpl   word ptr [ebp + 76h], ax
        outsb
        db  074h, 073h
        cmp    eax, 207725h
        add    byte ptr [eax], al
        add    byte ptr [2a2a2a21h], ah
        sub    ch, byte ptr [edx]
        or     al, byte ptr [eax]
loc_1_30AED:
        add    byte ptr [eax], al
        db  000h
loc_1_30AF0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        xor    dl, byte ptr [eax + 6fh]
loc_1_30AFB:
        db  073h, 074h
        and    byte ptr [eax + 43h], dh
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
        dec    esp
loc_1_30B08:
        arpl   word ptr [ebx + eax*2 + 43h], bp
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_30B14:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        db  077h, 069h
        jb     SHORT loc_1_30B84
        and    byte ptr [ebx + 43h], al
        inc    edx
        pop    edi
        db  070h, 041h
        db  070h, 070h
        insb
loc_1_30B29:
        cmp    eax, 63206c25h
        outsd
        db  072h, 072h
        db  075h, 070h
        db  074h, 065h
        and    byte ptr fs:[ebx + 43h], al
        inc    edx
        cmp    eax, 42250ah
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        popad
        jo     NEAR PTR loc_1_30BBF
        jae    SHORT loc_1_30BC6
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
        add    byte ptr [edi + ecx*2 + 4eh], al
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        xor    dl, byte ptr [eax + 6fh]
        jae    SHORT loc_1_30BD9
        and    byte ptr [ecx + 6dh], cl
        insd
        and    byte ptr fs:[edx + 45h], dl
        inc    ecx
        inc    esp
        and    byte ptr [ebx + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        popad
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
loc_1_30B84:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        popad
        db  064h, 070h, 06Fh
        db  073h, 074h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        popad
        db  064h, 070h, 06Fh
        db  073h, 074h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
        db  000h
loc_1_30BB4:
        inc    ebx
        inc    edx
        xor    bh, byte ptr [edx]
        and    byte ptr [ebp + 76h], ah
        outsb
        je     SHORT loc_1_30BDF
loc_1_30BBF:
        jae    SHORT loc_1_30C2A
        outsb
        popad
        insb
        insb
loc_1_30BC6:
        and    byte ptr fs:[eax + 47h], ch
        bound  ebp, qword ptr [ebp + eax*2 + 76h]
        outsb
        db  074h, 03Dh
        and    eax, 0a6ch
loc_1_30BD8:
        inc    esp
loc_1_30BD9:
        dec    edi
        dec    esi
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
loc_1_30BDF:
        inc    edx
        xor    dl, byte ptr [eax + 6fh]
        jae    SHORT loc_1_30C59
        and    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        and    byte ptr [ebx + 6dh], al
        and    byte ptr fs:[ebx + 43h], al
        inc    edx
        cmp    eax, 52206c25h
        popad
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 6c25h
        add    byte ptr [eax], al
loc_1_30C04:
        and    eax, 20200a21h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    ebp
        db  076h, 065h
        outsb
        je     SHORT loc_1_30C6A
        db  065h, 074h, 03Dh
        and    eax, 76452062h
        outsb
        db  074h, 03Dh
        and    eax, 0a62h
loc_1_30C28:
        inc    esp
        dec    edi
loc_1_30C2A:
        dec    esi
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        xor    dl, byte ptr [eax + 6fh]
        db  073h, 074h
        and    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        and    byte ptr [eax + 6dh], bl
        imul   esi, dword ptr [eax + 43h], 253d4243h
        insb
        and    byte ptr [edx + 65h], dl
        popad
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 6c25h
        add    byte ptr [20200a21h], ah
loc_1_30C59:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    ebp
        db  076h, 065h
        outsb
        je     SHORT loc_1_30CBA
        je     NEAR PTR loc_1_30CA7
loc_1_30C6A:
        and    eax, 76452062h
        outsb
        db  074h, 03Dh
        and    eax, 0a62h
loc_1_30C78:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        popad
        db  064h, 070h, 06Fh
        db  073h, 074h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
        add    byte ptr [ebx + 42h], al
        xor    bh, byte ptr [edx]
        and    byte ptr [ebp + 76h], ah
        outsb
        db  074h, 020h
        db  073h, 069h
        outsb
        popad
        insb
        insb
        and    byte ptr fs:[eax + 47h], ch
loc_1_30CA7:
        bound  ebp, qword ptr [ebp + eax*2 + 76h]
        outsb
        db  074h, 03Dh
        and    eax, 0a6ch
loc_1_30CB4:
        pop    esp
        pop    esp
loc_1_30CBA:
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        arpl   word ptr cs:[eax], ax
loc_1_30CC8:
        pop    esp
        pop    esp
        jae    SHORT loc_1_30D42
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        arpl   word ptr cs:[eax], ax
loc_1_30CDC:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_30D52
        arpl   word ptr cs:[eax], ax
loc_1_30CF0:
        pop    esp
        pop    esp
        jae    SHORT loc_1_30D6A
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_30D66
        arpl   word ptr cs:[eax], ax
loc_1_30D04:
        pop    esp
        pop    esp
        jae    SHORT loc_1_30D7E
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        arpl   word ptr cs:[eax], ax
loc_1_30D18:
        pop    esp
        pop    esp
        jae    SHORT loc_1_30D92
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        arpl   word ptr cs:[eax], ax
loc_1_30D2C:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        arpl   word ptr cs:[eax], ax
loc_1_30D40:
        inc    esp
        inc    edx
loc_1_30D42:
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_30D8F
        db  072h, 06Dh
        inc    edx
        jne    SHORT loc_1_30DB4
        jb     NEAR PTR loc_1_30DA5
loc_1_30D52:
        je     NEAR PTR loc_1_30D75
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_30D7B
        db  067h, 065h, 074h, 020h
        bound  esi, qword ptr [ebp + 66h]
        db  066h, 065h, 072h, 020h
loc_1_30D66:
        db  070h, 041h
        dec    edi
        inc    ebx
loc_1_30D6A:
        inc    edx
        cmp    eax, 0a6c25h
loc_1_30D70:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
loc_1_30D75:
        inc    edi
        je     NEAR PTR loc_1_30DBF
        jb     SHORT loc_1_30DE8
loc_1_30D7B:
        inc    edx
        jne    SHORT loc_1_30DE4
loc_1_30D7E:
        db  066h, 065h, 072h, 053h
        je     NEAR PTR loc_1_30DA5
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_30DAB
        db  067h, 065h, 074h, 020h
loc_1_30D8F:
        bound  esi, qword ptr [ebp + 66h]
loc_1_30D92:
        jae    SHORT loc_1_30DB4
        jo     SHORT loc_1_30DD7
        dec    edi
        inc    ebx
        inc    edx
        cmp    eax, 6c206c25h
        outsb
        cmp    eax, 0a6425h
loc_1_30DA5:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
loc_1_30DAB:
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 052h
        arpl   word ptr [esi + 45h], si
loc_1_30DB4:
        jbe    SHORT loc_1_30E1B
        outsb
        db  074h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
loc_1_30DBF:
        db  067h, 065h, 074h, 020h
        db  065h, 076h, 065h
        outsb
        db  074h, 020h
        db  073h, 074h
        jb     SHORT loc_1_30E42
        arpl   word ptr [ecx + edi*2 + 70h], dx
        cmp    eax, 0a622520h
loc_1_30DD7:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        imul   sp, word ptr [ebp + 6ch], 4c64h
loc_1_30DE4:
        outsb
        inc    edx
        popad
loc_1_30DE8:
        and    byte ptr fs:[ecx + 2dh], cl
        imul   sp, word ptr [ebp + 6ch], 2064h
        je     SHORT loc_1_30E63
        outsd
        and    byte ptr [edi + ebp*2 + 6eh], ch
        and    byte ptr [bx + si + 4ch], dh
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        jae    SHORT loc_1_30E7A
        popad
        dec    ecx
        inc    esp
        cmp    eax, 0a7725h
        add    byte ptr [eax], al
loc_1_30E10:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        insd
        popad
        db  078h, 049h
loc_1_30E1B:
        imul   sp, word ptr [ebp + 6ch], 3d64h
        and    eax, 46692064h
        imul   esp, dword ptr [ebp + 6ch], 6e654c64h
        cmp    eax, 0a6425h
        add    byte ptr [eax], al
loc_1_30E34:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
        db  076h, 065h
        sub    byte ptr [ecx], ch
loc_1_30E42:
        and    byte ptr [ebx + 74h], dh
        popad
        dec    ecx
        inc    esp
        cmp    eax, 66207725h
        insb
        popad
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        add    byte ptr [20202021h], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_30E63:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        jae    SHORT loc_1_30EAD
        and    eax, 65722062h
        popad
        dec    edi
        db  070h, 074h
loc_1_30E7A:
        cmp    eax, 0a6225h
        add    byte ptr [edx + eax*2 + 47h], al
        cmp    ah, byte ptr [eax]
        push   edx
        arpl   word ptr [esi + 43h], si
        insd
        jo     SHORT loc_1_30EF8
        db  074h, 020h
        inc    ebx
        inc    ebx
        inc    edx
        xor    ah, byte ptr [eax]
        jo     SHORT loc_1_30F04
        db  073h, 074h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
loc_1_30EAD:
        arpl   word ptr cs:[eax], ax
loc_1_30EB0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edx
        arpl   word ptr [esi + 4ch], si
        dec    esp
        inc    ebx
        pop    edi
        xor    bl, byte ptr [edi + 4eh]
        jne    SHORT loc_1_30F2E
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [edx + 43h], dl
        push   esi
        and    byte ptr [ebx + 43h], al
        inc    edx
        and    byte ptr [eax + 53h], dh
        inc    ecx
        push   eax
        cmp    eax, 70206c25h
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 0a6c253dh
        add    byte ptr [eax], al
loc_1_30EE0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edx
        arpl   word ptr [esi + 4ch], si
        dec    esp
        inc    ebx
        pop    edi
        xor    bl, byte ptr [edi + 4eh]
        db  075h, 06Dh
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_30F18
loc_1_30EF8:
        je     NEAR PTR loc_1_30F1C
        jbe    NEAR PTR loc_1_30F64
        outsb
        db  074h, 00Ah
        add    byte ptr [eax], al
loc_1_30F04:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edx
        arpl   word ptr [esi + 4ch], si
        outsd
        outsd
        imul   esp, dword ptr [ecx + 68h], 65h
        popad
        and    byte ptr fs:[ebx + 61h], ah
loc_1_30F18:
        outsb
        daa
        je     SHORT loc_1_30F3C
loc_1_30F1C:
        je     NEAR PTR loc_1_30F40
        db  070h, 06Bh
        je     SHORT loc_1_30F44
        db  064h, 065h, 073h, 063h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_30F2C:
        inc    ebp
        push   edx
loc_1_30F2E:
        push   edx
        cmp    ah, byte ptr [eax]
        push   edx
        arpl   word ptr [esi + 4ch], si
        outsd
        outsd
        imul   esp, dword ptr [ecx + 68h], 65h
        popad
loc_1_30F3C:
        and    byte ptr fs:[ebx + 61h], ah
loc_1_30F40:
        outsb
        daa
        je     SHORT loc_1_30F64
loc_1_30F44:
        db  067h, 065h, 074h, 020h
        bound  esi, qword ptr [ebp + 66h]
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [edx], cx
        add    byte ptr [eax], al
        add    byte ptr [edx + 43h], dl
        push   esi
        cmp    ah, byte ptr [eax]
        jo     SHORT loc_1_30F9C
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
        inc    ecx
        dec    edi
loc_1_30F64:
        inc    ebx
        inc    edx
        cmp    eax, 66206c25h
        jb     SHORT loc_1_30FDA
        push   esp
        jns    SHORT loc_1_30FE0
        cmp    eax, 66206225h
        jb     SHORT loc_1_30FE5
        dec    esp
        outsb
        cmp    eax, 0a6425h
loc_1_30F80:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        insb
        insb
        arpl   word ptr [eax + 64h], cx
        db  072h, 03Dh
        and    eax, 2e000042h
        pop    esp
        pop    esp
        jae    SHORT loc_1_3100A
        arpl   word ptr [edx + esi*2 + 65h], bx
loc_1_30F9C:
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        arpl   word ptr cs:[eax], ax
loc_1_30FA4:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        outsb
        outsd
        outsb
        sub    eax, 746e6f43h
        imul   esp, dword ptr [edi + 75h], 2073756fh
        jb     SHORT loc_1_3101D
        db  076h, 04Ch
        outsb
        cmp    eax, 73206425h
        db  074h, 061h
        dec    ecx
        inc    esp
        cmp    eax, 6c207725h
        popad
        outsb
        dec    eax
        db  064h, 072h, 03Dh
        or     ah, byte ptr [42h]
loc_1_30FD8:
        push   edx
        inc    ebx
loc_1_30FDA:
        push   esi
        cmp    ah, byte ptr [eax]
        outsb
        outsd
        outsb
loc_1_30FE0:
        sub    eax, 746e6f43h
loc_1_30FE5:
        imul   esp, dword ptr [edi + 75h], 2073756fh
        insd
        db  073h, 067h
        push   esp
        db  079h, 070h
        cmp    eax, 64206225h
        insb
        arpl   word ptr [eax + 64h], cx
        db  072h, 03Dh
        and    eax, 42h
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
loc_1_3100A:
        jae    SHORT loc_1_3107E
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_3107A
        arpl   word ptr cs:[eax], ax
loc_1_31018:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
loc_1_3101D:
        inc    ebx
        outsd
        outsb
        db  074h, 069h
        jne    NEAR PTR loc_1_31094
        jne    SHORT loc_1_3109A
        and    byte ptr [edx + 63h], dh
        jbe    SHORT loc_1_31078
        outsb
        cmp    eax, 73206425h
        db  074h, 061h
        dec    ecx
        inc    esp
        cmp    eax, 6d207725h
        jae    SHORT loc_1_310A5
        push   esp
        db  079h, 070h
        cmp    eax, 0a6225h
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        jae    SHORT loc_1_310C2
        arpl   word ptr [edx + esi*2 + 65h], bx
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        arpl   word ptr cs:[eax], ax
loc_1_3105C:
        push   edx
        inc    ebx
        push   esi
        cmp    ah, byte ptr [eax]
        bound  esi, qword ptr [ebp + 66h]
        and    esp, dword ptr [656c2064h]
        outsb
        cmp    eax, 64206425h
        popad
        je     SHORT loc_1_310D4
        cmp    eax, 4125h
loc_1_31078:
        push   edx
        inc    ebx
loc_1_3107A:
        push   esi
        cmp    ah, byte ptr [eax]
        push   ebx
loc_1_3107E:
        dec    ebp
        inc    ecx
        dec    esp
        dec    esp
        pop    edi
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        and    byte ptr [ebp + 6eh], ch
        cmp    eax, 64206425h
        popad
        je     SHORT loc_1_310F5
loc_1_31094:
        cmp    eax, 45004125h
        push   edx
loc_1_3109A:
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 42766352h
loc_1_310A5:
        db  075h, 066h
        db  073h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        je     SHORT loc_1_310CF
        db  067h, 065h, 074h, 020h
        jo     SHORT loc_1_31120
        je     SHORT loc_1_310D7
        jae    NEAR PTR loc_1_3111E
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
loc_1_310C2:
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 42766352h
        jne    SHORT loc_1_31135
loc_1_310CF:
        db  073h, 020h
        arpl   word ptr [ecx + 6eh], sp
loc_1_310D4:
        daa
        je     SHORT loc_1_310F7
loc_1_310D7:
        db  067h, 065h, 074h, 020h
        bound  esi, qword ptr [ebp + 66h]
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [edx], cx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_310E8:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
loc_1_310F5:
        inc    esp
        inc    ebp
loc_1_310F7:
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_310FC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   esp
        db  072h, 063h
        dec    ecx
        dec    edi
        inc    ebx
        push   esp
        dec    esp
        sub    byte ptr [ecx], ch
        and    byte ptr [edx + esi*2 + 61h], dh
        arpl   word ptr [ebp + 20h], sp
        jo     SHORT loc_1_31175
        db  072h, 06Dh
        db  073h, 020h
        jo     SHORT loc_1_3115E
        popad
        je     SHORT loc_1_3117E
        inc    edx
loc_1_3111E:
        db  075h, 066h
loc_1_31120:
        and    byte ptr [esi + 55h], cl
        dec    esp
        dec    esp
        and    dword ptr [edx], ecx
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   esp
        jb     SHORT loc_1_31193
        dec    ecx
        dec    edi
        inc    ebx
        push   esp
        dec    esp
loc_1_31135:
        sub    byte ptr [ecx], ch
        and    byte ptr [eax + esi*2 + 63h], ch
        bound  eax, qword ptr [edx + 79h]
        je     SHORT loc_1_311A5
        db  073h, 052h
        db  065h, 074h, 075h
        db  072h, 06Eh
        and    byte ptr fs:[esi + 55h], cl
        dec    esp
        dec    esp
        and    dword ptr [edx], ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31154:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   esp
        db  072h, 063h
        dec    ecx
        dec    edi
loc_1_3115E:
        inc    ebx
        push   esp
        dec    esp
        sub    byte ptr [ecx], ch
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 69h], 6c74636fh
        push   edx
        db  065h, 071h, 03Dh
loc_1_31175:
        and    eax, 0a77h
        add    byte ptr [eax], al
loc_1_3117C:
        inc    ebp
        push   edx
loc_1_3117E:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edx
        db  075h, 069h
        insb
        dec    eax
        jb     NEAR PTR loc_1_311CC
        jne    SHORT loc_1_311F2
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
loc_1_31193:
        db  074h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[ecx + 55h], al
        pop    eax
        pop    edi
        pop    eax
        dec    ebp
        dec    ecx
loc_1_311A5:
        push   esp
        and    byte ptr [edx + 75h], ah
        or     al, byte ptr [eax]
loc_1_311AC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        outsb
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 0a6ch
        db  000h
loc_1_311C4:
        pop    esp
        pop    esp
        jae    SHORT loc_1_3123E
loc_1_311CC:
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_311DC:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edx
        jne    SHORT loc_1_3124D
        insb
        dec    eax
        jb     NEAR PTR loc_1_3122C
        db  075h, 066h
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 61h], ah
        outsb
loc_1_311F2:
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_31219
        bound  esi, qword ptr [ebp + 66h]
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [edx], cx
        add    byte ptr [eax], al
loc_1_31204:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        pop    eax
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6dh], 20746c70h
        jae    SHORT loc_1_3127D
        outsb
loc_1_31219:
        and    byte ptr fs:[ecx + 62h], ah
        outsd
        jb     SHORT loc_1_31294
        and    byte ptr fs:[eax], ch
        db  072h, 065h
        db  073h, 065h
        db  074h, 029h
        or     al, byte ptr [eax]
loc_1_3122C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
loc_1_3123E:
        db  075h, 06Dh
        cmp    eax, 6e206425h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
loc_1_3124D:
        db  073h, 03Dh
        and    eax, 4370206ch
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        inc    ebx
        inc    edx
        pop    eax
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6dh], 20746c70h
        jae    SHORT loc_1_312D5
        outsb
        and    byte ptr fs:[esi + 61h], ah
        db  074h, 061h
        insb
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
loc_1_3127D:
        jb     SHORT loc_1_31289
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
        inc    ebx
loc_1_31289:
        inc    edx
        cmp    eax, 61206c25h
        jo     NEAR PTR loc_1_312E0
        jne    SHORT loc_1_31301
loc_1_31294:
        cmp    eax, 6e206425h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        jae    SHORT loc_1_312E0
        and    eax, 4370206ch
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        db  000h
loc_1_312B0:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_312C8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        je     NEAR PTR loc_1_31319
        jb     NEAR PTR loc_1_3131D
        outsb
loc_1_312D5:
        outsw
        sub    byte ptr [ecx], ch
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [ebp + 41h], cl
        inc    ebx
loc_1_312E0:
        and    byte ptr [eax + 65h], ch
        popad
        jb     NEAR PTR loc_1_31308
        bound  esi, qword ptr [ebp + 66h]
        jb     NEAR PTR loc_1_312F9
        db  000h
loc_1_312F0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 048h
loc_1_312F9:
        db  064h, 072h, 049h
        outsb
        outsw
        sub    byte ptr [ecx], ch
loc_1_31301:
        and    byte ptr [ebp + 41h], cl
        inc    ebx
        and    byte ptr [eax + 65h], ch
loc_1_31308:
        popad
        db  064h, 065h, 072h, 020h
        je     SHORT loc_1_3137E
        outsd
        and    byte ptr [ebx + 68h], dh
        outsd
        db  072h, 074h
        or     al, byte ptr [eax]
loc_1_31318:
        inc    ebp
loc_1_31319:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
loc_1_3131D:
        inc    edi
        je     NEAR PTR loc_1_31369
        db  064h, 072h, 049h
        outsb
        outsw
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 72h], dl
        arpl   word ptr [eax], sp
        push   edx
        je     SHORT loc_1_31396
        and    byte ptr [ecx + 6eh], ch
        outsw
        and    byte ptr [66756220h], ch
        and    byte ptr [edi + ebp*2 + 6fh], dh
        and    byte ptr [ebx + 68h], dh
        outsd
        db  072h, 074h
        or     al, byte ptr [eax]
loc_1_31348:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        db  065h, 074h, 048h
        jb     NEAR PTR loc_1_3139D
        outsb
        outsw
        sub    byte ptr [ecx], ch
        and    byte ptr [ebx + 6fh], dl
        jne    SHORT loc_1_313D0
        arpl   word ptr [ebp + 20h], sp
        push   edx
        outsd
        jne    SHORT loc_1_313D9
        and    byte ptr gs:[ecx + 6eh], ch
loc_1_31369:
        outsw
        and    byte ptr [ecx + 6eh], ch
        jbe    SHORT loc_1_313D1
        insb
        db  069h, 064h, 00Ah, 000h, 000h, 000h, 000h
loc_1_31378:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esi
loc_1_3137E:
        outsd
        outsb
        inc    ecx
        jo     SHORT loc_1_313F3
        insb
        pop    eax
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6dh], 20746c70h
        jae    SHORT loc_1_313F5
        outsb
        and    byte ptr fs:[ecx + 62h], ah
        outsd
loc_1_31396:
        jb     SHORT loc_1_3140C
        or     al, byte ptr fs:[eax]
loc_1_3139C:
        inc    ebp
loc_1_3139D:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
        cmp    eax, 6e206425h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 5370206ch
        inc    ecx
        push   eax
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 0a6c253dh
        add    byte ptr [eax], al
loc_1_313D0:
        inc    ebp
loc_1_313D1:
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        dec    esi
        outsd
        outsb
        inc    ecx
loc_1_313D9:
        jo     SHORT loc_1_3144B
        insb
        pop    eax
        insd
        imul   esi, dword ptr [ebx + eax*2 + 6dh], 20746c70h
        jae    SHORT loc_1_3144D
        outsb
        and    byte ptr fs:[esi + 61h], ah
        db  074h, 061h
        insb
        and    byte ptr [ebp + 72h], ah
loc_1_313F3:
        db  072h, 06Fh
loc_1_313F5:
        jb     SHORT loc_1_31401
        db  000h
loc_1_313F8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax + 41h], dh
        inc    ebx
loc_1_31401:
        inc    edx
        cmp    eax, 61206c25h
        db  064h, 070h, 04Eh
        db  075h, 06Dh
loc_1_3140C:
        cmp    eax, 6e206425h
        imul   esi, dword ptr fs:[ebx + 53h], 75746174h
        db  073h, 03Dh
        and    eax, 5370206ch
        inc    ecx
        push   eax
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 0a6c253dh
        add    byte ptr [eax], al
loc_1_3142C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        outsb
        inc    edx
        imul   esp, dword ptr [edi + 41h], 72467373h
        insd
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_31465
        je     NEAR PTR loc_1_31469
        jo     SHORT loc_1_314B6
loc_1_3144B:
        je     SHORT loc_1_3146D
loc_1_3144D:
        jae    NEAR PTR loc_1_314B4
        or     al, byte ptr [eax]
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        outsb
        inc    edx
        imul   esp, dword ptr [edi + 41h], 72467373h
loc_1_31465:
        insd
        and    byte ptr [ebx + 61h], ah
loc_1_31469:
        outsb
        daa
        db  074h, 020h
loc_1_3146D:
        db  067h, 065h, 074h, 020h
        bound  esi, qword ptr [ebp + 66h]
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [edx], cx
        add    byte ptr [eax], al
loc_1_3147C:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        outsb
        push   ebx
        insd
        insb
        inc    esi
        jb     SHORT loc_1_314F8
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_314B2
        je     NEAR PTR loc_1_314B6
        push   ebx
        dec    ebp
        inc    ecx
        dec    esp
        dec    esp
        pop    edi
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        and    byte ptr [eax + 41h], dh
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
loc_1_314B2:
        outsb
loc_1_314B4:
        push   ebx
loc_1_314B6:
        insd
        insb
        inc    esi
        db  072h, 06Dh
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_1_314E2
        je     NEAR PTR loc_1_314E6
        jo     SHORT loc_1_31533
        je     SHORT loc_1_314EA
        jae    NEAR PTR loc_1_31531
        or     al, byte ptr [eax]
loc_1_314D0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        outsb
        push   ebx
        insd
        insb
        inc    esi
        db  072h, 06Dh
        and    byte ptr [ebx + 61h], ah
loc_1_314E2:
        outsb
        daa
        db  074h, 020h
loc_1_314E6:
        db  067h, 065h, 074h, 020h
loc_1_314EA:
        bound  esi, qword ptr [ebp + 66h]
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [edx], cx
        add    byte ptr [ebp + 72h], dl
        insd
loc_1_314F8:
        imul   ebp, dword ptr [esi + 61h], 46496574h
        jb     SHORT loc_1_3156E
        jae    SHORT loc_1_31523
        db  070h, 04Ch
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  073h, 074h
        popad
        dec    ecx
        inc    esp
        cmp    eax, 0a7725h
        add    byte ptr [eax], al
loc_1_31518:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebx
        db  074h, 075h
        arpl   word ptr [ebx + 20h], bp
loc_1_31523:
        db  065h, 076h, 065h
        outsb
        je     SHORT loc_1_31549
        sub    eax, 45564520h
        dec    esi
        push   esp
        pop    edi
loc_1_31531:
        je     SHORT loc_1_315AC
loc_1_31533:
        jo     SHORT loc_1_3159A
        cmp    eax, 65206225h
        db  076h, 065h
        outsb
        je     SHORT loc_1_31593
        db  079h, 070h
        cmp    eax, 0a6225h
        add    byte ptr [esi], ch
loc_1_31549:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_31560:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
        outsb
loc_1_3156E:
        jae    SHORT loc_1_315DD
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_31578:
        pop    eax
        dec    ebp
        push   esp
        cmp    ah, byte ptr [eax]
        db  070h, 041h
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
        push   eax
        imul   esi, dword ptr [ebp + edi + 25h], 6ch
        and    byte ptr [eax + 6bh], dh
        je     SHORT loc_1_315DD
        outsb
loc_1_31593:
        cmp    eax, 62206425h
        jne    SHORT loc_1_31600
loc_1_3159A:
        inc    ebx
        outsb
        db  074h, 03Dh
        and    eax, 0a64h
        add    byte ptr [eax + 4dh], bl
        push   esp
        cmp    ah, byte ptr [eax]
        bound  esi, qword ptr [ebp + 66h]
loc_1_315AC:
        and    esp, dword ptr [42702064h]
        jne    SHORT loc_1_3161A
        cmp    eax, 70206c25h
        inc    ebx
        cmp    eax, 6c206c25h
        outsb
        cmp    eax, 64206425h
        popad
        je     SHORT loc_1_3162A
        cmp    eax, 4125h
        add    byte ptr [eax], al
loc_1_315D0:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
loc_1_315DD:
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_315E8:
        pop    esp
        pop    esp
        jae    SHORT loc_1_31662
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_31600:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        inc    edx
        jne    SHORT loc_1_31676
        db  073h, 020h
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_1_31666
loc_1_3161A:
        arpl   word ptr [ecx + eax*2 + 64h], bp
        db  064h, 072h, 03Dh
        and    eax, 0a6ch
        add    byte ptr [eax], al
loc_1_31628:
        inc    ebp
        push   edx
loc_1_3162A:
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        inc    edx
        jne    SHORT loc_1_3169E
        jae    SHORT loc_1_3165A
        outsb
        outsd
        db  074h, 020h
        imul   ebp, dword ptr [esi + 75h], 63206573h
        db  075h, 072h
        dec    esp
        arpl   word ptr [ecx + eax*2 + 64h], bp
        db  064h, 072h, 03Dh
        and    eax, 0a6ch
loc_1_31654:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
loc_1_3165A:
        imul   esi, dword ptr [edx + 65h], 74696d58h
        inc    edx
loc_1_31662:
        jne    SHORT loc_1_316CA
        jae    SHORT loc_1_31686
loc_1_31666:
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [eax + 4ch], dh
        arpl   word ptr [edx + eax*2 + 75h], bp
        cmp    ax, 6c25h
        or     al, byte ptr [eax]
loc_1_31676:
        add    byte ptr [eax], al
loc_1_31678:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        inc    edx
loc_1_31686:
        db  075h, 066h
        jae    SHORT loc_1_316AA
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [eax + 4ch], dh
        arpl   word ptr [edx + eax*2 + 75h], bp
        cmp    ax, 6c25h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_3169C:
        inc    ebp
        push   edx
loc_1_3169E:
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        inc    edx
loc_1_316AA:
        db  075h, 066h
        db  073h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
        bound  esi, qword ptr [ebp + 66h]
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [edx], cx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
loc_1_316CA:
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
        arpl   word ptr [eax], sp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [ebp + 41h], cl
        inc    ebx
        and    byte ptr [eax + 65h], ch
        popad
        jb     NEAR PTR loc_1_31708
        imul   ebp, dword ptr [esi + 66h], 0a6fh
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
        arpl   word ptr [eax], sp
        arpl   word ptr [ecx + 6eh], sp
loc_1_31708:
        daa
        db  074h, 020h
        je     NEAR PTR loc_1_3172F
        db  070h, 06Bh
        db  074h, 020h
        jae    NEAR PTR loc_1_3177A
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
loc_1_3172F:
        arpl   word ptr [eax], sp
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        bound  esi, qword ptr [ebp + 69h]
        insb
        and    byte ptr fs:[ebp + 41h], cl
        inc    ebx
        das
        dec    esp
        dec    esp
        inc    ebx
        and    byte ptr [eax + 65h], ch
        popad
        jb     NEAR PTR loc_1_31756
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31750:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
loc_1_31756:
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
        arpl   word ptr [eax], sp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [ecx + 75h], dl
        jne    NEAR PTR loc_1_317D3
        and    byte ptr [ecx], dh
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_31774:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
loc_1_3177A:
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
        arpl   word ptr [eax], sp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [ecx + 75h], dl
        jne    NEAR PTR loc_1_317F7
        and    byte ptr [edx], dh
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_31798:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
        arpl   word ptr [eax], sp
        outsb
        outsd
        and    byte ptr [edx + 75h], ah
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [eax], sp
        sub    byte ptr [ecx], dh
        sub    dword ptr [edx], ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
loc_1_317D3:
        arpl   word ptr [eax], sp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        xor    dword ptr [edx], ecx
        add    byte ptr [eax], al
loc_1_317E4:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
loc_1_317F7:
        arpl   word ptr [eax], sp
        outsb
        outsd
        and    byte ptr [edx + 75h], ah
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [eax], sp
        sub    byte ptr [edx], dh
        sub    dword ptr [edx], ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        imul   esi, dword ptr [edx + 65h], 74696d58h
        push   eax
        imul   esi, dword ptr [esp + eax*2 + 65h], 73h
        arpl   word ptr [eax], sp
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [edx + 75h], al
        jb     NEAR PTR loc_1_3184C
        xor    cl, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_31830:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        pop    eax
        insd
        imul   esi, dword ptr [ebx + eax*2 + 43h], 43702042h
        inc    ebx
        inc    edx
        cmp    eax, 51206c25h
        xor    dword ptr [51206c25h], edi
loc_1_3184C:
        xor    bh, byte ptr [62206c25h]
        db  075h, 066h
        dec    esp
        outsb
        pop    edi
        xor    dword ptr [62206425h], edi
        jne    SHORT loc_1_318C6
        dec    esp
        outsb
        pop    edi
        xor    bh, byte ptr [0a6425h]
        add    byte ptr [eax], al
loc_1_3186C:
        inc    esp
        inc    edx
        inc    edi
        cmp    ah, byte ptr [eax]
        pop    eax
        insd
        imul   esi, dword ptr [ebx + eax*2 + 43h], 2d492042h
        db  066h, 072h, 06Dh
        and    byte ptr [ecx + ebp*2 + 6eh], ch
        imul   esp, dword ptr [eax], 63h
        insb
        outsd
        db  073h, 065h
        and    byte ptr fs:[eax + 4ch], dh
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  070h, 043h
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        pop    eax
        insd
        imul   esi, dword ptr [ebx + eax*2 + 43h], 61622042h
        and    byte ptr fs:[ecx + 2dh], cl
        jb     NEAR PTR loc_1_31917
        insd
        and    byte ptr gs:[eax + 4ch], dh
        imul   ebp, dword ptr [esi + 6bh], 206c253dh
        db  070h, 043h
        inc    ebx
        inc    edx
loc_1_318C6:
        cmp    eax, 72206c25h
        db  065h, 074h, 043h
        outsd
        cmp    eax, 0a6225h
        add    byte ptr [eax], al
loc_1_318D8:
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_318F0:
        pop    esp
        pop    esp
        jae    SHORT loc_1_3196A
        arpl   word ptr [esp + esi*2 + 72h], bx
        popad
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esi + ebp + 63h], 0
loc_1_31908:
        pop    eax
        dec    ebp
        push   esp
        cmp    ah, byte ptr [eax]
        pop    eax
        insd
        imul   esi, dword ptr [esi + ecx*2 + 64h], 6b507369h
loc_1_31917:
        db  074h, 028h
        sub    dword ptr [eax], esp
        db  070h, 061h
        arpl   word ptr [ebx + 65h], bp
        db  074h, 020h
        db  073h, 065h
        outsb
        je     SHORT loc_1_31947
        jo     SHORT loc_1_3196A
        inc    ebx
        inc    edx
        cmp    eax, 70206c25h
        push   eax
        imul   esi, dword ptr [ebp + edi + 25h], 6ch
        or     al, byte ptr [eax]
loc_1_31938:
        pop    eax
        dec    ebp
        push   esp
        cmp    ah, byte ptr [eax]
        pop    eax
        insd
        imul   esi, dword ptr [esi + ecx*2 + 64h], 6b507369h
loc_1_31947:
        db  074h, 020h
        db  073h, 065h
        outsb
        and    byte ptr fs:[eax + 65h], dh
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 43417020h
        inc    edx
        cmp    eax, 70206c25h
        push   eax
        imul   esi, dword ptr [ebp + edi + 25h], 6ch
        and    byte ptr [ebp + 76h], ah
        outsb
loc_1_3196A:
        db  074h, 03Dh
        and    eax, 45000a6ch
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        pop    eax
        insd
        imul   esi, dword ptr [esi + ecx*2 + 64h], 6b507369h
        je     SHORT loc_1_319A1
        db  073h, 065h
        outsb
        and    byte ptr fs:[esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 43417020h
        inc    edx
        cmp    eax, 70206c25h
        push   eax
        imul   esi, dword ptr [ebp + edi + 25h], 6ch
        and    byte ptr [esi + 64h], ch
        jae    SHORT loc_1_31A0A
loc_1_319A1:
        push   ebx
        je     SHORT loc_1_31A05
        je     SHORT loc_1_31A1B
        db  073h, 03Dh
        and    eax, 45000a6ch
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edx
        popad
        and    byte ptr fs:[eax + 4ch], dh
        arpl   word ptr [ebx + eax*2 + 43h], bp
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        db  073h, 072h
        arpl   word ptr [edi + esi*2 + 69h], bx
        db  072h, 065h
        arpl   word ptr [ebx + 62h], sp
        arpl   word ptr cs:[eax], ax
loc_1_319D8:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        bound  esp, qword ptr [ecx + 64h]
        and    byte ptr [eax + 4ch], dh
        arpl   word ptr [eax + edx*2 + 61h], bp
        db  072h, 06Dh
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_319F0:
        inc    ebp
        push   edx
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        ja     SHORT loc_1_31A62
        jb     SHORT loc_1_31A60
        and    byte ptr [eax + 50h], dh
        db  072h, 06Fh
        db  078h, 079h
        inc    ebx
        inc    ebx
        inc    edx
loc_1_31A05:
        cmp    eax, 70206c25h
loc_1_31A0A:
        dec    esp
        arpl   word ptr [ecx + eax*2 + 70h], bp
        db  070h, 06Ch
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 0a6c25h
        add    byte ptr [eax], al
loc_1_31A1B:
        add    byte ptr [ebp + 52h], al
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb
        ja     SHORT loc_1_31A8E
        jb     SHORT sub_1_31A8C
        and    byte ptr [eax + 47h], dh
        bound  ebp, qword ptr [ecx + eax*2 + 70h]
        jo     SHORT loc_1_31A9C
        inc    ebx
        inc    ebx
        inc    edx
        cmp    eax, 63206c25h
        outsd
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_31AA4
        and    byte ptr fs:[ebx + 43h], al
        inc    edx
        cmp    eax, 42250ah
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        pop    esp
        jae    SHORT loc_1_31AC6
        arpl   word ptr [edi + esi*2 + 69h], bx
        db  072h, 065h
        arpl   word ptr [ebx + 62h], sp
        arpl   word ptr cs:[eax], ax
loc_1_31A60:
        xor    al, 2eh
loc_1_31A62:
        xor    byte ptr [esi], ch
        cmp    dword ptr [322e30h], esi
        add    byte ptr [eax], al
loc_1_31A6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31A70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31A74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31A78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31A7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31A80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_31A84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
