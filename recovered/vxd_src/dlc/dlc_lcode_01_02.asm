; dlc_lcode_01_02.asm -- DLC object 1 segment _LTEXT  (part 2/9)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_50CE:
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
        je     NEAR PTR loc_1_5117
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
        jmp    NEAR PTR loc_1_5292
loc_1_5117:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
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
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_51CC
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], ecx
        je     NEAR PTR loc_1_51CC
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   5ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5292
loc_1_51CC:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_5257
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 38h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        lea    eax, [ebp - 14h]
        push   eax
        push   62h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_5232
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_5232:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5292
        db  0E9h, 03Bh, 000h, 000h, 000h
loc_1_5257:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_5274
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_5274:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5292
loc_1_5292:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5297:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
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
        je     NEAR PTR loc_1_52E0
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
        jmp    NEAR PTR loc_1_53B9
loc_1_52E0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        push   20h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_5320
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_53B9
loc_1_5320:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E1B9
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_535E
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_5359
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_5359:
        jmp    NEAR PTR loc_1_539B
loc_1_535E:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   5ah
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_53B9
loc_1_539B:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_53B9
loc_1_53B9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_53BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
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
        je     NEAR PTR loc_1_5407
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
        jmp    NEAR PTR loc_1_5425
loc_1_5407:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5425
loc_1_5425:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_542A:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        push   10h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_547D
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_547D:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        push   2ah
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_54B9
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_54B9:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 14h], eax
        push   0ch
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_54F6
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_54F6:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        push   0ah
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_5533
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_5533:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_5599
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_555D
loc_1_5555:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_555D:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_5599
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], ecx
        jne    NEAR PTR loc_1_5594
        push   3
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_5594:
        jmp    NEAR PTR loc_1_5555
loc_1_5599:
        mov    word ptr [ebp - 18h], 1
        jmp    NEAR PTR loc_1_55A8
loc_1_55A4:
        inc    word ptr [ebp - 18h]
loc_1_55A8:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_567C
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_567C
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5640
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 14h], ecx
        jne    NEAR PTR loc_1_5640
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], ecx
        jne    NEAR PTR loc_1_5640
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_5640
        jmp    NEAR PTR loc_1_567C
loc_1_5640:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_5677
        push   18h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_5677:
        jmp    NEAR PTR loc_1_55A4
loc_1_567C:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 8], 3
        jle    NEAR PTR loc_1_56A7
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_56A7:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_56FF
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0c4h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DD35
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_56FF
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 11h
        push   5ah
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_56FF:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D278
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_5750
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_5750:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_189AC
        add    esp, 4
        cmp    eax, 1
        je     NEAR PTR loc_1_5783
        push   12h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_5783:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FE83
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_57B7
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0bh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 14h], al
        jmp    NEAR PTR loc_1_57EE
loc_1_57B7:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_57D0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
loc_1_57D0:
        push   61h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_57EE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_5895
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_5870
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_1_5870
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5864
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ah], 3
        jmp    NEAR PTR loc_1_586B
loc_1_5864:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ah], 2
loc_1_586B:
        jmp    NEAR PTR loc_1_5895
loc_1_5870:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        je     NEAR PTR loc_1_5889
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ah], 0
        jmp    NEAR PTR loc_1_5895
loc_1_5889:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0ah], al
loc_1_5895:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5951
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7fh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 80h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 81h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 82h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 9], al
        jmp    NEAR PTR loc_1_596B
loc_1_5951:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
loc_1_596B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 87h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0ah], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 83h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0eh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 16h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 22h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6dh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6eh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6fh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 70h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 71h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 72h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 73h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 74h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 9], al
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5A3E
loc_1_5A3E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5A43:
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
        je     NEAR PTR loc_1_5A8C
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
        jmp    NEAR PTR loc_1_5E78
loc_1_5A8C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        push   18h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_5ACC
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5E78
loc_1_5ACC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_5B9E
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_5B9E
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        cmp    eax, 5dch
        jle    NEAR PTR loc_1_5B28
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        sub    eax, 0ah
        mov    ecx, dword ptr [ebp - 10h]
        mov    cx, word ptr [ecx + 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jae    NEAR PTR loc_1_5B9E
loc_1_5B28:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5E78
loc_1_5B9E:
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
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5C18
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   63h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5E78
loc_1_5C18:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_11D37
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_5C7C
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 6ch], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 70h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 74h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 6eh], ax
loc_1_5C7C:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 38h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    word ptr [ebp - 14h], 1
        jmp    NEAR PTR loc_1_5CDA
loc_1_5CA8:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 2
        je     NEAR PTR loc_1_5CCB
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        inc    eax
        mov    word ptr [ebp - 14h], ax
        jmp    NEAR PTR loc_1_5CDA
loc_1_5CCB:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 14h], ax
loc_1_5CDA:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 5
        jg     NEAR PTR loc_1_5E29
        cmp    byte ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_5D1B
        mov    eax, dword ptr [ebp - 14h]
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
        jmp    NEAR PTR loc_1_5DAA
loc_1_5D1B:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_5D52
        mov    eax, dword ptr [ebp - 14h]
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
        jmp    NEAR PTR loc_1_5DAA
loc_1_5D52:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 2
        jle    NEAR PTR loc_1_5D8A
        mov    eax, dword ptr [ebp - 14h]
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
        jmp    NEAR PTR loc_1_5DAA
loc_1_5D8A:
        mov    eax, dword ptr [ebp - 14h]
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
loc_1_5DAA:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5DE8
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   310h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 7
        mov    eax, 5
        jmp    NEAR PTR loc_1_5E78
loc_1_5DE8:
        push   1
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_119D8
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 4], 2
        mov    ax, word ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        jmp    NEAR PTR loc_1_5CA8
loc_1_5E29:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_5E5A
        mov    eax, dword ptr [ebp - 4]
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
loc_1_5E5A:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_5E78
loc_1_5E78:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5E7D:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
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
        je     NEAR PTR loc_1_5EC6
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
        jmp    NEAR PTR loc_1_629D
loc_1_5EC6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 14h], eax
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
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        push   0ch
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_5F31
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_629D
loc_1_5F31:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_5F79
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_629D
loc_1_5F79:
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_5F88
loc_1_5F84:
        inc    word ptr [ebp - 20h]
loc_1_5F88:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_5FB9
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [eax + ecx], 0
        jmp    NEAR PTR loc_1_5F84
loc_1_5FB9:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_5FE1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], 0eh
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_60E7
loc_1_5FE1:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_6027
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], 12h
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 12h
        jl     NEAR PTR loc_1_601E
        mov    eax, 12h
loc_1_601E:
        mov    word ptr [ebp - 1ch], ax
        jmp    NEAR PTR loc_1_60E7
loc_1_6027:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_60C9
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], 20h
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 0eh
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_6091
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 12h
        jl     NEAR PTR loc_1_6088
        mov    eax, 12h
loc_1_6088:
        mov    word ptr [ebp - 1ch], ax
        jmp    NEAR PTR loc_1_60C4
loc_1_6091:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 0eh
        jle    NEAR PTR loc_1_60BE
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        sub    eax, 0eh
        mov    word ptr [ebp - 1ch], ax
        jmp    NEAR PTR loc_1_60C4
loc_1_60BE:
        mov    word ptr [ebp - 1ch], 0
loc_1_60C4:
        jmp    NEAR PTR loc_1_60E7
loc_1_60C9:
        push   13h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_629D
loc_1_60E7:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_610E
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_6170
loc_1_610E:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D946
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_6170
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_629D
loc_1_6170:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_61FB
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 78h
        mov    dword ptr [ebp - 24h], eax
        mov    word ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_6192
loc_1_618E:
        inc    word ptr [ebp - 20h]
loc_1_6192:
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 1ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_61CD
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_618E
loc_1_61CD:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 78h], 0
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 7ch], 0
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 80h], 0
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 84h], 0
loc_1_61FB:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_623B
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 8], ecx
        je     NEAR PTR loc_1_623B
        push   0
        push   0
        push   3
        push   0
        push   40h
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_103C0
        add    esp, 1ch
loc_1_623B:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_627F
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_629D
        db  0E9h, 01Eh, 000h, 000h, 000h
loc_1_627F:
        push   15h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_629D
loc_1_629D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_62A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
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
        je     NEAR PTR loc_1_62EB
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
        jmp    NEAR PTR loc_1_638A
loc_1_62EB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        push   10h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_632B
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_638A
loc_1_632B:
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
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 20h
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_638A
loc_1_638A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_638F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
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
        je     NEAR PTR loc_1_63D8
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
        jmp    NEAR PTR loc_1_6553
loc_1_63D8:
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
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        test   byte ptr [eax + 0ch], 80h
        je     NEAR PTR loc_1_6420
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        not    eax
        mov    ecx, dword ptr [ebp - 4]
        and    dword ptr [ecx + 50h], eax
        jmp    NEAR PTR loc_1_642F
loc_1_6420:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        or     dword ptr [ecx + 50h], eax
loc_1_642F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_64D0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8bh]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_645E
loc_1_6456:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_645E:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_6476
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 50h]
        or     dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_6456
loc_1_6476:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E359
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_64CB
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_6553
loc_1_64CB:
        jmp    NEAR PTR loc_1_6521
loc_1_64D0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E527
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_6521
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_6553
loc_1_6521:
        push   0
        push   4
        push   0
        push   40h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_6553
loc_1_6553:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6558:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
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
        je     NEAR PTR loc_1_65A1
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
        jmp    NEAR PTR loc_1_67A1
loc_1_65A1:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_66ED
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_65C4
loc_1_65BC:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_65C4:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_6655
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8fh], 0
        jne    NEAR PTR loc_1_6613
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_6650
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_6650
loc_1_6613:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   2
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_67A1
loc_1_6650:
        jmp    NEAR PTR loc_1_65BC
loc_1_6655:
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
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 54h], eax
        or     dword ptr [ebp - 0ch], 80h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E440
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_66E8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_67A1
loc_1_66E8:
        jmp    NEAR PTR loc_1_676F
loc_1_66ED:
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
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 54h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E527
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_676F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_67A1
loc_1_676F:
        push   0
        push   5
        push   0
        push   40h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_67A1
loc_1_67A1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_67A6:
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
        je     NEAR PTR loc_1_67EF
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
        jmp    NEAR PTR loc_1_699E
loc_1_67EF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 14h], eax
        push   6
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_682F
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_699E
loc_1_682F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_6879
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_699E
loc_1_6879:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 10h], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0fh], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0eh], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0dh], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0bh], al
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
        mov    eax, dword ptr [ebp - 0eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 54h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E527
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_696C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_699E
loc_1_696C:
        push   0
        push   5
        push   0
        push   40h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_101A9
        add    esp, 14h
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_699E
loc_1_699E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_69A3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
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
        cmp    eax, 0ch
        je     NEAR PTR loc_1_69FA
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_69FA
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
        jmp    NEAR PTR loc_1_6CDA
loc_1_69FA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        push   34h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_6A3A
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_6CDA
loc_1_6A3A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D278
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_6A8B
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_6CDA
loc_1_6A8B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_6BBF
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 77h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 78h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 79h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7ah]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7bh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7dh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7eh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7fh]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 80h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 81h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 82h]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0bh], al
        jmp    NEAR PTR loc_1_6BF2
loc_1_6BBF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 79h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 77h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 6], ax
loc_1_6BF2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 87h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 83h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 69h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 15h], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 16h], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6bh]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 17h], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 18h], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 19h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_6C89
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 32h], 40h
        jmp    NEAR PTR loc_1_6CBA
loc_1_6C89:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_6CA4
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 32h], 100h
        jmp    NEAR PTR loc_1_6CBA
loc_1_6CA4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 2
        jne    NEAR PTR loc_1_6CBA
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 32h], 200h
loc_1_6CBA:
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
        jmp    NEAR PTR loc_1_6CDA
loc_1_6CDA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6CDF:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
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
        je     NEAR PTR loc_1_6D28
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
        jmp    NEAR PTR loc_1_6E15
loc_1_6D28:
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
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 34h
        push   eax
        call   sub_1_19A0D
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_6D96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 48h
        push   eax
        call   sub_1_19A0D
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_6DF7
loc_1_6D96:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_6DBC
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 0ch], 0
loc_1_6DBC:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0ah
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_6E15
        db  0E9h, 01Eh, 000h, 000h, 000h
loc_1_6DF7:
        push   11h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_6E15
loc_1_6E15:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6E1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
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
        je     NEAR PTR loc_1_6E63
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
        jmp    NEAR PTR loc_1_7025
loc_1_6E63:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 34h]
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_6E96
loc_1_6E90:
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 1ch], eax
loc_1_6E96:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_6F2F
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 20h]
        cmp    dword ptr [eax + 8], ecx
        jne    NEAR PTR loc_1_6F1C
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 34h
        push   eax
        call   sub_1_198DA
        add    esp, 0ch
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_6EF2
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 0
loc_1_6EF2:
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0ah
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 12h]
        push   eax
        call   sub_1_19871
        add    esp, 8
        inc    word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_6F2A
loc_1_6F1C:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 24h], eax
loc_1_6F2A:
        jmp    NEAR PTR loc_1_6E90
loc_1_6F2F:
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
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_6F6C
loc_1_6F66:
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 1ch], eax
loc_1_6F6C:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_7005
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 20h]
        cmp    dword ptr [eax + 8], ecx
        jne    NEAR PTR loc_1_6FF2
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 48h
        push   eax
        call   sub_1_198DA
        add    esp, 0ch
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_6FC8
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_00EB
        add    esp, 4
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 0
loc_1_6FC8:
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0ah
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 12h]
        push   eax
        call   sub_1_19871
        add    esp, 8
        inc    word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_7000
loc_1_6FF2:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 24h], eax
loc_1_7000:
        jmp    NEAR PTR loc_1_6F66
loc_1_7005:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_7025
loc_1_7025:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_702A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
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
        je     NEAR PTR loc_1_7073
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
        jmp    NEAR PTR loc_1_7116
loc_1_7073:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        cmp    eax, 3333h
        jle    NEAR PTR loc_1_70AB
        push   11h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_7116
loc_1_70AB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_70FC
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_7116
loc_1_70FC:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 34h
        push   eax
        call   sub_1_19871
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_7116
loc_1_7116:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_711C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 10h]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_718C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_71B5
loc_1_718C:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        test   eax, eax
        je     NEAR PTR loc_1_71A6
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
        jmp    NEAR PTR loc_1_71AE
loc_1_71A6:
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx], al
loc_1_71AE:
        xor    eax, eax
        jmp    NEAR PTR loc_1_71B5
loc_1_71B5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_71BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_71D5
loc_1_71D5:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_724C
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_724C
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_7247
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_119D8
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        dec    byte ptr [eax + 63h]
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 6ch]
        inc    byte ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 10h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_7247
        jmp    NEAR PTR loc_1_724C
loc_1_7247:
        jmp    NEAR PTR loc_1_71D5
loc_1_724C:
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7254
loc_1_7254:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7259:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
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
        je     NEAR PTR loc_1_72A2
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
        jmp    NEAR PTR loc_1_746F
loc_1_72A2:
        lea    eax, [ebp - 18h]
        push   eax
        push   1
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
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_72F3
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
        jmp    NEAR PTR loc_1_746F
loc_1_72F3:
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_7309
loc_1_7301:
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 20h], eax
loc_1_7309:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_7382
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_737D
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    word ptr [ecx + 0eh], ax
        push   47h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_746F
loc_1_737D:
        jmp    NEAR PTR loc_1_7301
loc_1_7382:
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10E7C
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_73EA
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   48h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_746F
loc_1_73EA:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 8]
        add    eax, 38h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        lea    eax, [ebp - 14h]
        push   eax
        push   0ah
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_744F
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_744F:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_746F
loc_1_746F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7474:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
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
        je     NEAR PTR loc_1_74BD
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
        jmp    NEAR PTR loc_1_752E
loc_1_74BD:
        lea    eax, [ebp - 8]
        push   eax
        push   2
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
        je     NEAR PTR loc_1_750E
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
        jmp    NEAR PTR loc_1_752E
loc_1_750E:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14110
        add    esp, 10h
        xor    eax, eax
        jmp    NEAR PTR loc_1_752E
loc_1_752E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7533:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
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
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_1_758B
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        cmp    eax, 4bh
        jne    NEAR PTR loc_1_75DF
loc_1_758B:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        push   0ah
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_75DA
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_75DA:
        jmp    NEAR PTR loc_1_7614
loc_1_75DF:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_7614:
        mov    al, byte ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7631:
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
        je     NEAR PTR loc_1_767A
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
        jmp    NEAR PTR loc_1_78C0
loc_1_767A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        push   8
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_76BA
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_78C0
loc_1_76BA:
        lea    eax, [ebp - 18h]
        push   eax
        push   2
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
        je     NEAR PTR loc_1_7707
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
        jmp    NEAR PTR loc_1_78C0
loc_1_7707:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 8fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_78A0
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_78A0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_78A0
        push   12h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_7771
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_78C0
loc_1_7771:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        shl    ecx, 8
        or     eax, ecx
        and    eax, 1f00h
        shr    eax, 8
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   al, 0e0h
        jne    NEAR PTR loc_1_77DD
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 12h
        jg     NEAR PTR loc_1_77DD
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        je     NEAR PTR loc_1_7826
loc_1_77DD:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   44h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_78C0
loc_1_7826:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_78A0
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 97h], ax
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_7855
loc_1_7851:
        inc    word ptr [ebp - 14h]
loc_1_7855:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        sub    eax, 2
        cdq
        sub    eax, edx
        sar    eax, 1
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_78A0
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + eax*2 + 2]
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 18h]
        mov    word ptr [edx + ecx*2 + 99h], ax
        jmp    NEAR PTR loc_1_7851
loc_1_78A0:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_142BC
        add    esp, 10h
        xor    eax, eax
        jmp    NEAR PTR loc_1_78C0
loc_1_78C0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_78C5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        jne    NEAR PTR loc_1_790D
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_7908
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_7908:
        jmp    NEAR PTR loc_1_793D
loc_1_790D:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_793D
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_793D:
        mov    al, byte ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7958:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
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
        je     NEAR PTR loc_1_79A1
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
        jmp    NEAR PTR loc_1_7AE0
loc_1_79A1:
        lea    eax, [ebp - 10h]
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
        je     NEAR PTR loc_1_79F2
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
        jmp    NEAR PTR loc_1_7AE0
loc_1_79F2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0c0h
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        cmp    byte ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_7A2C
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_146A5
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7A7F
loc_1_7A2C:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_7A48
loc_1_7A40:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_7A48:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_7A7F
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_7A7A
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_146A5
        add    esp, 0ch
loc_1_7A7A:
        jmp    NEAR PTR loc_1_7A40
loc_1_7A7F:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_7AC2
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_7AC2:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_7AE0
loc_1_7AE0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7AE5:
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
        je     NEAR PTR loc_1_7B2E
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
        jmp    NEAR PTR loc_1_8255
loc_1_7B2E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
        push   18h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_7B6E
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7B6E:
        lea    eax, [ebp - 14h]
        push   eax
        push   3
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 2]
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
        je     NEAR PTR loc_1_7BBC
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
        jmp    NEAR PTR loc_1_8255
loc_1_7BBC:
        mov    eax, dword ptr [ebp - 14h]
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
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 0ah
        jle    NEAR PTR loc_1_7C45
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        push   0ah
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7C45:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jle    NEAR PTR loc_1_7CAE
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        push   0ffh
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7CAE:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        cmp    eax, 0ah
        jle    NEAR PTR loc_1_7D12
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        push   0ah
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7D12:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        cmp    eax, 7fh
        jle    NEAR PTR loc_1_7D76
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        push   7fh
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7D76:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        cmp    eax, 7fh
        jle    NEAR PTR loc_1_7DDA
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        push   7fh
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7DDA:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jle    NEAR PTR loc_1_7E43
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        push   0ffh
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7E43:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jle    NEAR PTR loc_1_7EAC
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        push   0ffh
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7EAC:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0e0h
        cmp    al, 60h
        jbe    NEAR PTR loc_1_7F15
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_7F15
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   8
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7F15:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 13h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_80BA
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 13h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 6eh]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_7F84
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7F84:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 13h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_7FCA
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_7FCA:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 14h], eax
        jne    NEAR PTR loc_1_8070
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 5
        je     NEAR PTR loc_1_8070
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 13h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11220
        add    esp, 14h
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_8046
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_8041
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_8041:
        jmp    NEAR PTR loc_1_806B
loc_1_8046:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_806B:
        jmp    NEAR PTR loc_1_80BA
loc_1_8070:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   45h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_80BA:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_80DE
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 48h], al
        jmp    NEAR PTR loc_1_80DE
loc_1_80DE:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8102
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 49h], al
        jmp    NEAR PTR loc_1_8102
loc_1_8102:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8126
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4ah], al
        jmp    NEAR PTR loc_1_8126
loc_1_8126:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8151
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        add    eax, eax
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4bh], al
        jmp    NEAR PTR loc_1_8151
loc_1_8151:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8175
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4ch], al
        jmp    NEAR PTR loc_1_8175
loc_1_8175:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8199
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 9]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4dh], al
        jmp    NEAR PTR loc_1_8199
loc_1_8199:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_81BD
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4eh], al
        jmp    NEAR PTR loc_1_81BD
loc_1_81BD:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0e0h
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 4fh], al
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        test   ah, ah
        je     NEAR PTR loc_1_820D
        mov    eax, dword ptr [ebp - 18h]
        cmp    byte ptr [eax + 2], 0
        je     NEAR PTR loc_1_820D
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 50h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14868
        add    esp, 8
loc_1_820D:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_8237
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_8237:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8255
loc_1_8255:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_825A:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
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
        je     NEAR PTR loc_1_82AF
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
        jmp    NEAR PTR loc_1_8EF5
loc_1_82AF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 20h], eax
        push   28h
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_82EF
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_82EF:
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
        mov    word ptr [ebp - 24h], 1
        jmp    NEAR PTR loc_1_8320
loc_1_831C:
        inc    word ptr [ebp - 24h]
loc_1_8320:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_841D
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_8418
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_11D37
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_83F4
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_83F4
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 88h], 3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 76h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_83E8
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 52h], 3
        jmp    NEAR PTR loc_1_83EF
loc_1_83E8:
        mov    eax, dword ptr [ebp - 1ch]
        mov    byte ptr [eax + 52h], 2
loc_1_83EF:
        jmp    NEAR PTR loc_1_840D
loc_1_83F4:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 88h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 52h], al
loc_1_840D:
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 69h]
        jmp    NEAR PTR loc_1_841D
loc_1_8418:
        jmp    NEAR PTR loc_1_831C
loc_1_841D:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 68h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_8476
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   46h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8476:
        db  068h, 000h, 000h, 000h, 000h
        push   5
        push   0ah
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 48h
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 4]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_84D1
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_84D1:
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0ffh
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 49h
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 5]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_852F
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_852F:
        db  068h, 000h, 000h, 000h, 000h
        push   0ah
        push   0ah
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4ah
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 6]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_858A
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_858A:
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   7fh
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4bh
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 7]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_85E5
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_85E5:
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   7fh
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4ch
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 8]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_8640
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8640:
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0ffh
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4dh
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 9]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_869E
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_869E:
        db  068h, 000h, 000h, 000h, 000h
        push   8
        push   0ffh
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0ah]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_86FC
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_86FC:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4bh]
        and    eax, 0ffh
        add    eax, eax
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 4bh], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 49h]
        and    eax, 0ffh
        cmp    eax, 0ah
        jg     NEAR PTR loc_1_8795
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 48h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 1ch]
        mov    cl, byte ptr [ecx + 49h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jg     NEAR PTR loc_1_8795
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8795:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_885B
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 6eh]
        and    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_881F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
        db  0E9h, 03Ch, 000h, 000h, 000h
loc_1_881F:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8849
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 60h], ax
        jmp    NEAR PTR loc_1_885B
loc_1_8849:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6eh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 60h], ax
loc_1_885B:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_89B6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 30h], eax
        jmp    NEAR PTR loc_1_88C1
loc_1_887C:
        mov    word ptr [ebp - 4], 0eh
        jmp    NEAR PTR loc_1_88E4
loc_1_8887:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_88A6
        mov    word ptr [ebp - 4], 0eh
        jmp    NEAR PTR loc_1_88AC
loc_1_88A6:
        mov    word ptr [ebp - 4], 11h
loc_1_88AC:
        jmp    NEAR PTR loc_1_88E4
loc_1_88B1:
        mov    word ptr [ebp - 4], 0dh
        jmp    NEAR PTR loc_1_88E4
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_88C1:
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_1_8887
        cmp    dword ptr [ebp - 30h], 1
        je     NEAR PTR loc_1_887C
        cmp    dword ptr [ebp - 30h], 2
        je     NEAR PTR loc_1_88B1
        jmp    NEAR PTR loc_1_88E4
loc_1_88E4:
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_8986
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    ecx, 4
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 20h]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jae    NEAR PTR loc_1_8986
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        sub    eax, 4
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 50h], ax
        jmp    NEAR PTR loc_1_89B6
loc_1_8986:
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_89AD
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 50h], ax
        jmp    NEAR PTR loc_1_89B6
loc_1_89AD:
        mov    eax, dword ptr [ebp - 1ch]
        mov    word ptr [eax + 50h], 258h
loc_1_89B6:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0feh
        mov    byte ptr [ebp - 2ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 2ch]
        test   eax, eax
        je     NEAR PTR loc_1_89E9
        mov    al, byte ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_FC8E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_8A4D
loc_1_89E9:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   43h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   43h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
        db  0E9h, 009h, 000h, 000h, 000h
loc_1_8A4D:
        mov    al, byte ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0fh]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 62h], al
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0e0h
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 4fh], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        cmp    eax, 60h
        jle    NEAR PTR loc_1_8AF3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_8AF3
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   6
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   8
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8AF3:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 7
        je     NEAR PTR loc_1_8B52
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 4
        jne    NEAR PTR loc_1_8B2C
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_8B52
loc_1_8B2C:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_8BB1
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        test   al, 4
        jne    NEAR PTR loc_1_8BB1
loc_1_8B52:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 62h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   6
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   6
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8BB1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ecx + 10h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_8C2F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 63h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8C2F:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A93F
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 63h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ecx + 10h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_8CDC
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 63h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ch]
        mov    ecx, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 24h], al
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8CDC:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        test   al, 5
        je     NEAR PTR loc_1_8E67
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 13h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8E67
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_8D82
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 13h]
        and    eax, 0ffh
        cmp    eax, 8
        jle    NEAR PTR loc_1_8D82
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 13h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8D82:
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_8DD9
        lea    eax, [ebp - 18h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8DD9:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 13h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    byte ptr [ecx + 64h], al
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 64h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10A1F
        add    esp, 14h
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 0
        jne    NEAR PTR loc_1_8E42
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_8E3D
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_8E3D:
        jmp    NEAR PTR loc_1_8E67
loc_1_8E42:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_8E67:
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 68h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 66h], ax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 20h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 38h
        push   eax
        call   sub_1_195C4
        add    esp, 8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_8ED7
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_8ED7:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_8EF5
loc_1_8EF5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8EFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
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
        je     NEAR PTR loc_1_8F4F
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
        jmp    NEAR PTR loc_1_99A3
loc_1_8F4F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
        push   14h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_8F8F
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_8F8F:
        lea    eax, [ebp - 20h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 18h]
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
        je     NEAR PTR loc_1_8FDC
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
        jmp    NEAR PTR loc_1_99A3
loc_1_8FDC:
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 1ch], eax
        push   6
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_9019
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_9019:
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 30h], eax
        mov    dword ptr [ebp - 2ch], 0
        jmp    NEAR PTR loc_1_9036
loc_1_902E:
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 30h], eax
loc_1_9036:
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_1_912E
        mov    eax, dword ptr [ebp - 30h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_9069
        cmp    dword ptr [ebp - 2ch], 0
        jne    NEAR PTR loc_1_9064
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [ebp - 2ch], eax
loc_1_9064:
        jmp    NEAR PTR loc_1_9129
loc_1_9069:
        mov    eax, dword ptr [ebp - 30h]
        mov    al, byte ptr [eax + 90h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 18h]
        mov    cl, byte ptr [ecx + 0bh]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_9129
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 2]
        cmp    dword ptr [eax + 93h], ecx
        jne    NEAR PTR loc_1_90CF
        mov    eax, dword ptr [ebp - 30h]
        mov    ax, word ptr [eax + 91h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 1ch]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_90CF
        mov    dword ptr [ebp - 34h], 1
        jmp    NEAR PTR loc_1_90D6
loc_1_90CF:
        mov    dword ptr [ebp - 34h], 0
loc_1_90D6:
        cmp    dword ptr [ebp - 34h], 0
        je     NEAR PTR loc_1_9129
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   43h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_9129:
        jmp    NEAR PTR loc_1_902E
loc_1_912E:
        cmp    dword ptr [ebp - 2ch], 0
        je     NEAR PTR loc_1_915A
        mov    eax, dword ptr [ebp - 2ch]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [ebp - 2ch]
        mov    byte ptr [eax + 8fh], 1
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 6bh]
        mov    eax, dword ptr [ebp - 20h]
        inc    byte ptr [eax + 65h]
        jmp    NEAR PTR loc_1_9197
loc_1_915A:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   46h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_9197:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 48h]
        push   eax
        push   0ah
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 48h
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 4]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_91FD
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_91FD:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 49h]
        push   eax
        push   0ffh
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 49h
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 5]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_9266
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_9266:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 4ah]
        push   eax
        push   0ah
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 4ah
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 6]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_92CC
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_92CC:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 4bh]
        push   eax
        push   0feh
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 4bh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        add    eax, eax
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_933C
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_933C:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 4ch]
        push   eax
        push   7fh
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 4ch
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 8]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_93A2
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_93A2:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 4dh]
        push   eax
        push   0ffh
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 4dh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 9]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_940B
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_940B:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 4eh]
        push   eax
        push   0ffh
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        push   eax
        call   sub_1_711C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_9474
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_9474:
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 49h]
        and    eax, 0ffh
        cmp    eax, 0ah
        jg     NEAR PTR loc_1_9500
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 48h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 2ch]
        mov    cl, byte ptr [ecx + 49h]
        and    ecx, 0ffh
        cmp    eax, ecx
        jg     NEAR PTR loc_1_9500
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_9500:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_9526
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_9590
loc_1_9526:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   43h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   43h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_9590:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0bh]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 90h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 3ch], eax
        jmp    NEAR PTR loc_1_95F5
loc_1_95AD:
        mov    word ptr [ebp - 4], 0eh
        jmp    NEAR PTR loc_1_9618
loc_1_95B8:
        mov    eax, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_95DA
        mov    word ptr [ebp - 4], 0eh
        jmp    NEAR PTR loc_1_95E0
loc_1_95DA:
        mov    word ptr [ebp - 4], 11h
loc_1_95E0:
        jmp    NEAR PTR loc_1_9618
loc_1_95E5:
        mov    word ptr [ebp - 4], 0dh
        jmp    NEAR PTR loc_1_9618
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_95F5:
        cmp    dword ptr [ebp - 3ch], 0
        je     NEAR PTR loc_1_95B8
        cmp    dword ptr [ebp - 3ch], 1
        je     NEAR PTR loc_1_95AD
        cmp    dword ptr [ebp - 3ch], 2
        je     NEAR PTR loc_1_95E5
        jmp    NEAR PTR loc_1_9618
loc_1_9618:
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_96D3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    ecx, 4
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 18h]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jae    NEAR PTR loc_1_96D3
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   42h
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   42h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
        db  0E9h, 035h, 000h, 000h, 000h
loc_1_96D3:
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_96FA
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    word ptr [ecx + 50h], ax
        jmp    NEAR PTR loc_1_9708
loc_1_96FA:
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 50h]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    word ptr [ecx + 50h], ax
loc_1_9708:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0e0h
        mov    ecx, dword ptr [ebp - 2ch]
        mov    byte ptr [ecx + 4fh], al
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        cmp    eax, 60h
        jle    NEAR PTR loc_1_979F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_979F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 2ch]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   8
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   8
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_979F:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 75h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_982E
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 28h], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 27h], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 26h], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 25h], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 24h], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 23h], al
        jmp    NEAR PTR loc_1_9841
loc_1_982E:
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 2]
        mov    dword ptr [ebp - 26h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 28h], ax
loc_1_9841:
        xor    eax, eax
        mov    al, byte ptr [ebp - 28h]
        test   al, 80h
        jne    NEAR PTR loc_1_98A4
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 0
        jne    NEAR PTR loc_1_990A
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 26h]
        cmp    dword ptr [eax + 7fh], ecx
        jne    NEAR PTR loc_1_9893
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 28h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_9893
        mov    dword ptr [ebp - 38h], 1
        jmp    NEAR PTR loc_1_989A
loc_1_9893:
        mov    dword ptr [ebp - 38h], 0
loc_1_989A:
        cmp    dword ptr [ebp - 38h], 0
        je     NEAR PTR loc_1_990A
loc_1_98A4:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        push   6
        push   6
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   4fh
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        push   4fh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
        db  0E9h, 01Fh, 000h, 000h, 000h
loc_1_990A:
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [ecx + 93h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    word ptr [ecx + 91h], ax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14953
        add    esp, 8
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_9985
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        push   6
        push   6
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 18h
loc_1_9985:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_99A3
loc_1_99A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_99A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
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
        je     NEAR PTR loc_1_99F1
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
        jmp    NEAR PTR loc_1_9BB2
loc_1_99F1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        push   8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_9A31
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_9BB2
loc_1_9A31:
        lea    eax, [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 8]
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
        je     NEAR PTR loc_1_9A7E
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
        jmp    NEAR PTR loc_1_9BB2
loc_1_9A7E:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_9AC3
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_9AC3
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A93F
        add    esp, 0ch
        mov    byte ptr [ebp - 4], al
        jmp    NEAR PTR loc_1_9B03
loc_1_9AC3:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_9B03
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_9B03
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_71BA
        add    esp, 0ch
        mov    byte ptr [ebp - 4], al
loc_1_9B03:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 63h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ecx + 65h]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 63h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_9B71
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_9B94
loc_1_9B71:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_9BB2
        db  0E9h, 01Eh, 000h, 000h, 000h
loc_1_9B94:
        push   57h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_9BB2
loc_1_9BB2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9BB7:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
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
        je     NEAR PTR loc_1_9C00
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
        jmp    NEAR PTR loc_1_9DB3
loc_1_9C00:
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
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_9D11
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_9C6B
        push   2
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_9DB3
loc_1_9C6B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_9CC2
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], ecx
        jne    NEAR PTR loc_1_9CC2
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 4], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 54h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_9CAC
loc_1_9CAC:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_9CC2
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 4], 4
        jmp    NEAR PTR loc_1_9CAC
loc_1_9CC2:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_9CF3
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_9CF3:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14A84
        add    esp, 10h
        jmp    NEAR PTR loc_1_9DAC
loc_1_9D11:
        lea    eax, [ebp - 0ch]
        push   eax
        push   1
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
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_9D62
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
        jmp    NEAR PTR loc_1_9DB3
loc_1_9D62:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_9D93
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_9D93:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14CA1
        add    esp, 10h
loc_1_9DAC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_9DB3
loc_1_9DB3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9DB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_1_9E16
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        jmp    NEAR PTR loc_1_9EFD
loc_1_9E16:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_9E70
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_9E70
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_9E70
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
loc_1_9E70:
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        lea    eax, [ebp - 0ch]
        push   eax
        push   0ah
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B98
        add    esp, 10h
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_9EAF
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_9EAF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 1
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_9EE7
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_9EE7:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
loc_1_9EFD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9F02:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        test   eax, eax
        je     NEAR PTR loc_1_9F60
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        jmp    NEAR PTR loc_1_9FE5
loc_1_9F60:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 38h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        lea    eax, [ebp - 10h]
        push   eax
        push   0ah
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     NEAR PTR loc_1_9FCF
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_9FCF:
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
loc_1_9FE5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
