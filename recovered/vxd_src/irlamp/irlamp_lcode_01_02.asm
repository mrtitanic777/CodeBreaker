; irlamp_lcode_01_02.asm -- IRLAMP object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_5CD9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 12h
        call   sub_1_49C7
        test   eax, eax
        je     NEAR PTR loc_1_5DD6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5DD1
        call   sub_1_4A69
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0fh
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0dh
        jne    NEAR PTR loc_1_5D3E
        mov    dword ptr [ebp - 4], 14h
        jmp    NEAR PTR loc_1_5DC5
loc_1_5D3E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 5
        jne    NEAR PTR loc_1_5D75
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0aeh], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b3h], 1
        jmp    NEAR PTR loc_1_5DC5
loc_1_5D75:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 1
        jne    NEAR PTR loc_1_5DC5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aeh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5DC5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0aeh], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0d6h], 1
loc_1_5DC5:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_458B
        add    esp, 4
loc_1_5DD1:
        jmp    NEAR PTR loc_1_5DE0
loc_1_5DD6:
        push   8
        call   sub_1_5758
        add    esp, 4
loc_1_5DE0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5DE5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    ax, word ptr [eax + 9ch]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_5E41
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 2ah]
        cmp    eax, 1
        jne    NEAR PTR loc_1_5E41
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        inc    word ptr [eax + 9ch]
        push   12ch
        push   0dh
        call   sub_1_12A4
        add    esp, 8
        jmp    NEAR PTR loc_1_5E9D
loc_1_5E41:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    word ptr [eax + 9ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    byte ptr [eax + 0b3h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5E8E
        push   13h
        call   sub_1_458B
        add    esp, 4
        jmp    NEAR PTR loc_1_5E9D
loc_1_5E8E:
        call   sub_1_4609
        call   sub_1_4782
        call   sub_1_44A0
loc_1_5E9D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5EA2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_5ECC
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_5FC3
loc_1_5ECC:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 1
        jne    NEAR PTR loc_1_5EFC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0afh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_5EFC
        mov    byte ptr [ebp + 8], 5
loc_1_5EFC:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        and    eax, 0ffh
        or     al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 45h], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        or     al, 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 46h], al
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3eh], al
        mov    al, byte ptr [ebp + 8]
        and    eax, 3
        cmp    al, 1
        jne    NEAR PTR loc_1_5F64
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a9h]
        and    eax, 0ffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        or     al, byte ptr [ecx + 46h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 46h], al
loc_1_5F64:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0ach]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3dh], al
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_5FA1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0abh]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4fh], al
        jmp    NEAR PTR loc_1_5FA8
loc_1_5FA1:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4fh], 7
loc_1_5FA8:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_5FC3
loc_1_5FC3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_5FC8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_5FF2
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_6085
loc_1_5FF2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a6h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 45h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 46h], 73h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 3eh], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 84h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 47h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 58h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4bh], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0ach]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3dh], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0abh]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4fh], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 0ah
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 0eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_6085
loc_1_6085:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_608C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 2ah
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_60DF
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3ah]
        add    esp, 4
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_60D8
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_60DF
loc_1_60D8:
        mov    dword ptr [ebp - 4], 2bh
loc_1_60DF:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_60E7
loc_1_60E7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_60EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_61CF
        db  0A1h, 000h, 000h, 000h, 000h
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        add    eax, 523h
        mov    dword ptr [ebp - 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, 0d37h
        mov    dword ptr [ebp - 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, 5cch
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        add    eax, 0ab5h
        mov    dword ptr [ebp - 0ch], eax
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 10h]
        add    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_DD74
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_619D
        jmp    NEAR PTR loc_1_61CF
loc_1_619D:
        mov    eax, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_61CF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_61D4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_1_60EC
        call   sub_1_6594
        jmp    NEAR PTR loc_1_6201
loc_1_6201:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6206:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_623D
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_DD7C
        add    esp, 8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_623D:
        mov    eax, 1
        jmp    NEAR PTR loc_1_6247
loc_1_6247:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_624C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0caah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_626E
loc_1_626E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6273:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_628D
loc_1_628D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6292:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        dec    eax
        or     eax, dword ptr [ebp + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 18h], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_62AF
loc_1_62AF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_62B4:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 80h], 0
        jne    NEAR PTR loc_1_62D9
        mov    eax, 1
        jmp    NEAR PTR loc_1_6438
loc_1_62D9:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 6ch]
        mov    dword ptr [ebp - 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 06Ch, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 26h]
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 6ch], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_643D
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp - 8], eax
loc_1_6328:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 48h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_63A4
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 35h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 40h], ecx
        jne    NEAR PTR loc_1_6399
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 54h]
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
loc_1_6399:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_6328
loc_1_63A4:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 40h]
        mov    dword ptr [ebp - 8], eax
loc_1_63B5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        add    eax, 40h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_6431
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 35h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 40h], ecx
        jne    NEAR PTR loc_1_6426
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        dec    dword ptr [eax + 50h]
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
loc_1_6426:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_63B5
loc_1_6431:
        xor    eax, eax
        jmp    NEAR PTR loc_1_6438
loc_1_6438:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_643D:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0a8h]
        mov    byte ptr [ebp - 10h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        mov    al, byte ptr [eax + 0aah]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 4], al
        jmp    NEAR PTR loc_1_6492
loc_1_648F:
        dec    byte ptr [ebp - 10h]
loc_1_6492:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        jle    NEAR PTR loc_1_6521
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ecx + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 40h], ecx
        jne    NEAR PTR loc_1_6511
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_64DB
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 3fh], 3
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 44h], 2
loc_1_64DB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ecx + eax*4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [edx + ecx*4], eax
loc_1_6511:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        inc    eax
        and    al, 7
        mov    byte ptr [ebp - 4], al
        jmp    NEAR PTR loc_1_648F
loc_1_6521:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6526:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0bh
        jne    NEAR PTR loc_1_658C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 46h]
        and    eax, 0efh
        cmp    al, 3
        jne    NEAR PTR loc_1_658C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        mov    al, byte ptr [eax + 3fh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_658C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 7ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 80h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 70h]
        call   dword ptr [eax + 39h]
        add    esp, 0ch
loc_1_658C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_6594:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_67A2
        call   sub_1_608C
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_65CB
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_67AA
loc_1_65CB:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_65E2
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_67AA
loc_1_65E2:
        db  0A1h, 000h, 000h, 000h, 000h
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, 0d37h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1044
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_0044
        add    esp, 4
        call   sub_1_84D0
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0c3dh
        push   eax
        call   sub_1_0231
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 18h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 1ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 1ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 18h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 0ccfh], ax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_66D4
loc_1_66D1:
        inc    dword ptr [ebp - 8]
loc_1_66D4:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ccfh]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_6744
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0cd1h
        push   eax
        call   sub_1_C9A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0ce1h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        call   sub_1_1F70
        add    esp, 8
        jmp    NEAR PTR loc_1_66D1
loc_1_6744:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 0B3h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_02BD
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_6790
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_8EC4
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_A235
        add    esp, 4
        jmp    NEAR PTR loc_1_6795
loc_1_6790:
        call   sub_1_028C
loc_1_6795:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
loc_1_67A2:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_67AA
loc_1_67AA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_67AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_67FE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   2dh
        call   sub_1_8E68
        add    esp, 0ch
        call   sub_1_039A
        mov    dword ptr [ebp - 4], eax
        push   3
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
        call   sub_1_028C
loc_1_67FE:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_6806
loc_1_6806:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_680B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6831
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_6850
loc_1_6831:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 29h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2dh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2dh]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 29h], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_6850
loc_1_6850:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6855:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6881
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_68BF
loc_1_6881:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 29h
        push   eax
        call   sub_1_208E
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_68A2
        mov    eax, 21h
        jmp    NEAR PTR loc_1_68BF
loc_1_68A2:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2ch
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 29h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_68BF
loc_1_68BF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_68C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_68F0
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_6944
loc_1_68F0:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 29h
        push   eax
        call   sub_1_208E
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_6911
        mov    eax, 21h
        jmp    NEAR PTR loc_1_6944
loc_1_6911:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_692E
        mov    eax, 20h
        jmp    NEAR PTR loc_1_6944
loc_1_692E:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2ch
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_6944
loc_1_6944:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6949:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6976
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_6B4C
loc_1_6976:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_69B4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_69B4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 29h
        push   eax
        call   sub_1_208E
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_69BE
loc_1_69B4:
        mov    eax, 21h
        jmp    NEAR PTR loc_1_6B4C
loc_1_69BE:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_69D5
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 1ch], 0
        jne    NEAR PTR loc_1_69DF
loc_1_69D5:
        mov    eax, 23h
        jmp    NEAR PTR loc_1_6B4C
loc_1_69DF:
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_69F5
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_6ADC
loc_1_69F5:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_7A9C
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_6AD5
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jne    NEAR PTR loc_1_6A81
        mov    dword ptr [ebp - 8], 3
        jmp    NEAR PTR loc_1_6A2B
loc_1_6A28:
        inc    dword ptr [ebp - 8]
loc_1_6A2B:
        cmp    dword ptr [ebp - 8], 70h
        jge    NEAR PTR loc_1_6A53
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + eax*4 + 34h], 0
        jne    NEAR PTR loc_1_6A4E
        jmp    NEAR PTR loc_1_6A53
loc_1_6A4E:
        jmp    NEAR PTR loc_1_6A28
loc_1_6A53:
        cmp    dword ptr [ebp - 8], 6fh
        jg     NEAR PTR loc_1_6A7C
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + ecx*4 + 34h], eax
        mov    dword ptr [ebp - 0ch], 0
loc_1_6A7C:
        jmp    NEAR PTR loc_1_6AD0
loc_1_6A81:
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        cmp    eax, 6fh
        jg     NEAR PTR loc_1_6AD0
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + eax*4 + 34h], 0
        jne    NEAR PTR loc_1_6AD0
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + ecx*4 + 34h], eax
        mov    dword ptr [ebp - 0ch], 0
loc_1_6AD0:
        jmp    NEAR PTR loc_1_6ADC
loc_1_6AD5:
        mov    dword ptr [ebp - 0ch], 23h
loc_1_6ADC:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_6B44
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 25h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1dh], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 21h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 11h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 11h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0dh], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 15h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 19h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 19h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 15h], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1F70
        add    esp, 8
loc_1_6B44:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_6B4C
loc_1_6B4C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6B51:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6B77
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_6C3A
loc_1_6B77:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_6C2B
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_6C2B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 15h
        cmp    dword ptr [eax + 15h], ecx
        jne    NEAR PTR loc_1_6C1F
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0dh], 0
        je     NEAR PTR loc_1_6BDB
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0dh
        push   eax
        call   sub_1_1FD6
        add    esp, 4
loc_1_6BDB:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 21h], 0
        je     NEAR PTR loc_1_6C13
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 6fh
        jg     NEAR PTR loc_1_6C13
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + eax*4 + 34h], 0
loc_1_6C13:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_6C26
loc_1_6C1F:
        mov    dword ptr [ebp - 8], 22h
loc_1_6C26:
        jmp    NEAR PTR loc_1_6C32
loc_1_6C2B:
        mov    dword ptr [ebp - 8], 21h
loc_1_6C32:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_6C3A
loc_1_6C3A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6C3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6C65
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_6CD4
loc_1_6C65:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_6CBD
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_6C99
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0dh], 0
        je     NEAR PTR loc_1_6CA3
loc_1_6C99:
        mov    eax, 21h
        jmp    NEAR PTR loc_1_6CD4
loc_1_6CA3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        cmp    dword ptr [eax + 4], 0
        jne    NEAR PTR loc_1_6CBD
        mov    eax, 23h
        jmp    NEAR PTR loc_1_6CD4
loc_1_6CBD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   -1
        call   sub_1_D781
        add    esp, 0ch
        jmp    NEAR PTR loc_1_6CD4
loc_1_6CD4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6CD9:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6D05
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_6E95
loc_1_6D05:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_6D3F
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 6fh
        jg     NEAR PTR loc_1_6D3F
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        cmp    dword ptr [ecx + eax*4 + 34h], edx
        je     NEAR PTR loc_1_6D4B
loc_1_6D3F:
        mov    dword ptr [ebp - 14h], 21h
        jmp    NEAR PTR loc_1_6E8D
loc_1_6D4B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 21h]
        cmp    dword ptr [eax + 4], 0
        jne    NEAR PTR loc_1_6D67
        mov    dword ptr [ebp - 14h], 23h
        jmp    NEAR PTR loc_1_6E8D
loc_1_6D67:
        push   3ch
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_6D87
        jmp    NEAR PTR loc_1_6E8D
loc_1_6D87:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C1CE
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_6DA9
        jmp    NEAR PTR loc_1_6E8D
loc_1_6DA9:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C450
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 14h], eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C64E
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7CE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_6E4A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        mov    dword ptr [ebp - 14h], 2
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 0ah
        je     NEAR PTR loc_1_6E45
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 28h
        je     NEAR PTR loc_1_6E45
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CC32
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C3A8
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
loc_1_6E45:
        jmp    NEAR PTR loc_1_6E77
loc_1_6E4A:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CC6F
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 14h
        jne    NEAR PTR loc_1_6E77
        call   sub_1_D0E9
        test   eax, eax
        je     NEAR PTR loc_1_6E77
        mov    dword ptr [ebp - 14h], 2
loc_1_6E77:
        cmp    dword ptr [ebp - 14h], 2
        jne    NEAR PTR loc_1_6E8D
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C5FF
        add    esp, 4
loc_1_6E8D:
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_6E95
loc_1_6E95:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6E9A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6EC0
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_6F3B
loc_1_6EC0:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C26E
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_6EE1
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_6F3B
loc_1_6EE1:
        push   3ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_6F04
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_6F3B
loc_1_6F04:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        push   80h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C64E
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C552
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C402
        add    esp, 4
        jmp    NEAR PTR loc_1_6F3B
loc_1_6F3B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0ch], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_6F74
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_7072
loc_1_6F74:
        push   3ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_6F94
        jmp    NEAR PTR loc_1_706A
loc_1_6F94:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C305
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_6FB2
        jmp    NEAR PTR loc_1_706A
loc_1_6FB2:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        mov    byte ptr [ebp - 10h], al
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 1
        je     NEAR PTR loc_1_702E
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 3fh], 1
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0ch]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 9]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_8281
        add    esp, 14h
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 2
        jne    NEAR PTR loc_1_7029
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 18h]
        inc    dword ptr [eax + 34h]
loc_1_7029:
        jmp    NEAR PTR loc_1_7041
loc_1_702E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C84F
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
loc_1_7041:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_7056
        mov    dword ptr [ebp - 4], 1
loc_1_7056:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_D14E
        add    esp, 0ch
loc_1_706A:
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_7072
loc_1_7072:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7077:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_709D
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_70D0
loc_1_709D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C144
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_70BB
        mov    eax, 20h
        jmp    NEAR PTR loc_1_70D0
loc_1_70BB:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C795
        add    esp, 8
        jmp    NEAR PTR loc_1_70D0
loc_1_70D0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_70D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 1dh
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_70FC
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_7161
loc_1_70FC:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_7159
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7CE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_7159
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 40h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 0ah
        je     NEAR PTR loc_1_7159
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_7159
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0ch]
        add    esi, 20h
        mov    ecx, 6
        rep movsd
loc_1_7159:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_7161
loc_1_7161:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7166:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7186
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_7200
loc_1_7186:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_71C3
        call   sub_1_04C9
        test   eax, eax
        je     NEAR PTR loc_1_71B7
        mov    dword ptr [ebp - 4], 17h
        jmp    NEAR PTR loc_1_71BE
loc_1_71B7:
        mov    dword ptr [ebp - 4], 18h
loc_1_71BE:
        jmp    NEAR PTR loc_1_71EF
loc_1_71C3:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_71E8
        mov    dword ptr [ebp - 4], 1fh
        jmp    NEAR PTR loc_1_71EF
loc_1_71E8:
        mov    dword ptr [ebp - 4], 1eh
loc_1_71EF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7200
loc_1_7200:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7205:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7222
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_723B
loc_1_7222:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_1_0FF9
        xor    eax, eax
        jmp    NEAR PTR loc_1_723B
loc_1_723B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_725D
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_7274
loc_1_725D:
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C6BA
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7274
loc_1_7274:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7279:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7296
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_72AD
loc_1_7296:
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C6BA
        add    esp, 0ch
        jmp    NEAR PTR loc_1_72AD
loc_1_72AD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_72B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_72D8
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_7390
loc_1_72D8:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_72FF
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_7309
loc_1_72FF:
        mov    eax, 21h
        jmp    NEAR PTR loc_1_7390
loc_1_7309:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_7323
        mov    eax, 23h
        jmp    NEAR PTR loc_1_7390
loc_1_7323:
        push   190h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_7349
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_7390
loc_1_7349:
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 3fh], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 47h], 70h
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 48h], 70h
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 44h], 2
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_03CC
        add    esp, 8
        jmp    NEAR PTR loc_1_7390
loc_1_7390:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7395:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_73AF
        xor    eax, eax
        jmp    NEAR PTR loc_1_73BC
loc_1_73AF:
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 36h]
        jmp    NEAR PTR loc_1_73BC
loc_1_73BC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_73C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_73E5
        xor    eax, eax
        jmp    NEAR PTR loc_1_742C
loc_1_73E5:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_7424
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7CE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_7424
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 18h], 0
        je     NEAR PTR loc_1_7424
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        add    eax, 20h
        mov    dword ptr [ebp - 8], eax
loc_1_7424:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_742C
loc_1_742C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7431:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_744B
        xor    eax, eax
        jmp    NEAR PTR loc_1_745A
loc_1_744B:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        jmp    NEAR PTR loc_1_745A
loc_1_745A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_745F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_747F
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_74D7
loc_1_747F:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 15h
        jl     NEAR PTR loc_1_7492
        mov    eax, 15h
loc_1_7492:
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [ebp + 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c8fh], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        add    ecx, 0c90h
        call   sub_1_0FF9
        mov    al, byte ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c8eh], al
        call   sub_1_83FE
        xor    eax, eax
        jmp    NEAR PTR loc_1_74D7
loc_1_74D7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_74DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7503
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_756B
loc_1_7503:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0c41h
        mov    dword ptr [ebp - 4], eax
loc_1_7510:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx], al
        cmp    dword ptr [ebp - 8], 16h
        je     NEAR PTR loc_1_7542
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_7547
loc_1_7542:
        jmp    NEAR PTR loc_1_754F
loc_1_7547:
        inc    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_7510
loc_1_754F:
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0ca5h], al
        call   sub_1_83FE
        xor    eax, eax
        jmp    NEAR PTR loc_1_756B
loc_1_756B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7570:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_758D
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_75A2
loc_1_758D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_04E4
        add    esp, 8
        jmp    NEAR PTR loc_1_75A2
loc_1_75A2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_75A7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_75C4
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_762D
loc_1_75C4:
        cmp    dword ptr [ebp + 8], 0
        jbe    NEAR PTR loc_1_75F7
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0bb8h
        ja     NEAR PTR loc_1_75E1
        mov    eax, 0bb8h
loc_1_75E1:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0cc7h], eax
        call   sub_1_D6DB
        jmp    NEAR PTR loc_1_7626
loc_1_75F7:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0cc7h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0cabh
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
loc_1_7626:
        xor    eax, eax
        jmp    NEAR PTR loc_1_762D
loc_1_762D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7632:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_764F
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_7666
loc_1_764F:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0cc7h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_7666
loc_1_7666:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_766B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_768C
        jmp    NEAR PTR loc_1_76C5
loc_1_768C:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_76C5
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_76C5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        cmp    dword ptr [eax + 18h], 0
        je     NEAR PTR loc_1_76C5
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 10h]
        mov    ecx, dword ptr [ecx + 18h]
        mov    byte ptr [ecx + 0afh], al
loc_1_76C5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_76CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_76E7
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_772C
loc_1_76E7:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_7711
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_7711
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_771B
loc_1_7711:
        mov    eax, 20h
        jmp    NEAR PTR loc_1_772C
loc_1_771B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_62B4
        add    esp, 4
        jmp    NEAR PTR loc_1_772C
loc_1_772C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_7734:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    byte ptr [ebp - 14h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1f4h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_78D1
        mov    dword ptr [ebp - 0ch], 0
loc_1_776F:
        mov    eax, dword ptr [ebp - 0ch]
        inc    eax
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_777E
loc_1_777B:
        inc    dword ptr [ebp - 10h]
loc_1_777E:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 10h]
        jbe    NEAR PTR loc_1_77D7
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edx + ebx + 8]
        cmp    dword ptr [eax + ecx + 8], edx
        jne    NEAR PTR loc_1_77D2
        inc    byte ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx + 5], 0ffh
loc_1_77D2:
        jmp    NEAR PTR loc_1_777B
loc_1_77D7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        test   eax, eax
        jle    NEAR PTR loc_1_785E
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx + 8]
        mov    ecx, dword ptr [ebp - 4]
        lea    ecx, [ecx + ecx*4]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + edx + 0bech], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx + 5], 0ffh
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 2
        jle    NEAR PTR loc_1_7843
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + ecx + 0bf0h], 10h
        jmp    NEAR PTR loc_1_7857
loc_1_7843:
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + ecx + 0bf0h], 8
loc_1_7857:
        inc    dword ptr [ebp - 4]
        mov    byte ptr [ebp - 14h], 0
loc_1_785E:
        mov    eax, dword ptr [ebp - 0ch]
        inc    eax
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_786D
loc_1_786A:
        inc    dword ptr [ebp - 10h]
loc_1_786D:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 10h]
        jbe    NEAR PTR loc_1_78AD
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx + 5]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_78A8
        jmp    NEAR PTR loc_1_78AD
loc_1_78A8:
        jmp    NEAR PTR loc_1_786A
loc_1_78AD:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 10h]
        jne    NEAR PTR loc_1_78C6
        jmp    NEAR PTR loc_1_78D1
loc_1_78C6:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_776F
loc_1_78D1:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 2ch], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_78E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1f4h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7907
loc_1_7904:
        inc    dword ptr [ebp - 8]
loc_1_7907:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_79E1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 5]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_79DC
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        dec    eax
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_7954
loc_1_7951:
        dec    dword ptr [ebp - 0ch]
loc_1_7954:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        jae    NEAR PTR loc_1_798C
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 5]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_7987
        jmp    NEAR PTR loc_1_798C
loc_1_7987:
        jmp    NEAR PTR loc_1_7951
loc_1_798C:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_799D
        jmp    NEAR PTR loc_1_79E1
loc_1_799D:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp - 4]
        lea    edi, [edx + ebx + 4]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 5], 0ffh
loc_1_79DC:
        jmp    NEAR PTR loc_1_7904
loc_1_79E1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_79EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1f4h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7A14
loc_1_7A11:
        inc    dword ptr [ebp - 8]
loc_1_7A14:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_7A6A
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 4]
        cmp    dword ptr [eax + ecx + 8], edx
        jne    NEAR PTR loc_1_7A65
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 5], 0ffh
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 2]
        jmp    NEAR PTR loc_1_7A97
loc_1_7A65:
        jmp    NEAR PTR loc_1_7A11
loc_1_7A6A:
        cmp    dword ptr [ebp - 8], 14h
        jae    NEAR PTR loc_1_7A97
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        lea    edi, [eax + ecx + 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax]
loc_1_7A97:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7A9C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_7AC9
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_7AC9
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 18h], 0
        jne    NEAR PTR loc_1_7AD3
loc_1_7AC9:
        mov    eax, 23h
        jmp    NEAR PTR loc_1_7AFE
loc_1_7AD3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], 0
        jne    NEAR PTR loc_1_7AF7
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_1_7AF7
        mov    eax, 23h
        jmp    NEAR PTR loc_1_7AFE
loc_1_7AF7:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7AFE
loc_1_7AFE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7B03:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_7B26
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 10h], eax
loc_1_7B26:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_7B30:
        mov    eax, dword ptr [ebp - 0ch]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_7B81
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 5
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1dh]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_7B76
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 0ch]
        add    esp, 0ch
loc_1_7B76:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_7B30
loc_1_7B81:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7B86:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
loc_1_7BA6:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_1_7C17
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 1ch
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_7C0C
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_7BF0
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_7C0C
loc_1_7BF0:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_1F70
        add    esp, 8
loc_1_7C0C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_7BA6
loc_1_7C17:
        lea    eax, [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], eax
        je     NEAR PTR loc_1_7C5C
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 1ch
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 0ch]
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7C17
loc_1_7C5C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7C61:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 20h], 0
        jne    NEAR PTR loc_1_7CE1
loc_1_7C79:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 10h
        cmp    dword ptr [eax + 10h], ecx
        je     NEAR PTR loc_1_7CE1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 1ch
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_CC6F
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_7CD7
        push   1
        push   3
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7B86
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7CDC
loc_1_7CD7:
        jmp    NEAR PTR loc_1_7CE1
loc_1_7CDC:
        jmp    NEAR PTR loc_1_7C79
loc_1_7CE1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7CE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7CFE
loc_1_7CFB:
        inc    dword ptr [ebp - 8]
loc_1_7CFE:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ccfh]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_7D8C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*2 + 0d35h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_7D87
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*2 + 0cedh]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_7D87
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + eax*2 + 0cd1h]
        jmp    NEAR PTR loc_1_7D93
loc_1_7D87:
        jmp    NEAR PTR loc_1_7CFB
loc_1_7D8C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7D93
loc_1_7D93:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7D98:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7DB0
loc_1_7DAD:
        inc    dword ptr [ebp - 8]
loc_1_7DB0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ccfh]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_7E54
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*2 + 0d35h]
        and    eax, 0ffh
        cmp    eax, 14h
        jne    NEAR PTR loc_1_7E4F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*2]
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + edx*2 + 0d09h]
        cmp    dword ptr [ecx + eax*2 + 0d0dh], edx
        jne    NEAR PTR loc_1_7E4F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + eax*2 + 0cd1h]
        jmp    NEAR PTR loc_1_7E5B
loc_1_7E4F:
        jmp    NEAR PTR loc_1_7DAD
loc_1_7E54:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7E5B
loc_1_7E5B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7E60:
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
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_7F09
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_7F04
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C552
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, 6
        jb     NEAR PTR loc_1_7EB9
        mov    eax, 6
loc_1_7EB9:
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        add    dword ptr [ecx + 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0ch]
        neg    eax
        mov    ecx, dword ptr [ebp + 0ch]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 4]
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
loc_1_7F04:
        jmp    NEAR PTR loc_1_7F2D
loc_1_7F09:
        push   0
        push   6
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8281
        add    esp, 14h
loc_1_7F2D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7F32:
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
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_7FA2
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_7F78
        mov    dword ptr [ebp - 8], 9
        jmp    NEAR PTR loc_1_7F9D
loc_1_7F78:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_7F98
        mov    dword ptr [ebp - 8], 101h
        jmp    NEAR PTR loc_1_7F9D
loc_1_7F98:
        jmp    NEAR PTR loc_1_80C5
loc_1_7F9D:
        jmp    NEAR PTR loc_1_80AD
loc_1_7FA2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 7fh
        mov    byte ptr [ebp - 10h], al
        mov    dword ptr [ebp - 0ch], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 6fh
        jg     NEAR PTR loc_1_7FD8
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*4 + 34h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_7FD8:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_80A6
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 21h]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_809A
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_CFC4
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_809A
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C450
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C5A4
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, 6
        jb     NEAR PTR loc_1_804A
        mov    eax, 6
loc_1_804A:
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        add    dword ptr [ecx + 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 14h]
        neg    eax
        mov    ecx, dword ptr [ebp + 0ch]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 10h]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 8]
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_80C5
loc_1_809A:
        mov    dword ptr [ebp - 8], 1
        jmp    NEAR PTR loc_1_80AD
loc_1_80A6:
        mov    dword ptr [ebp - 8], 8
loc_1_80AD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C888
        add    esp, 10h
loc_1_80C5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_80CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_81CA
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_1_811F
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_81CA
loc_1_811F:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ah]
        mov    byte ptr [ebp - 14h], al
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        mov    dword ptr [ebp - 0ch], 6
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 6
        jae    NEAR PTR loc_1_8153
        dec    dword ptr [ebp - 0ch]
        mov    byte ptr [ebp - 10h], 0ffh
        jmp    NEAR PTR loc_1_815F
loc_1_8153:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 10h], al
loc_1_815F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        add    dword ptr [ecx + 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0ch]
        neg    eax
        mov    ecx, dword ptr [ebp + 0ch]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 0ch]
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 1
        jne    NEAR PTR loc_1_81B8
        mov    dword ptr [ebp - 4], 0
loc_1_81B8:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D14E
        add    esp, 0ch
loc_1_81CA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_81CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 7
        jae    NEAR PTR loc_1_81E9
        jmp    NEAR PTR loc_1_827C
loc_1_81E9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_827C
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_821C
        jmp    NEAR PTR loc_1_827C
loc_1_821C:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 3fh], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 47h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 48h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 49h], 83h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4ah], 0ffh
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4bh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 5
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        call   sub_1_37A8
        add    esp, 0ch
loc_1_827C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8281:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0
        jne    NEAR PTR loc_1_82B9
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp + 18h], eax
        cmp    dword ptr [ebp + 18h], 0
        jne    NEAR PTR loc_1_82B2
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_8303
loc_1_82B2:
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 3fh], 3
loc_1_82B9:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        or     al, 80h
        mov    ecx, dword ptr [ebp + 18h]
        mov    byte ptr [ecx + 47h], al
        mov    al, byte ptr [ebp + 10h]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 18h]
        mov    byte ptr [ecx + 48h], al
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 49h], 2
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    byte ptr [ecx + 4ah], al
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 44h], 4
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        call   sub_1_37A8
        add    esp, 0ch
        jmp    NEAR PTR loc_1_8303
loc_1_8303:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8308:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
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
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_831E:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_8349
        mov    eax, 3
        jmp    NEAR PTR loc_1_83F9
loc_1_8349:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_83C5
loc_1_835C:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_83DE
loc_1_8370:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_837F
loc_1_837C:
        inc    dword ptr [ebp - 0ch]
loc_1_837F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 0ch]
        jbe    NEAR PTR loc_1_83AF
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 9]
        and    eax, 0ffffh
        add    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_837C
loc_1_83AF:
        jmp    NEAR PTR loc_1_83DE
loc_1_83B4:
        mov    dword ptr [ebp - 8], 3
        jmp    NEAR PTR loc_1_83DE
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_83C5:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_835C
        cmp    dword ptr [ebp - 10h], 1
        je     NEAR PTR loc_1_8370
        jmp    NEAR PTR loc_1_83B4
loc_1_83DE:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jbe    NEAR PTR loc_1_83F1
        mov    dword ptr [ebp - 8], 0ch
loc_1_83F1:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_83F9
loc_1_83F9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_83FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0c41h
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0ca5h]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 16h
        jge    NEAR PTR loc_1_84BA
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c8eh]
        and    eax, 0ffh
        test   eax, eax
        jle    NEAR PTR loc_1_84BA
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c8fh]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + edx], al
        inc    byte ptr [ebp - 4]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c8eh]
        and    eax, 0ffh
        mov    ecx, 17h
        xor    edx, edx
        mov    dl, byte ptr [ebp - 4]
        sub    ecx, edx
        cmp    eax, ecx
        jl     NEAR PTR loc_1_8487
        mov    eax, ecx
loc_1_8487:
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        add    eax, dword ptr [ebp - 0ch]
        add    edx, 0c90h
        mov    ecx, eax
        call   sub_1_0FF9
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        add    eax, ecx
        mov    byte ptr [ebp - 4], al
loc_1_84BA:
        mov    al, byte ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c61h], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_84D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 062h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 066h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 06Eh, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 06Ah, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 072h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 076h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 07Ah, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 082h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 07Eh, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 086h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 08Ah, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_8580:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_8629
        call   sub_1_7734
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 2ch], 0
        jne    NEAR PTR loc_1_85C0
        call   sub_1_DB14
        jmp    NEAR PTR loc_1_8624
loc_1_85C0:
        call   sub_1_78E2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 2ch]
        lea    eax, [eax + eax*4 - 5]
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0bech
        mov    dword ptr [ebp - 4], eax
        push   4
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 52ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1624
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 2
        jne    NEAR PTR loc_1_861F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3ch], 2
        jmp    NEAR PTR loc_1_8624
loc_1_861F:
        call   sub_1_DB14
loc_1_8624:
        jmp    NEAR PTR loc_1_86DC
loc_1_8629:
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_8638
loc_1_8635:
        inc    dword ptr [ebp - 10h]
loc_1_8638:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 10h]
        jbe    NEAR PTR loc_1_866B
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 4
        push   eax
        call   sub_1_79EF
        add    esp, 4
        jmp    NEAR PTR loc_1_8635
loc_1_866B:
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 0ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 2ch], 0
        jbe    NEAR PTR loc_1_86C8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 2ch]
        lea    eax, [eax + eax*4 - 5]
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0bech
        mov    dword ptr [ebp - 4], eax
        push   4
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 52ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1624
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
loc_1_86C8:
        cmp    dword ptr [ebp - 0ch], 2
        je     NEAR PTR loc_1_86DC
        call   sub_1_78E2
        call   sub_1_DB14
loc_1_86DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_86E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 0ch], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D3A1
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_870F:
        mov    eax, dword ptr [ebp - 10h]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_875A
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 5
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_874F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 8]
        add    esp, 4
loc_1_874F:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_870F
loc_1_875A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_875F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 0ah
        jne    NEAR PTR loc_1_87A3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CADF
        add    esp, 8
        push   1ah
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7B03
        add    esp, 8
loc_1_87A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_87A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        call   sub_1_C9EF
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_8834
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
        push   0
        call   sub_1_227C
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CADF
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 7ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 58h]
        mov    dword ptr [ebp - 30h], eax
        mov    byte ptr [ebp - 34h], 1
        mov    byte ptr [ebp - 33h], 0
        mov    byte ptr [ebp - 31h], 0
        mov    byte ptr [ebp - 0ch], 0
        lea    eax, [ebp - 34h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_DC25
        add    esp, 4
        push   1ah
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7B03
        add    esp, 8
        jmp    NEAR PTR loc_1_8840
loc_1_8834:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_387C
        add    esp, 4
loc_1_8840:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_8845:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 7ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], 3
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_886D
        jmp    NEAR PTR loc_1_89F5
loc_1_886D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 64h]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_88D6
loc_1_8883:
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 24h]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    byte ptr [eax + 2], 0
        mov    dword ptr [ebp - 0ch], 2
        jmp    NEAR PTR loc_1_892C
loc_1_88A1:
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 28h]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    byte ptr [eax + 2], 0
        mov    dword ptr [ebp - 0ch], 2
        jmp    NEAR PTR loc_1_892C
loc_1_88BF:
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 20h]
        jmp    NEAR PTR loc_1_892C
loc_1_88CC:
        jmp    NEAR PTR loc_1_89F5
        db  0E9h, 056h, 000h, 000h, 000h
loc_1_88D6:
        sub    dword ptr [ebp - 10h], 0ah
        cmp    dword ptr [ebp - 10h], 1eh
        ja     NEAR PTR loc_1_89F5
        mov    eax, dword ptr [ebp - 10h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 001h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 002h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 003h
loc_1_892C:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CEC3
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_1_CEC3
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CA6F
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        sub    eax, 0ah
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7B86
        add    esp, 0ch
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0ah
        je     NEAR PTR loc_1_8995
        push   1dh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7B03
        add    esp, 8
loc_1_8995:
        cmp    dword ptr [ebp + 8], 1bh
        jne    NEAR PTR loc_1_89B2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [eax + 864h], 0
        call   sub_1_DC87
loc_1_89B2:
        call   sub_1_7C61
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 20h], 0
        jne    NEAR PTR loc_1_89F5
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 24h], 0
        jne    NEAR PTR loc_1_89F5
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 28h], 0
        jne    NEAR PTR loc_1_89F5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3ch], 0
        call   sub_1_D6DB
loc_1_89F5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_89FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 3fh]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_8A8B
loc_1_8A16:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 40h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 40h]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 18h]
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 40h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C7E5
        add    esp, 8
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 7ch]
        push   eax
        call   sub_1_D211
        add    esp, 8
        jmp    NEAR PTR loc_1_8AA4
loc_1_8A62:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 7ch]
        push   eax
        call   sub_1_D211
        add    esp, 8
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
        jmp    NEAR PTR loc_1_8AA4
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_8A8B:
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_8A16
        cmp    dword ptr [ebp - 4], 3
        je     NEAR PTR loc_1_8A62
        jmp    NEAR PTR loc_1_8AA4
loc_1_8AA4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_8AA9:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 7ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 4
        jae    NEAR PTR loc_1_8ACC
        jmp    NEAR PTR loc_1_8D0C
loc_1_8ACC:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_8C30
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 5
        jb     NEAR PTR loc_1_8AF8
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_8AFD
loc_1_8AF8:
        jmp    NEAR PTR loc_1_8D0C
loc_1_8AFD:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_8B71
loc_1_8B13:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7F32
        add    esp, 8
        jmp    NEAR PTR loc_1_8C2B
loc_1_8B28:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7E60
        add    esp, 8
        jmp    NEAR PTR loc_1_8C2B
loc_1_8B3D:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_80CA
        add    esp, 8
        jmp    NEAR PTR loc_1_8C2B
loc_1_8B52:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_81CF
        add    esp, 8
        jmp    NEAR PTR loc_1_8C2B
loc_1_8B67:
        jmp    NEAR PTR loc_1_8C2B
        db  0E9h, 0BAh, 000h, 000h, 000h
loc_1_8B71:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 80h
        ja     NEAR PTR loc_1_8C2B
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 002h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 003h
loc_1_8C2B:
        jmp    NEAR PTR loc_1_8CFC
loc_1_8C30:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 6fh
        jg     NEAR PTR loc_1_8CFC
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_8CD8
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        add    dword ptr [eax + 4], 4
        mov    eax, dword ptr [ebp + 10h]
        sub    dword ptr [eax], 4
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_8CA0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 10h]
        add    esp, 8
        jmp    NEAR PTR loc_1_8CCA
loc_1_8CA0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_8CCA
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 14h]
        add    esp, 8
loc_1_8CCA:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_8D0C
loc_1_8CD8:
        push   0
        push   6
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_8281
        add    esp, 14h
loc_1_8CFC:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8D3C
        add    esp, 8
loc_1_8D0C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_8D11:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 40h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 14h]
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8D3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 4
        jb     NEAR PTR loc_1_8D7F
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        cmp    eax, 70h
        jne    NEAR PTR loc_1_8D7F
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        cmp    eax, 70h
        je     NEAR PTR loc_1_8D84
loc_1_8D7F:
        jmp    NEAR PTR loc_1_8DF8
loc_1_8D84:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [eax + 4], 4
        mov    eax, dword ptr [ebp + 0ch]
        sub    dword ptr [eax], 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_8DA4:
        mov    eax, dword ptr [ebp - 10h]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8DEF
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 5
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 1dh]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_8DE4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 10h]
        add    esp, 4
loc_1_8DE4:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_8DA4
loc_1_8DEF:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
loc_1_8DF8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_8DFD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_8E47
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 7ch]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_8E47
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_D060
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_8E47
        jmp    NEAR PTR loc_1_8E63
loc_1_8E47:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7B03
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D885
        add    esp, 4
loc_1_8E63:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8E68:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_8E7B:
        mov    eax, dword ptr [ebp - 0ch]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_8EBE
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 5
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 1ch]
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_8E7B
loc_1_8EBE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_8EC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_90FD
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, 5cch
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 68h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 42h], 1f4h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 04Ah, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 31h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 35h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 35h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 31h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 39h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 3dh], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 3dh]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 39h], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_8F7A
loc_1_8F77:
        inc    dword ptr [ebp - 4]
loc_1_8F7A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_1_90CD
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0fbh
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 39h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + ecx + 0ach], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 1cch
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*8]
        lea    ecx, [edx + ecx*2]
        lea    ecx, [edx + ecx*8]
        lea    ecx, [ecx + ecx*8]
        add    ecx, edx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + edx + 0afh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [eax + ecx + 0b3h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + ecx + 183h], 7d0h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0C7h, 084h, 008h, 08Bh, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + ecx + 19fh], 4e20h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0C7h, 084h, 008h, 0A7h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_8F77
loc_1_90CD:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_680B
        add    esp, 4
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0ffffh
        call   sub_1_6949
        add    esp, 14h
loc_1_90FD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9102:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_9122
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_923E
loc_1_9122:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B930
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_9143
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_923E
loc_1_9143:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 31h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9C83
        add    esp, 4
        mov    dword ptr [ebp - 8], 2
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 62h], 0
        jne    NEAR PTR loc_1_9236
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_95F0
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_9202
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 15eh], ecx
        je     NEAR PTR loc_1_9202
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_91FD
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 15eh], 0
        jne    NEAR PTR loc_1_91FD
        mov    eax, dword ptr [ebp - 4]
        add    eax, 119h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7077
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_99F3
        add    esp, 4
loc_1_91FD:
        jmp    NEAR PTR loc_1_9220
loc_1_9202:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_9219
        call   sub_1_96A8
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_9220
loc_1_9219:
        mov    dword ptr [ebp - 8], 25h
loc_1_9220:
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_9236
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
loc_1_9236:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_923E
loc_1_923E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9243:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 18h
        jne    NEAR PTR loc_1_926E
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 41h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_926E
        call   sub_1_96A8
loc_1_926E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9273:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_92EE
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, 400h
        jb     NEAR PTR loc_1_92AE
        mov    eax, 400h
loc_1_92AE:
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0c7h], ax
        mov    eax, dword ptr [ebp - 4]
        dec    word ptr [eax + 0c7h]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 90h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_92EE
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0c2h], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_99F3
        add    esp, 4
loc_1_92EE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_92F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_9360
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 15eh]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 31h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_9353
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 15eh]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        jmp    NEAR PTR loc_1_9360
loc_1_9353:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 15eh], 0
loc_1_9360:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_9384
        push   13h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F67
        add    esp, 8
loc_1_9384:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9389:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_93D3
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0c3h], 0
        je     NEAR PTR loc_1_93D3
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0c3h]
        add    esp, 8
loc_1_93D3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_93D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 40h
        cmp    eax, dword ptr [ebp + 10h]
        jne    NEAR PTR loc_1_9449
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 90h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_9444
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0c7h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9444
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0c2h], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_99F3
        add    esp, 4
loc_1_9444:
        jmp    NEAR PTR loc_1_9457
loc_1_9449:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F67
        add    esp, 8
loc_1_9457:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_945C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 135h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        jbe    NEAR PTR loc_1_94CA
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_94CA
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 3fh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    cl, byte ptr [ecx + 2ch]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_94CA
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B4B
        add    esp, 4
        jmp    NEAR PTR loc_1_94D6
loc_1_94CA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_94D6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_94DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 135h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        jbe    NEAR PTR loc_1_95C7
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 3fh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    cl, byte ptr [ecx + 2ch]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_95C7
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_95C0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        call   dword ptr [eax + 14h]
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_9596
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0c3h], 0
        jne    NEAR PTR loc_1_9591
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_99F3
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_1_9591:
        jmp    NEAR PTR loc_1_95BB
loc_1_9596:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0c3h], 0
        je     NEAR PTR loc_1_95BB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_9E9F
        add    esp, 8
        mov    dword ptr [ebp - 4], 0
loc_1_95BB:
        jmp    NEAR PTR loc_1_95C7
loc_1_95C0:
        mov    dword ptr [ebp - 4], 0
loc_1_95C7:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_95DD
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_95DD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_95E2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_95F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_9608
loc_1_9605:
        inc    dword ptr [ebp - 4]
loc_1_9608:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 68h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 4]
        jle    NEAR PTR loc_1_969C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + ecx + 12ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_9697
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + ecx + 107h], edx
        jne    NEAR PTR loc_1_9697
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [eax + ecx + 6ah]
        jmp    NEAR PTR loc_1_96A3
loc_1_9697:
        jmp    NEAR PTR loc_1_9605
loc_1_969C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_96A3
loc_1_96A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_96A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 2
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 31h
        cmp    dword ptr [eax + 31h], ecx
        jne    NEAR PTR loc_1_96D9
        mov    eax, 1
        jmp    NEAR PTR loc_1_97FA
loc_1_96D9:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 39h
        cmp    dword ptr [eax + 39h], ecx
        je     NEAR PTR loc_1_97ED
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 39h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 91h
        mov    dword ptr [ebp - 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 31h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 14h]
        add    edi, 99h
        mov    esi, dword ptr [eax + 20h]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 99h
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 43h], 0
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 0c7h], 0
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 40h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6CD9
        add    esp, 10h
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 2
        jne    NEAR PTR loc_1_97CE
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 90h], 1
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 91h
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 0c2h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        inc    word ptr [eax + 66h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 41h], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 15eh], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_1_97E8
loc_1_97CE:
        cmp    dword ptr [ebp - 10h], 17h
        jne    NEAR PTR loc_1_97E8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 41h], 1
        mov    dword ptr [ebp - 10h], 2
loc_1_97E8:
        jmp    NEAR PTR loc_1_97F2
loc_1_97ED:
        call   sub_1_97FF
loc_1_97F2:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_97FA
loc_1_97FA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_97FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_9817
loc_1_9814:
        inc    dword ptr [ebp - 4]
loc_1_9817:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 68h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 4]
        jle    NEAR PTR loc_1_9900
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + ecx + 12ch]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_98FB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + ecx + 1c8h], 0
        jne    NEAR PTR loc_1_98FB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 183h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 6ah
        push   eax
        call   sub_1_7077
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 6ah
        push   eax
        call   sub_1_9F2B
        add    esp, 4
        jmp    NEAR PTR loc_1_9900
loc_1_98FB:
        jmp    NEAR PTR loc_1_9814
loc_1_9900:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9905:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_991D
loc_1_991A:
        inc    dword ptr [ebp - 8]
loc_1_991D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 68h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jle    NEAR PTR loc_1_99EE
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + ecx + 12ch]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_99E9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 6ah
        push   eax
        call   sub_1_9F2B
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_99E9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 6ah
        push   eax
        call   sub_1_9F2B
        add    esp, 4
        cmp    eax, 2
        je     NEAR PTR loc_1_99E9
        push   1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 6ah
        push   eax
        call   sub_1_9F67
        add    esp, 8
loc_1_99E9:
        jmp    NEAR PTR loc_1_991A
loc_1_99EE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_99F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9ACA
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_9A7C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 15eh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 162h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 155h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 10h]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B4B
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_9AC5
loc_1_9A7C:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 15eh], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 119h
        push   eax
        call   sub_1_1526
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7077
        add    esp, 8
        call   sub_1_96A8
        cmp    eax, 2
        jne    NEAR PTR loc_1_9ABD
        mov    dword ptr [ebp - 8], 1
loc_1_9ABD:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_9AC5
loc_1_9AC5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9ACA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 31h]
        mov    dword ptr [ebp - 8], eax
loc_1_9ADE:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 31h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_9B27
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 9dh]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_9B1A
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_9B2E
        db  0E9h, 008h, 000h, 000h, 000h
loc_1_9B1A:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_9ADE
loc_1_9B27:
        xor    eax, eax
        jmp    NEAR PTR loc_1_9B2E
loc_1_9B2E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9B33:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0c3h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9B4B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 155h]
        add    eax, dword ptr [ebp + 8]
        add    eax, 162h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4bh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 43h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 90h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_9C72
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0c7h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 151h]
        jb     NEAR PTR loc_1_9BEA
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 162h]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 162h], al
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 151h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 4fh], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 0
        jmp    NEAR PTR loc_1_9C3F
loc_1_9BEA:
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0c7h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 4fh], ax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0c7h]
        and    ecx, 0ffffh
        sub    eax, ecx
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 151h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0c7h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 155h], eax
loc_1_9C3F:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 90h], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 40h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7240
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 135h
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    NEAR PTR loc_1_9C7E
loc_1_9C72:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9C7E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9C83:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 5eh], 0
        jne    NEAR PTR loc_1_9CB5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 5eh], 1
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 42h
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_9CB5:
        call   sub_1_DDB0
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9CC5:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        call   sub_1_DDB0
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 31h]
        mov    dword ptr [ebp - 10h], eax
loc_1_9CE1:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 31h
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_1_9D7B
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        sub    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ecx + 28h]
        jb     NEAR PTR loc_1_9D70
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 15eh], ecx
        jne    NEAR PTR loc_1_9D49
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 15eh], 0
loc_1_9D49:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 62h], 1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   0fh
        mov    eax, dword ptr [ebp - 0ch]
        call   dword ptr [eax + 1ch]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 62h], 0
loc_1_9D70:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_9CE1
loc_1_9D7B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9D80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 41h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_9DA1
        call   sub_1_96A8
loc_1_9DA1:
        call   sub_1_9CC5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 41h]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_9DE9
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 31h
        cmp    dword ptr [eax + 31h], ecx
        jne    NEAR PTR loc_1_9DE9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 5eh], 0
        call   sub_1_9905
        jmp    NEAR PTR loc_1_9DF5
loc_1_9DE9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_9DF5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9DFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 119h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_9E31
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9E31:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_9E36:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 135h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    al, byte ptr [eax + 0b2h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_9E77
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    NEAR PTR loc_1_9E9A
loc_1_9E77:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_9E9A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9E9A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9E9F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 90h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_9F1A
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 15eh]
        mov    al, byte ptr [eax + 2ch]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 8]
        or     eax, ecx
        or     al, 40h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 162h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 43h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 90h], 1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_7240
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 135h
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    NEAR PTR loc_1_9F26
loc_1_9F1A:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9F26:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9F2B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0c3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c2h], 3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0c9h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        jmp    NEAR PTR loc_1_9F62
loc_1_9F62:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_9F67:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 119h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 135h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c2h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        dec    word ptr [eax + 66h]
        mov    eax, dword ptr [ebp + 8]
        add    eax, 91h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 39h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 15eh], 0
        je     NEAR PTR loc_1_A011
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 15eh], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 62h], 1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 1ch]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 62h], 0
loc_1_A011:
        call   sub_1_96A8
        cmp    eax, 2
        je     NEAR PTR loc_1_A03E
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 66h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_A03E
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 41h], 0
loc_1_A03E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_A044:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_A064
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_A0CB
loc_1_A064:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ACCF
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_A085
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A0CB
loc_1_A085:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 167h]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_A0B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        call   sub_1_1F70
        add    esp, 8
        jmp    NEAR PTR loc_1_A0C4
loc_1_A0B8:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB18
        add    esp, 4
loc_1_A0C4:
        xor    eax, eax
        jmp    NEAR PTR loc_1_A0CB
loc_1_A0CB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A0D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_A0ED
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_A1C0
loc_1_A0ED:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_A101
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_A10B
loc_1_A101:
        mov    eax, 1
        jmp    NEAR PTR loc_1_A1C0
loc_1_A10B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_A145
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp + 0ch]
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_A1C0
loc_1_A145:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_A169
        mov    eax, 1
        jmp    NEAR PTR loc_1_A1C0
loc_1_A169:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 167h]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_A1AD
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, 2
        jmp    NEAR PTR loc_1_A1C0
loc_1_A1AD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB95
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_A1C0
loc_1_A1C0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A1C5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_A1E2
        mov    eax, 2dh
        jmp    NEAR PTR loc_1_A230
loc_1_A1E2:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0ffh
        jl     NEAR PTR loc_1_A1F7
        mov    eax, 0ffh
loc_1_A1F7:
        mov    byte ptr [ebp + 8], al
        mov    al, byte ptr [ebp + 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 4bh], al
        mov    al, byte ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 4ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        push   eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        add    ecx, 51h
        call   sub_1_0FF9
        xor    eax, eax
        jmp    NEAR PTR loc_1_A230
loc_1_A230:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A235:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_A480
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        add    eax, 0ab5h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1044
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 169h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 02Fh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 2eh], 6
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 033h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [eax + 37h], 2
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 03Ah, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 39h], 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 3eh], 2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [eax + 3fh], 3
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 041h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 046h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 45h], 0ah
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 4ah], 3
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 4bh], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 4ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 51h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4dh], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 18h
        push   eax
        call   sub_1_A044
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 16bh
        push   eax
        call   sub_1_680B
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_A3BA
loc_1_A3B7:
        inc    dword ptr [ebp - 4]
loc_1_A3BA:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_1_A45C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + ecx + 1e0h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 6abh
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*8]
        lea    ecx, [edx + ecx*8]
        shl    ecx, 2
        sub    ecx, edx
        lea    ecx, [edx + ecx*8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + edx + 1e3h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 19ch
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 16bh
        push   eax
        call   sub_1_6855
        add    esp, 8
        jmp    NEAR PTR loc_1_A3B7
loc_1_A45C:
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 16bh
        push   eax
        push   0
        call   sub_1_6949
        add    esp, 14h
loc_1_A480:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A485:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_A8F7
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_A525
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 40h], 2
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 080h, 0E2h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0f9h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0e6h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 45h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, 40ah
        jb     NEAR PTR loc_1_A4FB
        mov    eax, 40ah
loc_1_A4FB:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10bh], eax
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 10bh]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 42h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6E9A
        add    esp, 8
        jmp    NEAR PTR loc_1_A558
loc_1_A525:
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_A558
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 3fh], 3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        jmp    NEAR PTR loc_1_A558
loc_1_A558:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A55D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_A9EC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A57D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 40h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_A5C4
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0e2h], 0
        je     NEAR PTR loc_1_A5C4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0e2h]
        add    esp, 8
loc_1_A5C4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A5C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 42h
        cmp    eax, dword ptr [ebp + 10h]
        jne    NEAR PTR loc_1_A5F3
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 41h], 0
        jmp    NEAR PTR loc_1_A5FF
loc_1_A5F3:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_A9EC
        add    esp, 4
loc_1_A5FF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A604:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A60F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 1
        jae    NEAR PTR loc_1_A626
        jmp    NEAR PTR loc_1_A7A0
loc_1_A626:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_A79B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f9h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_A674
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7077
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AE94
        add    esp, 8
        jmp    NEAR PTR loc_1_A69B
loc_1_A674:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 3fh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0efh]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_A69B
        jmp    NEAR PTR loc_1_A7A0
loc_1_A69B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0f1h]
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_A772
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f9h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_A6F8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AA92
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
        jmp    NEAR PTR loc_1_A76D
loc_1_A6F8:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f9h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_A713
        jmp    NEAR PTR loc_1_A7A0
loc_1_A713:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0fah], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0f5h]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0fah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_A761
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABBE
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0E2h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_A76D
loc_1_A761:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
loc_1_A76D:
        jmp    NEAR PTR loc_1_A79B
loc_1_A772:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f9h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_A78D
        jmp    NEAR PTR loc_1_A7A0
loc_1_A78D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_AA17
        add    esp, 8
loc_1_A79B:
        jmp    NEAR PTR loc_1_A7AC
loc_1_A7A0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_A7AC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A7B1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        jbe    NEAR PTR loc_1_A80F
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_A80F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0f5h]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0fah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_A80A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
loc_1_A80A:
        jmp    NEAR PTR loc_1_A81B
loc_1_A80F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_A81B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A820:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   17h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 150h
        push   eax
        call   sub_1_1044
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_A848:
        mov    eax, dword ptr [ebp - 4]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_A8DF
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1bh], 0
        je     NEAR PTR loc_1_A8D2
        mov    dword ptr [ebp - 0ch], 0
loc_1_A871:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1bh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        or     al, byte ptr [ecx + edx + 150h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + edx + 150h], al
        cmp    dword ptr [ebp - 0ch], 16h
        je     NEAR PTR loc_1_A8C5
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 1bh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_A8CA
loc_1_A8C5:
        jmp    NEAR PTR loc_1_A8D2
loc_1_A8CA:
        inc    dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_A871
loc_1_A8D2:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_A848
loc_1_A8DF:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 150h
        push   eax
        call   sub_1_74DC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A8F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_A90F
loc_1_A90C:
        inc    dword ptr [ebp - 4]
loc_1_A90F:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 169h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 4]
        jle    NEAR PTR loc_1_A9AC
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + ecx + 1dch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_A9A7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + ecx + 19ch]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_A9A7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [eax + ecx + 19ch]
        jmp    NEAR PTR loc_1_A9B3
loc_1_A9A7:
        jmp    NEAR PTR loc_1_A90C
loc_1_A9AC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_A9B3
loc_1_A9B3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A9B8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 92h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0e2h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 40h], 3
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A9EC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0e2h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_AA17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 41h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_AA81
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0efh]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 8]
        or     eax, ecx
        or     al, 40h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 50fh], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 4bh], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 45h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 41h], 1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 42h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_7240
        add    esp, 8
        jmp    NEAR PTR loc_1_AA8D
loc_1_AA81:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_AA8D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_AA92:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 41h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_AB07
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0efh]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 50fh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 510h], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 4bh], 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 45h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 41h], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 42h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7240
        add    esp, 8
        jmp    NEAR PTR loc_1_AB13
loc_1_AB07:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_AB13:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
