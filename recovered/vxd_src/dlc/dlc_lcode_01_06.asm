; dlc_lcode_01_06.asm -- DLC object 1 segment _LTEXT  (part 6/9)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_1A171:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 8
        je     NEAR PTR loc_1_1A1BF
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1A1BF
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A1CB
        add    esp, 0ch
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A1C6
loc_1_1A1BF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A1C6
loc_1_1A1C6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A1CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 4], 8
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 68h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 84h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 86h
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1A227
loc_1_1A223:
        inc    word ptr [ebp - 8]
loc_1_1A227:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 5
        jge    NEAR PTR loc_1_1A26A
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + edx + 10h], al
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx], 0
        jmp    NEAR PTR loc_1_1A223
loc_1_1A26A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4fh]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 15h], al
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1A285
loc_1_1A281:
        inc    word ptr [ebp - 8]
loc_1_1A285:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_1A2BD
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 91h]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + edx + 16h], al
        jmp    NEAR PTR loc_1_1A281
loc_1_1A2BD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 90h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 1ch], al
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 66h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 1eh], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 84h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A2F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_1A3FF
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 38h
        push   eax
        call   sub_1_19800
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1A3FF
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_1A40B
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_1A35D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1A35D:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1A38D
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1A38D
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A406
loc_1_1A38D:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
loc_1_1A39D:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 38h
        push   eax
        call   sub_1_19800
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_1A3E7
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1F076
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        inc    word ptr [eax + 1ch]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1A39D
loc_1_1A3E7:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A406
loc_1_1A3FF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A406
loc_1_1A406:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A40B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 4], 4
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 1ch], 1
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 24h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1A473
loc_1_1A46F:
        inc    word ptr [ebp - 4]
loc_1_1A473:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_1A499
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [eax + ecx + 26h], 0
        jmp    NEAR PTR loc_1_1A46F
loc_1_1A499:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1F076
        add    esp, 8
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A4B7:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_1A5D8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 30h
        push   eax
        call   sub_1_19A9D
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1A5D8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1A5E4
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1A53A
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A5DF
loc_1_1A53A:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
loc_1_1A550:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 30h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_1A5AE
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1A585
        jmp    NEAR PTR loc_1_1A5AE
loc_1_1A585:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 30h
        push   eax
        call   sub_1_19A9D
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        inc    word ptr [eax + 0ch]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1A550
loc_1_1A5AE:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_24419
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A5DF
loc_1_1A5D8:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A5DF
loc_1_1A5DF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A5E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 24h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1A64C
loc_1_1A648:
        inc    word ptr [ebp - 4]
loc_1_1A64C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_1A672
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + ecx + 26h], 0
        jmp    NEAR PTR loc_1_1A648
loc_1_1A672:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A677:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_1A733
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19A9D
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_1A733
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_1A715
loc_1_1A6CB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19A9D
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_1A715
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ah]
        push   eax
        call   sub_1_19871
        add    esp, 8
        inc    word ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_1A710
        jmp    NEAR PTR loc_1_1A715
loc_1_1A710:
        jmp    NEAR PTR loc_1_1A6CB
loc_1_1A715:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_1A73F
        add    esp, 0ch
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A73A
loc_1_1A733:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A73A
loc_1_1A73A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A73F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 24h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_1A78B
loc_1_1A787:
        inc    word ptr [ebp - 4]
loc_1_1A78B:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_1A7B1
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + ecx + 26h], 0
        jmp    NEAR PTR loc_1_1A787
loc_1_1A7B1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 18h], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_1A803
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        inc    eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_24419
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
        jmp    NEAR PTR loc_1_1A820
loc_1_1A803:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 1
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
loc_1_1A820:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A825:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1A850
loc_1_1A842:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_1A850:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1A8AE
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        test   ecx, eax
        je     NEAR PTR loc_1_1A8A9
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_198DA
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1A8B5
loc_1_1A8A9:
        jmp    NEAR PTR loc_1_1A842
loc_1_1A8AE:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A8B5
loc_1_1A8B5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A8BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 40h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1A8F7
loc_1_1A8EF:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_1A8F7:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1A92E
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1A92E
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 20h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1A8EF
loc_1_1A92E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1A944
loc_1_1A93C:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_1A944:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1A9BE
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1A9BE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_1A96E
loc_1_1A966:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1A96E:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1A9B9
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1A9B9
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1A9B4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 20h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
loc_1_1A9B4:
        jmp    NEAR PTR loc_1_1A966
loc_1_1A9B9:
        jmp    NEAR PTR loc_1_1A93C
loc_1_1A9BE:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_1A9C6
loc_1_1A9C6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A9CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 0
        je     NEAR PTR loc_1_1A9FA
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 3]
        and    eax, 1
        mov    dword ptr [ebp - 0ch], eax
loc_1_1A9FA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1AA40
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_1AA40
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 27h
        jne    NEAR PTR loc_1_1AA9D
loc_1_1AA40:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 10h
        je     NEAR PTR loc_1_1AA98
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 18h
loc_1_1AA98:
        jmp    NEAR PTR loc_1_1AB14
loc_1_1AA9D:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 10h
        je     NEAR PTR loc_1_1AAF5
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 18h
loc_1_1AAF5:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1AB14:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1ACD2
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 16h]
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        not    eax
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_1ABE9
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        push   8
        push   18h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
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
        push   3a6h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1B2B2
loc_1_1ABE9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1A73F
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 10h
        je     NEAR PTR loc_1_1AC73
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1AC73:
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1ACCD
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_00E5
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_1ACC1
        push   0ch
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   3c7h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1B2B2
loc_1_1ACC1:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_00EB
        add    esp, 4
loc_1_1ACCD:
        jmp    NEAR PTR loc_1_1B22F
loc_1_1ACD2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_1B1FB
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
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1AD45
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 40h
        je     NEAR PTR loc_1_1AD45
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 1bh
        jne    NEAR PTR loc_1_1ADC5
loc_1_1AD45:
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_1ADC0
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_00E5
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_1AD93
        push   0ch
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   3e6h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1B2B2
loc_1_1AD93:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 80h
        je     NEAR PTR loc_1_1ADC0
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_1ADC0:
        jmp    NEAR PTR loc_1_1B2B2
loc_1_1ADC5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 30h], eax
        jmp    NEAR PTR loc_1_1B16D
loc_1_1ADD8:
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 8]
        add    eax, 40h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_1AE85
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1A73F
        add    esp, 0ch
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 10h
        je     NEAR PTR loc_1_1AE37
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1AE37:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 10h
        je     NEAR PTR loc_1_1AE6C
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1AE6C:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        jmp    NEAR PTR loc_1_1AF51
loc_1_1AE85:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 17h], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_1AF3E
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1AF26
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 17h], al
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 6]
        mov    byte ptr [eax + 17h], 0
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 4ch], 0
        je     NEAR PTR loc_1_1AEFE
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 4ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1AF07
loc_1_1AEFE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 48h], eax
loc_1_1AF07:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        jmp    NEAR PTR loc_1_1AF39
loc_1_1AF26:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 48h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_1AF39:
        jmp    NEAR PTR loc_1_1AF51
loc_1_1AF3E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 48h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_1AF51:
        jmp    NEAR PTR loc_1_1B1F6
loc_1_1AF56:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1AF9C
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
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_1B06F
loc_1_1AF9C:
        mov    eax, dword ptr [ebp - 10h]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_1AFD1
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
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_1B06F
loc_1_1AFD1:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_1B00E
        mov    eax, dword ptr [ebp - 10h]
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
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_1B06F
loc_1_1B00E:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_1B04C
        mov    eax, dword ptr [ebp - 10h]
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
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_1B06F
loc_1_1B04C:
        mov    eax, dword ptr [ebp - 10h]
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
        mov    dword ptr [ebp - 24h], eax
loc_1_1B06F:
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   2
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 20h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_1B0B4
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   2
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 8]
        add    eax, 40h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_1B14B
loc_1_1B0B4:
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1A5E4
        add    esp, 8
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        mov    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [ecx + 10h], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 10h
        je     NEAR PTR loc_1_1B0FD
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1B0FD:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 10h
        je     NEAR PTR loc_1_1B132
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1B132:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        jmp    NEAR PTR loc_1_1B15E
loc_1_1B14B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 30h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_1B15E:
        jmp    NEAR PTR loc_1_1B1F6
loc_1_1B163:
        jmp    NEAR PTR loc_1_1B1F6
        db  0E9h, 089h, 000h, 000h, 000h
loc_1_1B16D:
        cmp    dword ptr [ebp - 30h], 35h
        ja     NEAR PTR loc_1_1B1F6
        mov    eax, dword ptr [ebp - 30h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 00Ch, 00Ch, 001h, 001h, 00Ch, 002h, 002h, 002h, 00Ch, 003h, 003h
        db  00Ch, 004h, 004h, 004h, 004h, 004h, 00Ch, 00Ch, 005h, 005h, 005h, 005h, 00Ch, 006h, 006h, 006h
        db  006h, 006h, 006h, 00Ch, 007h, 007h, 007h, 007h, 00Ch, 00Ch, 00Ch, 00Ch, 008h, 008h, 00Ch, 00Ch
        db  009h, 009h, 00Ch, 00Ch, 00Ch, 00Ah, 00Ah, 00Ch, 00Bh, 00Bh
loc_1_1B1F6:
        jmp    NEAR PTR loc_1_1B22F
loc_1_1B1FB:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_1B221
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_24419
        add    esp, 4
        jmp    NEAR PTR loc_1_1B22F
loc_1_1B221:
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_242B3
        add    esp, 8
loc_1_1B22F:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_1B29C
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_00E5
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_1B26F
        push   0ch
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4adh
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1B2B2
loc_1_1B26F:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 80h
        je     NEAR PTR loc_1_1B29C
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_1B29C:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1B2B2
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_1_2877E
        add    esp, 4
loc_1_1B2B2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1B2B7:
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
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_1B300
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
        jmp    NEAR PTR loc_1_1B448
loc_1_1B300:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
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
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 40h
        push   eax
        call   sub_1_19A0D
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1B35A
loc_1_1B352:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_1B35A:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1B3E2
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_1B3E2
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 20h
        push   eax
        call   sub_1_19A0D
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_1B39A
loc_1_1B392:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
loc_1_1B39A:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1B3DD
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_1B3DD
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1B3D8
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 20h
        push   eax
        call   sub_1_19A0D
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_1_1B3D8:
        jmp    NEAR PTR loc_1_1B392
loc_1_1B3DD:
        jmp    NEAR PTR loc_1_1B352
loc_1_1B3E2:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1B42A
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1B412
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
loc_1_1B412:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0ah
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_242B3
        add    esp, 8
loc_1_1B42A:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1B448
loc_1_1B448:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_1B450:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_1B510
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 0c000c0h
        jne    NEAR PTR loc_1_1B4A4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_1B4A4
        mov    dword ptr [ebp - 10h], 1
        jmp    NEAR PTR loc_1_1B4AB
loc_1_1B4A4:
        mov    dword ptr [ebp - 10h], 0
loc_1_1B4AB:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_1B50B
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 50h]
        shl    eax, 8
        and    eax, 0ff0000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 50h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 50h]
        shl    ecx, 18h
        and    ecx, 0ff000080h
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 50h]
        shr    ecx, 18h
        and    ecx, 7fh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    eax, dword ptr [ecx + 2]
        test   eax, 0ffffff7fh
        jne    NEAR PTR loc_1_1B50B
        xor    eax, eax
        jmp    NEAR PTR loc_1_1B608
loc_1_1B50B:
        jmp    NEAR PTR loc_1_1B5FE
loc_1_1B510:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 30003h
        jne    NEAR PTR loc_1_1B5FE
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1B5B5
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0bh], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0ah], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 9], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 7], al
        jmp    NEAR PTR loc_1_1B5C8
loc_1_1B5B5:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2]
        mov    dword ptr [ebp - 0ah], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0ch], ax
loc_1_1B5C8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 50h]
        test   dword ptr [ebp - 0ah], eax
        je     NEAR PTR loc_1_1B5E8
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 50h]
        not    eax
        test   dword ptr [ebp - 0ah], eax
        je     NEAR PTR loc_1_1B5FE
loc_1_1B5E8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ah]
        cmp    dword ptr [eax + 54h], ecx
        je     NEAR PTR loc_1_1B5FE
        xor    eax, eax
        jmp    NEAR PTR loc_1_1B608
loc_1_1B5FE:
        mov    eax, 1
        jmp    NEAR PTR loc_1_1B608
loc_1_1B608:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1B60D:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 1], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 18h]
        test   eax, eax
        je     NEAR PTR loc_1_1B759
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 12h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 13h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 0dh], al
        jmp    NEAR PTR loc_1_1B78D
loc_1_1B759:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 0ah], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 8], ax
loc_1_1B78D:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1B84A
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_1B7F5
loc_1_1B7F1:
        inc    word ptr [ebp - 0ch]
loc_1_1B7F5:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        sub    eax, 2
        cdq
        sub    eax, edx
        sar    eax, 1
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1B845
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [ecx + eax*2 + 2]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 4]
        mov    word ptr [edx + ecx*2 + 2], ax
        jmp    NEAR PTR loc_1_1B7F1
loc_1_1B845:
        jmp    NEAR PTR loc_1_1B850
loc_1_1B84A:
        mov    word ptr [ebp - 10h], 0
loc_1_1B850:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1B871
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        mov    dword ptr [ebp - 8], eax
loc_1_1B871:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 24h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1B8E0
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_1B894
loc_1_1B890:
        inc    word ptr [ebp - 0ch]
loc_1_1B894:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1B8D7
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ah]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_1B890
loc_1_1B8D7:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
loc_1_1B8E0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, 0eh
        mov    word ptr [ebp - 14h], ax
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_1B907
loc_1_1B907:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1B90C:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [ecx + eax*8 + 5eh]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        test   al, 20h
        jne    NEAR PTR loc_1_1B976
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1B97E
loc_1_1B976:
        mov    eax, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_1B97E:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 2ch], ax
        mov    eax, dword ptr [ebp - 34h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    eax, 18h
        jne    NEAR PTR loc_1_1BA0D
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1B60D
        add    esp, 14h
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        add    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 1ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 2ch], ax
        mov    ax, word ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_1_1BA3E
loc_1_1BA0D:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 3ah
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 1ah
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1B60D
        add    esp, 14h
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax], 0
loc_1_1BA3E:
        mov    eax, dword ptr [ebp - 34h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 1ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        add    eax, dword ptr [ecx + 2ah]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 24h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        mov    ax, word ptr [ebp - 2ch]
        mov    word ptr [ebp - 30h], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_1BAB4
loc_1_1BAAC:
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 20h], eax
loc_1_1BAB4:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_1BD90
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1BB33
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 20h], eax
        jne    NEAR PTR loc_1_1BAE9
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        test   al, 20h
        je     NEAR PTR loc_1_1BB33
loc_1_1BAE9:
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1BB28
        push   15h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   169h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1BD90
loc_1_1BB28:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 20h]
        mov    word ptr [ecx + 6], ax
loc_1_1BB33:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 20h], eax
        je     NEAR PTR loc_1_1BB7B
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 30h], ax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1BB7B
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax], 0
loc_1_1BB7B:
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1BBC6
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1BBC6
        push   15h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   187h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1BD90
loc_1_1BBC6:
        mov    word ptr [ebp - 24h], 0
        jmp    NEAR PTR loc_1_1BBD5
loc_1_1BBD1:
        inc    word ptr [ebp - 24h]
loc_1_1BBD5:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 30h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_1BBF0
        mov    eax, ecx
loc_1_1BBF0:
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1BC16
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 28h]
        inc    dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_1BBD1
loc_1_1BC16:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1BC36
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 38h], eax
        jmp    NEAR PTR loc_1_1BC41
loc_1_1BC36:
        mov    eax, dword ptr [ebp - 30h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 38h], eax
loc_1_1BC41:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 38h]
        jge    NEAR PTR loc_1_1BC74
        push   15h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   194h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1BD90
loc_1_1BC74:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1BCA0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_1BCB7
loc_1_1BCA0:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 30h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
loc_1_1BCB7:
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1BD74
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1BD74
        mov    eax, dword ptr [ebp - 30h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        sub    eax, ecx
        je     NEAR PTR loc_1_1BD74
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1BD74
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1BD74
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1BD74
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 30h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 14h]
        and    edx, 0ffffh
        sub    ecx, edx
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
loc_1_1BD74:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 14h], ax
        jmp    NEAR PTR loc_1_1BAAC
loc_1_1BD90:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1BD95:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1BDCE
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 6], al
        jmp    NEAR PTR loc_1_1BDD5
loc_1_1BDCE:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 6], 0
loc_1_1BDD5:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1B60D
        add    esp, 14h
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 26h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, 28h
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1BE69
        push   16h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   219h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1BF6A
loc_1_1BE69:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        add    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        add    eax, dword ptr [ecx + 2ah]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 24h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1BEC3
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 0ch], ax
loc_1_1BEC3:
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_1BED2
loc_1_1BECE:
        inc    word ptr [ebp - 20h]
loc_1_1BED2:
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1BF00
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 24h]
        inc    dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_1BECE
loc_1_1BF00:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1BF58
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1BF6A
loc_1_1BF58:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
loc_1_1BF6A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1BF6F:
        push   ebp
        mov    ebp, esp
        sub    esp, 7ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 14h]
        add    eax, 1ah
        lea    ecx, [ebp - 5eh]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_1C002
        xor    eax, eax
        mov    al, byte ptr [ebp - 56h]
        test   al, 80h
        je     NEAR PTR loc_1_1C002
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        add    eax, 0eh
        mov    dword ptr [ebp - 50h], eax
        mov    eax, dword ptr [ebp - 50h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp - 50h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        and    eax, 0ffffh
        add    eax, 0eh
        mov    word ptr [ebp - 42h], ax
        jmp    NEAR PTR loc_1_1C00F
loc_1_1C002:
        mov    dword ptr [ebp - 50h], 0
        mov    word ptr [ebp - 42h], 0eh
loc_1_1C00F:
        mov    dword ptr [ebp - 4ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 44h], ax
        mov    word ptr [ebp - 40h], 3
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 3eh], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 7]
        mov    byte ptr [ebp - 3ch], al
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 3ah], eax
        lea    edi, [ebp - 34h]
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, 6
        rep movsd
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 4]
        and    eax, 20h
        or     al, 2
        mov    byte ptr [ebp - 30h], al
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 64h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C370
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_1C0C3
        xor    eax, eax
        mov    al, byte ptr [ebp - 2eh]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [ecx + eax*8 + 5eh]
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_1_1C0CD
loc_1_1C0C3:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_1C36B
loc_1_1C0CD:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 6ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 64h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B90C
        add    esp, 18h
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1C352
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 7ch], eax
        mov    eax, dword ptr [ebp - 7ch]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 6ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 7ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 7ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 6ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 78h], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 74h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 70h], ax
loc_1_1C171:
        mov    word ptr [ebp - 68h], 0
        jmp    NEAR PTR loc_1_1C180
loc_1_1C17C:
        inc    word ptr [ebp - 68h]
loc_1_1C180:
        mov    eax, dword ptr [ebp - 70h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 78h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_1C19B
        mov    eax, ecx
loc_1_1C19B:
        mov    ecx, dword ptr [ebp - 68h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1C1C1
        mov    eax, dword ptr [ebp - 74h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 74h]
        inc    dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_1C17C
loc_1_1C1C1:
        mov    eax, dword ptr [ebp - 78h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 68h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 78h], ax
        mov    eax, dword ptr [ebp - 70h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 68h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 70h], ax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 68h]
        and    ecx, 0ffffh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_1C26B
        mov    eax, dword ptr [ebp - 78h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_1C23F
        mov    eax, dword ptr [ebp - 70h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_1C23F
        mov    eax, dword ptr [ebp - 7ch]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1C23F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1C266
loc_1_1C23F:
        push   1ch
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2eah
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_1C36B
loc_1_1C266:
        jmp    NEAR PTR loc_1_1C352
loc_1_1C26B:
        mov    eax, dword ptr [ebp - 78h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_1C2DC
        mov    eax, dword ptr [ebp - 7ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 7ch], eax
        cmp    dword ptr [ebp - 7ch], 0
        jne    NEAR PTR loc_1_1C2B4
        push   1ch
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2f8h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_1C36B
loc_1_1C2B4:
        mov    eax, dword ptr [ebp - 7ch]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 7ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 7ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 78h], ax
loc_1_1C2DC:
        mov    eax, dword ptr [ebp - 70h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_1C34D
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1C325
        push   1ch
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   306h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_1C36B
loc_1_1C325:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 74h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 70h], ax
loc_1_1C34D:
        jmp    NEAR PTR loc_1_1C171
loc_1_1C352:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_1C36B
loc_1_1C36B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1C370:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1C39D
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_1C3EC
loc_1_1C39D:
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_1C3BA
loc_1_1C3B2:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1C3BA:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1C3DB
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1C3D6
        jmp    NEAR PTR loc_1_1C3DB
loc_1_1C3D6:
        jmp    NEAR PTR loc_1_1C3B2
loc_1_1C3DB:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_1C3EC
        xor    eax, eax
        jmp    NEAR PTR loc_1_1C84A
loc_1_1C3EC:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx + 6], al
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_1C48B
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0ch]
        mov    byte ptr [ebp - 30h], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 20h
        mov    byte ptr [ebp - 24h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 2
        je     NEAR PTR loc_1_1C447
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_1C46E
loc_1_1C447:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1C477
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_1C477
loc_1_1C46E:
        mov    byte ptr [ebp - 10h], 1
        jmp    NEAR PTR loc_1_1C47B
loc_1_1C477:
        mov    byte ptr [ebp - 10h], 0
loc_1_1C47B:
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_1C4B9
loc_1_1C48B:
        mov    byte ptr [ebp - 30h], 0
        mov    byte ptr [ebp - 24h], 0
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        cmp    al, 3
        jne    NEAR PTR loc_1_1C4B3
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1C4B9
loc_1_1C4B3:
        mov    word ptr [ebp - 8], 24h
loc_1_1C4B9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 24h]
        test   eax, eax
        je     NEAR PTR loc_1_1C598
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_21D8
        add    esp, 1ch
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 0
        je     NEAR PTR loc_1_1C593
        cmp    dword ptr [ebp - 28h], 3
        je     NEAR PTR loc_1_1C593
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1C523
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_1C523:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_1C55F
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 4], 8
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_1C58C
loc_1_1C55F:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1C58C
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 4], 9
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
loc_1_1C58C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1C84A
loc_1_1C593:
        jmp    NEAR PTR loc_1_1C59F
loc_1_1C598:
        mov    dword ptr [ebp - 14h], 0
loc_1_1C59F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        je     NEAR PTR loc_1_1C5BC
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 26h]
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_1C5EA
loc_1_1C5BC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 26h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 24h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    dx, word ptr [edx + 22h]
        and    edx, 0ffffh
        add    ecx, edx
        sub    eax, ecx
        mov    word ptr [ebp - 4], ax
loc_1_1C5EA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 24h]
        test   eax, eax
        je     NEAR PTR loc_1_1C607
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1C746
loc_1_1C607:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_1C61C
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 34h], eax
        jmp    NEAR PTR loc_1_1C622
loc_1_1C61C:
        lea    eax, [ebp - 14h]
        mov    dword ptr [ebp - 34h], eax
loc_1_1C622:
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 1
        jg     NEAR PTR loc_1_1C646
        mov    eax, 1
loc_1_1C646:
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_249F
        add    esp, 18h
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 0
        je     NEAR PTR loc_1_1C746
        cmp    dword ptr [ebp - 28h], 3
        je     NEAR PTR loc_1_1C746
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1C697
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1C697:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_1C6B8
        push   1
        push   1
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
loc_1_1C6B8:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_1C708
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1C708
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_1C708
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 4], 8
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_1C73F
loc_1_1C708:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_1C73F
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1C73F
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 4], 9
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
loc_1_1C73F:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1C84A
loc_1_1C746:
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 20h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 0ch], ax
        mov    al, byte ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 0eh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 0fh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 12h], 0cch
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 7]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 13h], al
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 14h], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        je     NEAR PTR loc_1_1C7CA
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 8], 18h
        jmp    NEAR PTR loc_1_1C7EB
loc_1_1C7CA:
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 8], 40h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 22h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 18h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 19h], al
loc_1_1C7EB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 24h]
        test   eax, eax
        je     NEAR PTR loc_1_1C806
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 6], 0
        jmp    NEAR PTR loc_1_1C842
loc_1_1C806:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0ffffh
        add    ecx, edx
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_1C83B
        mov    eax, ecx
loc_1_1C83B:
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 6], ax
loc_1_1C842:
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_1C84A
loc_1_1C84A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1C84F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 0a0h
        jl     NEAR PTR loc_1_1C877
        mov    byte ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_1C90A
loc_1_1C877:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 8
        je     NEAR PTR loc_1_1C89F
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 0ah
        jne    NEAR PTR loc_1_1C8B2
loc_1_1C89F:
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_1C8B2
        mov    byte ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_1C90A
loc_1_1C8B2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 80h
        je     NEAR PTR loc_1_1C8DE
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        cmp    eax, 82h
        jne    NEAR PTR loc_1_1C906
loc_1_1C8DE:
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_1C906
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 26h]
        and    eax, 0ffffh
        cmp    eax, 28h
        jg     NEAR PTR loc_1_1C906
        mov    byte ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_1C90A
loc_1_1C906:
        mov    byte ptr [ebp - 8], 1
loc_1_1C90A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 2
        jne    NEAR PTR loc_1_1C92C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19506
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1C934
loc_1_1C92C:
        call   sub_1_19428
        mov    dword ptr [ebp - 4], eax
loc_1_1C934:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1C9B8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2ah]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 2
        jne    NEAR PTR loc_1_1C9AB
        mov    eax, dword ptr [ebp - 4]
        add    eax, 18h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
loc_1_1C9AB:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1C9E4
        db  0E9h, 02Ch, 000h, 000h, 000h
loc_1_1C9B8:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_1C9E4
loc_1_1C9E4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1C9E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        sub    eax, 4
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 50h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_1CA90
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 50h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_1CA97
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1CA90:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1CA97
loc_1_1CA97:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1CA9C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
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
        je     NEAR PTR loc_1_1CAE5
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
        jmp    NEAR PTR loc_1_1CCBB
loc_1_1CAE5:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 10h
        push   eax
        push   14h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_24241
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_1CB2B
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1CCBB
loc_1_1CB2B:
        lea    eax, [ebp - 14h]
        push   eax
        push   3
        mov    eax, dword ptr [ebp - 10h]
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
        je     NEAR PTR loc_1_1CB78
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
        jmp    NEAR PTR loc_1_1CCBB
loc_1_1CB78:
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
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1CBE9
        push   2
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1CCBB
loc_1_1CBE9:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        add    eax, 50h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1CC27
        push   1ah
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1CCBB
loc_1_1CC27:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1CCB4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1CC7E
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1CC7E:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1CCB4
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1CCB4:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1CCBB
loc_1_1CCBB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1CCC0:
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
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_1CD09
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
        jmp    NEAR PTR loc_1_1CDEE
loc_1_1CD09:
        lea    eax, [ebp - 18h]
        push   eax
        push   3
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
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
        je     NEAR PTR loc_1_1CD5A
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
        jmp    NEAR PTR loc_1_1CDEE
loc_1_1CD5A:
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_1CDCE
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1CDA9
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 0ch], 0
loc_1_1CDA9:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0ah
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0eh]
        push   eax
        call   sub_1_19871
        add    esp, 8
        inc    word ptr [ebp - 10h]
loc_1_1CDCE:
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_1CDEE
loc_1_1CDEE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1CDF3:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 7ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1CFB7
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_1CE45
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_1CE45:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_1CF5C
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1CE6C
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp + 10h], eax
loc_1_1CE6C:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 20h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_1CEDF
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], 0
        jne    NEAR PTR loc_1_1CEBB
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 20h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_1CEDF
loc_1_1CEBB:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        add    eax, 40h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_1CF0F
loc_1_1CEDF:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1A40B
        add    esp, 0ch
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        jmp    NEAR PTR loc_1_1CF4D
loc_1_1CF0F:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1CF37
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 38h
        push   eax
        call   sub_1_19795
        add    esp, 8
        jmp    NEAR PTR loc_1_1CF4D
loc_1_1CF37:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 38h
        push   eax
        call   sub_1_19795
        add    esp, 8
loc_1_1CF4D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 0
        jmp    NEAR PTR loc_1_1CFB2
loc_1_1CF5C:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1CF78
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 0ch], 0
        jmp    NEAR PTR loc_1_1CF82
loc_1_1CF78:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0ch], 0
loc_1_1CF82:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1F076
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
loc_1_1CFB2:
        jmp    NEAR PTR loc_1_1CFD4
loc_1_1CFB7:
        push   11h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   5eeh
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_1CFD4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1CFD9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_1D017
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1D017
        push   1
        push   1
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
loc_1_1D017:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1D040
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_1D04C
loc_1_1D040:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_1D04C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1D051:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1D080
        mov    word ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 28h], 2
        jmp    NEAR PTR loc_1_1D08D
loc_1_1D080:
        mov    word ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 28h], 14h
loc_1_1D08D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 8]
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
        cmp    byte ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1D0E7
        mov    eax, dword ptr [ebp - 8]
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
        jmp    NEAR PTR loc_1_1D176
loc_1_1D0E7:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_1D11E
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffh
        add    eax, 3
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_1D176
loc_1_1D11E:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_1D156
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffh
        sub    eax, 2
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_1D176
loc_1_1D156:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffh
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*4]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1D176:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B450
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_1D19C
        xor    eax, eax
        jmp    NEAR PTR loc_1_1D27C
loc_1_1D19C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1D1C0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1D1EB
loc_1_1D1C0:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1D1E4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1D1EB
loc_1_1D1E4:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1D27C
loc_1_1D1EB:
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 20h], ax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1D247
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 22h], ax
        jmp    NEAR PTR loc_1_1D250
loc_1_1D247:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 22h], 0eh
loc_1_1D250:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 22h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 26h], ax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1D27C
loc_1_1D27C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1D281:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 10h], 4
        jmp    NEAR PTR loc_1_1D299
loc_1_1D295:
        inc    word ptr [ebp - 10h]
loc_1_1D299:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 5
        jg     NEAR PTR loc_1_1D39D
        cmp    byte ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_1D2DA
        mov    eax, dword ptr [ebp - 10h]
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
        jmp    NEAR PTR loc_1_1D369
loc_1_1D2DA:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_1D311
        mov    eax, dword ptr [ebp - 10h]
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
        jmp    NEAR PTR loc_1_1D369
loc_1_1D311:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_1D349
        mov    eax, dword ptr [ebp - 10h]
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
        jmp    NEAR PTR loc_1_1D369
loc_1_1D349:
        mov    eax, dword ptr [ebp - 10h]
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
loc_1_1D369:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1D398
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 20h], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1D39D
loc_1_1D398:
        jmp    NEAR PTR loc_1_1D295
loc_1_1D39D:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 5
        jle    NEAR PTR loc_1_1D3B5
        xor    eax, eax
        jmp    NEAR PTR loc_1_1D4F5
loc_1_1D3B5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B450
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_1D3DB
        xor    eax, eax
        jmp    NEAR PTR loc_1_1D4F5
loc_1_1D3DB:
        mov    eax, dword ptr [ebp - 10h]
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
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1D4B0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2ah]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 6ch]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1D472
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        sub    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 6eh]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jae    NEAR PTR loc_1_1D479
loc_1_1D472:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1D4F5
loc_1_1D479:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6eh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 2ah]
        mov    eax, dword ptr [eax + ecx]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 70h]
        and    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 74h], ecx
        je     NEAR PTR loc_1_1D4B0
        xor    eax, eax
        jmp    NEAR PTR loc_1_1D4F5
loc_1_1D4B0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 22h], 0eh
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 22h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 26h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 28h], 70h
        mov    eax, 1
        jmp    NEAR PTR loc_1_1D4F5
loc_1_1D4F5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1D4FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1D558
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1ECCD
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1D5AA
loc_1_1D558:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1ECCD
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        cmp    eax, 0a0h
        jl     NEAR PTR loc_1_1D595
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_130CC
        add    esp, 8
        jmp    NEAR PTR loc_1_1D5A5
loc_1_1D595:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_13E59
        add    esp, 8
loc_1_1D5A5:
        jmp    NEAR PTR loc_1_1D836
loc_1_1D5AA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_1D60B
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
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
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_13469
        add    esp, 10h
        jmp    NEAR PTR loc_1_1D836
        db  0E9h, 02Dh, 000h, 000h, 000h
loc_1_1D60B:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0ch], 0
        jne    NEAR PTR loc_1_1D632
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_13E59
        add    esp, 8
        jmp    NEAR PTR loc_1_1D836
        db  0E9h, 006h, 000h, 000h, 000h
loc_1_1D632:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        cmp    al, 2
        jne    NEAR PTR loc_1_1D6F0
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_1D688
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_1D688
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CFD9
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1D836
loc_1_1D688:
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1D6BC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_10683
        add    esp, 0ch
loc_1_1D6BC:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CFD9
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CDF3
        add    esp, 10h
        jmp    NEAR PTR loc_1_1D836
        db  0E9h, 046h, 001h, 000h, 000h
loc_1_1D6F0:
        mov    dword ptr [ebp - 4], 0
loc_1_1D6F7:
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10E7C
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1D790
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1D78B
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1BF6F
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_1D78B
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1D773
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_10683
        add    esp, 0ch
loc_1_1D773:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CDF3
        add    esp, 10h
loc_1_1D78B:
        jmp    NEAR PTR loc_1_1D831
loc_1_1D790:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1D818
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1BF6F
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1D7DD
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_10683
        add    esp, 0ch
loc_1_1D7DD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CFD9
        add    esp, 0ch
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_1D813
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CDF3
        add    esp, 10h
loc_1_1D813:
        jmp    NEAR PTR loc_1_1D82C
loc_1_1D818:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1CFD9
        add    esp, 0ch
loc_1_1D82C:
        jmp    NEAR PTR loc_1_1D836
loc_1_1D831:
        jmp    NEAR PTR loc_1_1D6F7
loc_1_1D836:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1D83B:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0efh
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_1DA8F
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 3
        je     NEAR PTR loc_1_1D8AB
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_1DA8F
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 0afh
        je     NEAR PTR loc_1_1D8AB
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 0e3h
        jne    NEAR PTR loc_1_1DA8F
loc_1_1D8AB:
        mov    word ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_1D8BA
loc_1_1D8B6:
        inc    word ptr [ebp - 0ch]
loc_1_1D8BA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        inc    eax
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1D916
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1D911
        jmp    NEAR PTR loc_1_1D916
loc_1_1D911:
        jmp    NEAR PTR loc_1_1D8B6
loc_1_1D916:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        inc    eax
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_1DA83
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_1D9A7
loc_1_1D940:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 28h], 6
        jmp    NEAR PTR loc_1_1D9D0
loc_1_1D94C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_1D96B
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 28h], 0ch
        jmp    NEAR PTR loc_1_1D972
loc_1_1D96B:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 28h], 0eh
loc_1_1D972:
        jmp    NEAR PTR loc_1_1D9D0
loc_1_1D977:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_1D996
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 28h], 10h
        jmp    NEAR PTR loc_1_1D99D
loc_1_1D996:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 28h], 12h
loc_1_1D99D:
        jmp    NEAR PTR loc_1_1D9D0
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_1D9A7:
        cmp    dword ptr [ebp - 14h], 3
        je     NEAR PTR loc_1_1D940
        cmp    dword ptr [ebp - 14h], 0afh
        je     NEAR PTR loc_1_1D94C
        cmp    dword ptr [ebp - 14h], 0e3h
        je     NEAR PTR loc_1_1D977
        jmp    NEAR PTR loc_1_1D9D0
loc_1_1D9D0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 24h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 20h], ax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1DA40
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 22h], ax
        jmp    NEAR PTR loc_1_1DA49
loc_1_1DA40:
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 22h], 0eh
loc_1_1DA49:
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 24h], 3
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    cx, word ptr [ecx + 22h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 26h], ax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DA96
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1DA83:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DA96
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1DA8F:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DA96
loc_1_1DA96:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DA9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 2ah]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        and    eax, 0feh
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 1ch], 3
        jae    NEAR PTR loc_1_1DACE
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DACE:
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FC8E
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1DBBB
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1DFCF
        add    esp, 18h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 1
        jne    NEAR PTR loc_1_1DB22
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DB22:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1DC45
        add    esp, 18h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 1
        jne    NEAR PTR loc_1_1DB59
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DB59:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E352
        add    esp, 18h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 1
        jne    NEAR PTR loc_1_1DB90
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DB90:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1EB34
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_1DBB6
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DBB6:
        jmp    NEAR PTR loc_1_1DC39
loc_1_1DBBB:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1EB34
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_1DBE1
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DBE1:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1D83B
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_1DC0F
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DC0F:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1D051
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_1DC39
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DC39:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DC40
loc_1_1DC40:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DC45:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0efh
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 3
        je     NEAR PTR loc_1_1DCD3
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_1DCA0
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 0afh
        jne    NEAR PTR loc_1_1DCA0
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 8
        jne    NEAR PTR loc_1_1DCD3
loc_1_1DCA0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_1DFC3
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 0afh
        je     NEAR PTR loc_1_1DCD3
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 0e3h
        jne    NEAR PTR loc_1_1DFC3
loc_1_1DCD3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1DD54
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1DD54
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_1DD22
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1DD4D
loc_1_1DD22:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1DD54
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1DD54
loc_1_1DD4D:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DFCA
loc_1_1DD54:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_1DDD6
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1DDD6
        mov    dword ptr [ebp - 4], 0
loc_1_1DD80:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10E7C
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1DDCC
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1DDC7
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 29h], 3
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_1_1DDD6
loc_1_1DDC7:
        jmp    NEAR PTR loc_1_1DDD1
loc_1_1DDCC:
        jmp    NEAR PTR loc_1_1DDD6
loc_1_1DDD1:
        jmp    NEAR PTR loc_1_1DD80
loc_1_1DDD6:
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 29h]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_1DE01
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1DFAE
loc_1_1DE01:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_1DEB3
loc_1_1DE0E:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 6
        jmp    NEAR PTR loc_1_1DEDC
loc_1_1DE1A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_1DE58
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_1DE4C
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0ch
        jmp    NEAR PTR loc_1_1DE53
loc_1_1DE4C:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0eh
loc_1_1DE53:
        jmp    NEAR PTR loc_1_1DE7E
loc_1_1DE58:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_1DE77
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 8
        jmp    NEAR PTR loc_1_1DE7E
loc_1_1DE77:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0ah
loc_1_1DE7E:
        jmp    NEAR PTR loc_1_1DEDC
loc_1_1DE83:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_1DEA2
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 10h
        jmp    NEAR PTR loc_1_1DEA9
loc_1_1DEA2:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 12h
loc_1_1DEA9:
        jmp    NEAR PTR loc_1_1DEDC
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_1DEB3:
        cmp    dword ptr [ebp - 14h], 3
        je     NEAR PTR loc_1_1DE0E
        cmp    dword ptr [ebp - 14h], 0afh
        je     NEAR PTR loc_1_1DE1A
        cmp    dword ptr [ebp - 14h], 0e3h
        je     NEAR PTR loc_1_1DE83
        jmp    NEAR PTR loc_1_1DEDC
loc_1_1DEDC:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B450
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_1DF02
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DFCA
loc_1_1DF02:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax + 24h], 3
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 20h], ax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1DF74
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 22h], ax
        jmp    NEAR PTR loc_1_1DF7D
loc_1_1DF74:
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax + 22h], 0eh
loc_1_1DF7D:
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    cx, word ptr [ecx + 22h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 26h], ax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1DFCA
        db  0E9h, 010h, 000h, 000h, 000h
loc_1_1DFAE:
        mov    eax, dword ptr [ebp + 10h]
        inc    dword ptr [eax + 80h]
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DFCA
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1DFC3:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1DFCA
loc_1_1DFCA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DFCF:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 10h], al
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   al, 1
        je     NEAR PTR loc_1_1E021
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 1
        je     NEAR PTR loc_1_1E021
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 5
        je     NEAR PTR loc_1_1E021
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 9
        jne    NEAR PTR loc_1_1E346
loc_1_1E021:
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1E03C
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E03C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0feh
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0eh]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1E07C
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_1E07C
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + 0eh], al
loc_1_1E07C:
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 0eh
        push   eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FD1A
        add    esp, 10h
        mov    dword ptr [ebp - 14h], eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + 0eh], al
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_1E2A4
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1E117
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_1E0E5
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1E110
loc_1_1E0E5:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1E117
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1E117
loc_1_1E110:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E117:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   al, 1
        jne    NEAR PTR loc_1_1E220
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_1C9E9
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_1E143
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E143:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1E171
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1E17D
loc_1_1E171:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 4
        jmp    NEAR PTR loc_1_1E21B
loc_1_1E17D:
        call   sub_1_19428
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1E1D2
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], 8
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 5], 1
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_1E1F8
loc_1_1E1D2:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E1F8:
        mov    eax, dword ptr [ebp + 10h]
        inc    dword ptr [eax + 80h]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_195C4
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E21B:
        jmp    NEAR PTR loc_1_1E28E
loc_1_1E220:
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 1ch], 4
        je     NEAR PTR loc_1_1E239
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
        db  0E9h, 055h, 000h, 000h, 000h
loc_1_1E239:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 1
        jne    NEAR PTR loc_1_1E253
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0aah
        jmp    NEAR PTR loc_1_1E28E
loc_1_1E253:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 5
        jne    NEAR PTR loc_1_1E26D
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0ach
        jmp    NEAR PTR loc_1_1E28E
loc_1_1E26D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 9
        jne    NEAR PTR loc_1_1E287
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0aeh
        jmp    NEAR PTR loc_1_1E28E
loc_1_1E287:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E28E:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax + 24h], 4
        jmp    NEAR PTR loc_1_1E2AB
loc_1_1E2A4:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E2AB:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 20h], ax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1E30C
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 22h], ax
        jmp    NEAR PTR loc_1_1E315
loc_1_1E30C:
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax + 22h], 0eh
loc_1_1E315:
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    cx, word ptr [ecx + 22h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 26h], ax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1E34D
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1E346:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E34D
loc_1_1E34D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1E352:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0efh
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 1ch], 3
        jne    NEAR PTR loc_1_1E3AE
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 6fh
        je     NEAR PTR loc_1_1E3CB
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 43h
        je     NEAR PTR loc_1_1E3CB
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 63h
        je     NEAR PTR loc_1_1E3CB
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 0fh
        je     NEAR PTR loc_1_1E3CB
loc_1_1E3AE:
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 1ch], 8
        jne    NEAR PTR loc_1_1E72E
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 87h
        jne    NEAR PTR loc_1_1E72E
loc_1_1E3CB:
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_1E3E6
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E735
loc_1_1E3E6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 6fh
        jne    NEAR PTR loc_1_1E489
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1E475
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1E475
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_1E443
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1E46E
loc_1_1E443:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1E475
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1E475
loc_1_1E46E:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E735
loc_1_1E475:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0a0h
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_1E681
loc_1_1E489:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0feh
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0eh]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1E4C9
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_1E4C9
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + 0eh], al
loc_1_1E4C9:
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 0eh
        push   eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FD1A
        add    esp, 10h
        mov    dword ptr [ebp - 10h], eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + 0eh], al
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_1E672
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_1E57B
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1E57B
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_1E549
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1E574
loc_1_1E549:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 52h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1E57B
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 80d5h
        jne    NEAR PTR loc_1_1E57B
loc_1_1E574:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E735
loc_1_1E57B:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_1E5BD
loc_1_1E588:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0a2h
        jmp    NEAR PTR loc_1_1E66D
loc_1_1E594:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0a4h
        jmp    NEAR PTR loc_1_1E66D
loc_1_1E5A0:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0a6h
        jmp    NEAR PTR loc_1_1E66D
loc_1_1E5AC:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax + 28h], 0a8h
        jmp    NEAR PTR loc_1_1E66D
        db  0E9h, 0B0h, 000h, 000h, 000h
loc_1_1E5BD:
        sub    dword ptr [ebp - 14h], 0fh
        cmp    dword ptr [ebp - 14h], 78h
        ja     NEAR PTR loc_1_1E66D
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 001h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 002h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 003h
loc_1_1E66D:
        jmp    NEAR PTR loc_1_1E679
loc_1_1E672:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E735
loc_1_1E679:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_1_1E681:
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax + 24h], 3
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 20h], ax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1E6F4
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 22h], ax
        jmp    NEAR PTR loc_1_1E6FD
loc_1_1E6F4:
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax + 22h], 0eh
loc_1_1E6FD:
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    cx, word ptr [ecx + 22h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 26h], ax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1E735
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1E72E:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1E735
loc_1_1E735:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1E73A:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_1E7BB
loc_1_1E756:
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1DA9B
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_1E7DE
loc_1_1E776:
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1D051
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_1E7DE
loc_1_1E796:
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1D281
        add    esp, 10h
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_1E7DE
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_1E7BB:
        cmp    dword ptr [ebp - 28h], 1
        je     NEAR PTR loc_1_1E776
        cmp    dword ptr [ebp - 28h], 2
        je     NEAR PTR loc_1_1E756
        cmp    dword ptr [ebp - 28h], 3
        je     NEAR PTR loc_1_1E796
        jmp    NEAR PTR loc_1_1E7DE
loc_1_1E7DE:
        cmp    dword ptr [ebp - 20h], 1
        je     NEAR PTR loc_1_1E7F2
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_1EB2F
loc_1_1E7F2:
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C84F
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1E821
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_1EB2F
loc_1_1E821:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1E9DB
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C370
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1E88F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1E900
loc_1_1E88F:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 8
        je     NEAR PTR loc_1_1E8F6
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 9
        je     NEAR PTR loc_1_1E8F6
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1E8E8
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_1E8F6
loc_1_1E8E8:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_1E8F6:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_1EB2F
loc_1_1E900:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B90C
        add    esp, 18h
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1E9CD
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1F10B
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1E9BB
        push   1
        push   1
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1E9A3
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_1E9B1
loc_1_1E9A3:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_1E9B1:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_1EB2F
loc_1_1E9BB:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 38h], eax
        jmp    NEAR PTR loc_1_1E9D6
loc_1_1E9CD:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0
loc_1_1E9D6:
        jmp    NEAR PTR loc_1_1EB28
loc_1_1E9DB:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1BD95
        add    esp, 14h
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_1EB1F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DCCE
        add    esp, 4
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1EA86
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1EA6E
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_1EA7C
loc_1_1EA6E:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_1EA7C:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_1EB2F
loc_1_1EA86:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_1EB0D
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_1EAF5
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_1EB03
loc_1_1EAF5:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_1EB03:
        mov    eax, 10003h
        jmp    NEAR PTR loc_1_1EB2F
loc_1_1EB0D:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 38h], eax
        jmp    NEAR PTR loc_1_1EB28
loc_1_1EB1F:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0
loc_1_1EB28:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1EB2F
loc_1_1EB2F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1EB34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ah]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0efh
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_1ECC1
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0afh
        je     NEAR PTR loc_1_1EB87
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0e3h
        jne    NEAR PTR loc_1_1ECC1
loc_1_1EB87:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0afh
        jne    NEAR PTR loc_1_1EBEC
        mov    word ptr [ebp - 4], 6
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_1EBC1
        xor    eax, eax
        jmp    NEAR PTR loc_1_1ECC8
loc_1_1EBC1:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_1EBE0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 28h], 8
        jmp    NEAR PTR loc_1_1EBE7
loc_1_1EBE0:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 28h], 0ah
loc_1_1EBE7:
        jmp    NEAR PTR loc_1_1EC12
loc_1_1EBEC:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_1EC0B
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 28h], 80h
        jmp    NEAR PTR loc_1_1EC12
loc_1_1EC0B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 28h], 82h
loc_1_1EC12:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_1EC7E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 14h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 22h], ax
        jmp    NEAR PTR loc_1_1EC87
loc_1_1EC7E:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 22h], 0eh
loc_1_1EC87:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 24h], 3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 22h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 26h], ax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1ECC8
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_1ECC1:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1ECC8
loc_1_1ECC8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1ECCD:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        jne    NEAR PTR loc_1_1ECEC
        jmp    NEAR PTR loc_1_1F071
loc_1_1ECEC:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1ED2B
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_1ED2B:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_1ED75
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1ED75
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_1ED75:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1F032
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [ecx + eax*8 + 5eh]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    word ptr [ebp - 14h], 1
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_1EDE4
loc_1_1EDC1:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        inc    word ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 0ch], ax
loc_1_1EDE4:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_1F02D
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_1F02D
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    eax, 40h
        jne    NEAR PTR loc_1_1EEEA
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1EE4F
        push   21h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0ddch
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1F071
loc_1_1EE4F:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1EE9C
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 1ah
        push   eax
        push   10h
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 18h
loc_1_1EE9C:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1EEE5
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 3ah
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 19h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 19h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_1EEE5:
        jmp    NEAR PTR loc_1_1EFC3
loc_1_1EEEA:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    eax, 18h
        jne    NEAR PTR loc_1_1EF7B
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1EF32
        push   21h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0df7h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1F071
loc_1_1EF32:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1EF76
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_1EF76:
        jmp    NEAR PTR loc_1_1EFC3
loc_1_1EF7B:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1EFA1
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    eax, 0ch
        je     NEAR PTR loc_1_1EFC3
loc_1_1EFA1:
        push   21h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0e08h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1F071
loc_1_1EFC3:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 18h], ax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 2
        je     NEAR PTR loc_1_1F028
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_1F028:
        jmp    NEAR PTR loc_1_1EDC1
loc_1_1F02D:
        jmp    NEAR PTR loc_1_1F071
loc_1_1F032:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 1
        je     NEAR PTR loc_1_1F071
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_1F071:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F076:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_1F09F
loc_1_1F091:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1F09F:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_1F0FE
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*8 + 5eh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 0ah], ax
        db  0A1h, 000h, 000h, 000h, 000h
        movsx  ecx, word ptr [ebp - 4]
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_1F0F1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1F0F9
loc_1_1F0F1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
loc_1_1F0F9:
        jmp    NEAR PTR loc_1_1F091
loc_1_1F0FE:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_1F106
loc_1_1F106:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1F10B:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DCCE
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1F153
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_1F245
loc_1_1F153:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1F166
loc_1_1F15E:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_1F166:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1F227
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 8], eax
        je     NEAR PTR loc_1_1F182
        mov    word ptr [ebp + 14h], 0
loc_1_1F182:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [ecx + eax*8 + 5eh]
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    eax, ecx
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DB30
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_1F215
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1F227
loc_1_1F215:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_28F5E
        jmp    NEAR PTR loc_1_1F15E
loc_1_1F227:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_1F23D
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_D1F8
        add    esp, 4
loc_1_1F23D:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1F245
loc_1_1F245:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
_LTEXT ends
        end
